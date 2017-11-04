#include "kprintf.h"

#include "console.h"
#include "util.h"

void kprintf(const char *format, ...)
{
    console_setcolor(VGA_COLOR_LIGHT_GREY);

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
                    itoa (buf, c, *((int *) arg++));
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
}
