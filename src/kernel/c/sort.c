/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <kernel/memory/mem.h>

#include <kernel/misc/sort.h>

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
                mem_copy(tmp, arr + j * elem_size, elem_size);
                mem_copy(arr + j * elem_size, arr + (j + 1) * elem_size, elem_size);
                mem_copy(arr + (j + 1) * elem_size, tmp, elem_size);
            }
        }
    }
}
