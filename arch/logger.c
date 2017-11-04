#include "logger.h"

#include "console.h"

void logger_info(const char *const s)
{
    console_setcolor(VGA_COLOR_LIGHT_GREY);
    console_putc('[');

    console_setcolor(VGA_COLOR_CYAN);
    console_print("INFO");

    console_setcolor(VGA_COLOR_LIGHT_GREY);
    console_putc(']');

    console_putc(' ');

    console_setcolor(VGA_COLOR_WHITE);
    console_puts(s);
}

void logger_warn(const char *const s)
{
    console_setcolor(VGA_COLOR_LIGHT_GREY);
    console_putc('[');

    console_setcolor(VGA_COLOR_MAGENTA);
    console_print("WARN");

    console_setcolor(VGA_COLOR_LIGHT_GREY);
    console_putc(']');

    console_putc(' ');

    console_setcolor(VGA_COLOR_WHITE);
    console_puts(s);
}

void logger_fail(const char *const s)
{
    console_setcolor(VGA_COLOR_LIGHT_GREY);
    console_putc('[');

    console_setcolor(VGA_COLOR_RED);
    console_print("FAIL");

    console_setcolor(VGA_COLOR_LIGHT_GREY);
    console_putc(']');

    console_putc(' ');

    console_setcolor(VGA_COLOR_WHITE);
    console_puts(s);
}
