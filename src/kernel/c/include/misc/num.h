/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxNUMHxuuq
#define quuxNUMHxuuq

#ifdef __cplusplus
extern "C" {
#endif

char *int_to_string(char *buffer, int i, int base);
char *long_to_string(char *buffer, long l, int base);
char *long_long_to_string(char *buffer, long long l, int base);

char *uint_to_string(char *buffer, unsigned int i, int base);
char *ulong_to_string(char *buffer, unsigned long l, int base);
char *ulong_long_to_string(char *buffer, unsigned long long l, int base);

#ifdef __cplusplus
}
#endif

#endif
