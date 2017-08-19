LINKER= arch/$(ARCH)/linker.ld
CFLAG= -I. -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAG= -ffreestanding -O2 -nostdlib -T $(LINKER) -o $(KERNEL) $(OBJS) -lgcc