.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bootstrap_stack, "aw", @nobits
  stack_bottom:
  .skip 16384
  stack_top:

.section .text
.extern init
.type init, @function
.global _start
.type _start, @function
_start:

    mov $stack_top, %esp

    and $-16, %esp
    pushl %esp
    pushl $0
    popf

    pushl %esp # arg[3]: Initial Kernel ESP
    pushl %ebx # arg[2]: MBI Kernel Struct
    pushL %eax # arg[1]: Multiboot Magic Number

    cli
    call init
    jmp hang

hang:
    hlt
    jmp hang

.section .x86_kend
.global x86_kernel_end
x86_kernel_end:
