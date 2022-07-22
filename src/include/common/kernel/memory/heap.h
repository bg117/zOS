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

/**
 * @brief Initializes the kernel heap.
 *
 * @param init_size The initial size of the heap.
 */
void heap_init(size_t init_size);

/**
 * @brief Allocates a block of memory from the kernel heap.
 *
 * @param size The size of the block.
 * @return The block allocated.
 */
void *heap_allocate(size_t size);

/**
 * @brief Resizes a block of memory to new_size, and frees the old
 *        block.
 *
 * @param old_ptr The block to reallocate.
 * @param new_size The new size of the block.
 * @return The reallocated block.
 */
void *heap_reallocate(void *old_ptr, size_t new_size);

/**
 * @brief Frees a block of memory in order to be reused for future
 *        allocations.
 *
 * @param ptr The block of memory to free.
 */
void heap_free(void *ptr);

#endif
