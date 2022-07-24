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
#include <kernel/misc/log_macros.h>

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

#define DELAY_READ_STAT(io)                         \
    ({                                              \
        uint8_t __stat__;                           \
        for (int i = 0; i < 15; i++)                \
            __stat__ = in_byte(io + ATA_IO_STATUS); \
                                                    \
        __stat__;                                   \
    })

AtaIdentifyStatus ata_identify(AtaBus bus, AtaDevice dev)
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
    uint8_t stat = DELAY_READ_STAT(port);
    if (stat == 0)
    {
        KSVLOG("error: device does not exist\n");
        return ATA_IDENTIFY_STATUS_NONEXISTENT; // drive doesn't exist
    }

    // wait for BSY to clear
    while (TESTBIT(stat = in_byte(port + ATA_IO_STATUS), ATA_STATUS_BSY))
    {
        // check if lbamid and lbahi are non-zero, that means the drive is not ATA
        uint8_t lbamid = in_byte(port + ATA_IO_LBA_MID), lbahi = in_byte(ATA_PORT_PRIMARY + ATA_IO_LBA_HIGH);
        if (lbamid != 0 || lbahi != 0)
        {
            KSVLOG("error: device is not an ATA device\n");
            return ATA_IDENTIFY_STATUS_NOT_ATA;
        }
    }

    // poll again until DRQ or ERR sets
    while (!(TESTBIT(stat = in_byte(port + ATA_IO_STATUS), ATA_STATUS_DRQ) || TESTBIT(stat, ATA_STATUS_ERR)))
        NOP;

    // ready!
    uint16_t buf[256];
    for (int i = 0; i < sizeof buf / sizeof *buf; i++)
        buf[i] = in_word(port + ATA_IO_DATA);

    KSVLOG("successfully identified device: ata%d-%s\n",
           bus == ATA_BUS_PRIMARY ? 0 : 1,
           dev == ATA_DEVICE_MASTER ? "master" : "slave");
    return ATA_IDENTIFY_STATUS_OK;
}
