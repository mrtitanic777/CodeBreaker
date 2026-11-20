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
        push   bp
        mov    bp, sp
        push   ds
        mov    ds, ax
        sub    sp, 2
        mov    ax, word ptr ds:[046h]
        or     ax, word ptr ds:[044h]
        je     short L_001C
L_0016:
        mov    ax, 1
        jmp    near ptr L_00AA
L_001C:
        push   02040h
        push   0
        push   01080h
        db   09Ah
        dd   _IMP_KERNEL_15
        mov    word ptr ds:[044h], 0
        mov    word ptr ds:[046h], ax
        or     ax, word ptr ds:[044h]
        je     short L_00A8
        mov    ax, 08000h
        mov    word ptr ds:[027Ch], ax
        mov    word ptr ds:[0276h], ax
L_0041:
        cmp    word ptr ds:[0276h], 0
        je     short L_009F
        push   02040h
        mov    ax, word ptr ds:[027Ch]
        sub    dx, dx
        add    ax, 1
        adc    dx, dx
        push   dx
        push   ax
        db   09Ah
        dd   _IMP_KERNEL_15
        mov    word ptr ds:[048h], 0
        mov    word ptr ds:[04Ah], ax
        or     ax, word ptr ds:[048h]
        je     short L_0091
        push   02040h
        push   0
        push   word ptr ds:[0276h]
        db   09Ah
        dd   _IMP_KERNEL_15
        mov    word ptr ds:[054h], 0
        mov    word ptr ds:[056h], ax
        or     ax, word ptr ds:[054h]
        jne    short L_0016
        push   word ptr ds:[04Ah]
        db   09Ah
        dd   _IMP_KERNEL_17
L_0091:
        sub    word ptr ds:[0276h], 0400h
        sub    word ptr ds:[027Ch], 0400h
        jne    short L_0041
L_009F:
        push   word ptr ds:[046h]
        db   09Ah
        dd   _IMP_KERNEL_17
L_00A8:
        sub    ax, ax
L_00AA:
        dec    bp
        dec    bp
        mov    sp, bp
        pop    ds
        pop    bp
        dec    bp
        retf
CODE1 ends
end
