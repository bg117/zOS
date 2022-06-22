#ifndef quuxFATHxuuq
#define quuxFATHxuuq

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct __attribute__((packed)) BiosParameterBlock
{
    char    *Oem;
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
};

struct __attribute__((packed)) ExtendedBootRecord
{
    uint8_t  DriveNumber;
    uint8_t  NtFlags;
    uint8_t  Signature;
    uint32_t VolumeSerialNumber;
    char    *VolumeLabel;
    char    *SystemIdentifier;
};

#ifdef __cplusplus
}
#endif

#endif
