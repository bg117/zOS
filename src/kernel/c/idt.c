/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <syslvl/idt.h>

#include <misc/type_macros.h>

struct idt_entry idtmkentry(void    *isr,
                            uint16_t code_segment,
                            uint8_t  access_byte)
{
    struct idt_entry ent;
    uint32_t         offset = CAST(uint32_t, isr);

    ent.offset_lower_16 = offset & 0xFFFF; // get lower 16 bits of ISR
    ent.offset_upper_16
        = (offset >> 16) & 0xFFFF;       // shift upper 16 bits to lower
    ent.segment_selector = code_segment; // code segment offset in GDT
    ent.access_byte      = access_byte;
    ent.reserved         = 0;

    return ent;
}

void idtmodentry(struct idt_entry *entry,
                 void             *isr,
                 uint16_t          code_segment,
                 uint8_t           access_byte)
{
    uint32_t offset = CAST(uint32_t, isr);

    entry->offset_lower_16 = offset & 0xFFFF; // get lower 16 bits of ISR
    entry->offset_upper_16
        = (offset >> 16) & 0xFFFF;          // shift upper 16 bits to lower
    entry->segment_selector = code_segment; // code segment offset in GDT
    entry->access_byte      = access_byte;
    entry->reserved         = 0;
}

void idtdescinit(struct idt_descriptor *desc,
                 struct idt_entry      *idt,
                 size_t                 entry_count)
{
    desc->offset  = CAST(uint16_t, sizeof *idt * entry_count - 1);
    desc->address = CAST(uint32_t, idt);
}
