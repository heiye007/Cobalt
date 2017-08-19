#include "idt.h"

extern void idt_flush(uint16_t);
void init_idt();
static void idt_set_gate(unsigned char,uint16_t,unsigned short,unsigned char);

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

void init_idt()
{
    idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
    idt_ptr.base  = (uint16_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

    idt_set_gate(0, (uint16_t)isr0 , 0x08, 0x8E);
    idt_set_gate(1, (uint16_t)isr1 , 0x08, 0x8E);
    idt_set_gate(2, (uint16_t)isr2 , 0x08, 0x8E);
    idt_set_gate(3, (uint16_t)isr3 , 0x08, 0x8E);
    idt_set_gate(4, (uint16_t)isr4 , 0x08, 0x8E);
    idt_set_gate(5, (uint16_t)isr5 , 0x08, 0x8E);
    idt_set_gate(6, (uint16_t)isr6 , 0x08, 0x8E);
    idt_set_gate(7, (uint16_t)isr7 , 0x08, 0x8E);
    idt_set_gate(8, (uint16_t)isr8 , 0x08, 0x8E);
    idt_set_gate(9, (uint16_t)isr9 , 0x08, 0x8E);
    idt_set_gate(10, (uint16_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint16_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint16_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint16_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint16_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint16_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint16_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint16_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint16_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint16_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint16_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint16_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint16_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint16_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint16_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint16_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint16_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint16_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint16_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint16_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint16_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint16_t)isr31, 0x08, 0x8E);

    idt_flush((uint16_t)&idt_ptr);
}

static void idt_set_gate(unsigned char num, uint16_t base, unsigned short sel, unsigned char flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel     = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags   = flags;
}