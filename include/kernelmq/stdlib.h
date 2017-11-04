#ifndef KERNELMQ_INCLUDED_STDLIB
#define KERNELMQ_INCLUDED_STDLIB 1

void kmemset(void *buffer, unsigned char value, unsigned int size);
unsigned int kstrlen(const char *s);
void kitoa(char *buf, int base, int d);

#endif
