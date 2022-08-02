/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <kernel/hw/atapio.h>
#include <kernel/hw/kbd.h>
#include <kernel/hw/serial.h>
#include <kernel/hw/timer.h>
#include <kernel/hw/video.h>
#include <kernel/int/idt.h>
#include <kernel/int/interrupt_info.h>
#include <kernel/int/isr.h>
#include <kernel/int/pic.h>
#include <kernel/kernel.h>
#include <kernel/ll/core.h>
#include <kernel/ll/gdt.h>
#include <kernel/memory/heap.h>
#include <kernel/memory/memdefs.h>
#include <kernel/memory/mmap.h>
#include <kernel/memory/page.h>
#include <kernel/memory/pmm.h>
#include <kernel/memory/vmm.h>
#include <kernel/misc/bit_macros.h>
#include <kernel/misc/log.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <utils/strings.h>

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

extern PageDirectoryEntry SYS_PGDIR;

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

// static const char *ATA_STATUS_STRINGS[] = { "Success", "Drive non-existent", "Drive not ATA" };

static void init_gdt(void);
static void load_gdt(void);

static void default_interrupt_handler(InterruptInfo *info);
static void page_fault_handler(InterruptInfo *info);
static void default_irq_handler(InterruptInfo *info);

void kernel_init(MemoryMapEntry *mmap, size_t mmap_length)
{
    log_all(LOG_INFO, "initializing the global descriptor table\n");
    init_gdt();

    log_all(LOG_INFO, "loading the GDT\n");
    load_gdt();

    log_all(LOG_INFO, "initializing the physical memory manager\n");
    pmm_init(mmap, mmap_length);

    log_all(LOG_INFO, "initializing the virtual memory manager\n");
    vmm_init((PhysicalAddress)&SYS_PGDIR - 0xC0000000, (VirtualAddress)&SYS_PGDIR);

    log_all(LOG_INFO, "initializing the kernel heap\n");
    heap_init(8192);

    log_all(LOG_INFO, "initializing the Programmable Interrupt Controller\n");
    pic_init(PIC1_OFFSET, PIC2_OFFSET);

    log_all(LOG_INFO, "initializing interrupt service routines\n");
    isr_init(0x08);

    log_all(LOG_INFO, "mapping default interrupt handler\n");
    for (int i = 0; i < 256; i++)
        isr_map_interrupt_handler(i, default_interrupt_handler);

    isr_map_interrupt_handler(14, page_fault_handler);

    log_all(LOG_INFO, "mapping default IRQ handlers\n");
    for (int i = 0; i < 16; i++)
        isr_map_interrupt_handler(i + PIC1_OFFSET, default_irq_handler);

    isr_load_idt();

    log_all(LOG_INFO, "initializing hardware peripherals\n");
    timer_init();
    kbd_init();

    log_all(LOG_INFO, "enabling interrupts\n");
    core_set_interrupt_flag();

    log_all(LOG_INFO, "identifying ata0-master\n");

    // for the meantime we'll have to trust that the boot disk is ata0-master
    AtaInfo           ata_info;
    AtaIdentifyStatus stat = ata_identify(ATA_BUS_PRIMARY, ATA_DEVICE_MASTER, &ata_info);
    if (stat != ATA_IDENTIFY_STATUS_OK)
    {
        PANIC("ata0-master identification failed\n");
        return;
    }

    // log ATA information
    log_all(LOG_INFO,
            "ata0-master:\n\tIs hard disk: %s\n\tSupports LBA48: %s\n\tTotal LBA28 sectors: %u\n\tTotal LBA48 sectors: "
            "%llu\n",
            ata_info.is_hard_disk ? "yes" : "no",
            ata_info.supports_lba48 ? "yes" : "no",
            ata_info.lba28_total_sectors,
            ata_info.lba48_total_sectors);
}

void kernel_deinit(void)
{
    log_all(LOG_INFO, "deinitializing the keyboard\n");
    kbd_deinit();

    log_all(LOG_INFO, "deinitializing the PIT\n");
    timer_deinit();
}

