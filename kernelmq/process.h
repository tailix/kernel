#ifndef KERNELMQ_INCLUDED_PROCESS
#define KERNELMQ_INCLUDED_PROCESS 1

#include "info.h"

#define KERNELMQ_PROCESS_LIST_LENGTH 20

#define KERNELMQ_PROCESS_CMDLINE_SIZE_MAX 256
#define KERNELMQ_PROCESS_CMDLINE_SLEN_MAX (KERNELMQ_PROCESS_CMDLINE_SIZE_MAX - 1)

#define KERNELMQ_PROCESS_AREAS_LENGTH_MAX 20

#ifdef __cplusplus
extern "C" {
#endif

enum KernelMQ_Process_Error {
    KERNELMQ_PROCESS_ERROR_OK = 0,
    KERNELMQ_PROCESS_ERROR_CMDLINE_TOO_LONG = 1,
    KERNELMQ_PROCESS_ERROR_ADDR_TOO_BIG = 2,
    KERNELMQ_PROCESS_ERROR_INVALID_AREA = 3,
    KERNELMQ_PROCESS_ERROR_KERNEL_AREAS_LENGTH_TOO_LONG = 4,
    KERNELMQ_PROCESS_ERROR_MODULES_TOO_MANY = 5,
};

enum KernelMQ_Process_CreatedFrom {
    KERNELMQ_PROCESS_CREATED_FROM_KERNEL = 0,
    KERNELMQ_PROCESS_CREATED_FROM_MODULE = 1,
};

struct KernelMQ_Process_Area {
    unsigned long base;
    unsigned long size;
    unsigned long limit;
};

struct KernelMQ_Process {
    unsigned char is_present;

    enum KernelMQ_Process_CreatedFrom created_from;

    char cmdline[KERNELMQ_PROCESS_CMDLINE_SIZE_MAX];

    struct KernelMQ_Process_Area areas[KERNELMQ_PROCESS_AREAS_LENGTH_MAX];
    unsigned int areas_length;
};

typedef struct KernelMQ_Process
    KernelMQ_Process_List[KERNELMQ_PROCESS_LIST_LENGTH];

void KernelMQ_Process_List_print(const KernelMQ_Process_List *process_list)
__attribute__((nonnull));

enum KernelMQ_Process_Error KernelMQ_Process_List_init(
    KernelMQ_Process_List *process_list,
    const struct KernelMQ_Info *kinfo
)
__attribute__((nonnull));

enum KernelMQ_Process_Error KernelMQ_Process_create_from_kernel(
    struct KernelMQ_Process *process,
    const struct KernelMQ_Info *kinfo
)
__attribute__((nonnull));

enum KernelMQ_Process_Error KernelMQ_Process_create_from_module(
    struct KernelMQ_Process *process,
    const struct KernelMQ_Info_Module *kinfo_module
)
__attribute__((nonnull));

#ifdef __cplusplus
}
#endif

#endif
