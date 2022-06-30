/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxBIT_MACROSHxuuq
#define quuxBIT_MACROSHxuuq

#define SETBIT(x, bit)   ((x) | (bit))
#define UNSETBIT(x, bit) ((x) & ~(bit))

#define SETBITVAR(x, bit)   ((x) = SETBIT(x, bit))
#define UNSETBITVAR(x, bit) ((x) = UNSETBIT(x, bit))

#endif