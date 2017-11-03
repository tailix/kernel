#ifndef KERNELMQ_INCLUDED_HWINT
#define KERNELMQ_INCLUDED_HWINT 1

typedef void(*hwint_handler_t)();

void hwint_register_handler(unsigned int int_no, hwint_handler_t handler);

void hwint_0();
void hwint_1();
void hwint_2();
void hwint_3();
void hwint_4();
void hwint_5();
void hwint_6();
void hwint_7();
void hwint_8();
void hwint_9();
void hwint_10();
void hwint_11();
void hwint_12();
void hwint_13();
void hwint_14();
void hwint_15();

#endif
