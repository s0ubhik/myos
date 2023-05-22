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
SRC += $(wildcard mem/*.c)

OBJS = boot/loader.o cpu/inturrpts.o
OBJS += $(SRC:.c=.o)
out = build/myos.iso
boot = build/iso/boot
grub = $(boot)/grub
cfg = $(grub)/grub.cfg
bin = $(boot)/myos.bin

all: build $(out)
	@echo $(out) is ready

$(out): $(bin)
	@mkdir -p $(grub)
	@echo "  ISO \033[32;1m$@\033[0m"
	@echo 'set timeout=0' > $(cfg)
	@echo 'set default=0' >> $(cfg)
	@echo '' >> $(cfg)
	@echo 'menuentry "myos" {' >> $(cfg)
	@echo '  multiboot /boot/myos.bin' >> $(cfg)
	@echo '  set root=(hd1)' >> $(cfg)
	@echo '  boot' >> $(cfg)
	@echo '}' >> $(cfg)
	@grub-mkrescue --output=$@ build/iso >/dev/null 2>&1

$(bin): linker.ld $(OBJS)
	@echo "  LD $<"
	@mkdir -p $(boot)
	@$(LD) $(LDPARAMS) -T $< -o $@ $(OBJS)

%.o: %.s
	@echo "  AS $^"
	@$(AS) $(ASPARAMS) -o $@ $<

%.o: %.c
	@echo "  CC $^"
	@$(CC) $(CCPARAMS) -o $@ -c $<

%.o: %.asm
	@echo "  NASM $^"

	@$(NASM) $(NASMFLAGS) -f elf32 -o $@ $<

build:
	@mkdir -vp build

.PHONY: clean clean_o run

run-vm: clean all
	@rm -rf boot/*.o kernel/*.o driver/*.o cpu/*.o libc/*.o mem/*.o
	@(killall VirtualBoxVM && sleep 1) || true
	@vboxmanage startvm myos &

run-qemu: all
	@qemu-system-i386 -kernel $(bin)

clean: clean_o
	@rm -rf build boot/*.o kernel/*.o driver/*.o cpu/*.o libc/*.o mem/*.o
