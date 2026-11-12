; strcpy(to, from), strcat(to, from): a string, with its 0, copied; to is the answer

_TEXT	SEGMENT	PARA PUBLIC 'CODE'
	ASSUME	CS:_TEXT
	EXTRN	cret:NEAR, _strlen:NEAR
	PUBLIC	_strcpy, _strcat
_strcpy	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	PUSH	DI
	PUSH	SI
	MOV	SI,[BP+6]
	MOV	DI,[BP+4]
	MOV	DX,DI
	CLD
	XOR	AX,AX
CPY1:	MOV	AL,[SI]
	MOVSB
	OR	AX,AX
	JNZ	CPY1
	MOV	AX,DX
	JMP	cret
_strcpy	ENDP

_strcat	PROC	NEAR
	PUSH	BP
	MOV	BP,SP
	PUSH	DI
	PUSH	SI
	MOV	DI,[BP+4]
	MOV	DX,DI
	PUSH	DI
	CALL	_strlen
	POP	BX
	ADD	DI,AX			; the end of what is there
	PUSH	[BP+6]
	PUSH	DI
	CALL	_strcpy
	ADD	SP,4
	MOV	AX,DX
	JMP	cret
_strcat	ENDP
_TEXT	ENDS
	END
