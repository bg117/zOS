ENTRY(_start)
. = 0x100000;

SECTIONS
{
    __start = .;
    .ld ALIGN(4K) : { *(.ld) }

    . += 0xC0000000; /* higher half */

    __text_start = . - 0xC0000000;
    .text ALIGN(4K) : AT(ADDR(.text) - 0xC0000000) { *(.text) }

    __rodata_start = . - 0xC0000000;
    .rodata ALIGN(4K) : AT(ADDR(.rodata) - 0xC0000000) { *(.rodata) }

    __data_start = . - 0xC0000000;
    .data ALIGN(4K) : AT(ADDR(.data) - 0xC0000000) { *(.data) }

    __bss_start = . - 0xC0000000;
    .bss ALIGN(4K) : AT(ADDR(.bss) - 0xC0000000) { *(.bss) }

    . = ALIGN(4K);

    __pmm_bitmap_start = . - 0xC0000000;
    __pmm_bitmap_start_virt = .;
    . += 128K; /* the max size for a bitmap (4G) */

    __vmm_bitmap_start = . - 0xC0000000; /* will probably never be used; just for consistency */
    __vmm_bitmap_start_virt = .;
    . += 98304; /* 0xC0000000 / PAGE_SIZE / 8 */

    __end = . - 0xC0000000;
}
