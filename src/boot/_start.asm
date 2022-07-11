; Copyright (c) 2022 bg117
;
; This software is released under the MIT License.
; https://opensource.org/licenses/MIT

; zOS boot loader

org     0x1000
bits    16

jmp short _start
nop

fat_info:

bios_parameter_block:
    .oem:                   db  'MSWIN4.1'
    .bytes_per_sector:      dw  512
    .sectors_per_cluster:   db  2
    .reserved_sectors:      dw  4
    .fats:                  db  2
    .root_dir_entries:      dw  512
    .total_sectors:         dw  0       ; in .large_sectors is the real sector count
    .media_descriptor_byte: db  0xF0    ; fixed disk
    .sectors_per_fat:       dw  251
    .sectors_per_track:     dw  63
    .heads:                 dw  2
    .hidden_sectors:        dd  0
    .large_sectors:         dd  129024  ; 63 SPT, 1024 cylinders, 2 heads, 512 BPS

extended_boot_record:
    .drive_number:          db  0x80
    .flags:                 db  0
    .signature:             db  0x29
    .volume_serial_number:  dd  0x4FFF8FC8
    .volume_label:          db  'NO NAME    '
    .system_identifier:     db  'FAT16   '

_start: ; fix regs
        xor     ax, ax
        mov     ds, ax
        mov     es, ax
        mov     fs, ax
        mov     gs, ax
        mov     ss, ax

        ; stack
        cli
        mov     sp, 0x1000
        mov     bp, sp
        sti

        ; relocate to lower memory address
        mov     ecx, 512 / 4    ; we'll use MOVSD that moves 4 bytes, so divide the bootloader size by 4
        mov     edi, 0x1000     ; new address
        mov     esi, 0x7C00     ; old address
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
        jmp     0x0000:main ; fix CS if not at 0x0000

main:   call    vgautils.clear

        ; get drive parameters
        push    es  ; ES is modified
        mov     ah, 0x08
        mov     dl, [extended_boot_record.drive_number]
        int     0x13
        pop     es

        ; heads
        xchg    dl, dh  ; heads stored in DH
        xor     dh, dh  ; clear upper half
        inc     dx      ; index starts at 0
        mov     [bios_parameter_block.heads], dx

        ; sectors
        xor     ch, ch      ; CH contains *cylinder* number, which we do not want
        and     cl, 0x3F    ; lower 6 bits (upper 2 bits are for cylinder number also)
        mov     [bios_parameter_block.sectors_per_track], cx

        ; load second-stage bootloader (2048 bytes)
        mov     ax, 1       ; read starting from LBA 1 (sector 2)
        mov     bx, real    ; read to memory address of real, about 0x1200
        mov     cl, 3       ; read 3 sectors, remaining 1536 bytes
        mov     dl, [extended_boot_record.drive_number]
        call    diskutils.read_sectors

        jc      unable_to_read_sector   ; if that didn't work, jump to error handler
        jmp     real

        jmp $

%include "convutils.inc"
%include "diskutils.inc"
%include "vgautils.inc"

; routine - waits for a key press then soft reboots
wait_key_and_reboot:    xor ax, ax
                        int 0x16

                        jmp 0xFFFF:0x0000   ; jmp to 0xFFFF0 (reboot)

; routine - prints a generic error message then jumps to wait_key_and_reboot
unsuccessful_boot:  mov     si, constants.UNABLE_TO_BOOT_MSG
                    call    vgautils.print
                    jmp     wait_key_and_reboot

; routine - prints a generic error message then jumps to wait_key_and_reboot
unable_to_read_sector:  mov     si, constants.UNABLE_TO_READ_SC_MSG
                        call    vgautils.print
                        jmp     wait_key_and_reboot

failed_to_get_memory_map:   mov     si, constants.FAIL_GET_MMAP_MSG
                            call    vgautils.print
                            jmp     wait_key_and_reboot

constants:
    .UNABLE_TO_BOOT_MSG:    db  `Unable to boot\0`
    .KERNEL_NOT_FOUND_MSG:  db  `Kernel not found\0`
    .FAIL_READ_FAT_MSG:     db  `Could not read next FAT sector\0`
    .UNABLE_TO_READ_SC_MSG: db  `Unable to read sector\0`
    .KERNEL_FILE:           db  `ZS         `
    .LOAD_KERNEL_MSG:       db  `Found kernel, loading\0`
    .FAIL_GET_MMAP_MSG:     db  `Failed to get memory map\0`

