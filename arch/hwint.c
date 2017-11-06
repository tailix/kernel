#include "hwint.h"

#include "interrupt.h"
#include "config.h"
#include "asm.h"
#include "logger.h"

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
    if (
        !(regs.int_no >= INT_HWINT_FIRST &&
          regs.int_no <= INT_HWINT_LAST)
    ) {
        return;
    }

    // Send an EOI (end of interrupt) signal to the PICs

    if (regs.int_no >= 40) { // TODO: hardcoded
        // Send reset signal to slave
        outportb(0xA0, 0x20); // TODO: hardcoded
    }

    // Send reset signal to master
    outportb(0x20, 0x20); // TODO: hardcoded

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
}
