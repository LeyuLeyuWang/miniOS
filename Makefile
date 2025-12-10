# MiniOS build system
.RECIPEPREFIX := >

CC = gcc
LD = ld
AS = nasm
OBJCOPY = objcopy

CFLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32 -Isrc -Isrc/lib
LDFLAGS := -T linker.ld -m elf_i386 -nostdlib

C_SOURCES := $(shell find src -name "*.c" ! -path "src/boot/*")
ASM_SOURCES := $(shell find src -name "*.asm" ! -path "src/boot/*")
C_OBJS := $(patsubst src/%.c,build/%.o,$(C_SOURCES))
ASM_OBJS := $(patsubst src/%.asm,build/%_asm.o,$(ASM_SOURCES))
OBJS := $(C_OBJS) $(ASM_OBJS)

BOOT_BIN := build/boot.bin
KERNEL_ELF := build/kernel.elf
KERNEL_BIN := build/kernel.bin
IMG := build/os.img

.PHONY: all clean run dirs

all: $(IMG)

$(IMG): $(BOOT_BIN) $(KERNEL_BIN)
> @mkdir -p $(dir $@)
> cat $(BOOT_BIN) $(KERNEL_BIN) > $@

$(BOOT_BIN): src/boot/boot.asm | dirs
> $(AS) -f bin $< -o $@

$(KERNEL_BIN): $(KERNEL_ELF)
> $(OBJCOPY) -O binary $< $@

$(KERNEL_ELF): $(OBJS) linker.ld | dirs
> $(LD) $(LDFLAGS) -o $@ $(OBJS)

build/%.o: src/%.c | dirs
> @mkdir -p $(dir $@)
> $(CC) $(CFLAGS) -c $< -o $@

build/%_asm.o: src/%.asm | dirs
> @mkdir -p $(dir $@)
> $(AS) -f elf32 $< -o $@

dirs:
> @mkdir -p build

run: $(IMG)
> qemu-system-i386 -drive format=raw,file=$(IMG)

clean:
> rm -rf build
