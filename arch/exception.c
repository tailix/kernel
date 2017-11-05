#include "interrupt.h"
#include "config.h"
#include "logger.h"

static const char *const messages[] = {
    "Unhandled protected-mode exception: 0x0",
    "Unhandled protected-mode exception: 0x1",
    "Unhandled protected-mode exception: 0x2",
    "Unhandled protected-mode exception: 0x3",
    "Unhandled protected-mode exception: 0x4",
    "Unhandled protected-mode exception: 0x5",
    "Unhandled protected-mode exception: 0x6",
    "Unhandled protected-mode exception: 0x7",
    "Unhandled protected-mode exception: 0x8",
    "Unhandled protected-mode exception: 0x9",
    "Unhandled protected-mode exception: 0xA",
    "Unhandled protected-mode exception: 0xB",
    "Unhandled protected-mode exception: 0xC",
    "Unhandled protected-mode exception: 0xD",
    "Unhandled protected-mode exception: 0xE",
    "Unhandled protected-mode exception: 0xF",
    "Unhandled protected-mode exception: 0x10",
    "Unhandled protected-mode exception: 0x11",
    "Unhandled protected-mode exception: 0x12",
    "Unhandled protected-mode exception: 0x13",
    "Unhandled protected-mode exception: 0x14",
    "Unhandled protected-mode exception: 0x15",
    "Unhandled protected-mode exception: 0x16",
    "Unhandled protected-mode exception: 0x17",
    "Unhandled protected-mode exception: 0x18",
    "Unhandled protected-mode exception: 0x19",
    "Unhandled protected-mode exception: 0x1A",
    "Unhandled protected-mode exception: 0x1B",
    "Unhandled protected-mode exception: 0x1C",
    "Unhandled protected-mode exception: 0x1D",
    "Unhandled protected-mode exception: 0x1E",
    "Unhandled protected-mode exception: 0x1F",
};

void exception_handler(struct IsrRegisters regs)
{
    if (
        !(/* regs.int_no >= INT_EXCEPTION_FIRST && */ // unsigned is always >= 0
          regs.int_no <= INT_EXCEPTION_LAST)
    ) {
        return;
    }

    logger_warn(messages[regs.int_no]);
}
