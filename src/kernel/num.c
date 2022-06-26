/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <syslvl/i.h>
#include <syslvl/num.h>

static const char POSSIBLE_CHARS[]
    = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";

char *intToString(char *buffer, int i, int base)
{
    return longToString(buffer, CAST(long, i), base);
}

char *longToString(char *buffer, long l, int base)
{
    return longLongToString(buffer, CAST(long long, l), base);
}

char *longLongToString(char *buffer, long long l, int base)
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

char *uintToString(char *buffer, unsigned int i, int base)
{
    return ulongToString(buffer, CAST(unsigned long, i), base);
}

char *ulongToString(char *buffer, unsigned long l, int base)
{
    return ulongLongToString(buffer, CAST(unsigned long long, l), base);
}

char *ulongLongToString(char *buffer, unsigned long long l, int base)
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
