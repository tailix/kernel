#include <kernaux/free_list.h>
#include <kernaux/generic/malloc.h>

#include <stddef.h>

static char memory[8192];
static struct KernAux_FreeList free_list;

const KernAux_Malloc heap_malloc = &free_list.malloc;

void heap_initialize()
{
    KernAux_FreeList_init(&free_list, NULL);
    KernAux_FreeList_add_zone(&free_list, memory, sizeof(memory));
}
