; demo.s -- written by CodeBreaker from demo.exe
; PlayStation executable: entry 0x80010000, program at 0x80010000 (192 bytes), 3 functions
; (MIPS syntax as the processor's SLEIGH description writes it: build with `assemble --cpu mips --sleigh`)

        .set noreorder
        .set noat

        .equ GPU_GP0, 0x1F801810
        .equ GPU_GP1, 0x1F801814
        .equ var_80100000, 0x80100000

        .ascii "PS-X EXE"
        .space 8
        .word entry    ; pc0: where it starts
        .word 0x80108000    ; gp0: the initial $gp
        .word 0x80010000    ; t_addr: where the program is loaded
        .word 0x000000C0    ; t_size: its length
        .word 0x00000000    ; data address
        .word 0x00000000    ; data size
        .word 0x00000000    ; bss address
        .word 0x00000000    ; bss size
        .word 0x801FFF00    ; stack base
        .word 0x00000000    ; stack offset
        .space 20
        .asciz "Sony Computer Entertainment Inc. for North America area"
        .space 1916

        .org 0x80010000

entry:
        addiu sp, sp, -0x18
        sw ra, 0x14(sp)
        lui a0, %hi(D_80010078)
        jal sub_8001004C
        addiu a0, a0, %lo(D_80010078)
        lui t0, %hi(GPU_GP0)
        lw t1, %lo(GPU_GP1)(t0)
        nop
        lui t2, 0xe100
        sw t2, %lo(GPU_GP0)(t0)
        beq v0, zero, L_8001003C
        nop
L_80010030:
        addiu v0, v0, -0x1
        bne v0, zero, L_80010030
        nop
L_8001003C:
        lw ra, 0x14(sp)
        addiu sp, sp, 0x18
        jr ra
        nop

sub_8001004C:
        jr ra
        or v0, a0, zero

sub_80010054:
        addiu sp, sp, -0x8
        sw a0, 0x0(sp)
        lui v0, %hi(var_80100000)
        lw v1, %lo(var_80100000)(v0)
        nop
        addiu v1, v1, 0x1
        sw v1, %lo(var_80100000)(v0)
        jr ra
        addiu sp, sp, 0x8
D_80010078:
        .asciz "HELLO, PLAYSTATION"
        .byte 0x00
        .word entry, sub_8001004C, sub_80010054, 0xFFFFFFFF
        .space 32
        .word 0x12345678
