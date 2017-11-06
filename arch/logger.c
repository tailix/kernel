#include "logger.h"

#include "console.h"

#include <kernelmq/stdlib.h>

#define LEVELS_COUNT 3

static const enum vga_color level_colors[LEVELS_COUNT] = {
    VGA_COLOR_CYAN,
    VGA_COLOR_MAGENTA,
    VGA_COLOR_RED,
};

static const char *const level_text[LEVELS_COUNT] = {
    "INFO",
    "WARN",
    "FAIL",
};

void logger_log(unsigned char level, const char *format, ...)
{
    if (level >= LEVELS_COUNT) {
        level = LEVELS_COUNT - 1;
    }

    console_setcolor(VGA_COLOR_LIGHT_GREY);
    console_putc('[');

    console_setcolor(level_colors[level]);
    console_print(level_text[level]);

    console_setcolor(VGA_COLOR_LIGHT_GREY);
    console_putc(']');

    console_putc(' ');

    console_setcolor(VGA_COLOR_WHITE);

    char **arg = (char **) &format;
    int c;
    char buf[20];
    arg++;

    while ((c = *format++) != 0)
    {
        if (c != '%') {
            console_putc(c);
        }
        else {
            char *p, *p2;
            int pad0 = 0, pad = 0;
            c = *format++;
            if (c == '0')
            {
                pad0 = 1;
                c = *format++;
            }
            if (c >= '0' && c <= '9')
            {
                pad = c - '0';
                c = *format++;
            }
            switch (c)
            {
                case 'd':
                case 'u':
                case 'x':
                    kitoa (buf, c, *((int *) arg++));
                    p = buf;
                    goto string;
                    break;
                case 's':
                    p = *arg++;
                    if (! p)
                        p = "(null)";
string:
                    for (p2 = p; *p2; p2++);
                    for (; p2 < p + pad; p2++)
                        console_putc(pad0 ? '0' : ' ');
                    while (*p)
                        console_putc(*p++);
                    break;
                default:
                    console_putc(*((int *) arg++));
                    break;
            }
        }
    }

    console_putc('\n');
}
