; xcopy.exe -- reconstructed from the program image by CodeBreaker.
;
; One segment for each segment the program names. A word holding a segment is written as
; that segment, so the program can be loaded anywhere; offsets inside a segment are the
; original's numbers. Labels are L_<position in the load module>. Instructions written as
; bytes are ones the assembler would encode differently; their mnemonic is in the comment.

.386p

S0000 segment para public use16 'C0000'
        assume cs:S0000, ds:nothing, es:nothing, ss:nothing
        public _codebreaker_seg_0000
_codebreaker_seg_0000 label byte
sub_00000:
        push   ax
        push   bx
        push   dx
        push   es
        push   di
        mov    ax, 05100h
        int    021h    ; DOS_GetCurrentProgramPrefix(-> BX = segment)
        xor    cx, cx
        mov    es, cx
        xor    di, di
        mov    ax, 0122Eh
        mov    dl, 0
        int    02Fh    ; Multiplex_GetOrSetErrorTable(DL = which)
        mov    word ptr ss:[012h], es
        mov    word ptr ss:[010h], di
        mov    dl, 6
        int    02Fh    ; Multiplex
        mov    word ptr ss:[016h], es
        mov    word ptr ss:[014h], di
        mov    ax, 0122Eh
        mov    dl, 2
        int    02Fh    ; Multiplex_GetOrSetErrorTable(DL = which)
        mov    word ptr ss:[01Eh], es
        mov    word ptr ss:[01Ch], di
        mov    ax, 0122Eh
        mov    dl, 4
        int    02Fh    ; Multiplex_GetOrSetErrorTable(DL = which)
        mov    word ptr ss:[02Eh], es
        mov    word ptr ss:[02Ch], di
        mov    es, bx
        mov    es, word ptr es:[02Ch]
        mov    di, 0
        call   near ptr sub_00252
        push   ds
        push   es
        pop    ds
        mov    dx, di
        add    dx, 3
        mov    ax, 03D00h
        int    021h    ; DOS_OpenFile(AL = access mode, DS:DX = path -> AX = handle)
        pop    ds
        jb     short L_000BE
        mov    bx, ax
        mov    word ptr ss:[0A3h], bx
        call   near ptr sub_000CA
        pushf
        mov    ax, 03E00h
        mov    bx, word ptr ss:[0A3h]
        int    021h    ; DOS_CloseFile(BX = handle)
        popf
        jb     short L_000C4
        mov    ax, 0122Eh
        mov    dl, 8
        int    02Fh    ; Multiplex_GetOrSetErrorTable(DL = which)
        mov    word ptr ss:[042h], es
        mov    word ptr ss:[040h], di
        mov    word ptr ss:[04Bh], 0A0Dh
        mov    byte ptr ss:[0A0h], 024h
        mov    word ptr ss:[04Fh], 0
        mov    word ptr ss:[053h], 0Ah
        push   cx
        call   near ptr sub_002B8
        jae    short L_000BD
        add    sp, 2
        jmp    short L_000C4
L_000BD:
        pop    cx
L_000BE:
        pop    di
        pop    es
        pop    dx
        pop    bx
        pop    ax
L_000C3:
        ret
L_000C4:
        add    sp, 0Ah
        stc
        jmp    short L_000C3
sub_000CA:
        push   es
        xor    cx, cx
        xor    dx, dx
        sub    dx, 7
        sbb    cx, 0
        mov    ax, 04202h
        int    021h    ; DOS_SeekFile(AL = origin, BX = handle, CX:DX = offset -> DX:AX = position)
        mov    cx, 7
        add    ax, cx
        adc    dx, 0
        mov    word ptr ss:[0ACh], ax
        mov    word ptr ss:[0AEh], dx
        mov    ax, 03F00h
        lea    dx, ds:[0BEh]
        int    021h    ; DOS_ReadFile(BX = handle, CX = bytes, DS:DX = buffer -> AX = bytes read)
        jb     short L_00114
        cmp    word ptr ss:[0C1h], 0534Eh
        jne    short L_00114
        mov    ax, word ptr ss:[0C3h]
        mov    dx, word ptr ss:[0ACh]
        mov    cx, word ptr ss:[0AEh]
        sub    dx, ax
        sbb    cx, 0
        jmp    short L_0015B
L_00114:
        xor    cx, cx
        xor    dx, dx
        mov    ax, 04200h
        int    021h    ; DOS_SeekFile(AL = origin, BX = handle, CX:DX = offset -> DX:AX = position)
        mov    cx, 01Ah
        mov    ax, 03F00h
        lea    dx, ds:[0BEh]
        int    021h    ; DOS_ReadFile(BX = handle, CX = bytes, DS:DX = buffer -> AX = bytes read)
        jb     short L_00181
        mov    ax, word ptr ss:[0BEh]
        cmp    ax, 05A4Dh
        jne    short L_00181
        mov    ax, word ptr ss:[0C4h]
        shl    ax, 1
        shl    ax, 1
        add    ax, word ptr ss:[0D6h]
        db   03Dh, 03Fh, 000h                ; cmp    ax, 03Fh
        jbe    short L_00156
        db   005h, 00Fh, 000h                ; add    ax, 0Fh
        mov    cx, 4
        shr    ax, cl
        shl    ax, cl
        xor    cx, cx
        mov    dx, ax
        jmp    short L_0015B
L_00156:
        xor    cx, cx
        mov    dx, 040h
L_0015B:
        mov    word ptr ss:[0ACh], cx
        mov    word ptr ss:[0AEh], dx
        mov    si, 1
        call   near ptr sub_00183
        jae    short L_0017E
        mov    cx, word ptr ss:[0ACh]
        mov    dx, word ptr ss:[0AEh]
        xor    si, si
        call   near ptr sub_00183
        jb     short L_00181
L_0017E:
        call   near ptr sub_00204
L_00181:
        pop    es
        ret
sub_00183:
        push   cx
        push   dx
        mov    ax, 04200h
        int    021h    ; DOS_SeekFile(AL = origin, BX = handle, CX:DX = offset -> DX:AX = position)
        mov    cx, 010h
        mov    ah, 03Fh
        lea    dx, ds:[0BEh]
        int    021h    ; DOS_ReadFile(BX = handle, CX = bytes, DS:DX = buffer -> AX = bytes read)
        jb     short L_001D0
        cmp    ax, cx
        jne    short L_001D0
        mov    ax, word ptr ss:[0C9h]
        cmp    ax, 0534Eh
        jne    short L_001D0
        or     si, si
        je     short L_001D5
        lea    si, ds:[0C2h]
        push   es
        push   ds
        pop    es
        lea    di, ds:[0A5h]
        mov    cx, 3
        repe cmpsb
        pop    es
        je     short L_001D5
        pop    dx
        pop    cx
        add    dx, word ptr ss:[0BEh]
        adc    cx, 0
        add    dx, 0Fh
        adc    cx, 0
        and    dl, 0F0h
        jmp    short sub_00183
L_001D0:
        pop    dx
        pop    cx
        stc
        jmp    short L_00200
L_001D5:
        pop    dx
        pop    cx
        push   bx
        mov    bx, word ptr ss:[0C0h]
        sub    bx, 010h
        mov    ax, 0200h
        sub    ax, bx
        pop    bx
        js     short L_00201
        mov    cx, word ptr ss:[0BEh]
        sub    cx, 010h
        push   ds
        mov    ax, ds
        mov    es, ax
        lea    dx, ds:[0D8h]
        mov    ah, 03Fh
        int    021h    ; DOS_ReadFile(BX = handle, CX = bytes, DS:DX = buffer -> AX = bytes read)
        pop    ds
        jb     short L_00201
L_00200:
        ret
L_00201:
        stc
        jmp    short L_00200
sub_00204:
        mov    di, dx
        mov    cl, byte ptr es:[di]
        sub    cl, 2
        inc    di
        mov    ax, word ptr es:[di]
        add    ax, di
        mov    word ptr ss:[0Ch], ax
        mov    word ptr ss:[0Eh], es
        mov    word ptr ss:[02Ah], es
        mov    word ptr ss:[028h], ax
        add    di, 2
        mov    ax, word ptr es:[di]
        add    ax, di
        mov    word ptr ss:[022h], es
        mov    word ptr ss:[020h], ax
        mov    si, 0
L_00238:
        add    di, 2
        mov    ax, word ptr es:[di]
        add    ax, di
        db   036h, 089h, 084h, 034h, 000h    ; mov    word ptr ss:[si + 034h], ax
        db   036h, 08Ch, 084h, 036h, 000h    ; mov    word ptr ss:[si + 036h], es
        add    si, 4
        dec    cl
        jne    short L_00238
        ret
sub_00252:
        push   si
        push   di
        lea    si, ds:[0B0h]
        mov    bx, 5
        xor    ax, ax
L_0025D:
        mov    cx, bx
        push   si
        repe cmpsb
        pop    si
        je     short L_00271
L_00265:
        mov    cx, 0FFFFh
        repne scasb
        cmp    byte ptr es:[di], al
        je     short L_00287
        jmp    short L_0025D
L_00271:
        lea    si, ds:[0A5h]
        mov    ax, word ptr es:[di]
        mov    word ptr [si], ax
        add    si, 2
        mov    al, byte ptr es:[di + 2]
        mov    byte ptr [si], al
        xor    ax, ax
        jmp    short L_00265
L_00287:
        pop    di
        lea    si, ds:[0B5h]
        mov    bx, 9
        xor    ax, ax
L_00291:
        mov    cx, bx
        push   si
        repe cmpsb
        pop    si
        je     short L_002A5
L_00299:
        mov    cx, 0FFFFh
        repne scasb
        cmp    byte ptr es:[di], al
        je     short L_002B6
        jmp    short L_00291
L_002A5:
        lea    si, ds:[0A8h]
        mov    word ptr [si], di
        add    si, 2
        mov    ax, es
        mov    word ptr [si], ax
        xor    ax, ax
        jmp    short L_00299
L_002B6:
        pop    si
        ret
sub_002B8:
        mov    ah, 030h
        int    021h    ; DOS_GetVersion(-> AL = major, AH = minor)
        cmp    ax, 0A07h
        je     short L_002D9
        db   03Dh, 002h, 000h                ; cmp    ax, 2
        jae    short L_002CB
        mov    bx, 0FFFFh
        jmp    short L_002CE
L_002CB:
        mov    bx, 2
L_002CE:
        mov    ax, 1
        mov    cx, 0
        mov    dl, 0
        mov    dh, 3
        stc
L_002D9:
        ret
sub_002DA:
        push   bx
        push   cx
        push   bp
        push   ax
        push   di
        push   es
        push   dx
        mov    bp, cx
        mov    word ptr ss:[048h], bx
        call   near ptr sub_005E6
        jcxz   L_00304
        or     bp, bp
        jne    short L_002F6
        call   near ptr sub_00318
        jmp    short L_002F9
L_002F6:
        call   near ptr sub_00413
L_002F9:
        jb     short L_00304
        pop    dx
        call   near ptr sub_003A0
        pop    es
        pop    di
        pop    ax
        jmp    short L_00309
L_00304:
        pop    dx
        pop    es
        pop    di
        pop    ax
        stc
L_00309:
        jb     short L_00310
        pop    bp
        pop    cx
        pop    bx
        jmp    short L_00317
L_00310:
        add    sp, 6
        stc
        mov    ax, 0FFFFh
L_00317:
        ret
sub_00318:
        push   ax
        push   bx
        push   dx
        mov    bx, word ptr ss:[048h]
        cmp    bx, -1
        jne    short L_0032A
        call   near ptr sub_00380
        jmp    short L_0032D
L_0032A:
        call   near ptr sub_00355
L_0032D:
        jae    short L_0033A
        mov    ah, 059h
        mov    bx, 0
        int    021h    ; DOS_GetExtendedError(BX = 0 -> AX = error, BH = class, BL = action, CH = locus)
        xor    ah, ah
        jmp    short L_0034B
L_0033A:
        cmp    bx, -1
        je     short L_00351
        cmp    ax, cx
        je     short L_00351
        mov    ax, 027h
        mov    bx, 4
        mov    ch, 1
L_0034B:
        add    sp, 6
        stc
        jmp    short L_00354
L_00351:
        pop    dx
        pop    bx
        pop    ax
L_00354:
        ret
sub_00355:
        xor    ax, ax
        jcxz   L_0037F
        push   ds
        push   es
        pop    ds
        mov    ah, 040h
        mov    dx, di
        cmp    cx, 1
        jne    short L_00369
        int    021h    ; DOS
        jmp    short L_0036B
L_00369:
        int    021h    ; DOS
L_0036B:
        jb     short L_0037E
        cmp    cx, ax
        je     short L_0037E
        push   di
        add    di, ax
        cmp    byte ptr es:[di], 01Ah
        pop    di
        clc
        jne    short L_0037E
        mov    ax, cx
L_0037E:
        pop    ds
L_0037F:
        ret
sub_00380:
        push   ds
        push   es
        pop    ds
        cmp    cx, 1
        jne    short L_00391
        mov    ah, 2
        mov    dl, byte ptr es:[di]
        int    021h    ; DOS_WriteCharacter(DL = character)
        jmp    short L_0039D
L_00391:
        mov    ah, 2
        jcxz   L_0039D
L_00395:
        mov    dl, byte ptr es:[di]
        int    021h    ; DOS
        inc    di
        loop   L_00395
L_0039D:
        clc
        pop    ds
        ret
sub_003A0:
        cmp    dh, 3
        je     short L_003B6
        test   dh, 080h
        jne    short L_003B6
        push   ds
        pop    es
        lea    di, ds:[04Bh]
        mov    cx, 2
        call   near ptr sub_00318
L_003B6:
        ret
sub_003B7:
        pop    word ptr ss:[04Dh]
        xor    bx, bx
        xchg   bx, ax
        xchg   dx, ax
L_003C0:
        div    word ptr ss:[053h]
        xchg   bx, ax
        div    word ptr ss:[053h]
        cmp    dx, 9
        jbe    short L_003D5
        add    dl, 037h
        jmp    short L_003D8
L_003D5:
        add    dl, 030h
L_003D8:
        push   dx
        inc    cx
        or     ax, ax
        jne    short L_003E2
        or     bx, bx
        je     short L_00409
L_003E2:
        cmp    cx, 3
        jne    short L_003FA
L_003E7:
        push   ax
        mov    al, byte ptr [si + 7]
        and    al, 0Fh
        cmp    al, 2
        pop    ax
        ja     short L_00404
        push   word ptr ss:[057h]
        inc    cx
        jmp    short L_00404
L_003FA:
        cmp    cx, 7
        je     short L_003E7
        cmp    cx, 0Bh
        je     short L_003E7
L_00404:
        xchg   bx, ax
        xor    dx, dx
        jmp    short L_003C0
L_00409:
        xor    dx, dx
        xor    ax, ax
        push   word ptr ss:[04Dh]
        ret
sub_00413:
        mov    word ptr ss:[051h], bp
L_00418:
        or     cx, cx
        jne    short L_0041F
        jmp    near ptr L_004A0
L_0041F:
        xor    dx, dx
L_00421:
        mov    al, byte ptr es:[di]
        cmp    al, 025h
        je     short L_00445
        jmp    short L_0042D
L_0042A:
        inc    di
        inc    dx
        dec    cx
L_0042D:
        inc    di
        inc    dx
        loop   L_00421
L_00431:
        xchg   dx, cx
        jcxz   L_00418
        push   di
        sub    di, cx
        call   near ptr sub_00318
        pop    di
        jae    short L_00441
        jmp    short L_004B2
        db   090h
L_00441:
        xchg   dx, cx
        jmp    short L_00418
L_00445:
        mov    al, byte ptr es:[di + 1]
        cmp    al, 025h
        je     short L_0042A
        sub    al, 030h
        xor    ah, ah
        cmp    ax, word ptr ss:[051h]
        ja     short L_0042A
        or     dx, dx
        jne    short L_00431
        dec    bp
L_0045D:
        push   si
L_0045E:
        dec    al
        je     short L_00468
        mov    dl, byte ptr [si]
        add    si, dx
        jmp    short L_0045E
L_00468:
        push   bp
        push   di
        push   es
        push   cx
        xor    cx, cx
        cmp    word ptr ss:[04Fh], 0
        jne    short L_00489
        test   byte ptr [si + 7], 0Fh
        jne    short L_00484
        db   0C4h, 07Ch, 002h                ; les    di, ptr [si + 2]
        call   near ptr sub_005C9
        jmp    short L_00484
L_00484:
        call   near ptr sub_004EB
        jmp    short L_0048C
L_00489:
        call   near ptr sub_004BA
L_0048C:
        pop    cx
        pop    es
        pop    di
        pop    bp
        pop    si
        cmp    word ptr ss:[04Fh], 0
        jl     short L_004A0
        inc    di
        mov    dx, 0FFFFh
        dec    cx
        jmp    short L_0042D
L_004A0:
        jb     short L_004B2
        or     bp, bp
        je     short L_004B2
        dec    bp
        mov    ax, word ptr ss:[051h]
        sub    ax, bp
        mov    cx, 2
        jmp    short L_0045D
L_004B2:
        mov    word ptr ss:[04Fh], 0
        ret
sub_004BA:
        xor    dx, dx
        mov    ax, word ptr ss:[04Fh]
        mov    word ptr ss:[053h], 0Ah
        call   near ptr sub_003B7
L_004CA:
        pop    ax
        db   036h, 088h, 087h, 060h, 000h    ; mov    byte ptr ss:[bx + 060h], al
        inc    bx
        cmp    bx, 040h
        jne    short L_004D9
        call   near ptr sub_005AD
L_004D9:
        dec    cl
        jne    short L_004CA
        mov    ax, 0A0Dh
        db   036h, 089h, 087h, 060h, 000h    ; mov    word ptr ss:[bx + 060h], ax
        inc    bx
        inc    bx
        call   near ptr sub_005AD
        ret
sub_004EB:
        xor    bx, bx
        cmp    byte ptr [si + 6], 0
        jne    short L_00506
        db   036h, 0C7h, 087h, 060h, 000h, 020h, 02Dh ; mov    word ptr ss:[bx + 060h], 02D20h
        inc    bx
        inc    bx
        db   036h, 0C6h, 087h, 060h, 000h, 020h ; mov    byte ptr ss:[bx + 060h], 020h
        inc    bx
        call   near ptr sub_005AD
L_00506:
        pop    bp
        xor    bx, bx
        xor    dx, dx
        mov    byte ptr ss:[04Ah], cl
        mov    al, byte ptr [si + 9]
        cmp    al, cl
        jbe    short L_00536
        sub    al, cl
        mov    dh, al
        test   byte ptr [si + 7], 080h
        je     short L_00536
L_00521:
        mov    al, byte ptr [si + 0Ah]
        db   036h, 088h, 087h, 060h, 000h    ; mov    byte ptr ss:[bx + 060h], al
        inc    bx
        cmp    bx, 040h
        jne    short L_00532
        call   near ptr sub_005AD
L_00532:
        dec    dh
        jne    short L_00521
L_00536:
        cmp    byte ptr [si + 8], 0
        je     short L_00549
        cmp    byte ptr [si + 8], cl
        jae    short L_00549
        sub    cl, byte ptr [si + 8]
        mov    dl, cl
        mov    cl, byte ptr [si + 8]
L_00549:
        jcxz   L_00570
L_0054B:
        test   byte ptr [si + 7], 0Fh
        jne    short L_0055D
        test   byte ptr [si + 7], 010h
        je     short L_0055D
        mov    al, byte ptr es:[di]
        inc    di
        jmp    short L_0055E
L_0055D:
        pop    ax
L_0055E:
        db   036h, 088h, 087h, 060h, 000h    ; mov    byte ptr ss:[bx + 060h], al
        inc    bx
        cmp    bx, 040h
        jne    short L_0056C
        call   near ptr sub_005AD
L_0056C:
        dec    cl
        jne    short L_0054B
L_00570:
        test   byte ptr [si + 7], 080h
        jne    short L_0058F
        or     dh, dh
        je     short L_0058F
L_0057A:
        mov    al, byte ptr [si + 0Ah]
        db   036h, 088h, 087h, 060h, 000h    ; mov    byte ptr ss:[bx + 060h], al
        inc    bx
        cmp    bx, 040h
        jne    short L_0058B
        call   near ptr sub_005AD
L_0058B:
        dec    dh
        jne    short L_0057A
L_0058F:
        test   byte ptr [si + 7], 0Fh
        jne    short L_0059B
        test   byte ptr [si + 7], 010h
        jne    short L_005A8
L_0059B:
        or     dl, dl
        je     short L_005A8
L_0059F:
        pop    word ptr ss:[04Dh]
        dec    dl
        jne    short L_0059F
L_005A8:
        call   near ptr sub_005AD
        push   bp
        ret
sub_005AD:
        push   cx
        push   es
        push   di
        push   ds
        pop    es
        mov    cx, bx
        xor    bx, bx
        lea    di, ds:[060h]
        call   near ptr sub_00318
        jb     short L_005C4
        pop    di
        pop    es
        pop    cx
        jmp    short L_005C8
L_005C4:
        add    sp, 6
        stc
L_005C8:
        ret
sub_005C9:
        pop    bp
        test   byte ptr [si + 7], 030h
        jne    short L_005D7
        mov    al, byte ptr es:[di]
        push   ax
        inc    cx
        jmp    short L_005E4
L_005D7:
        mov    al, byte ptr es:[di]
        or     al, al
        je     short L_005E2
        inc    di
        inc    cx
        jmp    short L_005D7
L_005E2:
        sub    di, cx
L_005E4:
        push   bp
        ret
sub_005E6:
        push   si
        push   bx
        xor    si, si
        xor    cx, cx
        mov    word ptr ss:[04Fh], 0
        cmp    dh, 0
        jne    short L_005FB
        jmp    short L_0066C
        db   090h
L_005FB:
        cmp    dh, 3
        jne    short L_00605
        mov    si, 028h
        jmp    short L_0061A
L_00605:
        test   dh, 2
        je     short L_0060D
        mov    si, 010h
L_0060D:
        db   03Dh, 013h, 000h                ; cmp    ax, 013h
        jb     short L_0061A
        db   03Dh, 027h, 000h                ; cmp    ax, 027h
        ja     short L_0061A
        mov    si, 01Ch
L_0061A:
        lea    bx, ds:[0Ch]
        add    si, bx
L_00620:
        db   0C4h, 03Ch                      ; les    di, ptr [si]
        mov    bx, es
        cmp    bx, -1
        jne    short L_00659
        cmp    dh, 3
        jne    short L_00636
        stc
        jmp    short L_0066C
L_00631:
        db   03Dh, 0FFh, 0FFh                ; cmp    ax, 0FFFFh
        je     short L_0066C
L_00636:
        cmp    dh, 1
        jne    short L_00642
        call   near ptr sub_0066F
        or     cx, cx
        jne    short L_0066B
L_00642:
        cmp    word ptr ss:[04Fh], 0
        jne    short L_0066C
        mov    word ptr ss:[04Fh], ax
        mov    ax, 0FFFFh
        mov    bp, 1
        xor    si, si
        clc
        jmp    short L_005FB
L_00659:
        cmp    bx, 0
        je     short L_00661
        call   near ptr sub_006A1
L_00661:
        add    si, 4
        jcxz   L_00620
        cmp    dh, 1
        je     short L_00631
L_0066B:
        clc
L_0066C:
        pop    bx
        pop    si
        ret
sub_0066F:
        push   dx
        push   bp
        push   cx
        push   es
        push   di
        push   ax
        mov    ax, 0500h
        int    02Fh    ; Multiplex_CriticalErrorMessages(AL = function)
        cmp    al, 0FFh
        pop    ax
        jne    short L_0069B
        mov    bx, ax
        mov    ax, 0502h
        int    02Fh    ; Multiplex_CriticalErrorMessages(AL = function)
        jb     short L_0069B
        push   di
        push   ax
        mov    cx, 0FFFFh
        xor    al, al
        repne scasb
        not    cx
        dec    cx
        pop    ax
        pop    di
        add    sp, 6
        jmp    short L_0069E
L_0069B:
        pop    di
        pop    es
        pop    cx
L_0069E:
        pop    bp
        pop    dx
        ret
sub_006A1:
        cmp    bx, 1
        jne    short L_006C0
        cmp    word ptr ss:[040h], -1
        je     short L_006C0
        db   03Dh, 0FFh, 0FFh                ; cmp    ax, 0FFFFh
        push   ax
        jne    short L_006B8
        mov    ax, word ptr ss:[04Fh]
L_006B8:
        db   036h, 0FFh, 01Eh, 040h, 000h    ; lcall  ss:[040h]
        pop    ax
        jmp    short L_006D4
L_006C0:
        xor    cx, cx
        cmp    dh, 3
        jne    short L_006CC
        cmp    byte ptr es:[di], dh
        jne    short L_006D0
L_006CC:
        mov    cl, byte ptr es:[di + 1]
L_006D0:
        add    di, 2
        stc
L_006D4:
        jae    short L_006EA
        clc
L_006D7:
        jcxz   L_006F7
        cmp    ax, word ptr es:[di]
        je     short L_006E6
        dec    cx
        je     short L_006F7
        add    di, 4
        jmp    short L_006D7
L_006E6:
        add    di, word ptr es:[di + 2]
L_006EA:
        xor    ch, ch
        mov    cl, byte ptr es:[di]
        inc    di
L_006F0:
        mov    byte ptr ss:[04Ah], 0
        ret
L_006F7:
        stc
        jmp    short L_006F0
sub_006FA:
        mov    ax, ss
        mov    ds, ax
        mov    byte ptr ds:[0362h], 1
        mov    ah, 062h
        int    021h    ; DOS_GetProgramPrefix(-> BX = segment)
        mov    es, bx
        mov    es, word ptr es:[02Ch]
        mov    si, 04A4h
        mov    cx, 8
        call   near ptr sub_0076E
        jb     short L_0072E
        call   near ptr sub_0075A
        jb     short L_0072D
        inc    di
        mov    al, byte ptr es:[di]
        and    al, 0DFh
        cmp    al, 059h
        jne    short L_0072D
L_00728:
        mov    byte ptr ds:[0362h], 0
L_0072D:
        ret
L_0072E:
        mov    es, bx
        mov    es, word ptr es:[016h]
        mov    ax, es
        cmp    ax, word ptr es:[016h]
        jne    short L_00728
        xor    di, di
        mov    cx, 01000h
        mov    al, 063h
        cld
L_00746:
        repne scasb
        jne    short L_00728
        cmp    word ptr es:[di], 0786Fh
        jne    short L_00746
        cmp    word ptr es:[di + 2], 0
        jne    short L_00728
        jmp    short L_0072D
sub_0075A:
        cmp    byte ptr es:[di], 0
        je     short L_0076C
        cmp    byte ptr es:[di], 02Fh
        je     short L_00769
        inc    di
        jmp    short sub_0075A
L_00769:
        clc
        jmp    short L_0076D
L_0076C:
        stc
L_0076D:
        ret
sub_0076E:
        xor    di, di
        xor    al, al
L_00772:
        cmp    byte ptr es:[di], 0
        je     short L_00789
        push   cx
        push   si
        repe cmpsb
        pop    si
        pop    cx
        je     short L_0078C
        push   cx
        mov    cx, 08000h
        repne scasb
        pop    cx
        je     short L_00772
L_00789:
        stc
        jmp    short L_0078D
L_0078C:
        clc
L_0078D:
        ret
sub_0078E:
        mov    bx, cx
        add    bx, 0Fh
        rcr    bx, 1
        shr    bx, 1
        shr    bx, 1
        shr    bx, 1
        mov    ah, 048h
        int    021h    ; DOS_AllocateMemory(BX = paragraphs -> AX = segment)
        jb     short L_007A5
        mov    es, ax
        xor    di, di
L_007A5:
        ret
sub_007A6:
        mov    ah, 062h
        int    021h    ; DOS_GetProgramPrefix(-> BX = segment)
        mov    es, bx
        mov    es, word ptr es:[02Ch]
        xor    di, di
        mov    ax, di
        cld
L_007B6:
        scasb
        je     short L_007BE
L_007B9:
        scasb
        jne    short L_007B9
        jmp    short L_007B6
L_007BE:
        mov    cx, di
        add    cx, 0A0h
        push   es
        push   di
        call   near ptr sub_0078E
        pop    cx
        pop    ds
        jb     short L_007DB
        push   di
        xor    si, si
        rep movsb
        xor    ax, ax
        mov    cx, 050h
        rep stosw
        pop    di
        clc
L_007DB:
        ret
sub_007DC:
        cld
L_007DD:
        cmp    byte ptr es:[di], 0
        je     short L_007F4
        push   cx
        push   di
        push   si
        repe cmpsb
        pop    si
        pop    di
        pop    cx
        je     short L_007F5
        xor    ax, ax
L_007EF:
        scasb
        jne    short L_007EF
        jmp    short L_007DD
L_007F4:
        stc
L_007F5:
        ret
sub_007F6:
        call   near ptr sub_007DC
        jb     short L_00814
        mov    si, es
        mov    ds, si
        mov    si, di
L_00801:
        lodsb
        or     al, al
        jne    short L_00801
L_00806:
        lodsb
        stosb
        or     al, al
        je     short L_00814
L_0080C:
        lodsb
        stosb
        or     al, al
        jne    short L_0080C
        jmp    short L_00806
L_00814:
        ret
sub_00815:
        cld
        mov    cx, si
L_00818:
        lodsb
        cmp    al, 03Dh
        jne    short L_00818
        xchg   si, cx
        sub    cx, si
        push   di
        push   ds
        push   si
        call   near ptr sub_007F6
        pop    si
        pop    ds
        pop    di
        xor    ax, ax
L_0082C:
        scasb
        je     short L_00834
L_0082F:
        scasb
        jne    short L_0082F
        jmp    short L_0082C
L_00834:
        dec    di
L_00835:
        lodsb
        stosb
        or     al, al
        jne    short L_00835
        stosb
        ret
sub_0083D:
        mov    bx, 0FFFFh
        mov    cx, 5
        mov    dx, 0FFFFh
        mov    di, ss
        mov    es, di
        mov    di, 035Bh
        mov    ax, 06507h
        int    021h    ; DOS_GetExtendedCountryInformation(AL = information wanted, BX = code page, DX = country, ES:DI = buffer)
        cld
L_00853:
        lodsb
L_00854:
        or     al, al
        je     short L_00879
        cmp    al, 0Dh
        je     short L_00879
        cmp    al, 02Fh
        je     short L_00868
        call   near ptr sub_0087B
        jb     short L_00853
        inc    si
        jmp    short L_00853
L_00868:
        lodsb
        cmp    al, 02Dh
        jne    short L_0086E
        lodsb
L_0086E:
        cmp    al, 059h
        je     short L_00876
        cmp    al, 079h
        jne    short L_00854
L_00876:
        clc
        jmp    short L_0087A
L_00879:
        stc
L_0087A:
        ret
sub_0087B:
        cld
        push   ax
        push   cx
        push   dx
        push   si
        push   ds
        mov    cx, ss
        mov    ds, cx
        db   036h, 0C5h, 036h, 05Ch, 003h    ; lds    si, ptr ss:[035Ch]
        mov    dl, al
        lodsw
        mov    cx, ax
        jcxz   L_0089C
L_00891:
        lodsw
        cmp    dl, al
        jb     short L_0089A
        cmp    ah, dl
        jae    short L_0089D
L_0089A:
        loop   L_00891
L_0089C:
        stc
L_0089D:
        pop    ds
        pop    si
        pop    dx
        pop    cx
        pop    ax
        ret
sub_008A3:
        mov    ah, 062h
        int    021h    ; DOS_GetProgramPrefix(-> BX = segment)
        mov    ds, bx
        mov    si, 081h
        call   near ptr sub_0083D
        jae    short L_008DC
        mov    ah, 062h
        int    021h    ; DOS_GetProgramPrefix(-> BX = segment)
        mov    es, bx
        cmp    byte ptr es:[080h], 07Fh
        jne    short L_008DC
        mov    es, word ptr es:[02Ch]
        mov    si, ss
        mov    ds, si
        mov    si, 04ACh
        mov    cx, 8
        call   near ptr sub_0076E
        jb     short L_008DC
        mov    si, es
        mov    ds, si
        mov    si, di
        call   near ptr sub_0083D
L_008DC:
        ret
sub_008DD:
        cld
        xor    ax, ax
        mov    cx, di
        push   di
L_008E3:
        scasb
        jne    short L_008E3
        sub    di, cx
        mov    cx, di
        pop    di
        dec    cx
        push   si
L_008ED:
        inc    cx
        lodsb
        or     al, al
        jne    short L_008ED
        pop    si
        push   ds
        push   si
        push   es
        push   di
        call   near ptr sub_0078E
        pop    si
        pop    ds
        jb     short L_00918
        mov    cx, di
L_00901:
        lodsb
        stosb
        or     al, al
        jne    short L_00901
        dec    di
        pop    si
        pop    ds
L_0090A:
        lodsb
        stosb
        or     al, al
        jne    short L_0090A
        mov    si, es
        mov    ds, si
        mov    si, cx
        clc
L_00917:
        ret
L_00918:
        add    sp, 4
        stc
        jmp    short L_00917
sub_0091E:
        mov    ah, 062h
        int    021h    ; DOS_GetProgramPrefix(-> BX = segment)
        mov    ds, bx
        mov    ds, word ptr ds:[02Ch]
        xor    si, si
        cld
L_0092B:
        lodsb
        or     al, al
        je     short L_00937
L_00930:
        lodsb
        or     al, al
        jne    short L_00930
        jmp    short L_0092B
L_00937:
        lodsw
        mov    di, ss
        mov    es, di
        mov    di, 0404h
L_0093F:
        mov    bx, di
L_00941:
        lodsb
        stosb
        cmp    al, 05Ch
        je     short L_0093F
        or     al, al
        jne    short L_00941
        mov    si, ss
        mov    ds, si
        mov    si, 04B4h
        mov    di, bx
L_00954:
        lodsb
        stosb
        or     al, al
        jne    short L_00954
        ret
entry:
        mov    ax, seg S00A9
        cli
        mov    ss, ax
        mov    sp, 06D0h
        sti
        mov    ds, ax
        push   es
        call   near ptr sub_00000
        pop    es
        jae    short L_00971
        db   0E9h, 022h, 001h                ; jmp    0A93h
L_00971:
        mov    word ptr ds:[034Dh], 0
        mov    word ptr ds:[0351h], es
        mov    word ptr ds:[034Fh], 080h
        mov    word ptr ds:[0355h], es
        mov    word ptr ds:[0353h], 05Ch
        mov    word ptr ds:[0359h], es
        mov    word ptr ds:[0357h], 06Ch
        call   near ptr sub_008A3
        jae    short L_009A5
        call   near ptr sub_006FA
        cmp    byte ptr ss:[0362h], 0
        jne    short L_009A8
L_009A5:
        jmp    near ptr L_00A63
L_009A8:
        call   near ptr sub_007A6
        jb     short L_009CD
        mov    word ptr ss:[034Dh], es
        mov    si, ss
        mov    ds, si
        mov    si, 04ACh
        mov    cx, 8
        call   near ptr sub_007DC
        jb     short L_009E0
        mov    si, ss
        mov    ds, si
        mov    si, 04C0h
        call   near ptr sub_008DD
        jae    short L_009D0
L_009CD:
        jmp    near ptr L_00A83
L_009D0:
        mov    es, word ptr ss:[034Dh]
        xor    di, di
        call   near ptr sub_00815
        mov    byte ptr ss:[0363h], 1
L_009E0:
        mov    ah, 062h
        int    021h    ; DOS_GetProgramPrefix(-> BX = segment)
        mov    ds, bx
        mov    si, 080h
        mov    di, ss
        mov    es, di
        mov    di, 0364h
        xor    ax, ax
        lodsb
        mov    cx, ax
        add    al, 4
        jno    short L_009FB
        mov    al, 07Fh
L_009FB:
        stosb
        push   cx
        rep movsb
        pop    cx
        mov    si, ss
        mov    ds, si
        mov    si, 04C0h
        sub    cx, ax
        neg    cx
        rep movsb
        mov    byte ptr es:[di], 0Dh
        mov    word ptr ss:[034Fh], 0364h
        mov    word ptr ss:[0351h], es
        cmp    al, 07Fh
        jb     short L_00A63
        cmp    byte ptr ss:[0363h], 0
        jne    short L_00A63
        push   ds
        push   si
        mov    si, es
        mov    ds, si
        mov    si, 04ACh
        mov    cx, 8
        rep movsb
        mov    si, 04C5h
        mov    cx, 5
        rep movsb
        pop    si
        pop    ds
        mov    cx, ax
        xor    ch, ch
        rep movsb
        mov    si, ss
        mov    ds, si
        mov    si, 04C0h
        mov    cx, 4
        rep movsb
        xor    ax, ax
        stosb
        mov    si, 0364h
        mov    es, word ptr ss:[034Dh]
        xor    di, di
        call   near ptr sub_00815
L_00A63:
        call   near ptr sub_0091E
        mov    dx, ss
        mov    ds, dx
        mov    dx, 0404h
        mov    bx, ds
        mov    es, bx
        mov    bx, 034Dh
        mov    ax, 04B00h
        int    021h    ; DOS_LoadAndExecute(DS:DX = path, ES:BX = parameter block)
        jb     short L_00A88
        mov    ah, 04Dh
        int    021h    ; DOS_GetReturnCode(-> AL = code, AH = how it ended)
        mov    ah, 04Ch
        int    021h    ; DOS_Exit(AL = return code)
L_00A83:
        mov    ax, 3
        jmp    short L_00A8B
L_00A88:
        mov    ax, 2
L_00A8B:
        mov    bx, 2
        xor    cx, cx
S0000 ends

S00A9 segment para public use16 'C00A9'
        assume cs:S00A9, ds:nothing, es:nothing, ss:nothing
        public _codebreaker_seg_00a9
_codebreaker_seg_00a9 label byte
        mov    dx, 0300h
        db   0E8h, 044h, 0F8h                ; call   02DAh
        mov    ax, 04C01h
        int    021h    ; DOS_Exit(AL = return code)
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh
        db   0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   00Dh, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 02Ch, 000h, 02Eh, 000h
        db   02Dh, 000h, 03Ah, 000h, 001h
        db   '$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$'
        db   '$$$$$'
        db   000h, 000h, 000h, 000h, 045h, 04Eh, 055h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   'LANG=LANGSPEC='
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   'MS-DOS Version 7 (C)Copyright 1981-1995 Microsoft Corp Licen'
        db   'sed Material - Property of Microsoft All rights reserved '
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 060h
        db   003h
        dw   seg S00A9
        db   000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h
        db   'COPYCMD=CMDLINE=XCOPY32.EXE'
        db   000h, 020h, 02Fh, 02Dh, 059h, 000h
        db   'XCOPYYYYYYY'
S00A9 ends

STK segment para stack use16 'STACK'
        db   512 dup (?)
STK ends

end entry
