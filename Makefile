ARCH=i386
KERNEL=kernel.bin
IMAGE=kernel.iso
IMAGEDIR = iso
BINDIR = ./bin
INCDIR= -I.

define i386
set timeout=0
set default=0
menuentry "Cobalt" {
	multiboot /boot/kernel.bin
	boot
}
endef
export i386

include ./arch/$(ARCH)/config.make
include ./core/Makefile
include ./usr/Makefile
include ./arch/$(ARCH)/Makefile

CFLAG:=$(CFLAG) -D__$(ARCH)__

all: $(OBJS)
	@echo "LD   $(KERNEL)"
	${GCC} ${LDFLAG}
	mv ${OBJS} ${BINDIR}/objs
	mv ${KERNEL} ${BINDIR}

%.o: %.c
	@echo "CC   $<"
	${GCC} ${CFLAG} ${INCDIR} -c $< -o $@

%.o: %.s
	@echo "AS   $<"
	${AS} $< -o $@

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
	-@echo "$$i386" > iso/boot/grub/grub.cfg
	-@cp ${BINDIR}/${KERNEL} iso/boot/${KERNEL}
	@echo "ISO  ${IMAGE}"
	-@grub-mkrescue -o ${IMAGE} ${IMAGEDIR} 2>/dev/null || true

$(V).SILENT:
