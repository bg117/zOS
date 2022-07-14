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
typedef enum gdt_access_flags
{
    GDT_AX_PRESENT = 0x80,

    // DPL
    GDT_AX_PRIV_RING0 = 0x00,
    GDT_AX_PRIV_RING1 = 0x20,
    GDT_AX_PRIV_RING2 = 0x40,
    GDT_AX_PRIV_RING3 = 0x60,

    GDT_AX_CODE = 0x10,
    GDT_AX_DATA = 0x10,

    GDT_AX_EXECUTABLE = 0x08,

    GDT_AX_CODE_CONFORMING = 0x04,
    GDT_AX_DATA_DIRECTION  = 0x04,

    GDT_AX_CODE_READABLE = 0x02,
    GDT_AX_DATA_WRITABLE = 0x02,

    GDT_AX_ACCESSED = 0x01
} GdtAccessFlags;

/**
 * @brief Flags for the "other" field of a GDT entry.
 */
typedef enum gdt_other_flags
{
    GDT_OF_GRANULARITY = 0x08,
    GDT_OF_32BIT       = 0x04,
} GdtOtherFlags;

/**
 * @brief Represents a global descriptor table entry.
 */
typedef struct __attribute__((packed)) gdt_entry
{
    uint16_t limit_lower_16;
    uint16_t base_lower_16;
    uint8_t  base_middle_8;
    uint8_t  access_byte;
    uint8_t  flags_limit_upper_4;
    uint8_t  base_upper_8;
} GdtEntry;

/**
 * @brief The GDT descriptor to load into the GDTR.
 */
typedef struct __attribute__((packed)) gdt_descriptor
{
    uint16_t offset;
    uint32_t address;
} GdtDescriptor;

/**
 * @brief Creates an entry for a global descriptor table.
 *
 * @param limit The 20-bit limit of the segment.
 * @param base The base of the segment.
 * @param access_byte The present, privilege, and type flags.
 * @param flags Other flags such as granularity, 32-bit, etc.
 * @return A gdt_entry struct populated with the provided arguments.
 */
GdtEntry gdt_make_entry(uint32_t limit, uint32_t base, uint8_t access_byte, uint8_t flags);

/**
 * @brief Initializes a pointer to a GDT descriptor with the GDT provided.
 *
 * @param desc The pointer to the GDT descriptor.
 * @param gdt The global descriptor table.
 * @param entry_count The number of entries in the GDT.
 */
void gdt_descriptor_init(GdtDescriptor *desc, GdtEntry *gdt, size_t entry_count);

/**
 * @brief Loads the GDT descriptor into the GDTR.
 *
 * @param desc The pointer to the GDT descriptor.
 * @param code_offset The offset to the code segment to be used. Must be a
 *                    multiple of 8.
 * @param data_offset The offset to the data segment to be used. Must also
 *                    be a multiple of 8.
 */
void gdt_descriptor_load(GdtDescriptor *desc, uint16_t code_offset, uint16_t data_offset);

#if defined(__cplusplus)
}
#endif

#endif
