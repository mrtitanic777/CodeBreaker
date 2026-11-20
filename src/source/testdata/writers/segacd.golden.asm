; demo.asm -- written by CodeBreaker from demo.iso
; Sega CD "TEST DISC" / "TEST DISC", GM 00000000-00: the system area (32 KB) of a 2048-byte-sector image

CDBIOS             equ $005F22
GA_MEMMODE_LO      equ $A12003
GA_COMFLAG_SUB     equ $A1200F

        org $0   ; the boot header
        dc.b "SEGADISCSYSTEM  "  ; disc ID
        dc.b "TESTCDGAME "  ; volume name
        dc.b $00
        dc.w $0100  ; volume version
        dc.w $0001  ; volume type
        dc.b "TESTCDGAME "  ; system name
        dc.b $00,$00,$00,$00,$00
        dc.l $00000800  ; IP: start on the disc
        dc.l $00000200  ; IP: size
        dc.l $00000000  ; IP: entry offset
        dc.l $00000000  ; IP: work RAM size
        dc.l $00001000  ; SP: start on the disc
        dc.l $00000400  ; SP: size
        dc.l $00000000  ; SP: entry offset
        dc.l $00000000  ; SP: work RAM size
        dcb.b 176,$00
        dc.b "SEGA MEGA DRIVE "  ; system type
        dc.b "(C)TEST 2026.NOV"  ; copyright
        dc.b "TEST DISC                                       "  ; domestic name
        dc.b "TEST DISC                                       "  ; overseas name
        dc.b "GM 00000000-00"  ; serial and version
        dc.b $00,$00
        dc.b "J               "  ; devices
        dcb.b 80,$00
        dc.b "E               "  ; regions

        org $FF0000   ; the main CPU's program: the security code and the IP, loaded at $FF0000

security:
        BRA.W ip_start
        dcb.b 1532,$00

ip_start:
        MOVE.B #$1,(GA_MEMMODE_LO).L
        BSR.S sub_FF0610
        JMP ($00200000).L

sub_FF0610:
        TST.B (GA_COMFLAG_SUB).L
        BNE.S sub_FF0610
        RTS
        dcb.b 486,$00

        org $100000   ; bytes of the system area no program runs
        dcb.b 1536,$00

        org $6000   ; the sub CPU's program (the SP), loaded at $6000 of PRG-RAM
        dc.b "MAIN-SUBCPU"
        dcb.b 16,$00
        dc.b $20,$00,$00,$00,$00,$00,$08,$00,$12,$00,$14,$00,$00

sp_init:
        MOVEQ #1,D0
        JSR (CDBIOS).L
        RTS

sp_main:
        BRA.S sp_main

sp_int2:
        RTE
        dcb.b 970,$00

        org $110000   ; bytes of the system area no program runs
        dcb.b 27648,$00

