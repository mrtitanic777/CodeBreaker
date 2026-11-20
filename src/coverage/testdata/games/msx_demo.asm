; A small MSX cartridge written by hand for tests/test_z80.py (sjasmplus syntax).
        ORG $4000
        DEFB "AB"
        DEFW init,0,0,0
        DEFS 6,0
init:   CALL $006F
        LD HL,text
        LD DE,$1800
        LD BC,5
        CALL $005C
        LD A,$0F
        OUT ($99),A
        LD A,$87
        OUT ($99),A
loop:   CALL tick
        JR loop
tick:   LD HL,($FC9E)
        INC HL
        LD ($C000),HL
        RET
text:   DEFB "HELLO"
        DEFS 16384-($-$4000),$FF
