; ColecoVision demo
; (written once in another assembler's syntax; this is the same program in the processor description's syntax, which the one assembler builds)
	.org 0x8000
	LD D,L
	XOR D
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	.byte 0x32
	ADD A, B
        .org 0x800c
        .space 21, 0x0
	.org 0x8021
	RETN
	NOP
	LD B,H
	LD B,L
	LD C,L
	LD C,A
	CPL
	LD D,H
	LD B,L
	LD D,E
	LD D,H
	CPL
	LD (0x3230),A
	LD (HL),0x31
	NOP
	LD (HL),H
	CALL 0x804c
	LD HL,0x8076
	LD DE,0x100
	LD BC,0x5
	CALL 0x1fdf
	CALL 0x8058
	CALL 0x8068
	JR 0x8044
	LD A,0xc2
	OUT (0xbf),A
	LD A,0x81
	OUT (0xbf),A
	CALL 0x1fd6
	RET
	IN A,(0xbf)
	LD (0x7000),A
	LD HL,(0x7020)
	INC HL
	LD (0x7020),HL
	CALL 0x1f79
	RET
	LD A,B
	ADD A, C
	LD (0x7030),A
	LD A,D
	XOR E
	OUT (0xff),A
	LD HL,(0x7030)
	ADD HL,DE
	RET
	LD C,B
	LD B,L
	LD C,H
	LD C,H
	LD C,A
