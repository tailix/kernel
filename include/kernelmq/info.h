#ifndef KERNELMQ_INCLUDED_INFO
#define KERNELMQ_INCLUDED_INFO 1

#ifdef __cplusplus
extern "C" {
#endif

#define KERNELMQ_INFO_CMDLINE_SIZE_MAX 256

#define KERNELMQ_INFO_MODULES_MAX 20
#define KERNELMQ_INFO_AREAS_MAX 20

#define KERNELMQ_INFO_MEM_LOWER_BASE ((unsigned long)0)
#define KERNELMQ_INFO_MEM_UPPER_BASE ((unsigned long)(1 * 1024 * 1024 * 1024)) // 1 MB

#define KERNELMQ_INFO_MEM_LOWER_SIZE_MAX ((unsigned long)(640 * 1024)) // 640 KB

struct KernelMQ_Info_Module {
    unsigned long base;
    unsigned long size;
    unsigned long limit;

    char cmdline[KERNELMQ_INFO_CMDLINE_SIZE_MAX];
};

struct KernelMQ_Info_Area {
    unsigned long long base;
    unsigned long long size;
    unsigned long long limit;

    unsigned char is_available;
};

struct KernelMQ_Info {
    char cmdline[KERNELMQ_INFO_CMDLINE_SIZE_MAX];

    struct KernelMQ_Info_Module modules[KERNELMQ_INFO_MODULES_MAX];
    unsigned int modules_count;

    struct KernelMQ_Info_Area areas[KERNELMQ_INFO_AREAS_MAX];
    unsigned int areas_count;

    unsigned long mem_lower_base;
    unsigned long mem_lower_size;
    unsigned long mem_lower_limit;

    unsigned long mem_upper_base;
    unsigned long mem_upper_size;
    unsigned long mem_upper_limit;

    unsigned long kernel_offset;

    unsigned long kernel_phys_base;
    unsigned long kernel_phys_limit;

    unsigned long kernel_virt_base;
    unsigned long kernel_virt_limit;

    unsigned long kernel_size;

    unsigned long modules_total_size;
    unsigned long kernel_and_modules_total_size;
};

unsigned char kernelmq_info_validate_and_copy(struct KernelMQ_Info *dest, const struct KernelMQ_Info *src);

#ifdef __cplusplus
}
#endif

#endif
