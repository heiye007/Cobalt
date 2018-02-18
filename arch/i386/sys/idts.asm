[GLOBAL idt_flush]	; Allow 'idt_flush' to be called from C

idt_flush:
   mov eax, [esp+4]
   lidt [eax]		; Load the IDT with out pointer
   ret