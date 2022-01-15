.code16
.code16gcc
    // set up registers
    xor %ax, %ax
    mov %ax, %ds
    mov %ax, %ss
    mov %ax, %es

    mov %dl, 0x1000

    mov $0x3000, %ax
    mov %ax, %sp

    mov $0x02, %ah
    mov $0x8, %al
    mov $0x7e00, %bx
    mov $0x0002, %cx
    mov $0x0, %dh
    int $0x13
    ljmp $0x0,$0x7e00
