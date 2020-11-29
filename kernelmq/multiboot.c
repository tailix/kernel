#include "multiboot.h"

#include "stdlib.h"

#define MULTIBOOT_TAG_TYPE_END    0
#define MULTIBOOT_TAG_TYPE_MODULE 3

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

static unsigned char print_multiboot_tag(struct KernelMQ_Info *kinfo, const struct multiboot_tag *tag);

static unsigned char print_multiboot_tag_module(struct KernelMQ_Info *kinfo, const struct multiboot_tag_module *tag);

unsigned char multiboot_parse(struct KernelMQ_Info *kinfo, unsigned long base)
{
    if (!kinfo) {
        return 0;
    }

    // Unaligned address
    if (base & 7) {
        return 0;
    }

    for (
        struct multiboot_tag *tag = (struct multiboot_tag*)(base + 8);
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
        case MULTIBOOT_TAG_TYPE_MODULE:
            return print_multiboot_tag_module(kinfo, (struct multiboot_tag_module*)tag);
    }

    return 1;
}

unsigned char print_multiboot_tag_module(struct KernelMQ_Info *kinfo, const struct multiboot_tag_module *const tag)
{
    if (kinfo->modules_count >= KERNELMQ_INFO_MODULES_MAX) {
        return 0;
    }

    unsigned int cmdline_slen = kstrlen(tag->cmdline);

    if (cmdline_slen > KERNELMQ_INFO_CMDLINE_SLEN_MAX) {
        return 0;
    }

    struct KernelMQ_Info_Module *module = &kinfo->modules[kinfo->modules_count];

    kstrncpy(module->cmdline, tag->cmdline, cmdline_slen);

    module->base  = tag->mod_start;
    module->limit = tag->mod_end;
    module->size  = module->limit - module->base + 1;

    ++kinfo->modules_count;

    kinfo->modules_total_size += module->size;

    return 1;
}
