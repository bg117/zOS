#ifndef quuxDISKHxuuq
#define quuxDISKHxuuq

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Disk
{
    uint8_t Index;

    uint16_t Cylinders;
    uint8_t  Heads;
    uint8_t  Sectors;

    uint8_t Type;
};

int diskGetInfo(struct Disk *disk, uint8_t index);
int diskReset(struct Disk *disk);

#ifdef __cplusplus
}
#endif

#endif
