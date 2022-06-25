#ifndef KERNEL_INCLUDED_INFO
#define KERNEL_INCLUDED_INFO 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <kernaux/multiboot2.h>
#include <kernaux/pfa.h>

#define KERNEL_INFO_CMDLINE_SIZE_MAX 256
#define KERNEL_INFO_CMDLINE_SLEN_MAX (KERNEL_INFO_CMDLINE_SIZE_MAX - 1)

#define KERNEL_INFO_MODULES_MAX 20
#define KERNEL_INFO_AREAS_MAX 20

struct Kernel_Info_Module {
    size_t base;
    size_t size;
    size_t limit;

    char cmdline[KERNEL_INFO_CMDLINE_SIZE_MAX];
};

struct Kernel_Info_Area {
    // We use uint64_t instead of size_t because it what Multiboot 2 gives us.
    uint64_t base;
    uint64_t size;
    uint64_t limit;

    bool is_available;
};

struct Kernel_Info {
    bool initialized;

    char cmdline[KERNEL_INFO_CMDLINE_SIZE_MAX];

    struct Kernel_Info_Module modules[KERNEL_INFO_MODULES_MAX];
    size_t modules_count;

    struct Kernel_Info_Area areas[KERNEL_INFO_AREAS_MAX];
    size_t areas_count;

    // Higher-half offset, typically 3 GiB
    size_t kernel_offset;
    size_t kernel_size;
    size_t kernel_phys_base;
    size_t kernel_virt_base;

    size_t modules_total_size;
    size_t kernel_and_modules_total_size;

    size_t kernel_stack_start;
    size_t kernel_stack_size;
};

void kernel_info_init_start(
    struct Kernel_Info *kinfo,
    size_t offset,
    size_t size,
    size_t phys_base,
    size_t virt_base,
    size_t stack_start,
    size_t stack_size
);

void kernel_info_init_finish(struct Kernel_Info *kinfo);

void kernel_info_init_from_multiboot2(
    struct Kernel_Info *kinfo,
    const struct KernAux_Multiboot2_Info *multiboot2_info
);

bool kernel_info_is_valid(const struct Kernel_Info *kinfo);
void kernel_info_print(const struct Kernel_Info *kinfo);
void kernel_info_setup_pfa(const struct Kernel_Info *kinfo, KernAux_PFA pfa);

#ifdef __cplusplus
}
#endif

#endif
