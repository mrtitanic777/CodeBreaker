; unimodem_pcode_03_02.asm -- UNIMODEM object 3 segment _PTEXT  (part 2/2)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_3_54CC:
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 48h]
        or     ebp, ebp
        je     NEAR PTR loc_3_5647
        cmp    dword ptr [esp + 4ch], 34h
        jb     NEAR PTR loc_3_5647
        mov    ebx, dword ptr [esp + 44h]
        mov    eax, dword ptr [ebx + 29ch]
        or     eax, eax
        je     NEAR PTR loc_3_5647
        cmp    word ptr [ebp + 4], 1
        jne    NEAR PTR loc_3_5647
        cmp    dword ptr [ebp], 34h
        jb     NEAR PTR loc_3_5647
        lea    ecx, [esp + 10h]
        push   ecx
        push   eax
        db  0E8h, 044h, 0AFh, 0FCh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_3_5528
        jmp    NEAR PTR loc_3_5647
loc_3_5528:
        lea    esi, [ebp + 8]
        lea    eax, [esp + 10h]
        push   eax
        push   esi
        db  0E8h, 09Dh, 0B1h, 0FCh, 0FFh
        add    esp, 8
        lea    eax, [esp + 10h]
        push   -1
        push   eax
        push   dword ptr [ebx + 29ch]
        db  0E8h, 00Dh, 0AFh, 0FCh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_5557
        jmp    NEAR PTR loc_3_5647
loc_3_5557:
        lea    eax, [esp + 10h]
        push   eax
        call   sub_3_34E7
        add    esp, 4
        mov    edi, dword ptr [ebx + 29ch]
        push   0
        push   0
        push   5
        push   edi
        db  0E8h, 00Ah, 0AFh, 0FCh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_3_5582
        jmp    NEAR PTR loc_3_5647
loc_3_5582:
        push   0
        push   0
        push   3
        push   edi
        db  0E8h, 0F2h, 0AEh, 0FCh, 0FFh
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_3_559A
        jmp    NEAR PTR loc_3_5647
loc_3_559A:
        lea    edi, [ebx + 555h]
        mov    ecx, 7
        rep movsd
        cmp    dword ptr [ebp + 24h], 6
        jne    NEAR PTR loc_3_5640
        mov    eax, dword ptr [ebp + 2ch]
        cmp    eax, 2dh
        jb     NEAR PTR loc_3_5640
        mov    ecx, dword ptr [ebp + 28h]
        add    eax, ecx
        cmp    eax, dword ptr [ebp]
        ja     SHORT loc_3_5640
        lea    edi, [ecx + ebp]
        lea    edx, [ebx + 581h]
        mov    ecx, dword ptr [ebx + 528h]
        lea    esi, [ebx + 4f8h]
        mov    eax, dword ptr [edi + 10h]
        cmp    ecx, eax
        jae    SHORT loc_3_55E8
        mov    dword ptr [edx + 10h], ecx
        jmp    SHORT loc_3_55F7
loc_3_55E8:
        cmp    eax, 1
        mov    dword ptr [edx + 10h], 1
        jb     SHORT loc_3_55F7
        mov    dword ptr [edx + 10h], eax
loc_3_55F7:
        mov    eax, dword ptr [edi + 14h]
        mov    ecx, dword ptr [esi + 34h]
        cmp    eax, ecx
        jbe    SHORT loc_3_5606
        mov    dword ptr [edx + 14h], ecx
        jmp    SHORT loc_3_5609
loc_3_5606:
        mov    dword ptr [edx + 14h], eax
loc_3_5609:
        mov    ecx, dword ptr [edi + 18h]
        mov    eax, 1
        shl    eax, cl
        test   dword ptr [esi + 38h], eax
        je     SHORT loc_3_561B
        mov    dword ptr [edx + 18h], ecx
loc_3_561B:
        mov    ecx, dword ptr [edi + 1ch]
        mov    eax, 1
        shl    eax, cl
        test   dword ptr [esi + 3ch], eax
        je     SHORT loc_3_562D
        mov    dword ptr [edx + 1ch], ecx
loc_3_562D:
        mov    eax, dword ptr [edi + 20h]
        and    eax, dword ptr [esi + 40h]
        mov    dword ptr [edx + 20h], eax
        mov    dword ptr [ebx + 5c2h], 0
loc_3_5640:
        mov    eax, 1
        jmp    SHORT loc_3_5649
loc_3_5647:
        sub    eax, eax
loc_3_5649:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
sub_3_5651:
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 40h]
        mov    dword ptr [esp + 38h], 0
        mov    eax, dword ptr [ebx + 5a1h]
        mov    dword ptr [esp + 2ch], eax
        mov    eax, dword ptr [ebx + 538h]
        mov    dword ptr [esp + 28h], eax
        mov    dword ptr [esp + 24h], 1
        push   1
        push   9ah
        db  0E8h, 03Ch, 0AEh, 0FCh, 0FFh
        mov    dword ptr [esp + 18h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_3_56AC
        push   0
        push   dword ptr [esp + 14h]
        db  0E8h, 02Eh, 0AEh, 0FCh, 0FFh
        add    esp, 8
        sub    eax, eax
        jmp    NEAR PTR loc_3_5E91
loc_3_56AC:
        mov    eax, dword ptr [esp + 10h]
        add    eax, 29h
        mov    dword ptr [esp + 18h], eax
        add    eax, 29h
        mov    dword ptr [esp + 20h], eax
        mov    ebp, eax
        add    ebp, 29h
        db  068h, 0B8h, 00Ah, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 06Dh, 0ADh, 0FCh, 0FFh
        lea    eax, [esp + 24h]
        add    esp, 8
        push   eax
        db  068h, 0B8h, 00Ah, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 04Dh, 0ADh, 0FCh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_5707
        push   0
        push   dword ptr [esp + 14h]
        db  0E8h, 0D3h, 0ADh, 0FCh, 0FFh
        add    esp, 8
        sub    eax, eax
        jmp    NEAR PTR loc_3_5E91
loc_3_5707:
        lea    eax, [esp + 14h]
        push   eax
        db  068h, 0ACh, 00Ah, 000h, 000h
        push   dword ptr [ebx + 2e4h]
        db  0E8h, 00Ch, 0ADh, 0FCh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_5744
        push   dword ptr [esp + 1ch]
        db  0E8h, 004h, 0ADh, 0FCh, 0FFh
        add    esp, 4
        push   0
        push   dword ptr [esp + 14h]
        db  0E8h, 096h, 0ADh, 0FCh, 0FFh
        add    esp, 8
        sub    eax, eax
        jmp    NEAR PTR loc_3_5E91
loc_3_5744:
        mov    dword ptr [esp + 34h], 28h
        lea    eax, [esp + 34h]
        push   eax
        lea    eax, [esp + 34h]
        push   dword ptr [esp + 14h]
        push   eax
        push   0
        db  068h, 0C8h, 00Ah, 000h, 000h
        push   dword ptr [esp + 28h]
        db  0E8h, 0DEh, 0ACh, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_3_5E67
        cmp    dword ptr [esp + 30h], 1
        jne    NEAR PTR loc_3_5E67
        push   0
        push   0
        push   0
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 20h]
        call   sub_3_2F1C
        mov    dword ptr [esp + 48h], 28h
        lea    eax, [esp + 48h]
        add    esp, 14h
        push   eax
        lea    eax, [esp + 34h]
        push   dword ptr [esp + 14h]
        push   eax
        push   0
        db  068h, 0D0h, 00Ah, 000h, 000h
        push   dword ptr [esp + 28h]
        db  0E8h, 08Ch, 0ACh, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_3_5E67
        cmp    dword ptr [esp + 30h], 1
        jne    NEAR PTR loc_3_5E67
        push   0
        push   0
        push   0
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 20h]
        call   sub_3_2F1C
        mov    edi, dword ptr [esp + 2ch]
        add    esp, 14h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, dword ptr [esp + 10h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cmp    dword ptr [ebx + 528h], 0
        je     SHORT loc_3_5850
        lea    eax, [esp + 24h]
        push   dword ptr [esp + 10h]
        push   eax
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 24h]
        push   dword ptr [ebx + 591h]
        db  068h, 0DCh, 00Ah, 000h, 000h
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 30h]
        push   dword ptr [ebx + 2e4h]
        call   sub_3_5E99
        add    esp, 24h
        or     eax, eax
        je     NEAR PTR loc_3_5E67
loc_3_5850:
        cmp    dword ptr [ebx + 52ch], 0
        je     SHORT loc_3_58AF
        mov    ecx, dword ptr [ebx + 595h]
        mov    esi, dword ptr [ebx + 5aeh]
        mov    eax, ecx
        sub    edx, edx
        div    esi
        cmp    edx, 1
        sbb    edi, edi
        mov    eax, ecx
        sub    edx, edx
        div    esi
        lea    ecx, [esp + 24h]
        push   dword ptr [esp + 10h]
        push   ecx
        lea    eax, [edi + eax + 1]
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 24h]
        push   eax
        db  068h, 0F0h, 00Ah, 000h, 000h
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 30h]
        push   dword ptr [ebx + 2e4h]
        call   sub_3_5E99
        add    esp, 24h
        or     eax, eax
        je     NEAR PTR loc_3_5E67
loc_3_58AF:
        cmp    dword ptr [ebx + 530h], 0
        je     NEAR PTR loc_3_5966
        db  0BFh, 004h, 00Bh, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebx + 599h]
        or     eax, eax
        je     SHORT loc_3_58F6
        cmp    eax, 1
        je     SHORT loc_3_58FD
        cmp    eax, 2
        je     SHORT loc_3_5904
        jmp    SHORT loc_3_5934
loc_3_58F6:
        db  0BFh, 084h, 00Bh, 000h, 000h
        jmp    SHORT loc_3_5909
loc_3_58FD:
        db  0BFh, 08Ch, 00Bh, 000h, 000h
        jmp    SHORT loc_3_5909
loc_3_5904:
        db  0BFh, 094h, 00Bh, 000h, 000h
loc_3_5909:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_3_5934:
        lea    eax, [esp + 24h]
        push   dword ptr [esp + 10h]
        push   eax
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 24h]
        push   0
        push   ebp
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 30h]
        push   dword ptr [ebx + 2e4h]
        call   sub_3_5E99
        add    esp, 24h
        or     eax, eax
        je     NEAR PTR loc_3_5E67
loc_3_5966:
        cmp    dword ptr [ebx + 534h], 0
        je     NEAR PTR loc_3_5A29
        db  0BFh, 014h, 00Bh, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebx + 59dh]
        or     eax, eax
        je     SHORT loc_3_59B2
        cmp    eax, 1
        je     SHORT loc_3_59B9
        cmp    eax, 2
        je     SHORT loc_3_59C0
        cmp    eax, 3
        je     SHORT loc_3_59C7
        jmp    SHORT loc_3_59F7
loc_3_59B2:
        db  0BFh, 0D0h, 00Bh, 000h, 000h
        jmp    SHORT loc_3_59CC
