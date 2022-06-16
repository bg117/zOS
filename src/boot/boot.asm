; zOS boot loader
; version 0.01

org     0x7C00
bits    16

jmp short _start
nop

bios_parameter_block:
    .oem:                   db  'MSWIN4.1'
    .bytes_per_sector:      dw  512
    .sectors_per_cluster:   db  1
    .reserved_sectors:      dw  1
    .fats:                  db  2
    .root_dir_entries:      dw  224
    .total_sectors:         dw  2880
    .media_descriptor_byte: db  0xF0
    .sectors_per_fat:       dw  9
    .sectors_per_track:     dw  18
    .heads:                 dw  2
    .hidden_sectors:        dd  0
    .large_sectors:         dd  0

extended_boot_record:
    .drive_number:          db  0
    .flags:                 db  0
    .signature:             db  0x29
    .volume_serial_number:  dd  0x40008FCE
    .volume_label:          db  'NO NAME    '
    .system_identifier:     db  'FAT12   '

_start: ; fix regs
        xor     ax, ax
        mov     ds, ax
        mov     es, ax
        mov     fs, ax
        mov     gs, ax
        mov     ss, ax

        ; stack
        cli
        mov     sp, 0x7C00
        mov     bp, 0x7C00
        sti

        mov     [extended_boot_record.drive_number], dl
        xor     ax, ax
        xor     bx, bx
        xor     cx, cx
        xor     dx, dx
        xor     si, si
        xor     di, di

        jmp     0x0000:main ; fix CS if not at 0x0000

%include "convutils.inc"
%include "diskutils.inc"
%include "vgautils.inc"

main:   mov     cx, [bios_parameter_block.reserved_sectors]
        mov     ax, [bios_parameter_block.sectors_per_fat]
        mul     byte [bios_parameter_block.fats]                ; AX == sectors_per_fat * fats
        mov     [variables.fat_size], ax

        add     cx, ax                                          ; CX += AX
        mov     [variables.root_dir_start], cx

        mov     ax, [bios_parameter_block.root_dir_entries]
        shl     ax, 5                                           ; AX *= 32
        xor     dx, dx                                          ; just to be sure; some PCs crash when DX isn't clear
        div     word [bios_parameter_block.bytes_per_sector]    ; AX /= bytes_per_sector
        mov     [variables.root_dir_size], ax

        mov     ax, [bios_parameter_block.reserved_sectors]
        mov     cx, [variables.fat_size]                        ; word size
        mov     dl, [extended_boot_record.drive_number]
        mov     bx, [variables.fat_buffer]

        call    diskutils.read_sectors
        mov     ax, cx
        mul     word [bios_parameter_block.bytes_per_sector]
        add     [variables.general_buffer], ax

        jmp     $

print_error_and_reboot: mov     si, constants.DISKUTILS_ERROR
                        call    vgautils.print

                        xor     ax, ax
                        int     0x16

                        jmp     0xFFFF:0x0000   ; jmp to 0xFFFF0 (reboot)

variables:
    .root_dir_start:    dw      0
    .root_dir_size:     dw      0
    .fat_size:          dw      0
    .fat_buffer:        dw      BUFFER
    .general_buffer:    dw      BUFFER

constants:
    .KERNEL_FILE:       db      `ZS         `
    .DISKUTILS_ERROR:   db      `DISKUTILS error!\0`

times 510-($-$$)        db      0    ; fill remaining with 0s
dw                      0xAA55       ; signature

BUFFER:
