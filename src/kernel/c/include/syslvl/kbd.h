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

struct get_char
{
    char scancode;
    char key;
};

void            kbd_init();
struct get_char kbd_get_char();

#ifdef __cplusplus
}
#endif

#endif
