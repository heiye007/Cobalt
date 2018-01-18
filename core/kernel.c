#ifdef i386
#include <i386/isr.h>
#endif
#include <multiboot.h>
#include <panic.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

void init(unsigned long magic, multiboot_info_t *mbi) {
	init_vga();
#ifdef DBG_CPU
	getCPUVendor();
	getCPUFeatures();
	getCPUArch();
	getCPUName();
#endif

	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("Invalid magic number: 0x%x\n", (unsigned) magic);
		__asm__ __volatile__ ("cli");
		__asm__ __volatile__ ("hlt");
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