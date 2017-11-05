#include "interrupt.h"
#include "kprintf.h"

void syscall_handler(struct IsrRegisters regs)
{
    const unsigned int id = regs.eax & 0xFFFF;

    kprintf("syscall %u\n", id);
}
