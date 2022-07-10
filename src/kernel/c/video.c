/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <ll/io.h>

#include <memory/mem.h>

#include <hw/video.h>

#include <misc/char_macros.h>
#include <misc/num.h>
#include <misc/strings.h>
#include <misc/type_macros.h>

#define SWITCH_LENGTH_SIGNED(ap, len, buf, base)                                                                       \
    switch (len)                                                                                                       \
    {                                                                                                                  \
    case LENGTH_NORMAL: int_to_string(buf, va_arg(ap, int), base); break;                                              \
    case LENGTH_SHORT: int_to_string(buf, (short)(va_arg(ap, int)), base); break;                                      \
    case LENGTH_VERY_SHORT: int_to_string(buf, (char)(va_arg(ap, int)), base); break;                                  \
    case LENGTH_LONG: long_to_string(buf, va_arg(ap, long), base); break;                                              \
    case LENGTH_VERY_LONG: long_long_to_string(buf, va_arg(ap, long long), base); break;                               \
    }                                                                                                                  \
                                                                                                                       \
    if (zero_pad)                                                                                                      \
    {                                                                                                                  \
        for (int i = 0; i < pad_len - str_get_length(buf); i++)                                                        \
            screen_print_char('0');                                                                                    \
    }                                                                                                                  \
    screen_print_format_string("%s", buf)

#define SWITCH_LENGTH_UNSIGNED(ap, len, buf, base)                                                                     \
    switch (len)                                                                                                       \
    {                                                                                                                  \
    case LENGTH_NORMAL: uint_to_string(buf, va_arg(ap, unsigned int), base); break;                                    \
    case LENGTH_SHORT: uint_to_string(buf, (unsigned short)(va_arg(ap, unsigned int)), base); break;                   \
    case LENGTH_VERY_SHORT: uint_to_string(buf, (unsigned char)(va_arg(ap, unsigned int)), base); break;               \
    case LENGTH_LONG: ulong_to_string(buf, va_arg(ap, unsigned long), base); break;                                    \
    case LENGTH_VERY_LONG: ulong_long_to_string(buf, va_arg(ap, unsigned long long), base); break;                     \
    }                                                                                                                  \
                                                                                                                       \
    if (zero_pad)                                                                                                      \
    {                                                                                                                  \
        for (int i = 0; i < pad_len - str_get_length(buf); i++)                                                        \
            screen_print_char('0');                                                                                    \
    }                                                                                                                  \
    screen_print_format_string("%s", buf)

#define SWITCH_LENGTH_UNSIGNED_UPR(ap, len, buf, base)                                                                 \
    switch (len)                                                                                                       \
    {                                                                                                                  \
    case LENGTH_NORMAL: uint_to_string(buf, va_arg(ap, unsigned int), base); break;                                    \
    case LENGTH_SHORT: uint_to_string(buf, (unsigned short)(va_arg(ap, unsigned int)), base); break;                   \
    case LENGTH_VERY_SHORT: uint_to_string(buf, (unsigned char)(va_arg(ap, unsigned int)), base); break;               \
    case LENGTH_LONG: ulong_to_string(buf, va_arg(ap, unsigned long), base); break;                                    \
    case LENGTH_VERY_LONG: ulong_long_to_string(buf, va_arg(ap, unsigned long long), base); break;                     \
    }                                                                                                                  \
                                                                                                                       \
    if (zero_pad)                                                                                                      \
    {                                                                                                                  \
        for (int i = 0; i < pad_len - str_get_length(buf); i++)                                                        \
            screen_print_char('0');                                                                                    \
    }                                                                                                                  \
    screen_print_format_string("%s", buf)

#define GET_VGA_POSITION_XY(x, y) (((y) * (VGA_WIDTH) + (x)) * 2)

static const uint8_t VGA_WIDTH  = 80;
static const uint8_t VGA_LENGTH = 25;

static const int TAB_WIDTH = 8;

static uint8_t *const VGA_BUFFER = (uint8_t *const)(0xC03FF000);
static uint16_t       _pos_x     = 0;
static uint16_t       _pos_y     = 0;

static volatile int _move_cursor_chr = 1;

static uint8_t _char_color = 0x07; // light gray

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

