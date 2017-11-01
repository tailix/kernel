#include "console.h"

#include "util.h"

static const unsigned int VGA_WIDTH  = 80;
static const unsigned int VGA_HEIGHT = 25;

static unsigned int console_row;
static unsigned int console_column;

static unsigned char console_color;

static unsigned short *console_buffer;

static unsigned char vga_entry_color(enum vga_color fg, enum vga_color bg);
static unsigned short vga_entry(unsigned char uc, unsigned char color);
static void console_putentryat(char c, unsigned char color, unsigned int x, unsigned int y);

void console_initialize() {
    console_row = 0;
    console_column = 0;
    console_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    console_buffer = (unsigned short*)0xB8000;

    for (unsigned int y = 0; y < VGA_HEIGHT; y++) {
        for (unsigned int x = 0; x < VGA_WIDTH; x++) {
            const unsigned int index = y * VGA_WIDTH + x;
            console_buffer[index] = vga_entry(' ', console_color);
        }
    }
}

void console_print(const char *const s)
{
    console_write(s, strlen(s));
}

void console_putc(const char c) {
    if (c == '\n') {
        console_column = 0;

        if (++console_row == VGA_HEIGHT) {
            console_row = 0;
        }

        return;
    }

    console_putentryat(c, console_color, console_column, console_row);

    if (++console_column == VGA_WIDTH) {
        console_column = 0;

        if (++console_row == VGA_HEIGHT) {
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
    const unsigned int index = y * VGA_WIDTH + x;
    console_buffer[index] = vga_entry(c, color);
}
