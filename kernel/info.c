#include "info.h"

static unsigned char cmdline_terminated(const char *s);

unsigned char kernel_info_validate(const struct Kernel_Info *const kinfo)
{
    if (!kinfo) {
        return 0;
    }

    if (!cmdline_terminated(kinfo->cmdline)) {
        return 0;
    }

    if (kinfo->modules_count > KERNEL_INFO_MODULES_MAX) {
        return 0;
    }

    if (kinfo->areas_count > KERNEL_INFO_AREAS_MAX) {
        return 0;
    }

    if (kinfo->kernel_size == 0) {
        return 0;
    }

    if (kinfo->kernel_offset == 0) {
        return 0;
    }

    if (kinfo->kernel_phys_base + kinfo->kernel_size != kinfo->kernel_phys_limit + 1) {
        return 0;
    }

    if (kinfo->kernel_virt_base + kinfo->kernel_size != kinfo->kernel_virt_limit + 1) {
        return 0;
    }

    if (kinfo->kernel_virt_base - kinfo->kernel_phys_base != kinfo->kernel_offset) {
        return 0;
    }

    unsigned long modules_total_size = 0;

    for (unsigned int i = 0; i < kinfo->modules_count; ++i) {
        const struct Kernel_Info_Module *const module = &kinfo->modules[i];

        modules_total_size += module->size;

        if (module->size == 0) {
            return 0;
        }

        if (module->base + module->size != module->limit + 1) {
            return 0;
        }

        if (!cmdline_terminated(module->cmdline)) {
            return 0;
        }
    }

    if (kinfo->modules_total_size != modules_total_size) {
        return 0;
    }

    if (kinfo->kernel_and_modules_total_size != kinfo->kernel_size + kinfo->modules_total_size) {
        return 0;
    }

    unsigned long long last = 0;

    for (unsigned int i = 0; i < kinfo->areas_count; ++i) {
        const struct Kernel_Info_Area *const area = &kinfo->areas[i];
        if (last > area->base) {
            return 0;
        }

        if (area->size == 0) {
            return 0;
        }

        if (area->base + area->size != area->limit + 1) {
            return 0;
        }

        last = area->limit + 1;

        if (!area->is_available) {
            if (kinfo->kernel_phys_base >= area->base &&
                kinfo->kernel_phys_base <= area->limit) {
                return 0;
            }

            if (kinfo->kernel_phys_limit >= area->base &&
                kinfo->kernel_phys_limit <= area->limit) {
                return 0;
            }

            for (unsigned int j = 0; j < kinfo->modules_count; ++j) {
                const struct Kernel_Info_Module *const module = &kinfo->modules[j];

                if (module->base >= area->base &&
                    module->base <= area->limit) {
                    return 0;
                }

                if (module->limit >= area->base &&
                    module->limit <= area->limit) {
                    return 0;
                }
            }
        }
    }

    return 1;
}

unsigned char cmdline_terminated(const char *const s)
{
    for (unsigned int i = 0; i < KERNEL_INFO_CMDLINE_SIZE_MAX; ++i) {
        if (s[i] == 0) {
            return 1;
        }
    }

    return 0;
}
