bits 16

;*****************************
;                            *
;   C runtime startup code   *
;                            *
;*****************************
;
; Filename: crt0.asm
; Description: Custom startup code for kernel
;
; Date created: 200622DDMMYY1905HHmm
;
;

section .text

global _start
extern __bss_start
extern __end
extern main

_start: ; initial setup (same as in bootloader)
        cli
        mov     sp, ax
        mov     bp, sp

        xor     ax, ax
        mov     ds, ax
        mov     es, ax
        mov     fs, ax
        mov     gs, ax
        mov     ss, ax
        sti

        ; clear .bss
        push    edi
        mov     ecx, __end
        mov     edi, __bss_start

        sub     ecx, edi        ; subtract EDI from ECX (giving the total size of the bss section)
        cld

        rep     stosb
        pop     edi

        push    dx              ; save data register

        ; __cdecl passes args from right to left
        xor     dh, dh
        mov     bx, ARGV        ; base register
        mov     [bx], dx        ; index 0
        mov     [bx + 2], di    ; index 1

        push    ARGV            ; push argv to stack

        mov     cx, 2
        push    cx              ; lastly, push argc

        call    main

        pop     dx              ; pop args count
        pop     dx              ; pop drive number
        pop     dx              ; pop BPB
        pop     dx              ; pop EBR

        pop     dx              ; bring back original value of DX

        jmp     $

ARGV:
