#ifndef _REGS_H_
#define _REGS_H_

#include <stdint.h>

struct regs
{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, csc, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, esp, ss;
};

typedef struct {
   uint32_t ds;
   uint32_t edi, esi, ebp, csc, ebx, edx, ecx, eax;
   uint32_t int_no, err_code;
   uint32_t eip, cs, eflags, esp, ss;
} registers_t;

#endif