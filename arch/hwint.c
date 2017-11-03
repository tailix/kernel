#include "logger.h"

struct IsrRegisters {
    unsigned int ds;                                     // Data segment selector
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    unsigned int int_no, err_code;                       // Interrupt number and error code (if applicable)
    unsigned int eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically.
};

static const char *const messages[] = {
    "Unhandled hwint: 0",
    "Unhandled hwint: 1",
    "Unhandled hwint: 2",
    "Unhandled hwint: 3",
    "Unhandled hwint: 4",
    "Unhandled hwint: 5",
    "Unhandled hwint: 6",
    "Unhandled hwint: 7",
    "Unhandled hwint: 8",
    "Unhandled hwint: 9",
    "Unhandled hwint: 10",
    "Unhandled hwint: 11",
    "Unhandled hwint: 12",
    "Unhandled hwint: 13",
    "Unhandled hwint: 14",
    "Unhandled hwint: 15",
};

void hwint_handler(struct IsrRegisters regs)
{
    logger_warn(messages[regs.int_no - 32]);
}
