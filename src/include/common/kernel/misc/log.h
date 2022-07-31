/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxLOGHxuuq
#define quuxLOGHxuuq

typedef enum log_level
{
    LOG_INFO,
    LOG_WARN,
    LOG_ERR,
    LOG_URGENT
} LogLevel;

void log_all(LogLevel lvl, const char *fmt, ...);

void log_noprint(LogLevel lvl, const char *fmt, ...);

#endif
