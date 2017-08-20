#include <i386/isr.h>
#include <multiboot.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

void init(unsigned long magic, unsigned long addr) {
	multiboot_info_t *mbi;
	init_vga();
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("Invalid magic number: 0x%x\n", (unsigned) magic);
		return;
	}
	mbi = (multiboot_info_t *) addr;
	uint16_t mem_lower = mbi->mem_lower;
	uint16_t mem_higher = mbi->mem_upper;
	uint16_t totalmem = mem_lower+mem_higher;

	if (CHECK_FLAG(mbi->flags, 0))
	printf("mem_lower = %uKB, mem_upper = %uKB\n",
		(unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);
	printf("totalmem = %uKB , in MB: %uMB\n", totalmem+1024, (totalmem+1024)/1024);
	init_gdt();
	printf("GDT Set\n");
	init_idt();
	printf("IDT Set\n");
	init_isr();
	printf("ISR Set\n");
	init_irq();
	printf("IRQ Set\n");
	__asm__ __volatile__ ("sti");
	keyboard_install();
	while(1);
	//timer_install();
	//int i = 10 / 0;
	//printk(i);
	//init_timer(50);
}