#ifndef quuxFATHxuuq
#define quuxFATHxuuq

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct __attribute__((packed)) BiosParameterBlock {
    char    *oem;
    uint16_t bytesPerSector;
    uint8_t  sectorsPerCluster;
    uint16_t reservedSectors;
    uint8_t  fats;
    uint16_t rootDirEntries;
    uint16_t totalSectors;
    uint8_t  mediaDescriptorByte;
    uint16_t sectorsPerFat;
    uint16_t sectorsPerTrack;
    uint16_t heads;
    uint32_t hiddenSectors;
    uint32_t largeSectors;
};

struct __attribute__((packed)) ExtendedBootRecord {
    uint8_t  driveNumber;
    uint8_t  ntFlags;
    uint8_t  signature;
    uint32_t volumeSerialNumber;
    char    *volumeLabel;
    char    *systemIdentifier;
};

#ifdef __cplusplus
}
#endif

#endif
