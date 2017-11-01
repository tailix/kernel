#ifndef TAILIX_KERNEL_INCLUDED_UTIL
#define TAILIX_KERNEL_INCLUDED_UTIL 1

#include <stdint.h>
#include <stddef.h>

static inline size_t strlen(const char *s);

size_t strlen(const char *const s)
{
    size_t len = 0;

    while (s[len]) {
        len++;
    }

    return len;
}

#endif
