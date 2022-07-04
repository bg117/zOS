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

/**
 * @brief Sets the default interrupt handler for the 256 interrupts.
 *
 * @param handler The interrupt handler which takes a pointer to an
 *                interrupt_info struct (found in interrupt_info.h).
 */
void kernel_use_default_interrupt_handler(void (*handler)(struct interrupt_info *));

/**
 * @brief Maps a handler for the corresponding interrupt.
 *
 * @param vector The index of the interrupt to map the handler to.
 * @param handler The interrupt handler.
 */
void kernel_map_exception_handler(uint8_t vector, void (*handler)(struct interrupt_info *));

#ifdef __cplusplus
}
#endif

#endif
