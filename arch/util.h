#ifndef KERNELMQ_INCLUDED_UTIL
#define KERNELMQ_INCLUDED_UTIL 1

static inline unsigned int strlen(const char *s);
static inline void itoa(char *buf, int base, int d);

unsigned int strlen(const char *const s)
{
    unsigned int result = 0;

    while (s[result]) {
        ++result;
    }

    return result;
}

void itoa(char *buf, int base, int d)
{
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;

    // If %d is specified and D is minus, put '-' in the head.
    if (base == 'd' && d < 0) {
        *p++ = '-';
        buf++;
        ud = -d;
    }
    else if (base == 'x') {
        divisor = 16;
    }

    // Divide UD by DIVISOR until UD == 0.
    do {
        int remainder = ud % divisor;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
    }
    while (ud /= divisor);

    // Terminate BUF.
    *p = 0;

    // Reverse BUF.
    p1 = buf;
    p2 = p - 1;

    while (p1 < p2) {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

#endif
