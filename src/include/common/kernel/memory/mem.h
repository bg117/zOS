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
 * @brief Fills a block of memory with a byte.
 *
 * @param ptr The block of memory to fill.
 * @param fill The byte to fill the block with.
 * @param len The length/size of the block of memory.
 * @return The block of memory.
 */
void *mem_fill8(void *ptr, uint8_t fill, size_t len);

/**
 * @brief Fills a block of memory with a word (16 bits, 2 bytes).
 *
 * @param ptr The block of memory to fill.
 * @param fill The byte to fill the block with.
 * @param len The length/size of the block of memory. Must be a multiple of 2.
 * @return The block of memory.
 */
void *mem_fill16(void *ptr, uint16_t fill, uint32_t len);

/**
 * @brief Fills a block of memory with a doubleword (32 bits, 4 bytes).
 *
 * @param ptr The block of memory to fill.
 * @param fill The byte to fill the block with.
 * @param len The length/size of the block of memory. Must be a multiple of 4.
 * @return The block of memory.
 */
void *mem_fill32(void *ptr, uint32_t fill, uint32_t len);

/**
 * @brief Copies a block of memory to another location.
 *
 * @param dest The destination location.
 * @param src The source block.
 * @param len The length of the source block to copy.
 * @return The destination block.
 */
void *mem_copy(void *restrict dest, const void *restrict src, uint32_t len);

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
void *mem_copy_with_overlap(void *dest, const void *src, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif
