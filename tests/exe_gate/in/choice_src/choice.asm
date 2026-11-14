; choice.com -- reconstructed from the program image by CodeBreaker.
;
; One segment for each segment the program names. A word holding a segment is written as
; that segment, so the program can be loaded anywhere; offsets inside a segment are the
; original's numbers. Labels are L_<position in the load module>. Instructions written as
; bytes are ones the assembler would encode differently; their mnemonic is in the comment.

.386p

S0000 segment para public use16 'C0000'
        assume cs:S0000, ds:nothing, es:nothing, ss:nothing
        org  256
entry:
        jmp    near ptr L_00211
        db   020h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
L_00211:
        cld
        call   near ptr sub_00AD1
        mov    ax, 7
        mov    dh, 3
        call   near ptr sub_00D54
        mov    di, 0103h
        movsw
        mov    al, 1
        mov    ah, 030h
        int    021h    ; DOS_GetVersion(-> AL = major, AH = minor)
        cmp    al, 3
        jg     short L_00236
        mov    bx, 1
        call   near ptr sub_0049F
        mov    al, 0FFh
        jmp    near ptr L_0049B
L_00236:
        push   es
        mov    ax, 06502h
        mov    bx, 0FFFFh
        mov    cx, 5
        mov    dx, bx
        push   cs
        mov    di, 020Bh
        int    021h    ; DOS_GetExtendedCountryInformation(AL = information wanted, BX = code page, DX = country, ES:DI = buffer)
        inc    di
        db   0C4h, 03Dh                      ; les    di, ptr [di]
        mov    bx, word ptr es:[di]
        mov    ax, 0100h
        sub    ax, bx
        mov    word ptr ds:[020Fh], ax
        add    di, 2
        mov    word ptr ds:[020Bh], di
        mov    word ptr ds:[020Dh], es
        pop    es
        mov    si, 081h
L_00265:
        lodsb
        cmp    al, 0Dh
        jne    short L_0026D
        jmp    near ptr L_003B1
L_0026D:
        cmp    al, 02Fh
        je     short L_002B2
        cmp    al, 020h
        je     short L_00265
        cmp    al, 9
        je     short L_00265
        mov    di, 018Bh
        cmp    byte ptr [di], 0
        jne    short L_002A1
        cmp    al, 022h
        jne    short L_00289
        mov    dl, 022h
        jmp    short L_0028C
L_00289:
        mov    dl, 02Fh
        stosb
L_0028C:
        mov    al, byte ptr [si]
        cmp    al, 0Dh
        je     short L_00265
        cmp    al, dl
        je     short L_0029A
        stosb
        inc    si
        jmp    short L_0028C
L_0029A:
        cmp    al, 02Fh
        je     short L_00265
        inc    si
        jmp    short L_00265
L_002A1:
        mov    bx, 6
        call   near ptr sub_0049F
        mov    bx, 065h
        call   near ptr sub_0049F
        mov    al, 0FFh
        jmp    near ptr L_0049B
L_002B2:
        lodsb
        call   near ptr sub_004B7
        cmp    al, 03Fh
        je     short L_002DE
        cmp    al, 043h
        je     short L_00301
        cmp    al, 04Eh
        je     short L_0033B
        cmp    al, 054h
        je     short L_00343
        cmp    al, 053h
        jne    short L_002CD
        jmp    near ptr L_003A9
L_002CD:
        mov    bx, 2
        call   near ptr sub_0049F
        mov    bx, 065h
        call   near ptr sub_0049F
        mov    al, 0FFh
        jmp    near ptr L_0049B
L_002DE:
        mov    bx, 064h
        call   near ptr sub_0049F
        mov    bx, 065h
        call   near ptr sub_0049F
        mov    bx, 066h
        call   near ptr sub_0049F
        mov    bx, 067h
        call   near ptr sub_0049F
        mov    bx, 068h
        call   near ptr sub_0049F
        mov    al, 0FFh
        jmp    near ptr L_0049B
L_00301:
        mov    di, 0103h
        mov    al, byte ptr [si]
        cmp    al, 03Ah
        jne    short L_0030D
        inc    si
        mov    al, byte ptr [si]
L_0030D:
        cmp    al, 020h
        je     short L_00323
        cmp    al, 9
        je     short L_00323
        cmp    al, 0Dh
        je     short L_00323
        cmp    al, 02Fh
        je     short L_00323
        stosb
        inc    si
        mov    al, byte ptr [si]
        jmp    short L_0030D
L_00323:
        mov    al, 0
        mov    byte ptr [di], al
        cmp    di, 0103h
        je     short L_00330
        jmp    near ptr L_00265
L_00330:
        mov    bx, 3
        call   near ptr sub_0049F
        mov    al, 0FFh
        jmp    near ptr L_0049B
L_0033B:
        mov    byte ptr ds:[0187h], 0
        jmp    near ptr L_00265
L_00343:
        mov    al, byte ptr [si]
        cmp    al, 03Ah
        jne    short L_0034C
        inc    si
        mov    al, byte ptr [si]
L_0034C:
        cmp    al, 0Dh
        je     short L_0039E
        cmp    al, 02Fh
        je     short L_0039E
        cmp    al, 020h
        je     short L_0039E
        cmp    al, 9
        je     short L_0039E
        mov    byte ptr ds:[018Ah], al
        inc    si
        mov    al, byte ptr [si]
        cmp    al, 02Ch
        jne    short L_0039E
        inc    si
        mov    al, byte ptr [si]
        cmp    al, 030h
        jb     short L_0039E
        cmp    al, 039h
        ja     short L_0039E
        and    ax, 0Fh
        mov    byte ptr ds:[0189h], al
        inc    si
        mov    al, byte ptr [si]
        cmp    al, 030h
        jae    short L_00381
        jmp    near ptr L_00265
L_00381:
        cmp    al, 039h
        jbe    short L_00388
        jmp    near ptr L_00265
L_00388:
        mov    al, byte ptr ds:[0189h]
        mov    dx, 0Ah
        mul    dl
        mov    bl, byte ptr [si]
        and    bl, 0Fh
        add    al, bl
        mov    byte ptr ds:[0189h], al
        inc    si
        jmp    near ptr L_00265
L_0039E:
        mov    bx, 4
        call   near ptr sub_0049F
        mov    al, 0FFh
        jmp    near ptr L_0049B
L_003A9:
        mov    byte ptr ds:[0188h], 1
        jmp    near ptr L_00265
L_003B1:
        cmp    byte ptr ds:[0188h], 1
        je     short L_003D3
        mov    al, byte ptr ds:[018Ah]
        call   near ptr sub_004B7
        mov    byte ptr ds:[018Ah], al
        mov    si, 0103h
L_003C4:
        cmp    byte ptr [si], 0
        je     short L_003D3
        mov    al, byte ptr [si]
        call   near ptr sub_004B7
        mov    byte ptr [si], al
        inc    si
        jmp    short L_003C4
L_003D3:
        mov    di, 0103h
        mov    cx, 080h
        mov    al, byte ptr ds:[018Ah]
        repne scasb
        je     short L_003EB
        mov    bx, 5
        call   near ptr sub_0049F
        mov    al, 0FFh
        jmp    near ptr L_0049B
L_003EB:
        mov    si, 018Bh
        lodsb
        jmp    short L_003F8
L_003F1:
        mov    ah, 2
        mov    dl, al
        int    021h    ; DOS_WriteCharacter(DL = character)
        lodsb
L_003F8:
        or     al, al
        jne    short L_003F1
        cmp    byte ptr ds:[0187h], 1
        jne    short L_0042C
        mov    di, 0103h
        mov    ah, 2
        mov    dl, 05Bh
        int    021h    ; DOS_WriteCharacter(DL = character)
L_0040C:
        mov    ah, 2
        mov    dl, byte ptr [di]
        int    021h    ; DOS_WriteCharacter(DL = character)
        inc    di
        cmp    byte ptr [di], 0
        je     short L_00420
        mov    ah, 2
        mov    dl, 02Ch
        int    021h    ; DOS_WriteCharacter(DL = character)
        jmp    short L_0040C
L_00420:
        mov    ah, 2
        mov    dl, 05Dh
        int    021h    ; DOS_WriteCharacter(DL = character)
        mov    ah, 2
        mov    dl, 03Fh
        int    021h    ; DOS_WriteCharacter(DL = character)
L_0042C:
        cmp    byte ptr ds:[0189h], 0
        je     short L_00460
        mov    ah, 02Ch
        int    021h    ; DOS_GetTime(-> CH = hours, CL = minutes, DH = seconds, DL = hundredths)
        mov    byte ptr ds:[0186h], dh
L_0043B:
        mov    ah, 0Bh
        int    021h    ; DOS_GetInputStatus(-> AL = FFh when a character waits)
        and    ax, 1
        or     ax, 0
        jne    short L_00460
        mov    ah, 02Ch
        int    021h    ; DOS_GetTime(-> CH = hours, CL = minutes, DH = seconds, DL = hundredths)
        cmp    byte ptr ds:[0186h], dh
        je     short L_0043B
        mov    byte ptr ds:[0186h], dh
        dec    byte ptr ds:[0189h]
        jne    short L_0043B
        mov    al, byte ptr ds:[018Ah]
        jmp    short L_00464
L_00460:
        mov    ah, 8
        int    021h    ; DOS_ReadCharacterNoEcho(-> AL = character)
L_00464:
        mov    di, 0103h
        cmp    byte ptr ds:[0188h], 1
        je     short L_00471
        call   near ptr sub_004B7
L_00471:
        cmp    byte ptr [di], 0
        je     short L_0047B
        scasb
        je     short L_00483
        jmp    short L_00471
L_0047B:
        mov    ah, 2
        mov    dl, 7
        int    021h    ; DOS_WriteCharacter(DL = character)
        jmp    short L_00460
L_00483:
        mov    ah, 2
        mov    dl, al
        int    021h    ; DOS_WriteCharacter(DL = character)
        mov    ah, 2
        mov    dl, 0Dh
        int    021h    ; DOS_WriteCharacter(DL = character)
        mov    ah, 2
        mov    dl, 0Ah
        int    021h    ; DOS_WriteCharacter(DL = character)
        sub    di, 0103h
        mov    ax, di
L_0049B:
        mov    ah, 04Ch
        int    021h    ; DOS_Exit(AL = return code)
sub_0049F:
        push   ax
        push   bx
        push   si
        push   di
        push   dx
        mov    ax, bx
        mov    bx, 1
        xor    cx, cx
        mov    dx, 0300h
        call   near ptr sub_00D6D
        pop    dx
        pop    di
        pop    si
        pop    bx
        pop    ax
        ret
sub_004B7:
        push   bx
        push   di
        push   es
        cmp    al, 061h
        jb     short L_004D9
        cmp    al, 07Ah
        ja     short L_004C6
        sub    al, 020h
        jmp    short L_004D9
L_004C6:
        cmp    al, 080h
        jb     short L_004D9
        db   0C4h, 03Eh, 00Bh, 002h          ; les    di, ptr ds:[020Bh]
        mov    bx, ax
        xor    bh, bh
        sub    bx, word ptr ds:[020Fh]
        mov    al, byte ptr es:[bx + di]
L_004D9:
        pop    es
        pop    di
        pop    bx
        ret
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh
        db   0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 00Dh, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 02Ch, 000h
        db   02Eh, 000h, 02Dh, 000h, 03Ah, 000h, 001h
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
sub_00AD1:
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
        mov    word ptr ds:[04E6h], es
        mov    word ptr ds:[04E4h], di
        mov    dl, 6
        int    02Fh    ; Multiplex
        mov    word ptr ds:[04EAh], es
        mov    word ptr ds:[04E8h], di
        mov    ax, 0122Eh
        mov    dl, 2
        int    02Fh    ; Multiplex_GetOrSetErrorTable(DL = which)
        mov    word ptr ds:[04F2h], es
        mov    word ptr ds:[04F0h], di
        mov    ax, 0122Eh
        mov    dl, 4
        int    02Fh    ; Multiplex_GetOrSetErrorTable(DL = which)
        mov    word ptr ds:[0502h], es
        mov    word ptr ds:[0500h], di
        mov    es, bx
        mov    es, word ptr es:[02Ch]
        mov    di, 0
        call   near ptr sub_00CEE
        push   ds
        push   es
        pop    ds
        mov    dx, di
        add    dx, 3
        mov    ax, 03D00h
        int    021h    ; DOS_OpenFile(AL = access mode, DS:DX = path -> AX = handle)
        pop    ds
        jb     short L_00B73
        mov    bx, ax
        mov    word ptr ds:[0577h], bx
        call   near ptr sub_00B7F
        pushf
        mov    ax, 03E00h
        mov    bx, word ptr ds:[0577h]
        int    021h    ; DOS_CloseFile(BX = handle)
        popf
        jb     short L_00B79
        mov    ax, 0122Eh
        mov    dl, 8
        int    02Fh    ; Multiplex_GetOrSetErrorTable(DL = which)
        mov    word ptr ds:[0516h], es
        mov    word ptr ds:[0514h], di
        mov    word ptr ds:[051Fh], 0A0Dh
        mov    byte ptr ds:[0574h], 024h
        mov    word ptr ds:[0523h], 0
        mov    word ptr ds:[0527h], 0Ah
L_00B73:
        pop    di
        pop    es
        pop    dx
        pop    bx
        pop    ax
L_00B78:
        ret
L_00B79:
        add    sp, 0Ah
        stc
        jmp    short L_00B78
sub_00B7F:
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
        mov    word ptr ds:[0580h], ax
        mov    word ptr ds:[0582h], dx
        mov    ax, 03F00h
        lea    dx, ds:[0592h]
        int    021h    ; DOS_ReadFile(BX = handle, CX = bytes, DS:DX = buffer -> AX = bytes read)
        jb     short L_00BC3
        cmp    word ptr ds:[0595h], 0534Eh
        jne    short L_00BC3
        mov    ax, word ptr ds:[0597h]
        mov    dx, word ptr ds:[0580h]
        mov    cx, word ptr ds:[0582h]
        sub    dx, ax
        sbb    cx, 0
        jmp    short L_00C07
L_00BC3:
        xor    cx, cx
        xor    dx, dx
        mov    ax, 04200h
        int    021h    ; DOS_SeekFile(AL = origin, BX = handle, CX:DX = offset -> DX:AX = position)
        mov    cx, 01Ah
        mov    ax, 03F00h
        lea    dx, ds:[0592h]
        int    021h    ; DOS_ReadFile(BX = handle, CX = bytes, DS:DX = buffer -> AX = bytes read)
        jb     short L_00C29
        mov    ax, word ptr ds:[0592h]
        cmp    ax, 05A4Dh
        jne    short L_00C29
        mov    ax, word ptr ds:[0598h]
        shl    ax, 1
        shl    ax, 1
        add    ax, word ptr ds:[05AAh]
        db   03Dh, 03Fh, 000h                ; cmp    ax, 03Fh
        jbe    short L_00C02
        db   005h, 00Fh, 000h                ; add    ax, 0Fh
        mov    cx, 4
        shr    ax, cl
        shl    ax, cl
        xor    cx, cx
        mov    dx, ax
        jmp    short L_00C07
L_00C02:
        xor    cx, cx
        mov    dx, 040h
L_00C07:
        mov    word ptr ds:[0580h], cx
        mov    word ptr ds:[0582h], dx
        mov    si, 1
        call   near ptr sub_00C2B
        jae    short L_00C26
        mov    cx, word ptr ds:[0580h]
        mov    dx, word ptr ds:[0582h]
        xor    si, si
        call   near ptr sub_00C2B
        jb     short L_00C29
L_00C26:
        call   near ptr sub_00CA8
L_00C29:
        pop    es
        ret
sub_00C2B:
        push   cx
        push   dx
        mov    ax, 04200h
        int    021h    ; DOS_SeekFile(AL = origin, BX = handle, CX:DX = offset -> DX:AX = position)
        mov    cx, 010h
        mov    ah, 03Fh
        lea    dx, ds:[0592h]
        int    021h    ; DOS_ReadFile(BX = handle, CX = bytes, DS:DX = buffer -> AX = bytes read)
        jb     short L_00C76
        cmp    ax, cx
        jne    short L_00C76
        mov    ax, word ptr ds:[059Dh]
        cmp    ax, 0534Eh
        jne    short L_00C76
        or     si, si
        je     short L_00C7B
        lea    si, ds:[0596h]
        push   es
        push   ds
        pop    es
        lea    di, ds:[0579h]
        mov    cx, 3
        repe cmpsb
        pop    es
        je     short L_00C7B
        pop    dx
        pop    cx
        add    dx, word ptr ds:[0592h]
        adc    cx, 0
        add    dx, 0Fh
        adc    cx, 0
        and    dl, 0F0h
        jmp    short sub_00C2B
L_00C76:
        pop    dx
        pop    cx
        stc
        jmp    short L_00CA4
L_00C7B:
        pop    dx
        pop    cx
        push   bx
        mov    bx, word ptr ds:[0594h]
        sub    bx, 010h
        mov    ax, 04B0h
        sub    ax, bx
        pop    bx
        js     short L_00CA5
        mov    cx, word ptr ds:[0592h]
        sub    cx, 010h
        push   ds
        mov    ax, ds
        mov    es, ax
        lea    dx, ds:[05ACh]
        mov    ah, 03Fh
        int    021h    ; DOS_ReadFile(BX = handle, CX = bytes, DS:DX = buffer -> AX = bytes read)
        pop    ds
        jb     short L_00CA5
L_00CA4:
        ret
L_00CA5:
        stc
        jmp    short L_00CA4
sub_00CA8:
        mov    di, dx
        mov    cl, byte ptr es:[di]
        sub    cl, 2
        inc    di
        mov    ax, word ptr es:[di]
        add    ax, di
        mov    word ptr ds:[04E0h], ax
        mov    word ptr ds:[04E2h], es
        mov    word ptr ds:[04FEh], es
        mov    word ptr ds:[04FCh], ax
        add    di, 2
        mov    ax, word ptr es:[di]
        add    ax, di
        mov    word ptr ds:[04F6h], es
        mov    word ptr ds:[04F4h], ax
        mov    si, 0
L_00CD6:
        add    di, 2
        mov    ax, word ptr es:[di]
        add    ax, di
        mov    word ptr [si + 0508h], ax
        mov    word ptr [si + 050Ah], es
        add    si, 4
        dec    cl
        jne    short L_00CD6
        ret
sub_00CEE:
        push   si
        push   di
        lea    si, ds:[0584h]
        mov    bx, 5
        xor    ax, ax
L_00CF9:
        mov    cx, bx
        push   si
        repe cmpsb
        pop    si
        je     short L_00D0D
L_00D01:
        mov    cx, 0FFFFh
        repne scasb
        cmp    byte ptr es:[di], al
        je     short L_00D23
        jmp    short L_00CF9
L_00D0D:
        lea    si, ds:[0579h]
        mov    ax, word ptr es:[di]
        mov    word ptr [si], ax
        add    si, 2
        mov    al, byte ptr es:[di + 2]
        mov    byte ptr [si], al
        xor    ax, ax
        jmp    short L_00D01
L_00D23:
        pop    di
        lea    si, ds:[0589h]
        mov    bx, 9
        xor    ax, ax
L_00D2D:
        mov    cx, bx
        push   si
        repe cmpsb
        pop    si
        je     short L_00D41
L_00D35:
        mov    cx, 0FFFFh
        repne scasb
        cmp    byte ptr es:[di], al
        je     short L_00D52
        jmp    short L_00D2D
L_00D41:
        lea    si, ds:[057Ch]
        mov    word ptr [si], di
        add    si, 2
        mov    ax, es
        mov    word ptr [si], ax
        xor    ax, ax
        jmp    short L_00D35
L_00D52:
        pop    si
        ret
sub_00D54:
        push   ax
        push   es
        push   di
        push   bp
        call   near ptr sub_01036
        jb     short L_00D68
        push   es
        pop    ds
        mov    si, di
        xor    ax, ax
L_00D63:
        pop    bp
        pop    di
        pop    es
        pop    ax
        ret
L_00D68:
        mov    ax, 0FFFFh
        jmp    short L_00D63
sub_00D6D:
        push   bx
        push   cx
        push   bp
        push   ax
        push   di
        push   es
        push   dx
        mov    bp, cx
        mov    word ptr ds:[051Ch], bx
        call   near ptr sub_01036
        jcxz   L_00D96
        or     bp, bp
        jne    short L_00D88
        call   near ptr sub_00DAA
        jmp    short L_00D8B
L_00D88:
        call   near ptr sub_00E9F
L_00D8B:
        jb     short L_00D96
        pop    dx
        call   near ptr sub_00E31
        pop    es
        pop    di
        pop    ax
        jmp    short L_00D9B
L_00D96:
        pop    dx
        pop    es
        pop    di
        pop    ax
        stc
L_00D9B:
        jb     short L_00DA2
        pop    bp
        pop    cx
        pop    bx
        jmp    short L_00DA9
L_00DA2:
        add    sp, 6
        stc
        mov    ax, 0FFFFh
L_00DA9:
        ret
sub_00DAA:
        push   ax
        push   bx
        push   dx
        mov    bx, word ptr ds:[051Ch]
        cmp    bx, -1
        jne    short L_00DBB
        call   near ptr sub_00E11
        jmp    short L_00DBE
L_00DBB:
        call   near ptr sub_00DE6
L_00DBE:
        jae    short L_00DCB
        mov    ah, 059h
        mov    bx, 0
        int    021h    ; DOS_GetExtendedError(BX = 0 -> AX = error, BH = class, BL = action, CH = locus)
        xor    ah, ah
        jmp    short L_00DDC
L_00DCB:
        cmp    bx, -1
        je     short L_00DE2
        cmp    ax, cx
        je     short L_00DE2
        mov    ax, 027h
        mov    bx, 4
        mov    ch, 1
L_00DDC:
        add    sp, 6
        stc
        jmp    short L_00DE5
L_00DE2:
        pop    dx
        pop    bx
        pop    ax
L_00DE5:
        ret
sub_00DE6:
        xor    ax, ax
        jcxz   L_00E10
        push   ds
        push   es
        pop    ds
        mov    ah, 040h
        mov    dx, di
        cmp    cx, 1
        jne    short L_00DFA
        int    021h    ; DOS
        jmp    short L_00DFC
L_00DFA:
        int    021h    ; DOS
L_00DFC:
        jb     short L_00E0F
        cmp    cx, ax
        je     short L_00E0F
        push   di
        add    di, ax
        cmp    byte ptr es:[di], 01Ah
        pop    di
        clc
        jne    short L_00E0F
        mov    ax, cx
L_00E0F:
        pop    ds
L_00E10:
        ret
sub_00E11:
        push   ds
        push   es
        pop    ds
        cmp    cx, 1
        jne    short L_00E22
        mov    ah, 2
        mov    dl, byte ptr es:[di]
        int    021h    ; DOS_WriteCharacter(DL = character)
        jmp    short L_00E2E
L_00E22:
        mov    ah, 2
        jcxz   L_00E2E
L_00E26:
        mov    dl, byte ptr es:[di]
        int    021h    ; DOS
        inc    di
        loop   L_00E26
L_00E2E:
        clc
        pop    ds
        ret
sub_00E31:
        cmp    dh, 3
        je     short L_00E47
        test   dh, 080h
        jne    short L_00E47
        push   ds
        pop    es
        lea    di, ds:[051Fh]
        mov    cx, 2
        call   near ptr sub_00DAA
L_00E47:
        ret
sub_00E48:
        pop    word ptr ds:[0521h]
        xor    bx, bx
        xchg   bx, ax
        xchg   dx, ax
L_00E50:
        div    word ptr ds:[0527h]
        xchg   bx, ax
        div    word ptr ds:[0527h]
        cmp    dx, 9
        jbe    short L_00E63
        add    dl, 037h
        jmp    short L_00E66
L_00E63:
        add    dl, 030h
L_00E66:
        push   dx
        inc    cx
        or     ax, ax
        jne    short L_00E70
        or     bx, bx
        je     short L_00E96
L_00E70:
        cmp    cx, 3
        jne    short L_00E87
L_00E75:
        push   ax
        mov    al, byte ptr [si + 7]
        and    al, 0Fh
        cmp    al, 2
        pop    ax
        ja     short L_00E91
        push   word ptr ds:[052Bh]
        inc    cx
        jmp    short L_00E91
L_00E87:
        cmp    cx, 7
        je     short L_00E75
        cmp    cx, 0Bh
        je     short L_00E75
L_00E91:
        xchg   bx, ax
        xor    dx, dx
        jmp    short L_00E50
L_00E96:
        xor    dx, dx
        xor    ax, ax
        push   word ptr ds:[0521h]
        ret
sub_00E9F:
        mov    word ptr ds:[0525h], bp
L_00EA3:
        or     cx, cx
        jne    short L_00EAA
        jmp    short L_00F1A
        db   090h
L_00EAA:
        xor    dx, dx
L_00EAC:
        mov    al, byte ptr es:[di]
        cmp    al, 025h
        je     short L_00ED0
        jmp    short L_00EB8
L_00EB5:
        inc    di
        inc    dx
        dec    cx
L_00EB8:
        inc    di
        inc    dx
        loop   L_00EAC
L_00EBC:
        xchg   dx, cx
        jcxz   L_00EA3
        push   di
        sub    di, cx
        call   near ptr sub_00DAA
        pop    di
        jae    short L_00ECC
        jmp    short L_00F2B
        db   090h
L_00ECC:
        xchg   dx, cx
        jmp    short L_00EA3
L_00ED0:
        mov    al, byte ptr es:[di + 1]
        cmp    al, 025h
        je     short L_00EB5
        sub    al, 030h
        xor    ah, ah
        cmp    ax, word ptr ds:[0525h]
        ja     short L_00EB5
        or     dx, dx
        jne    short L_00EBC
        dec    bp
L_00EE7:
        push   si
L_00EE8:
        dec    al
        je     short L_00EF2
        mov    dl, byte ptr [si]
        add    si, dx
        jmp    short L_00EE8
L_00EF2:
        push   bp
        push   di
        push   es
        push   cx
        xor    cx, cx
        cmp    word ptr ds:[0523h], 0
        jne    short L_00F04
        call   near ptr sub_00F5F
        jmp    short L_00F07
L_00F04:
        call   near ptr sub_00F32
L_00F07:
        pop    cx
        pop    es
        pop    di
        pop    bp
        pop    si
        cmp    word ptr ds:[0523h], 0
        jl     short L_00F1A
        inc    di
        mov    dx, 0FFFFh
        dec    cx
        jmp    short L_00EB8
L_00F1A:
        jb     short L_00F2B
        or     bp, bp
        je     short L_00F2B
        dec    bp
        mov    ax, word ptr ds:[0525h]
        sub    ax, bp
        mov    cx, 2
        jmp    short L_00EE7
L_00F2B:
        mov    word ptr ds:[0523h], 0
        ret
sub_00F32:
        xor    dx, dx
        mov    ax, word ptr ds:[0523h]
        mov    word ptr ds:[0527h], 0Ah
        call   near ptr sub_00E48
L_00F40:
        pop    ax
        mov    byte ptr [bx + 0534h], al
        inc    bx
        cmp    bx, 040h
        jne    short L_00F4E
        call   near ptr sub_0101A
L_00F4E:
        dec    cl
        jne    short L_00F40
        mov    ax, 0A0Dh
        mov    word ptr [bx + 0534h], ax
        inc    bx
        inc    bx
        call   near ptr sub_0101A
        ret
sub_00F5F:
        xor    bx, bx
        cmp    byte ptr [si + 6], 0
        jne    short L_00F78
        mov    word ptr [bx + 0534h], 02D20h
        inc    bx
        inc    bx
        mov    byte ptr [bx + 0534h], 020h
        inc    bx
        call   near ptr sub_0101A
L_00F78:
        pop    bp
        xor    bx, bx
        xor    dx, dx
        mov    byte ptr ds:[051Eh], cl
        mov    al, byte ptr [si + 9]
        cmp    al, cl
        jbe    short L_00FA6
        sub    al, cl
        mov    dh, al
        test   byte ptr [si + 7], 080h
        je     short L_00FA6
L_00F92:
        mov    al, byte ptr [si + 0Ah]
        mov    byte ptr [bx + 0534h], al
        inc    bx
        cmp    bx, 040h
        jne    short L_00FA2
        call   near ptr sub_0101A
L_00FA2:
        dec    dh
        jne    short L_00F92
L_00FA6:
        cmp    byte ptr [si + 8], 0
        je     short L_00FB9
        cmp    byte ptr [si + 8], cl
        jae    short L_00FB9
        sub    cl, byte ptr [si + 8]
        mov    dl, cl
        mov    cl, byte ptr [si + 8]
L_00FB9:
        jcxz   L_00FDF
L_00FBB:
        test   byte ptr [si + 7], 0Fh
        jne    short L_00FCD
        test   byte ptr [si + 7], 010h
        je     short L_00FCD
        mov    al, byte ptr es:[di]
        inc    di
        jmp    short L_00FCE
L_00FCD:
        pop    ax
L_00FCE:
        mov    byte ptr [bx + 0534h], al
        inc    bx
        cmp    bx, 040h
        jne    short L_00FDB
        call   near ptr sub_0101A
L_00FDB:
        dec    cl
        jne    short L_00FBB
L_00FDF:
        test   byte ptr [si + 7], 080h
        jne    short L_00FFD
        or     dh, dh
        je     short L_00FFD
L_00FE9:
        mov    al, byte ptr [si + 0Ah]
        mov    byte ptr [bx + 0534h], al
        inc    bx
        cmp    bx, 040h
        jne    short L_00FF9
        call   near ptr sub_0101A
L_00FF9:
        dec    dh
        jne    short L_00FE9
L_00FFD:
        test   byte ptr [si + 7], 0Fh
        jne    short L_01009
        test   byte ptr [si + 7], 010h
        jne    short L_01015
L_01009:
        or     dl, dl
        je     short L_01015
L_0100D:
        pop    word ptr ds:[0521h]
        dec    dl
        jne    short L_0100D
L_01015:
        call   near ptr sub_0101A
        push   bp
        ret
sub_0101A:
        push   cx
        push   es
        push   di
        push   ds
        pop    es
        mov    cx, bx
        xor    bx, bx
        lea    di, ds:[0534h]
        call   near ptr sub_00DAA
        jb     short L_01031
        pop    di
        pop    es
        pop    cx
        jmp    short L_01035
L_01031:
        add    sp, 6
        stc
L_01035:
        ret
sub_01036:
        push   si
        push   bx
        xor    si, si
        xor    cx, cx
        mov    word ptr ds:[0523h], 0
        cmp    dh, 0
        jne    short L_0104A
        jmp    short L_010B9
        db   090h
L_0104A:
        cmp    dh, 3
        jne    short L_01054
        mov    si, 028h
        jmp    short L_01069
L_01054:
        test   dh, 2
        je     short L_0105C
        mov    si, 010h
L_0105C:
        db   03Dh, 013h, 000h                ; cmp    ax, 013h
        jb     short L_01069
        db   03Dh, 027h, 000h                ; cmp    ax, 027h
        ja     short L_01069
        mov    si, 01Ch
L_01069:
        lea    bx, ds:[04E0h]
        add    si, bx
L_0106F:
        db   0C4h, 03Ch                      ; les    di, ptr [si]
        mov    bx, es
        cmp    bx, -1
        jne    short L_010A6
        cmp    dh, 3
        jne    short L_01085
        stc
        jmp    short L_010B9
L_01080:
        db   03Dh, 0FFh, 0FFh                ; cmp    ax, 0FFFFh
        je     short L_010B9
L_01085:
        cmp    dh, 1
        jne    short L_01091
        call   near ptr sub_010BC
        or     cx, cx
        jne    short L_010B8
L_01091:
        cmp    word ptr ds:[0523h], 0
        jne    short L_010B9
        mov    word ptr ds:[0523h], ax
        mov    ax, 0FFFFh
        mov    bp, 1
        xor    si, si
        clc
        jmp    short L_0104A
L_010A6:
        cmp    bx, 0
        je     short L_010AE
        call   near ptr sub_010EE
L_010AE:
        add    si, 4
        jcxz   L_0106F
        cmp    dh, 1
        je     short L_01080
L_010B8:
        clc
L_010B9:
        pop    bx
        pop    si
        ret
sub_010BC:
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
        jne    short L_010E8
        mov    bx, ax
        mov    ax, 0502h
        int    02Fh    ; Multiplex_CriticalErrorMessages(AL = function)
        jb     short L_010E8
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
        jmp    short L_010EB
L_010E8:
        pop    di
        pop    es
        pop    cx
L_010EB:
        pop    bp
        pop    dx
        ret
sub_010EE:
        cmp    bx, 1
        jne    short L_0110A
        cmp    word ptr ds:[0514h], -1
        je     short L_0110A
        db   03Dh, 0FFh, 0FFh                ; cmp    ax, 0FFFFh
        push   ax
        jne    short L_01103
        mov    ax, word ptr ds:[0523h]
L_01103:
        db   0FFh, 01Eh, 014h, 005h          ; lcall  ds:[0514h]
        pop    ax
        jmp    short L_0111E
L_0110A:
        xor    cx, cx
        cmp    dh, 3
        jne    short L_01116
        cmp    byte ptr es:[di], dh
        jne    short L_0111A
L_01116:
        mov    cl, byte ptr es:[di + 1]
L_0111A:
        add    di, 2
        stc
L_0111E:
        jae    short L_01134
        clc
L_01121:
        jcxz   L_01140
        cmp    ax, word ptr es:[di]
        je     short L_01130
        dec    cx
        je     short L_01140
        add    di, 4
        jmp    short L_01121
L_01130:
        add    di, word ptr es:[di + 2]
L_01134:
        xor    ch, ch
        mov    cl, byte ptr es:[di]
        inc    di
L_0113A:
        mov    byte ptr ds:[051Eh], 0
        ret
L_01140:
        stc
        jmp    short L_0113A
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0D8h, 003h, 0D8h
        db   003h, 045h, 04Eh, 055h, 001h, 000h, 0B5h, 001h, 04Eh, 053h, 043h, 048h, 000h, 003h, 006h, 000h
        db   00Ah, 000h, 00Eh, 000h, 001h, 001h, 0FFh, 0FFh, 040h, 000h, 002h, 001h, 0FFh, 0FFh, 04Ch, 000h
        db   003h, 00Dh, 001h, 000h, 055h, 000h, 002h, 000h, 081h, 000h, 003h, 000h, 0B2h, 000h, 004h, 000h
        db   0F2h, 000h, 005h, 000h, 031h, 001h, 006h, 000h, 06Eh, 001h, 007h, 000h, 0A7h, 001h, 064h, 000h
        db   0A6h, 001h, 065h, 000h, 0DCh, 001h, 066h, 000h, 00Fh, 002h, 067h, 000h, 086h, 002h, 068h, 000h
        db   0E9h, 002h, 0ACh, 026h, 04Eh, 003h, 011h
        db   'Extended Error %1'
        db   00Eh
        db   'Parse Error %1/CHOICE: requires MS-DOS version 4.0 or later.'
        db   00Dh, 00Ah
        db   '4Invalid switch on command line. Expected form:'
        db   00Dh, 00Ah
        db   '    CCHOICE: invalid choice switch syntax. Expected form: /C'
        db   '[:]choices'
        db   00Dh, 00Ah
        db   'BCHOICE: Incorrect timeout syntax.  Expected form Tc,nn or T'
        db   ':c,nn'
        db   00Dh, 00Ah
        db   '@CHOICE: Timeout default not in specified (or default) choic'
        db   'es.'
        db   00Dh, 00Ah
        db   '<CHOICE: only one prompt string allowed. Expected Form:'
        db   00Dh, 00Ah, 020h, 020h, 020h, 020h, 002h
        db   'YN9Waits for the user to choose one of a set of choices.'
        db   00Dh, 00Ah, 00Dh, 00Ah
        db   '6CHOICE [/C[:]choices] [/N] [/S] [/T[:]c,nn] [text]'
        db   00Dh, 00Ah, 00Dh, 00Ah
        db   'z/C[:]choices Specifies allowable keys. Default is YN'
        db   00Dh, 00Ah
        db   '/N           Do not display choices and ? at end of prompt s'
        db   'tring.'
        db   00Dh, 00Ah
        db   'f/S           Treat choice keys as case sensitive.'
        db   00Dh, 00Ah
        db   '/T[:]c,nn    Default choice to c after nn seconds'
        db   00Dh, 00Ah
        db   'htext         Prompt string to display'
        db   00Dh, 00Ah, 00Dh, 00Ah
        db   'ERRORLEVEL is set to offset of key user presses in choices.'
        db   00Dh, 00Ah, 00Dh, 00Ah, 034h, 0BDh, 004h, 0EFh, 0FEh, 000h, 000h, 001h, 000h, 000h, 000h, 004h
        db   000h, 000h, 001h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 001h, 000h, 000h, 03Fh, 000h, 000h
        db   000h, 001h, 000h, 000h, 000h, 00Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db   000h
        db   'ENUNS'
        db   0E7h, 003h
S0000 ends

end entry
