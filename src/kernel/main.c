#include <stddef.h>
#include <stdint.h>
#include "include/disk.h"

// _cstart_
__declspec(__cdecl) int cstart_(void *any)
{
    uint8_t *disk_ptr = (uint8_t *)any;

    struct Disk *i = getDisk(*disk_ptr);

    return 1;
}
