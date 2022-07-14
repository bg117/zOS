/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <kernel/int/idt.h>

#include <kernel/hw/serial.h>

#include <kernel/misc/log_macros.h>

IdtEntry idt_make_entry(void (*isr)(void), uint16_t code_segment, uint8_t access_byte)
{
    IdtEntry entry;
    uint32_t offset = (uint32_t)isr;

    entry.offset_lower_16  = offset & 0xFFFF;         // get lower 16 bits of ISR
    entry.offset_upper_16  = (offset >> 16) & 0xFFFF; // shift upper 16 bits to lower
    entry.segment_selector = code_segment;            // code segment offset in GDT
    entry.access_byte      = access_byte;
    entry.reserved         = 0;

    KSLOG("creating entry: offset=0x%08X, segment=0x%04hX, access_byte=0x%02hhX\n",
          entry.offset_upper_16 << 16 | entry.offset_lower_16,
          entry.segment_selector,
          entry.access_byte);

    return entry;
}

void idt_modify_entry(IdtEntry *entry, void *isr, uint16_t code_segment, uint8_t access_byte)
{
    uint32_t offset = (uint32_t)isr;

    entry->offset_lower_16  = offset & 0xFFFF;         // get lower 16 bits of ISR
    entry->offset_upper_16  = (offset >> 16) & 0xFFFF; // shift upper 16 bits to lower
    entry->segment_selector = code_segment;            // code segment offset in GDT
    entry->access_byte      = access_byte;
    entry->reserved         = 0;
}

void idt_descriptor_init(IdtDescriptor *desc, IdtEntry *idt, size_t entry_count)
{
    desc->offset  = (uint16_t)(sizeof *idt * entry_count - 1);
    desc->address = (uint32_t)idt;
}

void idt_descriptor_load(IdtDescriptor *desc)
{
    __asm__ volatile("lidt %0;" ::"m"(*desc));
}
