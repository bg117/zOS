%ifndef quuxCHSINCxuuq
%define quuxCHSINCxuuq

; --
; \brief            Converts LBA to CHS format.
; \param[in]    AX  The LBA of the sector.
; \param[out]   CX  (CL, bits 0-5) Sector number.
;                   (CL, bits 6-7) Upper 2 bits of track number.
;                   (CH) Lower 8 bits of track number.
; \param[out]   DH  Head index.
; \remarks          This assumes that the symbol bios_parameter_block is defined.
; --
lba_to_chs: ; save clobbered registers
            push    ax
            push    dx      ; save DL

            div     word [bios_parameter_block.sectors_per_track]   ; tmp = lba / spt
            inc     dx                                              ; sec = lba % spt + 1
            mov     cx, dx

            xor     dx, dx
            div     word [bios_parameter_block.heads]   ; head = tmp % heads
                                                        ; cyl  = tmp / heads
            mov     dh, dl      ; head
            mov     ch, al      ; lower 8 bits of track
            shl     ah, 6       ; AL = lower 8 bits of track
                                ; AH = upper 8 bits
            or      cl, ah

            pop     ax      ; get DL
            mov     dl, al
            pop     ax      ; pop the previous AL

            ret

%endif
