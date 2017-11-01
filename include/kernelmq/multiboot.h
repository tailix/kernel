#ifndef KERNELMQ_INCLUDED_MULTIBOOT
#define KERNELMQ_INCLUDED_MULTIBOOT 1

#ifdef __cplusplus
extern "C" {
#endif

struct KernelMQ_Multiboot_Info {
    unsigned long magic;
    unsigned long addr;
};

void print_multiboot_info(struct KernelMQ_Multiboot_Info info);

#ifdef __cplusplus
}
#endif

#endif
