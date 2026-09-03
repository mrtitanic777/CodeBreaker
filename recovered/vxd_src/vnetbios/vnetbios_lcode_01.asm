; vnetbios_lcode_01.asm -- VNETBIOS object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  051h, 0B9h, 00Ch, 019h, 000h, 000h, 085h, 0C0h, 074h, 005h, 0B9h, 010h, 019h, 000h, 000h, 08Bh
        db  001h, 085h, 0C0h, 074h, 008h, 0FFh, 070h, 0FCh, 08Fh, 001h, 059h, 0F8h, 0C3h, 0F9h, 059h, 0C3h
sub_1_0020:
        push   ecx
        db  0B9h, 00Ch, 019h, 000h, 000h
        bt     dword ptr [eax - 8], 9
        jae    SHORT loc_1_0032
        db  0B9h, 010h, 019h, 000h, 000h
loc_1_0032:
        push   dword ptr [ecx]
        mov    dword ptr [ecx], eax
        pop    dword ptr [eax - 4]
        pop    ecx
        ret
sub_1_003B:
        jecxz  loc_1_005D
        test   esi, esi
        je     SHORT loc_1_005D
        pushad
        test   byte ptr [eax - 7], 4
        jne    SHORT loc_1_0060
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_0074
        movzx  edx, si
        shr    esi, 10h
        shl    esi, 4
        add    esi, edx
        add    esi, dword ptr [ebx + 4]
        jmp    SHORT loc_1_0090
loc_1_005D:
        xor    esi, esi
        ret
loc_1_0060:
        cmp    esi, 100000h
        jae    SHORT loc_1_0090
        test   byte ptr [eax - 7], 2
        jne    SHORT loc_1_0096
        mov    edi, esi
        xor    esi, esi
        jmp    SHORT loc_1_00CA
loc_1_0074:
        movzx  edi, si
        shr    esi, 10h
        push   ecx
        push   0
        push   esi
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        lea    esi, [edi + eax]
        mov    eax, dword ptr [esp + 1ch]
loc_1_0090:
        test   byte ptr [eax - 7], 2
        je     SHORT loc_1_00C2
loc_1_0096:
        mov    edi, esi
        and    edi, 0fffh
        add    ecx, edi
        dec    ecx
        shr    ecx, 0ch
        inc    ecx
        mov    dword ptr [esp + 4], ecx
        mov    dword ptr [esp], esi
        shr    esi, 0ch
        push   0
        push   ecx
        push   esi
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_00DB
        popad
        ret
loc_1_00C2:
        db  0CDh, 020h, 00Dh, 000h, 006h, 000h
        jb     SHORT loc_1_00DB
loc_1_00CA:
        mov    dword ptr [esp + 4], esi
        shl    edi, 0ch
        shr    di, 0ch
        mov    dword ptr [esp], edi
        popad
        clc
        ret
loc_1_00DB:
        popad
        xor    esi, esi
        mov    edi, esi
        stc
        ret
sub_1_00E2:
        test   esi, esi
        je     SHORT loc_1_00F2
        test   byte ptr [eax - 7], 2
        jne    SHORT loc_1_00F3
        db  0CDh, 020h, 00Eh, 000h, 006h, 000h
loc_1_00F2:
        ret
loc_1_00F3:
        push   eax
        push   ecx
        shr    edi, 0ch
        push   0
        push   esi
        push   edi
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        pop    eax
        ret
sub_1_0108:
        push   edi
        db  0CDh, 020h, 008h, 001h, 001h, 000h
        mov    eax, edi
        pop    edi
        ret
sub_1_0113:
        push   ebx
        push   edi
        push   esi
        push   eax
        push   ecx
        mov    ecx, eax
        and    ecx, 0fffh
        add    ebx, ecx
        shr    eax, 0ch
        add    ebx, 0fffh
        shr    ebx, 0ch
        pop    ecx
        push   40000000h
        push   ebx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_015D
        cmp    eax, 0fffffbadh
        je     SHORT loc_1_0158
        pop    ebx
        and    ebx, 0fffh
        add    eax, ebx
        clc
loc_1_0154:
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_1_0158:
        int3
        stc
        pop    ebx
        jmp    SHORT loc_1_0154
loc_1_015D:
        int3
        stc
        pop    ebx
        jmp    SHORT loc_1_0154
sub_1_0162:
        push   ebx
        push   edi
        push   esi
        push   ecx
        mov    ecx, eax
        and    ecx, 0fffh
        add    ebx, ecx
        shr    eax, 0ch
        add    ebx, 0fffh
        shr    ebx, 0ch
        pop    ecx
        push   40000000h
        push   ebx
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_0196
        clc
loc_1_0192:
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_1_0196:
        int3
        jmp    SHORT loc_1_0192
sub_1_0199:
        push   ebx
        mov    ebx, dword ptr [esp + 8]
        push   esi
        push   edi
        push   ebp
        mov    esi, ebx
        mov    edx, 5ch
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, 23h
        cmp    byte ptr [esi], 37h
        je     SHORT loc_1_0216
        movzx  ecx, byte ptr [esi + 30h]
        cmp    cl, 8
        jae    SHORT loc_1_0211
        db  08Bh, 00Ch, 08Dh, 08Ch, 01Ah, 000h, 000h
        jecxz  loc_1_01DA
        call   sub_1_0223
        jmp    SHORT loc_1_0211
loc_1_01DA:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  08Bh, 03Dh, 0A4h, 019h, 000h, 000h
        mov    ecx, 1
        call   sub_1_0F48
        mov    al, byte ptr [ebp + 1ch]
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
loc_1_0211:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0216:
        call   sub_1_1722
        jmp    SHORT loc_1_0211
loc_1_021D:
        mov    eax, 22h
        ret
sub_1_0223:
        db  0CDh, 020h, 00Dh, 000h, 02Ah, 000h
        mov    ecx, eax
        call   0
        jb     SHORT loc_1_021D
        and    dword ptr [eax - 8], 200h
        or     word ptr [eax - 8], 0a20h
        mov    dword ptr [eax - 10h], esi
        mov    dword ptr [eax - 28h], ecx
        push   eax
        call   sub_1_0108
        mov    ecx, eax
        pop    eax
        mov    dword ptr [eax - 20h], ecx
        db  083h, 03Dh, 0DCh, 019h, 000h, 000h, 001h
        jb     SHORT loc_1_0265
        db  083h, 03Dh, 0D8h, 019h, 000h, 000h, 001h
        jb     SHORT loc_1_0265
        jmp    SHORT loc_1_026E
loc_1_0265:
        pushad
        xor    ebx, ebx
        call   sub_1_0A57
        popad
loc_1_026E:
        xor    ecx, ecx
        mov    dword ptr [eax - 18h], ecx
        mov    dword ptr [eax - 1ch], ecx
        mov    dword ptr [eax - 24h], ecx
        mov    dword ptr [eax - 0ch], ebx
        mov    dword ptr [eax - 14h], ecx
        xchg   esi, eax
        push   ebx
        mov    edi, eax
        mov    ebx, 40h
        call   sub_1_0113
        pop    ebx
        xchg   esi, eax
        jb     NEAR PTR loc_1_048B
        mov    dword ptr [eax - 14h], esi
        mov    byte ptr [esi + 1], 0ffh
        mov    byte ptr [esi + 31h], 0ffh
        db  08Bh, 015h, 0A4h, 019h, 000h, 000h
        mov    ecx, eax
        xchg   dword ptr [edx + ebx], ecx
        mov    dword ptr [eax - 4], ecx
        mov    edi, eax
        cld
        mov    ecx, 10h
        rep movsd
        movzx  ecx, byte ptr [eax]
        and    cl, 7fh
        db  08Ah, 089h, 008h, 01Ah, 000h, 000h
        xor    esi, esi
        mov    edi, eax
        cmp    cl, 0
        je     SHORT loc_1_031A
        cmp    cl, 4
        je     SHORT loc_1_031A
        cmp    cl, 18h
        je     NEAR PTR loc_1_035E
        cmp    cl, 14h
        jne    SHORT loc_1_02FD
        cmp    word ptr [edi + 0ah], 0
        je     SHORT loc_1_02FD
        mov    eax, dword ptr [edi + 0ch]
        movzx  ebx, word ptr [edi + 0ah]
        call   sub_1_0113
        jb     NEAR PTR loc_1_03BF
        inc    esi
        mov    dword ptr [edi + 0ch], eax
loc_1_02FD:
        cmp    word ptr [edi + 8], 0
        je     SHORT loc_1_031A
        mov    eax, dword ptr [edi + 4]
        movzx  ebx, word ptr [edi + 8]
        call   sub_1_0113
        jb     NEAR PTR loc_1_03CA
        inc    esi
        mov    dword ptr [edi + 4], eax
loc_1_031A:
        mov    dword ptr [edi - 24h], esi
        test   byte ptr [edi], 80h
        je     SHORT loc_1_0329
        db  0C7h, 047h, 02Ch, 099h, 004h, 000h, 000h
loc_1_0329:
        movzx  ecx, byte ptr [edi + 30h]
        db  08Bh, 00Ch, 08Dh, 08Ch, 01Ah, 000h, 000h
        mov    ebx, edi
        push   ebx
        call   ecx
        pop    ebx
        test   byte ptr [ebx], 80h
        je     SHORT loc_1_034F
        cmp    al, 0
        je     SHORT loc_1_034B
        cmp    al, 0ffh
        jne    NEAR PTR loc_1_03DD
loc_1_034B:
        xor    eax, eax
        clc
        ret
loc_1_034F:
        mov    al, byte ptr [ebx + 1]
        cmp    al, 0ffh
        jne    NEAR PTR loc_1_03DD
        xor    eax, eax
        clc
        ret
loc_1_035E:
        mov    eax, dword ptr [edi + 4]
        mov    edx, dword ptr [edi - 28h]
        db  08Bh, 00Dh, 0A4h, 019h, 000h, 000h
        mov    ecx, dword ptr [ebx + ecx]
