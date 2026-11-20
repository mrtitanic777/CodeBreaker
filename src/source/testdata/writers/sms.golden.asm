; demo.asm -- written by CodeBreaker from demo.sms
; Master System, 32 KB ROM
; 2 pages of 16 KB: page 0 is seen at $0000, page 1 at $4000, every later page at $8000
; (Z80 syntax as the processor's SLEIGH description writes it: build with `assemble --cpu z80 --sleigh`)

.equ VDP_DATA, 0xbe
.equ VDP_CONTROL, 0xbf
.equ MAPPER_SLOT0, 0xfffd

; read ahead of their lines (behind a register the assembler's first pass takes them for unknown names)
.equ D_0120, 0x0120

; page 0 in slot 0
        .org 0x0000

reset:
        DI
        IM 1
        JP L_0080
        .space 50, 0xff

irq:
        EI
        RETI
        .space 43, 0xff

nmi:
        RETN
        .space 24, 0xff
L_0080:
        LD HL,0xc000
        LD A,0x1
        OUT (VDP_CONTROL),A
        CALL sub_0100
        LD (MAPPER_SLOT0),A
        LD HL,D_0120
L_0090:
        JP L_0090
        .space 109, 0xff

sub_0100:
        LD A,0x0
        OUT (VDP_DATA),A
        RET
        .space 27, 0xff
D_0120:
        .ascii "HELLO WORLD"
        .space 16085, 0xff

; page 1 in slot 1
        .org 0x4000
        .space 16384, 0xff

