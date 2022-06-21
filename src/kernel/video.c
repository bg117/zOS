#include "include/video.h"

void screenWriteString(const char *s)
{
    while (*s) {
        screenWriteChar(*s);
        ++s;
    }
}

void screenWriteFmtString(const char *restrict fmt, ...)
{
    screenWriteString(fmt);
}
