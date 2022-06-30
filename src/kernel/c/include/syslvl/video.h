/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef quuxVIDEOHxuuq
#define quuxVIDEOHxuuq

#ifdef __cplusplus
extern "C" {
#endif

void screen_clear(void);
void screen_clear_line(int y);
void screen_print_char(char c);
void screen_print_string(const char *__restrict__ s);
void screen_print_format_string(const char *__restrict__ fmt, ...);
void screen_scroll(void);
void screen_move_cursor(int x, int y);
void screen_get_cursor_position(int *x, int *y);

#ifdef __cplusplus
}
#endif

#endif
