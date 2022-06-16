%ifndef quuxDISKINCxuuq
%define quuxDISKINCxuuq

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
read_sectors:   push    di
                push    cx
                call    lba_to_chs
                pop     ax

                mov     ah, 0x02

                pusha
                mov     di, 3

                .retry: stc             ; some BIOSes are stupid and doesn't set the CF
                        int     0x13
                        jnc     .end

                        test    di, di
                        jz      .end
                        jmp     .retry

                .end:   popa
                        pop      di
                        ret

%endif
