/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <utils/chars.h>
#include <utils/strings.h>

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
