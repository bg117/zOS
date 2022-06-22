.code16

/***************************
 *                         *
 *   x86 helper routines   *
 *                         *
 ***************************
 *
 * Filename: string.S
 * Description: String helper routines for use with x86
 *
 * Date created: 220621DDMMYY1502HHmm
 *
 */

.section .text

/**
 * \brief Fills the first n bytes of ptr with c.
 * \param[in] bp+4 The pointer to fill.
 * \param[in] bp+6 The character to fill the pointer with.
 * \param[in] bp+8 The length.
 * \return The pointer.
 */
.globl memset
memset: pushw   %bp
        movw    %sp, %bp        // Stack frame begin

        pushl   %edi

        movl    8(%bp), %ecx
        movb    6(%bp), %al
        movl    4(%bp), %edi

        cld
        rep     stosb

        movw    4(%bp), %ax     // Return value (ptr)

        popl    %edi

        movw    %bp, %sp        // Stack frame end
        popw    %bp
        ret
