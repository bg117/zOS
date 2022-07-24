/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxATAPIOHxuuq
#define quuxATAPIOHxuuq

#include <stdbool.h>
#include <stdint.h>

typedef enum ata_identify_status
{
    ATA_IDENTIFY_STATUS_OK,
    ATA_IDENTIFY_STATUS_NONEXISTENT,
    ATA_IDENTIFY_STATUS_NOT_ATA
} AtaIdentifyStatus;

typedef enum ata_bus
{
    ATA_BUS_PRIMARY,
    ATA_BUS_SECONDARY
} AtaBus;

typedef enum ata_device
{
    ATA_DEVICE_MASTER,
    ATA_DEVICE_SLAVE
} AtaDevice;

typedef struct ata_info
{
    bool     is_hard_disk;
    bool     supports_lba48;
    uint32_t lba28_total_sectors;
    uint64_t lba48_total_sectors;
    int      bus;
    int      device;
} AtaInfo;

/**
 * @brief Identifies an ATA device.
 *
 * @param bus The bus of the device (primary or secondary).
 * @param dev The device (master or slave).
 * @param info A pointer to a AtaInfo struct to store the information.
 * @return The status of the identify operation.
 */
AtaIdentifyStatus ata_identify(AtaBus bus, AtaDevice dev, AtaInfo *info);

#endif