loc_1_036D:
        jecxz  loc_1_037E
        cmp    dword ptr [ecx - 10h], eax
        jne    SHORT loc_1_0379
        cmp    dword ptr [ecx - 28h], edx
        je     SHORT loc_1_038A
loc_1_0379:
        mov    ecx, dword ptr [ecx - 4]
        jmp    SHORT loc_1_036D
loc_1_037E:
        mov    ebx, edi
        mov    al, 24h
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx + 31h], al
        jmp    SHORT loc_1_034F
loc_1_038A:
        mov    ebx, edi
        mov    dword ptr [ebx + 4], ecx
        push   ecx
        push   edi
        movzx  ecx, byte ptr [edi + 30h]
        db  08Bh, 00Ch, 08Dh, 08Ch, 01Ah, 000h, 000h
        call   ecx
        pop    edi
        pop    ecx
        test   byte ptr [ecx], 80h
        jne    SHORT loc_1_03BD
        test   word ptr [ecx - 8], 10fh
        jne    SHORT loc_1_034F
        pushad
        mov    edi, ecx
        call   sub_1_058C
        popad
        xchg   ecx, eax
        call   sub_1_16A7
        xchg   ecx, eax
loc_1_03BD:
        jmp    SHORT loc_1_034F
loc_1_03BF:
        mov    eax, 1
        push   ebx
        push   eax
        mov    edx, esi
        jmp    SHORT loc_1_0425
loc_1_03CA:
        mov    eax, 1
        push   ebx
        push   eax
        mov    edx, esi
        jmp    SHORT loc_1_03F7
        db  0CCh, 0B8h, 0FFh, 000h, 000h, 000h, 090h, 0C3h
loc_1_03DD:
        xor    edx, edx
        mov    edi, ebx
        pushad
        call   sub_1_058C
        popad
        push   ebx
        push   eax
        xchg   dword ptr [edi - 24h], edx
        cmp    edx, 0
        je     SHORT loc_1_0425
        cmp    edx, 1
        je     SHORT loc_1_040F
loc_1_03F7:
        dec    edx
        push   edx
        mov    eax, dword ptr [edi + 0ch]
        mov    edx, dword ptr [edi - 14h]
        movzx  ebx, word ptr [edx + 0ah]
        call   sub_1_0162
        pop    edx
        jb     NEAR PTR loc_1_0496
loc_1_040F:
        cmp    dl, 0
        je     SHORT loc_1_0425
        mov    edx, dword ptr [edi - 14h]
        mov    eax, dword ptr [edi + 4]
        movzx  ebx, word ptr [edx + 8]
        call   sub_1_0162
        jb     SHORT loc_1_0496
loc_1_0425:
        mov    eax, dword ptr [edi - 14h]
        pop    ecx
        mov    byte ptr [eax + 31h], cl
        mov    byte ptr [eax + 1], cl
        push   ecx
        mov    cl, byte ptr [edi + 2]
        mov    byte ptr [eax + 2], cl
        mov    cl, byte ptr [edi + 3]
        mov    byte ptr [eax + 3], cl
        movzx  ecx, byte ptr [edi]
        and    cl, 7fh
        db  08Ah, 089h, 008h, 01Ah, 000h, 000h
        cmp    cl, 14h
        jne    SHORT loc_1_0455
        mov    cx, word ptr [edi + 0ah]
        mov    word ptr [eax + 0ah], cx
loc_1_0455:
        mov    cx, word ptr [edi + 8]
        mov    word ptr [eax + 8], cx
        mov    cl, byte ptr [edi]
        cmp    cl, 33h
        jne    SHORT loc_1_0470
        mov    ecx, dword ptr [edi + 1ah]
        mov    dword ptr [eax + 1ah], ecx
        mov    ecx, dword ptr [edi + 1eh]
        mov    dword ptr [eax + 1eh], ecx
loc_1_0470:
        mov    ecx, 4
        push   edi
        push   esi
        lea    esi, [edi + 0ah]
        lea    edi, [eax + 0ah]
        rep movsd
        pop    esi
        pop    edi
        mov    ebx, 40h
        call   sub_1_0162
loc_1_048B:
        mov    eax, edi
        call   sub_1_16A7
        pop    eax
        pop    ebx
        clc
        ret
loc_1_0496:
        int3
        jmp    SHORT loc_1_0425
loc_1_0499:
        push   ebx
        push   eax
        push   edi
        push   esi
        or     byte ptr [ebx - 8], 2
        mov    esi, dword ptr [ebx - 14h]
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [esi + 1], al
        mov    cl, byte ptr [ebx + 2]
        mov    byte ptr [esi + 2], cl
        mov    cl, byte ptr [ebx + 3]
        mov    byte ptr [esi + 3], cl
        xor    esi, esi
        mov    edi, ebx
        pushad
        call   sub_1_058C
        popad
        push   ebx
        push   eax
        xchg   dword ptr [edi - 24h], esi
        cmp    esi, 0
        je     SHORT loc_1_0504
        cmp    esi, 1
        je     SHORT loc_1_04E6
        dec    esi
        mov    eax, dword ptr [edi + 0ch]
        mov    edx, dword ptr [edi - 14h]
        movzx  ebx, word ptr [edx + 0ah]
        call   sub_1_0162
        jb     NEAR PTR loc_1_0586
loc_1_04E6:
        cmp    esi, 0
        je     NEAR PTR loc_1_0425
        mov    eax, dword ptr [edi + 4]
        mov    edx, dword ptr [edi - 14h]
        movzx  ebx, word ptr [edx + 8]
        call   sub_1_0162
        jb     NEAR PTR loc_1_0586
loc_1_0504:
        mov    esi, dword ptr [edi - 14h]
        movzx  ecx, byte ptr [esi]
        and    cl, 7fh
        db  08Ah, 089h, 008h, 01Ah, 000h, 000h
        cmp    cl, 14h
        jne    SHORT loc_1_0520
        mov    cx, word ptr [edi + 0ah]
        mov    word ptr [esi + 0ah], cx
loc_1_0520:
        mov    cx, word ptr [edi + 8]
        mov    word ptr [esi + 8], cx
        mov    eax, dword ptr [edi - 14h]
        mov    ecx, 4
        push   edi
        push   esi
        lea    esi, [edi + 0ah]
        lea    edi, [eax + 0ah]
        rep movsd
        pop    esi
        pop    edi
        mov    ebx, 40h
        mov    esi, dword ptr [esi + 2ch]
        call   sub_1_0162
        mov    ecx, esi
        jecxz  loc_1_0566
        test   dword ptr [edi - 8], 1000h
        jne    SHORT loc_1_0566
        push   dword ptr [edi - 20h]
        push   dword ptr [edi - 10h]
        push   esi
        db  0CDh, 020h, 002h, 000h, 02Ah, 000h
        add    esp, 0ch
loc_1_0566:
        mov    eax, edi
        and    word ptr [eax - 8], 0f7ddh
        test   word ptr [eax - 8], 10fh
        jne    SHORT loc_1_057E
        mov    ebx, dword ptr [edi - 0ch]
        call   sub_1_16A7
loc_1_057E:
        pop    eax
        pop    ebx
        pop    esi
        pop    edi
        pop    eax
        pop    ebx
        clc
        ret
loc_1_0586:
        int3
        jmp    NEAR PTR loc_1_0504
sub_1_058C:
        push   ecx
        xor    ch, ch
        mov    cl, byte ptr [edi]
        and    cl, 7fh
        cmp    byte ptr [edi + 1], 0
        jne    NEAR PTR loc_1_0644
        cmp    cl, 10h
        je     SHORT loc_1_05CC
        inc    ch
        cmp    cl, 11h
        je     SHORT loc_1_05CC
        inc    ch
        cmp    cl, 30h
        je     SHORT loc_1_05CC
        inc    ch
        cmp    cl, 36h
        je     SHORT loc_1_05CC
        inc    ch
        cmp    cl, 31h
        je     NEAR PTR loc_1_0644
        inc    ch
        cmp    cl, 12h
        je     SHORT loc_1_0644
loc_1_05CA:
        pop    ecx
        ret
loc_1_05CC:
        push   esi
        call   sub_1_0746
        or     esi, esi
        jne    SHORT loc_1_05D9
        pop    esi
        jmp    SHORT loc_1_05CA
loc_1_05D9:
        cmp    ch, 2
        jb     SHORT loc_1_0619
        db  0FFh, 00Dh, 0DCh, 019h, 000h, 000h
        push   ebx
        push   eax
        db  0A1h, 0D4h, 019h, 000h, 000h
        mov    ebx, dword ptr [eax + 10h]
        db  089h, 01Dh, 0D4h, 019h, 000h, 000h
        mov    ebx, esi
        push   edi
        lea    esi, [edi + 1ah]
        lea    edi, [eax]
        mov    ecx, 4
        cld
        rep movsd
        pop    edi
        mov    cl, byte ptr [edi + 30h]
        mov    byte ptr [eax + 14h], cl
        mov    esi, dword ptr [ebx + 4]
        mov    dword ptr [eax + 10h], esi
        mov    dword ptr [ebx + 4], eax
        pop    eax
        pop    ebx
        pop    esi
        jmp    SHORT loc_1_05CA
loc_1_0619:
        push   edx
        mov    dl, byte ptr [edi + 30h]
        call   sub_1_0707
        or     edx, edx
        je     SHORT loc_1_0640
        mov    ch, byte ptr [edi + 2]
        mov    cl, ch
        shr    cl, 3
        push   eax
        movzx  eax, cl
        add    edx, eax
        pop    eax
        mov    cl, ch
        and    cl, 7
        mov    ch, 1
        shl    ch, cl
        or     byte ptr [edx], ch
loc_1_0640:
        pop    edx
        pop    esi
        jmp    SHORT loc_1_05CA
loc_1_0644:
        push   esi
        call   sub_1_0746
        or     esi, esi
        jne    SHORT loc_1_064F
        int3
loc_1_064F:
        cmp    ch, 0
        jne    SHORT loc_1_0699
        cmp    cl, 15h
        je     SHORT loc_1_0669
        cmp    cl, 14h
        je     SHORT loc_1_0669
        cmp    cl, 17h
        je     SHORT loc_1_0669
        pop    esi
        jmp    NEAR PTR loc_1_05CA
loc_1_0669:
        push   edx
        mov    dl, byte ptr [edi + 30h]
        call   sub_1_0707
        or     edx, edx
        je     SHORT loc_1_0692
        mov    ch, byte ptr [edi + 2]
        mov    cl, ch
        shr    cl, 3
        push   eax
        movzx  eax, cl
        add    edx, eax
        pop    eax
        mov    cl, ch
        and    cl, 7
        mov    ch, 1
        shl    ch, cl
        not    ch
        and    byte ptr [edx], ch
loc_1_0692:
        pop    edx
        pop    esi
        jmp    NEAR PTR loc_1_05CA
loc_1_0699:
        cmp    ch, 5
        je     SHORT loc_1_0669
        push   ebx
        push   eax
        mov    bl, byte ptr [edi + 30h]
        xor    ecx, ecx
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_1_06EC
loc_1_06AC:
        cmp    byte ptr [eax + 14h], bl
        jne    SHORT loc_1_06C6
        push   edi
        push   esi
        push   ecx
        lea    edi, [edi + 1ah]
        lea    esi, [eax]
        mov    ecx, 4
        cld
        repe cmpsd
        pop    ecx
        pop    esi
        pop    edi
        je     SHORT loc_1_06D7
loc_1_06C6:
        mov    ecx, eax
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_1_06AC
        pop    eax
        pop    ebx
        pop    esi
        jmp    NEAR PTR loc_1_05CA
loc_1_06D7:
        or     ecx, ecx
        jne    SHORT loc_1_06F1
        mov    ecx, dword ptr [eax + 10h]
        mov    dword ptr [esi + 4], ecx
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_1_06EC:
        pop    eax
        pop    ebx
        pop    esi
        pop    ecx
        ret
loc_1_06F1:
        mov    ebx, dword ptr [eax + 10h]
        mov    dword ptr [ecx + 10h], ebx
        push   0
        push   eax
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    eax
        pop    ebx
        pop    esi
        pop    ecx
        ret
sub_1_0707:
        push   edi
        mov    edi, dword ptr [esi + 8]
        or     edi, edi
        je     SHORT loc_1_071B
loc_1_070F:
        cmp    byte ptr [edi + 24h], dl
        je     SHORT loc_1_0742
        mov    edi, dword ptr [edi + 20h]
        or     edi, edi
        jne    SHORT loc_1_070F
loc_1_071B:
        db  0FFh, 00Dh, 0D8h, 019h, 000h, 000h
        push   ebx
        db  08Bh, 03Dh, 0D0h, 019h, 000h, 000h
        mov    ebx, dword ptr [edi + 20h]
        db  089h, 01Dh, 0D0h, 019h, 000h, 000h
        mov    ebx, dword ptr [esi + 8]
        mov    dword ptr [esi + 8], edi
        mov    dword ptr [edi + 20h], ebx
        mov    byte ptr [edi + 24h], dl
        pop    ebx
        mov    edx, edi
        pop    edi
        ret
loc_1_0742:
        mov    edx, edi
        pop    edi
        ret
sub_1_0746:
        push   ebx
        db  08Bh, 035h, 0CCh, 019h, 000h, 000h
        mov    ebx, dword ptr [edi - 28h]
        or     esi, esi
        je     SHORT loc_1_075F
loc_1_0754:
        cmp    ebx, dword ptr [esi]
        je     SHORT loc_1_075F
        mov    esi, dword ptr [esi + 0ch]
        or     esi, esi
        jne    SHORT loc_1_0754
loc_1_075F:
        pop    ebx
        ret
VNETBIOS_Get_Version:
        mov    eax, 30ah
        mov    ebx, 1
        clc
        ret
VNETBIOS_Register2:
        movzx  eax, al
        cmp    al, 8
        jae    SHORT loc_1_07C0
        db  083h, 03Ch, 085h, 08Ch, 01Ah, 000h, 000h, 000h
        jne    SHORT loc_1_07DD
        test   ebx, ebx
        jne    SHORT loc_1_078B
        db  00Fh, 0A3h, 005h, 0D1h, 01Ah, 000h, 000h
        jb     SHORT loc_1_07DD
loc_1_078B:
        db  00Fh, 0B3h, 005h, 0D1h, 01Ah, 000h, 000h
loc_1_0792:
        db  089h, 014h, 085h, 08Ch, 01Ah, 000h, 000h
        db  089h, 00Ch, 085h, 0ACh, 01Ah, 000h, 000h
        db  00Fh, 0ABh, 005h, 0CDh, 01Ah, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_1_07B2
        db  00Fh, 0ABh, 005h, 0D9h, 01Ah, 000h, 000h
loc_1_07B2:
        pushad
        push   eax
        push   ecx
        call   sub_1_17E0
        add    esp, 8
        popad
        clc
        ret
loc_1_07C0:
        cmp    al, 0ffh
        jne    SHORT loc_1_07DD
        mov    al, 7
loc_1_07C6:
        db  00Fh, 0A3h, 005h, 0D1h, 01Ah, 000h, 000h
        jb     SHORT loc_1_07D9
        db  083h, 03Ch, 085h, 08Ch, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_0792
loc_1_07D9:
        dec    al
        jns    SHORT loc_1_07C6
loc_1_07DD:
        stc
        ret
VNETBIOS_Register:
        xor    ecx, ecx
        jmp    SHORT VNETBIOS_Register2
VNETBIOS_Deregister:
        movzx  eax, al
        cmp    al, 8
        jae    SHORT loc_1_0809
        db  08Dh, 01Ch, 085h, 08Ch, 01Ah, 000h, 000h
        xor    ecx, ecx
        cmp    dword ptr [ebx], ecx
        je     SHORT loc_1_0809
        db  00Fh, 0B3h, 005h, 0CDh, 01Ah, 000h, 000h
        db  00Fh, 0B3h, 005h, 0D9h, 01Ah, 000h, 000h
        mov    dword ptr [ebx], ecx
        clc
        ret
loc_1_0809:
        stc
        ret
VNETBIOS_Submit:
        push   esi
        push   edi
        push   ebx
        push   ebp
        mov    esi, ebx
        mov    edx, 5ch
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        mov    ebx, esi
        movzx  ecx, byte ptr [ebx + 30h]
        db  08Bh, 00Ch, 08Dh, 08Ch, 01Ah, 000h, 000h
        jecxz  loc_1_0832
        call   ecx
        jmp    SHORT loc_1_0864
loc_1_0832:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  08Bh, 03Dh, 0A4h, 019h, 000h, 000h
        call   sub_1_0F48
        mov    al, byte ptr [ebp + 1ch]
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
loc_1_0864:
        pop    ebp
        pop    ebx
        pop    edi
        pop    esi
        ret
VNETBIOS_Enum:
        db  0A1h, 0CDh, 01Ah, 000h, 000h
        db  08Bh, 015h, 0D1h, 01Ah, 000h, 000h
        ret
VNETBIOS_Enum2:
        db  0A1h, 0D9h, 01Ah, 000h, 000h
        ret
VNETBIOS_Map:
        jcxz   loc_1_088F
        mov    eax, 7
loc_1_0883:
        db  039h, 00Ch, 085h, 0ACh, 01Ah, 000h, 000h
        je     SHORT loc_1_0891
        dec    eax
        jns    SHORT loc_1_0883
loc_1_088F:
        stc
        ret
loc_1_0891:
        clc
        ret
VNETBIOS_Control:
        cmp    eax, 0
        db  00Fh, 084h, 0A5h, 0F8h, 0FEh, 0FFh
        cmp    eax, 1
        db  00Fh, 084h, 03Eh, 0F9h, 0FEh, 0FFh
        cmp    eax, 0ah
        je     NEAR PTR loc_1_0C13
        cmp    eax, 4
        je     NEAR PTR loc_1_0C13
        cmp    eax, 0bh
        je     NEAR PTR loc_1_0C13
        cmp    eax, 13h
        je     NEAR PTR loc_1_0D6C
        cmp    eax, 19h
        je     NEAR PTR loc_1_0C13
        cmp    eax, 6
        je     SHORT loc_1_08EB
        cmp    eax, 21h
        je     NEAR PTR loc_1_099C
        cmp    eax, 23h
        je     NEAR PTR loc_1_09F6
        clc
        ret
loc_1_08EB:
        db  083h, 03Dh, 0D5h, 01Ah, 000h, 000h, 000h
        jne    SHORT loc_1_090D
        mov    eax, 5ch
        db  00Fh, 0B7h, 015h, 0E1h, 01Ah, 000h, 000h
        db  066h, 08Bh, 00Dh, 0E3h, 01Ah, 000h, 000h
        db  0CDh, 020h, 043h, 000h, 001h, 000h
loc_1_090D:
        clc
        ret
loc_1_090F:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_1_0199
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    0ch
loc_1_0923:
        pop    edi
        pop    esi
        pop    ebx
        call   sub_1_0108
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   ebx
        push   esi
        push   edi
        ret    8
sub_1_0937:
        push   edi
        mov    edi, ebx
        push   esi
        push   ebx
        sub    esp, 68h
        mov    eax, esp
        mov    edx, ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  08Bh, 00Dh, 0A4h, 019h, 000h, 000h
        lea    esi, [ecx + ebx]
        mov    ecx, dword ptr [ecx + ebx]
        xor    ebx, ebx
loc_1_0957:
        or     ecx, ecx
        je     SHORT loc_1_0995
        mov    edx, dword ptr [ecx - 4]
        cmp    dword ptr [ecx - 28h], edi
        jne    SHORT loc_1_0991
        pushad
        mov    byte ptr [eax], 35h
        mov    dl, byte ptr [ecx + 30h]
        mov    byte ptr [eax + 30h], dl
        mov    dword ptr [eax + 4], ecx
        movzx  edx, dl
        mov    ebx, eax
        db  08Bh, 034h, 095h, 08Ch, 01Ah, 000h, 000h
        or     dword ptr [ecx - 8], 1100h
        push   ecx
        call   esi
        pop    eax
        and    byte ptr [eax - 7], 0feh
        call   sub_1_16A7
        popad
