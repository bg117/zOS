/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxISRHxuuq
#define quuxISRHxuuq

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct interrupt_info InterruptInfo;

/**
 * @brief Initializes the interrupt service routines.
 *
 * @param code_offset The offset to the code segment in the GDT.
 */
void isr_init(uint8_t code_offset);

/**
 * @brief Loads the Interrupt Descriptor Table created by
 *        the initialization function.
 */
void isr_load_idt(void);

/**
 * @brief Maps a custom interrupt handler for the specified
 *        interrupt vector to be executed when an interrupt with
 *        the vector gets raised.
 * @param vector The vector of the interrupt for the handler to be
 *               assigned to.
 * @param handler A pointer to a function that handles the specified
 *                interrupt.
 */
void isr_map_interrupt_handler(uint8_t vector, void (*handler)(InterruptInfo *));

/**
 * @brief Unmaps the custom interrupt handler assigned to the
 *        specified interrupt vector.
 *
 * @param vector The vector of the interrupt for the handler to be
 *               unmapped from.
 */
void isr_unmap_interrupt_handler(uint8_t vector);

#ifdef __cplusplus
}
#endif

#endif
