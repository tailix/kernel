#include "logger.h"

#include "console.h"

#include <kernaux/stdlib.h>

#define LEVELS_COUNT 4

static const char *const level_text[LEVELS_COUNT] = {
    "DBUG",
    "INFO",
    "WARN",
    "FAIL",
};

static void print_prefix(unsigned char level, const char *source);

void logger_log(unsigned char level, const char *const source, const char *format, ...)
{
    if (level >= LEVELS_COUNT) {
        level = LEVELS_COUNT - 1;
    }

    print_prefix(level, source);

    char **arg = (char **) &format;
    int c;
    char buf[20];
    arg++;

    while ((c = *format++) != 0)
    {
        if (c == '\n') {
            console_putc('\n');
            print_prefix(level, source);
        }
        else if (c != '%') {
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
                    kernaux_itoa(*((int*)arg++), buf, c);
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

void print_prefix(const unsigned char level, const char *const source)
{
    console_putc('[');
    console_print(level_text[level]);
    console_putc(']');
    console_putc(' ');

    if (source) {
        console_print(source);
        console_print(": ");
    }
}

void print(const char *format, ...)
{
    char **arg = (char **) &format;
    int c;
    char buf[20];
    arg++;

    while ((c = *format++) != 0)
    {
        if (c == '\n') {
            if (*format != 0) {
                console_putc('\n');
            }
        }
        else if (c != '%') {
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
                    kernaux_itoa(*((int*)arg++), buf, c);
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
