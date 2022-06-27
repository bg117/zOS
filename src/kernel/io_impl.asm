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
; Filename: io_impl.asm
; Description: Input and output helper routines for use with x86
;
; Date created: 250621DDMMYY1400HHmm
;
;

; --
; \brief Calls the IN instruction (byte). Gets data from the port specified.
; \param[in] ebp+8 The port to retrieve data from.
; \return The data received from the port.
; --
global inByte
inByte: push    ebp
        mov     ebp, esp

        mov     dx, [ebp + 8]
        in      al, dx

        and     eax, 0xFF       ; retrieve only the lower 8 bits

        mov     esp, ebp
        pop     ebp
        ret

; --
; \brief Calls the IN instruction (word). Gets data from the port specified.
; \param[in] ebp+8 The port to retrieve data from.
; \return The data received from the port.
; --
global inWord
inWord: push    ebp
        mov     ebp, esp

        mov     dx, [ebp + 8]
        in      ax, dx

        and     eax, 0xFFFF     ; retrieve only the lower 8 bits

        mov     esp, ebp
        pop     ebp
        ret

; --
; \brief Calls the OUT instruction (byte). Writes data to the port specified.
; \param[in] ebp+8  The port to write data to.
; \param[in] ebp+12 The data to write to the port.
; \return The data received from the port.
; --
global outByte
outByte:    push    ebp
            mov     ebp, esp

            mov     dx, [ebp + 8]
            mov     ax, [ebp + 12]
            out     dx, al

            mov     esp, ebp
            pop     ebp
            ret

; --
; \brief Calls the OUT instruction (word). Writes data to the port specified.
; \param[in] ebp+8  The port to write data to.
; \param[in] ebp+12 The data to write to the port.
; \return The data received from the port.
; --
global outWord
outWord:    push    ebp
            mov     ebp, esp

            mov     dx, [ebp + 8]
            mov     ax, [ebp + 12]
            out     dx, ax

            mov     esp, ebp
            pop     ebp
            ret

global __defExcept
__defExcept:    cli
                jmp     $
