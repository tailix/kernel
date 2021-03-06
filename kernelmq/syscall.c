#include "interrupt.h"

#include "syscall.h"

#include <kernaux/console.h>

static void syscall_do_exit(struct IsrRegisters regs);

void syscall_handler(const struct IsrRegisters regs)
{
    const unsigned int id = regs.eax << 16 >> 16;

    kernaux_console_printf("[INFO] syscall: number %u\n", id);

    switch (id) {
        case KERNELMQ_SYSCALL_EXIT: return syscall_do_exit(regs);
    }
}

void syscall_do_exit(const struct IsrRegisters regs)
{
    const unsigned int exit_code = regs.ebx << 16 >> 16;

    kernaux_console_printf("[WARN] syscall: process try to exit with error code %u, haha\n", exit_code);
}
