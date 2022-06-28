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

uint8_t __attribute__((cdecl)) inb(uint8_t port);
void __attribute__((cdecl)) outb(uint8_t port, uint8_t byte);

uint16_t __attribute__((cdecl)) inw(uint16_t port);
void __attribute__((cdecl)) outw(uint16_t port, uint16_t word);

#if defined(__cplusplus)
}
#endif

#endif
