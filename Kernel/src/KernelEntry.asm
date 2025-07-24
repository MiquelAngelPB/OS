[ORG 0x1000]
[bits 32]

start:

    mov al, 'K'
    mov ah, 0x0f
    mov [0xb8000], ax
    jmp halt

halt:
    jmp halt

;Padding: the compiler executes this, not executed at runtime
times 512-($-$$) db 0