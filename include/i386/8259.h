#include <stdint.h>

#define PIC_MASTER_CMD 0x20
#define PIC_SLAVE_CMD 0xA0
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_DATA 0xA1
#define PIC_CMD_EOI 0x20

void pic_send_eoi(char irq);