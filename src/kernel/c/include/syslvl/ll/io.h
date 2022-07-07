/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxIOHxuuq
#define quuxIOHxuuq

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @brief Gets a byte from the port specified.
 *
 * @param port The port to obtain data from.
 * @return The byte returned by the port.
 */
uint8_t in_byte(uint16_t port);

/**
 * @brief Writes a byte to the port specified.
 *
 * @param port The port to write the byte to.
 * @param byte The byte to write to the port.
 */
void out_byte(uint16_t port, uint8_t byte);

/**
 * @brief Gets a word from the port specified.
 *
 * @param port The port to obtain data from.
 * @return The word returned by the port.
 */
uint16_t in_word(uint16_t port);

/**
 * @brief Writes a word to the port specified.
 *
 * @param port The port to write the word to.
 * @param byte The word to write to the port.
 */
void out_word(uint16_t port, uint16_t word);

#if defined(__cplusplus)
}
#endif

#endif
