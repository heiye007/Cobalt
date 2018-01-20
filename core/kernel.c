#ifdef i386
#include <i386/isr.h>
#endif
#include <multiboot.h>
#include <panic.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

extern uint32_t kernel_end;
extern uint32_t kernel_start;

void init(unsigned long magic, multiboot_info_t *mbi) {
	init_vga();

#ifdef DBG_CPU
	getCPUVendor();
	getCPUFeatures();
	getCPUArch();
	getCPUName();
#endif

	uint32_t low_pages = 256;
    uint32_t high_pages = (mbi->mem_upper * 1024) / 4096 + 30000;

    uint32_t total_frames = high_pages + low_pages;

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("Invalid magic number: 0x%x\n", (unsigned) magic);
		__asm__ __volatile__ ("cli");
		__asm__ __volatile__ ("hlt");
		return;
	}

	printf("Kernel base is %x, end is %x\n", &kernel_start, &kernel_end);

    multiboot_memory_map_t* mmap = mbi->mmap_addr;
    while(mmap < mbi->mmap_addr + mbi->mmap_length) {
        mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(mmap->size) );
    }

    printf("RAM size: %d MB \n",  mbi->mem_upper / 1024 + 2);
    printf("PAGING size: %d \n",  high_pages);

	init_gdt();
	printkok("Initialized GDT");
	init_idt();
	printkok("Initialized IDT");
	init_isr();
	printkok("Initialized ISR's");
	initialize_paging(total_frames, 0, 0);
	malloc_stats();
	init_irq();
	printkok("Initialized IRQ's");
	__asm__ __volatile__ ("sti");
	pic_init();
	printkok("Initialized PIC");
	pit_init();
	printkok("Initialized PIT");
	init_keyboard();
	printkok("Initialized Keyboard");
	
	/* XXX: Legacy Paging Tester
	unsigned int *ptr = (unsigned int*)0xA0000000;
   	unsigned int do_page_fault = *ptr;*/
	shell();

	/* XXX: Kernel can't reach this zone,
	if it does, throw a Kernel Panic Error */
	PANIC("Reached Kernel End!");
	while(1);
}