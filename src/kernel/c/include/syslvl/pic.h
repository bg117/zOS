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

void pic_init(uint8_t master_offset, uint8_t slave_offset);
void pic_send_eoi(uint8_t irq);

uint8_t pic_get_pic1_offset();
uint8_t pic_get_pic2_offset();

#ifdef __cplusplus
}
#endif

#endif
