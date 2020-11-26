#include "process.h"

#include "stdlib.h"

static enum KernelMQ_Process_List_InitResult create_kernel_process(
    struct KernelMQ_Process *process,
    const struct KernelMQ_Info *kinfo
)
__attribute__((nonnull));

static enum KernelMQ_Process_List_InitResult create_module_process(
    struct KernelMQ_Process *process,
    const struct KernelMQ_Info *kinfo,
    unsigned int module_index
)
__attribute__((nonnull));

enum KernelMQ_Process_List_InitResult KernelMQ_Process_List_init(
    KernelMQ_Process_List *const process_list,
    const struct KernelMQ_Info *const kinfo
) {
    kmemset(process_list, 0, sizeof(*process_list));

    const enum KernelMQ_Process_List_InitResult create_kernel_process_result =
        create_kernel_process(&(*process_list)[0], kinfo);

    if (create_kernel_process_result != KERNELMQ_PROCESS_LIST_INIT_RESULT_OK) {
        kmemset(process_list, 0, sizeof(*process_list));
        return create_kernel_process_result;
    }

    const unsigned int modules_length = kinfo->modules_count;

    if (modules_length > KERNELMQ_PROCESS_LIST_LENGTH - 1) {
        kmemset(process_list, 0, sizeof(*process_list));
        return KERNELMQ_PROCESS_LIST_INIT_RESULT_MODULES_TOO_MANY;
    }

    for (unsigned int mod_index = 0; mod_index < modules_length; ++mod_index) {
        const enum KernelMQ_Process_List_InitResult create_mod_process_result =
            create_module_process(
                &(*process_list)[mod_index + 1],
                kinfo,
                mod_index
            );

        if (create_mod_process_result != KERNELMQ_PROCESS_LIST_INIT_RESULT_OK) {
            kmemset(process_list, 0, sizeof(*process_list));
            return create_mod_process_result;
        }
    }

    return KERNELMQ_PROCESS_LIST_INIT_RESULT_OK;
}

enum KernelMQ_Process_List_InitResult create_kernel_process(
    struct KernelMQ_Process *const process,
    const struct KernelMQ_Info *const kinfo
) {
    process->is_present = 1;
    process->created_from = KERNELMQ_PROCESS_CREATED_FROM_KERNEL;

    const unsigned int cmdline_slen = kstrlen(kinfo->cmdline);

    if (cmdline_slen > KERNELMQ_PROCESS_CMDLINE_SLEN_MAX) {
        return KERNELMQ_PROCESS_LIST_INIT_RESULT_CMDLINE_TOO_LONG;
    }

    kstrncpy(process->cmdline, kinfo->cmdline, cmdline_slen);

    if (kinfo->areas_count > KERNELMQ_PROCESS_AREAS_LENGTH_MAX) {
        return KERNELMQ_PROCESS_LIST_INIT_RESULT_KERNEL_AREAS_LENGTH_TOO_LONG;
    }

    const unsigned int areas_length = kinfo->areas_count;

    process->areas_length = areas_length;

    for (unsigned int area_index = 0; area_index < areas_length; ++area_index) {
        const unsigned long long base  = kinfo->areas[area_index].base;
        const unsigned long long size  = kinfo->areas[area_index].size;
        const unsigned long long limit = kinfo->areas[area_index].limit;

        if (base > 0xFFFFFFFF || size > 0xFFFFFFFF || limit > 0xFFFFFFFF) {
            return KERNELMQ_PROCESS_LIST_INIT_RESULT_ADDR_TOO_BIG;
        }

        process->areas[area_index].base  = base;
        process->areas[area_index].size  = size;
        process->areas[area_index].limit = limit;
    }

    return KERNELMQ_PROCESS_LIST_INIT_RESULT_OK;
}

enum KernelMQ_Process_List_InitResult create_module_process(
    struct KernelMQ_Process *const process,
    const struct KernelMQ_Info *const kinfo,
    const unsigned int module_index
) {
    process->is_present = 1;
    process->created_from = KERNELMQ_PROCESS_CREATED_FROM_MODULE;

    const unsigned int cmdline_slen = kstrlen(kinfo->modules[module_index].cmdline);

    if (cmdline_slen > KERNELMQ_PROCESS_CMDLINE_SLEN_MAX) {
        return KERNELMQ_PROCESS_LIST_INIT_RESULT_CMDLINE_TOO_LONG;
    }

    kstrncpy(
        process->cmdline,
        kinfo->modules[module_index].cmdline,
        cmdline_slen
    );

    process->areas_length = 1;

    const unsigned long long base  = kinfo->modules[module_index].base;
    const unsigned long long size  = kinfo->modules[module_index].size;
    const unsigned long long limit = kinfo->modules[module_index].limit;

    if (base > 0xFFFFFFFF || size > 0xFFFFFFFF || limit > 0xFFFFFFFF) {
        return KERNELMQ_PROCESS_LIST_INIT_RESULT_ADDR_TOO_BIG;
    }

    process->areas[0].base  = base;
    process->areas[0].size  = size;
    process->areas[0].limit = limit;

    return KERNELMQ_PROCESS_LIST_INIT_RESULT_OK;
}
