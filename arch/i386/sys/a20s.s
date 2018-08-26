.section .text
.align 4

.global a20_enable_bios
.type a20_enable_bios, @function
a20_enable_bios:
.code16 # Switch to 16 bits to use BIOS calls
    movw $0x2401, %ax
    int $0x15

.code32 # Switch back to 32 bits
    jc _a20_enable_bios_fail
    mov $1, %eax
    ret

.global _a20_enable_bios_fail
.type _a20_enable_bios_fail, @function
_a20_enable_bios_fail:
    xor %eax, %eax
    ret

.global check_a20
.type check_a20, @function
check_a20:
    cli # Disable interrupts
    push %ebx
    push %ecx
    push %edx

    call wait_input
    mov $0xAD, %al 
    outb %al, $0x64

    call wait_input
    mov $0xD0, %al
    out %al, $0x64

    call wait_output
    inb $0x60, %al
    push %eax

    call wait_input
    mov $0xd1, %al
    outb %al, $0x64

    call wait_input
    pop %eax
    or $2, %al
    out %al, $0x60

    call wait_input
    mov $0xae, %al
    out %al, $0x64

    call wait_input
    
    pop %eax
    pop %edx
    pop %ecx
    pop %ebx
    sti # Enable interrupts
    ret

.global wait_input
.type wait_input, @function
wait_input:
    inb $0x64, %al
    test $2, %al
    jnz wait_input
    ret

.global wait_output
.type wait_output, @function
wait_output:
    inb $0x64, %al
    test $1, %al
    jz wait_output
    ret
