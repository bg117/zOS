/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxSTRINGSHxuuq
#define quuxSTRINGSHxuuq

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

char       *str_to_upper(char *str);
char       *str_to_lower(char *str);
size_t      str_get_length(const char *str);
int         str_compare(const char *s1, const char *s2);
const char *str_get_first_occ(const char *s, char c);
const char *str_get_last_occ(const char *s, char c);
size_t      str_format(const char *fmt, char *out, ...);
size_t      str_vformat(const char *fmt, char *out, va_list ap);

#ifdef __cplusplus
}
#endif

#endif
