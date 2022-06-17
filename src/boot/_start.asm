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

main:   call    vgautils.clear

        mov cx, [bios_parameter_block.reserved_sectors]
        mov ax, [bios_parameter_block.sectors_per_fat]
        mul byte [bios_parameter_block.fats]                ; AX == sectors_per_fat * fats
        mov [variables.fat_size], ax

        add cx, ax                                          ; CX += AX
        mov [variables.root_dir_start], cx

        mov ax, [bios_parameter_block.root_dir_entries]
        shl ax, 5                                           ; AX *= 32
        xor dx, dx                                          ; just to be sure; some PCs crash when DX isn't clear
        div word [bios_parameter_block.bytes_per_sector]    ; AX /= bytes_per_sector
        mov [variables.root_dir_size], ax

        mov     ax, [bios_parameter_block.reserved_sectors]
        mov     cx, [variables.fat_size]                    ; word size
        mov     dl, [extended_boot_record.drive_number]
        mov     bx, [variables.fat_buffer]
        call    diskutils.read_sectors

        mov ax, cx
        mul word [bios_parameter_block.bytes_per_sector]
        add [variables.general_buffer], ax

        mov     ax, [variables.root_dir_start]
        mov     cx, [variables.root_dir_size]
        mov     bx, [variables.general_buffer]
        call    diskutils.read_sectors

        mov di, bx

        .find_kernel:   push    cx
                        push    di

                        mov     cx, 11
                        mov     si, constants.KERNEL_FILE
                        repe    cmpsb

                        pop di

                        je  .found_kernel
                        add di, 32

                        pop     cx
                        loop    .find_kernel

        ; intermediate (if kernel not found)
        jmp unsuccessful_boot

        .found_kernel:  ; test
                        mov     si, constants.LOAD_KERNEL_MSG
                        call    vgautils.print

        mov ax, [bios_parameter_block.bytes_per_sector]
        mov cx, [bios_parameter_block.sectors_per_cluster]
        mul cx
        mov [variables.cluster_size_bytes], ax

        mov ax, [di + 26]
        mov [variables.cluster], ax
        mov si, [variables.fat_buffer]

        .load_kernel:   mov ax, [variables.cluster]
                        add ax, [variables.root_dir_start]  ; ax += root_dir_start
                        add ax, [variables.root_dir_size]   ; ax += root_dir_size
                        sub ax, 2                           ; ax -= 2 (cluster starts at 2 for some reason)

                        mov     cl, 1
                        mov     dl, [extended_boot_record.drive_number]
                        call    diskutils.read_sectors

                        mov ax, [variables.cluster]
                        mov cx, 3
                        mul cx      ; ax *= 3
                        mov cx, 2
                        div cx      ; ax /= 2

                        add si, ax
                        mov ax, [ds:si]

                        add bx, [variables.cluster_size_bytes]

                        or  dx, dx
                        jz  .lk_even

                        .lk_odd:    shr ax, 4
                                    jmp .after_load_kernel

                        .lk_even:   and ax, 0xFFF

        .after_load_kernel: cmp ax, 0xFF8
                            jae .loaded_kernel  ; if ax >= 0x0FF8, no more clusters

                            mov [variables.cluster], ax
                            jmp .load_kernel

        .loaded_kernel:     ; base register = base address of kernel
                            mov bx, [variables.general_buffer]

                            push    cs
                            push    word [variables.general_buffer]

                            retf

        jmp $

wait_key_and_reboot:    xor ax, ax
                        int 0x16

                        jmp 0xFFFF:0x0000   ; jmp to 0xFFFF0 (reboot)

unsuccessful_boot:  mov     si, constants.UNABLE_TO_BOOT_MSG
                    call    vgautils.print
                    jmp     wait_key_and_reboot

variables:
    .root_dir_start:        dw  0
    .root_dir_size:         dw  0
    .fat_size:              dw  0
    .cluster:               dw  0
    .cluster_size_bytes:    dw  0
    .fat_buffer:            dw  BUFFER
    .general_buffer:        dw  BUFFER

constants:
    .KERNEL_FILE:           db  `ZS         `
    .LOAD_KERNEL_MSG:       db  `Found kernel, loading\0`
    .UNABLE_TO_BOOT_MSG:    db  `Unable to boot\0`

times 510-($-$$)    db  0       ; fill remaining with 0s
dw  0xAA55                      ; signature

BUFFER:
