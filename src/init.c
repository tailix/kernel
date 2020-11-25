#include "panic.h"
#include "pfa.h"
#include "protected.h"
#include "paging.h"

#include "info.h"
#include "stdlib.h"
#include "module.h"

#include "tasks.h"

static struct KernelMQ_Info kinfo;

void init(const struct KernelMQ_Info *const kinfo_ptr)
{
    kmemset(&kinfo, 0, sizeof(struct KernelMQ_Info));

    assert(kernelmq_info_validate_and_copy(&kinfo, kinfo_ptr), "Invalid kernel information.");

    pfa_initialize(&kinfo);

    protected_initialize(&kinfo);

    // Set up a new post-relocate bootstrap pagetable so that
    // we can map in VM, and we no longer rely on pre-relocated
    // data.
    paging_clear();
    paging_identity(); // Still need 1:1 for lapic and video mem and such.
    paging_mapkernel(&kinfo);
    paging_load();

    if (kinfo.modules_count > 0) {
        tasks_switch_to_user(kinfo.modules[0].base);
    }
}
