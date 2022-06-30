/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <syslvl/gdt.h>

#include <misc/type_macros.h>

struct gdt_entry gdt_make_entry(uint32_t limit,
                                uint32_t base,
                                uint8_t  access_byte,
                                uint8_t  flags)
{
    struct gdt_entry entry;
    entry.limit_lower_16      = limit & 0xFFFF;
    entry.base_lower_16       = base & 0xFFFF;
    entry.base_middle_8       = (base >> 16) & 0xFF;
    entry.access_byte         = access_byte;
    entry.flags_limit_upper_4 = (flags << 4) | ((limit >> 16) & 0xF);
    entry.base_upper_8        = (base >> 24) & 0xFF;

    return entry;
}

void gdt_descriptor_init(struct gdt_descriptor *desc,
                         struct gdt_entry      *gdt,
                         size_t                 entry_count)
{
    desc->offset  = CAST(uint16_t, sizeof *gdt * entry_count - 1);
    desc->address = CAST(uint32_t, gdt);
}

void gdt_descriptor_load(struct gdt_descriptor *desc,
                         uint16_t               code_offset,
                         uint16_t               data_offset)
{
    __asm__ __volatile__(
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
