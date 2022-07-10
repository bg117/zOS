/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stddef.h>
#include <stdint.h>

#include <kernel.h>

#include <fs/fat.h>

#include <memory/mem.h>
#include <memory/memdefs.h>
#include <memory/mmap.h>
#include <memory/vmm.h>

#include <hw/kbd.h>
#include <hw/serial.h>
#include <hw/video.h>

#include <misc/log_macros.h>

// main
int kmain(uint8_t drive_number, FatInfo *fi, MemoryMap *mmap, uint16_t mmap_length)
{
    screen_clear();

    screen_print_string("---------------- MEMORY MAP ----------------\n");
    screen_print_format_string("%hu %s in the memory map\n", mmap_length, mmap_length > 1 ? "entries" : "entry");
    for (int i = 0; i < mmap_length; i++)
    {
        screen_print_format_string("%d: Base=0x%016llX, Length=0x%016llX, Type=%u, AcpiExtAttrs=%u\n",
                                   i,
                                   mmap[i].base,
                                   mmap[i].length,
                                   mmap[i].type,
                                   mmap[i].acpi_extended_attributes);
    }

    KSLOG("initializing the kernel\n");
    kernel_init(mmap, mmap_length);

    // VMM test
    unsigned int *p = vmm_allocate_page();
    *p              = 42; // IT ACTUALLY WORKS!!!
    screen_print_format_string("%u\n", *p);
    vmm_free_page(p);

    //! page fault test
    char *pf = (char *)(0x8765ABCD);
    *pf      = 'A';

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

    return 1;
}
