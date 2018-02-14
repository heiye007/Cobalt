#include <stdint.h>

void timer_phase(int hz);
void timer_handler();
void timer_wait(int ticks);
void pit_init(void);