global gdt_flush	; Allow 'gdt_flush' to be called from C
extern gp			; gp is declared on C code
gdt_flush:
    lgdt [gp]		; Load the GDT with our gp pointer
    mov ax, 0x10	; 0x10 is our data segment offset
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2	; 0x08 is our code segment offset
flush2:
    ret				; Return to C code