#include <kernelmq/info.h>

unsigned char kernelmq_info_validate_and_copy(
    struct KernelMQ_Info *const dest,
    const struct KernelMQ_Info *const src
) {
    if (!src) {
        return 0;
    }

    *dest = *src;

    return 1;
}
