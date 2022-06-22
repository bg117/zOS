#include <stddef.h>
#include <stdint.h>

#include <root/disk.h>
#include <root/fat.h>
#include <root/i.h>
#include <root/num.h>
#include <root/video.h>

extern void *memset(void *ptr, int c, size_t n);

// main
int main(int argc, char **argv)
{
    screenClear();
    screenWriteString("Loaded kernel." NL);

    if (argc != 3)
    {
        screenWriteFmtString("Wrong amount of arguments passed: %d" NL, argc);
        return -1;
    }

    screenWriteString(NL);

    struct BiosParameterBlock *bpb
        = CAST(struct BiosParameterBlock **, argv)[1];
    struct ExtendedBootRecord *ebr
        = CAST(struct ExtendedBootRecord **, argv)[2];
    struct Disk disk;

    screenWriteFmtString("Test for %s functions: " NL,
                         NAME(screenWriteFmtString));

    screenWriteFmtString("    %xh %dd %oo \"%s\" '%c' %lloo %lbb" NL,
                         100,
                         100,
                         100,
                         "foo",
                         'B',
                         (long long)07001342,
                         (long)257);
    screenWriteFmtString("    %xh %dd %oo \"%s\" '%c' %llxh %lbb" NL NL,
                         0xFE,
                         0xFE,
                         0xFE,
                         "bar",
                         'A',
                         (long long)0xCAB01F,
                         (long)0xF);

    screenWriteFmtString(
        "BPB located at: %p" NL "EBR located at: %p" NL NL, bpb, ebr);

    screenWriteFmtString("Booting from drive index: %hhu" NL, disk.Type);

    if (!diskGetInfo(&disk, ebr->DriveNumber))
    {
        screenWriteFmtString(
            "Unable to obtain parameters from drive: %hhu. Aborting...",
            ebr->DriveNumber);
        return -2;
    }

    screenWriteFmtString("With parameters:" NL "    cyl: %hhu" NL
                         "    head: %hhu" NL "    sec: %hhu" NL,
                         disk.Cylinders,
                         disk.Heads,
                         disk.Sectors);

    screenWriteFmtString("%s", bpb->Oem);

    return 1;
}
