/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

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
