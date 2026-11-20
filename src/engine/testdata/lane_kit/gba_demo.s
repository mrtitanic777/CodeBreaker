@ A Game Boy Advance cartridge written for the lane's tests (GNU as syntax; built with CodeBreaker's own assembler).
@ `entry` branches to `start` (ARM), which calls a Thumb function through a literal pool and `bx` (the call comes back because
@ `mov lr, pc` comes first); the Thumb function calls another with `bl`; an ARM function is only reached through a table.
        .syntax unified
        .org 0x08000000
        .arm
entry:  b start
        .space 0xAE
        .byte 0x96
        .space 0xC0 - 0xB3
start:  push {r4, lr}
        ldr r0, pointer
        mov lr, pc
        bx r0
        mov r4, #0x04000000
        ldrh r1, [r4, #6]
        ldr r2, table
        mov lr, pc
        bx r2
        pop {r4, pc}
pointer: .word thumb_fn + 1
table:  .word arm_fn
        .thumb
thumb_fn: push {r4, lr}
        movs r4, #3
        adds r0, r4, r4
        cmp r0, #6
        bne skip
        movs r0, #9
skip:   bl helper
        pop {r4, pc}
helper: lsls r0, r0, #1
        bx lr
        .align 2
        .arm
arm_fn: push {lr}
        add r0, r0, #1
        subs r1, r1, #1
        popne {pc}
        mov r0, #0
        pop {pc}
arm_table: .word arm_fn
        .asciz "HELLO, GBA"
        .align 2
        .space 32
