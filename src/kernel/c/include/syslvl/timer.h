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

void timer_init(void);
void timer_set_cycle(int hz);
void timer_wait(int ms);

#ifdef __cplusplus
}
#endif

#endif
