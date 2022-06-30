/**
 * Copyright (c) 2022 bg117
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <syslvl/exception_info.h>
#include <syslvl/hal.h>
#include <syslvl/io.h>
#include <syslvl/kbd.h>
#include <syslvl/pic.h>

#include <misc/type_macros.h>

static const char US_SCANCODES[128] = {
    0,    27,  '1', '2', '3',  '4', '5', '6', '7',  '8', /* 9 */
    '9',  '0', '-', '=', '\b',                           /* Backspace */
    '\t',                                                /* Tab */
    'q',  'w', 'e', 'r',                                 /* 19 */
    't',  'y', 'u', 'i', 'o',  'p', '[', ']', '\n',      /* Enter key */
    0,                                                   /* 29   - Control */
    'a',  's', 'd', 'f', 'g',  'h', 'j', 'k', 'l',  ';', /* 39 */
    '\'', '`', 0,                                        /* Left shift */
    '\\', 'z', 'x', 'c', 'v',  'b', 'n',                 /* 49 */
    'm',  ',', '.', '/', 0,                              /* Right shift */
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
    0, /* All other keys are undefined */
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
    0, /* All other keys are undefined */
};

static char         _last_char;
static char         _last_scancode;
static volatile int _got_key;

static enum key_flags
{
    KEY_SHIFT = 0x01,
    KEY_CTRL  = 0x02,
    KEY_ALT   = 0x04
} _key_flags;

static void _irq1_handler(struct exception_info *);

void kbd_init()
{
    hal_map_exception_handler(1 + pic_get_pic1_offset(), _irq1_handler);

    _last_char     = 0;
    _last_scancode = 0;
    _got_key       = 0;

    _key_flags = 0;
}

char kbd_get_char()
{
    _got_key = 0;
    while (!_got_key)
        ;

    return _last_char;
}

#define NO_TOP_BIT(x)  ((x)&0x7F)
#define SHIFT_CHECK(x) (NO_TOP_BIT(x) == 0x2A || NO_TOP_BIT(x) == 0x36)
#define ALT_CHECK(x)   (NO_TOP_BIT(x) == 0x38)
#define CTRL_CHECK(x)  (NO_TOP_BIT(x) == 0x1D)
#define SHIFT_ALT_CTRL_CHECK(x)                                                \
    (SHIFT_CHECK(x) || ALT_CHECK(x) || CTRL_CHECK(x))

void _irq1_handler(struct exception_info *info)
{
    uint8_t read;
    int     is_ready = 0;

    while (!is_ready)
    {
        if ((in_byte(0x64) & 1) == 0)
            continue;

        read     = in_byte(0x60);
        is_ready = 1;
    }

    if ((read & 0x80))
    {
        if (SHIFT_ALT_CTRL_CHECK(read))
        {
            if (SHIFT_CHECK(read))
                _key_flags ^= KEY_SHIFT;

            if (ALT_CHECK(read))
                _key_flags ^= KEY_ALT;

            if (CTRL_CHECK(read))
                _key_flags ^= KEY_CTRL;
        }
    }
    else
    {
        if (SHIFT_ALT_CTRL_CHECK(read))
        {
            if (SHIFT_CHECK(read))
                _key_flags ^= KEY_SHIFT;

            if (ALT_CHECK(read))
                _key_flags ^= KEY_ALT;

            if (CTRL_CHECK(read))
                _key_flags ^= KEY_CTRL;
        }
        else
        {
            _last_char = (_key_flags & KEY_SHIFT) == 1 ? US_SCANCODES_UPR[read]
                                                       : US_SCANCODES[read];
            _last_scancode = read;

            _got_key = 1;
        }
    }

    pic_send_eoi(0x01);
}
