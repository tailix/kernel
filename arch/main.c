#include <kernelmq/multiboot.h>

#include "logger.h"
#include "gdt.h"
#include "idt.h"

static void print_multiboot2_info(unsigned long addr);

void main(unsigned int multiboot_magic, unsigned long multiboot_info)
{
    logger_initialize();

    if (multiboot_magic == KERNELMQ_MULTIBOOT_1_MAGIC) {
        logger_info("Loaded with Multiboot-compliant bootloader, specification version 1.");
    }
    else if (multiboot_magic == KERNELMQ_MULTIBOOT_2_MAGIC) {
        logger_info("Loaded with Multiboot-compliant bootloader, specification version 2.");
        print_multiboot2_info(multiboot_info);
    }
    else {
        logger_warn("Loaded with no Multiboot-compliant bootloader.");
    }

    logger_info("Kernel initialization started.");

    gdt_initialize();
    idt_initialize();

    asm volatile ("int $0x0");
    asm volatile ("int $0x3");
    asm volatile ("int $0x4");
    asm volatile ("int $0xF");
    asm volatile ("int $0x10");
    asm volatile ("int $0x1F");

    logger_warn("Nothing to do.");
    logger_fail("Halt.");
}

#include "console.h"
#include "multiboot2.h"

static void itoa(char *buf, int base, int d);
static void printf(const char *format, ...);

void print_multiboot2_info(unsigned long addr)
{
    struct multiboot_tag *tag;
    unsigned size;

    if (addr & 7)
    {
        printf ("Unaligned mbi: 0x%x\n", addr);
        return;
    }

    size = *(unsigned *) addr;

    printf ("Announced mbi size 0x%x\n", size);

    for (
        tag = (struct multiboot_tag *) (addr + 8);
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7)))
    {
        switch (tag->type)
        {
            case MULTIBOOT_TAG_TYPE_MODULE:
                printf(
                    "Module at 0x%x-0x%x. Command line %s\n",
                    ((struct multiboot_tag_module *) tag)->mod_start,
                    ((struct multiboot_tag_module *) tag)->mod_end,
                    ((struct multiboot_tag_module *) tag)->cmdline
                );
                break;
        }
    }

    tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7));

    printf ("Total mbi size 0x%x\n", (unsigned) tag - addr);
}

void itoa(char *buf, int base, int d)
{
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;
    /*
     * If %d is specified and D is minus, put '-' in the head.
     * */
    if (base == 'd' && d < 0)
    {
        *p++ = '-';
        buf++;
        ud = -d;
    }
    else if (base == 'x')
        divisor = 16;
    /*
     * Divide UD by DIVISOR until UD == 0.
     * */
    do
    {
        int remainder = ud % divisor;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
    while (ud /= divisor);
    /*
     * Terminate BUF.
     * */
    *p = 0;
    /*
     * Reverse BUF.
     * */
    p1 = buf;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

void printf(const char *format, ...)
{
    char **arg = (char **) &format;
    int c;
    char buf[20];
    arg++;
    while ((c = *format++) != 0)
    {
        if (c != '%')
            console_putc(c);
        else
        {
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
