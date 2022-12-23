#define KERNAUX_ACCESS_PROTECTED

#include "paging.h"

#include "info.h"

#include "heap.h"
#include "panic.h"
#include "protected.h"
#include "interrupts.h"

#include <drivers/console.h>
#include <kernaux/generic/display.h>
#include <kernaux/macro.h>
#include <kernaux/memmap.h>
#include <kernaux/multiboot2.h>
#include <kernaux/pfa.h>
#include <kernaux/runtime.h>

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

// Defined in linker script
extern uint8_t _kernel_offset;
extern uint8_t _kernel_size;
extern uint8_t _kernel_phys_base;
extern uint8_t _kernel_virt_base;
extern uint8_t _kernel_stack_start;
extern uint8_t _kernel_stack_size;

static KernAux_Memmap memmap = NULL;

static struct Kernel_Info kinfo;
static struct KernAux_PFA pfa;
static struct Paging paging;

void my_putc(void *display KERNAUX_UNUSED, char c);
void my_vprintf(void *display KERNAUX_UNUSED, const char *format, va_list va);

static const struct KernAux_Display display = {
    .putc = my_putc,
    .vprintf = my_vprintf,
};

void main(
    const unsigned long multiboot2_info_magic,
    const struct KernAux_Multiboot2_Info *const multiboot2_info
) {
    kernaux_assert_cb = kernaux_assert_fn;

    if (multiboot2_info_magic != KERNAUX_MULTIBOOT2_INFO_MAGIC) {
        panic("Multiboot 2 info magic number is invalid.");
    }

    KernAux_Multiboot2_Info_print(multiboot2_info, &display);

    if (!KernAux_Multiboot2_Info_is_valid(multiboot2_info)) {
        panic("Multiboot 2 info is invalid.");
    }

    heap_initialize();

    {
        const KernAux_Memmap_Builder builder =
            KernAux_Multiboot2_Info_to_memmap_builder(
                multiboot2_info,
                heap_malloc
            );
        assert(builder, "builder");
        memmap = KernAux_Memmap_Builder_finish_and_free(builder);
        assert(memmap, "memmap");
    }

    {
        KernAux_Display_println(&display, "Memory map:");
        KernAux_Memmap_print(memmap, &display);
    }

    kernel_info_init_start(
        &kinfo,
        (size_t)&_kernel_offset,
        (size_t)&_kernel_size,
        (size_t)&_kernel_phys_base,
        (size_t)&_kernel_virt_base,
        (size_t)&_kernel_stack_start,
        (size_t)&_kernel_stack_size
    );
    kernel_info_init_from_multiboot2(&kinfo, multiboot2_info);
    kernel_info_init_finish(&kinfo);
    kernel_info_print(&kinfo);
    assert(kernel_info_is_valid(&kinfo), "Invalid kernel information.");

    KernAux_PFA_initialize(&pfa);
    kernel_info_setup_pfa(&kinfo, &pfa);

    // TODO: maybe rename to init?
    paging_clear(&paging);

    paging_identity(&paging);
    paging_mapkernel(&paging, &kinfo);

    paging_load(&paging);
    paging_enable();

    interrupts_init();

    protected_initialize(&kinfo);

    drivers_console_puts("[INFO] main: Finished.");
}

void my_putc(void *display KERNAUX_UNUSED, char c)
{
    drivers_console_putc(c);
}

void my_vprintf(void *display KERNAUX_UNUSED, const char *format, va_list va)
{
    drivers_console_vprintf(format, va);
}
