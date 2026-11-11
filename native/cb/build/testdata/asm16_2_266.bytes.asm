; LZEXPAND, _f2_266: 2:0266-02ee -- written in assembly, as its author wrote it: what the
; loader fixes and the calls out of the function are written as what they refer to;
; an instruction the assembler would encode differently is given as bytes.
.386p
CODE2 segment byte public use16 'MCODE'
        assume cs:CODE2, ds:nothing, es:nothing, ss:nothing
        extrn _IMP_KERNEL_74:far
        extrn _IMP_KERNEL_81:far
        extrn _IMP_KERNEL_88:far
        extrn _f10_124:far
        extrn _f2_0:near
        public _f2_266
_f2_266:
        db   055h                                     ; push bp
        db   08Bh, 0ECh                               ; mov bp, sp
        db   01Eh                                     ; push ds
        db   08Eh, 0D8h                               ; mov ds, ax
        db   083h, 0ECh, 002h                         ; sub sp, 2
        db   057h                                     ; push di
        db   056h                                     ; push si
        db   08Bh, 076h, 008h                         ; mov si, word ptr [bp + 8]
        db   01Eh                                     ; push ds
        db   068h, 0E0h, 001h                         ; push 0x1e0
        db   0FFh, 076h, 00Eh                         ; push word ptr [bp + 0xe]
        db   0FFh, 076h, 00Ch                         ; push word ptr [bp + 0xc]
        call far ptr _IMP_KERNEL_88
        db   01Eh                                     ; push ds
        db   068h, 0E0h, 001h                         ; push 0x1e0
        db   0FFh, 076h, 00Ah                         ; push word ptr [bp + 0xa]
        db   056h                                     ; push si
        db   0FFh, 076h, 006h                         ; push word ptr [bp + 6]
        call far ptr _IMP_KERNEL_74
        db   08Bh, 0F8h                               ; mov di, ax
        db   083h, 0FFh, 0FFh                         ; cmp di, -1
        db   075h, 029h                               ; jne 0x2c3
        db   08Eh, 046h, 00Ah                         ; mov es, word ptr [bp + 0xa]
        db   026h, 083h, 07Ch, 002h, 002h             ; cmp word ptr es:[si + 2], 2
        db   075h, 01Fh                               ; jne 0x2c3
        db   01Eh                                     ; push ds
        db   068h, 0E0h, 001h                         ; push 0x1e0
        call far ptr _f10_124
        db   083h, 0C4h, 004h                         ; add sp, 4
        db   01Eh                                     ; push ds
        db   068h, 0E0h, 001h                         ; push 0x1e0
        db   08Bh, 046h, 00Ah                         ; mov ax, word ptr [bp + 0xa]
        db   050h                                     ; push ax
        db   056h                                     ; push si
        db   0FFh, 076h, 006h                         ; push word ptr [bp + 6]
        call far ptr _IMP_KERNEL_74
        db   08Bh, 0F8h                               ; mov di, ax
L_02C3:
        db   083h, 0FFh, 0FFh                         ; cmp di, -1
        db   074h, 01Ah                               ; je 0x2e2
        db   0F7h, 046h, 006h, 00Fh, 0FFh             ; test word ptr [bp + 6], 0xff0f
        db   075h, 013h                               ; jne 0x2e2
        db   057h                                     ; push di
        db   00Eh                                     ; push cs
        call near ptr _f2_0
        db   08Bh, 0F0h                               ; mov si, ax
        db   00Bh, 0F6h                               ; or si, si
        db   07Dh, 006h                               ; jge 0x2e0
        db   057h                                     ; push di
        call far ptr _IMP_KERNEL_81
L_02E0:
        db   08Bh, 0C6h                               ; mov ax, si
L_02E2:
        db   05Eh                                     ; pop si
        db   05Fh                                     ; pop di
        db   04Dh                                     ; dec bp
        db   04Dh                                     ; dec bp
        db   08Bh, 0E5h                               ; mov sp, bp
        db   01Fh                                     ; pop ds
        db   05Dh                                     ; pop bp
        db   04Dh                                     ; dec bp
        db   0CAh, 00Ah, 000h                         ; retf 0xa
CODE2 ends
end
