#include <stdint.h>

struct regs {
    uint16_t gs, fs, es, ds;
    uint16_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint16_t int_no, err_code;
    uint16_t eip, cs, eflags, useresp, ss;
};

typedef struct registers
{
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;