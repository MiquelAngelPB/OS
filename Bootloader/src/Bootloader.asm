;Resources:
;Registers: https://www.eecg.utoronto.ca/~amza/www.mindsec.com/files/x86regs.html 
;x86 instruction set: https://www.felixcloutier.com/x86/
;ASCII codes: https://www.ascii-codes.com/ 
;BIOS interrupt 13h: https://es.wikipedia.org/wiki/Int_13h;
;Global Descriptor Table: https://www.youtube.com/watch?v=Wh5nPn2U_1w&list=PLm3B56ql_akNcvH8vvJRYOc7TbYhRs19M&index=6

[ORG 0x7c00]
[bits 16]

KERNEL_SEGMENT  equ 0x1000
KERNEL_OFFSET   equ 0x0000
KERNEL_PHYSICAL equ (KERNEL_SEGMENT << 4) + KERNEL_OFFSET
;KERNEL_SECTORS  defined in makefile
KERNEL_START    dq 1

start:
    mov [bootdrive], dl

    mov bx, bootmsg
    call printstr

    jmp initializestack

initializestack:
    mov ax, 0x0000
    mov ss, ax
    mov sp, 0x7BFF          ;just before the bootloader (counts down for some reason)
    jmp loadkernel

loadkernel:
    ;Check for extensions
    mov ah, 0x41
    mov bx, 0x55aa
    mov dl, [bootdrive]
    int 0x13
    
    jc extensionerr
    cmp bx, 0xaa55
    jne extensionerr

    ;upon succes, load data
    mov bx, extmsg
    call printstr

    call .preparedap
    call .loadkernel_lba

    jc diskerror   

    ;data is suposed to be loaded, tell the user
    push bx
    mov bx, kernelmsg       ;display that the kernel has been loaded into memory
    call printstr
    pop bx

    mov al, 0x13
    int 0x10

    jmp protectedmode

.preparedap:
    mov ax, [KERNEL_START]
    mov [dap + 8], ax
    mov ax, [KERNEL_START + 2]
    mov [dap + 10], ax
    mov ax, [KERNEL_START + 4]
    mov [dap + 12], ax
    mov ax, [KERNEL_START + 6]
    mov [dap + 14], ax
    ret

.loadkernel_lba:
    mov ax, KERNEL_SEGMENT
    mov es, ax
    mov bx, KERNEL_OFFSET

    mov dl, [bootdrive]
    mov si, dap ; DS:SI --> DAP adress (DS is alredy set by the BIOS at the start)

    mov ah, 0x42
    int 0x13

    ret

protectedmode:    
    cli

    lgdt [GDT_Descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SEGMENT:startprotectedmode

diskerror:
    mov bx, errmsg
    call printstr

    jmp halt

extensionerr:
    mov bx, errmsg2
    call printstr

    jmp halt

halt: 
    jmp $ ;do nothing for eternity and beyond

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
    db "Bootloader running...", 0x0D, 0x0A, 0
kernelmsg:
    db "Kernel loaded into memory...", 0x0D, 0x0A, 0
extmsg:
    db "Extensions are supported! ", 0x0D, 0x0A, 0
errmsg:
    db "Disk error! ", 0x0D, 0x0A, 0
errmsg2:
    db "Extensions are not supported! ", 0x0D, 0x0A, 0

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
    ;setup memory segments and stack
    mov ax, DATA_SEGMENT
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp

    mov eax, KERNEL_PHYSICAL
    jmp eax ;CODE_SEGMENT:KERNEL_PHYSICAL   ;jump to kernel

dap: ;Disk Adress Packet
    db 0x10             ;DAP size
    db 0x00             ;whatever this is
    dw KERNEL_SECTORS   ;KERNEL_SECTORS is defined in the makefile, VSCode might complain
    dw KERNEL_OFFSET
    dw KERNEL_SEGMENT
    dq 0                ;Initial LBA

;Padding and signature
times 510-($-$$) db 0
db 0x55, 0xaa