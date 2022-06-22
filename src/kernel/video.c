#include <stdarg.h>
#include <stddef.h>

#include <root/i.h>
#include <root/num.h>
#include <root/video.h>

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

enum PrintfState
{
    //
    STATE_NORMAL,
    STATE_SPECIFIER
};
enum PrintfLengthState
{
    //
    LENGTH_NORMAL,
    LENGTH_SHORT,
    LENGTH_VERY_SHORT,
    LENGTH_LONG,
    LENGTH_VERY_LONG
};

extern void *memset(void *ptr, int c, size_t n);

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

    char numBuf[30] = { 0 };

    while (*fmt)
    {
        memset(numBuf, 0, sizeof numBuf);
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
            case 'x': SWITCH_LENGTH_SIGNED(ap, length, numBuf, 16); break;
            case 'p':
                screenWriteString("0x");
                SWITCH_LENGTH_UNSIGNED(ap, length, numBuf, 16);
                break;
            case 'o': SWITCH_LENGTH_SIGNED(ap, length, numBuf, 8); break;
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
