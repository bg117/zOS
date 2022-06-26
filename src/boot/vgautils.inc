%ifndef quuxPRINTINCxuuq
%define quuxPRINTINCxuuq

vgautils:

; --
; \brief            Prints the string specified.
; \param[in]    SI  The string to print.
; --
.print: push    si
        push    ax

        .p_loop:    mov     ah, 0x0E
                    lodsb

                    test    al, al
                    jz      .p_end

                    int     0x10

                    jmp     .p_loop

        .p_end:     pop     ax
                    pop     si

                    ret

; --
; \brief    Clears the console.
; --
.clear: mov     ax, 0x0003  ; AH=0x00, AL=0x03
        int     0x10
        ret

%endif
