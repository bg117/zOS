/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <syslvl/i.h>
#include <syslvl/num.h>
#include <syslvl/video.h>

#define SWITCH_LENGTH_SIGNED(ap, len, buf, base)                               \
    do                                                                         \
    {                                                                          \
        switch (len)                                                           \
        {                                                                      \
        case LENGTH_NORMAL:                                                    \
            intToString(buf, va_arg(ap, int), base);                           \
            screenWriteFmtString("%s", buf);                                   \
            break;                                                             \
        case LENGTH_SHORT:                                                     \
            intToString(buf, CAST(short, va_arg(ap, int)), base);              \
            screenWriteFmtString("%s", buf);                                   \
            break;                                                             \
        case LENGTH_VERY_SHORT:                                                \
            intToString(buf, CAST(char, va_arg(ap, int)), base);               \
            screenWriteFmtString("%s", buf);                                   \
            break;                                                             \
        case LENGTH_LONG:                                                      \
            longToString(buf, va_arg(ap, long), base);                         \
            screenWriteFmtString("%s", buf);                                   \
            break;                                                             \
        case LENGTH_VERY_LONG:                                                 \
            longLongToString(buf, va_arg(ap, long long), base);                \
            screenWriteFmtString("%s", buf);                                   \
            break;                                                             \
        }                                                                      \
    } while (0)

#define SWITCH_LENGTH_UNSIGNED(ap, len, buf, base)                             \
    do                                                                         \
    {                                                                          \
        switch (len)                                                           \
        {                                                                      \
        case LENGTH_NORMAL:                                                    \
            uintToString(buf, va_arg(ap, unsigned int), base);                 \
            screenWriteFmtString("%s", buf);                                   \
            break;                                                             \
        case LENGTH_SHORT:                                                     \
            uintToString(                                                      \
                buf, CAST(unsigned short, va_arg(ap, unsigned int)), base);    \
            screenWriteFmtString("%s", buf);                                   \
            break;                                                             \
        case LENGTH_VERY_SHORT:                                                \
            uintToString(                                                      \
                buf, CAST(unsigned char, va_arg(ap, unsigned int)), base);     \
            screenWriteFmtString("%s", buf);                                   \
            break;                                                             \
        case LENGTH_LONG:                                                      \
            ulongToString(buf, va_arg(ap, unsigned long), base);               \
            screenWriteFmtString("%s", buf);                                   \
            break;                                                             \
        case LENGTH_VERY_LONG:                                                 \
            ulongLongToString(buf, va_arg(ap, unsigned long long), base);      \
            screenWriteFmtString("%s", buf);                                   \
            break;                                                             \
        }                                                                      \
    } while (0)

#define GET_VGA_POSITION_XY(x, y) (((y) * (VGA_WIDTH) + (x)) * 2)

static const uint8_t VGA_WIDTH  = 80;
static const uint8_t VGA_LENGTH = 25;

static uint8_t *const VGA_BUFFER = CAST(uint8_t *const, 0xB8000);
static uint8_t        g_X        = 0;
static uint8_t        g_Y        = 0;

enum PrintfState
{
    STATE_NORMAL,
    STATE_SPECIFIER
};

enum PrintfLengthState
{
    LENGTH_NORMAL,
    LENGTH_SHORT,
    LENGTH_VERY_SHORT,
    LENGTH_LONG,
    LENGTH_VERY_LONG
};

void screenWriteString(const char *__restrict__ s)
{
    while (*s)
    {
        screenWriteChar(*s);
        ++s;
    }
}

void screenWriteFmtString(const char *__restrict__ fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    enum PrintfState       state  = STATE_NORMAL;
    enum PrintfLengthState length = LENGTH_NORMAL;

    char numBuf[30];

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
            case '%': screenWriteChar('%'); break;
            case 's': screenWriteString(va_arg(ap, const char *)); break;
            case 'c': screenWriteChar(CAST(char, va_arg(ap, int))); break;
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
            case 'd': SWITCH_LENGTH_SIGNED(ap, length, numBuf, 10); break;
            case 'u': SWITCH_LENGTH_UNSIGNED(ap, length, numBuf, 10); break;
            case 'x': SWITCH_LENGTH_UNSIGNED(ap, length, numBuf, 16); break;
            case 'p':
                screenWriteString("0x");
                SWITCH_LENGTH_UNSIGNED(ap, length, numBuf, 16);
                break;
            case 'o': SWITCH_LENGTH_UNSIGNED(ap, length, numBuf, 8); break;
            case 'b': SWITCH_LENGTH_UNSIGNED(ap, length, numBuf, 2); break;
            default: screenWriteFmtString("%%%c", *fmt);
            }
        }
        else
        {
            screenWriteChar(*fmt);
        }

        state  = STATE_NORMAL;
        length = LENGTH_NORMAL;
        ++fmt;
    }

    va_end(ap);
}

void screenWriteChar(char c)
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

void screenClear()
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
