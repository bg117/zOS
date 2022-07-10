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
 * @brief Represents modifiers which, combined with the key, may produce
 *        a different character.
 */
typedef enum key_modifiers
{
    KEY_ALT   = 0x01,
    KEY_CTRL  = 0x02,
    KEY_SHIFT = 0x04
} KeyModifiers;

/**
 * @brief Represents the key read from the keyboard.
 */
typedef struct read_key
{
    char         c;
    KeyModifiers modifiers;
} ReadKey;

/**
 * @brief Initializes the keyboard and its handler.
 */
void kbd_init(void);

/**
 * @brief Gets a single character from the user.
 *
 * @return The character entered.
 */
ReadKey kbd_get_char(void);

#ifdef __cplusplus
}
#endif

#endif
