#include "pfa.h"

#include "config.h"
#include "panic.h"
#include "logger.h"

#include "stdlib.h"

#define FRAMES_COUNT (PAGE_DIR_LENGTH * PAGE_TABLE_LENGTH)

static unsigned char frames[FRAMES_COUNT];

static void mark_used(unsigned long base, unsigned long limit);

void pfa_initialize(const struct KernelMQ_Info *const kinfo)
{
    logger_info_from("pfa", "Initialize page frame allocator.");

    kmemset(frames, 0, sizeof(frames));

    mark_used(0, MEM_UPPER_BASE - 1);

    mark_used(kinfo->kernel_phys_base, kinfo->kernel_phys_limit);

    for (unsigned int i = 0; i < kinfo->modules_count; ++i) {
        const struct KernelMQ_Info_Module *const module = &kinfo->modules[i];

        mark_used(module->base, module->limit);
    }

    for (unsigned int i = 0; i < kinfo->areas_count; ++i) {
        const struct KernelMQ_Info_Area *const area = &kinfo->areas[i];

        if (!area->is_available) {
            mark_used(area->base, area->limit);
        }
    }
}

void pfa_print_info()
{
    unsigned char started = 0;
    unsigned int start = 0;

    for (unsigned int index = 0; index <= FRAMES_COUNT; ++index) {
        if (frames[index]) {
            if (!started) {
                started = 1;
                start = index;
            }
        }
        else {
            if (started) {
                const unsigned int end = index - 1;

                if (start < end) {
                    logger_debug_from(
                        "pfa",
                        "Pages allocated: %u-%u",
                        start * PAGE_SIZE,
                        end * PAGE_SIZE
                    );
                }
                else {
                    logger_debug_from(
                        "pfa",
                        "Page allocated: %u",
                        start * PAGE_SIZE
                    );
                }
            }

            started = 0;
        }
    }
}

void mark_used(const unsigned long base, const unsigned long limit)
{
    const unsigned int start = base  / PAGE_SIZE;
    const unsigned int end   = limit / PAGE_SIZE;

    for (unsigned int i = start; i <= end; ++i) {
        frames[i] = 0xFF;
    }
}
