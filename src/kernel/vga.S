.code16

/***************************
 *                         *
 *   x86 helper routines   *
 *                         *
 ***************************
 *
 * Filename: vga.S
 * Description: VGA helper routines for use with x86
 *
 * Date created: 200621DDMMYY1723HHmm
 *
 */

.section .text

/**
 * \brief Clears the screen.
 */
.globl screenClear
screenClear:    pushw   %bp
                movw    %sp, %bp

                movw    $0x0003, %ax
                int     $0x10

                movw    %bp, %sp
                popw    %bp
                ret

/**
 * \brief Writes a single character to the screen.
 * \param[in] bp+4 The character to write.
 */
.globl screenWriteChar
screenWriteChar:    pushw   %bp
                    movw    %sp, %bp

                    movb    $0x0E, %ah
                    movb    4(%bp), %al
                    int     $0x10

                    movw    %bp, %sp
                    popw    %bp
                    ret
