/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxLOGHxuuq
#define quuxLOGHxuuq

/**
 * @brief Represents a log level.
 */
typedef enum log_level
{
    LOG_INFO,
    LOG_WARN,
    LOG_ERR,
    LOG_URGENT
} LogLevel;

/**
 * @brief Logs both to the screen and to the serial port (COM1).
 *
 * @param lvl The log level.
 * @param fmt The format string to use.
 * @param ... Arguments to use with the format string.
 */
void log_all(LogLevel lvl, const char *fmt, ...);

/**
 * @brief Logs to the serial port (COM1).
 *
 * @param lvl The log level.
 * @param fmt The format string to use.
 * @param ... Arguments to use with the format string.
 */
void log_noprint(LogLevel lvl, const char *fmt, ...);

#endif
