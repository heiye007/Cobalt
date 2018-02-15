#include <i386/cpuid.h>

void cpuid(uint16_t reg, uint16_t *eax, uint16_t *ebx, uint16_t *ecx, uint16_t *edx)
{
    __asm__ __volatile__("cpuid"
        : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx)
        : "0" (reg));
}