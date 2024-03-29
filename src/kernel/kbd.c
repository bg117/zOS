/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <kernel/hw/kbd.h>
#include <kernel/hw/serial.h>
#include <kernel/hw/video.h>
#include <kernel/int/interrupt_info.h>
#include <kernel/int/isr.h>
#include <kernel/int/pic.h>
#include <kernel/ll/io.h>
#include <kernel/misc/bit_macros.h>
#include <kernel/misc/log.h>

static const char US_SCANCODES[128] = {
    0,    27,   '1', '2', '3', '4', '5', '6', '7',  '8', '9', '0', '-', '=', '\b', /* Backspace */
    '\t',                                                                          /* Tab */
    'q',  'w',  'e', 'r',                                                          /* 19 */
    't',  'y',  'u', 'i', 'o', 'p', '[', ']', '\n',                                /* Enter key */
    0,                                                                             /* 29   - Control */
    'a',  's',  'd', 'f', 'g', 'h', 'j', 'k', 'l',  ';',                           /* 39 */
    '\'', '`',  0,                                                                 /* Left shift */
    '\\', 'z',  'x', 'c', 'v', 'b', 'n',                                           /* 49 */
    'm',  ',',  '.', '/', 0,                                                       /* Right shift */
    '*',  0,                                                                       /* Alt */
    ' ',                                                                           /* Space bar */
    0,                                                                             /* Caps lock */
    0,                                                                             /* 59 - F1 key ... > */
    0,    0,    0,   0,   0,   0,   0,   0,   0,                                   /* < ... F10 */
    0,                                                                             /* 69 - Num lock*/
    0,                                                                             /* Scroll Lock */
    0,                                                                             /* Home key */
    0x48,                                                                          /* Up Arrow */
    0,                                                                             /* Page Up */
    '-',  0x4B,                                                                    /* Left Arrow */
    0,    0x4D,                                                                    /* Right Arrow */
    '+',  0,                                                                       /* 79 - End key*/
    0x50,                                                                          /* Down Arrow */
    0,                                                                             /* Page Down */
    0,                                                                             /* Insert Key */
    0,                                                                             /* Delete Key */
    0,    0,    0,   0,                                                            /* F11 Key */
    0,                                                                             /* F12 Key */
    0,                                                                             /* All other keys are undefined */
};

static const char US_SCANCODES_UPR[128] = {
    0,    27,  '!', '@', '#',  '$', '%', '^', '&',  '*', /* 9 */
    '(',  ')', '_', '+', '\b',                           /* Backspace */
    '\t',                                                /* Tab */
    'Q',  'W', 'E', 'R',                                 /* 19 */
    'T',  'Y', 'U', 'I', 'O',  'P', '{', '}', '\n',      /* Enter key */
    0,                                                   /* 29   - Control */
    'A',  'S', 'D', 'F', 'G',  'H', 'J', 'K', 'L',  ':', /* 39 */
    '"',  '~', 0,                                        /* Left shift */
    '|',  'Z', 'X', 'C', 'V',  'B', 'N',                 /* 49 */
    'M',  '<', '>', '?', 0,                              /* Right shift */
    '*',  0,                                             /* Alt */
    ' ',                                                 /* Space bar */
    0,                                                   /* Caps lock */
    0,                                                   /* 59 - F1 key ... > */
    0,    0,   0,   0,   0,    0,   0,   0,   0,         /* < ... F10 */
    0,                                                   /* 69 - Num lock*/
    0,                                                   /* Scroll Lock */
    0,                                                   /* Home key */
    0,                                                   /* Up Arrow */
    0,                                                   /* Page Up */
    '-',  0,                                             /* Left Arrow */
    0,    0,                                             /* Right Arrow */
    '+',  0,                                             /* 79 - End key*/
    0,                                                   /* Down Arrow */
    0,                                                   /* Page Down */
    0,                                                   /* Insert Key */
    0,                                                   /* Delete Key */
    0,    0,   0,   0,                                   /* F11 Key */
    0,                                                   /* F12 Key */
    0,                                                   /* All other keys are undefined */
};

static volatile char g_last_char;
static volatile char g_last_scancode;
static volatile int  g_got_key;

static volatile KeyModifiers g_key_flags;

static void kbd_handler(InterruptInfo *);

void kbd_init(void)
{
    log_all(LOG_INFO, "mapping IRQ 1 handler\n");
    isr_map_interrupt_handler(1 + pic_get_pic1_offset(), kbd_handler);

    g_last_char     = 0;
    g_last_scancode = 0;
    g_got_key       = 0;

    g_key_flags = 0;
}

void kbd_deinit(void)
{
    log_all(LOG_INFO, "unmapping IRQ 1 handler\n");
    isr_unmap_interrupt_handler(1 + pic_get_pic1_offset());
}

ReadKey kbd_get_char(void)
{
    ReadKey key;

    g_got_key = 0;
    while (!g_got_key)
        ;

    key.c         = g_last_char;
    key.modifiers = g_key_flags;
    return key;
}

#define NO_TOP_BIT(x)           ((x)&0x7F)
#define SHIFT_CHECK(x)          (NO_TOP_BIT(x) == 0x2A || NO_TOP_BIT(x) == 0x36)
#define ALT_CHECK(x)            (NO_TOP_BIT(x) == 0x38)
#define CTRL_CHECK(x)           (NO_TOP_BIT(x) == 0x1D)
#define SHIFT_ALT_CTRL_CHECK(x) (SHIFT_CHECK(x) || ALT_CHECK(x) || CTRL_CHECK(x))

void kbd_handler(InterruptInfo *info)
{
    (void)info;

    uint8_t read;
    int     is_ready = 0;

    while (!is_ready)
    {
        if ((in_byte(0x64) & 1) == 0)
            continue;

        read     = in_byte(0x60);
        is_ready = 1;
    }

    log_noprint(LOG_INFO, "read key with scancode=0x%hhX\n", read);

    if ((read & 0x80))
    {
        if (SHIFT_CHECK(read))
            UNSETBITVAR(g_key_flags, KEY_SHIFT);

        if (ALT_CHECK(read))
            UNSETBITVAR(g_key_flags, KEY_ALT);

        if (CTRL_CHECK(read))
            UNSETBITVAR(g_key_flags, KEY_CTRL);
    }
    else
    {
        if (SHIFT_ALT_CTRL_CHECK(read))
        {
            if (SHIFT_CHECK(read))
                SETBITVAR(g_key_flags, KEY_SHIFT);

            if (ALT_CHECK(read))
                SETBITVAR(g_key_flags, KEY_ALT);

            if (CTRL_CHECK(read))
                SETBITVAR(g_key_flags, KEY_CTRL);
        }
        else
        {
            g_last_char     = TESTBIT(g_key_flags, KEY_SHIFT) ? US_SCANCODES_UPR[read] : US_SCANCODES[read];
            g_last_scancode = read;

            g_got_key = 1;
        }
    }

    pic_send_eoi(0x01);
}
