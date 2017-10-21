KERNEL=kernel.bin
IMAGE=kernel.iso
IMAGEDIR = iso
BINDIR = ./bin
INCDIR= -Iinclude

include ./core/Makefile
include ./include/cobalt/arch.mk
include ./usr/Makefile

CFLAG:=$(CFLAG) -D$(ARCH)

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

kernel: 
	make clean
	make compile
	make iso

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

iso:
	-@mkdir -p iso/boot/grub
	-@echo $(CPUARCH) > iso/boot/grub/grub.cfg
	-@cp ${BINDIR}/${KERNEL} iso/boot/${KERNEL}
	@echo "ISO  ${IMAGE}"
	-@grub-mkrescue -o ${IMAGE} ${IMAGEDIR} 2>/dev/null || true

run:
	qemu-system-${ARCH} -cdrom kernel.iso

$(V).SILENT:
