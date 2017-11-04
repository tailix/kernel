#include "memory.h"

#include "kprintf.h"

// Defined in linker script
extern char _memory_offset;
extern char _memory_phys_base;
extern char _memory_virt_base;

const unsigned int memory_offset = (unsigned int)&_memory_offset;
const unsigned int memory_phys_base = (unsigned int)&_memory_phys_base;
const unsigned int memory_virt_base = (unsigned int)&_memory_virt_base;

void print_memory_info()
{
    kprintf("0x%x + 0x%x = 0x%x\n", memory_phys_base, memory_offset, memory_virt_base);
}
