#ifndef KERNELMQ_INCLUDED_STDLIB
#define KERNELMQ_INCLUDED_STDLIB 1

#ifdef __cplusplus
extern "C" {
#endif

void kmemset(void *buffer, unsigned char value, unsigned int size);
unsigned int kstrlen(const char *s);
void kitoa(char *buf, int base, int d);

#ifdef __cplusplus
}
#endif

#endif
