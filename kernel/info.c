#include "info.h"
#include "panic.h"

#include <kernaux/assert.h>
#include <kernaux/console.h>
#include <kernaux/libc.h>

static bool cmdline_terminated(const char *s);

/******************
 * Initialization *
 ******************/

void kernel_info_init_start(
    struct Kernel_Info *const kinfo,
    const size_t offset,
    const size_t size,
    const size_t phys_base,
    const size_t virt_base,
    const size_t stack_start,
    const size_t stack_size
) {
    KERNAUX_NOTNULL_RETURN(kinfo);

    memset(kinfo, 0, sizeof(*kinfo));

    kinfo->initialized = false;

    kinfo->kernel_offset = offset;
    kinfo->kernel_size   = size;

    kinfo->kernel_phys_base = phys_base;
    kinfo->kernel_virt_base = virt_base;

    kinfo->kernel_stack_start = stack_start;
    kinfo->kernel_stack_size  = stack_size;
}

void kernel_info_init_finish(struct Kernel_Info *const kinfo)
{
    KERNAUX_NOTNULL_RETURN(kinfo);
    KERNAUX_ASSERT_RETURN(!kinfo->initialized);

    kinfo->kernel_and_modules_total_size =
        kinfo->kernel_size + kinfo->modules_total_size;

    kinfo->initialized = true;
}

void kernel_info_init_from_multiboot2(
    struct Kernel_Info *kinfo,
    const struct KernAux_Multiboot2 *multiboot2_info
) {
    KERNAUX_NOTNULL_RETURN(kinfo);
    KERNAUX_NOTNULL_RETURN(multiboot2_info);
    KERNAUX_ASSERT_RETURN(!kinfo->initialized);

    {
        const char *const cmdline =
            KernAux_Multiboot2_boot_cmd_line(multiboot2_info);

        if (cmdline) {
            assert(strlen(cmdline) <= KERNEL_INFO_CMDLINE_SLEN_MAX,
                   "Kernel cmdline is too long");

            strcpy(kinfo->cmdline, cmdline);
        } else {
            memset(kinfo->cmdline, '\0', sizeof(kinfo->cmdline));
        }
    }

    {
        const struct KernAux_Multiboot2_Tag_MemoryMap *const tag =
            (struct KernAux_Multiboot2_Tag_MemoryMap*)
            KernAux_Multiboot2_first_tag_with_type(
                multiboot2_info,
                KERNAUX_MULTIBOOT2_TAGTYPE_MEMORY_MAP
            );

        if (!tag) {
            panic("No memory map provided in Multiboot 2 info.");
        }

        for (
            const struct KernAux_Multiboot2_Tag_MemoryMap_EntryBase *entry =
                (struct KernAux_Multiboot2_Tag_MemoryMap_EntryBase*)tag->data;
            (unsigned char*)entry < (unsigned char*)tag + tag->base.size;
            entry =
                (struct KernAux_Multiboot2_Tag_MemoryMap_EntryBase*)
                ((unsigned char*)entry + tag->entry_size)
        ) {
            if (kinfo->areas_count >= KERNEL_INFO_AREAS_MAX) {
                panic("Too many memory map entries in Multiboot 2 info.");
            }

            struct Kernel_Info_Area *const area =
                &kinfo->areas[kinfo->areas_count];

            area->base = entry->base_addr;
            area->size = entry->length;
            area->limit = area->base + area->size - 1;

            area->is_available = entry->type == 1;

            ++kinfo->areas_count;
        }
    }

    for (
        const struct KernAux_Multiboot2_Tag_Module *tag =
            (struct KernAux_Multiboot2_Tag_Module*)
            KernAux_Multiboot2_first_tag_with_type(
                multiboot2_info,
                KERNAUX_MULTIBOOT2_TAGTYPE_MODULE
            );
        tag;
        tag = (struct KernAux_Multiboot2_Tag_Module*)
            KernAux_Multiboot2_tag_with_type_after(
                multiboot2_info,
                KERNAUX_MULTIBOOT2_TAGTYPE_MODULE,
                (struct KernAux_Multiboot2_TagBase*)tag
            )
    ) {
        if (kinfo->modules_count >= KERNEL_INFO_MODULES_MAX) {
            panic("Too many modules in Multiboot 2 info.");
        }

        unsigned int slen = strlen(tag->cmdline);

        if (slen > KERNEL_INFO_CMDLINE_SLEN_MAX) {
            panic("Multiboot 2 module cmd line is too long.");
        }

        struct Kernel_Info_Module *const module =
            &kinfo->modules[kinfo->modules_count];

        strcpy(module->cmdline, tag->cmdline);

        module->base = tag->mod_start;
        module->limit = tag->mod_end;
        module->size = module->limit - module->base + 1;

        ++kinfo->modules_count;

        kinfo->modules_total_size += module->size;
    }
}

/*******************************
 * Other kernel info functions *
 *******************************/

void kernel_info_print(const struct Kernel_Info *const kinfo)
{
    KERNAUX_NOTNULL_RETURN(kinfo);

    kernaux_console_printf("Kernel info\n");
    kernaux_console_printf("  cmdline: %s\n", kinfo->cmdline);
    kernaux_console_printf("  modules: %lu\n", kinfo->modules_count);
    kernaux_console_printf("  areas: %lu\n", kinfo->areas_count);
    kernaux_console_printf("\n");
    kernaux_console_printf("  offset: %lu\n", kinfo->kernel_offset);
    kernaux_console_printf("  size: %lu\n", kinfo->kernel_size);
    kernaux_console_printf("  phys base: %lu\n", kinfo->kernel_phys_base);
    kernaux_console_printf("  virt base: %lu\n", kinfo->kernel_virt_base);
    kernaux_console_printf("\n");
    kernaux_console_printf("  modules size: %lu\n", kinfo->modules_total_size);
    kernaux_console_printf("  kernel & modules size: %lu\n", kinfo->kernel_and_modules_total_size);
    kernaux_console_printf("\n");
    kernaux_console_printf("  stack start: %lu\n", kinfo->kernel_stack_start);
    kernaux_console_printf("  stack size: %lu\n", kinfo->kernel_stack_size);
}

bool kernel_info_is_valid(const struct Kernel_Info *const kinfo)
{
    KERNAUX_NOTNULL_RETVAL(kinfo, false);
    KERNAUX_ASSERT_RETVAL(kinfo->initialized, false);

    if (!cmdline_terminated(kinfo->cmdline)) return false;

    if (kinfo->modules_count > KERNEL_INFO_MODULES_MAX) return false;
    if (kinfo->areas_count   > KERNEL_INFO_AREAS_MAX)   return false;

    if (kinfo->kernel_offset == 0) return false;
    if (kinfo->kernel_size   == 0) return false;

    if (kinfo->kernel_virt_base - kinfo->kernel_phys_base !=
        kinfo->kernel_offset)
    {
        return false;
    }

    if (kinfo->kernel_stack_size != 16 * 1024) return false;

    if (!(kinfo->kernel_stack_start >= kinfo->kernel_phys_base &&
          kinfo->kernel_stack_start <
          kinfo->kernel_phys_base + kinfo->kernel_size))
    {
        return false;
    }

    const size_t stack_end =
        kinfo->kernel_stack_start + kinfo->kernel_stack_size;

    if (!(stack_end - 1 >= kinfo->kernel_phys_base &&
          stack_end - 1 < kinfo->kernel_phys_base + kinfo->kernel_size))
    {
        return false;
    }

    size_t modules_total_size = 0;

    for (size_t i = 0; i < kinfo->modules_count; ++i) {
        const struct Kernel_Info_Module *const module = &kinfo->modules[i];

        modules_total_size += module->size;

        if (module->size == 0) return false;
        if (module->base + module->size != module->limit + 1) return false;
        if (!cmdline_terminated(module->cmdline)) return false;
    }

    if (kinfo->modules_total_size != modules_total_size) return false;

    if (kinfo->kernel_and_modules_total_size !=
        kinfo->kernel_size + kinfo->modules_total_size)
    {
        return false;
    }

    uint64_t last = 0;

    for (size_t i = 0; i < kinfo->areas_count; ++i) {
        const struct Kernel_Info_Area *const area = &kinfo->areas[i];

        if (last > area->base) return false;
        if (area->size == 0) return false;
        if (area->base + area->size != area->limit + 1) return false;

        last = area->limit + 1;

        if (!area->is_available) {
            if (kinfo->kernel_phys_base >= area->base &&
                kinfo->kernel_phys_base <= area->limit)
            {
                return false;
            }

            if (kinfo->kernel_phys_base + kinfo->kernel_size - 1 >= area->base
                &&
                kinfo->kernel_phys_base + kinfo->kernel_size - 1 <= area->limit)
            {
                return false;
            }

            for (unsigned int j = 0; j < kinfo->modules_count; ++j) {
                const struct Kernel_Info_Module *const module =
                    &kinfo->modules[j];

                if (module->base >= area->base && module->base <= area->limit) {
                    return false;
                }

                if (module->limit >= area->base &&
                    module->limit <= area->limit)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

void kernel_info_setup_pfa(
    const struct Kernel_Info *const kinfo,
    const KernAux_PFA pfa
) {
    KERNAUX_NOTNULL_RETURN(kinfo);
    KERNAUX_NOTNULL_RETURN(pfa);
    KERNAUX_ASSERT_RETURN(kinfo->initialized);

    for (size_t i = 0; i < kinfo->areas_count; ++i) {
        const struct Kernel_Info_Area *const area = &kinfo->areas[i];

        if (area->is_available) {
            KernAux_PFA_mark_available(pfa, area->base, area->limit);
        }
    }
}

/*********************
 * Utility functions *
 *********************/

bool cmdline_terminated(const char *const str)
{
    for (size_t i = 0; i < KERNEL_INFO_CMDLINE_SIZE_MAX; ++i) {
        if (str[i] == '\0') return true;
    }

    return false;
}
