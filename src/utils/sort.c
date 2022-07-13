/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <utils/sort.h>

// rudimentary memcopy
static void *rd_mem_copy(void *restrict dest, const void *restrict src, uint32_t len);

void sort_bubble(void *arr, size_t elems, size_t elem_size, int (*cmp)(const void *, const void *))
{
    // warning: very slow
    for (size_t i = 0; i < elems; i++)
    {
        for (size_t j = 0; j < elems - i - 1; j++)
        {
            if (cmp(arr + j * elem_size, arr + (j + 1) * elem_size) > 0)
            {
                uint8_t tmp[elem_size];
                rd_mem_copy(tmp, arr + j * elem_size, elem_size);
                rd_mem_copy(arr + j * elem_size, arr + (j + 1) * elem_size, elem_size);
                rd_mem_copy(arr + (j + 1) * elem_size, tmp, elem_size);
            }
        }
    }
}

void *rd_mem_copy(void *restrict dest, const void *restrict src, uint32_t len)
{
    uint32_t i = 0;
    uint32_t x = len / 4;

    uint32_t       *al_dest = dest;
    const uint32_t *al_src  = src;

    for (; i < x; i++)
        al_dest[i] = al_src[i];

    i *= 4;

    uint8_t       *sec_dest = (uint8_t *)al_dest;
    const uint8_t *sec_src  = (uint8_t *)al_src;

    for (; i < len; i++)
        sec_dest[i] = sec_src[i];

    return dest;
}
