#ifndef KERNELMQ_INCLUDED_PFA
#define KERNELMQ_INCLUDED_PFA 1

#include "info.h"

#ifdef __cplusplus
extern "C" {
#endif

void pfa_initialize(const struct KernelMQ_Info *kinfo);

void pfa_print_info();

#ifdef __cplusplus
}
#endif

#endif
