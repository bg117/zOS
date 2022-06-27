/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stddef.h>
#include <stdint.h>

#include <syslvl/disk.h>
#include <syslvl/fat.h>
#include <syslvl/gdt.h>
#include <syslvl/i.h>
#include <syslvl/mem.h>
#include <syslvl/num.h>
#include <syslvl/video.h>
#include "include/syslvl/gdt.h"

static void hexDump(const void *ptr, size_t n);
static void testVga(void);
static void dumpGdt(void);

static struct GdtDescriptor _gdtDesc;
static struct GdtEntry      _gdt[3];
static volatile int         _lock = 1;

// main
int main(int argc, char **argv)
{
    screenClear();
    screenWriteString("Loaded kernel." NL);

    if (argc != 2)
    {
        screenWriteFmtString("Wrong amount of arguments passed: %d" NL, argc);
        return -1;
    }

    struct FatInfo fi;
    struct Disk    disk;

    screenWriteString(NL);

    // works!
    memoryCopy(&fi, argv[1], sizeof fi);

    testVga();
    screenWriteFmtString("BPB located at: %p" NL "EBR located at: %p" NL NL,
                         CAST(void *, &fi.Bpb),
                         CAST(void *, &fi.Ebr));
    hexDump(&fi, sizeof fi);
    screenWriteFmtString("Booting from drive index: %hhu" NL NL, disk.Type);

    screenWriteString("Creating new GDT entries..." NL);
    _gdt[0] = gdtCreateEntry(0x0000, 0x0000, 0x0000, 0x0000);
    _gdt[1] = gdtCreateEntry(0xFFFFF,
                             0x0000,
                             GDT_AX_PRESENT | GDT_AX_PRIV_RING0 | GDT_AX_CODE
                                 | GDT_AX_EXECUTABLE | GDT_AX_CODE_READABLE,
                             GDT_OF_GRANULARITY | GDT_OF_32BIT);
    _gdt[2] = gdtCreateEntry(0xFFFFF,
                             0x0000,
                             GDT_AX_PRESENT | GDT_AX_PRIV_RING0 | GDT_AX_DATA
                                 | GDT_AX_DATA_WRITABLE,
                             GDT_OF_GRANULARITY | GDT_OF_32BIT);

    screenWriteString("Loading new GDT..." NL NL);
    gdtInitializeDescriptor(&_gdtDesc, _gdt, sizeof _gdt / sizeof *_gdt);
    gdtLoadDescriptor(&_gdtDesc, 0x08, 0x10);

    dumpGdt();

    while (_lock)
        ;

    return 1;
}

void hexDump(const void *ptr, size_t n)
{
    const int      WIDTH = 16;
    const uint8_t *blk   = ptr;

    size_t i = 0;

    for (i = 0; i < n; i++)
    {
        // hex representation
        screenWriteFmtString("%s%hhx ", blk[i] < 0x10 ? "0" : "", blk[i]);
        if (i % WIDTH == WIDTH - 1)
            screenWriteString(NL);
    }

    if (i % WIDTH != WIDTH - 1)
        screenWriteString(NL);
}

void testVga(void)
{
    screenWriteFmtString("Test for %s functions: " NL,
                         NAME(screenWriteFmtString));

    screenWriteFmtString("    %xh %dd %oo \"%s\" '%c' %lloo %lbb" NL,
                         100,
                         100,
                         100,
                         "foo",
                         'B',
                         CAST(long long, 07001342),
                         GDT_AX_PRESENT | GDT_AX_PRIV_RING1
                             | GDT_AX_CODE_CONFORMING);
    screenWriteFmtString("    %xh %dd %oo \"%s\" '%c' %hhxh %lbb" NL NL,
                         0xFE,
                         0xFE,
                         0xFE,
                         "bar",
                         'A',
                         CAST(char, 0xE0),
                         GDT_AX_ACCESSED);
}

void dumpGdt()
{
    for (int i = 0; i < sizeof _gdt / sizeof *_gdt; i++)
    {
        screenWriteFmtString(
            "GDT index #%d: Base=0x%x, Limit=0x%x, AccessByte=0x%x, "
            "Flags=0x%x" NL,
            i,
            _gdt[i].BaseLower16 | _gdt[i].BaseMiddle8 << 16
                | _gdt[i].BaseUpper8 << 24,
            _gdt[i].LimitLower16 | (_gdt[i].FlagsAndLimitUpper4 & 0xF) << 16,
            _gdt[i].AccessByte,
            _gdt[i].FlagsAndLimitUpper4 & 0xF0 >> 4);
    }
}
