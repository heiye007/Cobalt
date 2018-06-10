LINKER= arch/$(ARCH)/linker.ld
CFLAG= -std=c11 -ffreestanding -g -Wall -Wextra
LDFLAG= -ffreestanding -nostdlib -g -T $(LINKER) $(OBJS)  -o $(KERNEL) -lgcc