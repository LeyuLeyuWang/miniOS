[BITS 32]

; void gdt_flush(uint32_t gdt_ptr)
global gdt_flush

gdt_flush:
    lgdt [esp + 4]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush_label

flush_label:
    ret
