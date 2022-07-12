/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <ll/gdt.h>

#include <hw/serial.h>

#include <misc/log_macros.h>
#include <misc/type_macros.h>

GdtEntry gdt_make_entry(uint32_t limit, uint32_t base, uint8_t access_byte, uint8_t flags)
{
    GdtEntry entry;
    entry.limit_lower_16      = limit & 0xFFFF;
    entry.base_lower_16       = base & 0xFFFF;
    entry.base_middle_8       = (base >> 16) & 0xFF;
    entry.access_byte         = access_byte;
    entry.flags_limit_upper_4 = (flags << 4) | ((limit >> 16) & 0xF);
    entry.base_upper_8        = (base >> 24) & 0xFF;

    KSLOG("creating entry: base=0x%08X, limit=0x%05hX, access_byte=0x%02hhX\n",
          entry.base_upper_8 << 24 | entry.base_middle_8 << 16 | entry.base_lower_16,
          (entry.flags_limit_upper_4 & 0xF) << 16 | entry.limit_lower_16,
          entry.access_byte);

    return entry;
}

void gdt_descriptor_init(GdtDescriptor *desc, GdtEntry *gdt, size_t entry_count)
{
    desc->offset  = (uint16_t)(sizeof *gdt * entry_count - 1);
    desc->address = (uint32_t)gdt;
}

void gdt_descriptor_load(GdtDescriptor *desc, uint16_t code_offset, uint16_t data_offset)
{
    asm volatile(
        "lgdt %0;"
        "pushl %1;"
        "pushl $.L1;"
        "lret;"
        ".L1:;"
        "movw %2, %%ds;"
        "movw %2, %%es;"
        "movw %2, %%fs;"
        "movw %2, %%gs;"
        "movw %2, %%ss;"
        :
        : "m"(*desc), "c"((uint32_t)code_offset), "a"(data_offset));
}
