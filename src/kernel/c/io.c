/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <kernel/ll/io.h>

uint8_t in_byte(uint16_t port)
{
    uint8_t ret = 0;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));

    return ret;
}

void out_byte(uint16_t port, uint8_t byte)
{
    asm volatile("outb %0, %1" ::"a"(byte), "Nd"(port));
}

uint16_t in_word(uint16_t port)
{
    uint16_t ret = 0;
    asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));

    return ret;
}

void out_word(uint16_t port, uint16_t word)
{
    asm volatile("outw %0, %1" ::"a"(word), "Nd"(port));
}
