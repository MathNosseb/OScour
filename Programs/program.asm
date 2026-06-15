[bits 32]
call next
next:
    pop ebx
    add ebx, hello - next
    push ebx
    mov eax, [0x504]
    call eax
    add esp, 4
    ret

hello db "Hello from external program", 0