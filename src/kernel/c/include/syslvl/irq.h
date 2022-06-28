/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxIRQHxuuq
#define quuxIRQHxuuq

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void irqinit(uint8_t master_offset, uint8_t slave_offset);
void irqsendeoi(uint8_t irq);

#ifdef __cplusplus
}
#endif

#endif
