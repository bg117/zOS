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
enum idt_access_flags
{
    IDT_AX_PRESENT = 0b10000000,

    // gate type
    IDT_AX_I386_TASK = 0b00000101,
    IDT_AX_I386_INT  = 0b00001110,
    IDT_AX_I386_TRAP = 0b00001111,

    // DPL
    IDT_AX_RING0 = 0b00000000,
    IDT_AX_RING1 = 0b00100000,
    IDT_AX_RING2 = 0b01000000,
    IDT_AX_RING3 = 0b01100000
};

/**
 * @brief Represents a interrupt descriptor table entry.
 */
struct __attribute__((packed)) idt_entry
{
    uint16_t offset_lower_16;
    uint16_t segment_selector;
    uint8_t  reserved;
    uint8_t  access_byte;
    uint16_t offset_upper_16;
};

/**
 * @brief The IDT descriptor to load into the IDTR.
 */
struct __attribute__((packed)) idt_descriptor
{
    uint16_t offset;
    uint32_t address;
};

/**
 * @brief Creates an IDT entry.
 *
 * @param isr The interrupt service routine to use.
 * @param code_segment The code segment to use (from GDT).
 * @param access_byte The present, privilege, and type flags.
 * @return A idt_entry struct populated with the provided arguments.
 */
struct idt_entry idt_make_entry(void    *isr,
                                uint16_t code_segment,
                                uint8_t  access_byte);

/**
 * @brief Creates an IDT entry.
 *
 * @param entry The pointer to the entry to modify.
 * @param isr The interrupt service routine to use.
 * @param code_segment The code segment to use (from GDT).
 * @param access_byte The present, privilege, and type flags.
 */
void idt_modify_entry(struct idt_entry *entry,
                      void             *isr,
                      uint16_t          code_segment,
                      uint8_t           access_byte);

/**
 * @brief Initializes a pointer to a IDT descriptor with the IDT provided.
 *
 * @param desc The pointer to the IDT descriptor.
 * @param idt The interrupt descriptor table.
 * @param entry_count The number of entries in the IDT.
 */
void idt_descriptor_init(struct idt_descriptor *desc,
                         struct idt_entry      *idt,
                         size_t                 entry_count);

/**
 * @brief Loads the IDT descriptor into the IDTR.
 *
 * @param desc The pointer to the IDT descriptor.
 */
void idt_descriptor_load(struct idt_descriptor *desc);

#if defined(__cplusplus)
}
#endif

#endif
