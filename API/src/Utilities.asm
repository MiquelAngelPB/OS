;cdecl Calling Convention: https://www.youtube.com/watch?v=frqPX7EHscM
global inpPort
global outPort
global halt

inpPort:
    push ebp
    mov ebp, esp
    mov dx, [ebp + 8]    ;16 bit port
    in al, dx
    movzx eax, al
    pop ebp
    ret

outPort:
    push ebp
    mov ebp, esp
    mov dx, [ebp + 8]   ;16 bit port
    mov al, [ebp + 12]  ;8 bit value
    out dx, al
    pop ebp
    ret

halt:
    hlt