/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <syslvl/gdt.h>
#include <syslvl/i.h>

struct GdtEntry gdtCreateEntry(uint32_t limit,
                               uint32_t base,
                               uint8_t  accessByte,
                               uint8_t  flags)
{
    struct GdtEntry entry;
    entry.LimitLower16        = limit & 0xFFFF;
    entry.BaseLower16         = base & 0xFFFF;
    entry.BaseMiddle8         = (base >> 16) & 0xFF;
    entry.AccessByte          = accessByte;
    entry.FlagsAndLimitUpper4 = (flags << 4) | ((limit >> 16) & 0xF);
    entry.BaseUpper8          = (base >> 24) & 0xFF;

    return entry;
}

void gdtInitializeDescriptor(struct GdtDescriptor *desc,
                             struct GdtEntry      *gdt,
                             size_t                gdtEntryCount)
{
    desc->Offset  = CAST(uint16_t, sizeof *gdt * gdtEntryCount - 1);
    desc->Address = CAST(uint32_t, gdt);
}
