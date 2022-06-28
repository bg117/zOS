/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <syslvl/gdt.h>

#include <misc/type_macros.h>

struct gdt_entry gdtmkentry(uint32_t limit,
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

void gdtdescinit(struct gdt_descriptor *desc,
                 struct gdt_entry      *gdt,
                 size_t                 entry_count)
{
    desc->offset  = CAST(uint16_t, sizeof *gdt * entry_count - 1);
    desc->address = CAST(uint32_t, gdt);
}
