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
; Date created: 220621DDMMYY1502HHmm
;
;

section .text

; --
; \brief Fills len bytes of ptr with fill.
; \param[in] bp+4 The pointer to fill.
; \param[in] bp+6 The character to fill the pointer with.
; \param[in] bp+8 The length.
; \return The pointer.
; --
global memoryFill
memoryFill: push    bp
            mov     bp, sp          ; stack frame begin

            push    edi

            mov     ecx, [bp + 8]   ; get length
            and     ecx, 0xFFFF     ; some garbage values are on the upper 16 bits of EC

            ; first iteration: 4-byte STOS (fast)
            mov     eax, ecx
            mov     ecx, 4          ; divide first the length by 4
            div     ecx             ; remainder stored in EDX, which we'll (probably)
                                    ; use later
            mov     ecx, eax        ; move result back to ECX, for STOSD

            mov     eax, [bp + 6]   ; get fill character
            and     eax, 0xFF       ; clear upper 24 bits
            mov     edi, [bp + 4]   ; get pointer to operate on

            cld
            rep     stosd           ; first "memset" iteration

            ; second iteration: remaining, 1-byte STOS
            mov     ecx, edx        ; get remainder of division operation earlier
            cld
            rep     stosb           ; use STOSB for remaining

            mov     ax, [bp + 4]    ; return value (ptr)

            pop     edi

            mov     sp, bp          ; stack frame end
            pop     bp
            ret
