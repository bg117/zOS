ENTRY(_start)
. = 0x1FC0;

SECTIONS
{
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
