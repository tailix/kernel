#include "hwint.h"

#include "main.h"
#include "../config.h"

#include <kernaux/drivers/console.h>
#include <kernaux/drivers/intel_8259_pic.h>

static hwint_handler_t handlers[INT_HWINT_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void hwint_handler(struct IsrRegisters regs)
{
    if (regs.int_no >= INT_HWINT_COUNT) return;

    const unsigned char hwint_no = regs.int_no - INT_HWINT_FIRST;

    const hwint_handler_t handler = handlers[hwint_no];

    if (!handler) {
        kernaux_drivers_console_printf("[WARN] hwint: Unhandled hardware interrupt: %u\n", hwint_no);
        return;
    }

    handler();
    kernaux_drivers_intel_8259_pic_eoi(regs.int_no);
}

void hwint_register_handler(unsigned int int_no, hwint_handler_t handler)
{
    if (int_no >= INT_HWINT_COUNT) {
        return;
    }

    handlers[int_no] = handler;
    kernaux_drivers_intel_8259_pic_enable(int_no);
}

void hwint_unregister_handler(unsigned int int_no)
{
    if (int_no >= INT_HWINT_COUNT) {
        return;
    }

    kernaux_drivers_intel_8259_pic_disable(int_no);
    handlers[int_no] = 0;
}
