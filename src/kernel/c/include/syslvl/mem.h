/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxMEMHxuuq
#define quuxMEMHxuuq

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void *mem_fill(void *ptr, uint8_t fill, size_t len);

void *mem_copy(void *__restrict__ dest,
               const void *__restrict__ src,
               size_t len);

void *mem_copy_with_overlap(void *__restrict__ dest,
                            const void *__restrict__ src,
                            size_t len);

#ifdef __cplusplus
}
#endif

#endif
