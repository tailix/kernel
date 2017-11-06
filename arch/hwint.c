#include "hwint.h"

#include "interrupt.h"
#include "config.h"
#include "asm.h"
#include "logger.h"
#include "pic.h"

static const char *const messages[] = {
    "Unhandled hardware interrupt: 0",
    "Unhandled hardware interrupt: 1",
    "Unhandled hardware interrupt: 2",
    "Unhandled hardware interrupt: 3",
    "Unhandled hardware interrupt: 4",
    "Unhandled hardware interrupt: 5",
    "Unhandled hardware interrupt: 6",
    "Unhandled hardware interrupt: 7",
    "Unhandled hardware interrupt: 8",
    "Unhandled hardware interrupt: 9",
    "Unhandled hardware interrupt: 10",
    "Unhandled hardware interrupt: 11",
    "Unhandled hardware interrupt: 12",
    "Unhandled hardware interrupt: 13",
    "Unhandled hardware interrupt: 14",
    "Unhandled hardware interrupt: 15",
};

static hwint_handler_t handlers[INT_HWINT_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void hwint_handler(struct IsrRegisters regs)
{
    if (!pic_end_of_interrupt(regs.int_no)) {
        return;
    }

    const unsigned char hwint_no = regs.int_no - INT_HWINT_FIRST;

    const hwint_handler_t handler = handlers[hwint_no];

    if (!handler) {
        logger_warn_from("hwint", messages[hwint_no]);
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
