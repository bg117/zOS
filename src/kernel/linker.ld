ENTRY(_start)

PHYS = 0x00100000;
VIRT = 0xC0000000;

SECTIONS
{
    . = PHYS;

    _sprog = .;

    .ld ALIGN(4K) :
    {
        _sld = .;
        *(.ld)
        eld = .;
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

    _spmm_bitmap = .;
    . += 128K; /* the max size for a bitmap (4G) */
    _epmm_bitmap = .;

    _svmm_bitmap = .;
    . += 96K; /* VIRT / PAGE_SIZE / 8 */
    _evmm_bitmap = .;

    .symtab ALIGN(4K) : AT(ADDR(.symtab) - 0xC000)
    {
        _ssymtab = .;
        *(.symtab)
        _esymtab = .;
    }

    _eprog = . - VIRT;
}
