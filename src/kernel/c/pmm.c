/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

#include <kernel/memory/memdefs.h>
#include <kernel/memory/mmap.h>
#include <kernel/memory/pmm.h>

#include <kernel/hw/serial.h>

#include <kernel/misc/bit_macros.h>
#include <kernel/misc/log_macros.h>

#include <utils/mem.h>
#include "kernel/memory/page.h"

#define PAGE_SIZE    0x1000
#define VIRTUAL_BASE 0xC0000000
#define MAGIC_NUMBER 0xDEADC0DE

/* defined in the linker script */
extern uint8_t _sprog, _spmm_bitmap, _eprog;

static uint64_t *g_bitmap; // more efficient comparison; 64 bits each

// size of the bitmap in bits
static uint32_t        g_bitmap_size;
static PhysicalAddress g_base;

static uint32_t get_first_n_free_idx(int n, uint32_t *bit);

void pmm_init(MemoryMapEntry *mmap, size_t mmap_length)
{
    g_bitmap = (uint64_t *)(&_spmm_bitmap);
    g_base   = 0;

    // limit address space to UINT32_MAX (0xFFFFFFFF)
    for (size_t i = 0; i < mmap_length; i++)
    {
        if ((uint64_t)mmap[i].base > UINT32_MAX)
        {
            mmap_length = i;
            break;
        }

        if ((uint64_t)mmap[i].base + mmap[i].length > UINT32_MAX)
        {
            mmap_length    = i + 1; // current index
            mmap[i].length = UINT32_MAX - mmap[i].base;
            break;
        }
    }

    // now, the memory map is limited to 0xFFFFFFFF
    uint64_t interm = mmap[mmap_length - 1].base + mmap[mmap_length - 1].length - mmap[0].base;
    interm          = ALIGN(interm, PAGE_SIZE);
    g_bitmap_size   = interm / PAGE_SIZE;

    // mark reserved areas as used
    for (size_t i = 0; i < mmap_length; i++)
    {
        // if not reserved, continue
        if (mmap[i].type != 0x02)
            continue;

        KSLOG("found reserved area (%d), marking as used\n", i);

        uint32_t base_lower = mmap[i].base; // limited to 32 bits
        uint32_t base_upper;
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

        uint32_t length = base_upper - base_lower;
        for (uint32_t j = base_lower, k = 0; j < g_bitmap_size && k < length; j++, k++)
        {
            uint32_t idx = j / 64;
            uint32_t bit = j % 64;

            SETBITVAR(g_bitmap[idx], 1 << bit);
        }
    }

    KSLOG("marking kernel as used\n");

    // mark bitmap and kernel as used
    uint32_t reserved = (PhysicalAddress)(&_eprog) - (PhysicalAddress)(&_sprog);
    reserved          = ALIGN(reserved, PAGE_SIZE) / PAGE_SIZE;
    for (uint32_t i = (PhysicalAddress)(&_sprog) / PAGE_SIZE, j = 0; j < reserved; i++, j++)
    {
        uint32_t idx = i / 64;
        uint32_t bit = i % 64;

        SETBITVAR(g_bitmap[idx], 1 << bit);
    }

    // also mark BDA as used
    SETBITVAR(g_bitmap[0], 1 << 0);
}

void *pmm_allocate_page(void)
{
    return pmm_allocate_pages(1); // cool trick guys
}

void pmm_free_page(void *page)
{
    pmm_free_pages(page, 1);
}

void *pmm_allocate_pages(int n)
{
    KSLOG("allocating %d physical page%s\n", n, n > 1 ? "s" : "");

    uint32_t bit;
    uint32_t idx = get_first_n_free_idx(n, &bit);

    if (idx == UINT32_MAX)
    {
        KSLOG("error: cannot find %d page%s of contiguous memory\n", n, n > 1 ? "s" : "");
        return (void *)MAGIC_NUMBER; // impossible to be returned on normal operation; not page-aligned
    }

    // get the bit-level index to the bitmap
    uint32_t i = idx * 64 + bit;
    for (int j = 0; i < g_bitmap_size && j < n; j++, i++)
    {
        // it's definitely true that setting metadata in a bitmap is faster than
        // allocation ;)
        SETBITVAR(g_bitmap[i / 64], 1 << (i % 64));
    }

    void *offset = (void *)(PhysicalAddress)((idx * 64 + bit) * PAGE_SIZE);
    KSLOG("returning page base 0x%08X\n", (PhysicalAddress)offset);
    return offset;
}

void pmm_free_pages(void *page_base, int n)
{
    KSLOG("freeing %d physical page%s\n", n, n > 1 ? "s" : "");

    PhysicalAddress base_addr = g_base;
    for (uint32_t i = 0; i < g_bitmap_size; i++, base_addr += PAGE_SIZE)
    {
        uint32_t idx = i / 64;
        uint32_t bit = i % 64;

        if (base_addr == (PhysicalAddress)page_base)
        {
            if (TESTBIT(g_bitmap[idx], (uint64_t)(1 << bit)))
            {
                for (int j = 0; j < n; j++, i++)
                    UNSETBITVAR(g_bitmap[i / 64], 1 << (i % 64));
            }
            else
            {
                KSLOG("warning: %d page%s starting from 0x%08X already free\n",
                      n,
                      n > 1 ? "s" : "",
                      (PhysicalAddress)page_base);
            }

            return;
        }
    }

    KSLOG("error: page 0x%08X cannot be found in the bitmap\n", (PhysicalAddress)page_base);
}

enum page_status pmm_get_page_status(void *page)
{
    PhysicalAddress base_addr = g_base;
    for (uint32_t i = 0; i < g_bitmap_size; i++, base_addr += PAGE_SIZE)
    {
        uint32_t idx = i / 64;
        uint32_t bit = i % 64;

        if (base_addr == (PhysicalAddress)page)
        {
            if (TESTBIT(g_bitmap[idx], (uint64_t)(1 << bit)))
                return PS_USED;
            else
                return PS_FREE;
        }
    }

    KSLOG("warning: cannot get status of page 0x%08X\n", (PhysicalAddress)page);
    return PS_UNKNOWN;
}

uint64_t pmm_get_bitmap_length(void)
{
    return ALIGN(g_bitmap_size, 64) / 64;
}

uint32_t get_first_n_free_idx(int n, uint32_t *bit)
{
    *bit = 0;

    for (uint32_t i = 0; i < g_bitmap_size;)
    {
        bool     found = true;
        uint32_t idx   = i / 64;
        *bit           = i % 64;

        // if all bits set, no free page for this index
        if (g_bitmap[idx] == UINT64_MAX)
        {
            i += 64;
            continue;
        }

        // if current bit is set, continue
        if (TESTBIT(g_bitmap[idx], (uint64_t)(1 << *bit)))
        {
            i++;
            continue;
        }

        // if not, look for n free blocks
        uint32_t init_idx = idx, init_bit = *bit;

        for (int j = 0; i < g_bitmap_size && j < n; j++, i++)
        {
            idx  = i / 64;
            *bit = i % 64;

            if (TESTBIT(g_bitmap[idx], (uint64_t)(1 << *bit)))
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            *bit = init_bit;
            return init_idx;
        }
    }

    return UINT32_MAX; // this is impossible to reach
}
