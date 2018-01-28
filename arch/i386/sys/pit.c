#include <i386/pit.h>
#include <i386/vga.h>
#include <i386/regs.h>
#include <stdint.h>

void pit_irq(void)
{
    __asm__ __volatile__("add $0x1c, %esp");
    __asm__ __volatile__("pusha");
    pic_send_eoi(0);
    __asm__ __volatile__("popa");
    __asm__ __volatile__("iret");
}

static inline void __pit_send_cmd(uint8_t cmd)
{
    outb(PIT_REG_COMMAND, cmd);
}

static inline void __pit_send_data(uint16_t data, uint8_t counter)
{
    uint8_t port = (counter==PIT_OCW_COUNTER_0) ? PIT_REG_COUNTER0 :
        ((counter==PIT_OCW_COUNTER_1) ? PIT_REG_COUNTER1 : PIT_REG_COUNTER2);

    outb (port, (uint8_t)data);
}

static inline uint8_t __pit_read_data(uint16_t counter)
{
    uint8_t port = (counter==PIT_OCW_COUNTER_0) ? PIT_REG_COUNTER0 :
        ((counter==PIT_OCW_COUNTER_1) ? PIT_REG_COUNTER1 : PIT_REG_COUNTER2);

    return inb(port);
}

static void pit_start_counter(uint32_t freq, uint8_t counter, uint8_t mode)
{
    if (freq==0)
    {
        return;
    }

#ifdef DBG_PIT
    printf("Starting counter %d with frequency %dHz\n", counter/0x40, freq);
#endif

    uint16_t divisor = (uint16_t)(1193181/(uint16_t)freq);
    uint8_t ocw = 0;
    ocw = (ocw & ~PIT_OCW_MASK_MODE) | mode;
    ocw = (ocw & ~PIT_OCW_MASK_RL) | PIT_OCW_RL_DATA;
    ocw = (ocw & ~PIT_OCW_MASK_COUNTER) | counter;
    __pit_send_cmd(ocw);
    __pit_send_data(divisor & 0xff, 0);
    __pit_send_data((divisor >> 8) & 0xff, 0);
}

extern int lastlinedetect;

void timer_handler(struct regs *r)
{
// XXX: Top Line Debugging for Cobalt

// XXX: Example that shows column, row & lastlinedetect
// cake: Used this to debug backspacing
#ifdef DBG_PIT_TOPLINE_VGA
    int old_row = get_row();
    int old_col = get_col();
    update_cursor(0,0);
    settextcolor(BLACK, LGRAY);
    for (int loop = 0; loop < 80; loop++) {
        putch(' ');
    }
    update_cursor(0,0);
    if (lastlinedetect == 0) {
       printf("Current row: %d , Current col: %d, lastlinedetect = %d, Detection Happened", old_row, old_col, lastlinedetect); 
    } else {
        printf("Current row: %d , Current col: %d, lastlinedetect = %d", old_row, old_col, lastlinedetect); 
    }
    settextcolor(WHITE, BLACK);
    update_cursor(old_row, old_col);
#endif
}

void pit_init(void)
{
#ifdef DBG_PIT
    printk("Starting PIT initialization...\n");
#endif
    irq_install_handler(0, (uint32_t)timer_handler);
    irq_install_handler(32, (uint32_t)pit_irq);
    pit_start_counter(200,PIT_OCW_COUNTER_0, PIT_OCW_MODE_SQUAREWAVEGEN);

#ifdef DBG_PIT
    printk("Started PIT successfully...\n");
#endif
}