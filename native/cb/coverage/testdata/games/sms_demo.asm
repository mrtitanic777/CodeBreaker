; A small Master System program written by hand for tests/test_z80.py (WLA-DX syntax).
.MEMORYMAP
SLOTSIZE $4000
SLOT 0 $0000
SLOT 1 $4000
SLOT 2 $8000
.ENDME
.ROMBANKMAP
BANKSTOTAL 4
BANKSIZE $4000
BANKS 4
.ENDRO
.BANK 0 SLOT 0
.ORG $0000
start:  di
        im 1
        ld sp,$DFF0
        jp main
.ORG $0038
        push af
        in a,($BF)
        call vbl
        pop af
        ei
        reti
.ORG $0066
        retn
.ORG $0100
main:   ld a,3
        ld ($FFFF),a
        ld hl,table
        ld b,4
loop:   ld a,(hl)
        out ($BE),a
        inc hl
        djnz loop
        call fill
        ei
idle:   halt
        jr idle
vbl:    ld a,($C000)
        inc a
        ld ($C000),a
        ret
fill:   ld ix,$C100
        ld (ix+3),$55
        set 2,(ix+3)
        ld hl,$C200
        ld de,$C201
        ld bc,15
        ld (hl),0
        ldir
        ret
table:  .db 1,2,3,4
.ORG $7FF0
        .db "TMR SEGA",0,0,$12,$34,$26,$70,$00,$4C
