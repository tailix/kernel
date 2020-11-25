#include "interrupt.h"
#include "logger.h"

#include "syscall.h"

static void syscall_do_exit(struct IsrRegisters regs);

void syscall_handler(const struct IsrRegisters regs)
{
    const unsigned int id = regs.eax << 16 >> 16;

    logger_info_from("syscall", "number %u", id);

    switch (id) {
        case KERNELMQ_SYSCALL_EXIT: return syscall_do_exit(regs);
    }
}

void syscall_do_exit(const struct IsrRegisters regs)
{
    const unsigned int exit_code = regs.ebx << 16 >> 16;

    logger_warn_from("syscall", "process try to exit with error code %u, haha", exit_code);
}
