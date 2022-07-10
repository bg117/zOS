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

%define KERNEL_SYM_PHYS(x) ((x) - 0xC0000000)

section .ld

global _start
global SYS_PGDIR
extern __bss_start
extern __start
extern __data_start
extern __end
extern kmain
extern mem_copy

_start: ; initial setup (same as in bootloader)
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

        and     edx, 0xFF
        mov     [KERNEL_SYM_PHYS(SMEM_MAP_LENGTH)], ecx
        mov     [KERNEL_SYM_PHYS(SDRIVE_NUMBER)], edx

        ; copy structs
        push    ecx
        push    62
        push    edi
        push    KERNEL_SYM_PHYS(SFAT_INFO)
        call    KERNEL_SYM_PHYS(mem_copy)
        add     esp, 12
        pop     ecx

        mov     eax, ecx
        mov     ecx, 24
        mul     ecx
        push    eax
        push    ebx
        push    KERNEL_SYM_PHYS(SMEM_MAP)
        call    KERNEL_SYM_PHYS(mem_copy)
        add     esp, 12

        ; set up paging
        mov     edi, KERNEL_SYM_PHYS(SYS_PGTAB)
        mov     esi, 0
        mov     ecx, 1023

        .1:     cmp     esi, __start
                jb      .2

                cmp     esi, __end
                jae     .3

                cmp     esi, __data_start ; encompasses .text and .rodata
                jb      .readonly

                mov     edx, esi
                or      edx, 0x03   ; read-write, present
                mov     es:[edi], edx
                jmp     .2

                .readonly:  mov     edx, esi
                            or      edx, 0x01
                            mov     es:[edi], edx

        .2:     add     esi, 0x1000
                add     edi, 4

                loop    .1

        .3:     mov     [KERNEL_SYM_PHYS(SYS_PGTAB) + 1023 * 4], dword 0xB8000 | 0x03

                ; identity map kernel first
                mov     [KERNEL_SYM_PHYS(SYS_PGDIR) + 0 * 4], dword KERNEL_SYM_PHYS(SYS_PGTAB) + 0x03
                
                ; map to 0xC0000000
                mov     [KERNEL_SYM_PHYS(SYS_PGDIR) + 768 * 4], dword KERNEL_SYM_PHYS(SYS_PGTAB) + 0x03

                ; map page directory recursively
                mov     [KERNEL_SYM_PHYS(SYS_PGDIR) + 1023 * 4], dword KERNEL_SYM_PHYS(SYS_PGDIR) + 0x03

                mov     ecx, KERNEL_SYM_PHYS(SYS_PGDIR)
                mov     cr3, ecx

                mov     ecx, cr0
                or      ecx, 0x80000000
                mov     cr0, ecx

                mov     ecx, [KERNEL_SYM_PHYS(SMEM_MAP_LENGTH)]
                mov     edx, [KERNEL_SYM_PHYS(SDRIVE_NUMBER)]

                mov     ebx, .after_setup
                jmp     ebx

section .text
        .after_setup:   mov     [SYS_PGDIR + 0 * 4], dword 0

                        ; flush TLB
                        mov     ecx, cr3
                        mov     cr3, ecx

                        mov     ebp, STACK_TOP
                        mov     esp, ebp

                        mov     ebx, SMEM_MAP
                        mov     edi, SFAT_INFO

                        ; __cdecl passes args from right to left
                        push    dword [SMEM_MAP_LENGTH]
                        push    SMEM_MAP
                        push    SFAT_INFO
                        push    dword [SDRIVE_NUMBER]

                        call    kmain

                        add     esp, 16

                        jmp     $

section .bss
alignb 0x1000

SYS_PGDIR:  resq 512
SYS_PGTAB:  resq 512

SFAT_INFO:  resq 8

SMEM_MAP:   resq 192 ; enough space for 64 entries

SMEM_MAP_LENGTH:    resd 1

SDRIVE_NUMBER:      resd 1

alignb 0x1000
resq 0x1000
STACK_TOP:

