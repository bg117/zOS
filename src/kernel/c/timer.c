/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdint.h>

#include <syslvl/hal.h>
#include <syslvl/interrupt_info.h>
#include <syslvl/io.h>
#include <syslvl/log.h>
#include <syslvl/pic.h>
#include <syslvl/timer.h>
#include <syslvl/video.h>
#include "syslvl/core.h"

static uint64_t _timer_ticks;
static uint64_t _seconds;

static uint64_t _cycles;

static void _irq0_handler(struct interrupt_info *);

void timer_init()
{
    hal_map_exception_handler(0 + pic_get_pic1_offset(), _irq0_handler);

    timer_set_cycle(100);

    _timer_ticks = 0;
    _seconds     = 0;
}

void timer_set_cycle(int hz)
{
    // I KNOW this is a bad idea, but it's only a temporary fix, and it works!
    const int ONE_MHZ = 1193180;
    uint16_t  div     = ONE_MHZ / hz;

    out_byte(0x43, 0x36);
    out_byte(0x40, div & 0xFF);
    out_byte(0x40, (div >> 8) & 0xFF);

    _cycles = hz;
}

void timer_wait(int ms)
{
    uint64_t ticks_final = _timer_ticks + ms / 1000 * _cycles;
    while (_timer_ticks < ticks_final)
        ;
}

static void _irq0_handler(struct interrupt_info *info)
{
    ++_timer_ticks;

    if (_timer_ticks % _cycles == 0)
        ++_seconds;

    pic_send_eoi(0x00);
}
