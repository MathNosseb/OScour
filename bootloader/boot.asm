[org 0x7c00]
KERNEL_LOCATION equ 0x1000

mov [BOOT_DISK], dl    

mov si, message1          ; Pointeur vers le message1
call print_string        ; Appeler la fonction pour imprimer le message1
mov si, message2
call print_string 
mov si, message3 
call print_string 
mov si, message4 
call print_string
mov si, message5 
call print_string 
mov si, message6 
call print_string
mov si, message7
call print_string 

call wait_keypress       ; Attendre qu'une touche soit pressée

xor ax, ax                          
mov es, ax
mov ds, ax
mov bp, 0x8000
mov sp, bp

mov bx, KERNEL_LOCATION
mov dh, 2

mov ah, 0x02
mov al, dh 
mov ch, 0x00
mov dh, 0x00
mov cl, 0x02
mov dl, [BOOT_DISK]
int 0x13                ; Pas de gestion d'erreur, à toi de le faire !

mov ah, 0x0
mov al, 0x3
int 0x10                ; Mode texte

; Attendre qu'une touche soit pressée
mov ah, 0x00
int 0x16

CODE_SEG equ GDT_code - GDT_start
DATA_SEG equ GDT_data - GDT_start

cli
lgdt [GDT_descriptor]
mov eax, cr0
or eax, 1
mov cr0, eax
jmp CODE_SEG:start_protected_mode

jmp $

BOOT_DISK: db 0

GDT_start:
    GDT_null:
        dd 0x0
        dd 0x0

    GDT_code:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10011010
        db 0b11001111
        db 0x0

    GDT_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

GDT_end:

GDT_descriptor:
    dw GDT_end - GDT_start - 1
    dd GDT_start

; Définir le message1 à afficher
message1 db '/ _ \ ___ / ___|___  _   _ _ __', 13, 10, 0
message2 db '| | | / __| |   / _ \| | | | __|', 13, 10, 0
message3 db '| |_| \__ \ |__| (_) | |_| | |', 13, 10, 0
message4 db '\___/|___/\____\___/ \__,_|_|', 13, 10, 0

message5 db '', 13, 10, 0
message6 db 'OSCour dev By Matheo Besson v0.5', 13, 10, 0
message7 db 'Press any key for boot', 13, 10, 0

; Fonction pour imprimer une chaîne de caractères
print_string:
    pusha                   ; Sauvegarder tous les registres
.next_char:
    lodsb                  ; Charger le prochain caractère dans AL
    or al, al             ; Vérifier si c'est le caractère nul
    jz .done              ; Si c'est nul, terminer
    mov ah, 0x0E         ; Fonction pour écrire un caractère
    int 0x10             ; Appeler l'interruption BIOS
    jmp .next_char       ; Passer au caractère suivant
.done:
    popa                  ; Restaurer tous les registres
    ret

; Fonction pour attendre qu'une touche soit pressée
wait_keypress:
    mov ah, 0x00          ; Appeler l'interruption 0x16 pour attendre une touche
    int 0x16
    ret

[bits 32]
start_protected_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000        ; Pointeur de base pour la pile 32 bits
    mov esp, ebp

    jmp KERNEL_LOCATION





times 510-($-$$) db 0              
dw 0xaa55
