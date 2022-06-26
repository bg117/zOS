%ifndef quuxDISKINCxuuq
%define quuxDISKINCxuuq

diskutils:

; --
; \brief                Reads a sector (or sectors) from the specified LBA.
; \param[in]        AX  The LBA of the sector.
; \param[in]        CL  Number of sectors to read.
; \param[in]        DL  Index of drive to read from.
; \param[in]        ES  Segment for BX.
; \param[in]        BX  Memory location to store read data from sector(s).
; \remarks              Assumes that the symbol lba_to_chs is defined.
;                       Also, clears CF if succeeded, and sets it if failed.
; --
.read_sectors:  push    ax
                push    dx
                push    di

                push    cx
                call    convutils.lba_to_chs
                pop     ax

                pusha
                mov     di, 3

                .rs_retry:  stc     ; some BIOSes are stupid and don't set the CF
                            mov     ah, 0x02
                            int     0x13
                            pushf   ; test modifies FLAGS

                            jnc     .rs_end

                            mov     ah, 0x00
                            int     0x13

                            test    di, di
                            je      .rs_end

                            popf
                            dec     di
                            jmp     .rs_retry

                .rs_end:    popf
                            popa
                            pop     di
                            pop     dx
                            pop     ax
                            ret

; --
; \brief                Writes a sector (or sectors) from the specified LBA.
; \param[in]        AX  The LBA of the sector.
; \param[in]        CL  Number of sectors to write.
; \param[in]        DL  Index of drive to write to.
; \param[in]        ES  Segment for BX.
; \param[in]        BX  Memory location that contains data to write to sector(s).
; \remarks              Assumes that the symbol lba_to_chs is defined.
;                       Also, clears CF if succeeded, and sets it if failed.
; --
.write_sectors: push    ax
                push    dx
                push    di
                push    cx
                call    convutils.lba_to_chs
                pop     ax

                mov     ah, 0x03

                pusha
                mov     di, 3

                .ws_retry:  stc             ; some BIOSes are stupid and doesn't set the CF
                            int     0x13
                            jnc     .ws_end

                            test    di, di
                            jz      .ws_end
                            jmp     .ws_retry

                .ws_end:    popa
                            pop     di
                            pop     dx
                            pop     ax
                            ret

%endif
