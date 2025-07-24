[bits 32]

extern kernelmain
global start

start:
    call kernelmain ;VSCode may complain with a false error.
    jmp halt

halt:
    jmp halt

;Padding: the compiler executes this, not executed at runtime
times 512-($-$$) db 0