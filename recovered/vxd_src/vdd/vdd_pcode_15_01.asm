; vdd_pcode_15_01.asm -- VDD object 15 segment _PTEXT  (part 1/2)
.386p
_PTEXT segment dword public use32 'PCODE'

loc_15_0000:
        test   edx, edx
        je     SHORT loc_15_000D
        cmp    edx, 0ah
        jne    NEAR PTR loc_15_0090
loc_15_000D:
        db  03Bh, 01Dh, 0A4h, 007h, 000h, 000h
        je     SHORT loc_15_0090
loc_15_0015:
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        pop    ebx
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  089h, 01Dh, 0A4h, 007h, 000h, 000h
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 040h
        jne    SHORT loc_15_0090
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 008h
        je     SHORT sub_15_0049
        db  080h, 00Dh, 0C4h, 006h, 000h, 000h, 080h
        clc
        ret
sub_15_0049:
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_15_005F
        push   0ffh
        push   1
        db  0E8h, 0CEh, 00Bh, 0F1h, 0FFh
        pop    ecx
        pop    ecx
loc_15_005F:
        call   sub_15_1802
        call   sub_15_12D5
        call   sub_15_35F9
        call   sub_15_119E
        call   sub_15_4B45
        call   sub_15_17B4
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_15_0090
        push   3
        push   1
        db  0E8h, 09Dh, 00Bh, 0F1h, 0FFh
        pop    ecx
        pop    ecx
loc_15_0090:
        clc
        ret
sub_15_0092:
        db  00Fh, 0BAh, 035h, 0C4h, 006h, 000h, 000h, 007h
        jb     SHORT loc_15_009D
        ret
loc_15_009D:
        db  08Bh, 01Dh, 0A4h, 007h, 000h, 000h
        jmp    NEAR PTR loc_15_0015
sub_15_00A8:
        db  03Bh, 01Dh, 0A4h, 007h, 000h, 000h
        jne    SHORT loc_15_010E
        test   byte ptr [edi + 1], 20h
        jne    SHORT loc_15_0107
        call   sub_15_015A
        jb     SHORT loc_15_0107
        call   sub_15_1802
        test   byte ptr [edi + 2], 40h
        jne    SHORT loc_15_0110
        call   sub_15_025E
        push   edi
        lea    esi, [edi + 543h]
        lea    edi, [edi + 57bh]
        mov    ecx, 4
loc_15_00DF:
        push   ecx
        movzx  ecx, word ptr [edi + 8]
        jecxz  loc_15_00F6
        push   esi
        push   edi
        mov    esi, dword ptr [esi + 4]
        mov    edi, dword ptr [edi + 4]
        cld
        shl    ecx, 0ah
        rep movsd
        pop    edi
        pop    esi
loc_15_00F6:
        add    esi, 0ch
        add    edi, 0ch
        pop    ecx
        loop   loc_15_00DF
        pop    edi
loc_15_0100:
        call   sub_15_17B4
        clc
        ret
loc_15_0107:
        call   sub_15_016E
        jmp    SHORT loc_15_010E
loc_15_010E:
        stc
        ret
loc_15_0110:
        push   edi
        lea    esi, [edi + 57bh]
        lea    edi, [edi + 543h]
        mov    ecx, 0eh
        cld
        rep movsd
        pop    edi
        call   sub_15_29F4
        mov    esi, dword ptr [edi + 5b8h]
        lea    esi, [esi + eax*4]
loc_15_0134:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_15_0141
        pushad
        call   sub_15_4009
        popad
loc_15_0141:
        add    esi, 4
        loop   loc_15_0134
        push   edi
        lea    edi, [edi + 543h]
        xor    eax, eax
        mov    ecx, 0eh
        cld
        rep stosd
        pop    edi
        jmp    SHORT loc_15_0100
sub_15_015A:
        lea    esi, [edi + 57bh]
        or     edx, 0ffffffffh
        call   sub_15_36D3
        call   sub_15_37CD
        ret
sub_15_016E:
        lea    esi, [edi + 57bh]
        call   sub_15_3695
        ret
sub_15_017A:
        call   sub_15_29F4
        lea    esi, [edi + 5f4h]
        cmp    cx, word ptr [esi + 0ah]
        jne    SHORT loc_15_0191
        cmp    dword ptr [esi + 4], 0
        jne    SHORT loc_15_01A9
loc_15_0191:
        cmp    cx, word ptr [esi + 0ah]
        ja     SHORT loc_15_01A0
        test   byte ptr [edi + 5e2h], 80h
        jne    SHORT loc_15_01A9
loc_15_01A0:
        mov    word ptr [esi + 8], cx
        call   sub_15_376B
loc_15_01A9:
        mov    edx, dword ptr [esi + 4]
        movzx  ecx, word ptr [esi + 0ah]
        ret
sub_15_01B1:
        test   byte ptr [edi + 5e2h], 80h
        jne    SHORT loc_15_01E6
        xor    eax, eax
        mov    dword ptr [edi + 5f8h], eax
        mov    word ptr [edi + 5fch], ax
        mov    word ptr [edi + 5feh], ax
        xchg   dword ptr [edi + 5f4h], eax
        or     eax, eax
        je     SHORT loc_15_01E6
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_15_01E6:
        ret
sub_15_01E7:
        mov    eax, dword ptr [edi + 5c0h]
        or     eax, eax
        stc
        jne    SHORT loc_15_024A
        cmp    dword ptr [edi + 5c8h], 0ffff0000h
        stc
        jne    SHORT loc_15_024A
        pushad
        call   sub_15_29F4
        mov    edx, ecx
        call   sub_15_2B01
        mov    ecx, eax
        shl    ecx, 0ch
        add    ecx, dword ptr [ebx + 4]
        shr    ecx, 0ch
        shl    edx, 2
        sub    esp, edx
        mov    eax, esp
        shr    edx, 2
        push   edx
        push   0
        push   eax
        push   edx
        push   ecx
        db  0CDh, 020h, 061h, 000h, 001h, 000h
        add    esp, 10h
        pop    ecx
        mov    eax, esp
        lea    edx, [ecx*4]
        xor    ebx, ebx
loc_15_023C:
        or     bl, byte ptr [eax]
        add    eax, 4
        loop   loc_15_023C
        add    esp, edx
        bt     ebx, 6
        popad
loc_15_024A:
        ret
sub_15_024B:
        mov    ecx, 1
        push   edi
        lea    edi, [edi + 5c4h]
        cld
        xor    eax, eax
        rep stosd
        pop    edi
        ret
sub_15_025E:
        push   esi
        mov    esi, dword ptr [edi + 5b8h]
        xor    ecx, ecx
loc_15_0267:
        mov    eax, dword ptr [esi + ecx*4]
        or     eax, eax
        je     SHORT loc_15_02D2
        call   sub_15_3F3C
        je     SHORT loc_15_02B9
        push   ecx
        push   esi
        call   sub_15_10CC
        call   sub_15_4009
        movzx  ecx, word ptr [eax + 6]
        jecxz  loc_15_02A6
loc_15_0287:
        push   ecx
        push   0
        push   -1
        push   0
        push   -1
        push   1
        push   ecx
        push   ebx
        db  0CDh, 020h, 060h, 000h, 001h, 000h
        add    esp, 1ch
        pop    ecx
        shl    ecx, 0ch
        add    ecx, dword ptr [ebx + 4]
        mov    cl, byte ptr [ecx]
loc_15_02A6:
        pop    esi
        pop    ecx
        mov    eax, dword ptr [edi + 5bch]
        or     byte ptr [ecx + eax], 1
        mov    eax, ecx
        call   sub_15_456A
loc_15_02B9:
        inc    ecx
        cmp    ecx, dword ptr [edi + 5b4h]
        jb     SHORT loc_15_0267
        pop    esi
        db  0F6h, 005h, 0C5h, 006h, 000h, 000h, 010h
        je     SHORT loc_15_02D1
        call   sub_15_10EA
loc_15_02D1:
        ret
loc_15_02D2:
        push   ecx
        push   esi
        mov    eax, ecx
        call   sub_15_3A9D
        jae    SHORT loc_15_02EE
        mov    esi, ebx
        push   eax
        call   sub_15_2D54
        pop    eax
        xchg   ebx, esi
        jb     SHORT loc_15_030A
        cmp    ebx, esi
        jne    SHORT loc_15_030A
loc_15_02EE:
        call   sub_15_2B01
        cmp    eax, 0c0h
        jae    SHORT loc_15_030A
        push   eax
        mov    ecx, eax
        call   sub_15_3F19
        test   cl, 40h
        jne    SHORT loc_15_030E
        add    esp, 4
loc_15_030A:
        pop    esi
        pop    ecx
        jmp    SHORT loc_15_02B9
loc_15_030E:
        pop    ecx
        jmp    NEAR PTR loc_15_0287
sub_15_0314:
        call   sub_15_025E
        call   sub_15_29F4
        or     byte ptr [esi], 10h
        mov    word ptr [esi + 2], cx
        add    ecx, 7
        shr    ecx, 3
        push   ebx
        push   ecx
        push   esi
        push   edi
        xor    ch, ch
        lea    ebx, [edi + 5c0h]
        lea    edi, [edi + 5c4h]
        lea    esi, [esi + 4]
        xchg   esi, edi
        cld
loc_15_0343:
        mov    al, byte ptr [ebx]
        mov    byte ptr [ebx], 0
        or     al, byte ptr [esi]
        mov    byte ptr [esi], al
        or     ch, al
        stosb
        inc    ebx
        inc    esi
        dec    cl
        jne    SHORT loc_15_0343
        pop    edi
        pop    esi
        or     ch, ch
        pop    ecx
        jne    SHORT loc_15_0370
        cmp    dword ptr [edi + 5c8h], 0ffff0000h
        jne    SHORT loc_15_036E
        and    word ptr [esi], 0ffefh
        xor    ecx, ecx
loc_15_036E:
        pop    ebx
        ret
loc_15_0370:
        mov    dword ptr [edi + 5c8h], 0ffffffffh
        pop    ebx
        ret
sub_15_037C:
        db  03Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        jne    NEAR PTR loc_15_047D
        db  083h, 03Dh, 000h, 00Fh, 000h, 000h, 000h
        jne    SHORT loc_15_03A6
        db  0A1h, 0F8h, 00Eh, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_15_05F2
        mov    byte ptr [eax], 0
        jmp    NEAR PTR loc_15_05F2
loc_15_03A6:
        db  080h, 025h, 0A2h, 007h, 000h, 000h, 0FBh
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        cmp    eax, ebx
        je     SHORT loc_15_03D2
        db  080h, 00Dh, 0A2h, 007h, 000h, 000h, 004h
        db  0BEh, 07Ch, 003h, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 001h, 000h
        jmp    NEAR PTR loc_15_047B
loc_15_03D2:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        mov    esi, edi
        db  080h, 00Dh, 0A0h, 007h, 000h, 000h, 001h
        xor    ecx, ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        call   sub_15_05F4
        mov    byte ptr [449h], 6
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 18h], 0
        je     SHORT loc_15_0406
        call   dword ptr [edi + 18h]
loc_15_0406:
        pop    edi
        push   ebx
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  00Fh, 0B7h, 015h, 000h, 00Fh, 000h, 000h
        db  066h, 08Bh, 00Dh, 002h, 00Fh, 000h, 000h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    ebx
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 1ch], 0
        je     SHORT loc_15_045C
        call   dword ptr [edi + 1ch]
loc_15_045C:
        pop    edi
        call   sub_15_062A
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        db  080h, 025h, 0A0h, 007h, 000h, 000h, 0FEh
        db  0A1h, 0F8h, 00Eh, 000h, 000h
        or     eax, eax
        je     SHORT loc_15_047B
        mov    byte ptr [eax], 0
loc_15_047B:
        stc
        ret
loc_15_047D:
        db  0A1h, 0FCh, 00Eh, 000h, 000h
        or     eax, eax
        je     SHORT loc_15_0490
        db  066h, 08Bh, 00Dh, 0B2h, 00Fh, 000h, 000h
        or     word ptr [eax], cx
loc_15_0490:
        db  0A1h, 0F8h, 00Eh, 000h, 000h
        or     eax, eax
        je     SHORT loc_15_049C
        mov    byte ptr [eax], 1
loc_15_049C:
        db  03Bh, 01Dh, 0B8h, 007h, 000h, 000h
        je     SHORT loc_15_04D2
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        test   byte ptr [edi + 1], 20h
        jne    SHORT loc_15_04B9
        db  0E8h, 0D5h, 0FDh, 0F0h, 0FFh
        jae    SHORT loc_15_04D2
loc_15_04B9:
        db  0BEh, 015h, 00Eh, 000h, 000h
        mov    eax, 1000h
        mov    ecx, 3
        xor    edx, edx
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        stc
        ret
loc_15_04D2:
        db  08Bh, 00Dh, 0A8h, 007h, 000h, 000h
        db  03Bh, 00Dh, 0F0h, 00Eh, 000h, 000h
        je     SHORT loc_15_04F0
        db  003h, 00Dh, 0CCh, 006h, 000h, 000h
        test   byte ptr [ecx + 1], 20h
        je     NEAR PTR loc_15_05F2
loc_15_04F0:
        xor    ecx, ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 10h], 0
        je     SHORT loc_15_0507
        call   dword ptr [edi + 10h]
loc_15_0507:
        pop    edi
        call   sub_15_05F4
        mov    eax, 400h
        db  03Bh, 01Dh, 0B8h, 007h, 000h, 000h
        je     SHORT loc_15_051D
        add    eax, dword ptr [ebx + 4]
loc_15_051D:
        mov    dx, word ptr [eax + 60h]
        shl    edx, 10h
        mov    dl, byte ptr [eax + 62h]
        mov    dh, byte ptr [eax + 49h]
        push   edx
        mov    dl, byte ptr [eax + 4ah]
        mov    dh, byte ptr [eax + 84h]
        shl    edx, 10h
        mov    dl, byte ptr [eax + 85h]
        mov    dh, byte ptr [eax + 89h]
        push   edx
        push   dword ptr [eax + 50h]
        push   dword ptr [eax + 54h]
        push   dword ptr [eax + 58h]
        push   dword ptr [eax + 5ch]
        push   dword ptr [eax + 87h]
        sub    eax, 400h
        push   dword ptr [eax + 10ch]
        push   eax
        movzx  eax, byte ptr [eax + 449h]
        db  03Bh, 01Dh, 0B8h, 007h, 000h, 000h
        jne    SHORT loc_15_0573
        mov    al, 3
loc_15_0573:
        or     al, 80h
        push   ebx
        db  080h, 00Dh, 0A0h, 007h, 000h, 000h, 002h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebx
        pop    eax
        pop    dword ptr [eax + 10ch]
        add    eax, 400h
        pop    dword ptr [eax + 87h]
        pop    dword ptr [eax + 5ch]
        pop    dword ptr [eax + 58h]
        pop    dword ptr [eax + 54h]
        pop    dword ptr [eax + 50h]
        pop    edx
        mov    byte ptr [eax + 85h], dl
        mov    byte ptr [eax + 89h], dh
        shr    edx, 10h
        mov    byte ptr [eax + 4ah], dl
        mov    byte ptr [eax + 84h], dh
        pop    edx
        mov    byte ptr [eax + 49h], dh
        mov    byte ptr [eax + 62h], dl
        shr    edx, 10h
        mov    word ptr [eax + 60h], dx
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 14h], 0
        je     SHORT loc_15_05DD
        call   dword ptr [edi + 14h]
loc_15_05DD:
        pop    edi
        call   sub_15_062A
        db  080h, 025h, 0A0h, 007h, 000h, 000h, 0FDh
        mov    esi, edi
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_15_05F2:
        clc
        ret
sub_15_05F4:
        db  0F6h, 005h, 0C8h, 006h, 000h, 000h, 008h
        je     SHORT loc_15_0618
        mov    edx, 3b4h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    dl, 0b5h
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        mov    dl, 0bah
        db  0CDh, 020h, 099h, 000h, 001h, 000h
loc_15_0618:
        mov    edx, 3c0h
loc_15_061D:
        db  0CDh, 020h, 099h, 000h, 001h, 000h
        inc    edx
        cmp    dl, 0e0h
        jb     SHORT loc_15_061D
        ret
sub_15_062A:
        db  0F6h, 005h, 0C8h, 006h, 000h, 000h, 008h
        je     SHORT loc_15_064E
        mov    edx, 3b4h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    dl, 0b5h
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        mov    dl, 0bah
        db  0CDh, 020h, 097h, 000h, 001h, 000h
loc_15_064E:
        mov    edx, 3c0h
loc_15_0653:
        db  0CDh, 020h, 097h, 000h, 001h, 000h
        inc    edx
        cmp    dl, 0e0h
        jb     SHORT loc_15_0653
        ret
sub_15_0660:
        pushfd
        pushad
        db  080h, 03Dh, 0A1h, 007h, 000h, 000h, 000h
        je     SHORT loc_15_06BF
        call   sub_15_0865
        db  0F6h, 005h, 0A0h, 007h, 000h, 000h, 003h
        jne    SHORT loc_15_06B8
        mov    cl, 1
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 28h], 0
        je     SHORT loc_15_068A
        call   dword ptr [edi + 28h]
loc_15_068A:
        pop    edi
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        call   sub_15_119E
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 30h], 0
        je     SHORT loc_15_06A7
        call   dword ptr [edi + 30h]
loc_15_06A7:
        pop    edi
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0ch], 0
        je     SHORT loc_15_06B7
        call   dword ptr [edi + 0ch]
loc_15_06B7:
        pop    edi
loc_15_06B8:
        db  0C6h, 005h, 0A1h, 007h, 000h, 000h, 000h
loc_15_06BF:
        popad
        popfd
        ret
loc_15_06C2:
        db  03Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        jne    NEAR PTR loc_15_0779
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    NEAR PTR loc_15_0779
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        jne    SHORT loc_15_0732
        push   eax
        push   ecx
        push   edx
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0ch], 0
        je     SHORT loc_15_06F4
        call   dword ptr [edi + 0ch]
loc_15_06F4:
        pop    edi
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 38h], 0
        je     SHORT loc_15_0704
        call   dword ptr [edi + 38h]
loc_15_0704:
        pop    edi
        xor    cl, cl
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 28h], 0
        je     SHORT loc_15_0716
        call   dword ptr [edi + 28h]
loc_15_0716:
        pop    edi
        push   esi
        mov    ecx, ebx
        mov    esi, ebx
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 24h], 0
        je     SHORT loc_15_072B
        call   dword ptr [edi + 24h]
loc_15_072B:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    eax
        jmp    SHORT loc_15_0779
loc_15_0732:
        push   eax
        push   ecx
        push   edx
        push   ebp
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0ch], 0
        je     SHORT loc_15_0745
        call   dword ptr [edi + 0ch]
loc_15_0745:
        pop    edi
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 38h], 0
        je     SHORT loc_15_0755
        call   dword ptr [edi + 38h]
loc_15_0755:
        pop    edi
        xor    cl, cl
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 28h], 0
        je     SHORT loc_15_0767
        call   dword ptr [edi + 28h]
loc_15_0767:
        pop    edi
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        call   sub_15_119E
        pop    ebp
        pop    edx
        pop    ecx
        pop    eax
loc_15_0779:
        clc
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0b0h], 0
        je     SHORT loc_15_0793
        popfd
        call   dword ptr [edi + 0b0h]
        jmp    SHORT loc_15_0794
loc_15_0793:
        popfd
loc_15_0794:
        pop    edi
        jb     SHORT loc_15_079D
        db  0CDh, 020h, 015h, 000h, 00Ah, 000h
loc_15_079D:
        ret
sub_15_079E:
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    NEAR PTR loc_15_083D
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        jne    SHORT loc_15_07FC
        push   eax
        push   ecx
        push   edx
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0ch], 0
        je     SHORT loc_15_07C4
        call   dword ptr [edi + 0ch]
loc_15_07C4:
        pop    edi
        db  081h, 025h, 09Ch, 010h, 000h, 000h, 0FFh, 0FFh, 0FFh, 07Fh
        xor    cl, cl
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 28h], 0
        je     SHORT loc_15_07E0
        call   dword ptr [edi + 28h]
loc_15_07E0:
        pop    edi
        push   esi
        mov    ecx, ebx
        mov    esi, ebx
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 24h], 0
        je     SHORT loc_15_07F5
        call   dword ptr [edi + 24h]
loc_15_07F5:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    eax
        jmp    SHORT loc_15_083D
loc_15_07FC:
        push   eax
        push   ecx
        push   edx
        push   ebp
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0ch], 0
        je     SHORT loc_15_080F
        call   dword ptr [edi + 0ch]
loc_15_080F:
        pop    edi
        db  081h, 025h, 09Ch, 010h, 000h, 000h, 0FFh, 0FFh, 0FFh, 07Fh
        xor    cl, cl
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 28h], 0
        je     SHORT loc_15_082B
        call   dword ptr [edi + 28h]
loc_15_082B:
        pop    edi
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        call   sub_15_119E
        pop    ebp
        pop    edx
        pop    ecx
        pop    eax
loc_15_083D:
        ret
sub_15_083E:
        push   ebx
        xor    eax, eax
        db  08Bh, 00Dh, 0D2h, 00Eh, 000h, 000h
loc_15_0847:
        push   ecx
        push   eax
        call   sub_15_2D54
        jb     SHORT loc_15_085D
        db  03Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        je     SHORT loc_15_085D
        call   sub_15_2D96
loc_15_085D:
        pop    eax
        pop    ecx
        inc    eax
        dec    ecx
        jne    SHORT loc_15_0847
        pop    ebx
        ret
sub_15_0865:
        push   ebx
        push   edi
        db  08Bh, 01Dh, 0ACh, 007h, 000h, 000h
        db  0A1h, 0CCh, 00Eh, 000h, 000h
        db  08Bh, 00Dh, 0D2h, 00Eh, 000h, 000h
        sub    ecx, eax
        jle    SHORT loc_15_08AA
loc_15_087C:
        push   ebx
        push   ecx
        push   eax
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        call   sub_15_2D54
        jb     SHORT loc_15_08A3
        db  03Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        je     SHORT loc_15_08A3
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        call   sub_15_3EDC
loc_15_08A3:
        pop    eax
        pop    ecx
        pop    ebx
        inc    eax
        dec    ecx
        jne    SHORT loc_15_087C
loc_15_08AA:
        pop    edi
        pop    ebx
        ret
        db  0EBh, 006h, 0FFh, 025h, 098h, 007h, 000h, 000h
loc_15_08B5:
        pushad
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    NEAR PTR loc_15_09F4
        mov    ebp, dword ptr [ebx + 8]
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        xor    eax, eax
        test   byte ptr [edi + 1], 20h
        je     SHORT loc_15_08D6
        dec    eax
loc_15_08D6:
        mov    ecx, dword ptr [edi + 620h]
        clc
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0ach], 0
        je     SHORT loc_15_08F6
        popfd
        call   dword ptr [edi + 0ach]
        jmp    SHORT loc_15_08F7
loc_15_08F6:
        popfd
loc_15_08F7:
        pop    edi
        jb     NEAR PTR loc_15_09FB
        test   byte ptr [edi + 1], 20h
        jne    SHORT loc_15_0926
        cmp    dword ptr [edi + 620h], 13h
        jbe    NEAR PTR loc_15_09F4
        db  083h, 03Dh, 07Ch, 007h, 000h, 000h, 000h
        je     NEAR PTR loc_15_09FB
        or     eax, eax
        jne    NEAR PTR loc_15_09FB
loc_15_0926:
        db  066h, 0F7h, 005h, 01Bh, 00Bh, 000h, 000h, 000h, 008h
        je     NEAR PTR loc_15_09FB
        mov    ecx, dword ptr [edi + 61ch]
        cmp    ecx, -1
        je     NEAR PTR loc_15_09FB
        or     ecx, ecx
        je     NEAR PTR loc_15_09FB
        push   ecx
        mov    eax, dword ptr [edi + 614h]
        or     eax, eax
        je     SHORT loc_15_0981
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [edi + 614h], 0
        mov    dword ptr [edi + 618h], 0
        mov    dword ptr [edi + 62ch], 0
loc_15_0981:
        pop    ecx
        add    ecx, 0fffh
        shr    ecx, 0ch
        push   0
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
        mov    ecx, eax
        or     ecx, edx
        je     SHORT loc_15_09FB
        mov    dword ptr [edi + 614h], eax
        mov    dword ptr [edi + 618h], edx
        db  080h, 00Dh, 0A2h, 007h, 000h, 000h, 001h
        db  0BEh, 0ECh, 00Ah, 000h, 000h
        mov    eax, 1000h
        mov    ecx, 1
        xor    edx, edx
        xor    edi, edi
        db  0CDh, 020h, 014h, 000h, 001h, 000h
loc_15_09D4:
        db  0F6h, 005h, 0A2h, 007h, 000h, 000h, 001h
        je     SHORT loc_15_09F4
        db  0B8h, 09Ch, 007h, 000h, 000h
        db  0A3h, 09Ch, 007h, 000h, 000h
        push   1
        push   eax
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        jmp    SHORT loc_15_09D4
loc_15_09F4:
        popad
        db  0FFh, 025h, 098h, 007h, 000h, 000h
loc_15_09FB:
        popad
        stc
        ret
sub_15_09FE:
        push   eax
        push   edi
        push   ebp
        mov    ecx, 0ffffffffh
        db  080h, 03Dh, 0A3h, 007h, 000h, 000h, 000h
        jne    NEAR PTR loc_15_0AE8
        db  0C6h, 005h, 0A3h, 007h, 000h, 000h, 0FFh
        mov    ebp, dword ptr [ebx + 8]
        clc
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 90h], 0
        je     SHORT loc_15_0A37
        popfd
        call   dword ptr [edi + 90h]
        jmp    SHORT loc_15_0A38
loc_15_0A37:
        popfd
loc_15_0A38:
        pop    edi
        jb     NEAR PTR loc_15_0ACC
        mov    ecx, 100h
        clc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        jb     NEAR PTR loc_15_0ADC
        cmp    ecx, 100h
        jb     SHORT loc_15_0AD5
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp], di
        ror    edi, 10h
        mov    word ptr [ebp + 38h], di
        mov    word ptr [ebp + 1ch], 4f00h
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    di, word ptr [ebp + 1ch]
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        cmp    di, 4fh
        jne    SHORT loc_15_0AD5
        movzx  eax, word ptr [eax + 12h]
        shl    eax, 10h
        or     eax, eax
        je     SHORT loc_15_0AD5
        mov    ecx, 100h
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        mov    ecx, eax
loc_15_0ACC:
        db  0C6h, 005h, 0A3h, 007h, 000h, 000h, 000h
        jmp    SHORT loc_15_0AE8
loc_15_0AD5:
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
loc_15_0ADC:
        db  0C6h, 005h, 0A3h, 007h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
loc_15_0AE8:
        pop    ebp
        pop    edi
        pop    eax
        ret
loc_15_0AEC:
        cld
        pushad
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        mov    dword ptr [edi + 62ch], 0
        xor    ecx, ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        db  0CDh, 020h, 000h, 000h, 00Ch, 000h
        cmp    eax, 400h
        jb     SHORT loc_15_0B1D
        xor    eax, eax
        db  0CDh, 020h, 00Ah, 000h, 00Ch, 000h
loc_15_0B1D:
        mov    edx, 21h
        mov    ecx, 0
        db  0CDh, 020h, 030h, 001h, 001h, 000h
        mov    byte ptr [edi + 613h], al
        or     al, 1
        mov    edx, 21h
        mov    ecx, 4
        db  0CDh, 020h, 030h, 001h, 001h, 000h
        clc
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 80h], 0
        je     SHORT loc_15_0B5F
        popfd
        call   dword ptr [edi + 80h]
        jmp    SHORT loc_15_0B60
loc_15_0B5F:
        popfd
loc_15_0B60:
        pop    edi
        jb     SHORT loc_15_0B77
        push   ebx
        mov    ax, 4f05h
        mov    bx, 100h
        xor    edx, edx
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebx
loc_15_0B77:
        mov    word ptr [edi + 60fh], dx
        clc
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 84h], 0
        je     SHORT loc_15_0B98
        popfd
        call   dword ptr [edi + 84h]
        jmp    SHORT loc_15_0B99
loc_15_0B98:
        popfd
loc_15_0B99:
        pop    edi
        jb     SHORT loc_15_0BB0
        push   ebx
        mov    ax, 4f05h
        mov    bx, 101h
        xor    edx, edx
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebx
loc_15_0BB0:
        mov    word ptr [edi + 611h], dx
        mov    edx, dword ptr [edi + 620h]
        and    dh, 3fh
        clc
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 94h], 0
        je     SHORT loc_15_0BDA
        popfd
        call   dword ptr [edi + 94h]
        jmp    SHORT loc_15_0BDB
loc_15_0BDA:
        popfd
loc_15_0BDB:
        pop    edi
        jae    SHORT loc_15_0BFF
        mov    ecx, eax
        db  0A1h, 0C8h, 00Eh, 000h, 000h
        or     ecx, ecx
        je     NEAR PTR loc_15_0CA3
        push   0
        push   edx
        push   eax
        db  0CDh, 020h, 06Ch, 000h, 001h, 000h
        add    esp, 0ch
        jmp    NEAR PTR loc_15_0CA3
loc_15_0BFF:
        mov    ecx, 100h
        clc
        db  0CDh, 020h, 008h, 000h, 006h, 000h
        jb     NEAR PTR loc_15_0DED
        cmp    ecx, 100h
        jb     SHORT loc_15_0C84
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp], di
        ror    edi, 10h
        mov    word ptr [ebp + 38h], di
        mov    word ptr [ebp + 18h], dx
        mov    word ptr [ebp + 1ch], 4f01h
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        mov    dx, word ptr [ebp + 1ch]
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        cmp    dx, 4fh
        je     SHORT loc_15_0C90
        mov    ecx, 100h
loc_15_0C84:
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
        jmp    NEAR PTR loc_15_0DED
loc_15_0C90:
        movzx  edx, word ptr [eax + 4]
        shl    edx, 0ah
        db  0A1h, 0C8h, 00Eh, 000h, 000h
        clc
        db  0CDh, 020h, 009h, 000h, 006h, 000h
loc_15_0CA3:
        push   eax
        push   ebx
        push   edx
        push   ebp
        push   edi
        mov    eax, 4f07h
        mov    ebx, 1
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edi
        mov    word ptr [edi + 634h], 0ffffh
        mov    word ptr [edi + 636h], 0ffffh
        cmp    ax, 4fh
        jne    SHORT loc_15_0CE1
        mov    word ptr [edi + 634h], cx
        mov    word ptr [edi + 636h], dx
loc_15_0CE1:
        pop    ebp
        pop    edx
        pop    ebx
        pop    eax
        push   ebx
        push   edx
        mov    dword ptr [edi + 62ch], edx
        mov    dword ptr [edi + 630h], eax
        db  0CDh, 020h, 000h, 000h, 00Ch, 000h
        cmp    eax, 400h
        jb     SHORT loc_15_0D0B
        mov    eax, 3
        db  0CDh, 020h, 005h, 000h, 00Ch, 000h
loc_15_0D0B:
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 9ch], 0
        je     SHORT loc_15_0D20
        call   dword ptr [edi + 9ch]
loc_15_0D20:
        pop    edi
        xor    edx, edx
        xor    eax, eax
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 88h], 0
        je     SHORT loc_15_0D3E
        popfd
        call   dword ptr [edi + 88h]
        jmp    SHORT loc_15_0D3F
loc_15_0D3E:
        popfd
loc_15_0D3F:
        pop    edi
        jb     SHORT loc_15_0D63
        mov    ax, 4f05h
        xor    ebx, ebx
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    ax, 4f05h
        mov    ebx, 1
        xor    edx, edx
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_15_0D63:
        pop    edx
        shr    edx, 2
        mov    ecx, dword ptr [edi + 61ch]
        shr    ecx, 2
        mov    ebx, edx
        xor    edx, edx
        mov    esi, dword ptr [edi + 630h]
        mov    edi, dword ptr [edi + 618h]
loc_15_0D80:
        push   ecx
        push   esi
        mov    ecx, ebx
        rep movsd
        pop    esi
        pop    ecx
        sub    ecx, ebx
        je     SHORT loc_15_0DD6
        inc    edx
        clc
        mov    eax, edx
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 88h], 0
        je     SHORT loc_15_0DA9
        popfd
        call   dword ptr [edi + 88h]
        jmp    SHORT loc_15_0DAA
loc_15_0DA9:
        popfd
loc_15_0DAA:
        pop    edi
        jb     SHORT loc_15_0D80
        push   ebx
        push   ecx
        push   edx
        mov    ax, 4f05h
        xor    ebx, ebx
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edx
        push   edx
        mov    ax, 4f05h
        mov    ebx, 1
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edx
        pop    ecx
        pop    ebx
        jmp    SHORT loc_15_0D80
loc_15_0DD6:
        pop    ebx
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0a0h], 0
        je     SHORT loc_15_0DEC
        call   dword ptr [edi + 0a0h]
loc_15_0DEC:
        pop    edi
loc_15_0DED:
        db  080h, 025h, 0A2h, 007h, 000h, 000h, 0FEh
        db  0FFh, 035h, 09Ch, 007h, 000h, 000h
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
        db  0C7h, 005h, 09Ch, 007h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        popad
        ret
loc_15_0E15:
        cld
        pushad
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        xor    ecx, ecx
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        cmp    dword ptr [edi + 62ch], 0
        jne    SHORT loc_15_0E56
        xor    edi, edi
        db  0BEh, 0E8h, 002h, 000h, 000h
        mov    edx, ebx
        mov    eax, 30h
        db  00Fh, 0B7h, 00Dh, 014h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  0CDh, 020h, 004h, 000h, 017h, 000h
        jmp    NEAR PTR loc_15_10C2
loc_15_0E56:
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 10h], 0
        je     SHORT loc_15_0E65
        call   dword ptr [edi + 10h]
loc_15_0E65:
        pop    edi
        db  080h, 00Dh, 0A0h, 007h, 000h, 000h, 002h
        call   sub_15_05F4
        mov    eax, dword ptr [edi + 620h]
        clc
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 98h], 0
        je     SHORT loc_15_0E92
        popfd
        call   dword ptr [edi + 98h]
        jmp    SHORT loc_15_0E93
loc_15_0E92:
        popfd
loc_15_0E93:
        pop    edi
        jb     SHORT loc_15_0EC2
        push   ebx
        mov    ebx, eax
        mov    eax, 4f02h
        or     bh, 80h
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebx
        cmp    ax, 4fh
        jne    NEAR PTR loc_15_1030
        and    byte ptr [449h], 7fh
        and    byte ptr [487h], 7fh
loc_15_0EC2:
        push   ebx
        push   edi
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 9ch], 0
        je     SHORT loc_15_0ED9
        call   dword ptr [edi + 9ch]
loc_15_0ED9:
        pop    edi
        mov    edx, dword ptr [edi + 62ch]
        or     edx, edx
        je     NEAR PTR loc_15_0F9C
        push   edx
        xor    edx, edx
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 88h], 0
        je     SHORT loc_15_0F04
        popfd
        call   dword ptr [edi + 88h]
        jmp    SHORT loc_15_0F05
loc_15_0F04:
        popfd
loc_15_0F05:
        pop    edi
        jb     SHORT loc_15_0F29
        mov    ax, 4f05h
        xor    ebx, ebx
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    ax, 4f05h
        mov    ebx, 1
        xor    edx, edx
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
loc_15_0F29:
        pop    edx
        shr    edx, 2
        mov    ecx, dword ptr [edi + 61ch]
        shr    ecx, 2
        mov    esi, dword ptr [edi + 618h]
        mov    edi, dword ptr [edi + 630h]
        mov    ebx, edx
        xor    edx, edx
loc_15_0F46:
        push   ecx
        push   edi
        mov    ecx, ebx
        rep movsd
        pop    edi
        pop    ecx
        sub    ecx, ebx
        je     SHORT loc_15_0F9C
        inc    edx
        mov    eax, edx
        clc
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 88h], 0
        je     SHORT loc_15_0F6F
        popfd
        call   dword ptr [edi + 88h]
        jmp    SHORT loc_15_0F70
loc_15_0F6F:
        popfd
loc_15_0F70:
        pop    edi
        jb     SHORT loc_15_0F46
        push   ebx
        push   ecx
        push   edx
        mov    ax, 4f05h
        xor    ebx, ebx
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edx
        push   edx
        mov    ax, 4f05h
        mov    ebx, 1
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edx
        pop    ecx
        pop    ebx
        jmp    SHORT loc_15_0F46
loc_15_0F9C:
        pop    edi
        pop    ebx
        call   sub_15_1657
        mov    edx, 3d4h
        call   sub_15_1639
        cmp    word ptr [edi + 634h], -1
        je     SHORT loc_15_0FDA
        push   ebx
        push   ebp
        push   edi
        mov    eax, 4f07h
        xor    ebx, ebx
        mov    cx, word ptr [edi + 634h]
        mov    dx, word ptr [edi + 636h]
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    edi
        pop    ebp
        pop    ebx
loc_15_0FDA:
        movzx  edx, word ptr [edi + 60fh]
        movzx  eax, word ptr [edi + 611h]
        clc
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 88h], 0
        je     SHORT loc_15_0FFE
        call   dword ptr [edi + 88h]
loc_15_0FFE:
        pop    edi
        jb     SHORT loc_15_1030
        push   ebx
        mov    ax, 4f05h
        xor    bx, bx
        movzx  edx, word ptr [edi + 60fh]
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        mov    ax, 4f05h
        mov    bx, 1
        movzx  edx, word ptr [edi + 611h]
        push   10h
        db  0CDh, 020h, 08Fh, 000h, 001h, 000h
        pop    ebx
loc_15_1030:
        mov    eax, dword ptr [edi + 614h]
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [edi + 614h], 0
        mov    dword ptr [edi + 618h], 0
        mov    dword ptr [edi + 62ch], 0
        push   edi
        db  080h, 025h, 0A0h, 007h, 000h, 000h, 0FDh
        call   sub_15_4BBC
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0a0h], 0
        je     SHORT loc_15_1082
        call   dword ptr [edi + 0a0h]
loc_15_1082:
        pop    edi
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 14h], 0
        je     SHORT loc_15_1092
        call   dword ptr [edi + 14h]
loc_15_1092:
        pop    edi
        pop    edi
        mov    al, byte ptr [edi + 613h]
        mov    edx, 21h
        mov    ecx, 4
        db  0CDh, 020h, 030h, 001h, 001h, 000h
        db  0CDh, 020h, 000h, 000h, 00Ch, 000h
        cmp    eax, 400h
        jb     SHORT loc_15_10C2
        mov    eax, 1
        db  0CDh, 020h, 00Ah, 000h, 00Ch, 000h
loc_15_10C2:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        popad
        ret
        db  000h, 000h
sub_15_10CC:
        db  00Fh, 0BAh, 02Dh, 0C4h, 006h, 000h, 000h, 00Ch
        jb     SHORT loc_15_10E9
        db  080h, 025h, 0C5h, 006h, 000h, 000h, 09Fh
        push   eax
        db  0A1h, 0ACh, 007h, 000h, 000h
        db  0A3h, 0B4h, 007h, 000h, 000h
        pop    eax
loc_15_10E9:
        ret
sub_15_10EA:
        db  00Fh, 0BAh, 035h, 0C4h, 006h, 000h, 000h, 00Ch
        jae    SHORT loc_15_1136
        push   eax
        db  0A1h, 0B4h, 007h, 000h, 000h
        cmp    eax, ebx
        jne    SHORT loc_15_1100
        xor    eax, eax
loc_15_1100:
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        je     SHORT loc_15_1114
        db  00Fh, 0BAh, 035h, 0C4h, 006h, 000h, 000h, 00Dh
        jae    SHORT loc_15_1130
        jmp    SHORT loc_15_1126
loc_15_1114:
        db  03Bh, 01Dh, 0B4h, 007h, 000h, 000h
        db  00Fh, 0BAh, 035h, 0C4h, 006h, 000h, 000h, 00Dh
        jne    SHORT loc_15_1126
        jae    SHORT loc_15_1135
loc_15_1126:
        db  0C7h, 005h, 0B0h, 007h, 000h, 000h, 000h, 000h, 000h, 000h
loc_15_1130:
        db  0A3h, 0ACh, 007h, 000h, 000h
loc_15_1135:
        pop    eax
loc_15_1136:
        call   sub_15_119E
        db  080h, 025h, 0C5h, 006h, 000h, 000h, 0BFh
        ret
sub_15_1143:
        push   eax
        mov    eax, ebx
        db  087h, 005h, 0B0h, 007h, 000h, 000h
        db  0F6h, 005h, 0C5h, 006h, 000h, 000h, 010h
        je     SHORT loc_15_117A
        or     eax, eax
        je     SHORT loc_15_1191
        db  00Fh, 0BAh, 02Dh, 0C4h, 006h, 000h, 000h, 00Dh
        jb     SHORT loc_15_1191
        push   ebx
        push   edi
        push   esi
        mov    ebx, eax
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        call   sub_15_1AA5
        pop    esi
        pop    edi
        pop    ebx
        jmp    SHORT loc_15_1191
loc_15_117A:
        or     ebx, ebx
        je     SHORT loc_15_1191
        cmp    ebx, eax
        je     SHORT loc_15_1191
        push   ebx
        mov    ebx, edi
        db  02Bh, 01Dh, 0CCh, 006h, 000h, 000h
        call   sub_15_19BD
        pop    ebx
loc_15_1191:
        pop    eax
        ret
sub_15_1193:
        db  0C7h, 005h, 0ACh, 007h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
sub_15_119E:
        pushad
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        je     NEAR PTR loc_15_12D3
        db  083h, 03Dh, 0ACh, 007h, 000h, 000h, 000h
        je     SHORT loc_15_11C1
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        je     SHORT loc_15_11C1
        call   sub_15_1969
loc_15_11C1:
        push   ebx
        push   edi
        db  0FFh, 035h, 0ACh, 007h, 000h, 000h
        db  089h, 01Dh, 0ACh, 007h, 000h, 000h
        db  03Bh, 01Dh, 0B8h, 007h, 000h, 000h
        je     SHORT loc_15_1240
        db  0F6h, 005h, 0C5h, 006h, 000h, 000h, 010h
        jne    SHORT loc_15_11E5
        call   sub_15_4BBC
loc_15_11E5:
        db  03Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        je     SHORT loc_15_1240
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 34h], 0
        je     SHORT loc_15_11FC
        call   dword ptr [edi + 34h]
loc_15_11FC:
        pop    edi
        db  081h, 00Dh, 09Ch, 010h, 000h, 000h, 000h, 000h, 000h, 080h
        db  080h, 03Dh, 0A1h, 007h, 000h, 000h, 000h
        jne    SHORT loc_15_1240
        db  080h, 03Dh, 0B0h, 00Fh, 000h, 000h, 000h
        je     SHORT loc_15_1224
        db  0BEh, 060h, 006h, 000h, 000h
        db  0CDh, 020h, 001h, 000h, 01Fh, 001h
loc_15_1224:
        call   sub_15_3023
        db  0C6h, 005h, 0A1h, 007h, 000h, 000h, 0FFh
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 2ch], 0
        je     SHORT loc_15_123F
        call   dword ptr [edi + 2ch]
loc_15_123F:
        pop    edi
loc_15_1240:
        pop    ebx
        or     ebx, ebx
        je     SHORT loc_15_1284
        db  03Bh, 01Dh, 0B8h, 007h, 000h, 000h
        je     SHORT loc_15_1284
        db  0F6h, 005h, 0C5h, 006h, 000h, 000h, 010h
        jne    SHORT loc_15_1284
        xor    eax, eax
        db  087h, 005h, 0ACh, 007h, 000h, 000h
        push   eax
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  03Bh, 01Dh, 0B0h, 007h, 000h, 000h
        jne    SHORT loc_15_1274
        call   sub_15_1AA5
loc_15_1274:
        call   sub_15_3A50
        db  08Fh, 005h, 0ACh, 007h, 000h, 000h
        call   sub_15_4BBC
loc_15_1284:
        pop    edi
        pop    ebx
        mov    esi, edi
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        je     SHORT loc_15_12A2
        db  08Bh, 01Dh, 0A8h, 007h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_15_12A2
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
loc_15_12A2:
        db  03Bh, 01Dh, 0B8h, 007h, 000h, 000h
        je     SHORT loc_15_12C8
        xor    eax, eax
        db  087h, 005h, 0ACh, 007h, 000h, 000h
        push   eax
        push   ebx
        mov    ebx, esi
        db  02Bh, 01Dh, 0CCh, 006h, 000h, 000h
        call   sub_15_1143
        pop    ebx
        db  08Fh, 005h, 0ACh, 007h, 000h, 000h
loc_15_12C8:
        call   sub_15_147C
        stc
        call   sub_15_1857
loc_15_12D3:
        popad
        ret
sub_15_12D5:
        pushad
        db  089h, 01Dh, 09Ah, 00Eh, 000h, 000h
        db  08Bh, 00Dh, 0A8h, 007h, 000h, 000h
        cmp    ebx, ecx
        je     NEAR PTR loc_15_1469
        push   edi
        push   ebx
        db  083h, 03Dh, 000h, 00Fh, 000h, 000h, 000h
        je     NEAR PTR loc_15_138F
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  089h, 01Dh, 09Eh, 00Eh, 000h, 000h
loc_15_1305:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        test   byte ptr [ebx], 2
        je     SHORT loc_15_137D
        test   byte ptr [edi], 80h
        jne    SHORT loc_15_137D
        test   byte ptr [edi + 2], 2
        jne    SHORT loc_15_137D
        call   sub_15_3A9D
        jae    SHORT loc_15_137D
        mov    eax, ebx
        db  0CDh, 020h, 0F1h, 000h, 001h, 000h
        xchg   ebx, eax
        cmp    eax, ebx
        je     SHORT loc_15_137D
        push   ebx
        db  08Bh, 01Dh, 09Ah, 00Eh, 000h, 000h
        db  03Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        jne    SHORT loc_15_1372
        db  08Bh, 01Dh, 0A8h, 007h, 000h, 000h
        db  03Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        je     SHORT loc_15_1372
        db  03Bh, 01Dh, 0B8h, 007h, 000h, 000h
        je     SHORT loc_15_1372
        or     ebx, ebx
        je     SHORT loc_15_1372
        db  0CDh, 020h, 000h, 000h, 00Ch, 000h
        cmp    eax, 400h
        jb     SHORT loc_15_1372
        mov    eax, 3
        db  0CDh, 020h, 005h, 000h, 00Ch, 000h
loc_15_1372:
        pop    ebx
        db  0CDh, 020h, 02Bh, 000h, 001h, 000h
        or     byte ptr [edi + 2], 20h
loc_15_137D:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        db  03Bh, 01Dh, 09Eh, 00Eh, 000h, 000h
        jne    NEAR PTR loc_15_1305
loc_15_138F:
        call   sub_15_083E
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0ch], 0
        je     SHORT loc_15_13A3
        call   dword ptr [edi + 0ch]
loc_15_13A3:
        pop    edi
        pop    ebx
        pop    edi
        db  0A1h, 0A8h, 007h, 000h, 000h
        db  03Bh, 005h, 0ACh, 007h, 000h, 000h
        je     SHORT loc_15_13C6
        push   ebx
        push   edi
        mov    ebx, eax
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        call   sub_15_119E
        pop    edi
        pop    ebx
loc_15_13C6:
        call   sub_15_1969
        mov    esi, edi
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        je     SHORT loc_15_13EA
        db  083h, 03Dh, 0ACh, 007h, 000h, 000h, 000h
        je     SHORT loc_15_13EA
        db  08Bh, 035h, 0ACh, 007h, 000h, 000h
        db  003h, 035h, 0CCh, 006h, 000h, 000h
loc_15_13EA:
        call   sub_15_147C
        db  083h, 03Dh, 000h, 00Fh, 000h, 000h, 000h
        je     SHORT loc_15_142C
        push   edi
        push   ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        push   ebx
loc_15_1401:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        test   byte ptr [edi + 2], 20h
        je     SHORT loc_15_141E
        and    byte ptr [edi + 2], 0dfh
        test   byte ptr [edi], 80h
        jne    SHORT loc_15_141E
        db  0CDh, 020h, 02Ch, 000h, 001h, 000h
loc_15_141E:
        db  0CDh, 020h, 03Bh, 000h, 001h, 000h
        cmp    ebx, dword ptr [esp]
        jne    SHORT loc_15_1401
        pop    ebx
        pop    ebx
        pop    edi
loc_15_142C:
        db  03Bh, 01Dh, 0B8h, 007h, 000h, 000h
        db  0FFh, 035h, 0A8h, 007h, 000h, 000h
        db  089h, 01Dh, 0A8h, 007h, 000h, 000h
        je     SHORT loc_15_1453
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        clc
        call   sub_15_1857
        call   sub_15_4BBC
loc_15_1453:
        pop    ebx
        db  03Bh, 01Dh, 0B8h, 007h, 000h, 000h
        je     SHORT loc_15_1469
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        call   sub_15_4BBC
loc_15_1469:
        popad
        ret
sub_15_146B:
        push   eax
        mov    edx, 3cch
        in     al, dx
        mov    dl, 0d4h
        test   al, 1
        pop    eax
        jne    SHORT loc_15_147B
        mov    dl, 0b4h
loc_15_147B:
        ret
sub_15_147C:
        pushad
        cld
        db  0F6h, 005h, 0A0h, 007h, 000h, 000h, 007h
        jne    NEAR PTR loc_15_1637
        call   sub_15_146B
        push   eax
        push   ecx
        db  0A1h, 0A8h, 007h, 000h, 000h
        db  08Bh, 00Dh, 0ACh, 007h, 000h, 000h
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 3ch], 0
        je     SHORT loc_15_14AC
        call   dword ptr [edi + 3ch]
loc_15_14AC:
        pop    edi
        pop    ecx
        pop    eax
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        je     NEAR PTR loc_15_155D
        call   sub_15_037C
        jb     NEAR PTR loc_15_1637
        call   sub_15_146B
        push   edx
        mov    edx, 3c4h
        mov    eax, 100h
        out    dx, ax
        mov    al, 1
        mov    ah, byte ptr [edi + 67h]
        out    dx, ax
        mov    al, 3
        mov    ah, byte ptr [edi + 69h]
        out    dx, ax
        mov    dl, 0ceh
        mov    al, 6
        mov    ah, byte ptr [edi + 7ch]
        out    dx, ax
        mov    dl, 0c4h
        xor    al, al
        mov    ah, byte ptr [edi + 66h]
        out    dx, ax
        pop    edx
        push   esi
        call   sub_15_1639
        push   edx
        add    edx, 6
        in     al, dx
        mov    dl, 0c0h
        mov    al, 10h
        out    dx, al
        mov    al, byte ptr [edi + 60h]
        out    dx, al
        mov    al, 12h
        out    dx, al
        mov    al, byte ptr [edi + 62h]
        out    dx, al
        mov    al, 13h
        out    dx, al
        mov    al, byte ptr [edi + 63h]
        out    dx, al
        mov    al, 14h
        out    dx, al
        mov    al, byte ptr [edi + 64h]
        out    dx, al
        mov    dl, 0c4h
        mov    al, 1
        mov    ah, byte ptr [edi + 67h]
        or     ah, 20h
        out    dx, ax
        lea    esi, [edi + 50h]
        mov    cl, 15h
        xor    ch, ch
        mov    dl, 0c0h
loc_15_1537:
        mov    al, ch
        out    dx, al
        mov    al, byte ptr [esi]
        out    dx, al
        inc    esi
        inc    ch
        dec    cl
        jne    SHORT loc_15_1537
        call   sub_15_1657
        mov    dl, 0c0h
        mov    al, 11h
        out    dx, al
        mov    al, byte ptr [edi + 61h]
        out    dx, al
        mov    dl, 0c4h
        mov    al, 1
        mov    ah, byte ptr [edi + 67h]
        out    dx, ax
        pop    edx
        pop    esi
loc_15_155D:
        push   esi
        push   edi
        push   edx
        lea    edi, [edi + 0ch]
        lea    esi, [esi + 0ch]
        mov    edx, 3c4h
        mov    al, 2
        mov    ah, byte ptr [esi + 5ch]
        out    dx, ax
        mov    al, byte ptr [esi + 5eh]
        mov    ah, byte ptr [edi + 5eh]
        and    ax, 10eh
        or     ah, al
        mov    al, 4
        out    dx, ax
        mov    dl, 0ceh
        mov    al, 0
        mov    ah, byte ptr [esi + 6ah]
        out    dx, ax
        mov    al, 1
        mov    ah, byte ptr [esi + 6bh]
        out    dx, ax
        mov    al, 2
        mov    ah, byte ptr [esi + 6ch]
        out    dx, ax
        mov    al, 3
        mov    ah, byte ptr [esi + 6dh]
        out    dx, ax
        mov    al, 4
        mov    ah, byte ptr [esi + 6eh]
        out    dx, ax
        mov    al, byte ptr [esi + 6fh]
        mov    ah, byte ptr [edi + 6fh]
        and    ax, 609fh
        or     ah, al
        mov    al, 5
        out    dx, ax
        mov    al, byte ptr [esi + 70h]
        mov    ah, byte ptr [edi + 70h]
        and    ax, 1feh
        or     ah, al
        mov    al, 6
        out    dx, ax
        mov    al, 7
        mov    ah, byte ptr [esi + 71h]
        out    dx, ax
        mov    al, 8
        mov    ah, byte ptr [esi + 72h]
        out    dx, ax
        mov    dl, 0c2h
        mov    al, byte ptr [edi + 8ah]
        mov    ah, byte ptr [esi + 8ah]
        and    ax, 22ddh
        or     ah, al
        mov    dl, 0cch
        in     al, dx
        cmp    ah, al
        je     SHORT loc_15_15F5
        mov    al, ah
        mov    dl, 0c2h
        out    dx, al
loc_15_15F5:
        pop    edx
        pop    edi
        pop    esi
        test   dword ptr [esi], 4000h
        je     SHORT loc_15_1611
        push   edx
        mov    edx, 3ceh
        mov    al, 6
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0f3h
        or     al, 4
        out    dx, al
        pop    edx
loc_15_1611:
        db  081h, 0FEh, 0F8h, 00Dh, 000h, 000h
        je     SHORT loc_15_161F
        db  02Bh, 035h, 0CCh, 006h, 000h, 000h
loc_15_161F:
        mov    ecx, edi
        db  02Bh, 00Dh, 0CCh, 006h, 000h, 000h
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 24h], 0
        je     SHORT loc_15_1636
        call   dword ptr [edi + 24h]
loc_15_1636:
        pop    edi
loc_15_1637:
        popad
        ret
sub_15_1639:
        mov    al, 11h
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 7fh
        out    dx, al
        dec    edx
        lea    esi, [edi + 0ch]
        mov    ecx, 19h
        xor    al, al
loc_15_164C:
        mov    ah, byte ptr [esi]
        out    dx, ax
        inc    esi
        inc    al
        dec    ecx
        jne    SHORT loc_15_164C
        ret
sub_15_1657:
        lea    esi, [edi + 0a0h]
        mov    ecx, 100h
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 040h
        je     SHORT loc_15_1670
        mov    ecx, 10h
loc_15_1670:
        mov    edx, 3c8h
loc_15_1675:
        mov    eax, dword ptr [esi]
        out    dx, al
        inc    dl
        shr    eax, 8
        out    dx, al
        shr    eax, 8
        out    dx, al
        shr    eax, 8
        out    dx, al
        add    esi, 4
        dec    dl
        dec    ecx
        jne    SHORT loc_15_1675
        ret
sub_15_168F:
        movzx  eax, word ptr [edi + 18h]
        xchg   al, ah
        test   byte ptr [edi + 20h], 40h
        je     SHORT loc_15_16A4
        lea    eax, [eax*4]
        jmp    SHORT loc_15_16AC
loc_15_16A4:
        test   byte ptr [edi + 23h], 40h
        jne    SHORT loc_15_16AC
        add    eax, eax
loc_15_16AC:
        ret
sub_15_16AD:
        db  0E8h, 0DAh, 0EBh, 0F0h, 0FFh
        jae    SHORT loc_15_16BC
        mov    byte ptr [edi + 5b3h], 56h
        ret
loc_15_16BC:
        movzx  eax, byte ptr [edi + 0dh]
        inc    eax
        jmp    SHORT loc_15_16E8
sub_15_16C3:
        mov    byte ptr [edi + 5b3h], 0ffh
        db  0E8h, 0BDh, 0EBh, 0F0h, 0FFh
        jae    SHORT loc_15_16D8
        mov    byte ptr [edi + 5b3h], 56h
loc_15_16D8:
        movzx  eax, byte ptr [edi + 1fh]
        shl    eax, 1
        mov    cl, byte ptr [edi + 23h]
        test   cl, 8
        je     SHORT loc_15_16E8
        shl    eax, 1
loc_15_16E8:
        push   edx
        push   eax
        movzx  eax, byte ptr [edi + 1eh]
        mov    cl, byte ptr [edi + 13h]
        shr    cl, 2
        adc    ah, 0
        shr    cl, 3
        and    cl, 2
        or     ah, cl
        inc    eax
        mov    cl, byte ptr [edi + 23h]
        test   cl, 4
        je     SHORT loc_15_170A
        shl    eax, 1
loc_15_170A:
        mov    edx, eax
        mov    cl, byte ptr [edi + 15h]
        test   cl, 80h
        je     SHORT loc_15_1716
        shr    edx, 1
loc_15_1716:
        pop    eax
        test   byte ptr [edi + 7ch], 1
        je     SHORT loc_15_1799
loc_15_171D:
        mov    cl, byte ptr [edi + 23h]
        test   cl, 1
        jne    SHORT loc_15_1727
        shl    edx, 1
loc_15_1727:
        movzx  ecx, byte ptr [edi + 15h]
        and    cl, 1fh
        je     SHORT loc_15_173B
        inc    ecx
        push   eax
        mov    eax, edx
        xor    edx, edx
        div    ecx
        mov    edx, eax
        pop    eax
loc_15_173B:
        mov    cl, byte ptr [edi + 60h]
        test   cl, 40h
        je     SHORT loc_15_1785
        shr    eax, 1
        mov    byte ptr [edi + 5b3h], 13h
        test   byte ptr [edi + 6ah], 8
        jne    SHORT loc_15_1785
        mov    byte ptr [edi + 5b3h], 58h
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        je     SHORT loc_15_1785
        or     ebx, ebx
        je     SHORT loc_15_1785
        test   byte ptr [edi], 80h
        jne    SHORT loc_15_1785
        pushad
        db  0CDh, 020h, 02Bh, 000h, 001h, 000h
        or     byte ptr [edi], 80h
        or     byte ptr [edi + 9], 4
        db  080h, 00Dh, 0C4h, 006h, 000h, 000h, 010h
        db  0E8h, 097h, 0EDh, 0F0h, 0FFh
        popad
loc_15_1785:
        mov    cl, byte ptr [edi + 67h]
        test   cl, 1
        mov    ecx, eax
        je     SHORT loc_15_1791
        xor    ecx, ecx
loc_15_1791:
        lea    ecx, [ecx + eax*8]
        mov    eax, edx
        stc
        jmp    SHORT loc_15_17B2
loc_15_1799:
        movzx  ecx, byte ptr [edi + 15h]
        and    cl, 1fh
        cmp    cl, 2
        jbe    NEAR PTR loc_15_171D
        inc    ecx
        xchg   edx, eax
        push   edx
        xor    edx, edx
        div    ecx
        pop    ecx
        clc
loc_15_17B2:
        pop    edx
        ret
sub_15_17B4:
        push   ebx
        push   edx
        push   edi
        mov    dx, 3dah
        in     al, dx
        push   edx
        jmp    SHORT loc_15_17BF
loc_15_17BF:
        mov    dl, 0c0h
        db  08Bh, 01Dh, 0A8h, 007h, 000h, 000h
        in     al, dx
        or     al, 20h
        test   ebx, ebx
        je     SHORT loc_15_17F7
        db  03Bh, 01Dh, 0B8h, 007h, 000h, 000h
        je     SHORT loc_15_17F7
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  080h, 025h, 0C4h, 006h, 000h, 000h, 0FDh
        push   edx
        call   sub_15_4BBC
        pop    edx
        and    byte ptr [edi + 1], 7fh
        mov    al, byte ptr [edi + 4ch]
        test   byte ptr [edi + 4ch], 80h
loc_15_17F7:
        out    dx, al
        jmp    SHORT loc_15_17FA
loc_15_17FA:
        pop    edx
        je     SHORT loc_15_17FE
        in     al, dx
loc_15_17FE:
        pop    edi
        pop    edx
        pop    ebx
        ret
sub_15_1802:
        push   ebx
        push   edx
        push   edi
        mov    edx, 3dah
        in     al, dx
        mov    dl, 0c0h
        in     al, dx
        mov    ah, al
        xor    al, al
        out    dx, al
        db  08Bh, 01Dh, 0A8h, 007h, 000h, 000h
        test   ebx, ebx
        je     SHORT loc_15_1853
        db  03Bh, 01Dh, 0B8h, 007h, 000h, 000h
        je     SHORT loc_15_1853
        db  00Fh, 0BAh, 02Dh, 0C4h, 006h, 000h, 000h, 001h
        jb     SHORT loc_15_1853
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        bts    dword ptr [edi], 0fh
        jb     SHORT loc_15_1847
        and    byte ptr [edi + 4ch], 80h
        and    ah, 7fh
        or     byte ptr [edi + 4ch], ah
loc_15_1847:
        call   sub_15_1969
        push   edx
        call   sub_15_4BBC
        pop    edx
loc_15_1853:
        pop    edi
        pop    edx
        pop    ebx
        ret
sub_15_1857:
        push   edx
        mov    dx, 3ceh
        mov    al, byte ptr [esi + 4fh]
        out    dx, al
        jmp    SHORT loc_15_1862
loc_15_1862:
        mov    dl, 0c4h
        mov    al, byte ptr [esi + 4dh]
        out    dx, al
        jmp    SHORT loc_15_186A
loc_15_186A:
        jb     SHORT loc_15_18A7
        call   sub_15_146B
        mov    al, byte ptr [edi + 4eh]
        out    dx, al
        jmp    SHORT loc_15_1877
loc_15_1877:
        add    edx, 6
        in     al, dx
        jmp    SHORT loc_15_187D
loc_15_187D:
        mov    al, byte ptr [edi + 4ch]
        and    al, 0dfh
        push   edx
        mov    dl, 0c0h
        test   byte ptr [edi + 4ch], 20h
        je     SHORT loc_15_1896
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 002h
        jne    SHORT loc_15_1896
        or     al, 20h
loc_15_1896:
        out    dx, al
        jmp    SHORT loc_15_1899
loc_15_1899:
        pop    edx
        test   byte ptr [edi + 4ch], 80h
        je     SHORT loc_15_18A3
        in     al, dx
        jmp    SHORT loc_15_18A3
loc_15_18A3:
        and    byte ptr [edi + 1], 7fh
loc_15_18A7:
        pop    edx
        ret
sub_15_18A9:
        db  0F6h, 005h, 0C5h, 006h, 000h, 000h, 010h
        jne    SHORT loc_15_18B9
        pushad
        call   sub_15_4BBC
        popad
loc_15_18B9:
        ret
sub_15_18BA:
        call   sub_15_1969
        push   ebx
        push   edi
        xor    ebx, ebx
        db  087h, 01Dh, 0ACh, 007h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_15_1905
        db  03Bh, 01Dh, 0B8h, 007h, 000h, 000h
        je     SHORT loc_15_1905
        db  0F6h, 005h, 0C5h, 006h, 000h, 000h, 010h
        jne    SHORT loc_15_1905
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  03Bh, 01Dh, 0B0h, 007h, 000h, 000h
        jne    SHORT loc_15_18F3
        call   sub_15_1AA5
loc_15_18F3:
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        je     SHORT loc_15_1900
        call   sub_15_3A50
loc_15_1900:
        call   sub_15_4BBC
loc_15_1905:
        db  0F6h, 005h, 0C5h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_1926
        db  08Bh, 01Dh, 0A8h, 007h, 000h, 000h
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  0BEh, 0F8h, 00Dh, 000h, 000h
        call   sub_15_147C
loc_15_1926:
        pop    edi
        pop    ebx
        db  00Fh, 0BAh, 025h, 0C4h, 006h, 000h, 000h, 008h
        cmc
        ret
sub_15_1932:
        push   ebx
        push   edi
        xor    ebx, ebx
        db  089h, 01Dh, 0ACh, 007h, 000h, 000h
        db  0F6h, 005h, 0C5h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_195D
        db  08Bh, 01Dh, 0A8h, 007h, 000h, 000h
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  0BEh, 0F8h, 00Dh, 000h, 000h
        call   sub_15_147C
loc_15_195D:
        pop    edi
        pop    ebx
        db  00Fh, 0BAh, 025h, 0C4h, 006h, 000h, 000h, 008h
        cmc
        ret
sub_15_1969:
        push   ebx
        push   edi
        db  08Bh, 01Dh, 0A8h, 007h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_15_19BA
        db  03Bh, 01Dh, 0B8h, 007h, 000h, 000h
        je     SHORT loc_15_19BA
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        jne    SHORT loc_15_19BA
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 002h
        jne    SHORT loc_15_199C
        test   byte ptr [edi + 2], 1
        jne    SHORT loc_15_19BA
loc_15_199C:
        db  0F6h, 005h, 0C5h, 006h, 000h, 000h, 040h
        jne    SHORT loc_15_19BA
        db  0F6h, 005h, 0C5h, 006h, 000h, 000h, 010h
        je     SHORT loc_15_19B5
        db  080h, 00Dh, 0C5h, 006h, 000h, 000h, 040h
loc_15_19B5:
        call   sub_15_1EAC
loc_15_19BA:
        pop    edi
        pop    ebx
        ret
sub_15_19BD:
        test   byte ptr [esi], 2
        jne    NEAR PTR loc_15_1AA4
        test   byte ptr [esi + 7ch], 8
        jne    NEAR PTR loc_15_1AA4
        db  083h, 03Dh, 000h, 00Fh, 000h, 000h, 000h
        je     SHORT loc_15_1A16
        db  0F6h, 005h, 0A0h, 007h, 000h, 000h, 007h
        jne    NEAR PTR loc_15_1AA4
        stc
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 64h], 0
        je     SHORT loc_15_19FA
        popfd
        call   dword ptr [edi + 64h]
        jmp    SHORT loc_15_19FB
loc_15_19FA:
        popfd
loc_15_19FB:
        pop    edi
        jae    NEAR PTR loc_15_1AA4
        mov    eax, esi
        db  02Bh, 005h, 0CCh, 006h, 000h, 000h
        db  03Bh, 005h, 0F0h, 00Eh, 000h, 000h
        je     NEAR PTR loc_15_1AA4
loc_15_1A16:
        push   ebx
        mov    ebx, esi
        call   sub_15_1932
        call   sub_15_28C8
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 4
        out    dx, al
        inc    edx
        in     al, dx
        ror    eax, 8
        xor    al, al
        out    dx, al
        mov    cl, byte ptr [esi]
        inc    al
        out    dx, al
        mov    ch, byte ptr [esi]
        ror    ecx, 10h
        inc    al
        out    dx, al
        mov    cl, byte ptr [esi]
        inc    al
        out    dx, al
        mov    ch, byte ptr [esi]
        rol    eax, 8
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        mov    dl, 0c4h
        mov    al, 2
        out    dx, al
        inc    edx
        mov    al, 1
        out    dx, al
        mov    al, byte ptr [ebx + 4abh]
        mov    byte ptr [esi], al
        mov    al, 2
        out    dx, al
        mov    al, byte ptr [ebx + 4ach]
        mov    byte ptr [esi], al
        mov    al, 4
        out    dx, al
        mov    al, byte ptr [ebx + 4adh]
        mov    byte ptr [esi], al
        mov    al, 8
        out    dx, al
        mov    al, byte ptr [ebx + 4aeh]
        mov    byte ptr [esi], al
        mov    al, byte ptr [esi]
        mov    byte ptr [esi], ch
        mov    al, 4
        out    dx, al
        mov    byte ptr [esi], cl
        ror    ecx, 10h
        mov    al, 2
        out    dx, al
        mov    byte ptr [esi], ch
        mov    al, 1
        out    dx, al
        mov    byte ptr [esi], cl
        call   sub_15_28F6
        mov    esi, ebx
        pop    ebx
loc_15_1AA4:
        ret
sub_15_1AA5:
        test   byte ptr [edi], 2
        jne    SHORT loc_15_1AB0
        test   byte ptr [edi + 7ch], 8
        je     SHORT loc_15_1AB1
loc_15_1AB0:
        ret
loc_15_1AB1:
        db  083h, 03Dh, 000h, 00Fh, 000h, 000h, 000h
        je     SHORT loc_15_1AEB
        db  0F6h, 005h, 0A0h, 007h, 000h, 000h, 007h
        jne    SHORT loc_15_1AB0
        stc
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 60h], 0
        je     SHORT loc_15_1AD7
        popfd
        call   dword ptr [edi + 60h]
        jmp    SHORT loc_15_1AD8
loc_15_1AD7:
        popfd
loc_15_1AD8:
        pop    edi
        jae    SHORT loc_15_1AB0
        db  03Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        je     SHORT loc_15_1AB0
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        je     SHORT loc_15_1AF0
loc_15_1AEB:
        call   sub_15_1932
loc_15_1AF0:
        call   sub_15_28C8
        mov    edx, 3ceh
        in     al, dx
        ror    eax, 8
        mov    al, 5
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        ror    eax, 8
        and    al, 70h
        or     al, 1
        out    dx, al
        dec    dl
        mov    byte ptr [esi], al
        mov    al, 4
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        xor    al, al
        out    dx, al
        mov    al, byte ptr [esi]
        mov    byte ptr [edi + 4abh], al
        mov    al, 1
        out    dx, al
        mov    al, byte ptr [esi]
        mov    byte ptr [edi + 4ach], al
        mov    al, 2
        out    dx, al
        mov    al, byte ptr [esi]
        mov    byte ptr [edi + 4adh], al
        mov    al, 3
        out    dx, al
        mov    al, byte ptr [esi]
        mov    byte ptr [edi + 4aeh], al
        mov    al, ah
        out    dx, al
        dec    dl
        rol    eax, 8
        mov    ah, al
        mov    al, 5
        out    dx, ax
        rol    eax, 8
        out    dx, al
        call   sub_15_28F6
        ret
sub_15_1B5E:
        pushad
        call   sub_15_16AD
        jb     SHORT loc_15_1B94
        mov    byte ptr [edi + 5b3h], 0
        cmp    cl, 28h
        je     SHORT loc_15_1BC0
        mov    byte ptr [edi + 5b3h], 3
        cmp    cl, 50h
        jne    NEAR PTR loc_15_1C32
        test   byte ptr [edi + 96h], 1
        jne    SHORT loc_15_1BC0
        mov    byte ptr [edi + 5b3h], 7
        jmp    SHORT loc_15_1BC0
loc_15_1B94:
        cmp    byte ptr [edi + 5b3h], 0ffh
        jne    SHORT loc_15_1BC0
        cmp    eax, 0c8h
        jne    SHORT loc_15_1BE7
        cmp    ecx, 140h
        jne    SHORT loc_15_1BC9
        mov    byte ptr [edi + 5b3h], 4
        test   byte ptr [edi + 6ah], 4
        je     SHORT loc_15_1BC0
        mov    byte ptr [edi + 5b3h], 0dh
loc_15_1BC0:
        popad
        movzx  eax, byte ptr [edi + 5b3h]
        ret
loc_15_1BC9:
        cmp    ecx, 280h
        jne    SHORT loc_15_1C32
        mov    byte ptr [edi + 5b3h], 6
        test   byte ptr [edi + 23h], 1
        je     SHORT loc_15_1BC0
        mov    byte ptr [edi + 5b3h], 0eh
        jmp    SHORT loc_15_1BC0
loc_15_1BE7:
        cmp    ecx, 280h
        jne    SHORT loc_15_1C32
        test   byte ptr [edi + 6ah], 8
        jne    SHORT loc_15_1C32
        cmp    eax, 15eh
        je     SHORT loc_15_1C19
        cmp    eax, 1e0h
        jne    SHORT loc_15_1C32
        mov    byte ptr [edi + 5b3h], 12h
        test   byte ptr [edi + 23h], 20h
        jne    SHORT loc_15_1BC0
        mov    byte ptr [edi + 5b3h], 11h
        jmp    SHORT loc_15_1BC0
loc_15_1C19:
        mov    byte ptr [edi + 5b3h], 10h
        test   byte ptr [edi + 96h], 1
        jne    SHORT loc_15_1BC0
        mov    byte ptr [edi + 5b3h], 0fh
        jmp    SHORT loc_15_1BC0
loc_15_1C32:
        mov    byte ptr [edi + 5b3h], 0ffh
        jmp    SHORT loc_15_1BC0
sub_15_1C3B:
        push   ecx
        mov    eax, 10a0h
        or     ebx, ebx
        je     SHORT loc_15_1C55
        movzx  ecx, byte ptr [edi + 7ch]
        shr    ecx, 1
        and    cl, 6
        db  066h, 08Bh, 081h, 0A4h, 00Eh, 000h, 000h
loc_15_1C55:
        pop    ecx
        ret
sub_15_1C57:
        test   byte ptr [edi], 1
        je     SHORT loc_15_1C75
        test   byte ptr [edi + 3], 80h
        je     SHORT loc_15_1C75
        test   byte ptr [edi], 4
        jne    SHORT loc_15_1C75
        call   sub_15_1D6D
        jne    SHORT loc_15_1C77
        call   sub_15_1CD3
        jb     SHORT loc_15_1C77
loc_15_1C75:
        clc
        ret
loc_15_1C77:
        stc
        ret
sub_15_1C79:
        and    byte ptr [edi + 5f0h], 0bfh
        test   byte ptr [edi + 3], 80h
        je     SHORT loc_15_1CA4
        test   byte ptr [edi], 4
        jne    SHORT loc_15_1CA4
        call   sub_15_1CD3
        jne    SHORT loc_15_1C9B
        pushfd
        or     byte ptr [edi + 5f0h], 40h
        popfd
loc_15_1C9B:
        jb     SHORT loc_15_1CAF
        call   sub_15_1D6D
        jne    SHORT loc_15_1CA8
loc_15_1CA4:
        xor    eax, eax
        jmp    SHORT loc_15_1CBE
loc_15_1CA8:
        mov    eax, 20h
        jmp    SHORT loc_15_1CBE
loc_15_1CAF:
        mov    byte ptr [edi + 9dh], al
        cmp    al, 0bh
        je     SHORT loc_15_1CA4
        mov    eax, 28h
loc_15_1CBE:
        or     dword ptr [edi + 5f0h], eax
        call   sub_15_1E91
        mov    eax, dword ptr [edi + 5f0h]
        mov    word ptr [esi], ax
        ret
sub_15_1CD3:
        call   sub_15_1B5E
        push   eax
        cmp    al, byte ptr [edi + 540h]
        jne    SHORT loc_15_1D57
        test   byte ptr [edi + 4ch], 20h
        jne    SHORT loc_15_1CF2
        test   byte ptr [edi + 4efh], 20h
        jne    SHORT loc_15_1D57
        jmp    SHORT loc_15_1CFB
loc_15_1CF2:
        test   byte ptr [edi + 4efh], 20h
        je     SHORT loc_15_1D57
loc_15_1CFB:
        mov    al, byte ptr [edi + 15h]
        cmp    al, byte ptr [edi + 4b8h]
        jne    SHORT loc_15_1D57
        mov    al, byte ptr [edi + 1eh]
        cmp    al, byte ptr [edi + 4c1h]
        jne    SHORT loc_15_1D57
        mov    ax, word ptr [edi + 18h]
        cmp    ax, word ptr [edi + 4bbh]
        jne    SHORT loc_15_1D57
        btr    dword ptr [edi], 1ch
        jb     SHORT loc_15_1D57
        mov    eax, dword ptr [edi + 50h]
        cmp    eax, dword ptr [edi + 4f3h]
        jne    SHORT loc_15_1D57
        mov    eax, dword ptr [edi + 54h]
        cmp    eax, dword ptr [edi + 4f7h]
        jne    SHORT loc_15_1D57
        mov    eax, dword ptr [edi + 58h]
        cmp    eax, dword ptr [edi + 4fbh]
        jne    SHORT loc_15_1D57
        mov    eax, dword ptr [edi + 5ch]
        cmp    eax, dword ptr [edi + 4ffh]
        jne    SHORT loc_15_1D57
        pop    eax
        test   byte ptr [edi + 4ch], 20h
        clc
        ret
loc_15_1D57:
        pop    eax
        test   byte ptr [edi + 4ch], 20h
        stc
        ret
sub_15_1D5E:
        mov    dword ptr [edi + 5f0h], 0
        and    byte ptr [edi + 1], 0fdh
        ret
sub_15_1D6D:
        mov    ax, word ptr [edi + 1ah]
        cmp    ax, word ptr [edi + 4bdh]
        jne    SHORT loc_15_1D85
        mov    ax, word ptr [edi + 16h]
        cmp    ax, word ptr [edi + 4b9h]
loc_15_1D85:
        ret
loc_15_1D86:
        pushfd
        lea    edx, [edi + 4afh]
        jae    SHORT loc_15_1D92
        lea    edx, [edi + 0ch]
loc_15_1D92:
        call   sub_15_1B5E
        mov    byte ptr [esi], al
        call   sub_15_16AD
        jb     SHORT loc_15_1DAD
        mov    byte ptr [esi + 3], al
        mov    word ptr [esi + 1ch], cx
        or     byte ptr [esi + 2], 8
        jmp    SHORT loc_15_1DB9
loc_15_1DAD:
        and    byte ptr [esi + 2], 0f7h
        mov    word ptr [esi + 1ch], cx
        mov    word ptr [esi + 1eh], ax
loc_15_1DB9:
        mov    al, byte ptr [edx + 55h]
        mov    byte ptr [esi + 1], al
        popfd
        jae    SHORT loc_15_1E1A
        call   sub_15_1E6D
        mov    word ptr [esi + 4], cx
        mov    word ptr [esi + 6], ax
        test   byte ptr [edi + 2], 8
        jne    SHORT loc_15_1E0C
        movzx  eax, byte ptr [edx + 0ah]
        and    al, 1fh
        movzx  ecx, byte ptr [edx + 0bh]
        and    cl, 1fh
        mov    ch, byte ptr [edx + 9]
        and    ch, 0fh
        cmp    al, ch
        ja     SHORT loc_15_1E0C
        cmp    al, cl
        jne    SHORT loc_15_1DF2
        inc    cl
loc_15_1DF2:
        or     cl, cl
        jne    SHORT loc_15_1DFA
        mov    cl, ch
        inc    cl
loc_15_1DFA:
        cmp    al, cl
        jb     SHORT loc_15_1E00
        xchg   cl, al
loc_15_1E00:
        cmp    al, ch
        jbe    SHORT loc_15_1E10
        mov    cl, ch
        inc    cl
        xor    al, al
        jmp    SHORT loc_15_1E10
loc_15_1E0C:
        xor    eax, eax
        mov    ecx, eax
loc_15_1E10:
        mov    word ptr [esi + 8], ax
        mov    ch, ah
        mov    word ptr [esi + 0ah], cx
loc_15_1E1A:
        mov    eax, dword ptr [edx + 44h]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [edx + 48h]
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [edx + 4ch]
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [edx + 50h]
        mov    dword ptr [esi + 18h], eax
        and    byte ptr [esi + 2], 0fbh
        mov    eax, dword ptr [edi + 0a0h]
        or     eax, eax
        je     SHORT loc_15_1E44
        or     byte ptr [esi + 2], 4
loc_15_1E44:
        and    byte ptr [esi + 2], 0feh
        test   byte ptr [edi + 3], 80h
        jne    SHORT loc_15_1E54
        or     byte ptr [esi + 2], 1
        jmp    SHORT loc_15_1E5E
loc_15_1E54:
        test   byte ptr [edi + 9], 1
        je     SHORT loc_15_1E5E
        or     byte ptr [esi + 2], 1
loc_15_1E5E:
        and    byte ptr [esi + 2], 0fdh
        test   byte ptr [edi + 1], 2
        je     SHORT loc_15_1E6C
        or     byte ptr [esi + 2], 2
loc_15_1E6C:
        ret
sub_15_1E6D:
        push   edx
        movzx  ecx, byte ptr [edx + 0dh]
        mov    ch, byte ptr [edx + 0ch]
        movzx  eax, byte ptr [edx + 0fh]
        mov    ah, byte ptr [edx + 0eh]
        sub    eax, ecx
        movzx  ecx, byte ptr [edx + 1]
        inc    cl
        xor    edx, edx
        or     ecx, ecx
        je     SHORT loc_15_1E8F
        div    cx
        mov    ecx, edx
loc_15_1E8F:
        pop    edx
        ret
sub_15_1E91:
        pushfd
        push   esi
        push   edi
        lea    esi, [edi + 0ch]
        lea    edi, [edi + 4afh]
        mov    ecx, 25h
        cld
        rep movsd
        pop    edi
        pop    esi
        popfd
        ret
        db  000h, 000h, 000h
sub_15_1EAC:
        pushad
        db  08Bh, 01Dh, 0ACh, 007h, 000h, 000h
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_1EF8
        db  0F6h, 005h, 0A0h, 007h, 000h, 000h, 003h
        jne    SHORT loc_15_1EF8
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        call   sub_15_146B
        call   sub_15_1FA3
        call   sub_15_1FBC
        call   sub_15_1FED
        call   sub_15_2014
        call   sub_15_1EFA
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 002h
        je     SHORT loc_15_1EF8
        call   sub_15_203B
loc_15_1EF8:
        popad
        ret
sub_15_1EFA:
        push   edx
        add    edx, 6
        mov    si, 3c0h
        in     al, dx
        xchg   esi, edx
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 002h
        jne    SHORT loc_15_1F1E
        test   byte ptr [edi + 1], 80h
        jne    SHORT loc_15_1F1E
        in     al, dx
        and    byte ptr [edi + 4ch], 80h
        and    al, 7fh
        or     byte ptr [edi + 4ch], al
loc_15_1F1E:
        push   edi
        mov    ecx, 15h
        xor    ah, ah
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 002h
        jne    SHORT loc_15_1F41
        test   byte ptr [edi + 4ch], 20h
        je     SHORT loc_15_1F38
        or     ah, 20h
loc_15_1F38:
        add    edi, 10h
        or     ah, 10h
        sub    cl, 10h
loc_15_1F41:
        lea    edi, [edi + 50h]
        cld
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 002h
        jne    SHORT loc_15_1F65
        test   ah, 10h
        jne    SHORT loc_15_1F65
        push   ecx
        mov    ecx, 10000h
        xchg   esi, edx
loc_15_1F5B:
        in     al, dx
        jmp    SHORT loc_15_1F5E
loc_15_1F5E:
        test   al, 8
        loope  loc_15_1F5B
        xchg   esi, edx
        pop    ecx
loc_15_1F65:
        xchg   esi, edx
        in     al, dx
        jmp    SHORT loc_15_1F6A
loc_15_1F6A:
        xchg   esi, edx
        mov    al, ah
        out    dx, al
        jmp    SHORT loc_15_1F71
loc_15_1F71:
        inc    edx
        in     al, dx
        jmp    SHORT loc_15_1F75
loc_15_1F75:
        dec    edx
        stosb
        inc    ah
        loop   loc_15_1F65
        xchg   esi, edx
        in     al, dx
        jmp    SHORT loc_15_1F80
loc_15_1F80:
        xchg   esi, edx
        pop    edi
        mov    al, byte ptr [edi + 4ch]
        and    al, 7fh
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 002h
        je     SHORT loc_15_1F93
        and    al, 0dfh
loc_15_1F93:
        out    dx, al
        jmp    SHORT loc_15_1F96
loc_15_1F96:
        xchg   esi, edx
        test   byte ptr [edi + 4ch], 80h
        je     SHORT loc_15_1FA1
        in     al, dx
        jmp    SHORT loc_15_1FA1
loc_15_1FA1:
        pop    edx
        ret
sub_15_1FA3:
        push   edx
        mov    dl, 0cch
        in     al, dx
        jmp    SHORT loc_15_1FA9
loc_15_1FA9:
        mov    byte ptr [edi + 96h], al
        mov    dl, 0cah
        in     al, dx
        jmp    SHORT loc_15_1FB4
loc_15_1FB4:
        mov    byte ptr [edi + 99h], al
        pop    edx
        ret
sub_15_1FBC:
        in     al, dx
        mov    byte ptr [edi + 4eh], al
        push   edi
        mov    ecx, 19h
        lea    edi, [edi + 0ch]
        xor    ah, ah
        cld
loc_15_1FCC:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        stosb
        inc    ah
        loop   loc_15_1FCC
        pop    edi
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 20h], 0
        je     SHORT loc_15_1FE7
        call   dword ptr [edi + 20h]
loc_15_1FE7:
        pop    edi
        mov    al, byte ptr [edi + 4eh]
        out    dx, al
        ret
sub_15_1FED:
        push   edx
        mov    dx, 3ceh
        in     al, dx
        mov    byte ptr [edi + 4fh], al
        mov    ecx, 9
        push   edi
        lea    edi, [edi + 76h]
        xor    ah, ah
        cld
loc_15_2002:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        stosb
        inc    ah
        loop   loc_15_2002
        pop    edi
        mov    al, byte ptr [edi + 4fh]
        out    dx, al
        pop    edx
        ret
sub_15_2014:
        push   edx
        mov    dx, 3c4h
        in     al, dx
        mov    byte ptr [edi + 4dh], al
        push   edi
        mov    ecx, 5
        lea    edi, [edi + 66h]
        xor    ah, ah
        cld
loc_15_2029:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        stosb
        inc    ah
        loop   loc_15_2029
        pop    edi
        mov    al, byte ptr [edi + 4dh]
        out    dx, al
        pop    edx
        ret
sub_15_203B:
        push   edx
        push   edi
        lea    edi, [edi + 0a0h]
        xor    ecx, ecx
        cld
        mov    dx, 3c7h
loc_15_204A:
        mov    al, cl
        out    dx, al
        add    edx, 2
        in     al, dx
        ror    eax, 8
        in     al, dx
        ror    eax, 8
        in     al, dx
        ror    eax, 8
        mov    al, cl
        mov    dword ptr [edi], eax
        add    edi, 4
        sub    edx, 2
        inc    cl
        jne    SHORT loc_15_204A
        pop    edi
        pop    edx
        ret
        db  000h, 000h, 000h
sub_15_2070:
        test   byte ptr [edi + 5e2h], 1
        je     SHORT loc_15_20D6
        mov    eax, dword ptr [40h]
        db  03Bh, 005h, 0B0h, 00Eh, 000h, 000h
        jne    SHORT loc_15_20B2
        cmp    word ptr [44ah], 50h
        jne    SHORT loc_15_20B2
        mov    esi, 0b8000h
        cmp    byte ptr [449h], 3
        je     SHORT loc_15_20B2
        test   byte ptr [edi + 4], 4
        je     SHORT loc_15_20D6
        cmp    byte ptr [449h], 7
        jne    SHORT loc_15_20B2
        mov    esi, 0b0000h
loc_15_20B2:
        jne    SHORT loc_15_20D5
        add    esi, dword ptr [ebx + 4]
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        je     SHORT loc_15_20D5
        cmp    byte ptr [462h], 0
        jne    SHORT loc_15_20D3
        call   sub_15_4538
        mov    esi, dword ptr [edi + 547h]
loc_15_20D3:
        cmp    esi, esi
loc_15_20D5:
        ret
loc_15_20D6:
        or     al, 1
        ret
loc_15_20D9:
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        je     SHORT loc_15_20F0
        db  080h, 035h, 0C5h, 00Eh, 000h, 000h, 001h
        jne    SHORT loc_15_20F0
        db  0CDh, 020h, 00Dh, 000h, 00Ah, 000h
loc_15_20F0:
        stc
        ret
loc_15_20F2:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        test   byte ptr [edi], 1
        je     SHORT loc_15_2134
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ah, 1
        je     SHORT loc_15_210C
        cmp    ah, 11h
        jne    SHORT loc_15_2134
loc_15_210C:
        cmp    word ptr [edi + 5cch], 0
        je     SHORT loc_15_2134
        dec    word ptr [edi + 5ceh]
        jne    SHORT loc_15_2134
        dec    word ptr [edi + 5cch]
        call   sub_15_4780
        or     byte ptr [edi + 3], 20h
        call   sub_15_46DC
loc_15_2134:
        stc
        ret
sub_15_2136:
        db  0A1h, 0CCh, 006h, 000h, 000h
        add    eax, ebx
        mov    ecx, dword ptr [eax + 5c8h]
        cmp    ecx, -1
        je     SHORT loc_15_2173
        xchg   ch, cl
        ror    ecx, 8
        cmp    dl, ch
        jae    SHORT loc_15_2153
        mov    ch, dl
loc_15_2153:
        cmp    dl, cl
        jbe    SHORT loc_15_2159
        mov    cl, dl
loc_15_2159:
        ror    ecx, 10h
        cmp    dh, cl
        jae    SHORT loc_15_2162
        mov    cl, dh
loc_15_2162:
        cmp    dh, ch
        jbe    SHORT loc_15_2168
        mov    ch, dh
loc_15_2168:
        ror    ecx, 8
        xchg   cl, ch
        mov    dword ptr [eax + 5c8h], ecx
loc_15_2173:
        ret
loc_15_2174:
        db  0F6h, 005h, 0A0h, 007h, 000h, 000h, 007h
        jne    NEAR PTR loc_15_225B
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_21D8
        db  0F6h, 005h, 0C5h, 006h, 000h, 000h, 002h
        jne    SHORT loc_15_21D8
        db  00Fh, 0BAh, 02Dh, 0C4h, 006h, 000h, 000h, 00Ah
        jb     SHORT loc_15_21D8
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    word ptr [ebp + 1ch], 3
        mov    eax, 10h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
loc_15_21D8:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ah, 2
        je     NEAR PTR loc_15_23DD
        cmp    ah, 3
        je     NEAR PTR loc_15_23B6
        cmp    ah, 6
        je     NEAR PTR loc_15_247B
        cmp    ah, 7
        je     NEAR PTR loc_15_247B
        cmp    ah, 9
        je     SHORT loc_15_225D
        cmp    ah, 0eh
        je     NEAR PTR loc_15_22B4
        or     ah, ah
        je     SHORT loc_15_2237
        cmp    ah, 4fh
        je     NEAR PTR loc_15_25A1
        cmp    ah, 11h
        jne    SHORT loc_15_225B
        cmp    al, 3
        je     SHORT loc_15_225B
        cmp    al, 30h
        je     SHORT loc_15_225B
        cmp    al, 20h
        jb     SHORT loc_15_2237
        cmp    al, 24h
        jbe    SHORT loc_15_225B
loc_15_2237:
        test   byte ptr [edi], 2
        jne    SHORT loc_15_225B
        call   sub_15_2769
        mov    eax, 1388h
        db  0BEh, 045h, 028h, 000h, 000h
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
        mov    dword ptr [edi + 5c8h], 0ffffffffh
loc_15_225B:
        stc
        ret
loc_15_225D:
        mov    ecx, eax
        cmp    word ptr [ebp + 18h], 1
        jne    SHORT loc_15_225B
        call   sub_15_2070
        jne    SHORT loc_15_225B
        mov    edi, esi
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        movzx  eax, word ptr [44ch]
        movzx  edx, dh
        mul    edx
        pop    edx
        add    esi, eax
        mov    ch, dl
        movzx  edx, byte ptr [462h]
        movzx  edx, word ptr [edx*2 + 450h]
        push   ebx
        mov    eax, 0a0h
        mul    dh
        add    eax, esi
        movzx  ebx, dl
        shl    ebx, 1
        add    eax, ebx
        mov    word ptr [eax], cx
        pop    ebx
        inc    dl
        call   sub_15_2136
        jmp    NEAR PTR loc_15_2368
loc_15_22B4:
        mov    ecx, eax
        cmp    cl, 7
        je     SHORT loc_15_225B
        cmp    cl, 0dh
        jne    SHORT loc_15_22F3
        test   byte ptr [edi], 1
        je     SHORT loc_15_22F3
        dec    word ptr [edi + 5d0h]
        jne    SHORT loc_15_22F3
        pushad
        db  0A1h, 0DCh, 00Fh, 000h, 000h
        mov    word ptr [edi + 5d0h], ax
        mov    word ptr [edi + 5cch], 1
        call   sub_15_4780
        or     byte ptr [edi + 3], 20h
        call   sub_15_46DC
        popad
loc_15_22F3:
        call   sub_15_2070
        jne    NEAR PTR loc_15_225B
        mov    edi, esi
        movzx  eax, word ptr [44eh]
        add    esi, eax
        movzx  edx, byte ptr [462h]
        movzx  edx, word ptr [edx*2 + 450h]
        cmp    cl, 0dh
        je     SHORT loc_15_236A
        cmp    cl, 8
        je     SHORT loc_15_236E
        cmp    cl, 0ah
        je     SHORT loc_15_234C
        push   ebx
        mov    eax, 0a0h
        mul    dh
        add    eax, esi
        movzx  ebx, dl
        shl    ebx, 1
        add    eax, ebx
        mov    byte ptr [eax], cl
        pop    ebx
        inc    dl
        call   sub_15_2136
        cmp    dl, byte ptr [44ah]
        jne    SHORT loc_15_235C
        xor    dl, dl
loc_15_234C:
        cmp    dh, byte ptr [484h]
        je     SHORT loc_15_2376
        ja     NEAR PTR loc_15_225B
        inc    dh
loc_15_235C:
        movzx  ecx, byte ptr [462h]
        call   sub_15_2702
loc_15_2368:
        clc
        ret
loc_15_236A:
        xor    dl, dl
        jmp    SHORT loc_15_235C
loc_15_236E:
        or     dl, dl
        je     SHORT loc_15_235C
        dec    dl
        jmp    SHORT loc_15_235C
loc_15_2376:
        movzx  ecx, byte ptr [462h]
        db  0A1h, 0CCh, 006h, 000h, 000h
        add    eax, ebx
        mov    dword ptr [eax + 5c8h], 0ffffffffh
        call   sub_15_2702
        mov    edi, esi
        mov    eax, 0a0h
        lea    esi, [eax + edi]
        mul    dh
        or     eax, eax
        je     SHORT loc_15_23A7
        mov    ecx, eax
        rep movsb
loc_15_23A7:
        mov    eax, dword ptr [edi]
        mov    al, 20h
        mov    ecx, 50h
        cld
        rep stosw
        jmp    SHORT loc_15_2368
loc_15_23B6:
        call   sub_15_2070
        jne    NEAR PTR loc_15_225B
        movzx  ecx, byte ptr [ebp + 11h]
        mov    ax, word ptr [460h]
        mov    word ptr [ebp + 18h], ax
        mov    ax, word ptr [ecx*2 + 450h]
        mov    word ptr [ebp + 14h], ax
        jmp    SHORT loc_15_2368
loc_15_23DD:
        movzx  ecx, byte ptr [ebp + 11h]
        cmp    cl, byte ptr [462h]
        jne    NEAR PTR loc_15_225B
        call   sub_15_2070
        jne    NEAR PTR loc_15_225B
        mov    edi, esi
        movzx  eax, word ptr [44eh]
        add    esi, eax
        mov    edx, dword ptr [ebp + 14h]
        call   sub_15_2702
        or     dl, dl
        jne    SHORT loc_15_2476
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        test   byte ptr [edi], 1
        je     SHORT loc_15_2476
        xchg   word ptr [edi + 5d2h], dx
        cmp    dx, word ptr [edi + 5d2h]
        jb     SHORT loc_15_2476
        je     SHORT loc_15_244A
        cmp    dh, byte ptr [484h]
        jne    SHORT loc_15_2476
        dec    dh
        cmp    dx, word ptr [edi + 5d2h]
        jne    SHORT loc_15_2476
        inc    word ptr [edi + 5d0h]
        jmp    SHORT loc_15_2476
loc_15_244A:
        dec    word ptr [edi + 5d0h]
        jne    SHORT loc_15_2476
        db  0A1h, 0DCh, 00Fh, 000h, 000h
        mov    word ptr [edi + 5d0h], ax
        mov    word ptr [edi + 5cch], 1
        call   sub_15_4780
        or     byte ptr [edi + 3], 20h
        call   sub_15_46DC
loc_15_2476:
        jmp    NEAR PTR loc_15_2368
loc_15_247B:
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        je     NEAR PTR loc_15_225B
        mov    dword ptr [edi + 5c8h], 0ffffffffh
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        mov    al, byte ptr [ebp + 19h]
        test   byte ptr [487h], 8
        jne    NEAR PTR loc_15_2593
        cmp    al, byte ptr [484h]
        jbe    SHORT loc_15_24BA
        mov    al, byte ptr [484h]
        mov    byte ptr [ebp + 19h], al
loc_15_24BA:
        mov    al, byte ptr [ebp + 15h]
        cmp    al, byte ptr [484h]
        jbe    SHORT loc_15_24CD
        mov    al, byte ptr [484h]
        mov    byte ptr [ebp + 15h], al
loc_15_24CD:
        sub    al, byte ptr [ebp + 19h]
        jb     NEAR PTR loc_15_2585
        inc    al
        cmp    byte ptr [ebp + 1ch], al
        jbe    SHORT loc_15_24E0
        mov    byte ptr [ebp + 1ch], al
loc_15_24E0:
        mov    esi, dword ptr [edi + 547h]
        call   sub_15_168F
        add    esi, eax
        mov    edx, eax
        call   sub_15_16C3
        jb     NEAR PTR loc_15_2593
        mov    eax, ecx
        mul    byte ptr [ebp + 15h]
        xchg   edx, eax
        add    edx, eax
        add    edx, ecx
        add    edx, ecx
        dec    edx
        shr    eax, 0ch
        shr    edx, 0ch
loc_15_250D:
        call   sub_15_456A
        inc    eax
        cmp    eax, edx
        jb     SHORT loc_15_250D
        mov    eax, ecx
        dec    cl
        cmp    byte ptr [ebp + 18h], cl
        jbe    SHORT loc_15_2523
        mov    byte ptr [ebp + 18h], cl
loc_15_2523:
        cmp    byte ptr [ebp + 14h], cl
        jbe    SHORT loc_15_252B
        mov    byte ptr [ebp + 14h], cl
loc_15_252B:
        mov    cl, byte ptr [ebp + 18h]
        cmp    cl, byte ptr [ebp + 14h]
        jg     SHORT loc_15_2585
        shl    eax, 1
        push   eax
        mul    byte ptr [ebp + 19h]
        add    esi, eax
        pop    eax
        push   edi
        movzx  edx, byte ptr [ebp + 14h]
        movzx  edi, byte ptr [ebp + 18h]
        add    esi, edi
        add    esi, edi
        sub    edx, edi
        inc    edx
        mov    edi, esi
        cmp    byte ptr [ebp + 1ch], 0
        je     SHORT loc_15_2568
        cmp    byte ptr [ebp + 1dh], 6
        je     SHORT loc_15_2561
        call   sub_15_26B2
        jmp    SHORT loc_15_257B
loc_15_2561:
        call   sub_15_2671
        jmp    SHORT loc_15_257B
loc_15_2568:
        xchg   edx, eax
        sub    edx, eax
        sub    edx, eax
        movzx  ecx, byte ptr [ebp + 15h]
        sub    cl, byte ptr [ebp + 19h]
        inc    cl
        call   sub_15_269E
loc_15_257B:
        pop    edi
        or     byte ptr [edi + 3], 20h
        call   sub_15_46DC
loc_15_2585:
        pop    dword ptr [ebp + 14h]
        pop    dword ptr [ebp + 18h]
        pop    dword ptr [ebp + 1ch]
        jmp    NEAR PTR loc_15_2368
loc_15_2593:
        pop    dword ptr [ebp + 14h]
        pop    dword ptr [ebp + 18h]
        pop    dword ptr [ebp + 1ch]
        jmp    NEAR PTR loc_15_225B
loc_15_25A1:
        mov    edx, dword ptr [ebp + 10h]
        shl    edx, 10h
        mov    dx, ax
        push   edx
        clc
        push   edi
        pushfd
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0a4h], 0
        je     SHORT loc_15_25C5
        popfd
        call   dword ptr [edi + 0a4h]
        jmp    SHORT loc_15_25C6
loc_15_25C5:
        popfd
loc_15_25C6:
        pop    edi
        pop    edx
        jae    SHORT loc_15_25D4
        call   sub_15_2611
        jmp    NEAR PTR loc_15_2368
loc_15_25D4:
        cmp    al, 3
        je     SHORT loc_15_25FC
        mov    dx, word ptr [ebp + 10h]
        shl    edx, 10h
        mov    dx, word ptr [ebp + 1ch]
        xor    eax, eax
        db  0BEh, 011h, 026h, 000h, 000h
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
        mov    byte ptr [edi + 638h], 0ffh
        jmp    NEAR PTR loc_15_225B
loc_15_25FC:
        mov    eax, dword ptr [edi + 620h]
        mov    word ptr [ebp + 10h], ax
        mov    word ptr [ebp + 1ch], 4fh
        jmp    NEAR PTR loc_15_2368
sub_15_2611:
        push   edi
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0bch], 0
        je     SHORT loc_15_262F
        call   dword ptr [edi + 0bch]
loc_15_262F:
        pop    edi
        cmp    dx, 4f02h
        jne    SHORT loc_15_2668
        cmp    word ptr [ebp + 1ch], 4fh
        jne    SHORT loc_15_2668
        shr    edx, 10h
        and    dl, 7fh
        and    dh, 7fh
        mov    dword ptr [edi + 620h], edx
        and    byte ptr [edi + 1], 0dfh
        cmp    edx, 100h
        jb     SHORT loc_15_2668
        or     byte ptr [edi + 1], 20h
        call   sub_15_09FE
        mov    dword ptr [edi + 61ch], ecx
loc_15_2668:
        mov    byte ptr [edi + 638h], 0
        pop    edi
        ret
sub_15_2671:
        push   edx
        mov    edx, eax
        mul    byte ptr [ebp + 1ch]
        add    esi, eax
        movzx  ecx, byte ptr [ebp + 15h]
        sub    cl, byte ptr [ebp + 19h]
        inc    cl
        sub    cl, byte ptr [ebp + 1ch]
        pop    eax
        sub    edx, eax
        sub    edx, eax
        jecxz  loc_15_269A
        cld
loc_15_268D:
        push   ecx
        mov    ecx, eax
        rep movsw
        add    esi, edx
        add    edi, edx
        pop    ecx
        loop   loc_15_268D
loc_15_269A:
        movzx  ecx, byte ptr [ebp + 1ch]
sub_15_269E:
        mov    esi, eax
        mov    ah, byte ptr [ebp + 11h]
        mov    al, 20h
        cld
loc_15_26A6:
        push   ecx
        mov    ecx, esi
        rep stosw
        add    edi, edx
        pop    ecx
        loop   loc_15_26A6
        ret
sub_15_26B2:
        push   ebx
        mov    ebx, eax
        add    esi, edx
        add    esi, edx
        mov    edi, esi
        movzx  ecx, byte ptr [ebp + 15h]
        sub    cl, byte ptr [ebp + 19h]
        mul    cl
        add    edi, eax
        mov    eax, ebx
        sub    ebx, edx
        sub    ebx, edx
        sub    edi, 2
        sub    cl, byte ptr [ebp + 1ch]
        jl     SHORT loc_15_26EB
        mul    cl
        add    esi, eax
        sub    esi, 2
        inc    cl
        std
loc_15_26DE:
        push   ecx
        mov    ecx, edx
        rep movsw
        sub    esi, ebx
        sub    edi, ebx
        pop    ecx
        loop   loc_15_26DE
loc_15_26EB:
        mov    cl, byte ptr [ebp + 1ch]
        mov    ah, byte ptr [ebp + 11h]
        mov    al, 20h
        std
loc_15_26F4:
        push   ecx
        mov    ecx, edx
        rep stosw
        sub    edi, ebx
        pop    ecx
        loop   loc_15_26F4
        cld
        pop    ebx
        ret
sub_15_2702:
        mov    word ptr [ecx*2 + 450h], dx
        mov    eax, 0a0h
        mul    dh
        add    eax, esi
        movzx  ecx, dl
        shl    ecx, 1
        add    eax, ecx
        sub    eax, edi
        shr    eax, 1
        xchg   al, ah
        mov    ecx, ebx
        db  003h, 00Dh, 0CCh, 006h, 000h, 000h
        mov    word ptr [ecx + 1ah], ax
        db  02Bh, 00Dh, 0CCh, 006h, 000h, 000h
        db  03Bh, 00Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_2752
        push   edx
        push   eax
        mov    dx, word ptr [463h]
        mov    al, 0fh
        out    dx, al
        inc    edx
        mov    al, ah
        out    dx, al
        dec    edx
        mov    al, 0eh
        out    dx, al
        inc    edx
        pop    eax
        out    dx, al
        pop    edx
loc_15_2752:
        ret
loc_15_2753:
        inc    word ptr [ebp + 24h]
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        cmp    byte ptr [ebp + 1dh], 5
        je     NEAR PTR loc_15_282C
sub_15_2769:
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 004h
        je     NEAR PTR loc_15_282B
        db  0F6h, 005h, 0A0h, 007h, 000h, 000h, 007h
        jne    NEAR PTR loc_15_282B
        movzx  eax, word ptr [ebp + 1ch]
        or     ah, ah
        jne    SHORT loc_15_27C3
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0c0h], 0
        je     SHORT loc_15_27A0
        call   dword ptr [edi + 0c0h]
loc_15_27A0:
        pop    edi
        and    al, 7fh
        mov    dword ptr [edi + 620h], eax
        and    byte ptr [edi + 1], 0dfh
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        jne    SHORT loc_15_27C3
        push   0ffh
        push   1
        db  0E8h, 06Ah, 0E4h, 0F0h, 0FFh
        pop    ecx
        pop    ecx
loc_15_27C3:
        cmp    byte ptr [ebp + 1dh], 11h
        jne    SHORT loc_15_27CE
        or     byte ptr [edi], 20h
        jmp    SHORT loc_15_27D5
loc_15_27CE:
        mov    byte ptr [edi + 4a6h], 1
loc_15_27D5:
        call   sub_15_3A50
        bts    dword ptr [edi], 1
        jb     SHORT loc_15_282B
        call   sub_15_4BBC
        mov    eax, 1000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        test   byte ptr [edi], 1
        je     SHORT loc_15_27FD
        call   sub_15_4780
        or     byte ptr [edi], 4
loc_15_27FD:
        mov    ax, word ptr [ebp + 1ch]
        cmp    ah, 11h
        je     SHORT loc_15_281B
        or     al, al
        js     SHORT loc_15_281B
        call   sub_15_4538
        mov    dword ptr [edi + 609h], 0
        jmp    SHORT loc_15_282B
loc_15_281B:
        or     byte ptr [edi], 8
        call   sub_15_29F4
        add    eax, ecx
        mov    dword ptr [edi + 609h], eax
loc_15_282B:
        ret
loc_15_282C:
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_282B
        bts    dword ptr [edi], 4
        jb     SHORT loc_15_282B
        call   sub_15_4BBC
        jmp    SHORT loc_15_282B
loc_15_2841:
        inc    word ptr [ebp + 24h]
loc_15_2845:
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 004h
        je     SHORT loc_15_288E
        db  0F6h, 005h, 0A0h, 007h, 000h, 000h, 007h
        jne    SHORT loc_15_288E
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        btr    dword ptr [edi], 4
        jb     SHORT loc_15_2886
        call   sub_15_4740
        btr    dword ptr [edi], 1
        jae    SHORT loc_15_288E
        mov    eax, 0fffff000h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
        call   sub_15_3864
loc_15_2886:
        call   sub_15_4BBC
        and    byte ptr [edi], 0d7h
loc_15_288E:
        ret
loc_15_288F:
        db  00Fh, 0BAh, 02Dh, 0C4h, 006h, 000h, 000h, 009h
        mov    eax, dword ptr [ebp + 1ch]
        cmp    ax, 1002h
        je     SHORT loc_15_28B2
loc_15_28A0:
        db  08Bh, 00Dh, 0B4h, 00Eh, 000h, 000h
        db  08Bh, 015h, 0B8h, 00Eh, 000h, 000h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_15_28B2:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        test   byte ptr [edi + 3], 1
        je     SHORT loc_15_28A0
        db  0CDh, 020h, 047h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_15_28C8:
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 58h], 0
        je     SHORT loc_15_28D7
        call   dword ptr [edi + 58h]
loc_15_28D7:
        pop    edi
        db  08Bh, 035h, 0C8h, 00Eh, 000h, 000h
        db  083h, 03Dh, 0D2h, 00Eh, 000h, 000h, 010h
        ja     SHORT loc_15_28EF
        add    esi, 9fffh
        jmp    SHORT loc_15_28F5
loc_15_28EF:
        add    esi, 0fffch
loc_15_28F5:
        ret
sub_15_28F6:
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 5ch], 0
        je     SHORT loc_15_2905
        call   dword ptr [edi + 5ch]
loc_15_2905:
        pop    edi
        ret
sub_15_2907:
        push   eax
        push   ecx
        test   byte ptr [edi + 23h], 1
        je     SHORT loc_15_2922
        push   eax
        mov    al, byte ptr [edi + 7ch]
        and    al, 3
        cmp    al, 3
        pop    eax
        jne    SHORT loc_15_2922
        and    eax, 0fh
        and    ecx, 0fh
        jmp    SHORT loc_15_2928
loc_15_2922:
        test   byte ptr [edi + 6ah], 8
        je     SHORT loc_15_2928
loc_15_2928:
        cmp    eax, ecx
        pop    ecx
        pop    eax
        ret
        db  0C1h, 0E8h, 002h, 0C1h, 0E9h, 002h, 0EBh, 0F3h
sub_15_2935:
        mov    esi, eax
        test   byte ptr [edi + 23h], 1
        je     SHORT loc_15_294D
        push   eax
        mov    al, byte ptr [edi + 7ch]
        and    al, 3
        cmp    al, 3
        pop    eax
        jne    SHORT loc_15_294D
        and    esi, 0fh
        jmp    SHORT loc_15_2953
loc_15_294D:
        test   byte ptr [edi + 6ah], 8
        je     SHORT loc_15_2953
loc_15_2953:
        shl    esi, 3
        db  003h, 035h, 0D6h, 00Eh, 000h, 000h
        ret
        db  0C1h, 0EEh, 002h, 0EBh, 0F1h
sub_15_2962:
        db  03Bh, 01Dh, 0A4h, 007h, 000h, 000h
        clc
        jne    SHORT loc_15_2996
        push   eax
        push   ecx
        push   esi
        db  08Bh, 035h, 0D6h, 00Eh, 000h, 000h
        db  08Bh, 00Dh, 0D2h, 00Eh, 000h, 000h
loc_15_297A:
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_15_2993
        cmp    ebx, dword ptr [eax + 8]
        clc
        jne    SHORT loc_15_2993
        cmp    dword ptr [esi], 0
        clc
        jne    SHORT loc_15_2993
        add    esi, 8
        loop   loc_15_297A
        stc
loc_15_2993:
        pop    esi
        pop    ecx
        pop    eax
loc_15_2996:
        ret
sub_15_2997:
        push   edx
        mov    ecx, eax
        movzx  eax, word ptr [ecx]
        db  03Bh, 005h, 0CCh, 00Eh, 000h, 000h
        jb     SHORT loc_15_29B7
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 8], 0
        je     SHORT loc_15_29B4
        call   dword ptr [edi + 8]
loc_15_29B4:
        pop    edi
        jmp    SHORT loc_15_29C7
loc_15_29B7:
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0ch], 0
        je     SHORT loc_15_29C6
        call   dword ptr [edi + 0ch]
loc_15_29C6:
        pop    edi
loc_15_29C7:
        and    eax, 0fh
        mov    edx, eax
        call   sub_15_1C3B
        movzx  eax, al
        mov    ah, al
        add    al, dl
        xor    ecx, ecx
        pop    edx
        ret
sub_15_29DC:
        push   ecx
        push   esi
        call   sub_15_2935
        movzx  ecx, word ptr [ecx]
        shl    ecx, 3
        db  003h, 00Dh, 0D6h, 00Eh, 000h, 000h
        cmp    ecx, esi
        pop    esi
        pop    ecx
        ret
sub_15_29F4:
        mov    ecx, edi
        db  02Bh, 00Dh, 0CCh, 006h, 000h, 000h
        db  03Bh, 00Dh, 0F0h, 00Eh, 000h, 000h
        jne    SHORT loc_15_2A17
        db  08Bh, 00Dh, 0CCh, 00Eh, 000h, 000h
        inc    ecx
        jecxz  loc_15_2A17
        dec    ecx
        db  00Fh, 0B7h, 005h, 0D0h, 00Eh, 000h, 000h
        stc
        ret
loc_15_2A17:
        db  0E8h, 070h, 0D8h, 0F0h, 0FFh
        jae    SHORT loc_15_2A53
        mov    ecx, dword ptr [edi + 61ch]
        cmp    ecx, -1
        je     SHORT loc_15_2A2D
        or     ecx, ecx
        jne    SHORT loc_15_2A4A
loc_15_2A2D:
        push   ebx
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jae    SHORT loc_15_2A3E
        mov    ebx, edi
        db  02Bh, 01Dh, 0CCh, 006h, 000h, 000h
loc_15_2A3E:
        call   sub_15_09FE
        mov    dword ptr [edi + 61ch], ecx
        pop    ebx
loc_15_2A4A:
        mov    ecx, 10h
        xor    eax, eax
        stc
        ret
loc_15_2A53:
        and    byte ptr [edi + 1], 0dfh
        push   edx
        call   sub_15_16C3
        jae    SHORT loc_15_2A7F
        imul   ecx, eax
        test   byte ptr [edi + 6ah], 8
        jne    SHORT loc_15_2A87
        shr    ecx, 2
        cmp    byte ptr [edi + 5b3h], 58h
        je     SHORT loc_15_2A87
        test   byte ptr [edi + 7bh], 20h
        jne    SHORT loc_15_2A87
        shr    ecx, 1
        clc
        jmp    SHORT loc_15_2A87
loc_15_2A7F:
        imul   eax, ecx
        shl    eax, 1
        mov    ecx, eax
        stc
loc_15_2A87:
        cmc
        pushfd
        call   sub_15_168F
        push   eax
        and    eax, 0fffh
        add    ecx, eax
        add    ecx, 0fffh
        shr    ecx, 0ch
        pop    eax
        shr    eax, 0ch
        cmp    dword ptr [edi + 609h], 0
        je     SHORT loc_15_2AC6
        db  081h, 0FFh, 0BCh, 007h, 000h, 000h
        je     SHORT loc_15_2AC6
        add    ecx, eax
        xor    eax, eax
        cmp    ecx, dword ptr [edi + 609h]
        jae    SHORT loc_15_2AC6
        mov    ecx, dword ptr [edi + 609h]
loc_15_2AC6:
        popfd
        pop    edx
        ret
sub_15_2AC9:
        push   ecx
        push   edx
        mov    edx, eax
        call   sub_15_29F4
        xchg   edx, eax
        cmp    eax, edx
        jb     SHORT loc_15_2AE1
        add    edx, ecx
        cmp    eax, edx
        jae    SHORT loc_15_2AE1
        xor    dl, dl
        jmp    SHORT loc_15_2AE4
loc_15_2AE1:
        or     dl, 1
loc_15_2AE4:
        pop    edx
        pop    ecx
        ret
sub_15_2AE7:
        push   ecx
        mov    ecx, eax
        call   sub_15_1C3B
        push   eax
        movzx  eax, al
        sub    ecx, eax
        pop    eax
        jb     SHORT loc_15_2AFF
        cmp    cl, ah
        cmc
        jb     SHORT loc_15_2AFF
        xchg   ecx, eax
        clc
loc_15_2AFF:
        pop    ecx
        ret
sub_15_2B01:
        push   edx
        mov    edx, eax
        call   sub_15_1C3B
        movzx  eax, al
        add    eax, edx
        pop    edx
        ret
sub_15_2B10:
        pushad
        call   sub_15_2935
        mov    edx, dword ptr [esi + 4]
        or     edx, edx
        jne    SHORT loc_15_2B2B
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        je     NEAR PTR loc_15_2BEE
        jmp    SHORT loc_15_2B55
loc_15_2B2B:
        cmp    ebx, dword ptr [edx + 8]
        jne    SHORT loc_15_2B49
        movzx  eax, word ptr [edx + 4]
        mov    edx, esi
        call   sub_15_2935
        mov    eax, dword ptr [esp + 1ch]
        cmp    esi, edx
        je     NEAR PTR loc_15_2BEE
        mov    esi, edx
loc_15_2B49:
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        je     NEAR PTR loc_15_2BE0
loc_15_2B55:
        db  08Bh, 035h, 0D6h, 00Eh, 000h, 000h
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        db  08Bh, 00Dh, 0D2h, 00Eh, 000h, 000h
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        je     SHORT loc_15_2B89
        push   eax
        push   edx
        db  0A1h, 0CCh, 00Eh, 000h, 000h
        sub    ecx, eax
        jg     SHORT loc_15_2B7E
        pop    edx
        pop    eax
        jmp    SHORT loc_15_2BDB
loc_15_2B7E:
        mov    edx, 8
        mul    edx
        add    esi, eax
        pop    edx
        pop    eax
loc_15_2B89:
        xor    ebx, ebx
        xor    edi, edi
loc_15_2B8D:
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_15_2BCE
        cmp    dword ptr [esi], 0
        je     SHORT loc_15_2BA4
        mov    ebp, eax
        sub    ebp, dword ptr [esi]
        cmp    ebp, ebx
        jb     SHORT loc_15_2BA4
        mov    ebx, ebp
        mov    edi, esi
loc_15_2BA4:
        add    esi, 8
        loop   loc_15_2B8D
        mov    esi, edi
        mov    edi, dword ptr [esp]
        mov    ebx, dword ptr [esp + 10h]
        or     esi, esi
        je     SHORT loc_15_2BDB
        mov    eax, dword ptr [esi + 4]
        call   sub_15_2D96
        mov    eax, dword ptr [esp + 1ch]
        jae    SHORT loc_15_2BEE
        mov    eax, dword ptr [esi + 4]
        call   sub_15_2CA7
        jmp    SHORT loc_15_2B55
loc_15_2BCE:
        mov    edi, dword ptr [esp]
        mov    ebx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 1ch]
        jmp    SHORT loc_15_2BEE
loc_15_2BDB:
        xor    eax, eax
        stc
        jmp    SHORT loc_15_2C4E
loc_15_2BE0:
        mov    ecx, eax
        mov    eax, dword ptr [esi + 4]
        call   sub_15_2D96
        jb     SHORT loc_15_2BDB
        mov    eax, ecx
loc_15_2BEE:
        mov    edx, eax
        mov    ecx, esi
        db  08Bh, 035h, 0DAh, 00Eh, 000h, 000h
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        jb     SHORT loc_15_2BDB
        mov    word ptr [eax + 4], dx
        mov    word ptr [eax + 6], 0
        mov    dword ptr [eax + 8], ebx
        mov    byte ptr [eax + 3], 0
        cmp    dword ptr [ecx + 4], 0
        je     SHORT loc_15_2C24
        push   ecx
        mov    ecx, dword ptr [ecx + 4]
        db  0CDh, 020h, 0A0h, 000h, 001h, 000h
        pop    ecx
        jmp    SHORT loc_15_2C2D
loc_15_2C24:
        mov    dword ptr [ecx + 4], eax
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
loc_15_2C2D:
        db  02Bh, 00Dh, 0D6h, 00Eh, 000h, 000h
        shr    ecx, 3
        mov    word ptr [eax], cx
        mov    esi, eax
        call   sub_15_2CA7
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [eax + 0ch], ecx
        clc
        inc    byte ptr [edi + 60eh]
loc_15_2C4E:
        mov    dword ptr [esp + 1ch], eax
        popad
        ret
sub_15_2C54:
        pushad
        db  08Bh, 01Dh, 0A4h, 007h, 000h, 000h
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  08Bh, 035h, 0D6h, 00Eh, 000h, 000h
        db  08Bh, 00Dh, 0D2h, 00Eh, 000h, 000h
loc_15_2C6F:
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_15_2CA0
        cmp    ebx, dword ptr [eax + 8]
        je     SHORT loc_15_2C8A
        cmp    dword ptr [esi], 0
        jne    SHORT loc_15_2CA0
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [esi], eax
        jmp    SHORT loc_15_2CA0
loc_15_2C8A:
        cmp    dword ptr [esi], 0
        je     SHORT loc_15_2CA0
        movzx  eax, word ptr [eax + 4]
        call   sub_15_2AC9
        jne    SHORT loc_15_2CA0
        mov    dword ptr [esi], 0
loc_15_2CA0:
        add    esi, 8
        loop   loc_15_2C6F
        popad
        ret
sub_15_2CA7:
        push   eax
        push   ecx
        push   esi
        push   0
        mov    eax, dword ptr [esi + 8]
        db  03Bh, 005h, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_2CF5
        movzx  eax, word ptr [esi]
        mov    ecx, eax
        shl    eax, 3
        db  003h, 005h, 0D6h, 00Eh, 000h, 000h
        mov    esi, dword ptr [eax + 4]
loc_15_2CC8:
        movzx  eax, word ptr [esi + 4]
        call   sub_15_2AC9
        je     SHORT loc_15_2CFE
        call   sub_15_3852
        jb     SHORT loc_15_2CFE
        mov    eax, esi
        db  08Bh, 035h, 0DAh, 00Eh, 000h, 000h
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    esi, eax
        je     SHORT loc_15_2CF1
        cmp    cx, word ptr [esi]
        je     SHORT loc_15_2CC8
loc_15_2CF1:
        mov    esi, dword ptr [esp + 4]
loc_15_2CF5:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [esp], eax
loc_15_2CFE:
        movzx  eax, word ptr [esi]
        shl    eax, 3
        db  003h, 005h, 0D6h, 00Eh, 000h, 000h
        pop    dword ptr [eax]
        pop    esi
        pop    ecx
        pop    eax
        ret
sub_15_2D10:
        pushad
        mov    edx, eax
        movzx  eax, word ptr [eax + 4]
        push   esi
        call   sub_15_2935
        db  02Bh, 035h, 0D6h, 00Eh, 000h, 000h
        shr    esi, 3
        cmp    si, word ptr [eax]
        pop    esi
        je     SHORT loc_15_2D4A
        or     esi, esi
        jne    SHORT loc_15_2D33
        mov    esi, dword ptr [edx + 0ch]
loc_15_2D33:
        mov    eax, ecx
        call   sub_15_2B10
        jb     SHORT loc_15_2D48
        mov    dword ptr [esp + 1ch], eax
        mov    eax, edx
        call   sub_15_2D96
        clc
loc_15_2D48:
        popad
        ret
loc_15_2D4A:
        or     esi, esi
        je     SHORT loc_15_2D48
        mov    dword ptr [edx + 0ch], esi
        clc
        jmp    SHORT loc_15_2D48
sub_15_2D54:
        push   ecx
        push   edx
        push   esi
        mov    ecx, eax
        db  03Bh, 00Dh, 0D2h, 00Eh, 000h, 000h
        jb     SHORT loc_15_2D61
loc_15_2D61:
        call   sub_15_2935
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_15_2D91
        mov    dx, word ptr [eax]
loc_15_2D70:
        mov    ebx, eax
        cmp    cx, word ptr [eax + 4]
        je     SHORT loc_15_2D8B
        db  08Bh, 035h, 0DAh, 00Eh, 000h, 000h
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        je     SHORT loc_15_2D8B
        cmp    dx, word ptr [eax]
        je     SHORT loc_15_2D70
loc_15_2D8B:
        mov    eax, ebx
        mov    ebx, dword ptr [eax + 8]
        stc
loc_15_2D91:
        cmc
        pop    esi
        pop    edx
        pop    ecx
        ret
sub_15_2D96:
        pushad
        movzx  ebp, word ptr [eax]
        shl    ebp, 3
        db  003h, 02Dh, 0D6h, 00Eh, 000h, 000h
        mov    eax, dword ptr [ebp + 4]
        mov    ebx, dword ptr [eax + 8]
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
loc_15_2DB1:
        mov    dword ptr [ebp + 4], eax
        xor    ecx, ecx
        push   eax
        push   ebp
        call   dword ptr [eax + 0ch]
        pop    ebp
        pop    eax
        jb     SHORT loc_15_2DF4
        dec    byte ptr [edi + 60eh]
        xor    edx, edx
        mov    dword ptr [ebp + 4], edx
        mov    edx, eax
        db  08Bh, 035h, 0DAh, 00Eh, 000h, 000h
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        xchg   edx, eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        movzx  ecx, word ptr [eax]
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        or     edx, edx
        je     SHORT loc_15_2DF3
        mov    eax, edx
        cmp    cx, word ptr [eax]
        je     SHORT loc_15_2DB1
loc_15_2DF3:
        clc
loc_15_2DF4:
        popad
        ret
sub_15_2DF6:
        push   eax
        push   edx
        mov    ah, byte ptr [edi + 7ch]
        and    ah, 0ch
        jmp    SHORT loc_15_2E04
sub_15_2E00:
        push   eax
        push   edx
        mov    ah, 4
loc_15_2E04:
        mov    edx, 3ceh
        in     al, dx
        push   eax
        mov    al, 6
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0f3h
        or     al, ah
        out    dx, al
        pop    eax
        dec    edx
        out    dx, al
        pop    edx
        pop    eax
        ret
sub_15_2E1B:
        or     ebx, ebx
        je     SHORT loc_15_2E24
        call   sub_15_18A9
loc_15_2E24:
        ret
sub_15_2E25:
        btr    dword ptr [edi], 0eh
        jae    SHORT loc_15_2E3D
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        jne    SHORT loc_15_2E38
        call   sub_15_1193
loc_15_2E38:
        call   sub_15_18A9
loc_15_2E3D:
        ret
