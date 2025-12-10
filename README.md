# MiniOS

MiniOS is a small teaching-oriented Unix-like OS for 32-bit x86, built from scratch in C and assembly and designed to run under QEMU.

## Building
- Requirements: `nasm`, `i386-elf-gcc` and `i386-elf-ld` (or host `gcc/ld` with `-m32`), `qemu-system-i386`.
- Build the image: `make`
- Run in QEMU: `make run`

## Current status
- Boots under QEMU using a 16-bit boot sector.
- Switches to 32-bit protected mode and prints a banner via the VGA text driver.

## Planned modules
- Interrupts (IDT/ISR/IRQ)
- Physical/virtual memory manager and kernel heap
- Scheduler and multitasking
- Syscall layer and basic userland (shell, demo programs)
- Timer and keyboard drivers
