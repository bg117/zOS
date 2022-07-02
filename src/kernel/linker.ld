ENTRY(_start)
. = 0x100000;

SECTIONS
{
    __start = .;

    __text_start = .;
    .text :
    {
        *(.ld)
        *(.text)
    }

    __rodata_start = .;
    .rodata : { *(.rodata) }

    __data_start = .;
    .data : { *(.data) }

    __bss_start = .;
    .bss : { *(.bss) }

    __end = .;
}
