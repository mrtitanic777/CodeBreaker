; demo.asm -- written by CodeBreaker
; ZX Spectrum demo
; 64 bytes at $8000

.equ ULA, 0xfe
.equ LAST_K, 0x5c08

        .org 0x8000
; read ahead of their lines (behind a register the assembler's first pass takes them for unknown names)
.equ D_8020, 0x8020


start:
        CALL sub_8010
        LD HL,D_8020
        OUT (ULA),A
        JP start
        .byte 0x00,0x00,0x00,0x00,0x00

sub_8010:
        LD (LAST_K),A
        RET
        .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
D_8020:
        .ascii "HELLO"
        .space 27, 0x00
