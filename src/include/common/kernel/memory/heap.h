/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

//! Kernel-only heap

#ifndef quuxHEAPHxuuq
#define quuxHEAPHxuuq

#include <stddef.h>

void heap_init(size_t init_size);

void *heap_allocate(size_t size);

void heap_free(void *ptr);

#endif
