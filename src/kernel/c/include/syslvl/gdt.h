/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxGDTHxuuq
#define quuxGDTHxuuq

#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

enum gdt_access_flags
{
    GDT_AX_PRESENT = 0b10000000,

    // DPL
    GDT_AX_PRIV_RING0 = 0b00000000,
    GDT_AX_PRIV_RING1 = 0b01000000,
    GDT_AX_PRIV_RING2 = 0b00100000,
    GDT_AX_PRIV_RING3 = 0b01100000,

    GDT_AX_CODE = 0b00010000,
    GDT_AX_DATA = 0b00010000,

    GDT_AX_EXECUTABLE = 0b00001000,

    GDT_AX_CODE_CONFORMING = 0b00000100,
    GDT_AX_DATA_DIRECTION  = 0b00000100,

    GDT_AX_CODE_READABLE = 0b00000010,
    GDT_AX_DATA_WRITABLE = 0b00000010,

    GDT_AX_ACCESSED = 0b00000001
};

enum gdt_other_flags
{
    GDT_OF_GRANULARITY = 0b1000,
    GDT_OF_32BIT       = 0b0100,
};

struct __attribute__((packed)) gdt_entry
{
    uint16_t limit_lower_16;
    uint16_t base_lower_16;
    uint8_t  base_middle_8;
    uint8_t  access_byte;
    uint8_t  flags_limit_upper_4;
    uint8_t  base_upper_8;
};

struct __attribute__((packed)) gdt_descriptor
{
    uint16_t offset;
    uint32_t address;
};

struct gdt_entry gdtmkentry(uint32_t limit,
                            uint32_t base,
                            uint8_t  access_byte,
                            uint8_t  flags);

void gdtdescinit(struct gdt_descriptor *desc,
                 struct gdt_entry      *gdt,
                 size_t                 entry_count);

void __attribute__((cdecl)) gdtdescld(struct gdt_descriptor *desc,
                                      uint16_t               code_offset,
                                      uint16_t               data_offset);

#if defined(__cplusplus)
}
#endif

#endif
