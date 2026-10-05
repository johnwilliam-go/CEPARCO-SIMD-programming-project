bits 64
default rel

section .data
	increment dq 4.25
section .text
	global ymmFunc
ymmFunc:
	vbroadcastsd ymm1, qword [increment]
	xor edi, edi
	sar rcx, 2 ; process 4 double-precision floats at a time
L1:
	vmovupd ymm0, [rdx + rdi]
	vaddpd ymm0, ymm0, ymm1
	vmovupd [rdx + rdi], ymm0

	add rdi, 8*4 ; move to the next 4 double-precision floats
	dec rcx ; assume n > 0
	jnz L1

	vzeroupper
	ret