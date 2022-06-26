/**
 * Copyright (c) 2022 iusearchbtw
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxDEVHxuuq
#define quuxDEVHxuuq

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

uint8_t inByte(uint8_t port);
void    outByte(uint8_t port);

uint16_t inWord(uint16_t port);
void     outWord(uint16_t port);

#if defined(__cplusplus)
}
#endif

#endif
