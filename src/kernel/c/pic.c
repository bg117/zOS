/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <syslvl/io.h>
#include <syslvl/pic.h>

#define PIC1_CMD 0x20
#define PIC2_CMD 0xA0
#define PIC1_DAT 0x21
#define PIC2_DAT 0xA1

#define PIC_EOI 0x20

#define UNUSED_PORT 0x80

// PIC ICW values
#define ICW1_ICW4 0x01
#define ICW1_INIT 0x10

#define ICW3_MASTER_LINE 0x04
#define ICW3_SLAVE_LINE  0x02

#define ICW4_8086 0x01 // 8086 mode

#define IOWAIT() out_byte(UNUSED_PORT, 0)

static uint8_t _pic1_offset;
static uint8_t _pic2_offset;

void pic_init(uint8_t master_offset, uint8_t slave_offset)
{
    _pic1_offset = master_offset;
    _pic2_offset = slave_offset;

    out_byte(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    IOWAIT();

    out_byte(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
    IOWAIT();

    out_byte(PIC1_DAT, master_offset);
    IOWAIT();

    out_byte(PIC2_DAT, slave_offset);
    IOWAIT();

    out_byte(PIC1_DAT, ICW3_MASTER_LINE);
    IOWAIT();

    out_byte(PIC2_DAT, ICW3_SLAVE_LINE);
    IOWAIT();

    out_byte(PIC1_DAT, ICW4_8086);
    IOWAIT();

    out_byte(PIC2_DAT, ICW4_8086);
    IOWAIT();

    out_byte(PIC1_DAT, 0xFC);
    out_byte(PIC2_DAT, 0xFF);
}

void pic_send_eoi(uint8_t irq)
{
    if (irq >= 8)
        out_byte(PIC2_CMD, PIC_EOI);

    out_byte(PIC1_CMD, PIC_EOI);
}

uint8_t pic_get_pic1_offset()
{
    return _pic1_offset;
}

uint8_t pic_get_pic2_offset()
{
    return _pic2_offset;
}
