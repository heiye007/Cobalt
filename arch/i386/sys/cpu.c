#include <i386/cpu.h>

void getCPUVendor(void)
{
    uint16_t d;
    char vendor[13];
    cpuid(0, &d, (uint16_t *)(vendor + 0), (uint16_t *)(vendor + 8), (uint16_t *)(vendor + 4));
    vendor[12] = '\0';

    printf("CPU Vendor: %s\n", vendor);
}

void getCPUFeatures(void)
{
    uint16_t check;

    if (check >= 0x01)
    {
        cpuid(0x01, &eax, &ebx, &ecx, &edx);

        printf("CPU Features:");

        if (edx & CPUID_FEAT_EDX_PSE)      printf(" PSE");
        if (edx & CPUID_FEAT_EDX_PAE)      printf(" PAE");
        if (edx & CPUID_FEAT_EDX_APIC)     printf(" APIC");
        if (edx & CPUID_FEAT_EDX_MTRR)     printf(" MTRR");

        printf("\n");
        printf("CPU Instruction Set:");

        if (edx & CPUID_FEAT_EDX_TSC)      printf(" TSC");
        if (edx & CPUID_FEAT_EDX_MSR)      printf(" MSR");
        if (edx & CPUID_FEAT_EDX_SSE)      printf(" SSE");
        if (edx & CPUID_FEAT_EDX_SSE2)     printf(" SSE2");
        if (ecx & CPUID_FEAT_ECX_SSE3)     printf(" SSE3");
        if (ecx & CPUID_FEAT_ECX_SSSE3)    printf(" SSSE3");
        if (ecx & CPUID_FEAT_ECX_SSE41)    printf(" SSE41");
        if (ecx & CPUID_FEAT_ECX_SSE42)    printf(" SSE42");
        if (ecx & CPUID_FEAT_ECX_AVX)      printf(" AVX");
        if (ecx & CPUID_FEAT_ECX_F16C)     printf(" F16C");
        if (ecx & CPUID_FEAT_ECX_RDRAND)   printf(" RDRAND");

        printf("\n");
    }
}

void getCPUArch(void)
{
    uint16_t a;
    cpuid(0x80000000, &a, &ebx, &ecx, &edx);

    if (a >= 0x80000001)
    {
        cpuid(0x80000001, &eax, &ebx, &ecx, &edx);

        if (edx & CPUID_FEAT_EDX_64_BIT)
        {
            printf("CPU Architecture: 64-bit\n");
        }
        else
        {
            printf("CPU Architecture: 32-bit\n");
        }
    }
}

void getCPUName(void)
{
    uint16_t cpuop;

    if (cpuop >= 0x80000004)
    {
        char name[48];
        /*  EBX = 'h' 't' 'u' 'A'
            EDX = 'c' 't' 'n' 'e'
            ECX = 'D' 'M' 'A' 'c' */
        cpuid(0x80000002, (uint16_t *)(name +  0), (uint16_t *)(name +  4), (uint16_t *)(name +  8), (uint16_t *)(name + 12));
        cpuid(0x80000003, (uint16_t *)(name + 16), (uint16_t *)(name + 20), (uint16_t *)(name + 24), (uint16_t *)(name + 28));
        cpuid(0x80000004, (uint16_t *)(name + 32), (uint16_t *)(name + 36), (uint16_t *)(name + 40), (uint16_t *)(name + 44));

        const char *p = name;

        while (*p == ' ')
        {
            ++p;
        }

        printf("CPU Name: %s\n", p);
    }
}