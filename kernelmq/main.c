#include "paging.h"

#include "info.h"

#include "panic.h"
#include "protected.h"

#include "tasks.h"
#include "elf.h"
#include "logger.h"

#include <kernaux/multiboot2.h>
#include <kernaux/pfa.h>
#include <kernaux/stdlib.h>

// Defined in linker script
extern char _kernel_offset;
extern char _kernel_size;
extern char _kernel_phys_base;
extern char _kernel_virt_base;
extern char _kernel_stack_top;

static struct KernelMQ_Info kinfo;

static struct KernAux_PFA pfa;

void main(
    const unsigned long multiboot2_magic,
    const struct KernAux_Multiboot2 *const multiboot2_info
) {
    if (multiboot2_magic != KERNAUX_MULTIBOOT2_MAGIC) {
        panic("Multiboot 2 magic number is invalid.");
    }

    KernAux_Multiboot2_print(multiboot2_info, print);

    if (!KernAux_Multiboot2_is_valid(multiboot2_info)) {
        panic("Multiboot 2 info is invalid.");
    }

    kernaux_memset(&kinfo, 0, sizeof(struct KernelMQ_Info));

    KernAux_PFA_initialize(&pfa);

    {
        const char *const cmdline =
            KernAux_Multiboot2_boot_cmd_line(multiboot2_info);

        if (cmdline) {
            unsigned int slen = kernaux_strlen(cmdline);

            if (slen > KERNELMQ_INFO_CMDLINE_SLEN_MAX) {
                panic("Multiboot 2 boot cmd line is too long.");
            }

            kernaux_strncpy(kinfo.cmdline, cmdline, slen);
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
            if (entry->type == 1) {
                KernAux_PFA_mark_available(
                    &pfa,
                    entry->base_addr,
                    entry->base_addr + entry->length - 1
                );
            }

            if (kinfo.areas_count >= KERNELMQ_INFO_AREAS_MAX) {
                panic("Too many memory map entries in Multiboot 2 info.");
            }

            struct KernelMQ_Info_Area *const area =
                &kinfo.areas[kinfo.areas_count];

            area->base = entry->base_addr;
            area->size = entry->length;
            area->limit = area->base + area->size - 1;

            area->is_available = entry->type == 1;

            ++kinfo.areas_count;
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
        if (kinfo.modules_count >= KERNELMQ_INFO_MODULES_MAX) {
            panic("Too many modules in Multiboot 2 info.");
        }

        unsigned int slen = kernaux_strlen(tag->cmdline);

        if (slen > KERNELMQ_INFO_CMDLINE_SLEN_MAX) {
            panic("Multiboot 2 module cmd line is too long.");
        }

        struct KernelMQ_Info_Module *const module =
            &kinfo.modules[kinfo.modules_count];

        kernaux_strncpy(module->cmdline, tag->cmdline, slen);

        module->base = tag->mod_start;
        module->limit = tag->mod_end;
        module->size = module->limit - module->base + 1;

        ++kinfo.modules_count;

        kinfo.modules_total_size += module->size;
    }

    kinfo.kernel_offset = (unsigned long)&_kernel_offset;
    kinfo.kernel_size   = (unsigned long)&_kernel_size;

    kinfo.kernel_phys_base = (unsigned long)&_kernel_phys_base;
    kinfo.kernel_virt_base = (unsigned long)&_kernel_virt_base;

    kinfo.kernel_phys_limit = kinfo.kernel_phys_base + kinfo.kernel_size - 1;
    kinfo.kernel_virt_limit = kinfo.kernel_virt_base + kinfo.kernel_size - 1;

    kinfo.kernel_and_modules_total_size = kinfo.kernel_size + kinfo.modules_total_size;

    kinfo.kernel_stack_top = (unsigned long)&_kernel_stack_top;

    paging_clear();
    paging_identity();
    /* kinfo.freepde_start = */ paging_mapkernel(&kinfo);
    paging_load();

    paging_enable();

    assert(kernelmq_info_validate(&kinfo), "Invalid kernel information.");

    protected_initialize(&kinfo);

    if (kinfo.modules_count > 0) {
        const struct KernelMQ_ELF_Header *const elf_header =
            (void*)kinfo.modules[0].base;

        if (KernelMQ_ELF_Header_is_valid(elf_header)) {
            const unsigned long real_entrypoint =
                kinfo.modules[0].base + elf_header->entrypoint;

            tasks_switch_to_user(real_entrypoint);
        }
        else {
            logger_warn_from("init", "Invalid ELF header");
        }
    }
}
