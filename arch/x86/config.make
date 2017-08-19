LINKER= arch/$(ARCH)/linker.ld
CFLAG= -Iinclude -ffreestanding -Wall -Wextra
LDFLAG= -nostdlib -T $(LINKER) -o $(KERNEL) $(OBJS) -lgcc