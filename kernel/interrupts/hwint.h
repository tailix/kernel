#ifndef KERNEL_INCLUDED_HWINT
#define KERNEL_INCLUDED_HWINT 1

typedef void(*hwint_handler_t)();

void hwint_register_handler(unsigned int int_no, hwint_handler_t handler);
void hwint_unregister_handler(unsigned int int_no);

#endif
