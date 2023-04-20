TOOLCHAIN = x86_64-linux-gnu
CC = $(TOOLCHAIN)-gcc
LD = $(TOOLCHAIN)-ld
AS = $(TOOLCHAIN)-as
NASM = nasm

CCPARAMS = -m32 -ffreestanding -Iinclude/ -w
ASPARAMS = --32
LDPARAMS = -m elf_i386
NASMFLAGS =

SRC = $(wildcard kernel/*.c)
SRC += $(wildcard driver/*.c)
SRC += $(wildcard cpu/*.c)
SRC += $(wildcard libc/*.c)

OBJS = boot/loader.o cpu/inturrpts.o
OBJS += $(SRC:.c=.o)
out = build/myos

all: build $(out)
	@echo $(out) is ready

$(out): linker.ld $(OBJS)
	@$(LD) $(LDPARAMS) -T $< -o $@ $(OBJS)
	@echo "  LD $<"

%.o: %.s
	@$(AS) $(ASPARAMS) -o $@ $<
	@echo "  AS $^"

%.o: %.c
	@$(CC) $(CCPARAMS) -o $@ -c $<
	@echo "  CC $^"

%.o: %.asm
	@$(NASM) $(NASMFLAGS) -f elf32 -o $@ $<

build:
	@mkdir -vp build

.PHONY: clean run

run: all
	qemu-system-i386 -kernel $(out)

clean:
	rm -rf boot/*.o kernel/*.o driver/*.o cpu/*.o build
