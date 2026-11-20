; ported by CodeBreaker (port/asm16) for a 64-bit machine: the instructions as they were, the
; registers that hold addresses used whole, members of structures at their new places, segments gone,
; DOS called through the platform layer.
OPTION CASEMAP:NONE

EXTERN cret:PROC
EXTERN dos_nullseg:BYTE

.data

.code

; movbyte(a0, a1, a2) - the last argument pushed first, taken off after
PUBLIC movbyte
movbyte PROC
	push	rbx
	push	rsi
	push	rdi
	push	rbp
	push	r15
	test	r8, r8
	jnz	@@null_movbyte_2
	lea	r8, dos_nullseg
@@null_movbyte_2:
	push	r8
	test	rdx, rdx
	jnz	@@null_movbyte_1
	lea	rdx, dos_nullseg
@@null_movbyte_1:
	push	rdx
	push	rcx
	call	_movbyte
	add	rsp, 24
	pop	r15
	pop	rbp
	pop	rdi
	pop	rsi
	pop	rbx
	cld
	ret
movbyte ENDP
PUBLIC _movbyte
@base__TEXT:			; (the start of the segment _TEXT: what its tables count from)
_movbyte:
	push	rbp
	mov	rbp, rsp
	push	rdi
	push	rsi
	; PUSH	ES		(a segment: one flat memory)
	mov	cx, word ptr [rbp+16]
	mov	rsi, qword ptr [rbp+24]
	mov	rdi, qword ptr [rbp+32]
	; PUSH	DS		(a segment: one flat memory)
	; POP	ES		(a segment: one flat memory)
	cld
	movzx	ecx, cx
	rep movsb
	; POP	ES		(a segment: one flat memory)
	jmp	cret

END