sub_15_2E3E:
        pushad
        mov    esi, eax
        call   sub_15_2CA7
        mov    esi, eax
        call   sub_15_2997
        xchg   ecx, eax
        xor    edx, edx
        or     ebx, ebx
        je     SHORT loc_15_2E89
        cmp    ch, 0b0h
        jb     SHORT loc_15_2E76
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        je     SHORT loc_15_2E76
        sub    cl, ch
        add    cl, 0a0h
        bts    dword ptr [edi], 0eh
        jb     SHORT loc_15_2E89
        or     dl, 1
        call   sub_15_2E00
        jmp    SHORT loc_15_2E84
loc_15_2E76:
        btr    dword ptr [edi], 0eh
        jae    SHORT loc_15_2E89
        or     dl, 1
        call   sub_15_2DF6
loc_15_2E84:
        call   sub_15_2E1B
loc_15_2E89:
        movzx  ecx, cl
        mov    dword ptr [esp + 18h], ecx
        bt     edx, 0
        popad
        ret
        db  050h, 051h, 0E8h, 0FAh, 0FAh, 0FFh, 0FFh, 03Bh, 01Dh, 0A8h, 007h, 000h, 000h, 075h, 005h, 080h
        db  0F9h, 0C0h, 072h, 005h, 080h, 0F9h, 0B8h, 0B1h, 000h, 0F5h, 080h, 0D1h, 000h, 00Fh, 0BAh, 027h
        db  00Eh, 080h, 0D9h, 000h, 0D0h, 0E9h, 059h, 058h, 0C3h
sub_15_2EBF:
        push   esi
        mov    esi, eax
        call   sub_15_2CA7
        pop    esi
        ret
        db  000h, 000h, 000h
VDD_Get_VM_Info:
        db  08Bh, 03Dh, 0A8h, 007h, 000h, 000h
        db  08Bh, 035h, 0ACh, 007h, 000h, 000h
        ret
VDD_Hide_Cursor:
        push   edi
        mov    edi, ebx
        db  003h, 03Dh, 0CCh, 006h, 000h, 000h
        and    byte ptr [edi + 2], 0f7h
        or     eax, eax
        je     SHORT loc_15_2EF7
        or     byte ptr [edi + 2], 8
        mov    word ptr [edi + 4bdh], 0ffffh
loc_15_2EF7:
        pop    edi
        ret
VDD_Query_Access:
        push   eax
        push   edi
        db  0F6h, 005h, 0A0h, 007h, 000h, 000h, 007h
        jne    SHORT loc_15_2F38
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        je     SHORT loc_15_2F2A
        db  0A1h, 0D2h, 00Eh, 000h, 000h
        db  03Bh, 005h, 0CCh, 00Eh, 000h, 000h
        je     SHORT loc_15_2F38
        cmp    byte ptr [edi + 5b3h], 58h
        je     SHORT loc_15_2F38
loc_15_2F2A:
        test   byte ptr [edi], 80h
        jne    SHORT loc_15_2F38
        test   byte ptr [edi + 2], 20h
        jne    SHORT loc_15_2F38
        clc
        jmp    SHORT loc_15_2F39
loc_15_2F38:
        stc
loc_15_2F39:
        pop    edi
        pop    eax
        ret
VDD_Set_VMType:
        pushad
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        or     byte ptr [edi], 1
        mov    dword ptr [edi + 5c8h], 0ffffffffh
        or     eax, eax
        jne    SHORT loc_15_2F70
        and    byte ptr [edi], 0feh
        call   sub_15_01B1
        call   sub_15_4780
        test   byte ptr [edi + 2], 2
        je     SHORT loc_15_2F6E
        db  0E8h, 0A7h, 0D4h, 0F0h, 0FFh
loc_15_2F6E:
        popad
        ret
loc_15_2F70:
        btr    dword ptr [edi], 16h
        jae    SHORT loc_15_2F7B
        call   sub_15_394E
loc_15_2F7B:
        mov    dword ptr [edi + 5f0h], 28h
        or     byte ptr [edi + 1], 2
        push   edi
        call   sub_15_46DC
        pop    edi
        test   dword ptr [edi], 200080h
        je     SHORT loc_15_2FAC
        test   byte ptr [edi + 1], 20h
        jne    SHORT loc_15_2FAC
        cmp    byte ptr [edi + 5b3h], 56h
        je     SHORT loc_15_2FAC
        db  0E8h, 06Fh, 0D5h, 0F0h, 0FFh
loc_15_2FAC:
        popad
        ret
VDD_Check_Update_Soon:
        pushad
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        cmp    word ptr [edi + 5cch], 1
        je     SHORT loc_15_2FF3
        test   byte ptr [edi], 1
        je     SHORT loc_15_2FF3
        call   sub_15_4780
        mov    eax, 5
        mov    edx, edi
        db  0BEh, 0A4h, 046h, 000h, 000h
        db  0CDh, 020h, 03Dh, 000h, 001h, 000h
        mov    dword ptr [edi + 5e4h], esi
        inc    word ptr [edi + 5cch]
        mov    word ptr [edi + 5ceh], 4
loc_15_2FF3:
        popad
        ret
VDD_Get_ModTime:
        pushad
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        test   byte ptr [edi], 4
        je     SHORT loc_15_300B
loc_15_3003:
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        jmp    SHORT loc_15_301D
loc_15_300B:
        call   sub_15_1C57
        jb     SHORT loc_15_3003
        call   sub_15_44DC
        mov    eax, dword ptr [edi + 5e8h]
loc_15_301D:
        mov    dword ptr [esp + 1ch], eax
        popad
        ret
sub_15_3023:
        pushad
        db  080h, 03Dh, 0B1h, 00Fh, 000h, 000h, 000h
        jne    SHORT loc_15_3076
        db  03Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        je     SHORT loc_15_3076
        mov    ecx, 8
        db  0BFh, 00Ch, 00Fh, 000h, 000h
loc_15_303F:
        push   ecx
        mov    edx, dword ptr [edi]
        or     edx, edx
        je     SHORT loc_15_3065
        mov    eax, dword ptr [edi + 0ch]
        mov    esi, dword ptr [edi + 10h]
        db  08Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        push   0
        push   eax
        push   esi
        push   ebx
        push   edx
        db  0CDh, 020h, 07Ch, 000h, 001h, 000h
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_15_3075
loc_15_3065:
        pop    ecx
        add    edi, 14h
        dec    ecx
        jne    SHORT loc_15_303F
        db  0C6h, 005h, 0B1h, 00Fh, 000h, 000h, 0FFh
        jmp    SHORT loc_15_3076
loc_15_3075:
        pop    ecx
loc_15_3076:
        popad
        ret
loc_15_3078:
        pushad
        pushfd
        sti
        db  03Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        jne    NEAR PTR loc_15_3134
        db  080h, 03Dh, 0B1h, 00Fh, 000h, 000h, 000h
        je     NEAR PTR loc_15_3134
        mov    ecx, 8
        db  0BFh, 00Ch, 00Fh, 000h, 000h
loc_15_309E:
        mov    edx, dword ptr [edi]
        or     edx, edx
        je     SHORT loc_15_30C4
        movzx  eax, word ptr [ebp + 38h]
        cmp    eax, edx
        je     SHORT loc_15_30CC
        movzx  eax, word ptr [ebp + 40h]
        cmp    eax, edx
        je     SHORT loc_15_30CC
        movzx  eax, word ptr [ebp + 44h]
        cmp    eax, edx
        je     SHORT loc_15_30CC
        movzx  eax, word ptr [ebp + 3ch]
        cmp    eax, edx
        je     SHORT loc_15_30CC
loc_15_30C4:
        add    edi, 14h
        dec    ecx
        jne    SHORT loc_15_309E
        jmp    SHORT loc_15_3134
loc_15_30CC:
        push   fs
        mov    fs, word ptr [ebp + 28h]
        mov    edx, dword ptr [ebp + 24h]
        mov    al, byte ptr fs:[edx]
        pop    fs
        cmp    al, 6ch
        jb     SHORT loc_15_30F2
        cmp    al, 6fh
        jbe    SHORT loc_15_3134
        cmp    al, 0ech
        jb     SHORT loc_15_30F2
        cmp    al, 0efh
        jbe    SHORT loc_15_3134
        cmp    al, 0fah
        je     SHORT loc_15_3134
        cmp    al, 0fbh
        je     SHORT loc_15_3134
loc_15_30F2:
        mov    ecx, 8
        db  0BFh, 00Ch, 00Fh, 000h, 000h
loc_15_30FC:
        push   ecx
        mov    edx, dword ptr [edi]
        or     edx, edx
        je     SHORT loc_15_311C
        mov    eax, dword ptr [edi + 4]
        mov    esi, dword ptr [edi + 8]
        push   0
        push   eax
        push   esi
        push   ebx
        push   edx
        db  0CDh, 020h, 07Ch, 000h, 001h, 000h
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_15_3131
loc_15_311C:
        pop    ecx
        add    edi, 14h
        dec    ecx
        jne    SHORT loc_15_30FC
        call   sub_15_0660
        db  0C6h, 005h, 0B1h, 00Fh, 000h, 000h, 000h
        jmp    SHORT loc_15_313C
loc_15_3131:
        pop    ecx
        jmp    SHORT loc_15_313C
loc_15_3134:
        popfd
        popad
        db  0FFh, 025h, 0ACh, 00Fh, 000h, 000h
loc_15_313C:
        popfd
        popad
        ret
VDD_PM_API:
        db  03Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        jne    SHORT loc_15_31BC
        movzx  ecx, byte ptr [ebp + 1ch]
        cmp    ecx, 80h
        jb     SHORT loc_15_3199
        sub    ecx, 80h
        cmp    ecx, 11h
        ja     SHORT loc_15_31BC
        cmp    ecx, 0bh
        jae    SHORT loc_15_316D
        db  0FFh, 024h, 08Dh, 0E5h, 031h, 000h, 000h
loc_15_316D:
        movzx  eax, word ptr [ebp + 38h]
        or     eax, eax
        je     SHORT loc_15_317F
        mov    ax, 3800h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
loc_15_317F:
        mov    ebx, esp
        push   dword ptr [ebp + 18h]
        push   eax
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        db  0FFh, 014h, 08Dh, 0E5h, 031h, 000h, 000h
        mov    esp, ebx
        mov    dword ptr [ebp + 1ch], eax
        jmp    SHORT loc_15_31BC
loc_15_3199:
        cmp    cl, 9
        ja     SHORT loc_15_31BC
        mov    edx, ebx
        or     ecx, ecx
        je     SHORT loc_15_31B5
        mov    ebx, dword ptr [ebp + 10h]
        mov    edi, ebx
        db  003h, 03Dh, 0CCh, 006h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 001h, 000h
loc_15_31B5:
        db  0FFh, 024h, 08Dh, 0BDh, 031h, 000h, 000h
loc_15_31BC:
        ret
        db  00Fh, 002h, 000h, 000h, 0F0h, 032h, 000h, 000h, 032h, 032h, 000h, 000h, 060h, 032h, 000h, 000h
        db  0E3h, 032h, 000h, 000h, 025h, 034h, 000h, 000h, 02Bh, 033h, 000h, 000h, 019h, 034h, 000h, 000h
        db  02Dh, 032h, 000h, 000h, 034h, 034h, 000h, 000h, 0FFh, 002h, 000h, 000h, 0CAh, 003h, 000h, 000h
        db  033h, 004h, 000h, 000h, 088h, 004h, 000h, 000h, 097h, 004h, 000h, 000h, 0CDh, 017h, 000h, 000h
        db  0A7h, 004h, 000h, 000h, 0B8h, 004h, 000h, 000h, 0C9h, 004h, 000h, 000h, 074h, 005h, 000h, 000h
        db  0BCh, 031h, 000h, 000h, 02Bh, 00Ch, 000h, 000h, 08Eh, 002h, 000h, 000h, 05Dh, 009h, 000h, 000h
        db  00Ch, 00Ah, 000h, 000h, 030h, 00Ah, 000h, 000h, 03Eh, 00Bh, 000h, 000h, 06Ch, 000h, 000h, 000h
loc_15_322D:
        jb     SHORT loc_15_3258
        clc
        jmp    SHORT loc_15_3235
loc_15_3232:
        jb     SHORT loc_15_3258
        stc
loc_15_3235:
        pushfd
        push   edx
        movzx  esi, word ptr [ebp + 38h]
        push   0
        push   esi
        push   edx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        inc    eax
        je     SHORT loc_15_3259
        dec    eax
        add    eax, dword ptr [ebp]
        mov    esi, eax
        popfd
        jmp    NEAR PTR loc_15_1D86
loc_15_3258:
        pushfd
loc_15_3259:
        xor    eax, eax
        mov    dword ptr [ebp + 18h], eax
        popfd
        ret
loc_15_3260:
        jb     SHORT loc_15_32DC
        push   edx
        movzx  esi, word ptr [ebp + 38h]
        push   0
        push   esi
        push   edx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        inc    eax
        je     SHORT loc_15_32DC
        dec    eax
        add    eax, dword ptr [ebp]
        mov    esi, eax
        call   sub_15_3F63
        jb     SHORT loc_15_32C5
        test   byte ptr [edi + 3], 80h
        je     SHORT loc_15_32C1
        test   byte ptr [edi], 4
        jne    SHORT loc_15_32C1
        call   sub_15_1C79
        call   sub_15_0314
loc_15_329A:
        add    ecx, 4
        mov    word ptr [ebp + 18h], cx
        mov    ecx, 0ffff0000h
        xchg   dword ptr [edi + 5c8h], ecx
        mov    word ptr [ebp + 10h], cx
        shr    ecx, 10h
        mov    word ptr [ebp + 1ch], cx
        movzx  cx, byte ptr [edi + 60h]
        mov    word ptr [ebp + 14h], cx
        ret
loc_15_32C1:
        xor    eax, eax
        jmp    SHORT loc_15_32D1
loc_15_32C5:
        push   esi
        db  0E8h, 06Dh, 0D2h, 0F0h, 0FFh
        pop    esi
        mov    eax, 8000h
loc_15_32D1:
        xor    ecx, ecx
        mov    word ptr [esi], ax
        mov    word ptr [esi + 2], cx
        jmp    SHORT loc_15_329A
loc_15_32DC:
        mov    word ptr [ebp + 18h], 0
        ret
loc_15_32E3:
        jb     SHORT loc_15_32EF
        call   sub_15_1D5E
        call   sub_15_024B
loc_15_32EF:
        ret
loc_15_32F0:
        jb     SHORT loc_15_3324
        inc    dword ptr [edi + 600h]
        cmp    byte ptr [edi + 9dh], 0dh
        jb     SHORT loc_15_3311
        xchg   ebx, edx
        db  0CDh, 020h, 040h, 000h, 001h, 000h
        mov    dword ptr [edi + 5ech], eax
        xchg   ebx, edx
loc_15_3311:
        lea    esi, [edi + 543h]
        call   sub_15_333F
        jb     SHORT loc_15_3323
        call   sub_15_33F9
loc_15_3323:
        ret
loc_15_3324:
        mov    word ptr [ebp + 18h], 0
        ret
loc_15_332B:
        jb     SHORT loc_15_3324
        lea    esi, [edi + 57bh]
        cmp    dword ptr [esi + 4], 0
        jne    SHORT sub_15_333F
        lea    esi, [edi + 543h]
sub_15_333F:
        movzx  eax, byte ptr [edi + 60h]
        mov    word ptr [ebp + 14h], ax
        push   edi
        mov    word ptr [ebp + 18h], 28h
        movzx  eax, word ptr [ebp + 38h]
        push   edx
        push   0
        push   eax
        push   edx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        pop    edx
        inc    eax
        je     SHORT loc_15_33C3
        dec    eax
        add    eax, dword ptr [ebp]
        lea    ecx, [edi + 0a0h]
        mov    dword ptr [eax + 24h], ecx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        movzx  ecx, word ptr [esi + 8]
        shl    ecx, 0ch
        mov    dword ptr [eax + 14h], ecx
        test   byte ptr [edi + 7ch], 1
        je     SHORT loc_15_33CC
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax + 4], ecx
        movzx  ecx, word ptr [esi + 14h]
        shl    ecx, 0ch
        mov    dword ptr [eax + 18h], ecx
        mov    ecx, dword ptr [esi + 1ch]
        mov    dword ptr [eax + 8], ecx
        movzx  ecx, word ptr [esi + 20h]
        shl    ecx, 0ch
        mov    dword ptr [eax + 1ch], ecx
        mov    ecx, dword ptr [esi + 28h]
        mov    dword ptr [eax + 0ch], ecx
        movzx  ecx, word ptr [esi + 2ch]
        shl    ecx, 0ch
        mov    dword ptr [eax + 20h], ecx
loc_15_33B6:
        push   eax
        call   sub_15_168F
        pop    edi
        mov    dword ptr [edi + 10h], eax
        pop    edi
        clc
        ret
loc_15_33C3:
        xor    eax, eax
        mov    word ptr [ebp + 18h], ax
        pop    edi
        stc
        ret
loc_15_33CC:
        push   esi
        push   edx
        push   eax
        call   sub_15_017A
        pop    eax
        mov    dword ptr [eax + 4], edx
        shl    ecx, 0ch
        mov    dword ptr [eax + 18h], ecx
        pop    edx
        pop    esi
        mov    ecx, dword ptr [esi + 1ch]
        mov    dword ptr [eax + 8], ecx
        movzx  ecx, word ptr [esi + 20h]
        shl    ecx, 0ch
        mov    dword ptr [eax + 1ch], ecx
        mov    dword ptr [eax + 20h], 0
        jmp    SHORT loc_15_33B6
sub_15_33F9:
        push   ebx
        push   edi
        mov    ebx, edx
        mov    edi, ebx
        db  003h, 03Dh, 0CCh, 006h, 000h, 000h
        bts    dword ptr [edi], 12h
        jb     SHORT loc_15_3416
        mov    eax, 10h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
loc_15_3416:
        pop    edi
        pop    ebx
        ret
loc_15_3419:
        jb     SHORT loc_15_3424
        test   byte ptr [edi], 1
        je     NEAR PTR sub_15_016E
loc_15_3424:
        ret
loc_15_3425:
        jb     SHORT loc_15_3433
        dec    dword ptr [edi + 600h]
        clc
        call   sub_15_3434
loc_15_3433:
        ret
sub_15_3434:
        jb     SHORT loc_15_3479
        mov    eax, dword ptr [edi + 600h]
        or     eax, eax
        jne    SHORT loc_15_3479
        push   edi
        mov    edi, edx
        db  003h, 03Dh, 0CCh, 006h, 000h, 000h
        btr    dword ptr [edi], 12h
        pop    edi
        jae    SHORT loc_15_345D
        xchg   ebx, edx
        mov    eax, 0fffffff0h
        db  0CDh, 020h, 01Eh, 000h, 001h, 000h
loc_15_345D:
        db  0CDh, 020h, 040h, 000h, 001h, 000h
        sub    eax, dword ptr [edi + 5ech]
        db  0CDh, 020h, 037h, 000h, 001h, 000h
        xchg   ebx, edx
        neg    eax
        db  0CDh, 020h, 037h, 000h, 001h, 000h
loc_15_3479:
        ret
        db  000h, 000h
sub_15_347C:
        mov    dword ptr [edi + 5b4h], eax
        shl    eax, 2
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_15_34B8
        mov    dword ptr [edi + 5b8h], eax
        mov    eax, dword ptr [edi + 5b4h]
        push   1
        push   eax
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        test   eax, eax
        je     SHORT loc_15_34B8
        mov    dword ptr [edi + 5bch], eax
        stc
loc_15_34B8:
        cmc
        ret
sub_15_34BA:
        push   eax
        inc    eax
        shl    eax, 2
        push   1
        push   eax
        push   dword ptr [edi + 5b8h]
        db  0CDh, 020h, 050h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_15_3506
        mov    dword ptr [edi + 5b8h], eax
        mov    eax, dword ptr [esp]
        inc    eax
        push   1
        push   eax
        push   dword ptr [edi + 5bch]
        db  0CDh, 020h, 050h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_15_3506
        mov    dword ptr [edi + 5bch], eax
        mov    eax, dword ptr [esp]
        inc    eax
        mov    dword ptr [edi + 5b4h], eax
        stc
loc_15_3506:
        cmc
        pop    eax
        ret
sub_15_3509:
        test   byte ptr [edi + 7], 80h
        je     SHORT loc_15_3525
        db  08Bh, 015h, 0B4h, 00Fh, 000h, 000h
        push   0
        push   2
        push   edx
        push   ebx
        push   edx
        db  0CDh, 020h, 05Eh, 000h, 001h, 000h
        add    esp, 14h
loc_15_3525:
        test   byte ptr [edi + 4], 20h
        je     SHORT loc_15_3540
        mov    edx, 0b0h
        push   0
        push   8
        push   edx
        push   ebx
        push   edx
        db  0CDh, 020h, 05Eh, 000h, 001h, 000h
        add    esp, 14h
loc_15_3540:
        clc
        ret
sub_15_3542:
        push   edx
        push   ecx
        push   eax
        mov    ecx, dword ptr [edi + 5bch]
        test   byte ptr [eax + ecx], 2
        je     SHORT loc_15_355A
        mov    ecx, eax
        call   sub_15_3D8B
        mov    eax, ecx
loc_15_355A:
        push   ebx
        call   sub_15_2D54
        mov    ecx, ebx
        pop    ebx
        jb     SHORT loc_15_3586
        cmp    ebx, ecx
        jne    SHORT loc_15_357F
        movzx  ecx, word ptr [eax + 4]
        cmp    ecx, dword ptr [esp]
        je     SHORT loc_15_35CF
        xchg   dword ptr [esp], eax
        call   sub_15_2907
        xchg   dword ptr [esp], eax
        je     SHORT loc_15_35A5
loc_15_357F:
        call   sub_15_2D96
        jb     SHORT loc_15_35DA
loc_15_3586:
        mov    eax, dword ptr [esp]
        mov    esi, dword ptr [edi + 5b8h]
        cmp    dword ptr [esi + eax*4], 0
        je     SHORT loc_15_35A5
        mov    ecx, eax
        mov    eax, dword ptr [esi + eax*4]
        xor    esi, esi
        call   sub_15_2D10
        jae    SHORT loc_15_35B6
        jmp    SHORT loc_15_35DA
loc_15_35A5:
        mov    eax, dword ptr [esp]
        mov    ecx, eax
        db  0BEh, 07Eh, 03Eh, 000h, 000h
        call   sub_15_2B10
        jb     SHORT loc_15_35DA
loc_15_35B6:
        mov    esi, dword ptr [edi + 5b8h]
        mov    dword ptr [esi + ecx*4], eax
        mov    esi, dword ptr [edi + 5bch]
        add    esi, ecx
        and    byte ptr [esi], 1
        call   sub_15_4177
loc_15_35CF:
        mov    esi, eax
        call   sub_15_2CA7
loc_15_35D6:
        pop    eax
        pop    ecx
        pop    edx
        ret
loc_15_35DA:
        db  0E8h, 02Bh, 0CFh, 0F0h, 0FFh
        jmp    SHORT loc_15_35D6
sub_15_35E1:
        push   ecx
        lea    edx, [ecx + eax]
        cmp    dword ptr [edi + 5b4h], edx
        jae    SHORT loc_15_35F7
        push   eax
        lea    eax, [edx - 1]
        call   sub_15_34BA
        pop    eax
loc_15_35F7:
        pop    ecx
        ret
sub_15_35F9:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        call   sub_15_49DE
        call   sub_15_3A1F
        call   sub_15_10CC
        call   sub_15_29F4
        pushfd
        cmp    ecx, 20h
        jbe    SHORT loc_15_3620
        mov    ecx, 20h
loc_15_3620:
        call   sub_15_35E1
        jae    SHORT loc_15_3636
        pushad
        db  0E8h, 00Bh, 0CFh, 0F0h, 0FFh
        popad
        mov    ecx, dword ptr [edi + 5b4h]
        sub    ecx, eax
loc_15_3636:
        mov    byte ptr [edi + 606h], al
        mov    word ptr [edi + 607h], cx
loc_15_3643:
        call   sub_15_3542
        inc    eax
        loop   loc_15_3643
        popfd
        jb     SHORT loc_15_366E
        movzx  ecx, byte ptr [edi + 4a6h]
        xor    eax, eax
loc_15_3657:
        jecxz  loc_15_366E
        shr    ecx, 1
        jae    SHORT loc_15_3669
        call   sub_15_3542
        inc    eax
        call   sub_15_3542
        dec    eax
loc_15_3669:
        add    eax, 2
        jmp    SHORT loc_15_3657
loc_15_366E:
        call   sub_15_2C54
        db  0E8h, 050h, 0CAh, 0F0h, 0FFh
        call   sub_15_4A18
        call   sub_15_10EA
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    SHORT loc_15_3694
        call   sub_15_119E
        call   sub_15_4B2C
loc_15_3694:
        ret
sub_15_3695:
        push   esi
        lea    esi, [esi]
        mov    ecx, 4
loc_15_369D:
        push   ecx
        xor    eax, eax
        mov    dword ptr [esi + 4], eax
        mov    word ptr [esi + 8], ax
        mov    word ptr [esi + 0ah], ax
        xchg   dword ptr [esi], eax
        or     eax, eax
        je     SHORT loc_15_36BD
        push   0
        push   eax
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_15_36BD:
        add    esi, 0ch
        pop    ecx
        loop   loc_15_369D
        pop    esi
        ret
sub_15_36C5:
        push   esi
        lea    esi, [edi + 543h]
        call   sub_15_3695
        pop    esi
        ret
sub_15_36D3:
        pushad
        call   sub_15_29F4
        add    eax, ecx
        cmp    byte ptr [edi + 5b3h], 58h
        je     SHORT loc_15_3720
        xor    ebx, ebx
        xor    ecx, ecx
        mov    dl, byte ptr [edi + 7ch]
        test   byte ptr [edi + 23h], 1
        je     SHORT loc_15_36FF
        and    dl, 3
        cmp    dl, 3
        je     SHORT loc_15_3727
        test   byte ptr [edi + 6ah], 4
        jne    SHORT loc_15_3720
loc_15_36FF:
        test   dl, 1
        jne    SHORT loc_15_3729
        mov    ax, 8
        or     dh, dh
        jne    SHORT loc_15_3729
        movzx  ebx, byte ptr [edi + 4a6h]
loc_15_3713:
        adc    ecx, 0
        shr    ebx, 1
        jb     SHORT loc_15_3713
        jne    SHORT loc_15_3713
        shl    ecx, 1
        jmp    SHORT loc_15_3729
loc_15_3720:
        mov    ebx, eax
        mov    ecx, eax
        push   eax
        jmp    SHORT loc_15_372B
loc_15_3727:
        mov    ecx, eax
loc_15_3729:
        push   0
loc_15_372B:
        lea    ebp, [esi + 8]
        call   sub_15_3752
        mov    eax, ebx
        lea    ebp, [esi + 14h]
        call   sub_15_3752
        mov    eax, ecx
        lea    ebp, [esi + 20h]
        call   sub_15_3752
        pop    eax
        lea    ebp, [esi + 2ch]
        call   sub_15_3752
        popad
        ret
sub_15_3752:
        test   byte ptr [edi + 5e2h], 80h
        jne    SHORT loc_15_3760
        test   byte ptr [edi], 8
        je     SHORT loc_15_3766
loc_15_3760:
        cmp    ax, word ptr [ebp]
        jbe    SHORT loc_15_376A
loc_15_3766:
        mov    word ptr [ebp], ax
loc_15_376A:
        ret
sub_15_376B:
        push   ecx
        movzx  eax, word ptr [esi + 8]
        mov    ecx, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_15_37AB
        jecxz  loc_15_378B
        db  0FFh, 035h, 0B8h, 00Fh, 000h, 000h
        push   eax
        push   ecx
        db  0CDh, 020h, 054h, 000h, 001h, 000h
        add    esp, 0ch
        jmp    SHORT loc_15_37A4
loc_15_378B:
        xor    ecx, ecx
        db  0FFh, 035h, 0B8h, 00Fh, 000h, 000h
        push   ecx
        push   ecx
        push   ecx
        push   ecx
        push   ebx
        push   7
        push   eax
        db  0CDh, 020h, 053h, 000h, 001h, 000h
        add    esp, 20h
loc_15_37A4:
        or     eax, eax
        jne    SHORT loc_15_37BD
        stc
        jmp    SHORT loc_15_37CB
loc_15_37AB:
        jecxz  loc_15_37B9
        push   0
        push   ecx
        db  0CDh, 020h, 055h, 000h, 001h, 000h
        add    esp, 8
loc_15_37B9:
        xor    eax, eax
        xor    edx, edx
loc_15_37BD:
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], edx
        movzx  edx, word ptr [esi + 8]
        mov    word ptr [esi + 0ah], dx
        clc
loc_15_37CB:
        pop    ecx
        ret
sub_15_37CD:
        pushad
        test   byte ptr [edi + 4], 1
        je     SHORT loc_15_37DC
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_15_3849
loc_15_37DC:
        and    byte ptr [edi + 9], 0feh
        lea    esi, [esi]
        mov    ecx, 4
loc_15_37E7:
        movzx  eax, word ptr [esi + 0ah]
        cmp    ax, word ptr [esi + 8]
        je     SHORT loc_15_3843
        jb     SHORT loc_15_37FC
        test   byte ptr [edi + 5e2h], 80h
        jne    SHORT loc_15_3843
loc_15_37FC:
        push   eax
        cmp    byte ptr [edi + 605h], 0
        je     SHORT loc_15_380D
        push   ecx
        call   sub_15_3A50
        pop    ecx
loc_15_380D:
        call   sub_15_376B
        pop    eax
        jb     SHORT loc_15_384B
        cmp    cl, 4
        jne    SHORT loc_15_3843
        test   byte ptr [edi + 7ch], 1
        jne    SHORT loc_15_3843
        cmp    ax, word ptr [esi + 0ah]
        jae    SHORT loc_15_3843
        push   ecx
        push   edi
        mov    edi, dword ptr [esi + 4]
        movzx  ecx, word ptr [esi + 0ah]
        sub    ecx, eax
        shl    ecx, 0ah
        shl    eax, 0ch
        add    edi, eax
        mov    eax, 7200720h
        cld
        rep stosd
        pop    edi
        pop    ecx
loc_15_3843:
        add    esi, 0ch
        loop   loc_15_37E7
        clc
loc_15_3849:
        popad
        ret
loc_15_384B:
        or     byte ptr [edi + 9], 1
        stc
        jmp    SHORT loc_15_3849
sub_15_3852:
        test   byte ptr [edi + 7ch], 1
        jne    SHORT loc_15_3863
        push   eax
        shr    eax, 1
        bt     dword ptr [edi + 4a6h], eax
        pop    eax
loc_15_3863:
        ret
sub_15_3864:
        pushad
        test   byte ptr [edi + 2], 40h
        jne    SHORT loc_15_387D
        lea    esi, [edi + 543h]
        xor    edx, edx
        call   sub_15_36D3
        call   sub_15_37CD
loc_15_387D:
        db  03Bh, 01Dh, 0A4h, 007h, 000h, 000h
        jne    SHORT loc_15_388F
        db  0E8h, 03Eh, 0C8h, 0F0h, 0FFh
        call   sub_15_4A18
loc_15_388F:
        xor    ebp, ebp
        call   sub_15_29F4
        jb     SHORT loc_15_389F
        movzx  ebp, byte ptr [edi + 4a6h]
loc_15_389F:
        call   sub_15_35E1
        jae    SHORT loc_15_38AE
        mov    ecx, dword ptr [edi + 5b4h]
        sub    ecx, eax
loc_15_38AE:
        lea    edx, [ecx + eax - 1]
        push   edi
        mov    esi, dword ptr [edi + 5b8h]
        mov    ecx, dword ptr [edi + 5bch]
        push   dword ptr [edi + 5b4h]
        xor    edi, edi
loc_15_38C7:
        push   eax
        mov    eax, edi
        shr    eax, 1
        bt     ebp, eax
        pop    eax
        jb     SHORT loc_15_38DA
        cmp    edi, eax
        jb     SHORT loc_15_38F3
        cmp    edx, edi
        jb     SHORT loc_15_38F3
loc_15_38DA:
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        je     SHORT loc_15_3933
        push   edi
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        call   sub_15_3A9D
        pop    edi
        jb     SHORT loc_15_3933
loc_15_38F3:
        push   eax
        pushfd
        mov    eax, dword ptr [esi + edi*4]
        or     eax, eax
        je     SHORT loc_15_3931
        popfd
        pushfd
        jae    SHORT loc_15_3927
        and    byte ptr [edi + ecx], 0feh
        pushad
        xor    ecx, ecx
        xchg   word ptr [eax + 6], cx
        jecxz  loc_15_3922
        xor    eax, eax
        push   eax
        push   7
        push   eax
        push   -8
        push   1
        push   ecx
        push   ebx
        db  0CDh, 020h, 060h, 000h, 001h, 000h
        add    esp, 1ch
loc_15_3922:
        popad
        and    byte ptr [eax + 3], 0bfh
loc_15_3927:
        call   sub_15_10CC
        call   sub_15_2D96
loc_15_3931:
        popfd
        pop    eax
loc_15_3933:
        inc    edi
        cmp    edi, dword ptr [esp]
        jb     SHORT loc_15_38C7
        add    esp, 4
        pop    edi
        call   sub_15_3E44
        call   sub_15_024B
        call   sub_15_10EA
        popad
        ret
sub_15_394E:
        test   byte ptr [edi + 2], 40h
        jne    SHORT loc_15_3968
        lea    esi, [edi + 543h]
        xor    edx, edx
        call   sub_15_36D3
        call   sub_15_37CD
        jmp    SHORT loc_15_396D
loc_15_3968:
        call   sub_15_36C5
loc_15_396D:
        ret
sub_15_396E:
        push   eax
        push   ecx
        call   sub_15_29F4
        mov    byte ptr [edi + 606h], al
        mov    word ptr [edi + 607h], cx
        push   edx
        lea    edx, [ecx + eax]
        cmp    edx, dword ptr [edi + 5b4h]
        pop    edx
        jbe    SHORT loc_15_3997
        mov    ecx, dword ptr [edi + 5b4h]
        sub    ecx, eax
loc_15_3997:
        call   sub_15_3542
        inc    eax
        loop   loc_15_3997
        dec    eax
        call   sub_15_39AD
        db  0E8h, 01Eh, 0C7h, 0F0h, 0FFh
        pop    ecx
        pop    eax
        ret
sub_15_39AD:
        pushad
        test   byte ptr [edi + 2], 40h
        jne    SHORT loc_15_3A1A
        mov    ecx, eax
        inc    ecx
        test   byte ptr [edi + 23h], 1
        je     SHORT loc_15_39D5
        mov    dl, byte ptr [edi + 7ch]
        and    dl, 3
        cmp    dl, 3
        je     SHORT loc_15_39E7
        mov    dl, byte ptr [edi + 6ah]
        test   dl, 4
        je     SHORT loc_15_39D5
        test   dl, 8
        je     SHORT loc_15_39EB
loc_15_39D5:
        cmp    cx, word ptr [edi + 54bh]
        jbe    SHORT loc_15_3A1C
        mov    word ptr [edi + 54bh], cx
        jmp    SHORT loc_15_3A0F
loc_15_39E7:
        mov    dl, 5
        jmp    SHORT loc_15_39EB
loc_15_39EB:
        xor    dh, dh
        mov    eax, ecx
        lea    esi, [edi + 543h]
        mov    ecx, 4
loc_15_39FA:
        cmp    ax, word ptr [esi + 8]
        jbe    SHORT loc_15_3A06
        mov    word ptr [esi + 8], ax
        inc    dh
loc_15_3A06:
        add    esi, 0ch
        loop   loc_15_39FA
        or     dh, dh
        je     SHORT loc_15_3A1C
loc_15_3A0F:
        lea    esi, [edi + 543h]
        call   sub_15_37CD
loc_15_3A1A:
        popad
        ret
loc_15_3A1C:
        clc
        jmp    SHORT loc_15_3A1A
sub_15_3A1F:
        push   eax
        db  0A1h, 0ACh, 007h, 000h, 000h
        cmp    eax, ebx
        je     SHORT loc_15_3A48
        or     eax, eax
        je     SHORT loc_15_3A46
loc_15_3A2D:
        db  03Bh, 005h, 0B8h, 007h, 000h, 000h
        je     SHORT loc_15_3A46
        pushad
        mov    ebx, eax
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        call   sub_15_3A50
        popad
loc_15_3A46:
        pop    eax
        ret
loc_15_3A48:
        test   byte ptr [edi + 1], 40h
        je     SHORT loc_15_3A46
        jmp    SHORT loc_15_3A2D
sub_15_3A50:
        db  0F6h, 005h, 0C4h, 006h, 000h, 000h, 001h
        je     SHORT loc_15_3A9C
        or     ebx, ebx
        je     SHORT loc_15_3A9C
        push   ebp
        push   esi
        mov    esi, dword ptr [edi + 5b8h]
        mov    ebp, dword ptr [edi + 5bch]
        call   sub_15_2E25
        xor    ecx, ecx
loc_15_3A72:
        test   byte ptr [ebp + ecx], 2
        je     SHORT loc_15_3A80
        call   sub_15_3D8B
        jmp    SHORT loc_15_3A8C
loc_15_3A80:
        mov    eax, dword ptr [esi + ecx*4]
        or     eax, eax
        je     SHORT loc_15_3A8C
        call   sub_15_3EDC
loc_15_3A8C:
        inc    ecx
        cmp    ecx, dword ptr [edi + 5b4h]
        jb     SHORT loc_15_3A72
        pop    esi
        pop    ebp
        jmp    NEAR PTR sub_15_3E44
loc_15_3A9C:
        ret
sub_15_3A9D:
        test   byte ptr [edi + 6ah], 4
        je     SHORT loc_15_3AAF
        test   byte ptr [edi + 23h], 1
        je     SHORT loc_15_3AAF
        test   byte ptr [edi + 6ah], 8
        je     SHORT loc_15_3AB9
loc_15_3AAF:
        push   eax
        mov    al, byte ptr [edi + 7bh]
        and    al, 3
        pop    eax
        jne    SHORT loc_15_3AB9
        ret
loc_15_3AB9:
        stc
        ret
loc_15_3ABB:
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        db  083h, 03Dh, 0F4h, 00Eh, 000h, 000h, 000h
        je     SHORT loc_15_3AEE
        db  03Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        je     SHORT loc_15_3AEE
        db  08Bh, 035h, 0F4h, 00Eh, 000h, 000h
loc_15_3ADA:
        cmp    byte ptr [esi], 0
        je     SHORT loc_15_3AEE
        push   eax
        mov    eax, 64h
        db  0CDh, 020h, 061h, 001h, 001h, 000h
        pop    eax
        jmp    SHORT loc_15_3ADA
loc_15_3AEE:
        mov    ecx, 6
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        push   eax
        or     byte ptr [edi + 3], 0ch
        db  0F6h, 005h, 0A0h, 007h, 000h, 000h, 003h
        jne    NEAR PTR loc_15_3C91
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_3B20
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        jne    SHORT loc_15_3B20
        call   sub_15_1969
loc_15_3B20:
        call   sub_15_2AE7
        jb     NEAR PTR loc_15_3C91
        cmp    eax, dword ptr [edi + 5b4h]
        jb     SHORT loc_15_3B3E
        call   sub_15_34BA
        jb     NEAR PTR loc_15_3C91
loc_15_3B3E:
        test   byte ptr [edi], 20h
        jne    SHORT loc_15_3B5E
        mov    cl, byte ptr [edi + 15h]
        and    cl, 1fh
        cmp    cl, 2
        jbe    SHORT loc_15_3B69
        mov    cl, byte ptr [edi + 7ch]
        test   cl, 1
        jne    SHORT loc_15_3B5B
        test   cl, 2
        jne    SHORT loc_15_3B69
loc_15_3B5B:
        or     byte ptr [edi], 40h
loc_15_3B5E:
        mov    ecx, eax
        shr    ecx, 1
        bts    dword ptr [edi + 4a6h], ecx
loc_15_3B69:
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        je     SHORT loc_15_3B9F
        test   byte ptr [edi], 80h
        jne    NEAR PTR loc_15_3C91
        test   byte ptr [edi], 60h
        jne    SHORT loc_15_3B86
        call   sub_15_3A9D
        jb     SHORT loc_15_3B9F
loc_15_3B86:
        mov    ecx, dword ptr [esp]
        call   sub_15_3CA8
        jb     SHORT loc_15_3B96
        pop    eax
        jmp    NEAR PTR loc_15_3C9C
loc_15_3B96:
        cmp    eax, -1
        je     NEAR PTR loc_15_3C91
loc_15_3B9F:
        mov    esi, dword ptr [edi + 5b8h]
        lea    esi, [esi + eax*4]
        mov    ecx, dword ptr [esi]
        or     ecx, ecx
        jne    SHORT loc_15_3BBA
        test   byte ptr [edi], 2
        jne    SHORT loc_15_3BD6
        call   sub_15_39AD
        jmp    SHORT loc_15_3BD6
loc_15_3BBA:
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_3C03
        call   sub_15_29DC
        je     SHORT loc_15_3C03
        xchg   ecx, eax
        call   sub_15_10CC
        call   sub_15_2D96
        mov    eax, ecx
loc_15_3BD6:
        call   sub_15_10CC
        push   eax
        push   esi
        db  0BEh, 07Eh, 03Eh, 000h, 000h
        call   sub_15_2B10
        pop    esi
        mov    ecx, eax
        pop    eax
        jb     SHORT loc_15_3C63
        mov    dword ptr [esi], ecx
        add    eax, dword ptr [edi + 5bch]
        and    byte ptr [eax], 1
        mov    esi, eax
        mov    eax, ecx
        call   sub_15_4177
        mov    ecx, eax
loc_15_3C03:
        db  03Bh, 01Dh, 0A8h, 007h, 000h, 000h
        jne    SHORT loc_15_3C2D
        test   byte ptr [edi], 2
        jne    SHORT loc_15_3C2D
        push   ecx
        call   sub_15_29F4
        cmp    cx, word ptr [edi + 607h]
        pop    ecx
        jne    SHORT loc_15_3C28
        cmp    al, byte ptr [edi + 606h]
        je     SHORT loc_15_3C2D
loc_15_3C28:
        call   sub_15_396E
loc_15_3C2D:
        mov    eax, ecx
        call   sub_15_10EA
        mov    edx, 1
        call   sub_15_2E3E
        mov    esi, eax
        cmp    word ptr [esi + 6], 0
        je     SHORT loc_15_3C4C
        call   sub_15_3EDC
loc_15_3C4C:
        pop    eax
        mov    word ptr [esi + 6], ax
        push   0
        push   1
        push   eax
        push   ebx
        push   ecx
        db  0CDh, 020h, 05Eh, 000h, 001h, 000h
        add    esp, 14h
        jmp    SHORT loc_15_3C9C
loc_15_3C63:
        call   sub_15_10EA
        test   byte ptr [edi + 2], 2
        je     SHORT loc_15_3C7B
        test   byte ptr [edi + 4], 10h
        je     SHORT loc_15_3C91
        call   sub_15_498E
        jmp    SHORT loc_15_3C91
loc_15_3C7B:
        test   byte ptr [edi], 2
        jne    SHORT loc_15_3C91
        test   byte ptr [edi + 4], 40h
        jne    SHORT loc_15_3C91
        db  0BEh, 0EEh, 004h, 000h, 000h
        db  0CDh, 020h, 00Fh, 000h, 001h, 000h
loc_15_3C91:
        call   sub_15_10EA
        pop    eax
        call   sub_15_3E18
loc_15_3C9C:
        and    dword ptr [edi], 0f7ffffbfh
        db  0CDh, 020h, 020h, 080h, 001h, 000h
sub_15_3CA8:
        pushad
        lea    esi, [edi + 543h]
        test   byte ptr [edi], 60h
        je     SHORT loc_15_3CC9
        add    esi, 18h
        cmp    ax, word ptr [esi + 8]
        jb     SHORT loc_15_3D0C
        mov    ecx, eax
        or     cl, 1
        inc    ecx
        mov    word ptr [esi + 8], cx
        jmp    SHORT loc_15_3D12
loc_15_3CC9:
        cmp    ax, word ptr [esi + 8]
        jb     SHORT loc_15_3D0C
        test   byte ptr [edi], 2
        je     SHORT loc_15_3CE0
loc_15_3CD4:
        popad
        mov    eax, 0ffffffffh
        stc
        jmp    NEAR PTR loc_15_3D8A
loc_15_3CE0:
        push   ecx
        db  08Bh, 00Dh, 0D2h, 00Eh, 000h, 000h
        db  03Bh, 00Dh, 0CCh, 00Eh, 000h, 000h
        pop    ecx
        jb     SHORT loc_15_3CF7
loc_15_3CF0:
        stc
        popad
        jmp    NEAR PTR loc_15_3D8A
loc_15_3CF7:
        pushad
        inc    ax
        mov    word ptr [esi + 8], ax
        lea    esi, [edi + 543h]
        call   sub_15_37CD
        popad
        jb     SHORT loc_15_3CD4
loc_15_3D0C:
        cmp    ax, word ptr [esi + 0ah]
        jb     SHORT loc_15_3D27
loc_15_3D12:
        push   eax
        call   sub_15_376B
        pop    eax
        jae    SHORT loc_15_3D27
        test   byte ptr [edi], 60h
        je     SHORT loc_15_3CD4
        db  0E8h, 0C9h, 0C7h, 0F0h, 0FFh
        jmp    SHORT loc_15_3CF0
loc_15_3D27:
        push   0
        push   eax
        push   1
        push   dword ptr [esi]
        db  0CDh, 020h, 056h, 000h, 001h, 000h
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_15_3CD4
        mov    eax, dword ptr [esp + 1ch]
        mov    ecx, dword ptr [esp + 18h]
        push   0
        push   eax
        push   1
        push   ecx
        push   ebx
        push   dword ptr [esi]
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        inc    byte ptr [edi + 605h]
        mov    esi, dword ptr [edi + 5b8h]
        mov    ecx, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esi + ecx*4]
        or     eax, eax
        je     SHORT loc_15_3D71
        call   sub_15_2D96
loc_15_3D71:
        mov    esi, dword ptr [edi + 5bch]
        mov    eax, dword ptr [esp + 1ch]
        or     byte ptr [esi + eax], 2
        test   byte ptr [edi], 60h
        je     SHORT loc_15_3D88
        or     byte ptr [esi + eax], 4
loc_15_3D88:
        clc
        popad
loc_15_3D8A:
        ret
sub_15_3D8B:
        push   esi
        mov    eax, ecx
        push   eax
        call   sub_15_2B01
        push   eax
        mov    ecx, eax
        shl    ecx, 0ch
        add    ecx, dword ptr [ebx + 4]
        shr    ecx, 0ch
        sub    esp, 4
        mov    eax, esp
        push   0
        push   eax
        push   1
        push   ecx
        db  0CDh, 020h, 061h, 000h, 001h, 000h
        add    esp, 10h
        pop    ecx
        test   cl, 40h
        pop    ecx
        mov    eax, dword ptr [esp]
        mov    esi, dword ptr [edi + 5bch]
        je     SHORT loc_15_3DCD
        or     byte ptr [esi + eax], 1
        call   sub_15_456A
loc_15_3DCD:
        and    byte ptr [esi + eax], 0fdh
        dec    byte ptr [edi + 605h]
        push   0
        push   7
        push   0
        push   -8
        push   1
        push   ecx
        push   ebx
        db  0CDh, 020h, 060h, 000h, 001h, 000h
        add    esp, 1ch
        pop    ecx
        btr    dword ptr [esi + ecx], 2
        lea    esi, [edi + 543h]
        jae    SHORT loc_15_3E00
        lea    esi, [edi + 55bh]
loc_15_3E00:
        cmp    cx, word ptr [esi + 0ah]
        jae    SHORT loc_15_3E16
        push   0
        push   ecx
        push   1
        push   dword ptr [esi]
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
loc_15_3E16:
        pop    esi
        ret
sub_15_3E18:
        push   edx
        push   eax
        mov    edx, eax
        sub    edx, 0a0h
        bts    dword ptr [edi + 5d8h], edx
        db  0CDh, 020h, 06Ah, 000h, 001h, 000h
        pop    edx
        push   0
        push   0
        push   1
        push   edx
        push   ebx
        push   eax
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h
        add    esp, 18h
        pop    edx
        ret
sub_15_3E44:
        mov    eax, dword ptr [edi + 5d8h]
        or     eax, eax
        je     SHORT loc_15_3E7D
        pushad
        mov    ebp, eax
        mov    esi, 0a0h
loc_15_3E56:
        shr    ebp, 1
        jae    SHORT loc_15_3E71
        xor    eax, eax
        push   eax
        push   7
        push   eax
        push   -8
        push   1
        push   esi
        push   ebx
        db  0CDh, 020h, 060h, 000h, 001h, 000h
        add    esp, 1ch
        or     al, 1
loc_15_3E71:
        je     SHORT loc_15_3E76
        inc    esi
        jmp    SHORT loc_15_3E56
loc_15_3E76:
        mov    dword ptr [edi + 5d8h], ebp
        popad
loc_15_3E7D:
        ret
loc_15_3E7E:
        jecxz  loc_15_3E8B
        cmp    ecx, 2
        je     SHORT loc_15_3E89
        jb     SHORT loc_15_3ED5
        clc
        ret
loc_15_3E89:
        stc
        ret
loc_15_3E8B:
        xor    edx, edx
        call   sub_15_3F3C
        je     SHORT loc_15_3EA6
        call   sub_15_4009
        mov    dl, 1
        push   eax
        movzx  eax, word ptr [eax + 4]
        call   sub_15_456A
        pop    eax
loc_15_3EA6:
        movzx  ecx, word ptr [eax + 4]
        mov    esi, dword ptr [edi + 5b8h]
        mov    dword ptr [esi + ecx*4], 0
        add    ecx, dword ptr [edi + 5bch]
        and    byte ptr [ecx], 1
        or     byte ptr [ecx], dl
        push   eax
        db  0A1h, 008h, 00Fh, 000h, 000h
        or     eax, eax
        je     SHORT loc_15_3ED4
        test   byte ptr [eax], 2
        je     SHORT loc_15_3ED4
        or     byte ptr [eax], 4
loc_15_3ED4:
        pop    eax
loc_15_3ED5:
        call   sub_15_3EDC
loc_15_3EDA:
        clc
        ret
sub_15_3EDC:
        push   ecx
        movzx  ecx, word ptr [eax + 6]
        jecxz  loc_15_3F17
        call   sub_15_3F3C
        jb     SHORT loc_15_3F17
        je     SHORT loc_15_3EF7
        push   eax
        movzx  eax, word ptr [eax + 4]
        call   sub_15_456A
        pop    eax