loc_1_0991:
        mov    ecx, edx
        jmp    SHORT loc_1_0957
loc_1_0995:
        add    esp, 68h
        pop    ebx
        pop    edi
        pop    esi
        ret
loc_1_099C:
        push   ebx
        push   esi
        sub    esp, 68h
        mov    eax, esp
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        db  08Bh, 00Dh, 0A4h, 019h, 000h, 000h
        mov    ecx, dword ptr [ecx + ebx]
loc_1_09B2:
        or     ecx, ecx
        je     SHORT loc_1_09EE
        mov    edx, dword ptr [ecx - 4]
        cmp    dword ptr [ecx - 20h], edi
        jne    SHORT loc_1_09EA
        pushad
        or     dword ptr [ecx - 8], 1100h
        mov    byte ptr [eax], 35h
        mov    dl, byte ptr [ecx + 30h]
        mov    byte ptr [eax + 30h], dl
        mov    dword ptr [eax + 4], ecx
        movzx  edx, dl
        mov    ebx, eax
        push   ecx
        db  0FFh, 014h, 095h, 08Ch, 01Ah, 000h, 000h
        pop    eax
        and    byte ptr [eax - 7], 0feh
        call   sub_1_16A7
        popad
loc_1_09EA:
        mov    ecx, edx
        jmp    SHORT loc_1_09B2
loc_1_09EE:
        add    esp, 68h
        xor    eax, eax
        pop    esi
        pop    ebx
        ret
loc_1_09F6:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esi + 0ch]
        cmp    ecx, 100h
        jne    SHORT loc_1_0A13
        push   dword ptr [esi + 10h]
        call   sub_1_0199
        add    esp, 4
loc_1_0A0F:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0A13:
        cmp    ecx, 0
        je     SHORT loc_1_0A24
        cmp    ecx, -1
        je     SHORT loc_1_0A35
        mov    eax, 1
        jmp    SHORT loc_1_0A0F
loc_1_0A24:
        mov    ebx, dword ptr [esi + 2ch]
        call   sub_1_0A57
        xor    eax, eax
        mov    ecx, 0
        jmp    SHORT loc_1_0A0F
loc_1_0A35:
        mov    ebx, dword ptr [esi + 2ch]
        call   sub_1_0937
        call   sub_1_0B00
        xor    eax, eax
        jmp    SHORT loc_1_0A0F
        db  08Bh, 05Eh, 02Ch, 0E8h, 009h, 000h, 000h, 000h, 033h, 0C0h, 0B9h, 000h, 000h, 000h, 000h, 0EBh
        db  0B8h
sub_1_0A57:
        push   esi
        push   edi
        push   eax
        mov    ecx, ebx
        or     ecx, ecx
        je     SHORT loc_1_0A8E
        push   ecx
        push   1
        push   10h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
        or     eax, eax
        je     NEAR PTR loc_1_0AFD
        mov    dword ptr [eax], ebx
        push   edx
        db  08Bh, 015h, 0CCh, 019h, 000h, 000h
        mov    dword ptr [eax + 0ch], edx
        db  0A3h, 0CCh, 019h, 000h, 000h
        pop    edx
        mov    ebx, eax
        or     ecx, ecx
        jne    SHORT loc_1_0A97
loc_1_0A8E:
        db  083h, 03Dh, 0D8h, 019h, 000h, 000h, 001h
        jae    SHORT loc_1_0AC6
loc_1_0A97:
        push   ecx
        push   1
        push   25h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_1_0AFD
        db  083h, 005h, 0D8h, 019h, 000h, 000h, 001h
        mov    edi, eax
        push   edx
        db  08Bh, 015h, 0D0h, 019h, 000h, 000h
        mov    dword ptr [edi + 20h], edx
        pop    edx
        db  0A3h, 0D0h, 019h, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_1_0ACF
loc_1_0AC6:
        db  083h, 03Dh, 0DCh, 019h, 000h, 000h, 001h
        jae    SHORT loc_1_0AF9
loc_1_0ACF:
        push   1
        push   15h
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        or     eax, eax
        je     SHORT loc_1_0AFD
        db  083h, 005h, 0DCh, 019h, 000h, 000h, 001h
        mov    edi, eax
        push   edx
        db  08Bh, 015h, 0D4h, 019h, 000h, 000h
        mov    dword ptr [edi + 10h], edx
        pop    edx
        db  0A3h, 0D4h, 019h, 000h, 000h
        clc
loc_1_0AF9:
        pop    eax
        pop    edi
        pop    esi
        ret
loc_1_0AFD:
        stc
        jmp    SHORT loc_1_0AF9
sub_1_0B00:
        push   esi
        push   edi
        push   eax
        push   ecx
        db  08Bh, 00Dh, 0CCh, 019h, 000h, 000h
        or     ecx, ecx
        je     NEAR PTR loc_1_0C0E
loc_1_0B12:
        cmp    dword ptr [ecx], ebx
        je     SHORT loc_1_0B22
        mov    ecx, dword ptr [ecx + 0ch]
        or     ecx, ecx
        jne    SHORT loc_1_0B12
        jmp    NEAR PTR loc_1_0C0E
loc_1_0B22:
        sub    esp, 68h
        mov    eax, esp
        mov    ebx, eax
        mov    edx, dword ptr [ecx + 8]
        xor    edi, edi
        mov    esi, ecx
        mov    byte ptr [ebx], 12h
        xor    ch, ch
        or     edx, edx
        je     SHORT loc_1_0B91
        mov    edi, edx
        dec    edx
        push   esi
loc_1_0B3D:
        inc    edx
        mov    cl, 1
        mov    al, byte ptr [edx]
loc_1_0B42:
        mov    ah, cl
        not    ah
        test   cl, al
        je     SHORT loc_1_0B6D
        and    al, ah
        mov    ah, byte ptr [edi + 24h]
        mov    byte ptr [ebx + 30h], ah
        mov    byte ptr [ebx + 2], ch
        push   edi
        push   edx
        movzx  edx, ah
        db  08Bh, 03Ch, 095h, 08Ch, 01Ah, 000h, 000h
        push   ecx
        push   eax
        test   edi, edi
        je     SHORT loc_1_0B69
        call   edi
loc_1_0B69:
        pop    eax
        pop    ecx
        pop    edx
        pop    edi
loc_1_0B6D:
        add    ch, 1
        jb     SHORT loc_1_0B78
        shl    cl, 1
        jb     SHORT loc_1_0B3D
        jmp    SHORT loc_1_0B42
loc_1_0B78:
        xor    ch, ch
        mov    edx, dword ptr [edi + 20h]
        pushad
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        popad
        mov    edi, edx
        or     edx, edx
        jne    SHORT loc_1_0B3D
        pop    esi
loc_1_0B91:
        mov    edx, dword ptr [esi + 4]
        mov    byte ptr [ebx], 31h
loc_1_0B97:
        or     edx, edx
        je     SHORT loc_1_0BD9
        mov    ah, byte ptr [edx + 14h]
        mov    byte ptr [ebx + 30h], ah
        mov    byte ptr [ebx + 2], ch
        pushad
        lea    edi, [ebx + 1ah]
        lea    esi, [edx]
        mov    ecx, 4
        cld
        rep movsd
        movzx  edx, ah
        db  08Bh, 03Ch, 095h, 08Ch, 01Ah, 000h, 000h
        test   edi, edi
        je     SHORT loc_1_0BC2
        call   edi
loc_1_0BC2:
        popad
        mov    edi, edx
        mov    edx, dword ptr [edx + 10h]
        pushad
        push   0
        push   edi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        popad
        or     edx, edx
        jne    SHORT loc_1_0B97
loc_1_0BD9:
        db  0A1h, 0CCh, 019h, 000h, 000h
        cmp    eax, esi
        jne    SHORT loc_1_0BEC
        mov    eax, dword ptr [esi + 0ch]
        db  0A3h, 0CCh, 019h, 000h, 000h
        jmp    SHORT loc_1_0C00
loc_1_0BEC:
        cmp    dword ptr [eax + 0ch], esi
        je     SHORT loc_1_0BFA
        mov    eax, dword ptr [eax + 0ch]
        or     eax, eax
        jne    SHORT loc_1_0BEC
        jmp    SHORT loc_1_0C0B
loc_1_0BFA:
        mov    edi, dword ptr [esi + 0ch]
        mov    dword ptr [eax + 0ch], edi
loc_1_0C00:
        push   0
        push   esi
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        pop    ecx
        pop    ecx
loc_1_0C0B:
        add    esp, 68h
loc_1_0C0E:
        pop    ecx
        pop    eax
        pop    edi
        pop    esi
        ret
loc_1_0C13:
        db  08Bh, 03Dh, 0A4h, 019h, 000h, 000h
        or     byte ptr [edi + ebx + 4], 1
        cmp    dword ptr [edi + ebx], 0
        je     NEAR PTR loc_1_0D52
        db  00Fh, 0B7h, 005h, 00Eh, 000h, 000h, 000h
        db  003h, 005h, 004h, 000h, 000h, 000h
        db  0A3h, 0B0h, 019h, 000h, 000h
        sub    eax, eax
        db  083h, 03Dh, 0D1h, 01Ah, 000h, 000h, 000h
        jne    SHORT loc_1_0C46
        inc    eax
loc_1_0C46:
        call   0
        jb     NEAR PTR loc_1_0D54
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
loc_1_0C66:
        mov    ecx, dword ptr [edi + ebx]
        test   ecx, ecx
        je     NEAR PTR loc_1_0D23
        bts    dword ptr [ecx - 8], 6
        jb     NEAR PTR loc_1_0D16
        or     byte ptr [ecx - 7], 1
        test   byte ptr [ecx - 7], 2
        je     SHORT loc_1_0CAB
        mov    dword ptr [eax + 4], ecx
        pushad
        movzx  ecx, byte ptr [ecx + 30h]
        mov    byte ptr [eax + 30h], cl
        mov    byte ptr [eax], 35h
        db  08Bh, 014h, 08Dh, 08Ch, 01Ah, 000h, 000h
        mov    ebx, eax
        sub    eax, eax
        or     edx, edx
        je     SHORT loc_1_0CA5
        call   edx
loc_1_0CA5:
        mov    byte ptr [ebp + 1ch], al
        popad
        jmp    SHORT loc_1_0CF5
loc_1_0CAB:
        push   edi
        db  00Fh, 0B7h, 03Dh, 014h, 019h, 000h, 000h
        mov    word ptr [eax + 6], di
        mov    word ptr [ebp + 38h], di
        shl    edi, 4
        push   ecx
        sub    ecx, edi
        mov    word ptr [eax + 4], cx
        mov    ecx, eax
        sub    ecx, edi
        mov    word ptr [ebp + 10h], cx
        pop    ecx
        push   ecx
        mov    cl, byte ptr [ecx + 30h]
        mov    byte ptr [eax + 30h], cl
        pop    ecx
        pop    edi
        mov    byte ptr [eax], 35h
        push   eax
        mov    eax, 5ch
        test   byte ptr [ecx - 8], 80h
        je     SHORT loc_1_0CEF
        mov    eax, 2ah
        mov    byte ptr [ebp + 1dh], 1
loc_1_0CEF:
        call   sub_1_10E7
        pop    eax
loc_1_0CF5:
        cmp    byte ptr [ebp + 1ch], 0
        je     SHORT loc_1_0D16
        cmp    byte ptr [ebp + 1ch], 0ffh
        je     SHORT loc_1_0D16
        push   ecx
        db  00Fh, 0B7h, 00Dh, 010h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        db  089h, 00Dh, 0B0h, 019h, 000h, 000h
        pop    ecx
loc_1_0D16:
        xchg   ecx, eax
        call   sub_1_16A7
        mov    eax, ecx
        jmp    NEAR PTR loc_1_0C66
loc_1_0D23:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        call   sub_1_0020
loc_1_0D3D:
        mov    eax, 80001030h
        db  08Bh, 00Dh, 0B0h, 019h, 000h, 000h
        xor    edi, edi
        xor    esi, esi
        db  0CDh, 020h, 003h, 000h, 017h, 000h
loc_1_0D52:
        clc
        ret
loc_1_0D54:
        db  0C7h, 005h, 0B0h, 019h, 000h, 000h, 003h, 000h, 000h, 000h
loc_1_0D5E:
        mov    eax, dword ptr [edi + ebx]
        test   eax, eax
        je     SHORT loc_1_0D3D
        call   sub_1_16A7
        jmp    SHORT loc_1_0D5E
loc_1_0D6C:
        db  08Bh, 00Dh, 0A4h, 019h, 000h, 000h
        cmp    dword ptr [ecx + ebx], 0
        je     SHORT loc_1_0D96
        mov    eax, 1030h
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        xor    edi, edi
        xor    esi, esi
        db  0CDh, 020h, 004h, 000h, 017h, 000h
        stc
        ret
loc_1_0D96:
        clc
        ret
loc_1_0D98:
        mov    eax, 2ah
        cmp    byte ptr [ebp + 1dh], 1
        je     SHORT loc_1_0DE2
        cmp    byte ptr [ebp + 1dh], 4
        je     SHORT loc_1_0DE2
        db  08Bh, 00Dh, 0B4h, 019h, 000h, 000h
        db  08Bh, 015h, 0B8h, 019h, 000h, 000h
        mov    eax, ecx
        or     eax, edx
        je     SHORT loc_1_0DC1
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_1_0DC1:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    eax, 2ah
        call   sub_1_10E7
        db  0CDh, 020h, 086h, 080h, 001h, 000h
loc_1_0DDD:
        mov    eax, 5ch
loc_1_0DE2:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        jmp    NEAR PTR loc_1_0F1E
        db  02Eh, 08Bh, 0C0h
loc_1_0DF0:
        cmp    byte ptr [ebp + 1dh], 1
        je     SHORT loc_1_0E20
        cmp    byte ptr [ebp + 1dh], 4
        je     SHORT loc_1_0E20
        cmp    word ptr [ebp + 1ch], 500h
        je     SHORT loc_1_0E06
        stc
        ret
loc_1_0E06:
        sub    eax, eax
        mov    word ptr [ebp + 1ch], ax
        mov    word ptr [ebp + 10h], 10h
        mov    word ptr [ebp + 18h], 10h
        mov    word ptr [ebp + 14h], 40h
        clc
        ret
loc_1_0E20:
        db  066h, 08Bh, 035h, 014h, 019h, 000h, 000h
        cmp    word ptr [ebp + 38h], si
        jne    SHORT loc_1_0E37
        cmp    word ptr [ebp + 10h], 1000h
        jae    SHORT loc_1_0E37
        stc
        ret
loc_1_0E37:
        movzx  esi, word ptr [ebp + 38h]
        shl    esi, 4
        movzx  ecx, word ptr [ebp + 10h]
        add    esi, ecx
        movzx  ecx, byte ptr [esi]
        and    ecx, 0ffffff7fh
        cmp    ecx, 0fh
        ja     SHORT loc_1_0E5F
        db  080h, 0B9h, 008h, 01Ah, 000h, 000h, 000h
        je     NEAR PTR loc_1_0EE6
loc_1_0E5F:
        movzx  ecx, byte ptr [esi + 30h]
        cmp    cl, 8
        jae    NEAR PTR loc_1_1041
        db  083h, 03Ch, 08Dh, 08Ch, 01Ah, 000h, 000h, 000h
        jne    NEAR PTR loc_1_0F1E
        push   eax
        push   0
        push   40h
        push   esi
        db  0CDh, 020h, 05Fh, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        pop    eax
        je     NEAR PTR loc_1_0F1E
        db  0CDh, 020h, 008h, 000h, 015h, 000h
        jne    SHORT loc_1_0F0E
        movzx  edi, byte ptr [esi]
        and    edi, 0ffffff7fh
        db  080h, 0BFh, 008h, 01Ah, 000h, 000h, 004h
        je     SHORT loc_1_0EE6
        db  080h, 0BFh, 008h, 01Ah, 000h, 000h, 000h
        jne    SHORT loc_1_0EC2
        cmp    dword ptr [esi + 4], 0
        je     SHORT loc_1_0EE6
        cmp    word ptr [esi + 8], 0
        je     SHORT loc_1_0EE6
loc_1_0EC2:
        push   eax
        movzx  eax, word ptr [esi + 6]
        shl    eax, 4
        movzx  ecx, word ptr [esi + 4]
        add    eax, ecx
        movzx  ecx, word ptr [esi + 8]
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 05Fh, 000h, 001h, 000h
        add    esp, 0ch
        test   eax, eax
        pop    eax
        je     SHORT loc_1_0F1E
loc_1_0EE6:
        db  08Bh, 03Dh, 0A4h, 019h, 000h, 000h
        bts    dword ptr [edi + ebx + 4], 1
        jb     SHORT loc_1_0F0C
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        xor    eax, eax
        db  0BEh, 0D7h, 010h, 000h, 000h
        db  0CDh, 020h, 00Dh, 000h, 001h, 000h
loc_1_0F0C:
        stc
        ret
loc_1_0F0E:
        cmp    byte ptr [esi + 2ah], 0
        jne    SHORT loc_1_0EE6
        db  0A0h, 08Ah, 01Ah, 000h, 000h
        mov    byte ptr [esi + 2ah], al
        jmp    SHORT loc_1_0EE6
loc_1_0F1E:
        mov    edx, eax
        db  08Bh, 03Dh, 0A4h, 019h, 000h, 000h
        mov    cx, word ptr [ebp + 38h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 10h]
        xchg   esi, eax
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        xchg   esi, eax
        test   byte ptr [edi + ebx + 4], 1
        jne    NEAR PTR loc_1_1052
sub_1_0F48:
        movzx  eax, byte ptr [esi + 30h]
        cmp    al, 8
        jae    NEAR PTR loc_1_1041
        db  00Fh, 0A3h, 005h, 0D5h, 01Ah, 000h, 000h
        jb     SHORT loc_1_0F6A
        db  00Fh, 0A3h, 005h, 0CDh, 01Ah, 000h, 000h
        jae    NEAR PTR loc_1_1041
loc_1_0F6A:
        db  08Bh, 004h, 085h, 08Ch, 01Ah, 000h, 000h
        call   0
        jb     NEAR PTR loc_1_1052
        and    dword ptr [eax - 8], 200h
        or     byte ptr [eax - 8], 21h
        mov    dword ptr [eax - 10h], ecx
        test   ecx, ecx
        je     SHORT loc_1_0FAA
        push   ecx
        push   eax
        call   sub_1_0108
        mov    ecx, eax
        pop    eax
        mov    dword ptr [eax - 20h], ecx
        pop    ecx
        cmp    ecx, 1
        jne    SHORT loc_1_0FB4
        mov    dword ptr [eax - 10h], esi
        or     byte ptr [eax - 7], 8
        jmp    SHORT loc_1_0FB4
loc_1_0FAA:
        mov    dword ptr [eax - 10h], esi
        or     byte ptr [eax - 7], 4
        mov    dword ptr [eax - 20h], ecx
loc_1_0FB4:
        xor    ecx, ecx
        mov    dword ptr [eax - 18h], ecx
        mov    dword ptr [eax - 1ch], ecx
        mov    dword ptr [eax - 0ch], ebx
        mov    dword ptr [eax - 14h], esi
        cmp    edx, 2ah
        jne    SHORT loc_1_0FCB
        or     byte ptr [eax - 8], 80h
loc_1_0FCB:
        test   byte ptr [eax - 7], 4
        jne    SHORT loc_1_0FED
        test   byte ptr [eax - 7], 8
        jne    SHORT loc_1_0FED
        mov    ecx, dword ptr [eax - 20h]
        mov    ecx, dword ptr [ecx + 24h]
        jecxz  loc_1_0FE4
        sub    ecx, ecx
        mov    dword ptr [eax - 20h], ecx
