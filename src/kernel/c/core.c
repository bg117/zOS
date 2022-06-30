/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <syslvl/core.h>

void core_halt(void)
{
    __asm__ __volatile__("hlt");
}

void core_set_interrupt_flag(void)
{
    __asm__ __volatile__("sti");
}

void core_clear_interrupt_flag(void)
{
    __asm__ __volatile__("cli");
}
