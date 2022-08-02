/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <utils/chars.h>
#include <utils/mem.h>
#include <utils/num.h>
#include <utils/strings.h>

#define SWITCH_LENGTH_SIGNED(ap, len, buf, base)                   \
    do                                                             \
    {                                                              \
        switch (len)                                               \
        {                                                          \
        case LENGTH_NORMAL:                                        \
            int_to_string(buf, va_arg(ap, int), base);             \
            break;                                                 \
        case LENGTH_SHORT:                                         \
            int_to_string(buf, (short)(va_arg(ap, int)), base);    \
            break;                                                 \
        case LENGTH_VERY_SHORT:                                    \
            int_to_string(buf, (char)(va_arg(ap, int)), base);     \
            break;                                                 \
        case LENGTH_LONG:                                          \
            long_to_string(buf, va_arg(ap, long), base);           \
            break;                                                 \
        case LENGTH_VERY_LONG:                                     \
            long_long_to_string(buf, va_arg(ap, long long), base); \
            break;                                                 \
        }                                                          \
                                                                   \
        size_t __length__ = str_get_length(buf);                   \
        if (zero_pad)                                              \
        {                                                          \
            size_t i = 0;                                          \
            for (; i < pad_len - __length__; i++, total_len++)     \
            {                                                      \
                SET_OUT_NN('0');                                   \
                INC_OUT_NN();                                      \
            }                                                      \
            if (i > 0)                                             \
            {                                                      \
                DEC_OUT_NN();                                      \
                --total_len;                                       \
            }                                                      \
        }                                                          \
                                                                   \
        if (__length__ > 0)                                        \
        {                                                          \
            size_t i = 0;                                          \
            for (; i < __length__; i++, total_len++)               \
            {                                                      \
                SET_OUT_NN(buf[i]);                                \
                INC_OUT_NN();                                      \
            }                                                      \
                                                                   \
            if (i > 0)                                             \
            {                                                      \
                DEC_OUT_NN();                                      \
                --total_len;                                       \
            }                                                      \
        }                                                          \
    } while (0)

#define SWITCH_LENGTH_UNSIGNED(ap, len, buf, base)                                 \
    do                                                                             \
    {                                                                              \
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
        size_t __length__ = str_get_length(buf);                                   \
        if (zero_pad)                                                              \
        {                                                                          \
            size_t i = 0;                                                          \
            for (; i < pad_len - __length__; i++, total_len++)                     \
            {                                                                      \
                SET_OUT_NN('0');                                                   \
                INC_OUT_NN();                                                      \
            }                                                                      \
            if (i > 0)                                                             \
            {                                                                      \
                DEC_OUT_NN();                                                      \
                --total_len;                                                       \
            }                                                                      \
        }                                                                          \
                                                                                   \
        if (__length__ > 0)                                                        \
        {                                                                          \
            size_t i = 0;                                                          \
            for (; i < __length__; i++, total_len++)                               \
            {                                                                      \
                SET_OUT_NN(buf[i]);                                                \
                INC_OUT_NN();                                                      \
            }                                                                      \
                                                                                   \
            if (i > 0)                                                             \
            {                                                                      \
                DEC_OUT_NN();                                                      \
                --total_len;                                                       \
            }                                                                      \
        }                                                                          \
    } while (0);

#define SWITCH_LENGTH_UNSIGNED_UPR(ap, len, buf, base)                             \
    do                                                                             \
    {                                                                              \
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
        size_t __length__ = str_get_length(buf);                                   \
        if (zero_pad)                                                              \
        {                                                                          \
            size_t i = 0;                                                          \
            for (; i < pad_len - __length__; i++, total_len++)                     \
            {                                                                      \
                SET_OUT_NN('0');                                                   \
                INC_OUT_NN();                                                      \
            }                                                                      \
            if (i > 0)                                                             \
            {                                                                      \
                DEC_OUT_NN();                                                      \
                --total_len;                                                       \
            }                                                                      \
        }                                                                          \
                                                                                   \
        if (__length__ > 0)                                                        \
        {                                                                          \
            size_t i = 0;                                                          \
            for (; i < __length__; i++, out++, total_len++)                        \
                SET_OUT_NN(buf[i]);                                                \
                                                                                   \
            if (i > 0)                                                             \
            {                                                                      \
                DEC_OUT_NN();                                                      \
                --total_len;                                                       \
            }                                                                      \
        }                                                                          \
    } while (0)

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