loc_1_0FE4:
        test   byte ptr [ebx], 20h
        je     SHORT loc_1_0FED
        or     byte ptr [eax - 8], 10h
loc_1_0FED:
        mov    ecx, eax
        xchg   dword ptr [edi + ebx], ecx
        mov    dword ptr [eax - 4], ecx
        mov    edi, eax
        cld
        mov    ecx, 10h
        rep movsd
        xor    esi, esi
        movzx  ecx, byte ptr [eax]
        test   cl, 80h
        jne    SHORT loc_1_102E
        test   byte ptr [eax - 7], 4
        jne    SHORT loc_1_106E
        db  080h, 03Dh, 08Bh, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_106E
        db  08Ah, 089h, 008h, 01Ah, 000h, 000h
        cmp    cl, 14h
        je     SHORT loc_1_1028
        cmp    cl, 1ch
        jb     SHORT loc_1_106E
loc_1_1028:
        or     byte ptr [eax], 80h
        mov    esi, dword ptr [eax - 14h]
loc_1_102E:
        or     byte ptr [eax - 8], 6
        test   byte ptr [eax - 7], 2
        je     SHORT loc_1_1065
        db  0C7h, 040h, 02Ch, 005h, 014h, 000h, 000h
        jmp    SHORT loc_1_106E
loc_1_1041:
        db  0A1h, 0D5h, 01Ah, 000h, 000h
        db  00Bh, 005h, 0CDh, 01Ah, 000h, 000h
        je     SHORT loc_1_105F
        mov    al, 23h
        jmp    SHORT loc_1_1054
loc_1_1052:
        mov    al, 22h
loc_1_1054:
        mov    byte ptr [esi + 1], al
        mov    byte ptr [esi + 31h], al
        mov    byte ptr [ebp + 1ch], al
        jmp    SHORT loc_1_10D5
loc_1_105F:
        mov    byte ptr [ebp + 1ch], 23h
        jmp    SHORT loc_1_10D5
loc_1_1065:
        db  08Bh, 00Dh, 0ACh, 019h, 000h, 000h
        mov    dword ptr [eax + 2ch], ecx
loc_1_106E:
        movzx  edi, byte ptr [eax]
        and    edi, 7fh
        db  00Fh, 0B6h, 0BFh, 008h, 01Ah, 000h, 000h
        push   esi
        db  0FFh, 097h, 0E0h, 019h, 000h, 000h
        pop    ecx
        jecxz  loc_1_10D5
        cmp    byte ptr [ecx + 31h], 0ffh
        jne    SHORT loc_1_10CF
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
loc_1_10A6:
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        cmp    byte ptr [ecx + 31h], 0ffh
        jne    SHORT loc_1_10BA
        db  0CDh, 020h, 038h, 000h, 001h, 000h
        jmp    SHORT loc_1_10A6
loc_1_10BA:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
loc_1_10CF:
        mov    al, byte ptr [ecx + 1]
        mov    byte ptr [ebp + 1ch], al
loc_1_10D5:
        clc
        ret
loc_1_10D7:
        db  003h, 01Dh, 0A4h, 019h, 000h, 000h
        and    byte ptr [ebx + 4], 0fdh
        db  0CDh, 020h, 020h, 080h, 001h, 000h
sub_1_10E7:
        push   ecx
        push   edx
        push   eax
        mov    ax, word ptr [ebp + 2ch]
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        pop    eax
        db  08Bh, 00Dh, 0DDh, 01Ah, 000h, 000h
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        mov    eax, dword ptr [eax*4]
        ror    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        shr    eax, 10h
        db  0CDh, 020h, 04Dh, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        pop    edx
        pop    ecx
        ret
sub_1_1129:
        mov    edi, eax
        test   byte ptr [eax - 7], 2
        jne    SHORT loc_1_119B
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        push   ecx
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        pop    ecx
        push   dword ptr [ebp + 38h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 2ch]
        and    byte ptr [ebp + 2dh], 0fbh
        db  00Fh, 0B7h, 035h, 014h, 019h, 000h, 000h
        mov    word ptr [ebp + 38h], si
        shl    esi, 4
        sub    eax, esi
        mov    word ptr [ebp + 10h], ax
        mov    eax, edx
        call   sub_1_10E7
        mov    eax, edi
        pop    dword ptr [ebp + 2ch]
        pop    dword ptr [ebp + 10h]
        pop    dword ptr [ebp + 38h]
        db  0CDh, 020h, 020h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
loc_1_1183:
        and    byte ptr [eax - 8], 0feh
        cmp    byte ptr [ebp + 1ch], 0
        je     SHORT loc_1_1196
        and    byte ptr [eax - 8], 0f9h
        call   sub_1_15A9
loc_1_1196:
        jmp    NEAR PTR sub_1_164D
loc_1_119B:
        pushad
        mov    ebx, eax
        movzx  ecx, byte ptr [eax + 30h]
        db  0FFh, 014h, 08Dh, 08Ch, 01Ah, 000h, 000h
        mov    byte ptr [ebp + 1ch], al
        popad
        jmp    SHORT loc_1_1183
loc_1_11AF:
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR sub_1_1129
        cmp    word ptr [esi + 8], 0
        je     NEAR PTR sub_1_1129
        test   byte ptr [eax - 7], 6
        jne    NEAR PTR loc_1_1261
        test   byte ptr [ebx], 20h
        je     SHORT loc_1_124D
        lar    cx, word ptr [eax + 6]
        je     SHORT loc_1_124D
        jmp    NEAR PTR sub_1_1129
loc_1_11DF:
        test   byte ptr [eax - 7], 6
        jne    SHORT loc_1_1261
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_1261
        test   byte ptr [eax], 80h
        je     NEAR PTR sub_1_1129
        or     byte ptr [eax - 8], 8
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        db  0FFh, 005h, 0C8h, 019h, 000h, 000h
        db  083h, 03Dh, 0C8h, 019h, 000h, 000h, 001h
        ja     NEAR PTR sub_1_1129
        db  089h, 01Dh, 0C4h, 019h, 000h, 000h
        db  08Bh, 035h, 0A4h, 019h, 000h, 000h
        test   byte ptr [esi + ebx + 4], 4
        jne    NEAR PTR sub_1_1129
        push   eax
        db  0A1h, 0BCh, 019h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_1247
        db  0BEh, 0CFh, 015h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 0C0h, 019h, 000h, 000h
loc_1_1247:
        pop    eax
        jmp    NEAR PTR sub_1_1129
loc_1_124D:
        test   byte ptr [eax - 7], 6
        jne    SHORT loc_1_1261
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_1261
        test   byte ptr [eax], 80h
        je     NEAR PTR sub_1_1129
loc_1_1261:
        movzx  ecx, word ptr [eax + 8]
        mov    esi, dword ptr [eax + 4]
        call   sub_1_003B
        jb     SHORT loc_1_127A
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax - 18h], esi
        jmp    NEAR PTR sub_1_1129
loc_1_127A:
        db  080h, 03Dh, 089h, 01Ah, 000h, 000h, 000h
        je     SHORT loc_1_12AC
        test   dword ptr [eax - 8], 600h
        jne    SHORT loc_1_12AC
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_12AC
        xor    esi, esi
        xchg   dword ptr [eax - 1ch], esi
        mov    edi, dword ptr [eax + 0ch]
        call   sub_1_00E2
        mov    esi, dword ptr [eax - 14h]
        mov    esi, dword ptr [esi + 0ch]
        mov    dword ptr [eax + 0ch], esi
        jmp    NEAR PTR loc_1_11DF
loc_1_12AC:
        mov    byte ptr [eax + 1], 22h
        mov    byte ptr [eax + 31h], 22h
        and    byte ptr [eax - 8], 0f8h
        mov    byte ptr [ebp + 1ch], 22h
        call   sub_1_15A9
        push   ecx
        call   sub_1_164D
        pop    ecx
        db  00Fh, 0B6h, 005h, 088h, 01Ah, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_1345
        db  08Dh, 03Dh, 018h, 019h, 000h, 000h
        cld
        db  0C6h, 005h, 088h, 01Ah, 000h, 000h, 000h
        add    ecx, 0fffh
        shr    ecx, 0ch
        add    eax, ecx
        shl    eax, 2
        mov    esi, eax
        xor    edx, edx
        mov    ecx, 64h
        idiv   ecx
        test   al, al
        je     SHORT loc_1_1301
        add    al, 30h
        stosb
loc_1_1301:
        mov    eax, edx
        aam    0ah
        add    eax, 3030h
        cmp    esi, 0ah
        jb     SHORT loc_1_1314
        xchg   al, ah
        stosb
        mov    al, ah
loc_1_1314:
        stosb
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        db  0B8h, 018h, 019h, 000h, 000h
        push   eax
        push   0c0004h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 059h, 001h, 001h, 000h
        mov    ecx, eax
        mov    eax, 1030h
        xor    edi, edi
        xor    esi, esi
        db  0CDh, 020h, 004h, 000h, 017h, 000h
        add    esp, dword ptr [esp]
loc_1_1345:
        ret
loc_1_1346:
        test   byte ptr [eax - 7], 6
        jne    SHORT loc_1_135A
        test   byte ptr [ebx], 20h
        jne    SHORT loc_1_135A
        test   byte ptr [eax], 80h
        je     NEAR PTR sub_1_1129
loc_1_135A:
        movzx  ecx, word ptr [eax + 0ah]
        mov    esi, dword ptr [eax + 0ch]
        call   sub_1_003B
        jb     NEAR PTR loc_1_127A
        mov    dword ptr [eax + 0ch], edi
        mov    dword ptr [eax - 1ch], esi
        jmp    NEAR PTR loc_1_1261
loc_1_1377:
        mov    edi, dword ptr [eax + 4]
        db  08Bh, 00Dh, 0A4h, 019h, 000h, 000h
        mov    ecx, dword ptr [ebx + ecx]
