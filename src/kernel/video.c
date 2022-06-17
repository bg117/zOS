#include "include/video.h"

__declspec(__cdecl) void printString(const char *s)
{
    while (*s) {
        printChar(*s);
        ++s;
    }
}
