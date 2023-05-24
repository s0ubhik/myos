TOOLCHAIN = x86_64-linux-gnu
CC = $(TOOLCHAIN)-gcc
LD = $(TOOLCHAIN)-ld
AS = $(TOOLCHAIN)-as
NASM = nasm

CCPARAMS = -m32 -ffreestanding -Iinclude/ -Ilibc -w
ASPARAMS = --32
LDPARAMS = -m elf_i386
NASMFLAGS =

SRC = $(wildcard kernel/*.c)
SRC += $(wildcard cpu/*.c)
SRC += $(wildcard driver/*.c)
SRC += $(wildcard libc/*.c)
SRC += $(wildcard mem/*.c)

OBJS = boot/loader.o cpu/inturrpts.o cpu/cpu.o
OBJS += $(SRC:.c=.o)
out = build/myos.iso
boot = build/iso/boot
grub = $(boot)/grub
cfg = $(grub)/grub.cfg
kernel = build/kernel.bin

all: build $(out)
	@echo $(out) is ready

$(out): $(kernel)
	@mkdir -p $(grub)
	@echo "  ISO \t\033[32;1m$@\033[0m"
	@echo 'set timeout=0' > $(cfg)
	@echo 'set default=0' >> $(cfg)
	@echo '' >> $(cfg)
	@echo 'menuentry "myos" {' >> $(cfg)
	@echo '  multiboot /boot/kernel.bin' >> $(cfg)
	@echo '  set root=(hd1)' >> $(cfg)
	@echo '  boot' >> $(cfg)
	@echo '}' >> $(cfg)
	@cp $(kernel) $(boot)
	@grub-mkrescue --output=$@ build/iso >/dev/null 2>&1

$(kernel): linker.ld $(OBJS)
	@echo "  LD \t$@"
	@mkdir -p $(boot)
	@$(LD) $(LDPARAMS) -T $< -o $@ $(OBJS)

%.o: %.s
	@echo "  AS \t$@"
	@$(AS) $(ASPARAMS) -o $@ $<

%.o: %.c
	@echo "  CC \t$@"
	@$(CC) $(CCPARAMS) -o $@ -c $<

%.o: %.asm
	@echo "  NASM \t$@"

	@$(NASM) $(NASMFLAGS) -f elf32 -o $@ $<

build:
	@mkdir -vp build

.PHONY: clean run

run-vm: clean all
	@rm -rf boot/*.o kernel/*.o driver/*.o cpu/*.o libc/*.o mem/*.o
	@(killall VirtualBoxVM && sleep 1) || true
	@vboxmanage startvm myos &
	@rm -rf boot/*.o kernel/*.o driver/*.o cpu/*.o libc/*.o mem/*.o

run-qemu: all
	@qemu-system-i386 -kernel $(bin)

clean:
	@rm -rf build boot/*.o kernel/*.o driver/*.o cpu/*.o libc/*.o mem/*.o
