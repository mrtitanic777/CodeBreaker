; demo.asm -- written by CodeBreaker from demo.sms
; Master System, 32 KB ROM

.MEMORYMAP
SLOTSIZE $4000
DEFAULTSLOT 2
SLOT 0 $0000
SLOT 1 $4000
SLOT 2 $8000
.ENDME

.ROMBANKMAP
BANKSTOTAL 2
BANKSIZE $4000
BANKS 2
.ENDRO

.DEFINE VDP_DATA         $BE
.DEFINE VDP_CONTROL      $BF
.DEFINE MAPPER_SLOT0     $FFFD

.BANK 0 SLOT 0
.ORG $0000

reset:
        DI
        IM 1
        JP L_0080
        .DSB 50,$FF

irq:
        EI
        RETI
        .DSB 43,$FF

nmi:
        RETN
        .DSB 24,$FF
L_0080:
        LD HL,$C000
        LD A,$01
        OUT (VDP_CONTROL),A
        CALL sub_0100
        LD (MAPPER_SLOT0),A
        LD HL,D_0120
L_0090:
        JP L_0090
        .DSB 109,$FF

sub_0100:
        LD A,$00
        OUT (VDP_DATA),A
        RET
        .DSB 27,$FF
D_0120:
        .DB "HELLO WORLD"
        .DSB 16085,$FF

.BANK 1 SLOT 1
.ORG $0000
        .DSB 16384,$FF

