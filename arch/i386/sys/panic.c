#include <i386/vga.h>
#include <i386/regs.h>
#include <i386/panic.h>
#include <stdint.h>

void panic(const char* err)
{
	struct regs *r;
	disable_cursor();
	settextcolor(CYAN, BLUE);
	cls();
	printk("\n");
	printkcenter("Kernel Panic!");
	printk("\n     EAX: 0x%x", r->eax);
	printk("  EBX: 0x%x", r->ebx);
	printk("  ECX: 0x%x", r->ecx);
	printk("  EDX: 0x%x", r->edx);
	printk("\n     ESI: 0x%x", r->esi);
	printk("  EDI: 0x%x", r->edi);
	printk("  EBP: 0x%x", r->ebp);
	printk("  ESP: 0x%x", r->esp);
	printk("\n     Reason: %s", err);
	__asm__ __volatile__ ("cli");
	__asm__ __volatile__ ("hlt");
	for (;;);
}