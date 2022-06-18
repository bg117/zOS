bits 16

section _TEXT class=CODE

global _clearScreen
_clearScreen:   push    bp
                mov     bp, sp

                mov     ah, 0x00
                mov     al, 0x03
                int     0x10

                mov sp, bp
                pop bp
                ret

global _printChar
_printChar: push    bp
            mov     bp, sp

            mov ah, 0x0E
            mov al, [bp + 4]
            int 0x10

            mov sp, bp
            pop bp
            ret
