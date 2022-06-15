%ifndef quuxPRINTINCxuuq
%define quuxPRINTINCxuuq

; --
; \brief            Prints the string specified.
; \param[in]    SI  The string to print.
; --
print:  push si
        push ax

        .loop:  mov ah, 0x0E
                lodsb

                test    al, al
                jz      .end

                int 0x10

                jmp .loop

        .end:   pop ax
                pop si

                ret

%endif
