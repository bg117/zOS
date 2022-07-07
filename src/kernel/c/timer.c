/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <ll/io.h>

#include <int/interrupt_info.h>
#include <int/isr.h>
#include <int/pic.h>

#include <hw/serial.h>
#include <hw/timer.h>

#include <misc/log_macros.h>

static uint64_t g_timer_ticks;
static uint64_t g_seconds;

static uint64_t g_cycles;

static void pit_handler(struct interrupt_info *);

void timer_init()
{
    KSLOG("mapping IRQ 0 handler\n");
    isr_map_interrupt_handler(0 + pic_get_pic1_offset(), pit_handler);

    timer_set_cycle(100);

    g_timer_ticks = 0;
    g_seconds     = 0;
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
    uint64_t ticks_final = g_timer_ticks + ms / 1000 * g_cycles;
    while (g_timer_ticks < ticks_final)
        ;
}

static void pit_handler(struct interrupt_info *info)
{
    ++g_timer_ticks;

    if (g_timer_ticks % g_cycles == 0)
        ++g_seconds;

    pic_send_eoi(0x00);
}
