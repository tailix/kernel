#include "pfa.h"

#include "config.h"
#include "panic.h"
#include "logger.h"

#include <kernelmq/stdlib.h>

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

unsigned long pfa_alloc_page()
{
    for (unsigned int i = 0; i < FRAMES_COUNT; ++i) {
        if (!frames[i]) {
            frames[i] = 0xFF;
            return i * PAGE_SIZE;
        }
    }

    return 0;
}

unsigned long pfa_alloc_big_page()
{
    unsigned int start = 0;

    for (unsigned int i = 0; i < FRAMES_COUNT; ++i) {
        if (frames[i]) {
            start = i + 1;
            continue;
        }

        if (start % (PAGE_BIG_SIZE / PAGE_SIZE)) {
            continue;
        }

        if (i - start + 1 == PAGE_BIG_SIZE / PAGE_SIZE) {
            for (unsigned int j = start; j <= i; ++j) {
                frames[j] = 0xFF;
            }

            return start * PAGE_SIZE;
        }
    }

    return 0;
}

void pfa_free_page(const unsigned long base)
{
    assert(!(base % PAGE_SIZE), "Small page address to free is not aligned.");

    const unsigned long i = base / PAGE_SIZE;

    if (i >= FRAMES_COUNT) {
        return;
    }

    frames[i] = 0;
}

void pfa_free_big_page(const unsigned long base)
{
    assert(!(base % PAGE_BIG_SIZE), "Big page address to free is not aligned.");

    const unsigned long start = base / PAGE_SIZE;
    const unsigned long end   = start + PAGE_BIG_SIZE / PAGE_SIZE;

    for (unsigned int i = start; i <= end && i < FRAMES_COUNT; ++i) {
        frames[i] = 0;
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
