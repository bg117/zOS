#ifndef quuxVIDEOHxuuq
#define quuxVIDEOHxuuq

#include "priv/xhdr.h"

#ifdef __cplusplus
extern "C" {
#endif

__declspec(__cdecl) void clearScreen(void);
__declspec(__cdecl) void printChar(char c);
__declspec(__cdecl) void printString(const char *s);
__declspec(__cdecl) void printFmtString(const char *restrict fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
