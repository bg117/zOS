bits 16

;**************************
;                         *
;   x86 helper routines   *
;                         *
;**************************
;
; Filename: vga.S
; Description: VGA helper routines for use with x86
;
; Date created: 200621DDMMYY1723HHmm
;
;

section .text

; --
; \brief Clears the screen.
; --
global screenClear
screenClear:    push    bp
                mov     bp, sp

                mov     ax, 0x0003
                int     0x10

                mov     sp, bp
                pop     bp
                ret

; --
; \brief Writes a single character to the screen.
; \param[in] bp+4 The character to write.
; --
global screenWriteChar
screenWriteChar:    push    bp
                    mov     bp, sp

                    mov     ah, 0x0E
                    mov     al, [bp + 4]
                    int     0x10

                    mov     sp, bp
                    pop     bp
                    ret
