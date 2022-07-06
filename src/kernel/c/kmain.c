/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stddef.h>
#include <stdint.h>

#include <core.h>
#include <fat.h>
#include <gdt.h>
#include <idt.h>
#include <interrupt_info.h>
#include <io.h>
#include <kbd.h>
#include <kernel.h>
#include <mem.h>
#include <mmap.h>
#include <pic.h>
#include <pmm.h>
#include <serial.h>
#include <timer.h>
#include <video.h>

#include <misc/log_macros.h>
#include <misc/num.h>
#include <misc/type_macros.h>

extern uint8_t __start;

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
static void _page_fault_handler(struct interrupt_info *info);

// main
int kmain(uint8_t drive_number, struct fat_info *fi, struct memory_map *mmap, uint16_t mmap_length)
{
    screen_clear();

    struct fat_info   fat_info;
    struct memory_map memory_map[mmap_length];

    // save structs because they may get overwritten in the future
    mem_copy(&fat_info, fi, sizeof fat_info);
    mem_copy(memory_map, mmap, sizeof(struct memory_map) * mmap_length);

    screen_print_string("---------------- MEMORY MAP ----------------\n");
    screen_print_format_string("%hu %s in the memory map\n", mmap_length, mmap_length > 1 ? "entries" : "entry");
    for (int i = 0; i < mmap_length; i++)
    {
        screen_print_format_string("%d: Base=0x%016llX, Length=0x%016llX, Type=%u, AcpiExtAttrs=%u\n",
                                   i,
                                   memory_map[i].base,
                                   memory_map[i].length,
                                   memory_map[i].type,
                                   memory_map[i].acpi_extended_attributes);
    }

    KSLOG("initializing the kernel\n");
    kernel_use_default_interrupt_handler(_default_exception_handler);
    kernel_map_exception_handler(14, _page_fault_handler);
    kernel_init(memory_map, mmap_length);

    //! page fault test
    (void)*(char *)(0x90000000);

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

    kernel_fin();

    return 1;
}

void _default_exception_handler(struct interrupt_info *info)
{
    KSLOG("unhandled interrupt 0x%02X.\n", info->vector);
    KSLOG(
        "register dump:\n\t"
        "eax=0x%08X ebx=0x%08X ecx=0x%08X edx=0x%08X\n\t"
        "esi=0x%08X edi=0x%08X ebp=0x%08X esp=0x%08X\n\t"
        "cs=0x%04X ds=0x%04X ss=0x%04X\n",
        info->eax,
        info->ebx,
        info->ecx,
        info->edx,
        info->esi,
        info->edi,
        info->ebp,
        info->esp,
        info->cs,
        info->ds,
        info->ss);

    screen_print_string("--------------- SYSTEM ERROR ---------------\n");
    screen_print_string("The kernel threw a fit trying to handle an interrupt and couldn't continue.\n"); // ah yes.

    screen_print_string("Check serial port COM1 for logs.\n");

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

static void _page_fault_handler(struct interrupt_info *info)
{
    uint32_t cr2;
    __asm__ __volatile__("movl %%cr2, %0" : "=r"(cr2) :); // get value of CR2 (contains the address of page fault)
    KSLOG("page fault@0x%08X. Error code: 0x%X\n", cr2, info->error_code);
    KSLOG(
        "register dump:\n\t"
        "eax=0x%08X ebx=0x%08X ecx=0x%08X edx=0x%08X\n\t"
        "esi=0x%08X edi=0x%08X ebp=0x%08X esp=0x%08X\n\t"
        "cs=0x%04X ds=0x%04X ss=0x%04X\n",
        info->eax,
        info->ebx,
        info->ecx,
        info->edx,
        info->esi,
        info->edi,
        info->ebp,
        info->esp,
        info->cs,
        info->ds,
        info->ss);

    screen_print_string("--------------- SYSTEM ERROR ---------------\n");
    screen_print_format_string("A process tried to access address 0x%08X and caused a page fault.\n", cr2);

    // temporary
    core_clear_interrupt_flag();
    core_halt();
}
