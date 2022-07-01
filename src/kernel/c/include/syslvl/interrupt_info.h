/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxEXCEPTION_INFOHxuuq
#define quuxEXCEPTION_INFOHxuuq

#include <stdint.h>

/**
 * @brief Provides a list of registers passed onto the stack
 *        when an interrupt is called.
 */
struct __attribute__((packed)) interrupt_info
{
    uint32_t ds;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    // CPU-managed
    uint32_t vector;
    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp_cpu;
    uint32_t ss;
};

#endif
