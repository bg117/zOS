/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxPICHxuuq
#define quuxPICHxuuq

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the Programmable Interrupt Controller.
 *
 * @param master_offset The offset to the interrupt vector to handle the IRQ
 *                      for IRQs 0-7.
 * @param slave_offset The offset to the interrupt vector to handle the IRQ
 *                     for IRQs 8-15.
 */
void pic_init(uint8_t master_offset, uint8_t slave_offset);

/**
 * @brief Sends an End-of-Interrupt signal to the master PIC (and slave PIC,
 *        if necessary.)
 *
 * @param irq The index of the IRQ.
 */
void pic_send_eoi(uint8_t irq);

/**
 * @brief Retrieves the offset to the interrupt vector handling IRQs 0-7.
 *
 * @return The offset.
 */
uint8_t pic_get_pic1_offset();

/**
 * @brief Retrieves the offset to the interrupt vector handling IRQs 8-15.
 *
 * @return The offset.
 */
uint8_t pic_get_pic2_offset();

#ifdef __cplusplus
}
#endif

#endif
