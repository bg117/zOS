#ifndef quuxGDTHxuuq
#define quuxGDTHxuuq

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef uint8_t GdtAccessByte;

struct __attribute__((packed)) GdtEntry
{
    uint16_t      LimitLower16;
    uint16_t      BaseLower16;
    uint8_t       BaseMiddle8;
    GdtAccessByte AccessByte;
    uint8_t       LimitUpper4 : 4;
    uint8_t       Flags : 4;
    uint8_t       BaseUpper8;
};

#if defined(__cplusplus)
}
#endif

#endif
