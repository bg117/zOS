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
; Filename: idt_impl.asm
; Description: Interrupt descriptor table helper routines for use with x86
;
; Date created: 270622DDMMYY2125HHmm
;
;

; --
; \brief Loads the specified IDT.
; \param[in] ebp+8  The pointer to the IDT descriptor.
; \return The pointer.
; --
global idtdescld
idtdescld:  push    ebp
            mov     ebp, esp

            mov     eax, [ebp + 8]
            lidt    [eax]

            mov     esp, ebp
            pop     ebp
            ret
