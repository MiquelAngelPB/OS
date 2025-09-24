;cdecl Calling Convention: https://www.youtube.com/watch?v=frqPX7EHscM
global inpPort
global outPort
global readMem
global setMem
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

readMem:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]    ;16 bit address
    mov dl, [eax]
    movzx eax, dl
    pop ebp
    ret

setMem:
    push ebp
    mov ebp, esp
    mov eax, [ebp + 8]   ;16 bit address
    mov al, [ebp + 12]      ;8 bit value
    mov [eax], al
    pop ebp
    ret

halt:
    hlt