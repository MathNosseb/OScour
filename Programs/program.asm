[bits 32]
call start
start:
    ;prend les 4 derniers octet de la pile (adresse de la fonction)
    ; les places dans ebx
    ;et les retires de la pile (add esp 4)
    pop ebx
    add ebx, hello
    sub ebx, 5
    ;il faut mettre l adresse de hello
    push ebx
    mov eax, [0x504];la fonction qui a son adresse a 0x504 -> print
    call eax
    add esp, 4
    ret

hello db "Hello World",0