#include <i386/isr.h>
#include <i386/shell.h>
#include <i386/a20.h>
#include <i386/8042.h>
#include <i386/8253.h>
#include <i386/8259.h>
#include <i386/kheap.h>
#include <i386/gdt.h>
#include <i386/paging.h>
#include <i386/idt.h>
#include <i386/irq.h>
#include <i386/vga.h>
#include <i386/panic.h>
#include <i386/tss.h>
#include <i386/gfx.h>
#include <i386/fs.h>
#include <i386/initrd.h>
#include <multiboot.h>
#include <stdbool.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

extern uint32_t x86_kernel_start, x86_kernel_end, placement_address;
uint32_t x86_ramsize, x86_initial_esp;
bool modules_exist = false;
const char* mem_type_names[] = {"", "Available", "Reserved", "ACPI", "NVS", "Bad RAM"};

void init(unsigned long magic, multiboot_info_t *mbi, unsigned int initial_boot_stack)
{
	x86_initial_esp = initial_boot_stack;

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		__asm__ __volatile__ ("cli");
		__asm__ __volatile__ ("hlt");
		return;
	}
	
	uint32_t initrd_location = *((uint32_t*)mbi->mods_addr);
	uint32_t initrd_end = *(uint32_t*)(mbi->mods_addr+4);
	placement_address = initrd_end;

	uint32_t low_pages = 256;
    uint32_t high_pages = (mbi->mem_upper * 1024) / 4096 + 30000;
    uint32_t total_frames = high_pages + low_pages;
    multiboot_memory_map_t* mmap = mbi->mmap_addr;
    x86_ramsize = mbi->mem_upper / 1024 + 2;
    
    init_text_mode();

    while(mmap < mbi->mmap_addr + mbi->mmap_length)
    {
       	uint32_t addr = (uint32_t)mmap->addr;
		uint32_t len = (uint32_t)mmap->len;

		printk("0x%x - 0x%x | %s\n", addr, addr + len, mem_type_names[mmap->type]);
        mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(mmap->size) );
    }

	init_a20();
	init_gdt();
	init_idt();
	initialize_paging(total_frames, 0, 0);
	malloc_stats();
	pit_init();
	init_8042_keyboard();

 	if (mbi->mods_count > 0)
	{
		modules_exist = true;
		fs_root = initialise_initrd(initrd_location);
	}
	else
	{
		printk("No Modules Found!\n");
	}

	shell();

	/* XXX: Kernel can't reach this zone,
	if it does, throw a Kernel Panic Error */
	PANIC("Reached Kernel End!");
	while(1);
}