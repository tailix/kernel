#include "interrupt.h"
#include "config.h"
#include "panic.h"

#include <kernaux/drivers/console.h>

static const char *const messages[] = {
    "0  #DE - Divide Error Exception",
    "1  #DB - Debug Exception",
    "2  NMI - Non-maskable interrupt",
    "3  #BP - Breakpoint Exception",
    "4  #OF - Overflow Exception",
    "5  #BR - BOUND Range Exceeded Exception",
    "6  #UD - Invalid Opcode Exception",
    "7  #NM - Device Not Available Exception",
    "8  #DF - Double Fault Exception",
    "9  Reserved - Coprocessor Segment Overrun",
    "10 #TS - Invalid TSS Exception",
    "11 #NP - Segment Not Present",
    "12 #SS - Stack Fault Exception",
    "13 #GP - General Protection Exception",
    "14 #PF - Page-Fault Exception",
    "15 Reserved",
    "16 #MF - x87 FPU Floating-Point Error",
    "17 #AC - Alignment Check Exception",
    "18 #MC - Machine-Check Exception",
    "19 #XF - SIMD Floating-Point Exception",
    "20 Reserved",
    "21 Reserved",
    "22 Reserved",
    "23 Reserved",
    "24 Reserved",
    "25 Reserved",
    "26 Reserved",
    "27 Reserved",
    "28 Reserved",
    "29 Reserved",
    "30 Reserved",
    "31 Reserved"
};

void exception_handler(struct IsrRegisters regs)
{
    if (regs.int_no > INT_EXCEPTION_LAST) {
        return;
    }

    kernaux_drivers_console_printf("[FAIL] exception: Unhandled protected-mode exception: %s\n", messages[regs.int_no]);

    panic("Can not continue.");
}
