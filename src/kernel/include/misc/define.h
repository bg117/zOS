/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxDEFINEHxuuq
#define quuxDEFINEHxuuq

#define CAST(t, x) ((t)(x))
#define USE(x)     ((void)(x))

#define NL      "\r\n"
#define NAME(x) #x

#define ISPRINT(x) ((x) >= ' ' && (x) <= '~')

#define SETBIT(x, bit)   ((x) | (bit))
#define UNSETBIT(x, bit) ((x) & ~(bit))

#endif
