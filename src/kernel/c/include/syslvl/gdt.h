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

/**
 * @brief The present, privilege, and type flags of a GDT entry.
 */
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

/**
 * @brief Flags for the "other" field of a GDT entry.
 */
enum gdt_other_flags
{
    GDT_OF_GRANULARITY = 0b1000,
    GDT_OF_32BIT       = 0b0100,
};

/**
 * @brief Represents a global descriptor table entry.
 */
struct __attribute__((packed)) gdt_entry
{
    uint16_t limit_lower_16;
    uint16_t base_lower_16;
    uint8_t  base_middle_8;
    uint8_t  access_byte;
    uint8_t  flags_limit_upper_4;
    uint8_t  base_upper_8;
};

/**
 * @brief The GDT descriptor to load into the GDTR.
 */
struct __attribute__((packed)) gdt_descriptor
{
    uint16_t offset;
    uint32_t address;
};

/**
 * @brief Creates an entry for a global descriptor table.
 *
 * @param limit The 20-bit limit of the segment.
 * @param base The base of the segment.
 * @param access_byte The present, privilege, and type flags.
 * @param flags Other flags such as granularity, 32-bit, etc.
 * @return A gdt_entry struct populated with the provided arguments.
 */
struct gdt_entry gdt_make_entry(uint32_t limit,
                                uint32_t base,
                                uint8_t  access_byte,
                                uint8_t  flags);

/**
 * @brief Initializes a pointer to a GDT descriptor with the GDT provided.
 *
 * @param desc The pointer to the GDT descriptor.
 * @param gdt The global descriptor table.
 * @param entry_count The number of entries in the GDT.
 */
void gdt_descriptor_init(struct gdt_descriptor *desc,
                         struct gdt_entry      *gdt,
                         size_t                 entry_count);

/**
 * @brief Loads the GDT descriptor into the GDTR.
 *
 * @param desc The pointer to the GDT descriptor.
 * @param code_offset The offset to the code segment to be used. Must be a
 *                    multiple of 8.
 * @param data_offset The offset to the data segment to be used. Must also
 *                    be a multiple of 8.
 */
void gdt_descriptor_load(struct gdt_descriptor *desc,
                         uint16_t               code_offset,
                         uint16_t               data_offset);

#if defined(__cplusplus)
}
#endif

#endif
