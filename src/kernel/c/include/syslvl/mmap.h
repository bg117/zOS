/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxMMAPHxuuq
#define quuxMMAPHxuuq

#include <stdint.h>

typedef struct __attribute__((packed)) memory_map
{
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t acpi_extended_attributes;
} MemoryMap;

#endif
