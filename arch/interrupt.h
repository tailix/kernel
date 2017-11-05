#ifndef KERNELMQ_INCLUDED_INTERRUPT
#define KERNELMQ_INCLUDED_INTERRUPT 1

struct IsrRegisters {
    unsigned int ds;                                     // Data segment selector
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    unsigned int int_no, err_code;                       // Interrupt number and error code (if applicable)
    unsigned int eip, cs, eflags, useresp, ss;           // Pushed by the processor automatically.
};

#endif
