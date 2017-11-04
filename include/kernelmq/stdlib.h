#ifndef KERNELMQ_INCLUDED_STDLIB
#define KERNELMQ_INCLUDED_STDLIB 1

#ifdef __cplusplus
extern "C" {
#endif

void kmemset(void *buffer, unsigned char value, unsigned long size);

unsigned int kstrlen(const char *s);

char *kstrncpy(char *dest, const char *src, unsigned long length);

void kitoa(char *buf, int base, int d);

#ifdef __cplusplus
}
#endif

#endif
