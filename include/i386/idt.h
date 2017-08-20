#include <stdint.h>

struct idt_entry
{
    unsigned short base_lo;
    unsigned short sel;
    uint8_t always0;
    uint8_t flags;
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr
{
    unsigned short limit;
    uint16_t base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load();