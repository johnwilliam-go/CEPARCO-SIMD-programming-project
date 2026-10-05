bits 64
default rel

section .data
	increment dq 4.25
section .text
	global noSIMDFunc
noSIMDFunc:
	xor edi, edi
L1:
	movsd xmm0, [rdx + rdi]
	vaddsd xmm0, xmm0, [increment]
	movsd [rdx + rdi], xmm0

	add rdi, 8
	dec rcx ; assume n > 0
	jnz L1

	ret