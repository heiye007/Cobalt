#include <i386/paging.h>
#include <i386/kframe.h>
#include <i386/kmalloc_early.h>
#include <i386/kheap.h>
#include <i386/panic.h>
#include <i386/vga.h>
#include <string.h>
#include <stdint.h>
#include <types.h>

page_directory *kernel_directory;
page_directory *current_directory;
extern uint32_t x86_placement_address;

uint8_t initialized = 0;

extern void x86_register_interrupt_handler(int irq, void (*handler));

void identity_map(uint32_t address, uint32_t length)
{
    uint32_t curr = address & 0xFFFFF000;

    while(curr < address + length)
    {
        page *p = get_page(curr, 1, kernel_directory);
        p->present = 1;
        p->rw = 1;
        p->user = 0;
        p->frame = curr >> 12;
        curr += PAGE_SIZE;
    }
}

static page_table *clone_table(page_table *src, unsigned int *physAddr)
{
    // Make a new page table, which is page aligned.
    page_table *table = (page_table*)kmalloc_ap(sizeof(page_table), 1, physAddr);
    // Ensure that the new table is blank.
    memset(table, 0, sizeof(page_directory));

    // For every entry in the table...
    int i;
    for (i = 0; i < 1024; i++)
    {
        // If the source entry has a frame associated with it...
        if (src->pages[i].frame)
        {
            // Get a new frame.
            alloc_frame(&table->pages[i], 0, 0);
            // Clone the flags from source to destination.
            if (src->pages[i].present) table->pages[i].present = 1;
            if (src->pages[i].rw) table->pages[i].rw = 1;
            if (src->pages[i].user) table->pages[i].user = 1;
            if (src->pages[i].accessed) table->pages[i].accessed = 1;
            if (src->pages[i].dirty) table->pages[i].dirty = 1;
            // Physically copy the data across. This function is in process.s.
            copy_page_physical(src->pages[i].frame*PAGE_SIZE, table->pages[i].frame*PAGE_SIZE);
        }
    }
    return table;
}

page_directory *clone_directory(page_directory *src)
{
    unsigned int phys;
    // Make a new page directory and obtain its physical address.
    page_directory *dir = (page_directory*)kmalloc_ap(sizeof(page_directory), 1, &phys);
    // Ensure that it is blank.
    memset(dir, 0, sizeof(page_directory));

    // Get the offset of tablesPhysical from the start of the page_directory structure.
    unsigned int offset = (unsigned int)dir->tablesPhysical - (unsigned int)dir;

    // Then the physical address of dir->tablesPhysical is:
    dir->physicalAddr = phys + offset;

    // Go through each page table. If the page table is in the kernel directory, do not make a new copy.
    int i;
    for (i = 0; i < 1024; i++)
    {
        if (!src->tables[i])
            continue;

        if (kernel_directory->tables[i] == src->tables[i])
        {
            // It's in the kernel, so just use the same pointer.
            dir->tables[i] = src->tables[i];
            dir->tablesPhysical[i] = src->tablesPhysical[i];
        }
        else
        {
            // Copy the table.
            unsigned int phys;
            dir->tables[i] = clone_table(src->tables[i], &phys);
            dir->tablesPhysical[i] = phys | 0x07;
        }
    }
    return dir;
}

