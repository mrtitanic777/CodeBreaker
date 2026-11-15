; A small ColecoVision cartridge written by hand for the engine.lane_kit.machines tests (sjasmplus syntax).
        ORG $8000
        DEFB $55,$AA
        DEFW 0,0,0,0,start
        DEFS 21,0
nmi:    RETN
        DEFB 0
        DEFB "DEMO/TEST/2026"
start:  LD SP,$7400
        CALL setup
        LD HL,text
        LD DE,$0100
        LD BC,5
        CALL $1FDF
loop:   CALL frame
        CALL mix
        JR loop
setup:  LD A,$C2
        OUT ($BF),A
        LD A,$81
        OUT ($BF),A
        CALL $1FD6
        RET
frame:  IN A,($BF)
        LD ($7000),A
        LD HL,($7020)
        INC HL
        LD ($7020),HL
        CALL $1F79
        RET
mix:    LD A,B
        ADD A,C
        LD ($7030),A
        LD A,D
        XOR E
        OUT ($FF),A
        LD HL,($7030)
        ADD HL,DE
        RET
text:   DEFB "HELLO"
