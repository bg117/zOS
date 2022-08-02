/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <kernel/hw/serial.h>
#include <kernel/hw/timer.h>
#include <kernel/hw/video.h>
#include <kernel/int/interrupt_info.h>
#include <kernel/int/isr.h>
#include <kernel/int/pic.h>
#include <kernel/ll/io.h>
#include <kernel/misc/log.h>
#include <stdint.h>

static volatile uint64_t g_timer_ticks;
static volatile uint64_t g_seconds;

static const volatile uint64_t PIT_CYCLES = 1000;

static void pit_handler(InterruptInfo *);

void timer_init(void)
{
    log_all(LOG_INFO, "mapping IRQ 0 handler\n");
    isr_map_interrupt_handler(0 + pic_get_pic1_offset(), pit_handler);

    log_all(LOG_INFO, "setting PIT phase to %llu Hz\n", PIT_CYCLES);
    uint16_t div = (7159090 + 6 / 2) / (6 * PIT_CYCLES);

    out_byte(0x43, 0x36);
    out_byte(0x40, div & 0xFF);
    out_byte(0x40, (div >> 8) & 0xFF);

    g_timer_ticks = 0;
    g_seconds     = 0;
}

void timer_deinit(void)
{
    log_all(LOG_INFO, "unmapping IRQ 0 handler\n");
    isr_unmap_interrupt_handler(0 + pic_get_pic1_offset());
}

uint64_t timer_get_current_tick(void)
{
    return g_timer_ticks;
}

void timer_wait(int ms)
{
    log_noprint(LOG_INFO, "waiting %d ms\n", ms);

    uint64_t ticks_final = g_timer_ticks + ms / 1000 * PIT_CYCLES;
    while (g_timer_ticks < ticks_final)
        __asm__ volatile("" : "+g"(g_timer_ticks)::);
}

void pit_handler(InterruptInfo *info)
{
    (void)info;

    ++g_timer_ticks;

    if (g_timer_ticks % PIT_CYCLES == 0)
        ++g_seconds;

    pic_send_eoi(0x00);
}
