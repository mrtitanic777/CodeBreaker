; A small ZX Spectrum program written by hand for tests/test_z80.py (sjasmplus syntax).
        ORG $8000
start:  LD A,2
        CALL $1601
        LD HL,msg
        CALL show
        LD A,4
        OUT ($FE),A
        LD HL,$5C78
        LD A,(HL)
        LD ($C000),A
        RET
show:   LD A,(HL)
        OR A
        RET Z
        RST $10
        INC HL
        JR show
msg:    DEFB "HELLO",13,0
