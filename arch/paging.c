#include "paging.h"

#include "logger.h"
#include "kmalloc.h"
#include "util.h"

#define PAGES_PER_TABLE 1024
#define TABLES_PER_DIR  1024

#define INDEX_FROM_BIT(a)  ((a) / (8 * 4))
#define OFFSET_FROM_BIT(a) ((a) % (8 * 4))

struct page {
    unsigned int present  : 1;
    unsigned int rw       : 1;
    unsigned int user     : 1;
    unsigned int accessed : 1;
    unsigned int dirty    : 1;
    unsigned int unused   : 7;
    unsigned int frame    : 20;
};

struct page_table {
    struct page pages[PAGES_PER_TABLE];
};

struct page_dir {
    struct page_table *tables[TABLES_PER_DIR];
    unsigned int tables_phys[TABLES_PER_DIR];
    unsigned int phys;
};

static struct page_dir *kernel_dir  = 0;
static struct page_dir *current_dir = 0;

static unsigned int *frames;
static unsigned int nframes;

static void switch_page_dir(struct page_dir *dir);
static struct page *get_page(unsigned int address, unsigned char make, struct page_dir *dir);

static void alloc_frame(struct page *page, unsigned char is_kernel, unsigned char is_writable);
static void free_frame(struct page *page);

// Set a bit in the frames bitset
static void set_frame(unsigned int frame_addr);

// Clear a bit in the frames bitset
static void clear_frame(unsigned int frame_addr);

// Test if a bit is set.
static unsigned int test_frame(unsigned int frame_addr);

// Find the first free frame.
static unsigned int first_frame();

void paging_initialize()
{
    logger_info("Initialize paging.");

    // The size of physical memory. For the moment we
    // assume it is 16MB big.
    unsigned int mem_end_page = 0x1000000;

    nframes = mem_end_page / 0x1000;
    frames = (unsigned int*)kmalloc(INDEX_FROM_BIT(nframes));
    memset(frames, 0, INDEX_FROM_BIT(nframes));

    // Let's make a page directory.
    kernel_dir = (struct page_dir*)kmalloc_a(sizeof(struct page_dir));
    memset(kernel_dir, 0, sizeof(struct page_dir));
    current_dir = kernel_dir;

    // We need to identity map (phys addr = virt addr) from
    // 0x0 to the end of used memory, so we can access this
    // transparently, as if paging wasn't enabled.
    // NOTE that we use a while loop here deliberately.
    // inside the loop body we actually change placement_address
    // by calling kmalloc(). A while loop causes this to be
    // computed on-the-fly rather than once at the start.
    int i = 0;
    while (i < kmalloc_placement_addr) {
        // Kernel code is readable but not writeable from userspace.
        alloc_frame(get_page(i, 1, kernel_dir), 0, 0);
        i += 0x1000;
    }

    // Now, enable paging!
    switch_page_dir(kernel_dir);
}

void switch_page_dir(struct page_dir *dir)
{
    current_dir = dir;
    asm volatile("mov %0, %%cr3":: "r"(&dir->phys));
    unsigned int cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

struct page *get_page(unsigned int address, unsigned char make, struct page_dir *dir)
{
    // Turn the address into an index.
    address /= 0x1000;

    // Find the page table containing this address.
    unsigned int table_idx = address / 1024;

    // If this table is already assigned
    if (dir->tables[table_idx]) {
        return &dir->tables[table_idx]->pages[address%1024];
    }

    if (make) {
        unsigned int tmp;
        dir->tables[table_idx] = (struct page_table*)kmalloc_ap(sizeof(struct page_table), &tmp);
        memset(dir->tables[table_idx], 0, 0x1000);
        dir->tables_phys[table_idx] = tmp | 0x7; // PRESENT, RW, US.
        return &dir->tables[table_idx]->pages[address%1024];
    }

    return 0;
}

void alloc_frame(struct page *page, unsigned char is_kernel, unsigned char is_writable)
{
    if (page->frame != 0) {
        return;
    }

    unsigned int frame = first_frame();

    if (frame == (unsigned int)-1) {
        return; // TODO: panic
    }

    set_frame(frame * 0x1000);

    page->present = 1;
    page->rw = !!is_writable;
    page->user = !is_kernel;
    page->frame = frame;
}

void free_frame(struct page *page)
{
    const unsigned int frame = page->frame;

    if (!frame) {
        return;
    }

    page->frame = 0;
    clear_frame(frame);
}

void set_frame(unsigned int frame_addr)
{
    unsigned int frame = frame_addr / 0x1000;
    unsigned int idx = INDEX_FROM_BIT(frame);
    unsigned int off = OFFSET_FROM_BIT(frame);
    frames[idx] |= 0x1 << off;
}

void clear_frame(unsigned int frame_addr)
{
    unsigned int frame = frame_addr / 0x1000;
    unsigned int idx = INDEX_FROM_BIT(frame);
    unsigned int off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}

unsigned int test_frame(unsigned int frame_addr)
{
    unsigned int frame = frame_addr / 0x1000;
    unsigned int idx = INDEX_FROM_BIT(frame);
    unsigned int off = OFFSET_FROM_BIT(frame);
    return frames[idx] & (0x1 << off);
}

unsigned int first_frame()
{
    for (unsigned int i = 0; i < INDEX_FROM_BIT(nframes); ++i) {
        // nothing free, exit early.
        if (frames[i] != 0xFFFFFFFF) {
            // at least one bit is free here.
            for (unsigned int j = 0; j < 32; ++j) {
                unsigned int to_test = 0x1 << j;
                if (!(frames[i] & to_test)) {
                    return i * 4 * 8 + j;
                }
            }
        }
    }

    return -1;
}
