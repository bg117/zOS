bits 16

section _ENTRY class=CODE

global _start
extern _cstart_

_start: ; initial setup (same as in bootloader)
        cli
        xor bx, bx
        mov ds, bx
        mov es, bx
        mov fs, bx
        mov gs, bx
        mov ss, bx

        mov sp, ax
        mov bp, sp
        sti

        xor     dh, dh
        push    dx          ; push drive number to stack

        call    _cstart_    ; pass the address to main

        jmp $
