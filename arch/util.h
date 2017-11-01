#ifndef TAILIX_KERNEL_INCLUDED_UTIL
#define TAILIX_KERNEL_INCLUDED_UTIL 1

static inline unsigned int strlen(const char *s);

unsigned int strlen(const char *const s)
{
    unsigned int result = 0;

    while (s[result]) {
        ++result;
    }

    return result;
}

#endif
