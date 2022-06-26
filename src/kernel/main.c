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
#include <syslvl/i.h>
#include <syslvl/num.h>
#include <syslvl/video.h>
#include "syslvl/gdt.h"
#include "syslvl/mem.h"

static void hexDump(const void *ptr, size_t n);
static void testVga(void);

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
    screenWriteFmtString("Booting from drive index: %hhu" NL, disk.Type);

    return 1;
}

void hexDump(const void *ptr, size_t n)
{
    const int      WIDTH = 8;
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
                             | GDT_AX_CODE_DIRECTION);
    screenWriteFmtString("    %xh %dd %oo \"%s\" '%c' %hhxh %lbb" NL NL,
                         0xFE,
                         0xFE,
                         0xFE,
                         "bar",
                         'A',
                         CAST(char, 0xE0),
                         GDT_AX_ACCESSED);
}
