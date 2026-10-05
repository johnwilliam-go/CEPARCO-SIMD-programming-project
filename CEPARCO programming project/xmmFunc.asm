bits 64
default rel

section .data
	increment dq 4.25
section .text
	global xmmFunc
xmmFunc:
	vmovddup xmm1, [increment]
	xor edi, edi
	sar rcx, 1 ; process 2 double-precision floats at a time
L1:
	vmovupd xmm0, [rdx + rdi]
	vaddpd xmm0, xmm0, xmm1
	vmovupd [rdx + rdi], xmm0

	add rdi, 8*2 ; move to the next 2 double-precision floats
	dec rcx ; assume n > 0
	jnz L1

	vzeroupper
	ret