loc_15_3EF7:
        xor    ecx, ecx
        xchg   word ptr [eax + 6], cx
        call   sub_15_2EBF
        push   0
        push   7
        push   0
        push   -8
        push   1
        push   ecx
        push   ebx
        db  0CDh, 020h, 060h, 000h, 001h, 000h
        add    esp, 1ch
loc_15_3F17:
        pop    ecx
        ret
sub_15_3F19:
        push   edx
        push   eax
        shl    ecx, 0ch
        add    ecx, dword ptr [ebx + 4]
        shr    ecx, 0ch
        sub    esp, 4
        mov    eax, esp
        push   0
        push   eax
        push   1
        push   ecx
        db  0CDh, 020h, 061h, 000h, 001h, 000h
        add    esp, 10h
        pop    ecx
        pop    eax
        pop    edx
        ret
sub_15_3F3C:
        push   ecx
        movzx  ecx, word ptr [eax + 6]
        jecxz  loc_15_3F5C
        test   byte ptr [eax + 3], 40h
        jne    SHORT loc_15_3F61
        call   sub_15_3F19
        and    cl, 40h
        je     SHORT loc_15_3F56
        or     byte ptr [eax + 3], cl
loc_15_3F56:
        test   byte ptr [eax + 3], 40h
        pop    ecx
        ret
loc_15_3F5C:
        test   byte ptr [eax + 3], 40h
        stc
loc_15_3F61:
        pop    ecx
        ret
sub_15_3F63:
        test   byte ptr [edi + 9], 1
        je     SHORT loc_15_3F90
        pushad
        lea    esi, [edi + 543h]
        mov    ecx, 4
loc_15_3F75:
        movzx  eax, word ptr [esi + 8]
        cmp    ax, word ptr [esi + 0ah]
        jbe    SHORT loc_15_3F86
        call   sub_15_376B
        jb     SHORT loc_15_3F8F
loc_15_3F86:
        add    esi, 0ch
        loop   loc_15_3F75
        and    byte ptr [edi + 9], 0feh
loc_15_3F8F:
        popad
loc_15_3F90:
        ret
sub_15_3F91:
        push   eax
        push   edx
        mov    dx, 3ceh
        cmp    byte ptr [edi + 77h], 0
        je     SHORT loc_15_3FA7
        mov    eax, 1
        call   sub_15_3FF1
loc_15_3FA7:
        cmp    byte ptr [edi + 79h], 0
        je     SHORT loc_15_3FB7
        mov    eax, 3
        call   sub_15_3FF1
loc_15_3FB7:
        mov    al, byte ptr [edi + 7bh]
        and    al, 0bh
        je     SHORT loc_15_3FC8
        mov    eax, 0f405h
        call   sub_15_3FF1
loc_15_3FC8:
        cmp    byte ptr [edi + 7eh], 0ffh
        jne    SHORT loc_15_3FD8
        mov    eax, 0ff0008h
        call   sub_15_3FF1
loc_15_3FD8:
        mov    dl, 0c4h
        mov    al, byte ptr [esp]
        cmp    al, byte ptr [edi + 68h]
        je     SHORT loc_15_3FEE
        shl    eax, 10h
        mov    ax, 2
        call   sub_15_3FF1
loc_15_3FEE:
        pop    edx
        pop    eax
        ret
sub_15_3FF1:
        out    dx, al
        jmp    SHORT loc_15_3FF4
loc_15_3FF4:
        inc    edx
        in     al, dx
        jmp    SHORT loc_15_3FF8
loc_15_3FF8:
        and    ah, al
        shr    eax, 8
        or     al, ah
        out    dx, al
        jmp    SHORT loc_15_4002
loc_15_4002:
        dec    edx
        call   sub_15_1193
        ret
sub_15_4009:
        mov    ecx, 6
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        and    byte ptr [eax + 3], 0bfh
        call   sub_15_3F63
        db  03Bh, 01Dh, 0ACh, 007h, 000h, 000h
        jne    SHORT loc_15_402E
        push   ebx
        xor    ebx, ebx
        call   sub_15_1143
        pop    ebx
loc_15_402E:
        call   sub_15_119E
        test   byte ptr [edi + 23h], 1
        je     SHORT loc_15_4059
        mov    dl, byte ptr [edi + 7ch]
        and    dl, 3
        cmp    dl, 3
        je     SHORT loc_15_40BF
        mov    dl, byte ptr [edi + 6ah]
        test   dl, 8
        jne    NEAR PTR loc_15_40FD
        test   dl, 4
        jne    NEAR PTR loc_15_40F8
loc_15_4059:
        xor    edx, edx
        call   sub_15_2E3E
        mov    dl, 3
        call   sub_15_3F91
        lea    esi, [edi + 543h]
        call   sub_15_43A3
        test   byte ptr [edi + 7ch], 1
        jne    NEAR PTR loc_15_416F
        push   eax
        call   sub_15_18BA
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0ch], 0
        je     SHORT loc_15_4091
        call   dword ptr [edi + 0ch]
loc_15_4091:
        pop    edi
        pop    eax
        xor    edx, edx
        push   ebx
        xor    ebx, ebx
        call   sub_15_2E3E
        pop    ebx
        mov    dx, 402h
        call   sub_15_4324
        lea    esi, [edi + 55bh]
        call   sub_15_4580
        call   sub_15_119E
        or     cl, 0ffh
        jmp    NEAR PTR loc_15_416F
loc_15_40BF:
        xor    edx, edx
        call   sub_15_2E3E
        call   sub_15_1193
        mov    dl, 3
        call   sub_15_3F91
        xor    edx, edx
        call   sub_15_4324
        lea    esi, [edi + 543h]
        call   sub_15_43A3
        mov    dl, 2
        call   sub_15_4324
        lea    esi, [edi + 55bh]
        call   sub_15_43A3
        jmp    SHORT loc_15_416F
loc_15_40F8:
        test   dl, 8
        je     SHORT loc_15_411B
loc_15_40FD:
        xor    edx, edx
        call   sub_15_2E3E
        mov    dl, 0fh
        call   sub_15_3F91
        lea    esi, [edi + 543h]
        call   sub_15_43A3
        or     cl, 0ffh
        jmp    SHORT loc_15_416F
loc_15_411B:
        push   eax
        call   sub_15_18BA
        pop    eax
        xor    edx, edx
        push   ebx
        xor    ebx, ebx
        call   sub_15_2E3E
        pop    ebx
        lea    esi, [edi + 543h]
        xor    edx, edx
loc_15_4135:
        call   sub_15_4324
        cmp    word ptr [esi + 8], 0
        jne    SHORT loc_15_4158
        cmp    dl, 1
        jne    SHORT loc_15_4158
        test   byte ptr [edi], 60h
        je     SHORT loc_15_4158
        sub    esi, 0ch
        call   sub_15_43E3
        add    esi, 18h
        jmp    SHORT loc_15_4160
loc_15_4158:
        call   sub_15_43A3
        add    esi, 0ch
loc_15_4160:
        inc    dl
        cmp    dl, 4
        jb     SHORT loc_15_4135
        call   sub_15_119E
        or     cl, 0ffh
loc_15_416F:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        clc
        ret
sub_15_4177:
        mov    ecx, 6
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        test   byte ptr [esi], 1
        je     SHORT loc_15_41A4
        db  0BAh, 048h, 044h, 000h, 000h
        db  089h, 015h, 0BCh, 00Fh, 000h, 000h
        db  089h, 015h, 0C0h, 00Fh, 000h, 000h
        db  0C7h, 005h, 0C4h, 00Fh, 000h, 000h, 006h, 046h, 000h, 000h
        jmp    SHORT loc_15_41C2
loc_15_41A4:
        db  0C7h, 005h, 0BCh, 00Fh, 000h, 000h, 088h, 044h, 000h, 000h
        db  0C7h, 005h, 0C0h, 00Fh, 000h, 000h, 0B6h, 044h, 000h, 000h
        db  0C7h, 005h, 0C4h, 00Fh, 000h, 000h, 088h, 044h, 000h, 000h
loc_15_41C2:
        call   sub_15_119E
        test   byte ptr [edi + 23h], 1
        je     SHORT loc_15_41F1
        mov    dl, byte ptr [edi + 7ch]
        and    dl, 3
        cmp    dl, 3
        je     NEAR PTR loc_15_426B
        mov    dl, byte ptr [edi + 6ah]
        test   dl, 8
        jne    NEAR PTR loc_15_42AF
        test   dl, 4
        jne    NEAR PTR loc_15_42AA
loc_15_41F1:
        xor    edx, edx
        call   sub_15_2E3E
        lea    esi, [edi + 543h]
        mov    dl, 3
        call   sub_15_3F91
        test   byte ptr [edi + 7ch], 1
        je     SHORT loc_15_4216
        db  0FFh, 015h, 0BCh, 00Fh, 000h, 000h
        jmp    NEAR PTR loc_15_431C
loc_15_4216:
        db  0FFh, 015h, 0C0h, 00Fh, 000h, 000h
        movzx  ecx, word ptr [eax + 4]
        shr    ecx, 1
        bt     dword ptr [edi + 4a6h], ecx
        jae    NEAR PTR loc_15_431C
        push   eax
        call   sub_15_18BA
        push   edi
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 0ch], 0
        je     SHORT loc_15_4244
        call   dword ptr [edi + 0ch]
loc_15_4244:
        pop    edi
        pop    eax
        xor    edx, edx
        push   ebx
        xor    ebx, ebx
        call   sub_15_2E3E
        pop    ebx
        mov    dx, 402h
        call   sub_15_4324
        lea    esi, [edi + 55bh]
        db  0FFh, 015h, 0C4h, 00Fh, 000h, 000h
        jmp    NEAR PTR loc_15_431C
loc_15_426B:
        xor    edx, edx
        call   sub_15_2E3E
        call   sub_15_1193
        mov    dl, 3
        call   sub_15_3F91
        mov    dx, 300h
        call   sub_15_4324
        lea    esi, [edi + 543h]
        db  0FFh, 015h, 0BCh, 00Fh, 000h, 000h
        mov    dx, 0c02h
        call   sub_15_4324
        lea    esi, [edi + 55bh]
        db  0FFh, 015h, 0BCh, 00Fh, 000h, 000h
        jmp    SHORT loc_15_431C
loc_15_42AA:
        test   dl, 8
        je     SHORT loc_15_42CB
loc_15_42AF:
        xor    edx, edx
        call   sub_15_2E3E
        mov    dl, 0fh
        call   sub_15_3F91
        lea    esi, [edi + 543h]
        db  0FFh, 015h, 0BCh, 00Fh, 000h, 000h
        jmp    SHORT loc_15_431C
loc_15_42CB:
        push   eax
        call   sub_15_18BA
        pop    eax
        xor    edx, edx
        push   ebx
        xor    ebx, ebx
        call   sub_15_2E3E
        pop    ebx
        lea    esi, [edi + 543h]
        xor    edx, edx
        inc    dh
loc_15_42E7:
        call   sub_15_4324
        cmp    word ptr [esi + 8], 0
        jne    SHORT loc_15_430A
        cmp    dl, 1
        jne    SHORT loc_15_430A
        test   byte ptr [edi], 60h
        je     SHORT loc_15_430A
        sub    esi, 0ch
        call   sub_15_4426
        add    esi, 18h
        jmp    SHORT loc_15_4313
loc_15_430A:
        db  0FFh, 015h, 0BCh, 00Fh, 000h, 000h
        add    esi, 0ch
loc_15_4313:
        inc    dl
        shl    dh, 1
        cmp    dl, 4
        jb     SHORT loc_15_42E7
loc_15_431C:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        clc
        ret
sub_15_4324:
        push   eax
        push   edi
        push   edx
        db  0BFh, 0D0h, 006h, 000h, 000h
        cmp    dword ptr [edi + 6ch], 0
        je     SHORT loc_15_4337
        call   dword ptr [edi + 6ch]
        jae    SHORT loc_15_435A
loc_15_4337:
        mov    edx, 3ceh
        mov    al, 4
        out    dx, al
        jmp    SHORT loc_15_4341
loc_15_4341:
        inc    dl
        mov    al, byte ptr [esp]
        out    dx, al
        jmp    SHORT loc_15_4349
loc_15_4349:
        mov    edx, 3c4h
        mov    al, 2
        out    dx, al
        jmp    SHORT loc_15_4353
loc_15_4353:
        inc    dl
        mov    al, byte ptr [esp + 1]
        out    dx, al
loc_15_435A:
        pop    edx
        pop    edi
        pop    eax
        ret
sub_15_435E:
        cmp    dx, word ptr [esi + 0ah]
        jb     SHORT loc_15_437A
        cmp    dx, word ptr [esi + 8]
        jae    SHORT loc_15_437A
        db  0CDh, 020h, 004h, 000h, 001h, 000h
        je     SHORT loc_15_4379
        pushad
        db  0E8h, 0C0h, 0C1h, 0F0h, 0FFh
        popad
loc_15_4379:
        clc
loc_15_437A:
        cmc
        ret
sub_15_437C:
        call   sub_15_435E
        jb     SHORT loc_15_43A2
        push   edx
        push   ecx
        push   0
        push   edx
        push   1
        push   dword ptr [esi]
        db  0CDh, 020h, 056h, 000h, 001h, 000h
        add    esp, 10h
        pop    ecx
        pop    edx
        or     eax, eax
        stc
        je     SHORT loc_15_43A2
        pop    eax
        push   dword ptr [esi]
        push   edx
        push   eax
        clc
loc_15_43A2:
        ret
sub_15_43A3:
        pushad
        movzx  edx, word ptr [eax + 4]
        call   sub_15_437C
        jb     SHORT loc_15_43E1
        mov    edi, edx
        shl    edi, 0ch
        add    edi, dword ptr [esi + 4]
        shl    ecx, 0ch
        sub    ecx, 0a0000h
        db  003h, 00Dh, 0C8h, 00Eh, 000h, 000h
        mov    esi, ecx
        mov    ecx, 1000h
        cld
        rep movsb
        pop    ecx
        pop    eax
        push   0
        push   ecx
        push   1
        push   eax
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
loc_15_43E1:
        popad
        ret
sub_15_43E3:
        pushad
        movzx  edx, word ptr [eax + 4]
        call   sub_15_437C
        jb     SHORT loc_15_4424
        mov    edi, edx
        shl    edi, 0ch
        add    edi, dword ptr [esi + 4]
        shl    ecx, 0ch
        sub    ecx, 0a0000h
        db  003h, 00Dh, 0C8h, 00Eh, 000h, 000h
        mov    esi, ecx
        mov    ecx, 800h
        cld
loc_15_440E:
        inc    edi
        movsb
        inc    esi
        loop   loc_15_440E
        pop    ecx
        pop    eax
        push   0
        push   ecx
        push   1
        push   eax
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
loc_15_4424:
        popad
        ret
sub_15_4426:
        pushad
        movzx  edx, word ptr [eax + 4]
        call   sub_15_437C
        jb     SHORT loc_15_4442
        shl    edx, 0ch
        add    edx, dword ptr [esi + 4]
        inc    edx
        mov    edi, ecx
        mov    ecx, 0fffh
        jmp    SHORT loc_15_4461
loc_15_4442:
        mov    ax, 2007h
        jmp    SHORT loc_15_44BB
loc_15_4448:
        pushad
        movzx  edx, word ptr [eax + 4]
        call   sub_15_437C
        jb     SHORT loc_15_4489
        shl    edx, 0ch
        add    edx, dword ptr [esi + 4]
        mov    edi, ecx
        mov    ecx, 1000h
loc_15_4461:
        shl    edi, 0ch
        sub    edi, 0a0000h
        db  003h, 03Dh, 0C8h, 00Eh, 000h, 000h
        mov    esi, edx
        cld
        rep movsb
        pop    ecx
        pop    eax
        push   0
        push   ecx
        push   1
        push   eax
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
        popad
        ret
loc_15_4488:
        pushad
loc_15_4489:
        db  03Bh, 01Dh, 0F0h, 00Eh, 000h, 000h
        je     SHORT loc_15_44B4
        test   dword ptr [edi], 2000h
        jne    SHORT loc_15_44B4
        shl    ecx, 0ch
        sub    ecx, 0a0000h
        db  003h, 00Dh, 0C8h, 00Eh, 000h, 000h
        mov    edi, ecx
        xor    eax, eax
        mov    ecx, 1000h
        cld
        rep stosb
loc_15_44B4:
        popad
        ret
loc_15_44B6:
        pushad
        mov    ax, 720h
loc_15_44BB:
        shl    ecx, 0ch
        sub    ecx, 0a0000h
        db  003h, 00Dh, 0C8h, 00Eh, 000h, 000h
        mov    edi, ecx
        mov    ecx, 800h
        cld
loc_15_44D2:
        stosb
        xchg   al, ah
        stosb
        xchg   al, ah
        loop   loc_15_44D2
        popad
        ret
sub_15_44DC:
        push   esi
        mov    esi, dword ptr [edi + 5b8h]
        xor    ecx, ecx
loc_15_44E5:
        mov    eax, dword ptr [esi + ecx*4]
        or     eax, eax
        je     SHORT loc_15_452D
        push   ecx
        movzx  ecx, word ptr [eax + 6]
        jecxz  loc_15_452C
        call   sub_15_3F19
        test   cl, 40h
        je     SHORT loc_15_452C
        or     byte ptr [eax + 3], 40h
        movzx  ecx, word ptr [eax + 6]
        push   ecx
        push   0
        push   -1
        push   0
        push   -1
        push   1
        push   ecx
        push   ebx
        db  0CDh, 020h, 060h, 000h, 001h, 000h
        add    esp, 1ch
        pop    ecx
        shl    ecx, 0ch
        add    ecx, dword ptr [ebx + 4]
        mov    cl, byte ptr [ecx]
        mov    eax, dword ptr [esp]
        call   sub_15_456A
loc_15_452C:
        pop    ecx
loc_15_452D:
        inc    ecx
        cmp    ecx, dword ptr [edi + 5b4h]
        jb     SHORT loc_15_44E5
        pop    esi
        ret
sub_15_4538:
        cmp    byte ptr [edi + 60eh], 0
        je     SHORT loc_15_4569
        pushad
        call   sub_15_3A50
        mov    esi, dword ptr [edi + 5b8h]
        xor    edx, edx
loc_15_454F:
        mov    eax, dword ptr [esi + edx*4]
        or     eax, eax
        je     SHORT loc_15_455F
        and    byte ptr [eax + 3], 0bfh
        call   sub_15_2D96
loc_15_455F:
        inc    edx
        cmp    edx, dword ptr [edi + 5b4h]
        jb     SHORT loc_15_454F
        popad
loc_15_4569:
        ret
sub_15_456A:
        push   eax
        db  0CDh, 020h, 0CFh, 000h, 001h, 000h
        mov    dword ptr [edi + 5e8h], eax
        pop    eax
        bts    dword ptr [edi + 5c0h], eax
        ret
sub_15_4580:
        pushad
        movzx  edx, word ptr [eax + 4]
        mov    ecx, edx
        shr    edx, 1
        je     SHORT loc_15_45C7
        movzx  ecx, byte ptr [edi + edx + 573h]
        jecxz  loc_15_4597
        jmp    SHORT loc_15_45C4
loc_15_4597:
        lea    ebx, [edi + 574h]
        mov    ecx, 7
        mov    ebp, 0feh
loc_15_45A7:
        movzx  edx, byte ptr [ebx]
        shr    edx, 1
        btr    ebp, edx
        inc    ebx
        loop   loc_15_45A7
        bsf    ecx, ebp
        shl    ecx, 1
        movzx  edx, word ptr [eax + 4]
        shr    edx, 1
        mov    byte ptr [edi + edx + 573h], cl
loc_15_45C4:
        adc    ecx, 0
loc_15_45C7:
        mov    edx, ecx
        call   sub_15_437C
        jb     SHORT loc_15_4604
        mov    edi, ecx
        shl    edi, 0ch
        add    edi, dword ptr [esi + 4]
        mov    esi, dword ptr [esp + 20h]
        shl    esi, 0ch
        sub    esi, 0a0000h
        db  003h, 035h, 0C8h, 00Eh, 000h, 000h
        mov    ecx, 1000h
        cld
        rep movsb
        pop    ecx
        pop    eax
        push   0
        push   ecx
        push   1
        push   eax
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
loc_15_4604:
        popad
        ret
loc_15_4606:
        pushad
        movzx  edx, word ptr [eax + 4]
        mov    eax, edx
        shr    eax, 1
        je     SHORT loc_15_4626
        movzx  edx, byte ptr [edi + eax + 573h]
        mov    eax, edx
        adc    edx, 0
        or     eax, eax
        je     NEAR PTR loc_15_4489
loc_15_4626:
        call   sub_15_437C
        jb     NEAR PTR loc_15_4489
        shl    edx, 0ch
        add    edx, dword ptr [esi + 4]
        shl    ecx, 0ch
        sub    ecx, 0a0000h
        db  003h, 00Dh, 0C8h, 00Eh, 000h, 000h
        mov    edi, ecx
        mov    esi, edx
        mov    ecx, 1000h
        cld
        rep movsb
        pop    ecx
        pop    eax
        push   0
        push   ecx
        push   1
        push   eax
        db  0CDh, 020h, 057h, 000h, 001h, 000h
        add    esp, 10h
        popad
        ret
        db  000h, 000h, 000h, 0EBh, 006h, 0FFh, 025h, 0E0h, 00Fh, 000h, 000h
loc_15_4670:
        pushad
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  08Bh, 03Dh, 0CCh, 006h, 000h, 000h
        add    edi, ebx
        test   byte ptr [edi], 1
        je     SHORT loc_15_469D
        call   sub_15_4780
        and    byte ptr [edi], 0fbh
        test   byte ptr [edi + 1], 8
        je     SHORT loc_15_4698
        or     byte ptr [edi + 1], 10h
        jmp    SHORT loc_15_469D
loc_15_4698:
        call   sub_15_46DC
loc_15_469D:
        popad
        db  0FFh, 025h, 0E0h, 00Fh, 000h, 000h
loc_15_46A4:
        mov    edi, edx
        mov    dword ptr [edi + 5e4h], 0
        test   byte ptr [edi], 2
        jne    SHORT loc_15_46D8
        and    byte ptr [edi], 0fbh
        test   byte ptr [edi], 1
        je     SHORT loc_15_46D8
        xor    eax, eax
        xchg   byte ptr [edi + 60dh], al
        btr    dword ptr [edi], 1ah
        jae    SHORT sub_15_46DC
        cmp    al, 8
        jae    SHORT sub_15_46DC
        inc    eax
        mov    byte ptr [edi + 60dh], al
        jmp    SHORT sub_15_4740
loc_15_46D8:
        ret
        db  02Eh, 08Bh, 0C0h
sub_15_46DC:
        mov    word ptr [edi + 5cch], 0
        bt     dword ptr [edi], 0bh
        jb     SHORT loc_15_4737
        call   sub_15_1C57
        jb     SHORT loc_15_46F9
        call   sub_15_01E7
        jae    SHORT loc_15_4738
loc_15_46F9:
        or     byte ptr [edi + 1], 8
        inc    byte ptr [edi + 604h]
        push   ebx
        cmp    byte ptr [edi + 9dh], 0dh
        jae    SHORT loc_15_471F
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  0CDh, 020h, 040h, 000h, 001h, 000h
        mov    dword ptr [edi + 5ech], eax
loc_15_471F:
        mov    eax, 10h
        mov    ecx, 0bh
        mov    edx, edi
        db  0BEh, 08Fh, 047h, 000h, 000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        pop    ebx
loc_15_4737:
        ret
loc_15_4738:
        mov    byte ptr [edi + 604h], 0
        nop
sub_15_4740:
        test   byte ptr [edi], 1
        je     SHORT loc_15_477C
        cmp    dword ptr [edi + 5e4h], 0
        jne    SHORT loc_15_477C
        db  0A1h, 0D8h, 00Fh, 000h, 000h
        test   byte ptr [edi], 4
        jne    SHORT loc_15_4769
        db  0A1h, 0D4h, 00Fh, 000h, 000h
        cmp    byte ptr [edi + 604h], 3
        jb     SHORT loc_15_4769
        shl    eax, 2
loc_15_4769:
        mov    edx, edi
        db  0BEh, 0A4h, 046h, 000h, 000h
        db  0CDh, 020h, 03Dh, 000h, 001h, 000h
        mov    dword ptr [edi + 5e4h], esi
loc_15_477C:
        ret
        db  02Eh, 08Bh, 0C0h
sub_15_4780:
        xor    esi, esi
        xchg   dword ptr [edi + 5e4h], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        ret
loc_15_478F:
        mov    ebx, edx
        db  02Bh, 01Dh, 0CCh, 006h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_15_47C1
        db  0BEh, 0C2h, 047h, 000h, 000h
        mov    ecx, 41ah
        mov    eax, 0
        btr    dword ptr [edx], 1dh
        jae    SHORT loc_15_47B9
        or     eax, 200000h
loc_15_47B9:
        db  0CDh, 020h, 002h, 000h, 017h, 000h
        jb     SHORT loc_15_47C2
loc_15_47C1:
        ret
loc_15_47C2:
        mov    ebx, edx
        db  02Bh, 01Dh, 0CCh, 006h, 000h, 000h
        db  0CDh, 020h, 005h, 000h, 001h, 000h
        jb     SHORT loc_15_47E7
        and    byte ptr [edx + 1], 0f7h
        mov    edi, edx
        btr    dword ptr [edi], 0ch
        jae    NEAR PTR sub_15_4740
        jmp    NEAR PTR sub_15_46DC
loc_15_47E7:
        ret
_PTEXT ends
        end
