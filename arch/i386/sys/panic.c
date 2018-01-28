#include <i386/vga.h>
#include <i386/regs.h>
#include <i386/panic.h>
#include <stdint.h>

void panic(char* err, const char *file, uint32_t line)
{
	struct regs* r;
	disable_cursor();
	settextcolor(CYAN, BLUE);
	cls();
	printf("\n");
	printkcenter("Kernel Panic!");
	printf("\n     EAX: 0x%x", r->eax);
	printf("  EBX: 0x%x", r->ebx);
	printf("  ECX: 0x%x", r->ecx);
	printf("  EDX: 0x%x", r->edx);
	printf("\n     ESI: 0x%x", r->esi);
	printf("  EDI: 0x%x", r->edi);
	printf("  EBP: 0x%x", r->ebp);
	printf("  ESP: 0x%x", r->esp);
	printf("\n     Reason: (%s) at %s:%d\n", err, file, line);
	__asm__ __volatile__ ("cli");
	__asm__ __volatile__ ("hlt");
	for (;;);
}