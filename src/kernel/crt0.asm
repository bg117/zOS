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
extern __end
extern main

_start: ; initial setup (same as in bootloader)
        mov     esp, eax
        mov     ebp, esp

        ; clear .bss
        push    edi
        mov     ecx, __end
        mov     edi, __bss_start

        sub     ecx, edi        ; subtract EDI from ECX (giving the total size of the bss section)
        cld

        rep     stosb
        pop     edi

        push    edx             ; save data register

        ; __cdecl passes args from right to left
        xor     dh, dh
        mov     ebx, ARGV       ; base register
        mov     [ebx], edx      ; index 0
        mov     [ebx + 4], edi  ; index 1

        push    ARGV            ; push argv to stack

        mov     ecx, 2
        push    ecx             ; lastly, push argc

        call    main

        pop     edx             ; pop args count
        pop     edx             ; pop drive number
        pop     edx             ; pop BPB
        pop     edx             ; pop EBR

        pop     edx             ; bring back original value of DX

        jmp     $

section .data
ARGV:
