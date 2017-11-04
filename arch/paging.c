#define PAGES_PER_TABLE 1024
#define TABLES_PER_DIR  1024

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
