; demo.asm -- written by CodeBreaker from demo.md
; "TEST CARTRIDGE" / "TEST CARTRIDGE", GM 00000000-00, 4 KB ROM
; (68000 syntax as the processor's SLEIGH description writes it: build with `assemble --cpu m68k --sleigh`)

.equ VDP_CTRL, 0xc00004

        .org 0x0
        .long 0x00ff0000  ; initial stack pointer
        .long reset  ; reset
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
        .long 0x00000000  ; level1
        .long 0x00000000  ; external
        .long 0x00000000  ; level3
        .long 0x00000000  ; hblank
        .long 0x00000000  ; level5
        .long vblank  ; vblank
        .long 0x00000000  ; level7
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
        .ascii "SEGA MEGA DRIVE "  ; system
        .ascii "(C)TEST 2026.NOV"  ; copyright
        .ascii "TEST CARTRIDGE                                  "  ; domestic name
        .ascii "TEST CARTRIDGE                                  "  ; overseas name
        .ascii "GM 00000000-00"  ; serial and version
        .byte 0x12,0x34  ; checksum
        .ascii "J               "  ; devices
        .byte 0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0xff  ; ROM start and end
        .byte 0x00,0xff,0x00,0x00,0x00,0xff,0xff,0xff  ; RAM start and end
        .ascii "            "  ; backup RAM
        .ascii "            "  ; modem
        .ascii "                                        "  ; notes
        .ascii "JUE             "  ; regions

reset:
        move.w #0x80,(VDP_CTRL).l
        jsr sub_000220.l
L_00020E:
        bra.b L_00020E
        .space 16, 0xff

sub_000220:
        movea.l #D_000400,A0
        rts
        .space 216, 0xff

vblank:
        rte
        .space 254, 0xff
D_000400:
        .ascii "SCORE LEVEL LIVES"
        .space 3055, 0xff