global_descriptor_table:
    dq  0
    .code:  dw  0xFFFF
            dw  0x0000
            db  0x00
            db  10011010b
            db  11001111b
            db  0x00

    .data:  dw  0xFFFF
            dw  0x0000
            db  0x00
            db  10010010b
            db  11001111b
            db  0x00

    .load:  dw  .load - global_descriptor_table - 1
            dd  global_descriptor_table

times 510-($-$$)    db  0x90    ; fill remaining with NOPs
dw  0xAA55                      ; signature

real:   ; get memory map
        xor     eax, eax
        mov     es, ax
        mov     di, BUFFER
        xor     si, si

        mov     es:[di + 20], dword 1

        xor     ebx, ebx
        mov     edx, 0x534D4150
        mov     eax, 0xE820
        mov     ecx, 24
        int     0x15

        jc      .mm_failed
        mov     edx, 0x534D4150
        cmp     eax, edx
        jne     .mm_failed
        test    ebx, ebx
        je      .mm_failed
        jmp     .mm_intermediate

        .mm_lp: mov     eax, 0xE820
                mov     es:[di + 20], dword 1
                mov     ecx, 24
                int     0x15

                jc      .mm_finished
                mov     edx, 0x534D4150

        .mm_intermediate:   jcxz    .mm_skip_entry
                            cmp     cl, 20
                            jbe     .mm_notext
                            test    byte es:[di + 20], 1
                            je      .mm_skip_entry

        .mm_notext: mov     ecx, es:[di + 8]
                    or      ecx, es:[di + 12]
                    jz      .mm_skip_entry
                    inc     si
                    add     di, 24

        .mm_skip_entry: test    ebx, ebx
                        jne     .mm_lp

        .mm_finished:   mov     [variables.mmap_entry_count], si
                        clc
                        jmp     .continue

        .mm_failed: jmp failed_to_get_memory_map

        .continue:

        ; reserve space for memory map
        mov     ax, 24                      ; 24 bytes per entry
        mul     si                          ; amount of entries

        add     [variables.font_buffer], ax
        add     [variables.fat_buffer], ax
        add     [variables.gen_buffer], ax

        pusha

        ; get font from VGA BIOS
        mov     bp, [variables.font_buffer]

        push    ds
        push    es

        mov     ax, 0x1130
        mov     bh, 0x06
        int     0x10

        push    es
        pop     ds
        pop     es

        mov     si, bp
        sub     si, 0x570
        mov     cx, 256 * 16 / 4
        mov     di, es:[variables.font_buffer]
        cld
        rep     movsd

        pop     ds

        popa

        add     word [variables.fat_buffer], 0x1000
        add     word [variables.gen_buffer], 0x1000

        mov     cx, [bios_parameter_block.reserved_sectors]
        mov     ax, [bios_parameter_block.sectors_per_fat]
        mul     byte [bios_parameter_block.fats]                ; AX = sectors_per_fat * fats
        mov     [variables.fat_size], ax

        add     cx, ax                                          ; CX += AX
        mov     [variables.root_dir_start], cx

        mov     ax, [bios_parameter_block.root_dir_entries]
        shl     ax, 5                                           ; AX *= 32
        xor     dx, dx                                          ; DX is the upper 16 bits of the dividend, make sure it is clear
        div     word [bios_parameter_block.bytes_per_sector]    ; AX /= bytes_per_sector
        mov     [variables.root_dir_size], ax

        call    load_next_fat   ; load first sector

        mov     ax, word [bios_parameter_block.bytes_per_sector]
        add     [variables.gen_buffer], ax

        mov     ax, [variables.root_dir_start]
        mov     cx, [variables.root_dir_size]
        mov     bx, [variables.gen_buffer]
        mov     dl, [extended_boot_record.drive_number]
        call    diskutils.read_sectors

        jc      unable_to_read_sector

        mov     di, bx

        .find_kernel:   push    cx
                        push    di

                        mov     cx, 11
                        mov     si, constants.KERNEL_FILE
                        repe    cmpsb

                        pop     di

                        je      .found_kernel
                        add     di, 32

                        pop     cx
                        loop    .find_kernel

        ; intermediate (if kernel not found)
        jmp kernel_not_found

        .found_kernel:  ; test
                        mov     si, constants.LOAD_KERNEL_MSG
                        call    vgautils.print

        mov     ax, [bios_parameter_block.bytes_per_sector]
        mov     cl, [bios_parameter_block.sectors_per_cluster]
        xor     ch, ch
        mul     cx
        mov     [variables.cluster_size_bytes], ax

        ; set up ES and BX for read operation
        xor     ax, ax
        mov     es, ax
        mov     bx, [variables.gen_buffer]

        ; extract first cluster
        mov     ax, es:[di + 26]            ; first cluster is the 26th element in file info
        mov     [variables.cluster], ax     ; store first cluster number
        mov     si, [variables.fat_buffer]  ; point to start of FAT

        .load_kernel:   mov     ax, [variables.cluster]
                        sub     ax, 2                           ; ax -= 2 (cluster starts at 2 for some reason)
                        mov     cl, [bios_parameter_block.sectors_per_cluster]
                        xor     ch, ch
                        mul     cx                              ; ax = cluster * sectors_per_cluster
                        add     ax, [variables.root_dir_start]  ; ax += root_dir_start
                        add     ax, [variables.root_dir_size]   ; ax += root_dir_size

                        ; read to KERNEL_SEGMENT:KERNEL_BUFFER
                        mov     cl, [bios_parameter_block.sectors_per_cluster]  ; read sectors_per_cluster sectors
                        mov     dl, [extended_boot_record.drive_number]         ; into drive drive_number
                        call    diskutils.read_sectors

                        mov     ax, [variables.cluster]
                        push    si

                        push    ax
                        mov     ax, [bios_parameter_block.bytes_per_sector]
                        mov     cx, [variables.current_fat]
                        dec     cx
                        mul     cx
                        mov     cx, ax
                        pop     ax

                        push    cx

                        push    ax
                        mov     ax, [bios_parameter_block.bytes_per_sector]
                        mul     word [variables.current_fat]
                        mov     cx, ax
                        pop     ax

                        cmp     ax, cx
                        ja      .lk_load_next_fat

                        .lk_fat_check_continue:

                        pop     cx

                        sub     ax, cx
                        mov     cx, 2
                        mul     cx
                        add     si, ax
                        mov     ax, ds:[si]
                        pop     si

                        mov     cx, [variables.cluster_size_bytes]
                        and     ecx, 0xFFFF
                        add     bx, cx
                        add     [variables.kernel_size_bytes], ecx

                        jmp     .after_load_kernel

                        .lk_load_next_fat:  call    load_next_fat
                                            jmp     .lk_fat_check_continue

        .after_load_kernel: cmp     ax, 0xFFF8
                            jae     .loaded_kernel  ; if ax >= 0x0FF8, no more clusters

                            mov     [variables.cluster], ax
                            jmp     .load_kernel

        .loaded_kernel:

        cli
        lgdt    [global_descriptor_table.load]
        mov     eax, cr0
        or      eax, 1
        mov     cr0, eax

        jmp     0x08:.pmode

        [bits 32]
        .pmode: mov     ax, 0x10
                mov     ds, ax
                mov     es, ax
                mov     fs, ax
                mov     gs, ax
                mov     ss, ax

        ; relocate kernel
        mov     ecx, [variables.kernel_size_bytes]
        mov     edi, 0x100000
        xor     esi, esi
        mov     si, [variables.gen_buffer]
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

        ; EDI=FAT info, EBX=memory map, ECX=memory map entry count, ESI=font info
        mov     edi, fat_info
        xor     ebx, ebx
        mov     bx, [variables.mmap_buffer]
        mov     ecx, [variables.mmap_entry_count]
        xor     edx, edx
        mov     dl, [extended_boot_record.drive_number]
        xor     esi, esi
        mov     si, [variables.font_buffer]

        jmp     0x08:0x100000

        [bits 16]
        jmp $

