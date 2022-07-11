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

typedef struct __attribute__((packed)) vga_font_glyph
{
    uint8_t h1;
    uint8_t h2;
    uint8_t h3;
    uint8_t h4;
    uint8_t h5;
    uint8_t h6;
    uint8_t h7;
    uint8_t h8;
    uint8_t h9;
    uint8_t h10;
    uint8_t h11;
    uint8_t h12;
    uint8_t h13;
    uint8_t h14;
    uint8_t h15;
    uint8_t h16;
} VgaFontGlyph;

/**
 * @brief Clears the VGA buffer.
 */
void screen_clear(void);

/**
 * @brief Clears an individual line in the VGA buffer.
 *
 * @param y The line to clear.
 */
void screen_clear_line(int y);

/**
 * @brief Writes a single character to the VGA buffer.
 *
 * @param c The character to write.
 */
void screen_print_char(char c);

/**
 * @brief Writes a string to the VGA buffer.
 *
 * @param s The string of characters to write,
 */
void screen_print_string(const char *s);

/**
 * @brief Writes a format string to the VGA buffer, and expands its
 *        arguments.
 *
 * @param fmt The format string which specifies what to write to
              the VGA buffer.
 * @param ... Additional arguments to be used by the format string.
 */
void screen_print_format_string(const char *fmt, ...);

/**
 * @brief Scrolls the VGA buffer by one line.
 */
void screen_scroll(void);

/**
 * @brief Moves the cursor to the specified location in the screen.
 *
 * @param x The x-coordinate of the location.
 * @param y The y-coordinate of the location.
 */
void screen_move_cursor(int x, int y);

/**
 * @brief Gets the current cursor position in the screen.
 *
 * @param x The pointer to the x-coordinate to be populated.
 * @param y The pointer to the y-coordinate to be populated.
 */
void screen_get_cursor_position(int *x, int *y);

#ifdef __cplusplus
}
#endif

#endif
