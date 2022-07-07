/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxKERNELHxuuq
#define quuxKERNELHxuuq

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct interrupt_info;
struct memory_map;

/**
 * @brief Initializes the kernel.
 *
 * @param mmap The memory map to use.
 * @param mmap_length The length of the memory map.
 */
void kernel_init(struct memory_map *mmap, size_t mmap_length);

#ifdef __cplusplus
}
#endif

#endif
