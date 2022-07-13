; Copyright (c) 2022 bg117
;
; This software is released under the MIT License.
; https://opensource.org/licenses/MIT

%ifndef quuxROUTINESINCxuuq
%define quuxROUTINESINCxuuq

failed_to_get_memory_map:   mov     si, FAILED_TO_GET_MMAP_MSG
                            call    print
                            jmp     wait_key_and_reboot

unable_to_read_fat: mov     si, FAILED_TO_READ_FAT_MSG
                    call    print
                    jmp     wait_key_and_reboot

kernel_not_found:   mov     si, KERNEL_NOT_FOUND_MSG
                    call    print
                    jmp     wait_key_and_reboot

sys_dir_not_found:  mov     si, SYS_DIR_NOT_FOUND_MSG
                    call    print
                    jmp     wait_key_and_reboot

modules_dir_not_found:  mov     si, MODULES_DIR_NOT_FOUND_MSG
                        call    print
                        jmp     wait_key_and_reboot

ata_mod_not_found:  mov     si, ATA_MOD_NOT_FOUND_MSG
                    call    print
                    jmp     wait_key_and_reboot

%endif
