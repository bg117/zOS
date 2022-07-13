/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <kernel/ll/core.h>

void core_halt(void)
{
    asm volatile("hlt");
}

void core_set_interrupt_flag(void)
{
    asm volatile("sti");
}

void core_clear_interrupt_flag(void)
{
    asm volatile("cli");
}
