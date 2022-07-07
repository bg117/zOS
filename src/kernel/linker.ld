ENTRY(_start)
. = 0x100000;

SECTIONS
{
    __start = .;

    __text_start = .;
    .text : ALIGN(4K)
    {
        *(.ld)
        *(.text)
    }

    __rodata_start = .;
    .rodata : ALIGN(4K) { *(.rodata) }

    __data_start = .;
    .data : ALIGN(4K) { *(.data) }

    __bss_start = .;
    .bss : ALIGN(4K) { *(.bss) }

    __end = .;
}
