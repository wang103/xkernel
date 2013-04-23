#include "page.h"
#include "mm.h"
#include "monitor.h"

extern uint32_t placement_address;

// Kernel's page directory.
page_directory *kernel_directory = NULL;

// Current page directory.
page_directory *current_directory = NULL;

/**
 * Handle the page fault exception.
 */
static void page_fault_handler(registers regs) {
    // Get the faulting address, which is stored in CR2 register.
    uint32_t faulting_addr;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_addr));

    int not_present = !(regs.err_code & 0x1);   // Page not present
    int write       = regs.err_code & 0x2;      // Write operation
    int user        = regs.err_code & 0x4;      // User mode
    int reserved    = regs.err_code & 0x8;      // Reserved bits overwritten
    int id          = regs.err_code & 0x10;     // Caused by instruction fetch

    // Output an error message.
    monitor_put("Page fault at 0x");
    monitor_puthex(faulting_addr);
    monitor_putchar('\n');

    if (not_present) {
        monitor_put("Page is not present.\n");
    } else {
        monitor_put("Page is present.\n");
    }
    if (write) {
        monitor_put("Caused by write operation.\n");
    } else {
        monitor_put("Caused by read operation.\n");
    }
    if (user) {
        monitor_put("Processor was in user-mode.\n");
    } else {
        monitor_put("Processor was in kernel-mode.\n");
    }
    if (reserved) {
        monitor_put("Reserve bits were overwritten.\n");
    } else {
        monitor_put("Reserve bits were not overwritten.\n");
    }
    if (id) {
        monitor_put("Occured during an instruction fetch.\n");
    } else {
        monitor_put("Occured not during an instruction fetch.\n");
    }
    PANIC("Page fault");
}

/**
 * Enable paging. After this function is called, all memory access will be
 * virtual memory.
 */
void initialize_paging() {
    uint32_t phys_addr;

    // First make a page directory for the kernel.
    kernel_directory = (page_directory *)kmalloc_early(sizeof(page_directory),
            1, &phys_addr);
    memset((uint8_t *)kernel_directory, 0, sizeof(page_directory));
    kernel_directory->page_directory_phys_addr = phys_addr;
    current_directory = kernel_directory;

    // Identity mapping! This is needed so kernel can continue to run after
    // paging is enabled.
    // We need the mapping between frames already used by kmalloc and pages.
    // Note that placement_address in changing during the loop.
    uint32_t i = 0;
    while (i < placement_address) {
        page *p = get_page(i, 1, kernel_directory);
        alloc_frame(p, 1, 1);

        // Next page/frame.
        i += MM_4K;
    }

    // Register the page fault handler.
    register_interrupt_handler(14, page_fault_handler);
    
    // Enable paging!
    switch_page_directory(kernel_directory);
}

/**
 * Load a page directory into the CR3 register.
 */
void switch_page_directory(page_directory *dir) {
    // Specify the page directory.
    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(&dir->page_table_phys_addrs));

    uint32_t cr0;
    // Enable paging.
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= ENABLE_PAGING_BIT;
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

/**
 * Return pointer to a page that contains the virtual address. If 'make' is 1,
 * and the page_table for this page doesn't exist, create the page_table.
 */
page *get_page(uint32_t addr, int make, page_directory *dir) {
    uint32_t index = addr / MM_4K;

    // Find the page table that contains this index.
    uint32_t table_index = index / PAGE_TABLE_ENTRIES;
    uint32_t page_index = index - table_index * PAGE_TABLE_ENTRIES;

    if (dir->page_tables[table_index]) {
        page_table *table = dir->page_tables[table_index];
        return &(table->pages[page_index]);
    }
    else if (make) {
        uint32_t phys_addr;
        dir->page_tables[table_index] = (page_table *)kmalloc_early(
                sizeof(page_table), 1, &phys_addr);

        // Initialize the table.
        memset((uint8_t *)(dir->page_tables[table_index]), 0,
                sizeof(page_table));

        // Set the directory entry to be kernel-only, writable, and present.
        dir->page_table_phys_addrs[table_index] = phys_addr | 0x3;

        return &(dir->page_tables[table_index]->pages[page_index]);
    }

    return NULL;
}
