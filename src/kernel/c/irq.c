/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>
#include <syslvl/io.h>
#include <syslvl/irq.h>

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

#define IOWAIT() outb(UNUSED_PORT, 0)

void irqinit(uint8_t master_offset, uint8_t slave_offset)
{
    uint8_t master = inb(PIC1_DAT);
    uint8_t slave  = inb(PIC2_DAT);

    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    IOWAIT();

    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
    IOWAIT();

    outb(PIC1_DAT, master_offset);
    IOWAIT();

    outb(PIC2_DAT, slave_offset);
    IOWAIT();

    outb(PIC1_DAT, ICW3_MASTER_LINE);
    IOWAIT();

    outb(PIC2_DAT, ICW3_SLAVE_LINE);
    IOWAIT();

    outb(PIC1_DAT, ICW4_8086);
    IOWAIT();

    outb(PIC2_DAT, ICW4_8086);
    IOWAIT();

    outb(PIC1_DAT, master);
    outb(PIC2_DAT, slave);
}

void irqsendeoi(uint8_t irq)
{
    if (irq >= 8)
        outb(PIC2_CMD, PIC_EOI);

    outb(PIC1_CMD, PIC_EOI);
}
