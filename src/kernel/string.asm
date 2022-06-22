bits 16

;**************************
;                         *
;   x86 helper routines   *
;                         *
;**************************
;
; Filename: string.S
; Description: String helper routines for use with x86
;
; Date created: 220621DDMMYY1502HHmm
;
;

section .text

; --
; \brief Fills the first n bytes of ptr with c.
; \param[in] bp+4 The pointer to fill.
; \param[in] bp+6 The character to fill the pointer with.
; \param[in] bp+8 The length.
; \return The pointer.
; --
global memset
memset: push    bp
        mov     bp, sp          ; Stack frame begin

        push    edi

        mov     ecx, [bp + 8]
        and     ecx, 0xFFFF
        mov     eax, ecx
        mov     ecx, 4
        div     ecx
        mov     ecx, eax

        mov     eax, [bp + 6]
        and     eax, 0xFF
        mov     edi, [bp + 4]

        cld
        rep     stosd

        mov     ecx, edx
        cld
        rep     stosb

        mov     ax, [bp + 4]      ; Return value (ptr)

        pop     edi

        mov     sp, bp          ; Stack frame end
        pop     bp
        ret
