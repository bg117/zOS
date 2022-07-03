/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <syslvl/log.h>
#include <syslvl/pmm.h>

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

void pmm_init(uint64_t len)
{
    // round len down to nearest multiple of PAGE_SIZE, just to be sure
    uint64_t tmp = len / PAGE_SIZE;
    uint64_t size;

    len          = tmp * PAGE_SIZE;
    _bitmap_size = tmp; // length / 4096 / 8

    if (tmp % 8 > 0)
        ++_bitmap_size;

    // mark reserved areas as used (kernel + bitmap)
    size = __end - __start + _bitmap_size;
    size = ((size + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE);
    size /= PAGE_SIZE;

    KLOG("marking reserved areas as used\n");
    // size = counter; mark all reserved pages as used
    for (uint64_t i = 0, bitmap_idx = 0, bit = 0; i < size; i++, bit++)
    {
        if (i > 0 && i % 8 == 0)
        {
            ++bitmap_idx;
            bit = 0;
        }

        SETBITVAR(_bitmap[bitmap_idx], 1 << bit);
    }

    _base = CAST(uint64_t, &__start);
}

void *pmm_allocate_page(void)
{
    uint8_t  bit;
    uint64_t idx = _pmm_get_first_free_idx(&bit);

    // it's definitely true that setting metadata in a bitmap is faster than
    // allocation ;)
    SETBITVAR(_bitmap[idx], 1 << bit);

    uint64_t offset = (idx * 8 + bit) * PAGE_SIZE + _base;
    return CAST(void *, offset);
}

void pmm_free_page(void *page)
{
    uint8_t bit = 0;
    for (uint64_t i = 0, base_addr = CAST(uint64_t, _base), bitmap_idx = 0; i < _bitmap_size;
         i++, bit++, base_addr += PAGE_SIZE)
    {
        if (i > 0 && i % 8 == 0)
        {
            ++bitmap_idx;
            bit = 0;
        }

        if (base_addr == CAST(uint64_t, page))
        {
            if ((_bitmap[bitmap_idx] & (1 << bit)) == 1)
                UNSETBITVAR(_bitmap[bitmap_idx], 1 << bit);
            else
                KLOG("warning: page %p already free\n", page);

            return;
        }
    }

    KLOG("warning: page %p cannot be found in the bitmap\n", page);
}

enum page_status pmm_get_page_status(void *page)
{
    uint8_t bit = 0;
    for (uint64_t i = 0, base_addr = CAST(uint64_t, _base), bitmap_idx = 0; i < _bitmap_size;
         i++, bit++, base_addr += PAGE_SIZE)
    {
        if (i > 0 && i % 8 == 0)
        {
            ++bitmap_idx;
            bit = 0;
        }

        if (base_addr == CAST(uint64_t, page))
        {
            if ((_bitmap[bitmap_idx] & (1 << bit)) == 1)
                return PS_USED;
            else
                return PS_FREE;
        }
    }

    KLOG("warning: cannot get status of page %p, possibly not page-aligned\n", page);
    return PS_UNKNOWN;
}

static uint64_t _pmm_get_first_free_idx(uint8_t *bit)
{
    *bit = 0;

    for (uint64_t i = 0, bitmap_idx = 0; i < _bitmap_size; i++, (*bit)++)
    {
        if (i > 0 && i % 8 == 0)
        {
            ++bitmap_idx;
            *bit = 0;
        }

        if ((_bitmap[bitmap_idx] & (1 << *bit)) == 0)
            return bitmap_idx;
    }

    KLOG("warning: no more free pages left\n");
    return UINT64_MAX; // this is impossible to reach
}
