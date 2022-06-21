ENTRY(_start)
. = 0x1000;

SECTIONS
{
    __text_start = .;
    .text : { *(.text) }

    __rodata_start = .;
    .rodata : { *(.rodata) }

    __data_start = .;
    .data : { *(.data) }

    __bss_start = .;
    .bss : { *(.bss) }

    __end = .;
}
