/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <limits.h>
#include <stdint.h>

#include <mem.h>
#include <mmap.h>
#include <pmm.h>
#include <serial.h>

#include <misc/bit_macros.h>
#include <misc/log_macros.h>
#include <misc/type_macros.h>

#define PAGE_SIZE 0x1000

/* defined in the linker script */
extern uint8_t __start, __end;

static uint8_t *_bitmap = &__end;

// size of the bitmap in bits
static uint64_t _bitmap_size;
static uint64_t _base;

static uint64_t _pmm_get_first_free_idx(uint8_t *bit);

void /* _pmm_proto_init */ pmm_init(struct memory_map *mmap, size_t mmap_length)
{
    _base        = 0ULL;
    _bitmap_size = mmap[mmap_length - 1].base + mmap[mmap_length - 1].length - mmap[0].base;
    _bitmap_size = ALIGN(_bitmap_size, PAGE_SIZE) / PAGE_SIZE;
    mem_fill(_bitmap, 0, _bitmap_size);

    // mark reserved areas as used
    for (size_t i = 0; i < mmap_length; i++)
    {
        // if not reserved, continue
        if (mmap[i].type != 0x02)
            continue;

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
        for (uint64_t j = base_lower, k = 0; j < _bitmap_size && k < length; j++, k++)
        {
            uint64_t byte = j / CHAR_BIT;
            uint8_t  bit  = j % CHAR_BIT;

            SETBITVAR(_bitmap[byte], 1 << bit);
        }
    }

    // mark bitmap and kernel as used
    uint64_t reserved = (uint64_t)(&__end) - (uint64_t)(&__start) + _bitmap_size;
    reserved          = ALIGN(reserved, PAGE_SIZE) / PAGE_SIZE;
    for (uint64_t i = (uint64_t)(&__start) / PAGE_SIZE; i < reserved; i++)
    {
        uint64_t byte = i / CHAR_BIT;
        uint8_t  bit  = i % CHAR_BIT;

        SETBITVAR(_bitmap[byte], 1 << bit);
    }
}

void *pmm_allocate_page(void)
{
    uint8_t  bit;
    uint64_t idx = _pmm_get_first_free_idx(&bit);

    if (idx == UINT64_MAX)
    {
        KSLOG("warning: no more free pages left\n");
        return NULL; // just like good old (m|c|re)alloc
    }

    // it's definitely true that setting metadata in a bitmap is faster than
    // allocation ;)
    SETBITVAR(_bitmap[idx], 1 << bit);

    void *offset = (void *)((idx * CHAR_BIT + bit) * PAGE_SIZE);
    KSLOG("returning page %p\n", offset);
    return offset;
}

void pmm_free_page(void *page)
{
    for (uint64_t i = 0, base_addr = _base; i < _bitmap_size; i++, base_addr += PAGE_SIZE)
    {
        uint64_t byte = i / CHAR_BIT;
        uint8_t  bit  = i % CHAR_BIT;

        if (base_addr == (uint64_t)(page))
        {
            if ((_bitmap[byte] & (1 << bit)))
                UNSETBITVAR(_bitmap[byte], 1 << bit);
            else
                KSLOG("warning: page %p already free\n", page);

            return;
        }
    }

    KSLOG("warning: page %p cannot be found in the bitmap\n", page);
}

enum page_status pmm_get_page_status(void *page)
{
    for (uint64_t i = 0, base_addr = _base; i < _bitmap_size; i++, base_addr += PAGE_SIZE)
    {
        uint64_t byte = i / CHAR_BIT;
        uint8_t  bit  = i % CHAR_BIT;

        if (base_addr == (uint64_t)(page))
        {
            if ((_bitmap[byte] & (1 << bit)))
                return PS_USED;
            else
                return PS_FREE;
        }
    }

    KSLOG("warning: cannot get status of page %p, possibly not page-aligned\n", page);
    return PS_UNKNOWN;
}

static uint64_t _pmm_get_first_free_idx(uint8_t *bit)
{
    *bit = 0;

    for (uint64_t i = 0, base_addr = _base; i < _bitmap_size; i++, base_addr += PAGE_SIZE)
    {
        uint64_t byte = i / CHAR_BIT;
        *bit          = i % CHAR_BIT;

        if ((_bitmap[byte] & (1 << *bit)) == 0)
            return byte;
    }

    return UINT64_MAX; // this is impossible to reach
}
