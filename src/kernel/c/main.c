/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stddef.h>
#include <stdint.h>

#include <syslvl/core.h>
#include <syslvl/disk.h>
#include <syslvl/fat.h>
#include <syslvl/gdt.h>
#include <syslvl/hal.h>
#include <syslvl/idt.h>
#include <syslvl/irq.h>
#include <syslvl/mem.h>
#include <syslvl/video.h>

#include <misc/num.h>

// main
int main(int argc, char **argv)
{
    scrclr();

    struct fat_info fi;
    uint8_t         driveNumber;

    // works!
    memcopy(&fi, argv[1], sizeof fi);
    memcopy(&driveNumber, &argv[0], sizeof driveNumber);

    scrputs("---------------- KERNEL LOG ----------------\r\n");

    scrputs("Initializing the hardware abstraction layer... ");
    halinit();
    scrputs("Done.\r\n");

    scrputs("Remapping the Programmable Interrupt Controller... ");
    irqinit(0x20,
            0x28); // master PIC from offset 0x20-0x27; slave from 0x28-0x2F
    scrputs("Done.\r\n");

    while (1)
        ;

    return 1;
}
