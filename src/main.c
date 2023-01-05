#define KERNAUX_ACCESS_PROTECTED

#include "heap.h"
#include "interrupts.h"
#include "paging.h"
#include "panic.h"
#include "protected.h"

#include <drivers/console.h>
#include <kernaux/generic/display.h>
#include <kernaux/macro.h>
#include <kernaux/memmap.h>
#include <kernaux/multiboot2.h>
#include <kernaux/pfa.h>
#include <kernaux/runtime.h>

#include <stdarg.h>
#include <stdbool.h>
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
            KernAux_Memmap_Builder_new(heap_malloc);
        assert(builder, "builder");

        const bool result = KernAux_Multiboot2_Info_to_memmap(
            multiboot2_info,
            builder,
            NULL
        );
        assert(result, "result");

        memmap = KernAux_Memmap_Builder_finish_and_free(builder);
        assert(memmap, "memmap");
    }

    {
        KernAux_Display_println(&display, "Memory map:");
        KernAux_Memmap_print(memmap, &display);
    }

    KernAux_PFA_initialize(&pfa);

    // TODO: maybe rename to init?
    paging_clear(&paging);

    paging_identity(&paging);
    paging_mapkernel(&paging);

    paging_load(&paging);
    paging_enable();

    interrupts_init();

    protected_initialize();

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
