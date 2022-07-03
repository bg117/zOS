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
#include <hal.h>
#include <idt.h>
#include <interrupt_info.h>
#include <io.h>
#include <kbd.h>
#include <log.h>
#include <mem.h>
#include <mmap.h>
#include <pic.h>
#include <pmm.h>
#include <timer.h>
#include <video.h>

#include <misc/log_macros.h>
#include <misc/num.h>
#include <misc/type_macros.h>

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

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

static void _pmm_test(void);

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

    // find region which is greater than or equal to __start
    size_t mmap_idx = 0;
    while (mmap_idx < mmap_length && memory_map[mmap_idx].base < CAST(uint64_t, &__start))
        ++mmap_idx;

    KLOG("initializing the hardware abstraction layer\n");
    hal_use_default_interrupt_handler(_default_exception_handler);
    hal_init(PIC1_OFFSET, PIC2_OFFSET);

    KLOG("initializing the Programmable Interval Timer\n");
    timer_init();

    KLOG("initializing the keyboard\n");
    kbd_init();

    KLOG("enabling interrupts\n");
    core_set_interrupt_flag();

    KLOG("initializing the physical memory manager\n");
    pmm_init(memory_map[mmap_idx].length);

    KLOG("testing the PMM\n");
    _pmm_test();

    screen_print_string("zOS version 0.01\n");
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

void _pmm_test()
{
    void            *p1        = pmm_allocate_page();
    enum page_status p1_status = pmm_get_page_status(p1);
    screen_print_format_string("Test 1 [alloc]: Address=%p, Status=%s\n",
                               p1,
                               p1_status == PS_USED   ? "used"
                               : p1_status == PS_FREE ? "free"
                                                      : "unknown");

    void            *p2        = pmm_allocate_page();
    enum page_status p2_status = pmm_get_page_status(p2);
    screen_print_format_string("Test 2 [alloc]: Address=%p, Status=%s\n",
                               p2,
                               p2_status == PS_USED   ? "used"
                               : p2_status == PS_FREE ? "free"
                                                      : "unknown");

    void            *p3        = pmm_allocate_page();
    enum page_status p3_status = pmm_get_page_status(p3);
    screen_print_format_string("Test 3 [alloc]: Address=%p, Status=%s\n",
                               p3,
                               p3_status == PS_USED   ? "used"
                               : p3_status == PS_FREE ? "free"
                                                      : "unknown");

    pmm_free_page(p2);
    p2_status = pmm_get_page_status(p2);
    screen_print_format_string("Test 2 [free]: Address=%p, Status=%s\n",
                               p2,
                               p2_status == PS_USED   ? "used"
                               : p2_status == PS_FREE ? "free"
                                                      : "unknown");

    pmm_free_page(p1);
    p1_status = pmm_get_page_status(p1);
    screen_print_format_string("Test 1 [free]: Address=%p, Status=%s\n",
                               p1,
                               p1_status == PS_USED   ? "used"
                               : p1_status == PS_FREE ? "free"
                                                      : "unknown");

    pmm_free_page(p3);
    p3_status = pmm_get_page_status(p3);
    screen_print_format_string("Test 3 [free]: Address=%p, Status=%s\n",
                               p3,
                               p3_status == PS_USED   ? "used"
                               : p3_status == PS_FREE ? "free"
                                                      : "unknown");
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
