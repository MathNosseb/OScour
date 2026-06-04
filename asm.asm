start:
    mov eax, 0x0F41
    mov [0xB8000], eax
    jmp start