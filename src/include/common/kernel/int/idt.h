/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxIDTHxuuq
#define quuxIDTHxuuq

#include <stddef.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief The present, privilege, and type flags of a IDT entry.
 */
typedef enum idt_access_flags
{
    IDT_AX_PRESENT = 0x80,

    // gate type
    IDT_AX_I386_TASK = 0x05,
    IDT_AX_I386_INT  = 0x0E,
    IDT_AX_I386_TRAP = 0x0F,

    // DPL
    IDT_AX_RING0 = 0x00,
    IDT_AX_RING1 = 0x20,
    IDT_AX_RING2 = 0x40,
    IDT_AX_RING3 = 0x60
} IdtAccessFlags;

/**
 * @brief Represents a interrupt descriptor table entry.
 */
typedef struct __attribute__((packed)) idt_entry
{
    uint16_t offset_lower_16;
    uint16_t segment_selector;
    uint8_t  reserved;
    uint8_t  access_byte;
    uint16_t offset_upper_16;
} IdtEntry;

/**
 * @brief The IDT descriptor to load into the IDTR.
 */
typedef struct __attribute__((packed)) idt_descriptor
{
    uint16_t offset;
    uint32_t address;
} IdtDescriptor;

/**
 * @brief Creates an IDT entry.
 *
 * @param isr The pointer to the interrupt service routine to use.
 * @param code_segment The code segment to use (from GDT).
 * @param access_byte The present, privilege, and type flags.
 * @return A idt_entry struct populated with the provided arguments.
 */
IdtEntry idt_make_entry(void (*isr)(void), uint16_t code_segment, uint8_t access_byte);

/**
 * @brief Creates an IDT entry.
 *
 * @param entry The pointer to the entry to modify.
 * @param isr The interrupt service routine to use.
 * @param code_segment The code segment to use (from GDT).
 * @param access_byte The present, privilege, and type flags.
 */
void idt_modify_entry(IdtEntry *entry, void *isr, uint16_t code_segment, uint8_t access_byte);

/**
 * @brief Initializes a pointer to a IDT descriptor with the IDT provided.
 *
 * @param desc The pointer to the IDT descriptor.
 * @param idt The interrupt descriptor table.
 * @param entry_count The number of entries in the IDT.
 */
void idt_descriptor_init(IdtDescriptor *desc, IdtEntry *idt, size_t entry_count);

/**
 * @brief Loads the IDT descriptor into the IDTR.
 *
 * @param desc The pointer to the IDT descriptor.
 */
void idt_descriptor_load(IdtDescriptor *desc);

#if defined(__cplusplus)
}
#endif

#endif
