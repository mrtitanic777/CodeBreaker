; Amstrad CPC demo
; (written once in another assembler's syntax; this is the same program in the processor description's syntax, which the one assembler builds)
	.org 0x4000
	LD BC,0x7f10
	OUT (C),C
	LD A,0x54
	OUT (C),A
	CALL 0xbb06
	CALL 0x4014
	CALL 0x401e
	JR 0x4000
	LD HL,0x4100
	LD A,(HL)
	INC A
	LD (HL),A
	CALL 0xbb5a
	RET
	LD A,B
	ADD A, C
	LD (0x4110),A
	LD A,D
	XOR E
	LD BC,0xbc00
	OUT (C),A
	LD HL,(0x4110)
	ADD HL,DE
	RET
