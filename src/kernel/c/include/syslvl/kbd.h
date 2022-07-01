/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxKBDHxuuq
#define quuxKBDHxuuq

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the keyboard and its handler.
 */
void kbd_init();

/**
 * @brief Gets a single character from the user.
 *
 * @return The character entered.
 */
char kbd_get_char();

#ifdef __cplusplus
}
#endif

#endif
