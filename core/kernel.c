#ifdef i386
#include <i386/isr.h>
#endif
#include <multiboot.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

void init(unsigned long magic, unsigned long addr, multiboot_info_t *mbi) {
	init_vga();
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("Invalid magic number: 0x%x\n", (unsigned) magic);
		return;
	}
	init_gdt();
	printkok("Initialized GDT");
	init_idt();
	printkok("Initialized IDT");
	init_isr();
	printkok("Initialized ISR's");
	init_irq();
	printkok("Initialized IRQ's");
	__asm__ __volatile__ ("sti");
	//init_timer();
	//detectCPU();
#ifdef i386
	init_paging(mbi);
	printkok("Initialized Paging");
#endif
	printkok("Initialized Keyboard");
	init_keyboard();
#ifdef DBG_EXCEPTION
	/* On any kind of CPU, a division by zero should fire up an interrupt */
	int i = 10 / 0;
	printk(i);
#endif
	shell();
	while(1);
	//init_timer(50);
}