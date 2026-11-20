; LZEXPAND, _f1_b6: 1:00b6-0116 -- written in assembly, as its author wrote it: what the
; loader fixes and the calls out of the function are written as what they refer to;
; an instruction the assembler would encode differently is given as bytes.
.386p
CODE1 segment byte public use16 'MCODE'
        assume cs:CODE1, ds:nothing, es:nothing, ss:nothing
        extrn _IMP_KERNEL_17:far
        public _f1_b6
_f1_b6:
        push   bp
        mov    bp, sp
        push   ds
        mov    ds, ax
        sub    sp, 2
        mov    ax, word ptr ds:[046h]
        or     ax, word ptr ds:[044h]
        je     short L_00D9
        push   word ptr ds:[046h]
        db   09Ah
        dd   _IMP_KERNEL_17
        sub    ax, ax
        mov    word ptr ds:[046h], ax
        mov    word ptr ds:[044h], ax
L_00D9:
        mov    ax, word ptr ds:[04Ah]
        db   14 dup (0CCh)                         ; _holedc, rebuilt from C, is placed after this
        db   000h, 02Bh, 0C0h, 0A3h, 04Ah, 000h, 0A3h, 048h, 000h, 0A1h, 056h, 000h, 00Bh, 006h, 054h, 000h
        db   074h, 011h, 0FFh, 036h, 056h, 000h, 09Ah
        dd   _IMP_KERNEL_17
        db   02Bh, 0C0h, 0A3h, 056h, 000h, 0A3h, 054h, 000h, 04Dh, 04Dh, 08Bh, 0E5h, 01Fh, 05Dh, 04Dh, 0CBh
        db   090h
CODE1 ends
end
