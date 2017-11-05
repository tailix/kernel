#include "memory.h"

#include "config.h"

#include <kernelmq/stdlib.h>

#define FRAMES_COUNT (PAGE_DIR_SIZE * PAGE_TABLE_SIZE) // / sizeof(unsigned char))

static unsigned char frames[FRAMES_COUNT];

static void mark_used(unsigned long base, unsigned long limit);

void memory_initialize(const struct KernelMQ_Info *const kinfo)
{
    kmemset(frames, 0, sizeof(frames));

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

unsigned long memory_alloc_page()
{
    for (unsigned int i = 0; i < FRAMES_COUNT; ++i) {
        if (!frames[i]) {
            return i * PAGE_SIZE;
        }
    }

    return 0;
}

void mark_used(const unsigned long base, const unsigned long limit)
{
    const unsigned int start = base / PAGE_SIZE;

    const unsigned int end = limit % PAGE_SIZE ? limit / PAGE_SIZE : limit / PAGE_SIZE + 1;

    for (unsigned int i = start; i <= end; ++i) {
        frames[i] = 1;
    }
}
