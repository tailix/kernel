#include <kernelmq/info.h>

static unsigned char cmdline_terminated(const char *s);

unsigned char kernelmq_info_validate_and_copy(
    struct KernelMQ_Info *const dest,
    const struct KernelMQ_Info *const src
) {
    if (!src) {
        return 0;
    }

    if (!cmdline_terminated(src->cmdline)) {
        return 0;
    }

    if (src->modules_count > KERNELMQ_INFO_MODULES_MAX) {
        return 0;
    }

    if (src->areas_count > KERNELMQ_INFO_AREAS_MAX) {
        return 0;
    }

    if (src->kernel_size == 0) {
        return 0;
    }

    if (src->kernel_offset == 0) {
        return 0;
    }

    if (src->kernel_phys_base + src->kernel_size != src->kernel_phys_limit + 1) {
        return 0;
    }

    if (src->kernel_virt_base + src->kernel_size != src->kernel_virt_limit + 1) {
        return 0;
    }

    if (src->kernel_virt_base - src->kernel_phys_base != src->kernel_offset) {
        return 0;
    }

    unsigned long modules_total_size = 0;

    for (unsigned int i = 0; i < src->modules_count; ++i) {
        const struct KernelMQ_Info_Module *const module = &src->modules[i];

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

    if (src->modules_total_size != modules_total_size) {
        return 0;
    }

    if (src->kernel_and_modules_total_size != src->kernel_size + src->modules_total_size) {
        return 0;
    }

    unsigned long long last = 0;

    for (unsigned int i = 0; i < src->areas_count; ++i) {
        const struct KernelMQ_Info_Area *const area = &src->areas[i];
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
            if (src->kernel_phys_base >= area->base &&
                src->kernel_phys_base <= area->limit) {
                return 0;
            }

            if (src->kernel_phys_limit >= area->base &&
                src->kernel_phys_limit <= area->limit) {
                return 0;
            }

            for (unsigned int j = 0; j < src->modules_count; ++j) {
                const struct KernelMQ_Info_Module *const module = &src->modules[j];

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

    *dest = *src;

    return 1;
}

unsigned char cmdline_terminated(const char *const s)
{
    for (unsigned int i = 0; i < KERNELMQ_INFO_CMDLINE_SIZE_MAX; ++i) {
        if (s[i] == 0) {
            return 1;
        }
    }

    return 0;
}
