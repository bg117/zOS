bits 16

;**************************
;                         *
;   x86 helper routines   *
;                         *
;**************************
;
; Filename: disk.asm
; Description: Disk drive helper routines for use with x86
;
; Date created: 200621DDMMYY1723HHmm
;
;

section .text

; --
; \brief Internal implementation of diskReset.
; \param[in] bp+4 The index of the drive to reset.
; \return 1 if success, 0 if fail.
; --
global __diskReset_internal_impl
__diskReset_internal_impl:  push    bp
                            mov     bp, sp          ; stack frame begin

                            mov     dl, [bp + 4]    ; get drive index
                            xor     ah, ah
                            int     0x13            ; call INT 13h with AH=0x00 (reset disk system)

                            mov     ax, 1           ; set return value
                            sbb     ax, 0           ; if carry flag set, then 0
                                                    ; else, 1

                            mov     sp, bp          ; stack frame end
                            pop     bp
                            ret

; --
; \brief Internal implementation of diskGetInfo.
; \param[in] bp+4 The index of the drive to obtain the info.
; \param[out] bp+6 Drive type
; \param[out] bp+8 Cylinders
; \param[out] bp+10 Heads
; \param[out] bp+12 Sectors
; \return 1 if success, 0 if fail.
; --
global __diskGetInfo_internal_impl
__diskGetInfo_internal_impl:    push    bp
                                mov     bp, sp          ; stack frame begin

                                mov     dl, [bp + 4]    ; get drive index
                                mov     ah, 0x08
                                int     0x13            ; call INT 13h with AH=08h

                                mov     ax, 1           ; set return value
                                sbb     ax, 0           ; if carry flag set, then 0
                                                        ; else, 1

                                xor     bh, bh          ; clear upper half for safety
                                mov     si, [bp + 6]    ; move (bp + 6) to SI
                                mov     [si], bx        ; move drive type to *(bp + 6)

                                inc     dh              ; increment head count (the interrupt returns heads - 1)
                                xchg    dl, dh
                                xor     dh, dh          ; clear upper half for safety
                                mov     si, [bp + 10]   ; move (bp + 10) to SI
                                mov     [si], dx        ; move head count to *(bp + 10)

                                mov     dl, cl          ; save sector count
                                and     dl, 0x3F        ; take only lower 6 bits (upper 2 bits are for cylinders)
                                mov     si, [bp + 12]   ; move (bp + 12) to SI
                                mov     [si], dx        ; move sector count to *(bp + 12)

                                shr     cl, 6           ; shift upper 2 bits of cylinder number to the right 6 times
                                xchg    cl, ch          ; exchange the upper and lower registers (they're reversed for some reason)
                                inc     cx              ; increment cylinder count
                                mov     si, [bp + 8]    ; move (bp + 8) to SI
                                mov     [si], cx        ; move cylinder count to *(bp + 8)

                                mov     sp, bp          ; stack frame end
                                pop     bp
                                ret

