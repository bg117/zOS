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
; Filename: mem_impl.asm
; Description: Memory helper routines for use with x86
;
; Date created: 220622DDMMYY1502HHmm
;
;

section .text

; --
; \brief Fills len bytes of ptr with fill.
; \param[in] ebp+8  The pointer to fill.
; \param[in] ebp+12 The character to fill the pointer with.
; \param[in] ebp+16 The length.
; \return The pointer.
; --
global memfill
memfill:    push    ebp
            mov     ebp, esp        ; stack frame begin

            push    edi
            push    ebx

            mov     ecx, [ebp + 16] ; get length
            and     ecx, 0xFFFF     ; some garbage values are on the upper 16 bits of EC

            ; first iteration: 4-byte STOS (fast)
            mov     eax, ecx
            mov     ecx, 4          ; divide first the length by 4
            xor     edx, edx
            div     ecx             ; remainder stored in EDX, which we'll (probably)
                                    ; use later
            mov     ecx, eax        ; move result back to ECX, for STOSD

            mov     eax, [ebp + 12] ; get fill character
            and     eax, 0xFF       ; clear upper 24 bits
            mov     ah, al
            mov     bx, ax
            shl     ebx, 16
            and     ebx, 0xFFFF0000
            or      eax, ebx
            mov     edi, [ebp + 8]  ; get pointer to operate on

            cld
            rep     stosd           ; first "memset" iteration

            ; second iteration: remaining, 1-byte STOS
            mov     ecx, edx        ; get remainder of division operation earlier
            cld
            rep     stosb           ; use STOSB for remaining

            mov     eax, [ebp + 8]  ; return value (ptr)

            pop     ebx
            pop     edi

            mov     esp, ebp        ; stack frame end
            pop     ebp
            ret
