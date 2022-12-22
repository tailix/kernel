#ifndef KERNEL_INCLUDED_PANIC
#define KERNEL_INCLUDED_PANIC 1

#define assert(cond, s) { if (!(cond)) { panic(s); } }

void panic(const char *s);
void kernaux_assert_fn(const char *file, int line, const char *str);

#endif
