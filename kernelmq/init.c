#include "panic.h"
#include "pfa.h"
#include "protected.h"
#include "paging.h"

#include "info.h"
#include "stdlib.h"
#include "module.h"

#include "tasks.h"
#include "elf.h"
#include "logger.h"

static struct KernelMQ_Info kinfo;

void init(const struct KernelMQ_Info *const kinfo_ptr)
{
    kmemset(&kinfo, 0, sizeof(struct KernelMQ_Info));

    assert(kernelmq_info_validate_and_copy(&kinfo, kinfo_ptr), "Invalid kernel information.");

    pfa_initialize(&kinfo);

    protected_initialize(&kinfo);

    // Set up a new post-relocate bootstrap pagetable so that
    // we can map in VM, and we no longer rely on pre-relocated
    // data.
    paging_clear();
    paging_identity(); // Still need 1:1 for lapic and video mem and such.
    paging_mapkernel(&kinfo);
    paging_load();

    if (kinfo.modules_count > 0) {
        const struct KernelMQ_ELF_Header *const elf_header =
            (void*)kinfo.modules[0].base;

        const unsigned char is_elf_header_valid =
            elf_header->magic_0x7F     == 0x7F &&
            elf_header->magic_E        == 'E'  &&
            elf_header->magic_L        == 'L'  &&
            elf_header->magic_F        == 'F'  &&
            elf_header->bitness        == 1    &&
            elf_header->endianness     == 1    &&
            elf_header->header_version == 1    &&
            elf_header->os_abi         == 0    &&
            elf_header->obj_type       == 2    &&
            elf_header->isa            == 3    &&
            elf_header->elf_version    == 1    &&
            elf_header->arch_flags     == 0    &&
            elf_header->header_size    == 52;

        if (is_elf_header_valid) {
            tasks_switch_to_user(elf_header->entrypoint);
        }
        else {
            logger_warn_from("init", "Invalid ELF header");
        }
    }
}
