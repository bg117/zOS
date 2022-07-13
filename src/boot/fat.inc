; Copyright (c) 2022 bg117
;
; This software is released under the MIT License.
; https://opensource.org/licenses/MIT

%ifndef quuxFATINCxuuq
%define quuxFATINCxuuq

load_next_fat:  pusha

                mov     ax, [bios_parameter_block.reserved_sectors]
                add     ax, [current_fat]
                inc     word [current_fat]

                mov     bx, [fat_buffer]
                mov     cl, 1
                mov     dl, [extended_boot_record.drive_number]

                call    read_sectors
                jc      unable_to_read_fat

                popa
                ret

; --
; SI=FAT buffer
; DI=directory entry
; ES=segment of output buffer
; BX=output buffer
; --
load_directory_entry:   pusha

                        mov     ax, [di + 26]
                        mov     [cluster], ax

                        mov     dword [cur_file_size_bytes], 0

                        .load_loop: mov     ax, [cluster]
                                    sub     ax, 2                           ; ax -= 2 (cluster starts at 2 for some reason)
                                    mov     cl, [bios_parameter_block.sectors_per_cluster]
                                    xor     ch, ch
                                    mul     cx                              ; ax = cluster * sectors_per_cluster
                                    add     ax, [root_dir_start]  ; ax += root_dir_start
                                    add     ax, [root_dir_size]   ; ax += root_dir_size

                                    ; read to gen_buffer
                                    mov     cl, [bios_parameter_block.sectors_per_cluster]  ; read sectors_per_cluster sectors
                                    mov     dl, [extended_boot_record.drive_number]         ; into drive drive_number
                                    call    read_sectors

                                    mov     ax, [cluster]
                                    push    si

                                    push    ax
                                    mov     ax, [bios_parameter_block.bytes_per_sector]
                                    mov     cx, [current_fat]
                                    dec     cx
                                    mul     cx
                                    mov     cx, ax
                                    pop     ax

                                    push    cx

                                    push    ax
                                    mov     ax, [bios_parameter_block.bytes_per_sector]
                                    mul     word [current_fat]
                                    mov     cx, ax
                                    pop     ax

                                    cmp     ax, cx
                                    ja      .ld_load_next_fat

                                    .ld_fat_check_continue:

                                    pop     cx

                                    sub     ax, cx
                                    mov     cx, 2
                                    mul     cx
                                    add     si, ax
                                    mov     ax, [ds:si]
                                    pop     si

                                    mov     cx, [cluster_size_bytes]
                                    and     ecx, 0FFFFh
                                    add     bx, cx
                                    add     [cur_file_size_bytes], ecx

                                    jmp     .after_load_loop

                                    .ld_load_next_fat:  call    load_next_fat
                                                        jmp     .ld_fat_check_continue

                        .after_load_loop:   cmp     ax, 0FFF8h
                                            jae     .loaded  ; if ax >= 0FF8h, no more clusters

                                            mov     [cluster], ax
                                            jmp     .load_loop

                        .loaded:

                        popa
                        ret

%endif
