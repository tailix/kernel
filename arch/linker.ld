OUTPUT_ARCH("i386")
ENTRY(_start)

_memory_offset = 0xC0000000; /* 3 GB */

_memory_phys_base = 4M;
_memory_virt_base = (_memory_phys_base + _memory_offset);

SECTIONS
{
    . = 1M;

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}
