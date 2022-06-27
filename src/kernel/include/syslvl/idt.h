/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxIRQHxuuq
#define quuxIRQHxuuq

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

struct __attribute__((packed)) IdtEntry
{
    uint16_t OffsetLower16;
    uint16_t SegmentSelector;
    uint8_t  Reserved;
    uint8_t  GateType : 4;
    uint8_t  Zero : 1;
    uint8_t  Dpl : 2;
    uint8_t  Present : 1;
    uint16_t OffsetUpper16;
};

struct __attribute__((packed)) IdtDescriptor
{
    uint16_t Offset;
    uint32_t Address;
};

struct IdtEntry idtCreateEntry(void   *isr,
                               uint8_t gateType,
                               uint8_t dpl,
                               uint8_t present);

#if defined(__cplusplus)
}
#endif

#endif
