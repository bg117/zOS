ENTRY(startup)

PHYS = 0x00100000;
VIRT = 0xC0000000;

SECTIONS
{
    . = PHYS;

    _sprog = .;

    .startup ALIGN(4K) :
    {
        _sstartup = .;
        *(.startup)
        _estartup = .;
    }

    . += VIRT; /* higher half */

    .text ALIGN(4K) : AT(ADDR(.text) - VIRT)
    {
        _stext = .;
        *(.text)
        _etext = .;
    }

    .rodata ALIGN(4K) : AT(ADDR(.rodata) - VIRT)
    {
        _srodata = .;
        *(.rodata)
        _erodata = .;
    }

    .data ALIGN(4K) : AT(ADDR(.data) - VIRT)
    {
        _sdata = .;
        *(.data)
        _edata = .;
    }

    .bss ALIGN(4K) : AT(ADDR(.bss) - VIRT)
    {
        _sbss = .;
        *(.bss)
        _ebss = .;
    }

    . = ALIGN(4K);

    _spmm_bitmap = .;
    . += 128K; /* the max size for a bitmap (4G) */
    _epmm_bitmap = .;

    _eprog = . - VIRT;
}
