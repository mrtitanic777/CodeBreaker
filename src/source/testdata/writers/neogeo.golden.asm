; demo-p1.asm -- written by CodeBreaker from demo-p1.p1
; Neo Geo, NGH 0123, 4 KB P-ROM (P-ROM)
; (68000 syntax as the processor's SLEIGH description writes it: build with `assemble --cpu m68k --sleigh`)

.equ REG_VRAMADDR, 0x3c0000
.equ CREDIT_CHECK, 0xc00450

        .org 0x0
        .long 0x0010f300  ; initial stack pointer
        .long 0x00c00402  ; reset
        .long 0x00000000  ; bus_error
        .long 0x00000000  ; address_error
        .long 0x00000000  ; illegal_instruction
        .long 0x00000000  ; zero_divide
        .long 0x00000000  ; chk
        .long 0x00000000  ; trapv
        .long 0x00000000  ; privilege
        .long 0x00000000  ; trace
        .long 0x00000000  ; line_a
        .long 0x00000000  ; line_f
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000  ; spurious
        .long vblank  ; vblank
        .long 0x00c00426  ; timer
        .long 0x00000000  ; reset_button
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000  ; trap0
        .long 0x00000000  ; trap1
        .long 0x00000000  ; trap2
        .long 0x00000000  ; trap3
        .long 0x00000000  ; trap4
        .long 0x00000000  ; trap5
        .long 0x00000000  ; trap6
        .long 0x00000000  ; trap7
        .long 0x00000000  ; trap8
        .long 0x00000000  ; trap9
        .long 0x00000000  ; trap10
        .long 0x00000000  ; trap11
        .long 0x00000000  ; trap12
        .long 0x00000000  ; trap13
        .long 0x00000000  ; trap14
        .long 0x00000000  ; trap15
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .long 0x00000000
        .ascii "NEO-GEO"  ; magic
        .byte 0x00  ; system version
        .half 0x0123  ; NGH number
        .long 0x00001000  ; program size
        .long 0x00000000  ; backup RAM
        .half 0x0000  ; save data size
        .byte 0x01  ; eye-catcher
        .byte 0x00  ; eye-catcher sprite bank
        .long 0x00000200  ; Japan settings
        .long 0x00000000  ; US settings
        .long 0x00000000  ; Europe settings
        jmp user.l
        jmp player_start.l
        jmp demo_end.l
        jmp demo_end.l
        .space 72, 0x00
        .long 0x00000300  ; security code
        .space 122, 0x00
        .space 256, 0xff

security:
        rts
        .space 254, 0xff

user:
        jsr CREDIT_CHECK.l
        move.w #0x7,(REG_VRAMADDR).l
        rts

player_start:
        nop
        rts
        .byte 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff

demo_end:
        bsr.w user
        rts
        .space 218, 0xff

vblank:
        rte
        .space 2814, 0xff
