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
.align 16
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
    pushl %ebx
    pushl %eax

    cli
    call init

    cli
hang:
    hlt
    jmp hang

.section .kend
.global kernel_end
kernel_end:
