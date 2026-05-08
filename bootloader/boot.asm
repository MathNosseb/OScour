[org 0x7c00]    

KERNEL_LOCATION equ 0x1000

mov ah, 0x00
mov al, 0x03    ; mode texte 80x25
int 0x10

xor ax, ax                          
mov es, ax
mov byte [diskNUM], dl
mov bx, KERNEL_LOCATION
jmp .load_kernel


.load_kernel:
    mov ah, 0x02 ; Fonction de lecture de secteur
    mov al, 10   ; Nombre de secteurs à lire
    mov ch, 0x00    ; Cylindre 0
    mov cl, 0x02    ; Secteur 9 (le premier secteur est le secteur
    mov dh, 0x00    ; Tête 0
    mov dl, [diskNUM] ; Numéro du disque

    ;stocker le nombre de secteurs a lire dans la ram pour la recuperer dans le kernel
    ;stocker a l adresse 0x00500, de 0x00500 a 0x00FFF c'est libre
    mov byte [0x500], al

    int 0x13 ; Appel de l'interruption pour lire le secteur
    mov si, diskerror ; Si l'erreur est présente, on saute à l'étiquette .error_disk
    jc .error_disk ; Si l'erreur est présente, on saute à l'étiquette .error_disk
    mov si, message ; Si l'erreur n'est pas présente, on saute à l'étiquette .print
    jmp .print

    

.error_disk:
    cmp byte [si], 0
    je .error_disk
    mov ah,0x0E
    mov al, [si]
    int 0x10
    inc si
    jmp .error_disk

.print:
    
    cmp byte [si], 0
    je .go_to_32bits
    mov ah,0x0E
    mov al, [si]
    int 0x10
    inc si
    jmp .print

.go_to_32bits:
    cli ; desactivation des iterruptions 


    lgdt [gdt_descriptor]                  ; Charge la GDT

    ;--- Passer en mode protégé ---
    ;en gros pour passer en mode protégé, il faut activer le bit 1 du registre CR0
    mov eax, cr0
    or eax, 0x1                            ; Activer le mode protégé
    mov cr0, eax
    jmp 0x08:.32bits


[BITS 32]
.32bits:
    
    mov ax, 0x10  ; sélecteur du data segment dans ta GDT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x80000		; 32 bit stack base pointer adresse de la stack (le debut) -> grandit vers le bas
	mov esp, ebp
    jmp KERNEL_LOCATION


.done:
    hlt ; Arrêter l'exécution
    jmp .done ; Boucle infinie pour éviter de sortir du programme


; ---------------------
; GDT : placer après le code (ou dans un segment dédié)
; ---------------------
;Totalement copié collé de github copilot (merci mon gars)
gdt_start:
    dq 0x0000000000000000        ; Null descriptor
    dq 0x00CF9A000000FFFF        ; Code segment (base=0, limit=4GB, exec/read)
    dq 0x00CF92000000FFFF        ; Data segment (base=0, limit=4GB, read/write)
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1   ; Limit (taille - 1)
    dd gdt_start                 ; Adresse base

diskNUM: db 0

message:
    db "                               Booting in OScour",0x0D, 0x0A, 0
diskerror:
    db "Disk error!",0x0D, 0x0A, 0

times 510 - ($ - $$) db 0
dw 0xAA55 ; signature boot
