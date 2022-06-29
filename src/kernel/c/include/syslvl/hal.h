/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxHALHxuuq
#define quuxHALHxuuq

#include <stdint.h>
#include <syslvl/exception_info.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*interrupt_service_routine_t)(struct exception_info *);

void halinit(uint8_t pic1_offset, uint8_t pic2_offset);
void halusedefexcept(interrupt_service_routine_t handler);
void halmapexcept(uint8_t vector, interrupt_service_routine_t handler);

#ifdef __cplusplus
}
#endif

#endif
