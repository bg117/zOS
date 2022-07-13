/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <kernel/misc/num.h>
#include <kernel/misc/type_macros.h>

static const char POSSIBLE_CHARS[] = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";

char *int_to_string(char *buffer, int i, int base)
{
    return long_to_string(buffer, (long)i, base);
}

char *long_to_string(char *buffer, long l, int base)
{
    return long_long_to_string(buffer, (long long)l, base);
}

char *long_long_to_string(char *buffer, long long l, int base)
{
    char *rc;
    char *ptr;

    // check for supported base
    if (base < 2 || base > 36)
    {
        *buffer = '\0';
        return buffer;
    }
    rc = ptr = buffer;
    // set '-' for negative decimals
    if (l < 0 && base == 10)
        *ptr++ = '-';

    // the actual conversion
    do
    {
        *ptr++ = POSSIBLE_CHARS[35 + l % base];
        l /= base;
    } while (l);

    // terminate the string
    *ptr-- = '\0';

    // invert the numbers
    char *p = buffer;
    char *q = buffer;
    char  tmp;

    while (*q)
        q++;

    q--;
    while (p < q)
    {
        tmp  = *p;
        *p++ = *q;
        *q-- = tmp;
    }

    return rc;
}

char *uint_to_string(char *buffer, unsigned int i, int base)
{
    return ulong_to_string(buffer, (unsigned long)i, base);
}

char *ulong_to_string(char *buffer, unsigned long l, int base)
{
    return ulong_long_to_string(buffer, (unsigned long long)l, base);
}

char *ulong_long_to_string(char *buffer, unsigned long long l, int base)
{
    char *rc;
    char *ptr;

    // check for supported base
    if (base < 2 || base > 36)
    {
        *buffer = '\0';
        return buffer;
    }
    rc = ptr = buffer;

    // the actual conversion
    do
    {
        *ptr++ = POSSIBLE_CHARS[35 + l % base];
        l /= base;
    } while (l);

    // terminate the string
    *ptr-- = '\0';

    // invert the numbers
    char *p = buffer;
    char *q = buffer;
    char  tmp;

    while (*q)
        q++;

    q--;
    while (p < q)
    {
        tmp  = *p;
        *p++ = *q;
        *q-- = tmp;
    }

    return rc;
}
