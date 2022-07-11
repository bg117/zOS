/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <limits.h>
#include <stdint.h>

#include <memory/mem.h>
#include <memory/memdefs.h>
#include <memory/mmap.h>
#include <memory/pmm.h>

#include <hw/serial.h>

#include <misc/bit_macros.h>
#include <misc/log_macros.h>
#include <misc/type_macros.h>

#define PAGE_SIZE    0x1000
#define VIRTUAL_BASE 0xC0000000
#define MAGIC_NUMBER 0xDEADC0DE

/* defined in the linker script */
extern uint8_t __start, __pmm_bitmap_start, __pmm_bitmap_start_virt, __end;

static uint64_t *g_bitmap;

// size of the bitmap in bits
static uint64_t        g_bitmap_size;
static PhysicalAddress g_base;

static uint64_t get_first_free_idx(uint8_t *bit);

void pmm_init(MemoryMapEntry *mmap, size_t mmap_length)
{
    g_bitmap = (uint64_t *)(&__pmm_bitmap_start_virt);

    g_base        = 0;
    g_bitmap_size = mmap[mmap_length - 1].base + mmap[mmap_length - 1].length - mmap[0].base;
    g_bitmap_size = ALIGN(g_bitmap_size, PAGE_SIZE) / PAGE_SIZE;

    // mark reserved areas as used
    for (size_t i = 0; i < mmap_length; i++)
    {
        // if not reserved, continue
        if (mmap[i].type != 0x02)
            continue;

        KSLOG("found reserved area (%d), marking as used\n", i);

        uint64_t base_lower = mmap[i].base;
        uint64_t base_upper;
        // get length of usable areas
        while (i < mmap_length && mmap[i].type != 0x01)
            ++i;

        if (i >= mmap_length)
        {
            --i; // if there are no usable areas then mark them as used
            base_upper
                = mmap[i].base + mmap[i].length; // and of course since it's the last entry, take up the full space
        }
        else
        {
            base_upper = mmap[i].base;
        }

        // round base_lower down and base_upper up, then get how many pages each take
        base_lower = base_lower / PAGE_SIZE;
        base_upper = ALIGN(base_upper, PAGE_SIZE) / PAGE_SIZE;

        uint64_t length = base_upper - base_lower;
        for (uint64_t j = base_lower, k = 0; j < g_bitmap_size && k < length; j++, k++)
        {
            uint64_t idx = j / 64;
            uint8_t  bit = j % 64;

            SETBITVAR(g_bitmap[idx], 1 << bit);
        }
    }

    KSLOG("marking kernel as used\n");

    // mark bitmap and kernel as used
    uint32_t reserved = (PhysicalAddress)(&__end) - (PhysicalAddress)(&__start);
    reserved          = ALIGN(reserved, PAGE_SIZE) / PAGE_SIZE;
    for (uint32_t i = (PhysicalAddress)(&__start) / PAGE_SIZE, j = 0; j < reserved; i++, j++)
    {
        uint64_t idx = i / 64;
        uint8_t  bit = i % 64;

        SETBITVAR(g_bitmap[idx], 1 << bit);
    }

    // also mark BDA as used
    SETBITVAR(g_bitmap[0], 1 << 0);
}

void *pmm_allocate_page(void)
{
    uint8_t  bit;
    uint64_t idx = get_first_free_idx(&bit);

    if (idx == UINT64_MAX)
    {
        KSLOG("error: no more free pages left\n");
        return (void *)(MAGIC_NUMBER); // impossible to be returned on normal operation; not page-aligned
    }

    // it's definitely true that setting metadata in a bitmap is faster than
    // allocation ;)
    SETBITVAR(g_bitmap[idx], 1 << bit);

    void *offset = (void *)((idx * 64 + bit) * PAGE_SIZE);
    KSLOG("returning page %p\n", offset);
    return offset;
}

void pmm_free_page(void *page)
{
    PhysicalAddress base_addr = g_base;
    for (uint32_t i = 0; i < g_bitmap_size; i++, base_addr += PAGE_SIZE)
    {
        uint32_t idx = i / 64;
        uint8_t  bit = i % 64;

        if (base_addr == (PhysicalAddress)(page))
        {
            if (TESTBIT(g_bitmap[idx], 1 << bit))
                UNSETBITVAR(g_bitmap[idx], 1 << bit);
            else
                KSLOG("warning: page %p already free\n", page);

            return;
        }
    }

    KSLOG("error: page %p cannot be found in the bitmap\n", page);
}

enum page_status pmm_get_page_status(void *page)
{
    PhysicalAddress base_addr = g_base;
    for (uint32_t i = 0; i < g_bitmap_size; i++, base_addr += PAGE_SIZE)
    {
        uint32_t idx = i / 64;
        uint8_t  bit = i % 64;

        if (base_addr == (PhysicalAddress)(page))
        {
            if (TESTBIT(g_bitmap[idx], 1 << bit))
                return PS_USED;
            else
                return PS_FREE;
        }
    }

    KSLOG("warning: cannot get status of page %p\n", page);
    return PS_UNKNOWN;
}

uint64_t pmm_get_bitmap_length(void)
{
    return ALIGN(g_bitmap_size, 64) / 64;
}

uint64_t get_first_free_idx(uint8_t *bit)
{
    *bit = 0;

    for (uint64_t i = 0, base_addr = g_base; i < g_bitmap_size; i++, base_addr += PAGE_SIZE)
    {
        uint64_t idx = i / 64;
        *bit         = i % 64;

        if (g_bitmap[idx] == 0xFFFFFFFFFFFFFFFF)
        {
            i += 63;
            continue;
        }

        if (!TESTBIT(g_bitmap[idx], 1 << *bit))
            return idx;
    }

    return UINT64_MAX; // this is impossible to reach
}