char *str_to_upper(char *str)
{
    char *p = str;
    while (*p)
    {
        if (ISLOWER(*p))
            *p -= ' ';

        ++p;
    }

    return str;
}

char *str_to_lower(char *str)
{
    char *p = str;
    while (*p)
    {
        if (ISUPPER(*p))
            *p += ' ';

        ++p;
    }

    return str;
}

size_t str_get_length(const char *str)
{
    size_t i = 0;
    for (; *str; i++, ++str)
        ;

    return i;
}

int str_compare(const char *s1, const char *s2)
{
    const uint8_t *us1 = (const uint8_t *)s1;
    const uint8_t *us2 = (const uint8_t *)s2;

    while (*us1 == *us2 && *us1 != '\0')
    {
        us1++;
        us2++;
    }

    return (*us1 > *us2) - (*us1 < *us2);
}

const char *str_get_first_occ(const char *s, char c)
{
    if (c == '\0')
        return s + str_get_length(s);

    size_t i = 0;
    while (s[i] != '\0')
    {
        if (s[i] == c)
            return s + i;
    }

    return NULL;
}

static volatile bool g_always_true = true;

const char *str_get_last_occ(const char *s, char c)
{
    size_t i = str_get_length(s);
    if (c == '\0')
        return s + i;

    while (g_always_true)
    {
        if (s[i] == c)
            return s + i;

        if (i == 0)
            break;

        --i;
    }

    if (i == 0 && s[i] != c)
        return NULL;

    return s + i;
}

size_t str_format(const char *fmt, char *out, ...)
{
    va_list ap;
    va_start(ap, out);

    size_t ret = str_vformat(fmt, out, ap);

    va_end(ap);

    return ret;
}

#define SET_OUT_NN(c) \
    if (out != NULL)  \
    *out = (c)
#define INC_OUT_NN() \
    if (out != NULL) \
    ++out
#define DEC_OUT_NN() \
    if (out != NULL) \
    --out

size_t str_vformat(const char *fmt, char *out, va_list ap)
{
    size_t                   total_len = 0;
    enum printf_state        state     = STATE_NORMAL;
    enum printf_length_state length    = LENGTH_NORMAL;

    int    zero_pad  = 0;
    int    space_pad = 0;
    size_t pad_len   = 0;

    char num_buf[256];

    while (*fmt)
    {
        mem_fill32(num_buf, 0, 256 / 4);
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
                SET_OUT_NN('%');
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
                    size_t      len = str_get_length(arg);
                    if (space_pad)
                    {
                        size_t i = 0;
                        for (; i < pad_len - len; i++, total_len++)
                        {
                            SET_OUT_NN(' ');
                            INC_OUT_NN();
                        }

                        if (i > 0)
                        {
                            DEC_OUT_NN();
                            --total_len;
                        }
                    }

                    if (len == 0)
                        break;

                    size_t i = 0;
                    for (; i < len; i++, total_len++)
                    {
                        SET_OUT_NN(arg[i]);
                        INC_OUT_NN();
                    }

                    if (i > 0)
                    {
                        DEC_OUT_NN();
                        --total_len;
                    }
                }
                break;
            case 'c':
                SET_OUT_NN((char)va_arg(ap, int));
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
                SET_OUT_NN('0');
                INC_OUT_NN();
                ++total_len;
                SET_OUT_NN('x');
                SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 16);
                break;
            case 'o':
                SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 8);
                break;
            case 'b':
                SWITCH_LENGTH_UNSIGNED(ap, length, num_buf, 2);
                break;
            default:
                SET_OUT_NN('%');
                INC_OUT_NN();
                ++total_len;
                SET_OUT_NN(*fmt);
                break;
            }
        }
        else
        {
            SET_OUT_NN(*fmt);
        }

        state     = STATE_NORMAL;
        length    = LENGTH_NORMAL;
        zero_pad  = 0;
        space_pad = 0;
        pad_len   = 0;
        ++fmt;
        ++total_len;
        INC_OUT_NN();
    }

    return total_len;
}
