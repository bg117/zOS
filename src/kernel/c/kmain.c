/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stddef.h>
#include <stdint.h>

#include <kernel.h>

#include <fs/fat.h>

#include <memory/mem.h>
#include <memory/mmap.h>
#include <memory/vmm.h>

#include <hw/kbd.h>
#include <hw/serial.h>
#include <hw/timer.h>
#include <hw/video.h>

#include <misc/char_macros.h>
#include <misc/log_macros.h>
#include <misc/strings.h>

#define UARROW      0x48
#define LARROW      0x4B
#define RARROW      0x4D
#define DARROW      0x50
#define MAX_KBD_BUF 0xFFF

char *get_keyboard_input(void);

// main
int kmain(uint8_t drive_number, FatInfo *fi, MemoryMapEntry *mmap, uint16_t mmap_length, VgaFontGlyph *vga_font_info)
{
    screen_clear();

    KSLOG("testing VGA glyphs\n");
    for (int i = 0; i < 256; i++)
    {
        KSLOG(
            "glyph %d:\n"
            "\t%08b\n"
            "\t%08b\n"
            "\t%08b\n"
            "\t%08b\n"
            "\t%08b\n"
            "\t%08b\n"
            "\t%08b\n"
            "\t%08b\n"
            "\t%08b\n"
            "\t%08b\n"
            "\t%08b\n"
            "\t%08b\n"
            "\t%08b\n"
            "\t%08b\n"
            "\t%08b\n"
            "\t%08b\n",
            i,
            vga_font_info[i].h1,
            vga_font_info[i].h2,
            vga_font_info[i].h3,
            vga_font_info[i].h4,
            vga_font_info[i].h5,
            vga_font_info[i].h6,
            vga_font_info[i].h7,
            vga_font_info[i].h8,
            vga_font_info[i].h9,
            vga_font_info[i].h10,
            vga_font_info[i].h11,
            vga_font_info[i].h12,
            vga_font_info[i].h13,
            vga_font_info[i].h14,
            vga_font_info[i].h15,
            vga_font_info[i].h16);
    }

    screen_print_string("---------------- MEMORY MAP ----------------\n");
    screen_print_format_string("%hu %s in the memory map\n", mmap_length, mmap_length > 1 ? "entries" : "entry");
    for (int i = 0; i < mmap_length; i++)
    {
        screen_print_format_string("%d: Base=0x%016llX, Length=0x%016llX, Type=%u, AcpiExtAttrs=%u\n",
                                   i,
                                   mmap[i].base,
                                   mmap[i].length,
                                   mmap[i].type,
                                   mmap[i].acpi_extended_attributes);
    }

    screen_clear();

    KSLOG("initializing the kernel\n");
    kernel_init(mmap, mmap_length);

    screen_print_string("zOS version 0.01\nTesting delay (2s)...\n");
    timer_wait(2000);

    while (1)
    {
        screen_print_string("> ");
        char *input = get_keyboard_input();

        screen_print_format_string("Input: %s\n", input);

        if (str_is_equal(input, "exit"))
        {
            vmm_free_page(input);
            break;
        }

        screen_print_string("Error: function not yet implemented\n");

        vmm_free_page(input);
    }

    screen_clear();

    KSLOG("commencing shutdown\n");

    KSLOG("deinitializing the kernel\n");
    kernel_deinit();

    screen_print_string("It is now safe to shut down the computer.");

    return 1;
}

// can probably be used if I make a command-line interpreter in the near future
char *get_keyboard_input(void)
{
    char   *buf = vmm_allocate_page();
    int     idx = 0;
    int     top = 0;
    ReadKey key = kbd_get_char();

    while (key.c != '\n')
    {
        switch (key.c)
        {
        case LARROW:
            {
                if (idx <= 0)
                    break;

                int x, y;
                screen_get_cursor_position(&x, &y);

                --x;
                screen_move_cursor(x, y);

                --idx;
            }
            break;
        case RARROW:
            {
                if (idx >= top)
                    break;

                int x, y;
                screen_get_cursor_position(&x, &y);

                ++x;
                screen_move_cursor(x, y);

                ++idx;
            }
            break;
        case UARROW:
        case DARROW: break;
        case '\b':
            {
                if (idx <= 0)
                    break;

                mem_copy_with_overlap(buf + idx - 1, buf + idx, top - idx);
                --idx;
                --top;

                buf[top] = '\0';

                int x, y;
                screen_get_cursor_position(&x, &y);
                --x;

                screen_print_format_string("\b%s ", buf + idx);
                screen_move_cursor(x, y);
            }
            break;
        default:
            {
                if (top >= MAX_KBD_BUF)
                    break;

                if (!ISPRINT(key.c))
                    break;

                mem_copy_with_overlap(buf + idx + 1, buf + idx, top - idx);
                buf[idx] = key.c;
                ++idx;
                ++top;

                buf[top] = '\0';

                int x, y;
                screen_get_cursor_position(&x, &y);
                ++x;

                screen_print_string(buf + idx - 1);
                screen_move_cursor(x, y);
            }
            break;
        }

        key = kbd_get_char();
    }

    screen_print_char('\n');
    return buf;
}
