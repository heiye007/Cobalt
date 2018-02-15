#include <i386/system.h>
#include <stdint.h>

uint16_t a20_enable_bios();
uint32_t check_a20();
int a20_enable_fast();
int a20_enable_ps2();
void init_a20();