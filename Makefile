KERNEL=kernel.bin
IMAGE=kernel.iso
IMAGEDIR = iso
BINDIR = ./bin
INCDIR= -Iinclude

include ./include/cobalt/arch.mk
include ./core/Makefile
include ./usr/Makefile

CFLAG:=$(CFLAG) -D$(ARCH)

all: $(OBJS)
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
	nasm -felf32 $< -o $@

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

$(V).SILENT:
