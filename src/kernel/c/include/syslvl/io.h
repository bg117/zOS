/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxIOHxuuq
#define quuxIOHxuuq

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

uint8_t in_byte(uint16_t port);
void    out_byte(uint16_t port, uint8_t byte);

uint16_t in_word(uint16_t port);
void     out_word(uint16_t port, uint16_t word);

#if defined(__cplusplus)
}
#endif

#endif
