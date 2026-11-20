; demo.asm -- written by CodeBreaker from demo.iso
; Sega CD "TEST DISC" / "TEST DISC", GM 00000000-00: the system area (32 KB) of a 2048-byte-sector image
; (68000 syntax as the processor's SLEIGH description writes it: build with `assemble --cpu m68k --sleigh`)

.equ CDBIOS, 0x005f22
.equ GA_MEMMODE_LO, 0xa12003
.equ GA_COMFLAG_SUB, 0xa1200f

        .org 0x0   ; the boot header
        .ascii "SEGADISCSYSTEM  "  ; disc ID
        .ascii "TESTCDGAME "  ; volume name
        .byte 0x00
        .half 0x0100  ; volume version
        .half 0x0001  ; volume type
        .ascii "TESTCDGAME "  ; system name
        .byte 0x00,0x00,0x00,0x00,0x00
        .long 0x00000800  ; IP: start on the disc
        .long 0x00000200  ; IP: size
        .long 0x00000000  ; IP: entry offset
        .long 0x00000000  ; IP: work RAM size
        .long 0x00001000  ; SP: start on the disc
        .long 0x00000400  ; SP: size
        .long 0x00000000  ; SP: entry offset
        .long 0x00000000  ; SP: work RAM size
        .space 176, 0x00
        .ascii "SEGA MEGA DRIVE "  ; system type
        .ascii "(C)TEST 2026.NOV"  ; copyright
        .ascii "TEST DISC                                       "  ; domestic name
        .ascii "TEST DISC                                       "  ; overseas name
        .ascii "GM 00000000-00"  ; serial and version
        .byte 0x00,0x00
        .ascii "J               "  ; devices
        .space 80, 0x00
        .ascii "E               "  ; regions

        .org 0xff0000   ; the main CPU's program: the security code and the IP, loaded at $FF0000

security:
        bra.w ip_start
        .space 1532, 0x00

ip_start:
        move.b #0x1,(GA_MEMMODE_LO).l
        bsr.b sub_FF0610
        jmp 0x200000.l

sub_FF0610:
        tst.b (GA_COMFLAG_SUB).l
        bne.b sub_FF0610
        rts
        .space 486, 0x00

        .org 0x100000   ; bytes of the system area no program runs
        .space 1536, 0x00

        .org 0x6000   ; the sub CPU's program (the SP), loaded at $6000 of PRG-RAM
        .ascii "MAIN-SUBCPU"
        .space 16, 0x00
        .byte 0x20,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x12,0x00,0x14,0x00,0x00

sp_init:
        moveq 0x1,D0
        jsr CDBIOS.l
        rts

sp_main:
        bra.b sp_main

sp_int2:
        rte
        .space 970, 0x00

        .org 0x110000   ; bytes of the system area no program runs
        .space 27648, 0x00

