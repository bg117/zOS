/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stddef.h>
#include <stdint.h>

#include <kernel/kernel.h>

#include <kernel/fs/fat.h>

#include <kernel/memory/heap.h>
#include <kernel/memory/mmap.h>
#include <kernel/memory/pmm.h>
#include <kernel/memory/vmm.h>

#include <kernel/hw/kbd.h>
#include <kernel/hw/serial.h>
#include <kernel/hw/timer.h>
#include <kernel/hw/video.h>

#include <kernel/misc/log_macros.h>

#include <utils/chars.h>
#include <utils/mem.h>
#include <utils/sort.h>
#include <utils/strings.h>

#define UARROW      0x48
#define LARROW      0x4B
#define RARROW      0x4D
#define DARROW      0x50
#define MAX_KBD_BUF 920

static char *get_keyboard_input(void);

static int sort_mmap(const void *i, const void *j);

// main
int kmain(uint8_t drive_number, FatInfo *fi, MemoryMapEntry *mmap, uint16_t mmap_length, VgaFontGlyph *vga_font_info)
{
    (void)drive_number;
    (void)fi;
    (void)vga_font_info;

    screen_clear();

    sort_bubble(mmap, mmap_length, sizeof *mmap, sort_mmap);

    KSLOG("%hu %s in the memory map\n", mmap_length, mmap_length > 1 ? "entries" : "entry");
    for (int i = 0; i < mmap_length; i++)
    {
        KSLOG("%d: Base=0x%016llX, Length=0x%016llX, Type=%u, AcpiExtAttrs=%u\n",
              i,
              mmap[i].base,
              mmap[i].length,
              mmap[i].type,
              mmap[i].acpi_extended_attributes);
    }

    screen_clear();

    KSLOG("initializing the kernel\n");
    kernel_init(mmap, mmap_length);

    screen_print_string("zOS version 0.01\nMade with \003 by bg117\n");

    // PMM contiguous memory test
    void *ten_pages = pmm_allocate_pages(10);

    // VMM contiguous memory test
    void *v_ten_pages = vmm_allocate_pages(10);
    // heap test
    void *p = heap_allocate(23);
    void *q = heap_allocate(34);
    void *r = heap_allocate(245);
    void *s = heap_allocate(sizeof(int));
    p       = heap_reallocate(p, 27);

    void *t = heap_allocate(20 * sizeof(void *));

    heap_free(p);

    while (1)
    {
        screen_print_string("> ");
        char *input = get_keyboard_input();

        screen_print_format_string("Input: \"%s\"\n", input);

        if (str_compare(input, "exit") == 0)
        {
            heap_free(input);
            break;
        }

        screen_print_string("Error: function not yet implemented\n");

        heap_free(input);
    }

    heap_free(q);
    heap_free(r);
    heap_free(s);
    heap_free(t);

    screen_clear();
    pmm_free_pages(ten_pages, 10);
    vmm_free_pages(v_ten_pages, 10);

    KSLOG("commencing shutdown\n");

    KSLOG("deinitializing the kernel\n");
    kernel_deinit();

    screen_print_string("It is now safe to shut down the computer.");

    return 1;
}

// can probably be used if I make a command-line interpreter in the near future
char *get_keyboard_input(void)
{
    char   *buf = heap_allocate(MAX_KBD_BUF);
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

static int sort_mmap(const void *i, const void *j)
{
    const MemoryMapEntry *d1 = i;
    const MemoryMapEntry *d2 = j;

    return (d1->base > d2->base) - (d1->base < d2->base);
}
