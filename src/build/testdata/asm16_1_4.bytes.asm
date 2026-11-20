; LZEXPAND, _f1_4: 1:0004-00b2 -- written in assembly, as its author wrote it: what the
; loader fixes and the calls out of the function are written as what they refer to;
; an instruction the assembler would encode differently is given as bytes.
.386p
CODE1 segment byte public use16 'MCODE'
        assume cs:CODE1, ds:nothing, es:nothing, ss:nothing
        extrn _IMP_KERNEL_15:far
        extrn _IMP_KERNEL_17:far
        public _f1_4
_f1_4:
        db   055h                                     ; push bp
        db   08Bh, 0ECh                               ; mov bp, sp
        db   01Eh                                     ; push ds
        db   08Eh, 0D8h                               ; mov ds, ax
        db   083h, 0ECh, 002h                         ; sub sp, 2
        db   0A1h, 046h, 000h                         ; mov ax, word ptr [0x46]
        db   00Bh, 006h, 044h, 000h                   ; or ax, word ptr [0x44]
        db   074h, 006h                               ; je 0x1c
L_0016:
        db   0B8h, 001h, 000h                         ; mov ax, 1
        db   0E9h, 08Eh, 000h                         ; jmp 0xaa
L_001C:
        db   068h, 040h, 020h                         ; push 0x2040
        db   06Ah, 000h                               ; push 0
        db   068h, 080h, 010h                         ; push 0x1080
        call far ptr _IMP_KERNEL_15
        db   0C7h, 006h, 044h, 000h, 000h, 000h       ; mov word ptr [0x44], 0
        db   0A3h, 046h, 000h                         ; mov word ptr [0x46], ax
        db   00Bh, 006h, 044h, 000h                   ; or ax, word ptr [0x44]
        db   074h, 070h                               ; je 0xa8
        db   0B8h, 000h, 080h                         ; mov ax, 0x8000
        db   0A3h, 07Ch, 002h                         ; mov word ptr [0x27c], ax
        db   0A3h, 076h, 002h                         ; mov word ptr [0x276], ax
L_0041:
        db   083h, 03Eh, 076h, 002h, 000h             ; cmp word ptr [0x276], 0
        db   074h, 057h                               ; je 0x9f
        db   068h, 040h, 020h                         ; push 0x2040
        db   0A1h, 07Ch, 002h                         ; mov ax, word ptr [0x27c]
        db   02Bh, 0D2h                               ; sub dx, dx
        db   005h, 001h, 000h                         ; add ax, 1
        db   013h, 0D2h                               ; adc dx, dx
        db   052h                                     ; push dx
        db   050h                                     ; push ax
        call far ptr _IMP_KERNEL_15
        db   0C7h, 006h, 048h, 000h, 000h, 000h       ; mov word ptr [0x48], 0
        db   0A3h, 04Ah, 000h                         ; mov word ptr [0x4a], ax
        db   00Bh, 006h, 048h, 000h                   ; or ax, word ptr [0x48]
        db   074h, 026h                               ; je 0x91
        db   068h, 040h, 020h                         ; push 0x2040
        db   06Ah, 000h                               ; push 0
        db   0FFh, 036h, 076h, 002h                   ; push word ptr [0x276]
        call far ptr _IMP_KERNEL_15
        db   0C7h, 006h, 054h, 000h, 000h, 000h       ; mov word ptr [0x54], 0
        db   0A3h, 056h, 000h                         ; mov word ptr [0x56], ax
        db   00Bh, 006h, 054h, 000h                   ; or ax, word ptr [0x54]
        db   075h, 08Eh                               ; jne 0x16
        db   0FFh, 036h, 04Ah, 000h                   ; push word ptr [0x4a]
        call far ptr _IMP_KERNEL_17
L_0091:
        db   081h, 02Eh, 076h, 002h, 000h, 004h       ; sub word ptr [0x276], 0x400
        db   081h, 02Eh, 07Ch, 002h, 000h, 004h       ; sub word ptr [0x27c], 0x400
        db   075h, 0A2h                               ; jne 0x41
L_009F:
        db   0FFh, 036h, 046h, 000h                   ; push word ptr [0x46]
        call far ptr _IMP_KERNEL_17
L_00A8:
        db   02Bh, 0C0h                               ; sub ax, ax
L_00AA:
        db   04Dh                                     ; dec bp
        db   04Dh                                     ; dec bp
        db   08Bh, 0E5h                               ; mov sp, bp
        db   01Fh                                     ; pop ds
        db   05Dh                                     ; pop bp
        db   04Dh                                     ; dec bp
        db   0CBh                                     ; retf 
CODE1 ends
end
