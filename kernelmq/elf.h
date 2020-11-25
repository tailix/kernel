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
    unsigned long  prog_table_pos : 32; // Must not be 0.
    unsigned long  sect_table_pos : 32;
    unsigned long  arch_flags     : 32; // Must be 0.
    unsigned short header_size    : 16; // Must be 52.
    unsigned short prog_entr_size : 16; // Must not be 0.
    unsigned short prog_entr_num  : 16; // Must not be 0.
    unsigned short sect_entr_size : 16;
    unsigned short sect_entr_num  : 16;
    unsigned short sect_names_idx : 16;
}
__attribute__((packed));

struct KernelMQ_ELF_ProgramEntry {
    unsigned long type       : 32;
    unsigned long offset     : 32;
    unsigned long virt_addr  : 32;
    unsigned long phys_addr  : 32;
    unsigned long file_size  : 32;
    unsigned long mem_size   : 32;
    unsigned long flags      : 32;
    unsigned long align      : 32;
}
__attribute__((packed));

struct KernelMQ_ELF_SectionEntry {
    unsigned long name        : 32;
    unsigned long type        : 32;
    unsigned long flags       : 32;
    unsigned long vaddr       : 32;
    unsigned long file_offset : 32;
    unsigned long file_size   : 32;
    unsigned long link        : 32;
    unsigned long info        : 32;
    unsigned long alignment   : 32;
    unsigned long ent_size    : 32;
}
__attribute__((packed));

typedef struct KernelMQ_ELF_ProgramEntry KernelMQ_ELF_ProgramTable[];

typedef struct KernelMQ_ELF_SectionEntry KernelMQ_ELF_SectionTable[];

inline static unsigned char KernelMQ_ELF_Header_is_valid(
    const struct KernelMQ_ELF_Header *header
);

unsigned char KernelMQ_ELF_Header_is_valid(
    const struct KernelMQ_ELF_Header *const header
) {
    return (
        header->magic_0x7F     == 0x7F &&
        header->magic_E        == 'E'  &&
        header->magic_L        == 'L'  &&
        header->magic_F        == 'F'  &&
        header->bitness        == 1    &&
        header->endianness     == 1    &&
        header->header_version == 1    &&
        header->os_abi         == 0    &&
        header->obj_type       == 2    &&
        header->isa            == 3    &&
        header->elf_version    == 1    &&
        header->arch_flags     == 0    &&
        header->header_size    == 52   &&
        header->prog_table_pos != 0    &&
        header->prog_entr_size != 0    &&
        header->prog_entr_num  != 0
    );
}

#ifdef __cplusplus
}
#endif

#endif
