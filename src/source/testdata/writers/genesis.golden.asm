; demo.asm -- written by CodeBreaker from demo.md
; "TEST CARTRIDGE" / "TEST CARTRIDGE", GM 00000000-00, 4 KB ROM

VDP_CTRL         equ $C00004

        org $0
        dc.l $00FF0000  ; initial stack pointer
        dc.l reset  ; reset
        dc.l $00000000  ; bus_error
        dc.l $00000000  ; address_error
        dc.l $00000000  ; illegal_instruction
        dc.l $00000000  ; zero_divide
        dc.l $00000000  ; chk
        dc.l $00000000  ; trapv
        dc.l $00000000  ; privilege
        dc.l $00000000  ; trace
        dc.l $00000000  ; line_a
        dc.l $00000000  ; line_f
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000  ; spurious
        dc.l $00000000  ; level1
        dc.l $00000000  ; external
        dc.l $00000000  ; level3
        dc.l $00000000  ; hblank
        dc.l $00000000  ; level5
        dc.l vblank  ; vblank
        dc.l $00000000  ; level7
        dc.l $00000000  ; trap0
        dc.l $00000000  ; trap1
        dc.l $00000000  ; trap2
        dc.l $00000000  ; trap3
        dc.l $00000000  ; trap4
        dc.l $00000000  ; trap5
        dc.l $00000000  ; trap6
        dc.l $00000000  ; trap7
        dc.l $00000000  ; trap8
        dc.l $00000000  ; trap9
        dc.l $00000000  ; trap10
        dc.l $00000000  ; trap11
        dc.l $00000000  ; trap12
        dc.l $00000000  ; trap13
        dc.l $00000000  ; trap14
        dc.l $00000000  ; trap15
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.b "SEGA MEGA DRIVE "  ; system
        dc.b "(C)TEST 2026.NOV"  ; copyright
        dc.b "TEST CARTRIDGE                                  "  ; domestic name
        dc.b "TEST CARTRIDGE                                  "  ; overseas name
        dc.b "GM 00000000-00"  ; serial and version
        dc.b $12,$34  ; checksum
        dc.b "J               "  ; devices
        dc.b $00,$00,$00,$00,$00,$00,$0F,$FF  ; ROM start and end
        dc.b $00,$FF,$00,$00,$00,$FF,$FF,$FF  ; RAM start and end
        dc.b "            "  ; backup RAM
        dc.b "            "  ; modem
        dc.b "                                        "  ; notes
        dc.b "JUE             "  ; regions

reset:
        MOVE.W #$80,(VDP_CTRL).L
        JSR (sub_000220).L
L_00020E:
        BRA.S L_00020E
        dcb.b 16,$FF

sub_000220:
        MOVEA.L #D_000400,A0
        RTS
        dcb.b 216,$FF

vblank:
        RTE
        dcb.b 254,$FF
D_000400:
        dc.b "SCORE LEVEL LIVE"
        dc.b $53
        dcb.b 3055,$FF
