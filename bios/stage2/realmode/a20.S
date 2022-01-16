.code16gcc
.code16
.global a20

a20:
    push %eax
    in $0x92, %al
    or $2, %al
    out %al, $0x92
    pop %eax
