bits 64
default rel
section .text

global ymmFunc

ymmFunc:
    movsxd rcx, ecx
    mov r10, [rsp+40]
    xor rax, rax
    mov r11, rcx
    and r11, -8
L1:
    cmp rax, r11
    jge last
    vmovups ymm0, [r8 + rax*4]
    vmovups ymm1, [r9 + rax*4]
    vmovups ymm2, [r10 + rax*4]
    vmulps ymm1, ymm1, ymm2
    vaddps ymm0, ymm0, ymm1
    vmovups [rdx + rax*4], ymm0
    add rax, 8
    jmp L1
last:
    cmp rax, rcx
    jge done
    vmovss xmm0, [r8 + rax*4]
    vmovss xmm1, [r9 + rax*4]
    vmovss xmm2, [r10 + rax*4]
    vmulss xmm1, xmm1, xmm2
    vaddss xmm0, xmm0, xmm1
    vmovss [rdx + rax*4], xmm0
    inc rax
    jmp last
done:
    ret