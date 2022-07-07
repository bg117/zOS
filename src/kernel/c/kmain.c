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
#include <isr.h>
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

// main
int kmain(uint8_t drive_number, FatInfo *fi, MemoryMap *mmap, uint16_t mmap_length)
{
    screen_clear();

    FatInfo   fat_info;
    MemoryMap memory_map[mmap_length];

    KSLOG("saving structs passed by bootloader\n");
    // save structs because they may get overwritten in the future
    mem_copy(&fat_info, fi, sizeof fat_info);
    mem_copy(memory_map, mmap, sizeof(MemoryMap) * mmap_length);

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
    kernel_init(memory_map, mmap_length);

    //! page fault test
    (void)*(char *)(0x90000000);

    screen_print_string("\nzOS version 0.01\n");
    screen_print_string("Type something:\n\n");

    while (1)
    {
        ReadKey key = kbd_get_char();

        if (key.c == '\b')
            screen_print_string("\b \b");
        else
            screen_print_char(key.c);
    }

    kernel_fin();

    return 1;
}
