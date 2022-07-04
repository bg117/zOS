/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <idt.h>
#include <log.h>

#include <misc/log_macros.h>
#include <misc/type_macros.h>

struct idt_entry idt_make_entry(void *isr, uint16_t code_segment, uint8_t access_byte)
{
    struct idt_entry entry;
    uint32_t         offset = (uint32_t)(isr);

    entry.offset_lower_16  = offset & 0xFFFF;         // get lower 16 bits of ISR
    entry.offset_upper_16  = (offset >> 16) & 0xFFFF; // shift upper 16 bits to lower
    entry.segment_selector = code_segment;            // code segment offset in GDT
    entry.access_byte      = access_byte;
    entry.reserved         = 0;

    return entry;
}

void idt_modify_entry(struct idt_entry *entry, void *isr, uint16_t code_segment, uint8_t access_byte)
{
    uint32_t offset = (uint32_t)(isr);

    entry->offset_lower_16  = offset & 0xFFFF;         // get lower 16 bits of ISR
    entry->offset_upper_16  = (offset >> 16) & 0xFFFF; // shift upper 16 bits to lower
    entry->segment_selector = code_segment;            // code segment offset in GDT
    entry->access_byte      = access_byte;
    entry->reserved         = 0;
}

void idt_descriptor_init(struct idt_descriptor *desc, struct idt_entry *idt, size_t entry_count)
{
    desc->offset  = (uint16_t)(sizeof *idt * entry_count - 1);
    desc->address = (uint32_t)(idt);
}

void idt_descriptor_load(struct idt_descriptor *desc)
{
    struct idt_entry *ent = (struct idt_entry *)(desc->address);
    KLOG("loading descriptor: offset=0x%08X, segment=0x%04X, access_byte=0x%02hhX\n",
         ent->offset_upper_16 << 16 | ent->offset_lower_16,
         ent->segment_selector,
         ent->access_byte);

    __asm__ __volatile__("lidt %0;" ::"m"(*desc));
}
