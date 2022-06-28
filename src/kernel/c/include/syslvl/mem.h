/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxMEMHxuuq
#define quuxMEMHxuuq

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void *__attribute__((cdecl)) memfill(void *ptr, int fill, size_t len);

void *memcopy(void *__restrict__ dest,
              const void *__restrict__ src,
              size_t len);

void *memcopyovlp(void *__restrict__ dest,
                  const void *__restrict__ src,
                  size_t len);

#ifdef __cplusplus
}
#endif

#endif
