.code16

/******************************
 *                            *
 *   C runtime startup code   *
 *                            *
 ******************************
 *
 * Filename: crt0.S
 * Description: Custom startup code for kernel
 *
 * Date created: 200622DDMMYY1905HHmm
 *
 */

.section .text

.globl  _start
.extern __bss_start
.extern __end
.extern main

jmp _start

.asciz "ZSKRNL "

_start: // initial setup (same as in bootloader)
        cli
        movw    %ax, %sp
        movw    %sp, %bp

        xorw    %ax, %ax
        movw    %ax, %ds
        movw    %ax, %es
        movw    %ax, %fs
        movw    %ax, %gs
        movw    %ax, %ss
        sti

        movl    $__end, %ecx
        movl    $__bss_start, %edi

        subl    %edi, %ecx  // subtract EDI from ECX (giving the total size of the bss section)
        cld

        rep     stosb

        pushw   %dx         // save data register

        // __cdecl passes args from right to left
        pushw   %si         // EBR
        pushw   %bx         // BPB

        xorb    %dh, %dh
        pushw   %dx         // push drive number to stack

        movw    $3, %cx
        pushw   %cx         // lastly, push argc

        call    main

        popw    %dx         // pop args count
        popw    %dx         // pop drive number
        popw    %dx         // pop BPB
        popw    %dx         // pop EBR

        popw    %dx         // bring back original value of DX

        .finish:    jmp .finish
