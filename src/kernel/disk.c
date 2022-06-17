#include "include/disk.h"

struct Disk {
    uint8_t index;

    uint16_t cylinders;
    uint16_t heads;
    uint16_t sectors;
};

__declspec(__cdecl) struct Disk *getDisk(uint8_t index)
{
    struct Disk disk;

    disk.index     = index;
    disk.cylinders = 0;
    disk.heads     = 0;
    disk.sectors   = 0;

    return &disk;
}
