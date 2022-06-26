/**
 * Copyright (c) 2022 iusearchbtw
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <syslvl/i.h>
#include <syslvl/mem.h>

void *memoryCopy(void *__restrict__ dest,
                 const void *__restrict__ src,
                 size_t len)
{
    size_t i = 0;
    size_t x = len / sizeof len;

    size_t       *alignedDest = dest;
    const size_t *alignedSrc  = src;

    for (; i < x; i++)
        alignedDest[i] = alignedSrc[i];

    i *= sizeof len;

    uint8_t       *secondItrDest = CAST(uint8_t *, alignedDest);
    const uint8_t *secondItrSrc  = CAST(uint8_t *, alignedSrc);

    for (; i < len; i++)
        secondItrDest[i] = secondItrSrc[i];

    return dest;
}
