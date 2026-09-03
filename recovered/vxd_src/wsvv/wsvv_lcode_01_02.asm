; wsvv_lcode_01_02.asm -- WSVV object 1 segment _LTEXT  (part 2/5)
.386p
_LTEXT segment dword public use32 'LCODE'

sub_1_4723:
        mov    ecx, dword ptr [esp + 4]
        db  0A1h, 08Ch, 038h, 001h, 000h
        mov    dword ptr [ecx], eax
        db  089h, 00Dh, 08Ch, 038h, 001h, 000h
        ret
sub_1_4735:
        db  0A1h, 08Ch, 038h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_4748
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 08Ch, 038h, 001h, 000h
        jmp    SHORT loc_1_4776
loc_1_4748:
        db  08Bh, 00Dh, 060h, 036h, 001h, 000h
        db  039h, 00Dh, 0BCh, 02Ah, 001h, 000h
        mov    eax, 0
        jbe    SHORT loc_1_476B
        push   1ch
        call   sub_1_F390
        add    esp, 4
        db  08Bh, 00Dh, 060h, 036h, 001h, 000h
loc_1_476B:
        or     eax, eax
        je     SHORT loc_1_4776
        inc    ecx
        db  089h, 00Dh, 060h, 036h, 001h, 000h
loc_1_4776:
        ret
sub_1_4777:
        mov    ecx, dword ptr [esp + 4]
        mov    edx, ecx
        and    edx, 0ffffffh
        db  039h, 015h, 0BCh, 037h, 001h, 000h
        jbe    SHORT loc_1_479F
        db  0A1h, 0C0h, 02Ah, 001h, 000h
        mov    eax, dword ptr [eax + edx*4]
        or     eax, eax
        je     SHORT loc_1_47A1
        shr    ecx, 18h
        cmp    cl, byte ptr [eax + 0ch]
        je     SHORT loc_1_47A1
loc_1_479F:
        sub    eax, eax
loc_1_47A1:
        ret
sub_1_47A2:
        push   ebx
        push   esi
        push   edi
        push   ebp
loc_1_47A6:
        db  08Bh, 015h, 0C0h, 02Ah, 001h, 000h
        db  08Bh, 01Dh, 0BCh, 037h, 001h, 000h
loc_1_47B2:
        db  08Bh, 035h, 05Ch, 038h, 001h, 000h
        sub    eax, eax
        cmp    ebx, eax
        je     SHORT loc_1_47D0
loc_1_47BE:
        cmp    ebx, esi
        jne    SHORT loc_1_47C4
        sub    esi, esi
loc_1_47C4:
        cmp    dword ptr [edx + esi*4], 0
        je     SHORT loc_1_47D0
        inc    esi
        inc    eax
        cmp    ebx, eax
        ja     SHORT loc_1_47BE
loc_1_47D0:
        db  089h, 015h, 0C0h, 02Ah, 001h, 000h
        db  089h, 01Dh, 0BCh, 037h, 001h, 000h
        cmp    ebx, eax
        ja     SHORT loc_1_485E
        db  03Bh, 01Dh, 00Ch, 02Bh, 001h, 000h
        je     NEAR PTR loc_1_488F
        add    ebx, 10h
        db  0A1h, 00Ch, 02Bh, 001h, 000h
        cmp    ebx, eax
        jb     SHORT loc_1_47FA
        mov    ebx, eax
loc_1_47FA:
        lea    eax, [ebx*4]
        push   eax
        call   sub_1_F390
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        db  08Bh, 02Dh, 0C0h, 02Ah, 001h, 000h
        db  08Bh, 035h, 0BCh, 037h, 001h, 000h
        je     SHORT loc_1_488F
        lea    ecx, [ebx*4]
        mov    edi, edx
        sub    eax, eax
        shr    ecx, 2
        rep stosd
        lea    ecx, [esi*4]
        mov    edi, edx
        mov    esi, ebp
        shr    ecx, 2
        rep movsd
        db  089h, 015h, 0C0h, 02Ah, 001h, 000h
        db  089h, 01Dh, 0BCh, 037h, 001h, 000h
        cmp    ebp, eax
        je     NEAR PTR loc_1_47B2
        push   ebp
        call   sub_1_F396
        add    esp, 4
        jmp    NEAR PTR loc_1_47A6
loc_1_485E:
        db  089h, 015h, 0C0h, 02Ah, 001h, 000h
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [edx + esi*4], ecx
        lea    eax, [esi + 1]
        db  0A3h, 05Ch, 038h, 001h, 000h
        db  0A0h, 068h, 038h, 001h, 000h
        inc    al
        db  0A2h, 068h, 038h, 001h, 000h
        mov    byte ptr [ecx + 0ch], al
        movzx  eax, al
        shl    eax, 18h
        or     eax, esi
        mov    dword ptr [ecx + 10h], eax
        jmp    SHORT loc_1_4894
loc_1_488F:
        mov    eax, 0ffffffffh
loc_1_4894:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_4899:
        mov    ecx, dword ptr [esp + 4]
        db  0A1h, 0C0h, 02Ah, 001h, 000h
        and    ecx, 0ffffffh
        mov    dword ptr [eax + ecx*4], 0
        ret
sub_1_48B0:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 2afah
        je     SHORT loc_1_48C8
        cmp    eax, 2afbh
        je     SHORT loc_1_48CF
        mov    eax, dword ptr [esp + 8]
        jmp    SHORT loc_1_48D4
loc_1_48C8:
        mov    eax, 1ch
        jmp    SHORT loc_1_48D4
loc_1_48CF:
        mov    eax, 1dh
loc_1_48D4:
        ret
sub_1_48D5:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 102h]
        or     ecx, ecx
        je     SHORT loc_1_48ED
        cmp    dword ptr [ecx], eax
        jne    SHORT loc_1_48ED
        mov    dword ptr [ecx], 0
loc_1_48ED:
        ret
sub_1_48EE:
        push   dword ptr [esp + 4]
        call   sub_1_48D5
        add    esp, 4
        ret
sub_1_48FB:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        sub    edx, edx
        mov    esi, dword ptr [ecx + 102h]
        cmp    esi, edx
        je     SHORT loc_1_4925
        mov    ax, word ptr [esi + 0eh]
        dec    ax
        mov    word ptr [esi + 0eh], ax
        jne    SHORT loc_1_491B
        mov    edx, dword ptr [esi + 20h]
loc_1_491B:
        mov    dword ptr [ecx + 102h], 0
loc_1_4925:
        or     edx, edx
        je     SHORT loc_1_492F
        push   esi
        call   edx
        add    esp, 4
loc_1_492F:
        pop    esi
        ret
sub_1_4931:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 14h]
        mov    bl, byte ptr [edi + 0bch]
        or     bl, bl
        je     NEAR PTR loc_1_4B6D
        mov    byte ptr [edi + 0bch], 0
        push   edi
        call   sub_1_48D5
        add    esp, 4
        push   edi
        call   sub_1_CD4E
        add    esp, 4
        cmp    bl, 3
        jbe    SHORT loc_1_4979
        cmp    bl, 9
        jae    SHORT loc_1_4979
        test   byte ptr [edi + 63h], 40h
        je     SHORT loc_1_4979
        push   edi
        call   sub_1_AD7A
        add    esp, 4
loc_1_4979:
        mov    al, byte ptr [edi + 10eh]
        test   al, 80h
        je     SHORT loc_1_498A
        mov    ebx, 14h
        jmp    SHORT loc_1_49C3
loc_1_498A:
        test   al, 40h
        je     SHORT loc_1_4995
        mov    ebx, 13h
        jmp    SHORT loc_1_49C3
loc_1_4995:
        test   al, 20h
        je     SHORT loc_1_499D
        push   15h
        jmp    SHORT loc_1_49B3
loc_1_499D:
        test   al, 10h
        je     SHORT loc_1_49A8
        mov    ebx, 0eh
        jmp    SHORT loc_1_49C3
loc_1_49A8:
        test   al, 8
        mov    ebx, 0
        je     SHORT loc_1_49C3
        push   1ch
loc_1_49B3:
        push   dword ptr [edi + 0c0h]
        call   sub_1_48B0
        add    esp, 8
        mov    ebx, eax
loc_1_49C3:
        push   dword ptr [edi + 8ch]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_49E5
        mov    cl, byte ptr [edi + 98h]
        push   ecx
        push   eax
        call   sub_1_3B6C
        add    esp, 8
loc_1_49E5:
        mov    esi, dword ptr [edi + 0eah]
        or     esi, esi
        je     SHORT loc_1_4A04
        push   0
        push   ebx
        push   dword ptr [esi + 0ch]
        call   dword ptr [esi + 8]
        add    esp, 0ch
        push   esi
        call   sub_1_4723
        add    esp, 4
loc_1_4A04:
        mov    esi, dword ptr [edi + 11ah]
        or     esi, esi
        je     SHORT loc_1_4A23
        push   0
        push   ebx
        push   dword ptr [esi + 0ch]
        call   dword ptr [esi + 8]
        add    esp, 0ch
        push   esi
        call   sub_1_4723
        add    esp, 4
loc_1_4A23:
        mov    eax, dword ptr [edi + 40h]
        lea    ebp, [edi + 40h]
        sub    eax, edi
        cmp    eax, 40h
        je     SHORT loc_1_4A92
loc_1_4A30:
        mov    esi, dword ptr [ebp]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebp], eax
        mov    dword ptr [eax + 4], ebp
        mov    ecx, 0
        mov    eax, dword ptr [esi + 18h]
        dec    eax
        mov    dword ptr [esi + 18h], eax
        or     eax, eax
        setl   cl
        or     eax, eax
        mov    eax, 0
        setle  al
        add    eax, ecx
        je     SHORT loc_1_4A85
        mov    eax, dword ptr [esi + 2ch]
        or     eax, eax
        je     SHORT loc_1_4A6E
        mov    dword ptr [eax], 0
        mov    dword ptr [esi + 2ch], 0
loc_1_4A6E:
        push   0
        push   ebx
        push   dword ptr [esi + 0ch]
        call   dword ptr [esi + 8]
        add    esp, 0ch
        push   esi
        call   sub_1_A7D4
        add    esp, 4
        jmp    SHORT loc_1_4A88
loc_1_4A85:
        mov    dword ptr [esi + 10h], ebx
loc_1_4A88:
        mov    eax, dword ptr [ebp]
        sub    eax, edi
        cmp    eax, 40h
        jne    SHORT loc_1_4A30
loc_1_4A92:
        cmp    dword ptr [edi + 0d6h], 0
        je     SHORT loc_1_4AC7
loc_1_4A9B:
        mov    esi, dword ptr [edi + 0d6h]
        push   0
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 0d6h], eax
        push   ebx
        push   dword ptr [esi + 8]
        call   dword ptr [esi + 4]
        add    esp, 0ch
        push   esi
        call   sub_1_7AA0
        add    esp, 4
        cmp    dword ptr [edi + 0d6h], 0
        jne    SHORT loc_1_4A9B
loc_1_4AC7:
        cmp    dword ptr [edi + 11eh], 0
        je     SHORT loc_1_4AFC
loc_1_4AD0:
        mov    esi, dword ptr [edi + 11eh]
        push   0
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 11eh], eax
        push   ebx
        push   dword ptr [esi + 8]
        call   dword ptr [esi + 4]
        add    esp, 0ch
        push   esi
        call   sub_1_7AA0
        add    esp, 4
        cmp    dword ptr [edi + 11eh], 0
        jne    SHORT loc_1_4AD0
loc_1_4AFC:
        mov    eax, dword ptr [edi + 0e2h]
        or     eax, eax
        je     SHORT loc_1_4B0F
        push   eax
        call   sub_1_95BA
        add    esp, 4
loc_1_4B0F:
        mov    eax, dword ptr [edi + 122h]
        or     eax, eax
        je     SHORT loc_1_4B22
        push   eax
        call   sub_1_95BA
        add    esp, 4
loc_1_4B22:
        cmp    dword ptr [edi + 0d2h], 0
        je     SHORT loc_1_4B5B
loc_1_4B2B:
        mov    esi, dword ptr [edi + 0d2h]
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 0d2h], eax
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_1_4B49
        push   eax
        call   sub_1_95BA
        add    esp, 4
loc_1_4B49:
        push   esi
        call   sub_1_F396
        add    esp, 4
        cmp    dword ptr [edi + 0d2h], 0
        jne    SHORT loc_1_4B2B
loc_1_4B5B:
        push   edi
        call   sub_1_48FB
        add    esp, 4
        push   edi
        call   sub_1_CF22
        add    esp, 4
loc_1_4B6D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_4B72:
        mov    eax, dword ptr [esp + 4]
        mov    cl, byte ptr [esp + 8]
        or     byte ptr [eax + 10eh], cl
        mov    cl, byte ptr [eax + 0beh]
        test   cl, 1
        jne    SHORT loc_1_4BB0
        or     cl, 1
        mov    byte ptr [eax + 0beh], cl
        inc    byte ptr [eax + 0bah]
        or     dword ptr [eax + 64h], 1
        cmp    dword ptr [eax + 0a6h], 0
        jne    SHORT loc_1_4BB0
        push   eax
        call   sub_1_4931
        add    esp, 4
loc_1_4BB0:
        ret
sub_1_4BB1:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0a6h]
        dec    eax
        mov    dword ptr [ecx + 0a6h], eax
        jne    SHORT loc_1_4BDB
        mov    al, byte ptr [ecx + 0beh]
        or     al, al
        je     SHORT loc_1_4BDB
        test   al, 1
        je     SHORT loc_1_4BDB
        push   ecx
        call   sub_1_4931
        add    esp, 4
loc_1_4BDB:
        ret
loc_1_4BDC:
        push   esi
        push   edi
        push   28h
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_4C37
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 0ah
        rep stosd
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 14h], eax
        push   esi
        call   sub_1_47A2
        add    esp, 4
        cmp    eax, -1
        je     SHORT loc_1_4C2E
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [ecx], eax
        mov    word ptr [esi + 0eh], 0
        mov    byte ptr [esi + 0dh], 0
        sub    eax, eax
        db  0C7h, 046h, 020h, 090h, 046h, 000h, 000h
        jmp    SHORT loc_1_4C3C
loc_1_4C2E:
        push   esi
        call   sub_1_F396
        add    esp, 4
loc_1_4C37:
        mov    eax, 1
loc_1_4C3C:
        pop    edi
        pop    esi
        ret
sub_1_4C3F:
        push   esi
        mov    esi, dword ptr [esp + 8]
        add    esi, 42h
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_1_4C70
        mov    edx, dword ptr [esp + 0ch]
        sub    ecx, ecx
loc_1_4C53:
        mov    eax, dword ptr [esi + 8]
        cmp    edx, eax
        je     SHORT loc_1_4C63
        mov    esi, eax
        cmp    dword ptr [esi + 8], ecx
        jne    SHORT loc_1_4C53
        jmp    SHORT loc_1_4C70
loc_1_4C63:
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [edx + 4], 0
loc_1_4C70:
        pop    esi
        ret
loc_1_4C72:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    ebx, dword ptr [edi]
        push   ebx
        call   sub_1_4777
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_4D41
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [eax + 38h]
        cmp    ecx, -1
        jne    SHORT loc_1_4C9C
        mov    ecx, dword ptr [eax + 3ch]
loc_1_4C9C:
        push   ecx
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_4CB4
        mov    ecx, dword ptr [eax + 14h]
        or     ecx, ecx
        je     SHORT loc_1_4CB4
        dec    ecx
        mov    dword ptr [eax + 14h], ecx
loc_1_4CB4:
        push   ebx
        call   sub_1_4899
        add    esp, 4
        mov    cl, byte ptr [esi + 0dh]
        or     cl, 1
        mov    byte ptr [esi + 0dh], cl
        cmp    word ptr [esi + 0eh], 0
        je     SHORT loc_1_4D2D
        mov    edx, dword ptr [esi + 18h]
        mov    ebx, dword ptr [esi + 1ch]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 18h], eax
        test   cl, 2
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 1ch], eax
        db  0C7h, 046h, 020h, 092h, 046h, 000h, 000h
        je     SHORT loc_1_4CF5
        push   0
        push   20h
        push   ebx
        call   edx
        add    esp, 0ch
loc_1_4CF5:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_1_4D26
        cmp    byte ptr [eax + 0bch], 0
        je     SHORT loc_1_4D26
        or     dword ptr [eax + 60h], 40000000h
        test   byte ptr [eax + 0beh], 1
        jne    SHORT loc_1_4D1F
        push   40h
        push   eax
        call   sub_1_4B72
        add    esp, 8
loc_1_4D1F:
        mov    eax, 0ffh
        jmp    SHORT loc_1_4D46
loc_1_4D26:
        mov    eax, 0ffh
        jmp    SHORT loc_1_4D46
loc_1_4D2D:
        db  0C7h, 046h, 018h, 091h, 046h, 000h, 000h
        push   esi
        call   sub_1_4692
        add    esp, 4
        sub    eax, eax
        jmp    SHORT loc_1_4D46
loc_1_4D41:
        mov    eax, 0fh
loc_1_4D46:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_4D4A:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        push   dword ptr [esp + 10h]
        mov    edi, dword ptr [eax]
        call   sub_1_3150
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_4D6B
        mov    eax, 6
        jmp    SHORT loc_1_4D9A
loc_1_4D6B:
        push   edi
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_4D95
        cmp    dword ptr [eax + 4], 0
        je     SHORT loc_1_4D85
        mov    eax, 10h
        jmp    SHORT loc_1_4D9A
loc_1_4D85:
        mov    dword ptr [eax + 4], esi
        mov    ecx, dword ptr [esi + 4ah]
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [esi + 4ah], eax
        sub    eax, eax
        jmp    SHORT loc_1_4D9A
loc_1_4D95:
        mov    eax, 0fh
loc_1_4D9A:
        pop    edi
        pop    esi
        ret
loc_1_4D9D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi]
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_4DFD
        mov    ecx, dword ptr [eax + 4]
        or     ecx, ecx
        je     SHORT loc_1_4DF6
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_4DEF
        cmp    word ptr [eax + 0eh], 0
        jne    SHORT loc_1_4DD1
        push   eax
        push   ecx
        call   sub_1_4C3F
        add    esp, 8
        sub    eax, eax
        jmp    SHORT loc_1_4E02
loc_1_4DD1:
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 18h], ecx
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [eax + 1ch], ecx
        db  0C7h, 040h, 020h, 0C7h, 046h, 000h, 000h
        or     byte ptr [eax + 0dh], 2
        mov    eax, 0ffh
        jmp    SHORT loc_1_4E02
loc_1_4DEF:
        mov    eax, 12h
        jmp    SHORT loc_1_4E02
loc_1_4DF6:
        mov    eax, 11h
        jmp    SHORT loc_1_4E02
loc_1_4DFD:
        mov    eax, 0fh
loc_1_4E02:
        pop    esi
        ret
sub_1_4E04:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 10h]
        test   byte ptr [edx + 0dh], 3
        je     SHORT loc_1_4E1B
        mov    eax, 0fh
        jmp    NEAR PTR loc_1_4EB2
loc_1_4E1B:
        cmp    dword ptr [edx], 0
        jne    NEAR PTR loc_1_4EAD
        mov    ebx, dword ptr [edx + 4]
        or     ebx, ebx
        je     SHORT loc_1_4EA6
        mov    ecx, dword ptr [esp + 14h]
        lea    esi, [ebx + 28h]
        lea    edi, [ecx + 6ch]
        movsd
        movsd
        movsd
        movsd
        mov    eax, dword ptr [ebx + 38h]
        cmp    eax, -1
        jne    SHORT loc_1_4E44
        mov    eax, dword ptr [ebx + 3ch]
loc_1_4E44:
        mov    dword ptr [ecx + 8ch], eax
        mov    esi, dword ptr [ebx + 72h]
        mov    dword ptr [ecx + 112h], esi
        mov    al, byte ptr [ebx + 42h]
        or     esi, esi
        mov    byte ptr [ecx + 99h], al
        mov    eax, dword ptr [ebx + 76h]
        mov    dword ptr [ecx + 116h], eax
        db  0C7h, 041h, 068h, 0CAh, 07Ch, 000h, 000h
        je     SHORT loc_1_4E77
        db  0C7h, 041h, 068h, 0D3h, 07Fh, 000h, 000h
loc_1_4E77:
        mov    eax, dword ptr [edx + 14h]
        mov    dword ptr [ecx + 0eeh], eax
        mov    eax, dword ptr [edx + 24h]
        or     eax, dword ptr [ecx + 60h]
        test   eax, 2000000h
        mov    dword ptr [ecx + 60h], eax
        je     SHORT loc_1_4EA2
        db  0A1h, 030h, 038h, 001h, 000h
        mov    dword ptr [ecx + 0ceh], eax
        mov    byte ptr [ecx + 0bfh], 0
loc_1_4EA2:
        sub    eax, eax
        jmp    SHORT loc_1_4EB2
loc_1_4EA6:
        mov    eax, 11h
        jmp    SHORT loc_1_4EB2
loc_1_4EAD:
        mov    eax, 12h
loc_1_4EB2:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_4EB6:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 34h]
        cmp    dword ptr [esp + 3ch], 0
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 18h], eax
        je     NEAR PTR loc_1_513E
        mov    eax, dword ptr [esp + 3ch]
        mov    ecx, dword ptr [eax + 14h]
        or     ecx, ecx
        je     NEAR PTR loc_1_513E
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        push   ecx
        call   sub_1_3BB5
        add    esp, 10h
        or     al, al
        je     NEAR PTR loc_1_513E
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [esp + 10h], eax
        call   sub_1_4735
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_4F1E
        mov    eax, 1
        jmp    NEAR PTR loc_1_5143
loc_1_4F1E:
        call   sub_1_CE5D
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_4F3C
        push   ebp
        call   sub_1_4723
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_5143
loc_1_4F3C:
        mov    eax, dword ptr [esp + 38h]
        or     eax, eax
        je     SHORT loc_1_4F69
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_1_4F69
        mov    ecx, 64h
        sub    edx, edx
        div    ecx
        inc    eax
        cmp    eax, 0ffffffh
        jbe    SHORT loc_1_4F62
        mov    eax, 0ffffffh
        jmp    SHORT loc_1_4F63
loc_1_4F62:
        inc    eax
loc_1_4F63:
        mov    word ptr [ebp + 1ah], ax
        jmp    SHORT loc_1_4F6F
loc_1_4F69:
        mov    word ptr [ebp + 1ah], 0
loc_1_4F6F:
        mov    word ptr [ebp + 18h], 0
        mov    eax, dword ptr [esp + 40h]
        mov    dword ptr [ebp + 14h], eax
        mov    eax, dword ptr [esi + 4]
        lea    edi, [ebx + 7ch]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp + 0ch], eax
        or     dword ptr [ebx + 60h], 8
        lea    esi, [esp + 20h]
        movsd
        movsd
        movsd
        movsd
        push   dword ptr [esp + 18h]
        call   sub_1_4777
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_5123
        db  083h, 03Dh, 054h, 036h, 001h, 000h, 000h
        jne    SHORT loc_1_4FDD
        db  068h, 073h, 052h, 000h, 000h
        push   12h
        call   sub_1_E730
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_4FD3
        mov    eax, 20h
        jmp    NEAR PTR loc_1_5143
loc_1_4FD3:
        db  0C7h, 005h, 054h, 036h, 001h, 000h, 001h, 000h, 000h, 000h
loc_1_4FDD:
        push   dword ptr [esp + 3ch]
        push   ebx
        push   esi
        call   sub_1_4E04
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        jne    NEAR PTR loc_1_5128
        mov    dword ptr [esi], ebx
        inc    word ptr [esi + 0eh]
        mov    dword ptr [ebx + 102h], esi
        db  0A1h, 0A0h, 036h, 001h, 000h
        sub    eax, 3ch
        mov    dword ptr [ebx + 9eh], eax
        push   ebx
        call   sub_1_B12A
        add    esp, 4
        mov    dword ptr [ebx + 0a6h], 1
        mov    byte ptr [ebx + 0bch], 2
        mov    dword ptr [ebx + 0eah], ebp
        push   ebx
        call   sub_1_CCC6
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_506A
        mov    dword ptr [ebx + 0eah], 0
        dec    dword ptr [ebx + 0a6h]
        push   40h
        push   ebx
        call   sub_1_4B72
        add    esp, 8
        push   ebp
        call   sub_1_4723
        add    esp, 4
        mov    eax, 2
        jmp    NEAR PTR loc_1_5143
loc_1_506A:
        test   byte ptr [ebx + 0beh], 1
        jne    NEAR PTR loc_1_5113
        cmp    byte ptr [ebx + 0bch], 2
        jne    NEAR PTR loc_1_5113
        cmp    dword ptr [esp + 10h], -1
        jne    SHORT loc_1_5095
        mov    eax, dword ptr [ebx + 8ch]
        mov    dword ptr [esp + 10h], eax
loc_1_5095:
        push   dword ptr [esp + 10h]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_50AB
        cmp    byte ptr [eax + 1ah], 0
        jne    SHORT loc_1_50D1
loc_1_50AB:
        push   0
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   12h
        call   sub_1_E8C0
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_50D1
        cmp    eax, 2b0ah
        je     SHORT loc_1_50D1
        mov    eax, 0fh
        jmp    SHORT loc_1_5143
loc_1_50D1:
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   12h
        call   sub_1_E90F
        add    esp, 10h
        mov    esi, eax
        push   ebx
        call   sub_1_4BB1
        add    esp, 4
        cmp    esi, 2b0ah
        jne    SHORT loc_1_5101
        mov    eax, 0ffh
        jmp    SHORT loc_1_5143
loc_1_5101:
        or     esi, esi
        jne    SHORT loc_1_510C
        mov    eax, 0ffh
        jmp    SHORT loc_1_5143
loc_1_510C:
        mov    eax, 0fh
        jmp    SHORT loc_1_5143
loc_1_5113:
        push   ebx
        call   sub_1_4BB1
        add    esp, 4
        mov    eax, 0ffh
        jmp    SHORT loc_1_5143
loc_1_5123:
        mov    edi, 0fh
loc_1_5128:
        push   ebp
        call   sub_1_4723
        add    esp, 4
        push   ebx
        call   sub_1_F396
        add    esp, 4
        mov    eax, edi
        jmp    SHORT loc_1_5143
loc_1_513E:
        mov    eax, 3
loc_1_5143:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
        db  08Bh, 044h, 024h, 004h, 08Bh, 040h, 04Ah, 00Bh, 0C0h, 074h, 01Ah, 0BAh, 001h, 000h, 000h, 000h
        db  08Bh, 008h, 00Bh, 0C9h, 074h, 008h, 088h, 091h, 0BCh, 000h, 000h, 000h, 0EBh, 009h, 08Bh, 040h
        db  008h, 00Bh, 0C0h, 075h, 0EBh, 02Bh, 0C0h, 0C3h
sub_1_5173:
        push   esi
        call   sub_1_F389
        mov    esi, eax
        call   sub_1_F37E
        xor    esi, eax
        mov    eax, esi
        pop    esi
        ret
sub_1_5186:
        sub    esp, 14h
        push   esi
        mov    esi, 1
loc_1_518F:
        call   sub_1_129F8
        mov    dword ptr [esp + 4], eax
        db  0A1h, 070h, 037h, 001h, 000h
        cmp    dword ptr [esp + 4], eax
        jne    SHORT loc_1_51B6
        mov    dword ptr [esp + 14h], esi
        lea    eax, [esp + 0ch]
        push   eax
        call   sub_1_EE98
        add    esp, 4
        jmp    SHORT loc_1_518F
loc_1_51B6:
        mov    eax, dword ptr [esp + 4]
        db  0A3h, 070h, 037h, 001h, 000h
        pop    esi
        add    esp, 14h
        ret
sub_1_51C4:
        db  0A1h, 070h, 037h, 001h, 000h
        shr    eax, 10h
        ret
sub_1_51CD:
        mov    al, 69h
        ret
sub_1_51D0:
        push   esi
        call   sub_1_5173
        mov    esi, dword ptr [esp + 8]
        mov    dword ptr [esi], eax
        call   sub_1_5186
        mov    word ptr [esi + 4], ax
        call   sub_1_51C4
        mov    word ptr [esi + 6], ax
        mov    byte ptr [esi + 8], 0c0h
        call   sub_1_51CD
        mov    byte ptr [esi + 9], al
        db  0A0h, 0C4h, 02Ah, 001h, 000h
        mov    byte ptr [esi + 0ah], al
        db  0A0h, 0C5h, 02Ah, 001h, 000h
        mov    byte ptr [esi + 0bh], al
        db  0A0h, 0C6h, 02Ah, 001h, 000h
        mov    byte ptr [esi + 0ch], al
        db  0A0h, 0C7h, 02Ah, 001h, 000h
        mov    byte ptr [esi + 0dh], al
        db  0A0h, 0C8h, 02Ah, 001h, 000h
        mov    byte ptr [esi + 0eh], al
        db  0A0h, 0C9h, 02Ah, 001h, 000h
        mov    byte ptr [esi + 0fh], al
        sub    eax, eax
        pop    esi
        ret    4
sub_1_5230:
        push   ebx
        push   esi
        sub    bl, bl
        db  0BAh, 0D8h, 038h, 001h, 000h
        mov    eax, dword ptr [esp + 0ch]
loc_1_523D:
        mov    ecx, dword ptr [edx]
        or     ecx, ecx
        je     SHORT loc_1_525D
        mov    esi, 80h
loc_1_5248:
        test   word ptr [ecx + 40h], si
        je     SHORT loc_1_5253
        cmp    dword ptr [ecx + 3ch], eax
        je     SHORT loc_1_525B
loc_1_5253:
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_5248
        jmp    SHORT loc_1_525D
loc_1_525B:
        mov    bl, 1
loc_1_525D:
        cmp    bl, 1
        je     SHORT loc_1_526D
        add    edx, 4
        db  081h, 0FAh, 018h, 039h, 001h, 000h
        jb     SHORT loc_1_523D
loc_1_526D:
        movzx  eax, bl
        pop    esi
        pop    ebx
        ret
loc_1_5273:
        sub    esp, 44h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 5ch]
        cmp    eax, 0ech
        ja     NEAR PTR loc_1_5330
        je     NEAR PTR loc_1_5B1D
        cmp    eax, 0e6h
        ja     SHORT loc_1_52FB
        je     NEAR PTR loc_1_55FA
        or     eax, eax
        je     NEAR PTR loc_1_53D0
        cmp    eax, 0e3h
        je     NEAR PTR loc_1_546D
        cmp    eax, 0e4h
        je     NEAR PTR loc_1_5494
        cmp    eax, 0e5h
        je     NEAR PTR loc_1_5541
loc_1_52C5:
        cmp    dword ptr [esp + 5ch], 1
        jb     NEAR PTR loc_1_53C6
        cmp    dword ptr [esp + 5ch], 0e1h
        ja     NEAR PTR loc_1_53C6
        mov    ebx, dword ptr [esp + 64h]
        push   ebx
        call   sub_1_F390
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_5341
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_52FB:
        cmp    eax, 0e7h
        jb     SHORT loc_1_52C5
        cmp    eax, 0e8h
        jbe    NEAR PTR loc_1_5637
        cmp    eax, 0e9h
        je     NEAR PTR loc_1_568D
        cmp    eax, 0eah
        je     NEAR PTR loc_1_58B7
        cmp    eax, 0ebh
        je     NEAR PTR loc_1_5A97
        jmp    SHORT loc_1_52C5
loc_1_5330:
        sub    eax, 0edh
        cmp    eax, 12h
        ja     SHORT loc_1_52C5
        db  0FFh, 024h, 085h, 055h, 074h, 000h, 000h
loc_1_5341:
        push   10h
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_535B
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_535B:
        mov    dword ptr [esi + 8], edi
        mov    dword ptr [esi + 4], 5
        mov    dword ptr [esi + 0ch], ebx
        push   0
        push   ebx
        push   edi
        mov    dword ptr [esi], 0
        push   dword ptr [esp + 64h]
        push   12h
        call   sub_1_EF50
        add    esp, 14h
        cmp    ebx, eax
        jne    SHORT loc_1_53BC
        push   dword ptr [esp + 58h]
        call   sub_1_35D1
        add    esp, 4
        mov    dword ptr [eax + 41h], 0
        push   dword ptr [esp + 58h]
        push   ebx
        push   esi
        call   sub_1_99FA
        add    esp, 0ch
        mov    esi, eax
        call   sub_1_8EF7
        push   edi
        call   sub_1_F396
        add    esp, 4
        mov    eax, esi
        jmp    NEAR PTR loc_1_744D
loc_1_53BC:
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_53C6:
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_53D0:
        mov    ebx, dword ptr [esp + 64h]
        push   ebx
        call   sub_1_F390
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_53ED
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_53ED:
        push   10h
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_5407
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_5407:
        mov    dword ptr [esi + 8], edi
        mov    dword ptr [esi + 4], 5
        mov    dword ptr [esi + 0ch], ebx
        push   0
        push   ebx
        push   edi
        mov    dword ptr [esi], 0
        push   dword ptr [esp + 64h]
        push   12h
        call   sub_1_EF50
        add    esp, 14h
        cmp    ebx, eax
        je     SHORT loc_1_543A
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_543A:
        push   dword ptr [esp + 58h]
        call   sub_1_35D1
        add    esp, 4
        mov    dword ptr [eax + 41h], 0
        push   dword ptr [esp + 58h]
        push   ebx
        push   esi
        call   sub_1_C3BC
        add    esp, 0ch
        mov    esi, eax
        push   edi
        call   sub_1_F396
        add    esp, 4
        mov    eax, esi
        jmp    NEAR PTR loc_1_744D
loc_1_546D:
        push   dword ptr [esp + 58h]
        call   sub_1_35D1
        add    esp, 4
        cmp    dword ptr [esp + 64h], 1
        jne    NEAR PTR loc_1_744D
        mov    byte ptr [eax + 30h], 2
        mov    dword ptr [eax + 39h], 0
        jmp    NEAR PTR loc_1_744D
loc_1_5494:
        call   sub_1_A7B0
        mov    esi, eax
        push   0
        mov    dword ptr [esi], 0
        push   3ch
        push   dword ptr [esi + 4]
        mov    dword ptr [esi + 0ch], 3ch
        push   dword ptr [esp + 64h]
        push   12h
        call   sub_1_EF50
        add    esp, 14h
        push   esi
        call   sub_1_A7C2
        add    esp, 4
        push   dword ptr [esp + 58h]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_744B
        mov    byte ptr [eax + 1ah], 0
        mov    dword ptr [eax + 39h], 0
        mov    ecx, dword ptr [eax + 41h]
        or     ecx, ecx
        je     SHORT loc_1_54F0
        dec    ecx
        mov    dword ptr [eax + 41h], ecx
loc_1_54F0:
        db  083h, 03Dh, 058h, 037h, 001h, 000h, 000h
        jne    NEAR PTR loc_1_744B
        sub    bl, bl
        db  0BAh, 0D8h, 038h, 001h, 000h
loc_1_5504:
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     SHORT loc_1_5528
        mov    ecx, 80h
loc_1_550F:
        test   word ptr [eax + 40h], cx
        je     SHORT loc_1_551E
        mov    esi, dword ptr [esp + 58h]
        cmp    dword ptr [eax + 3ch], esi
        je     SHORT loc_1_5526
loc_1_551E:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_550F
        jmp    SHORT loc_1_5528
loc_1_5526:
        mov    bl, 1
loc_1_5528:
        cmp    bl, 1
        je     NEAR PTR loc_1_744B
        add    edx, 4
        db  081h, 0FAh, 018h, 039h, 001h, 000h
        jb     SHORT loc_1_5504
        jmp    NEAR PTR loc_1_744B
loc_1_5541:
        push   dword ptr [esp + 58h]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_55F0
        movzx  ecx, byte ptr [eax + 30h]
        cmp    ecx, 1
        je     SHORT loc_1_5570
        cmp    ecx, 3
        je     NEAR PTR loc_1_744B
        cmp    ecx, 4
        jne    NEAR PTR loc_1_744B
loc_1_5570:
        mov    ebx, dword ptr [esp + 64h]
        cmp    ebx, 1
        je     SHORT loc_1_5596
        cmp    ebx, 2
        je     SHORT loc_1_559F
        cmp    ebx, 3
        je     SHORT loc_1_55AC
        cmp    ebx, 4
        jb     SHORT loc_1_558D
        cmp    ebx, 5
        jbe    SHORT loc_1_55B9
loc_1_558D:
        mov    byte ptr [eax + 30h], 1
        jmp    NEAR PTR loc_1_744B
loc_1_5596:
        mov    byte ptr [eax + 30h], 1
        jmp    NEAR PTR loc_1_744B
loc_1_559F:
        mov    byte ptr [eax + 30h], 2
        mov    dword ptr [eax + 5dh], 0
        jmp    SHORT loc_1_55E4
loc_1_55AC:
        mov    byte ptr [eax + 30h], 3
        mov    dword ptr [eax + 5dh], 0
        jmp    SHORT loc_1_55E4
loc_1_55B9:
        mov    byte ptr [eax + 30h], 1
        mov    byte ptr [eax + 19h], 1
        mov    dword ptr [eax + 41h], 0
        mov    dword ptr [eax + 5dh], 0
        mov    ecx, dword ptr [eax + 45h]
        sub    ecx, eax
        cmp    ecx, 45h
        mov    dword ptr [eax + 39h], 12ch
        jne    NEAR PTR loc_1_744B
loc_1_55E4:
        mov    dword ptr [eax + 39h], 0
        jmp    NEAR PTR loc_1_744B
loc_1_55F0:
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_55FA:
        mov    ebx, dword ptr [esp + 64h]
        mov    eax, dword ptr [ebx + 4]
        mov    al, byte ptr [eax]
        or     al, al
        je     SHORT loc_1_5625
        cmp    al, 1
        jb     SHORT loc_1_561B
        cmp    al, 0e1h
        ja     SHORT loc_1_561B
        push   ebx
        push   dword ptr [esp + 64h]
        call   sub_1_A828
        jmp    SHORT loc_1_562F
loc_1_561B:
        push   ebx
        push   0
        call   sub_1_A828
        jmp    SHORT loc_1_562F
loc_1_5625:
        push   ebx
        push   dword ptr [esp + 64h]
        call   sub_1_BC34
loc_1_562F:
        add    esp, 8
        jmp    NEAR PTR loc_1_744B
loc_1_5637:
        push   dword ptr [esp + 58h]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_5661
        push   dword ptr [esp + 58h]
        call   sub_1_35F7
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_5661
        mov    eax, 2afch
        jmp    NEAR PTR loc_1_744D
loc_1_5661:
        cmp    dword ptr [esp + 5ch], 0e7h
        jne    SHORT loc_1_567C
        mov    byte ptr [eax + 18h], 1
        mov    byte ptr [eax + 30h], 1
        mov    byte ptr [eax + 2bh], 0
        jmp    NEAR PTR loc_1_744B
loc_1_567C:
        mov    byte ptr [eax + 18h], 0
        mov    byte ptr [eax + 30h], 1
        mov    byte ptr [eax + 2bh], 1
        jmp    NEAR PTR loc_1_744B
loc_1_568D:
        lea    eax, [esp + 44h]
        push   eax
        call   sub_1_51D0
        cmp    eax, 720h
        jne    SHORT loc_1_56A0
        sub    eax, eax
loc_1_56A0:
        or     eax, eax
        jne    SHORT loc_1_56B0
        lea    esi, [esp + 44h]
        lea    edi, [esp + 34h]
        movsd
        movsd
        movsd
        movsd
loc_1_56B0:
        push   dword ptr [esp + 58h]
        call   sub_1_35D1
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_56DE
        push   dword ptr [esp + 58h]
        call   sub_1_35F7
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_56EE
        mov    eax, 2afch
        jmp    NEAR PTR loc_1_744D
loc_1_56DE:
        cmp    byte ptr [ebx + 1ah], 1
        jne    SHORT loc_1_56EE
        mov    eax, 2b08h
        jmp    NEAR PTR loc_1_744D
loc_1_56EE:
        cmp    byte ptr [ebx + 2bh], 0
        je     NEAR PTR loc_1_58AD
        mov    byte ptr [ebx + 18h], 1
        mov    byte ptr [ebx + 1ah], 1
        lea    esi, [esp + 34h]
        lea    edi, [ebx + 1bh]
        movsd
        movsd
        movsd
        movsd
        call   sub_1_A7B0
        mov    dword ptr [esp + 14h], eax
        mov    ebp, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 0ch], 3ch
        lea    esi, [esp + 34h]
        mov    byte ptr [ebp], 0eah
        lea    edi, [ebp + 3]
        movsd
        movsd
        movsd
        movsd
        lea    esi, [esp + 34h]
        lea    edi, [ebp + 13h]
        movsd
        movsd
        movsd
        movsd
        mov    ecx, dword ptr [esp + 60h]
        mov    edx, ecx
        shl    edx, 10h
        mov    eax, ecx
        and    eax, 0ff00h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        shr    eax, 10h
        and    ecx, 0ff0000h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebp + 23h], edx
        movzx  ecx, byte ptr [ebp + 6]
        movzx  eax, byte ptr [ebp + 17h]
        add    ecx, eax
        push   dword ptr [esp + 14h]
        movzx  eax, byte ptr [ebp + 14h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 15h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 4]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 5]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 16h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 7]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 18h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 8]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 19h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 9]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 1ah]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0ah]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 1bh]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0bh]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 1ch]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0ch]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 1dh]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0dh]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 1eh]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0eh]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 1fh]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0fh]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 20h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 21h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 10h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 11h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 22h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 12h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 13h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 3]
        add    ecx, eax
        add    ecx, 8c00h
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   0
        push   0
        push   3ch
        mov    word ptr [ebp + 37h], ax
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 68h]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        cmp    eax, 3ch
        je     SHORT loc_1_5895
        push   2
        push   0
        push   dword ptr [esp + 64h]
        db  068h, 0A1h, 074h, 000h, 000h
        push   0
        push   3ch
        push   dword ptr [esp + 2ch]
        push   ebx
        push   dword ptr [esp + 78h]
        call   sub_1_3A0F
        add    esp, 24h
        mov    eax, dword ptr [ebx + 55h]
        mov    dword ptr [ebx + 4dh], eax
        mov    dword ptr [ebx + 51h], 0
        mov    byte ptr [ebx + 1ah], 0
        jmp    NEAR PTR loc_1_744B
loc_1_5895:
        cmp    dword ptr [ebx + 39h], 0
        jne    NEAR PTR loc_1_744B
        mov    eax, dword ptr [ebx + 31h]
        mov    dword ptr [ebx + 39h], eax
        mov    dword ptr [ebx + 59h], eax
        jmp    NEAR PTR loc_1_744B
loc_1_58AD:
        mov    eax, 2afch
        jmp    NEAR PTR loc_1_744D
loc_1_58B7:
        call   sub_1_A7B0
        mov    dword ptr [esp + 14h], eax
        mov    ebx, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        push   0
        push   3ch
        push   ebx
        mov    dword ptr [eax + 0ch], 3ch
        push   dword ptr [esp + 64h]
        push   12h
        call   sub_1_EF50
        add    esp, 14h
        cmp    eax, 3ch
        je     SHORT loc_1_58FE
        push   dword ptr [esp + 14h]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_58FE:
        push   dword ptr [esp + 58h]
        call   sub_1_35D1
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_5946
        push   dword ptr [esp + 58h]
        call   sub_1_35F7
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_592C
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_592C:
        or     word ptr [ebx + 33h], 4000h
        lea    esi, [ebx + 3]
        lea    edi, [ebp + 1bh]
        movsd
        movsd
        movsd
        movsd
        mov    byte ptr [ebp + 18h], 1
        mov    byte ptr [ebp + 1ah], 1
        jmp    SHORT loc_1_5977
loc_1_5946:
        cmp    byte ptr [ebp + 2bh], 0
        jne    SHORT loc_1_5954
        and    word ptr [ebx + 33h], 0bfffh
        jmp    SHORT loc_1_5977
loc_1_5954:
        or     word ptr [ebx + 33h], 4000h
        mov    byte ptr [ebp + 1ah], 1
        mov    byte ptr [ebp + 18h], 1
        mov    eax, dword ptr [ebp + 41h]
        or     eax, eax
        je     SHORT loc_1_596D
        dec    eax
        mov    dword ptr [ebp + 41h], eax
loc_1_596D:
        lea    esi, [ebx + 3]
        lea    edi, [ebp + 1bh]
        movsd
        movsd
        movsd
        movsd
loc_1_5977:
        mov    byte ptr [ebx], 0ebh
        movzx  ecx, byte ptr [ebx + 6]
        movzx  eax, byte ptr [ebx + 17h]
        add    ecx, eax
        push   dword ptr [esp + 14h]
        movzx  eax, byte ptr [ebx + 13h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 14h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 3]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 15h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 4]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 5]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 16h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 7]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 18h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 8]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 19h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 9]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 1ah]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0ah]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 1bh]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0bh]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 1ch]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0ch]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 1dh]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0dh]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 1eh]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0eh]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 1fh]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0fh]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 20h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 21h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 10h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 11h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 22h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 12h]
        add    ecx, eax
        add    ecx, 8c00h
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   0
        push   0
        push   3ch
        mov    word ptr [ebx + 37h], ax
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 68h]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        cmp    eax, 3ch
        je     NEAR PTR loc_1_744B
        push   2
        push   0
        push   dword ptr [esp + 64h]
        push   0
        push   0
        push   3ch
        push   dword ptr [esp + 2ch]
        push   ebp
        push   dword ptr [esp + 78h]
        call   sub_1_3A0F
        add    esp, 24h
        jmp    NEAR PTR loc_1_744B
loc_1_5A97:
        call   sub_1_A7B0
        mov    edi, eax
        push   0
        mov    esi, dword ptr [edi + 4]
        mov    dword ptr [edi], 0
        push   3ch
        push   esi
        mov    dword ptr [edi + 0ch], 3ch
        push   dword ptr [esp + 64h]
        push   12h
        call   sub_1_EF50
        add    esp, 14h
        cmp    eax, 3ch
        je     SHORT loc_1_5AD9
        push   edi
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_5AD9:
        push   dword ptr [esp + 58h]
        call   sub_1_35D1
        add    esp, 4
        mov    ecx, dword ptr [eax + 41h]
        or     ecx, ecx
        je     SHORT loc_1_5AF0
        dec    ecx
        mov    dword ptr [eax + 41h], ecx
loc_1_5AF0:
        mov    dword ptr [eax + 4dh], 0
        test   byte ptr [esi + 34h], 40h
        push   edi
        je     SHORT loc_1_5B0B
        call   sub_1_A7C2
        add    esp, 4
        jmp    NEAR PTR loc_1_744B
loc_1_5B0B:
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_5B1D:
        push   dword ptr [esp + 58h]
        call   sub_1_35D1
        mov    dword ptr [esp + 1ch], eax
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_5B3B
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_5B3B:
        mov    ebx, dword ptr [esp + 64h]
        push   ebx
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_5B56
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_5B56:
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [esp + 2ch], ecx
        mov    edx, dword ptr [eax]
        mov    dword ptr [esp + 1ch], edx
        call   sub_1_A7B0
        mov    dword ptr [esp + 14h], eax
        or     eax, eax
        je     NEAR PTR loc_1_744D
        mov    ebp, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        mov    edx, dword ptr [esp + 1ch]
        mov    dword ptr [eax + 0ch], 3ch
        mov    ecx, dword ptr [edx + 10h]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        mov    eax, ecx
        shr    eax, 18h
        or     edx, eax
        mov    dword ptr [ebp + 2bh], edx
        mov    edx, dword ptr [esp + 1ch]
        inc    ecx
        mov    dword ptr [edx + 10h], ecx
        mov    eax, ecx
        sub    eax, dword ptr [edx + 14h]
        or     eax, eax
        jle    SHORT loc_1_5BC7
        mov    dword ptr [edx + 14h], ecx
loc_1_5BC7:
        mov    edx, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [edx + 48h]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    eax, 8
        or     edx, eax
        shr    ecx, 18h
        or     edx, ecx
        mov    dword ptr [ebp + 2fh], edx
        mov    edx, dword ptr [esp + 1ch]
        cmp    byte ptr [edx + 0bch], 3
        mov    word ptr [ebp + 33h], 12f0h
        je     SHORT loc_1_5C0D
        mov    word ptr [ebp + 33h], 2f0h
loc_1_5C0D:
        mov    edx, dword ptr [esp + 1ch]
        mov    esi, edx
        mov    ax, word ptr [edx + 4ch]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebp + 35h], cx
        mov    dword ptr [edx + 94h], ebx
        add    esi, 6ch
        lea    edi, [ebp + 3]
        movsd
        movsd
        movsd
        movsd
        lea    eax, [ebp + 13h]
        mov    esi, dword ptr [esp + 1ch]
        add    esi, 7ch
        mov    edi, eax
        movsd
        movsd
        movsd
        movsd
        mov    edx, ebx
        mov    ecx, dword ptr [esp + 2ch]
        mov    byte ptr [ebp], 0edh
        mov    al, byte ptr [ecx + 42h]
        and    edx, 0ff00h
        mov    byte ptr [ebp + 2], al
        mov    eax, ebx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ebx
        and    eax, 0ff0000h
        mov    ecx, ebx
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebp + 23h], edx
        push   dword ptr [esp + 18h]
        call   sub_1_3ADE
        mov    byte ptr [esp + 17h], al
        add    esp, 4
        or     al, al
        jne    SHORT loc_1_5C9F
        push   dword ptr [esp + 14h]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_5C9F:
        push   ebx
        push   dword ptr [esp + 17h]
        push   dword ptr [esp + 20h]
        call   sub_1_3A84
        mov    edx, dword ptr [esp + 28h]
        mov    al, byte ptr [esp + 1fh]
        add    esp, 0ch
        mov    byte ptr [edx + 98h], al
        mov    byte ptr [ebp + 1], al
        mov    byte ptr [ebp + 3bh], 0
        movzx  ecx, byte ptr [ebp + 6]
        movzx  eax, byte ptr [ebp + 17h]
        add    ecx, eax
        push   dword ptr [esp + 14h]
        movzx  eax, byte ptr [ebp + 14h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 15h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 4]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 5]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 16h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 7]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 18h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 8]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 19h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 9]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 1ah]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0ah]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 1bh]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0bh]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 1ch]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0ch]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 1dh]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0dh]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 1eh]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0eh]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 1fh]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 0fh]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 20h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 21h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 10h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 11h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 22h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 12h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 13h]
        add    ecx, eax
        movzx  eax, byte ptr [ebp + 3]
        add    ecx, eax
        add    ecx, 8c00h
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        mov    edx, dword ptr [esp + 1ch]
        mov    word ptr [ebp + 37h], ax
        mov    eax, dword ptr [edx + 0a2h]
        push   2
        push   0
        push   3ch
        mov    dword ptr [edx + 0c8h], eax
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 68h]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        cmp    eax, 3ch
        je     SHORT loc_1_5DEC
        push   2
        push   2
        push   dword ptr [esp + 64h]
        push   0
        push   0
        push   3ch
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 78h]
        call   sub_1_3A0F
        add    esp, 24h
loc_1_5DEC:
        mov    eax, 2b0ah
        jmp    NEAR PTR loc_1_744D
loc_1_5DF6:
        call   sub_1_A7B0
        mov    ebp, eax
        push   0
        mov    esi, dword ptr [ebp + 4]
        mov    dword ptr [ebp], 0
        push   3ch
        push   esi
        mov    dword ptr [ebp + 0ch], 3ch
        push   dword ptr [esp + 64h]
        push   12h
        call   sub_1_EF50
        mov    dword ptr [esp + 3ch], eax
        add    esp, 14h
        cmp    eax, 3ch
        je     SHORT loc_1_5E3D
        push   ebp
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_5E3D:
        push   dword ptr [esp + 58h]
        call   sub_1_35D1
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_5E5D
        push   dword ptr [esp + 58h]
        call   sub_1_35F7
        add    esp, 4
        mov    edi, eax
loc_1_5E5D:
        mov    eax, dword ptr [edi + 41h]
        or     eax, eax
        je     SHORT loc_1_5E68
        dec    eax
        mov    dword ptr [edi + 41h], eax
loc_1_5E68:
        lea    eax, [esp + 20h]
        push   eax
        mov    al, byte ptr [esi + 1]
        push   eax
        push   edi
        call   sub_1_3AAC
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_601E
        lea    eax, [esp + 20h]
        push   eax
        mov    al, byte ptr [esi + 1]
        push   eax
        push   edi
        call   sub_1_3AAC
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_5EA2
        cmp    dword ptr [esp + 20h], edi
        je     NEAR PTR loc_1_601E
loc_1_5EA2:
        or     word ptr [esi + 33h], 8000h
        push   edi
        call   sub_1_3ADE
        add    esp, 4
        mov    bl, al
        or     bl, bl
        je     SHORT loc_1_5ED2
        push   edi
        push   ebx
        push   edi
        call   sub_1_3A84
        add    esp, 0ch
        mov    byte ptr [esi + 1], bl
        mov    ebx, 2
        or     word ptr [esi + 33h], 4000h
        jmp    SHORT loc_1_5EDD
loc_1_5ED2:
        mov    ebx, 2
        and    word ptr [esi + 33h], 0bfffh
loc_1_5EDD:
        mov    byte ptr [esi], 0eeh
        mov    byte ptr [esi + 3bh], 0fah
        mov    ecx, edi
        and    ecx, 0ff00h
        mov    eax, edi
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        mov    eax, edi
        and    eax, 0ff0000h
        mov    edx, edi
        shr    edx, 10h
        or     eax, edx
        shr    eax, 8
        or     ecx, eax
        mov    dword ptr [esi + 27h], ecx
        push   ebp
        movzx  eax, byte ptr [esi + 17h]
        movzx  ecx, byte ptr [esi + 6]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 13h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 14h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 15h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 16h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 18h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 19h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 20h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 21h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 10h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 11h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 22h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 12h]
        add    ecx, eax
        add    ecx, 8c00h
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   ebx
        push   0
        push   3ch
        push   ebp
        mov    word ptr [esi + 37h], ax
        push   dword ptr [esp + 68h]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        cmp    eax, 3ch
        je     NEAR PTR loc_1_744B
        push   2
        push   ebx
        push   dword ptr [esp + 64h]
        push   0
        push   0
        push   3ch
        push   ebp
        push   edi
        push   dword ptr [esp + 78h]
        call   sub_1_3A0F
        add    esp, 24h
        jmp    NEAR PTR loc_1_744B
loc_1_601E:
        mov    ebx, edi
        cmp    dword ptr [esp + 20h], ebx
        je     SHORT loc_1_6034
        push   ebx
        mov    al, byte ptr [esi + 1]
        push   eax
        push   edi
        call   sub_1_3A84
        add    esp, 0ch
loc_1_6034:
        mov    byte ptr [esi], 0eeh
        or     word ptr [esi + 33h], 4000h
        mov    byte ptr [esi + 3bh], 0fah
        mov    ecx, ebx
        and    ecx, 0ff00h
        mov    eax, ebx
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        mov    eax, ebx
        and    eax, 0ff0000h
        shr    ebx, 10h
        or     eax, ebx
        shr    eax, 8
        or     ecx, eax
        mov    dword ptr [esi + 27h], ecx
        push   ebp
        inc    dword ptr [edi + 14h]
        movzx  ecx, byte ptr [esi + 6]
        movzx  eax, byte ptr [esi + 17h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 13h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 14h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 15h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 16h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 18h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 19h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 20h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 21h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 10h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 11h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 22h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 12h]
        add    ecx, eax
        add    ecx, 8c00h
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   0
        push   0
        push   3ch
        push   ebp
        mov    word ptr [esi + 37h], ax
        push   dword ptr [esp + 68h]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        cmp    eax, 3ch
        je     SHORT loc_1_6175
        push   2
        push   0
        push   dword ptr [esp + 64h]
        push   0
        push   0
        push   3ch
        push   ebp
        push   edi
        push   dword ptr [esp + 78h]
        call   sub_1_3A0F
        add    esp, 24h
loc_1_6175:
        push   10h
        call   sub_1_F390
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_618F
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_618F:
        mov    dword ptr [edi + 8], esi
        mov    dword ptr [edi + 4], 5
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [edi + 0ch], eax
        push   dword ptr [esp + 58h]
        mov    dword ptr [edi], 0
        push   dword ptr [esp + 2ch]
        push   edi
        call   sub_1_99FA
        add    esp, 0ch
        mov    esi, eax
        push   edi
        call   sub_1_F396
        add    esp, 4
        mov    eax, esi
        jmp    NEAR PTR loc_1_744D
loc_1_61C9:
        call   sub_1_A7B0
        mov    dword ptr [esp + 14h], eax
        mov    esi, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        push   0
        push   3ch
        push   esi
        mov    dword ptr [eax + 0ch], 3ch
        push   dword ptr [esp + 64h]
        push   12h
        call   sub_1_EF50
        add    esp, 14h
        cmp    eax, 3ch
        je     SHORT loc_1_6210
        push   dword ptr [esp + 14h]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_6210:
        mov    ecx, dword ptr [esi + 23h]
        mov    ebp, ecx
        and    ebp, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     ebp, eax
        shl    ebp, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     ebp, eax
        push   ebp
        call   sub_1_4777
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_625B
        push   dword ptr [esp + 14h]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_625B:
        mov    edi, dword ptr [ebx]
        push   dword ptr [esp + 58h]
        call   sub_1_35D1
        mov    dword ptr [esp + 1ch], eax
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_6287
        push   dword ptr [esp + 14h]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_6287:
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [eax + 41h]
        or     ecx, ecx
        je     SHORT loc_1_6296
        dec    ecx
        mov    dword ptr [eax + 41h], ecx
loc_1_6296:
        mov    ax, word ptr [esi + 33h]
        test   ah, 40h
        je     NEAR PTR loc_1_65C7
        test   ah, 80h
        je     NEAR PTR loc_1_6597
        mov    ecx, dword ptr [esi + 27h]
        mov    edx, ecx
        and    edx, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [edi + 9ah], edx
        lea    eax, [esp + 20h]
        push   eax
        mov    al, byte ptr [esi + 1]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_1_3AAC
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_630A
        mov    bl, byte ptr [esi + 1]
        push   ebp
        push   ebx
        push   dword ptr [esp + 20h]
        call   sub_1_3A84
        add    esp, 0ch
        and    word ptr [esi + 33h], 7fffh
        jmp    SHORT loc_1_6318
loc_1_630A:
        push   dword ptr [esp + 18h]
        call   sub_1_3ADE
        add    esp, 4
        mov    bl, al
loc_1_6318:
        mov    al, byte ptr [edi + 98h]
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_1_3B6C
        add    esp, 8
        or     bl, bl
        je     NEAR PTR loc_1_6463
        mov    byte ptr [esi], 0fbh
        mov    byte ptr [esi + 1], bl
        mov    byte ptr [edi + 98h], bl
        mov    byte ptr [esi + 3bh], 0
        movzx  ecx, byte ptr [esi + 6]
        movzx  eax, byte ptr [esi + 17h]
        add    ecx, eax
        push   dword ptr [esp + 14h]
        movzx  eax, byte ptr [esi + 13h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 14h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 15h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 16h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 18h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 19h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 20h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 21h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 10h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 11h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 22h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 12h]
        add    ecx, eax
        add    ecx, 8c00h
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   2
        push   0
        push   3ch
        mov    word ptr [esi + 37h], ax
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 68h]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        cmp    eax, 3ch
        je     NEAR PTR loc_1_744B
        push   2
        push   2
        push   dword ptr [esp + 64h]
        push   0
        push   0
        push   3ch
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 78h]
        call   sub_1_3A0F
        add    esp, 24h
        jmp    NEAR PTR loc_1_744B
loc_1_6463:
        and    word ptr [esi + 33h], 0bfffh
        mov    byte ptr [esi], 0fbh
        mov    byte ptr [esi + 3bh], 0
        movzx  ecx, byte ptr [esi + 6]
        movzx  eax, byte ptr [esi + 17h]
        add    ecx, eax
        push   dword ptr [esp + 14h]
        movzx  eax, byte ptr [esi + 13h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 14h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 15h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 16h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 18h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 19h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 20h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 21h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 10h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 11h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 22h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 12h]
        add    ecx, eax
        add    ecx, 8c00h
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   0
        push   0
        push   3ch
        mov    word ptr [esi + 37h], ax
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 68h]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        cmp    eax, 3ch
        je     SHORT loc_1_6587
        push   2
        push   0
        push   dword ptr [esp + 64h]
        push   0
        push   0
        push   3ch
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 78h]
        call   sub_1_3A0F
        add    esp, 24h
loc_1_6587:
        push   40h
        push   edi
        call   sub_1_4B72
        add    esp, 8
        jmp    NEAR PTR loc_1_744B
loc_1_6597:
        mov    eax, dword ptr [esp + 18h]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebx + 4]
        mov    ecx, dword ptr [eax + 38h]
        cmp    ecx, -1
        je     SHORT loc_1_65AF
        cmp    dword ptr [esp + 58h], ecx
        je     SHORT loc_1_65B6
loc_1_65AF:
        mov    esi, dword ptr [esp + 58h]
        mov    dword ptr [eax + 3ch], esi
loc_1_65B6:
        push   dword ptr [esp + 14h]
        call   sub_1_A7C2
        add    esp, 4
        jmp    NEAR PTR loc_1_744B
loc_1_65C7:
        mov    al, byte ptr [edi + 98h]
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_1_3B6C
        add    esp, 8
        push   10h
        push   edi
        call   sub_1_4B72
        add    esp, 8
        push   dword ptr [esp + 14h]
        call   sub_1_A7C2
        add    esp, 4
        jmp    NEAR PTR loc_1_744B
loc_1_65F6:
        call   sub_1_A7B0
        mov    dword ptr [esp + 14h], eax
        mov    esi, dword ptr [eax + 4]
        mov    dword ptr [eax], 0
        push   0
        push   3ch
        push   esi
        mov    dword ptr [eax + 0ch], 3ch
        push   dword ptr [esp + 64h]
        push   12h
        call   sub_1_EF50
        add    esp, 14h
        mov    edi, eax
        cmp    edi, 3ch
        je     SHORT loc_1_663F
        push   dword ptr [esp + 14h]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_663F:
        test   byte ptr [esi + 34h], 40h
        jne    SHORT loc_1_665B
        push   dword ptr [esp + 14h]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_665B:
        push   dword ptr [esp + 58h]
        call   sub_1_35D1
        mov    dword ptr [esp + 1ch], eax
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_6685
        push   dword ptr [esp + 14h]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_6685:
        mov    ecx, dword ptr [esp + 18h]
        mov    eax, dword ptr [ecx + 41h]
        or     eax, eax
        je     SHORT loc_1_6694
        dec    eax
        mov    dword ptr [ecx + 41h], eax
loc_1_6694:
        cmp    byte ptr [esi + 3bh], 0
        jne    NEAR PTR loc_1_6A56
        test   byte ptr [esi + 34h], 80h
        jne    SHORT loc_1_6700
        mov    ecx, dword ptr [esp + 18h]
        push   10h
        inc    dword ptr [ecx + 14h]
        call   sub_1_F390
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_66C5
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_66C5:
        mov    dword ptr [ebx + 8], esi
        mov    dword ptr [ebx + 4], 5
        mov    dword ptr [ebx + 0ch], edi
        mov    dword ptr [ebx], 0
        push   dword ptr [esp + 58h]
        push   edi
        push   ebx
        call   sub_1_99FA
        add    esp, 0ch
        push   ebx
        call   sub_1_F396
        add    esp, 4
        push   dword ptr [esp + 14h]
        call   sub_1_A7C2
        add    esp, 4
        jmp    NEAR PTR loc_1_744B
loc_1_6700:
        lea    eax, [esp + 20h]
        push   eax
        mov    al, byte ptr [esi + 1]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_1_3AAC
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_68AD
        lea    eax, [esp + 20h]
        push   eax
        mov    al, byte ptr [esi + 1]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_1_3AAC
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_6744
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [esp + 20h], eax
        je     NEAR PTR loc_1_68AD
loc_1_6744:
        or     word ptr [esi + 33h], 8000h
        push   dword ptr [esp + 18h]
        call   sub_1_3ADE
        add    esp, 4
        mov    bl, al
        or     bl, bl
        jne    SHORT loc_1_6769
        mov    edi, 2
        and    word ptr [esi + 33h], 0bfffh
        jmp    SHORT loc_1_6788
loc_1_6769:
        push   dword ptr [esp + 18h]
        push   ebx
        mov    edi, 2
        push   dword ptr [esp + 20h]
        call   sub_1_3A84
        add    esp, 0ch
        mov    byte ptr [esi + 1], bl
        or     word ptr [esi + 33h], 4000h
loc_1_6788:
        mov    byte ptr [esi], 0fbh
        mov    byte ptr [esi + 3bh], 0fah
        movzx  ecx, byte ptr [esi + 6]
        movzx  eax, byte ptr [esi + 17h]
        add    ecx, eax
        push   dword ptr [esp + 14h]
        movzx  eax, byte ptr [esi + 13h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 14h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 15h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 16h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 18h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 19h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 20h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 21h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 10h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 11h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 22h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 12h]
        add    ecx, eax
        add    ecx, 8c00h
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   edi
        push   0
        push   3ch
        mov    word ptr [esi + 37h], ax
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 68h]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        cmp    eax, 3ch
        je     NEAR PTR loc_1_744B
        push   2
        push   edi
        push   dword ptr [esp + 64h]
        push   0
        push   0
        push   3ch
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 78h]
        call   sub_1_3A0F
        add    esp, 24h
        jmp    NEAR PTR loc_1_744B
loc_1_68AD:
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [esp + 20h], eax
        je     SHORT loc_1_68C8
        push   eax
        mov    al, byte ptr [esi + 1]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_1_3A84
        add    esp, 0ch
loc_1_68C8:
        mov    ecx, dword ptr [esp + 18h]
        inc    dword ptr [ecx + 14h]
        mov    ax, word ptr [esi + 33h]
        push   dword ptr [esp + 14h]
        and    ax, 7fffh
        movzx  ecx, byte ptr [esi + 6]
        mov    word ptr [esi + 33h], ax
        or     ax, 4000h
        mov    word ptr [esi + 33h], ax
        mov    byte ptr [esi], 0fbh
        mov    byte ptr [esi + 3bh], 0fah
        movzx  eax, byte ptr [esi + 17h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 13h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 14h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 15h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 16h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 18h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 19h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 20h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 21h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 10h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 11h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 22h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 12h]
        add    ecx, eax
        add    ecx, 8c00h
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   0
        push   0
        push   3ch
        mov    word ptr [esi + 37h], ax
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 68h]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        cmp    eax, 3ch
        je     SHORT loc_1_6A01
        push   2
        push   0
        push   dword ptr [esp + 64h]
        push   0
        push   0
        push   3ch
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 78h]
        call   sub_1_3A0F
        add    esp, 24h
loc_1_6A01:
        push   10h
        call   sub_1_F390
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_6A1B
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_6A1B:
        mov    dword ptr [ebx + 8], esi
        mov    dword ptr [ebx + 4], 5
        mov    dword ptr [ebx + 0ch], edi
        mov    dword ptr [ebx], 0
        push   dword ptr [esp + 58h]
        push   edi
        push   ebx
        call   sub_1_99FA
        add    esp, 0ch
        push   ebx
        call   sub_1_F396
        add    esp, 4
        push   dword ptr [esp + 14h]
        call   sub_1_A7C2
        add    esp, 4
        jmp    NEAR PTR loc_1_744B
loc_1_6A56:
        mov    ecx, dword ptr [esi + 23h]
        mov    edi, ecx
        and    edi, 0ff00h
        mov    eax, ecx
        shl    eax, 10h
        or     edi, eax
        shl    edi, 8
        mov    eax, ecx
        and    eax, 0ff0000h
        shr    ecx, 10h
        or     eax, ecx
        shr    eax, 8
        or     edi, eax
        push   edi
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_6A9F
        push   dword ptr [esp + 14h]
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_6A9F:
        mov    ebp, dword ptr [eax]
        test   byte ptr [esi + 34h], 80h
        jne    SHORT loc_1_6ABF
        mov    ecx, dword ptr [esp + 18h]
        push   dword ptr [esp + 14h]
        inc    dword ptr [ecx + 14h]
        call   sub_1_A7C2
        add    esp, 4
        jmp    NEAR PTR loc_1_744B
loc_1_6ABF:
        lea    eax, [esp + 20h]
        push   eax
        mov    al, byte ptr [esi + 1]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_1_3AAC
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_6C73
        lea    eax, [esp + 20h]
        push   eax
        mov    al, byte ptr [esi + 1]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_1_3AAC
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_6AFF
        cmp    dword ptr [esp + 20h], edi
        je     NEAR PTR loc_1_6C8A
loc_1_6AFF:
        or     word ptr [esi + 33h], 8000h
        push   dword ptr [esp + 18h]
        call   sub_1_3ADE
        add    esp, 4
        mov    bl, al
        or     bl, bl
        jne    SHORT loc_1_6B1F
        and    word ptr [esi + 33h], 0bfffh
        jmp    SHORT loc_1_6B36
loc_1_6B1F:
        push   edi
        push   ebx
        push   dword ptr [esp + 20h]
        call   sub_1_3A84
        add    esp, 0ch
        mov    byte ptr [esi + 1], bl
        or     word ptr [esi + 33h], 4000h
loc_1_6B36:
        mov    al, byte ptr [ebp + 98h]
        push   eax
        mov    edi, 2
        push   dword ptr [esp + 1ch]
        call   sub_1_3B6C
        add    esp, 8
        mov    byte ptr [esi], 0fbh
        mov    byte ptr [esi + 3bh], 0
        movzx  ecx, byte ptr [esi + 6]
        movzx  eax, byte ptr [esi + 17h]
        add    ecx, eax
        push   dword ptr [esp + 14h]
        movzx  eax, byte ptr [esi + 13h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 14h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 15h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 16h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 18h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 19h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 20h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 21h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 10h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 11h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 22h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 12h]
        add    ecx, eax
        add    ecx, 8c00h
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   edi
        push   0
        push   3ch
        mov    word ptr [esi + 37h], ax
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 68h]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        cmp    eax, 3ch
        je     NEAR PTR loc_1_744B
        push   2
        push   edi
        push   dword ptr [esp + 64h]
        push   0
        push   0
        push   3ch
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 78h]
        call   sub_1_3A0F
        add    esp, 24h
        jmp    NEAR PTR loc_1_744B
loc_1_6C73:
        cmp    dword ptr [esp + 20h], edi
        je     SHORT loc_1_6C8A
        push   edi
        mov    al, byte ptr [esi + 1]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_1_3A84
        add    esp, 0ch
loc_1_6C8A:
        mov    ecx, dword ptr [esp + 18h]
        inc    dword ptr [ecx + 14h]
        mov    al, byte ptr [ebp + 98h]
        push   eax
        push   dword ptr [esp + 1ch]
        call   sub_1_3B6C
        add    esp, 8
        mov    al, byte ptr [esi + 1]
        mov    byte ptr [ebp + 98h], al
        mov    ax, word ptr [esi + 33h]
        push   dword ptr [esp + 14h]
        and    ax, 7fffh
        movzx  ecx, byte ptr [esi + 6]
        mov    word ptr [esi + 33h], ax
        or     ax, 4000h
        mov    word ptr [esi + 33h], ax
        mov    byte ptr [esi], 0fbh
        mov    byte ptr [esi + 3bh], 0
        movzx  eax, byte ptr [esi + 17h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 13h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 14h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 15h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 16h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 18h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 19h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 20h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 21h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 10h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 11h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 22h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 12h]
        add    ecx, eax
        add    ecx, 8c00h
        push   ecx
        call   sub_1_4380
        add    esp, 8
        movzx  ecx, byte ptr [esi + 6]
        not    ax
        push   dword ptr [esp + 14h]
        mov    word ptr [esi + 37h], ax
        movzx  eax, byte ptr [esi + 17h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 13h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 14h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 15h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 16h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 18h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 19h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ah]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0bh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0ch]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0dh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0eh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 0fh]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 20h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 21h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 10h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 11h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 22h]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 12h]
        add    ecx, eax
        add    ecx, 8c00h
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   2
        push   0
        push   3ch
        mov    word ptr [esi + 37h], ax
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 68h]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        cmp    eax, 3ch
        je     NEAR PTR loc_1_744B
        push   2
        push   2
        push   dword ptr [esp + 64h]
        push   0
        push   0
        push   3ch
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 78h]
        call   sub_1_3A0F
        add    esp, 24h
        jmp    NEAR PTR loc_1_744B
loc_1_6EC0:
        push   dword ptr [esp + 58h]
        call   sub_1_35D1
        add    esp, 4
        mov    esi, eax
        push   dword ptr [esp + 58h]
        call   sub_1_EE10
        add    esp, 4
        mov    ebp, eax
        or     esi, esi
        je     NEAR PTR loc_1_744B
        cmp    byte ptr [esi + 1ah], 1
        jne    NEAR PTR loc_1_744B
        call   sub_1_A7B0
        mov    edi, eax
        push   edi
        mov    ebx, dword ptr [edi + 4]
        mov    dword ptr [edi], 0
        mov    dword ptr [edi + 0ch], 3ch
        mov    byte ptr [ebx], 0fdh
        movzx  eax, byte ptr [ebx + 17h]
        movzx  ecx, byte ptr [ebx + 6]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 13h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 14h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 3]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 15h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 4]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 5]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 16h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 7]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 18h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 8]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 19h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 9]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 1ah]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0ah]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 1bh]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0bh]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 1ch]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0ch]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 1dh]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0dh]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 1eh]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0eh]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 1fh]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 0fh]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 20h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 21h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 10h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 11h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 22h]
        add    ecx, eax
        movzx  eax, byte ptr [ebx + 12h]
        add    ecx, eax
        add    ecx, 8c00h
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        mov    word ptr [ebx + 37h], ax
        or     ebp, ebp
        je     NEAR PTR loc_1_744B
        mov    ecx, dword ptr [ebp + 14h]
        or     ecx, ecx
        je     NEAR PTR loc_1_744B
        lea    eax, [esp + 30h]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        push   ecx
        call   sub_1_10879
        mov    dword ptr [esp + 30h], eax
        add    esp, 0ch
        cmp    eax, 1
        jne    SHORT loc_1_7040
        push   2
        push   0
        push   3ch
        push   edi
        push   dword ptr [esp + 68h]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        mov    byte ptr [esi + 1ah], 0
        mov    dword ptr [esi + 39h], 0
        mov    eax, dword ptr [esi + 41h]
        or     eax, eax
        je     NEAR PTR loc_1_744B
        dec    eax
        mov    dword ptr [esi + 41h], eax
        jmp    NEAR PTR loc_1_744B
loc_1_7040:
        cmp    dword ptr [esp + 24h], 0
        mov    byte ptr [esi + 1ah], 0
        mov    dword ptr [esi + 39h], 0
        mov    eax, dword ptr [esi + 41h]
        jne    SHORT loc_1_708F
        or     eax, eax
        je     SHORT loc_1_705D
        dec    eax
        mov    dword ptr [esi + 41h], eax
loc_1_705D:
        db  083h, 03Dh, 058h, 037h, 001h, 000h, 000h
        jne    NEAR PTR loc_1_744B
        push   dword ptr [esp + 58h]
        call   sub_1_5230
        add    esp, 4
        or     eax, eax
        jne    NEAR PTR loc_1_744B
        push   1
        push   12h
        call   sub_1_F02D
        add    esp, 8
        jmp    NEAR PTR loc_1_744B
loc_1_708F:
        or     eax, eax
        je     NEAR PTR loc_1_744B
        dec    eax
        mov    dword ptr [esi + 41h], eax
        jmp    NEAR PTR loc_1_744B
loc_1_70A0:
        call   sub_1_A7B0
        mov    esi, eax
        push   0
        mov    dword ptr [esi], 0
        push   3ch
        push   dword ptr [esi + 4]
        mov    dword ptr [esi + 0ch], 3ch
        push   dword ptr [esp + 64h]
        push   12h
        call   sub_1_EF50
        add    esp, 14h
        push   dword ptr [esp + 58h]
        call   sub_1_35D1
        add    esp, 4
        push   esi
        call   sub_1_A7C2
        add    esp, 4
        jmp    NEAR PTR loc_1_744B
loc_1_70E3:
        call   sub_1_A7B0
        mov    esi, eax
        push   0
        mov    edi, dword ptr [esi + 4]
        mov    dword ptr [esi], 0
        push   3ch
        push   edi
        mov    dword ptr [esi + 0ch], 3ch
        push   dword ptr [esp + 64h]
        push   12h
        call   sub_1_EF50
        add    esp, 14h
        push   dword ptr [esp + 58h]
        call   sub_1_35D1
        add    esp, 4
        mov    ecx, dword ptr [eax + 3dh]
        or     ecx, ecx
        je     SHORT loc_1_7124
        dec    ecx
        mov    dword ptr [eax + 3dh], ecx
loc_1_7124:
        test   byte ptr [edi + 34h], 80h
        je     SHORT loc_1_7168
        mov    cl, byte ptr [eax + 30h]
        cmp    cl, 1
        je     SHORT loc_1_7137
        cmp    cl, 4
        jne    SHORT loc_1_7168
loc_1_7137:
        cmp    byte ptr [eax + 19h], 1
        je     SHORT loc_1_7168
        mov    dword ptr [eax + 39h], 0
        mov    ecx, dword ptr [eax + 35h]
        mov    dword ptr [eax + 39h], ecx
        mov    dword ptr [eax + 59h], ecx
        mov    dword ptr [eax + 5dh], 0
        db  08Bh, 00Dh, 0ECh, 037h, 001h, 000h
        mov    dword ptr [eax + 61h], ecx
        db  08Bh, 00Dh, 088h, 037h, 001h, 000h
        mov    dword ptr [eax + 65h], ecx
        jmp    SHORT loc_1_71AA
loc_1_7168:
        mov    cl, byte ptr [eax + 30h]
        cmp    cl, 1
        je     SHORT loc_1_7175
        cmp    cl, 4
        jne    SHORT loc_1_71A6
loc_1_7175:
        cmp    byte ptr [eax + 19h], 1
        je     SHORT loc_1_71A6
        mov    dword ptr [eax + 39h], 0
        mov    ecx, dword ptr [eax + 31h]
        mov    dword ptr [eax + 39h], ecx
        mov    dword ptr [eax + 59h], ecx
        mov    dword ptr [eax + 5dh], 0
        db  08Bh, 00Dh, 0ECh, 037h, 001h, 000h
        mov    dword ptr [eax + 61h], ecx
        db  08Bh, 00Dh, 048h, 036h, 001h, 000h
        mov    dword ptr [eax + 65h], ecx
        jmp    SHORT loc_1_71AA
loc_1_71A6:
        mov    byte ptr [eax + 19h], 0
loc_1_71AA:
        push   esi
        call   sub_1_A7C2
        add    esp, 4
        mov    eax, 2b0ch
        jmp    NEAR PTR loc_1_744D
loc_1_71BD:
        push   dword ptr [esp + 58h]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_71E7
        push   dword ptr [esp + 58h]
        call   sub_1_35F7
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_71E7
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_71E7:
        movzx  ecx, byte ptr [eax + 30h]
        cmp    ecx, 1
        je     SHORT loc_1_7207
        cmp    ecx, 2
        je     SHORT loc_1_7263
        cmp    ecx, 3
        je     NEAR PTR loc_1_7416
        cmp    ecx, 4
        jne    NEAR PTR loc_1_744B
loc_1_7207:
        mov    ebx, dword ptr [esp + 64h]
        cmp    ebx, 2
        je     SHORT loc_1_7228
        cmp    ebx, 3
        je     SHORT loc_1_7247
        cmp    ebx, 4
        jb     NEAR PTR loc_1_744B
        cmp    ebx, 5
        jbe    SHORT loc_1_7250
        jmp    NEAR PTR loc_1_744B
loc_1_7228:
        mov    byte ptr [eax + 30h], 1
        cmp    dword ptr [eax + 39h], 0
        jne    SHORT loc_1_723B
        mov    ecx, dword ptr [eax + 31h]
        mov    dword ptr [eax + 39h], ecx
        mov    dword ptr [eax + 59h], ecx
loc_1_723B:
        mov    dword ptr [eax + 3dh], 0
        jmp    NEAR PTR loc_1_744B
loc_1_7247:
        mov    byte ptr [eax + 30h], 1
        jmp    NEAR PTR loc_1_744B
loc_1_7250:
        mov    ecx, dword ptr [eax + 45h]
        sub    ecx, eax
        cmp    ecx, 45h
        je     NEAR PTR loc_1_7443
        jmp    NEAR PTR loc_1_742E
loc_1_7263:
        mov    ebx, dword ptr [esp + 64h]
        cmp    ebx, 2
        jb     NEAR PTR loc_1_744B
        cmp    ebx, 3
        jbe    SHORT loc_1_728C
        cmp    ebx, 4
        jb     NEAR PTR loc_1_744B
        cmp    ebx, 5
        jbe    NEAR PTR loc_1_740A
        jmp    NEAR PTR loc_1_744B
loc_1_728C:
        mov    byte ptr [eax + 30h], 1
        cmp    ebx, 2
        je     SHORT loc_1_7299
        mov    byte ptr [eax + 30h], 4
loc_1_7299:
        cmp    dword ptr [eax + 39h], 0
        jne    SHORT loc_1_72A8
        mov    ecx, dword ptr [eax + 31h]
        mov    dword ptr [eax + 39h], ecx
        mov    dword ptr [eax + 59h], ecx
loc_1_72A8:
        mov    dword ptr [eax + 5dh], 0
        db  08Bh, 00Dh, 044h, 038h, 001h, 000h
        mov    dword ptr [eax + 61h], ecx
        db  08Bh, 00Dh, 02Ch, 038h, 001h, 000h
        mov    dword ptr [eax + 65h], ecx
        mov    dword ptr [eax + 3dh], 0
        call   sub_1_A7B0
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_73EB
        mov    edi, dword ptr [esi + 4]
        or     edi, edi
        jne    SHORT loc_1_72E8
        mov    eax, 2b09h
        jmp    NEAR PTR loc_1_744D
loc_1_72E8:
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 0ch], 3ch
        mov    byte ptr [edi], 0feh
        push   esi
        movzx  eax, byte ptr [edi + 17h]
        movzx  ecx, byte ptr [edi + 6]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 13h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 14h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 15h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 16h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 18h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 19h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 1ah]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0ah]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 1bh]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0bh]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 1ch]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0ch]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 1dh]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0dh]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 1eh]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0eh]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 1fh]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0fh]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 20h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 21h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 10h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 11h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 22h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 12h]
        add    ecx, eax
        add    ecx, 8c00h
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        push   0
        push   0
        push   0
        push   esi
        mov    word ptr [edi + 37h], ax
        push   dword ptr [esp + 68h]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        push   esi
        call   sub_1_A7C2
        add    esp, 4
loc_1_73EB:
        push   dword ptr [esp + 58h]
        call   sub_1_CF55
        add    esp, 4
        db  081h, 03Dh, 09Ch, 035h, 001h, 000h, 09Ch, 035h, 001h, 000h
        je     SHORT loc_1_744B
        call   sub_1_BB33
        jmp    SHORT loc_1_744B
loc_1_740A:
        mov    ecx, dword ptr [eax + 45h]
        sub    ecx, eax
        cmp    ecx, 45h
        je     SHORT loc_1_7443
        jmp    SHORT loc_1_742E
loc_1_7416:
        mov    ebx, dword ptr [esp + 64h]
        cmp    ebx, 4
        jb     SHORT loc_1_744B
        cmp    ebx, 5
        ja     SHORT loc_1_744B
        mov    ecx, dword ptr [eax + 45h]
        sub    ecx, eax
        cmp    ecx, 45h
        je     SHORT loc_1_7443
loc_1_742E:
        mov    dword ptr [eax + 3dh], 0
        mov    dword ptr [eax + 41h], 0
        mov    dword ptr [eax + 39h], 3
loc_1_7443:
        mov    byte ptr [eax + 30h], 1
        mov    byte ptr [eax + 19h], 1
loc_1_744B:
        sub    eax, eax
loc_1_744D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 44h
        ret
        db  0F6h, 05Dh, 000h, 000h, 0C9h, 061h, 000h, 000h, 0C5h, 052h, 000h, 000h, 04Dh, 074h, 000h, 000h
        db  04Dh, 074h, 000h, 000h, 0C5h, 052h, 000h, 000h, 0C5h, 052h, 000h, 000h, 0C5h, 052h, 000h, 000h
        db  0C5h, 052h, 000h, 000h, 0C5h, 052h, 000h, 000h, 0C5h, 052h, 000h, 000h, 04Dh, 074h, 000h, 000h
        db  04Dh, 074h, 000h, 000h, 04Dh, 074h, 000h, 000h, 0F6h, 065h, 000h, 000h, 0C0h, 06Eh, 000h, 000h
        db  0A0h, 070h, 000h, 000h, 0E3h, 070h, 000h, 000h, 0BDh, 071h, 000h, 000h
loc_1_74A1:
        sub    esp, 4
        push   dword ptr [esp + 8]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_74BB
        mov    eax, 2b0eh
        jmp    SHORT loc_1_7539
loc_1_74BB:
        cmp    dword ptr [esp + 10h], 0e9h
        je     SHORT loc_1_74CC
        mov    ecx, 2b0fh
        jmp    SHORT loc_1_7537
loc_1_74CC:
        mov    ecx, dword ptr [esp + 14h]
        or     ecx, ecx
        jne    SHORT loc_1_74ED
        mov    dword ptr [eax + 4dh], 0
        mov    dword ptr [eax + 51h], 0
        mov    byte ptr [eax + 1ah], 1
        mov    ecx, 2b0eh
        jmp    SHORT loc_1_7537
loc_1_74ED:
        cmp    ecx, 2b09h
        jne    SHORT loc_1_7533
        cmp    dword ptr [eax + 4dh], 0
        jne    SHORT loc_1_752C
        db  08Bh, 00Dh, 014h, 02Bh, 001h, 000h
        cmp    dword ptr [eax + 51h], ecx
        jne    SHORT loc_1_751F
        mov    byte ptr [eax + 1ah], 0
        mov    ecx, 2b0dh
        mov    dword ptr [eax + 51h], 0
        mov    dword ptr [eax + 39h], 0
        jmp    SHORT loc_1_7537
loc_1_751F:
        mov    ecx, dword ptr [eax + 55h]
        mov    dword ptr [eax + 4dh], ecx
        mov    ecx, 2b0fh
        jmp    SHORT loc_1_7537
loc_1_752C:
        mov    ecx, 2b0fh
        jmp    SHORT loc_1_7537
loc_1_7533:
        mov    ecx, dword ptr [esp]
loc_1_7537:
        mov    eax, ecx
loc_1_7539:
        add    esp, 4
        ret
loc_1_753D:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 30h]
        cmp    dword ptr [esp + 38h], 0
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 10h], eax
        je     SHORT loc_1_7586
        mov    eax, dword ptr [esp + 38h]
        mov    ecx, dword ptr [eax + 14h]
        or     ecx, ecx
        je     SHORT loc_1_7586
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        push   ecx
        call   sub_1_3BB5
        add    esp, 10h
        or     al, al
        jne    SHORT loc_1_7586
        mov    eax, 3
        jmp    NEAR PTR loc_1_7668
loc_1_7586:
        call   sub_1_4735
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_759B
        mov    eax, 1
        jmp    NEAR PTR loc_1_7668
loc_1_759B:
        call   sub_1_CE5D
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_75B9
        push   ebx
        call   sub_1_4723
        add    esp, 4
        mov    eax, 1
        jmp    NEAR PTR loc_1_7668
loc_1_75B9:
        db  083h, 03Dh, 054h, 036h, 001h, 000h, 000h
        jne    SHORT loc_1_75E9
        db  068h, 073h, 052h, 000h, 000h
        push   12h
        call   sub_1_E730
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_75DF
        mov    eax, 20h
        jmp    NEAR PTR loc_1_7668
loc_1_75DF:
        db  0C7h, 005h, 054h, 036h, 001h, 000h, 001h, 000h, 000h, 000h
loc_1_75E9:
        mov    ax, word ptr [esp + 34h]
        mov    word ptr [ebx + 18h], ax
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [ebx + 14h], eax
        mov    eax, dword ptr [esi + 4]
        lea    edi, [ebp + 7ch]
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebx + 0ch], eax
        mov    dword ptr [ebp + 0eah], ebx
        lea    esi, [esp + 1ch]
        mov    byte ptr [ebp + 0bch], 1
        movsd
        movsd
        movsd
        movsd
        push   dword ptr [esp + 10h]
        call   sub_1_4777
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_7654
        push   dword ptr [esp + 38h]
        push   ebp
        push   esi
        call   sub_1_4E04
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_7668
        mov    dword ptr [esi], ebp
        mov    dword ptr [ebp + 102h], esi
        inc    word ptr [esi + 0eh]
        mov    eax, 0ffh
        jmp    SHORT loc_1_7668
loc_1_7654:
        push   ebp
        call   sub_1_CF22
        add    esp, 4
        push   ebx
        call   sub_1_4723
        add    esp, 4
        sub    eax, eax
loc_1_7668:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
sub_1_7670:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_B12A
        add    esp, 4
        db  0A1h, 000h, 038h, 001h, 000h
        or     eax, 2
        db  0A3h, 000h, 038h, 001h, 000h
        push   esi
        call   sub_1_A929
        add    esp, 4
        push   esi
        call   sub_1_4BB1
        add    esp, 4
        pop    esi
        ret
loc_1_769F:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        mov    edi, dword ptr [ebx]
        call   sub_1_4735
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_76BA
        mov    eax, 1
        jmp    SHORT loc_1_7734
loc_1_76BA:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 0ch], eax
        push   edi
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_7724
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     SHORT loc_1_7724
        test   byte ptr [ecx + 0beh], 1
        jne    SHORT loc_1_7724
        cmp    byte ptr [ecx + 0bch], 3
        jne    SHORT loc_1_7724
        mov    eax, dword ptr [ecx + 60h]
        test   al, 4
        jne    SHORT loc_1_7724
        cmp    dword ptr [ecx + 0eah], 0
        jne    SHORT loc_1_7724
        mov    dword ptr [ecx + 0eah], esi
        or     eax, 4
        mov    dword ptr [ecx + 60h], eax
        inc    dword ptr [ecx + 0a6h]
        push   ecx
        call   sub_1_7670
        add    esp, 4
        mov    eax, 0ffh
        jmp    SHORT loc_1_7734
loc_1_7724:
        mov    edi, 0fh
        push   esi
        call   sub_1_4723
        add    esp, 4
        mov    eax, edi
loc_1_7734:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_7738:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        push   dword ptr [ebx]
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_7896
        mov    edi, dword ptr [eax]
        or     edi, edi
        je     NEAR PTR loc_1_7896
        mov    si, word ptr [esp + 18h]
        test   si, 2
        je     SHORT loc_1_779C
        cmp    byte ptr [edi + 0bch], 0
        je     SHORT loc_1_7792
        test   byte ptr [edi + 0beh], 1
        jne    SHORT loc_1_778B
        or     dword ptr [edi + 60h], 40000000h
        push   40h
        push   edi
        call   sub_1_4B72
        add    esp, 8
loc_1_778B:
        sub    eax, eax
        jmp    NEAR PTR loc_1_789B
loc_1_7792:
        mov    eax, 19h
        jmp    NEAR PTR loc_1_789B
loc_1_779C:
        call   sub_1_4735
        mov    ecx, eax
        or     ecx, ecx
        je     NEAR PTR loc_1_788F
        mov    word ptr [ecx + 18h], 0
        mov    dword ptr [ecx + 14h], 0
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 8], eax
        test   si, 1
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ecx + 0ch], eax
        jne    NEAR PTR loc_1_786C
        mov    eax, dword ptr [esp + 14h]
        or     eax, eax
        je     SHORT loc_1_77FC
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_1_77FC
        mov    esi, 64h
        sub    edx, edx
        div    esi
        inc    eax
        cmp    eax, 0ffffffh
        jbe    SHORT loc_1_77F5
        mov    eax, 0ffffffh
        jmp    SHORT loc_1_77F6
loc_1_77F5:
        inc    eax
loc_1_77F6:
        mov    word ptr [ecx + 1ah], ax
        jmp    SHORT loc_1_7802
loc_1_77FC:
        mov    word ptr [ecx + 1ah], 0
loc_1_7802:
        mov    al, byte ptr [edi + 0bch]
        cmp    al, 4
        jne    SHORT loc_1_7840
        mov    byte ptr [edi + 0bch], 5
        inc    byte ptr [edi + 0bah]
        or     dword ptr [edi + 64h], 1
        mov    dword ptr [edi + 0eah], ecx
        or     dword ptr [edi + 60h], 10000h
        inc    dword ptr [edi + 0a6h]
        push   edi
        call   sub_1_B37E
        add    esp, 4
        mov    eax, 0ffh
        jmp    SHORT loc_1_789B
loc_1_7840:
        cmp    al, 7
        jne    SHORT loc_1_785C
        mov    byte ptr [edi + 0bch], 9
        push   0
        push   0
        push   edi
        call   sub_1_7998
        add    esp, 0ch
        sub    eax, eax
        jmp    SHORT loc_1_789B
loc_1_785C:
        push   ecx
        call   sub_1_4723
        add    esp, 4
        mov    eax, 19h
        jmp    SHORT loc_1_789B
loc_1_786C:
        mov    word ptr [ecx + 1ah], 0
        cmp    dword ptr [edi + 11ah], 0
        jne    SHORT loc_1_7888
        mov    dword ptr [edi + 11ah], ecx
        mov    eax, 0ffh
        jmp    SHORT loc_1_789B
loc_1_7888:
        mov    eax, 19h
        jmp    SHORT loc_1_789B
loc_1_788F:
        mov    eax, 1
        jmp    SHORT loc_1_789B
loc_1_7896:
        mov    eax, 0fh
loc_1_789B:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_789F:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0deh], 0
        jne    SHORT loc_1_78CC
        cmp    dword ptr [eax + 126h], 0
        jne    SHORT loc_1_78CC
        cmp    dword ptr [eax + 0d6h], 0
        jne    SHORT loc_1_78CC
        cmp    dword ptr [eax + 11eh], 0
        mov    eax, 1
        je     SHORT loc_1_78CE
loc_1_78CC:
        sub    eax, eax
loc_1_78CE:
        ret
sub_1_78CF:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        cmp    byte ptr [ebx + 0bch], 3
        jbe    NEAR PTR loc_1_7994
        test   byte ptr [ebx + 60h], 10h
        jne    NEAR PTR loc_1_7994
        mov    esi, dword ptr [esp + 14h]
        cmp    esi, 16h
        jne    SHORT loc_1_790F
        push   ebx
        call   sub_1_789F
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_790F
        or     dword ptr [ebx + 60h], 100000h
        jmp    NEAR PTR loc_1_7994
loc_1_790F:
        mov    eax, dword ptr [ebx + 60h]
        or     eax, 10h
        mov    dword ptr [ebx + 60h], eax
        and    eax, 0ffefffffh
        mov    dword ptr [ebx + 60h], eax
        mov    edi, dword ptr [ebx + 11ah]
        or     edi, edi
        je     SHORT loc_1_7948
        mov    dword ptr [ebx + 11ah], 0
        push   0
        push   esi
        push   dword ptr [edi + 0ch]
        call   dword ptr [edi + 8]
        add    esp, 0ch
        push   edi
        call   sub_1_4723
        jmp    SHORT loc_1_7991
loc_1_7948:
        mov    edx, dword ptr [ebx + 0eeh]
        mov    eax, dword ptr [ebx + 102h]
        or     eax, eax
        je     SHORT loc_1_7994
        mov    edi, dword ptr [eax + 4]
        or     edi, edi
        je     SHORT loc_1_7994
        mov    eax, dword ptr [edi + 62h]
        mov    ebx, dword ptr [edi + 66h]
        or     eax, eax
        je     SHORT loc_1_7994
        inc    word ptr [edi + 46h]
        sub    esi, 16h
        cmp    esi, 1
        sbb    ecx, ecx
        and    ecx, 2
        add    ecx, 2
        push   ecx
        push   0
        push   0
        push   0
        push   0
        push   edx
        push   ebx
        call   eax
        add    esp, 1ch
        push   edi
        call   sub_1_4325
loc_1_7991:
        add    esp, 4
loc_1_7994:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_7998:
        push   esi
        push   edi
        cmp    dword ptr [esp + 14h], 0
        mov    esi, dword ptr [esp + 0ch]
        je     SHORT loc_1_7A06
        push   esi
        call   sub_1_789F
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_79EC
        mov    edi, dword ptr [esp + 10h]
        or     edi, edi
        je     SHORT loc_1_79CE
        call   sub_1_F21C
        mov    dword ptr [esi + 0ceh], eax
        mov    byte ptr [esi + 0bch], 0ah
        jmp    SHORT loc_1_79D9
loc_1_79CE:
        push   0
        push   esi
        call   sub_1_4B72
        add    esp, 8
loc_1_79D9:
        push   esi
        call   sub_1_48EE
        add    esp, 4
        push   16h
        push   esi
        call   sub_1_78CF
        jmp    SHORT loc_1_7A45
loc_1_79EC:
        cmp    dword ptr [esp + 10h], 1
        cmc
        sbb    eax, eax
        and    eax, 200000h
        or     eax, 1000000h
        or     dword ptr [esi + 60h], eax
        jmp    NEAR PTR loc_1_7A8E
loc_1_7A06:
        mov    eax, dword ptr [esi + 60h]
        test   al, 10h
        je     SHORT loc_1_7A6A
        mov    edi, dword ptr [esp + 10h]
        or     edi, edi
        je     SHORT loc_1_7A29
        call   sub_1_F21C
        mov    dword ptr [esi + 0ceh], eax
        mov    byte ptr [esi + 0bch], 0ah
        jmp    SHORT loc_1_7A34
loc_1_7A29:
        push   0
        push   esi
        call   sub_1_4B72
        add    esp, 8
loc_1_7A34:
        push   esi
        call   sub_1_48EE
        add    esp, 4
        push   0
        push   esi
        call   sub_1_8FCD
loc_1_7A45:
        add    esp, 8
        test   byte ptr [esi + 0beh], 1
        jne    SHORT loc_1_7A8E
        or     edi, edi
        je     SHORT loc_1_7A8E
        mov    dword ptr [esi + 0c8h], 3e8h
        push   esi
        call   sub_1_48FB
        add    esp, 4
        jmp    SHORT loc_1_7A8E
loc_1_7A6A:
        cmp    dword ptr [esp + 10h], 1
        cmc
        sbb    ecx, ecx
        and    ecx, 200000h
        or     ecx, eax
        or     ecx, 1000000h
        mov    dword ptr [esi + 60h], ecx
        push   0
        push   esi
        call   sub_1_8FCD
        add    esp, 8
loc_1_7A8E:
        push   esi
        call   sub_1_4BB1
        add    esp, 4
        pop    edi
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_7AA0:
        mov    ecx, dword ptr [esp + 4]
        db  0A1h, 0CCh, 02Ah, 001h, 000h
        mov    dword ptr [ecx], eax
        db  089h, 00Dh, 0CCh, 02Ah, 001h, 000h
        ret
sub_1_7AB2:
        db  0A1h, 0CCh, 02Ah, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_7AC5
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 0CCh, 02Ah, 001h, 000h
        jmp    SHORT loc_1_7AF3
loc_1_7AC5:
        db  08Bh, 00Dh, 0D0h, 02Ah, 001h, 000h
        db  039h, 00Dh, 0D4h, 02Ah, 001h, 000h
        mov    eax, 0
        jbe    SHORT loc_1_7AE8
        push   24h
        call   sub_1_F390
        add    esp, 4
        db  08Bh, 00Dh, 0D0h, 02Ah, 001h, 000h
loc_1_7AE8:
        or     eax, eax
        je     SHORT loc_1_7AF3
        inc    ecx
        db  089h, 00Dh, 0D0h, 02Ah, 001h, 000h
loc_1_7AF3:
        ret
sub_1_7AF4:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_7B04
loc_1_7AFD:
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_7AFD
loc_1_7B04:
        ret
sub_1_7B05:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        or     edi, edi
        mov    esi, dword ptr [esp + 10h]
        je     SHORT loc_1_7B40
        mov    ebx, 5
loc_1_7B19:
        mov    eax, dword ptr [esi + 0ch]
        cmp    edi, eax
        jb     SHORT loc_1_7B3A
        sub    edi, eax
        mov    eax, esi
        mov    esi, dword ptr [esi]
        cmp    dword ptr [eax + 4], ebx
        jne    SHORT loc_1_7B34
        push   eax
        call   sub_1_F396
        add    esp, 4
loc_1_7B34:
        or     edi, edi
        jne    SHORT loc_1_7B19
        jmp    SHORT loc_1_7B40
loc_1_7B3A:
        sub    dword ptr [esi + 0ch], edi
        add    dword ptr [esi + 8], edi
loc_1_7B40:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_7B46:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 20h]
        cmp    dword ptr [ebp + 4], 5
        je     NEAR PTR loc_1_7C5E
        mov    eax, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [esp + 28h]
        cmp    eax, ebx
        jb     SHORT loc_1_7B68
        mov    eax, ebx
loc_1_7B68:
        mov    dword ptr [esp + 18h], eax
        add    eax, 10h
        push   eax
        call   sub_1_F390
        mov    dword ptr [esp + 18h], eax
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_7C5A
        mov    edx, eax
        mov    ecx, eax
        mov    dword ptr [esp + 10h], edx
        mov    dword ptr [ecx], 0
        mov    ecx, eax
        mov    edi, dword ptr [esp + 14h]
        mov    dword ptr [ecx + 4], 5
        mov    ecx, eax
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, dword ptr [esp + 14h]
        add    edi, 10h
        mov    dword ptr [ecx + 8], edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        sub    ebx, dword ptr [esp + 18h]
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [esp + 20h], ecx
        or     ecx, ecx
        je     SHORT loc_1_7C3E
loc_1_7BD2:
        mov    ecx, dword ptr [esp + 20h]
        cmp    dword ptr [ecx + 4], 5
        je     SHORT loc_1_7C3E
        or     ebx, ebx
        je     SHORT loc_1_7C3E
        mov    ebp, dword ptr [ecx + 0ch]
        cmp    ebp, ebx
        jb     SHORT loc_1_7BE9
        mov    ebp, ebx
loc_1_7BE9:
        lea    eax, [ebp + 10h]
        push   eax
        call   sub_1_F390
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_7C4A
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 5
        mov    dword ptr [eax + 0ch], ebp
        lea    edi, [eax + 10h]
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [eax + 8], edi
        mov    esi, dword ptr [ecx + 8]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [esp + 10h]
        mov    dword ptr [esp + 10h], eax
        mov    dword ptr [edx], eax
        mov    ecx, dword ptr [esp + 20h]
        sub    ebx, ebp
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [esp + 20h], ecx
        or     ecx, ecx
        jne    SHORT loc_1_7BD2
loc_1_7C3E:
        mov    edx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_1_7C78
loc_1_7C4A:
        push   dword ptr [esp + 14h]
        call   sub_1_95BA
        add    esp, 4
        sub    eax, eax
        jmp    SHORT loc_1_7CC2
loc_1_7C5A:
        sub    eax, eax
        jmp    SHORT loc_1_7CC2
loc_1_7C5E:
        mov    dword ptr [esp + 14h], ebp
        mov    dword ptr [esp + 10h], ebp
        add    ebp, 0ch
        mov    ebx, dword ptr [esp + 28h]
        cmp    dword ptr [ebp], ebx
        jbe    SHORT loc_1_7C75
        mov    dword ptr [ebp], ebx
loc_1_7C75:
        sub    ebx, dword ptr [ebp]
loc_1_7C78:
        or     ebx, ebx
        je     SHORT loc_1_7C97
loc_1_7C7C:
        mov    edx, dword ptr [esp + 10h]
        mov    edx, dword ptr [edx]
        mov    dword ptr [esp + 10h], edx
        cmp    dword ptr [edx + 0ch], ebx
        jbe    SHORT loc_1_7C8E
        mov    dword ptr [edx + 0ch], ebx
loc_1_7C8E:
        mov    edx, dword ptr [esp + 10h]
        sub    ebx, dword ptr [edx + 0ch]
        jne    SHORT loc_1_7C7C
loc_1_7C97:
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     SHORT loc_1_7CB4
        push   eax
        call   sub_1_95BA
        mov    edx, dword ptr [esp + 14h]
        add    esp, 4
        mov    dword ptr [edx], 0
loc_1_7CB4:
        mov    eax, dword ptr [esp + 24h]
        mov    edx, dword ptr [esp + 10h]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 14h]
loc_1_7CC2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
loc_1_7CCA:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        lea    eax, [esp + 8]
        push   esi
        mov    edi, dword ptr [esp + 1ch]
        push   eax
        push   edi
        call   sub_1_7B46
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_7D19
        mov    edx, dword ptr [esp + 10h]
        cmp    dword ptr [edx + 0e2h], 0
        jne    SHORT loc_1_7CFF
        mov    dword ptr [edx + 0e2h], eax
        jmp    SHORT loc_1_7D07
loc_1_7CFF:
        mov    ecx, dword ptr [edx + 0e6h]
        mov    dword ptr [ecx], eax
loc_1_7D07:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx + 0e6h], eax
        add    dword ptr [edx + 0deh], esi
        jmp    SHORT loc_1_7D24
loc_1_7D19:
        push   edi
        call   sub_1_95BA
        add    esp, 4
        sub    esi, esi
loc_1_7D24:
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 4
        ret
sub_1_7D2C:
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 3ch]
        mov    dword ptr [esp + 14h], 0
        mov    ebx, dword ptr [ecx + 58h]
        mov    ebp, dword ptr [ebx + 20h]
        mov    edx, dword ptr [ebx + 10h]
        mov    eax, dword ptr [ebx + 1ch]
        sub    eax, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        sub    ecx, edx
        cmp    eax, ecx
        jb     SHORT loc_1_7D59
        mov    eax, ecx
loc_1_7D59:
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [ebp + 4]
        add    eax, edx
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [esp + 44h]
        mov    dword ptr [esp + 18h], eax
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 24h], ecx
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esp + 28h], eax
        mov    eax, dword ptr [esp + 40h]
        and    eax, 800h
        cmp    eax, 1
        cmc
        sbb    eax, eax
        and    eax, 80000000h
        mov    dword ptr [esp + 2ch], eax
        mov    dword ptr [esp + 30h], eax
        or     dword ptr [ebx + 14h], eax
loc_1_7D9B:
        mov    eax, dword ptr [esp + 48h]
        sub    eax, dword ptr [esp + 14h]
        mov    ecx, dword ptr [esp + 20h]
        mov    edx, dword ptr [esp + 24h]
        cmp    ecx, edx
        jb     SHORT loc_1_7DB1
        mov    ecx, edx
loc_1_7DB1:
        cmp    eax, ecx
        jb     SHORT loc_1_7DB7
        mov    eax, ecx
loc_1_7DB7:
        mov    dword ptr [esp + 10h], eax
        mov    edi, dword ptr [esp + 1ch]
        mov    esi, dword ptr [esp + 28h]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 10h]
        add    dword ptr [esp + 14h], eax
        add    dword ptr [ebx + 0ch], eax
        sub    dword ptr [esp + 24h], eax
        jne    SHORT loc_1_7E17
        mov    esi, dword ptr [esp + 18h]
        mov    edx, esi
        mov    eax, dword ptr [esp + 14h]
        mov    edx, dword ptr [edx]
        mov    dword ptr [esp + 18h], edx
        cmp    eax, dword ptr [esp + 48h]
        je     SHORT loc_1_7E06
        mov    ecx, dword ptr [edx + 0ch]
        mov    dword ptr [esp + 24h], ecx
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [esp + 28h], eax
loc_1_7E06:
        cmp    dword ptr [esi + 4], 5
        jne    SHORT loc_1_7E1F
        push   esi
        call   sub_1_F396
        add    esp, 4
        jmp    SHORT loc_1_7E1F
loc_1_7E17:
        mov    eax, dword ptr [esp + 10h]
        add    dword ptr [esp + 28h], eax
loc_1_7E1F:
        mov    eax, dword ptr [esp + 10h]
        sub    dword ptr [esp + 20h], eax
        jne    NEAR PTR loc_1_7EB0
        mov    eax, dword ptr [ebx + 1ch]
        sub    eax, dword ptr [ebx + 0ch]
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        je     SHORT loc_1_7E54
        or     eax, eax
        je     SHORT loc_1_7E54
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    ecx, eax
        jb     SHORT loc_1_7E47
        mov    ecx, eax
loc_1_7E47:
        mov    dword ptr [esp + 20h], ecx
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [esp + 1ch], eax
        jmp    SHORT loc_1_7EB8
loc_1_7E54:
        mov    eax, dword ptr [ebx + 14h]
        or     eax, 80000000h
        mov    dword ptr [ebx + 14h], eax
        test   ah, 1
        je     SHORT loc_1_7E75
        push   200h
        push   dword ptr [esp + 40h]
        call   sub_1_8EB3
        add    esp, 8
loc_1_7E75:
        mov    dword ptr [esp + 30h], 1
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     SHORT loc_1_7EC6
        mov    ebp, dword ptr [ebx + 20h]
        mov    eax, dword ptr [ebx + 1ch]
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    eax, ecx
        jb     SHORT loc_1_7E92
        mov    eax, ecx
loc_1_7E92:
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [esp + 1ch], eax
        mov    eax, dword ptr [esp + 14h]
        cmp    eax, dword ptr [esp + 48h]
        je     SHORT loc_1_7EB8
        mov    eax, dword ptr [esp + 2ch]
        or     dword ptr [ebx + 14h], eax
        jmp    SHORT loc_1_7EB8
loc_1_7EB0:
        mov    eax, dword ptr [esp + 10h]
        add    dword ptr [esp + 1ch], eax
loc_1_7EB8:
        mov    eax, dword ptr [esp + 14h]
        cmp    eax, dword ptr [esp + 48h]
        jne    NEAR PTR loc_1_7D9B
loc_1_7EC6:
        mov    ecx, dword ptr [esp + 3ch]
        or     ebx, ebx
        mov    dword ptr [ecx + 58h], ebx
        je     SHORT loc_1_7EE7
        mov    dword ptr [ebx + 20h], ebp
        mov    eax, dword ptr [esp + 1ch]
        sub    eax, dword ptr [ebp + 4]
        mov    dword ptr [ebx + 10h], eax
        db  0C7h, 041h, 068h, 02Ch, 07Dh, 000h, 000h
        jmp    SHORT loc_1_7EF2
loc_1_7EE7:
        mov    ecx, dword ptr [esp + 3ch]
        db  0C7h, 041h, 068h, 0CAh, 07Ch, 000h, 000h
loc_1_7EF2:
        mov    ecx, dword ptr [esp + 3ch]
        mov    eax, dword ptr [ecx + 5ch]
        mov    ecx, dword ptr [esp + 14h]
        mov    edx, eax
        cmp    ecx, edx
        jb     SHORT loc_1_7F05
        mov    ecx, edx
loc_1_7F05:
        sub    eax, ecx
        mov    ecx, dword ptr [esp + 3ch]
        mov    dword ptr [ecx + 5ch], eax
        mov    eax, dword ptr [esp + 14h]
        cmp    eax, dword ptr [esp + 48h]
        je     NEAR PTR loc_1_7F9C
        mov    edx, dword ptr [esp + 18h]
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [edx + 8], eax
        mov    ecx, dword ptr [esp + 24h]
        mov    eax, dword ptr [esp + 48h]
        sub    eax, dword ptr [esp + 14h]
        push   eax
        mov    dword ptr [edx + 0ch], ecx
        lea    eax, [esp + 38h]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_1_7B46
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_7FA3
        mov    ecx, dword ptr [esp + 3ch]
        mov    ecx, dword ptr [ecx + 0e2h]
        or     ecx, ecx
        jne    SHORT loc_1_7F70
        mov    ecx, dword ptr [esp + 3ch]
        mov    dword ptr [ecx + 0e2h], eax
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [ecx + 0e6h], eax
        jmp    SHORT loc_1_7F80
loc_1_7F70:
        mov    edx, dword ptr [esp + 34h]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [esp + 3ch]
        mov    dword ptr [ecx + 0e2h], eax
loc_1_7F80:
        mov    eax, dword ptr [esp + 48h]
        mov    ecx, dword ptr [esp + 3ch]
        sub    eax, dword ptr [esp + 14h]
        add    dword ptr [ecx + 0deh], eax
        mov    eax, dword ptr [esp + 48h]
        mov    dword ptr [esp + 14h], eax
        jmp    SHORT loc_1_7FAF
loc_1_7F9C:
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_7FAF
loc_1_7FA3:
        push   dword ptr [esp + 18h]
        call   sub_1_95BA
        add    esp, 4
loc_1_7FAF:
        cmp    dword ptr [esp + 30h], 0
        je     SHORT loc_1_7FC7
        push   100h
        push   dword ptr [esp + 40h]
        call   sub_1_8EB3
        add    esp, 8
loc_1_7FC7:
        mov    eax, dword ptr [esp + 14h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 28h
        ret
loc_1_7FD3:
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], 0
        call   sub_1_7AB2
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_1_81D9
        mov    esi, dword ptr [esp + 34h]
        mov    edi, dword ptr [esp + 40h]
        mov    dword ptr [esi + 5ch], edi
        or     dword ptr [esi + 60h], 80h
        lea    eax, [esp + 1ch]
        mov    ebx, dword ptr [esp + 3ch]
        push   eax
        lea    eax, [esp + 14h]
        push   dword ptr [ebx + 8]
        push   eax
        push   edi
        push   dword ptr [ebx + 0ch]
        push   230h
        push   dword ptr [esi + 0eeh]
        push   dword ptr [esi + 116h]
        call   dword ptr [esi + 112h]
        mov    dword ptr [esp + 38h], eax
        add    esp, 20h
        and    dword ptr [esi + 60h], 0ffffff7fh
        cmp    eax, 18h
        jne    NEAR PTR loc_1_80F4
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [ebp + 4], eax
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ebp + 20h], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ebp + 1ch], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [ebp + 18h], eax
        mov    eax, dword ptr [esp + 2ch]
        movzx  eax, word ptr [eax]
        mov    dword ptr [ebp + 14h], eax
        mov    dword ptr [ebp + 10h], 0
        mov    dword ptr [ebp + 0ch], 0
        mov    eax, dword ptr [esi + 0d6h]
        or     eax, eax
        jne    SHORT loc_1_80A0
        mov    dword ptr [esi + 0d6h], ebp
        mov    dword ptr [esi + 0dah], ebp
        mov    dword ptr [ebp], 0
        jmp    SHORT loc_1_80A9
loc_1_80A0:
        mov    dword ptr [ebp], eax
        mov    dword ptr [esi + 0d6h], ebp
loc_1_80A9:
        mov    dword ptr [esi + 58h], ebp
        db  0C7h, 046h, 068h, 02Ch, 07Dh, 000h, 000h
        mov    eax, dword ptr [esp + 10h]
        sub    dword ptr [esi + 5ch], eax
        sub    edi, dword ptr [esp + 10h]
        je     SHORT loc_1_80E2
        push   dword ptr [esp + 10h]
        push   ebx
        call   sub_1_7B05
        add    esp, 8
        push   edi
        push   eax
        push   dword ptr [esp + 40h]
        push   esi
        call   sub_1_7D2C
        add    dword ptr [esp + 20h], eax
        add    esp, 10h
        jmp    SHORT loc_1_80EB
loc_1_80E2:
        push   ebx
        call   sub_1_95BA
        add    esp, 4
loc_1_80EB:
        mov    eax, dword ptr [esp + 10h]
        jmp    NEAR PTR loc_1_81E7
loc_1_80F4:
        push   ebp
        call   sub_1_7AA0
        add    esp, 4
        cmp    dword ptr [esp + 18h], 17h
        jne    SHORT loc_1_810C
        mov    dword ptr [esp + 10h], 0
loc_1_810C:
        mov    eax, dword ptr [esp + 10h]
        sub    dword ptr [esi + 5ch], eax
        cmp    dword ptr [esi + 0d6h], 0
        je     SHORT loc_1_8123
        db  0C7h, 046h, 068h, 02Ch, 07Dh, 000h, 000h
loc_1_8123:
        cmp    edi, dword ptr [esp + 10h]
        jne    SHORT loc_1_813B
        push   ebx
        call   sub_1_95BA
        mov    eax, dword ptr [esp + 14h]
        add    esp, 4
        jmp    NEAR PTR loc_1_81E7
loc_1_813B:
        push   dword ptr [esp + 10h]
        push   ebx
        call   sub_1_7B05
        add    esp, 8
        mov    ebx, eax
        cmp    dword ptr [esi + 0d6h], 0
        jne    SHORT loc_1_81BC
        db  0C7h, 046h, 068h, 0CAh, 07Ch, 000h, 000h
        mov    eax, edi
        sub    eax, dword ptr [esp + 10h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   ebx
        call   sub_1_7B46
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_81AD
        mov    ecx, dword ptr [esi + 0e2h]
        or     ecx, ecx
        jne    SHORT loc_1_818F
        mov    dword ptr [esi + 0e2h], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esi + 0e6h], eax
        jmp    SHORT loc_1_819B
loc_1_818F:
        mov    edx, dword ptr [esp + 14h]
        mov    dword ptr [edx], ecx
        mov    dword ptr [esi + 0e2h], eax
loc_1_819B:
        mov    eax, edi
        sub    eax, dword ptr [esp + 10h]
        add    dword ptr [esi + 0deh], eax
        mov    dword ptr [esp + 10h], edi
        jmp    SHORT loc_1_81B6
loc_1_81AD:
        push   ebx
        call   sub_1_95BA
        add    esp, 4
loc_1_81B6:
        mov    eax, dword ptr [esp + 10h]
        jmp    SHORT loc_1_81E7
loc_1_81BC:
        sub    edi, dword ptr [esp + 10h]
        push   edi
        push   ebx
        push   dword ptr [esp + 40h]
        push   esi
        call   sub_1_7D2C
        add    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [esp + 20h]
        add    esp, 10h
        jmp    SHORT loc_1_81E7
loc_1_81D9:
        push   dword ptr [esp + 3ch]
        call   sub_1_95BA
        add    esp, 4
        sub    eax, eax
loc_1_81E7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 20h
        ret
sub_1_81EF:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 0ch], 0
        sub    edi, edi
        mov    esi, dword ptr [esp + 28h]
loc_1_8203:
        mov    ecx, dword ptr [esi + 0deh]
        mov    dword ptr [esi + 5ch], ecx
        or     dword ptr [esi + 60h], 80h
        mov    edx, dword ptr [esi + 0e2h]
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 10h]
        push   dword ptr [edx + 8]
        push   eax
        push   ecx
        push   dword ptr [edx + 0ch]
        push   230h
        push   dword ptr [esi + 0eeh]
        push   dword ptr [esi + 116h]
        call   dword ptr [esi + 112h]
        add    esp, 20h
        and    dword ptr [esi + 60h], 0ffffff7fh
        cmp    eax, 18h
        je     SHORT loc_1_82B9
        cmp    eax, 17h
        jne    SHORT loc_1_8259
        mov    dword ptr [esp + 0ch], edi
loc_1_8259:
        mov    eax, dword ptr [esp + 0ch]
        sub    dword ptr [esi + 5ch], eax
        mov    ebx, dword ptr [esi + 0deh]
        mov    eax, dword ptr [esi + 0e2h]
        cmp    ebx, dword ptr [esp + 0ch]
        je     NEAR PTR loc_1_837F
        push   dword ptr [esp + 0ch]
        push   eax
        call   sub_1_7B05
        add    esp, 8
        cmp    dword ptr [esi + 0d6h], edi
        jne    NEAR PTR loc_1_83C0
        mov    dword ptr [esi + 0e2h], eax
        mov    eax, dword ptr [esp + 0ch]
        sub    dword ptr [esi + 0deh], eax
        cmp    dword ptr [esi + 5ch], edi
        jne    NEAR PTR loc_1_83F3
        cmp    dword ptr [esi + 112h], edi
        jne    NEAR PTR loc_1_8203
        jmp    NEAR PTR loc_1_83F3
loc_1_82B9:
        mov    ecx, dword ptr [esp + 2ch]
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 1ch]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [esp + 20h]
        movzx  eax, word ptr [eax]
        mov    dword ptr [ecx + 14h], eax
        mov    dword ptr [ecx + 10h], 0
        mov    dword ptr [ecx + 0ch], 0
        mov    eax, dword ptr [esi + 0d6h]
        or     eax, eax
        jne    SHORT loc_1_8316
        mov    dword ptr [esi + 0d6h], ecx
        mov    dword ptr [esi + 0dah], ecx
        mov    dword ptr [ecx], 0
        jmp    SHORT loc_1_831E
loc_1_8316:
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 0d6h], ecx
loc_1_831E:
        mov    dword ptr [esi + 58h], ecx
        db  0C7h, 046h, 068h, 02Ch, 07Dh, 000h, 000h
        mov    edi, dword ptr [esi + 0deh]
        mov    eax, dword ptr [esi + 0e2h]
        mov    dword ptr [esi + 0deh], 0
        mov    dword ptr [esi + 0e2h], 0
        mov    ecx, dword ptr [esp + 0ch]
        sub    dword ptr [esi + 5ch], ecx
        sub    edi, dword ptr [esp + 0ch]
        je     SHORT loc_1_8377
        push   dword ptr [esp + 0ch]
        push   eax
        call   sub_1_7B05
        add    esp, 8
        push   edi
        push   eax
        push   800h
        push   esi
        call   sub_1_7D2C
        add    esp, 10h
        jmp    NEAR PTR loc_1_83FF
loc_1_8377:
        push   eax
        call   sub_1_95BA
        jmp    SHORT loc_1_83FC
loc_1_837F:
        mov    dword ptr [esi + 0deh], 0
        mov    dword ptr [esi + 0e2h], 0
        cmp    dword ptr [esi + 0d6h], 0
        jne    SHORT loc_1_83AE
        cmp    dword ptr [esi + 112h], 0
        je     SHORT loc_1_83B5
        db  0C7h, 046h, 068h, 0D3h, 07Fh, 000h, 000h
        jmp    SHORT loc_1_83B5
loc_1_83AE:
        db  0C7h, 046h, 068h, 02Ch, 07Dh, 000h, 000h
loc_1_83B5:
        push   eax
        call   sub_1_95BA
        add    esp, 4
        jmp    SHORT loc_1_83F3
loc_1_83C0:
        db  0C7h, 046h, 068h, 02Ch, 07Dh, 000h, 000h
        mov    dword ptr [esi + 0deh], 0
        mov    dword ptr [esi + 0e2h], 0
        sub    ebx, dword ptr [esp + 0ch]
        push   ebx
        push   eax
        push   800h
        push   esi
        call   sub_1_7D2C
        add    dword ptr [esp + 1ch], eax
        add    esp, 10h
loc_1_83F3:
        push   dword ptr [esp + 2ch]
        call   sub_1_7AA0
loc_1_83FC:
        add    esp, 4
loc_1_83FF:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
sub_1_8406:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], 0
        mov    edi, dword ptr [esp + 34h]
        or     edi, edi
        mov    esi, dword ptr [esp + 30h]
        je     SHORT loc_1_8449
        cmp    dword ptr [esi + 122h], 0
        jne    SHORT loc_1_8430
        test   byte ptr [esi + 61h], 40h
        je     SHORT loc_1_847D
loc_1_8430:
        add    esi, 122h
        cmp    dword ptr [esi], 0
        je     SHORT loc_1_8442
loc_1_843B:
        mov    esi, dword ptr [esi]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_843B
loc_1_8442:
        mov    dword ptr [esi], edi
        jmp    NEAR PTR loc_1_871E
loc_1_8449:
        mov    edi, dword ptr [esi + 122h]
        or     edi, edi
        je     NEAR PTR loc_1_871E
        test   byte ptr [esi + 61h], 40h
        jne    NEAR PTR loc_1_871E
        mov    ebx, dword ptr [esi + 126h]
        mov    dword ptr [esi + 122h], 0
        mov    dword ptr [esi + 126h], 0
        jmp    SHORT loc_1_8481
loc_1_847D:
        mov    ebx, dword ptr [esp + 38h]
loc_1_8481:
        or     dword ptr [esi + 60h], 4000h
        inc    byte ptr [esi + 0bah]
        or     dword ptr [esi + 64h], 1
loc_1_8492:
        mov    dword ptr [esp + 10h], 0
        mov    ebp, dword ptr [esi + 11eh]
        or     ebp, ebp
        je     SHORT loc_1_84AF
        mov    eax, dword ptr [ebp]
        mov    dword ptr [esi + 11eh], eax
        jmp    SHORT loc_1_852D
loc_1_84AF:
        mov    ebp, dword ptr [esi + 0d6h]
        lea    ecx, [esi + 0d6h]
        or     ebp, ebp
        je     NEAR PTR loc_1_8578
        mov    eax, 40h
        sub    edx, edx
loc_1_84CA:
        test   dword ptr [ebp + 14h], eax
        je     SHORT loc_1_84D4
        cmp    dword ptr [ebp + 0ch], edx
        je     SHORT loc_1_84DF
loc_1_84D4:
        mov    ecx, ebp
        mov    ebp, dword ptr [ebp]
        cmp    ebp, edx
        jne    SHORT loc_1_84CA
        jmp    SHORT loc_1_852D
loc_1_84DF:
        cmp    dword ptr [esi + 0dah], ebp
        jne    SHORT loc_1_84FF
        cmp    dword ptr [esi + 0d6h], ebp
        mov    dword ptr [esi + 0dah], 0
        je     SHORT loc_1_84FF
        mov    dword ptr [esi + 0dah], ecx
loc_1_84FF:
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ecx], eax
        cmp    dword ptr [esi + 58h], ebp
        jne    SHORT loc_1_852D
        mov    dword ptr [esi + 58h], eax
        or     eax, eax
        jne    SHORT loc_1_852D
        cmp    dword ptr [esi + 112h], 0
        je     SHORT loc_1_8526
        cmp    dword ptr [esi + 5ch], 0
        db  0C7h, 046h, 068h, 0D3h, 07Fh, 000h, 000h
        je     SHORT loc_1_852D
loc_1_8526:
        db  0C7h, 046h, 068h, 0CAh, 07Ch, 000h, 000h
loc_1_852D:
        or     ebp, ebp
        je     SHORT loc_1_8578
        push   0
        push   ebx
        push   dword ptr [ebp + 20h]
        push   edi
        call   sub_1_442C
        mov    dword ptr [esp + 20h], eax
        add    esp, 10h
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   dword ptr [ebp + 4]
        add    esp, 0ch
        push   ebp
        call   sub_1_7AA0
        add    esp, 4
        mov    eax, dword ptr [esi + 12ah]
        mov    ecx, dword ptr [esp + 10h]
        mov    edx, eax
        cmp    ecx, edx
        jb     SHORT loc_1_856B
        mov    ecx, edx
loc_1_856B:
        sub    eax, ecx
        mov    dword ptr [esi + 12ah], eax
        jmp    NEAR PTR loc_1_866C
loc_1_8578:
        cmp    dword ptr [esi + 12ah], 0
        jne    NEAR PTR loc_1_86E6
        mov    ecx, dword ptr [esi + 0eeh]
        mov    eax, dword ptr [esi + 102h]
        or     eax, eax
        je     NEAR PTR loc_1_86E6
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     NEAR PTR loc_1_86E6
        test   byte ptr [eax + 40h], 80h
        je     NEAR PTR loc_1_86E6
        mov    edx, dword ptr [eax + 82h]
        or     edx, edx
        je     NEAR PTR loc_1_86E6
        mov    eax, dword ptr [eax + 86h]
        mov    dword ptr [esi + 12ah], ebx
        lea    ebp, [esp + 18h]
        push   ebp
        lea    ebp, [esp + 14h]
        push   dword ptr [edi + 8]
        push   ebp
        push   ebx
        push   dword ptr [edi + 0ch]
        push   250h
        push   ecx
        push   eax
        call   edx
        add    esp, 20h
        cmp    eax, 18h
        jne    SHORT loc_1_8645
        cmp    dword ptr [esp + 10h], ebx
        mov    eax, 0
        je     SHORT loc_1_8617
        push   dword ptr [esp + 10h]
        push   edi
        call   sub_1_7B05
        add    esp, 8
        push   0
        mov    edi, eax
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 20h]
        push   edi
        call   sub_1_442C
        add    esp, 10h
loc_1_8617:
        add    dword ptr [esp + 10h], eax
        mov    ecx, dword ptr [esi + 12ah]
        mov    edx, ecx
        mov    ebp, dword ptr [esp + 10h]
        cmp    ebp, ecx
        jb     SHORT loc_1_862D
        mov    ebp, ecx
loc_1_862D:
        sub    edx, ebp
        push   eax
        push   0
        mov    dword ptr [esi + 12ah], edx
        push   dword ptr [esp + 2ch]
        call   dword ptr [esp + 2ch]
        add    esp, 0ch
        jmp    SHORT loc_1_866C
loc_1_8645:
        cmp    eax, 17h
        jne    SHORT loc_1_8652
        mov    dword ptr [esp + 10h], 0
loc_1_8652:
        mov    eax, dword ptr [esi + 12ah]
        mov    ecx, eax
        mov    edx, dword ptr [esp + 10h]
        cmp    edx, eax
        jb     SHORT loc_1_8664
        mov    edx, eax
loc_1_8664:
        sub    ecx, edx
        mov    dword ptr [esi + 12ah], ecx
loc_1_866C:
        cmp    dword ptr [esp + 10h], ebx
        jne    SHORT loc_1_868F
        push   edi
        call   sub_1_95BA
        add    esp, 4
        mov    edi, dword ptr [esi + 122h]
        mov    ebx, dword ptr [esi + 126h]
        lea    ebp, [esi + 126h]
        jmp    SHORT loc_1_86CB
loc_1_868F:
        push   dword ptr [esp + 10h]
        push   edi
        call   sub_1_7B05
        add    esp, 8
        mov    edi, eax
        lea    ebp, [esi + 126h]
        sub    ebx, dword ptr [esp + 10h]
        add    ebx, dword ptr [ebp]
        lea    eax, [esi + 122h]
        cmp    dword ptr [esi + 122h], 0
        je     SHORT loc_1_86CB
        push   edi
        call   sub_1_7AF4
        add    esp, 4
        mov    ecx, dword ptr [esi + 122h]
        mov    dword ptr [eax], ecx
loc_1_86CB:
        mov    dword ptr [esi + 122h], 0
        mov    dword ptr [ebp], 0
        or     edi, edi
        jne    NEAR PTR loc_1_8492
        jmp    SHORT loc_1_8703
loc_1_86E6:
        push   edi
        call   sub_1_7AF4
        add    esp, 4
        mov    ecx, dword ptr [esi + 122h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 122h], edi
        add    dword ptr [esi + 126h], ebx
loc_1_8703:
        and    dword ptr [esi + 60h], 0ffffbfffh
        mov    al, byte ptr [esi + 0bah]
        dec    al
        mov    byte ptr [esi + 0bah], al
        jne    SHORT loc_1_871E
        and    dword ptr [esi + 64h], 0fffffffeh
loc_1_871E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
sub_1_8726:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [edx + 0d6h]
        mov    eax, esi
        or     eax, eax
        je     SHORT loc_1_8744
loc_1_8737:
        or     dword ptr [eax + 14h], 80000000h
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_8737
loc_1_8744:
        mov    ecx, dword ptr [edx + 11eh]
        mov    eax, ecx
        or     eax, eax
        je     SHORT loc_1_875D
loc_1_8750:
        or     dword ptr [eax + 14h], 80000000h
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_8750
loc_1_875D:
        or     esi, esi
        jne    SHORT loc_1_8765
        or     ecx, ecx
        je     SHORT loc_1_8773
loc_1_8765:
        push   100h
        push   edx
        call   sub_1_8EB3
        add    esp, 8
loc_1_8773:
        pop    esi
        ret
sub_1_8775:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 24h]
        or     edi, edi
        mov    esi, dword ptr [esp + 20h]
        je     SHORT loc_1_87D3
        lea    eax, [esi + 10h]
        push   eax
        call   sub_1_F390
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_87C3
        mov    dword ptr [ebx + 0ch], esi
        mov    dword ptr [ebx + 4], 5
        lea    eax, [ebx + 10h]
        mov    dword ptr [ebx + 8], eax
        push   dword ptr [esp + 1ch]
        push   esi
        mov    dword ptr [ebx], 0
        push   dword ptr [esp + 1ch]
        push   eax
        call   sub_1_44E1
        add    esp, 10h
        mov    dword ptr [edi], ebx
        jmp    SHORT loc_1_87D3
loc_1_87C3:
        mov    dword ptr [edi], 0
        mov    ebx, dword ptr [esp + 28h]
        or     ebx, ebx
        je     SHORT loc_1_882D
        jmp    SHORT loc_1_8827
loc_1_87D3:
        mov    ebx, dword ptr [esp + 28h]
        or     ebx, ebx
        je     SHORT loc_1_882D
        mov    edi, dword ptr [esp + 18h]
        sub    edi, esi
        sub    edi, dword ptr [esp + 1ch]
        lea    eax, [edi + 10h]
        push   eax
        call   sub_1_F390
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_8827
        mov    dword ptr [ebp + 0ch], edi
        mov    dword ptr [ebp + 4], 5
        lea    eax, [ebp + 10h]
        mov    dword ptr [ebp + 8], eax
        mov    ecx, dword ptr [esp + 1ch]
        add    ecx, esi
        push   ecx
        push   edi
        mov    dword ptr [ebp], 0
        push   dword ptr [esp + 1ch]
        push   eax
        call   sub_1_44E1
        add    esp, 10h
        mov    dword ptr [ebx], ebp
        jmp    SHORT loc_1_882D
loc_1_8827:
        mov    dword ptr [ebx], 0
loc_1_882D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_8832:
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 4ch]
        mov    ecx, dword ptr [esi + 60h]
        test   ch, 20h
        je     SHORT loc_1_886C
        mov    edi, dword ptr [esp + 50h]
        mov    ebx, dword ptr [edi + 0ch]
        or     ebx, ebx
        je     SHORT loc_1_8860
        mov    edx, dword ptr [esp + 58h]
        cmp    dword ptr [edx], ebx
        jb     SHORT loc_1_8860
        mov    ebp, 1
        dec    ebx
        jmp    SHORT loc_1_8882
loc_1_8860:
        and    dword ptr [edi + 10h], 0ffffdfffh
        jmp    NEAR PTR loc_1_8A74
loc_1_886C:
        mov    edi, dword ptr [esp + 50h]
        sub    ebx, ebx
        mov    ebp, dword ptr [edi + 0ch]
        mov    edx, dword ptr [esp + 58h]
        inc    ebp
        mov    eax, dword ptr [edx]
        cmp    ebp, eax
        jb     SHORT loc_1_8882
        mov    ebp, eax
loc_1_8882:
        mov    edx, dword ptr [esp + 58h]
        mov    eax, dword ptr [edx]
        sub    eax, ebp
        sub    eax, ebx
        mov    dword ptr [esp + 20h], eax
        mov    eax, dword ptr [edi]
        add    eax, ebx
        mov    dword ptr [esp + 18h], eax
        lea    eax, [ebp + eax - 1]
        mov    dword ptr [esp + 14h], eax
        test   ecx, 4000000h
        jne    NEAR PTR loc_1_8A31
        mov    dword ptr [esp + 10h], 0
        test   byte ptr [esi + 61h], 80h
        je     NEAR PTR loc_1_897E
        mov    ecx, dword ptr [esi + 48h]
        mov    eax, dword ptr [esi + 12eh]
        mov    edx, ecx
        sub    edx, eax
        jns    NEAR PTR loc_1_897E
        mov    ebx, dword ptr [esi + 132h]
        mov    ebp, ebx
        sub    ebp, eax
        mov    edx, dword ptr [esp + 58h]
        inc    ebp
        mov    edx, dword ptr [edx]
        sub    eax, ecx
        cmp    eax, edx
        jb     SHORT loc_1_88EA
        mov    eax, edx
loc_1_88EA:
        mov    dword ptr [esp + 24h], eax
        sub    ecx, ebx
        lea    ebx, [edx + ecx]
        or     ebx, ebx
        jne    SHORT loc_1_88F9
        sub    ebx, ebx
loc_1_88F9:
        lea    eax, [esp + 10h]
        or     ebx, ebx
        jne    SHORT loc_1_8903
        sub    eax, eax
loc_1_8903:
        push   eax
        push   0
        push   ebp
        push   dword ptr [esp + 30h]
        push   edx
        push   dword ptr [esp + 68h]
        call   sub_1_8775
        add    esp, 18h
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_8968
        mov    eax, dword ptr [esi + 132h]
        inc    eax
        mov    dword ptr [esp + 28h], eax
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esp + 38h], eax
        mov    eax, dword ptr [esp + 14h]
        sub    eax, dword ptr [esp + 28h]
        mov    dword ptr [esp + 34h], eax
        test   byte ptr [esi + 61h], 20h
        je     SHORT loc_1_8947
        inc    dword ptr [esp + 34h]
loc_1_8947:
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esp + 2ch], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esp + 30h], eax
        push   ebx
        lea    eax, [esp + 2ch]
        push   dword ptr [esp + 14h]
        push   eax
        push   esi
        call   sub_1_9770
        add    esp, 10h
loc_1_8968:
        mov    edx, dword ptr [esp + 58h]
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [edx], eax
        and    dword ptr [edi + 10h], 0ffffdfffh
        jmp    NEAR PTR loc_1_8A74
loc_1_897E:
        lea    eax, [esp + 10h]
        cmp    dword ptr [esp + 20h], 0
        jne    SHORT loc_1_898B
        sub    eax, eax
loc_1_898B:
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        push   ebp
        mov    edx, dword ptr [esp + 64h]
        push   ebx
        push   dword ptr [edx]
        push   dword ptr [esp + 68h]
        call   sub_1_8775
        add    esp, 18h
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_89E6
        mov    eax, dword ptr [edi]
        add    eax, ebp
        add    eax, ebx
        mov    dword ptr [esp + 28h], eax
        mov    eax, dword ptr [edi + 10h]
        and    eax, 0ffffdfffh
        mov    dword ptr [esp + 38h], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esp + 2ch], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esp + 30h], eax
        push   dword ptr [esp + 20h]
        lea    eax, [esp + 2ch]
        push   dword ptr [esp + 14h]
        push   eax
        push   esi
        call   sub_1_9770
        add    esp, 10h
loc_1_89E6:
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_8A29
        mov    eax, dword ptr [esi + 60h]
        test   ah, 80h
        jne    SHORT loc_1_8A07
        or     eax, 8000h
        mov    dword ptr [esi + 60h], eax
        inc    byte ptr [esi + 0bah]
        or     dword ptr [esi + 64h], 1
loc_1_8A07:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 12eh], eax
        mov    eax, dword ptr [esp + 14h]
        push   ebp
        mov    dword ptr [esi + 132h], eax
        push   dword ptr [esp + 20h]
        push   esi
        call   sub_1_8406
        add    esp, 0ch
loc_1_8A29:
        mov    edx, dword ptr [esp + 58h]
        mov    dword ptr [edx], ebx
        jmp    SHORT loc_1_8A74
loc_1_8A31:
        and    dword ptr [edi + 10h], 0ffffdfffh
        test   ch, 80h
        je     SHORT loc_1_8A4D
        mov    eax, dword ptr [esp + 14h]
        sub    eax, dword ptr [esi + 132h]
        or     eax, eax
        jg     SHORT loc_1_8A60
        jmp    SHORT loc_1_8A74
loc_1_8A4D:
        or     ecx, 8000h
        mov    dword ptr [esi + 60h], ecx
        inc    byte ptr [esi + 0bah]
        or     dword ptr [esi + 64h], 1
loc_1_8A60:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esi + 12eh], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esi + 132h], eax
loc_1_8A74:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
loc_1_8A7C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        push   dword ptr [esi]
        call   sub_1_4777
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_8C8E
        mov    ebx, dword ptr [eax]
        or     ebx, ebx
        je     NEAR PTR loc_1_8C87
        mov    edi, dword ptr [esp + 18h]
        mov    al, byte ptr [ebx + 0bch]
        mov    bp, word ptr [edi]
        cmp    al, 4
        jb     SHORT loc_1_8AB5
        cmp    al, 6
        jbe    SHORT loc_1_8ADD
loc_1_8AB5:
        cmp    dword ptr [ebx + 0deh], 0
        je     SHORT loc_1_8AC5
        test   bp, 20h
        jne    SHORT loc_1_8ADD
loc_1_8AC5:
        cmp    dword ptr [ebx + 126h], 0
        je     NEAR PTR loc_1_8C80
        test   bp, 40h
        je     NEAR PTR loc_1_8C80
loc_1_8ADD:
        test   byte ptr [ebx + 0beh], 1
        jne    NEAR PTR loc_1_8C80
        call   sub_1_7AB2
        or     eax, eax
        je     NEAR PTR loc_1_8C79
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [eax + 20h], ecx
        mov    ecx, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [eax + 1ch], ecx
        movzx  ecx, bp
        mov    dword ptr [eax + 18h], edi
        mov    dword ptr [eax + 10h], 0
        mov    dword ptr [eax + 0ch], 0
        mov    dword ptr [eax + 14h], ecx
        mov    cx, bp
        and    cl, 60h
        cmp    cl, 40h
        je     NEAR PTR loc_1_8C2C
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebx + 0d6h], 0
        jne    SHORT loc_1_8B50
        mov    dword ptr [ebx + 0d6h], eax
        jmp    SHORT loc_1_8B58
loc_1_8B50:
        mov    ecx, dword ptr [ebx + 0dah]
        mov    dword ptr [ecx], eax
loc_1_8B58:
        mov    dword ptr [ebx + 0dah], eax
        test   bp, 40h
        je     SHORT loc_1_8B94
        cmp    dword ptr [ebx + 126h], 0
        je     SHORT loc_1_8B94
        inc    dword ptr [ebx + 0a6h]
        push   0
        push   0
        push   ebx
        call   sub_1_8406
        add    esp, 0ch
        push   ebx
        call   sub_1_4BB1
        add    esp, 4
        mov    eax, 0ffh
        jmp    NEAR PTR loc_1_8C93
loc_1_8B94:
        cmp    dword ptr [ebx + 58h], 0
        jne    NEAR PTR loc_1_8C25
        mov    dword ptr [ebx + 58h], eax
        test   byte ptr [ebx + 60h], 80h
        jne    SHORT loc_1_8C25
        db  0C7h, 043h, 068h, 02Ch, 07Dh, 000h, 000h
        mov    eax, dword ptr [ebx + 0e2h]
        or     eax, eax
        jne    SHORT loc_1_8BC2
        mov    eax, 0ffh
        jmp    NEAR PTR loc_1_8C93
loc_1_8BC2:
        mov    ecx, dword ptr [ebx + 0deh]
        mov    dword ptr [ebx + 0e2h], 0
        mov    dword ptr [ebx + 0deh], 0
        inc    dword ptr [ebx + 0a6h]
        push   ecx
        push   eax
        push   800h
        push   ebx
        call   sub_1_7D2C
        add    esp, 10h
        mov    esi, dword ptr [ebx + 4ch]
        push   ebx
        call   sub_1_A8F8
        add    esp, 4
        cmp    eax, esi
        je     SHORT loc_1_8C10
        push   200h
        push   ebx
        call   sub_1_8EB3
        add    esp, 8
loc_1_8C10:
        push   ebx
        call   sub_1_4BB1
        add    esp, 4
        call   sub_1_8DFA
        mov    eax, 0ffh
        jmp    SHORT loc_1_8C93
loc_1_8C25:
        mov    eax, 0ffh
        jmp    SHORT loc_1_8C93
loc_1_8C2C:
        lea    ecx, [ebx + 11eh]
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_8C3E
loc_1_8C37:
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_1_8C37
loc_1_8C3E:
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], eax
        cmp    dword ptr [ebx + 122h], 0
        je     SHORT loc_1_8C72
        inc    dword ptr [ebx + 0a6h]
        push   0
        push   0
        push   ebx
        call   sub_1_8406
        add    esp, 0ch
        push   ebx
        call   sub_1_4BB1
        add    esp, 4
        mov    eax, 0ffh
        jmp    SHORT loc_1_8C93
loc_1_8C72:
        mov    eax, 0ffh
        jmp    SHORT loc_1_8C93
loc_1_8C79:
        mov    eax, 1
        jmp    SHORT loc_1_8C93
loc_1_8C80:
        mov    eax, 19h
        jmp    SHORT loc_1_8C93
loc_1_8C87:
        mov    eax, 19h
        jmp    SHORT loc_1_8C93
loc_1_8C8E:
        mov    eax, 0fh
loc_1_8C93:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_8CA0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 14h]
        test   byte ptr [edi + 0beh], 1
        jne    NEAR PTR loc_1_8DF5
        mov    eax, dword ptr [edi + 60h]
        test   al, 40h
        jne    NEAR PTR loc_1_8DF5
        cmp    dword ptr [edi + 0d6h], 0
        je     NEAR PTR loc_1_8DF5
        or     eax, 40h
        mov    dword ptr [edi + 60h], eax
        mov    ebx, 80000000h
        sub    ebp, ebp
loc_1_8CDA:
        mov    esi, dword ptr [edi + 0d6h]
        mov    eax, ebp
        test   dword ptr [esi + 14h], ebx
        je     SHORT loc_1_8D23
        mov    eax, dword ptr [esi]
        cmp    dword ptr [edi + 58h], esi
        jne    SHORT loc_1_8CF1
        mov    dword ptr [edi + 58h], eax
loc_1_8CF1:
        mov    dword ptr [edi + 0d6h], eax
        cmp    eax, ebp
        jne    SHORT loc_1_8D02
        db  0C7h, 047h, 068h, 0CAh, 07Ch, 000h, 000h
loc_1_8D02:
        mov    eax, dword ptr [esi + 18h]
        cmp    eax, ebp
        je     SHORT loc_1_8D0E
        mov    word ptr [eax], 30h
loc_1_8D0E:
        push   dword ptr [esi + 0ch]
        push   ebp
        push   dword ptr [esi + 8]
        call   dword ptr [esi + 4]
        add    esp, 0ch
        mov    eax, esi
        mov    esi, dword ptr [edi + 0d6h]
loc_1_8D23:
        cmp    esi, ebp
        jne    SHORT loc_1_8D68
        cmp    dword ptr [edi + 0e2h], ebp
        je     SHORT loc_1_8D72
        cmp    dword ptr [edi + 112h], ebp
        je     NEAR PTR loc_1_8DE8
        cmp    dword ptr [edi + 5ch], ebp
        jne    NEAR PTR loc_1_8DE8
        push   eax
        push   edi
        call   sub_1_81EF
        add    esp, 8
        push   edi
        call   sub_1_ABE1
        add    esp, 4
        cmp    dword ptr [edi + 0d6h], ebp
        je     NEAR PTR loc_1_8DF1
        jmp    NEAR PTR loc_1_8CDA
loc_1_8D68:
        or     eax, eax
        je     NEAR PTR loc_1_8DF1
        jmp    SHORT loc_1_8DE8
loc_1_8D72:
        push   eax
        call   sub_1_7AA0
        add    esp, 4
        mov    eax, dword ptr [edi + 60h]
        test   eax, 1100000h
        jne    SHORT loc_1_8DA6
        cmp    dword ptr [edi + 112h], 0
        je     SHORT loc_1_8D9D
        cmp    dword ptr [edi + 5ch], 0
        jne    SHORT loc_1_8D9D
        db  0C7h, 047h, 068h, 0D3h, 07Fh, 000h, 000h
        jmp    SHORT loc_1_8DF1
loc_1_8D9D:
        db  0C7h, 047h, 068h, 0CAh, 07Ch, 000h, 000h
        jmp    SHORT loc_1_8DF1
loc_1_8DA6:
        test   eax, 1000000h
        je     SHORT loc_1_8DCC
        and    eax, 0ffffffbfh
        mov    dword ptr [edi + 60h], eax
        inc    dword ptr [edi + 0a6h]
        push   1
        and    eax, 200000h
        push   eax
        push   edi
        call   sub_1_7998
        add    esp, 0ch
        jmp    SHORT loc_1_8DF5
loc_1_8DCC:
        test   eax, 100000h
        je     SHORT loc_1_8DF1
        and    eax, 0ffefffffh
        mov    dword ptr [edi + 60h], eax
        push   16h
        push   edi
        call   sub_1_78CF
        add    esp, 8
        jmp    SHORT loc_1_8DF1
loc_1_8DE8:
        push   eax
        call   sub_1_7AA0
        add    esp, 4
loc_1_8DF1:
        and    dword ptr [edi + 60h], 0ffffffbfh
loc_1_8DF5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_8DFA:
        push   esi
        push   edi
        db  081h, 03Dh, 040h, 036h, 001h, 000h, 040h, 036h, 001h, 000h
        je     NEAR PTR loc_1_8EB0
        db  0BFh, 040h, 036h, 001h, 000h
loc_1_8E11:
        db  08Bh, 035h, 040h, 036h, 001h, 000h
        mov    eax, esi
        sub    esi, 106h
        mov    eax, dword ptr [eax]
        db  0A3h, 040h, 036h, 001h, 000h
        mov    eax, dword ptr [esi + 106h]
        mov    dword ptr [eax + 4], edi
        test   byte ptr [esi + 0beh], 1
        jne    SHORT loc_1_8E97
loc_1_8E38:
        mov    eax, dword ptr [esi + 60h]
        test   ah, 7
        je     SHORT loc_1_8E97
        test   ah, 1
        je     SHORT loc_1_8E56
        and    eax, 0fffffeffh
        mov    dword ptr [esi + 60h], eax
        push   esi
        call   sub_1_8CA0
        add    esp, 4
loc_1_8E56:
        mov    eax, dword ptr [esi + 60h]
        test   ah, 4
        je     SHORT loc_1_8E75
        and    eax, 0fffffbffh
        mov    dword ptr [esi + 60h], eax
        inc    dword ptr [esi + 0a6h]
        push   esi
        call   sub_1_B37E
        add    esp, 4
loc_1_8E75:
        mov    eax, dword ptr [esi + 60h]
        test   ah, 2
        je     SHORT loc_1_8E8E
        and    eax, 0fffffdffh
        mov    dword ptr [esi + 60h], eax
        push   esi
        call   sub_1_ABE1
        add    esp, 4
loc_1_8E8E:
        test   byte ptr [esi + 0beh], 1
        je     SHORT loc_1_8E38
loc_1_8E97:
        and    dword ptr [esi + 60h], 0ffffffdfh
        push   esi
        call   sub_1_4BB1
        add    esp, 4
        db  039h, 03Dh, 040h, 036h, 001h, 000h
        jne    NEAR PTR loc_1_8E11
loc_1_8EB0:
        pop    edi
        pop    esi
        ret
sub_1_8EB3:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [edx + 60h]
        or     ecx, dword ptr [esp + 8]
        mov    dword ptr [edx + 60h], ecx
        test   cl, 20h
        jne    SHORT loc_1_8EF6
        or     ecx, 20h
        mov    dword ptr [edx + 60h], ecx
        inc    dword ptr [edx + 0a6h]
        db  0A1h, 044h, 036h, 001h, 000h
        lea    ecx, [edx + 106h]
        mov    dword ptr [eax], ecx
        db  0A1h, 044h, 036h, 001h, 000h
        mov    dword ptr [edx + 10ah], eax
        db  089h, 00Dh, 044h, 036h, 001h, 000h
        db  0C7h, 001h, 040h, 036h, 001h, 000h
loc_1_8EF6:
        ret
sub_1_8EF7:
        push   ebx
        push   esi
        push   edi
        db  0A1h, 000h, 038h, 001h, 000h
        test   al, 3
        je     NEAR PTR loc_1_8FC4
        test   al, 10h
        jne    NEAR PTR loc_1_8FC4
        or     eax, 10h
        db  0BEh, 048h, 039h, 001h, 000h
        db  0BFh, 020h, 037h, 001h, 000h
loc_1_8F1C:
        test   al, 1
        je     SHORT loc_1_8F62
        db  0A3h, 000h, 038h, 001h, 000h
        db  039h, 035h, 048h, 039h, 001h, 000h
        je     SHORT loc_1_8F5F
        db  08Bh, 01Dh, 048h, 039h, 001h, 000h
        mov    eax, ebx
        push   0
        mov    eax, dword ptr [eax]
        db  0A3h, 048h, 039h, 001h, 000h
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 4], esi
        push   dword ptr [ebx + 10h]
        push   dword ptr [ebx + 0ch]
        call   dword ptr [ebx + 8]
        add    esp, 0ch
        push   ebx
        call   sub_1_4723
        add    esp, 4
        db  0A1h, 000h, 038h, 001h, 000h
        jmp    SHORT loc_1_8F62
loc_1_8F5F:
        and    eax, 0fffffffeh
loc_1_8F62:
        test   al, 2
        je     SHORT loc_1_8FB4
        db  0A3h, 000h, 038h, 001h, 000h
        db  039h, 03Dh, 020h, 037h, 001h, 000h
        jne    SHORT loc_1_8F78
        and    eax, 0fffffffdh
        jmp    SHORT loc_1_8FB4
loc_1_8F78:
        db  08Bh, 01Dh, 020h, 037h, 001h, 000h
        mov    eax, ebx
        mov    ecx, 0
        mov    eax, dword ptr [eax]
        db  0A3h, 020h, 037h, 001h, 000h
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 4], edi
        mov    eax, dword ptr [ebx + 10h]
        or     eax, eax
        jne    SHORT loc_1_8F9B
        mov    ecx, dword ptr [ebx + 14h]
loc_1_8F9B:
        push   ecx
        push   eax
        push   dword ptr [ebx + 0ch]
        call   dword ptr [ebx + 8]
        add    esp, 0ch
        push   ebx
        call   sub_1_A7D4
        add    esp, 4
        db  0A1h, 000h, 038h, 001h, 000h
loc_1_8FB4:
        test   al, 3
        jne    NEAR PTR loc_1_8F1C
        and    eax, 0ffffffefh
        db  0A3h, 000h, 038h, 001h, 000h
loc_1_8FC4:
        call   sub_1_8DFA
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_8FCD:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 0eah]
        or     esi, esi
        je     SHORT loc_1_902A
        mov    dword ptr [eax + 0eah], 0
        push   dword ptr [eax + 8ch]
        add    eax, 7ch
        push   eax
        push   dword ptr [esi + 14h]
        call   sub_1_463E
        mov    eax, dword ptr [esp + 18h]
        add    esp, 0ch
        mov    dword ptr [esi + 10h], eax
        db  0A1h, 000h, 038h, 001h, 000h
        or     eax, 1
        db  0A3h, 000h, 038h, 001h, 000h
        db  0A1h, 04Ch, 039h, 001h, 000h
        mov    dword ptr [eax], esi
        db  0A1h, 04Ch, 039h, 001h, 000h
        mov    dword ptr [esi + 4], eax
        db  089h, 035h, 04Ch, 039h, 001h, 000h
        db  0C7h, 006h, 048h, 039h, 001h, 000h
loc_1_902A:
        pop    esi
        ret
sub_1_902C:
        sub    esp, 44h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 58h]
        sub    ebx, ebx
        test   byte ptr [edi + 40h], 80h
        jne    SHORT loc_1_9044
        jmp    NEAR PTR loc_1_92CB
loc_1_9044:
        mov    ecx, dword ptr [edi + 4ah]
        or     ecx, ecx
        mov    esi, dword ptr [esp + 5ch]
        je     NEAR PTR loc_1_914E
loc_1_9053:
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     NEAR PTR loc_1_913A
        cmp    byte ptr [eax + 0bch], 1
        jne    NEAR PTR loc_1_913A
        test   byte ptr [eax + 0beh], 1
        jne    NEAR PTR loc_1_913A
        mov    dl, byte ptr [esi]
        cmp    byte ptr [eax + 7ch], dl
        jne    NEAR PTR loc_1_913A
        mov    dl, byte ptr [esi + 1]
        cmp    byte ptr [eax + 7dh], dl
        jne    NEAR PTR loc_1_913A
        mov    dl, byte ptr [eax + 7eh]
        cmp    byte ptr [esi + 2], dl
        jne    NEAR PTR loc_1_913A
        mov    dl, byte ptr [eax + 7fh]
        cmp    byte ptr [esi + 3], dl
        jne    NEAR PTR loc_1_913A
        mov    dl, byte ptr [eax + 80h]
        cmp    byte ptr [esi + 4], dl
        jne    NEAR PTR loc_1_913A
        mov    dl, byte ptr [eax + 81h]
        cmp    byte ptr [esi + 5], dl
        jne    SHORT loc_1_913A
        mov    dl, byte ptr [eax + 82h]
        cmp    byte ptr [esi + 6], dl
        jne    SHORT loc_1_913A
        mov    dl, byte ptr [eax + 83h]
        cmp    byte ptr [esi + 7], dl
        jne    SHORT loc_1_913A
        mov    dl, byte ptr [eax + 84h]
        cmp    byte ptr [esi + 8], dl
        jne    SHORT loc_1_913A
        mov    dl, byte ptr [eax + 85h]
        cmp    byte ptr [esi + 9], dl
        jne    SHORT loc_1_913A
        mov    dl, byte ptr [eax + 86h]
        cmp    byte ptr [esi + 0ah], dl
        jne    SHORT loc_1_913A
        mov    dl, byte ptr [eax + 87h]
        cmp    byte ptr [esi + 0bh], dl
        jne    SHORT loc_1_913A
        mov    dl, byte ptr [eax + 88h]
        cmp    byte ptr [esi + 0ch], dl
        jne    SHORT loc_1_913A
        mov    dl, byte ptr [eax + 89h]
        cmp    byte ptr [esi + 0dh], dl
        jne    SHORT loc_1_913A
        mov    dl, byte ptr [esi + 0eh]
        cmp    byte ptr [eax + 8ah], dl
        jne    SHORT loc_1_913A
        mov    dl, byte ptr [esi + 0fh]
        cmp    byte ptr [eax + 8bh], dl
        jne    SHORT loc_1_913A
        mov    edx, dword ptr [esp + 60h]
        cmp    dword ptr [eax + 8ch], edx
        je     SHORT loc_1_9147
loc_1_913A:
        mov    ecx, dword ptr [ecx + 8]
        or     ecx, ecx
        jne    NEAR PTR loc_1_9053
        jmp    SHORT loc_1_9152
loc_1_9147:
        mov    ebx, 1
        jmp    SHORT loc_1_9152
loc_1_914E:
        mov    eax, dword ptr [esp + 1ch]
loc_1_9152:
        or     ebx, ebx
        je     SHORT loc_1_9175
        mov    ecx, dword ptr [eax + 0eah]
        inc    dword ptr [eax + 0a6h]
        test   byte ptr [ecx + 18h], 1
        jne    NEAR PTR loc_1_92CD
        or     dword ptr [eax + 60h], 4
        jmp    NEAR PTR loc_1_92CD
loc_1_9175:
        cmp    dword ptr [edi + 5ah], 0
        je     NEAR PTR loc_1_92CB
        call   sub_1_CE5D
        mov    ebx, eax
        call   sub_1_4735
        mov    ebp, eax
        or     ebx, ebx
        je     NEAR PTR loc_1_92B1
        or     ebp, ebp
        je     NEAR PTR loc_1_92BE
        mov    eax, dword ptr [edi + 5ah]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [edi + 5eh]
        mov    dword ptr [esp + 18h], eax
        push   dword ptr [esp + 60h]
        lea    eax, [esp + 34h]
        push   esi
        push   eax
        call   sub_1_460F
        add    esp, 0ch
        inc    word ptr [edi + 46h]
        mov    byte ptr [ebx + 0bch], 1
        mov    dword ptr [ebx + 0eah], ebp
        mov    al, byte ptr [edi + 42h]
        mov    byte ptr [ebx + 99h], al
        lea    eax, [esp + 20h]
        push   eax
        or     dword ptr [ebx + 60h], 4
        lea    eax, [esp + 14h]
        push   eax
        push   0
        push   0
        push   0
        lea    eax, [esp + 44h]
        push   0
        push   eax
        push   22h
        push   dword ptr [esp + 38h]
        call   dword ptr [esp + 38h]
        add    esp, 24h
        cmp    eax, 18h
        jne    NEAR PTR loc_1_9297
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [ebp + 0ch], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [ebp + 14h], eax
        mov    eax, 0fh
        mov    esi, dword ptr [edi + 4ah]
        or     esi, esi
        je     SHORT loc_1_9275
        mov    ecx, 3
loc_1_922E:
        mov    edx, dword ptr [esp + 10h]
        cmp    dword ptr [esi + 14h], edx
        jne    SHORT loc_1_923C
        test   byte ptr [esi + 0dh], cl
        je     SHORT loc_1_9245
loc_1_923C:
        mov    esi, dword ptr [esi + 8]
        or     esi, esi
        jne    SHORT loc_1_922E
        jmp    SHORT loc_1_9271
loc_1_9245:
        mov    dword ptr [ebx + 0a6h], 1
        push   dword ptr [esp + 28h]
        push   ebx
        push   esi
        call   sub_1_4E04
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_9265
        sub    esi, esi
        jmp    SHORT loc_1_9271
loc_1_9265:
        mov    dword ptr [esi], ebx
        mov    dword ptr [ebx + 102h], esi
        inc    word ptr [esi + 0eh]
loc_1_9271:
        or     esi, esi
        jne    SHORT loc_1_928A
loc_1_9275:
        push   eax
        push   ebx
        call   sub_1_8FCD
        add    esp, 8
        push   ebx
        call   sub_1_CF22
        add    esp, 4
        sub    ebx, ebx
loc_1_928A:
        push   edi
        call   sub_1_4325
        add    esp, 4
        mov    eax, ebx
        jmp    SHORT loc_1_92CD
loc_1_9297:
        push   ebp
        call   sub_1_4723
        add    esp, 4
        push   ebx
        call   sub_1_CF22
        add    esp, 4
        push   edi
        call   sub_1_4325
        jmp    SHORT loc_1_92C8
loc_1_92B1:
        or     ebp, ebp
        je     SHORT loc_1_92BE
        push   ebp
        call   sub_1_4723
        add    esp, 4
loc_1_92BE:
        or     ebx, ebx
        je     SHORT loc_1_92CB
        push   ebx
        call   sub_1_CF22
loc_1_92C8:
        add    esp, 4
loc_1_92CB:
        sub    eax, eax
loc_1_92CD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 44h
        ret
sub_1_92D5:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0ch]
        test   byte ptr [eax + 11h], 4
        jne    SHORT loc_1_9300
        cmp    byte ptr [esi + 0bch], 0ah
        jne    SHORT loc_1_92F7
        mov    dword ptr [esi + 0c8h], 3e8h
loc_1_92F7:
        push   esi
        call   sub_1_ABE1
        add    esp, 4
loc_1_9300:
        push   esi
        call   sub_1_4BB1
        add    esp, 4
        pop    esi
        ret
sub_1_930B:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 24h]
        mov    ebx, dword ptr [esp + 28h]
        sub    ebx, dword ptr [esi + 0ch]
        mov    eax, dword ptr [esi + 1ch]
        cmp    ebx, eax
        mov    dword ptr [esp + 14h], eax
        jne    NEAR PTR loc_1_93F4
        cmp    byte ptr [esi + 0bch], 4
        jne    NEAR PTR loc_1_93F4
        mov    edi, dword ptr [esi + 40h]
        lea    eax, [esi + 40h]
        mov    edx, dword ptr [esp + 28h]
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 44h], eax
        mov    dword ptr [esi + 10h], edx
        sub    ebp, ebp
        mov    dword ptr [esi + 0ch], edx
        mov    dword ptr [esi + 30h], 0
        mov    dword ptr [esi + 34h], 0
        mov    dword ptr [esi + 38h], 0
        mov    dword ptr [esi + 3ch], 0
        mov    dword ptr [esi + 1ch], 0
loc_1_9373:
        mov    ebx, edi
        mov    edi, dword ptr [edi]
        mov    dword ptr [ebx + 10h], ebp
        mov    edx, dword ptr [ebx + 18h]
        dec    edx
        mov    dword ptr [ebx + 18h], edx
        cmp    edx, ebp
        mov    ecx, ebp
        setl   cl
        cmp    edx, ebp
        mov    edx, ebp
        setle  dl
        add    ecx, edx
        cmp    ecx, ebp
        je     SHORT loc_1_93EB
        mov    ecx, dword ptr [ebx + 2ch]
        cmp    ecx, ebp
        je     SHORT loc_1_93A1
        mov    dword ptr [ecx], ebp
        mov    dword ptr [ebx + 2ch], ebp
loc_1_93A1:
        db  08Bh, 00Dh, 030h, 038h, 001h, 000h
        sub    ecx, dword ptr [ebx + 30h]
        add    dword ptr [esi + 0fah], ecx
        mov    ecx, dword ptr [esi + 0f2h]
        mov    edx, dword ptr [ebx + 14h]
        add    edx, ecx
        cmp    ecx, edx
        mov    ecx, ebp
        seta   cl
        mov    dword ptr [esi + 0f2h], edx
        add    dword ptr [esi + 0f6h], ecx
        db  08Bh, 00Dh, 024h, 037h, 001h, 000h
        mov    dword ptr [ecx], ebx
        db  08Bh, 00Dh, 024h, 037h, 001h, 000h
        mov    dword ptr [ebx + 4], ecx
        db  089h, 01Dh, 024h, 037h, 001h, 000h
        db  0C7h, 003h, 020h, 037h, 001h, 000h
loc_1_93EB:
        cmp    edi, eax
        jne    SHORT loc_1_9373
        jmp    NEAR PTR loc_1_9573
loc_1_93F4:
        mov    dword ptr [esp + 18h], ebx
        mov    ecx, dword ptr [esi + 40h]
        mov    edi, ecx
        mov    dword ptr [esp + 10h], 0
        or     ebx, ebx
        mov    eax, dword ptr [esp + 1ch]
        je     SHORT loc_1_9456
loc_1_940D:
        lea    edx, [esi + 40h]
        cmp    edi, edx
        je     SHORT loc_1_9456
        mov    eax, edi
        mov    edx, dword ptr [edi + 20h]
        cmp    ebx, edx
        jb     SHORT loc_1_9429
        sub    ebx, edx
        mov    dword ptr [esp + 10h], edi
        mov    edi, dword ptr [edi]
        jne    SHORT loc_1_940D
        jmp    SHORT loc_1_9456
loc_1_9429:
        mov    edx, dword ptr [eax + 28h]
        sub    dword ptr [eax + 20h], ebx
        mov    ebp, dword ptr [edx + 0ch]
        sub    ebp, dword ptr [eax + 24h]
        cmp    ebx, ebp
        jae    SHORT loc_1_9443
        mov    edx, dword ptr [eax + 24h]
        add    edx, ebx
        mov    dword ptr [eax + 24h], edx
        jmp    SHORT loc_1_9454
loc_1_9443:
        mov    edx, dword ptr [edx]
        sub    ebx, ebp
        mov    ebp, dword ptr [edx + 0ch]
        cmp    ebx, ebp
        jae    SHORT loc_1_9443
        mov    dword ptr [eax + 24h], ebx
        mov    dword ptr [eax + 28h], edx
loc_1_9454:
        sub    ebx, ebx
loc_1_9456:
        mov    edx, dword ptr [esp + 28h]
        sub    edx, dword ptr [esi + 10h]
        or     edx, edx
        jle    SHORT loc_1_94A4
        lea    edx, [esi + 40h]
        cmp    edi, edx
        je     SHORT loc_1_9481
        mov    dword ptr [esi + 30h], edi
        mov    edx, dword ptr [edi + 28h]
        mov    dword ptr [esi + 34h], edx
        mov    edx, dword ptr [edi + 24h]
        mov    eax, edi
        mov    dword ptr [esi + 38h], edx
        mov    edx, dword ptr [edi + 20h]
        mov    dword ptr [esi + 3ch], edx
        jmp    SHORT loc_1_949D
loc_1_9481:
        mov    dword ptr [esi + 30h], 0
        mov    dword ptr [esi + 34h], 0
        mov    dword ptr [esi + 38h], 0
        mov    dword ptr [esi + 3ch], 0
loc_1_949D:
        mov    edx, dword ptr [esp + 28h]
        mov    dword ptr [esi + 10h], edx
loc_1_94A4:
        sub    ebx, dword ptr [esp + 18h]
        add    ebx, dword ptr [esp + 14h]
        mov    dword ptr [esi + 1ch], ebx
        mov    edx, dword ptr [esp + 28h]
        mov    dword ptr [esi + 0ch], edx
        cmp    dword ptr [esp + 10h], 0
        je     NEAR PTR loc_1_9580
        lea    edx, [esi + 40h]
        cmp    edi, edx
        jne    SHORT loc_1_94CF
        mov    dword ptr [edx], edx
        mov    dword ptr [esi + 44h], edx
        jmp    SHORT loc_1_94D8
loc_1_94CF:
        mov    dword ptr [esi + 40h], edi
        lea    edx, [esi + 40h]
        mov    dword ptr [edi + 4], edx
loc_1_94D8:
        cmp    ecx, edi
        je     NEAR PTR loc_1_9573
loc_1_94E0:
        mov    ebp, dword ptr [ecx]
        mov    dword ptr [ecx + 10h], 0
        mov    ebx, dword ptr [ecx + 18h]
        dec    ebx
        mov    dword ptr [ecx + 18h], ebx
        or     ebx, ebx
        mov    edx, 0
        setl   dl
        or     ebx, ebx
        mov    ebx, 0
        setle  bl
        add    edx, ebx
        je     SHORT loc_1_9569
        mov    edx, dword ptr [ecx + 2ch]
        or     edx, edx
        je     SHORT loc_1_951C
        mov    dword ptr [edx], 0
        mov    dword ptr [ecx + 2ch], 0
loc_1_951C:
        db  08Bh, 015h, 030h, 038h, 001h, 000h
        sub    edx, dword ptr [eax + 30h]
        add    dword ptr [esi + 0fah], edx
        mov    ebx, dword ptr [esi + 0f2h]
        mov    edx, dword ptr [eax + 14h]
        add    edx, ebx
        mov    dword ptr [esi + 0f2h], edx
        cmp    ebx, edx
        mov    edx, 0
        seta   dl
        add    dword ptr [esi + 0f6h], edx
        db  08Bh, 015h, 024h, 037h, 001h, 000h
        mov    dword ptr [edx], ecx
        db  08Bh, 015h, 024h, 037h, 001h, 000h
        mov    dword ptr [ecx + 4], edx
        db  089h, 00Dh, 024h, 037h, 001h, 000h
        db  0C7h, 001h, 020h, 037h, 001h, 000h
loc_1_9569:
        mov    ecx, ebp
        cmp    ebp, edi
        jne    NEAR PTR loc_1_94E0
loc_1_9573:
        db  0A1h, 000h, 038h, 001h, 000h
        or     eax, 2
        db  0A3h, 000h, 038h, 001h, 000h
loc_1_9580:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
_LTEXT ends
        end
