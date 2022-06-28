; Copyright (c) 2022 bg117
;
; This software is released under the MIT License.
; https://opensource.org/licenses/MIT

bits 32

;**************************
;                         *
;   x86 helper routines   *
;                         *
;**************************
;
; Filename: core_impl.asm
; Description: Core x86
;
; Date created: 280622DDMMYY1852HHmm
;
;

global corehlt
corehlt:    push    ebp
            mov     ebp, esp

            cli
            hlt

            mov     esp, ebp
            pop     ebp
            ret

global coresetint
coresetint: push    ebp
            mov     ebp, esp

            sti

            mov     esp, ebp
            pop     ebp
            ret

global coreclrint
coreclrint: push    ebp
            mov     ebp, esp

            cli

            mov     esp, ebp
            pop     ebp
            ret

global coresetcarry
coresetcarry:   push    ebp
                mov     ebp, esp

                stc

                mov     esp, ebp
                pop     ebp
                ret

global coreclrcarry
coreclrcarry:   push    ebp
                mov     ebp, esp

                clc

                mov     esp, ebp
                pop     ebp
                ret
