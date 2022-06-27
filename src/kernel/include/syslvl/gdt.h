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

enum GdtAccessFlags
{
    GDT_AX_PRESENT         = 0b10000000,
    GDT_AX_PRIV_RING0      = 0b00000000,
    GDT_AX_PRIV_RING1      = 0b01000000,
    GDT_AX_PRIV_RING2      = 0b00100000,
    GDT_AX_PRIV_RING3      = 0b01100000,
    GDT_AX_CODE            = 0b00010000,
    GDT_AX_DATA            = 0b00010000,
    GDT_AX_EXECUTABLE      = 0b00001000,
    GDT_AX_CODE_CONFORMING = 0b00000100,
    GDT_AX_DATA_DIRECTION  = 0b00000100,
    GDT_AX_CODE_READABLE   = 0b00000010,
    GDT_AX_DATA_WRITABLE   = 0b00000010,
    GDT_AX_ACCESSED        = 0b00000001
};

enum GdtOtherFlags
{
    GDT_OF_GRANULARITY = 0b1000,
    GDT_OF_32BIT       = 0b0100,
    GDT_
};

struct __attribute__((packed)) GdtEntry
{
    uint16_t LimitLower16;
    uint16_t BaseLower16;
    uint8_t  BaseMiddle8;
    uint8_t  AccessByte;
    uint8_t  FlagsAndLimitUpper4;
    uint8_t  BaseUpper8;
};

struct __attribute__((packed)) GdtDescriptor
{
    uint16_t Offset;
    uint32_t Address;
};

struct GdtEntry gdtCreateEntry(uint32_t limit,
                               uint32_t base,
                               uint8_t  accessByte,
                               uint8_t  flags);

void gdtInitializeDescriptor(struct GdtDescriptor *desc,
                             struct GdtEntry      *gdt,
                             size_t                gdtEntryCount);

void gdtLoadDescriptor(struct GdtDescriptor *desc,
                       uint16_t              codeOffset,
                       uint16_t              dataOffset);

#if defined(__cplusplus)
}
#endif

#endif
