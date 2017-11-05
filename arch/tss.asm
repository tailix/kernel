#include "config.h"

[GLOBAL tss_flush]

tss_flush:
    mov ax, GDT_TSS_SELECTOR
    ltr ax
    ret
