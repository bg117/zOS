/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stddef.h>

#include <misc/char_macros.h>
#include <misc/strings.h>

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

int str_is_equal(const char *s1, const char *s2)
{
    while (*s1 && *s2)
    {
        if (*s1 != *s2)
            return 0;

        ++s1;
        ++s2;
    }

    return 1;
}
