/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <kernel/ll/io.h>

#include <kernel/int/interrupt_info.h>
#include <kernel/int/isr.h>
#include <kernel/int/pic.h>

#include <kernel/hw/serial.h>
#include <kernel/hw/timer.h>

#include <kernel/misc/log_macros.h>

static volatile uint64_t g_timer_ticks;
static volatile uint64_t g_seconds;

static volatile uint64_t g_cycles;

static void pit_handler(InterruptInfo *);

void timer_init(void)
{
    KSLOG("mapping IRQ 0 handler\n");
    isr_map_interrupt_handler(0 + pic_get_pic1_offset(), pit_handler);

    timer_set_cycle(100);

    g_timer_ticks = 0;
    g_seconds     = 0;
}

void timer_deinit(void)
{
    KSLOG("unmapping IRQ 0 handler\n");
    isr_unmap_interrupt_handler(0 + pic_get_pic1_offset());
}

void timer_set_cycle(int hz)
{
    const int ONE_MHZ = 1193180;
    uint16_t  div     = ONE_MHZ / hz;

    out_byte(0x43, 0x36);
    out_byte(0x40, div & 0xFF);
    out_byte(0x40, (div >> 8) & 0xFF);

    g_cycles = hz;
}

void timer_wait(int ms)
{
    KSLOG("waiting %d ms\n", ms);

    uint64_t ticks_final = g_timer_ticks + ms / 1000 * g_cycles;
    while (g_timer_ticks < ticks_final)
        __asm__ volatile("" : "+g"(g_timer_ticks)::);
}

void pit_handler(InterruptInfo *info)
{
    (void)info;

    ++g_timer_ticks;

    if (g_timer_ticks % g_cycles == 0)
        ++g_seconds;

    pic_send_eoi(0x00);
}
