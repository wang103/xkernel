#include "page.h"

/**
 * Enable paging.
 */
void initialize_paging() {

}

/**
 * Load a page directory into the CR3 register.
 */
void switch_page_directory(page_directory *dir) {

}

/**
 * Return pointer to a page. If 'make' is 1, and the page_table for this page
 * doesn't exist, create the page_table.
 */
page *get_page(uint32_t addr, int make, page_directory *dir) {
    return NULL;
}

/**
 * Handle the page fault exception.
 */
void page_fault_hander(registers regs) {

}
