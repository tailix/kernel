#include "info.h"

#include <kernaux/libc.h>

static bool cmdline_terminated(const char *s);

void kernel_info_initialize(struct Kernel_Info *const kinfo)
{
    memset(kinfo, 0, sizeof(*kinfo));
}

bool kernel_info_validate(const struct Kernel_Info *const kinfo)
{
    if (!kinfo) return false;

    if (!cmdline_terminated(kinfo->cmdline)) return false;

    if (kinfo->modules_count > KERNEL_INFO_MODULES_MAX) return false;
    if (kinfo->areas_count   > KERNEL_INFO_AREAS_MAX)   return false;

    if (kinfo->kernel_offset == 0) return false;
    if (kinfo->kernel_size   == 0) return false;

    if (kinfo->kernel_phys_base + kinfo->kernel_size !=
        kinfo->kernel_phys_limit + 1)
    {
        return false;
    }

    if (kinfo->kernel_virt_base + kinfo->kernel_size !=
        kinfo->kernel_virt_limit + 1)
    {
        return false;
    }

    if (kinfo->kernel_virt_base - kinfo->kernel_phys_base !=
        kinfo->kernel_offset)
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

            if (kinfo->kernel_phys_limit >= area->base &&
                kinfo->kernel_phys_limit <= area->limit)
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

    return 1;
}

bool cmdline_terminated(const char *const str)
{
    for (size_t i = 0; i < KERNEL_INFO_CMDLINE_SIZE_MAX; ++i) {
        if (str[i] == '\0') return true;
    }

    return false;
}
