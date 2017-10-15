ARCH=x86
KERNEL=kernel.bin
IMAGE=kernel.iso
IMAGEDIR = iso
BINDIR = ./bin
INCDIR= -I.

define x86
set timeout=0
set default=0
menuentry "Cobalt" {
	multiboot /boot/kernel.bin
	boot
}
endef
export x86

include ./arch/$(ARCH)/config.make
include ./core/Makefile
include ./usr/Makefile
include ./arch/$(ARCH)/Makefile

CFLAG:=$(CFLAG) -D__$(ARCH)__

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
	-@echo "$$x86" > iso/boot/grub/grub.cfg
	-@cp ${BINDIR}/${KERNEL} iso/boot/${KERNEL}
	@echo "ISO  ${IMAGE}"
	-@grub-mkrescue -o ${IMAGE} ${IMAGEDIR} 2>/dev/null || true

$(V).SILENT:
