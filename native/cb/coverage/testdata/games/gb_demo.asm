; A small Game Boy program written by hand for tests/test_z80.py (RGBDS syntax).
SECTION "rst", ROM0[$0040]
        jp vbl
SECTION "entry", ROM0[$0100]
        nop
        jp main
        db $CE,$ED,$66,$66,$CC,$0D,$00,$0B,$03,$73,$00,$83,$00,$0C,$00,$0D
        db $00,$08,$11,$1F,$88,$89,$00,$0E,$DC,$CC,$6E,$E6,$DD,$DD,$D9,$99
        db $BB,$BB,$67,$63,$6E,$0E,$EC,$CC,$DD,$DC,$99,$9F,$BB,$B9,$33,$3E
        db "TESTGAME",0,0,0,0,0,0,0
        db $00,$00,$00,$00,$01,$01,$00,$01,$33,$00,$00,$00,$00
main:   di
        ld sp,$FFFE
        ld a,$91
        ldh ($FF40),a
        ld a,2
        ld ($2000),a
        ld hl,table
        ld de,$C000
        ld b,4
copy:   ld a,(hl+)
        ld (de),a
        inc de
        dec b
        jr nz,copy
        call clear
        ei
wait:   halt
        jr wait
vbl:    push af
        ldh a,($FF44)
        ld ($C010),a
        pop af
        reti
clear:  ld hl,$C100
        xor a
        ld c,16
.l:     ld (hl+),a
        dec c
        jr nz,.l
        swap a
        ret
table:  db 9,8,7,6
