#include <stddef.h>
#include <stdint.h>
#include "include/disk.h"
#include "include/video.h"

// _cstart_
__declspec(__cdecl) int cstart_(void *any)
{
    uint8_t driveNumber = (uint8_t)any; // push DX

    clearScreen();
    printString("Loaded kernel.\r\n");
    printFmtString("Booting from drive index: %hhu\r\n", driveNumber);
    return 1;
}
