#ifndef KERNELMQ_INCLUDED_ASM
#define KERNELMQ_INCLUDED_ASM 1

inline static unsigned char  inb(unsigned short port);
inline static unsigned short inw(unsigned short port);
inline static unsigned int   ind(unsigned short port);

inline static void outb(unsigned short port, unsigned char  value);
inline static void outw(unsigned short port, unsigned short value);
inline static void outd(unsigned short port, unsigned int   value);

unsigned char inb(unsigned short port)
{
    register unsigned char result;
    asm volatile("inb %1, %0" : "=a" (result) : "dN" (port));
    return result;
}

unsigned short inw(unsigned short port)
{
    register unsigned short result;
    asm volatile("inw %1, %0" : "=a" (result) : "dN" (port));
    return result;
}

unsigned int ind(unsigned short port)
{
    register unsigned int result;
    asm volatile("ind %1, %0" : "=a" (result) : "dN" (port));
    return result;
}

void outb(unsigned short port, unsigned char value)
{
    asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

void outw(unsigned short port, unsigned short value)
{
    asm volatile("outw %1, %0" : : "dN" (port), "a" (value));
}

void outd(unsigned short port, unsigned int value)
{
    asm volatile("outd %1, %0" : : "dN" (port), "a" (value));
}

#endif