loc_1_1383:
        jecxz  loc_1_139B
        cmp    dword ptr [ecx - 10h], edi
        je     SHORT loc_1_138F
        mov    ecx, dword ptr [ecx - 4]
        jmp    SHORT loc_1_1383
loc_1_138F:
        test   byte ptr [ecx - 8], 2
        jne    SHORT loc_1_13BA
        test   byte ptr [ecx - 8], 4
        je     SHORT loc_1_13BA
loc_1_139B:
        mov    byte ptr [eax + 1], 24h
        mov    byte ptr [eax + 31h], 24h
        mov    byte ptr [ebp + 1ch], 24h
        and    byte ptr [eax - 8], 0f8h
        call   sub_1_15A9
        call   sub_1_164D
        and    byte ptr [eax - 8], 0dfh
        ret
loc_1_13BA:
        test   byte ptr [eax - 7], 2
        je     SHORT loc_1_13C5
        mov    dword ptr [eax + 4], ecx
        jmp    SHORT loc_1_13DB
loc_1_13C5:
        db  00Fh, 0B7h, 035h, 014h, 019h, 000h, 000h
        mov    word ptr [eax + 6], si
        shl    esi, 4
        push   ecx
        sub    ecx, esi
        mov    word ptr [eax + 4], cx
        pop    ecx
loc_1_13DB:
        or     byte ptr [ecx - 7], 1
        call   sub_1_1129
        and    byte ptr [ecx - 7], 0feh
        cmp    byte ptr [ebp + 1ch], 0
        jne    SHORT loc_1_1404
        xchg   ecx, eax
        call   sub_1_15A9
        xchg   ecx, eax
        test   word ptr [ecx - 8], 10fh
        jne    SHORT loc_1_1404
        xchg   ecx, eax
        call   sub_1_16A7
        xchg   ecx, eax
loc_1_1404:
        ret
loc_1_1405:
        mov    edx, ebx
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebp, dword ptr [ebx + 8]
        jmp    SHORT loc_1_1428
loc_1_1412:
        db  0CDh, 020h, 047h, 000h, 001h, 000h
        db  00Fh, 0B7h, 015h, 014h, 019h, 000h, 000h
        shl    edx, 4
        movzx  esi, word ptr [ebp + 10h]
        add    edx, esi
loc_1_1428:
        and    byte ptr [edx - 8], 0fdh
        test   byte ptr [edx - 7], 4
        jne    SHORT loc_1_146C
        test   byte ptr [edx - 7], 8
        jne    SHORT loc_1_146C
        mov    ebx, dword ptr [edx - 0ch]
        db  08Bh, 03Dh, 0A4h, 019h, 000h, 000h
        test   byte ptr [edi + ebx + 4], 1
        jne    SHORT loc_1_149C
        test   dword ptr [edx - 8], 1000h
        jne    SHORT loc_1_149D
        db  0CDh, 020h, 039h, 000h, 001h, 000h
        db  0BEh, 0C6h, 014h, 000h, 000h
        mov    ecx, 9
        mov    eax, 1000h
        db  0CDh, 020h, 05Ah, 081h, 001h, 000h
loc_1_146C:
        xchg   edx, eax
        and    byte ptr [eax - 8], 0fbh
        call   sub_1_15A9
        test   byte ptr [eax - 8], 40h
        jne    NEAR PTR loc_1_159C
        mov    ebx, dword ptr [eax - 14h]
        call   sub_1_164D
        and    byte ptr [eax - 8], 0dfh
        mov    ecx, dword ptr [ebx + 2ch]
        jecxz  loc_1_149C
        test   byte ptr [eax - 7], 8
        jne    SHORT loc_1_14A7
        mov    al, byte ptr [ebx + 1]
        call   ecx
loc_1_149C:
        ret
loc_1_149D:
        push   eax
        mov    eax, edx
        call   sub_1_16A7
        pop    eax
        ret
loc_1_14A7:
        push   ebp
        push   ebx
        push   esi
        push   edi
        movzx  edx, byte ptr [ebx + 1]
        push   edx
        push   dword ptr [eax - 20h]
        push   ebx
        push   dword ptr [ebx + 2ch]
        db  0CDh, 020h, 002h, 000h, 02Ah, 000h
        add    esp, 0ch
        pop    eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_14C6:
        db  0CDh, 020h, 039h, 000h, 001h, 000h
        db  08Bh, 03Dh, 0A4h, 019h, 000h, 000h
        test   byte ptr [edi + ebx + 4], 1
        jne    NEAR PTR loc_1_159B
        mov    eax, edx
        and    byte ptr [eax - 8], 0fbh
        call   sub_1_15A9
        test   byte ptr [eax - 8], 40h
        jne    NEAR PTR loc_1_159C
        call   sub_1_16EC
        mov    ecx, dword ptr [eax - 14h]
        test   byte ptr [ecx], 80h
        je     NEAR PTR loc_1_1592
        mov    ecx, dword ptr [ecx + 2ch]
        test   ecx, ecx
        je     NEAR PTR loc_1_1592
        mov    ecx, 5
        db  0CDh, 020h, 01Fh, 000h, 001h, 000h
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        test   byte ptr [eax - 8], 10h
        jne    SHORT loc_1_1536
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        jmp    SHORT loc_1_153C
loc_1_1536:
        db  0CDh, 020h, 083h, 000h, 001h, 000h
loc_1_153C:
        mov    esi, dword ptr [eax - 14h]
        mov    esi, dword ptr [esi + 2ch]
        mov    edx, dword ptr [eax - 10h]
        mov    word ptr [ebp + 10h], dx
        shr    edx, 10h
        mov    word ptr [ebp + 38h], dx
        mov    dl, byte ptr [eax + 1]
        mov    byte ptr [ebp + 1ch], dl
        call   sub_1_164D
        and    byte ptr [eax - 8], 0dfh
        mov    ecx, esi
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 04Ch, 000h, 001h, 000h
        and    byte ptr [ebp + 2dh], 0fbh
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  0CDh, 020h, 020h, 080h, 001h, 000h
loc_1_1592:
        call   sub_1_164D
        and    byte ptr [eax - 8], 0dfh
loc_1_159B:
        ret
loc_1_159C:
        test   word ptr [eax - 8], 10fh
        jne    SHORT loc_1_159B
        jmp    NEAR PTR sub_1_16A7
sub_1_15A9:
        btr    dword ptr [eax - 8], 3
        jae    SHORT loc_1_15CE
        db  0FFh, 00Dh, 0C8h, 019h, 000h, 000h
        jne    SHORT loc_1_15C8
        push   esi
        xor    esi, esi
        db  087h, 035h, 0C0h, 019h, 000h, 000h
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
loc_1_15C8:
        db  0CDh, 020h, 020h, 000h, 001h, 000h
loc_1_15CE:
        ret
loc_1_15CF:
        db  0C7h, 005h, 0C0h, 019h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 036h, 000h, 001h, 000h
        cmp    eax, 1
        ja     SHORT loc_1_15F1
        db  03Bh, 01Dh, 0C4h, 019h, 000h, 000h
        jne    SHORT loc_1_15F1
        test   byte ptr [ebx], 1
        jne    SHORT loc_1_164C
loc_1_15F1:
        db  08Bh, 01Dh, 0C4h, 019h, 000h, 000h
        mov    eax, 1034h
        db  00Fh, 0B7h, 00Dh, 014h, 000h, 000h, 000h
        db  003h, 00Dh, 004h, 000h, 000h, 000h
        xor    edi, edi
        xor    esi, esi
        db  0CDh, 020h, 003h, 000h, 017h, 000h
        cmp    eax, 6
        jne    SHORT loc_1_162E
        xor    ecx, ecx
        db  089h, 00Dh, 0C4h, 019h, 000h, 000h
        db  089h, 00Dh, 0C8h, 019h, 000h, 000h
        db  0CDh, 020h, 02Eh, 000h, 001h, 000h
        jmp    SHORT loc_1_164C
loc_1_162E:
        mov    esi, 1
        xor    edi, edi
        xor    edx, edx
        mov    eax, 0fh
        db  0CDh, 020h, 093h, 000h, 001h, 000h
        db  003h, 01Dh, 0A4h, 019h, 000h, 000h
        or     byte ptr [ebx + 4], 4
loc_1_164C:
        ret
sub_1_164D:
        pushad
        test   byte ptr [eax - 8], 20h
        je     SHORT loc_1_1698
        call   sub_1_16EC
        mov    edi, dword ptr [eax - 14h]
        mov    esi, eax
        cld
        inc    esi
        inc    edi
        movsb
        movsw
        add    esi, 4
        add    edi, 4
        movsw
        mov    bl, byte ptr [eax]
        and    ebx, 7fh
        cmp    ebx, 17h
        je     SHORT loc_1_167D
        mov    ecx, 11h
        jmp    SHORT loc_1_1688
loc_1_167D:
        add    esi, 10h
        add    edi, 10h
        mov    ecx, 9
loc_1_1688:
        rep movsw
        add    esi, 4
        add    edi, 4
        mov    ecx, 4
        rep movsd
loc_1_1698:
        test   word ptr [eax - 8], 10fh
        jne    SHORT loc_1_16A5
        call   sub_1_16A7
loc_1_16A5:
        popad
        ret
sub_1_16A7:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax - 18h]
        mov    edi, dword ptr [eax + 4]
        call   sub_1_00E2
        mov    esi, dword ptr [eax - 1ch]
        mov    edi, dword ptr [eax + 0ch]
        call   sub_1_00E2
        mov    ebx, dword ptr [eax - 0ch]
        db  003h, 01Dh, 0A4h, 019h, 000h, 000h
        lea    ebx, [ebx + 4]
loc_1_16CC:
        cmp    dword ptr [ebx - 4], eax
        je     SHORT loc_1_16DA
        mov    ebx, dword ptr [ebx - 4]
        test   ebx, ebx
        jne    SHORT loc_1_16CC
        jmp    SHORT loc_1_16E5
loc_1_16DA:
        mov    esi, dword ptr [eax - 4]
        mov    dword ptr [ebx - 4], esi
        call   sub_1_0020
