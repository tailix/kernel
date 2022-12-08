#include "main.h"

#include <stddef.h>

#include <drivers/intel_8259_pic.h>

static hwint_handler_t handlers[INT_HWINT_COUNT] = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

void hwint_handler(struct IsrRegisters regs)
{
    if (regs.int_no < INT_HWINT_FIRST || regs.int_no > INT_HWINT_LAST) return;
    const unsigned int hwint_no = regs.int_no - INT_HWINT_FIRST;

    const hwint_handler_t handler = handlers[hwint_no];
    if (handler) handler();

    drivers_intel_8259_pic_eoi(regs.int_no);
}

void hwint_register_handler(unsigned int hwint_no, hwint_handler_t handler)
{
    if (hwint_no >= INT_HWINT_COUNT) return;

    handlers[hwint_no] = handler;
    drivers_intel_8259_pic_enable(hwint_no);
}

void hwint_unregister_handler(unsigned int hwint_no)
{
    if (hwint_no >= INT_HWINT_COUNT) return;

    drivers_intel_8259_pic_disable(hwint_no);
    handlers[hwint_no] = NULL;
}
