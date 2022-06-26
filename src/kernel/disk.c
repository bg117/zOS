/**
 * Copyright (c) 2022 iusearchbtw
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <syslvl/disk.h>
#include <syslvl/i.h>

// extern int __diskReset_internal_impl(uint8_t index);

// extern int __diskGetInfo_internal_impl(uint8_t   index,
//                                        uint8_t  *driveType,
//                                        uint16_t *cylinders,
//                                        uint8_t  *heads,
//                                        uint8_t  *sectors);

int diskGetInfo(struct Disk *disk, uint8_t index)
{
    USE(disk);
    USE(index);
    // disk->Index = index;
    // return __diskGetInfo_internal_impl(disk->Index,
    //                                    &disk->Type,
    //                                    &disk->Cylinders,
    //                                    &disk->Heads,
    //                                    &disk->Sectors);
    return 1;
}

int diskReset(struct Disk *disk)
{
    USE(disk);
    // return __diskReset_internal_impl(disk->Index);
    return 1;
}
