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

typedef struct memory_map_entry MemoryMapEntry;

/**
 * @brief Initializes the kernel.
 *
 * @param mmap The memory map to use.
 * @param mmap_length The length of the memory map.
 */
void kernel_init(MemoryMapEntry *mmap, size_t mmap_length);

/**
 * @brief Deinitializes the kernel, modules, and the associated
 *        hardware.
 */
void kernel_deinit(void);

#ifdef __cplusplus
}
#endif

#endif
