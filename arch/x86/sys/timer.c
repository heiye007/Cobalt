#include <i386/regs.h>

int timer_ticks = 0;

void timer_handler(struct regs *r) {
    timer_ticks++;

    if (timer_ticks % 18 == 0) {
        printf("One second has passed\n");
    }
}

void timer_wait(int ticks) {
    size_t eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks) 
    {
        __asm__ __volatile__ ("sti//hlt//cli");
    }
}

void init_timer(void) {
    irq_install_handler(0, timer_handler);
}