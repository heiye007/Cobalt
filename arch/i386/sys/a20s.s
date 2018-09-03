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
    pusha
    mov $0x112345, %edi
    mov $0x12345, %esi
    mov %esi,(%esi)
    mov %edi,(%edi)
    cmpsl %es:(%edi), %ds:(%esi)
    popa
    jne _return_a20_code
    xor %eax, %eax
    ret

_return_a20_code:
    mov $0x1,%eax
    ret
