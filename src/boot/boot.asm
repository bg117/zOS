; Copyright (c) 2022 bg117
;
; This software is released under the MIT License.
; https://opensource.org/licenses/MIT
;
; zOS boot loader

org     1000h
bits    16

jmp short boot
nop

%macro SWITCH_TO_REAL_MODE 0
    cli

    jmp     18h:%%pmode

    [bits 16]
    %%pmode:    mov     ax, 20h
                mov     ds, ax
                mov     es, ax
                mov     fs, ax
                mov     gs, ax
                mov     ss, ax

    mov     eax, cr0
    and     eax, ~1
    mov     cr0, eax

    jmp     0h:%%rmode

    %%rmode:    xor     ax, ax
                mov     ds, ax
                mov     es, ax
                mov     fs, ax
                mov     gs, ax
                mov     ss, ax

    sti
%endmacro

%macro SWITCH_TO_PROTECTED_MODE 0
    cli
    lgdt    [global_descriptor_table.load]
    mov     eax, cr0
    or      eax, 1
    mov     cr0, eax

    jmp     08h:%%pmode

    [bits 32]
    %%pmode:    mov     ax, 10h
                mov     ds, ax
                mov     es, ax
                mov     fs, ax
                mov     gs, ax
                mov     ss, ax
%endmacro

fat_info:

bios_parameter_block:
    .oem:                   db  'MSWIN4.1'
    .bytes_per_sector:      dw  512
    .sectors_per_cluster:   db  2
    .reserved_sectors:      dw  4
    .fats:                  db  2
    .root_dir_entries:      dw  512
    .total_sectors:         dw  0       ; in .large_sectors is the real sector count
    .media_descriptor_byte: db  0F0h    ; fixed disk
    .sectors_per_fat:       dw  251
    .sectors_per_track:     dw  63
    .heads:                 dw  2
    .hidden_sectors:        dd  0
    .large_sectors:         dd  129024  ; 63 SPT, 1024 cylinders, 2 heads, 512 BPS

extended_boot_record:
    .drive_number:          db  80h
    .flags:                 db  0
    .signature:             db  29h
    .volume_serial_number:  dd  4FFF8FC8h
    .volume_label:          db  'NO NAME    '
    .system_identifier:     db  'FAT16   '

boot:   ; fix regs
        xor     ax, ax
        mov     ds, ax
        mov     es, ax
        mov     fs, ax
        mov     gs, ax
        mov     ss, ax

        ; stack
        cli
        mov     sp, 1000h
        mov     bp, sp
        sti

        ; relocate to lower memory address
        mov     ecx, 512 / 4    ; we'll use MOVSD that moves 4 bytes, so divide the bootloader size by 4
        mov     edi, 1000h     ; new address
        mov     esi, 7C00h     ; old address
        cld
        rep     movsd

        ; clear registers
        mov     [extended_boot_record.drive_number], dl
        xor     ax, ax
        xor     bx, bx
        xor     cx, cx
        xor     dx, dx
        xor     si, si
        xor     di, di

        ; for some BIOSes
        jmp     0000h:main ; fix CS if not at 0000h

main:   call    clear

        ; get drive parameters
        push    es  ; ES is modified
        mov     ah, 08h
        mov     dl, [extended_boot_record.drive_number]
        int     13h
        pop     es

        ; heads
        xchg    dl, dh      ; heads stored in DH
        xor     dh, dh      ; clear upper half
        inc     dx          ; index starts at 0
        mov     [bios_parameter_block.heads], dx

        ; sectors
        xor     ch, ch      ; CH contains *cylinder* number, which we do not want
        and     cl, 3Fh     ; lower 6 bits (upper 2 bits are for cylinder number also)
        mov     [bios_parameter_block.sectors_per_track], cx

        ; load second-stage bootloader (2048 bytes)
        mov     ax, 1       ; read starting from LBA 1 (sector 2)
        mov     bx, real    ; read to memory address of real, about 1200h
        mov     cl, 3       ; read 3 sectors, remaining 1536 bytes
        mov     dl, [extended_boot_record.drive_number]
        call    read_sectors

        jc      unable_to_read_sector   ; if that didn't work, jump to error handler
        jmp     real

        jmp $

%include "conv.inc"
%include "disk.inc"
%include "vga.inc"

; routine - waits for a key press then soft reboots
wait_key_and_reboot:    xor ax, ax
                        int 16h

                        jmp 0FFFFh:0000h   ; jmp to 0FFFF0h (reboot)

; routine - prints a generic error message then jumps to wait_key_and_reboot
unsuccessful_boot:  mov     si, UNABLE_TO_BOOT_MSG
                    call    print
                    jmp     wait_key_and_reboot

; routine - prints a generic error message then jumps to wait_key_and_reboot
unable_to_read_sector:  mov     si, UNABLE_TO_READ_SECTOR_MSG
                        call    print
                        jmp     wait_key_and_reboot

