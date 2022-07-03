; Copyright (c) 2022 bg117
;
; This software is released under the MIT License.
; https://opensource.org/licenses/MIT

bits 32

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

section .ld

global _start
extern __bss_start
extern __start
extern __end
extern kmain

_start: ; initial setup (same as in bootloader)
        mov     esp, 0x10000    ; gives approx. 64 KiB of stack
        mov     ebp, esp

        ; clear .bss
        push    ecx
        push    edi

        mov     ecx, __end
        mov     edi, __bss_start

        sub     ecx, edi        ; subtract EDI from ECX (giving the total size of the bss section)
        cld

        rep     stosb

        pop     edi
        pop     ecx

        ; __cdecl passes args from right to left
        xor     dh, dh
        push    ecx
        push    ebx
        push    edi
        push    edx

        call    kmain

        add     esp, 16

        jmp     $
