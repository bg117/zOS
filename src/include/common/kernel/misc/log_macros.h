/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#define KSLOG(msg, ...) serial_write_format_string("[%s]: " msg, __func__, ##__VA_ARGS__)
#define KSVLOG(msg, ...)                                                                                               \
    KSLOG(msg, ##__VA_ARGS__);                                                                                         \
    screen_print_format_string("[%s]: " msg, __func__, ##__VA_ARGS__)
