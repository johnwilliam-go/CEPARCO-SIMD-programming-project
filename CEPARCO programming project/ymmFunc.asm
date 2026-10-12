section .text

global ymmFunc

ymmFunc:
    mov r10, [rsp+40]
    xor rax, rax
L1:
    cmp rax, rcx
    jae done
    vmovups ymm0, [r8 + rax*4]
    vmovups ymm1, [r9 + rax*4]
    vmovups ymm2, [r10 + rax*4]

    vmulps ymm1, ymm1, ymm2
    vaddps ymm0, ymm0, ymm1
    vmovups [rdx + rax*4], ymm0

    add rax, 8
    jmp L1

done:
    ret