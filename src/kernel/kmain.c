/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <kernel/fs/fat.h>
#include <kernel/hw/kbd.h>
#include <kernel/hw/serial.h>
#include <kernel/hw/timer.h>
#include <kernel/hw/video.h>
#include <kernel/kernel.h>
#include <kernel/memory/heap.h>
#include <kernel/memory/mmap.h>
#include <kernel/memory/pmm.h>
#include <kernel/memory/vmm.h>
#include <kernel/misc/log.h>
#include <stddef.h>
#include <stdint.h>
#include <utils/chars.h>
#include <utils/mem.h>
#include <utils/sort.h>
#include <utils/strings.h>

#define UARROW      0x48
#define LARROW      0x4B
#define RARROW      0x4D
#define DARROW      0x50
#define MAX_KBD_BUF 920

static char *get_keyboard_input(char *buf, size_t max_buf_size);

static int sort_mmap(const void *i, const void *j);

// main
int kmain(uint8_t drive_number, FatInfo *fi, VgaFontGlyph *vga_font_info, uint16_t mmap_length, MemoryMapEntry *mmap)
{
    (void)drive_number;
    (void)fi;
    (void)vga_font_info;

    screen_clear();

    sort_bubble(mmap, mmap_length, sizeof *mmap, sort_mmap);

    log_all(LOG_INFO, "%hu %s in the memory map\n", mmap_length, mmap_length > 1 ? "entries" : "entry");
    for (int i = 0; i < mmap_length; i++)
    {
        log_all(LOG_INFO,
                "%d) base=0x%016llX,len=0x%016llX,type=%u,ext=%u\n",
                i + 1,
                mmap[i].base,
                mmap[i].length,
                mmap[i].type,
                mmap[i].acpi_extended_attributes);
    }

    log_all(LOG_INFO, "initializing the kernel\n");
    kernel_init(mmap, mmap_length);

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

    screen_clear();
    screen_print_string("zOS version 0.01\n\n");

    while (1)
    {
        screen_print_string("> ");
        char *input = heap_allocate(MAX_KBD_BUF * sizeof *input);
        get_keyboard_input(input, MAX_KBD_BUF);

        if (str_compare(input, "exit") == 0)
        {
            heap_free(input);
            break;
        }
        else if (str_compare(input, "clear") == 0)
        {
            screen_clear();
        }
        else if (str_compare(input, "help") == 0)
        {
            screen_print_string(
                "Help\n\tclear: clears the screen\n\texit: exits the operating system, freeing "
                "all resources in the "
                "process\n\tpf: trigger page fault handler (FOR DEBUGGING PURPOSES ONLY)\n\texcept: trigger "
                "exception handler (FOR DEBUGGING PURPOSES ONLY)\n\thelp: show this help message\n");
        }
        else if (str_compare(input, "pf") == 0)
        {
            screen_print_string("Triggering page fault handler...\n");

            // for debugging BS only
            volatile const char *const volatile KILL_ME = (const char *const volatile)1; // not mapped
            (void)*KILL_ME;
        }
        else if (str_compare(input, "except") == 0)
        {
            screen_print_string("Triggering unhandled exception handler...\n");

            // for debugging BS only
            __asm__ volatile("int $0x03");
        }
        else
        {
            goto other;
        }

        heap_free(input);
        continue;

    other:
        log_all(LOG_ERR, "cannot execute \"%s\". Try \"help\"\n", input);
        heap_free(input);
    }

    heap_free(q);
    heap_free(r);
    heap_free(s);
    heap_free(t);

    pmm_free_pages(ten_pages, 10);
    vmm_free_pages(v_ten_pages, 10);

    log_all(LOG_INFO, "commencing shutdown\n");

    log_all(LOG_INFO, "deinitializing the kernel\n");
    kernel_deinit();

    screen_clear();
    screen_print_string("It is now safe to shut down the computer.");

    return 1;
}

// can probably be used if I make a command-line interpreter in the near future
char *get_keyboard_input(char *buf, size_t max_buf_size)
{
    size_t  idx = 0;
    size_t  top = 0;
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
        case DARROW:
            break;
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
                if (top >= max_buf_size)
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
