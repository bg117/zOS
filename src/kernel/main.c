#include <stddef.h>
#include <stdint.h>
#include "include/disk.h"
#include "include/video.h"

// _cstart_
__declspec(__cdecl) int cstart_(void *any)
{
    char hi[] = "Hello world from kernel!!!";
    printString(hi);
    return 1;
}
