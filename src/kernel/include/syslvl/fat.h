/**
 * Copyright (c) 2022 iusearchbtw
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxFATHxuuq
#define quuxFATHxuuq

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct __attribute__((packed)) FatInfo
{
    struct __attribute__((packed)) BiosParameterBlock
    {
        char     Oem[8];
        uint16_t BytesPerSector;
        uint8_t  SectorsPerCluster;
        uint16_t ReservedSectors;
        uint8_t  Fats;
        uint16_t RootDirEntries;
        uint16_t TotalSectors;
        uint8_t  MediaDescriptorByte;
        uint16_t SectorsPerFat;
        uint16_t SectorsPerTrack;
        uint16_t Heads;
        uint32_t HiddenSectors;
        uint32_t LargeSectors;
    } Bpb;

    struct __attribute__((packed)) ExtendedBootRecord
    {
        uint8_t  DriveNumber;
        uint8_t  NtFlags;
        uint8_t  Signature;
        uint32_t VolumeSerialNumber;
        char     VolumeLabel[11];
        char     SystemIdentifier[8];
    } Ebr;

    uint16_t Tracks;
};

#ifdef __cplusplus
}
#endif

#endif
