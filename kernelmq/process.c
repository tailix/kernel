#include "process.h"

#include "stdlib.h"

enum KernelMQ_Process_List_InitResult KernelMQ_Process_List_init(
    KernelMQ_Process_List *const process_list
) {
    kmemset(process_list, 0, sizeof(*process_list));

    return KERNELMQ_PROCESS_LIST_INIT_RESULT_OK;
}
