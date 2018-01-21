LINKER= arch/$(ARCH)/linker.ld
CFLAG= -I./include -std=c11 -ffreestanding -Wall -Wextra
LDFLAG= -nostdlib -T $(LINKER) -o $(KERNEL) $(OBJS) -lgcc