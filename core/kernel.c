#include <i386/isr.h>
#include <multiboot.h>

void init(unsigned long magic, unsigned long addr) {
    multiboot_info_t *mbi;
	init_vga();
    init_gdt();
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        printf ("Invalid magic number: %x\n", (unsigned) magic);
        return;
    }
    printf("GDT Set\n");
    init_idt();
    printf("IDT Set\n");
    init_isr();
    printf("ISR Set\n");
    init_irq();
    printf("IRQ Set\n");
    char a[] = "aasdasd";
    printf("Hello: %s", a);
    __asm__ __volatile__ ("sti");
	//timer_install();
	//int i = 10 / 0;
	//printk(i);

    //init_timer(50);
	for(;;) {}
}