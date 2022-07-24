/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>
#include <utils/mem.h>

void *mem_fill(void *ptr, uint8_t fill, uint32_t len)
{
    return mem_fill8(ptr, fill, len);
}

void *mem_fill8(void *ptr, uint8_t fill, uint32_t len)
{
    uint32_t i = 0;
    uint32_t x = len / 4;

    uint32_t *al_ptr = ptr;
    uint32_t  u_fill = fill << 8 | fill << 16 | fill << 8 | fill;

    for (; i < x; i++)
        al_ptr[i] = u_fill;

    i *= 4;

    uint8_t *sec_ptr = (uint8_t *)al_ptr;

    for (; i < len; i++)
        sec_ptr[i] = fill;

    return ptr;
}

void *mem_fill16(void *ptr, uint16_t fill, uint32_t len)
{
    if (len % 2 != 0)
        return ptr; // not a multiple of 2

    uint32_t i = 0;
    uint32_t x = len / 4;

    uint32_t *al_ptr = ptr;
    uint32_t  u_fill = fill << 16 | fill;

    for (; i < x; i++)
        al_ptr[i] = u_fill;

    i *= 4;

    uint16_t *sec_ptr = (uint16_t *)al_ptr;

    for (; i < (len - i) / 2; i++)
        sec_ptr[i] = fill;

    return ptr;
}

void *mem_fill32(void *ptr, uint32_t fill, uint32_t len)
{
    if (len % 4 != 0)
        return ptr; // not a multiple of 4

    uint32_t i = 0;
    uint32_t x = len / 4;

    uint32_t *al_ptr = ptr;

    for (; i < x; i++)
        al_ptr[i] = fill;

    return ptr;
}

void *mem_copy(void *restrict dest, const void *restrict src, uint32_t len)
{
    uint32_t i = 0;
    uint32_t x = len / 4;

    uint32_t       *al_dest = dest;
    const uint32_t *al_src  = src;

    for (; i < x; i++)
        al_dest[i] = al_src[i];

    i *= 4;

    uint8_t       *sec_dest = (uint8_t *)al_dest;
    const uint8_t *sec_src  = (const uint8_t *)al_src;

    for (; i < len; i++)
        sec_dest[i] = sec_src[i];

    return dest;
}

void *mem_copy_with_overlap(void *dest, const void *src, uint32_t len)
{
    uint32_t isrc  = (uint32_t)src;
    uint32_t idest = (uint32_t)dest;

    if (isrc > idest)
        return mem_copy(dest, src, len);

    if (isrc == idest)
        return dest;

    // optimized a bit
    uint32_t x = len / 4;
    uint32_t i = x;
    uint32_t j = len;

    uint32_t       *al_dest = (uint32_t *)dest + len;
    const uint32_t *al_src  = (const uint32_t *)src + len;

    for (; i > 0; i--, j -= 4)
        al_dest[i] = al_src[i];

    uint8_t       *sec_dest = (uint8_t *)al_dest;
    const uint8_t *sec_src  = (const uint8_t *)al_src;

    for (; j > 0; j--)
        sec_dest[i] = sec_src[i];

    return dest;
}