bits 16

load_next_fat:  pusha

                mov     ax, [bios_parameter_block.reserved_sectors]
                add     ax, [variables.current_fat]
                inc     word [variables.current_fat]

                mov     bx, [variables.fat_buffer]
                mov     cl, 1
                mov     dl, [extended_boot_record.drive_number]

                call    diskutils.read_sectors
                jc      unable_to_read_fat

                popa
                ret

unable_to_read_fat: mov     si, constants.FAIL_READ_FAT_MSG
                    call    vgautils.print
                    jmp     wait_key_and_reboot

kernel_not_found:   mov     si, constants.KERNEL_NOT_FOUND_MSG
                    call    vgautils.print
                    jmp     wait_key_and_reboot

variables:
    .root_dir_start:        dw  0
    .root_dir_size:         dw  0
    .fat_size:              dw  0
    .cluster:               dw  0
    .cluster_size_bytes:    dw  0
    .kernel_size_bytes:     dd  0
    .current_fat:           dw  0
    .mmap_buffer:           dw  BUFFER
    .font_buffer:           dw  BUFFER
    .fat_buffer:            dw  BUFFER
    .gen_buffer:            dw  BUFFER
    .mmap_entry_count:      dd  0

times 1534-($-real) db 0x90 ; NOP
dw                  0x55AA
BUFFER:
