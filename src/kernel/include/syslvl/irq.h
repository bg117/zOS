#ifndef quuxIRQHxuuq
#define quuxIRQHxuuq

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

struct __attribute__((packed)) IrqEntry
{
};

uint8_t inByte(uint8_t port);
void    outByte(uint8_t port);

uint16_t inWord(uint16_t port);
void     outWord(uint16_t port);

#if defined(__cplusplus)
}
#endif

#endif
