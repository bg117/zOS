/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <syslvl/gdt.h>
#include <syslvl/hal.h>
#include <syslvl/idt.h>

static struct gdt_entry      _gdt[3];
static struct gdt_descriptor _gdt_desc;

static struct idt_entry      _idt[256];
static struct idt_descriptor _idt_desc;

static void _halinitgdt(void);
static void _halinitidt(void);

void halinit(void)
{
    _halinitgdt();
    gdtdescld(&_gdt_desc, 0x08, 0x10);

    _halinitidt();
    // idtdescld(...)
}

void _halinitgdt()
{
    _gdt[0] = gdtmkentry(0x0000, 0x0000, 0x0000, 0x0000);
    _gdt[1] = gdtmkentry(0xFFFFF,
                         0x0000,
                         GDT_AX_PRESENT | GDT_AX_PRIV_RING0 | GDT_AX_CODE
                             | GDT_AX_EXECUTABLE | GDT_AX_CODE_READABLE,
                         GDT_OF_GRANULARITY | GDT_OF_32BIT);
    _gdt[2] = gdtmkentry(0xFFFFF,
                         0x0000,
                         GDT_AX_PRESENT | GDT_AX_PRIV_RING0 | GDT_AX_DATA
                             | GDT_AX_DATA_WRITABLE,
                         GDT_OF_GRANULARITY | GDT_OF_32BIT);

    gdtdescinit(&_gdt_desc, _gdt, sizeof _gdt / sizeof *_gdt);
}

void _halinitidt()
{
}
