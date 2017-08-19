#include <stdint.h>

typedef struct registers
{
    uint16_t ds;
    uint16_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint16_t int_no, err_code; 
    uint16_t eip, cs, eflags, useresp, ss;
} registers_t;