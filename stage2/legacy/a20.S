.code16gcc
.code16
a20:
    push %al
    in $0x92, %al
	or $2, %al
	out %al, $0x92
    pop %al