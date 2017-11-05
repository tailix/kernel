#include "multiboot.h"

#include <kernelmq/stdlib.h>

#define MULTIBOOT_TAG_TYPE_END     0
#define MULTIBOOT_TAG_TYPE_CMDLINE 1
#define MULTIBOOT_TAG_TYPE_MODULE  3
#define MULTIBOOT_TAG_TYPE_MMAP    6

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

struct multiboot_tag_mmap
{
    unsigned int type;
    unsigned int size;
    unsigned int entry_size;
    unsigned int entry_version;
    struct multiboot_mmap_entry entries[0];
};

static unsigned char print_multiboot_tag(struct KernelMQ_Info *kinfo, const struct multiboot_tag *tag);

static unsigned char print_multiboot_tag_cmdline(struct KernelMQ_Info *kinfo, const struct multiboot_tag_string *tag);
static unsigned char print_multiboot_tag_module (struct KernelMQ_Info *kinfo, const struct multiboot_tag_module *tag);
static unsigned char print_multiboot_tag_mmap   (struct KernelMQ_Info *kinfo, const struct multiboot_tag_mmap   *tag);

unsigned char multiboot_parse(struct KernelMQ_Info *kinfo, unsigned long addr)
{
    if (!kinfo) {
        return 0;
    }

    // Unaligned address
    if (addr & 7) {
        return 0;
    }

    for (
        struct multiboot_tag *tag = (struct multiboot_tag*)(addr + 8);
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag*)((unsigned char*)tag + ((tag->size + 7) & ~7))
    ) {
        if (!print_multiboot_tag(kinfo, tag)) {
            return 0;
        }
    }

    return 1;
}

unsigned char print_multiboot_tag(struct KernelMQ_Info *kinfo, const struct multiboot_tag *const tag)
{
    switch (tag->type)
    {
        case MULTIBOOT_TAG_TYPE_CMDLINE:
            return print_multiboot_tag_cmdline(kinfo, (struct multiboot_tag_string*)tag);

        case MULTIBOOT_TAG_TYPE_MODULE:
            return print_multiboot_tag_module(kinfo, (struct multiboot_tag_module*)tag);

        case MULTIBOOT_TAG_TYPE_MMAP:
            return print_multiboot_tag_mmap(kinfo, (struct multiboot_tag_mmap*)tag);
    }

    return 1;
}

unsigned char print_multiboot_tag_cmdline(struct KernelMQ_Info *kinfo, const struct multiboot_tag_string *const tag)
{
    unsigned int length = kstrlen(tag->string);

    if (length >= KERNELMQ_INFO_CMDLINE_SIZE_MAX) {
        return 0;
    }

    kstrncpy(kinfo->cmdline, tag->string, length);

    return 1;
}

unsigned char print_multiboot_tag_module(struct KernelMQ_Info *kinfo, const struct multiboot_tag_module *const tag)
{
    if (kinfo->modules_count >= KERNELMQ_INFO_MODULES_MAX) {
        return 0;
    }

    unsigned int cmdline_length = kstrlen(tag->cmdline);

    if (cmdline_length >= KERNELMQ_INFO_CMDLINE_SIZE_MAX) {
        return 0;
    }

    struct KernelMQ_Info_Module *module = &kinfo->modules[kinfo->modules_count];

    kstrncpy(module->cmdline, tag->cmdline, cmdline_length);

    module->base  = tag->mod_start;
    module->limit = tag->mod_end;
    module->size  = module->limit - module->base + 1;

    ++kinfo->modules_count;

    kinfo->modules_total_size += module->size;

    return 1;
}

unsigned char print_multiboot_tag_mmap(struct KernelMQ_Info *kinfo, const struct multiboot_tag_mmap *const tag)
{
    for (
        const multiboot_memory_map_t *mmap = tag->entries;
        (unsigned char*)mmap < (unsigned char*)tag + tag->size;
        mmap = (multiboot_memory_map_t*)((unsigned long) mmap + tag->entry_size)
    ) {
        if (kinfo->areas_count >= KERNELMQ_INFO_AREAS_MAX) {
            return 0;
        }

        struct KernelMQ_Info_Area *area = &kinfo->areas[kinfo->areas_count];

        area->base  = mmap->addr;
        area->size  = mmap->len;
        area->limit = area->base + area->size - 1;

        area->is_available = mmap->type == MULTIBOOT_MEMORY_AVAILABLE;

        ++kinfo->areas_count;
    }

    return 1;
}
