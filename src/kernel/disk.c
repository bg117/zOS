#include "include/disk.h"

void diskGetInfo(struct Disk *disk, uint8_t index)
{
    disk->index     = index;
    disk->cylinders = 0;
    disk->heads     = 0;
    disk->sectors   = 0;
}
