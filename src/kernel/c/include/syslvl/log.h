/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxLOGHxuuq
#define quuxLOGHxuuq

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Appends a single character to the temporary kernel
 *        log buffer.
 *
 * @param c The character to append.
 */
void log_append_char(char c);

/**
 * @brief Appends a string to the temporary kernel log buffer.
 *
 * @param s The string to append.
 */
void log_append_string(const char *__restrict__ s);

/**
 * @brief Appends a format string to the temporary kernel log
 *        buffer, and expands its arguments.
 *
 * @param fmt The format string which specifies what to write to
              the log buffer.
 * @param ... Additional arguments to be used by the format string.
 */
void log_append_format_string(const char *__restrict__ fmt, ...);

/**
 * @brief Gets the temporary kernel log buffer.
 *
 * @return The kernel's log buffer.
 */
char *log_get_tmp_buffer(void);

#ifdef __cplusplus
}
#endif

#endif