void screen_print_string(const char *__restrict__ s)
{
    int old_flag     = _move_cursor_chr;
    _move_cursor_chr = 0;

    while (*s)
    {
        screen_print_char(*s);
        ++s;
    }

    _move_cursor_chr = old_flag;
    screen_move_cursor(_pos_x, _pos_y);
}

void screen_print_format_string(const char *__restrict__ fmt, ...)
{
    int old_flag     = _move_cursor_chr;
    _move_cursor_chr = 0;

    va_list ap;
    va_start(ap, fmt);

    enum printf_state        state  = STATE_NORMAL;
    enum printf_length_state length = LENGTH_NORMAL;

    int zero_pad  = 0;
    int space_pad = 0;
    int pad_len   = 0;

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
            case '%': screen_print_char('%'); break;
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
                            screen_print_char(' ');
                    }

                    screen_print_string(arg);
                }
                break;
            case 'c': screen_print_char((char)(va_arg(ap, int))); break;
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
                screen_print_string("0x");
                SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 16);
                break;
            case 'o': SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 8); break;
            case 'b': SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 2); break;
            default: screen_print_format_string("%%%c", *fmt); break;
            }
        }
        else
        {
            screen_print_char(*fmt);
        }

        state     = STATE_NORMAL;
        length    = LENGTH_NORMAL;
        zero_pad  = 0;
        space_pad = 0;
        pad_len   = 0;
        ++fmt;
    }

    va_end(ap);

    _move_cursor_chr = old_flag;
    screen_move_cursor(_pos_x, _pos_y);
}

void screen_print_char(char c)
{
    if (_pos_y >= VGA_LENGTH)
        screen_scroll();

    switch (c)
    {
    case '\n':
        _pos_x = 0;
        ++_pos_y;
        break;
    case '\t':
        if (_pos_x % TAB_WIDTH == 0)
        {
            _pos_x += TAB_WIDTH;
        }
        else
        {
            for (int i = 0; i < _pos_x % TAB_WIDTH; i++)
                screen_print_char(' ');
        }
        break;
    case '\r': _pos_x = 0; break;
    case '\b':
        if (_pos_x > 0)
            --_pos_x;
        break;
    default:
        {
            int position             = GET_VGA_POSITION_XY(_pos_x, _pos_y);
            VGA_BUFFER[position]     = c;
            VGA_BUFFER[position + 1] = _char_color;

            ++_pos_x;

            if (_pos_x >= VGA_WIDTH)
            {
                _pos_x = 0;
                ++_pos_y;
            }

            if (_move_cursor_chr)
                screen_move_cursor(_pos_x, _pos_y);
        }
        break;
    }
}

void screen_clear(void)
{
    _pos_x = 0;
    _pos_y = 0;

    mem_fill(VGA_BUFFER, 0, VGA_LENGTH * VGA_WIDTH * 2);

    screen_move_cursor(_pos_x, _pos_y);
}

void screen_clear_line(int y)
{
    _pos_x = 0;

    mem_fill(VGA_BUFFER + VGA_WIDTH * y * 2, 0, VGA_WIDTH * 2);

    screen_move_cursor(_pos_x, _pos_y);
}

void screen_scroll(void)
{
    mem_copy_with_overlap(VGA_BUFFER, VGA_BUFFER + VGA_WIDTH * 2, VGA_WIDTH * VGA_LENGTH * 2);
    mem_fill(VGA_BUFFER + VGA_WIDTH * (VGA_LENGTH - 1) * 2, 0, VGA_WIDTH);

    if (_pos_y > 0)
        --_pos_y;
}

void screen_move_cursor(int x, int y)
{
    uint16_t pos = GET_VGA_POSITION_XY(x, y) / 2;
    out_byte(0x3D4, 0x0F);
    out_byte(0x3D5, (uint8_t)(pos & 0xFF));
    out_byte(0x3D4, 0x0E);
    out_byte(0x3D5, (uint8_t)((pos >> 8) & 0xFF));

    _pos_x = x;
    _pos_y = y;
}

void screen_get_cursor_position(int *x, int *y)
{
    *x = _pos_x;
    *y = _pos_y;
}
