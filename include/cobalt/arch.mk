ifeq ($(ARCH), x86)
include ./arch/$(ARCH)/config.make
include ./arch/$(ARCH)/Makefile
CPUARCH = '\
set timeout=0\n\
set default=0\n\
menuentry "Cobalt" {\n\
\tmultiboot /boot/kernel.bin\n\
\tboot\n\
}'
endif