global idt_load	; Allow 'idt_load' to be called from C
extern idtp		; idtp is declared on C code
idt_load:
    lidt [idtp]	; Load the IDT with our idtp pointer
    ret			; Return to C code