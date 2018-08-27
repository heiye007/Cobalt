#include <stdint.h>

static const char *x86_exception_messages[] =
{
    "Divide-by-zero Error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Reserved",
    "Security Exception",
    "Reserved",
    "Triple Fault",
    "FPU Error Interrupt",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

enum x86_interrupts
{
	X86_INT_DIVISION_BY_ZERO	= 0,
	X86_INT_DEBUG,
	X86_INT_NMI,
	X86_INT_BREAKPOINT,
	X86_INT_OVERFLOW,
	X86_INT_OUT_OF_BOUNDS,
	X86_INT_INVALID_OPCODE,
	X86_INT_DEVICE_NA,
	X86_INT_DOUBLE_FAULT,
	X86_INT_INVALID_TSS			= 0xA,
	X86_INT_SEGMENT_NOT_PRESENT,
	X86_INT_STACK_FAULT,
	X86_INT_GP_FAULT,
	X86_INT_PAGE_FAULT,
	X86_INT_FPU_EXCEPTION		= 0x10,
	X86_INT_ALIGNMENT_CHECK,
	X86_INT_MACHINE_CHECK,
	X86_INT_SIMD_FP_EXCEPTION,
	X86_INT_VIRT_EXCEPTION,
	X86_INT_SECURITY_EXCEPTION	= 0x1E,
	X86_INT_MAX_INTEL			= 0x1F,
};

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

typedef void (*isr_t)(registers_t);
isr_t x86_interrupt_handlers[256];

void init_isr(void);