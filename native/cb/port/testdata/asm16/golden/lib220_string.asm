; ported by CodeBreaker (port/asm16) for a 64-bit machine: the instructions as they were, the
; registers that hold addresses used whole, members of structures at their new places, segments gone,
; DOS called through the platform layer.
OPTION CASEMAP:NONE

EXTERN cret:PROC
EXTERN _strlen:PROC
EXTERN dos_nullseg:BYTE

.data

.code

; strcpy(a0, a1) - the last argument pushed first, taken off after
PUBLIC strcpy
strcpy PROC
	push	rbx
	push	rsi
	push	rdi
	push	rbp
	push	r15
	test	rdx, rdx
	jnz	@@null_strcpy_1
	lea	rdx, dos_nullseg
@@null_strcpy_1:
	push	rdx
	test	rcx, rcx
	jnz	@@null_strcpy_0
	lea	rcx, dos_nullseg
@@null_strcpy_0:
	push	rcx
	call	_strcpy
	add	rsp, 16
	pop	r15
	pop	rbp
	pop	rdi
	pop	rsi
	pop	rbx
	cld
	ret
strcpy ENDP

; strcat(a0, a1) - the last argument pushed first, taken off after
PUBLIC strcat
strcat PROC
	push	rbx
	push	rsi
	push	rdi
	push	rbp
	push	r15
	push	rdx
	push	rcx
	call	_strcat
	add	rsp, 16
	pop	r15
	pop	rbp
	pop	rdi
	pop	rsi
	pop	rbx
	cld
	ret
strcat ENDP
PUBLIC _strcpy
@base__TEXT:			; (the start of the segment _TEXT: what its tables count from)
_strcpy:
	push	rbp
	mov	rbp, rsp
	push	rdi
	push	rsi
	mov	rsi, qword ptr [rbp+24]
	mov	rdi, qword ptr [rbp+16]
	mov	rdx, rdi
	cld
	xor	eax, eax
CPY1:
	mov	al, byte ptr [rsi]
	movsb
	or	ax, ax
	jnz	CPY1
	mov	rax, rdx
	jmp	cret
PUBLIC _strcat
_strcat:
	push	rbp
	mov	rbp, rsp
	push	rdi
	push	rsi
	mov	rdi, qword ptr [rbp+16]
	mov	rdx, rdi
	push	rdi
	call	_strlen
	pop	rbx
	add	rdi, rax
	push	qword ptr [rbp+24]
	push	rdi
	call	_strcpy
	add	rsp, 16
	mov	ax, dx
	jmp	cret

END
