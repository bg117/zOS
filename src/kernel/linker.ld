ENTRY(_start)
. = 0x100000;

SECTIONS
{
    __start = .;

    . += 0xC0000000; /* higher half */

    __text_start = . - 0xC0000000;
    .text ALIGN(4K) : AT(ADDR(.text) - 0xC0000000)
    {
        *(.ld)
        *(.text)
    }

    __rodata_start = . - 0xC0000000;
    .rodata ALIGN(4K) : AT(ADDR(.rodata) - 0xC0000000) { *(.rodata) }

    __data_start = . - 0xC0000000;
    .data ALIGN(4K) : AT(ADDR(.data) - 0xC0000000) { *(.data) }

    __bss_start = . - 0xC0000000;
    .bss ALIGN(4K) : AT(ADDR(.bss) - 0xC0000000) { *(.bss) }

    __end = . - 0xC0000000;
}