UNABLE_TO_BOOT_MSG:         db  'Unable to boot', 0
UNABLE_TO_READ_SECTOR_MSG:  db  'Unable to read sector', 0

times 510-($-$$)    db  90h    ; fill remaining with NOPs
dw  0AA55h                      ; signature

real:   ; get memory map
        xor     eax, eax
        mov     es, ax
        mov     di, BUFFER
        xor     si, si

        mov     [es:di + 20], dword 1

        xor     ebx, ebx
        mov     edx, 534D4150h
        mov     eax, 0E820h
        mov     ecx, 24
        int     15h

        jc      .mm_failed
        mov     edx, 534D4150h
        cmp     eax, edx
        jne     .mm_failed
        test    ebx, ebx
        je      .mm_failed
        jmp     .mm_intermediate

        .mm_lp: mov     eax, 0E820h
                mov     [es:di + 20], dword 1
                mov     ecx, 24
                int     15h

                jc      .mm_finished
                mov     edx, 534D4150h

        .mm_intermediate:   jcxz    .mm_skip_entry
                            cmp     cl, 20
                            jbe     .mm_notext
                            test    byte [es:di + 20], 1
                            je      .mm_skip_entry

        .mm_notext: mov     ecx, [es:di + 8]
                    or      ecx, [es:di + 12]
                    jz      .mm_skip_entry
                    inc     si
                    add     di, 24

        .mm_skip_entry: test    ebx, ebx
                        jne     .mm_lp

        .mm_finished:   mov     [mmap_entry_count], si
                        clc
                        jmp     .continue

        .mm_failed: jmp failed_to_get_memory_map

        .continue:

        ; reserve space for memory map
        mov     ax, 24                      ; 24 bytes per entry
        mul     si                          ; amount of entries

        add     [font_buffer], ax
        add     [fat_buffer], ax
        add     [gen_buffer], ax
        add     [file_buffer], ax

        pusha

        ; get font from VGA BIOS
        mov     bp, [font_buffer]

        push    ds
        push    es

        mov     ax, 1130h
        mov     bh, 06h
        int     10h

        push    es
        pop     ds
        pop     es

        mov     si, bp
        sub     si, 570h    ; I don't know if this is actually a constant value but
                            ; it doesn't work without this
        mov     cx, 256 * 16 / 4
        mov     di, [es:font_buffer]
        cld
        rep     movsd

        pop     ds

        popa

        add     word [fat_buffer], 1000h
        add     word [gen_buffer], 1000h
        add     word [file_buffer], 1000h

        mov     cx, [bios_parameter_block.reserved_sectors]
        mov     ax, [bios_parameter_block.sectors_per_fat]
        mul     byte [bios_parameter_block.fats]                ; AX = sectors_per_fat * fats
        mov     [fat_size], ax

        add     cx, ax                                          ; CX += AX
        mov     [root_dir_start], cx

        mov     ax, [bios_parameter_block.root_dir_entries]
        shl     ax, 5                                           ; AX *= 32
        xor     dx, dx                                          ; DX is the upper 16 bits of the dividend, make sure it is clear
        div     word [bios_parameter_block.bytes_per_sector]    ; AX /= bytes_per_sector
        mov     [root_dir_size], ax

        call    load_next_fat   ; load first sector

        mov     ax, word [bios_parameter_block.bytes_per_sector]
        add     [gen_buffer], ax
        add     [file_buffer], ax

        mov     ax, [root_dir_start]
        mov     cx, [root_dir_size]
        mov     bx, [gen_buffer]
        mov     dl, [extended_boot_record.drive_number]
        call    read_sectors

        jc      unable_to_read_sector

        mov     di, bx

        .find_sys_dir:  push    cx
                        push    di

                        mov     cx, 11
                        mov     si, SYS_DIR
                        repe    cmpsb

                        pop     di
                        pop     cx

                        je      .found_sys_dir
                        add     di, 32 ; next entry

                        loop    .find_sys_dir

        jmp     sys_dir_not_found

        .found_sys_dir: ; DI will be pointing to the directory entry of SYS

        mov     ax, [bios_parameter_block.bytes_per_sector]
        mov     cl, [bios_parameter_block.sectors_per_cluster]
        xor     ch, ch
        mul     cx
        mov     [cluster_size_bytes], ax

        ; set up ES and BX for read operation
        xor     ax, ax
        mov     es, ax
        mov     bx, [gen_buffer]

        ; read entry
        mov     si, [fat_buffer]  ; point to start of FAT

        call    load_directory_entry

        mov     ax, [file_buffer]
        add     ax, [cur_file_size_bytes]
        mov     [file_buffer], ax

        ; find kernel
        mov     di, [gen_buffer]

        ; get how many entries
        mov     ax, [cur_file_size_bytes]
        mov     cx, 32
        xor     dx, dx
        div     cx
        mov     cx, ax

        push    cx

        .find_kernel:   push    cx
                        push    di

                        mov     cx, 11
                        mov     si, KERNEL_FILE
                        repe    cmpsb

                        pop     di
                        pop     cx

                        je      .found_kernel
                        add     di, 32

                        loop    .find_kernel

        ; intermediate (if kernel not found)
        jmp kernel_not_found

        .found_kernel:

        ; set up ES and BX for read operation
        xor     ax, ax
        mov     es, ax
        mov     bx, [file_buffer]

        ; read kernel
        mov     si, [fat_buffer]  ; point to start of FAT

        call    load_directory_entry

        mov     eax, [cur_file_size_bytes]
        mov     [kernel_size_bytes], eax

        ; switch to PMode temporarily and relocate kernel
        SWITCH_TO_PROTECTED_MODE

        ; relocate kernel
        mov     ecx, [kernel_size_bytes]
        mov     edi, 100000h
        xor     esi, esi
        mov     si, [file_buffer]
        xor     edx, edx
        mov     eax, ecx
        mov     ecx, 4
        div     ecx
        mov     ecx, eax

        cld
        rep     movsd

        mov     ecx, edx

        cld
        rep     movsb

        ; switch back to real mode
        SWITCH_TO_REAL_MODE

        pop     cx

        mov     di, [gen_buffer] ; still contains SYS

        ; locate and load MODULES folder
        .find_mod_dir:  push    cx
                        push    di

                        mov     cx, 11
                        mov     si, MODULES_DIR
                        repe    cmpsb

                        pop     di
                        pop     cx

                        je      .found_mod_dir
                        add     di, 32 ; next entry

                        loop    .find_mod_dir

        jmp     modules_dir_not_found

        .found_mod_dir: ; DI will be pointing to the directory entry of MODULES

        ; set up ES and BX for read operation
        xor     ax, ax
        mov     es, ax
        mov     bx, [gen_buffer]

        ; read entry
        mov     si, [fat_buffer]  ; point to start of FAT

        call    load_directory_entry

        mov     ax, [gen_buffer]  ; initial value of file_buffer; cur_file_size_bytes probably changed
        add     ax, [cur_file_size_bytes]
        mov     [file_buffer], ax

        SWITCH_TO_PROTECTED_MODE

        ; EDI=FAT info, EBX=memory map, ECX=memory map entry count, ESI=font info
        mov     edi, fat_info
        xor     ebx, ebx
        mov     bx, [mmap_buffer]
        mov     ecx, [mmap_entry_count]
        xor     edx, edx
        mov     dl, [extended_boot_record.drive_number]
        xor     esi, esi
        mov     si, [font_buffer]

        jmp     08h:100000h

        ; if that didn't work
        jmp $

