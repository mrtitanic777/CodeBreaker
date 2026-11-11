; A small NES program, written by hand for the NES lane's tests (tests/test_nes.py): a reset
; that waits for the picture unit, clears RAM and loads a palette; an NMI that reads the
; controller and counts frames; a multiply routine; a table read by index.

.segment "HEADER"
        .byte "NES", $1A, 1, 1, $01, $00, 0, 0, 0, 0, 0, 0, 0, 0

.segment "BANK0"
reset:
        SEI
        CLD
        LDX #$FF
        TXS
        INX
        STX $2000
        STX $2001
vblank1:
        BIT $2002
        BPL vblank1
clear:
        LDA #$00
        STA $00,x
        STA $0200,x
        STA $0300,x
        INX
        BNE clear
vblank2:
        BIT $2002
        BPL vblank2
        LDA #$3F
        STA $2006
        LDA #$00
        STA $2006
        LDX #$00
palette:
        LDA colours,x
        STA $2007
        INX
        CPX #$04
        BNE palette
        LDA #$06
        STA $10
        LDA #$07
        STA $11
        JSR multiply
        STA $12
        LDA #$80
        STA $2000
forever:
        JMP forever

; $10 * $11 -> A (8 bits)
multiply:
        LDA #$00
        LDX $11
        BEQ done
more:
        CLC
        ADC $10
        DEX
        BNE more
done:
        RTS

nmi:
        PHA
        LDA #$01
        STA $4016
        LDA #$00
        STA $4016
        LDX #$08
read:
        LDA $4016
        LSR a
        ROL $13
        DEX
        BNE read
        INC $14
        PLA
        RTI

irq:
        RTI

colours:
        .byte $0F, $16, $27, $30

.segment "VECTORS"
        .word nmi, reset, irq
