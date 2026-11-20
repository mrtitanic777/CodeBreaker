; ported by CodeBreaker (port/asm16) for a 64-bit machine: the instructions as they were, the
; registers that hold addresses used whole, members of structures at their new places, segments gone,
; DOS called through the platform layer.
OPTION CASEMAP:NONE


.data

.code

; p2() - the last argument pushed first, taken off after
PUBLIC p2
p2 PROC
	push	rbx
	push	rsi
	push	rdi
	push	rbp
	push	r15
	call	_p2
	pop	r15
	pop	rbp
	pop	rdi
	pop	rsi
	pop	rbx
	cld
	ret
p2 ENDP

; p3() - the last argument pushed first, taken off after
PUBLIC p3
p3 PROC
	push	rbx
	push	rsi
	push	rdi
	push	rbp
	push	r15
	call	_p3
	pop	r15
	pop	rbp
	pop	rdi
	pop	rsi
	pop	rbx
	cld
	ret
p3 ENDP
PUBLIC _p2
@base__TEXT:			; (the start of the segment _TEXT: what its tables count from)
_p2:
	mov	ax, 1
@d_0021:
	DB	74h
@d_0031:
	DB	0f0h
@d_0041:
	DB	8bh
@d_0051:
	DB	1eh
@d_0061:
	DW	1234h
	call	_p3
	ret
PUBLIC _p3
_p3:
	mov	ax, word ptr [rbp+4]
	ret

END
