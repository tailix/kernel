#include "kprintf.h"

struct IsrRegisters {
    unsigned int ds;                                     // Data segment selector
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    unsigned int eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically.
};

void syscall_handler(struct IsrRegisters regs)
{
    const unsigned int id = regs.eax;

    kprintf("syscall %u\n", id);
}
