/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxCOREHxuuq
#define quuxCOREHxuuq

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Halts the CPU (executes the HLT instruction).
 */
void core_halt(void);

/**
 * @brief Sets the interrupt flag.
 */
void core_set_interrupt_flag(void);

/**
 * @brief Clears the interrupt flag.
 */
void core_clear_interrupt_flag(void);

#ifdef __cplusplus
}
#endif

#endif
