#include "config.h"
#include "asm.h"
#include "logger.h"

struct IsrRegisters {
    unsigned int ds;                                     // Data segment selector
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    unsigned int int_no, err_code;                       // Interrupt number and error code (if applicable)
    unsigned int eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically.
};

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
        outb(0xA0, 0x20);
    }

    // Send reset signal to master
    outb(0x20, 0x20);

    const unsigned char hwint_no = regs.int_no - INT_HWINT_FIRST;

    logger_warn(messages[hwint_no]);
}
