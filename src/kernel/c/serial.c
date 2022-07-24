/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <kernel/hw/serial.h>
#include <kernel/ll/io.h>
#include <stdarg.h>
#include <stddef.h>
#include <utils/chars.h>
#include <utils/num.h>
#include <utils/strings.h>

#define SWITCH_LENGTH_SIGNED(ap, len, buf, base)                   \
    switch (len)                                                   \
    {                                                              \
    case LENGTH_NORMAL:                                            \
        int_to_string(buf, va_arg(ap, int), base);                 \
        break;                                                     \
    case LENGTH_SHORT:                                             \
        int_to_string(buf, (short)(va_arg(ap, int)), base);        \
        break;                                                     \
    case LENGTH_VERY_SHORT:                                        \
        int_to_string(buf, (char)(va_arg(ap, int)), base);         \
        break;                                                     \
    case LENGTH_LONG:                                              \
        long_to_string(buf, va_arg(ap, long), base);               \
        break;                                                     \
    case LENGTH_VERY_LONG:                                         \
        long_long_to_string(buf, va_arg(ap, long long), base);     \
        break;                                                     \
    }                                                              \
                                                                   \
    if (zero_pad)                                                  \
    {                                                              \
        for (size_t i = 0; i < pad_len - str_get_length(buf); i++) \
            serial_write_char('0');                                \
    }                                                              \
    serial_write_format_string("%s", buf)

#define SWITCH_LENGTH_UNSIGNED(ap, len, buf, base)                             \
    switch (len)                                                               \
    {                                                                          \
    case LENGTH_NORMAL:                                                        \
        uint_to_string(buf, va_arg(ap, unsigned int), base);                   \
        break;                                                                 \
    case LENGTH_SHORT:                                                         \
        uint_to_string(buf, (unsigned short)(va_arg(ap, unsigned int)), base); \
        break;                                                                 \
    case LENGTH_VERY_SHORT:                                                    \
        uint_to_string(buf, (unsigned char)(va_arg(ap, unsigned int)), base);  \
        break;                                                                 \
    case LENGTH_LONG:                                                          \
        ulong_to_string(buf, va_arg(ap, unsigned long), base);                 \
        break;                                                                 \
    case LENGTH_VERY_LONG:                                                     \
        ulong_long_to_string(buf, va_arg(ap, unsigned long long), base);       \
        break;                                                                 \
    }                                                                          \
                                                                               \
    if (zero_pad)                                                              \
    {                                                                          \
        for (size_t i = 0; i < pad_len - str_get_length(buf); i++)             \
            serial_write_char('0');                                            \
    }                                                                          \
    serial_write_format_string("%s", buf)

#define SWITCH_LENGTH_UNSIGNED_UPR(ap, len, buf, base)                         \
    switch (len)                                                               \
    {                                                                          \
    case LENGTH_NORMAL:                                                        \
        uint_to_string(buf, va_arg(ap, unsigned int), base);                   \
        break;                                                                 \
    case LENGTH_SHORT:                                                         \
        uint_to_string(buf, (unsigned short)(va_arg(ap, unsigned int)), base); \
        break;                                                                 \
    case LENGTH_VERY_SHORT:                                                    \
        uint_to_string(buf, (unsigned char)(va_arg(ap, unsigned int)), base);  \
        break;                                                                 \
    case LENGTH_LONG:                                                          \
        ulong_to_string(buf, va_arg(ap, unsigned long), base);                 \
        break;                                                                 \
    case LENGTH_VERY_LONG:                                                     \
        ulong_long_to_string(buf, va_arg(ap, unsigned long long), base);       \
        break;                                                                 \
    }                                                                          \
                                                                               \
    if (zero_pad)                                                              \
    {                                                                          \
        for (size_t i = 0; i < pad_len - str_get_length(buf); i++)             \
            serial_write_char('0');                                            \
    }                                                                          \
    serial_write_format_string("%s", buf)

enum printf_state
{
    STATE_NORMAL,
    STATE_SPECIFIER
};

enum printf_length_state
{
    LENGTH_NORMAL,
    LENGTH_SHORT,
    LENGTH_VERY_SHORT,
    LENGTH_LONG,
    LENGTH_VERY_LONG
};

void serial_write_char(char c)
{
    out_byte(0x3F8, c);
}

void serial_write_string(const char *s)
{
    while (*s)
    {
        serial_write_char(*s);
        ++s;
    }
}

void serial_write_format_string(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    enum printf_state        state  = STATE_NORMAL;
    enum printf_length_state length = LENGTH_NORMAL;

    int    zero_pad  = 0;
    int    space_pad = 0;
    size_t pad_len   = 0;

    char num_buf[256];

    while (*fmt)
    {
        if (*fmt == '%')
        {
            ++fmt;
            state = STATE_SPECIFIER;
        }

        if (state == STATE_SPECIFIER)
        {
            switch (*fmt)
            {
            case '%':
                serial_write_char('%');
                break;
            case '0':
                zero_pad = 1;
                pad_len  = 0;
                ++fmt;
                while (*fmt && ISDIGIT(*fmt))
                {
                    pad_len *= 10;
                    pad_len += *fmt - '0';

                    ++fmt;
                }
                continue;
            case ' ':
                space_pad = 1;
                ++fmt;
                while (*fmt && ISDIGIT(*fmt))
                {
                    pad_len *= 10;
                    pad_len += *fmt - '0';

                    ++fmt;
                }
                continue;
            case 's':
                {
                    const char *arg = va_arg(ap, const char *);
                    if (space_pad)
                    {
                        for (size_t i = 0; i < pad_len - str_get_length(arg); i++)
                            serial_write_char(' ');
                    }

                    serial_write_string(arg);
                }
                break;
            case 'c':
                serial_write_char((char)(va_arg(ap, int)));
                break;
            case 'l':
                length = length == LENGTH_LONG ? LENGTH_VERY_LONG : LENGTH_LONG;
                ++fmt;
                continue;
            case 'h':
                length = length == LENGTH_SHORT ? LENGTH_VERY_SHORT : LENGTH_SHORT;
                ++fmt;
                continue;
            case 'i':
            case 'd':
                SWITCH_LENGTH_SIGNED(ap, length, num_buf, 10);
                break;
            case 'u':
                SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 10);
                break;
            case 'x':
                SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 16);
                break;
            case 'X':
                SWITCH_LENGTH_UNSIGNED_UPR(ap, length, num_buf, 16);
                break;
            case 'p':
                serial_write_string("0x");
                SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 16);
                break;
            case 'o':
                SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 8);
                break;
            case 'b':
                SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 2);
                break;
            default:
                serial_write_format_string("%%%c", *fmt);
                break;
            }
        }
        else
        {
            serial_write_char(*fmt);
        }

        state     = STATE_NORMAL;
        length    = LENGTH_NORMAL;
        zero_pad  = 0;
        space_pad = 0;
        pad_len   = 0;
        ++fmt;
    }

    va_end(ap);
}
