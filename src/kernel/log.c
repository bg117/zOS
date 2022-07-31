/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <kernel/hw/serial.h>
#include <kernel/hw/video.h>
#include <kernel/misc/log.h>
#include <stdarg.h>
#include <stdint.h>

void log_all(LogLevel lvl, const char *fmt, ...)
{
    uint8_t     old_attr = screen_get_current_attribute_byte();
    uint8_t     new_attr = old_attr & 0xF0 /* no foreground */;
    const char *i        = "UNKNOWN";

    switch (lvl)
    {
    case LOG_INFO:
        new_attr |= 0x09;
        i = "INFO    ";
        break;
    case LOG_WARN:
        new_attr |= 0x0E;
        i = "WARNING ";
        break;
    case LOG_ERR:
        new_attr |= 0x04;
        i = "ERROR   ";
        break;
    case LOG_URGENT:
        new_attr |= 0x05;
        i = "URGENT  ";
        break;
    }

    va_list ap;
    va_start(ap, fmt);

    serial_write_string(i);
    serial_write_vformat_string(fmt, ap);

    screen_set_current_attribute_byte(new_attr);
    screen_print_string(i);
    screen_set_current_attribute_byte(old_attr);

    screen_print_vformat_string(fmt, ap);

    va_end(ap);
}

void log_noprint(LogLevel lvl, const char *fmt, ...)
{
    const char *i = "UNKNOWN";

    switch (lvl)
    {
    case LOG_INFO:
        i = "INFO    ";
        break;
    case LOG_WARN:
        i = "WARNING ";
        break;
    case LOG_ERR:
        i = "ERROR   ";
        break;
    case LOG_URGENT:
        i = "URGENT  ";
        break;
    }

    va_list ap;
    va_start(ap, fmt);

    serial_write_string(i);
    serial_write_vformat_string(fmt, ap);

    va_end(ap);
}
