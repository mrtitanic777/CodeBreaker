; ported by CodeBreaker (port/asm16) for a 64-bit machine: the instructions as they were, the
; registers that hold addresses used whole, members of structures at their new places, segments gone,
; DOS called through the platform layer.
OPTION CASEMAP:NONE

EXTERN cret:PROC
EXTERN dos_nullseg:BYTE

.data

.code

; strlen(a0) - the last argument pushed first, taken off after
PUBLIC strlen
strlen PROC
	push	rbx
	push	rsi
	push	rdi
	push	rbp
	push	r15
	test	rcx, rcx
	jnz	@@null_strlen_0
	lea	rcx, dos_nullseg
@@null_strlen_0:
	push	rcx
	call	_strlen
	add	rsp, 8
	pop	r15
	pop	rbp
	pop	rdi
	pop	rsi
	pop	rbx
	cld
	ret
strlen ENDP
PUBLIC _strlen
@base__TEXT:			; (the start of the segment _TEXT: what its tables count from)
_strlen:
	push	rbp
	mov	rbp, rsp
	push	rdi
	push	rsi
	; PUSH	ES		(a segment: one flat memory)
	; PUSH	DS		(a segment: one flat memory)
	; POP	ES		(a segment: one flat memory)
	mov	rdi, qword ptr [rbp+16]
	mov	rsi, rdi
	xor	eax, eax
	mov	cx, ax
	not	cx
	cld
	movzx	ecx, cx
	repne scasb
	sub	rdi, rsi
	mov	rax, rdi
	dec	rax
	; POP	ES		(a segment: one flat memory)
	jmp	cret

END