void init_gdt(void)
{
    g_gdt[0] = gdt_make_entry(0x0000, 0x0000, 0x0000, 0x0000);
    g_gdt[1] =
        gdt_make_entry(0xFFFFF,
                       0x0000,
                       GDT_AX_PRESENT | GDT_AX_PRIV_RING0 | GDT_AX_CODE | GDT_AX_EXECUTABLE | GDT_AX_CODE_READABLE,
                       GDT_OF_GRANULARITY | GDT_OF_32BIT);
    g_gdt[2] = gdt_make_entry(0xFFFFF,
                              0x0000,
                              GDT_AX_PRESENT | GDT_AX_PRIV_RING0 | GDT_AX_DATA | GDT_AX_DATA_WRITABLE,
                              GDT_OF_GRANULARITY | GDT_OF_32BIT);
    g_gdt[3] =
        gdt_make_entry(0xFFFFF,
                       0x0000,
                       GDT_AX_PRESENT | GDT_AX_PRIV_RING3 | GDT_AX_CODE | GDT_AX_EXECUTABLE | GDT_AX_CODE_READABLE,
                       GDT_OF_GRANULARITY | GDT_OF_32BIT);
    g_gdt[4] = gdt_make_entry(0xFFFFF,
                              0x0000,
                              GDT_AX_PRESENT | GDT_AX_PRIV_RING3 | GDT_AX_DATA | GDT_AX_DATA_WRITABLE,
                              GDT_OF_GRANULARITY | GDT_OF_32BIT);
}

void kernel_panic(const char *file, int line, const char *fmt, ...)
{
    screen_print_string("--------------- KERNEL PANIC ---------------\n");
    bool nomsg = str_compare(fmt, EMPTY_PANIC_MSG) == 0;

    log_all(LOG_ERR, "kernel panic thrown at %s:%d\n", file, line);
    if (nomsg)
    {
        core_clear_interrupt_flag();
        core_halt();
    }

    va_list ap;
    va_start(ap, fmt);

    size_t str = str_vformat(fmt, NULL, ap);
    char   buf[str];

    str_vformat(fmt, buf, ap);
    log_all(LOG_INFO, "%s\n", buf);

    core_clear_interrupt_flag();
    core_halt();
}

void load_gdt(void)
{
    gdt_descriptor_init(&g_gdt_desc, g_gdt, sizeof g_gdt / sizeof g_gdt[0]);
    gdt_descriptor_load(&g_gdt_desc, 0x08, 0x10);
}

void default_interrupt_handler(InterruptInfo *info)
{
    screen_print_string("--------------- SYSTEM ERROR ---------------\n");
    log_all(LOG_ERR,
            "unhandled interrupt 0x%02X (%s). Error code: 0x%02X\n",
            info->vector,
            info->vector < 32 ? EXCEPTION_IDS[info->vector] : "unknown",
            info->error_code);
    log_all(LOG_INFO,
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

    core_clear_interrupt_flag();
    core_halt();
}

static void page_fault_handler(InterruptInfo *info)
{
    uint32_t cr2;
    __asm__ volatile("movl %%cr2, %0" : "=r"(cr2) :); // get value of CR2 (contains the address of page fault)

    screen_print_string("---------------- PAGE FAULT ----------------\n");
    log_all(LOG_ERR, "page fault at address 0x%08X. Error code: 0x%X\n", cr2, info->error_code);
    log_all(LOG_INFO,
            "register dump:\n\t"
            "eax=0x%08X ebx=0x%08X ecx=0x%08X edx=0x%08X\n\t"
            "esi=0x%08X edi=0x%08X ebp=0x%08X esp=0x%08X\n\t"
            "eip=0x%08X cs=0x%04X      ds=0x%04X      ss=0x%04X\n",
            info->eax,
            info->ebx,
            info->ecx,
            info->edx,
            info->esi,
            info->edi,
            info->ebp,
            info->esp,
            info->eip,
            info->cs,
            info->ds,
            info->ss);

    // temporary
    core_clear_interrupt_flag();
    core_halt();
}

void default_irq_handler(InterruptInfo *info)
{
    pic_send_eoi(info->vector - PIC1_OFFSET);
}
