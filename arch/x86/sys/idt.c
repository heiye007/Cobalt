#include <x86/idt.h>

void idt_set_gate(uint8_t num, size_t base, unsigned short sel, uint8_t flags) {
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void init_idt(void) {
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &idt;
    memset(&idt, 0, sizeof(struct idt_entry) * 256);
    idt_load();
}