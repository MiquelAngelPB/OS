[bits 32]

extern kernelmain
global start

start:
    mov ebp, 0x90000
    and ebp, 0xfffffff0
    mov esp, ebp
    call kernelmain ;VSCode might complain with a false error.
    jmp halt

halt:
    jmp halt

;Padding: the compiler executes this, not executed at runtime