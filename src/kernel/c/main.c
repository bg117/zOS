/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stddef.h>
#include <stdint.h>

#include <syslvl/core.h>
#include <syslvl/disk.h>
#include <syslvl/fat.h>
#include <syslvl/gdt.h>
#include <syslvl/hal.h>
#include <syslvl/idt.h>
#include <syslvl/io.h>
#include <syslvl/mem.h>
#include <syslvl/pic.h>
#include <syslvl/video.h>

#include <misc/num.h>
#include <misc/type_macros.h>

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

static const char US_SCANCODES[128] = {
    0,    27,  '1', '2', '3',  '4', '5', '6', '7',  '8', /* 9 */
    '9',  '0', '-', '=', '\b',                           /* Backspace */
    '\t',                                                /* Tab */
    'q',  'w', 'e', 'r',                                 /* 19 */
    't',  'y', 'u', 'i', 'o',  'p', '[', ']', '\n',      /* Enter key */
    0,                                                   /* 29   - Control */
    'a',  's', 'd', 'f', 'g',  'h', 'j', 'k', 'l',  ';', /* 39 */
    '\'', '`', 0,                                        /* Left shift */
    '\\', 'z', 'x', 'c', 'v',  'b', 'n',                 /* 49 */
    'm',  ',', '.', '/', 0,                              /* Right shift */
    '*',  0,                                             /* Alt */
    ' ',                                                 /* Space bar */
    0,                                                   /* Caps lock */
    0,                                                   /* 59 - F1 key ... > */
    0,    0,   0,   0,   0,    0,   0,   0,   0,         /* < ... F10 */
    0,                                                   /* 69 - Num lock*/
    0,                                                   /* Scroll Lock */
    0,                                                   /* Home key */
    0,                                                   /* Up Arrow */
    0,                                                   /* Page Up */
    '-',  0,                                             /* Left Arrow */
    0,    0,                                             /* Right Arrow */
    '+',  0,                                             /* 79 - End key*/
    0,                                                   /* Down Arrow */
    0,                                                   /* Page Down */
    0,                                                   /* Insert Key */
    0,                                                   /* Delete Key */
    0,    0,   0,   0,                                   /* F11 Key */
    0,                                                   /* F12 Key */
    0, /* All other keys are undefined */
};

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

static void _default_exception_handler(struct exception_info *regs);
static void _default_irq_handler(struct exception_info *regs);
static void _irq1_handler(struct exception_info *regs);

static volatile int _lock = 1;

// main
int main(int argc, char **argv)
{
    scrclr();

    struct fat_info fi;
    uint8_t         driveNumber;

    // works!
    memcopy(&fi, argv[1], sizeof fi);
    memcopy(&driveNumber, &argv[0], sizeof driveNumber);

    halusedefexcept(_default_exception_handler);
    halinit(PIC1_OFFSET, PIC2_OFFSET);

    // map default IRQ handlers
    for (int i = 0; i < 16; i++)
        halmapexcept(i + PIC1_OFFSET, _default_irq_handler);

    // map keyboard handler
    halmapexcept(1 /* IRQ 1 */ + PIC1_OFFSET, _irq1_handler);

    scrputs("zOS version 0.01\r\n");
    scrputs("Type something:\r\n\r\n");

    while (_lock)
        ;

    return 1;
}

void _default_irq_handler(struct exception_info *regs)
{
    picsendeoi(regs->vector - PIC1_OFFSET);
}

void _irq1_handler(struct exception_info *regs)
{
    char read;
    int  isready = 0;

    while (_lock /* remove opportunity to optimize */)
    {
        if ((inb(0x64) & 1) == 0)
            continue;

        read    = inb(0x60);
        isready = 1;
        break;
    }

    if (isready)
    {
        if (!(read & 0x80))
            scrputc(US_SCANCODES[CAST(int, read)]);
    }

    outb(0x80, 0x00);

    picsendeoi(regs->vector - PIC1_OFFSET);
}

void _default_exception_handler(struct exception_info *regs)
{
    scrputs("--------------- SYSTEM ERROR ---------------\r\n");
    scrputs(
        "The kernel threw a fit trying to handle an interrupt and "
        "couldn't continue.\r\n"); // ah yes.
    scrputsf("Interrupt index: 0x%X\r\n", regs->vector);
    if (regs->vector < 0x20)
        scrputsf("Identifier: %s\r\n", EXCEPTION_IDS[regs->vector]);

    scrputs("Dump:\r\n");
    scrputsf(
        "    EAX: 0x%X\r\n    EBX: 0x%X\r\n    ECX: 0x%X\r\n    EDX: "
        "0x%X\r\n    ESI: "
        "0x%X\r\n    EDI: "
        "0x%X\r\n    EBP: 0x%X\r\n    ESP: 0x%X\r\n    CS: "
        "0x%X\r\n    DS: "
        "0x%X\r\n    SS: "
        "0x%X\r\n    EFLAGS: 0b%b\r\n",
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

    corehlt();
}
