; Copyright (c) 2022 bg117
;
; This software is released under the MIT License.
; https://opensource.org/licenses/MIT

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

extern _sbss, _ebss
extern _sprog, _eprog
extern _sdata

extern kmain

extern mem_copy
extern mem_fill

_start: ; initial setup (same as in bootloader)
        ; clear .bss
        push    ecx
        push    edi

        mov     ecx, KERNEL_SYM_PHYS(_ebss)
        mov     edi, KERNEL_SYM_PHYS(_sbss)

        sub     ecx, edi        ; subtract EDI from ECX (giving the total size of the bss section)

        push    ecx
        push    dword 0
        push    edi
        call    KERNEL_SYM_PHYS(mem_fill)
        add     esp, 12

        pop     edi
        pop     ecx

        and     edx, 0xFF
        mov     [KERNEL_SYM_PHYS(SMEM_MAP_LENGTH)], ecx
        mov     [KERNEL_SYM_PHYS(SDRIVE_NUMBER)], edx

        push    dword 0x1000
        push    esi
        push    KERNEL_SYM_PHYS(SFONT_INFO)
        call    KERNEL_SYM_PHYS(mem_copy)
        add     esp, 12

        ; copy structs
        push    ecx
        push    dword 62
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

        .1:     cmp     esi, _sprog
                jb      .2

                cmp     esi, _eprog
                jae     .3

                cmp     esi, KERNEL_SYM_PHYS(_sdata) ; encompasses .text and .rodata
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

        .3:     ; identity map kernel first
                mov     [KERNEL_SYM_PHYS(SYS_PGDIR) + 0 * 4], dword KERNEL_SYM_PHYS(SYS_PGTAB) + 0x03

                ; map to 0xC0000000
                mov     [KERNEL_SYM_PHYS(SYS_PGDIR) + 768 * 4], dword KERNEL_SYM_PHYS(SYS_PGTAB) + 0x03

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
        .after_setup:   ; map page directory recursively
                        mov     [SYS_PGDIR + 1023 * 4], dword KERNEL_SYM_PHYS(SYS_PGDIR) + 0x03

                        mov     edx, 160 * 4 ; 0x000A0000
                        mov     ecx, 191 * 4 ; 0x000BF000
                        mov     eax, 0xA0000
                        mov     ebx, SYS_PGTAB_VGA
                        add     ebx, edx

                        .identity_map_vga:  or      eax, 0x03
                                            mov     [ebx], eax
                                            cmp     edx, ecx
                                            jae     .map_end

                                            add     edx, 4
                                            add     ebx, 4
                                            add     eax, 0x1000
                                            jmp     .identity_map_vga

                        .map_end:

                        mov     [SYS_PGDIR + 0 * 4], dword KERNEL_SYM_PHYS(SYS_PGTAB_VGA) + 0x03

                        ; flush TLB
                        mov     ecx, cr3
                        mov     cr3, ecx

                        mov     ebp, STACK_TOP
                        mov     esp, ebp

                        mov     ebx, SMEM_MAP
                        mov     edi, SFAT_INFO

                        ; __cdecl passes args from right to left
                        push    SFONT_INFO
                        push    dword [SMEM_MAP_LENGTH]
                        push    SMEM_MAP
                        push    SFAT_INFO
                        push    dword [SDRIVE_NUMBER]

                        call    kmain

                        add     esp, 20

                        jmp     $

section .bss
alignb 0x1000

SYS_PGDIR:          resq 512
SYS_PGTAB:          resq 512
SYS_PGTAB_VGA:      resq 512

SMEM_MAP_LENGTH:    resd 1
SFAT_INFO:          resb 62
SMEM_MAP:           resq 192 ; enough space for 64 entries
SFONT_INFO:         resq 512 ; 4096 bytes; 256 glyphs
SDRIVE_NUMBER:      resd 1

alignb 0x1000
resq 0x1000
STACK_TOP:

