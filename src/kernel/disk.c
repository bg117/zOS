#include <root/disk.h>

extern int __diskReset_internal_impl(uint8_t index);

extern int __diskGetInfo_internal_impl(uint8_t   index,
                                       uint8_t  *driveType,
                                       uint16_t *cylinders,
                                       uint8_t  *heads,
                                       uint8_t  *sectors);

int diskGetInfo(struct Disk *disk, uint8_t index)
{
    disk->Index = index;
    return __diskGetInfo_internal_impl(disk->Index,
                                       &disk->Type,
                                       &disk->Cylinders,
                                       &disk->Heads,
                                       &disk->Sectors);
}

int diskReset(struct Disk *disk)
{
    return __diskReset_internal_impl(disk->Index);
}
