; MSX cartridge demo
; (written once in another assembler's syntax; this is the same program in the processor description's syntax, which the one assembler builds)
	.org 0x4000
	LD B,C
	LD B,D
	DJNZ 0x4044
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	CALL 0x6f
	LD HL,0x4034
	LD DE,0x1800
	LD BC,0x5
	CALL 0x5c
	LD A,0xf
	OUT (0x99),A
	LD A,0x87
	OUT (0x99),A
	CALL 0x402c
	JR 0x4027
	LD HL,(0xfc9e)
	INC HL
	LD (0xc000),HL
	RET
	LD C,B
	LD B,L
	LD C,H
	LD C,H
	LD C,A
        .org 0x4039
        .space 16327, 0xff
