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

char *inttos(char *buffer, int i, int base);
char *longtos(char *buffer, long l, int base);
char *llongtos(char *buffer, long long l, int base);

char *uinttos(char *buffer, unsigned int i, int base);
char *ulongtos(char *buffer, unsigned long l, int base);
char *ullongtos(char *buffer, unsigned long long l, int base);

#ifdef __cplusplus
}
#endif

#endif
