#ifndef KERNELMQ_INCLUDED_CONSOLE
#define KERNELMQ_INCLUDED_CONSOLE 1

void console_print(const char *s);
void console_putc(char c);
void console_puts(const char *s);
void console_write(const char *data, unsigned int size);

#endif
