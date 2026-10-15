bits 64
default rel

section .text
    global noSIMDFunc

noSIMDFunc:
    movsxd rcx, ecx
    mov r10, [rsp+40]
    xor rax, rax
    test rcx, rcx
    jle done
L1:
    movss xmm0, [r9 + rax*4]
    mulss xmm0, [r10 + rax*4]
    addss xmm0, [r8 + rax*4]
    movss [rdx + rax*4], xmm0 
    
    inc rax
    cmp rax, rcx
    jl L1
done:
    ret