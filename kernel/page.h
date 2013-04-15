#ifndef PAGE_H
#define PAGE_H

#include "common.h"
#include "isr.h"

#define PAGE_TABLE_ENTRIES      1024
#define PAGE_DIRECTORY_ENTRIES  1024

#define ENABLE_PAGING_BIT       0x80000000

/**
 * Represent a page descriptor.
 */
typedef struct _page {
    uint32_t present    : 1;    // Page present in the memory
    uint32_t rw         : 1;    // Read-only if clear, read/write if set
    uint32_t user       : 1;    // Kernel level if clear
    uint32_t accessed   : 1;    // If the page has been accessed since refresh
    uint32_t dirty      : 1;    // If the page has been written since refresh
    uint32_t unused     : 7;    // Un-used and reserved bits
    uint32_t frame      : 20;   // Frame/physical address (shifted right by 12)
} page;

/**
 * Represent a single page table.
 */
typedef struct _page_table {
    page pages[PAGE_TABLE_ENTRIES];
} page_table;

typedef struct _page_directory {
    /**
     * Array of pointers to page_tables.
     */
    page_table *page_tables[PAGE_DIRECTORY_ENTRIES];

    /**
     * Physical address of each page_table. This is the actual page_directory.
     */
    uint32_t page_table_phys_addrs[PAGE_DIRECTORY_ENTRIES];

    /**
     * Physical address of the 'page_table_phys_addrs'. When kernel heap is
     * allocated, the directy may be in a different location in the virtual
     * memory.
     */
    uint32_t page_directory_phys_addr;
} page_directory;

void initialize_paging();

void switch_page_directory(page_directory *dir);

page *get_page(uint32_t addr, int make, page_directory *dir);

#endif
