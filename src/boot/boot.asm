; zOS boot loader
; version 0.01
; ---
; Modified on 2022-06-09

org     0x7C00
bits    16

jmp short _start
nop

bios_parameter_block:
    .oem:                   db  'MSWIN4.1'
    .bytes_per_sector:      dw  512
    .sectors_per_cluster:   db  1
    .reserved_sectors:      dw  16
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
    .volume_label:          db  'NONAME     '
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

        jmp     0x0000:main ; fix CS if not at 0x0000

%include "chs.inc"
%include "print.inc"
%include "disk.inc"

main:   mov     si, HELLO
        call    print

        mov     ax, 1
        mov     bx, 0x7E00
        mov     cl, 1
        mov     dl, [extended_boot_record.drive_number]
        call    read_sectors

        mov     si, bx
        call    print

        jmp $

HELLO:              db `Hello, world!\0`

times 510-($-$$)    db 0    ; fill remaining with 0s
dw                  0xAA55  ; signature
