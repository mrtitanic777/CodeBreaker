; ZX Spectrum demo
; (written once in another assembler's syntax; this is the same program in the processor description's syntax, which the one assembler builds)
	.org 0x8000
	LD A,0x2
	CALL 0x1601
	LD HL,0x801e
	CALL 0x8017
	LD A,0x4
	OUT (0xfe),A
	LD HL,0x5c78
	LD A,(HL)
	LD (0xc000),A
	RET
	LD A,(HL)
	OR A
	RET Z
	RST 0x10
	INC HL
	JR 0x8017
	LD C,B
	LD B,L
	LD C,H
	LD C,H
	LD C,A
	DEC C
	NOP
