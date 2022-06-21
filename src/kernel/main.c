#include <stddef.h>
#include <stdint.h>

#include <root/disk.h>
#include <root/fat.h>
#include <root/i.h>
#include <root/num.h>
#include <root/video.h>

// main
int main(int argc, char **argv)
{
    if (argc != 3) {
        screenWriteFmtString("Wrong amount of arguments passed: %d" NL, argc);
        return -1;
    }

    struct BiosParameterBlock *bpb = CAST(struct BiosParameterBlock **, argv)[1];
    struct ExtendedBootRecord *ebr = CAST(struct ExtendedBootRecord **, argv)[2];
    struct Disk                disk;

    const int i = 100, j = 254;

    diskGetInfo(&disk, ebr->driveNumber);

    screenClear();
    screenWriteString("Loaded kernel." NL);
    screenWriteFmtString("Test for %s functions: " NL, NAME(screenWriteFmtString));

    screenWriteFmtString("    %xh %dd %oo \"%s\" %lloo" NL, i, i, i, "foo", 0xFFF27D);
    screenWriteFmtString("    %xh %dd %oo \"%s\" %llxh" NL NL, j, j, j, "bar", 0xCAB01F);

    screenWriteFmtString("Booting from drive index: %hhu" NL, disk.index);
    return 1;
}
