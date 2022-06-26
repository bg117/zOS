/**
 * Copyright (c) 2022 iusearchbtw
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>
#include <syslvl/i.h>
#include <syslvl/irq.h>

static struct IdtEntry g_Idt[256];

// for lidt instruction
static struct __attribute__((packed)) LoadIdt
{
    uint16_t         Limit;
    struct IdtEntry *Address;
} g_Lidt;

extern void __defExcept(void) __attribute__((noreturn));

void idtSetDescriptor(
    uint8_t idx, void *isr, uint8_t gateType, uint8_t dpl, uint8_t present)
{
    struct IdtEntry *ent    = &g_Idt[idx];
    uint32_t         offset = CAST(uint32_t, isr);

    ent->OffsetLower16   = offset & 0xFFFF; // get lower 16 bits of ISR
    ent->OffsetUpper16   = offset >> 16;    // shift upper 16 bits to lower
    ent->SegmentSelector = 0x08;            // data in GDT
    ent->GateType        = gateType;
    ent->Dpl             = dpl;
    ent->Present         = present;
}
