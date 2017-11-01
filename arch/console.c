#include "console.h"

#include "util.h"

#include <stdbool.h>

static const size_t VGA_WIDTH  = 80;
static const size_t VGA_HEIGHT = 25;

static size_t console_row;
static size_t console_column;
static uint8_t console_color;
static uint16_t* console_buffer;

static uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
static uint16_t vga_entry(unsigned char uc, uint8_t color);
static void console_putentryat(char c, uint8_t color, size_t x, size_t y);

void console_initialize() {
    console_row = 0;
    console_column = 0;
    console_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    console_buffer = (uint16_t*)0xB8000;

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
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

void console_setcolor(const uint8_t color) {
    console_color = color;
}

void console_write(const char *const data, const size_t size) {
    for (size_t i = 0; i < size; i++) {
        console_putc(data[i]);
    }
}

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

void console_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    console_buffer[index] = vga_entry(c, color);
}
