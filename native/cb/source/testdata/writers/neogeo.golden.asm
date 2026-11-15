; demo-p1.asm -- written by CodeBreaker from demo-p1.p1
; Neo Geo, NGH 0123, 4 KB P-ROM (P-ROM)

REG_VRAMADDR         equ $3C0000
CREDIT_CHECK         equ $C00450

        org $0
        dc.l $0010F300  ; initial stack pointer
        dc.l $00C00402  ; reset
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
        dc.l vblank  ; vblank
        dc.l $00C00426  ; timer
        dc.l $00000000  ; reset_button
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
        dc.l $00000000
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
        dc.b "NEO-GEO"  ; magic
        dc.b $00  ; system version
        dc.w $0123  ; NGH number
        dc.l $00001000  ; program size
        dc.l $00000000  ; backup RAM
        dc.w $0000  ; save data size
        dc.b $01  ; eye-catcher
        dc.b $00  ; eye-catcher sprite bank
        dc.l $00000200  ; Japan settings
        dc.l $00000000  ; US settings
        dc.l $00000000  ; Europe settings
        JMP (user).L
        JMP (player_start).L
        JMP (demo_end).L
        JMP (demo_end).L
        dcb.b 72,$00
        dc.l $00000300  ; security code
        dcb.b 122,$00
        dcb.b 256,$FF

security:
        RTS
        dcb.b 254,$FF

user:
        JSR (CREDIT_CHECK).L
        MOVE.W #$7,(REG_VRAMADDR).L
        RTS

player_start:
        NOP
        RTS
        dc.b $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF

demo_end:
        BSR.W user
        RTS
        dcb.b 218,$FF

vblank:
        RTE
        dcb.b 2814,$FF
