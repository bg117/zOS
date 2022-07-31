/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxSERIALHxuuq
#define quuxSERIALHxuuq

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Appends a single character to the
 *        serial port.
 *
 * @param c The character to write.
 */
void serial_write_char(char c);

/**
 * @brief Appends a string to the serial port.
 *
 * @param s The string to write.
 */
void serial_write_string(const char *s);

/**
 * @brief Appends a format string to the serial
 *        port, and expands its arguments.
 *
 * @param fmt The format string which specifies what to write to
              the serial port.
 * @param ... Additional arguments to be used by the format string.
 */
void serial_write_format_string(const char *fmt, ...);

void serial_write_vformat_string(const char *fmt, va_list ap);

#ifdef __cplusplus
}
#endif

#endif
