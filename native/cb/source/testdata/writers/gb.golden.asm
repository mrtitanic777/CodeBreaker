; demo.asm -- written by CodeBreaker from demo.gb
; "DEMO", ROM, 32 KB ROM

DEF rROMB0   EQU $2000
DEF rLCDC    EQU $FF40
DEF rLY      EQU $FF44

SECTION "bank0", ROM0[$0000]
        ds 64, $FF

vblank:
        RETI
        ds 191, $FF

entry:
        NOP
        JP L_0150
        db $CE,$ED,$66,$66,$CC,$0D,$00,$0B,$03,$73,$00,$83,$00,$0C,$00,$0D  ; Nintendo's logo (the boot ROM checks it)
        db $00,$08,$11,$1F,$88,$89,$00,$0E,$DC,$CC,$6E,$E6,$DD,$DD,$D9,$99
        db $BB,$BB,$67,$63,$6E,$0E,$EC,$CC,$DD,$DC,$99,$9F,$BB,$B9,$33,$3E
        db "DEMO",$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00  ; title
        db $00  ; Color support
        db $00,$00  ; licensee
        db $00  ; Super Game Boy support
        db $00  ; cartridge type
        db $00  ; ROM size
        db $00  ; RAM size
        db $00  ; destination
        db $00  ; old licensee
        db $00  ; version
        db $00  ; header checksum
        db $00,$00  ; global checksum
L_0150:
        CALL sub_0170
L_0153:
        LDH A,(rLY)
        CP $90
        JR NZ,L_0153
        LD HL,D_0180
        LD (rROMB0),A
L_015F:
        JP L_015F
        db $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF

sub_0170:
        LD A,$91
        LDH (rLCDC),A
        RET
        db $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
D_0180:
        db "TILES AND MAPS"
        ds 15986, $FF

SECTION "bank1", ROMX[$4000], BANK[1]
        ds 16384, $FF

