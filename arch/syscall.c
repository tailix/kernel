#include "interrupt.h"
#include "logger.h"

#include <kernelmq/syscall.h>

static void syscall_do_exit(struct IsrRegisters regs);

void syscall_handler(const struct IsrRegisters regs)
{
    const unsigned int id = regs.eax & 0xFFFF;

    switch (id) {
        case KERNELMQ_SYSCALL_EXIT: return syscall_do_exit(regs);
    }

    logger_info("syscall %u", id);
}

void syscall_do_exit(const struct IsrRegisters regs)
{
    logger_warn("process try to exit with error code %u, haha", regs.ebx & 0xFFFF);
}
