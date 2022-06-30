/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

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
