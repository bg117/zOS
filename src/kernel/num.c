#include "include/num.h"

static const char POSSIBLE_CHARS[] = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";

char *intToString(char *buffer, int i, int base)
{
    return longToString(buffer, (long)i, base);
}

char *longToString(char *buffer, long l, int base)
{
    return longLongToString(buffer, (long long)l, base);
}

char *longLongToString(char *buffer, long long l, int base)
{
    char *rc;
    char *ptr;

    // Check for supported base.
    if (base < 2 || base > 36) {
        *buffer = '\0';
        return buffer;
    }
    rc = ptr = buffer;
    // Set '-' for negative decimals.
    if (l < 0 && base == 10) {
        *ptr++ = '-';
    }

    // The actual conversion.
    do {
        // Modulo is negative for negative value. This trick makes abs()
        // unnecessary.
        // *ptr++ = POSSIBLE_CHARS[35 + l % base];
        // l /= base;
    }
    while (l);

    // Terminate the string.
    *ptr-- = '\0';

    // Invert the numbers.
    char *p = buffer;
    char *q = buffer;
    char  tmp;

    while (*q)
        q++;

    q--;
    while (p < q) {
        tmp  = *p;
        *p++ = *q;
        *q-- = tmp;
    }

    return rc;
}

char *uintToString(char *buffer, unsigned int i, int base)
{
    return ulongToString(buffer, (unsigned long)i, base);
}

char *ulongToString(char *buffer, unsigned long l, int base)
{
    return ulongLongToString(buffer, (unsigned long long)l, base);
}

char *ulongLongToString(char *buffer, unsigned long long l, int base)
{
    char *rc;
    char *ptr;

    // Check for supported base.
    if (base < 2 || base > 36) {
        *buffer = '\0';
        return buffer;
    }
    rc = ptr = buffer;

    // The actual conversion.
    do {
        // Modulo is negative for negative value. This trick makes abs()
        // unnecessary.
        //*ptr++ = POSSIBLE_CHARS[35 + l % base];
        // l /= base;
    }
    while (l);

    // Terminate the string.
    *ptr-- = '\0';

    // Invert the numbers.
    char *p = buffer;
    char *q = buffer;
    char  tmp;

    while (*q)
        q++;

    q--;
    while (p < q) {
        tmp  = *p;
        *p++ = *q;
        *q-- = tmp;
    }

    return rc;
}
