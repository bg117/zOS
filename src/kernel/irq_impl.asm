bits 32

;**************************
;                         *
;   x86 helper routines   *
;                         *
;**************************
;
; Filename: irq_impl.asm
; Description: Interrupt & interrupt request helper routines for use with x86
;
; Date created: 250621DDMMYY1400HHmm
;
;

global inByte
inByte: push    ebp
        mov     ebp, esp

        mov     dx, [ebp + 8]
        in      al, dx

        and     0xFF, eax       ; retrieve only the lower 8 bits

        mov     esp, ebp
        pop     ebp
        ret

global inWord
inWord: push    ebp
        mov     ebp, esp

        mov     dx, [ebp + 8]
        in      ax, dx

        and     0xFFFF, eax       ; retrieve only the lower 8 bits

        mov     esp, ebp
        pop     ebp
        ret

global outByte
outByte:    push    ebp
            mov     ebp, esp

            mov     dx, [ebp + 8]
            out     dx, al

            mov     esp, ebp
            pop     ebp
            ret

global outWord
outWord:    push    ebp
            mov     ebp, esp

            mov     dx, [ebp + 8]
            out     dx, ax

            mov     esp, ebp
            pop     ebp
            ret
