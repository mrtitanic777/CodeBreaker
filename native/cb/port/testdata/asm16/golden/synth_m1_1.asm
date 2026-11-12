; ported by CodeBreaker (port/asm16) for a 64-bit machine: the instructions as they were, the
; registers that hold addresses used whole, members of structures at their new places, segments gone,
; DOS called through the platform layer.
OPTION CASEMAP:NONE

EXTERN bytev:BYTE
EXTERN gv:WORD
EXTERN tbl:WORD
EXTERN dos_int21:PROC
EXTERN arr:BYTE
EXTERN dos_nullseg:BYTE

.data
dosregs	QWORD	8 DUP (0)		; (the registers given to DOS and back)
_val	DW	0
_arr	DW	10 DUP (0)
_ptrs	DW	_val
_cb	DB	1, 2, 3
_lbl	LABEL	WORD
	DQ	val, arr

.code

; walk(a0, a1, a2, a3) - the last argument pushed first, taken off after
PUBLIC walk
walk PROC
	push	rbx
	push	rsi
	push	rdi
	push	rbp
	push	r15
	test	r8, r8
	jnz	@@null_walk_3
	lea	r8, dos_nullseg
@@null_walk_3:
	push	r8
	mov	r11, rdx
	shr	r11, 16
	push	r11
	test	rdx, rdx
	jnz	@@null_walk_1
	lea	rdx, dos_nullseg
@@null_walk_1:
	push	rdx
	push	rcx
	call	_walk
	add	rsp, 32
	pop	r15
	pop	rbp
	pop	rdi
	pop	rsi
	pop	rbx
	cld
	ret
walk ENDP

; sum(a0, a1, a2) - the last argument pushed first, taken off after
PUBLIC sum
sum PROC
	push	rbx
	push	rsi
	push	rdi
	push	rbp
	push	r15
	push	r8
	push	rdx
	push	rcx
	call	_sum
	add	rsp, 24
	pop	r15
	pop	rbp
	pop	rdi
	pop	rsi
	pop	rbx
	cld
	ret
sum ENDP

; lenp(a0) - the last argument pushed first, taken off after
PUBLIC lenp
lenp PROC
	push	rbx
	push	rsi
	push	rdi
	push	rbp
	push	r15
	push	rcx
	call	_lenp
	add	rsp, 8
	pop	r15
	pop	rbp
	pop	rdi
	pop	rsi
	pop	rbx
	cld
	ret
lenp ENDP

; strx(a0, a1) - the last argument pushed first, taken off after
PUBLIC strx
strx PROC
	push	rbx
	push	rsi
	push	rdi
	push	rbp
	push	r15
	push	rdx
	push	rcx
	call	@strx
	add	rsp, 16
	pop	r15
	pop	rbp
	pop	rdi
	pop	rsi
	pop	rbx
	cld
	ret
strx ENDP

; far1(a0) - the last argument pushed first, taken off after
PUBLIC far1
far1 PROC
	push	rbx
	push	rsi
	push	rdi
	push	rbp
	push	r15
	push	rcx
	call	_far1
	add	rsp, 8
	pop	r15
	pop	rbp
	pop	rdi
	pop	rsi
	pop	rbx
	cld
	ret
far1 ENDP

; dosit() - the last argument pushed first, taken off after
PUBLIC dosit
dosit PROC
	push	rbx
	push	rsi
	push	rdi
	push	rbp
	push	r15
	call	_dosit
	pop	r15
	pop	rbp
	pop	rdi
	pop	rsi
	pop	rbx
	cld
	ret
