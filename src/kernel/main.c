/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#pragma region Includes

#include <stddef.h>
#include <stdint.h>

#include <syslvl/disk.h>
#include <syslvl/fat.h>
#include <syslvl/gdt.h>
#include <syslvl/idt.h>
#include <syslvl/mem.h>
#include <syslvl/num.h>
#include <syslvl/video.h>

#include <misc/define.h>

#pragma endregion

#pragma region Function declarations

static void dumpgdt(void);

#pragma endregion

#pragma region Global variables

static struct gdt_entry      _gdt[3];
static struct gdt_descriptor _gdt_desc;

static struct idt_entry      _idt[256];
static struct idt_descriptor _idt_desc;

static volatile int _lock = 1;

#pragma endregion

// main
int main(int argc, char **argv)
{
    scrclr();
    scrputs("Loaded kernel." NL);

    if (argc != 2)
    {
        scrputsf("Wrong amount of arguments passed: %d" NL, argc);
        return -1;
    }

    scrputs(NL);

    struct fat_info fi;
    uint8_t         driveNumber;

    // works!
    memcopy(&fi, argv[1], sizeof fi);
    memcopy(&driveNumber, &argv[0], sizeof driveNumber);

    scrputsf("Booting from drive index: %hhu" NL NL, driveNumber);

    scrputs("Creating new GDT entries..." NL);
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

    scrputs("Loading new GDT..." NL NL);
    gdtdescinit(&_gdt_desc, _gdt, sizeof _gdt / sizeof *_gdt);
    gdtdescld(&_gdt_desc, 0x08, 0x10);

    dumpgdt();

    while (_lock)
        ;

    return 1;
}

#pragma region Function definitions

void dumpgdt()
{
    for (int i = 0; i < sizeof _gdt / sizeof *_gdt; i++)
    {
        uint8_t tmpFlags = _gdt[i].flags_limit_upper_4 & 0xF0;

        scrputsf(
            "GDT index #%d: Base=%xh, Limit=%xh, AccessByte=%hhbb, "
            "Flags=%hhbb" NL,
            i,
            _gdt[i].base_lower_16 | _gdt[i].base_middle_8 << 16
                | _gdt[i].base_upper_8 << 24,
            _gdt[i].limit_lower_16 | (_gdt[i].flags_limit_upper_4 & 0xF) << 16,
            _gdt[i].access_byte,
            tmpFlags >> 4);
    }
}

#pragma endregion
