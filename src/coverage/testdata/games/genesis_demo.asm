; A small Genesis program written by hand for tests/test_m68k.py (Motorola syntax).
        org $0
        dc.l $00FFFE00, start
        dcb.l 22,$00000200
        dc.l $00000200, $00000200, $00000200, $00000200, hblank, $00000200, vblank, $00000200
        dcb.l 32,$00000200
        org $100
        dc.b "SEGA MEGA DRIVE "
        dc.b "(C)TEST 2026.OCT"
        dc.b "DEMO                                            "
        dc.b "DEMO                                            "
        dc.b "GM 00000000-00"
        dc.w $0000
        dc.b "J               "
        dc.l $00000000, $0001FFFF, $00FF0000, $00FFFFFF
        dcb.b 52,$20
        dc.b "JUE             "
        org $200
spurious:
        RTE
start:
        MOVE.W #$2700,SR
        MOVE.B ($00A10001).L,D0
        ANDI.B #$0F,D0
        BEQ.S notmss
        MOVE.L #$53454741,($00A14000).L
notmss:
        LEA ($00C00004).L,A1
        LEA regs(PC),A0
        MOVEQ #2,D1
setreg:
        MOVE.W (A0)+,(A1)
        DBF D1,setreg
        BSR.W sum
        MOVE.W D0,($FFFF8000).W
        MOVE.W #$2000,SR
idle:
        BRA.S idle
sum:
        MOVEM.L D1/A0,-(SP)
        LEA table(PC),A0
        MOVEQ #0,D0
        MOVEQ #3,D1
.add:
        ADD.W (A0)+,D0
        DBF D1,.add
        MOVEM.L (SP)+,D1/A0
        RTS
vblank:
        ADDQ.W #1,($FFFF8002).W
        RTE
hblank:
        RTE
regs:
        dc.w $8004, $8174, $8F02
table:
        dc.w 10, 20, 30, 40
