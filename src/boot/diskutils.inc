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

                mov     ah, 0x02

                pusha
                mov     di, 3

                .rs_retry:  stc             ; some BIOSes are stupid and doesn't set the CF
                            int     0x13
                            jnc     .rs_end

                            test    di, di
                            jz      .rs_end
                            jmp     .rs_retry

                .rs_end:    popa
                            pop     di
                            pop     dx
                            pop     ax
                            ret

%endif
