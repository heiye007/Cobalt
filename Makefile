KERNEL=kernel.elf
IMAGE=kernel.iso
IMAGEDIR = iso
BINDIR = ./bin
INCDIR= -Iinclude

include ./include/cobalt/arch.mk

CFLAG:=$(CFLAG) -D$(ARCH)

kernel: 
	make clean
	make compile
	make iso
	
all:
	make clean
	make compile
	make iso
	make run
	
debug:
	make clean
	make compile
	make iso
	make run
	make clean

compile: $(OBJS)
	@echo "LD   $(KERNEL)"
	${CROSS_COMPILE}gcc ${LDFLAG}
	mv ${OBJS} ${BINDIR}/objs
	mv ${KERNEL} ${BINDIR}

%.o: %.c
	@echo "CC   $<"
	${CROSS_COMPILE}gcc ${CFLAG} ${INCDIR} -c $< -o $@

%.o: %.s
	@echo "AS   $<"
	${CROSS_COMPILE}as $< -o $@

%.o: %.S
	@echo "AS   $<"
	${CROSS_COMPILE}as $< -o $@

%.o: %.asm
	@echo "AS   $<"

ifeq (${ARCH},i386)
	nasm -felf32 $< -o $@
else
	nasm -fbin $< -o $@
endif

clean:
	@echo "RM    OBJS"
	rm -rf ${OBJS}
	rm -rf ${BINDIR}/objs/*.o
	@echo "RM    BIN"
	rm -rf ${BINDIR}/${KERNEL}
	rm -rf ${IMAGEDIR}/boot/${KERNEL}
	@echo "RM    ISO"
	rm -rf *.iso
	rm -rf iso
	@echo "RM    INITRD"
	rm -rf make_initrd
	rm -rf initrd.img
	rm -rf *.txt
iso:
	-@mkdir -p iso/boot/grub
	-@echo $(CPUARCH) > iso/boot/grub/grub.cfg
	-@cp ${BINDIR}/${KERNEL} iso/boot/${KERNEL}
	-@gcc tools/make_initrd.c -o make_initrd
	-@printf "Cobalt version: 1.0\0" > version.txt
	-@printf "Welcome to Cobalt!\0" > motd.txt
	-@./make_initrd version.txt version.txt motd.txt motd.txt
	-@mv initrd.img iso/boot/initrd.img
	@echo "ISO  ${IMAGE}"
	-@grub-mkrescue -o ${IMAGE} ${IMAGEDIR} 2>/dev/null || true

run:
	qemu-system-${ARCH} -cdrom kernel.iso

$(V).SILENT:
