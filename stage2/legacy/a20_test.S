check_a20:
    pushf
    push %ds
    push %es
    push %di
    push %si
    cli
    xor %ax, %ax    // ax = 0
    mov %ax, %es    // es = ax
    not %ax         // ax = 0xFFFF
    mov %ax, %ds    // ds = ax
    mov $0x0500 %di // di = 0x0500
    mov $0x0510 %si // si = 0x0510
 
    movb %es:(%di), %al
    push %ax
 
    mov %ds:(%si), %al
    push %ax
 
    movb $0x00, %es:(%di) 
    movb $0xff, %ds:(%si) 
 
    cmpb $0xff, %es:(%di) 
 
    pop %ax
    movb %al, %ds:(%si)
 
    pop %ax
    movb %al,%ds:(%si)
 
    mov $0, %ax
    je .L1
    mov $1, %ax
 
.L1:
    pop si
    pop di
    pop es
    pop ds
    popf
 
    ret