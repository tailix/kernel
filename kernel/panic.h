#ifndef KERNEL_INCLUDED_PANIC
#define KERNEL_INCLUDED_PANIC 1

void panic(const char *s);

#define assert(cond, s) { if (!(cond)) { panic(s); } }

#endif
