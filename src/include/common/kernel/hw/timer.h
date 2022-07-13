/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxTIMERHxuuq
#define quuxTIMERHxuuq

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the Programmable Interval Timer and its handler.
 */
void timer_init(void);

/**
 * @brief Deinitializes the PIC (unmaps its handler).
 */
void timer_deinit(void);

/**
 * @brief Sets how many cycles it takes for 1 second.
 *
 * @param hz The new frequency of the PIT.
 */
void timer_set_cycle(int hz);

/**
 * @brief Waits for the specified amount of time (in milliseconds).
 *
 * @param ms The amount of time to wait.
 */
void timer_wait(int ms);

#ifdef __cplusplus
}
#endif

#endif
