#include "hwint.h"

#include "interrupt.h"
#include "config.h"
#include "pic.h"

#include <kernaux/drivers/console.h>

static hwint_handler_t handlers[INT_HWINT_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void hwint_handler(struct IsrRegisters regs)
{
    if (!pic_end_of_interrupt(regs.int_no)) {
        return;
    }

    const unsigned char hwint_no = regs.int_no - INT_HWINT_FIRST;

    const hwint_handler_t handler = handlers[hwint_no];

    if (!handler) {
        kernaux_drivers_console_printf("[WARN] hwint: Unhandled hardware interrupt: %u\n", hwint_no);
        return;
    }

    handler();
}

void hwint_register_handler(unsigned int int_no, hwint_handler_t handler)
{
    if (int_no >= INT_HWINT_COUNT) {
        return;
    }

    handlers[int_no] = handler;
    pic_enable(int_no);
}

void hwint_unregister_handler(unsigned int int_no)
{
    if (int_no >= INT_HWINT_COUNT) {
        return;
    }

    pic_disable(int_no);
    handlers[int_no] = 0;
}
