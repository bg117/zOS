/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <syslvl/num.h>
#include <syslvl/video.h>

#include <misc/define.h>
#include <syslvl/mem.h>

#define SWITCH_LENGTH_SIGNED(ap, len, buf, base)                               \
    do                                                                         \
    {                                                                          \
        switch (len)                                                           \
        {                                                                      \
        case LENGTH_NORMAL:                                                    \
            inttos(buf, va_arg(ap, int), base);                                \
            scrputsf("%s", buf);                                               \
            break;                                                             \
        case LENGTH_SHORT:                                                     \
            inttos(buf, CAST(short, va_arg(ap, int)), base);                   \
            scrputsf("%s", buf);                                               \
            break;                                                             \
        case LENGTH_VERY_SHORT:                                                \
            inttos(buf, CAST(char, va_arg(ap, int)), base);                    \
            scrputsf("%s", buf);                                               \
            break;                                                             \
        case LENGTH_LONG:                                                      \
            longtos(buf, va_arg(ap, long), base);                              \
            scrputsf("%s", buf);                                               \
            break;                                                             \
        case LENGTH_VERY_LONG:                                                 \
            llongtos(buf, va_arg(ap, long long), base);                        \
            scrputsf("%s", buf);                                               \
            break;                                                             \
        }                                                                      \
    } while (0)

#define SWITCH_LENGTH_UNSIGNED(ap, len, buf, base)                             \
    do                                                                         \
    {                                                                          \
        switch (len)                                                           \
        {                                                                      \
        case LENGTH_NORMAL:                                                    \
            uinttos(buf, va_arg(ap, unsigned int), base);                      \
            scrputsf("%s", buf);                                               \
            break;                                                             \
        case LENGTH_SHORT:                                                     \
            uinttos(buf, va_arg(ap, unsigned int), base);                      \
            scrputsf("%s", buf);                                               \
            break;                                                             \
        case LENGTH_VERY_SHORT:                                                \
            uinttos(buf, va_arg(ap, unsigned int), base);                      \
            scrputsf("%s", buf);                                               \
            break;                                                             \
        case LENGTH_LONG:                                                      \
            ulongtos(buf, va_arg(ap, unsigned long), base);                    \
            scrputsf("%s", buf);                                               \
            break;                                                             \
        case LENGTH_VERY_LONG:                                                 \
            ullongtos(buf, va_arg(ap, unsigned long long), base);              \
            scrputsf("%s", buf);                                               \
            break;                                                             \
        }                                                                      \
    } while (0)

#define GET_VGA_POSITION_XY(x, y) (((y) * (VGA_WIDTH) + (x)) * 2)

static const uint8_t VGA_WIDTH  = 80;
static const uint8_t VGA_LENGTH = 25;

static uint8_t *const VGA_BUFFER = CAST(uint8_t *const, 0xB8000);
static uint8_t        g_X        = 0;
static uint8_t        g_Y        = 0;

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

void scrputs(const char *__restrict__ s)
{
    while (*s)
    {
        scrputc(*s);
        ++s;
    }
}

void scrputsf(const char *__restrict__ fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    enum printf_state        state  = STATE_NORMAL;
    enum printf_length_state length = LENGTH_NORMAL;

    char num_buf[30];

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
            case '%': scrputc('%'); break;
            case 's': scrputs(va_arg(ap, const char *)); break;
            case 'c': scrputc(CAST(char, va_arg(ap, int))); break;
            case 'l':
                length = length == LENGTH_LONG ? LENGTH_VERY_LONG : LENGTH_LONG;
                ++fmt;
                continue;
            case 'h':
                length
                    = length == LENGTH_SHORT ? LENGTH_VERY_SHORT : LENGTH_SHORT;
                ++fmt;
                continue;
            case 'i':
            case 'd': SWITCH_LENGTH_SIGNED(ap, length, num_buf, 10); break;
            case 'u': SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 10); break;
            case 'x': SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 16); break;
            case 'p':
                scrputs("0x");
                SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 16);
                break;
            case 'o': SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 8); break;
            case 'b': SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 2); break;
            default: scrputsf("%%%c", *fmt);
            }
        }
        else
        {
            scrputc(*fmt);
        }

        state  = STATE_NORMAL;
        length = LENGTH_NORMAL;
        ++fmt;
    }

    va_end(ap);
}

void scrputc(char c)
{
    switch (c)
    {
    case '\n': ++g_Y; return;
    case '\r': g_X = 0; return;
    case '\b':
        if (g_X > 0)
            --g_X;
        return;
    }

    if (g_Y >= VGA_LENGTH)
        scrscroll();

    int position             = GET_VGA_POSITION_XY(g_X, g_Y);
    VGA_BUFFER[position]     = c;
    VGA_BUFFER[position + 1] = 0x0F;

    ++g_X;

    if (g_X >= VGA_WIDTH)
    {
        g_X = 0;
        ++g_Y;
    }
}

void scrclr()
{
    g_X = 0;
    g_Y = 0;

    for (int i = 0; i < VGA_LENGTH; i++)
    {
        for (int j = 0; j < VGA_WIDTH; j += 2)
        {
            VGA_BUFFER[i * VGA_WIDTH + j]     = '\0';
            VGA_BUFFER[i * VGA_WIDTH + j + 1] = 0x0F;
        }
    }
}

void scrscroll()
{
    memcopyovlp(
        VGA_BUFFER, VGA_BUFFER + VGA_WIDTH * 2, VGA_WIDTH * VGA_LENGTH * 2);
    memfill(VGA_BUFFER + VGA_WIDTH * (VGA_LENGTH - 1) * 2, 0, VGA_WIDTH);

    if (g_Y > 0)
        --g_Y;
}
