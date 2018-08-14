#ifndef PAGING_H
#define PAGING_H

#include <i386/regs.h>
#include <stdint.h>

// An actual page descriptor
typedef struct
{
    uint32_t present    : 1;   // Page present in memory
    uint32_t rw         : 1;   // Read-only if clear, readwrite if set
    uint32_t user       : 1;   // Supervisor level only if clear
    uint32_t accessed   : 1;   // Has the page been accessed since last refresh?
    uint32_t dirty      : 1;   // Has the page been written to since last refresh?
    uint32_t unused     : 7;   // Amalgamation of unused and reserved bits
    uint32_t frame      : 20;  // Frame address (shifted right 12 bits)
} page;

// A table of pages. Each table has 1024 descriptors.
typedef struct
{
    page pages[1024];
} page_table;

// A directory of page tables. Each directory has 1024 tables.
typedef struct
{
    // Array of pointers to pagetables.
    page_table *tables[1024];

    // Array of pointers to the pagetables above, but gives their *physical*
    // location, for loading into the CR3 register.
    uint32_t tablesPhysical[1024];

    // The physical address of tablesPhysical. This comes into play
    // when we get our kernel heap allocated and the directory
    // may be in a different location in virtual memory.
    uint32_t physicalAddr;
} page_directory;

/**
 * Sets up the environment, page directories etc and
 * enables paging.
 * ident_addr (if specified) causes the initializer
 * to identity-map all the pages containing
 * ident_addr through ident_addr + ident_len.
 * This is currently used to map the VESA LFB in.
 */
void initialize_paging(uint32_t total_frames, uint32_t ident_addr, uint32_t ident_len);

/**
 * Causes the specified page directory to be loaded into the
 * CR3 register.
 */
void switch_page_directory(page_directory *new);

/**
 * Retrieves a pointer to the page required.
 * If make == 1, if the page-table in which this page should
 * reside isn't created, create it!
 */
page *get_page(uint32_t address, int make, page_directory *dir);

/**
 * Calls get_page with the kernel directory.
 */
page *get_kernel_page(uint32_t address, int make);

/**
 * Maps a kernel page. Returns NULL if it was unable to map.
 */
page *map_kernel_page(uint32_t address, int make);

/**
 * Unmaps a page from the kernel and returns its frame to the pool.
 */
void unmap_kernel_page(uint32_t address);

/**
 * Handler for page faults.
 */
void x86_pagefault_handler();

#endif