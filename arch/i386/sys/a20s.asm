[GLOBAL a20_enable_bios]

a20_enable_bios:
    [BITS 16]
    mov ax, 0x2401
    int 0x15

    [BITS 32]
    jc _a20_enable_bios_fail
    mov eax, 1
    ret

_a20_enable_bios_fail:
    xor eax, eax
    ret

[GLOBAL check_a20]
check_a20:
    cli
    push ebx
    push ecx
    push edx

    call wait_input

    mov al, 0xAD
    out 0x64, al
    call wait_input

    mov al, 0xD0
    out 0x64, al
    call wait_output

    in al, 0x60
    shr eax, 1
    and eax, 1
    push eax

    call wait_input
    mov al, 0xAE
    out 0x64, al

    call wait_input
    
    pop eax
    pop edx
    pop ecx
    pop ebx
    ; sti
    ret

wait_input:
    in al, 0x64
    test al, 2
    jnz wait_input
    ret

wait_output:
    in al, 0x64
    test al, 1
    jz wait_output
    ret