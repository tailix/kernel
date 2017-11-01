#include "logger.h"

struct IsrRegisters {
    unsigned int ds;                                     // Data segment selector
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    unsigned int int_no, err_code;                       // Interrupt number and error code (if applicable)
    unsigned int eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically.
};

static const char *const messages[] = {
    "Unhandled interrupt: 0x0",
    "Unhandled interrupt: 0x1",
    "Unhandled interrupt: 0x2",
    "Unhandled interrupt: 0x3",
    "Unhandled interrupt: 0x4",
    "Unhandled interrupt: 0x5",
    "Unhandled interrupt: 0x6",
    "Unhandled interrupt: 0x7",
    "Unhandled interrupt: 0x8",
    "Unhandled interrupt: 0x9",
    "Unhandled interrupt: 0xA",
    "Unhandled interrupt: 0xB",
    "Unhandled interrupt: 0xC",
    "Unhandled interrupt: 0xD",
    "Unhandled interrupt: 0xE",
    "Unhandled interrupt: 0xF",
    "Unhandled interrupt: 0x10",
    "Unhandled interrupt: 0x11",
    "Unhandled interrupt: 0x12",
    "Unhandled interrupt: 0x13",
    "Unhandled interrupt: 0x14",
    "Unhandled interrupt: 0x15",
    "Unhandled interrupt: 0x16",
    "Unhandled interrupt: 0x17",
    "Unhandled interrupt: 0x18",
    "Unhandled interrupt: 0x19",
    "Unhandled interrupt: 0x1A",
    "Unhandled interrupt: 0x1B",
    "Unhandled interrupt: 0x1C",
    "Unhandled interrupt: 0x1D",
    "Unhandled interrupt: 0x1E",
    "Unhandled interrupt: 0x1F",
};

void isr(struct IsrRegisters regs)
{
    logger_warn(messages[regs.int_no]);
}
