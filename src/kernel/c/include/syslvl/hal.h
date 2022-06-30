/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxHALHxuuq
#define quuxHALHxuuq

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct exception_info;

void hal_init(uint8_t pic1_offset, uint8_t pic2_offset);
void hal_use_default_exception_handler(
    void (*handler)(struct exception_info *));
void hal_map_exception_handler(uint8_t vector,
                               void (*handler)(struct exception_info *));

#ifdef __cplusplus
}
#endif

#endif
