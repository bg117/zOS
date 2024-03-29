/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <kernel/hw/atapio.h>
#include <kernel/hw/serial.h>
#include <kernel/hw/video.h>
#include <kernel/ll/io.h>
#include <kernel/misc/bit_macros.h>
#include <kernel/misc/log.h>
#include <stddef.h>

#define ATA_CMD_IDENTIFY 0xEC

#define ATA_PORT_PRIMARY   0x1F0
#define ATA_PORT_SECONDARY 0x170

#define ATA_IO_SELECT       0x06
#define ATA_IO_SECTOR_COUNT 0x02
#define ATA_IO_LBA_LOW      0x02
#define ATA_IO_LBA_MID      0x03
#define ATA_IO_LBA_HIGH     0x05
#define ATA_IO_CMD          0x07
#define ATA_IO_STATUS       0x07
#define ATA_IO_DATA         0x00

#define NOP __asm__ volatile("" ::: "memory")

enum ata_status
{
    ATA_STATUS_BSY  = 0x80,
    ATA_STATUS_DRDY = 0x40,
    ATA_STATUS_DWF  = 0x20,
    ATA_STATUS_DSC  = 0x10,
    ATA_STATUS_DRQ  = 0x08,
    ATA_STATUS_ERR  = 0x01
};
uint8_t delay_400ns(int io);

AtaIdentifyStatus ata_identify(AtaBus bus, AtaDevice dev, AtaInfo *info)
{
    int port = ATA_PORT_PRIMARY;
    // select drive
    if (bus == ATA_BUS_PRIMARY)
    {
        if (dev == ATA_DEVICE_MASTER)
            out_byte(ATA_PORT_PRIMARY + ATA_IO_SELECT /* 0x1F6, the "drive select" port */, 0xA0);
        else
            out_byte(ATA_PORT_PRIMARY + ATA_IO_SELECT, 0xB0);
    }
    else
    {
        port = ATA_PORT_SECONDARY;
        if (dev == ATA_DEVICE_MASTER)
            out_byte(ATA_PORT_SECONDARY + ATA_IO_SELECT, 0xA0);
        else
            out_byte(ATA_PORT_SECONDARY + ATA_IO_SELECT, 0xB0);
    }

    // send 0 to sectorcount, lbalo, lbamid, lbahi
    out_byte(port + ATA_IO_SECTOR_COUNT, 0);
    out_byte(port + ATA_IO_LBA_LOW, 0);
    out_byte(port + ATA_IO_LBA_MID, 0);
    out_byte(port + ATA_IO_LBA_HIGH, 0);

    // send IDENTIFY to command
    out_byte(port + ATA_IO_CMD, ATA_CMD_IDENTIFY);

    // read status port
    uint8_t stat = delay_400ns(port);
    if (stat == 0)
    {
        log_all(LOG_ERR, "device does not exist\n");
        return ATA_IDENTIFY_STATUS_NONEXISTENT; // drive doesn't exist
    }

    // wait for BSY to clear
    while (TESTBIT(stat = in_byte(port + ATA_IO_STATUS), ATA_STATUS_BSY))
    {
        // check if lbamid and lbahi are non-zero, that means the drive is not ATA
        uint8_t lbamid = in_byte(port + ATA_IO_LBA_MID), lbahi = in_byte(ATA_PORT_PRIMARY + ATA_IO_LBA_HIGH);
        if (lbamid != 0 || lbahi != 0)
        {
            log_all(LOG_ERR, "error: device is not an ATA device\n");
            return ATA_IDENTIFY_STATUS_NOT_ATA;
        }
    }

    // poll again until DRQ or ERR sets
    while (!(TESTBIT(stat, ATA_STATUS_DRQ) || TESTBIT(stat, ATA_STATUS_ERR)))
        stat = in_byte(port + ATA_IO_STATUS);

    // ready!
    uint16_t buf[256];
    for (size_t i = 0; i < sizeof buf / sizeof *buf; i++)
        buf[i] = in_word(port + ATA_IO_DATA);

    log_all(LOG_INFO,
            "successfully identified device: ata%d-%s\n",
            bus == ATA_BUS_PRIMARY ? 0 : 1,
            dev == ATA_DEVICE_MASTER ? "master" : "slave");

    info->is_hard_disk        = TESTBIT(buf[0], 0);
    info->supports_lba48      = TESTBIT(buf[83], 1 << 10);
    info->lba28_total_sectors = ((uint32_t *)buf)[60 / 2];
    info->lba48_total_sectors = ((uint64_t *)buf)[100 / 4];
    info->bus                 = bus;
    info->device              = dev;
    return ATA_IDENTIFY_STATUS_OK;
}

uint8_t delay_400ns(int io)
{
    uint8_t stat;
    // spec says to read 15 times from status port and only care about the last one
    for (size_t i = 0; i < 15; i++)
        stat = in_byte(io + ATA_IO_STATUS);

    return stat;
}