loc_3_59B9:
        db  0BFh, 09Ch, 00Bh, 000h, 000h
        jmp    SHORT loc_3_59CC
loc_3_59C0:
        db  0BFh, 0D8h, 00Bh, 000h, 000h
        jmp    SHORT loc_3_59CC
loc_3_59C7:
        db  0BFh, 0A4h, 00Bh, 000h, 000h
loc_3_59CC:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_3_59F7:
        lea    eax, [esp + 24h]
        push   dword ptr [esp + 10h]
        push   eax
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 24h]
        push   0
        push   ebp
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 30h]
        push   dword ptr [ebx + 2e4h]
        call   sub_3_5E99
        add    esp, 24h
        or     eax, eax
        je     NEAR PTR loc_3_5E67
loc_3_5A29:
        test   byte ptr [esp + 28h], 2
        je     NEAR PTR loc_3_5B1A
        db  0BFh, 02Ch, 00Bh, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 2ch]
        and    eax, 0eh
        cmp    eax, 2
        je     SHORT loc_3_5A7A
        cmp    eax, 6
        je     SHORT loc_3_5AB8
        cmp    eax, 0ah
        je     SHORT loc_3_5A81
        cmp    eax, 0eh
        je     SHORT loc_3_5A88
        db  0BFh, 0D0h, 00Bh, 000h, 000h
        jmp    SHORT loc_3_5ABD
loc_3_5A7A:
        db  0BFh, 0D8h, 00Bh, 000h, 000h
        jmp    SHORT loc_3_5ABD
loc_3_5A81:
        db  0BFh, 0B4h, 00Bh, 000h, 000h
        jmp    SHORT loc_3_5ABD
loc_3_5A88:
        db  0BFh, 0B4h, 00Bh, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_3_5AB8:
        db  0BFh, 0ACh, 00Bh, 000h, 000h
loc_3_5ABD:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 24h]
        push   dword ptr [esp + 10h]
        push   eax
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 24h]
        push   0
        push   ebp
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 30h]
        push   dword ptr [ebx + 2e4h]
        call   sub_3_5E99
        add    esp, 24h
        or     eax, eax
        je     NEAR PTR loc_3_5E67
loc_3_5B1A:
        test   byte ptr [esp + 28h], 1
        je     NEAR PTR loc_3_5BB7
        db  0BFh, 03Ch, 00Bh, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BFh, 0D8h, 00Bh, 000h, 000h
        test   byte ptr [esp + 2ch], 1
        jne    SHORT loc_3_5B5A
        db  0BFh, 0D0h, 00Bh, 000h, 000h
loc_3_5B5A:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 24h]
        push   dword ptr [esp + 10h]
        push   eax
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 24h]
        push   0
        push   ebp
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 30h]
        push   dword ptr [ebx + 2e4h]
        call   sub_3_5E99
        add    esp, 24h
        or     eax, eax
        je     NEAR PTR loc_3_5E67
loc_3_5BB7:
        test   byte ptr [esp + 28h], 30h
        je     NEAR PTR loc_3_5C7A
        db  0BFh, 020h, 00Bh, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 2ch]
        and    eax, 30h
        cmp    eax, 10h
        je     SHORT loc_3_5C03
        cmp    eax, 20h
        je     SHORT loc_3_5C18
        cmp    eax, 30h
        je     SHORT loc_3_5C0A
        db  0BFh, 0D0h, 00Bh, 000h, 000h
        jmp    SHORT loc_3_5C1D
loc_3_5C03:
        db  0BFh, 0C0h, 00Bh, 000h, 000h
        jmp    SHORT loc_3_5C1D
loc_3_5C0A:
        test   byte ptr [esp + 28h], 10h
        je     SHORT loc_3_5C18
        db  0BFh, 0C0h, 00Bh, 000h, 000h
        jmp    SHORT loc_3_5C1D
loc_3_5C18:
        db  0BFh, 0C8h, 00Bh, 000h, 000h
loc_3_5C1D:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 24h]
        push   dword ptr [esp + 10h]
        push   eax
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 24h]
        push   0
        push   ebp
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 30h]
        push   dword ptr [ebx + 2e4h]
        call   sub_3_5E99
        add    esp, 24h
        or     eax, eax
        je     NEAR PTR loc_3_5E67
loc_3_5C7A:
        test   byte ptr [esp + 28h], 40h
        je     NEAR PTR loc_3_5D2E
        db  0BFh, 048h, 00Bh, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        test   byte ptr [esp + 2ch], 40h
        je     SHORT loc_3_5CCC
        test   byte ptr [esp + 29h], 4
        je     SHORT loc_3_5CC5
        test   byte ptr [esp + 2dh], 4
        je     SHORT loc_3_5CC5
        db  0BFh, 064h, 00Bh, 000h, 000h
        jmp    SHORT loc_3_5CD1
loc_3_5CC5:
        db  0BFh, 054h, 00Bh, 000h, 000h
        jmp    SHORT loc_3_5CD1
loc_3_5CCC:
        db  0BFh, 05Ch, 00Bh, 000h, 000h
loc_3_5CD1:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 24h]
        push   dword ptr [esp + 10h]
        push   eax
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 24h]
        push   0
        push   ebp
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 30h]
        push   dword ptr [ebx + 2e4h]
        call   sub_3_5E99
        add    esp, 24h
        or     eax, eax
        je     NEAR PTR loc_3_5E67
loc_3_5D2E:
        test   byte ptr [esp + 28h], 80h
        je     NEAR PTR loc_3_5DCB
        db  0BFh, 070h, 00Bh, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BFh, 0D8h, 00Bh, 000h, 000h
        test   byte ptr [esp + 2ch], 80h
        jne    SHORT loc_3_5D6E
        db  0BFh, 0D0h, 00Bh, 000h, 000h
loc_3_5D6E:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebp
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 24h]
        push   dword ptr [esp + 10h]
        push   eax
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 24h]
        push   0
        push   ebp
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 30h]
        push   dword ptr [ebx + 2e4h]
        call   sub_3_5E99
        add    esp, 24h
        or     eax, eax
        je     NEAR PTR loc_3_5E67
loc_3_5DCB:
        test   byte ptr [esp + 29h], 2
        je     SHORT loc_3_5E30
        db  0BFh, 0A4h, 008h, 000h, 000h
        test   byte ptr [esp + 2dh], 2
        jne    SHORT loc_3_5DE3
        db  0BFh, 0B0h, 008h, 000h, 000h
loc_3_5DE3:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 24h]
        push   dword ptr [esp + 10h]
        push   eax
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 24h]
        push   0
        push   ebp
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 30h]
        push   dword ptr [ebx + 2e4h]
        call   sub_3_5E99
        add    esp, 24h
        or     eax, eax
        je     SHORT loc_3_5E67
loc_3_5E30:
        lea    eax, [esp + 24h]
        push   dword ptr [esp + 10h]
        push   eax
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 24h]
        push   0
        push   0
        push   dword ptr [esp + 34h]
        push   dword ptr [esp + 30h]
        push   dword ptr [ebx + 2e4h]
        call   sub_3_5E99
        add    esp, 24h
        or     eax, eax
        je     SHORT loc_3_5E67
        mov    dword ptr [esp + 38h], 1
loc_3_5E67:
        push   dword ptr [esp + 1ch]
        db  0E8h, 0C0h, 0A5h, 0FCh, 0FFh
        add    esp, 4
        push   dword ptr [esp + 14h]
        db  0E8h, 0B4h, 0A5h, 0FCh, 0FFh
        add    esp, 4
        push   0
        push   dword ptr [esp + 14h]
        db  0E8h, 046h, 0A6h, 0FCh, 0FFh
        mov    eax, dword ptr [esp + 40h]
        add    esp, 8
loc_3_5E91:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 2ch
        ret
sub_3_5E99:
        sub    esp, 270h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 290h]
        or     esi, esi
        je     NEAR PTR loc_3_60D2
        mov    dword ptr [esp + 10h], 28h
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 58h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   0
        push   esi
        push   dword ptr [esp + 29ch]
        db  0E8h, 070h, 0A5h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_3_5F0A
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     NEAR PTR loc_3_638C
        push   esi
        db  00Fh, 0B7h, 005h, 0C6h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 086h, 0A6h, 0FCh, 0FFh
        add    esp, 0ch
        jmp    NEAR PTR loc_3_638C
loc_3_5F0A:
        cmp    dword ptr [esp + 14h], 1
        je     SHORT loc_3_5F18
        sub    eax, eax
        jmp    NEAR PTR loc_3_6391
loc_3_5F18:
        db  0BFh, 0E8h, 00Bh, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 80h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 1ch]
        push   eax
        push   dword ptr [esp + 298h]
        call   sub_3_30F5
        lea    edi, [esp + 24h]
        add    esp, 8
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 180h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esp + 18h], eax
        lea    eax, [esp + 80h]
        push   1
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 34h]
        push   eax
        lea    eax, [esp + 64h]
        push   eax
        call   sub_3_2F1C
        add    esp, 14h
        or     eax, eax
        jne    SHORT loc_3_5FB1
        sub    eax, eax
        jmp    NEAR PTR loc_3_6391
