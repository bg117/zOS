/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxDISKHxuuq
#define quuxDISKHxuuq

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct disk
{
    uint8_t index;

    uint16_t cylinders;
    uint8_t  heads;
    uint8_t  sectors;

    uint8_t type;
};

int diskqueryinfo(struct disk *disk, uint8_t index);
int diskreset(struct disk *disk);

#ifdef __cplusplus
}
#endif

#endif
