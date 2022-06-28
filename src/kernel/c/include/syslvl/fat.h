/**
 * Copyright (c) 2022 bg117
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

struct __attribute__((packed)) fat_info
{
    struct __attribute__((packed)) bios_parameter_block
    {
        char     oem[8];
        uint16_t bytes_per_sector;
        uint8_t  sectors_per_cluster;
        uint16_t reserved_sectors;
        uint8_t  fats;
        uint16_t root_directory_entries;
        uint16_t total_sectors;
        uint8_t  media_descriptor_byte;
        uint16_t sectors_per_fat;
        uint16_t sectors_per_track;
        uint16_t heads;
        uint32_t hidden_sectors;
        uint32_t large_sectors;
    } bios_parameter_block;

    struct __attribute__((packed)) extended_boot_record
    {
        uint8_t  drive_number;
        uint8_t  winnt_flags;
        uint8_t  signature;
        uint32_t volume_serial_number;
        char     volume_label[11];
        char     system_identifier[8];
    } extended_boot_record;
};

#ifdef __cplusplus
}
#endif

#endif
