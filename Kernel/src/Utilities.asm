global readKey
global halt

readKey:
    mov dx, 0x60
    in al, dx
    movzx eax, al
    ret

halt:
    hlt