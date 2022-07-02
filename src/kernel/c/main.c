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
#include <syslvl/mem.h>
#include <syslvl/pic.h>
#include <syslvl/timer.h>
#include <syslvl/video.h>

#include <misc/num.h>
#include <misc/type_macros.h>

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

static const char *EXCEPTION_IDS[]
    = { "Divide-by-zero error",
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

static void _default_exception_handler(struct interrupt_info *regs);

// main
int main(int argc, char **argv)
{
    screen_clear();

    struct fat_info fi;
    uint8_t         drive_number;

    // works!
    mem_copy(&fi, argv[1], sizeof fi);
    mem_copy(&drive_number, &argv[0], sizeof drive_number);

    hal_use_default_interrupt_handler(_default_exception_handler);
    hal_init(PIC1_OFFSET, PIC2_OFFSET);

    timer_init();
    kbd_init();

    screen_print_string("zOS version 0.01\n");
    screen_print_string("Type something:\n\n");

    while (1)
    {
        char c = kbd_get_char();
        screen_print_char(c);
    }

    return 1;
}

void _default_exception_handler(struct interrupt_info *regs)
{
    screen_print_string("--------------- SYSTEM ERROR ---------------\n");
    screen_print_string(
        "The kernel threw a fit trying to handle an interrupt and "
        "couldn't continue.\n"); // ah yes.
    screen_print_format_string("Interrupt index: 0x%X\n", regs->vector);
    if (regs->vector < 0x20)
        screen_print_format_string("Identifier: %s\n",
                                   EXCEPTION_IDS[regs->vector]);

    screen_print_string("Dump:\n");
    screen_print_format_string(
        "    EAX: 0x%X\n    EBX: 0x%X\n    ECX: 0x%X\n    EDX: "
        "0x%X\n    ESI: "
        "0x%X\n    EDI: "
        "0x%X\n    EBP: 0x%X\n    ESP: 0x%X\n    CS: "
        "0x%X\n    DS: "
        "0x%X\n    SS: "
        "0x%X\n    EFLAGS: 0b%b\n",
        regs->eax,
        regs->ebx,
        regs->ecx,
        regs->edx,
        regs->esi,
        regs->edi,
        regs->ebp,
        regs->esp,
        regs->cs,
        regs->ds,
        regs->ss,
        regs->eflags);

    core_clear_interrupt_flag();
    core_halt();
}
