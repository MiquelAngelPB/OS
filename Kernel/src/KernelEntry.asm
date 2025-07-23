[ORG 0x1000]
[bits 32]

start:

    mov al, 'K'
    mov ah, 0x0f
    mov [0xb8000], ax
    jmp halt

halt:
    jmp halt

printstr:
    pusha               
.printloop:
    mov al, [bx]
    cmp al, 0
    je .end
    mov ah, 0x0e
    int 0x10
    inc bx
    jmp .printloop
.end:
    popa
    ret

string:
    db "Hello from the kernel!", 0x0D, 0x0A, 0

;Padding: the compiler executes this, not executed at runtime
times 512-($-$$) db 0