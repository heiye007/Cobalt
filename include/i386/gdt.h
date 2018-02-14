#include <stdint.h>

/* cake: We use __attribute__((packed))
   in order to prevent the compiler
   from optimizing the code in any way */

/* GDT scheme */
struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

/* GDT's pointer scheme */
struct gdt_ptr
{
    unsigned short limit;
    uint16_t base;
} __attribute__((packed));

/* GDT structure */
struct gdt_entry gdt[3];

/* GDT pointer */
struct gdt_ptr gp;

/* cake: Function declared on another file
   to reload the segment registers and swap
   out CPU's GDT table with ours */
extern void gdt_flush();
void gdt_set_gate(int num, size_t base, size_t limit, uint8_t access, uint8_t gran);
void init_gdt(void);		