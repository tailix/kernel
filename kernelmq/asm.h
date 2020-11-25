#ifndef KERNELMQ_INCLUDED_ASM
#define KERNELMQ_INCLUDED_ASM 1

inline static unsigned char  inportb(unsigned short port);
inline static unsigned short inportw(unsigned short port);
inline static unsigned int   inportd(unsigned short port);

inline static void outportb(unsigned short port, unsigned char  value);
inline static void outportw(unsigned short port, unsigned short value);
inline static void outportd(unsigned short port, unsigned int   value);

unsigned char inportb(unsigned short port)
{
    register unsigned char result;
    asm volatile("inb %1, %0" : "=a" (result) : "dN" (port));
    return result;
}

unsigned short inportw(unsigned short port)
{
    register unsigned short result;
    asm volatile("inw %1, %0" : "=a" (result) : "dN" (port));
    return result;
}

unsigned int inportd(unsigned short port)
{
    register unsigned int result;
    asm volatile("ind %1, %0" : "=a" (result) : "dN" (port));
    return result;
}

void outportb(unsigned short port, unsigned char value)
{
    asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

void outportw(unsigned short port, unsigned short value)
{
    asm volatile("outw %1, %0" : : "dN" (port), "a" (value));
}

void outportd(unsigned short port, unsigned int value)
{
    asm volatile("outd %1, %0" : : "dN" (port), "a" (value));
}

#endif
