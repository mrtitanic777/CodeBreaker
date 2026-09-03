; nwserver_pcode_05_02.asm -- NWSERVER object 5 segment _PTEXT  (part 2/3)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_5_472B:
        push   ebx
        push   esi
        push   edi
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 004h, 04Dh, 0FBh, 0FFh
        add    esp, 4
        db  08Bh, 03Dh, 050h, 0A1h, 000h, 000h
        or     edi, edi
        je     SHORT loc_5_4779
        mov    esi, dword ptr [esp + 10h]
        lea    ebx, [esi + 1]
loc_5_474C:
        movzx  eax, byte ptr [edi + 0ch]
        push   eax
        lea    eax, [edi + 0dh]
        push   eax
        push   ebx
        db  0E8h, 0ADh, 0D0h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_4773
        mov    al, byte ptr [edi + 0ch]
        cmp    byte ptr [esi], al
        je     SHORT loc_5_478E
        movzx  eax, al
        cmp    byte ptr [esi + eax + 1], 3ah
        je     SHORT loc_5_478E
loc_5_4773:
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_5_474C
loc_5_4779:
        mov    bl, 0ffh
loc_5_477B:
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 00Bh, 04Dh, 0FBh, 0FFh
        add    esp, 4
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_478E:
        mov    bl, byte ptr [edi + 1eh]
        jmp    SHORT loc_5_477B
sub_5_4793:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 14h], 0
        mov    dword ptr [esp + 18h], 0
        lea    eax, [esp + 18h]
        push   0feh
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        push   0
        lea    ecx, [eax + 0ch]
        push   ecx
        push   1
        movzx  eax, byte ptr [eax + 0ah]
        lea    ecx, [esp + 28h]
        push   eax
        sub    edx, edx
        call   sub_5_62A6
        mov    bp, ax
        or     bp, bp
        je     SHORT loc_5_47E2
        jmp    NEAR PTR loc_5_48EE
loc_5_47E2:
        push   dword ptr [esp + 10h]
        db  0E8h, 089h, 0FDh, 0FAh, 0FFh
        add    esp, 4
        cmp    ax, 0ffffh
        je     NEAR PTR loc_5_48CB
        test   al, 10h
        je     NEAR PTR loc_5_48CB
        mov    esi, dword ptr [esp + 2ch]
        push   esi
        call   sub_5_434B
        add    esp, 4
        mov    bl, al
        or     bl, bl
        jne    SHORT loc_5_481C
        mov    bp, 9dh
        jmp    NEAR PTR loc_5_48CF
loc_5_481C:
        mov    ecx, dword ptr [esi + 40h]
        movzx  eax, bl
        mov    edi, dword ptr [ecx + eax*4]
        mov    eax, dword ptr [esp + 14h]
        push   0
        inc    dword ptr [eax + 4]
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [edi], eax
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    ax, word ptr [eax + 491h]
        mov    word ptr [edi + 8], ax
        mov    eax, dword ptr [esp + 14h]
        movzx  eax, byte ptr [eax]
        sub    eax, dword ptr [esp + 1ch]
        inc    eax
        push   eax
        db  0E8h, 00Eh, 0FBh, 0FAh, 0FFh
        add    esp, 8
        mov    dword ptr [edi + 4], eax
        or     eax, eax
        jne    SHORT loc_5_4870
        push   ebx
        push   esi
        call   sub_5_437F
        add    esp, 8
        mov    bp, 96h
        jmp    SHORT loc_5_48CF
loc_5_4870:
        mov    eax, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 18h]
        mov    al, byte ptr [eax]
        sub    al, byte ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 10h]
        mov    byte ptr [edx + ecx], al
        mov    esi, dword ptr [esp + 18h]
        add    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [edi + 4]
        movzx  eax, byte ptr [esi]
        inc    eax
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    esi, dword ptr [esp + 24h]
        push   0
        mov    byte ptr [esi + 8], bl
        mov    eax, dword ptr [esp + 14h]
        inc    eax
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_5_4329
        add    esp, 0ch
        mov    byte ptr [esi + 9], al
        mov    eax, dword ptr [esp + 28h]
        mov    word ptr [eax], 2
        jmp    SHORT loc_5_48CF
loc_5_48CB:
        mov    bp, 9ch
loc_5_48CF:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_48E2
        push   dword ptr [esp + 14h]
        call   sub_5_3340
        add    esp, 4
loc_5_48E2:
        push   dword ptr [esp + 10h]
        db  0E8h, 06Eh, 0EAh, 0FAh, 0FFh
        add    esp, 4
loc_5_48EE:
        mov    ax, bp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_5_48F9:
        db  0A1h, 094h, 0B8h, 000h, 000h
        push   dword ptr [esp + 10h]
        mov    word ptr [eax + 491h], 0ffffh
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        call   sub_5_4793
        add    esp, 10h
        ret
loc_5_4920:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    al, byte ptr [esi + 0ah]
        push   eax
        call   sub_5_458D
        add    esp, 4
        or     ax, ax
        je     SHORT loc_5_493C
        mov    ax, 9bh
        jmp    SHORT loc_5_494F
loc_5_493C:
        mov    al, byte ptr [esi + 0ah]
        push   eax
        push   dword ptr [esp + 18h]
        call   sub_5_437F
        add    esp, 8
        sub    ax, ax
loc_5_494F:
        pop    esi
        ret
loc_5_4951:
        sub    esp, 8
        push   esi
        push   0feh
        lea    eax, [esp + 0ch]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   0
        lea    ecx, [eax + 0bh]
        push   ecx
        push   1
        movzx  eax, byte ptr [eax + 0ah]
        lea    ecx, [esp + 1ch]
        push   eax
        sub    edx, edx
        call   sub_5_62A6
        mov    si, ax
        or     si, si
        je     SHORT loc_5_4987
        jmp    SHORT loc_5_49E5
loc_5_4987:
        mov    eax, dword ptr [esp + 4]
        cmp    byte ptr [eax], 3
        je     SHORT loc_5_49A9
        push   eax
        db  0E8h, 0DEh, 0FBh, 0FAh, 0FFh
        add    esp, 4
        cmp    ax, 0ffffh
        je     SHORT loc_5_49A3
        test   al, 10h
        jne    SHORT loc_5_49A9
loc_5_49A3:
        mov    si, 9ch
        jmp    SHORT loc_5_49CD
loc_5_49A9:
        mov    eax, dword ptr [esp + 4]
        push   0
        inc    eax
        push   eax
        push   dword ptr [esp + 10h]
        call   sub_5_4329
        mov    ecx, dword ptr [esp + 20h]
        add    esp, 0ch
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [esp + 18h]
        mov    word ptr [eax], 1
loc_5_49CD:
        push   dword ptr [esp + 4]
        db  0E8h, 083h, 0E9h, 0FAh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 8]
        call   sub_5_3340
        add    esp, 4
loc_5_49E5:
        mov    ax, si
        pop    esi
        add    esp, 8
        ret
loc_5_49ED:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0feh
        lea    eax, [esp + 18h]
        push   0
        push   eax
        push   0
        sub    edx, edx
        push   0
        mov    eax, dword ptr [esp + 30h]
        push   0
        movzx  eax, byte ptr [eax + 0ah]
        lea    ecx, [esp + 28h]
        push   eax
        call   sub_5_62A6
        mov    bp, ax
        or     bp, bp
        je     SHORT loc_5_4A27
        jmp    NEAR PTR loc_5_4B4A
loc_5_4A27:
        db  0E8h, 045h, 0E9h, 0FAh, 0FFh
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_5_4A3B
        mov    bp, 96h
        jmp    NEAR PTR loc_5_4B32
loc_5_4A3B:
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax], 3
        jne    SHORT loc_5_4AA1
        mov    dword ptr [ebx + 4], 0
        mov    dword ptr [ebx + 14h], 0
loc_5_4A52:
        mov    eax, dword ptr [esp + 10h]
        push   0
        inc    eax
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_5_4329
        mov    edx, dword ptr [esp + 2ch]
        add    esp, 0ch
        mov    word ptr [edx + 6ch], ax
        mov    dword ptr [edx + 8], 0ffffffffh
        mov    ax, word ptr [ebx]
        mov    word ptr [edx + 0ch], ax
        lea    edi, [edx + 14h]
        mov    byte ptr [edx + 11h], al
        mov    word ptr [edx + 0eh], 0
        mov    byte ptr [edx + 10h], 0
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        lea    esi, [ebx + 234h]
        mov    byte ptr [edx + 12h], al
        cmp    byte ptr [esi], al
        je     SHORT loc_5_4AC4
        jmp    SHORT loc_5_4AC7
loc_5_4AA1:
        push   16h
        push   ebx
        push   dword ptr [esp + 18h]
        db  0E8h, 037h, 025h, 0FBh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_4B25
        push   eax
        db  0E8h, 052h, 026h, 0FBh, 0FFh
        add    esp, 4
        test   byte ptr [ebx], 10h
        jne    SHORT loc_5_4A52
        jmp    SHORT loc_5_4B25
loc_5_4AC4:
        lea    esi, [ebx + 2ch]
loc_5_4AC7:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [edx + 13h], cl
        movzx  eax, cl
        lea    edi, [edx + 14h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebx + 14h]
        mov    dword ptr [edx + 30h], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [edx + 20h], eax
        db  0A1h, 020h, 09Bh, 000h, 000h
        mov    dword ptr [edx + 24h], eax
        mov    word ptr [edx + 28h], 0
        mov    word ptr [edx + 2ah], 0
        mov    dword ptr [edx + 2ch], 0
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [edx + 6eh], 7fffffffh
        mov    word ptr [eax], 80h
        jmp    SHORT loc_5_4B29
loc_5_4B25:
        mov    bp, 9ch
loc_5_4B29:
        push   ebx
        db  0E8h, 0A7h, 0E8h, 0FAh, 0FFh
        add    esp, 4
loc_5_4B32:
        push   dword ptr [esp + 10h]
        db  0E8h, 01Eh, 0E8h, 0FAh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 14h]
        call   sub_5_3340
        add    esp, 4
loc_5_4B4A:
        mov    ax, bp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_4B55:
        sub    esp, 8
        push   esi
        push   0feh
        lea    eax, [esp + 0ch]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   0
        lea    ecx, [eax + 0bh]
        push   ecx
        push   1
        movzx  eax, byte ptr [eax + 0ah]
        lea    ecx, [esp + 1ch]
        push   eax
        sub    edx, edx
        call   sub_5_62A6
        mov    si, ax
        or     si, si
        je     SHORT loc_5_4B8B
        jmp    SHORT loc_5_4BE6
loc_5_4B8B:
        mov    eax, dword ptr [esp + 4]
        cmp    byte ptr [eax], 3
        je     SHORT loc_5_4BA9
        push   eax
        db  0E8h, 0DAh, 0F9h, 0FAh, 0FFh
        add    esp, 4
        cmp    ax, 0ffffh
        jne    SHORT loc_5_4BA9
        mov    si, 9ch
        jmp    SHORT loc_5_4BCE
loc_5_4BA9:
        mov    eax, dword ptr [esp + 4]
        push   0
        inc    eax
        push   eax
        push   dword ptr [esp + 10h]
        call   sub_5_4329
        mov    ecx, dword ptr [esp + 20h]
        add    esp, 0ch
        mov    word ptr [ecx + 8], ax
        mov    eax, dword ptr [esp + 18h]
        mov    word ptr [eax], 1
loc_5_4BCE:
        push   dword ptr [esp + 4]
        db  0E8h, 082h, 0E7h, 0FAh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 8]
        call   sub_5_3340
        add    esp, 4
loc_5_4BE6:
        mov    ax, si
        pop    esi
        add    esp, 8
        ret
loc_5_4BEE:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 18h]
        mov    al, byte ptr [esi + 0ah]
        push   eax
        call   sub_5_458D
        add    esp, 4
        or     ax, ax
        jne    NEAR PTR loc_5_4CED
        mov    eax, dword ptr [esp + 24h]
        movzx  ecx, byte ptr [esi + 0ah]
        mov    eax, dword ptr [eax + 40h]
        push   0feh
        mov    edi, dword ptr [eax + ecx*4]
        lea    eax, [esp + 14h]
        push   eax
        push   0
        push   0
        lea    eax, [esi + 0ch]
        push   eax
        push   1
        movzx  eax, byte ptr [esi + 0bh]
        lea    ecx, [esp + 24h]
        push   eax
        sub    edx, edx
        call   sub_5_62A6
        mov    bx, ax
        or     bx, bx
        je     SHORT loc_5_4C4C
        jmp    NEAR PTR loc_5_4CEA
loc_5_4C4C:
        mov    eax, dword ptr [esp + 0ch]
        cmp    byte ptr [eax], 3
        jne    SHORT loc_5_4C5D
        mov    eax, dword ptr [edi + 4]
        mov    byte ptr [eax], 0
        jmp    SHORT loc_5_4C73
loc_5_4C5D:
        push   dword ptr [esp + 0ch]
        db  0E8h, 00Eh, 0F9h, 0FAh, 0FFh
        add    esp, 4
        cmp    ax, 0ffffh
        je     SHORT loc_5_4CDA
        test   al, 10h
        je     SHORT loc_5_4CDA
loc_5_4C73:
        mov    esi, dword ptr [edi + 4]
        mov    eax, dword ptr [esp + 0ch]
        push   0
        movzx  eax, byte ptr [eax]
        sub    eax, dword ptr [esp + 14h]
        inc    eax
        push   eax
        db  0E8h, 0DAh, 0F6h, 0FAh, 0FFh
        add    esp, 8
        mov    dword ptr [edi + 4], eax
        or     eax, eax
        jne    SHORT loc_5_4C9D
        mov    dword ptr [edi + 4], esi
        mov    bx, 96h
        jmp    SHORT loc_5_4CDE
loc_5_4C9D:
        or     esi, esi
        je     SHORT loc_5_4CAA
        push   esi
        db  0E8h, 0E9h, 0F6h, 0FAh, 0FFh
        add    esp, 4
loc_5_4CAA:
        mov    eax, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 10h]
        mov    cl, byte ptr [eax]
        sub    cl, byte ptr [esp + 10h]
        mov    byte ptr [eax + edx], cl
        mov    esi, dword ptr [esp + 0ch]
        add    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [edi + 4]
        movzx  eax, byte ptr [esi]
        inc    eax
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        jmp    SHORT loc_5_4CDE
loc_5_4CDA:
        mov    bx, 9ch
loc_5_4CDE:
        push   dword ptr [esp + 0ch]
        db  0E8h, 072h, 0E6h, 0FAh, 0FFh
        add    esp, 4
loc_5_4CEA:
        mov    ax, bx
loc_5_4CED:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_5_4CF4:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 1ch]
        or     esi, esi
        mov    ax, 4
        jne    SHORT loc_5_4D08
        mov    ax, 10h
loc_5_4D08:
        push   0feh
        push   0
        push   0
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        sub    edx, edx
        lea    ecx, [eax + 0ch]
        push   ecx
        push   1
        movzx  eax, byte ptr [eax + 0ah]
        lea    ecx, [esp + 1ch]
        push   eax
        call   sub_5_62A6
        or     ax, ax
        jne    SHORT loc_5_4D7A
        or     esi, esi
        push   dword ptr [esp + 4]
        je     SHORT loc_5_4D4B
        db  0E8h, 04Ah, 0F7h, 0FAh, 0FFh
        add    esp, 4
        mov    si, ax
        db  0BAh, 08Ch, 0A1h, 000h, 000h
        jmp    SHORT loc_5_4D5B
loc_5_4D4B:
        db  0E8h, 04Eh, 0F7h, 0FAh, 0FFh
        add    esp, 4
        mov    si, ax
        db  0BAh, 094h, 0A1h, 000h, 000h
loc_5_4D5B:
        or     si, si
        je     SHORT loc_5_4D6B
        mov    ecx, esi
        db  0E8h, 099h, 0DCh, 0FAh, 0FFh
        movzx  si, al
loc_5_4D6B:
        push   dword ptr [esp + 4]
        db  0E8h, 0E5h, 0E5h, 0FAh, 0FFh
        add    esp, 4
        mov    ax, si
loc_5_4D7A:
        pop    esi
        add    esp, 4
        ret
loc_5_4D7F:
        push   1
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        call   sub_5_4CF4
        add    esp, 14h
        ret
loc_5_4D9A:
        push   0
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        call   sub_5_4CF4
        add    esp, 14h
        ret
loc_5_4DB5:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    ecx, dword ptr [esp + 1ch]
        mov    byte ptr [eax + 493h], 1
        movzx  eax, byte ptr [ecx + 0bh]
        lea    edx, [ecx + 0bh]
        lea    ebx, [eax + ecx + 0ch]
        push   0feh
        lea    eax, [esp + 18h]
        push   0
        push   eax
        push   10h
        push   edx
        sub    edx, edx
        push   1
        movzx  eax, byte ptr [ecx + 0ah]
        lea    ecx, [esp + 28h]
        push   eax
        call   sub_5_62A6
        or     ax, ax
        jne    NEAR PTR loc_5_4EFC
        db  0E8h, 0FAh, 0E4h, 0FAh, 0FFh
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_5_4E15
        mov    bx, 96h
        jmp    NEAR PTR loc_5_4EE1
loc_5_4E15:
        mov    eax, dword ptr [esp + 10h]
        mov    edi, ebp
        movzx  eax, byte ptr [eax]
        inc    eax
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   0ffh
        mov    eax, dword ptr [esp + 18h]
        push   0ffh
        mov    al, byte ptr [eax + 24h]
        push   eax
        db  068h, 09Ch, 0A1h, 000h, 000h
        push   ebp
        db  0E8h, 042h, 0CEh, 0FAh, 0FFh
        add    esp, 14h
        push   0feh
        mov    eax, dword ptr [esp + 18h]
        push   0ffh
        mov    al, byte ptr [eax + 24h]
        push   eax
        push   ebx
        push   ebp
        db  0E8h, 026h, 0CEh, 0FAh, 0FFh
        add    esp, 14h
        lea    edi, [ebp + 1]
        push   4
        push   edi
        push   dword ptr [esp + 1ch]
        call   sub_5_6205
        add    esp, 0ch
        mov    bx, ax
        or     bx, bx
        jne    SHORT loc_5_4ED8
        push   10h
        push   ebp
        push   dword ptr [esp + 18h]
        db  0E8h, 0BDh, 0F5h, 0FAh, 0FFh
        add    esp, 0ch
        mov    bx, ax
        or     bx, bx
        je     SHORT loc_5_4EAD
        sub    edx, edx
        mov    ecx, ebx
        db  0E8h, 059h, 0DBh, 0FAh, 0FFh
        movzx  bx, al
        jmp    SHORT loc_5_4ED8
loc_5_4EAD:
        mov    eax, dword ptr [esp + 10h]
        mov    al, byte ptr [eax]
        cmp    byte ptr [ebp], al
        jne    SHORT loc_5_4ED8
        mov    esi, dword ptr [esp + 10h]
        movzx  eax, al
        mov    ecx, eax
        shr    ecx, 2
        lea    esi, [esi + 1]
        repe cmpsd
        jne    SHORT loc_5_4ED8
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_5_4ED8
        mov    bx, 92h
loc_5_4ED8:
        push   ebp
        db  0E8h, 07Bh, 0E4h, 0FAh, 0FFh
        add    esp, 4
loc_5_4EE1:
        push   dword ptr [esp + 10h]
        db  0E8h, 06Fh, 0E4h, 0FAh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 14h]
        call   sub_5_3340
        add    esp, 4
        mov    ax, bx
loc_5_4EFC:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_4F04:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    bl, byte ptr [eax + 0ah]
        push   ebx
        call   sub_5_458D
        add    esp, 4
        mov    bp, ax
        or     bp, bp
        je     SHORT loc_5_4F22
        jmp    SHORT loc_5_4F88
loc_5_4F22:
        mov    eax, dword ptr [esp + 20h]
        movzx  ecx, bl
        mov    eax, dword ptr [eax + 40h]
        mov    eax, dword ptr [eax + ecx*4]
        mov    esi, dword ptr [eax]
        mov    al, byte ptr [esi + 25h]
        sub    al, 40h
        mov    ebx, dword ptr [esp + 18h]
        push   eax
        lea    eax, [ebx + 8]
        push   eax
        db  0E8h, 088h, 0EDh, 0FAh, 0FFh
        add    esp, 8
        movzx  eax, ax
        cmp    eax, -1
        jne    SHORT loc_5_4F55
        mov    ax, 0ffh
        jmp    SHORT loc_5_4F8B
loc_5_4F55:
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 0DDh, 044h, 0FBh, 0FFh
        add    esp, 4
        add    esi, 0dh
        lea    edi, [ebx + 12h]
        movsd
        movsd
        movsd
        movsd
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 01Ah, 045h, 0FBh, 0FFh
        add    esp, 4
        mov    word ptr [ebx + 22h], 0
        mov    ecx, dword ptr [esp + 1ch]
        mov    word ptr [ecx], 1ch
loc_5_4F88:
        mov    ax, bp
loc_5_4F8B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_4F90:
        push   esi
        db  08Bh, 035h, 050h, 0A1h, 000h, 000h
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 09Bh, 044h, 0FBh, 0FFh
        add    esp, 4
        or     esi, esi
        je     SHORT loc_5_4FBE
        mov    al, byte ptr [esp + 8]
loc_5_4FAC:
        cmp    byte ptr [esi + 1eh], al
        je     SHORT loc_5_4FB7
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_5_4FAC
loc_5_4FB7:
        or     esi, esi
        je     SHORT loc_5_4FBE
        inc    dword ptr [esi + 4]
loc_5_4FBE:
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 0C8h, 044h, 0FBh, 0FFh
        add    esp, 4
        mov    eax, esi
        pop    esi
        ret
loc_5_4FCF:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 1ch]
        mov    word ptr [eax], 1ch
        mov    eax, dword ptr [esp + 14h]
        mov    al, byte ptr [eax + 7]
        push   eax
        call   sub_5_4F90
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_5_503C
        mov    al, byte ptr [ebp + 25h]
        sub    al, 40h
        mov    ebx, dword ptr [esp + 18h]
        push   eax
        lea    eax, [ebx + 8]
        push   eax
        db  0E8h, 0C7h, 0ECh, 0FAh, 0FFh
        add    esp, 8
        movzx  eax, ax
        cmp    eax, -1
        je     SHORT loc_5_5040
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 022h, 044h, 0FBh, 0FFh
        add    esp, 4
        lea    esi, [ebp + 0dh]
        lea    edi, [ebx + 12h]
        movsd
        movsd
        movsd
        movsd
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 05Fh, 044h, 0FBh, 0FFh
        add    esp, 4
        mov    word ptr [ebx + 22h], 0
        jmp    SHORT loc_5_504C
loc_5_503C:
        mov    ebx, dword ptr [esp + 18h]
loc_5_5040:
        lea    edi, [ebx + 8]
        sub    eax, eax
        mov    ecx, 7
        rep stosd
loc_5_504C:
        or     ebp, ebp
        je     SHORT loc_5_5059
        push   ebp
        call   sub_5_3340
        add    esp, 4
loc_5_5059:
        sub    ax, ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_5061:
        mov    eax, dword ptr [esp + 4]
        add    eax, 0ah
        push   eax
        call   sub_5_472B
        add    esp, 4
        cmp    al, 0ffh
        je     SHORT loc_5_508A
        mov    ecx, dword ptr [esp + 8]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [esp + 0ch]
        mov    word ptr [eax], 1
        sub    ax, ax
        jmp    SHORT loc_5_508E
loc_5_508A:
        mov    ax, 0ffh
loc_5_508E:
        ret
loc_5_508F:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    al, byte ptr [esi + 0ah]
        push   eax
        call   sub_5_4F90
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_5_50E5
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 08Ah, 043h, 0FBh, 0FFh
        add    esp, 4
        mov    al, byte ptr [ebx + 0ch]
        mov    edi, dword ptr [esp + 14h]
        movzx  cx, al
        mov    byte ptr [edi + 8], al
        mov    eax, dword ptr [esp + 18h]
        inc    cx
        mov    word ptr [eax], cx
        lea    esi, [ebx + 0dh]
        add    edi, 9
        movsd
        movsd
        movsd
        movsd
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 0B0h, 043h, 0FBh, 0FFh
        add    esp, 4
        jmp    SHORT loc_5_5102
loc_5_50E5:
        cmp    byte ptr [esi + 0ah], 40h
        jb     SHORT loc_5_50F1
        mov    si, 98h
        jmp    SHORT loc_5_5105
loc_5_50F1:
        mov    eax, dword ptr [esp + 18h]
        mov    edi, dword ptr [esp + 14h]
        mov    word ptr [eax], 1
        mov    byte ptr [edi + 8], 0
loc_5_5102:
        sub    si, si
loc_5_5105:
        or     ebx, ebx
        je     SHORT loc_5_5112
        push   ebx
        call   sub_5_3340
        add    esp, 4
loc_5_5112:
        mov    ax, si
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_5119:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 18h]
        mov    word ptr [eax], 2eh
        mov    eax, dword ptr [esp + 10h]
        mov    al, byte ptr [eax + 0ah]
        push   eax
        call   sub_5_4F90
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        mov    edi, dword ptr [esp + 14h]
        je     NEAR PTR loc_5_51DC
        lea    esi, [edi + 2eh]
        mov    al, byte ptr [ebx + 25h]
        sub    al, 40h
        push   eax
        push   esi
        db  0E8h, 07Ah, 0EBh, 0FAh, 0FFh
        add    esp, 8
        movzx  eax, ax
        cmp    eax, -1
        je     SHORT loc_5_51DC
        mov    ax, word ptr [edi + 30h]
        push   eax
        db  0E8h, 0F9h, 0E8h, 0FAh, 0FFh
        add    esp, 4
        movzx  eax, ax
        mov    dword ptr [edi + 8], eax
        mov    ax, word ptr [edi + 32h]
        push   eax
        db  0E8h, 0E6h, 0E8h, 0FAh, 0FFh
        add    esp, 4
        movzx  eax, ax
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [edi + 10h], 0
        mov    dword ptr [edi + 14h], 0
        mov    dword ptr [edi + 18h], 7fffh
        mov    dword ptr [edi + 1ch], 7fffh
        mov    dword ptr [edi + 20h], 0
        mov    ax, word ptr [esi]
        push   eax
        db  0E8h, 0B1h, 0E8h, 0FAh, 0FFh
        add    esp, 4
        mov    byte ptr [edi + 24h], al
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 07Dh, 042h, 0FBh, 0FFh
        add    esp, 4
        lea    esi, [ebx + 0ch]
        add    edi, 25h
        movsd
        movsd
        movsd
        movsd
        movsb
        db  068h, 054h, 0A1h, 000h, 000h
        db  0E8h, 0B9h, 042h, 0FBh, 0FFh
        add    esp, 4
        jmp    SHORT loc_5_51EA
loc_5_51DC:
        add    edi, 8
        sub    eax, eax
        mov    ecx, 0bh
        rep stosd
        stosw
loc_5_51EA:
        or     ebx, ebx
        je     SHORT loc_5_51F7
        push   ebx
        call   sub_5_3340
        add    esp, 4
loc_5_51F7:
        sub    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_51FE:
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [eax + 8], 0
        mov    eax, dword ptr [esp + 0ch]
        mov    word ptr [eax], 1
        sub    ax, ax
        ret
loc_5_5213:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 8], 40000000h
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [esp + 0ch]
        mov    word ptr [eax], 8
        sub    ax, ax
        ret
sub_5_5232:
        sub    esp, 8
        push   esi
        push   edi
        mov    dword ptr [esp + 8], 0
        push   dword ptr [esp + 28h]
        lea    eax, [esp + 0ch]
        push   0
        push   eax
        mov    ax, word ptr [esp + 30h]
        sub    edx, edx
        push   eax
        push   dword ptr [esp + 28h]
        movzx  eax, byte ptr [esp + 28h]
        push   1
        lea    ecx, [esp + 24h]
        push   eax
        call   sub_5_62A6
        mov    di, ax
        or     di, di
        je     SHORT loc_5_5271
        jmp    SHORT loc_5_52D4
loc_5_5271:
        push   dword ptr [esp + 0ch]
        call   sub_5_7D1A
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_5_5295
        mov    di, 96h
        push   dword ptr [esp + 8]
        call   sub_5_3340
        add    esp, 4
        jmp    SHORT loc_5_52C2
loc_5_5295:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [esi + 10h], 0ffffffffh
        mov    word ptr [esi + 18h], 0
        lea    eax, [esi + 2eh]
        mov    byte ptr [esi + 15h], 2
        push   0
        push   eax
        push   dword ptr [esi + 0ch]
        call   sub_5_4329
        add    esp, 0ch
        mov    word ptr [esi + 1ch], ax
loc_5_52C2:
        push   dword ptr [esp + 0ch]
        db  0E8h, 08Eh, 0E0h, 0FAh, 0FFh
        mov    ecx, dword ptr [esp + 20h]
        add    esp, 4
        mov    dword ptr [ecx], esi
loc_5_52D4:
        mov    ax, di
        pop    edi
        pop    esi
        add    esp, 8
        ret
loc_5_52DD:
        sub    esp, 4
        push   esi
        db  0A1h, 094h, 0B8h, 000h, 000h
        push   0feh
        push   80h
        mov    byte ptr [eax + 493h], 1
        lea    eax, [esp + 0ch]
        push   dword ptr [esp + 20h]
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        lea    ecx, [eax + 8]
        push   ecx
        mov    al, byte ptr [eax + 7]
        push   eax
        call   sub_5_5232
        add    esp, 18h
        mov    si, ax
        or     si, si
        jne    SHORT loc_5_5378
        mov    eax, dword ptr [esp + 4]
        add    eax, 2dh
        push   eax
        db  0E8h, 04Bh, 0F2h, 0FAh, 0FFh
        add    esp, 4
        cmp    ax, 0ffffh
        je     SHORT loc_5_5368
        test   al, 10h
        je     SHORT loc_5_5368
        mov    ecx, dword ptr [esp + 10h]
        mov    byte ptr [ecx + 0dh], 0ffh
        mov    word ptr [ecx + 0bh], 0ffffh
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0ch]
        mov    al, byte ptr [eax + 1eh]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 1ah]
        mov    word ptr [ecx + 9], ax
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [eax], 6
        jmp    SHORT loc_5_536C
loc_5_5368:
        mov    si, 9ch
loc_5_536C:
        push   dword ptr [esp + 4]
        call   sub_5_7FAD
        add    esp, 4
loc_5_5378:
        mov    ax, si
        pop    esi
        add    esp, 4
        ret
loc_5_5380:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    byte ptr [eax + 493h], 1
        mov    eax, dword ptr [esp + 2ch]
        push   0
        mov    ax, word ptr [eax + 8]
        push   eax
        call   sub_5_7DFC
        mov    dword ptr [esp + 20h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_53BA
        mov    word ptr [esp + 12h], 0ffh
        jmp    NEAR PTR loc_5_56F9
loc_5_53BA:
        mov    ebx, dword ptr [esp + 18h]
        add    ebx, 15h
        mov    byte ptr [ebx], 1
        db  0E8h, 037h, 0DFh, 0FAh, 0FFh
        mov    dword ptr [esp + 24h], eax
        or     eax, eax
        jne    SHORT loc_5_53DD
        mov    word ptr [esp + 12h], 96h
        jmp    NEAR PTR loc_5_56ED
loc_5_53DD:
        db  0E8h, 08Fh, 0DFh, 0FAh, 0FFh
        mov    dword ptr [esp + 14h], eax
        or     eax, eax
        jne    SHORT loc_5_53F6
        mov    word ptr [esp + 12h], 96h
        jmp    NEAR PTR loc_5_56E1
loc_5_53F6:
        mov    eax, dword ptr [esp + 2ch]
        cmp    word ptr [eax + 0ah], -1
        je     SHORT loc_5_540F
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [eax + 8], 0
        jne    NEAR PTR loc_5_5522
loc_5_540F:
        mov    ebp, dword ptr [esp + 18h]
        mov    eax, dword ptr [ebp + 8]
        add    ebp, 8
        or     eax, eax
        je     SHORT loc_5_542D
        push   eax
        db  0E8h, 0E9h, 01Ch, 0FBh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp], 0
loc_5_542D:
        mov    esi, dword ptr [esp + 18h]
        movzx  eax, byte ptr [esi + 2dh]
        add    esi, 2dh
        inc    eax
        mov    edi, dword ptr [esp + 24h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   0feh
        mov    eax, dword ptr [esp + 28h]
        push   0ffh
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [esp + 38h]
        add    eax, 0dh
        push   eax
        push   dword ptr [esp + 34h]
        db  0E8h, 023h, 0C8h, 0FAh, 0FFh
        mov    word ptr [esp + 26h], ax
        add    esp, 14h
        or     ax, ax
        jne    NEAR PTR loc_5_56D5
        mov    eax, dword ptr [esp + 24h]
        push   4080h
        inc    eax
        push   eax
        mov    eax, dword ptr [esp + 20h]
        push   dword ptr [eax + 0ch]
        call   sub_5_6205
        mov    word ptr [esp + 1eh], ax
        add    esp, 0ch
        or     ax, ax
        jne    NEAR PTR loc_5_56D5
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax + 10h], 0ffffffffh
        mov    eax, dword ptr [esp + 2ch]
        movzx  ax, byte ptr [eax + 0ch]
        push   eax
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 2ch]
        db  0E8h, 0F4h, 01Ah, 0FBh, 0FFh
        add    esp, 0ch
        mov    dword ptr [ebp], eax
        or     eax, eax
        jne    SHORT loc_5_54DE
        mov    word ptr [esp + 12h], 0ffh
        jmp    NEAR PTR loc_5_56D5
loc_5_54DE:
        db  0A1h, 094h, 0B8h, 000h, 000h
        cmp    byte ptr [eax + 494h], 1
        sbb    al, al
        and    al, 0feh
        add    al, 4
        mov    byte ptr [ebx], al
loc_5_54F2:
        mov    eax, dword ptr [esp + 2ch]
        mov    al, byte ptr [eax + 0ch]
        and    al, 10h
        mov    byte ptr [esp + 20h], al
        or     al, al
        je     SHORT loc_5_550D
        mov    eax, dword ptr [esp + 14h]
        cmp    byte ptr [eax + 2ch], 2eh
        je     SHORT loc_5_5522
loc_5_550D:
        cmp    byte ptr [esp + 20h], 0
        je     SHORT loc_5_557E
        mov    eax, dword ptr [esp + 14h]
        test   byte ptr [eax], 10h
        jne    SHORT loc_5_557E
        db  0E8h, 08Fh, 03Eh, 0FBh, 0FFh
loc_5_5522:
        mov    eax, dword ptr [esp + 18h]
        add    eax, 8
        mov    dword ptr [esp + 1ch], eax
        push   dword ptr [esp + 14h]
        push   dword ptr [eax]
        db  0E8h, 078h, 01Bh, 0FBh, 0FFh
        mov    word ptr [esp + 1ah], ax
        add    esp, 8
        or     ax, ax
        je     SHORT loc_5_5571
        mov    eax, dword ptr [esp + 1ch]
        mov    word ptr [esp + 12h], 0ffh
        push   dword ptr [eax]
        db  0E8h, 0B5h, 01Bh, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 20h]
        add    esp, 4
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esp + 18h]
        mov    byte ptr [eax + 15h], 1
        jmp    NEAR PTR loc_5_56D5
loc_5_5571:
        mov    eax, dword ptr [esp + 18h]
        mov    byte ptr [eax + 15h], 4
        jmp    NEAR PTR loc_5_54F2
loc_5_557E:
        mov    eax, dword ptr [esp + 2ch]
        mov    ebx, dword ptr [esp + 30h]
        mov    cx, word ptr [eax + 0ah]
        mov    eax, dword ptr [esp + 18h]
        inc    cx
        mov    word ptr [eax + 10h], cx
        mov    word ptr [ebx + 8], cx
        mov    word ptr [ebx + 0ah], 0
        lea    edx, [ebx + 0ch]
        mov    edi, edx
        sub    eax, eax
        stosd
        stosd
        stosd
        stosw
        mov    edi, dword ptr [esp + 14h]
        add    edi, 234h
        cmp    byte ptr [edi], al
        je     SHORT loc_5_55BA
        jmp    SHORT loc_5_55C1
loc_5_55BA:
        mov    edi, dword ptr [esp + 14h]
        add    edi, 2ch
loc_5_55C1:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 14h]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebx + 1ah], al
        mov    eax, dword ptr [esp + 14h]
        test   byte ptr [eax], 10h
        je     NEAR PTR loc_5_568D
        mov    esi, dword ptr [esp + 18h]
        movzx  eax, byte ptr [esi + 2dh]
        add    esi, 2dh
        inc    eax
        mov    edi, dword ptr [esp + 24h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [esp + 14h]
        add    edx, 2bh
        mov    edi, dword ptr [esp + 14h]
        add    edi, 2ch
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [edx], cl
        push   0ffh
        mov    eax, dword ptr [esp + 28h]
        push   0ffh
        mov    al, byte ptr [eax]
        push   eax
        push   edx
        push   dword ptr [esp + 34h]
        db  0E8h, 045h, 0C6h, 0FAh, 0FFh
        mov    word ptr [esp + 26h], ax
        add    esp, 14h
        or     ax, ax
        je     SHORT loc_5_5662
        mov    eax, dword ptr [esp + 18h]
        mov    byte ptr [eax + 15h], 1
        jmp    SHORT loc_5_56D5
loc_5_5662:
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [ebx + 1bh], 0ffh
        push   dword ptr [eax + 14h]
        db  0E8h, 0F6h, 0E3h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [ebx + 1ch], eax
        mov    dword ptr [ebx + 20h], 0
        mov    word ptr [ebx + 24h], 0
        mov    word ptr [ebx + 26h], 0
        jmp    SHORT loc_5_56CC
loc_5_568D:
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [ebx + 1bh], 0
        push   dword ptr [eax + 20h]
        db  0E8h, 0CBh, 0E3h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [ebx + 1ch], eax
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [eax + 4]
        db  0E8h, 0B9h, 0E3h, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [ebx + 20h], ax
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [eax + 14h]
        db  0E8h, 0A6h, 0E3h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [ebx + 24h], eax
        mov    word ptr [ebx + 22h], ax
loc_5_56CC:
        mov    eax, dword ptr [esp + 34h]
        mov    word ptr [eax], 20h
loc_5_56D5:
        push   dword ptr [esp + 14h]
        db  0E8h, 0F8h, 0DCh, 0FAh, 0FFh
        add    esp, 4
loc_5_56E1:
        push   dword ptr [esp + 24h]
        db  0E8h, 06Fh, 0DCh, 0FAh, 0FFh
        add    esp, 4
loc_5_56ED:
        push   dword ptr [esp + 18h]
        call   sub_5_7FAD
        add    esp, 4
loc_5_56F9:
        mov    ax, word ptr [esp + 12h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_5_5706:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    word ptr [esp + 0ch], 0
        mov    byte ptr [eax + 493h], 1
        mov    esi, dword ptr [esp + 20h]
        mov    ax, word ptr [esi + 7]
        add    esi, 7
        cmp    ax, 0ffffh
        jne    SHORT loc_5_5798
        push   0feh
        push   4080h
        lea    eax, [esp + 18h]
        push   dword ptr [esp + 34h]
        push   eax
        mov    eax, dword ptr [esp + 30h]
        add    eax, 0bh
        push   eax
        mov    eax, dword ptr [esp + 34h]
        mov    al, byte ptr [eax + 9]
        push   eax
        call   sub_5_5232
        mov    word ptr [esp + 24h], ax
        add    esp, 18h
        or     ax, ax
        je     SHORT loc_5_576A
        jmp    NEAR PTR loc_5_59CF
loc_5_576A:
        mov    eax, dword ptr [esp + 10h]
        mov    ax, word ptr [eax + 1ah]
        mov    word ptr [esp + 0eh], ax
loc_5_5777:
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax + 15h], 1
        db  0E8h, 0EDh, 0DBh, 0FAh, 0FFh
        mov    dword ptr [esp + 14h], eax
        or     eax, eax
        jne    SHORT loc_5_57BF
        mov    word ptr [esp + 0ch], 96h
        jmp    NEAR PTR loc_5_59BE
loc_5_5798:
        mov    word ptr [esp + 0eh], ax
        push   0
        push   dword ptr [esp + 12h]
        call   sub_5_7DFC
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_5777
        mov    word ptr [esp + 0ch], 0ffh
        jmp    NEAR PTR loc_5_59CA
loc_5_57BF:
        cmp    word ptr [esi], -1
        je     SHORT loc_5_57D3
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 8], 0
        jne    NEAR PTR loc_5_58D4
loc_5_57D3:
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax + 2dh], 3
        jne    SHORT loc_5_5832
        mov    eax, dword ptr [esp + 20h]
        test   byte ptr [eax + 0ah], 10h
        je     SHORT loc_5_5832
        mov    edi, dword ptr [esp + 14h]
        add    edi, 2ch
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax + 15h], 2
loc_5_57FC:
        mov    ebx, dword ptr [esp + 24h]
        mov    ax, word ptr [esp + 0eh]
        mov    word ptr [ebx + 8], ax
        lea    edx, [ebx + 0ch]
        mov    word ptr [ebx + 0ah], 0
        mov    edi, edx
        sub    eax, eax
        stosd
        stosd
        stosd
        stosw
        mov    edi, dword ptr [esp + 14h]
        add    edi, 234h
        cmp    byte ptr [edi], al
        je     NEAR PTR loc_5_592C
        jmp    NEAR PTR loc_5_5933
loc_5_5832:
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        je     SHORT loc_5_5846
        push   eax
        db  0E8h, 0C9h, 018h, 0FBh, 0FFh
        add    esp, 4
loc_5_5846:
        mov    eax, dword ptr [esp + 20h]
        movzx  ax, byte ptr [eax + 0ah]
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [esp + 18h]
        add    eax, 2dh
        push   eax
        db  0E8h, 05Bh, 017h, 0FBh, 0FFh
        mov    ecx, dword ptr [esp + 1ch]
        add    esp, 0ch
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_5_5881
        mov    word ptr [esp + 0ch], 0ffh
        jmp    NEAR PTR loc_5_59B2
loc_5_5881:
        db  0A1h, 094h, 0B8h, 000h, 000h
        cmp    byte ptr [eax + 494h], 1
        sbb    cl, cl
        and    cl, 0feh
        mov    eax, dword ptr [esp + 10h]
        add    cl, 4
        mov    byte ptr [eax + 15h], cl
loc_5_589C:
        mov    eax, dword ptr [esp + 20h]
        mov    al, byte ptr [eax + 0ah]
        and    al, 10h
        mov    byte ptr [esp + 18h], al
        or     al, al
        je     SHORT loc_5_58B7
        mov    eax, dword ptr [esp + 14h]
        cmp    byte ptr [eax + 2ch], 2eh
        je     SHORT loc_5_58D4
loc_5_58B7:
        cmp    byte ptr [esp + 18h], 0
        je     NEAR PTR loc_5_57FC
        mov    eax, dword ptr [esp + 14h]
        test   byte ptr [eax], 10h
        jne    NEAR PTR loc_5_57FC
        db  0E8h, 0DDh, 03Ah, 0FBh, 0FFh
loc_5_58D4:
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [esp + 14h]
        push   dword ptr [eax + 8]
        db  0E8h, 0CCh, 017h, 0FBh, 0FFh
        mov    word ptr [esp + 14h], ax
        add    esp, 8
        or     ax, ax
        je     SHORT loc_5_591F
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [esp + 0ch], 0ffh
        push   dword ptr [eax + 8]
        db  0E8h, 008h, 018h, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 14h]
        add    esp, 4
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax + 15h], 1
        jmp    NEAR PTR loc_5_59B2
loc_5_591F:
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax + 15h], 4
        jmp    NEAR PTR loc_5_589C
loc_5_592C:
        mov    edi, dword ptr [esp + 14h]
        add    edi, 2ch
loc_5_5933:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 14h]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebx + 1ah], al
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [ebx + 1bh], 0
        push   dword ptr [eax + 20h]
        db  0E8h, 0FDh, 0E0h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [ebx + 1ch], eax
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [eax + 4]
        db  0E8h, 0EBh, 0E0h, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [ebx + 20h], ax
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [eax + 0ch]
        db  0E8h, 0D8h, 0E0h, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [ebx + 22h], ax
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [eax + 14h]
        db  0E8h, 0C5h, 0E0h, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [ebx + 24h], eax
        mov    eax, dword ptr [esp + 28h]
        mov    word ptr [eax], 20h
loc_5_59B2:
        push   dword ptr [esp + 14h]
        db  0E8h, 01Bh, 0DAh, 0FAh, 0FFh
        add    esp, 4
loc_5_59BE:
        push   dword ptr [esp + 10h]
        call   sub_5_7FAD
        add    esp, 4
loc_5_59CA:
        mov    ax, word ptr [esp + 0ch]
loc_5_59CF:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_59D6:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    dword ptr [esp + 20h], 0
        mov    esi, dword ptr [esp + 28h]
        mov    byte ptr [eax + 493h], 1
        mov    ax, word ptr [esi + 0bh]
        push   eax
        db  0E8h, 062h, 0E0h, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [esi + 0bh], ax
        cmp    ax, 1
        je     SHORT loc_5_5A10
        or     ax, ax
        jne    SHORT loc_5_5A16
loc_5_5A10:
        mov    word ptr [esi + 0bh], 0ffffh
loc_5_5A16:
        mov    ax, word ptr [esi + 0bh]
        cmp    ax, 0ffffh
        jne    NEAR PTR loc_5_5AC0
        push   0feh
        push   4080h
        mov    edi, 1
        lea    eax, [esp + 18h]
        push   dword ptr [esp + 3ch]
        push   eax
        lea    eax, [esi + 0dh]
        push   eax
        mov    al, byte ptr [esi + 0ah]
        push   eax
        call   sub_5_5232
        add    esp, 18h
        or     ax, ax
        jne    NEAR PTR loc_5_5D31
        mov    eax, dword ptr [esp + 10h]
        mov    ax, word ptr [eax + 1ah]
        mov    word ptr [esp + 0eh], ax
loc_5_5A62:
        mov    eax, dword ptr [esp + 10h]
        push   0feh
        mov    byte ptr [eax + 15h], 1
        lea    eax, [esp + 24h]
        push   0
        push   eax
        lea    eax, [esi + 0dh]
        push   4080h
        push   eax
        push   1
        movzx  eax, byte ptr [esi + 0ah]
        sub    edx, edx
        lea    ecx, [esp + 34h]
        push   eax
        call   sub_5_62A6
        mov    word ptr [esp + 0ch], ax
        or     ax, ax
        jne    NEAR PTR loc_5_5D0D
        db  0E8h, 0CDh, 0D8h, 0FAh, 0FFh
        mov    dword ptr [esp + 14h], eax
        or     eax, eax
        jne    SHORT loc_5_5AEC
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [esp + 0ch], 96h
        mov    byte ptr [eax + 15h], 1
        jmp    NEAR PTR loc_5_5D01
loc_5_5AC0:
        sub    edi, edi
        sub    ax, 3
        mov    word ptr [esp + 0eh], ax
        push   edi
        push   dword ptr [esp + 12h]
        call   sub_5_7DFC
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        cmp    eax, edi
        jne    SHORT loc_5_5A62
        mov    word ptr [esp + 0ch], 0ffh
        jmp    NEAR PTR loc_5_5D19
loc_5_5AEC:
        or     edi, edi
        je     SHORT loc_5_5B5A
        mov    eax, dword ptr [esp + 1ch]
        cmp    byte ptr [eax], 3
        jne    SHORT loc_5_5B13
        mov    edi, dword ptr [esp + 14h]
        add    edi, 2ch
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax + 15h], 2
        jmp    NEAR PTR loc_5_5C00
loc_5_5B13:
        push   16h
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 24h]
        db  0E8h, 0C2h, 014h, 0FBh, 0FFh
        mov    ecx, dword ptr [esp + 1ch]
        add    esp, 0ch
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_5_5B3D
        mov    word ptr [esp + 0ch], 0ffh
loc_5_5B3D:
        db  0A1h, 094h, 0B8h, 000h, 000h
        cmp    byte ptr [eax + 494h], 1
        sbb    cl, cl
        and    cl, 0feh
        mov    eax, dword ptr [esp + 10h]
        add    cl, 4
        mov    byte ptr [eax + 15h], cl
        jmp    SHORT loc_5_5B97
loc_5_5B5A:
        mov    eax, dword ptr [esp + 1ch]
        cmp    byte ptr [eax], 3
        jne    SHORT loc_5_5B6C
        mov    word ptr [esp + 0ch], 3
        jmp    SHORT loc_5_5B97
loc_5_5B6C:
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [esp + 14h]
        push   dword ptr [eax + 8]
        db  0E8h, 034h, 015h, 0FBh, 0FFh
        mov    word ptr [esp + 14h], ax
        add    esp, 8
        or     ax, ax
        mov    eax, dword ptr [esp + 10h]
        je     SHORT loc_5_5B93
        mov    byte ptr [eax + 15h], 1
        jmp    SHORT loc_5_5B97
loc_5_5B93:
        mov    byte ptr [eax + 15h], 4
loc_5_5B97:
        cmp    word ptr [esp + 0ch], 0
        je     SHORT loc_5_5BE9
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        je     SHORT loc_5_5BBE
        push   eax
        db  0E8h, 05Ch, 015h, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 14h]
        add    esp, 4
        mov    dword ptr [eax + 8], 0
loc_5_5BBE:
        cmp    word ptr [esp + 0ch], 3
        je     SHORT loc_5_5BD5
        cmp    word ptr [esp + 0ch], 12h
        mov    word ptr [esp + 0ch], 0ffh
        jne    SHORT loc_5_5BDC
loc_5_5BD5:
        mov    word ptr [esp + 0ch], 9ch
loc_5_5BDC:
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax + 15h], 1
        jmp    NEAR PTR loc_5_5CF5
loc_5_5BE9:
        mov    eax, dword ptr [esp + 14h]
        test   byte ptr [eax], 10h
        je     NEAR PTR loc_5_5B5A
        cmp    byte ptr [eax + 2ch], 2eh
        je     NEAR PTR loc_5_5B5A
loc_5_5C00:
        mov    ax, word ptr [esp + 0eh]
        add    ax, 2
        push   eax
        db  0E8h, 051h, 0DEh, 0FAh, 0FFh
        mov    ebx, dword ptr [esp + 30h]
        add    esp, 4
        mov    word ptr [ebx + 22h], ax
        mov    esi, dword ptr [esp + 10h]
        movzx  eax, byte ptr [esi + 2dh]
        add    esi, 2dh
        inc    eax
        mov    edi, dword ptr [esp + 1ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [esp + 14h]
        add    edx, 2bh
        mov    esi, dword ptr [esp + 14h]
        add    esi, 2ch
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [edx], cl
        push   0ffh
        mov    eax, dword ptr [esp + 20h]
        push   0ffh
        mov    al, byte ptr [eax]
        push   eax
        push   edx
        push   dword ptr [esp + 2ch]
        db  0E8h, 01Fh, 0C0h, 0FAh, 0FFh
        mov    word ptr [esp + 20h], ax
        add    esp, 14h
        or     ax, ax
        je     SHORT loc_5_5C88
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax + 15h], 1
        jmp    SHORT loc_5_5CF5
loc_5_5C88:
        mov    byte ptr [ebx + 20h], 0ffh
        lea    edx, [ebx + 8]
        mov    edi, edx
        sub    eax, eax
        stosd
        stosd
        stosd
        mov    edi, dword ptr [esp + 14h]
        add    edi, 234h
        cmp    byte ptr [edi], al
        je     SHORT loc_5_5CA6
        jmp    SHORT loc_5_5CA8
loc_5_5CA6:
        mov    edi, esi
loc_5_5CA8:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 14h]
        push   dword ptr [eax + 4]
        db  0E8h, 095h, 0DDh, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [ebx + 18h], eax
        db  0A1h, 020h, 09Bh, 000h, 000h
        mov    dword ptr [ebx + 1ch], eax
        mov    eax, dword ptr [esp + 30h]
        mov    byte ptr [ebx + 21h], 0
        mov    word ptr [eax], 1ch
        mov    word ptr [esp + 0ch], 0
loc_5_5CF5:
        push   dword ptr [esp + 14h]
        db  0E8h, 0D8h, 0D6h, 0FAh, 0FFh
        add    esp, 4
loc_5_5D01:
        push   dword ptr [esp + 1ch]
        db  0E8h, 04Fh, 0D6h, 0FAh, 0FFh
        add    esp, 4
loc_5_5D0D:
        push   dword ptr [esp + 10h]
        call   sub_5_7FAD
        add    esp, 4
loc_5_5D19:
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_5_5D2C
        push   dword ptr [esp + 20h]
        call   sub_5_3340
        add    esp, 4
loc_5_5D2C:
        mov    ax, word ptr [esp + 0ch]
loc_5_5D31:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_5_5D38:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [esp + 28h], 0
        mov    al, byte ptr [eax + 9]
        mov    byte ptr [esp + 0fh], al
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    ecx, dword ptr [esp + 30h]
        add    ecx, 0ch
        mov    byte ptr [eax + 493h], 0
        mov    eax, dword ptr [ecx]
        cmp    eax, 1
        je     SHORT loc_5_5D6F
        or     eax, eax
        jne    SHORT loc_5_5D75
loc_5_5D6F:
        mov    dword ptr [ecx], 0ffffffffh
loc_5_5D75:
        mov    eax, dword ptr [ecx]
        cmp    eax, -1
        jne    NEAR PTR loc_5_5E23
        push   4
        push   4080h
        mov    esi, 1
        lea    eax, [esp + 1ch]
        push   dword ptr [esp + 44h]
        push   eax
        mov    eax, dword ptr [esp + 40h]
        add    eax, 10h
        push   eax
        mov    eax, dword ptr [esp + 44h]
        mov    al, byte ptr [eax + 0ah]
        push   eax
        call   sub_5_5232
        add    esp, 18h
        or     ax, ax
        je     SHORT loc_5_5DBB
        mov    ax, 0ffh
        jmp    NEAR PTR loc_5_6121
loc_5_5DBB:
        mov    eax, dword ptr [esp + 14h]
        mov    ax, word ptr [eax + 1ah]
        mov    word ptr [esp + 12h], ax
loc_5_5DC8:
        mov    eax, dword ptr [esp + 14h]
        push   4
        mov    byte ptr [eax + 15h], 1
        lea    eax, [esp + 2ch]
        push   0
        push   eax
        mov    eax, dword ptr [esp + 3ch]
        push   4080h
        add    eax, 10h
        push   eax
        mov    eax, dword ptr [esp + 44h]
        push   1
        movzx  eax, byte ptr [eax + 0ah]
        sub    edx, edx
        lea    ecx, [esp + 3ch]
        push   eax
        call   sub_5_62A6
        mov    word ptr [esp + 10h], ax
        or     ax, ax
        jne    NEAR PTR loc_5_60FD
        db  0E8h, 062h, 0D5h, 0FAh, 0FFh
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        jne    SHORT loc_5_5E4F
        mov    word ptr [esp + 10h], 96h
        jmp    NEAR PTR loc_5_60F1
loc_5_5E23:
        sub    esi, esi
        sub    ax, 2
        mov    word ptr [esp + 12h], ax
        push   esi
        push   dword ptr [esp + 16h]
        call   sub_5_7DFC
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        cmp    eax, esi
        jne    SHORT loc_5_5DC8
        mov    word ptr [esp + 10h], 0ffh
        jmp    NEAR PTR loc_5_6109
loc_5_5E4F:
        or     esi, esi
        je     SHORT loc_5_5EC4
        mov    eax, dword ptr [esp + 24h]
        cmp    byte ptr [eax], 3
        jne    SHORT loc_5_5E75
        mov    edi, dword ptr [esp + 18h]
        add    edi, 2ch
        sub    eax, eax
        stosd
        stosd
        stosd
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [eax + 15h], 2
        jmp    NEAR PTR loc_5_5F63
loc_5_5E75:
        mov    eax, dword ptr [esp + 30h]
        movzx  ax, byte ptr [eax + 0bh]
        push   eax
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 2ch]
        db  0E8h, 030h, 011h, 0FBh, 0FFh
        mov    ecx, dword ptr [esp + 20h]
        add    esp, 0ch
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_5_5EA7
        mov    word ptr [esp + 10h], 0ffh
loc_5_5EA7:
        db  0A1h, 094h, 0B8h, 000h, 000h
        cmp    byte ptr [eax + 494h], 1
        sbb    cl, cl
        and    cl, 0feh
        mov    eax, dword ptr [esp + 14h]
        add    cl, 4
        mov    byte ptr [eax + 15h], cl
        jmp    SHORT loc_5_5F10
loc_5_5EC4:
        mov    eax, dword ptr [esp + 24h]
        cmp    byte ptr [eax], 3
        je     SHORT loc_5_5EF3
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esp + 20h], eax
        or     eax, eax
        je     SHORT loc_5_5EF3
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 24h]
        db  0E8h, 0C7h, 011h, 0FBh, 0FFh
        mov    word ptr [esp + 18h], ax
        add    esp, 8
        jmp    SHORT loc_5_5EFA
loc_5_5EF3:
        mov    word ptr [esp + 10h], 0ffh
loc_5_5EFA:
        cmp    word ptr [esp + 10h], 0
        mov    eax, dword ptr [esp + 14h]
        je     SHORT loc_5_5F0C
        mov    byte ptr [eax + 15h], 1
        jmp    SHORT loc_5_5F10
loc_5_5F0C:
        mov    byte ptr [eax + 15h], 4
loc_5_5F10:
        cmp    word ptr [esp + 10h], 0
        je     SHORT loc_5_5F4B
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        je     SHORT loc_5_5F37
        push   eax
        db  0E8h, 0E3h, 011h, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 18h]
        add    esp, 4
        mov    dword ptr [eax + 8], 0
loc_5_5F37:
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [esp + 10h], 0ffh
        mov    byte ptr [eax + 15h], 1
        jmp    NEAR PTR loc_5_60E5
loc_5_5F4B:
        mov    eax, dword ptr [esp + 30h]
        test   byte ptr [eax + 0bh], 10h
        je     SHORT loc_5_5F63
        mov    eax, dword ptr [esp + 18h]
        cmp    byte ptr [eax + 2ch], 2eh
        je     NEAR PTR loc_5_5EC4
loc_5_5F63:
        mov    ebx, dword ptr [esp + 34h]
        sub    eax, eax
        lea    edx, [ebx + 8]
        mov    edi, edx
        mov    ecx, 23h
        rep stosd
        movzx  eax, word ptr [esp + 12h]
        add    eax, 2
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ebx + 0ch], 0ffffffffh
        movzx  eax, word ptr [eax]
        mov    dword ptr [ebx + 10h], eax
        mov    byte ptr [ebx + 15h], al
        mov    byte ptr [ebx + 14h], 0
        lea    edx, [ebx + 18h]
        mov    byte ptr [ebx + 16h], 0
        mov    dword ptr [ebx + 2ch], 0
        mov    dword ptr [ebx + 30h], 0
        db  0A1h, 020h, 09Bh, 000h, 000h
        mov    edi, edx
        mov    dword ptr [ebx + 28h], eax
        mov    dword ptr [ebx + 6ch], 1
        sub    eax, eax
        stosd
        stosd
        stosd
        mov    edi, dword ptr [esp + 18h]
        add    edi, 234h
        cmp    byte ptr [edi], al
        je     SHORT loc_5_5FD2
        jmp    SHORT loc_5_5FD9
loc_5_5FD2:
        mov    edi, dword ptr [esp + 18h]
        add    edi, 2ch
loc_5_5FD9:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    eax, dword ptr [esp + 18h]
        mov    byte ptr [ebx + 17h], cl
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebx + 34h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebx + 24h], eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ebx + 38h], 0
        test   byte ptr [eax], 10h
        je     SHORT loc_5_60A8
        mov    dword ptr [ebx + 6ch], 1
        mov    esi, dword ptr [esp + 14h]
        movzx  eax, byte ptr [esi + 2dh]
        add    esi, 2dh
        inc    eax
        mov    edi, dword ptr [esp + 24h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [esp + 18h]
        add    edx, 2bh
        mov    edi, dword ptr [esp + 18h]
        add    edi, 2ch
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [edx], cl
        push   0ffh
        mov    eax, dword ptr [esp + 28h]
        push   0ffh
        mov    al, byte ptr [eax]
        push   eax
        push   edx
        push   dword ptr [esp + 34h]
        db  0E8h, 007h, 0BCh, 0FAh, 0FFh
        mov    word ptr [esp + 24h], ax
        add    esp, 14h
        or     ax, ax
        je     SHORT loc_5_60A0
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [eax + 15h], 1
        jmp    SHORT loc_5_60E5
loc_5_60A0:
        mov    word ptr [ebx + 70h], 0ffffh
        jmp    SHORT loc_5_60C4
loc_5_60A8:
        mov    eax, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax + 20h]
        mov    dword ptr [ebx + 3ch], eax
        mov    eax, dword ptr [esp + 18h]
        mov    word ptr [ebx + 6ch], 0ffffh
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ebx + 6eh], ax
loc_5_60C4:
        mov    al, byte ptr [esp + 0fh]
        sub    al, 28h
        cmp    al, 1
        sbb    ecx, ecx
        and    ecx, 8
        mov    eax, dword ptr [esp + 38h]
        add    ecx, 84h
        mov    word ptr [eax], cx
        mov    word ptr [esp + 10h], 0
loc_5_60E5:
        push   dword ptr [esp + 18h]
        db  0E8h, 0E8h, 0D2h, 0FAh, 0FFh
        add    esp, 4
loc_5_60F1:
        push   dword ptr [esp + 24h]
        db  0E8h, 05Fh, 0D2h, 0FAh, 0FFh
        add    esp, 4
loc_5_60FD:
        push   dword ptr [esp + 14h]
        call   sub_5_7FAD
        add    esp, 4
loc_5_6109:
        cmp    dword ptr [esp + 28h], 0
        je     SHORT loc_5_611C
        push   dword ptr [esp + 28h]
        call   sub_5_3340
        add    esp, 4
loc_5_611C:
        mov    ax, word ptr [esp + 10h]
loc_5_6121:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_6130:
        mov    ax, 0c9h
        ret
loc_5_6135:
        push   edi
        mov    eax, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        mov    word ptr [eax], 18h
        add    edi, 8
        sub    eax, eax
        mov    ecx, 6
        rep stosd
        pop    edi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_5_6160:
        mov    ecx, 1
        mov    ax, word ptr [esp + 4]
        cmp    ax, 0ffh
        jbe    SHORT loc_5_6176
        mov    ax, 0ffh
        jmp    SHORT loc_5_6191
loc_5_6176:
        sub    edx, edx
        movzx  eax, ax
loc_5_617B:
        test   eax, ecx
        jne    SHORT loc_5_6189
        inc    edx
        add    ecx, ecx
        cmp    edx, 8
        jb     SHORT loc_5_617B
        jmp    SHORT loc_5_6191
loc_5_6189:
        db  066h, 08Bh, 004h, 055h, 068h, 0AFh, 000h, 000h
loc_5_6191:
        ret
sub_5_6192:
        sub    esp, 4
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    word ptr [esp + 2], 0
        mov    ecx, dword ptr [eax]
        cmp    byte ptr [ecx + 6ah], 3
        je     SHORT loc_5_61FD
        test   byte ptr [ecx + 1ah], 10h
        jne    SHORT loc_5_61FD
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        je     SHORT loc_5_61D8
        mov    eax, dword ptr [eax + 8]
        test   al, 0eh
        je     SHORT loc_5_61D8
        test   al, 0ah
        je     SHORT loc_5_61CB
        mov    word ptr [esp + 2], 89h
        jmp    SHORT loc_5_61F6
loc_5_61CB:
        test   al, 4
        je     SHORT loc_5_61F6
        mov    word ptr [esp + 2], 0
        jmp    SHORT loc_5_61F6
loc_5_61D8:
        lea    eax, [esp + 2]
        push   eax
        push   0
        push   0ffh
        add    ecx, 73h
        push   dword ptr [esp + 18h]
        push   ecx
        push   0
        db  0E8h, 0A7h, 0DBh, 0FAh, 0FFh
        add    esp, 18h
loc_5_61F6:
        mov    ax, word ptr [esp + 2]
        jmp    SHORT loc_5_6201
loc_5_61FD:
        mov    ax, 1ffh
loc_5_6201:
        add    esp, 4
        ret
sub_5_6205:
        sub    esp, 4
        push   esi
        push   edi
        mov    word ptr [esp + 0ah], 0
        sub    di, di
        mov    si, word ptr [esp + 18h]
        cmp    si, di
        je     SHORT loc_5_629D
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    ecx, dword ptr [eax]
        cmp    byte ptr [ecx + 6ah], 3
        je     SHORT loc_5_629D
        test   byte ptr [ecx + 1ah], 10h
        jne    SHORT loc_5_629D
        mov    eax, dword ptr [esp + 10h]
        or     eax, eax
        je     SHORT loc_5_625A
        mov    eax, dword ptr [eax + 8]
        test   al, 0eh
        je     SHORT loc_5_625A
        test   al, 0ah
        je     SHORT loc_5_624D
        mov    word ptr [esp + 0ah], 89h
        jmp    SHORT loc_5_627B
loc_5_624D:
        test   al, 4
        je     SHORT loc_5_627B
        mov    word ptr [esp + 0ah], 0
        jmp    SHORT loc_5_627B
loc_5_625A:
        lea    eax, [esp + 0ah]
        push   eax
        push   0
        mov    ax, si
        and    ax, 0bfffh
        push   eax
        add    ecx, 73h
        push   dword ptr [esp + 20h]
        push   ecx
        push   0
        db  0E8h, 022h, 0DBh, 0FAh, 0FFh
        add    esp, 18h
loc_5_627B:
        mov    ax, word ptr [esp + 0ah]
        not    ax
        and    ax, si
        and    ax, 0bfffh
        je     SHORT loc_5_6298
        push   eax
        call   sub_5_6160
        add    esp, 4
        mov    di, ax
loc_5_6298:
        mov    ax, di
        jmp    SHORT loc_5_62A0
loc_5_629D:
        sub    ax, ax
loc_5_62A0:
        pop    edi
        pop    esi
        add    esp, 4
        ret
sub_5_62A6:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 12h], dl
        sub    ebp, ebp
        mov    dword ptr [esp + 2ch], ecx
        mov    dword ptr [esp + 24h], ebp
        mov    dword ptr [esp + 28h], ebp
        mov    dword ptr [esp + 1ch], ebp
        db  0E8h, 038h, 0D0h, 0FAh, 0FFh
        mov    ebx, eax
        cmp    ebx, ebp
        jne    SHORT loc_5_62D7
        mov    ax, 96h
        jmp    NEAR PTR loc_5_639A
loc_5_62D7:
        cmp    byte ptr [esp + 12h], 0ffh
        je     NEAR PTR loc_5_63AA
        cmp    dword ptr [esp + 34h], 0
        je     NEAR PTR loc_5_63AA
        push   dword ptr [esp + 3ch]
        db  0E8h, 0C6h, 0B5h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_5_63AA
        cmp    byte ptr [esp + 12h], 0
        jne    SHORT loc_5_6330
        movzx  esi, byte ptr [esp + 34h]
        push   esi
        call   sub_5_458D
        add    esp, 4
        mov    di, ax
        or     di, di
        jne    SHORT loc_5_637B
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 40h]
        mov    ebp, dword ptr [eax + esi*4]
        mov    ecx, dword ptr [ebp]
        jmp    SHORT loc_5_6356
loc_5_6330:
        cmp    byte ptr [esp + 12h], 1
        jne    SHORT loc_5_63A4
        mov    eax, dword ptr [esp + 34h]
        shr    eax, 10h
        push   eax
        push   dword ptr [esp + 38h]
        call   sub_5_7DFC
        mov    dword ptr [esp + 24h], eax
        add    esp, 8
        or     eax, eax
        je     SHORT loc_5_63A4
        mov    ecx, dword ptr [eax + 0ch]
loc_5_6356:
        mov    dword ptr [esp + 18h], ecx
        mov    ecx, dword ptr [esp + 18h]
        inc    dword ptr [ecx + 4]
        test   byte ptr [ecx + 0ah], 1
        je     NEAR PTR loc_5_6406
        mov    di, 98h
loc_5_636F:
        push   dword ptr [esp + 18h]
        call   sub_5_3340
        add    esp, 4
loc_5_637B:
        push   ebx
        db  0E8h, 0D8h, 0CFh, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_5_6397
        push   dword ptr [esp + 1ch]
        call   sub_5_7FAD
        add    esp, 4
loc_5_6397:
        mov    ax, di
loc_5_639A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret    1ch
loc_5_63A4:
        mov    di, 0ceh
        jmp    SHORT loc_5_637B
loc_5_63AA:
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [esp + 40h]
        call   sub_5_4697
        mov    dword ptr [esp + 20h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_63C9
        mov    di, 98h
        jmp    SHORT loc_5_637B
loc_5_63C9:
        mov    edx, dword ptr [esp + 3ch]
        mov    cl, byte ptr [edx]
        movzx  eax, cl
        cmp    dword ptr [esp + 24h], eax
        jb     SHORT loc_5_63E6
        dec    byte ptr [esp + 38h]
        lea    edx, [eax + edx + 1]
        mov    dword ptr [esp + 3ch], edx
        jmp    SHORT loc_5_6406
loc_5_63E6:
        mov    dword ptr [esp + 28h], 1
        mov    edx, dword ptr [esp + 3ch]
        add    edx, dword ptr [esp + 24h]
        mov    dword ptr [esp + 3ch], edx
        mov    al, byte ptr [edx]
        mov    byte ptr [esp + 13h], al
        sub    cl, byte ptr [esp + 24h]
        mov    byte ptr [edx], cl
loc_5_6406:
        cmp    dword ptr [esp + 1ch], 0
        jne    SHORT loc_5_6461
        mov    esi, dword ptr [esp + 18h]
        movzx  eax, byte ptr [esi + 24h]
        add    esi, 24h
        inc    eax
        mov    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  esi, byte ptr [ebx]
        mov    dword ptr [esp + 20h], esi
        cmp    esi, 3
        jbe    SHORT loc_5_6436
        dec    esi
loc_5_6436:
        or     ebp, ebp
        je     SHORT loc_5_64A3
        push   0ffh
        push   0ffh
        push   dword ptr [esp + 28h]
        push   dword ptr [ebp + 4]
        push   ebx
        db  0E8h, 03Fh, 0B8h, 0FAh, 0FFh
        add    esp, 14h
        mov    di, ax
        or     di, di
        je     SHORT loc_5_64A3
        jmp    NEAR PTR loc_5_636F
loc_5_6461:
        mov    esi, dword ptr [esp + 1ch]
        movzx  eax, byte ptr [esi + 2dh]
        add    esi, 2dh
        inc    eax
        mov    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 18h]
        movzx  esi, byte ptr [ecx + 24h]
        mov    dword ptr [esp + 20h], esi
        cmp    esi, 3
        jbe    SHORT loc_5_648F
        dec    esi
loc_5_648F:
        push   dword ptr [esp + 1ch]
        call   sub_5_7FAD
        mov    dword ptr [esp + 20h], 0
        add    esp, 4
loc_5_64A3:
        movzx  eax, byte ptr [esp + 38h]
        mov    dword ptr [esp + 14h], eax
        or     eax, eax
        je     SHORT loc_5_64F2
        mov    ebp, dword ptr [esp + 3ch]
        dec    dword ptr [esp + 14h]
        or     eax, eax
        je     SHORT loc_5_64F2
loc_5_64BC:
        push   dword ptr [esp + 4ch]
        push   0ffh
        push   dword ptr [esp + 28h]
        push   ebp
        push   ebx
        db  0E8h, 0C0h, 0B7h, 0FAh, 0FFh
        add    esp, 14h
        mov    di, ax
        or     di, di
        jne    NEAR PTR loc_5_636F
        movzx  eax, byte ptr [ebp]
        inc    eax
        add    ebp, eax
        mov    eax, dword ptr [esp + 14h]
        dec    dword ptr [esp + 14h]
        or     eax, eax
        jne    SHORT loc_5_64BC
loc_5_64F2:
        cmp    dword ptr [esp + 28h], 0
        je     SHORT loc_5_6503
        mov    edx, dword ptr [esp + 3ch]
        mov    al, byte ptr [esp + 13h]
        mov    byte ptr [edx], al
loc_5_6503:
        mov    al, byte ptr [ebx]
        cmp    al, 3
        jbe    SHORT loc_5_6523
        movzx  eax, al
        add    eax, ebx
        inc    eax
        push   eax
        lea    eax, [ebx + 1]
        push   eax
        db  0E8h, 0F7h, 0B1h, 0FAh, 0FFh
        add    esp, 8
        cmp    byte ptr [eax], 5ch
        jne    SHORT loc_5_6523
        dec    byte ptr [ebx]
loc_5_6523:
        movzx  eax, byte ptr [ebx]
        mov    byte ptr [ebx + eax + 1], 0
        cmp    esi, 3
        jbe    SHORT loc_5_6538
        movzx  eax, byte ptr [ebx]
        cmp    eax, esi
        jbe    SHORT loc_5_6538
        inc    esi
loc_5_6538:
        push   dword ptr [esp + 40h]
        lea    eax, [ebx + 1]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_5_6205
        add    esp, 0ch
        mov    di, ax
        or     di, di
        jne    NEAR PTR loc_5_636F
        mov    eax, dword ptr [esp + 48h]
        or     eax, eax
        je     SHORT loc_5_6562
        mov    dword ptr [eax], esi
loc_5_6562:
        mov    eax, dword ptr [esp + 44h]
        or     eax, eax
        je     SHORT loc_5_6572
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_5_657E
loc_5_6572:
        push   dword ptr [esp + 18h]
        call   sub_5_3340
        add    esp, 4
loc_5_657E:
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [eax], ebx
        sub    ax, ax
        jmp    NEAR PTR loc_5_639A
sub_5_658C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 12h], 0
        cmp    byte ptr [esp + 28h], 0
        je     SHORT loc_5_65BA
        db  083h, 03Dh, 044h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_5_65B1
        cmp    byte ptr [esp + 28h], 4
        je     SHORT loc_5_65BA
loc_5_65B1:
        mov    ax, 0bfh
        jmp    NEAR PTR loc_5_66F0
loc_5_65BA:
        mov    ebp, dword ptr [esp + 24h]
        mov    ebx, dword ptr [esp + 1ch]
        cmp    byte ptr [ebp + 234h], 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 14h], eax
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 13h
        rep stosd
        movzx  eax, word ptr [ebp]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebx + 0ah], eax
        lea    esi, [ebp + 4]
        cmp    dword ptr [esp + 14h], 1
        cmc
        sbb    eax, eax
        and    eax, 4
        mov    dword ptr [ebx + 48h], eax
        lea    edi, [ebx + 14h]
        movsd
        lea    esi, [ebp + 14h]
        lea    edi, [ebx + 1ch]
        movsd
        lea    esi, [ebp + 0eh]
        lea    edi, [ebx + 24h]
        movsw
        test   byte ptr [esp + 21h], 4
        mov    word ptr [ebx + 2eh], 0ffffh
        je     SHORT loc_5_668D
        mov    eax, dword ptr [esp + 30h]
        or     eax, eax
        jne    SHORT loc_5_6670
        push   dword ptr [esp + 34h]
        call   sub_5_7D1A
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_663D
        mov    word ptr [esp + 12h], 96h
        jmp    NEAR PTR loc_5_66EB
loc_5_663D:
        mov    esi, dword ptr [esp + 38h]
        mov    dword ptr [eax + 0ch], esi
        inc    dword ptr [esi + 4]
        mov    word ptr [eax + 1ch], 0ffffh
        mov    dword ptr [eax + 8], 0
        mov    byte ptr [eax + 15h], 2
        movzx  edi, word ptr [eax + 18h]
        shl    edi, 10h
        movsx  ecx, word ptr [eax + 1ah]
        or     edi, ecx
        push   eax
        call   sub_5_7FAD
        add    esp, 4
        jmp    SHORT loc_5_6680
loc_5_6670:
        mov    esi, dword ptr [eax + 0ch]
        movzx  edi, word ptr [eax + 18h]
        shl    edi, 10h
        movsx  eax, word ptr [eax + 1ah]
        or     edi, eax
loc_5_6680:
        mov    dword ptr [ebx + 30h], edi
        mov    dword ptr [ebx + 34h], edi
        movzx  eax, byte ptr [esi + 1eh]
        mov    dword ptr [ebx + 38h], eax
loc_5_668D:
        test   byte ptr [esp + 20h], 1
        je     SHORT loc_5_66DD
        cmp    byte ptr [esp + 28h], 0
        jne    SHORT loc_5_66A8
        cmp    dword ptr [esp + 14h], 0
        lea    esi, [ebp + 234h]
        jne    SHORT loc_5_66AB
loc_5_66A8:
        lea    esi, [ebp + 2ch]
loc_5_66AB:
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [ebx + 4ch], cl
        movzx  edx, cl
        lea    edi, [ebx + 4dh]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 2ch]
        or     eax, eax
        je     SHORT loc_5_66EB
        inc    edx
        mov    dword ptr [eax], edx
        jmp    SHORT loc_5_66EB
loc_5_66DD:
        mov    eax, dword ptr [esp + 2ch]
        or     eax, eax
        je     SHORT loc_5_66EB
        mov    dword ptr [eax], 0
loc_5_66EB:
        mov    ax, word ptr [esp + 12h]
loc_5_66F0:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_66F8:
        sub    esp, 8
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [esp + 8], 0
        mov    al, byte ptr [ecx + 8]
        or     al, al
        je     SHORT loc_5_6726
        db  083h, 03Dh, 044h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_5_671D
        cmp    al, 4
        je     SHORT loc_5_6726
loc_5_671D:
        mov    ax, 0bfh
        jmp    NEAR PTR loc_5_680E
loc_5_6726:
        push   eax
        lea    eax, [esp + 0ch]
        push   0
        push   eax
        lea    eax, [ecx + 11h]
        push   0
        push   eax
        mov    al, byte ptr [ecx + 10h]
        push   eax
        push   dword ptr [ecx + 0bh]
        mov    dl, byte ptr [ecx + 0fh]
        lea    ecx, [esp + 28h]
        call   sub_5_62A6
        mov    si, ax
        or     si, si
        je     SHORT loc_5_6754
        jmp    NEAR PTR loc_5_680B
loc_5_6754:
        push   dword ptr [esp + 0ch]
        db  0E8h, 017h, 0DEh, 0FAh, 0FFh
        add    esp, 4
        cmp    ax, 0ffffh
        jne    SHORT loc_5_676C
        mov    si, 9ch
        jmp    SHORT loc_5_67A4
loc_5_676C:
        mov    eax, dword ptr [esp + 0ch]
        push   0
        inc    eax
        push   eax
        push   dword ptr [esp + 10h]
        call   sub_5_4329
        add    esp, 0ch
        mov    di, ax
        test   di, 40h
        jne    SHORT loc_5_6790
        mov    si, 89h
        jmp    SHORT loc_5_67A4
loc_5_6790:
        push   dword ptr [esp + 0ch]
        call   sub_5_7D1A
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_67B2
        mov    si, 96h
loc_5_67A4:
        push   dword ptr [esp + 8]
        call   sub_5_3340
        add    esp, 4
        jmp    SHORT loc_5_67FF
loc_5_67B2:
        mov    byte ptr [eax + 15h], 1
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax + 0ch], ecx
        mov    word ptr [eax + 1ch], di
        mov    edx, dword ptr [esp + 18h]
        mov    dword ptr [eax + 10h], 0ffffffffh
        mov    dword ptr [edx + 0dh], 0ffffffffh
        mov    cx, word ptr [eax + 18h]
        mov    word ptr [edx + 0bh], cx
        mov    ecx, dword ptr [esp + 8]
        push   eax
        mov    cl, byte ptr [ecx + 1eh]
        mov    byte ptr [edx + 8], cl
        mov    cx, word ptr [eax + 1ah]
        mov    word ptr [edx + 9], cx
        call   sub_5_7FAD
        mov    eax, dword ptr [esp + 20h]
        add    esp, 4
        mov    word ptr [eax], 9
loc_5_67FF:
        push   dword ptr [esp + 0ch]
        db  0E8h, 051h, 0CBh, 0FAh, 0FFh
        add    esp, 4
loc_5_680B:
        mov    ax, si
loc_5_680E:
        pop    edi
        pop    esi
        add    esp, 8
        ret
loc_5_6814:
        sub    esp, 8
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [esp + 8], 0
        mov    al, byte ptr [ecx + 8]
        or     al, al
        je     SHORT loc_5_6842
        db  083h, 03Dh, 044h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_5_6839
        cmp    al, 4
        je     SHORT loc_5_6842
loc_5_6839:
        mov    ax, 0bfh
        jmp    NEAR PTR loc_5_68FF
loc_5_6842:
        push   eax
        lea    eax, [esp + 0ch]
        push   0
        push   eax
        lea    eax, [ecx + 13h]
        push   0
        push   eax
        mov    al, byte ptr [ecx + 12h]
        push   eax
        push   dword ptr [ecx + 0dh]
        mov    dl, byte ptr [ecx + 11h]
        lea    ecx, [esp + 28h]
        call   sub_5_62A6
        mov    si, ax
        or     si, si
        je     SHORT loc_5_6870
        jmp    NEAR PTR loc_5_68FC
loc_5_6870:
        push   dword ptr [esp + 0ch]
        db  0E8h, 0FBh, 0DCh, 0FAh, 0FFh
        add    esp, 4
        cmp    ax, 0ffffh
        jne    SHORT loc_5_6888
        mov    si, 9ch
        jmp    SHORT loc_5_689E
loc_5_6888:
        push   dword ptr [esp + 0ch]
        call   sub_5_7D1A
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_5_68AC
        mov    si, 96h
loc_5_689E:
        push   dword ptr [esp + 8]
        call   sub_5_3340
        add    esp, 4
        jmp    SHORT loc_5_68F0
loc_5_68AC:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edi + 0ch], eax
        mov    word ptr [edi + 1ch], 0ffffh
        mov    byte ptr [edi + 15h], 2
        push   edi
        call   sub_5_7FAD
        mov    eax, dword ptr [esp + 0ch]
        add    esp, 4
        mov    al, byte ptr [eax + 1eh]
        mov    edx, dword ptr [esp + 18h]
        mov    byte ptr [edx + 10h], al
        movzx  ecx, word ptr [edi + 18h]
        shl    ecx, 10h
        movsx  eax, word ptr [edi + 1ah]
        or     ecx, eax
        mov    dword ptr [edx + 0ch], ecx
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edx + 8], ecx
        mov    word ptr [eax], 9
loc_5_68F0:
        push   dword ptr [esp + 0ch]
        db  0E8h, 060h, 0CAh, 0FAh, 0FFh
        add    esp, 4
loc_5_68FC:
        mov    ax, si
loc_5_68FF:
        pop    edi
        pop    esi
        add    esp, 8
        ret
loc_5_6905:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 38h]
        mov    al, byte ptr [ebp + 8]
        add    ebp, 8
        mov    byte ptr [esp + 11h], al
        or     al, al
        je     SHORT loc_5_6934
        db  083h, 03Dh, 044h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_5_692B
        cmp    al, 4
        je     SHORT loc_5_6934
loc_5_692B:
        mov    ax, 0bfh
        jmp    NEAR PTR loc_5_6CA6
loc_5_6934:
        mov    eax, dword ptr [esp + 38h]
        mov    eax, dword ptr [eax + 0ch]
        or     eax, 1
        mov    dword ptr [esp + 30h], eax
        mov    eax, dword ptr [esp + 38h]
        mov    ax, word ptr [eax + 13h]
        push   eax
        mov    eax, dword ptr [esp + 3ch]
        mov    ax, word ptr [eax + 11h]
        push   eax
        call   sub_5_7DFC
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_6970
        mov    word ptr [esp + 12h], 0ffh
        jmp    NEAR PTR loc_5_6CA1
loc_5_6970:
        mov    ebx, dword ptr [esp + 14h]
        add    ebx, 15h
        mov    byte ptr [ebx], 1
        db  0E8h, 081h, 0C9h, 0FAh, 0FFh
        mov    dword ptr [esp + 28h], eax
        or     eax, eax
        jne    SHORT loc_5_6993
        mov    word ptr [esp + 12h], 96h
        jmp    NEAR PTR loc_5_6C95
loc_5_6993:
        db  0E8h, 0D9h, 0C9h, 0FAh, 0FFh
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        jne    SHORT loc_5_69AC
        mov    word ptr [esp + 12h], 96h
        jmp    NEAR PTR loc_5_6C89
loc_5_69AC:
        mov    esi, dword ptr [esp + 14h]
        movzx  eax, byte ptr [esi + 2dh]
        add    esi, 2dh
        inc    eax
        mov    edi, dword ptr [esp + 28h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    al, byte ptr [ebp]
        push   eax
        mov    eax, dword ptr [esp + 2ch]
        push   0ffh
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [esp + 44h]
        add    eax, 19h
        push   eax
        push   dword ptr [esp + 38h]
        db  0E8h, 0A5h, 0B2h, 0FAh, 0FFh
        mov    word ptr [esp + 26h], ax
        add    esp, 14h
        cmp    ax, 9ch
        jne    SHORT loc_5_6A00
        mov    word ptr [esp + 12h], 0ffh
loc_5_6A00:
        cmp    word ptr [esp + 12h], 0
        jne    NEAR PTR loc_5_6C7D
        mov    eax, dword ptr [esp + 38h]
        cmp    dword ptr [eax + 15h], -1
        je     SHORT loc_5_6A24
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [eax + 8], 0
        jne    NEAR PTR loc_5_6AF5
loc_5_6A24:
        mov    esi, dword ptr [esp + 14h]
        mov    eax, dword ptr [esi + 8]
        add    esi, 8
        or     eax, eax
        je     SHORT loc_5_6A41
        push   eax
        db  0E8h, 0D4h, 006h, 0FBh, 0FFh
        add    esp, 4
        mov    dword ptr [esi], 0
loc_5_6A41:
        mov    eax, dword ptr [esp + 28h]
        push   4080h
        inc    eax
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   dword ptr [eax + 0ch]
        call   sub_5_6205
        mov    word ptr [esp + 1eh], ax
        add    esp, 0ch
        or     ax, ax
        jne    NEAR PTR loc_5_6C7D
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [eax + 10h], 0ffffffffh
        mov    eax, dword ptr [esp + 38h]
        mov    ax, word ptr [eax + 0ah]
        push   eax
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 30h]
        db  0E8h, 07Ah, 005h, 0FBh, 0FFh
        add    esp, 0ch
        mov    dword ptr [esi], eax
        or     eax, eax
        jne    SHORT loc_5_6A9F
        mov    word ptr [esp + 12h], 0ffh
        jmp    NEAR PTR loc_5_6C7D
loc_5_6A9F:
        db  0A1h, 094h, 0B8h, 000h, 000h
        cmp    byte ptr [eax + 494h], 1
        sbb    al, al
        and    al, 0feh
        add    al, 4
        mov    byte ptr [ebx], al
loc_5_6AB3:
        mov    eax, dword ptr [esp + 38h]
        mov    ax, word ptr [eax + 0ah]
        mov    word ptr [esp + 20h], ax
        and    ax, 10h
        mov    word ptr [esp + 24h], ax
        or     ax, ax
        je     SHORT loc_5_6AD8
        mov    eax, dword ptr [esp + 18h]
        cmp    byte ptr [eax + 2ch], 2eh
        je     SHORT loc_5_6AF5
loc_5_6AD8:
        cmp    word ptr [esp + 24h], 0
        je     SHORT loc_5_6B51
        test   byte ptr [esp + 21h], 80h
        jne    SHORT loc_5_6B51
        mov    eax, dword ptr [esp + 18h]
        test   byte ptr [eax], 10h
        jne    SHORT loc_5_6B51
        db  0E8h, 0BCh, 028h, 0FBh, 0FFh
loc_5_6AF5:
        mov    eax, dword ptr [esp + 14h]
        add    eax, 8
        mov    dword ptr [esp + 1ch], eax
        push   dword ptr [esp + 18h]
        push   dword ptr [eax]
        db  0E8h, 0A5h, 005h, 0FBh, 0FFh
        mov    word ptr [esp + 1ah], ax
        add    esp, 8
        or     ax, ax
        je     SHORT loc_5_6B44
        mov    eax, dword ptr [esp + 1ch]
        mov    word ptr [esp + 12h], 0ffh
        push   dword ptr [eax]
        db  0E8h, 0E2h, 005h, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 20h]
        add    esp, 4
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [eax + 15h], 1
        jmp    NEAR PTR loc_5_6C7D
loc_5_6B44:
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [eax + 15h], 4
        jmp    NEAR PTR loc_5_6AB3
loc_5_6B51:
        mov    eax, dword ptr [esp + 14h]
        mov    ebx, dword ptr [esp + 3ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [esp + 14h]
        add    ecx, 10h
        mov    al, byte ptr [eax + 1eh]
        mov    byte ptr [ebx + 8], al
        mov    eax, dword ptr [esp + 14h]
        mov    ax, word ptr [eax + 1ah]
        mov    word ptr [ebx + 9], ax
        mov    eax, dword ptr [ecx]
        inc    eax
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebx + 0dh], eax
        mov    eax, dword ptr [esp + 14h]
        push   0
        mov    ax, word ptr [eax + 18h]
        push   0
        mov    word ptr [ebx + 0bh], ax
        lea    eax, [esp + 34h]
        push   dword ptr [esp + 1ch]
        push   eax
        lea    eax, [ebx + 12h]
        push   dword ptr [esp + 21h]
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 48h]
        push   eax
        call   sub_5_658C
        mov    eax, dword ptr [esp + 38h]
        add    esp, 20h
        test   byte ptr [eax], 10h
        je     SHORT loc_5_6C23
        mov    esi, dword ptr [esp + 14h]
        movzx  eax, byte ptr [esi + 2dh]
        add    esi, 2dh
        inc    eax
        mov    edi, dword ptr [esp + 28h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [esp + 18h]
        add    edx, 2bh
        mov    edi, dword ptr [esp + 18h]
        add    edi, 2ch
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [edx], cl
        push   0ffh
        mov    eax, dword ptr [esp + 2ch]
        push   0ffh
        mov    al, byte ptr [eax]
        push   eax
        push   edx
        push   dword ptr [esp + 38h]
        db  0E8h, 084h, 0B0h, 0FAh, 0FFh
        mov    word ptr [esp + 26h], ax
        add    esp, 14h
        or     ax, ax
        je     SHORT loc_5_6C23
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [eax + 15h], 1
        jmp    SHORT loc_5_6C7D
loc_5_6C23:
        mov    esi, dword ptr [esp + 18h]
        add    esi, 234h
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        or     cl, cl
        je     SHORT loc_5_6C52
        mov    eax, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 14h]
        mov    byte ptr [eax + 1eh], cl
        movzx  eax, cl
        add    edi, 1fh
        jmp    SHORT loc_5_6C60
loc_5_6C52:
        mov    eax, dword ptr [esp + 2ch]
        lea    esi, [ebx + 5eh]
        mov    edi, dword ptr [esp + 14h]
        add    edi, 1eh
loc_5_6C60:
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [esp + 2ch]
        mov    eax, dword ptr [esp + 40h]
        add    cx, 56h
        mov    word ptr [eax], cx
loc_5_6C7D:
        push   dword ptr [esp + 18h]
        db  0E8h, 050h, 0C7h, 0FAh, 0FFh
        add    esp, 4
loc_5_6C89:
        push   dword ptr [esp + 28h]
        db  0E8h, 0C7h, 0C6h, 0FAh, 0FFh
        add    esp, 4
loc_5_6C95:
        push   dword ptr [esp + 14h]
        call   sub_5_7FAD
        add    esp, 4
loc_5_6CA1:
        mov    ax, word ptr [esp + 12h]
loc_5_6CA6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
loc_5_6CAE:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 24h]
        mov    dword ptr [esp + 14h], 0
        mov    cl, byte ptr [edi + 8]
        or     cl, cl
        je     SHORT loc_5_6CDF
        db  083h, 03Dh, 044h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_5_6CD6
        cmp    cl, 4
        je     SHORT loc_5_6CDF
loc_5_6CD6:
        mov    ax, 0bfh
        jmp    NEAR PTR loc_5_6DDB
loc_5_6CDF:
        mov    bl, byte ptr [edi + 9]
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esp + 1ch], eax
        or     bl, bl
        je     SHORT loc_5_6D04
        db  083h, 03Dh, 044h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_5_6CFB
        cmp    bl, 4
        je     SHORT loc_5_6D04
loc_5_6CFB:
        mov    ax, 0bfh
        jmp    NEAR PTR loc_5_6DDB
loc_5_6D04:
        push   ecx
        lea    eax, [esp + 18h]
        push   0
        push   eax
        lea    eax, [edi + 17h]
        push   4080h
        push   eax
        mov    al, byte ptr [edi + 16h]
        push   eax
        push   dword ptr [edi + 11h]
        mov    dl, byte ptr [edi + 15h]
        lea    ecx, [esp + 2ch]
        call   sub_5_62A6
        mov    si, ax
        or     si, si
        je     SHORT loc_5_6D35
        jmp    NEAR PTR loc_5_6DD8
loc_5_6D35:
        db  0E8h, 037h, 0C6h, 0FAh, 0FFh
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_5_6D46
        mov    si, 96h
        jmp    SHORT loc_5_6DB9
loc_5_6D46:
        mov    eax, dword ptr [esp + 10h]
        cmp    byte ptr [eax], 3
        jne    NEAR PTR loc_5_6DE3
        test   word ptr [edi + 0ah], 8010h
        je     NEAR PTR loc_5_6DE3
        mov    byte ptr [ebp + 2ch], 0
        mov    byte ptr [ebp + 234h], 0
loc_5_6D6A:
        mov    edi, dword ptr [esp + 28h]
        push   dword ptr [esp + 14h]
        add    edi, 8
        push   dword ptr [esp + 14h]
        lea    eax, [esp + 20h]
        push   0
        push   eax
        push   ebx
        push   ebp
        push   dword ptr [esp + 34h]
        push   edi
        call   sub_5_658C
        mov    eax, dword ptr [esp + 30h]
        add    esp, 20h
        cmp    byte ptr [eax], 3
        jne    SHORT loc_5_6DA1
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
loc_5_6DA1:
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 2ch]
        add    cx, 4ch
        mov    word ptr [eax], cx
loc_5_6DB0:
        push   ebp
        db  0E8h, 020h, 0C6h, 0FAh, 0FFh
        add    esp, 4
loc_5_6DB9:
        push   dword ptr [esp + 10h]
        db  0E8h, 097h, 0C5h, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_6DD8
        push   dword ptr [esp + 14h]
        call   sub_5_3340
        add    esp, 4
loc_5_6DD8:
        mov    ax, si
loc_5_6DDB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_6DE3:
        mov    ax, word ptr [edi + 0ah]
        push   eax
        push   ebp
        push   dword ptr [esp + 18h]
        db  0E8h, 046h, 002h, 0FBh, 0FFh
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_5_6E0C
        push   edi
        db  0E8h, 00Bh, 003h, 0FBh, 0FFh
        add    esp, 4
        or     edi, edi
        jne    NEAR PTR loc_5_6D6A
loc_5_6E0C:
        mov    si, 0ffh
        jmp    SHORT loc_5_6DB0
loc_5_6E12:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        mov    dword ptr [esp + 18h], 0
        mov    al, byte ptr [esi + 8]
        or     al, al
        je     SHORT loc_5_6E42
        db  083h, 03Dh, 044h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_5_6E39
        cmp    al, 4
        je     SHORT loc_5_6E42
loc_5_6E39:
        mov    ax, 0bfh
        jmp    NEAR PTR loc_5_6FE7
loc_5_6E42:
        mov    cx, 4002h
        mov    ebx, dword ptr [esi + 0ch]
        mov    edi, ebx
        and    edi, 2
        je     SHORT loc_5_6E54
        mov    cx, 4022h
loc_5_6E54:
        push   eax
        lea    eax, [esp + 1ch]
        push   0
        push   eax
        lea    eax, [esi + 3dh]
        push   ecx
        push   eax
        mov    al, byte ptr [esi + 3ch]
        push   eax
        push   dword ptr [esi + 37h]
        mov    dl, byte ptr [esi + 3bh]
        lea    ecx, [esp + 30h]
        call   sub_5_62A6
        mov    bp, ax
        or     bp, bp
        je     SHORT loc_5_6E81
        jmp    NEAR PTR loc_5_6FE4
loc_5_6E81:
        mov    eax, ebx
        and    eax, 0ch
        je     SHORT loc_5_6EF1
        cmp    eax, 0ch
        je     SHORT loc_5_6EB1
        lea    eax, [esp + 12h]
        push   8
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 20h]
        db  0E8h, 02Fh, 0D6h, 0FAh, 0FFh
        add    esp, 10h
        mov    bp, ax
        or     bp, bp
        jne    NEAR PTR loc_5_6F92
loc_5_6EB1:
        test   bl, 4
        je     SHORT loc_5_6EBF
        mov    ax, word ptr [esi + 14h]
        mov    word ptr [esp + 10h], ax
loc_5_6EBF:
        test   bl, 8
        je     SHORT loc_5_6ECD
        mov    ax, word ptr [esi + 16h]
        mov    word ptr [esp + 12h], ax
loc_5_6ECD:
        lea    eax, [esp + 12h]
        push   7
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 20h]
        db  0E8h, 0EFh, 0D5h, 0FAh, 0FFh
        add    esp, 10h
        mov    bp, ax
        or     bp, bp
        jne    NEAR PTR loc_5_6F92
loc_5_6EF1:
        mov    eax, ebx
        and    eax, 300h
        je     SHORT loc_5_6F5D
        cmp    eax, 300h
        je     SHORT loc_5_6F21
        lea    eax, [esp + 12h]
        push   4
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 20h]
        db  0E8h, 0BBh, 0D5h, 0FAh, 0FFh
        add    esp, 10h
        mov    bp, ax
        or     bp, bp
        jne    SHORT loc_5_6F92
loc_5_6F21:
        test   bh, 1
        je     SHORT loc_5_6F2F
        mov    ax, word ptr [esi + 1ch]
        mov    word ptr [esp + 10h], ax
loc_5_6F2F:
        test   bh, 2
        je     SHORT loc_5_6F3D
        mov    ax, word ptr [esi + 1eh]
        mov    word ptr [esp + 12h], ax
loc_5_6F3D:
        lea    eax, [esp + 12h]
        push   3
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 20h]
        db  0E8h, 07Fh, 0D5h, 0FAh, 0FFh
        add    esp, 10h
        mov    bp, ax
        or     bp, bp
        jne    SHORT loc_5_6F92
loc_5_6F5D:
        test   bh, 8
        je     SHORT loc_5_6F96
        mov    ax, word ptr [esi + 2ch]
        mov    word ptr [esp + 10h], ax
        mov    word ptr [esp + 12h], 0
        lea    eax, [esp + 12h]
        push   5
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 20h]
        db  0E8h, 04Ah, 0D5h, 0FAh, 0FFh
        add    esp, 10h
        mov    bp, ax
        or     bp, bp
        je     SHORT loc_5_6F96
loc_5_6F92:
        sub    edx, edx
        jmp    SHORT loc_5_6FBA
loc_5_6F96:
        or     edi, edi
        je     SHORT loc_5_6FC5
        mov    ax, word ptr [esi + 10h]
        and    al, 27h
        push   eax
        push   dword ptr [esp + 18h]
        db  0E8h, 00Ah, 0D5h, 0FAh, 0FFh
        add    esp, 8
        mov    bp, ax
        or     bp, bp
        je     SHORT loc_5_6FC5
        db  0BAh, 00Ch, 0B2h, 000h, 000h
loc_5_6FBA:
        mov    ecx, ebp
        db  0E8h, 03Fh, 0BAh, 0FAh, 0FFh
        movzx  bp, al
loc_5_6FC5:
        push   dword ptr [esp + 14h]
        db  0E8h, 08Bh, 0C3h, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_5_6FE4
        push   dword ptr [esp + 18h]
        call   sub_5_3340
        add    esp, 4
loc_5_6FE4:
        mov    ax, bp
loc_5_6FE7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_5_6FEF:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 24h]
        mov    dword ptr [esp + 10h], 0
        mov    al, byte ptr [ebp + 8]
        or     al, al
        je     SHORT loc_5_701F
        db  083h, 03Dh, 044h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_5_7016
        cmp    al, 4
        je     SHORT loc_5_701F
loc_5_7016:
        mov    ax, 0bfh
        jmp    NEAR PTR loc_5_7160
loc_5_701F:
        mov    bl, byte ptr [ebp + 9]
        or     bl, bl
        je     SHORT loc_5_703D
        db  083h, 03Dh, 044h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_5_7034
        cmp    bl, 4
        je     SHORT loc_5_703D
loc_5_7034:
        mov    ax, 0bfh
        jmp    NEAR PTR loc_5_7160
loc_5_703D:
        mov    edi, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [esp + 14h]
        push   0
        push   eax
        lea    eax, [ebp + 17h]
        push   0
        push   eax
        mov    al, byte ptr [ebp + 16h]
        push   eax
        push   dword ptr [ebp + 11h]
        mov    dl, byte ptr [ebp + 15h]
        lea    ecx, [esp + 30h]
        call   sub_5_62A6
        mov    si, ax
        or     si, si
        je     SHORT loc_5_706E
        jmp    NEAR PTR loc_5_715D
loc_5_706E:
        db  0E8h, 0FEh, 0C2h, 0FAh, 0FFh
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        jne    SHORT loc_5_7084
        mov    si, 96h
        jmp    NEAR PTR loc_5_713E
loc_5_7084:
        mov    eax, dword ptr [esp + 14h]
        cmp    byte ptr [eax], 3
        jne    SHORT loc_5_70B8
        and    edi, 1
        cmp    edi, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 1ch], eax
        mov    edx, eax
        mov    ebx, dword ptr [esp + 28h]
        add    edx, 4ch
        lea    edi, [ebx + 0ah]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        jmp    SHORT loc_5_710B
loc_5_70B8:
        mov    ax, word ptr [ebp + 0ah]
        push   eax
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        db  0E8h, 03Ah, 0FFh, 0FAh, 0FFh
        add    esp, 0ch
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_5_70E0
        push   ebp
        db  0E8h, 033h, 000h, 0FBh, 0FFh
        add    esp, 4
        or     ebp, ebp
        jne    SHORT loc_5_70E6
loc_5_70E0:
        mov    si, 0ffh
        jmp    SHORT loc_5_7132
loc_5_70E6:
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 18h]
        lea    eax, [esp + 24h]
        push   0
        push   eax
        push   ebx
        push   dword ptr [esp + 2ch]
        mov    ebx, dword ptr [esp + 40h]
        push   edi
        lea    eax, [ebx + 0ah]
        push   eax
        call   sub_5_658C
        add    esp, 20h
loc_5_710B:
        mov    eax, dword ptr [esp + 14h]
        push   0
        inc    eax
        push   eax
        push   dword ptr [esp + 18h]
        call   sub_5_4329
        add    esp, 0ch
        mov    word ptr [ebx + 8], ax
        mov    ecx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esp + 2ch]
        add    cx, 4eh
        mov    word ptr [eax], cx
loc_5_7132:
        push   dword ptr [esp + 18h]
        db  0E8h, 09Bh, 0C2h, 0FAh, 0FFh
        add    esp, 4
loc_5_713E:
        push   dword ptr [esp + 14h]
        db  0E8h, 012h, 0C2h, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_5_715D
        push   dword ptr [esp + 10h]
        call   sub_5_3340
        add    esp, 4
loc_5_715D:
        mov    ax, si
loc_5_7160:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_7168:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    bp, bp
        mov    word ptr [esp + 14h], bp
        mov    word ptr [esp + 16h], bp
        mov    esi, dword ptr [esp + 34h]
        mov    dword ptr [esp + 1ch], 0
        mov    al, byte ptr [esi + 8]
        sub    bx, bx
        mov    byte ptr [esp + 11h], al
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [esp + 28h], eax
        cmp    byte ptr [esp + 11h], bl
        je     SHORT loc_5_71B8
        db  083h, 03Dh, 044h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_5_71AF
        cmp    byte ptr [esp + 11h], 4
        je     SHORT loc_5_71B8
loc_5_71AF:
        mov    ax, 0bfh
        jmp    NEAR PTR loc_5_7417
loc_5_71B8:
        mov    al, byte ptr [esi + 9]
        mov    cl, al
        and    cl, 8
        mov    dl, cl
        shr    dl, 3
        and    eax, 2
        movzx  edx, dl
        mov    edi, eax
        db  066h, 00Fh, 0B6h, 0BCh, 03Ah, 088h, 0B0h, 000h, 000h
        or     cl, cl
        je     SHORT loc_5_71DE
        mov    bx, 4
loc_5_71DE:
        or     al, al
        je     SHORT loc_5_71E6
        or     bx, 6
loc_5_71E6:
        mov    cx, word ptr [esi + 14h]
        test   cl, 10h
        jne    SHORT loc_5_7203
        mov    ax, cx
        and    ax, 0ch
        shr    ax, 1
        movzx  eax, ax
        db  066h, 08Bh, 0A8h, 04Ch, 0B0h, 000h, 000h
loc_5_7203:
        and    ecx, 3
        lea    eax, [ecx*2]
        push   dword ptr [esp + 11h]
        db  066h, 00Bh, 0A8h, 054h, 0B0h, 000h, 000h
        db  066h, 00Bh, 098h, 05Ch, 0B0h, 000h, 000h
        lea    eax, [esp + 20h]
        push   0
        push   eax
        lea    eax, [esi + 1dh]
        push   ebx
        push   eax
        mov    al, byte ptr [esi + 1ch]
        push   eax
        push   dword ptr [esi + 17h]
        mov    dl, byte ptr [esi + 1bh]
        lea    ecx, [esp + 34h]
        call   sub_5_62A6
        mov    word ptr [esp + 12h], ax
        or     ax, ax
        je     SHORT loc_5_724D
        jmp    NEAR PTR loc_5_7417
loc_5_724D:
        db  0E8h, 01Fh, 0C1h, 0FAh, 0FFh
        mov    dword ptr [esp + 20h], eax
        or     eax, eax
        jne    SHORT loc_5_7266
        mov    word ptr [esp + 12h], 96h
        jmp    NEAR PTR loc_5_73F3
loc_5_7266:
        push   dword ptr [esp + 18h]
        db  0E8h, 005h, 0D3h, 0FAh, 0FFh
        add    esp, 4
        cmp    ax, 0ffffh
        jne    NEAR PTR loc_5_731A
        test   di, 10h
        jne    SHORT loc_5_728F
        mov    word ptr [esp + 12h], 0a5h
        jmp    NEAR PTR loc_5_73E7
loc_5_728F:
        test   byte ptr [esi + 10h], 10h
        je     SHORT loc_5_72BE
        push   dword ptr [esp + 18h]
        db  0E8h, 0EAh, 0D1h, 0FAh, 0FFh
        mov    word ptr [esp + 16h], ax
        add    esp, 4
        or     ax, ax
        je     SHORT loc_5_72B7
        mov    word ptr [esp + 12h], 0ffh
        jmp    NEAR PTR loc_5_73E7
loc_5_72B7:
        mov    word ptr [esp + 16h], 2
loc_5_72BE:
        or     di, 1
        mov    cx, word ptr [esi + 10h]
        test   cl, 10h
        jne    SHORT loc_5_7332
        lea    eax, [esp + 12h]
        push   eax
        and    cx, 0ffh
        lea    eax, [esp + 1ah]
        push   eax
        push   edi
        push   ebp
        mov    ax, word ptr [esi + 0ah]
        shl    ax, 8
        add    ax, cx
        push   eax
        push   dword ptr [esp + 2ch]
        db  0E8h, 0AFh, 0B7h, 0FAh, 0FFh
        mov    word ptr [esp + 2ch], ax
        add    esp, 18h
        cmp    word ptr [esp + 12h], 0
        je     SHORT loc_5_7332
        sub    edx, edx
        mov    ecx, dword ptr [esp + 12h]
        db  0E8h, 0F4h, 0B6h, 0FAh, 0FFh
        movzx  ax, al
        mov    word ptr [esp + 12h], ax
        jmp    NEAR PTR loc_5_73E7
loc_5_731A:
        test   bl, 4
        je     SHORT loc_5_72BE
        test   di, 1
        jne    SHORT loc_5_72BE
        mov    word ptr [esp + 12h], 0ffh
        jmp    NEAR PTR loc_5_73E7
loc_5_7332:
        push   16h
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 20h]
        db  0E8h, 0A3h, 0FCh, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_7354
        mov    word ptr [esp + 12h], 96h
        jmp    NEAR PTR loc_5_73E7
loc_5_7354:
        push   eax
        db  0E8h, 0B2h, 0FDh, 0FAh, 0FFh
        add    esp, 4
        cmp    word ptr [esp + 14h], 0
        je     SHORT loc_5_7398
        push   dword ptr [esp + 1ch]
        movzx  eax, word ptr [esp + 18h]
        push   dword ptr [esp + 1ch]
        push   eax
        push   dword ptr [esp + 4ch]
        db  0E8h, 064h, 0F4h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_5_739C
        push   dword ptr [esp + 14h]
        db  0E8h, 071h, 0B8h, 0FAh, 0FFh
        mov    word ptr [esp + 16h], 96h
        add    esp, 4
        jmp    SHORT loc_5_73E7
loc_5_7398:
        mov    eax, dword ptr [esp + 2ch]
loc_5_739C:
        mov    ecx, dword ptr [esp + 38h]
        mov    dword ptr [ecx + 8], eax
        movzx  eax, word ptr [esp + 16h]
        db  08Ah, 080h, 08Ch, 0B0h, 000h, 000h
        mov    byte ptr [ecx + 0ch], al
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        add    ecx, 0eh
        lea    eax, [esp + 2ch]
        push   0
        push   eax
        push   dword ptr [esp + 21h]
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 40h]
        push   ecx
        call   sub_5_658C
        mov    ecx, dword ptr [esp + 44h]
        add    esp, 20h
        add    cx, 52h
        mov    eax, dword ptr [esp + 3ch]
        mov    word ptr [eax], cx
loc_5_73E7:
        push   dword ptr [esp + 20h]
        db  0E8h, 0E6h, 0BFh, 0FAh, 0FFh
        add    esp, 4
loc_5_73F3:
        push   dword ptr [esp + 18h]
        db  0E8h, 05Dh, 0BFh, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_5_7412
        push   dword ptr [esp + 1ch]
        call   sub_5_3340
        add    esp, 4
loc_5_7412:
        mov    ax, word ptr [esp + 12h]
loc_5_7417:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
loc_5_741F:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 12h], 0
        sub    bx, bx
        mov    word ptr [esp + 14h], bx
        mov    word ptr [esp + 16h], bx
        mov    edi, dword ptr [esp + 30h]
        mov    dword ptr [esp + 1ch], 0
        mov    al, byte ptr [edi + 8]
        sub    cx, cx
        mov    byte ptr [esp + 11h], al
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esp + 24h], eax
        cmp    byte ptr [esp + 11h], cl
        je     SHORT loc_5_7476
        db  083h, 03Dh, 044h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_5_746D
        cmp    byte ptr [esp + 11h], 4
        je     SHORT loc_5_7476
loc_5_746D:
        mov    ax, 0bfh
        jmp    NEAR PTR loc_5_76C4
loc_5_7476:
        cmp    byte ptr [edi + 9], 0
        je     SHORT loc_5_7485
        mov    ax, 0beh
        jmp    NEAR PTR loc_5_76C4
loc_5_7485:
        mov    dx, word ptr [edi + 0ah]
        mov    si, dx
        and    si, 8
        mov    ax, si
        shr    ax, 3
        and    edx, 2
        movzx  eax, ax
        mov    ebp, edx
        db  066h, 00Fh, 0B6h, 0ACh, 028h, 088h, 0B0h, 000h, 000h
        or     si, si
        je     SHORT loc_5_74B1
        mov    cx, 4
loc_5_74B1:
        or     dx, dx
        je     SHORT loc_5_74BA
        or     cx, 6
loc_5_74BA:
        or     bp, 1
        mov    dx, word ptr [edi + 18h]
        test   dl, 10h
        jne    SHORT loc_5_74DB
        mov    ax, dx
        and    ax, 0ch
        shr    ax, 1
        movzx  eax, ax
        db  066h, 08Bh, 098h, 04Ch, 0B0h, 000h, 000h
loc_5_74DB:
        and    edx, 3
        lea    eax, [edx*2]
        push   dword ptr [esp + 11h]
        db  066h, 00Bh, 098h, 054h, 0B0h, 000h, 000h
        db  066h, 00Bh, 088h, 05Ch, 0B0h, 000h, 000h
        lea    eax, [esp + 20h]
        push   0
        push   eax
        lea    eax, [edi + 21h]
        push   ecx
        push   eax
        mov    al, byte ptr [edi + 20h]
        push   eax
        push   dword ptr [edi + 1bh]
        mov    dl, byte ptr [edi + 1fh]
        lea    ecx, [esp + 34h]
        call   sub_5_62A6
        mov    word ptr [esp + 12h], ax
        or     ax, ax
        je     SHORT loc_5_7525
        jmp    NEAR PTR loc_5_76C4
loc_5_7525:
        db  0E8h, 047h, 0BEh, 0FAh, 0FFh
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_5_753C
        mov    word ptr [esp + 12h], 96h
        jmp    NEAR PTR loc_5_76A0
loc_5_753C:
        push   dword ptr [esp + 18h]
        db  0E8h, 02Fh, 0D0h, 0FAh, 0FFh
        add    esp, 4
        cmp    ax, 0ffffh
        jne    SHORT loc_5_7590
        test   bp, 10h
        jne    SHORT loc_5_7561
        mov    word ptr [esp + 12h], 0a5h
        jmp    NEAR PTR loc_5_7697
loc_5_7561:
        test   byte ptr [edi + 14h], 10h
        je     SHORT loc_5_7590
        push   dword ptr [esp + 18h]
        db  0E8h, 018h, 0CFh, 0FAh, 0FFh
        mov    word ptr [esp + 16h], ax
        add    esp, 4
        or     ax, ax
        je     SHORT loc_5_7589
        mov    word ptr [esp + 12h], 0ffh
        jmp    NEAR PTR loc_5_7697
loc_5_7589:
        mov    word ptr [esp + 16h], 2
loc_5_7590:
        mov    cx, word ptr [edi + 14h]
        test   cl, 10h
        jne    SHORT loc_5_75E8
        lea    eax, [esp + 12h]
        push   eax
        and    cx, 0ffh
        lea    eax, [esp + 1ah]
        push   eax
        push   ebp
        push   ebx
        mov    ax, word ptr [edi + 0ch]
        shl    ax, 8
        add    ax, cx
        push   eax
        push   dword ptr [esp + 2ch]
        db  0E8h, 0E1h, 0B4h, 0FAh, 0FFh
        mov    word ptr [esp + 2ch], ax
        add    esp, 18h
        cmp    word ptr [esp + 12h], 0
        je     SHORT loc_5_75E8
        sub    edx, edx
        mov    ecx, dword ptr [esp + 12h]
        db  0E8h, 026h, 0B4h, 0FAh, 0FFh
        movzx  ax, al
        mov    word ptr [esp + 12h], ax
        jmp    NEAR PTR loc_5_7697
loc_5_75E8:
        push   16h
        push   esi
        push   dword ptr [esp + 20h]
        db  0E8h, 0F0h, 0F9h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_7607
        mov    word ptr [esp + 12h], 96h
        jmp    NEAR PTR loc_5_7697
loc_5_7607:
        push   eax
        db  0E8h, 0FFh, 0FAh, 0FAh, 0FFh
        add    esp, 4
        cmp    word ptr [esp + 14h], 0
        je     SHORT loc_5_764B
        push   dword ptr [esp + 1ch]
        movzx  eax, word ptr [esp + 18h]
        push   dword ptr [esp + 1ch]
        push   eax
        push   dword ptr [esp + 48h]
        db  0E8h, 0B1h, 0F1h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_5_764F
        push   dword ptr [esp + 14h]
        db  0E8h, 0BEh, 0B5h, 0FAh, 0FFh
        mov    word ptr [esp + 16h], 96h
        add    esp, 4
        jmp    SHORT loc_5_7697
loc_5_764B:
        mov    eax, dword ptr [esp + 28h]
loc_5_764F:
        mov    ecx, dword ptr [esp + 34h]
        mov    dword ptr [ecx + 8], eax
        movzx  eax, word ptr [esp + 16h]
        db  08Ah, 080h, 08Ch, 0B0h, 000h, 000h
        mov    byte ptr [ecx + 0ch], al
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        add    ecx, 0eh
        lea    eax, [esp + 28h]
        push   0
        push   eax
        push   dword ptr [esp + 21h]
        push   esi
        push   dword ptr [esp + 3ch]
        push   ecx
        call   sub_5_658C
        mov    ecx, dword ptr [esp + 40h]
        add    esp, 20h
        add    cx, 52h
        mov    eax, dword ptr [esp + 38h]
        mov    word ptr [eax], cx
loc_5_7697:
        push   esi
        db  0E8h, 039h, 0BDh, 0FAh, 0FFh
        add    esp, 4
loc_5_76A0:
        push   dword ptr [esp + 18h]
        db  0E8h, 0B0h, 0BCh, 0FAh, 0FFh
        add    esp, 4
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_5_76BF
        push   dword ptr [esp + 1ch]
        call   sub_5_3340
        add    esp, 4
loc_5_76BF:
        mov    ax, word ptr [esp + 12h]
loc_5_76C4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
loc_5_76CC:
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        mov    al, byte ptr [edi + 8]
        or     al, al
        je     SHORT loc_5_76F2
        db  083h, 03Dh, 044h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_5_76E9
        cmp    al, 4
        je     SHORT loc_5_76F2
loc_5_76E9:
        mov    ax, 0bfh
        jmp    NEAR PTR loc_5_779C
loc_5_76F2:
        push   eax
        push   0
        push   0
        push   10h
        lea    esi, [edi + 1ah]
        push   esi
        mov    al, byte ptr [edi + 12h]
        push   eax
        push   dword ptr [edi + 0dh]
        mov    dl, byte ptr [edi + 11h]
        lea    ecx, [esp + 24h]
        call   sub_5_62A6
        or     ax, ax
        jne    NEAR PTR loc_5_779C
        movzx  ecx, byte ptr [edi + 12h]
        mov    eax, ecx
        dec    ecx
        or     eax, eax
        je     SHORT loc_5_7731
loc_5_7724:
        movzx  eax, byte ptr [esi]
        inc    eax
        add    esi, eax
        mov    eax, ecx
        dec    ecx
        or     eax, eax
        jne    SHORT loc_5_7724
loc_5_7731:
        mov    al, byte ptr [edi + 8]
        push   eax
        push   0
        push   0
        push   4
        push   esi
        mov    al, byte ptr [edi + 19h]
        push   eax
        push   dword ptr [edi + 14h]
        mov    dl, byte ptr [edi + 18h]
        lea    ecx, [esp + 28h]
        call   sub_5_62A6
        mov    si, ax
        or     si, si
        jne    SHORT loc_5_778D
        mov    ax, word ptr [edi + 0ah]
        push   eax
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        db  0E8h, 0E7h, 0CCh, 0FAh, 0FFh
        add    esp, 0ch
        mov    si, ax
        or     si, si
        je     SHORT loc_5_7781
        sub    edx, edx
        mov    ecx, esi
        db  0E8h, 083h, 0B2h, 0FAh, 0FFh
        movzx  si, al
loc_5_7781:
        push   dword ptr [esp + 0ch]
        db  0E8h, 0CFh, 0BBh, 0FAh, 0FFh
        add    esp, 4
loc_5_778D:
        push   dword ptr [esp + 8]
        db  0E8h, 0C3h, 0BBh, 0FAh, 0FFh
        add    esp, 4
        mov    ax, si
loc_5_779C:
        pop    edi
        pop    esi
        add    esp, 8
        ret
loc_5_77A2:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    al, byte ptr [esi + 8]
        or     al, al
        je     SHORT loc_5_77C4
        db  083h, 03Dh, 044h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_5_77BE
        cmp    al, 4
        je     SHORT loc_5_77C4
loc_5_77BE:
        mov    ax, 0bfh
        jmp    SHORT loc_5_782E
loc_5_77C4:
        push   eax
        push   0
        push   0
        push   10h
        lea    eax, [esi + 13h]
        push   eax
        mov    al, byte ptr [esi + 12h]
        push   eax
        push   dword ptr [esi + 0dh]
        mov    dl, byte ptr [esi + 11h]
        lea    ecx, [esp + 20h]
        call   sub_5_62A6
        or     ax, ax
        jne    SHORT loc_5_782E
        mov    ax, word ptr [esi + 0ah]
        test   al, 10h
        je     SHORT loc_5_77FD
        push   dword ptr [esp + 4]
        db  0E8h, 0A6h, 0CCh, 0FAh, 0FFh
        add    esp, 4
        jmp    SHORT loc_5_780A
loc_5_77FD:
        push   eax
        push   dword ptr [esp + 8]
        db  0E8h, 067h, 0CCh, 0FAh, 0FFh
        add    esp, 8
loc_5_780A:
        mov    si, ax
        or     si, si
        je     SHORT loc_5_781F
        sub    edx, edx
        mov    ecx, esi
        db  0E8h, 0E5h, 0B1h, 0FAh, 0FFh
        movzx  si, al
loc_5_781F:
        push   dword ptr [esp + 4]
        db  0E8h, 031h, 0BBh, 0FAh, 0FFh
        add    esp, 4
        mov    ax, si
loc_5_782E:
        pop    esi
        add    esp, 4
        ret
loc_5_7833:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 24h]
        mov    al, byte ptr [ebp + 8]
        or     al, al
        je     SHORT loc_5_785B
        db  083h, 03Dh, 044h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_5_7852
        cmp    al, 4
        je     SHORT loc_5_785B
loc_5_7852:
        mov    ax, 0bfh
        jmp    NEAR PTR loc_5_79A3
loc_5_785B:
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        push   0
        lea    eax, [ebp + 13h]
        push   eax
        mov    al, byte ptr [ebp + 12h]
        push   eax
        push   dword ptr [ebp + 0dh]
        mov    dl, byte ptr [ebp + 11h]
        lea    ecx, [esp + 30h]
        call   sub_5_62A6
        mov    bx, ax
        or     bx, bx
        je     SHORT loc_5_788C
        jmp    NEAR PTR loc_5_79A0
loc_5_788C:
        push   dword ptr [esp + 14h]
        db  0E8h, 0DFh, 0CCh, 0FAh, 0FFh
        add    esp, 4
        cmp    ax, 0ffffh
        je     NEAR PTR loc_5_7984
        test   al, 10h
        je     NEAR PTR loc_5_7984
        mov    esi, dword ptr [esp + 30h]
        push   esi
        call   sub_5_434B
        mov    byte ptr [esp + 17h], al
        add    esp, 4
        or     al, al
        jne    SHORT loc_5_78C8
        mov    bx, 9dh
        jmp    NEAR PTR loc_5_7988
loc_5_78C8:
        mov    ecx, dword ptr [esi + 40h]
        movzx  eax, byte ptr [esp + 13h]
        mov    edi, dword ptr [ecx + eax*4]
        mov    eax, dword ptr [esp + 1ch]
        inc    dword ptr [eax + 4]
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [edi], eax
        test   byte ptr [ebp + 0ah], 1
        je     SHORT loc_5_78F8
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    ax, word ptr [eax + 491h]
        mov    word ptr [edi + 8], ax
        jmp    SHORT loc_5_78FE
loc_5_78F8:
        mov    word ptr [edi + 8], 0ffffh
loc_5_78FE:
        mov    eax, dword ptr [esp + 14h]
        push   0
        movzx  eax, byte ptr [eax]
        sub    eax, dword ptr [esp + 1ch]
        inc    eax
        push   eax
        db  0E8h, 052h, 0CAh, 0FAh, 0FFh
        add    esp, 8
        mov    dword ptr [edi + 4], eax
        or     eax, eax
        jne    SHORT loc_5_792F
        push   dword ptr [esp + 13h]
        push   esi
        call   sub_5_437F
        add    esp, 8
        mov    bx, 96h
        jmp    SHORT loc_5_7988
loc_5_792F:
        mov    eax, dword ptr [esp + 14h]
        mov    edx, dword ptr [esp + 14h]
        mov    al, byte ptr [eax]
        sub    al, byte ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 18h]
        mov    byte ptr [edx + ecx], al
        mov    esi, dword ptr [esp + 14h]
        add    esi, dword ptr [esp + 18h]
        mov    edi, dword ptr [edi + 4]
        movzx  eax, byte ptr [esi]
        inc    eax
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 28h]
        mov    cl, byte ptr [ebp + 0ch]
        mov    byte ptr [eax + 9], cl
        mov    cl, byte ptr [esp + 13h]
        mov    byte ptr [eax + 8], cl
        mov    dword ptr [eax + 0ah], 0
        mov    eax, dword ptr [esp + 2ch]
        mov    word ptr [eax], 6
        jmp    SHORT loc_5_7988
loc_5_7984:
        mov    bx, 9ch
loc_5_7988:
        push   dword ptr [esp + 14h]
        db  0E8h, 0C8h, 0B9h, 0FAh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 1ch]
        call   sub_5_3340
        add    esp, 4
loc_5_79A0:
        mov    ax, bx
loc_5_79A3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_79AB:
        push   esi
        push   edi
        db  0BEh, 078h, 0AFh, 000h, 000h
        mov    edi, dword ptr [esp + 10h]
        add    edi, 8
        mov    ecx, 1ah
        rep movsd
        movsb
        mov    eax, dword ptr [esp + 14h]
        pop    edi
        mov    word ptr [eax], 69h
        sub    ax, ax
        pop    esi
        ret
loc_5_79D0:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, 1
        mov    byte ptr [eax + 0ah], 0
        db  083h, 03Dh, 044h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_5_79EF
        mov    byte ptr [eax + 0bh], 4
        mov    ecx, 2
loc_5_79EF:
        mov    word ptr [eax + 8], cx
        mov    eax, dword ptr [esp + 0ch]
        add    cx, 2
        mov    word ptr [eax], cx
        sub    ax, ax
        ret
loc_5_7A02:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        mov    word ptr [esp + 12h], 0
        mov    bl, byte ptr [esi + 0fh]
        or     bl, bl
        je     SHORT loc_5_7A32
        db  083h, 03Dh, 044h, 0AFh, 000h, 000h, 000h
        je     SHORT loc_5_7A29
        cmp    bl, 4
        je     SHORT loc_5_7A32
loc_5_7A29:
        mov    ax, 0bfh
        jmp    NEAR PTR loc_5_7BB7
loc_5_7A32:
        mov    eax, dword ptr [esi + 0bh]
        mov    ecx, eax
        shr    ecx, 10h
        push   ecx
        push   eax
        call   sub_5_7DFC
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_7A58
        mov    word ptr [esp + 12h], 0ffh
        jmp    NEAR PTR loc_5_7BA6
loc_5_7A58:
        mov    ebp, dword ptr [esp + 24h]
        mov    eax, dword ptr [esi + 0bh]
        mov    dword ptr [ebp + 8], eax
        cmp    bl, 4
        mov    dword ptr [ebp + 0ch], 0
        mov    word ptr [ebp + 10h], 0
        mov    word ptr [ebp + 12h], 0
        mov    byte ptr [ebp + 14h], 0
        mov    byte ptr [ebp + 15h], 14h
        mov    byte ptr [ebp + 16h], bl
        je     SHORT loc_5_7ABA
        add    ebp, 17h
        mov    edi, ebp
        sub    eax, eax
        stosd
        stosd
        stosd
        stosb
        mov    edi, ebp
        mov    esi, dword ptr [esp + 14h]
        movzx  eax, byte ptr [esi + 1eh]
        add    esi, 1eh
        inc    eax
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 28h]
        mov    word ptr [eax], 1ch
        jmp    NEAR PTR loc_5_7BA6
loc_5_7ABA:
        db  0E8h, 041h, 0B8h, 0FAh, 0FFh
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_5_7AD1
        mov    word ptr [esp + 12h], 96h
        jmp    NEAR PTR loc_5_7BA6
loc_5_7AD1:
        db  0E8h, 09Bh, 0B8h, 0FAh, 0FFh
        mov    dword ptr [esp + 18h], eax
        or     eax, eax
        jne    SHORT loc_5_7AEA
        mov    word ptr [esp + 12h], 96h
        jmp    NEAR PTR loc_5_7B9D
loc_5_7AEA:
        mov    esi, dword ptr [esp + 14h]
        movzx  eax, byte ptr [esi + 2dh]
        add    esi, 2dh
        inc    eax
        mov    edi, ebx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        push   0ffh
        push   0ffh
        mov    al, byte ptr [ebx]
        push   eax
        mov    eax, dword ptr [esp + 20h]
        add    eax, 1eh
        push   eax
        push   ebx
        db  0E8h, 06Fh, 0A1h, 0FAh, 0FFh
        mov    word ptr [esp + 26h], ax
        add    esp, 14h
        or     ax, ax
        je     SHORT loc_5_7B30
        jmp    SHORT loc_5_7B8A
loc_5_7B30:
        push   16h
        push   dword ptr [esp + 1ch]
        push   ebx
        db  0E8h, 0A8h, 0F4h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_5_7B8A
        push   eax
        db  0E8h, 0C3h, 0F5h, 0FAh, 0FFh
        mov    esi, dword ptr [esp + 1ch]
        add    esp, 4
        add    esi, 2ch
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [ebp + 17h], cl
        movzx  eax, cl
        lea    edi, [ebp + 18h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  cx, byte ptr [ebp + 17h]
        mov    eax, dword ptr [esp + 28h]
        add    cx, 10h
        mov    word ptr [eax], cx
        jmp    SHORT loc_5_7B91
loc_5_7B8A:
        mov    word ptr [esp + 12h], 0ffh
loc_5_7B91:
        push   dword ptr [esp + 18h]
        db  0E8h, 03Ch, 0B8h, 0FAh, 0FFh
        add    esp, 4
loc_5_7B9D:
        push   ebx
        db  0E8h, 0B6h, 0B7h, 0FAh, 0FFh
        add    esp, 4
loc_5_7BA6:
        push   dword ptr [esp + 14h]
        call   sub_5_7FAD
        mov    ax, word ptr [esp + 16h]
        add    esp, 4
loc_5_7BB7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
        db  000h
sub_5_7BC0:
        db  0A1h, 048h, 0B0h, 000h, 000h
loc_5_7BC5:
        add    eax, 0a2bh
        je     SHORT loc_5_7BC5
        db  0A3h, 048h, 0B0h, 000h, 000h
        ret
sub_5_7BD2:
        push   esi
        push   edi
        mov    cx, word ptr [esp + 0ch]
        db  08Bh, 015h, 058h, 0B9h, 000h, 000h
        movzx  eax, cx
        lea    esi, [eax*4]
        add    edx, esi
        db  0A1h, 048h, 0B9h, 000h, 000h
        mov    eax, dword ptr [esi + eax]
        or     eax, eax
        je     SHORT loc_5_7BFC
        mov    al, byte ptr [eax + 15h]
        jmp    SHORT loc_5_7BFE
loc_5_7BFC:
        sub    al, al
loc_5_7BFE:
        cmp    al, 4
        jb     SHORT loc_5_7C08
        db  0FFh, 00Dh, 05Ch, 0B9h, 000h, 000h
loc_5_7C08:
        db  066h, 039h, 00Dh, 052h, 0B9h, 000h, 000h
        jne    SHORT loc_5_7C2B
        mov    ax, word ptr [edx]
        db  066h, 039h, 00Dh, 044h, 0B9h, 000h, 000h
        db  066h, 0A3h, 052h, 0B9h, 000h, 000h
        jne    SHORT loc_5_7C4F
        db  066h, 0A3h, 044h, 0B9h, 000h, 000h
        jmp    SHORT loc_5_7C4F
loc_5_7C2B:
        db  066h, 039h, 00Dh, 044h, 0B9h, 000h, 000h
        jne    SHORT loc_5_7C3E
        mov    ax, word ptr [edx + 2]
        db  066h, 0A3h, 044h, 0B9h, 000h, 000h
loc_5_7C3E:
        mov    di, word ptr [edx]
        db  08Bh, 035h, 058h, 0B9h, 000h, 000h
        movzx  eax, word ptr [edx + 2]
        mov    word ptr [esi + eax*4], di
loc_5_7C4F:
        db  066h, 039h, 00Dh, 050h, 0B9h, 000h, 000h
        jne    SHORT loc_5_7C64
        mov    ax, word ptr [edx + 2]
        db  066h, 0A3h, 050h, 0B9h, 000h, 000h
        jmp    SHORT loc_5_7C76
loc_5_7C64:
        mov    si, word ptr [edx + 2]
        db  08Bh, 00Dh, 058h, 0B9h, 000h, 000h
        movzx  eax, word ptr [edx]
        mov    word ptr [ecx + eax*4 + 2], si
loc_5_7C76:
        pop    edi
        pop    esi
        ret
sub_5_7C79:
        push   ebx
        push   esi
        movzx  eax, word ptr [esp + 0ch]
        lea    esi, [eax*4]
        db  0A1h, 048h, 0B9h, 000h, 000h
        push   dword ptr [esi + eax]
        db  0E8h, 0CCh, 083h, 0FAh, 0FFh
        add    esp, 4
        db  0A1h, 048h, 0B9h, 000h, 000h
        mov    eax, dword ptr [esi + eax]
        cmp    dword ptr [eax + 8], 0
        je     SHORT loc_5_7CE3
        db  08Bh, 00Dh, 094h, 0B8h, 000h, 000h
        mov    bl, byte ptr [ecx + 491h]
        add    ecx, 491h
        movzx  ax, byte ptr [eax + 14h]
        mov    word ptr [ecx], ax
        db  0A1h, 048h, 0B9h, 000h, 000h
        mov    eax, dword ptr [esi + eax]
        push   dword ptr [eax + 8]
        db  0E8h, 03Dh, 0F4h, 0FAh, 0FFh
        add    esp, 4
        db  08Bh, 00Dh, 094h, 0B8h, 000h, 000h
        movzx  ax, bl
        mov    word ptr [ecx + 491h], ax
loc_5_7CE3:
        db  0A1h, 048h, 0B9h, 000h, 000h
        mov    eax, dword ptr [esi + eax]
        mov    eax, dword ptr [eax + 0ch]
        or     eax, eax
        je     SHORT loc_5_7CFB
        push   eax
        call   sub_5_3340
        add    esp, 4
loc_5_7CFB:
        db  0A1h, 048h, 0B9h, 000h, 000h
        push   dword ptr [esi + eax]
        db  0E8h, 088h, 0C6h, 0FAh, 0FFh
        add    esp, 4
        db  0A1h, 048h, 0B9h, 000h, 000h
        mov    dword ptr [esi + eax], 0
        pop    esi
        pop    ebx
        ret
sub_5_7D1A:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        push   201h
        movzx  eax, byte ptr [ebp]
        add    eax, 2fh
        push   eax
        db  0E8h, 030h, 0C6h, 0FAh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_5_7D44
        sub    eax, eax
        jmp    NEAR PTR loc_5_7DF7
loc_5_7D44:
        movzx  eax, byte ptr [ebp]
        inc    eax
        lea    edi, [ebx + 2dh]
        mov    esi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  eax, byte ptr [ebp]
        mov    byte ptr [ebx + eax + 2eh], 0
        call   sub_5_7BC0
        mov    word ptr [ebx + 18h], ax
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    al, byte ptr [eax + 491h]
        mov    byte ptr [ebx + 14h], al
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax + 24h]
        mov    word ptr [ebx + 16h], ax
        db  0FFh, 035h, 044h, 0B0h, 000h, 000h
        db  0E8h, 065h, 015h, 0FBh, 0FFh
        add    esp, 4
        db  066h, 08Bh, 035h, 052h, 0B9h, 000h, 000h
        push   esi
        call   sub_5_7BD2
        add    esp, 4
        movzx  eax, si
        lea    edi, [eax*4]
        db  0A1h, 048h, 0B9h, 000h, 000h
        cmp    dword ptr [edi + eax], 0
        je     SHORT loc_5_7DC7
        push   esi
        call   sub_5_7C79
        add    esp, 4
loc_5_7DC7:
        db  0A1h, 048h, 0B9h, 000h, 000h
        mov    dword ptr [edi + eax], ebx
        db  0FFh, 035h, 044h, 0B0h, 000h, 000h
        db  0E8h, 017h, 015h, 0FBh, 0FFh
        add    esp, 4
        mov    word ptr [ebx + 1ah], si
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    eax, dword ptr [eax]
        add    eax, 8
        push   eax
        push   ebx
        db  0E8h, 03Eh, 082h, 0FAh, 0FFh
        add    esp, 8
        mov    eax, ebx
loc_5_7DF7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_7DFC:
        push   esi
        push   edi
        mov    si, word ptr [esp + 0ch]
        or     si, si
        je     SHORT loc_5_7E77
        cmp    si, 400h
        ja     SHORT loc_5_7E77
        db  0FFh, 035h, 044h, 0B0h, 000h, 000h
        db  0E8h, 0E1h, 014h, 0FBh, 0FFh
        add    esp, 4
        db  08Bh, 00Dh, 048h, 0B9h, 000h, 000h
        movzx  eax, si
        mov    edi, dword ptr [ecx + eax*4]
        or     edi, edi
        je     SHORT loc_5_7E69
        mov    ax, word ptr [esp + 10h]
        or     ax, ax
        je     SHORT loc_5_7E3D
        cmp    word ptr [edi + 18h], ax
        jne    SHORT loc_5_7E69
loc_5_7E3D:
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    eax, dword ptr [eax]
        mov    cx, word ptr [edi + 16h]
        cmp    word ptr [eax + 24h], cx
        jne    SHORT loc_5_7E69
        push   esi
        call   sub_5_7BD2
        add    esp, 4
        db  0FFh, 035h, 044h, 0B0h, 000h, 000h
        db  0E8h, 08Fh, 014h, 0FBh, 0FFh
        add    esp, 4
        mov    eax, edi
        jmp    SHORT loc_5_7E79
loc_5_7E69:
        db  0FFh, 035h, 044h, 0B0h, 000h, 000h
        db  0E8h, 07Dh, 014h, 0FBh, 0FFh
        add    esp, 4
loc_5_7E77:
        sub    eax, eax
loc_5_7E79:
        pop    edi
        pop    esi
        ret
sub_5_7E7C:
        push   esi
        push   edi
        mov    si, word ptr [esp + 0ch]
        db  08Bh, 03Dh, 058h, 0B9h, 000h, 000h
        movzx  eax, si
        lea    ecx, [eax*4]
        add    edi, ecx
        db  0A1h, 048h, 0B9h, 000h, 000h
        mov    eax, dword ptr [ecx + eax]
        or     eax, eax
        mov    cl, 0
        je     SHORT loc_5_7EA6
        mov    cl, byte ptr [eax + 15h]
loc_5_7EA6:
        or     cl, cl
        jne    SHORT loc_5_7EE3
        or     eax, eax
        je     SHORT loc_5_7EB7
        push   esi
        call   sub_5_7C79
        add    esp, 4
loc_5_7EB7:
        db  066h, 0A1h, 052h, 0B9h, 000h, 000h
        mov    word ptr [edi], ax
        mov    word ptr [edi + 2], 0
        db  0A1h, 058h, 0B9h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 052h, 0B9h, 000h, 000h
        mov    word ptr [eax + ecx*4 + 2], si
        db  066h, 089h, 035h, 052h, 0B9h, 000h, 000h
        jmp    NEAR PTR loc_5_7FAA
loc_5_7EE3:
        cmp    cl, 4
        jb     SHORT loc_5_7F4E
        db  066h, 0A1h, 050h, 0B9h, 000h, 000h
        mov    word ptr [edi + 2], ax
        mov    word ptr [edi], 0
        db  08Bh, 00Dh, 058h, 0B9h, 000h, 000h
        db  00Fh, 0B7h, 005h, 050h, 0B9h, 000h, 000h
        mov    word ptr [ecx + eax*4], si
        db  081h, 03Dh, 05Ch, 0B9h, 000h, 000h, 000h, 002h, 000h, 000h
        db  066h, 089h, 035h, 050h, 0B9h, 000h, 000h
        jae    SHORT loc_5_7F26
        db  0FFh, 005h, 05Ch, 0B9h, 000h, 000h
        jmp    NEAR PTR loc_5_7FAA
loc_5_7F26:
        db  08Bh, 00Dh, 058h, 0B9h, 000h, 000h
        db  00Fh, 0B7h, 005h, 044h, 0B9h, 000h, 000h
        mov    ax, word ptr [ecx + eax*4]
        movzx  ecx, ax
        db  066h, 0A3h, 044h, 0B9h, 000h, 000h
        db  0A1h, 048h, 0B9h, 000h, 000h
        mov    eax, dword ptr [eax + ecx*4]
        mov    byte ptr [eax + 15h], 3
        jmp    SHORT loc_5_7FAA
loc_5_7F4E:
        db  066h, 0A1h, 044h, 0B9h, 000h, 000h
        mov    word ptr [edi + 2], ax
        db  08Bh, 00Dh, 058h, 0B9h, 000h, 000h
        db  00Fh, 0B7h, 005h, 044h, 0B9h, 000h, 000h
        mov    ax, word ptr [ecx + eax*4]
        mov    word ptr [edi], ax
        db  08Bh, 00Dh, 058h, 0B9h, 000h, 000h
        db  00Fh, 0B7h, 005h, 044h, 0B9h, 000h, 000h
        mov    word ptr [ecx + eax*4], si
        db  066h, 0A1h, 050h, 0B9h, 000h, 000h
        db  066h, 039h, 005h, 044h, 0B9h, 000h, 000h
        jne    SHORT loc_5_7F95
        db  066h, 089h, 035h, 050h, 0B9h, 000h, 000h
        jmp    SHORT loc_5_7FA3
loc_5_7F95:
        db  08Bh, 00Dh, 058h, 0B9h, 000h, 000h
        movzx  eax, word ptr [edi]
        mov    word ptr [ecx + eax*4 + 2], si
loc_5_7FA3:
        db  066h, 089h, 035h, 044h, 0B9h, 000h, 000h
loc_5_7FAA:
        pop    edi
        pop    esi
        ret
sub_5_7FAD:
        push   esi
        mov    eax, dword ptr [esp + 8]
        db  0FFh, 035h, 044h, 0B0h, 000h, 000h
        mov    si, word ptr [eax + 1ah]
        db  0E8h, 03Ah, 013h, 0FBh, 0FFh
        add    esp, 4
        push   esi
        call   sub_5_7E7C
        add    esp, 4
        db  0FFh, 035h, 044h, 0B0h, 000h, 000h
        db  0E8h, 019h, 013h, 0FBh, 0FFh
        add    esp, 4
        pop    esi
        ret
sub_5_7FDD:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0FFh, 035h, 044h, 0B0h, 000h, 000h
        db  0E8h, 00Fh, 013h, 0FBh, 0FFh
        db  0A1h, 094h, 0B8h, 000h, 000h
        add    esp, 4
        mov    edi, dword ptr [eax]
        mov    ebx, dword ptr [edi + 8]
        add    edi, 8
        cmp    ebx, edi
        je     SHORT loc_5_8042
        mov    esi, dword ptr [esp + 14h]
loc_5_8004:
        mov    ebp, ebx
        mov    ebx, dword ptr [ebx]
        or     esi, esi
        jne    SHORT loc_5_8020
        db  0A1h, 094h, 0B8h, 000h, 000h
        movzx  ecx, byte ptr [ebp + 14h]
        movzx  eax, word ptr [eax + 491h]
        cmp    ecx, eax
        jne    SHORT loc_5_803E
loc_5_8020:
        mov    ax, word ptr [ebp + 1ah]
        push   eax
        call   sub_5_7BD2
        add    esp, 4
        mov    byte ptr [ebp + 15h], 0
        mov    ax, word ptr [ebp + 1ah]
        push   eax
        call   sub_5_7E7C
        add    esp, 4
loc_5_803E:
        cmp    ebx, edi
        jne    SHORT loc_5_8004
loc_5_8042:
        db  0FFh, 035h, 044h, 0B0h, 000h, 000h
        db  0E8h, 0A4h, 012h, 0FBh, 0FFh
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_8055:
        push   0
        push   1004h
        db  0E8h, 003h, 0C3h, 0FAh, 0FFh
        add    esp, 8
        db  0A3h, 058h, 0B9h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_5_806F
        jmp    SHORT loc_5_80CA
loc_5_806F:
        push   1
        push   1004h
        db  0E8h, 0E9h, 0C2h, 0FAh, 0FFh
        add    esp, 8
        db  0A3h, 048h, 0B9h, 000h, 000h
        or     eax, eax
        je     SHORT loc_5_80B2
        push   1
        db  0E8h, 04Ah, 012h, 0FBh, 0FFh
        add    esp, 4
        db  0A3h, 044h, 0B0h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_5_80CD
        db  0FFh, 035h, 048h, 0B9h, 000h, 000h
        db  0E8h, 0EBh, 0C2h, 0FAh, 0FFh
        add    esp, 4
        db  0C7h, 005h, 048h, 0B9h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_80B2:
        db  0FFh, 035h, 058h, 0B9h, 000h, 000h
        db  0E8h, 0D3h, 0C2h, 0FAh, 0FFh
        add    esp, 4
        db  0C7h, 005h, 058h, 0B9h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_80CA:
        sub    eax, eax
loc_5_80CC:
        ret
loc_5_80CD:
        mov    edx, 1
loc_5_80D2:
        db  08Bh, 00Dh, 058h, 0B9h, 000h, 000h
        lea    eax, [edx + 1]
        mov    word ptr [ecx + edx*4], ax
        db  08Bh, 00Dh, 058h, 0B9h, 000h, 000h
        lea    eax, [edx - 1]
        mov    word ptr [ecx + edx*4 + 2], ax
        inc    edx
        cmp    edx, 400h
        jbe    SHORT loc_5_80D2
        db  0A1h, 058h, 0B9h, 000h, 000h
        mov    word ptr [eax + 6], 0
        db  0A1h, 058h, 0B9h, 000h, 000h
        mov    word ptr [eax + 1000h], 0
        mov    eax, 1
        db  066h, 0C7h, 005h, 052h, 0B9h, 000h, 000h, 001h, 000h
        db  066h, 0C7h, 005h, 050h, 0B9h, 000h, 000h, 000h, 004h
        db  066h, 0C7h, 005h, 044h, 0B9h, 000h, 000h, 000h, 004h
        db  0C7h, 005h, 05Ch, 0B9h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_5_80CC
sub_5_813B:
        db  0A1h, 048h, 0B9h, 000h, 000h
        or     eax, eax
        je     SHORT loc_5_8157
        push   eax
        db  0E8h, 046h, 0C2h, 0FAh, 0FFh
        add    esp, 4
        db  0C7h, 005h, 048h, 0B9h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_8157:
        db  0A1h, 058h, 0B9h, 000h, 000h
        or     eax, eax
        je     SHORT loc_5_8173
        push   eax
        db  0E8h, 02Ah, 0C2h, 0FAh, 0FFh
        add    esp, 4
        db  0C7h, 005h, 058h, 0B9h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_8173:
        db  0A1h, 044h, 0B0h, 000h, 000h
        or     eax, eax
        je     SHORT loc_5_818F
        push   eax
        db  0E8h, 065h, 011h, 0FBh, 0FFh
        add    esp, 4
        db  0C7h, 005h, 044h, 0B0h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_818F:
        ret
sub_5_8190:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    bl, byte ptr [esp + 18h]
        movzx  ebp, bl
        push   200h
        lea    eax, [ebp + 6]
        push   eax
        db  0E8h, 0BBh, 0C1h, 0FAh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_81B4
        sub    eax, eax
        jmp    SHORT loc_5_81D7
loc_5_81B4:
        mov    dword ptr [eax], 1
        mov    byte ptr [eax + 4], bl
        lea    edi, [eax + 5]
        mov    esi, dword ptr [esp + 14h]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    byte ptr [ebp + eax + 5], 0
loc_5_81D7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_81DC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        db  0E8h, 06Dh, 0B8h, 0FAh, 0FFh
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_5_81F7
        push   esi
        db  0E8h, 09Ch, 0C1h, 0FAh, 0FFh
        add    esp, 4
loc_5_81F7:
        pop    esi
        ret
sub_5_81F9:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esi + 8]
        dec    word ptr [eax + 60h]
        push   esi
        db  0E8h, 050h, 07Eh, 0FAh, 0FFh
        add    esp, 4
        push   esi
        db  0E8h, 077h, 0C1h, 0FAh, 0FFh
        add    esp, 4
        db  0FFh, 00Dh, 094h, 0B0h, 000h, 000h
        push   edi
        call   sub_5_81DC
        add    esp, 4
        pop    edi
        pop    esi
        ret
sub_5_822E:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        cmp    word ptr [ebp + 60h], 0
        jne    SHORT loc_5_8241
        sub    eax, eax
        jmp    SHORT loc_5_82BD
loc_5_8241:
        db  0FFh, 035h, 090h, 0B0h, 000h, 000h
        db  0E8h, 0AFh, 010h, 0FBh, 0FFh
        add    esp, 4
        mov    edx, dword ptr [ebp + 58h]
        mov    eax, edx
        sub    eax, ebp
        cmp    eax, 58h
        jne    SHORT loc_5_826D
        db  0FFh, 035h, 090h, 0B0h, 000h, 000h
        db  0E8h, 08Bh, 010h, 0FBh, 0FFh
        add    esp, 4
        sub    eax, eax
        jmp    SHORT loc_5_82BD
loc_5_826D:
        mov    esi, dword ptr [edx + 8]
        mov    eax, dword ptr [esp + 1ch]
        movzx  ebx, byte ptr [esi + 4]
        cmp    ebx, eax
        jbe    SHORT loc_5_827E
        mov    ebx, eax
loc_5_827E:
        or     ebx, ebx
        je     SHORT loc_5_8297
        add    esi, 5
        mov    edi, dword ptr [esp + 18h]
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
loc_5_8297:
        push   edx
        push   ebp
        call   sub_5_81F9
        add    esp, 8
        db  0FFh, 035h, 090h, 0B0h, 000h, 000h
        db  0E8h, 045h, 010h, 0FBh, 0FFh
        mov    eax, dword ptr [esp + 24h]
        add    esp, 4
        or     eax, eax
        je     SHORT loc_5_82B9
        mov    dword ptr [eax], ebx
loc_5_82B9:
        mov    eax, dword ptr [esp + 18h]
loc_5_82BD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_82C2:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    word ptr [esi + 60h], 0
        je     SHORT loc_5_82E4
loc_5_82CE:
        push   0
        push   0
        push   0
        push   esi
        call   sub_5_822E
        add    esp, 10h
        cmp    word ptr [esi + 60h], 0
        jne    SHORT loc_5_82CE
loc_5_82E4:
        pop    esi
        ret
sub_5_82E6:
        push   ebx
        push   esi
        push   edi
        push   200h
        push   0ch
        db  0E8h, 06Fh, 0C0h, 0FAh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_5_8302
        sub    eax, eax
        jmp    SHORT loc_5_835A
loc_5_8302:
        mov    edi, dword ptr [esp + 14h]
        mov    dword ptr [ebx + 8], edi
        db  0FFh, 035h, 090h, 0B0h, 000h, 000h
        db  0E8h, 0E7h, 00Fh, 0FBh, 0FFh
        mov    esi, dword ptr [esp + 14h]
        add    esp, 4
        cmp    word ptr [esi + 60h], 5
        jb     SHORT loc_5_832E
        push   dword ptr [esi + 58h]
        push   esi
        call   sub_5_81F9
        add    esp, 8
loc_5_832E:
        lea    eax, [esi + 58h]
        push   eax
        push   ebx
        db  0E8h, 00Fh, 07Dh, 0FAh, 0FFh
        add    esp, 8
        inc    dword ptr [edi]
        db  0FFh, 005h, 094h, 0B0h, 000h, 000h
        inc    word ptr [esi + 60h]
        db  0FFh, 035h, 090h, 0B0h, 000h, 000h
        db  0E8h, 09Fh, 00Fh, 0FBh, 0FFh
        add    esp, 4
        mov    eax, 1
loc_5_835A:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_835E:
        mov    eax, dword ptr [esp + 4]
        test   byte ptr [eax + 1ah], 20h
        je     SHORT loc_5_836E
        mov    ax, 0ffh
        jmp    SHORT loc_5_8393
loc_5_836E:
        cmp    word ptr [eax + 60h], 5
        jb     SHORT loc_5_837B
        mov    ax, 0fch
        jmp    SHORT loc_5_8393
loc_5_837B:
        push   dword ptr [esp + 8]
        push   eax
        call   sub_5_82E6
        add    esp, 8
        or     eax, eax
        mov    ax, 96h
        je     SHORT loc_5_8393
        sub    ax, ax
loc_5_8393:
        ret
sub_5_8394:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 34h], 0
        mov    ecx, dword ptr [esp + 24h]
        je     SHORT loc_5_83B9
        movzx  eax, word ptr [ecx + 0ah]
        mov    dword ptr [esp + 18h], eax
        lea    eax, [ecx + eax*4 + 0ch]
        mov    byte ptr [esp + 13h], 2
        jmp    SHORT loc_5_83CA
loc_5_83B9:
        movzx  eax, byte ptr [ecx + 0ah]
        mov    dword ptr [esp + 18h], eax
        lea    eax, [ecx + eax + 0bh]
        mov    byte ptr [esp + 13h], 0fdh
loc_5_83CA:
        mov    cl, byte ptr [eax]
        push   ecx
        inc    eax
        push   eax
        call   sub_5_8190
        mov    dword ptr [esp + 1ch], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_5_83E8
        mov    si, 96h
        jmp    NEAR PTR loc_5_8560
loc_5_83E8:
        mov    eax, dword ptr [esp + 30h]
        push   dword ptr [eax + 10h]
        db  0E8h, 0FDh, 00Eh, 0FBh, 0FFh
        add    esp, 4
        cmp    dword ptr [esp + 18h], 0
        jne    SHORT loc_5_846C
        db  08Bh, 03Dh, 050h, 0B4h, 000h, 000h
        db  081h, 0FFh, 050h, 0B4h, 000h, 000h
        je     SHORT loc_5_843B
loc_5_840C:
        mov    esi, edi
        mov    edi, dword ptr [edi]
        push   dword ptr [esi + 10h]
        db  0E8h, 0E3h, 00Eh, 0FBh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 14h]
        push   esi
        call   sub_5_835E
        add    esp, 8
        push   dword ptr [esi + 10h]
        db  0E8h, 0C1h, 00Eh, 0FBh, 0FFh
        add    esp, 4
        db  081h, 0FFh, 050h, 0B4h, 000h, 000h
        jne    SHORT loc_5_840C
loc_5_843B:
        cmp    dword ptr [esp + 34h], 0
        mov    edx, dword ptr [esp + 28h]
        je     SHORT loc_5_845A
        mov    eax, dword ptr [esp + 2ch]
        mov    word ptr [edx + 8], 0
        mov    word ptr [eax], 2
        jmp    NEAR PTR loc_5_854E
loc_5_845A:
        mov    eax, dword ptr [esp + 2ch]
        mov    byte ptr [edx + 8], 0
        mov    word ptr [eax], 1
        jmp    NEAR PTR loc_5_854E
loc_5_846C:
        sub    edi, edi
        cmp    dword ptr [esp + 18h], edi
        je     NEAR PTR loc_5_8518
        mov    eax, dword ptr [esp + 28h]
        add    eax, 0ah
        mov    dword ptr [esp + 1ch], eax
        mov    esi, dword ptr [esp + 24h]
        add    esi, 0ch
loc_5_848A:
        cmp    dword ptr [esp + 34h], 0
        je     SHORT loc_5_8495
        mov    eax, dword ptr [esi]
        jmp    SHORT loc_5_849E
loc_5_8495:
        mov    ecx, dword ptr [esp + 24h]
        movzx  eax, byte ptr [edi + ecx + 0bh]
loc_5_849E:
        db  03Bh, 005h, 024h, 0B4h, 000h, 000h
        ja     SHORT loc_5_84E5
        or     eax, eax
        je     SHORT loc_5_84E5
        db  08Bh, 00Dh, 04Ch, 0B4h, 000h, 000h
        mov    ebp, dword ptr [ecx + eax*4]
        db  039h, 02Dh, 024h, 0B4h, 000h, 000h
        jb     SHORT loc_5_84BD
        jmp    SHORT loc_5_84E5
loc_5_84BD:
        push   dword ptr [ebp + 10h]
        db  0E8h, 036h, 00Eh, 0FBh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 14h]
        push   ebp
        call   sub_5_835E
        add    esp, 8
        push   dword ptr [ebp + 10h]
        mov    bx, ax
        db  0E8h, 011h, 00Eh, 0FBh, 0FFh
        add    esp, 4
        jmp    SHORT loc_5_84EB
loc_5_84E5:
        movzx  bx, byte ptr [esp + 13h]
loc_5_84EB:
        cmp    dword ptr [esp + 34h], 0
        je     SHORT loc_5_84FD
        mov    eax, dword ptr [esp + 1ch]
        movzx  ecx, bx
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_5_8505
loc_5_84FD:
        mov    edx, dword ptr [esp + 28h]
        mov    byte ptr [edi + edx + 9], bl
loc_5_8505:
        add    dword ptr [esp + 1ch], 4
        add    esi, 4
        inc    edi
        cmp    edi, dword ptr [esp + 18h]
        jb     NEAR PTR loc_5_848A
loc_5_8518:
        cmp    dword ptr [esp + 34h], 0
        je     SHORT loc_5_8539
        mov    edx, dword ptr [esp + 28h]
        mov    ax, word ptr [esp + 18h]
        mov    word ptr [edx + 8], ax
        lea    ecx, [eax*4]
        add    cx, 2
        jmp    SHORT loc_5_8547
loc_5_8539:
        mov    edx, dword ptr [esp + 28h]
        mov    cx, word ptr [esp + 18h]
        mov    byte ptr [edx + 8], cl
        inc    cx
loc_5_8547:
        mov    eax, dword ptr [esp + 2ch]
        mov    word ptr [eax], cx
loc_5_854E:
        mov    eax, dword ptr [esp + 30h]
        sub    si, si
        push   dword ptr [eax + 10h]
        db  0E8h, 09Eh, 00Dh, 0FBh, 0FFh
        add    esp, 4
loc_5_8560:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_5_8573
        push   dword ptr [esp + 14h]
        call   sub_5_81DC
        add    esp, 4
loc_5_8573:
        or     si, si
        jne    SHORT loc_5_857D
        db  0E8h, 06Dh, 012h, 0FBh, 0FFh
loc_5_857D:
        mov    ax, si
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_8588:
        push   0
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        call   sub_5_8394
        add    esp, 14h
        ret
loc_5_85A3:
        push   1
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        call   sub_5_8394
        add    esp, 14h
        ret
loc_5_85BE:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [esp + 14h], 0
        cmp    byte ptr [eax + 6ah], 3
        je     SHORT loc_5_85E6
        test   byte ptr [eax + 1ah], 10h
        jne    SHORT loc_5_85E6
        mov    ax, 0c6h
        jmp    NEAR PTR loc_5_870B
loc_5_85E6:
        mov    eax, dword ptr [esp + 1ch]
        movzx  ecx, byte ptr [eax + 0ah]
        mov    dword ptr [esp + 10h], ecx
        lea    eax, [ecx + eax + 0bh]
        mov    cl, byte ptr [eax]
        push   ecx
        inc    eax
        push   eax
        call   sub_5_8190
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_5_8612
        mov    di, 96h
        jmp    NEAR PTR loc_5_86F1
loc_5_8612:
        mov    eax, dword ptr [esp + 28h]
        push   dword ptr [eax + 10h]
        db  0E8h, 0D3h, 00Ch, 0FBh, 0FFh
        add    esp, 4
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_5_8668
        db  08Bh, 01Dh, 050h, 0B4h, 000h, 000h
        db  081h, 0FBh, 050h, 0B4h, 000h, 000h
        je     NEAR PTR loc_5_86DF
loc_5_863A:
        mov    edi, ebx
        mov    ebx, dword ptr [ebx]
        push   dword ptr [edi + 10h]
        db  0E8h, 0B5h, 00Ch, 0FBh, 0FFh
        add    esp, 4
        push   esi
        push   edi
        call   sub_5_835E
        add    esp, 8
        push   dword ptr [edi + 10h]
        db  0E8h, 096h, 00Ch, 0FBh, 0FFh
        add    esp, 4
        db  081h, 0FBh, 050h, 0B4h, 000h, 000h
        jne    SHORT loc_5_863A
        jmp    SHORT loc_5_86DF
loc_5_8668:
        sub    edi, edi
        cmp    dword ptr [esp + 10h], edi
        je     SHORT loc_5_86D1
loc_5_8670:
        mov    eax, dword ptr [esp + 1ch]
        movzx  eax, byte ptr [edi + eax + 0bh]
        db  039h, 005h, 024h, 0B4h, 000h, 000h
        jb     SHORT loc_5_86BD
        or     eax, eax
        je     SHORT loc_5_86BD
        db  08Bh, 00Dh, 04Ch, 0B4h, 000h, 000h
        mov    ebp, dword ptr [ecx + eax*4]
        db  039h, 02Dh, 024h, 0B4h, 000h, 000h
        jb     SHORT loc_5_8698
        jmp    SHORT loc_5_86BD
loc_5_8698:
        push   dword ptr [ebp + 10h]
        db  0E8h, 05Bh, 00Ch, 0FBh, 0FFh
        add    esp, 4
        push   esi
        push   ebp
        call   sub_5_835E
        add    esp, 8
        push   dword ptr [ebp + 10h]
        mov    bx, ax
        db  0E8h, 039h, 00Ch, 0FBh, 0FFh
        add    esp, 4
        jmp    SHORT loc_5_86C1
loc_5_86BD:
        mov    bx, 0fdh
loc_5_86C1:
        or     bx, bx
        je     SHORT loc_5_86CA
        inc    dword ptr [esp + 14h]
loc_5_86CA:
        inc    edi
        cmp    edi, dword ptr [esp + 10h]
        jb     SHORT loc_5_8670
loc_5_86D1:
        mov    ecx, dword ptr [esp + 10h]
        cmp    dword ptr [esp + 14h], ecx
        mov    di, 0fdh
        jae    SHORT loc_5_86E2
loc_5_86DF:
        sub    di, di
loc_5_86E2:
        mov    eax, dword ptr [esp + 28h]
        push   dword ptr [eax + 10h]
        db  0E8h, 00Dh, 00Ch, 0FBh, 0FFh
        add    esp, 4
loc_5_86F1:
        or     esi, esi
        je     SHORT loc_5_86FE
        push   esi
        call   sub_5_81DC
        add    esp, 4
loc_5_86FE:
        or     di, di
        jne    SHORT loc_5_8708
        db  0E8h, 0E2h, 010h, 0FBh, 0FFh
loc_5_8708:
        mov    ax, di
loc_5_870B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_5_8713:
        sub    esp, 4
        push   esi
        lea    eax, [esp + 4]
        push   eax
        mov    esi, dword ptr [esp + 14h]
        push   37h
        lea    eax, [esi + 9]
        push   eax
        push   dword ptr [esp + 24h]
        call   sub_5_822E
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_5_873E
        mov    dword ptr [esp + 4], 0
loc_5_873E:
        mov    al, byte ptr [esp + 4]
        mov    byte ptr [esi + 8], al
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 14h]
        inc    cx
        mov    word ptr [eax], cx
        sub    ax, ax
        pop    esi
        add    esp, 4
        ret
loc_5_875A:
        mov    eax, dword ptr [esp + 10h]
        or     word ptr [eax + 1ah], 20h
        sub    ax, ax
        ret
loc_5_8767:
        mov    eax, dword ptr [esp + 10h]
        and    word ptr [eax + 1ah], 0ffdfh
        sub    ax, ax
        ret
sub_5_8775:
        push   1
        db  0E8h, 05Ch, 00Bh, 0FBh, 0FFh
        add    esp, 4
        db  0A3h, 090h, 0B0h, 000h, 000h
        or     eax, eax
        mov    eax, 1
        jne    SHORT loc_5_878F
        sub    eax, eax
loc_5_878F:
        ret
sub_5_8790:
        db  0FFh, 035h, 090h, 0B0h, 000h, 000h
        db  0E8h, 04Ch, 00Bh, 0FBh, 0FFh
        add    esp, 4
        db  0C7h, 005h, 090h, 0B0h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_87B0:
        mov    eax, 0fbh
        ret
loc_5_87B6:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        mov    word ptr [ebx], 0
        db  0E8h, 050h, 0BCh, 0FAh, 0FFh
        mov    dword ptr [ebx + 2], eax
        db  0CDh, 020h, 000h, 000h, 001h, 000h
        mov    word ptr [ebp - 2], ax
        mov    al, byte ptr [ebp - 2]
        mov    cx, word ptr [ebp - 2]
        mov    byte ptr [ebx + 6], ch
        mov    byte ptr [ebx + 7], al
        mov    byte ptr [ebx + 8], 0
        db  083h, 03Dh, 0E0h, 0B3h, 000h, 000h, 000h
        je     SHORT loc_5_8838
        mov    dword ptr [ebp - 1ch], 0
        lea    eax, [ebp - 24h]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 005h, 000h, 093h, 004h
        add    esp, 4
        cmp    dword ptr [ebp - 1ch], 0
        je     SHORT loc_5_8838
        mov    eax, dword ptr [ebp - 20h]
        dec    al
        mov    byte ptr [ebx + 8], al
        mov    eax, dword ptr [ebp - 20h]
        dec    eax
        lea    edi, [ebx + 9]
        mov    esi, dword ptr [ebp - 1ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_5_8838:
        movzx  eax, byte ptr [ebx + 8]
        lea    edx, [ebx + eax + 9]
        db  0BFh, 0BCh, 0B1h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    byte ptr [edx], cl
        movzx  eax, cl
        db  0BEh, 0BCh, 0B1h, 000h, 000h
        lea    edi, [edx + 1]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        movzx  eax, byte ptr [edx]
        lea    esi, [edx + eax + 1]
        push   esi
        db  0E8h, 0C3h, 0FAh, 0FAh, 0FFh
        add    esp, 4
        movzx  ecx, byte ptr [esi]
        sub    ecx, ebx
        add    ecx, esi
        mov    eax, dword ptr [ebp + 10h]
        inc    ecx
        mov    dword ptr [eax], ecx
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_8892:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   200h
        push   64h
        db  0E8h, 0BFh, 0BAh, 0FAh, 0FFh
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_5_88B7
        mov    ax, 96h
        jmp    NEAR PTR loc_5_89A5
loc_5_88B7:
        mov    byte ptr [esi], 1
        lea    ebx, [esi + 2]
        mov    byte ptr [esi + 1], 40h
        mov    word ptr [ebx], 3b06h
        db  068h, 0A4h, 0B0h, 000h, 000h
        lea    eax, [esi + 4]
        push   eax
        call   sub_5_9B16
        mov    edi, dword ptr [esp + 20h]
        add    esp, 8
        push   edi
        push   esi
        call   sub_5_18BC
        add    esp, 8
        mov    bp, ax
        or     bp, bp
        jne    NEAR PTR loc_5_8999
        mov    byte ptr [esi], 0
        mov    byte ptr [esi + 1], 40h
        db  068h, 0B8h, 0B0h, 000h, 000h
        push   ebx
        call   sub_5_9B16
        lea    eax, [esp + 18h]
        add    esp, 8
        push   eax
        push   esi
        push   dword ptr [edi]
        call   sub_5_2583
        add    esp, 0ch
        mov    bp, ax
        or     bp, bp
        jne    SHORT loc_5_8999
        push   0ch
        db  068h, 098h, 0B0h, 000h, 000h
        push   0ffh
        push   1
        push   dword ptr [esp + 20h]
        call   sub_5_2CCA
        add    esp, 14h
        mov    bp, ax
        or     bp, bp
        jne    SHORT loc_5_8999
        mov    byte ptr [esi], 0
        mov    byte ptr [esi + 1], 40h
        db  068h, 0C8h, 0B0h, 000h, 000h
        push   ebx
        call   sub_5_9B16
        lea    eax, [esp + 18h]
        add    esp, 8
        push   eax
        push   esi
        push   dword ptr [edi]
        call   sub_5_2583
        add    esp, 0ch
        mov    bp, ax
        or     bp, bp
        jne    SHORT loc_5_8999
        call   sub_5_14E9
        mov    edx, eax
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        push   edx
        push   0ffh
        push   1
        push   dword ptr [esp + 20h]
        call   sub_5_2CCA
        add    esp, 14h
        mov    bp, ax
loc_5_8999:
        push   esi
        db  0E8h, 0F1h, 0B9h, 0FAh, 0FFh
        add    esp, 4
        mov    ax, bp
loc_5_89A5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_5_89AD:
        sub    esp, 4
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 00Eh, 00Bh, 0FBh, 0FFh
        add    esp, 4
        db  068h, 0E6h, 019h, 000h, 000h
        db  068h, 0A0h, 0B0h, 000h, 000h
        push   0
        db  0E8h, 044h, 0A7h, 0FAh, 0FFh
        lea    eax, [esp + 0ch]
        add    esp, 0ch
        push   eax
        call   sub_5_8892
        add    esp, 4
        db  068h, 028h, 09Bh, 000h, 000h
        db  0E8h, 024h, 00Bh, 0FBh, 0FFh
        add    esp, 4
        mov    eax, 1
        add    esp, 4
        ret
sub_5_89F4:
        db  00Fh, 0B7h, 005h, 0A0h, 0B0h, 000h, 000h
        push   eax
        db  0E8h, 059h, 0A7h, 0FAh, 0FFh
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_5_8A10:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 20h]
        test   esi, 2
        mov    ax, 6
        jne    SHORT loc_5_8A2B
        mov    ax, 4
loc_5_8A2B:
        push   0feh
        lea    ecx, [esp + 1ch]
        push   0
        push   ecx
        mov    edi, dword ptr [esp + 30h]
        push   eax
        lea    eax, [edi + 9]
        push   eax
        push   1
        movzx  eax, byte ptr [edi + 7]
        lea    ecx, [esp + 2ch]
        push   eax
        sub    edx, edx
        call   sub_5_62A6
        mov    word ptr [esp + 10h], ax
        or     ax, ax
        je     SHORT loc_5_8A61
        jmp    NEAR PTR loc_5_8BDA
loc_5_8A61:
        db  0E8h, 00Bh, 0A9h, 0FAh, 0FFh
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_5_8A78
        mov    word ptr [esp + 10h], 96h
        jmp    NEAR PTR loc_5_8BBD
loc_5_8A78:
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 16h]
        push   eax
        push   esi
        shr    esi, 10h
        push   esi
        movzx  ax, byte ptr [edi + 8]
        or     ax, 1f00h
        push   eax
        push   dword ptr [esp + 28h]
        db  0E8h, 006h, 0A0h, 0FAh, 0FFh
        add    esp, 18h
        mov    si, ax
        or     si, si
        jne    SHORT loc_5_8ABE
        sub    edx, edx
        mov    ecx, dword ptr [esp + 10h]
        db  0E8h, 050h, 09Fh, 0FAh, 0FFh
        movzx  ax, al
        mov    word ptr [esp + 10h], ax
        jmp    NEAR PTR loc_5_8BB4
loc_5_8ABE:
        mov    word ptr [esp + 10h], 0
        push   16h
        push   ebx
        push   dword ptr [esp + 1ch]
        db  0E8h, 013h, 0E5h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_8AE4
        mov    word ptr [esp + 10h], 96h
        jmp    NEAR PTR loc_5_8BB4
loc_5_8AE4:
        push   eax
        db  0E8h, 022h, 0E6h, 0FAh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   esi
        push   dword ptr [esp + 3ch]
        db  0E8h, 0E1h, 0DCh, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_5_8B1B
        push   esi
        db  0E8h, 0F1h, 0A0h, 0FAh, 0FFh
        mov    word ptr [esp + 14h], 96h
        add    esp, 4
        jmp    NEAR PTR loc_5_8BB4
loc_5_8B1B:
        mov    ebp, dword ptr [esp + 28h]
        mov    dword ptr [ebp + 0ah], eax
        mov    word ptr [ebp + 8], 0
        lea    edi, [ebp + 0eh]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        cmp    byte ptr [ebx + 234h], al
        lea    esi, [ebx + 234h]
        je     SHORT loc_5_8B41
        jmp    SHORT loc_5_8B44
loc_5_8B41:
        lea    esi, [ebx + 2ch]
loc_5_8B44:
        lea    edi, [ebp + 10h]
        movsd
        movsd
        movsd
        movsw
        mov    al, byte ptr [ebx]
        mov    byte ptr [ebp + 1eh], al
        mov    byte ptr [ebp + 1fh], 0
        push   dword ptr [ebx + 20h]
        db  0E8h, 00Bh, 0AFh, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp + 20h], eax
        mov    eax, dword ptr [ebx + 4]
        shr    eax, 10h
        push   eax
        db  0E8h, 0F1h, 0AEh, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [ebp + 24h], ax
        mov    eax, dword ptr [ebx + 0ch]
        shr    eax, 10h
        push   eax
        db  0E8h, 0DEh, 0AEh, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [ebp + 26h], ax
        mov    eax, dword ptr [ebx + 14h]
        shr    eax, 10h
        push   eax
        db  0E8h, 0CBh, 0AEh, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [ebp + 28h], ax
        push   dword ptr [ebx + 14h]
        db  0E8h, 0BCh, 0AEh, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [ebp + 2ah], ax
        mov    eax, dword ptr [esp + 2ch]
        mov    word ptr [eax], 24h
loc_5_8BB4:
        push   ebx
        db  0E8h, 01Ch, 0A8h, 0FAh, 0FFh
        add    esp, 4
loc_5_8BBD:
        push   dword ptr [esp + 14h]
        db  0E8h, 093h, 0A7h, 0FAh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 18h]
        call   sub_5_3340
        mov    ax, word ptr [esp + 14h]
        add    esp, 4
loc_5_8BDA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_5_8BE2:
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   20010h
        call   sub_5_8A10
        add    esp, 14h
        ret
loc_5_8C00:
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   dword ptr [esp + 10h]
        push   20012h
        call   sub_5_8A10
        add    esp, 14h
        ret
sub_5_8C1E:
        mov    eax, dword ptr [esp + 8]
        push   1
        inc    eax
        push   eax
        push   dword ptr [esp + 0ch]
        call   sub_5_6192
        mov    dx, word ptr [esp + 18h]
        add    esp, 0ch
        mov    cx, dx
        and    cx, ax
        cmp    cx, dx
        je     SHORT loc_5_8C78
        mov    cx, ax
        not    cx
        and    cx, dx
        cmp    dx, 3
        jne    SHORT loc_5_8C87
        test   al, 3
        je     SHORT loc_5_8C87
        mov    ecx, dword ptr [esp + 10h]
        mov    dx, word ptr [ecx]
        and    dx, 0fffdh
        mov    word ptr [ecx], dx
        test   al, 2
        je     SHORT loc_5_8C7C
        or     dx, 1
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [ecx], dx
        mov    word ptr [eax], 2
loc_5_8C78:
        sub    ax, ax
loc_5_8C7B:
        ret
loc_5_8C7C:
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [eax], 1
        jmp    SHORT loc_5_8C78
loc_5_8C87:
        push   ecx
        call   sub_5_6160
        add    esp, 4
        jmp    SHORT loc_5_8C7B
loc_5_8C92:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 12h], 0
        mov    esi, dword ptr [esp + 24h]
        mov    word ptr [esp + 14h], 0
        movzx  cx, byte ptr [esi + 9]
        test   cl, 10h
        jne    SHORT loc_5_8CCE
        mov    ax, cx
        and    ax, 0ch
        shr    ax, 1
        movzx  eax, ax
        db  066h, 08Bh, 080h, 04Ch, 0B0h, 000h, 000h
        mov    word ptr [esp + 12h], ax
loc_5_8CCE:
        and    ecx, 3
        lea    edi, [ecx*2]
        push   0feh
        db  066h, 08Bh, 087h, 054h, 0B0h, 000h, 000h
        or     word ptr [esp + 16h], ax
        lea    eax, [esp + 20h]
        push   0
        push   eax
        lea    eax, [esi + 0ah]
        push   0
        push   eax
        push   1
        movzx  eax, byte ptr [esi + 7]
        sub    edx, edx
        lea    ecx, [esp + 30h]
        push   eax
        call   sub_5_62A6
        mov    word ptr [esp + 10h], ax
        or     ax, ax
        je     SHORT loc_5_8D17
        jmp    NEAR PTR loc_5_8ECE
loc_5_8D17:
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 16h]
        push   eax
        db  066h, 08Bh, 087h, 05Ch, 0B0h, 000h, 000h
        push   eax
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 2ch]
        call   sub_5_8C1E
        mov    word ptr [esp + 24h], ax
        add    esp, 14h
        or     ax, ax
        jne    NEAR PTR loc_5_8EB1
        db  0E8h, 025h, 0A6h, 0FAh, 0FFh
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_5_8D5E
        mov    word ptr [esp + 10h], 96h
        jmp    NEAR PTR loc_5_8EB1
loc_5_8D5E:
        lea    eax, [esp + 10h]
        push   eax
        movzx  cx, byte ptr [esi + 8]
        lea    eax, [esp + 1ah]
        push   eax
        push   1
        shl    cx, 8
        push   dword ptr [esp + 1eh]
        movzx  ax, byte ptr [esi + 9]
        or     cx, ax
        push   ecx
        push   dword ptr [esp + 2ch]
        db  0E8h, 017h, 09Dh, 0FAh, 0FFh
        add    esp, 18h
        mov    si, ax
        or     si, si
        jne    SHORT loc_5_8DAD
        sub    edx, edx
        mov    ecx, dword ptr [esp + 10h]
        db  0E8h, 061h, 09Ch, 0FAh, 0FFh
        movzx  ax, al
        mov    word ptr [esp + 10h], ax
        jmp    NEAR PTR loc_5_8EA8
loc_5_8DAD:
        mov    word ptr [esp + 10h], 0
        push   16h
        push   ebx
        push   dword ptr [esp + 20h]
        db  0E8h, 024h, 0E2h, 0FAh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_5_8DD3
        mov    word ptr [esp + 10h], 96h
        jmp    NEAR PTR loc_5_8EA8
loc_5_8DD3:
        push   eax
        db  0E8h, 033h, 0E3h, 0FAh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 1ch]
        push   dword ptr [esp + 1ch]
        push   esi
        push   dword ptr [esp + 3ch]
        db  0E8h, 0F2h, 0D9h, 0FAh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_5_8E0A
        push   esi
        db  0E8h, 002h, 09Eh, 0FAh, 0FFh
        mov    word ptr [esp + 14h], 96h
        add    esp, 4
        jmp    NEAR PTR loc_5_8EA8
loc_5_8E0A:
        mov    ebp, dword ptr [esp + 28h]
        mov    dword ptr [ebp + 0ah], eax
        mov    word ptr [ebp + 8], 0
        lea    edi, [ebp + 0eh]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        cmp    byte ptr [ebx + 234h], al
        lea    esi, [ebx + 234h]
        je     SHORT loc_5_8E30
        jmp    SHORT loc_5_8E33
loc_5_8E30:
        lea    esi, [ebx + 2ch]
loc_5_8E33:
        lea    edi, [ebp + 10h]
        movsd
        movsd
        movsd
        movsw
        mov    ax, word ptr [ebx]
        or     al, byte ptr [esp + 14h]
        mov    byte ptr [ebp + 1eh], al
        mov    byte ptr [ebp + 1fh], 0
        push   dword ptr [ebx + 20h]
        db  0E8h, 017h, 0ACh, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [ebp + 20h], eax
        mov    eax, dword ptr [ebx + 4]
        shr    eax, 10h
        push   eax
        db  0E8h, 0FDh, 0ABh, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [ebp + 24h], ax
        mov    eax, dword ptr [ebx + 0ch]
        shr    eax, 10h
        push   eax
        db  0E8h, 0EAh, 0ABh, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [ebp + 26h], ax
        mov    eax, dword ptr [ebx + 14h]
        shr    eax, 10h
        push   eax
        db  0E8h, 0D7h, 0ABh, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [ebp + 28h], ax
        push   dword ptr [ebx + 14h]
        db  0E8h, 0C8h, 0ABh, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [ebp + 2ah], ax
        mov    eax, dword ptr [esp + 2ch]
        mov    word ptr [eax], 24h
loc_5_8EA8:
        push   ebx
        db  0E8h, 028h, 0A5h, 0FAh, 0FFh
        add    esp, 4
loc_5_8EB1:
        push   dword ptr [esp + 18h]
        db  0E8h, 09Fh, 0A4h, 0FAh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 1ch]
        call   sub_5_3340
        mov    ax, word ptr [esp + 14h]
        add    esp, 4
loc_5_8ECE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
loc_5_8ED6:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 10h]
        sub    bx, bx
        mov    esi, dword ptr [esp + 1ch]
        mov    edi, dword ptr [eax + 0ah]
        mov    eax, dword ptr [esi + 44h]
        movzx  ecx, word ptr [eax - 4]
        cmp    ecx, edi
        jae    SHORT loc_5_8EF4
        jmp    SHORT loc_5_8EFE
loc_5_8EF4:
        cmp    dword ptr [eax + edi*4], ecx
        mov    eax, 1
        ja     SHORT loc_5_8F00
loc_5_8EFE:
        sub    eax, eax
loc_5_8F00:
        or     eax, eax
        jne    SHORT loc_5_8F0A
        mov    ax, 88h
        jmp    SHORT loc_5_8F57
loc_5_8F0A:
        mov    eax, dword ptr [esi + 44h]
        db  08Bh, 00Dh, 094h, 0B8h, 000h, 000h
        mov    eax, dword ptr [eax + edi*4]
        mov    ax, word ptr [eax + 2]
        cmp    word ptr [ecx + 491h], ax
        jne    SHORT loc_5_8F54
        mov    eax, dword ptr [esi + 44h]
        mov    eax, dword ptr [eax + edi*4]
        mov    ax, word ptr [eax]
        push   eax
        db  0E8h, 0CBh, 09Ch, 0FAh, 0FFh
        add    esp, 4
        mov    bx, ax
        or     bx, bx
        je     SHORT loc_5_8F4A
        sub    edx, edx
        mov    ecx, ebx
        db  0E8h, 0BAh, 09Ah, 0FAh, 0FFh
        movzx  bx, al
loc_5_8F4A:
        push   edi
        push   esi
        db  0E8h, 026h, 0D9h, 0FAh, 0FFh
        add    esp, 8
loc_5_8F54:
        mov    ax, bx
loc_5_8F57:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_8F5B:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 14h]
        mov    esi, dword ptr [eax + 0ah]
        mov    eax, dword ptr [edx + 44h]
        movzx  ecx, word ptr [eax - 4]
        cmp    ecx, esi
        jae    SHORT loc_5_8F74
        jmp    SHORT loc_5_8F7E
loc_5_8F74:
        cmp    dword ptr [eax + esi*4], ecx
        mov    eax, 1
        ja     SHORT loc_5_8F80
loc_5_8F7E:
        sub    eax, eax
loc_5_8F80:
        or     eax, eax
        jne    SHORT loc_5_8F8A
        mov    ax, 88h
        jmp    SHORT loc_5_8FAE
loc_5_8F8A:
        mov    eax, dword ptr [edx + 44h]
        mov    eax, dword ptr [eax + esi*4]
        mov    ax, word ptr [eax]
        push   eax
        db  0E8h, 080h, 09Ch, 0FAh, 0FFh
        add    esp, 4
        or     ax, ax
        je     SHORT loc_5_8FAE
        sub    edx, edx
        mov    ecx, eax
        db  0E8h, 056h, 09Ah, 0FAh, 0FFh
        movzx  ax, al
loc_5_8FAE:
        pop    esi
        ret
loc_5_8FB0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 18h]
        mov    edi, dword ptr [esi + 0ah]
        mov    eax, dword ptr [edx + 44h]
        movzx  ecx, word ptr [eax - 4]
        cmp    ecx, edi
        jae    SHORT loc_5_8FCA
        jmp    SHORT loc_5_8FD4
loc_5_8FCA:
        cmp    dword ptr [eax + edi*4], ecx
        mov    eax, 1
        ja     SHORT loc_5_8FD6
loc_5_8FD4:
        sub    eax, eax
loc_5_8FD6:
        or     eax, eax
        jne    SHORT loc_5_8FE0
        mov    ax, 88h
        jmp    SHORT loc_5_9008
loc_5_8FE0:
        mov    eax, dword ptr [edx + 44h]
        add    esi, 0eh
        mov    eax, dword ptr [eax + edi*4]
        push   esi
        mov    ax, word ptr [eax]
        push   eax
        db  0E8h, 03Dh, 09Ch, 0FAh, 0FFh
        add    esp, 8
        or     ax, ax
        je     SHORT loc_5_9008
        sub    edx, edx
        mov    ecx, eax
        db  0E8h, 0FCh, 099h, 0FAh, 0FFh
        movzx  ax, al
loc_5_9008:
        pop    edi
        pop    esi
        ret
loc_5_900B:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        mov    word ptr [esp + 6], 0
        mov    edx, dword ptr [esp + 18h]
        mov    esi, dword ptr [eax + 0ah]
        mov    eax, dword ptr [edx + 44h]
        movzx  ecx, word ptr [eax - 4]
        cmp    ecx, esi
        jae    SHORT loc_5_902E
        jmp    SHORT loc_5_9038
loc_5_902E:
        cmp    dword ptr [eax + esi*4], ecx
        mov    eax, 1
        ja     SHORT loc_5_903A
loc_5_9038:
        sub    eax, eax
loc_5_903A:
        or     eax, eax
        jne    SHORT loc_5_9044
        mov    ax, 88h
        jmp    SHORT loc_5_908D
loc_5_9044:
        mov    eax, dword ptr [edx + 44h]
        lea    ecx, [esp + 6]
        mov    eax, dword ptr [eax + esi*4]
        mov    esi, dword ptr [esp + 10h]
        push   ecx
        mov    ax, word ptr [eax]
        push   eax
        db  0E8h, 024h, 0ACh, 0FAh, 0FFh
        add    esp, 8
        push   eax
        db  0E8h, 003h, 0AAh, 0FAh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 8], eax
        cmp    eax, -1
        jne    SHORT loc_5_9081
        sub    edx, edx
        mov    ecx, dword ptr [esp + 6]
        db  0E8h, 085h, 099h, 0FAh, 0FFh
        movzx  ax, al
        jmp    SHORT loc_5_908D
loc_5_9081:
        mov    eax, dword ptr [esp + 14h]
        mov    word ptr [eax], 4
        sub    ax, ax
loc_5_908D:
        pop    esi
        add    esp, 4
        ret
loc_5_9092:
        sub    esp, 4
        push   esi
        push   0feh
        push   0
        push   0
        mov    esi, dword ptr [esp + 18h]
        push   20h
        lea    eax, [esi + 0ah]
        push   eax
        push   1
        movzx  eax, byte ptr [esi + 8]
        lea    ecx, [esp + 1ch]
        push   eax
        sub    edx, edx
        call   sub_5_62A6
        or     ax, ax
        jne    SHORT loc_5_90F7
        mov    al, byte ptr [esi + 7]
        push   eax
        push   dword ptr [esp + 8]
        db  0E8h, 0E7h, 0B3h, 0FAh, 0FFh
        add    esp, 8
        mov    si, ax
        or     si, si
        je     SHORT loc_5_90E8
        db  0BAh, 00Ch, 0B2h, 000h, 000h
        mov    ecx, esi
        db  0E8h, 01Ch, 099h, 0FAh, 0FFh
        movzx  si, al
loc_5_90E8:
        push   dword ptr [esp + 4]
        db  0E8h, 068h, 0A2h, 0FAh, 0FFh
        add    esp, 4
        mov    ax, si
loc_5_90F7:
        pop    esi
        add    esp, 4
        ret
loc_5_90FC:
        mov    eax, dword ptr [esp + 0ch]
        mov    word ptr [eax], 7
        mov    eax, dword ptr [esp + 8]
        add    eax, 8
        push   eax
        db  0E8h, 082h, 0A9h, 0FAh, 0FFh
        add    esp, 4
        sub    ax, ax
        ret
loc_5_9119:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 18h]
        sub    edi, edi
        mov    esi, dword ptr [esp + 24h]
        mov    edx, dword ptr [ecx + 0ah]
        mov    ebx, dword ptr [esi + 44h]
        movzx  eax, word ptr [ebx - 4]
        cmp    eax, edx
        jae    SHORT loc_5_913A
        jmp    SHORT loc_5_9144
loc_5_913A:
        cmp    dword ptr [ebx + edx*4], eax
        mov    eax, 1
        ja     SHORT loc_5_9146
loc_5_9144:
        sub    eax, eax
loc_5_9146:
        or     eax, eax
        jne    SHORT loc_5_9153
        mov    ax, 88h
        jmp    NEAR PTR loc_5_91E2
loc_5_9153:
        mov    eax, dword ptr [esi + 44h]
        mov    ebx, dword ptr [ecx + 0eh]
        mov    eax, dword ptr [eax + edx*4]
        test   ebx, 1000000h
        mov    bp, word ptr [eax]
        je     SHORT loc_5_916C
        mov    edi, 1
loc_5_916C:
        lea    eax, [esp + 12h]
        push   eax
        mov    ax, word ptr [ecx + 12h]
        push   eax
        db  0E8h, 0E5h, 0A8h, 0FAh, 0FFh
        add    esp, 4
        push   eax
        push   ebx
        db  0E8h, 0E3h, 0A8h, 0FAh, 0FFh
        add    esp, 4
        mov    ebx, dword ptr [esp + 24h]
        push   eax
        lea    eax, [edi + ebx]
        add    eax, 0ah
        push   eax
        push   ebp
        db  0E8h, 0A4h, 099h, 0FAh, 0FFh
        add    esp, 14h
        cmp    word ptr [esp + 12h], 0
        je     SHORT loc_5_91B6
        sub    edx, edx
        mov    ecx, dword ptr [esp + 12h]
        db  0E8h, 050h, 098h, 0FAh, 0FFh
        movzx  ax, al
        jmp    SHORT loc_5_91E2
loc_5_91B6:
        cmp    edi, 1
        jne    SHORT loc_5_91BF
        mov    byte ptr [ebx + 0ah], 0
loc_5_91BF:
        lea    edx, [eax + edi]
        mov    ecx, dword ptr [esp + 20h]
        add    dx, 2
        mov    word ptr [ecx], dx
        push   eax
        db  0E8h, 08Dh, 0A8h, 0FAh, 0FFh
        add    esp, 4
        mov    word ptr [ebx + 8], ax
        or     word ptr [esi + 1ah], 2
        sub    ax, ax
loc_5_91E2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_5_91EA:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 1ch]
        mov    edi, dword ptr [esi + 0ah]
        mov    eax, dword ptr [edx + 44h]
        movzx  ecx, word ptr [eax - 4]
        cmp    ecx, edi
        jae    SHORT loc_5_9207
        jmp    SHORT loc_5_9211
loc_5_9207:
        cmp    dword ptr [eax + edi*4], ecx
        mov    eax, 1
        ja     SHORT loc_5_9213
loc_5_9211:
        sub    eax, eax
loc_5_9213:
        or     eax, eax
        jne    SHORT loc_5_921D
        mov    ax, 88h
        jmp    SHORT loc_5_926E
loc_5_921D:
        mov    eax, dword ptr [edx + 44h]
        mov    eax, dword ptr [eax + edi*4]
        mov    di, word ptr [eax]
        lea    eax, [esp + 0ah]
        push   eax
        mov    ax, word ptr [esi + 12h]
        push   eax
        db  0E8h, 02Bh, 0A8h, 0FAh, 0FFh
        add    esp, 4
        push   eax
        push   dword ptr [esi + 0eh]
        db  0E8h, 027h, 0A8h, 0FAh, 0FFh
        add    esp, 4
        push   eax
        add    esi, 14h
        push   esi
        push   edi
        db  0E8h, 029h, 099h, 0FAh, 0FFh
        add    esp, 14h
        cmp    word ptr [esp + 0ah], 0
        je     SHORT loc_5_926B
        sub    edx, edx
        mov    ecx, dword ptr [esp + 0ah]
        db  0E8h, 09Bh, 097h, 0FAh, 0FFh
        movzx  ax, al
        jmp    SHORT loc_5_926E
loc_5_926B:
        sub    ax, ax
loc_5_926E:
        pop    edi
        pop    esi
        add    esp, 4
        ret
loc_5_9274:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 18h]
        mov    edi, dword ptr [esi + 0ah]
        mov    eax, dword ptr [edx + 44h]
        movzx  ecx, word ptr [eax - 4]
        cmp    ecx, edi
        jae    SHORT loc_5_928E
        jmp    SHORT loc_5_9298
loc_5_928E:
        cmp    dword ptr [eax + edi*4], ecx
        mov    eax, 1
        ja     SHORT loc_5_929A
loc_5_9298:
        sub    eax, eax
loc_5_929A:
        or     eax, eax
        jne    SHORT loc_5_92A4
        mov    ax, 88h
        jmp    SHORT loc_5_92EA
loc_5_92A4:
        mov    eax, dword ptr [edx + 44h]
        mov    eax, dword ptr [eax + edi*4]
        mov    di, word ptr [eax]
        mov    al, byte ptr [esi + 7]
        push   eax
        push   dword ptr [esi + 12h]
        db  0E8h, 0AFh, 0A7h, 0FAh, 0FFh
        add    esp, 4
        push   eax
        push   dword ptr [esi + 0eh]
        db  0E8h, 0A3h, 0A7h, 0FAh, 0FFh
        add    esp, 4
        push   eax
        push   edi
        db  0E8h, 0E3h, 098h, 0FAh, 0FFh
        add    esp, 10h
        mov    cx, ax
        or     cx, cx
        je     SHORT loc_5_92E7
        sub    edx, edx
        db  0E8h, 01Fh, 097h, 0FAh, 0FFh
        movzx  ax, al
        jmp    SHORT loc_5_92EA
loc_5_92E7:
        sub    ax, ax
loc_5_92EA:
        pop    edi
        pop    esi
        ret
loc_5_92ED:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 18h]
        mov    edi, dword ptr [esi + 0ah]
        mov    eax, dword ptr [edx + 44h]
        movzx  ecx, word ptr [eax - 4]
        cmp    ecx, edi
        jae    SHORT loc_5_9307
        jmp    SHORT loc_5_9311
loc_5_9307:
        cmp    dword ptr [eax + edi*4], ecx
        mov    eax, 1
        ja     SHORT loc_5_9313
loc_5_9311:
        sub    eax, eax
loc_5_9313:
        or     eax, eax
        jne    SHORT loc_5_931D
        mov    ax, 88h
        jmp    SHORT loc_5_935F
loc_5_931D:
        mov    eax, dword ptr [edx + 44h]
        push   dword ptr [esi + 12h]
        mov    eax, dword ptr [eax + edi*4]
        mov    di, word ptr [eax]
        db  0E8h, 03Ah, 0A7h, 0FAh, 0FFh
        add    esp, 4
        push   eax
        push   dword ptr [esi + 0eh]
        db  0E8h, 02Eh, 0A7h, 0FAh, 0FFh
        add    esp, 4
        push   eax
        push   edi
        db  0E8h, 095h, 098h, 0FAh, 0FFh
        add    esp, 0ch
        mov    cx, ax
        or     cx, cx
        je     SHORT loc_5_935C
        sub    edx, edx
        db  0E8h, 0AAh, 096h, 0FAh, 0FFh
        movzx  ax, al
        jmp    SHORT loc_5_935F
loc_5_935C:
        sub    ax, ax
loc_5_935F:
        pop    edi
        pop    esi
        ret
loc_5_9362:
        sub    esp, 8
        push   esi
        push   edi
        db  0A1h, 094h, 0B8h, 000h, 000h
        mov    esi, dword ptr [esp + 14h]
        mov    byte ptr [eax + 493h], 1
        movzx  eax, byte ptr [esi + 9]
        lea    ecx, [esi + 9]
        push   0feh
        push   0
        push   0
        push   10h
        lea    edi, [eax + esi + 0ah]
        push   ecx
        push   1
        movzx  eax, byte ptr [esi + 7]
        lea    ecx, [esp + 24h]
        push   eax
        sub    edx, edx
        call   sub_5_62A6
        or     ax, ax
        je     SHORT loc_5_93B8
        cmp    ax, 8ah
        jne    NEAR PTR loc_5_9456
        mov    ax, 8bh
        jmp    NEAR PTR loc_5_9456
loc_5_93B8:
        push   0feh
        push   0
        push   0
        push   4
        lea    eax, [edi + 1]
        push   eax
        push   1
        movzx  eax, byte ptr [edi]
        lea    ecx, [esp + 20h]
        push   eax
        sub    edx, edx
        call   sub_5_62A6
        mov    di, ax
        or     di, di
        je     SHORT loc_5_93ED
        cmp    di, 84h
        jne    SHORT loc_5_9447
        mov    di, 8bh
        jmp    SHORT loc_5_9447
loc_5_93ED:
        movzx  ax, byte ptr [esi + 8]
        push   eax
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 14h]
        db  0E8h, 050h, 0B0h, 0FAh, 0FFh
        mov    di, ax
        add    esp, 0ch
        movzx  eax, di
        or     eax, eax
        je     SHORT loc_5_943B
        cmp    eax, 5
        je     SHORT loc_5_9421
        sub    edx, edx
        mov    ecx, edi
        db  0E8h, 0E5h, 095h, 0FAh, 0FFh
        movzx  di, al
        jmp    SHORT loc_5_943B
loc_5_9421:
        push   dword ptr [esp + 8]
        db  0E8h, 04Ah, 0B1h, 0FAh, 0FFh
        add    esp, 4
        cmp    ax, 0ffffh
        mov    di, 8eh
        je     SHORT loc_5_943B
        mov    di, 92h
loc_5_943B:
        push   dword ptr [esp + 8]
        db  0E8h, 015h, 09Fh, 0FAh, 0FFh
        add    esp, 4
loc_5_9447:
        push   dword ptr [esp + 0ch]
        db  0E8h, 009h, 09Fh, 0FAh, 0FFh
        add    esp, 4
        mov    ax, di
loc_5_9456:
        pop    edi
        pop    esi
        add    esp, 8
        ret
loc_5_945C:
        sub    esp, 4
        push   esi
        db  0A1h, 094h, 0B8h, 000h, 000h
        push   0feh
        mov    byte ptr [eax + 493h], 1
        push   0
        push   0
        mov    esi, dword ptr [esp + 18h]
        push   10h
        lea    eax, [esi + 9]
        push   eax
        push   1
        movzx  eax, byte ptr [esi + 7]
        lea    ecx, [esp + 1ch]
        push   eax
        sub    edx, edx
        call   sub_5_62A6
        or     ax, ax
        je     SHORT loc_5_94A2
        cmp    ax, 0ceh
        jne    SHORT loc_5_9505
        mov    ax, 9bh
        jmp    SHORT loc_5_9505
loc_5_94A2:
        movzx  ax, byte ptr [esi + 8]
        push   eax
        push   dword ptr [esp + 8]
        db  0E8h, 0BDh, 0AFh, 0FAh, 0FFh
        mov    si, ax
        add    esp, 8
        movzx  eax, si
        or     eax, eax
        je     SHORT loc_5_94F6
        cmp    eax, 5
        je     SHORT loc_5_94D2
        sub    edx, edx
        mov    ecx, esi
        db  0E8h, 034h, 095h, 0FAh, 0FFh
        movzx  si, al
        jmp    SHORT loc_5_94F6
loc_5_94D2:
        push   dword ptr [esp + 4]
        db  0E8h, 099h, 0B0h, 0FAh, 0FFh
        add    esp, 4
        cmp    ax, 0ffffh
        jne    SHORT loc_5_94EA
        mov    si, 9ch
        jmp    SHORT loc_5_94F6
loc_5_94EA:
        test   al, 1
        mov    si, 90h
        jne    SHORT loc_5_94F6
        mov    si, 8eh
loc_5_94F6:
        push   dword ptr [esp + 4]
        db  0E8h, 05Ah, 09Eh, 0FAh, 0FFh
        add    esp, 4
        mov    ax, si
loc_5_9505:
        pop    esi
        add    esp, 4
        ret
loc_5_950A:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 28h]
        mov    dword ptr [esp + 20h], 0
        mov    ecx, dword ptr [esp + 34h]
        mov    edi, dword ptr [esi + 0ah]
        mov    eax, dword ptr [ecx + 44h]
        movzx  edx, word ptr [eax - 4]
        cmp    edx, edi
        jae    SHORT loc_5_9531
        jmp    SHORT loc_5_953B
loc_5_9531:
        cmp    dword ptr [eax + edi*4], edx
        mov    eax, 1
        ja     SHORT loc_5_953D
loc_5_953B:
        sub    eax, eax
loc_5_953D:
        or     eax, eax
        jne    SHORT loc_5_954A
        mov    ax, 88h
        jmp    NEAR PTR loc_5_96FA
loc_5_954A:
        mov    eax, dword ptr [ecx + 44h]
        mov    edx, dword ptr [esi + 10h]
        movzx  ebx, word ptr [eax - 4]
        cmp    ebx, edx
        jae    SHORT loc_5_955A
        jmp    SHORT loc_5_9564
loc_5_955A:
        cmp    dword ptr [eax + edx*4], ebx
        mov    eax, 1
        ja     SHORT loc_5_9566
loc_5_9564:
        sub    eax, eax
loc_5_9566:
        or     eax, eax
        jne    SHORT loc_5_9573
        mov    ax, 88h
        jmp    NEAR PTR loc_5_96FA
loc_5_9573:
        mov    eax, dword ptr [ecx + 44h]
        mov    eax, dword ptr [eax + edi*4]
        push   dword ptr [esi + 1ch]
        mov    ax, word ptr [eax]
        mov    word ptr [esp + 1ah], ax
        mov    eax, dword ptr [ecx + 44h]
        mov    eax, dword ptr [eax + edx*4]
        mov    ax, word ptr [eax]
        mov    word ptr [esp + 18h], ax
        db  0E8h, 0D1h, 0A4h, 0FAh, 0FFh
        add    esp, 4
        mov    edi, eax
        push   dword ptr [esi + 14h]
        db  0E8h, 0C4h, 0A4h, 0FAh, 0FFh
        mov    dword ptr [esp + 20h], eax
        add    esp, 4
        push   dword ptr [esi + 18h]
        db  0E8h, 0B5h, 0A4h, 0FAh, 0FFh
        mov    dword ptr [esp + 1ch], eax
        add    esp, 4
        push   0f000h
        db  068h, 030h, 0B9h, 000h, 000h
        db  0E8h, 0F7h, 07Eh, 0FAh, 0FFh
        add    esp, 8
        push   0fh
        db  0E8h, 03Dh, 080h, 0FAh, 0FFh
        add    esp, 4
        mov    esi, eax
loc_5_95D8:
        or     edi, edi
        je     NEAR PTR loc_5_96B2
        or     esi, esi
        je     NEAR PTR loc_5_9695
        mov    ebx, edi
        cmp    ebx, 0f000h
        jb     SHORT loc_5_95F7
        mov    ebx, 0f000h
loc_5_95F7:
        lea    eax, [esp + 12h]
        push   eax
        push   ebx
        push   dword ptr [esp + 24h]
        push   esi
        push   dword ptr [esp + 26h]
        db  0E8h, 033h, 095h, 0FAh, 0FFh
        add    esp, 14h
        mov    ebp, eax
        cmp    word ptr [esp + 12h], 0
        jne    NEAR PTR loc_5_969E
        lea    eax, [esp + 12h]
        push   eax
        push   ebp
        push   dword ptr [esp + 20h]
        push   esi
        sub    edi, ebp
        push   dword ptr [esp + 24h]
        db  0E8h, 046h, 095h, 0FAh, 0FFh
        add    dword ptr [esp + 34h], ebp
        add    dword ptr [esp + 30h], ebp
        add    dword ptr [esp + 2ch], ebp
        add    esp, 14h
        cmp    word ptr [esp + 12h], 0
        jne    SHORT loc_5_969E
        db  083h, 03Dh, 034h, 0B9h, 000h, 000h, 000h
        je     SHORT loc_5_968B
        push   esi
        db  0E8h, 0DAh, 07Fh, 0FAh, 0FFh
        add    esp, 4
        push   0f000h
        db  068h, 030h, 0B9h, 000h, 000h
        db  0E8h, 0B2h, 07Eh, 0FAh, 0FFh
        add    esp, 8
        push   0f000h
        db  068h, 030h, 0B9h, 000h, 000h
        db  0E8h, 044h, 07Eh, 0FAh, 0FFh
        add    esp, 8
        push   0fh
        db  0E8h, 08Ah, 07Fh, 0FAh, 0FFh
        add    esp, 4
        mov    esi, eax
loc_5_968B:
        cmp    ebx, ebp
        je     NEAR PTR loc_5_95D8
        jmp    SHORT loc_5_96B2
loc_5_9695:
        mov    word ptr [esp + 12h], 96h
        jmp    SHORT loc_5_96D6
loc_5_969E:
        sub    edx, edx
        mov    ecx, dword ptr [esp + 12h]
        db  0E8h, 057h, 093h, 0FAh, 0FFh
        movzx  ax, al
        mov    word ptr [esp + 12h], ax
loc_5_96B2:
        cmp    word ptr [esp + 12h], 0
        jne    SHORT loc_5_96D6
        mov    eax, dword ptr [esp + 30h]
        push   dword ptr [esp + 20h]
        mov    word ptr [eax], 4
        db  0E8h, 09Ch, 0A3h, 0FAh, 0FFh
        mov    ecx, dword ptr [esp + 30h]
        add    esp, 4
        mov    dword ptr [ecx + 8], eax
loc_5_96D6:
        or     esi, esi
        je     SHORT loc_5_96E3
        push   esi
        db  0E8h, 052h, 07Fh, 0FAh, 0FFh
        add    esp, 4
loc_5_96E3:
        push   0f000h
        db  068h, 030h, 0B9h, 000h, 000h
        db  0E8h, 02Ah, 07Eh, 0FAh, 0FFh
        mov    ax, word ptr [esp + 1ah]
        add    esp, 8
loc_5_96FA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
_PTEXT ends
        end
