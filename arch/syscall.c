#include "interrupt.h"
#include "kprintf.h"

#include <kernelmq/syscall.h>

static void syscall_do_exit(struct IsrRegisters regs);

void syscall_handler(const struct IsrRegisters regs)
{
    const unsigned int id = regs.eax & 0xFFFF;

    switch (id) {
        case KERNELMQ_SYSCALL_EXIT: return syscall_do_exit(regs);
    }

    kprintf("syscall %u\n", id);
}

void syscall_do_exit(const struct IsrRegisters regs)
{
    kprintf("process try to exit with error code %u, haha\n", regs.ebx & 0xFFFF);
}
