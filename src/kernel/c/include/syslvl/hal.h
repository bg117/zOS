/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxHALHxuuq
#define quuxHALHxuuq

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct interrupt_info;

/**
 * @brief Initializes the hardware abstraction layer.
 *
 * @param pic1_offset The offset of the master PIC, for IRQs.
 * @param pic2_offset The offset of the slave PIC.
 */
void hal_init(uint8_t pic1_offset, uint8_t pic2_offset);

/**
 * @brief Sets the default interrupt handler for the 256 interrupts.
 *
 * @param handler The interrupt handler which takes a pointer to an
 *                interrupt_info struct (found in interrupt_info.h).
 */
void hal_use_default_interrupt_handler(
    void (*handler)(struct interrupt_info *));

/**
 * @brief Maps a handler for the corresponding interrupt.
 *
 * @param vector The index of the interrupt to map the handler to.
 * @param handler The interrupt handler.
 */
void hal_map_exception_handler(uint8_t vector,
                               void (*handler)(struct interrupt_info *));

#ifdef __cplusplus
}
#endif

#endif
