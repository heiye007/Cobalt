ENTRY(_start)
SECTIONS
{
    . = 1M;
    x86_kernel_start = .;

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        x86_kernel_code = .;
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        x86_kernel_rodata = .;
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        x86_kernel_data = .;
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        x86_kernel_bss = .;
        *(COMMON)
        *(.bss)
        x86_kernel_isi = .;
        *(.bootstrap_stack)
        x86_kernel_ise = .;
    }

    .x86_kend BLOCK(4K) : ALIGN(4K)
    {
        *(.x86_kend)
    }
    
    x86_kernel_end = .;
    x86_kernel_size = x86_kernel_end - x86_kernel_code;
    x86_kernel_iss = x86_kernel_ise - x86_kernel_isi;
}