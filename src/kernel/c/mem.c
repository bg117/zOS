/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <memory/mem.h>

#include <misc/type_macros.h>

void *mem_fill(void *ptr, uint8_t fill, size_t len)
{
    size_t i = 0;
    size_t x = len / sizeof len;

    uint32_t *al_ptr = ptr;
    uint32_t  u_fill = (((fill << 8) | (fill)) << 16) | ((fill << 8) | (fill));

    for (; i < x; i++)
        al_ptr[i] = u_fill;

    i *= sizeof len;

    uint8_t *sec_ptr = (uint8_t *)(al_ptr);

    for (; i < len; i++)
        sec_ptr[i] = fill;

    return ptr;
}

void *mem_copy(void *restrict dest, const void *restrict src, size_t len)
{
    size_t i = 0;
    size_t x = len / sizeof len;

    uint32_t       *al_dest = dest;
    const uint32_t *al_src  = src;

    for (; i < x; i++)
        al_dest[i] = al_src[i];

    i *= sizeof len;

    uint8_t       *sec_dest = (uint8_t *)(al_dest);
    const uint8_t *sec_src  = (uint8_t *)(al_src);

    for (; i < len; i++)
        sec_dest[i] = sec_src[i];

    return dest;
}

void *mem_copy_with_overlap(void *dest, const void *src, size_t len)
{
    uintptr_t isrc  = (uintptr_t)(src);
    uintptr_t idest = (uintptr_t)(dest);

    if (isrc > idest)
        return mem_copy(dest, src, len);

    if (isrc == idest)
        return dest;

    // do memcopy-the slow way
    const uint8_t *psrc  = src;
    uint8_t       *pdest = dest;

    while (len > 0)
    {
        pdest[len] = psrc[len];
        --len;
    }

    return dest;
}
