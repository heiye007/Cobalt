#include <i386/8259.h>
#include <i386/vga.h>
#include <i386/system.h>
#include <stdint.h>

void pic_remap(void)
{
    outb(PIC_MASTER_CMD, 0x11);
    outb(PIC_SLAVE_CMD, 0x11);
    outb(PIC_MASTER_DATA, 0x20);
    outb(PIC_SLAVE_DATA, 0x28);
    outb(PIC_MASTER_DATA, 0x04);
    outb(PIC_SLAVE_DATA, 0x02);
    outb(PIC_MASTER_DATA, 0x01);
    outb(PIC_SLAVE_DATA, 0x01);
    outb(PIC_MASTER_DATA, 0x0);
    outb(PIC_SLAVE_DATA, 0x0);
}

void pic_send_eoi(unsigned char irq)
{
    if(irq >= 8)
        outb(PIC_SLAVE_CMD, PIC_CMD_EOI);

    outb(PIC_MASTER_CMD, PIC_CMD_EOI);
}