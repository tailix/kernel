#ifndef KERNELMQ_INCLUDED_KEYBOARD
#define KERNELMQ_INCLUDED_KEYBOARD 1

typedef void(*keyboard_handler_t)(char);

void keyboard_register_handler(keyboard_handler_t handler);
void keyboard_unregister_handler();

#endif
