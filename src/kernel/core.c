/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <kernel/ll/core.h>

void core_halt(void)
{
    __asm__ volatile("hlt");
}

void core_set_interrupt_flag(void)
{
    __asm__ volatile("sti");
}

void core_clear_interrupt_flag(void)
{
    __asm__ volatile("cli");
}
