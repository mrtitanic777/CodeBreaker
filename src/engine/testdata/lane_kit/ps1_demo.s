# A small PlayStation program for the PS1 lane's tests: a PS-X EXE written in GNU as syntax, built with CodeBreaker's own assembler
# (build/mips_asm). It has what the lane compares: arguments in registers, memory written and read, a hardware register polled and
# written, a BIOS call, a loop, a call through a table, and strings and tables as data.
        .set noreorder
        .set noat

        .equ GPU_GP0, 0x1F801810
        .equ GPU_GP1, 0x1F801814
        .equ I_STAT,  0x1F801070

        .ascii "PS-X EXE"
        .space 8
        .word main
        .word 0
        .word 0x80010000
        .word text_end - 0x80010000
        .word 0, 0, 0, 0
        .word 0x801FFF00, 0
        .space 20
        .asciz "Sony Computer Entertainment Inc. for North America area"
        .space 0x800 - 0x4C - 56

        .org 0x80010000

# main: clears the buffer, fills it, sums it, sends the sum to the GPU and a character to the BIOS
main:   addiu $sp, $sp, -32
        sw    $ra, 28($sp)
        sw    $s0, 24($sp)
        lui   $s0, %hi(buffer)
        addiu $s0, $s0, %lo(buffer)
        move  $a0, $s0
        jal   clear
        addiu $a1, $zero, 16
        move  $a0, $s0
        addiu $a1, $zero, 16
        jal   fill
        addiu $a2, $zero, 3
        move  $a0, $s0
        jal   sum
        addiu $a1, $zero, 16
        jal   send
        move  $a0, $v0
        jal   puts
        lui   $a0, %hi(message)
        lw    $s0, 24($sp)
        lw    $ra, 28($sp)
        jr    $ra
        addiu $sp, $sp, 32

# clear(a0 = words, a1 = count): zero count words
clear:  beq   $a1, $zero, clear_done
        nop
clear_loop:
        sw    $zero, 0($a0)
        addiu $a1, $a1, -1
        bne   $a1, $zero, clear_loop
        addiu $a0, $a0, 4
clear_done:
        jr    $ra
        nop

# fill(a0 = words, a1 = count, a2 = step): word i = i * step
fill:   beq   $a1, $zero, fill_done
        move  $t0, $zero
fill_loop:
        sw    $t0, 0($a0)
        addu  $t0, $t0, $a2
        addiu $a1, $a1, -1
        bne   $a1, $zero, fill_loop
        addiu $a0, $a0, 4
fill_done:
        jr    $ra
        nop

# sum(a0 = words, a1 = count): v0 = their sum, v1 = the largest
sum:    move  $v0, $zero
        move  $v1, $zero
sum_loop:
        beq   $a1, $zero, sum_done
        nop
        lw    $t0, 0($a0)
        nop
        addu  $v0, $v0, $t0
        sltu  $t1, $v1, $t0
        beq   $t1, $zero, sum_next
        nop
        move  $v1, $t0
sum_next:
        addiu $a1, $a1, -1
        b     sum_loop
        addiu $a0, $a0, 4
sum_done:
        jr    $ra
        nop

# send(a0 = value): wait until the GPU takes commands, then write the value to GP0 and read I_STAT
send:   lui   $t0, 0x1F80
send_wait:
        lw    $t1, 0x1814($t0)
        lui   $t2, 0x0400
        and   $t1, $t1, $t2
        beq   $t1, $zero, send_wait
        nop
        sw    $a0, 0x1810($t0)
        lw    $v0, 0x1070($t0)
        jr    $ra
        sll   $v0, $v0, 1

# puts(a0 = high half of the message's address): put the message's characters to the BIOS, counting them
puts:   addiu $sp, $sp, -16
        sw    $ra, 12($sp)
        sw    $s1, 8($sp)
        addiu $s1, $a0, %lo(message)
        addiu $s2, $zero, 0
puts_next:
        lbu   $a0, 0($s1)
        nop
        beq   $a0, $zero, puts_done
        addiu $t2, $zero, 0xB0
        jalr  $t2
        addiu $t1, $zero, 0x3C
        addiu $s2, $s2, 1
        b     puts_next
        addiu $s1, $s1, 1
puts_done:
        lw    $s1, 8($sp)
        lw    $ra, 12($sp)
        move  $v0, $s2
        jr    $ra
        addiu $sp, $sp, 16

# apply(a0 = index, a1 = value): call the function in the table, with value
apply:  sll   $t0, $a0, 2
        lui   $t1, %hi(table)
        addu  $t1, $t1, $t0
        lw    $t2, %lo(table)($t1)
        nop
        jr    $t2
        move  $a0, $a1

# lowest(a0 = a, a1 = b): v0 = the smaller, signed
lowest: slt   $t0, $a0, $a1
        bne   $t0, $zero, lowest_a
        move  $v0, $a1
        move  $v0, $a0
lowest_a:
        jr    $ra
        nop

message:
        .asciz "PS1"
        .align 2
table:  .word lowest, send, sum
buffer: .space 64
text_end:
