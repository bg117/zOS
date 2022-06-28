/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxCHAR_MACROSHxuuq
#define quuxCHAR_MACROSHxuuq

#define ISPRINT(x) ((x) >= ' ' && (x) <= '~')
#define ISUPPER(x) ((x) >= 'A' && (x) <= 'Z')
#define ISLOWER(x) ((x) >= 'a' && (x) <= 'z')
#define ISALPHA(x) (ISUPPER(x) || ISLOWER(x))
#define ISDIGIT(x) ((x) >= '0' && (x) <= '9')
#define ISALNUM(x) (ISALPHA(x) || ISDIGIT(x))

#endif
