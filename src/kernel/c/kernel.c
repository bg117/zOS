/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <core.h>
#include <gdt.h>
#include <idt.h>
#include <interrupt_info.h>
#include <isr.h>
#include <kbd.h>
#include <kernel.h>
#include <mmap.h>
#include <page.h>
#include <pic.h>
#include <pmm.h>
#include <serial.h>
#include <timer.h>
#include <video.h>
#include <vmm.h>

#include <misc/bit_macros.h>
#include <misc/log_macros.h>

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

static GdtEntry      g_gdt[5];
static GdtDescriptor g_gdt_desc;

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
                                       "Control protection exception",
                                       "Hypervisor injection exception",
                                       "VMM communication exception",
                                       "Security exception",
                                       "Reserved",
                                       "Triple fault",
                                       "FPU error interrupt" };

static void init_gdt(void);
static void load_gdt(void);

static void default_interrupt_handler(InterruptInfo *info);
static void page_fault_handler(InterruptInfo *info);
static void default_irq_handler(InterruptInfo *info);

void kernel_init(MemoryMap *mmap, size_t mmap_length)
{
    KSLOG("initializing the global descriptor table\n");
    init_gdt();

    KSLOG("loading the GDT\n");
    load_gdt();

    KSLOG("initializing the Programmable Interrupt Controller\n");
    pic_init(PIC1_OFFSET, PIC2_OFFSET);

    KSLOG("initializing interrupt service routines\n");
    isr_init(0x08);

    KSLOG("mapping default interrupt handler\n");
    for (int i = 0; i < 256; i++)
        isr_map_interrupt_handler(i, default_interrupt_handler);

    isr_map_interrupt_handler(14, page_fault_handler);

    KSLOG("mapping default IRQ handlers\n");
    for (int i = 0; i < 16; i++)
        isr_map_interrupt_handler(i + PIC1_OFFSET, default_irq_handler);

    isr_load_idt();

    KSLOG("initializing hardware peripherals\n");
    timer_init();
    kbd_init();

    KSLOG("enabling interrupts\n");
    core_set_interrupt_flag();

    KSLOG("initializing the physical memory manager\n");
    pmm_init(mmap, mmap_length);

    KSLOG("initializing the virtual memory manager\n");
    vmm_init();
}

void init_gdt()
{
    g_gdt[0] = gdt_make_entry(0x0000, 0x0000, 0x0000, 0x0000);
    g_gdt[1]
        = gdt_make_entry(0xFFFFF,
                         0x0000,
                         GDT_AX_PRESENT | GDT_AX_PRIV_RING0 | GDT_AX_CODE | GDT_AX_EXECUTABLE | GDT_AX_CODE_READABLE,
                         GDT_OF_GRANULARITY | GDT_OF_32BIT);
    g_gdt[2] = gdt_make_entry(0xFFFFF,
                              0x0000,
                              GDT_AX_PRESENT | GDT_AX_PRIV_RING0 | GDT_AX_DATA | GDT_AX_DATA_WRITABLE,
                              GDT_OF_GRANULARITY | GDT_OF_32BIT);
    g_gdt[3]
        = gdt_make_entry(0xFFFFF,
                         0x0000,
                         GDT_AX_PRESENT | GDT_AX_PRIV_RING3 | GDT_AX_CODE | GDT_AX_EXECUTABLE | GDT_AX_CODE_READABLE,
                         GDT_OF_GRANULARITY | GDT_OF_32BIT);
    g_gdt[4] = gdt_make_entry(0xFFFFF,
                              0x0000,
                              GDT_AX_PRESENT | GDT_AX_PRIV_RING3 | GDT_AX_DATA | GDT_AX_DATA_WRITABLE,
                              GDT_OF_GRANULARITY | GDT_OF_32BIT);
}

void load_gdt()
{
    gdt_descriptor_init(&g_gdt_desc, g_gdt, sizeof g_gdt / sizeof g_gdt[0]);
    gdt_descriptor_load(&g_gdt_desc, 0x08, 0x10);
}

void default_interrupt_handler(InterruptInfo *info)
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

static void page_fault_handler(InterruptInfo *info)
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

void default_irq_handler(InterruptInfo *info)
{
    pic_send_eoi(info->vector - PIC1_OFFSET);
}