loc_3_5FB1:
        mov    ebx, dword ptr [esp + 2a4h]
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, dword ptr [esp + 29ch]
        lea    edx, [ecx - 1]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edx, [edx + ecx - 1]
        lea    edi, [esp + 28h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edx, [edx + ecx - 1]
        cmp    edx, 28h
        jbe    NEAR PTR loc_3_609E
        mov    edi, dword ptr [esp + 29ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 1ch]
        mov    ebp, dword ptr [esp + 2a0h]
        push   eax
        push   dword ptr [ebp]
        call   sub_3_30F5
        add    esp, 8
        inc    dword ptr [ebp]
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        push   ebx
        push   1
        lea    eax, [esp + 28h]
        push   0
        push   eax
        push   dword ptr [esp + 2a0h]
        db  0E8h, 0E6h, 0A3h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_3_6078
        sub    eax, eax
        jmp    NEAR PTR loc_3_6391
loc_3_6078:
        mov    edi, dword ptr [esp + 298h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_3_609E:
        lea    edi, [esp + 28h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        jmp    NEAR PTR loc_3_638C
loc_3_60D2:
        mov    edi, dword ptr [esp + 29ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    ebx, dword ptr [esp + 2a4h]
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 1ch]
        mov    ebp, dword ptr [esp + 2a0h]
        push   eax
        push   dword ptr [ebp]
        call   sub_3_30F5
        add    esp, 8
        inc    dword ptr [ebp]
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        push   ebx
        push   1
        lea    eax, [esp + 28h]
        push   0
        push   eax
        push   dword ptr [esp + 2a0h]
        db  0E8h, 007h, 0A3h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_3_6157
        sub    eax, eax
        jmp    NEAR PTR loc_3_6391
loc_3_6157:
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 18h]
        push   0
        push   eax
        push   0
        db  068h, 0DCh, 00Bh, 000h, 000h
        push   dword ptr [esp + 298h]
        db  0E8h, 0D2h, 0A2h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_3_6182
        jmp    NEAR PTR loc_3_638C
loc_3_6182:
        cmp    dword ptr [esp + 14h], 1
        je     SHORT loc_3_6190
        sub    eax, eax
        jmp    NEAR PTR loc_3_6391
loc_3_6190:
        cmp    dword ptr [esp + 10h], 1
        jne    SHORT loc_3_619C
        jmp    NEAR PTR loc_3_638C
loc_3_619C:
        mov    edi, dword ptr [esp + 29ch]
        push   1
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edi, dword ptr [esp + 29ch]
        lea    edx, [ecx - 1]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edx, [edx + ecx - 1]
        add    edx, dword ptr [esp + 14h]
        inc    edx
        push   edx
        db  0E8h, 0F4h, 0A2h, 0FCh, 0FFh
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_3_61E4
        sub    eax, eax
        jmp    NEAR PTR loc_3_6391
loc_3_61E4:
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 18h]
        push   ebx
        push   eax
        push   0
        db  068h, 0DCh, 00Bh, 000h, 000h
        push   dword ptr [esp + 298h]
        db  0E8h, 046h, 0A2h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_3_621B
        push   0
        push   ebx
        db  0E8h, 0BFh, 0A2h, 0FCh, 0FFh
        add    esp, 8
        sub    eax, eax
        jmp    NEAR PTR loc_3_6391
loc_3_621B:
        mov    edi, dword ptr [esp + 298h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   dword ptr [esp + 29ch]
        push   ebx
        call   sub_3_3146
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_62B6
        mov    edi, dword ptr [esp + 298h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 10h]
        mov    edi, dword ptr [esp + 298h]
        push   eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ebx + ecx - 1]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   0
        db  068h, 0DCh, 00Bh, 000h, 000h
        push   dword ptr [esp + 298h]
        db  0E8h, 0ABh, 0A1h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        je     SHORT loc_3_62B6
        push   0
        push   ebx
        db  0E8h, 024h, 0A2h, 0FCh, 0FFh
        add    esp, 8
        sub    eax, eax
        jmp    NEAR PTR loc_3_6391
loc_3_62B6:
        mov    edi, dword ptr [esp + 29ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        push   dword ptr [esp + 2a0h]
        repne scasb
        not    ecx
        mov    edi, dword ptr [esp + 2a4h]
        lea    edx, [ecx - 1]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        sub    edx, ecx
        add    edx, ebx
        push   edx
        call   sub_3_3146
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_6337
        mov    edi, dword ptr [esp + 29ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_3_6337:
        lea    eax, [esp + 1ch]
        push   eax
        push   dword ptr [ebp]
        call   sub_3_30F5
        add    esp, 8
        inc    dword ptr [ebp]
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        push   ebx
        push   1
        lea    eax, [esp + 28h]
        push   0
        push   eax
        push   dword ptr [esp + 2a0h]
        db  0E8h, 0E2h, 0A0h, 0FCh, 0FFh
        add    esp, 18h
        or     eax, eax
        push   0
        push   ebx
        je     SHORT loc_3_6384
        db  0E8h, 053h, 0A1h, 0FCh, 0FFh
        add    esp, 8
        sub    eax, eax
        jmp    SHORT loc_3_6391
loc_3_6384:
        db  0E8h, 047h, 0A1h, 0FCh, 0FFh
        add    esp, 8
loc_3_638C:
        mov    eax, 1
loc_3_6391:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 270h
        ret
        db  000h, 000h, 000h, 000h
sub_3_63A0:
        sub    esp, 10ch
        push   esi
        push   edi
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        jne    NEAR PTR loc_3_646A
        lea    eax, [esp + 8]
        push   eax
        db  0E8h, 095h, 0A1h, 0FCh, 0FFh
        mov    edi, eax
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [esp + 14h]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [esp + eax + 14h], 0
        db  0BFh, 0ECh, 00Bh, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [esp + 14h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [esp + 14h]
        push   1
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        db  0E8h, 0ADh, 09Fh, 0FCh, 0FFh
        or     eax, eax
        je     SHORT loc_3_643B
        sub    eax, eax
        jmp    SHORT loc_3_6482
loc_3_643B:
        push   0
        push   dword ptr [esp + 11ch]
        db  0E8h, 02Fh, 0A1h, 0FCh, 0FFh
        add    esp, 8
        db  0A3h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        jne    SHORT loc_3_646A
        push   0
        push   49dh
        db  0E8h, 0A3h, 09Fh, 0FCh, 0FFh
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_6477
loc_3_646A:
        db  08Bh, 00Dh, 004h, 00Ch, 000h, 000h
        inc    ecx
        db  089h, 00Dh, 004h, 00Ch, 000h, 000h
loc_3_6477:
        db  0A3h, 000h, 00Ch, 000h, 000h
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_3_6482:
        pop    edi
        pop    esi
        add    esp, 10ch
        ret
sub_3_648B:
        db  0A1h, 004h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_64B8
        dec    eax
        db  0A3h, 004h, 00Ch, 000h, 000h
        jne    SHORT loc_3_64B8
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_64B8
        push   eax
        db  0E8h, 0D5h, 0A0h, 0FCh, 0FFh
        add    esp, 4
        db  0C7h, 005h, 000h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h
loc_3_64B8:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_64C0:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ecx + 8], eax
        push   1
        push   ecx
        call   sub_3_64E3
        ret    10h
sub_3_64E3:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 8]
        call   sub_3_65CD
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 10h], 0ffffffffh
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 2ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 30h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 28h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 24h], 0
        push   edx
        mov    edx, dword ptr [ebp + 8]
        call   sub_3_660F
        pop    edx
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_3_6597
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp + 8]
        push   1
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 28h], 0
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 4]
        db  0E8h, 0F8h, 09Eh, 0FCh, 0FFh
        add    esp, 8
        or     eax, eax
        je     SHORT loc_3_65C7
loc_3_6597:
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   dword ptr [eax + 4]
        db  0E8h, 0C8h, 09Eh, 0FCh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_3_65C7
        push   dword ptr [ebp + 8]
        db  068h, 090h, 003h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   1
        push   dword ptr [eax + 4]
        db  0E8h, 0F4h, 09Eh, 0FCh, 0FFh
        add    esp, 10h
loc_3_65C7:
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_65CD:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        push   0
        push   0
        mov    edi, dword ptr [ebp + 8]
        push   -1
        push   dword ptr [edi + 4]
        db  0E8h, 0D2h, 09Eh, 0FCh, 0FFh
        add    esp, 10h
        mov    eax, dword ptr [edi + 18h]
        or     eax, eax
        je     SHORT loc_3_6607
        mov    dword ptr [ebp - 4], eax
        pushfd
        push   esi
        mov    esi, dword ptr [ebp - 4]
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        pop    esi
        popfd
        mov    dword ptr [edi + 18h], 0
loc_3_6607:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_3_660F:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], edx
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 24h], 80h
        jae    NEAR PTR loc_3_67AB
        sub    esi, esi
        mov    edi, 1
        mov    ebx, 10h
loc_3_6642:
        mov    eax, dword ptr [esp + 10h]
        push   esi
        push   dword ptr [eax + 8]
        call   sub_3_3349
        lea    eax, [esp + 1ch]
        add    esp, 8
        push   eax
        mov    eax, dword ptr [esp + 14h]
        push   edi
        mov    eax, dword ptr [eax + 1ch]
        add    eax, dword ptr [esp + 18h]
        add    eax, 34h
        push   eax
        mov    eax, dword ptr [esp + 1ch]
        push   dword ptr [eax + 4]
        db  0E8h, 035h, 09Eh, 0FCh, 0FFh
        add    esp, 10h
        cmp    eax, esi
        je     NEAR PTR loc_3_678A
        cmp    dword ptr [esp + 14h], esi
        je     NEAR PTR loc_3_67AB
        mov    eax, dword ptr [esp + 10h]
        add    eax, 28h
        cmp    dword ptr [eax], esi
        jne    SHORT loc_3_66DF
        mov    edx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [edx + 1ch]
        add    edx, 1ch
        mov    ebp, dword ptr [esp + 10h]
        cmp    byte ptr [ecx + ebp + 34h], bl
        jne    SHORT loc_3_66C2
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax + 0b8h], bl
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 0b4h], edi
        jmp    NEAR PTR loc_3_674A
loc_3_66C2:
        mov    eax, dword ptr [esp + 14h]
        add    eax, ecx
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esp + 10h]
        and    dword ptr [eax + 1ch], 7fh
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        add    dword ptr [eax + 24h], ecx
        jmp    SHORT loc_3_674A
loc_3_66DF:
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax + 1ch]
        mov    al, byte ptr [eax + ecx + 34h]
        mov    ecx, dword ptr [esp + 10h]
        inc    dword ptr [ecx + 0b4h]
        mov    ecx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [ecx + 0b4h]
        cmp    ecx, 2
        jbe    SHORT loc_3_6705
        jmp    SHORT loc_3_6743
loc_3_6705:
        mov    edx, dword ptr [esp + 10h]
        mov    byte ptr [ecx + edx + 0b7h], al
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [eax]
        push   dword ptr [eax + 0b4h]
        add    eax, 0b8h
        push   eax
        push   ecx
        call   dword ptr [ecx]
        add    esp, 0ch
        cmp    eax, esi
        jne    SHORT loc_3_672E
        jmp    SHORT loc_3_6743
loc_3_672E:
        cmp    eax, 3
        jne    SHORT loc_3_6735
        jmp    SHORT loc_3_6743
loc_3_6735:
        cmp    eax, 2
        jne    SHORT loc_3_6743
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 28h], edi
        jmp    SHORT loc_3_674A
loc_3_6743:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 28h], esi
loc_3_674A:
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax + 10h]
        cmp    eax, -1
        je     SHORT loc_3_6777
        mov    ecx, dword ptr [esp + 10h]
        cmp    dword ptr [ecx + 24h], eax
        jb     SHORT loc_3_6777
        mov    eax, ecx
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [eax + 620h], edi
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [eax + 14h]
        call   dword ptr [eax + 0ch]
        add    esp, 4
loc_3_6777:
        mov    eax, dword ptr [esp + 10h]
        cmp    dword ptr [eax + 24h], 80h
        jb     NEAR PTR loc_3_6642
        jmp    SHORT loc_3_67AB
loc_3_678A:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax + 30h], 1
        mov    eax, dword ptr [esp + 10h]
        push   dword ptr [eax + 4]
        db  0E8h, 00Fh, 09Dh, 0FCh, 0FFh
        mov    ecx, dword ptr [esp + 14h]
        add    esp, 4
        mov    dword ptr [ecx + 2ch], eax
