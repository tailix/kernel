#ifndef KERNELMQ_INCLUDED_KMALLOC
#define KERNELMQ_INCLUDED_KMALLOC 1

extern unsigned int kmalloc_placement_addr;

unsigned int kmalloc(unsigned int size);
unsigned int kmalloc_a(unsigned int size);
unsigned int kmalloc_p(unsigned int size, unsigned int *phys);
unsigned int kmalloc_ap(unsigned int size, unsigned int *phys);

#endif