dosit ENDP
PUBLIC _walk
@base__TEXT:			; (the start of the segment _TEXT: what its tables count from)
_walk:
	push	rbp
	mov	rbp, rsp
	push	rsi
	push	rdi
	mov	rbx, qword ptr [rbp+16]
	mov	ax, word ptr [rbx+8]
	mov	rsi, qword ptr [rbx]
	mov	rdi, qword ptr [rbp+24]
	mov	cx, word ptr [rbp+32]
	mov	rbx, qword ptr [rbp+40]
	mov	rsi, qword ptr [rbp+40]
	mov	rax, rbx		; (the segment of the address in bx: the address, 0 when it is)
	mov	rdx, rsi		; (the segment of the address in si: the address, 0 when it is)
	or	rax, rax
	movsx	r11, cx
	add	rbx, r11
	sub	rbx, rdi
	inc	rbx
	dec	rsi
	cmp	bx, 0
	mov	ax, word ptr gv
	mov	ax, word ptr gv
	lea	rax, gv
	lea	rax, gv+2
	mov	word ptr gv, ax
	lea	r11, tbl
	mov	qword ptr gv, r11
	mov	word ptr gv, 0
	mov	word ptr gv, 5
	mov	al, byte ptr [rbx]
	mov	byte ptr [rbx], al
	mov	byte ptr [rbx+rsi], 3
	add	word ptr [rbx], 2
	sub	word ptr gv, 1
	inc	word ptr [rbx]
	dec	word ptr [rbx+2]
	shl	word ptr [rbx], 1
	shl	ax, cl
	mul	word ptr [rbx]
	div	cx
	imul	bx
	neg	ax
	not	word ptr [rbx]
	xchg	rax, rbx
	movzx	r11d, bx
	xchg	ax, word ptr [r11]
	movzx	r11d, bx
	lea	rax, [r11+4]
	lea	rdi, [rbp-16]
	lea	rsi, tbl
	push	rax
	movzx	r11d, bx
	movzx	r11d, word ptr [r11]
	push	r11
	; push    es		(a segment: one flat memory)
	; pop     es		(a segment: one flat memory)
	lea	r11, tbl
	push	r11
	push	5
	pop	rax
	pushfq
	popfq
	xor	eax, eax
	xor	ecx, ecx
	and	ax, ax
	test	ax, 1
	movzx	r11d, bx
	cmp	word ptr [r11], 0
	movzx	r11d, bx
	cmp	word ptr [r11], ax
	mov	word ptr [rbp-8], ax
	mov	rax, qword ptr [rbp-8]
	sub	rsp, 4
	add	rsp, 4
	mov	rsp, rbp
	mov	rbp, rsp
	mov	rbx, rsp
	mov	rax, qword ptr [rbx+16]
	lea	r10, tbl
	mov	ax, word ptr [r10+rbx]
	lea	r10, tbl
	mov	ax, word ptr [r10+rbx+16]
	mov	ax, word ptr bytev
	cld
	movzx	ecx, cx
	rep movsb
	movzx	ecx, cx
	repne scasb
	lodsw
	stosb
	cmpsw
	xlatb
	cbw
	cwd
	nop
	sti
	cli
	stc
	clc
	cmc
	std
	sahf
	lahf
	; mov     ax, SEG _gv		(a segment: one flat memory)
	; mov     ax, DGROUP		(a segment: one flat memory)
	; mov     ax, _TEXT		(a segment: one flat memory)
	; mov     es, ax		(a segment: one flat memory)
	; mov     ax, es		(a segment: one flat memory)
	mov	bx, 100h
	lea	rbx, arr		; (200h: the old address of arr)
	mov	ax, word ptr arr
	movzx	ecx, cx
	jrcxz	@@j99_t
	jmp	@@j99_n
	@@j99_t:
	jmp	done
	@@j99_n:
	movzx	ecx, cx
	loop	@@j100_t
	jmp	@@j100_n
	@@j100_t:
	jmp	done
	@@j100_n:
	movzx	ecx, cx
	loope	@@j101_t
	jmp	@@j101_n
	@@j101_t:
	jmp	done
	@@j101_n:
	je	done
	jmp	done
	; (never reached) call    done
	; (never reached) call    _cfun
	; (never reached) call    _other
	; (never reached) call    _malloc
	; (never reached) call    __ffree
	; (never reached) call    [bx]
	; (never reached) call    WORD PTR cs:[tb2+bx]
	; (never reached) int     21h
	; (never reached) int     10h
done:
	pop	rdi
	pop	rsi
	pop	rbp
	ret
	; (never reached) ret     4
PUBLIC _sum
_sum:
	push	rbp
	mov	rbp, rsp
	mov	rax, qword ptr [rbp+16]
	add	rax, qword ptr [rbp+24]
	add	rax, qword ptr [rbp+32]
	mov	rbx, rax
	mov	rdx, qword ptr [rbx]
	pop	rbp
	ret
PUBLIC _lenp
_lenp:
	mov	rbx, rsp
	mov	rsi, qword ptr [rbx+8]
	xor	eax, eax
lp:
	cmp	byte ptr [rsi], 0
	je	lq
	inc	rsi
	inc	ax
	jmp	lp
lq:
	ret
PUBLIC @strx
@strx:
	push	rbp
	mov	rbp, rsp
	mov	rdi, qword ptr [rbp+16]
	mov	rax, qword ptr [rbp+24]
	pop	rbp
	ret	16
PUBLIC _far1
_far1:
	mov	rbp, rsp
	mov	rax, qword ptr [rbp+8]
	ret
PUBLIC _dosit
_dosit:
	mov	ah, 9
	lea	rdx, msg
	mov	qword ptr dosregs, rax
	mov	qword ptr dosregs+8, rbx
	mov	qword ptr dosregs+16, rcx
	mov	qword ptr dosregs+24, rdx
	mov	qword ptr dosregs+32, rsi
	mov	qword ptr dosregs+40, rdi
	mov	r15, rsp		; (DOS: the platform layer, called as today's C is)
	and	rsp, -16
	sub	rsp, 32
	lea	rcx, dosregs
	call	dos_int21
	mov	rsp, r15
	mov	rax, qword ptr dosregs
	mov	rbx, qword ptr dosregs+8
	mov	rcx, qword ptr dosregs+16
	mov	rdx, qword ptr dosregs+24
	bt	qword ptr dosregs+48, 0	; (the carry: an error)
	mov	ax, 4c00h
	mov	qword ptr dosregs, rax
	mov	qword ptr dosregs+8, rbx
	mov	qword ptr dosregs+16, rcx
	mov	qword ptr dosregs+24, rdx
	mov	qword ptr dosregs+32, rsi
	mov	qword ptr dosregs+40, rdi
	mov	r15, rsp		; (DOS: the platform layer, called as today's C is)
	and	rsp, -16
	sub	rsp, 32
	lea	rcx, dosregs
	call	dos_int21
	mov	rsp, r15
	mov	rax, qword ptr dosregs
	mov	rbx, qword ptr dosregs+8
	mov	rcx, qword ptr dosregs+16
	mov	rdx, qword ptr dosregs+24
	bt	qword ptr dosregs+48, 0	; (the carry: an error)
	ret
@d_09c1:
	DW	_dosit, _lenp - @base__TEXT, 5
@d_09d1:
	DB	'hello$'
	; (never reached) DB      0CBh
	; (never reached) DB      0CAh

END