loc_3_67AB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_3_67B3:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, 0
        cmp    dword ptr [ecx + 24h], 80h
        je     SHORT loc_3_67E4
        mov    eax, dword ptr [ecx + 20h]
        add    eax, 7fh
        mov    dword ptr [ecx + 20h], eax
        and    eax, 7fh
        mov    dword ptr [ecx + 20h], eax
        mov    dl, byte ptr [esp + 8]
        mov    byte ptr [eax + ecx + 34h], dl
        inc    dword ptr [ecx + 24h]
        mov    eax, 1
loc_3_67E4:
        ret    8
sub_3_67E7:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 30h], 1
        jne    SHORT loc_3_67FE
        mov    dword ptr [esi + 30h], 0
        sub    eax, eax
        jmp    SHORT loc_3_6842
loc_3_67FE:
        cmp    dword ptr [esp + 14h], 0
        jle    SHORT loc_3_6833
        mov    edx, dword ptr [esi + 24h]
        or     edx, edx
        je     SHORT loc_3_6833
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], 1
        mov    eax, dword ptr [esi + 20h]
        dec    edx
        mov    bl, byte ptr [eax + esi + 34h]
        mov    byte ptr [ecx], bl
        inc    eax
        mov    dword ptr [esi + 20h], eax
        and    eax, 7fh
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], edx
        jmp    SHORT loc_3_683D
loc_3_6833:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax], 0
loc_3_683D:
        mov    eax, 1
loc_3_6842:
        pop    esi
        pop    ebx
        ret    10h
sub_3_6847:
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        call   sub_3_660F
        pop    edx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 24h], 0
        mov    eax, 1
        pop    ebp
        ret    4
sub_3_687B:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 0ch], edx
        mov    ecx, dword ptr [esp + 0ch]
        cmp    ecx, -1
        mov    dword ptr [eax + 10h], ecx
        mov    esi, dword ptr [esp + 14h]
        mov    dword ptr [eax + 14h], esi
        je     SHORT loc_3_68A5
        cmp    dword ptr [eax + 24h], ecx
        jb     SHORT loc_3_68A5
        push   esi
        call   edx
        add    esp, 4
loc_3_68A5:
        mov    eax, 1
        pop    esi
        ret    10h
        db  000h, 000h
loc_3_68B0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        push   dword ptr [esi + 0ch]
        db  0E8h, 05Dh, 09Eh, 0FCh, 0FFh
        mov    esi, dword ptr [esi]
        mov    edi, eax
        add    esi, 6fah
        mov    ecx, 26h
        rep movsd
        sub    ax, ax
        pop    edi
        pop    esi
        ret
loc_3_68D5:
        push   edi
        mov    edx, dword ptr [esp + 8]
        sub    eax, eax
        mov    edi, dword ptr [edx]
        add    edi, 6fah
        mov    ecx, 26h
        rep stosd
        mov    eax, dword ptr [edx]
        pop    edi
        mov    dword ptr [eax + 6fah], 10h
        sub    ax, ax
        ret
loc_3_68FC:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 2e0h]
        cmp    eax, 1ah
        je     SHORT loc_3_6916
        cmp    eax, 1ch
        mov    ax, 0
        jne    SHORT loc_3_691A
loc_3_6916:
        mov    ax, 2
loc_3_691A:
        ret
loc_3_691B:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax + 82ch]
        ret
sub_3_6929:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 828h], eax
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_6959
        db  00Fh, 0B7h, 00Dh, 022h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 032h, 09Ch, 0FCh, 0FFh
        add    esp, 8
loc_3_6959:
        cmp    dword ptr [esi + 2e0h], 0fh
        mov    ax, 0
        je     SHORT loc_3_696A
        mov    ax, 2
loc_3_696A:
        pop    esi
        ret
loc_3_696C:
        sub    esp, 60h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 70h]
        mov    ebx, dword ptr [eax]
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_6998
        db  00Fh, 0B7h, 00Dh, 024h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 0F3h, 09Bh, 0FCh, 0FFh
        add    esp, 8
loc_3_6998:
        cmp    dword ptr [ebx + 2e0h], 1ah
        jne    NEAR PTR loc_3_6A41
        mov    dword ptr [ebx + 2e0h], 0fh
        mov    dword ptr [ebx + 7ebh], 0
        push   dword ptr [ebx + 828h]
        db  0E8h, 032h, 09Eh, 0FCh, 0FFh
        add    esp, 4
        mov    dword ptr [ebx + 828h], 0
        mov    eax, dword ptr [ebx + 7efh]
        or     eax, eax
        jne    SHORT loc_3_69E4
        test   byte ptr [ebx + 825h], 8
        jne    SHORT loc_3_69F2
loc_3_69E4:
        cmp    eax, 1
        jne    SHORT loc_3_6A21
        test   byte ptr [ebx + 825h], 4
        je     SHORT loc_3_6A21
loc_3_69F2:
        lea    esi, [ebx + 7f3h]
        lea    edi, [esp + 1ch]
        movsd
        movsd
        movsb
        mov    byte ptr [ebx + 7fbh], 0
        mov    dword ptr [esp + 10h], 0ffffffffh
        mov    dword ptr [esp + 0ch], ebx
        lea    eax, [esp + 0ch]
        push   13h
        push   eax
        call   sub_3_7341
        add    esp, 8
loc_3_6A21:
        push   ebx
        call   sub_3_73DA
        add    esp, 4
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_6A3C
        push   eax
        db  0E8h, 057h, 09Bh, 0FCh, 0FFh
        add    esp, 4
loc_3_6A3C:
        sub    ax, ax
        jmp    SHORT loc_3_6A45
loc_3_6A41:
        mov    ax, 2
loc_3_6A45:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 60h
        ret
loc_3_6A4C:
        sub    esp, 60h
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 6ch]
        mov    esi, dword ptr [edi]
        mov    eax, dword ptr [esi + 2e0h]
        cmp    eax, 4
        jne    NEAR PTR loc_3_6B1F
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_6A86
        db  00Fh, 0B7h, 00Dh, 022h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 005h, 09Bh, 0FCh, 0FFh
        add    esp, 8
loc_3_6A86:
        lea    eax, [esi + 7dfh]
        push   eax
        db  068h, 068h, 00Ch, 000h, 000h
        push   esi
        call   sub_3_70CA
        or     eax, eax
        jne    SHORT loc_3_6AD2
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 828h], eax
        mov    dword ptr [esi + 2e0h], 1ah
        mov    dword ptr [esi + 7ebh], 1
        sub    ax, ax
        mov    dword ptr [esi + 7fch], 1
        mov    dword ptr [esi + 800h], 1
        jmp    SHORT loc_3_6B19
loc_3_6AD2:
        mov    dword ptr [esi + 2e0h], 4
        mov    dword ptr [esp + 8], esi
        mov    eax, dword ptr [esi + 7d3h]
        mov    dword ptr [esp + 0ch], eax
        mov    eax, dword ptr [esi + 7d7h]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esi + 7dbh]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esi + 5beh]
        mov    dword ptr [esp + 18h], eax
        lea    eax, [esp + 8]
        push   eax
        call   sub_3_0823
        add    esp, 4
loc_3_6B15:
        mov    ax, 2
loc_3_6B19:
        pop    edi
        pop    esi
        add    esp, 60h
        ret
loc_3_6B1F:
        cmp    eax, 1ch
        jne    SHORT loc_3_6B15
        push   dword ptr [esi + 804h]
        db  0E8h, 015h, 09Ah, 0FCh, 0FFh
        mov    dword ptr [esi + 804h], 0
        push   dword ptr [esi + 808h]
        db  0E8h, 0DCh, 098h, 0FCh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 808h], 0
        mov    dword ptr [esi + 2e0h], 1ah
        mov    dword ptr [esi + 7ebh], 1
        mov    dword ptr [esi + 7fch], 1
        sub    ax, ax
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [esi + 828h], ecx
        jmp    SHORT loc_3_6B19
sub_3_6B7D:
        sub    esp, 60h
        push   esi
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_6BA1
        db  00Fh, 0B7h, 00Dh, 024h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 0EAh, 099h, 0FCh, 0FFh
        add    esp, 8
loc_3_6BA1:
        mov    esi, dword ptr [esp + 68h]
        lea    eax, [esi + 7dfh]
        push   eax
        db  068h, 074h, 00Ch, 000h, 000h
        push   esi
        call   sub_3_70CA
        mov    dword ptr [esi + 2e0h], 4
        mov    dword ptr [esi + 7ebh], 0
        mov    dword ptr [esi + 7fch], 0
        mov    dword ptr [esp + 4], esi
        mov    eax, dword ptr [esi + 7d3h]
        mov    dword ptr [esp + 8], eax
        mov    eax, dword ptr [esi + 7d7h]
        mov    dword ptr [esp + 0ch], eax
        mov    eax, dword ptr [esi + 7dbh]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esi + 5beh]
        mov    dword ptr [esp + 14h], eax
        lea    eax, [esp + 4]
        push   eax
        call   sub_3_0823
        add    esp, 4
        pop    esi
        add    esp, 60h
        ret
loc_3_6C13:
        mov    eax, dword ptr [esp + 4]
        push   eax
        mov    dword ptr [eax + 808h], 0
        call   sub_3_6B7D
        add    esp, 4
        ret
loc_3_6C2B:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 804h], 0
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 800h], 0
        push   0
        push   1
        push   dword ptr [esp + 8]
        db  068h, 013h, 06Ch, 000h, 000h
        db  0E8h, 0B8h, 097h, 0FCh, 0FFh
        add    esp, 10h
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ecx + 808h], eax
        add    esp, 4
        ret
sub_3_6C71:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    dword ptr [esi + 800h], 0
        cmp    dword ptr [esi + 2e0h], 1ch
        jne    SHORT loc_3_6CB1
        push   dword ptr [esi + 804h]
        db  0E8h, 0B0h, 098h, 0FCh, 0FFh
        mov    dword ptr [esi + 2e0h], 1ah
        push   esi
        push   1
        db  068h, 02Bh, 06Ch, 000h, 000h
        db  0E8h, 07Dh, 098h, 0FCh, 0FFh
        mov    dword ptr [esi + 804h], eax
loc_3_6CB1:
        pop    esi
        ret
loc_3_6CB3:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax]
        cmp    dword ptr [esi + 2e0h], 1ah
        jne    SHORT loc_3_6D2F
        cmp    dword ptr [esi + 828h], 0
        je     SHORT loc_3_6CE4
        push   dword ptr [esi + 828h]
        db  0E8h, 01Fh, 09Bh, 0FCh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 828h], 0
loc_3_6CE4:
        cmp    dword ptr [esi + 800h], 0
        jne    SHORT loc_3_6CF8
        push   esi
        call   sub_3_6B7D
        add    esp, 4
        jmp    SHORT loc_3_6D2A
loc_3_6CF8:
        mov    dword ptr [esi + 2e0h], 1ch
        mov    dword ptr [esi + 7ebh], 0
        push   esi
        mov    eax, dword ptr [esi + 80ch]
        imul   eax, eax, 3e8h
        push   eax
        db  068h, 02Bh, 06Ch, 000h, 000h
        db  0E8h, 004h, 098h, 0FCh, 0FFh
        mov    dword ptr [esi + 804h], eax
