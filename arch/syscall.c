#include "interrupt.h"
#include "kprintf.h"

void syscall_handler(struct IsrRegisters regs)
{
    const unsigned int id = regs.eax;

    kprintf("syscall %u\n", id);
}