loc_1_16E5:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  02Eh, 08Bh, 0C0h
sub_1_16EC:
        test   byte ptr [eax - 7], 4
        jne    SHORT loc_1_1721
        test   byte ptr [eax - 8], 10h
        je     SHORT loc_1_1721
        push   ebx
        push   ecx
        push   edx
        movzx  edx, word ptr [eax - 0eh]
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        xchg   ebx, eax
        push   0
        push   edx
        push   eax
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        movzx  edx, word ptr [ebx - 10h]
        add    edx, eax
        mov    eax, ebx
        mov    dword ptr [eax - 14h], edx
        pop    edx
        pop    ecx
        pop    ebx
loc_1_1721:
        ret
sub_1_1722:
        pushad
        mov    edi, esi
        mov    ebx, dword ptr [edi + 4]
        inc    ebx
        movzx  ecx, word ptr [edi + 8]
        db  0BEh, 08Ch, 01Ah, 000h, 000h
        sub    edx, edx
        jecxz  loc_1_175C
        dec    ecx
loc_1_1737:
        lodsd
        test   eax, eax
        je     SHORT loc_1_1744
        jecxz  loc_1_175C
        mov    byte ptr [ebx], dl
        inc    ebx
        dec    ecx
        inc    dh
loc_1_1744:
        inc    dl
        db  081h, 0FEh, 0ACh, 01Ah, 000h, 000h
        jne    SHORT loc_1_1737
        mov    byte ptr [edi + 1], 0
loc_1_1752:
        mov    ebx, dword ptr [edi + 4]
        mov    byte ptr [ebx], dh
        popad
        sub    eax, eax
        clc
        ret
loc_1_175C:
        mov    byte ptr [edi + 1], 1
        jmp    SHORT loc_1_1752
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1770:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        mov    esi, dword ptr [esp + 18h]
loc_1_177C:
        mov    ecx, 0ah
        mov    eax, esi
        sub    edx, edx
        div    ecx
        lea    eax, [esp + 0eh]
        sub    eax, edi
        inc    edi
        mov    byte ptr [eax], dl
        mov    eax, esi
        sub    edx, edx
        div    ecx
        mov    esi, eax
        cmp    edi, 3
        jl     SHORT loc_1_177C
        sub    edx, edx
        sub    eax, eax
loc_1_17A1:
        cmp    byte ptr [esp + edx + 0ch], al
        jne    SHORT loc_1_17AD
        inc    edx
        cmp    edx, 2
        jl     SHORT loc_1_17A1
loc_1_17AD:
        sub    eax, eax
        cmp    edx, 3
        jge    SHORT loc_1_17D1
        mov    eax, 3
        sub    eax, edx
        mov    ecx, dword ptr [esp + 14h]
loc_1_17BF:
        mov    bl, byte ptr [esp + edx + 0ch]
        add    bl, 30h
        mov    byte ptr [ecx], bl
        inc    ecx
        inc    edx
        cmp    edx, 3
        jl     SHORT loc_1_17BF
        jmp    SHORT loc_1_17D5
loc_1_17D1:
        mov    ecx, dword ptr [esp + 14h]
loc_1_17D5:
        mov    byte ptr [ecx], 0
        inc    eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_17E0:
        sub    esp, 4ch
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 58h]
        or     ecx, ecx
        je     NEAR PTR loc_1_1905
        mov    dword ptr [esp + 10h], 40h
        lea    eax, [esp + 10h]
        push   1
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        push   1
        db  068h, 060h, 01Bh, 000h, 000h
        db  068h, 070h, 01Bh, 000h, 000h
        push   ecx
        db  0E8h, 001h, 0E8h, 0FEh, 0FFh
        add    esp, 1ch
        or     eax, eax
        jne    NEAR PTR loc_1_1905
        sub    esi, esi
        db  0BFh, 044h, 01Bh, 000h, 000h
loc_1_1829:
        mov    eax, dword ptr [edi]
        lea    ecx, [esp + 14h]
loc_1_182F:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    SHORT loc_1_184F
        or     dl, dl
        je     SHORT loc_1_184B
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    SHORT loc_1_184F
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    SHORT loc_1_182F
loc_1_184B:
        sub    eax, eax
        jmp    SHORT loc_1_1854
loc_1_184F:
        sbb    eax, eax
        sbb    eax, -1
loc_1_1854:
        or     eax, eax
        je     SHORT loc_1_1864
        add    edi, 0ch
        inc    esi
        db  081h, 0FFh, 05Ch, 01Bh, 000h, 000h
        jb     SHORT loc_1_1829
loc_1_1864:
        cmp    esi, 2
        je     NEAR PTR loc_1_1905
        imul   esi, esi, 0ch
        db  066h, 08Bh, 086h, 04Eh, 01Bh, 000h, 000h
        lea    ecx, [eax + 1]
        db  066h, 089h, 08Eh, 04Eh, 01Bh, 000h, 000h
        movsx  eax, ax
        push   eax
        db  00Fh, 0BFh, 086h, 04Ch, 01Bh, 000h, 000h
        db  003h, 086h, 048h, 01Bh, 000h, 000h
        push   eax
        call   sub_1_1770
        add    esp, 8
        db  080h, 03Dh, 05Ch, 01Bh, 000h, 000h, 000h
        je     SHORT loc_1_18BD
        db  068h, 0F0h, 01Ah, 000h, 000h
        push   80000002h
        db  0E8h, 055h, 0E7h, 0FEh, 0FFh
        add    esp, 8
        db  0C6h, 005h, 05Ch, 01Bh, 000h, 000h, 000h
loc_1_18BD:
        lea    eax, [esp + 8]
        push   eax
        db  068h, 0F0h, 01Ah, 000h, 000h
        push   80000002h
        db  0E8h, 02Fh, 0E7h, 0FEh, 0FFh
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_1905
        lea    eax, [esp + 0ch]
        push   dword ptr [esp + 5ch]
        push   eax
        call   sub_1_1770
        add    esp, 8
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        push   1
        push   0
        db  0FFh, 0B6h, 048h, 01Bh, 000h, 000h
        push   dword ptr [esp + 1ch]
        db  0E8h, 00Eh, 0E7h, 0FEh, 0FFh
        add    esp, 18h
loc_1_1905:
        pop    edi
        pop    esi
        add    esp, 4ch
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1918:
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        db  061h, 007h, 000h, 000h, 0DFh, 007h, 000h, 000h, 00Bh, 008h, 000h, 000h, 069h, 008h, 000h, 000h
        db  0E3h, 007h, 000h, 000h, 06Dh, 007h, 000h, 000h, 07Bh, 008h, 000h, 000h, 075h, 008h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 014h, 000h, 003h, 000h, 000h, 000h, 056h, 04Eh, 045h, 054h
        db  042h, 049h, 04Fh, 053h, 000h, 000h, 000h, 054h, 093h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  01Ch, 019h, 000h, 000h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Fh, 009h, 000h, 000h, 001h, 000h, 000h, 000h
        db  023h, 009h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_19BC:
        mov    byte ptr [ebx], dl
        add    byte ptr [eax], al
loc_1_19C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_19E0:
        scasd
        adc    dword ptr [eax], eax
        db  000h, 029h
        adc    dword ptr [eax], eax
        db  000h, 0DFh
        adc    dword ptr [eax], eax
        db  000h, 0DFh
        adc    dword ptr [eax], eax
        db  000h, 0DFh
        adc    dword ptr [eax], eax
        db  000h, 046h, 013h
        add    byte ptr [eax], al
        ja     SHORT loc_1_1A0D
        add    byte ptr [eax], al
        dec    ebp
        adc    al, byte ptr [eax]
        db  000h, 04Dh, 012h
        add    byte ptr [eax], al
        dec    ebp
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A0D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esp + eax], al
        add    al, 0
        sbb    al, 20h
        and    byte ptr [eax + eax], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        sbb    al, 20h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 4
        add    al, 0ch
        or     al, 18h
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    al, 1ch
        add    al, 4
        add    al, 1ch
        and    byte ptr [eax], bl
        and    byte ptr [eax], ah
        sbb    al, 0
        add    al, 0
loc_1_1A89:
        add    byte ptr [eax], al
loc_1_1A8B:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1ACD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1AD1:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1AD5:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1AD9:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1ADD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1AE1:
        add    byte ptr [eax], al
loc_1_1AE3:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 6fh], dl
        je     NEAR PTR loc_1_1B6C
        popad
        db  072h, 065h
        pop    esp
        dec    ebp
        imul   esp, dword ptr [ebx + 72h], 666f736fh
        db  074h, 05Ch
        push   edx
        db  070h, 063h
        pop    esp
        dec    esi
        db  065h, 074h, 062h
        imul   ebp, dword ptr [edi + 73h], 454e0000h
        push   esp
        inc    edx
        inc    ebp
        push   ebp
        dec    ecx
        add    byte ptr [esi + 63h], ch
        popad
        arpl   word ptr [esi + 5fh], bp
        outsb
        bound  ebx, qword ptr [edi + 6eh]
        bound  edi, qword ptr [edi]
        aas
        aas
        add    byte ptr [eax], al
loc_1_1B28:
        dec    esi
        push   edi
        dec    esi
        inc    edx
        dec    esp
        dec    ecx
        dec    esi
        dec    ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1B34:
        outsb
        arpl   word ptr [ecx + 63h], sp
        outsb
        pop    edi
        outsb
        bound  ebx, qword ptr [edi + 69h]
        db  070h, 078h
        aas
        aas
        aas
        db  000h, 010h
        sbb    eax, dword ptr [eax]
        db  000h, 018h
        sbb    eax, dword ptr [eax]
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        db  000h, 028h
        sbb    eax, dword ptr [eax]
        db  000h, 034h, 01Bh
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
loc_1_1B5C:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_1B60:
        dec    esp
        outsd
        inc    esp
        db  072h, 069h
        db  076h, 065h
        db  072h, 04Eh
        popad
        insd
loc_1_1B6C:
        add    byte ptr gs:[eax], al
        add    byte ptr [esi + 44h], cl
        dec    ecx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
_LTEXT ends
        end