bits 16

%include "routines.inc"
%include "fat.inc"

; --
;

global_descriptor_table:
    dq  0
    .code32:    dw  0FFFFh
                dw  0000h
                db  00h
                db  10011010b
                db  11001111b
                db  00h

    .data32:    dw  0FFFFh
                dw  0000h
                db  00h
                db  10010010b
                db  11001111b
                db  00h

    .code16:    dw  0FFFFh
                dw  0000h
                db  00h
                db  10011010b
                db  00000000b
                db  00h

    .data16:    dw  0FFFFh
                dw  0000h
                db  00h
                db  10010010b
                db  00000000b
                db  00h

    .load:  dw  .load - global_descriptor_table - 1
            dd  global_descriptor_table

FAILED_TO_GET_MMAP_MSG:     db  'Failed to get memory map', 0
KERNEL_NOT_FOUND_MSG:       db  'Kernel SYS/ZS not found', 0
ATA_MOD_NOT_FOUND_MSG:      db  'ATA driver SYS/MODULES/ATA.MOD not found', 0
SYS_DIR_NOT_FOUND_MSG:      db  'SYS directory not found', 0
MODULES_DIR_NOT_FOUND_MSG:  db  'SYS/MODULES directory not found'
FAILED_TO_READ_FAT_MSG:     db  'Failed to read the FAT', 0
KERNEL_FILE:                db  'ZS         '
SYS_DIR:                    db  'SYS        '
MODULES_DIR:                db  'MODULES    '
ATA_MOD:                    db  'ATA     MOD'

root_dir_start:             dw  0
root_dir_size:              dw  0
fat_size:                   dw  0
cluster:                    dw  0
cluster_size_bytes:         dw  0
cur_file_size_bytes:        dd  0
kernel_size_bytes:          dd  0
current_fat:                dw  0
mmap_buffer:                dw  BUFFER
font_buffer:                dw  BUFFER
fat_buffer:                 dw  BUFFER
gen_buffer:                 dw  BUFFER
file_buffer:                dw  BUFFER
mmap_entry_count:           dd  0

times 1534-($-real) db 90h ; NOP
dw                  0A450h

BUFFER:
