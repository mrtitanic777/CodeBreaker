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
        push   bp
        mov    bp, sp
        push   ds
        mov    ds, ax
        sub    sp, 2
        push   di
        push   si
        mov    si, word ptr [bp + 8]
        push   ds
        push   01E0h
        push   word ptr [bp + 0Eh]
        push   word ptr [bp + 0Ch]
        db   09Ah
        dd   _IMP_KERNEL_88
        push   ds
        push   01E0h
        push   word ptr [bp + 0Ah]
        push   si
        push   word ptr [bp + 6]
        db   09Ah
        dd   _IMP_KERNEL_74
        mov    di, ax
        cmp    di, -1
        jne    short L_02C3
        mov    es, word ptr [bp + 0Ah]
        cmp    word ptr es:[si + 2], 2
        jne    short L_02C3
        push   ds
        push   01E0h
        db   09Ah
        dd   _f10_124
        add    sp, 4
        push   ds
        push   01E0h
        mov    ax, word ptr [bp + 0Ah]
        push   ax
        push   si
        push   word ptr [bp + 6]
        db   09Ah
        dd   _IMP_KERNEL_74
        mov    di, ax
L_02C3:
        cmp    di, -1
        je     short L_02E2
        test   word ptr [bp + 6], 0FF0Fh
        jne    short L_02E2
        push   di
        push   cs
        call near ptr _f2_0
        mov    si, ax
        or     si, si
        jge    short L_02E0
        push   di
        db   09Ah
        dd   _IMP_KERNEL_81
L_02E0:
        mov    ax, si
L_02E2:
        pop    si
        pop    di
        dec    bp
        dec    bp
        mov    sp, bp
        pop    ds
        pop    bp
        dec    bp
        retf   0Ah
CODE2 ends
end
