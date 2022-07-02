/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stddef.h>
#include <stdint.h>

#include <syslvl/core.h>
#include <syslvl/fat.h>
#include <syslvl/gdt.h>
#include <syslvl/hal.h>
#include <syslvl/idt.h>
#include <syslvl/interrupt_info.h>
#include <syslvl/io.h>
#include <syslvl/kbd.h>
#include <syslvl/log.h>
#include <syslvl/mem.h>
#include <syslvl/pic.h>
#include <syslvl/timer.h>
#include <syslvl/video.h>

#include <misc/num.h>
#include <misc/type_macros.h>

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

static const char *EXCEPTION_IDS[] = { "Divide-by-zero error",
                                       "Debug",
                                       "NMI",
                                       "Breakpoint",
                                       "Overflow",
                                       "Bound range exceeded",
                                       "Invalid opcode",
                                       "Device not available",
                                       "Double fault",
                                       "Coprocessor segment overrun", // should not be thrown
                                       "Invalid TSS",
                                       "Segment not present",
                                       "Stack segment fault",
                                       "General protection fault",
                                       "Page fault",
                                       "Reserved",
                                       "x87 floating-point exception",
                                       "Alignment check",
                                       "Machine check",
                                       "SIMD floating-point exception",
                                       "Virtualization exception",
                                       "Control protection exxception",
                                       "Hypervisor injection exception",
                                       "VMM communication exception",
                                       "Security exception",
                                       "Reserved",
                                       "Triple fault",
                                       "FPU error interrupt" };

static void _default_exception_handler(struct interrupt_info *info);

// main
int main(int argc, char **argv)
{
    screen_clear();

    struct fat_info fi;
    uint8_t         drive_number;

    // works!
    mem_copy(&fi, argv[1], sizeof fi);
    mem_copy(&drive_number, &argv[0], sizeof drive_number);

    log_append_format_string("[%s]: initializing the hardware abstraction layer\n", __func__);
    hal_use_default_interrupt_handler(_default_exception_handler);
    hal_init(PIC1_OFFSET, PIC2_OFFSET);

    log_append_format_string("[%s]: initializing the Programmable Interval Timer\n", __func__);
    timer_init();

    log_append_format_string("[%s]: initializing the keyboard\n", __func__);
    kbd_init();

    screen_print_string("\nzOS version 0.01\n");
    screen_print_string("Type something:\n\n");

    while (1)
    {
        char c = kbd_get_char();

        if (c == '\b')
            screen_print_string("\b \b");
        else
            screen_print_char(c);
    }

    return 1;
}

void _default_exception_handler(struct interrupt_info *info)
{
    screen_print_string("--------------- SYSTEM ERROR ---------------\n");
    screen_print_string("The kernel threw a fit trying to handle an interrupt and couldn't continue.\n"); // ah yes.

    screen_print_string("Kernel log for reference:\n");
    screen_print_format_string("%s\n", log_get_tmp_buffer());

    screen_print_format_string("Interrupt index: 0x%02X\n", info->vector);
    if (info->vector < 0x20)
        screen_print_format_string("Identifier: %s\n\n", EXCEPTION_IDS[info->vector]);

    screen_print_string("Register dump:\n");
    screen_print_format_string(
        "    EAX: 0x%08X     "
        "    EBX:    0x%08X\n"
        "    ECX: 0x%08X     "
        "    EDX:    0x%08X\n"
        "    ESI: 0x%08X     "
        "    EDI:    0x%08X\n"
        "    EBP: 0x%08X     "
        "    ESP:    0x%08X\n"
        "    EIP: 0x%08X     "
        "    EFLAGS: 0x%08X\n"
        "    CS: 0x%04X  "
        "    DS: 0x%04X  "
        "    SS: 0x%04X\n",
        info->eax,
        info->ebx,
        info->ecx,
        info->edx,
        info->esi,
        info->edi,
        info->ebp,
        info->esp,
        info->eip,
        info->eflags,
        info->cs,
        info->ds,
        info->ss);

    core_clear_interrupt_flag();
    core_halt();
}
