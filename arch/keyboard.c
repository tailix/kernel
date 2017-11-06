#include "keyboard.h"

#include "hwint.h"
#include "asm.h"

static keyboard_handler_t handler = 0;

static void internal_handler();

unsigned char keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', '-', '=', '\b', /* Backspace */
    '\t',           /* Tab */
    'q', 'w', 'e', 'r',   /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',   /* Enter key */
    0,          /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
    '\'', '`',   0,      /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',         /* 49 */
    'm', ',', '.', '/',   0,              /* Right shift */
    '*',
    0,  /* Alt */
    ' ',  /* Space bar */
    0,    /* Caps lock */
    0,    /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,    /* < ... F10 */
    0,    /* 69 - Num lock*/
    0,    /* Scroll Lock */
    0,    /* Home key */
    0,    /* Up Arrow */
    0,    /* Page Up */
    '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',
    0,    /* 79 - End key*/
    0,    /* Down Arrow */
    0,    /* Page Down */
    0,    /* Insert Key */
    0,    /* Delete Key */
    0,   0,   0,
    0,    /* F11 Key */
    0,    /* F12 Key */
    0,    /* All other keys are undefined */
};

void keyboard_register_handler(keyboard_handler_t new_handler)
{
    if (!new_handler) {
        keyboard_unregister_handler();
        return;
    }

    handler = new_handler;
    hwint_register_handler(1, internal_handler);
}

void keyboard_unregister_handler()
{
    hwint_unregister_handler(1);
    handler = 0;
}

void internal_handler()
{
    for (unsigned char status = inportb(0x64);
         status & 1;
         status = inportb(0x64)
    ) {
        char keycode = inportb(0x60);

        if (keycode < 0) {
            continue;
        }

        if (handler) {
            handler(keyboard_map[(unsigned char)keycode]);
        }
    }
}
