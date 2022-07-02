/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdarg.h>
#include <stddef.h>

#include <syslvl/log.h>

#include <misc/char_macros.h>
#include <misc/num.h>
#include <misc/strings.h>
#include <misc/type_macros.h>

#define SWITCH_LENGTH_SIGNED(ap, len, buf, base)                                                                       \
    switch (len)                                                                                                       \
    {                                                                                                                  \
    case LENGTH_NORMAL: int_to_string(buf, va_arg(ap, int), base); break;                                              \
    case LENGTH_SHORT: int_to_string(buf, CAST(short, va_arg(ap, int)), base); break;                                  \
    case LENGTH_VERY_SHORT: int_to_string(buf, CAST(char, va_arg(ap, int)), base); break;                              \
    case LENGTH_LONG: long_to_string(buf, va_arg(ap, long), base); break;                                              \
    case LENGTH_VERY_LONG: long_long_to_string(buf, va_arg(ap, long long), base); break;                               \
    }                                                                                                                  \
                                                                                                                       \
    if (zero_pad)                                                                                                      \
    {                                                                                                                  \
        for (int i = 0; i < pad_len - str_get_length(buf); i++)                                                        \
            log_append_char('0');                                                                                      \
    }                                                                                                                  \
    log_append_format_string("%s", buf)

#define SWITCH_LENGTH_UNSIGNED(ap, len, buf, base)                                                                     \
    switch (len)                                                                                                       \
    {                                                                                                                  \
    case LENGTH_NORMAL: uint_to_string(buf, va_arg(ap, unsigned int), base); break;                                    \
    case LENGTH_SHORT: uint_to_string(buf, va_arg(ap, unsigned int), base); break;                                     \
    case LENGTH_VERY_SHORT: uint_to_string(buf, va_arg(ap, unsigned int), base); break;                                \
    case LENGTH_LONG: ulong_to_string(buf, va_arg(ap, unsigned long), base); break;                                    \
    case LENGTH_VERY_LONG: ulong_long_to_string(buf, va_arg(ap, unsigned long long), base); break;                     \
    }                                                                                                                  \
                                                                                                                       \
    if (zero_pad)                                                                                                      \
    {                                                                                                                  \
        for (int i = 0; i < pad_len - str_get_length(buf); i++)                                                        \
            log_append_char('0');                                                                                      \
    }                                                                                                                  \
    log_append_format_string("%s", buf)

#define SWITCH_LENGTH_UNSIGNED_UPR(ap, len, buf, base)                                                                 \
    switch (len)                                                                                                       \
    {                                                                                                                  \
    case LENGTH_NORMAL: uint_to_string(buf, va_arg(ap, unsigned int), base); break;                                    \
    case LENGTH_SHORT: uint_to_string(buf, va_arg(ap, unsigned int), base); break;                                     \
    case LENGTH_VERY_SHORT: uint_to_string(buf, va_arg(ap, unsigned int), base); break;                                \
    case LENGTH_LONG: ulong_to_string(buf, va_arg(ap, unsigned long), base); break;                                    \
    case LENGTH_VERY_LONG: ulong_long_to_string(buf, va_arg(ap, unsigned long long), base); break;                     \
    }                                                                                                                  \
                                                                                                                       \
    if (zero_pad)                                                                                                      \
    {                                                                                                                  \
        for (int i = 0; i < pad_len - str_get_length(buf); i++)                                                        \
            log_append_char('0');                                                                                      \
    }                                                                                                                  \
    log_append_format_string("%s", buf)

static char  *_tmp_msg_buffer = CAST(char *, 0x10FEF);
static size_t _msg_buffer_idx = 0;

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

void log_append_char(char c)
{
    _tmp_msg_buffer[_msg_buffer_idx] = c;
    ++_msg_buffer_idx;
}

void log_append_string(const char *__restrict__ s)
{
    while (*s)
    {
        log_append_char(*s);
        ++s;
    }
}

void log_append_format_string(const char *__restrict__ fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    enum printf_state        state  = STATE_NORMAL;
    enum printf_length_state length = LENGTH_NORMAL;

    int zero_pad = 0;
    int pad_len  = 0;

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
            case '%': log_append_char('%'); break;
            case '0':
                zero_pad = 1;
                ++fmt;
                while (*fmt && ISDIGIT(*fmt))
                {
                    pad_len *= 10;
                    pad_len += *fmt - '0';

                    ++fmt;
                }
                continue;
            case 's': log_append_string(va_arg(ap, const char *)); break;
            case 'c': log_append_char(CAST(char, va_arg(ap, int))); break;
            case 'l':
                length = length == LENGTH_LONG ? LENGTH_VERY_LONG : LENGTH_LONG;
                ++fmt;
                continue;
            case 'h':
                length = length == LENGTH_SHORT ? LENGTH_VERY_SHORT : LENGTH_SHORT;
                ++fmt;
                continue;
            case 'i':
            case 'd': SWITCH_LENGTH_SIGNED(ap, length, num_buf, 10); break;
            case 'u': SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 10); break;
            case 'x': SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 16); break;
            case 'X': SWITCH_LENGTH_UNSIGNED_UPR(ap, length, num_buf, 16); break;
            case 'p':
                log_append_string("0x");
                SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 16);
                break;
            case 'o': SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 8); break;
            case 'b': SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 2); break;
            default: log_append_format_string("%%%c", *fmt); break;
            }
        }
        else
        {
            log_append_char(*fmt);
        }

        state  = STATE_NORMAL;
        length = LENGTH_NORMAL;
        ++fmt;
    }

    va_end(ap);
}

char *log_get_tmp_buffer()
{
    return _tmp_msg_buffer;
}
