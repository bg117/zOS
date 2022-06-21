#ifndef quuxDISKHxuuq
#define quuxDISKHxuuq

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Disk {
    uint8_t index;

    uint16_t cylinders;
    uint16_t heads;
    uint16_t sectors;
};

void diskGetInfo(struct Disk *disk, uint8_t index);
void diskReset(struct Disk *disk);

#ifdef __cplusplus
}
#endif

#endif
