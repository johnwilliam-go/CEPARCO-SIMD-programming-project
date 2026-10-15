bits 64
default rel

section .data
    temp dd 0.0

section .text
    global noSIMDFunc

noSIMDFunc:
    movsxd rcx, ecx
    mov r10, [rsp+40]
    xor rax, rax
    test rcx, rcx
    jle done
L1:
    fld dword [r9 + rax*4]
    fmul dword [r10 + rax*4]
    fstp dword [temp]
    fld dword [temp]
    fadd dword [r8 + rax*4]
    fstp dword [rdx + rax*4]
    inc rax
    cmp rax, rcx
    jl L1
done:
    ret