#include <i386/idt.h>

/* Function to craft a gate for the Interrupt Descriptor Table */
void idt_set_gate(uint8_t num, size_t base, unsigned short sel, uint8_t flags)
{
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

/* Called on architecture init.
   Setups the IDT, initializes the ISR's
   and IRQ's within and finally enables
   interrupts */
void init_idt(void)
{
    /* Setup IDT limit */
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;

    /* Setup IDT pointer */
    idtp.base = &idt;

    /* Clear the IDT by zeroing out it */
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    /* Init ISR's */
    init_isr();

    /* Init IRQ's */
    init_irq();

    /* Replace old IDT with the new one by 
       flushing all the changes */
    idt_load();

    /* Enable Interrupts */
    __asm__ __volatile__ ("sti");
}