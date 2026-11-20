; movbytes(count, from, to): count bytes moved, within the data segment

_TEXT	SEGMENT	PARA PUBLIC 'CODE'
	ASSUME	CS:_TEXT
	EXTRN	cret:NEAR
	PUBLIC	_movbyte
_movbyte	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	PUSH	DI
	PUSH	SI
	PUSH	ES
	MOV	CX,[BP+4]
	MOV	SI,[BP+6]
	MOV	DI,[BP+8]
	PUSH	DS
	POP	ES
	CLD
	REP	MOVSB
	POP	ES
	JMP	cret
_movbyte	ENDP
_TEXT	ENDS
	END
