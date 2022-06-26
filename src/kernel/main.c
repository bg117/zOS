/**
 * Copyright (c) 2022 iusearchbtw
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

void hexDump(const void *ptr, size_t n);

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

    screenWriteString(NL);

    struct FatInfo *fi = CAST(struct FatInfo **, argv)[1];
    struct Disk     disk;

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

    screenWriteFmtString("BPB located at: %p" NL "EBR located at: %p" NL NL,
                         (void *)&fi->Bpb,
                         (void *)&fi->Ebr);

    hexDump(fi, sizeof *fi);

    screenWriteFmtString("Booting from drive index: %hhu" NL, disk.Type);

    if (!diskGetInfo(&disk, fi->Ebr.DriveNumber))
    {
        screenWriteFmtString(
            "Unable to obtain parameters from drive: %hhu. Aborting...",
            fi->Ebr.DriveNumber);
        return -2;
    }

    screenWriteFmtString("With parameters:" NL "    cyl: %u" NL
                         "    head: %hhu" NL "    sec: %hhu" NL,
                         disk.Cylinders,
                         disk.Heads,
                         disk.Sectors);

    screenWriteFmtString("%c", fi->Bpb.Oem[0]);

    return 1;
}

void hexDump(const void *ptr, size_t n)
{
    uint8_t  *blk   = CAST(uint8_t *, ptr);
    const int WIDTH = 8;
    size_t    i     = 0;
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
