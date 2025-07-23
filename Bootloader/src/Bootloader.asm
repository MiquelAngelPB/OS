;Resources:
;Registers: https://www.eecg.utoronto.ca/~amza/www.mindsec.com/files/x86regs.html 
;x86 instruction set: https://www.felixcloutier.com/x86/
;ASCII codes: https://www.ascii-codes.com/ 
;BIOS interrupt 13h: https://es.wikipedia.org/wiki/Int_13h;
;Global Descriptor Table: https://www.youtube.com/watch?v=Wh5nPn2U_1w&list=PLm3B56ql_akNcvH8vvJRYOc7TbYhRs19M&index=6

[ORG 0x7c00]
[bits 16]

KERNEL_ADDRESS equ 0x1000

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
    mov bx, KERNEL_ADDRESS  ;destination es:bx
    push ax
    mov ax, 0x0000  ;temporal value for es
    mov es, ax      ;destination es:bx
    pop ax
    int 0x13

    jc diskerror         ;jump if carry flag is set (if there is a disk error)

    push bx
    mov bx, kernelmsg    ;display that the kernel has been loaded into memory
    call printstr
    pop bx

    jmp protectedmode
    jmp halt            ;do nothing for eternity and beyond

protectedmode:
    cli

    lgdt [GDT_Descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SEGMENT:startprotectedmode

diskerror:  ;display disk error message
    mov bx, errmsg
    call printstr

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

;Global Descriptor Table
GDT_Start:
    null_descriptor:
        dd 0
        dd 0
    code_descriptor:
        dw 0xffff
        dw 0
        db 0
        db 0b10011010
        db 0b11001111
        db 0
    data_descriptor:
        dw 0xffff
        dw 0
        db 0
        db 0b10010010
        db 0b11001111
        db 0
GDT_End:

GDT_Descriptor:
    dw GDT_End - GDT_Start - 1
    dd GDT_Start

;constants
CODE_SEGMENT equ code_descriptor - GDT_Start
DATA_SEGMENT equ data_descriptor - GDT_Start

[bits 32]
startprotectedmode:
    mov ax, DATA_SEGMENT
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp

    mov al, 'A'
    mov ah, 0x0f
    mov [0xb8000], ax
    jmp $
    jmp KERNEL_ADDRESS   ;jump to kernel

;Padding and signature: the compiler executes this, not executed at runtime
times 510-($-$$) db 0
db 0x55, 0xaa