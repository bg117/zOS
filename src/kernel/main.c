#include <stddef.h>
#include <stdint.h>
#include "include/disk.h"
#include "include/fat.h"
#include "include/num.h"
#include "include/video.h"

#define NL          "\r\n"
#define _CAST(a, x) ((a)(x))

// main
int main(int argc, char **argv)
{
    if (argc != 3) {
        screenWriteFmtString("Wrong amount of arguments passed: %d" NL, argc);
        return -1;
    }

    uint8_t                    driveNumber = _CAST(uint8_t *, argv)[0];
    struct BiosParameterBlock *realBpb     = _CAST(struct BiosParameterBlock **, argv)[1];
    struct ExtendedBootRecord *realEbr     = _CAST(struct ExtendedBootRecord **, argv)[2];
    struct Disk                disk;

    char number[30];
    int  i = 100;


    diskGetInfo(&disk, driveNumber);

    screenClear();

    intToString(number, i, 10);
    screenWriteString(number);

    screenWriteString("Loaded kernel." NL);
    screenWriteFmtString("Booting from drive index: %hhu" NL, disk.index);
    screenWriteFmtString("%s" NL, realEbr->systemIdentifier);
    return 1;
}
