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
#include <multiboot.h>
#include <stdbool.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

extern uint32_t kernel_end;
extern uint32_t kernel_start;
unsigned int initial_esp;

void init(unsigned long magic, multiboot_info_t *mbi, unsigned int initial_stack)
{
	initial_esp = initial_stack;

	init_text_mode();

	printk("initial_esp : 0x%x\n", initial_esp);
	init_a20();

#ifdef DBG_CPU
	getCPUVendor();
	getCPUFeatures();
	getCPUArch();
	getCPUName();
#endif

	uint32_t low_pages = 256;
    uint32_t high_pages = (mbi->mem_upper * 1024) / 4096 + 30000;

    uint32_t total_frames = high_pages + low_pages;

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		printk("Invalid magic number: 0x%x\n", (unsigned) magic);
		__asm__ __volatile__ ("cli");
		__asm__ __volatile__ ("hlt");
		return;
	}

#ifdef DBG_INIT
	printk("Kernel base is 0x%x, end is 0x%x\n", &kernel_start, &kernel_end);
#endif

    multiboot_memory_map_t* mmap = mbi->mmap_addr;

    while(mmap < mbi->mmap_addr + mbi->mmap_length)
    {
        mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(mmap->size) );
    }

#ifdef DBG_INIT
    printk("RAM size: %d MB \n",  mbi->mem_upper / 1024 + 2);
    printk("PAGING size: %d \n",  high_pages);
#endif

	init_gdt();
	printkok("Initialized GDT");
	init_idt();
	printkok("Initialized IDT");
	initialize_paging(total_frames, 0, 0);
	printkok("Initialized Paging");
	malloc_stats();
	printkok("Initialized Memory Management Engine");
	pic_init();
	printkok("Initialized PIC");
	pit_init();
	printkok("Initialized PIT");
	init_keyboard();
	printkok("Initialized Keyboard");
	shell();

	/* XXX: Kernel can't reach this zone,
	if it does, throw a Kernel Panic Error */
	PANIC("Reached Kernel End!");
	while(1);
}