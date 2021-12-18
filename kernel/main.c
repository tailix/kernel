#include "paging.h"

#include "info.h"

#include "panic.h"
#include "protected.h"

#include "tasks.h"
#include "elf.h"

#include <kernaux/console.h>
#include <kernaux/libc.h>
#include <kernaux/multiboot2.h>
#include <kernaux/pfa.h>
#include <kernaux/stdlib.h>

// Defined in linker script
extern char _kernel_offset;
extern char _kernel_size;
extern char _kernel_phys_base;
extern char _kernel_virt_base;
extern char _kernel_stack_top;

static struct Kernel_Info kinfo;
static struct KernAux_PFA pfa;
static struct Paging paging;

void main(
    const unsigned long multiboot2_magic,
    const struct KernAux_Multiboot2 *const multiboot2_info
) {
    kernaux_assert_cb = kernaux_assert_fn;

    if (multiboot2_magic != KERNAUX_MULTIBOOT2_MAGIC) {
        panic("Multiboot 2 magic number is invalid.");
    }

    KernAux_Multiboot2_print(multiboot2_info, kernaux_console_printf);

    if (!KernAux_Multiboot2_is_valid(multiboot2_info)) {
        panic("Multiboot 2 info is invalid.");
    }

    kernel_info_init_start(
        &kinfo,
        (size_t)&_kernel_offset,
        (size_t)&_kernel_size,
        (size_t)&_kernel_phys_base,
        (size_t)&_kernel_virt_base,
        (size_t)&_kernel_stack_top
    );

    KernAux_PFA_initialize(&pfa);

    kernel_info_init_cmdline(&kinfo,
                             KernAux_Multiboot2_boot_cmd_line(multiboot2_info));

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

            if (kinfo.areas_count >= KERNEL_INFO_AREAS_MAX) {
                panic("Too many memory map entries in Multiboot 2 info.");
            }

            struct Kernel_Info_Area *const area =
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
        if (kinfo.modules_count >= KERNEL_INFO_MODULES_MAX) {
            panic("Too many modules in Multiboot 2 info.");
        }

        unsigned int slen = strlen(tag->cmdline);

        if (slen > KERNEL_INFO_CMDLINE_SLEN_MAX) {
            panic("Multiboot 2 module cmd line is too long.");
        }

        struct Kernel_Info_Module *const module =
            &kinfo.modules[kinfo.modules_count];

        strcpy(module->cmdline, tag->cmdline);

        module->base = tag->mod_start;
        module->limit = tag->mod_end;
        module->size = module->limit - module->base + 1;

        ++kinfo.modules_count;

        kinfo.modules_total_size += module->size;
    }

    kernel_info_init_finish(&kinfo);
    kernel_info_print(&kinfo);
    assert(kernel_info_is_valid(&kinfo), "Invalid kernel information.");

    // TODO: maybe rename to init?
    paging_clear(&paging);

    paging_identity(&paging);
    paging_mapkernel(&paging, &kinfo);

    paging_load(&paging);
    paging_enable();

    protected_initialize(&kinfo);

    if (kinfo.modules_count > 0) {
        const struct Kernel_ELF_Header *const elf_header =
            (void*)kinfo.modules[0].base;

        if (Kernel_ELF_Header_is_valid(elf_header)) {
            const unsigned long real_entrypoint =
                kinfo.modules[0].base + elf_header->entrypoint;

            tasks_switch_to_user(real_entrypoint);
        }
        else {
            kernaux_console_print("[WARN] init: Invalid ELF header.\n");
        }
    }
}
