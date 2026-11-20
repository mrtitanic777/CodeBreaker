; A small Amstrad CPC program written by hand for the engine.lane_kit.machines tests (sjasmplus syntax); the test wraps it in an AMSDOS file.
        ORG $4000
start:  LD BC,$7F10
        OUT (C),C
        LD A,$54
        OUT (C),A
        CALL $BB06
        CALL work
        CALL mix
        JR start
work:   LD HL,$4100
        LD A,(HL)
        INC A
        LD (HL),A
        CALL $BB5A
        RET
mix:    LD A,B
        ADD A,C
        LD ($4110),A
        LD A,D
        XOR E
        LD BC,$BC00
        OUT (C),A
        LD HL,($4110)
        ADD HL,DE
        RET
