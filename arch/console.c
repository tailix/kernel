#include "console.h"

#include <kernelmq/stdlib.h>

static unsigned short *const console_buffer = (unsigned short*)0xB8000;

static const unsigned int console_width  = 80;
static const unsigned int console_height = 25;

static unsigned int console_row;
static unsigned int console_column;

static unsigned char console_color;

static unsigned char vga_entry_color(enum vga_color fg, enum vga_color bg);
static unsigned short vga_entry(unsigned char uc, unsigned char color);
static void console_putentryat(char c, unsigned char color, unsigned int x, unsigned int y);

void console_initialize() {
    console_row = 0;
    console_column = 0;
    console_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    for (unsigned int y = 0; y < console_height; y++) {
        for (unsigned int x = 0; x < console_width; x++) {
            const unsigned int index = y * console_width + x;
            console_buffer[index] = vga_entry(' ', console_color);
        }
    }
}

void console_print(const char *const s)
{
    console_write(s, kstrlen(s));
}

void console_putc(const char c) {
    if (c == '\n') {
        console_column = 0;

        if (++console_row == console_height) {
            console_row = 0;
        }

        return;
    }

    console_putentryat(c, console_color, console_column, console_row);

    if (++console_column == console_width) {
        console_column = 0;

        if (++console_row == console_height) {
            console_row = 0;
        }
    }
}

void console_puts(const char *const s)
{
    console_print(s);
    console_putc('\n');
}

void console_setcolor(const unsigned char color) {
    console_color = color;
}

void console_write(const char *const data, const unsigned int size) {
    for (unsigned int i = 0; i < size; i++) {
        console_putc(data[i]);
    }
}

unsigned char vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

unsigned short vga_entry(unsigned char uc, unsigned char color) {
    return (unsigned short) uc | (unsigned short) color << 8;
}

void console_putentryat(char c, unsigned char color, unsigned int x, unsigned int y) {
    const unsigned int index = y * console_width + x;
    console_buffer[index] = vga_entry(c, color);
}
