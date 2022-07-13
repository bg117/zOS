; Copyright (c) 2022 bg117
;
; This software is released under the MIT License.
; https://opensource.org/licenses/MIT

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
read_sectors:   push    ax
                push    dx

                push    cx
                call    lba_to_chs
                pop     ax

                stc     ; some BIOSes are stupid and don't set the CF
                mov     ah, 0x02
                int     0x13

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
write_sectors:  push    ax
                push    dx
                push    di
                push    cx
                call    lba_to_chs
                pop     ax

                mov     ah, 0x03

                stc     ; some BIOSes are stupid and don't set the CF
                int     0x13

                pop     di
                pop     dx
                pop     ax
                ret

%endif
