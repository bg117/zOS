/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxMEMHxuuq
#define quuxMEMHxuuq

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Fills a block of memory with a byte.
 *
 * @param ptr The block of memory to fill.
 * @param fill The byte to fill the block with.
 * @param len The length/size of the block of memory.
 * @return The block of memory.
 */
void *mem_fill(void *ptr, uint8_t fill, size_t len);

/**
 * @brief Copies a block of memory to another location.
 *
 * @param dest The destination location.
 * @param src The source block.
 * @param len The length of the source block to copy.
 * @return The destination block.
 */
void *mem_copy(void *restrict dest, const void *restrict src, size_t len);

/**
 * @brief Copies a block of memory to another location.
 *        If the source block is greater than the destination block,
 *        it is equivalent to calling memory_copy with the same parameters.
 *        If the destination is greater, however, it copies from back to front.
 *
 * @param dest The destination location.
 * @param src The source block.
 * @param len The length of the source block to copy.
 * @return The destination block.
 */
void *mem_copy_with_overlap(void *dest, const void *src, size_t len);

#ifdef __cplusplus
}
#endif

#endif
