/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxMMAPHxuuq
#define quuxMMAPHxuuq

#include <stdint.h>

/**
 * @brief Represents a memory map entry returned by INT 0x15, AX=0xE820.
 */
typedef struct __attribute__((packed)) memory_map_entry
{
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t acpi_extended_attributes;
} MemoryMapEntry;

#endif
