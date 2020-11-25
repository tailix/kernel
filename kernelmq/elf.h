#ifndef KERNELMQ_INCLUDED_ELF
#define KERNELMQ_INCLUDED_ELF 1

#ifdef __cplusplus
extern "C" {
#endif

struct KernelMQ_ELF_Header {
    unsigned char  magic_0x7F     : 8; // Must be 0x7F.
    unsigned char  magic_E        : 8; // Must be 'E'.
    unsigned char  magic_L        : 8; // Must be 'L'.
    unsigned char  magic_F        : 8; // Must be 'F'.
    unsigned char  bitness        : 8; // Must be 1 (32 bit).
    unsigned char  endianness     : 8; // Must be 1 (little endian).
    unsigned char  header_version : 8; // Must be 1.
    unsigned char  os_abi         : 8; // Must be 0 (System V).
    unsigned long  unused1        : 32;
    unsigned long  unused2        : 32;
    unsigned short obj_type       : 16; // Must be 2 (executable).
    unsigned short isa            : 16; // Must be 3 (x86).
    unsigned long  elf_version    : 32; // Must be 1.
    unsigned long  entrypoint     : 32;
    unsigned long  prog_table_pos : 32;
    unsigned long  sect_table_pos : 32;
    unsigned long  arch_flags     : 32; // Must be 0.
    unsigned short header_size    : 16; // Must be 52.
    unsigned short prog_entr_size : 16;
    unsigned short prog_entr_num  : 16;
    unsigned short sect_entr_size : 16;
    unsigned short sect_entr_num  : 16;
    unsigned short sect_names_idx : 16;
}
__attribute__((packed));

#ifdef __cplusplus
}
#endif

#endif
