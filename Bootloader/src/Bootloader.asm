;Registers: https://www.eecg.utoronto.ca/~amza/www.mindsec.com/files/x86regs.html
[ORG 0x7c00]

section .text
    global start

start:
    mov [bootdrive], dl

    mov bx, bootmsg
    call printstr

    jmp initializestack

initializestack:
    mov ax, 0x0000
    mov ss, ax
    mov sp, 0x7BFF  ;just before the bootloader (counts down for some reason)
    jmp loadkernel

loadkernel:
    mov ah, 0x02    ;read (block writing permissions)
    mov al, 1       ;number of sectors (starts at 1)
    mov ch, 0       ;cylinder
    mov cl, 2       ;Sector inside the cylinder (starts at 1)
    mov dh, 0       ;head
    mov dl, [bootdrive] ;disk, will use the one that the BIOS gives you
    mov bx, 0x7e00  ;destination bx:es
    mov cx, 0x0000  ;temporal value for es
    mov es, cx      ;destination bx:es
    int 0x13

    jc diskerror         ;jump if carry flag is set (if there is a disk error)

    ; ================================================
    ; Debugging
    ; ================================================

    ;jmp 0x0000:0x1000   ;jump to kernel

    mov ah, 0x0e         
    mov bx, 0x7dfe

    ;ignore this one
    ;Write the int 13h error code. Check: https://www.ascii-codes.com/ and https://es.wikipedia.org/wiki/Int_13h
    ;mov al, bh
    ;int 0x10

    ;Display 0x55
    mov al, [es:bx]
    int 0x10

    ;Display 0xAA
    inc bx
    mov al, [es:bx]
    int 0x10

    ;Display a few characters from the 2nd sector (SHould be CCCCCCC...)
    inc bx
    mov al, [es:bx]
    int 0x10

    inc bx
    mov al, [es:bx]
    int 0x10

    inc bx
    mov al, [es:bx]
    int 0x10

    inc bx
    mov al, [es:bx]
    int 0x10

    inc bx
    mov al, [es:bx]
    int 0x10

    inc bx
    mov al, [es:bx]
    int 0x10

    inc bx
    mov al, [es:bx]
    int 0x10

    inc bx
    mov al, [es:bx]
    int 0x10

    mov bx, kernelmsg    ;display that the kernel has been loaded into memory
    call printstr

    jmp halt            ;do nothing for eternity and beyond

diskerror:              ;display disk error message
    mov bx, errmsg
    call printstr

    mov al, [0x7e00]
    int 0x10

    jmp halt

halt:
    jmp $

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

;Strings and data
bootdrive: 
    db 0
bootmsg:
    db "Bootloader loaded...", 0x0D, 0x0A, 0
kernelmsg:
    db "Kernel loaded into memory...", 0x0D, 0x0A, 0
errmsg:
    db "Disk error! ", 0x0D, 0x0A, 0
teststr:
    db "A", 0x0D, 0x0A, 0

;Padding and signature: the compiler executes this, not executed at runtime
times 510-($-$$) db 0
db 0x55, 0xaa