loc_3_6D2A:
        sub    ax, ax
        jmp    SHORT loc_3_6D33
loc_3_6D2F:
        mov    ax, 2
loc_3_6D33:
        pop    esi
        ret
loc_3_6D35:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, 2
        mov    ebx, dword ptr [esi]
        cmp    dword ptr [ebx + 2e0h], 1ah
        jne    NEAR PTR loc_3_6E55
        cmp    dword ptr [ebx + 7ebh], 1
        jne    NEAR PTR loc_3_6E55
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_6D7D
        db  00Fh, 0B7h, 00Dh, 020h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 00Eh, 098h, 0FCh, 0FFh
        add    esp, 8
loc_3_6D7D:
        mov    dword ptr [ebx + 7efh], 0
        mov    dword ptr [ebx + 7ebh], 2
        lea    eax, [ebx + 7dfh]
        push   eax
        lea    eax, [esi + 10h]
        push   eax
        push   ebx
        call   sub_3_70CA
        mov    edi, dword ptr [esi]
        mov    ebx, eax
        mov    dword ptr [edi + 2e0h], 1ah
        cmp    ebx, 2
        jne    SHORT loc_3_6DE5
        mov    dword ptr [edi + 7ebh], 6
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_6DE0
        db  00Fh, 0B7h, 005h, 0BAh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 0ABh, 097h, 0FCh, 0FFh
        add    esp, 8
loc_3_6DE0:
        mov    ax, bx
        jmp    SHORT loc_3_6E58
loc_3_6DE5:
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_6E06
        db  00Fh, 0B7h, 005h, 01Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 085h, 097h, 0FCh, 0FFh
        add    esp, 8
loc_3_6E06:
        lea    eax, [edi + 7dfh]
        push   eax
        db  068h, 008h, 00Ch, 000h, 000h
        push   edi
        call   sub_3_70CA
        mov    edi, eax
        mov    eax, dword ptr [esi]
        cmp    edi, 2
        mov    dword ptr [eax + 2e0h], 1ah
        jne    SHORT loc_3_6E55
        mov    dword ptr [eax + 7ebh], 6
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_6E55
        db  00Fh, 0B7h, 00Dh, 0B6h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 036h, 097h, 0FCh, 0FFh
        add    esp, 8
loc_3_6E55:
        mov    ax, di
loc_3_6E58:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_6E5C:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, 2
        mov    ebx, dword ptr [edi]
        cmp    dword ptr [ebx + 2e0h], 1ah
        jne    NEAR PTR loc_3_6EF8
        cmp    dword ptr [ebx + 7ebh], esi
        jne    SHORT loc_3_6EF8
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_6E9F
        db  00Fh, 0B7h, 00Dh, 01Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 0ECh, 096h, 0FCh, 0FFh
        add    esp, 8
loc_3_6E9F:
        lea    eax, [ebx + 7dfh]
        push   eax
        db  068h, 014h, 00Ch, 000h, 000h
        push   ebx
        call   sub_3_70CA
        mov    esi, eax
        mov    eax, dword ptr [edi]
        cmp    esi, 2
        mov    dword ptr [eax + 2e0h], 1ah
        mov    dword ptr [eax + 7ebh], 1
        jne    SHORT loc_3_6EF8
        mov    dword ptr [eax + 7ebh], 6
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_6EF8
        db  00Fh, 0B7h, 00Dh, 0B8h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 093h, 096h, 0FCh, 0FFh
        add    esp, 8
loc_3_6EF8:
        mov    ax, si
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_6EFF:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, 2
        mov    ebx, dword ptr [esi]
        cmp    dword ptr [ebx + 2e0h], 1ah
        jne    NEAR PTR loc_3_701F
        cmp    dword ptr [ebx + 7ebh], 1
        jne    NEAR PTR loc_3_701F
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_6F47
        db  00Fh, 0B7h, 00Dh, 020h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 044h, 096h, 0FCh, 0FFh
        add    esp, 8
loc_3_6F47:
        mov    dword ptr [ebx + 7efh], 0
        mov    dword ptr [ebx + 7ebh], 3
        lea    eax, [ebx + 7dfh]
        push   eax
        lea    eax, [esi + 10h]
        push   eax
        push   ebx
        call   sub_3_70CA
        mov    edi, dword ptr [esi]
        mov    ebx, eax
        mov    dword ptr [edi + 2e0h], 1ah
        cmp    ebx, 2
        jne    SHORT loc_3_6FAF
        mov    dword ptr [edi + 7ebh], 6
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_6FAA
        db  00Fh, 0B7h, 005h, 0BAh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 0E1h, 095h, 0FCh, 0FFh
        add    esp, 8
loc_3_6FAA:
        mov    ax, bx
        jmp    SHORT loc_3_7022
loc_3_6FAF:
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_6FD0
        db  00Fh, 0B7h, 005h, 01Ch, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 0BBh, 095h, 0FCh, 0FFh
        add    esp, 8
loc_3_6FD0:
        lea    eax, [edi + 7dfh]
        push   eax
        db  068h, 020h, 00Ch, 000h, 000h
        push   edi
        call   sub_3_70CA
        mov    edi, eax
        mov    eax, dword ptr [esi]
        cmp    edi, 2
        mov    dword ptr [eax + 2e0h], 1ah
        jne    SHORT loc_3_701F
        mov    dword ptr [eax + 7ebh], 6
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_701F
        db  00Fh, 0B7h, 00Dh, 0B6h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 06Ch, 095h, 0FCh, 0FFh
        add    esp, 8
loc_3_701F:
        mov    ax, di
loc_3_7022:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_7026:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    esi, 2
        mov    ebx, dword ptr [edi]
        cmp    dword ptr [ebx + 2e0h], 1ah
        jne    NEAR PTR loc_3_70C3
        cmp    dword ptr [ebx + 7ebh], 3
        jne    SHORT loc_3_70C3
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_706A
        db  00Fh, 0B7h, 00Dh, 01Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 021h, 095h, 0FCh, 0FFh
        add    esp, 8
loc_3_706A:
        lea    eax, [ebx + 7dfh]
        push   eax
        db  068h, 02Ch, 00Ch, 000h, 000h
        push   ebx
        call   sub_3_70CA
        mov    esi, eax
        mov    eax, dword ptr [edi]
        cmp    esi, 2
        mov    dword ptr [eax + 2e0h], 1ah
        mov    dword ptr [eax + 7ebh], 1
        jne    SHORT loc_3_70C3
        mov    dword ptr [eax + 7ebh], 6
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_70C3
        db  00Fh, 0B7h, 00Dh, 0B8h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 0C8h, 094h, 0FCh, 0FFh
        add    esp, 8
loc_3_70C3:
        mov    ax, si
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_70CA:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0
        mov    ebx, 2
        mov    esi, dword ptr [esp + 18h]
        push   dword ptr [esp + 1ch]
        push   esi
        call   sub_3_31F4
        add    esp, 0ch
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_3_711F
        mov    dword ptr [esi + 2e0h], 14h
        mov    edi, dword ptr [esp + 1ch]
        push   edi
        db  0E8h, 05Fh, 096h, 0FCh, 0FFh
        push   ebp
        push   0
        push   0
        push   -1
        push   esi
        call   sub_3_11F4
        add    esp, 14h
        mov    ebx, eax
        cmp    ebx, 1
        jne    SHORT loc_3_711F
        push   edi
        db  0E8h, 070h, 096h, 0FCh, 0FFh
        mov    ebx, eax
loc_3_711F:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
loc_3_7128:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    si, 2
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ecx + 2e0h]
        cmp    eax, 3
        je     SHORT loc_3_714A
        cmp    eax, 4
        je     SHORT loc_3_714A
        cmp    eax, 8
        jne    SHORT loc_3_71AF
loc_3_714A:
        push   0
        push   ecx
        call   sub_3_3E17
        add    esp, 8
        push   0
        db  068h, 038h, 00Ch, 000h, 000h
        push   dword ptr [edi]
        call   sub_3_31F4
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_3_71AF
        mov    esi, dword ptr [edi]
        mov    dword ptr [esi + 2e0h], 11h
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_7199
        db  00Fh, 0B7h, 005h, 018h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 0F2h, 093h, 0FCh, 0FFh
        add    esp, 8
loc_3_7199:
        push   ebx
        push   dword ptr [edi + 0ch]
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        push   esi
        call   sub_3_11F4
        add    esp, 14h
        mov    si, ax
loc_3_71AF:
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        cmp    si, 2
        jne    SHORT loc_3_71D6
        or     ecx, ecx
        je     SHORT loc_3_71D6
        db  00Fh, 0B7h, 005h, 0B2h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 0B5h, 093h, 0FCh, 0FFh
        add    esp, 8
loc_3_71D6:
        mov    ax, si
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_71DD:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, 2
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [ecx + 2e0h]
        cmp    eax, 3
        je     SHORT loc_3_7200
        cmp    eax, 4
        je     SHORT loc_3_7200
        cmp    eax, 8
        jne    SHORT loc_3_7265
loc_3_7200:
        push   0
        push   ecx
        call   sub_3_3E17
        add    esp, 8
        push   0
        db  068h, 050h, 00Ch, 000h, 000h
        push   dword ptr [esi]
        call   sub_3_31F4
        add    esp, 0ch
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_3_7265
        mov    edi, dword ptr [esi]
        mov    dword ptr [edi + 2e0h], 0eh
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        or     ecx, ecx
        je     SHORT loc_3_724F
        db  00Fh, 0B7h, 005h, 016h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 03Ch, 093h, 0FCh, 0FFh
        add    esp, 8
loc_3_724F:
        push   ebx
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   edi
        call   sub_3_11F4
        add    esp, 14h
        movzx  edi, ax
loc_3_7265:
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        cmp    edi, 2
        jne    SHORT loc_3_728B
        or     ecx, ecx
        je     SHORT loc_3_728B
        db  00Fh, 0B7h, 005h, 0B0h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 000h, 093h, 0FCh, 0FFh
        add    esp, 8
loc_3_728B:
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_7292:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        mov    di, 2
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + 2e0h], 0fh
        jne    NEAR PTR loc_3_7339
        lea    ebx, [esi + 10h]
        push   0
        push   ebx
        push   eax
        call   sub_3_31F4
        add    esp, 0ch
        mov    ebp, eax
        or     ebp, ebp
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
        je     SHORT loc_3_730B
        or     ecx, ecx
        je     SHORT loc_3_72E3
        db  00Fh, 0B7h, 005h, 01Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 0A8h, 092h, 0FCh, 0FFh
        add    esp, 8
loc_3_72E3:
        mov    eax, dword ptr [esi]
        push   ebp
        mov    dword ptr [eax + 2e0h], 17h
        push   dword ptr [esi + 0ch]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   eax
        call   sub_3_11F4
        add    esp, 14h
        mov    di, ax
        db  08Bh, 00Dh, 000h, 00Ch, 000h, 000h
loc_3_730B:
        cmp    di, 2
        jne    SHORT loc_3_7339
        mov    eax, dword ptr [esi]
        or     ecx, ecx
        mov    dword ptr [eax + 2e0h], 0fh
        je     SHORT loc_3_7339
        push   ebx
        db  00Fh, 0B7h, 005h, 0B4h, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   ecx
        db  0E8h, 052h, 092h, 0FCh, 0FFh
        add    esp, 0ch
loc_3_7339:
        mov    ax, di
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_7341:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    ebx, dword ptr [esi]
        mov    eax, dword ptr [ebx + 2e0h]
        cmp    eax, 0fh
        je     SHORT loc_3_7366
        cmp    eax, 1ah
        je     SHORT loc_3_736B
        cmp    eax, 17h
        je     SHORT loc_3_7366
        mov    eax, 2
        jmp    SHORT loc_3_73D6
loc_3_7366:
        cmp    eax, 1ah
        jne    SHORT loc_3_737B
loc_3_736B:
        mov    edi, dword ptr [esp + 14h]
        cmp    edi, 11h
        jne    SHORT loc_3_737F
        mov    eax, 2
        jmp    SHORT loc_3_73D6
loc_3_737B:
        mov    edi, dword ptr [esp + 14h]
loc_3_737F:
        push   1
        push   68h
        db  0E8h, 040h, 091h, 0FCh, 0FFh
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_3_7396
        mov    eax, 2
        jmp    SHORT loc_3_73D6
loc_3_7396:
        mov    dword ptr [eax + 4], edi
        lea    edi, [eax + 8]
        mov    ecx, 18h
        rep movsd
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebx + 838h], 0
        jne    SHORT loc_3_73BA
        mov    dword ptr [ebx + 838h], eax
        jmp    SHORT loc_3_73C2
loc_3_73BA:
        mov    ecx, dword ptr [ebx + 83ch]
        mov    dword ptr [ecx], eax
loc_3_73C2:
        mov    dword ptr [ebx + 83ch], eax
        push   ebx
        call   sub_3_73DA
        add    esp, 4
        mov    eax, 1
loc_3_73D6:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_73DA:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 2e0h], 0fh
        jne    NEAR PTR loc_3_7473
        cmp    dword ptr [esi + 834h], 0
        jne    SHORT loc_3_7473
        mov    ecx, dword ptr [esi + 838h]
        or     ecx, ecx
        je     SHORT loc_3_7473
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 838h], eax
        or     ecx, ecx
        je     SHORT loc_3_7473
        mov    dword ptr [esi + 834h], ecx
        mov    eax, dword ptr [ecx + 4]
        cmp    eax, 4
        je     SHORT loc_3_742F
        cmp    eax, 11h
        je     SHORT loc_3_743D
        cmp    eax, 13h
        je     SHORT loc_3_7448
        cmp    eax, 14h
        je     SHORT loc_3_7453
        mov    eax, 2
        jmp    SHORT loc_3_7462
loc_3_742F:
        add    ecx, 8
        push   ecx
        call   sub_3_0C35
        add    esp, 4
        jmp    SHORT loc_3_7462
loc_3_743D:
        add    ecx, 8
        push   ecx
        call   sub_3_6929
        jmp    SHORT loc_3_745C
loc_3_7448:
        add    ecx, 8
        push   ecx
        call   sub_3_765E
        jmp    SHORT loc_3_745C
loc_3_7453:
        add    ecx, 8
        push   ecx
        call   sub_3_793F
loc_3_745C:
        add    esp, 4
        movzx  eax, ax
loc_3_7462:
        cmp    eax, 1
        je     SHORT loc_3_7473
        push   1
        push   eax
        push   esi
        call   sub_3_7475
        add    esp, 0ch
loc_3_7473:
        pop    esi
        ret
sub_3_7475:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esi + 834h]
        or     edi, edi
        je     NEAR PTR loc_3_75BE
        mov    dword ptr [esi + 834h], 0
        mov    eax, dword ptr [edi + 4]
        cmp    eax, 11h
        je     SHORT loc_3_74B2
        cmp    eax, 13h
        jb     NEAR PTR loc_3_7591
        cmp    eax, 14h
        jbe    NEAR PTR loc_3_7548
        jmp    NEAR PTR loc_3_7591
loc_3_74B2:
        mov    ecx, dword ptr [esp + 10h]
        or     ecx, ecx
        jne    SHORT loc_3_74E7
        mov    dword ptr [esi + 2e0h], 1ah
        mov    dword ptr [esi + 7ebh], 1
        mov    dword ptr [esi + 82ch], ecx
        push   dword ptr [esi + 828h]
        db  0E8h, 005h, 093h, 0FCh, 0FFh
        add    esp, 4
        jmp    NEAR PTR loc_3_7591
loc_3_74E7:
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [esi + 828h], eax
        mov    dword ptr [esi + 82ch], ecx
        push   dword ptr [esi + 828h]
        db  0E8h, 0E3h, 092h, 0FCh, 0FFh
        add    esp, 4
        push   dword ptr [esi + 828h]
        db  0E8h, 0E7h, 092h, 0FCh, 0FFh
        add    esp, 4
        mov    dword ptr [esi + 828h], 0
        mov    dword ptr [esi + 7ebh], 6
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_7591
        db  00Fh, 0B7h, 00Dh, 0C0h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 045h, 090h, 0FCh, 0FFh
        add    esp, 8
        jmp    SHORT loc_3_7591
loc_3_7548:
        mov    dword ptr [esi + 2e0h], 0fh
        mov    eax, dword ptr [esi + 5e2h]
        or     eax, eax
        je     SHORT loc_3_7579
        cmp    dword ptr [esi + 5deh], eax
        je     SHORT loc_3_756F
        push   0
        push   eax
        db  0E8h, 064h, 08Fh, 0FCh, 0FFh
        add    esp, 8
loc_3_756F:
        mov    dword ptr [esi + 5e2h], 0
loc_3_7579:
        mov    eax, dword ptr [edi + 0ch]
        cmp    eax, -1
        je     SHORT loc_3_7591
        push   dword ptr [edi + 14h]
        push   dword ptr [esp + 14h]
        push   dword ptr [edi + 10h]
        push   eax
        db  0E8h, 05Fh, 091h, 0FCh, 0FFh
loc_3_7591:
        push   0
        push   edi
        db  0E8h, 037h, 08Fh, 0FCh, 0FFh
        add    esp, 8
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_3_75AC
        push   esi
        call   sub_3_73DA
        add    esp, 4
loc_3_75AC:
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_75BE
        push   eax
        db  0E8h, 0D5h, 08Fh, 0FCh, 0FFh
        add    esp, 4
loc_3_75BE:
        pop    edi
        pop    esi
        ret
sub_3_75C1:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 834h], 0
        jne    SHORT loc_3_75F1
        mov    eax, dword ptr [esi + 838h]
        or     eax, eax
        je     SHORT loc_3_75E2
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 838h], ecx
loc_3_75E2:
        mov    dword ptr [esi + 834h], eax
        cmp    dword ptr [esi + 834h], 0
        je     SHORT loc_3_761D
loc_3_75F1:
        mov    edi, dword ptr [esp + 10h]
loc_3_75F5:
        push   0
        push   edi
        push   esi
        call   sub_3_7475
        add    esp, 0ch
        mov    eax, dword ptr [esi + 838h]
        or     eax, eax
        je     SHORT loc_3_7613
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 838h], ecx
loc_3_7613:
        mov    dword ptr [esi + 834h], eax
        or     eax, eax
        jne    SHORT loc_3_75F5
loc_3_761D:
        pop    edi
        pop    esi
        ret
sub_3_7620:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, ecx
        mov    edi, dword ptr [esp + 14h]
        sub    esi, edi
        mov    eax, esi
        cdq
        xor    eax, edx
        sub    eax, edx
        lea    esi, [eax + 1]
        mov    eax, 10000h
        cdq
        idiv   esi
        cmp    edi, ecx
        lea    ecx, [eax + 1]
        mov    eax, dword ptr [esp + 0ch]
        ja     SHORT loc_3_7653
        sub    edx, edx
        div    ecx
        add    eax, edi
        jmp    SHORT loc_3_765B
loc_3_7653:
        sub    edx, edx
        div    ecx
        sub    edi, eax
        mov    eax, edi
loc_3_765B:
        pop    edi
        pop    esi
        ret
sub_3_765E:
        sub    esp, 410h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 424h]
        mov    dword ptr [esp + 10h], 0
        mov    ebp, dword ptr [eax]
        add    eax, 10h
        mov    dword ptr [esp + 18h], eax
        cmp    dword ptr [ebp + 2e0h], 0fh
        jne    NEAR PTR loc_3_7921
        movzx  ecx, byte ptr [ebp + 7fbh]
        imul   ecx, ecx, 3
        movzx  eax, byte ptr [eax + 8]
        db  08Ah, 09Ch, 001h, 05Ch, 00Ch, 000h, 000h
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_76C2
        db  00Fh, 0B7h, 00Dh, 026h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 0C9h, 08Eh, 0FCh, 0FFh
        add    esp, 8
loc_3_76C2:
        cmp    bl, 1
        jne    SHORT loc_3_76E6
        mov    eax, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [ebp + 7f3h], eax
        jne    SHORT loc_3_76E6
        mov    eax, dword ptr [esp + 18h]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [ebp + 7f7h], eax
        jne    SHORT loc_3_76E6
        sub    bl, bl
loc_3_76E6:
        lea    edi, [ebp + 7f3h]
        mov    esi, dword ptr [esp + 18h]
        movsd
        movsd
        movsb
        or     bl, bl
        jne    SHORT loc_3_76FF
        sub    ax, ax
        jmp    NEAR PTR loc_3_7934
loc_3_76FF:
        test   bl, 8
        je     SHORT loc_3_7725
        push   0
        db  068h, 084h, 00Ch, 000h, 000h
        push   ebp
        call   sub_3_31F4
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_7725
        mov    ax, 2
        jmp    NEAR PTR loc_3_7934
loc_3_7725:
        test   bl, 10h
        je     SHORT loc_3_774D
        push   dword ptr [esp + 10h]
        db  068h, 098h, 00Ch, 000h, 000h
        push   ebp
        call   sub_3_31F4
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_774D
        mov    ax, 2
        jmp    NEAR PTR loc_3_7934
loc_3_774D:
        test   bl, 2
        je     SHORT loc_3_7775
        push   dword ptr [esp + 10h]
        db  068h, 0ACh, 00Ch, 000h, 000h
        push   ebp
        call   sub_3_31F4
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_7775
        mov    ax, 2
        jmp    NEAR PTR loc_3_7934
loc_3_7775:
        test   bl, 4
        je     SHORT loc_3_779D
        push   dword ptr [esp + 10h]
        db  068h, 0C0h, 00Ch, 000h, 000h
        push   ebp
        call   sub_3_31F4
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_779D
        mov    ax, 2
        jmp    NEAR PTR loc_3_7934
loc_3_779D:
        test   bl, 1
        je     SHORT loc_3_77C5
        push   dword ptr [esp + 10h]
        db  068h, 0D4h, 00Ch, 000h, 000h
        push   ebp
        call   sub_3_31F4
        mov    dword ptr [esp + 1ch], eax
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_77C5
        mov    ax, 2
        jmp    NEAR PTR loc_3_7934
loc_3_77C5:
        push   1
        push   19ah
        db  0E8h, 0F7h, 08Ch, 0FCh, 0FFh
        mov    dword ptr [esp + 24h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_3_77F3
        push   0
        push   dword ptr [esp + 14h]
        db  0E8h, 0E9h, 08Ch, 0FCh, 0FFh
        add    esp, 8
        mov    ax, 2
        jmp    NEAR PTR loc_3_7934
loc_3_77F3:
        db  0BEh, 0F0h, 00Ch, 000h, 000h
        lea    edi, [esp + 20h]
        movsd
        movsw
        lea    eax, [esp + 120h]
        push   eax
        push   dword ptr [ebp + 818h]
        mov    eax, dword ptr [esp + 20h]
        push   dword ptr [ebp + 814h]
        push   dword ptr [eax]
        call   sub_3_7620
        add    esp, 0ch
        push   eax
        call   sub_3_30F5
        add    esp, 8
        db  0BEh, 0F8h, 00Ch, 000h, 000h
        lea    edi, [esp + 220h]
        movsd
        movsw
        movsb
        lea    eax, [esp + 320h]
        push   eax
        push   dword ptr [ebp + 820h]
        mov    eax, dword ptr [esp + 20h]
        push   dword ptr [ebp + 81ch]
        push   dword ptr [eax + 4]
        call   sub_3_7620
        add    esp, 0ch
        push   eax
        call   sub_3_30F5
        mov    esi, dword ptr [esp + 18h]
        mov    ebx, dword ptr [esp + 24h]
        add    esp, 8
        mov    eax, esi
        cmp    byte ptr [eax], 0
        je     SHORT loc_3_78A6
loc_3_7875:
        lea    eax, [esp + 20h]
        push   2
        push   eax
        mov    edi, esi
        lea    eax, [esp + 1ch]
        push   eax
        push   ebx
        push   esi
        call   sub_3_2F1C
        add    esp, 14h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    esi, ecx
        mov    eax, dword ptr [esp + 14h]
        inc    eax
        add    ebx, eax
        cmp    byte ptr [esi], 0
        jne    SHORT loc_3_7875
loc_3_78A6:
        push   0
        mov    byte ptr [ebx], 0
        push   dword ptr [esp + 14h]
        db  0E8h, 01Ch, 08Ch, 0FCh, 0FFh
        mov    eax, dword ptr [esp + 42ch]
        add    esp, 8
        mov    edi, dword ptr [eax]
        add    edi, 2e0h
        push   dword ptr [esp + 1ch]
        mov    dword ptr [edi], 17h
        push   dword ptr [eax + 0ch]
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 4]
        push   ebp
        call   sub_3_11F4
        add    esp, 14h
        mov    si, ax
        cmp    si, 2
        jne    SHORT loc_3_7931
        push   0
        mov    dword ptr [edi], 0fh
        push   dword ptr [esp + 20h]
        db  0E8h, 0D4h, 08Bh, 0FCh, 0FFh
        add    esp, 8
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_7931
        db  00Fh, 0B7h, 00Dh, 0BCh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 06Ch, 08Ch, 0FCh, 0FFh
        add    esp, 8
        jmp    SHORT loc_3_7931
loc_3_7921:
        lea    edi, [ebp + 7f3h]
        mov    esi, dword ptr [esp + 18h]
        movsd
        movsd
        movsb
        sub    si, si
loc_3_7931:
        mov    ax, si
loc_3_7934:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 410h
        ret
sub_3_793F:
        sub    esp, 210h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 224h]
        mov    si, 2
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esp + 1ch], eax
        cmp    dword ptr [eax + 2e0h], 0fh
        jne    NEAR PTR loc_3_7AEC
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_7987
        db  00Fh, 0B7h, 00Dh, 028h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 004h, 08Ch, 0FCh, 0FFh
        add    esp, 8
loc_3_7987:
        push   0
        db  068h, 000h, 00Dh, 000h, 000h
        push   dword ptr [esp + 24h]
        call   sub_3_31F4
        mov    dword ptr [esp + 20h], eax
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_79CB
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_79C2
        db  00Fh, 0B7h, 00Dh, 0BEh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 0C9h, 08Bh, 0FCh, 0FFh
        add    esp, 8
loc_3_79C2:
        mov    ax, 2
        jmp    NEAR PTR loc_3_7AEF
loc_3_79CB:
        push   1
        push   19ah
        db  0E8h, 0F1h, 08Ah, 0FCh, 0FFh
        mov    dword ptr [esp + 20h], eax
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_3_7A19
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_7A02
        db  00Fh, 0B7h, 00Dh, 0BEh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 089h, 08Bh, 0FCh, 0FFh
        add    esp, 8
loc_3_7A02:
        push   0
        push   dword ptr [esp + 18h]
        db  0E8h, 0C3h, 08Ah, 0FCh, 0FFh
        add    esp, 8
        mov    ax, 2
        jmp    NEAR PTR loc_3_7AEF
loc_3_7A19:
        db  0BEh, 010h, 00Dh, 000h, 000h
        lea    edi, [esp + 20h]
        movsd
        movsd
        mov    al, byte ptr [ebx + 10h]
        mov    byte ptr [esp + 120h], al
        mov    byte ptr [esp + 121h], 0
        mov    esi, dword ptr [esp + 14h]
        mov    eax, esi
        mov    ebp, dword ptr [esp + 18h]
        cmp    byte ptr [eax], 0
        je     SHORT loc_3_7A76
loc_3_7A45:
        lea    eax, [esp + 20h]
        push   1
        push   eax
        mov    edi, esi
        lea    eax, [esp + 18h]
        push   eax
        push   ebp
        push   esi
        call   sub_3_2F1C
        add    esp, 14h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    esi, ecx
        mov    eax, dword ptr [esp + 10h]
        inc    eax
        add    ebp, eax
        cmp    byte ptr [esi], 0
        jne    SHORT loc_3_7A45
loc_3_7A76:
        push   0
        mov    byte ptr [ebp], 0
        push   dword ptr [esp + 18h]
        db  0E8h, 04Bh, 08Ah, 0FCh, 0FFh
        add    esp, 8
        mov    edi, dword ptr [ebx]
        add    edi, 2e0h
        push   dword ptr [esp + 18h]
        mov    dword ptr [edi], 17h
        push   dword ptr [ebx + 0ch]
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        push   dword ptr [esp + 2ch]
        call   sub_3_11F4
        add    esp, 14h
        mov    si, ax
        cmp    si, 2
        jne    SHORT loc_3_7AEC
        push   0
        mov    dword ptr [edi], 0fh
        push   dword ptr [esp + 1ch]
        db  0E8h, 007h, 08Ah, 0FCh, 0FFh
        add    esp, 8
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_7AEC
        db  00Fh, 0B7h, 00Dh, 0BEh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 09Fh, 08Ah, 0FCh, 0FFh
        add    esp, 8
loc_3_7AEC:
        mov    ax, si
loc_3_7AEF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 210h
        ret
sub_3_7AFA:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        sub    ebx, ebx
        mov    edi, dword ptr [eax + 5fah]
        sub    ebp, ebp
        cmp    edi, ebx
        je     SHORT loc_3_7B46
        mov    esi, dword ptr [esp + 1ch]
loc_3_7B14:
        push   esi
        lea    eax, [edi + 0fh]
        push   eax
        push   dword ptr [esp + 20h]
        call   sub_3_3146
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_3_7B40
        movzx  eax, byte ptr [edi + 0eh]
        sub    eax, esi
        cmp    eax, -1
        jne    SHORT loc_3_7B3B
        mov    ebx, 1
        jmp    SHORT loc_3_7B40
loc_3_7B3B:
        mov    ebp, 1
loc_3_7B40:
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    SHORT loc_3_7B14
loc_3_7B46:
        lea    eax, [ebp + ebx*2]
        or     eax, eax
        je     SHORT loc_3_7B64
        cmp    eax, 1
        je     SHORT loc_3_7B6B
        cmp    eax, 2
        je     SHORT loc_3_7B72
        cmp    eax, 3
        je     SHORT loc_3_7B76
        mov    eax, 3
        jmp    SHORT loc_3_7B7B
loc_3_7B64:
        mov    eax, 3
        jmp    SHORT loc_3_7B7B
loc_3_7B6B:
        mov    eax, 2
        jmp    SHORT loc_3_7B7B
loc_3_7B72:
        sub    eax, eax
        jmp    SHORT loc_3_7B7B
loc_3_7B76:
        mov    eax, 4
loc_3_7B7B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_7B80:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    ebx, 2
        cmp    dword ptr [esi + 2e0h], 1ah
        jne    NEAR PTR loc_3_7C1D
        cmp    dword ptr [esi + 7ebh], 1
        jne    SHORT loc_3_7C1D
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_7BC2
        db  00Fh, 0B7h, 00Dh, 020h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 0C9h, 089h, 0FCh, 0FFh
        add    esp, 8
loc_3_7BC2:
        lea    eax, [esi + 7dfh]
        mov    edi, dword ptr [esp + 14h]
        push   eax
        push   dword ptr [edi + 10h]
        push   esi
        call   sub_3_70CA
        mov    ebx, eax
        mov    dword ptr [esi + 2e0h], 1ah
        mov    dword ptr [esi + 7ebh], 9
        cmp    ebx, 2
        jne    SHORT loc_3_7C21
        mov    dword ptr [esi + 7ebh], 6
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_7C21
        db  00Fh, 0B7h, 00Dh, 0BAh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 070h, 089h, 0FCh, 0FFh
        add    esp, 8
        jmp    SHORT loc_3_7C21
loc_3_7C1D:
        mov    edi, dword ptr [esp + 14h]
loc_3_7C21:
        mov    dword ptr [edi + 4], ebx
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_7C28:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        mov    ebx, 2
        cmp    dword ptr [esi + 2e0h], 1ah
        jne    NEAR PTR loc_3_7D09
        cmp    dword ptr [esi + 7ebh], 9
        jne    NEAR PTR loc_3_7D09
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_7C6F
        db  00Fh, 0B7h, 00Dh, 01Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 01Ch, 089h, 0FCh, 0FFh
        add    esp, 8
loc_3_7C6F:
        mov    edi, dword ptr [esp + 18h]
        lea    eax, [esi + 7dfh]
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [esi + 7efh], ecx
        push   eax
        db  0B8h, 020h, 00Ch, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_3_7C91
        db  0B8h, 008h, 00Ch, 000h, 000h
loc_3_7C91:
        push   eax
        push   esi
        call   sub_3_70CA
        mov    ebx, eax
        mov    dword ptr [esi + 2e0h], 1ah
        cmp    ebx, 2
        jne    SHORT loc_3_7CD5
        mov    dword ptr [esi + 7ebh], 6
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_7D0D
        db  00Fh, 0B7h, 00Dh, 0B8h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 0B8h, 088h, 0FCh, 0FFh
        add    esp, 8
        jmp    SHORT loc_3_7D0D
loc_3_7CD5:
        mov    dword ptr [esi + 7ebh], 4
        mov    ebp, dword ptr [esi + 29ch]
        push   esi
        db  068h, 072h, 001h, 000h, 000h
        push   -1
        push   ebp
        db  0E8h, 0C5h, 087h, 0FCh, 0FFh
        add    esp, 10h
        push   esi
        db  068h, 072h, 001h, 000h, 000h
        push   -1
        push   ebp
        db  0E8h, 0BCh, 087h, 0FCh, 0FFh
        add    esp, 10h
        jmp    SHORT loc_3_7D0D
loc_3_7D09:
        mov    edi, dword ptr [esp + 18h]
loc_3_7D0D:
        mov    dword ptr [edi + 4], ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_7D15:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, 2
        cmp    dword ptr [esi + 2e0h], 1ah
        jne    NEAR PTR loc_3_7DFD
        mov    eax, dword ptr [esi + 7ebh]
        cmp    eax, 4
        je     SHORT loc_3_7D42
        cmp    eax, 5
        jne    NEAR PTR loc_3_7DFD
loc_3_7D42:
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_7D62
        db  00Fh, 0B7h, 00Dh, 01Eh, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 029h, 088h, 0FCh, 0FFh
        add    esp, 8
loc_3_7D62:
        push   0
        push   0
        push   -1
        push   dword ptr [esi + 29ch]
        db  0E8h, 04Dh, 087h, 0FCh, 0FFh
        add    esp, 10h
        push   0
        push   dword ptr [esi + 29ch]
        db  0E8h, 005h, 087h, 0FCh, 0FFh
        add    esp, 8
        push   1
        lea    eax, [esi + 640h]
        push   eax
        call   sub_3_64E3
        lea    eax, [esi + 7dfh]
        push   eax
        db  0B8h, 02Ch, 00Ch, 000h, 000h
        mov    ebx, dword ptr [esp + 18h]
        cmp    dword ptr [ebx + 8], 0
        jne    SHORT loc_3_7DAF
        db  0B8h, 014h, 00Ch, 000h, 000h
loc_3_7DAF:
        push   eax
        push   esi
        call   sub_3_70CA
        mov    edi, eax
        mov    dword ptr [esi + 2e0h], 1ah
        mov    dword ptr [esi + 7ebh], 1
        cmp    edi, 2
        jne    SHORT loc_3_7E01
        mov    dword ptr [esi + 7ebh], 6
        db  0A1h, 000h, 00Ch, 000h, 000h
        or     eax, eax
        je     SHORT loc_3_7E01
        db  00Fh, 0B7h, 00Dh, 0B8h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        push   eax
        db  0E8h, 090h, 087h, 0FCh, 0FFh
        add    esp, 8
        jmp    SHORT loc_3_7E01
loc_3_7DFD:
        mov    ebx, dword ptr [esp + 14h]
loc_3_7E01:
        mov    dword ptr [ebx + 4], edi
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_7E08:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 10h]
        add    eax, 40000000h
        cmp    eax, 5
        ja     SHORT loc_3_7E1F
        db  0FFh, 024h, 085h, 0B5h, 07Eh, 000h, 000h
loc_3_7E1F:
        sub    eax, eax
        jmp    NEAR PTR loc_3_7EB2
loc_3_7E26:
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esp + 14h]
        add    esi, 7a2h
        push   dword ptr [edi + 4]
        lea    eax, [edi + 0ch]
        push   eax
        push   esi
        call   dword ptr [esi]
        add    esp, 0ch
        mov    dword ptr [edi + 8], eax
        jmp    SHORT loc_3_7EAD
loc_3_7E46:
        mov    edi, dword ptr [esp + 14h]
        push   dword ptr [edi + 4]
        lea    eax, [edi + 0ch]
        push   eax
        push   dword ptr [esp + 14h]
        call   sub_3_7AFA
        add    esp, 0ch
        mov    dword ptr [edi + 8], eax
        jmp    SHORT loc_3_7EAD
loc_3_7E62:
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 10h]
        call   sub_3_7B80
        add    esp, 8
        jmp    SHORT loc_3_7EAD
loc_3_7E74:
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 10h]
        call   sub_3_7C28
        add    esp, 8
        jmp    SHORT loc_3_7EAD
loc_3_7E86:
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 10h]
        call   sub_3_7D15
        add    esp, 8
        jmp    SHORT loc_3_7EAD
loc_3_7E98:
        mov    esi, dword ptr [esp + 0ch]
        add    esi, 84dh
        mov    edi, dword ptr [esp + 14h]
        mov    ecx, 13h
        rep movsd
loc_3_7EAD:
        mov    eax, 1
loc_3_7EB2:
        pop    edi
        pop    esi
        ret
loc_3_7EB5:
        jle    NEAR PTR loc_3_7EB8
loc_3_7EB8:
        db  000h, 046h, 07Eh
        add    byte ptr [eax], al
        bound  edi, qword ptr [esi]
        db  000h, 074h, 07Eh, 000h
        db  000h, 086h, 07Eh, 000h, 000h, 098h
        jle    SHORT loc_3_7ECC
loc_3_7ECC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_7ED0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [esi + 0ch], eax
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [esi + 10h], ecx
        add    esi, 18h
        mov    edx, dword ptr [esp + 1ch]
        mov    dword ptr [esi - 4], edx
        db  0C7h, 046h, 0E8h, 00Eh, 07Fh, 000h, 000h
        push   ecx
        push   eax
        push   esi
        call   sub_3_7FC1
        pop    esi
        ret    18h
loc_3_7F0E:
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [esp + 18h], 2
        jne    SHORT loc_3_7F6E
        mov    eax, dword ptr [esp + 14h]
        mov    bl, byte ptr [eax + 1]
        cmp    bl, 80h
        jae    SHORT loc_3_7F6E
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 4]
        or     edi, edi
        je     SHORT loc_3_7F6E
        db  08Bh, 015h, 000h, 00Ch, 000h, 000h
        or     edx, edx
        je     SHORT loc_3_7F59
        movzx  eax, bl
        movzx  ecx, byte ptr [edi + eax]
        push   ecx
        push   eax
        db  00Fh, 0B7h, 005h, 02Ah, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        push   eax
        push   edx
        db  0E8h, 032h, 086h, 0FCh, 0FFh
        add    esp, 10h
loc_3_7F59:
        movzx  eax, bl
        mov    al, byte ptr [edi + eax]
        cmp    al, 0ffh
        je     SHORT loc_3_7F6E
        push   eax
        push   esi
        call   sub_3_7F77
        sub    eax, eax
        jmp    SHORT loc_3_7F73
loc_3_7F6E:
        mov    eax, 3
loc_3_7F73:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_7F77:
        push   ebx
        push   esi
        mov    bl, byte ptr [esp + 10h]
        cmp    bl, 11h
        ja     SHORT loc_3_7F92
        mov    esi, dword ptr [esp + 0ch]
        push   ebx
        add    esi, 18h
        push   esi
        call   sub_3_7FFF
        jmp    SHORT loc_3_7FBC
loc_3_7F92:
        cmp    bl, 22h
        mov    esi, dword ptr [esp + 0ch]
        jne    SHORT loc_3_7FA6
        push   dword ptr [esi + 14h]
        call   sub_3_6C71
        add    esp, 4
loc_3_7FA6:
        push   dword ptr [esi + 10h]
        push   0
        movzx  eax, bl
        add    eax, 510h
        push   eax
        push   dword ptr [esi + 0ch]
        db  0E8h, 034h, 087h, 0FCh, 0FFh
loc_3_7FBC:
        pop    esi
        pop    ebx
        ret    8
sub_3_7FC1:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 5], eax
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 9], eax
        ret    0ch
sub_3_7FDC:
        mov    al, byte ptr [esp + 4]
        cmp    al, 9
        ja     SHORT loc_3_7FE8
        add    al, 30h
        jmp    SHORT loc_3_7FFC
loc_3_7FE8:
        cmp    al, 0ah
        jb     SHORT loc_3_7FF4
        cmp    al, 0dh
        ja     SHORT loc_3_7FF4
        add    al, 37h
        jmp    SHORT loc_3_7FFC
loc_3_7FF4:
        cmp    al, 0eh
        mov    al, 2ah
        je     SHORT loc_3_7FFC
        mov    al, 23h
loc_3_7FFC:
        ret    4
sub_3_7FFF:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_3_8019
        cmp    eax, 1
        je     SHORT loc_3_804B
        cmp    eax, 2
        je     SHORT loc_3_8078
        jmp    NEAR PTR loc_3_80BF
loc_3_8019:
        mov    al, byte ptr [esp + 0ch]
        cmp    al, 10h
        jne    SHORT loc_3_802C
        mov    dword ptr [esi], 1
        jmp    NEAR PTR loc_3_80BF
loc_3_802C:
        cmp    al, 0fh
        ja     NEAR PTR loc_3_80BF
        mov    byte ptr [esi + 4], al
        push   dword ptr [esi + 9]
        push   eax
        call   sub_3_7FDC
        movzx  eax, al
        push   eax
        push   505h
        jmp    SHORT loc_3_80B7
loc_3_804B:
        mov    al, byte ptr [esp + 0ch]
        cmp    al, 0fh
        ja     SHORT loc_3_8070
        mov    byte ptr [esi + 4], al
        mov    dword ptr [esi], 2
        push   dword ptr [esi + 9]
        push   eax
        call   sub_3_7FDC
        movzx  eax, al
        push   eax
        push   503h
        jmp    SHORT loc_3_80B7
loc_3_8070:
        mov    dword ptr [esi], 0
        jmp    SHORT loc_3_80BF
loc_3_8078:
        mov    cl, byte ptr [esi + 4]
        mov    al, byte ptr [esp + 0ch]
        cmp    al, cl
        jne    SHORT loc_3_808B
        mov    dword ptr [esi], 2
        jmp    SHORT loc_3_80BF
loc_3_808B:
        cmp    al, 11h
        je     SHORT loc_3_8097
        cmp    al, 10h
        jne    SHORT loc_3_80BF
        cmp    al, 11h
        jne    SHORT loc_3_809F
loc_3_8097:
        mov    dword ptr [esi], 0
        jmp    SHORT loc_3_80A5
loc_3_809F:
        mov    dword ptr [esi], 1
loc_3_80A5:
        push   dword ptr [esi + 9]
        push   ecx
        call   sub_3_7FDC
        movzx  eax, al
        push   eax
        push   504h
loc_3_80B7:
        push   dword ptr [esi + 5]
        db  0E8h, 031h, 086h, 0FCh, 0FFh
loc_3_80BF:
        pop    esi
        ret    8
        db  000h
_PTEXT ends
        end
