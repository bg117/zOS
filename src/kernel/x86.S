.code16

/***************************
 *                         *
 *   x86 helper routines   *
 *                         *
 ***************************
 *
 * Filename: x86.S
 * Description: Helper routines for use with x86 (interrupt-based)
 *
 * Date created: 200617DDMMYY1441HHmm
 *
 */

.section .text

#### section "VGA functions"
.globl screenClear
screenClear:    pushw   %bp
                movw    %sp, %bp

                movw    $0x0003, %ax
                int     $0x10

                movw    %bp, %sp
                popw    %bp
                ret

.globl screenWriteChar
screenWriteChar:    pushw   %bp
                    movw    %sp, %bp

                    movb    $0x0E, %ah
                    movb    4(%bp), %al
                    int     $0x10

                    movw    %bp, %sp
                    popw    %bp
                    ret
#### endsection
