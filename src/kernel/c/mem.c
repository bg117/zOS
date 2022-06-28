/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <syslvl/mem.h>

#include <misc/type_macros.h>

void *memcopy(void *__restrict__ dest, const void *__restrict__ src, size_t len)
{
    size_t i = 0;
    size_t x = len / sizeof len;

    size_t       *al_dest = dest;
    const size_t *al_src  = src;

    for (; i < x; i++)
        al_dest[i] = al_src[i];

    i *= sizeof len;

    uint8_t       *sec_dest = CAST(uint8_t *, al_dest);
    const uint8_t *sec_src  = CAST(uint8_t *, al_src);

    for (; i < len; i++)
        sec_dest[i] = sec_src[i];

    return dest;
}

void *memcopyovlp(void *__restrict__ dest,
                  const void *__restrict__ src,
                  size_t len)
{
    uintptr_t isrc  = CAST(uintptr_t, src);
    uintptr_t idest = CAST(uintptr_t, dest);

    if (isrc > idest)
        return memcopy(dest, src, len);

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
