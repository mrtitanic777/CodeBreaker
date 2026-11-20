; demo.asm -- written by CodeBreaker
; ZX Spectrum demo
; 64 bytes at $8000

ULA              EQU $FE
LAST_K           EQU $5C08

        ORG $8000

start:
        CALL sub_8010
        LD HL,D_8020
        OUT (ULA),A
        JP start
        DEFB $00,$00,$00,$00,$00

sub_8010:
        LD (LAST_K),A
        RET
        DEFB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
D_8020:
        DEFB "HELLO"
        DEFS 27,$00
