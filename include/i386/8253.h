#include <stdint.h>
#include <i386/vga.h>
#include <i386/regs.h>
#include <stdint.h>

void timer_phase(int hz);
void timer_handler(struct regs *r);
void timer_wait(int ticks);
void pit_init(void);