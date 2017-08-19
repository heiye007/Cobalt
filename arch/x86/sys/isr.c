#include "isr.h"

void isr_handler(registers_t registers)
{
    printk("recieved interrupt: ");
    printkhex(registers.int_no);
    printk("\n");
}