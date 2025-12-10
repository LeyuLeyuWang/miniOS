; MiniOS Bootloader - 16-bit real mode boot sector
; Loads kernel placed immediately after boot sector and jumps to 32-bit entry

BITS 16
ORG 0x7C00

KERNEL_LOAD_SEG equ 0x1000           ; Segment to load kernel (physical 0x10000)
KERNEL_LOAD_OFF equ 0x0000
KERNEL_ENTRY    equ (KERNEL_LOAD_SEG << 4) + KERNEL_LOAD_OFF

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov [boot_drive], dl              ; Preserve boot drive

    call load_kernel

    ; Set up a simple GDT for protected mode switch
    lgdt [gdt_descriptor]

    ; Enter protected mode
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:init_pm

; 32-bit protected mode
[BITS 32]
init_pm:
    ; Update segment registers
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Set up stack
    mov esp, 0x9FC00          ; place stack near top of first MiB

    ; Jump to kernel entry
    jmp KERNEL_ENTRY

.hang:
    hlt
    jmp .hang

; ------------------------------
; Kernel loader
; ------------------------------
[BITS 16]
load_kernel:
    mov bx, KERNEL_LOAD_OFF
    mov ax, KERNEL_LOAD_SEG
    mov es, ax

    mov ah, 0x02              ; BIOS read sectors
    mov al, 16                ; number of sectors to read (adjust as kernel grows)
    mov ch, 0x00              ; cylinder
    mov dh, 0x00              ; head
    mov cl, 0x02              ; sector (start after boot sector)
    mov dl, [boot_drive]
    int 0x13
    jc disk_error
    ret

disk_error:
    mov si, disk_error_msg
    call print_string
    jmp $  ; halt

; Simple routine to print a string in real mode
print_string:
    mov ah, 0x0E
.print_loop:
    lodsb
    or al, al
    jz .done
    int 0x10
    jmp .print_loop
.done:
    ret

boot_drive db 0

disk_error_msg db 'Disk read error', 0

; ------------------------------
; Simple Global Descriptor Table
; ------------------------------
ALIGN 8
gdt_start:
    ; Null descriptor
    dq 0x0000000000000000
    ; Code segment: base 0x0, limit 0xFFFFF, flags 0x9A, granularity 4K/32-bit
    dq 0x00CF9A000000FFFF
    ; Data segment: base 0x0, limit 0xFFFFF, flags 0x92, granularity 4K/32-bit
    dq 0x00CF92000000FFFF

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ 0x08
DATA_SEG equ 0x10

; ------------------------------
; Boot sector padding and magic
; ------------------------------
times 510-($-$$) db 0
DW 0xAA55
