; Copyright (c) 2022 bg117
;
; This software is released under the MIT License.
; https://opensource.org/licenses/MIT

bits 32

;**************************
;                         *
;   x86 helper routines   *
;                         *
;**************************
;
; Filename: gdt_impl.asm
; Description: Global descriptor table helper routines for use with x86
;
; Date created: 270621DDMMYY1852HHmm
;
;

; --
; \brief Loads the specified GDT.
; \param[in] ebp+8  The pointer to the GDT descriptor.
; \param[in] ebp+12 The offset to the code segment in the GDT.
; \param[in] ebp+16 The offset to the data segment in the GDT.
; \return The pointer.
; --
global gdtLoadDescriptor
gdtLoadDescriptor:  push    ebp
                    mov     ebp, esp

                    mov     eax, [ebp + 8]
                    lgdt    [eax]

                    mov     eax, [ebp + 12]
                    push    eax
                    push    .reload_cs
                    retf

                    .reload_cs:

                    mov     eax, [ebp + 16]
                    mov     ds, ax
                    mov     es, ax
                    mov     fs, ax
                    mov     gs, ax
                    mov     ss, ax

                    mov     esp, ebp
                    pop     ebp
                    ret