void initialize_paging(uint32_t total_frames, uint32_t ident_addr, uint32_t ident_len)
{
    init_frame_allocator(total_frames);

    // Make a page directory for the kernel.
    kernel_directory = (page_directory *)e_kmalloc_a(sizeof (page_directory));
    memset(kernel_directory, 0, sizeof (page_directory));
    current_directory = kernel_directory;

    // Go ahead and allocate all the page tables for the kernel.
    // This is wasteful, but a lot easier than figuring out how to build
    // a kernel page allocator.
#ifdef DBG_PAGING
    printk("[PAGING] Allocating kernel page tables...\n");
#endif
    uint32_t i = 0;

    for(i = 0; i < 0xFFFFFFFF;)
    {
        get_page(i, 1, kernel_directory);
        i += PAGE_SIZE * 1024;

        if(i == 0)
        {
            break;
        }
    }

#ifdef DBG_PAGING
    printk("[PAGING] Allocated kernel page tables\n");
#endif

    // We need to identity map (phys addr = virt addr) from
    // 0x0 to the end of used memory, so we can access this
    // transparently, as if paging wasn't enabled.
    // NOTE that we use a while loop here deliberately.
    // inside the loop body we actually change x86_placement_address
    // by calling kmalloc(). A while loop causes this to be
    // computed on-the-fly rather than once at the start.

    // This is hacky. Probably want to do this some other way.
    // Reaching into kmalloc_early and grabbing x86_placement_address
    // is not ideal.
    i = 0;

    while(i < x86_placement_address)
    {
        // Kernel code is readable but not writeable from userspace.
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
        i += PAGE_SIZE;
    }

    // Before we do anything else, disable the original kmalloc so we don't
    // start leaking into the address space.
    uint32_t heap_start = disable_early_kmalloc();

#ifdef DBG_PAGING
    printk("  KHEAP_START: %x\n", heap_start);
#endif

    // bootstrap the kheap with INITIAL_HEAP_PAGE_COUNT pages.
    for(i = 0; i < INITIAL_HEAP_PAGE_COUNT; i++)
    {
        alloc_frame(get_page(heap_start + (i * PAGE_SIZE), 1, kernel_directory), 0, 0);
    }

    // Before we enable paging, we must register our page fault handler.
    x86_register_interrupt_handler(14, x86_pagefault_handler);

    if(ident_addr)
    {
        identity_map(ident_addr, ident_len);
    }

    // Now, enable paging!
    switch_page_directory(kernel_directory);
    initialized = 1;

#ifdef DBG_PAGING
    printk("[PAGING] Initializing kernel heap...\n");
#endif

    // Set up the kernel heap!
    initialize_kheap(heap_start);

#ifdef DBG_PAGING
    printk("[PAGING] Kernel heap initialized\n");
#endif

    current_directory = clone_directory(kernel_directory);
    switch_page_directory(current_directory);
    printkok("Initialized Paging");
}

void switch_page_directory(page_directory *dir)
{
    current_directory = dir;
    __asm__ __volatile__("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
    uint32_t cr0;
    __asm__ __volatile__("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    __asm__ __volatile__("mov %0, %%cr0":: "r"(cr0));
}


page *get_page(uint32_t address, int make, page_directory *dir)
{
    // Turn the address into an index.
    address /= PAGE_SIZE;
    // Find the page table containing this address.
    uint32_t table_idx = address / 1024;

    if (dir->tables[table_idx]) // If this table is already assigned
    {
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else if(make)
    {
        uint32_t tmp;

        if(!initialized)
        {
            dir->tables[table_idx] = (page_table *)e_kmalloc_ap(sizeof(page_table), &tmp);
        }
        else
        {
            dir->tables[table_idx] = (page_table *)kmalloc_ap(sizeof(page_table), 1, &tmp);
        }

        memset(dir->tables[table_idx], 0, PAGE_SIZE);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
        return &dir->tables[table_idx]->pages[address%1024];
    }
    else
    {
        return NULL;
    }
}

page *get_kernel_page(uint32_t address, int make)
{
    return get_page(address, make, kernel_directory);
}

page *map_kernel_page(uint32_t address, int make)
{
    page *p = get_page(address, make, kernel_directory);

    if(!p) return NULL;

    alloc_frame(p, 0, 0);
    return p;
}

void unmap_kernel_page(uint32_t address)
{
    page *p = get_page(address, 0, kernel_directory);

    if(p)
    {
        free_frame(p);
    }
}

void x86_pagefault_handler(struct regs* r)
{
    disable_cursor();
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    __asm__ __volatile__("mov %%cr2, %0" : "=r" (faulting_address));

    // The error code gives us details of what happened.
    int present = !(r->err_code & 0x1); // Page not present
    int rw = r->err_code & 0x2;         // Write operation?
    int us = r->err_code & 0x4;         // Processor was in user-mode?
    int reserved = r->err_code & 0x8;   // Overwritten CPU-reserved bits of page entry?
    //int id = regs.err_code & 0x10;        // Caused by an instruction fetch?

    // Output an error message.
    printk("Page fault! ( ");
    if (present) {printk("present ");}
    if (rw) {printk("read-only ");}
    if (us) {printk("user-mode ");}
    if (reserved) {printk("reserved ");}
    printk(") at %x\n", faulting_address);
    __asm__ __volatile__ ("cli");
    __asm__ __volatile__ ("hlt");
    for (;;);
}