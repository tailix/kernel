#ifndef KERNELMQ_INCLUDED_HWINT
#define KERNELMQ_INCLUDED_HWINT 1

typedef void(*hwint_handler_t)();

void hwint_register_handler(unsigned int int_no, hwint_handler_t handler);

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

#endif
