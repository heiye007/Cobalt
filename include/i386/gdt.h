#include <stdint.h>

struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr
{
    unsigned short limit;
    uint16_t base;
} __attribute__((packed));

struct gdt_entry gdt[3];
struct gdt_ptr gp;

extern void gdt_flush();		