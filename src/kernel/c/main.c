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

    scrputs("Initializing hardware abstraction layer (HAL)... ");
    halinit();
    scrputs("Done.\r\n");

    __asm__ __volatile__("int $0x03"); // test!!

    scrputs("Halting system...");
    corehlt();

    return 1;
}
