/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <misc/num.h>
#include <misc/type_macros.h>

static const char POSSIBLE_CHARS[]
    = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";

char *inttos(char *buffer, int i, int base)
{
    return longtos(buffer, CAST(long, i), base);
}

char *longtos(char *buffer, long l, int base)
{
    return llongtos(buffer, CAST(long long, l), base);
}

char *llongtos(char *buffer, long long l, int base)
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

char *uinttos(char *buffer, unsigned int i, int base)
{
    return ulongtos(buffer, CAST(unsigned long, i), base);
}

char *ulongtos(char *buffer, unsigned long l, int base)
{
    return ullongtos(buffer, CAST(unsigned long long, l), base);
}

char *ullongtos(char *buffer, unsigned long long l, int base)
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
