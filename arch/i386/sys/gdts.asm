[GLOBAL gdt_flush]	; Allow 'gdt_flush' to be called from C

gdt_flush:
   mov eax, [esp+4]
   lgdt [eax]        ; Load the GDT with our pointer

   mov ax, 0x10		; 0x10 is our data segment offset
   mov ds, ax
   mov es, ax
   mov fs, ax
   mov gs, ax
   mov ss, ax
   jmp 0x08:.flush   ; 0x08 is our code segment offset
.flush:
   ret               ; Return to C code