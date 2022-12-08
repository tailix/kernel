#ifndef KERNEL_INCLUDED_INTERRUPTS_MAIN
#define KERNEL_INCLUDED_INTERRUPTS_MAIN 1

#include "config.h"

struct IsrRegisters {
    unsigned int ds;                                     // Data segment selector
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    unsigned int int_no, err_code;                       // Interrupt number and error code (if applicable)
    unsigned int ip, cs, flags, sp, ss;                  // Pushed by the processor automatically.
};

// Protected mode exteptions
void interrupt_0();
void interrupt_1();
void interrupt_2();
void interrupt_3();
void interrupt_4();
void interrupt_5();
void interrupt_6();
void interrupt_7();
void interrupt_8();
void interrupt_9();
void interrupt_10();
void interrupt_11();
void interrupt_12();
void interrupt_13();
void interrupt_14();
void interrupt_15();
void interrupt_16();
void interrupt_17();
void interrupt_18();
void interrupt_19();
void interrupt_20();
void interrupt_21();
void interrupt_22();
void interrupt_23();
void interrupt_24();
void interrupt_25();
void interrupt_26();
void interrupt_27();
void interrupt_28();
void interrupt_29();
void interrupt_30();
void interrupt_31();

// Hardware IRQs
void interrupt_32();
void interrupt_33();
void interrupt_34();
void interrupt_35();
void interrupt_36();
void interrupt_37();
void interrupt_38();
void interrupt_39();
void interrupt_40();
void interrupt_41();
void interrupt_42();
void interrupt_43();
void interrupt_44();
void interrupt_45();
void interrupt_46();
void interrupt_47();

// Syscalls
void interrupt_0x80();



/*********
 * hwint *
 *********/

typedef void(*hwint_handler_t)();

void hwint_register_handler(unsigned int hwint_no, hwint_handler_t handler);
void hwint_unregister_handler(unsigned int hwint_no);



/***********
 * syscall *
 ***********/

enum Kernel_Syscall_Number {
    KERNEL_SYSCALL_EXIT = 0,
};

#endif
