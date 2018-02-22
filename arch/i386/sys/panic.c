#include <i386/vga.h>
#include <i386/isr.h>
#include <i386/regs.h>
#include <i386/panic.h>
#include <stdint.h>

void x86_division_by_zero(struct regs *r)
{
	/* cakeh: When I implement tasking, recovering
	   from a division by zero will be as simple as
	   exiting and freeing the program that caused
	   the error, for now, just notify the user about
	   a division by zero being caught */
	settextcolor(RED, BLACK);
	printk("\nDivision by Zero!\n");
	disable_cursor();
	__asm__ __volatile__ ("cli");
	__asm__ __volatile__ ("hlt");
	while(1) {}
}

void x86_breakpoint_handler(struct regs *r)
{
	// TODO: Search up how to properly handle interrupts
	/* cakeh: Temporal fix for notifying the user */
	settextcolor(LCYAN, BLACK);
	printk("\nBreakpoint!\n");
	while(1) {}
}

void x86_gp_fault_handler(struct regs *r)
{
	/* cakeh: When I implement tasking, recovering
	   from a general protection fault will be as
	   simple as exiting and freeing the program
	   that caused the error, for now, just notify
	   the user about a GPF being caught */
	settextcolor(RED, BLACK);
	printk("\nGeneral Protection Fault! Continuing execution...\n");
	disable_cursor();
	__asm__ __volatile__ ("cli");
	__asm__ __volatile__ ("hlt");
	while(1) {}
}

void x86_unhandled_exception(struct regs *r)
{
	/* cakeh: Probably not the best method to deal
	   with unhandled exceptions, might look at this
	   in the future to ensure we're doing it correctly */
	PANIC(x86_exception_messages[r->int_no]);
}

void x86_panic(const char* err)
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