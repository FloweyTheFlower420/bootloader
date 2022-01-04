_Z9fetch_farj:
    pushl   %ebp
    movl    %esp, %ebp
    // eax = segment
    // ebx = offset
    mov 8(%ebp), %eax
    push %ebx
    // eax = ebx = full-address, bx = offset
    mov %eax, %ebx
    // ax now contains segment
    shr $4, %eax
    mov %ax, %ds
    xor %eax, %eax
    movw %ds:0(%bx), %ax
    xor %ecx, %ecx
    mov %cx, %ds
    pop %ebx
    popl %ebp
    ret