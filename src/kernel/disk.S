.code16

/***************************
 *                         *
 *   x86 helper routines   *
 *                         *
 ***************************
 *
 * Filename: disk.S
 * Description: Disk drive helper routines for use with x86
 *
 * Date created: 200621DDMMYY1723HHmm
 *
 */

.section .text

/**
 * \brief Internal implementation of diskReset.
 * \param[in] bp+4 The index of the drive to reset.
 * \return 1 if success, 0 if fail.
 */
.globl __diskReset_internal_impl
__diskReset_internal_impl:  pushw   %bp
                            movw    %sp, %bp        // Stack frame begin

                            movb    4(%bp), %dl     // Get drive index
                            movb    $0x00, %ah
                            int     $0x13           // Call INT 13h with AH=0x00 (reset disk system)

                            movw    $1, %ax         // Set return value
                            sbbw    $0, %ax         // If carry flag set, then 0
                                                    // Else, 1

                            movw    %bp, %sp        // Stack frame end
                            popw    %bp
                            ret

/**
 * \brief Internal implementation of diskGetInfo.
 * \param[in] bp+4 The index of the drive to obtain the info.
 * \param[out] bp+6 Drive type
 * \param[out] bp+8 Cylinders
 * \param[out] bp+10 Heads
 * \param[out] bp+12 Sectors
 * \return 1 if success, 0 if fail.
 */
.globl __diskGetInfo_internal_impl
__diskGetInfo_internal_impl:    pushw   %bp
                                movw    %sp, %bp        // Stack frame begin

                                movb    4(%bp), %dl     // Get drive index
                                movb    $0x08, %ah
                                int     $0x13           // Call INT 13h with AH=08h

                                movw    $1, %ax         // Set return value
                                sbbw    $0, %ax         // If carry flag set, then 0
                                                        // Else, 1

                                xorb    %bh, %bh        // Clear upper half for safety
                                movw    6(%bp), %si     // Move (bp + 6) to SI
                                movw    %bx, (%si)      // Move drive type to *(bp + 6)

                                incb    %dh             // Increment head count (the interrupt returns heads - 1)
                                xchgb   %dh, %dl
                                xorb    %dh, %dh        // Clear upper half for safety
                                movw    10(%bp), %si    // Move (bp + 10) to SI
                                movw    %dx, (%si)      // Move head count to *(bp + 10)

                                movb    %cl, %dl        // Save sector count
                                andb    $0x3F, %dl      // Take only lower 6 bits (upper 2 bits are for cylinders)
                                movw    12(%bp), %si    // Move (bp + 12) to SI
                                movw    %dx, (%si)      // Move sector count to *(bp + 12)

                                shlb    $6, %cl         // Shift upper 2 bits of cylinder number to the left 6 times
                                xchgb   %ch, %cl        // Exchange the upper and lower registers (they're reversed for some reason)
                                incw    %cx             // Increment cylinder count
                                movw    8(%bp), %si     // Move (bp + 8) to SI
                                movw    %cx, (%si)      // Move cylinder count to *(bp + 8)

                                movw    %bp, %sp        // Stack frame end
                                popw    %bp
                                ret

