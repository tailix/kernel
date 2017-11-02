#include <kernelmq/multiboot.h>

#include "console.h"

#define MULTIBOOT_1_MAGIC 0x2BADB002
#define MULTIBOOT_2_MAGIC 0x36d76289

#define MULTIBOOT_TAG_TYPE_END           0
#define MULTIBOOT_TAG_TYPE_CMDLINE       1
#define MULTIBOOT_TAG_TYPE_MODULE        3
#define MULTIBOOT_TAG_TYPE_BASIC_MEMINFO 4
#define MULTIBOOT_TAG_TYPE_MMAP          6

#define MULTIBOOT_MEMORY_AVAILABLE        1
#define MULTIBOOT_MEMORY_RESERVED         2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE 3
#define MULTIBOOT_MEMORY_NVS              4

struct multiboot_mmap_entry
{
    unsigned long long addr;
    unsigned long long len;
    unsigned int type;
    unsigned int zero;
}
__attribute__((packed));

typedef struct multiboot_mmap_entry multiboot_memory_map_t;

struct multiboot_tag
{
    unsigned int type;
    unsigned int size;
};

struct multiboot_tag_string
{
    unsigned int type;
    unsigned int size;
    char string[0];
};

struct multiboot_tag_module
{
    unsigned int type;
    unsigned int size;
    unsigned int mod_start;
    unsigned int mod_end;
    char cmdline[0];
};

struct multiboot_tag_basic_meminfo
{
    unsigned int type;
    unsigned int size;
    unsigned int mem_lower;
    unsigned int mem_upper;
};

struct multiboot_tag_mmap
{
    unsigned int type;
    unsigned int size;
    unsigned int entry_size;
    unsigned int entry_version;
    struct multiboot_mmap_entry entries[0];
};

static void itoa(char *buf, int base, int d);
static void printf(const char *format, ...);

static void print_multiboot_tag(const struct multiboot_tag *tag);

static void print_multiboot_tag_cmdline      (const struct multiboot_tag_string        *tag);
static void print_multiboot_tag_module       (const struct multiboot_tag_module        *tag);
static void print_multiboot_tag_basic_meminfo(const struct multiboot_tag_basic_meminfo *tag);
static void print_multiboot_tag_mmap         (const struct multiboot_tag_mmap          *tag);

void print_multiboot_info(struct KernelMQ_Multiboot_Info info)
{
    if (info.magic == MULTIBOOT_1_MAGIC) {
        printf("Old Multiboot specification does not support modules.");
        return;
    }

    if (info.magic != MULTIBOOT_2_MAGIC) {
        printf("No Multiboot-compliant bootloader is not supported.");
        return;
    }

    if (info.addr & 7) {
        printf("Unaligned Multiboot information address: 0x%x\n", info.addr);
        return;
    }

    for (
        struct multiboot_tag *tag = (struct multiboot_tag*)(info.addr + sizeof(struct KernelMQ_Multiboot_Info));
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag*)((unsigned char*)tag + ((tag->size + 7) & ~7))
    ) {
        print_multiboot_tag(tag);
    }
}

void print_multiboot_tag(const struct multiboot_tag *const tag)
{
    switch (tag->type)
    {
        case MULTIBOOT_TAG_TYPE_CMDLINE:
            print_multiboot_tag_cmdline((struct multiboot_tag_string*)tag);
            break;

        case MULTIBOOT_TAG_TYPE_MODULE:
            print_multiboot_tag_module((struct multiboot_tag_module*)tag);
            break;

        case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
            print_multiboot_tag_basic_meminfo((struct multiboot_tag_basic_meminfo*)tag);
            break;

        case MULTIBOOT_TAG_TYPE_MMAP:
            print_multiboot_tag_mmap((struct multiboot_tag_mmap*)tag);
            break;
    }
}

void print_multiboot_tag_cmdline(const struct multiboot_tag_string *const tag)
{
    printf("Kernel command line: %s\n", tag->string);
}

void print_multiboot_tag_module(const struct multiboot_tag_module *const tag)
{
    printf("Module at 0x%x-0x%x, command line: %s\n", tag->mod_start, tag->mod_end, tag->cmdline);
}

void print_multiboot_tag_basic_meminfo(const struct multiboot_tag_basic_meminfo *const tag)
{
    printf("mem_lower = %uKB, mem_upper = %uKB\n", tag->mem_lower, tag->mem_upper);
}

void print_multiboot_tag_mmap(const struct multiboot_tag_mmap *const tag)
{
    printf("Memory map:\n");

    for (
        const multiboot_memory_map_t *mmap = tag->entries;
        (unsigned char*)mmap < (unsigned char*)tag + tag->size;
        mmap = (multiboot_memory_map_t*)((unsigned long) mmap + tag->entry_size)
    ) {
        printf(
            " base_addr = 0x%x%x, length = 0x%x%x, type = 0x%x\n",
            (unsigned)(mmap->addr >> 32),
            (unsigned)(mmap->addr & 0xffffffff),
            (unsigned)(mmap->len >> 32),
            (unsigned)(mmap->len & 0xffffffff),
            (unsigned)mmap->type
        );
    }
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
