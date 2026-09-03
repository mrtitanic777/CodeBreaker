; e30n3_pcode_04.asm -- E30N3 object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_4_0000:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        lea    esi, [edi + 1789h]
        push   esi
        call   sub_4_1DA0
        test   al, al
        jne    SHORT loc_4_0026
        push   esi
        call   sub_4_1F50
        xor    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_0026:
        and    ax, 0ffh
        or     word ptr [edi + 48h], ax
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_0040:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 0
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 13h], 0
        jne    SHORT loc_4_00C4
        mov    bl, byte ptr [esi + 48h]
        test   bl, 1fh
        mov    word ptr [esi + 48h], 0
        je     SHORT loc_4_00C4
        inc    dword ptr [esi + 4ah]
        mov    byte ptr [esi + 13h], 1
        test   bl, 10h
        je     SHORT loc_4_0080
        push   esi
        call   sub_4_02F0
        test   al, al
        je     SHORT loc_4_007D
        mov    byte ptr [ebp - 1], 1
loc_4_007D:
        and    bl, 0eah
loc_4_0080:
        test   bl, 1
        je     SHORT loc_4_0096
        push   esi
        call   sub_4_0340
        test   al, al
        je     SHORT loc_4_0093
        mov    byte ptr [ebp - 1], 1
loc_4_0093:
        and    bl, 0feh
loc_4_0096:
        test   bl, 8
        je     SHORT loc_4_00A4
        push   esi
        and    bl, 0f5h
        call   sub_4_05B0
loc_4_00A4:
        test   bl, 2
        je     SHORT loc_4_00B2
        push   esi
        and    bl, 0fdh
        call   sub_4_0470
loc_4_00B2:
        test   bl, 4
        je     SHORT loc_4_00BD
        push   esi
        call   sub_4_02B0
loc_4_00BD:
        mov    byte ptr [esi + 13h], 0
        dec    dword ptr [esi + 4ah]
loc_4_00C4:
        cmp    byte ptr [esi + 14h], 0
        jne    SHORT loc_4_00FB
        cmp    byte ptr [esi + 9ch], 0
        jne    SHORT loc_4_00FB
        cmp    dword ptr [esi + 9eh], 0
        je     SHORT loc_4_00FB
        cmp    byte ptr [esi + 98h], 0
        je     SHORT loc_4_00FB
        push   1
        inc    dword ptr [esi + 4ah]
        mov    byte ptr [esi + 14h], 1
        push   esi
        call   sub_4_1830
        mov    byte ptr [esi + 14h], 0
        dec    dword ptr [esi + 4ah]
loc_4_00FB:
        cmp    dword ptr [esi + 7ah], 0
        je     SHORT loc_4_010B
        push   esi
        call   sub_4_0680
        mov    byte ptr [ebp - 1], 1
loc_4_010B:
        cmp    byte ptr [esi + 15h], 0
        jne    SHORT loc_4_013C
        cmp    byte ptr [esi + 92h], 0
        je     SHORT loc_4_0120
        cmp    dword ptr [esi + 4ah], 1
        je     SHORT loc_4_012A
loc_4_0120:
        mov    eax, dword ptr [esi + 5ah]
        sub    eax, esi
        cmp    eax, 5ah
        je     SHORT loc_4_013C
loc_4_012A:
        push   0
        lea    eax, [esi + 18h]
        mov    byte ptr [esi + 15h], 1
        push   eax
        inc    dword ptr [esi + 4ah]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_013C:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_4_014B
        mov    eax, dword ptr [esi + 6ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_014B:
        add    esi, 1789h
        push   esi
        call   sub_4_1F50
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  090h
loc_4_0160:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
loc_4_016B:
        mov    cl, byte ptr [esi + 92h]
        test   cl, cl
        je     SHORT loc_4_017B
        cmp    dword ptr [esi + 4ah], 2
        je     SHORT loc_4_0185
loc_4_017B:
        mov    eax, dword ptr [esi + 5ah]
        sub    eax, esi
        cmp    eax, 5ah
        je     SHORT loc_4_01C7
loc_4_0185:
        test   cl, cl
        je     SHORT loc_4_018F
        cmp    dword ptr [esi + 4ah], 2
        je     SHORT loc_4_0200
loc_4_018F:
        mov    ecx, dword ptr [esi + 5ah]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, 5ah
        je     SHORT loc_4_016B
        cmp    dword ptr [ecx + 14h], ebx
        jne    SHORT loc_4_016B
        mov    edi, dword ptr [ecx + 10h]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [ecx + 4]
        push   ebx
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    edx, dword ptr [ecx]
        push   45h
        push   ecx
        mov    dword ptr [edx + 4], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_01BB:
        push   ebx
        dec    dword ptr [esi + 56h]
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_01C5:
        jmp    SHORT loc_4_016B
loc_4_01C7:
        mov    byte ptr [esi + 15h], 0
        dec    dword ptr [esi + 4ah]
        cmp    byte ptr [esi + 9ch], 0
        jne    SHORT loc_4_0228
        cmp    dword ptr [esi + 9eh], 0
        je     SHORT loc_4_0228
        cmp    byte ptr [esi + 98h], 0
        je     SHORT loc_4_0228
        mov    byte ptr [esi + 14h], 1
        push   1
        push   esi
        call   sub_4_1830
        mov    byte ptr [esi + 14h], 0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_4_0200:
        mov    byte ptr [esi + 15h], 0
        push   esi
        dec    dword ptr [esi + 4ah]
        call   sub_4_34E0
        mov    byte ptr [esi + 92h], 0
        mov    esi, dword ptr [esi + 8eh]
        push   0
        mov    eax, dword ptr [esi + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0225:
        dec    dword ptr [esi + 14h]
loc_4_0228:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  090h
loc_4_0230:
        push   ebp
        mov    ebp, esp
        push   esi
        push   3e8h
        mov    esi, dword ptr [ebp + 0ch]
        lea    eax, [esi + 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0245:
        cmp    byte ptr [esi + 9ch], 0
        jne    SHORT loc_4_0270
        cmp    dword ptr [esi + 9eh], 0
        je     SHORT loc_4_0270
        cmp    byte ptr [esi + 98h], 0
        je     SHORT loc_4_0270
        mov    byte ptr [esi + 14h], 1
        push   1
        push   esi
        call   sub_4_1830
        mov    byte ptr [esi + 14h], 0
loc_4_0270:
        cmp    byte ptr [esi + 18ch], 1
        jne    SHORT loc_4_028C
        push   esi
        inc    dword ptr [esi + 16ah]
        call   sub_4_0470
        mov    byte ptr [esi + 18ch], 0
loc_4_028C:
        mov    eax, dword ptr [esi + 182h]
        test   eax, eax
        je     SHORT loc_4_02A5
        dec    eax
        mov    dword ptr [esi + 182h], eax
        jne    SHORT loc_4_02A5
        push   esi
        call   sub_4_05B0
loc_4_02A5:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_02B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        add    esi, 1789h
        push   esi
        inc    dword ptr [esi - 16bfh]
        inc    dword ptr [esi - 1627h]
        call   sub_4_1E00
        movsx  eax, ax
        cmp    eax, -1
        jne    SHORT loc_4_02DE
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
loc_4_02DE:
        push   esi
        call   sub_4_1F00
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_02F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    edi, dword ptr [ebp + 8]
        lea    esi, [edi + 1789h]
        inc    dword ptr [edi + 136h]
        push   esi
        inc    dword ptr [edi + 166h]
        call   sub_4_25C0
        push   edi
        call   sub_4_0340
        test   al, al
        je     SHORT loc_4_0322
        mov    ebx, 1
loc_4_0322:
        push   esi
        call   sub_4_2670
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0340:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], ebx
loc_4_0351:
        lea    eax, [esi + 1789h]
        push   eax
        call   sub_4_1E00
        movsx  edi, ax
        cmp    edi, -1
        je     NEAR PTR loc_4_0460
        mov    eax, edi
        add    dword ptr [esi + 15eh], edi
        mov    dword ptr [ebp - 8], 1
        inc    dword ptr [esi + 156h]
        cmp    edi, 5eah
        jbe    SHORT loc_4_038B
        mov    eax, 5eah
loc_4_038B:
        mov    ecx, dword ptr [esi + 17c9h]
        cmp    edi, 0eh
        jae    SHORT loc_4_03AE
        cmp    edi, 6
        jb     SHORT loc_4_03CB
        lea    eax, [ecx + 0eh]
        push   0
        push   0
        push   0
        push   edi
        push   ecx
        push   ecx
        push   eax
        mov    ecx, dword ptr [esi + 6ah]
        push   ecx
        jmp    SHORT loc_4_03C6
loc_4_03AE:
        lea    edx, [edi - 0eh]
        sub    eax, 0eh
        push   edx
        push   eax
        lea    edx, [ecx + 0eh]
        mov    eax, dword ptr [esi + 6ah]
        push   edx
        push   0eh
        push   ecx
        push   ecx
        add    ecx, 0eh
        push   ecx
        push   eax
loc_4_03C6:
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_03CB:
        lea    eax, [esi + 1789h]
        push   eax
        call   sub_4_1F00
        mov    al, byte ptr [esi + 17c8h]
        test   al, 10h
        je     SHORT loc_4_03ED
        inc    dword ptr [esi + 0ceh]
        inc    dword ptr [esi + 152h]
loc_4_03ED:
        test   al, 1
        je     SHORT loc_4_0431
        inc    dword ptr [esi + 0c2h]
        test   al, 20h
        je     SHORT loc_4_0416
        lea    ecx, [esi + 0f2h]
        inc    dword ptr [esi + 112h]
        mov    eax, dword ptr [ecx]
        add    eax, edi
        mov    dword ptr [ecx], eax
        cmp    edi, eax
        jbe    SHORT loc_4_044D
        inc    dword ptr [ecx + 4]
        jmp    SHORT loc_4_044D
loc_4_0416:
        lea    ecx, [esi + 0eah]
        mov    eax, dword ptr [ecx]
        add    eax, edi
        mov    dword ptr [ecx], eax
        cmp    edi, eax
        jbe    SHORT loc_4_0429
        inc    dword ptr [ecx + 4]
loc_4_0429:
        inc    dword ptr [esi + 10eh]
        jmp    SHORT loc_4_044D
loc_4_0431:
        inc    dword ptr [esi + 0cah]
        test   al, 2
        je     SHORT loc_4_0443
        inc    dword ptr [esi + 11ah]
        jmp    SHORT loc_4_044D
loc_4_0443:
        test   al, 4
        je     SHORT loc_4_044D
        inc    dword ptr [esi + 122h]
loc_4_044D:
        inc    ebx
        cmp    ebx, 0ah
        jb     NEAR PTR loc_4_0351
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_0460:
        mov    al, byte ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_4_0470:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ecx, dword ptr [esi + 197h]
        mov    edi, dword ptr [ecx + 2ch]
        add    ecx, 24h
        inc    dword ptr [esi + 0beh]
        mov    al, byte ptr [ecx + 0eh]
        cmp    eax, 1
        je     SHORT loc_4_04A1
        cmp    eax, 2
        je     SHORT loc_4_04C3
        cmp    eax, 3
        je     SHORT loc_4_04E8
        jmp    SHORT loc_4_050B
loc_4_04A1:
        lea    edx, [esi + 0d2h]
        xor    eax, eax
        xor    ebx, ebx
        inc    dword ptr [esi + 102h]
        mov    ax, word ptr [ecx + 0ch]
        add    eax, dword ptr [edx]
        mov    dword ptr [edx], eax
        mov    bx, word ptr [ecx + 0ch]
        cmp    ebx, eax
        jbe    SHORT loc_4_050B
        jmp    SHORT loc_4_0508
loc_4_04C3:
        lea    eax, [esi + 0dah]
        xor    edx, edx
        xor    ebx, ebx
        inc    dword ptr [esi + 106h]
        mov    dx, word ptr [ecx + 0ch]
        add    edx, dword ptr [eax]
        mov    dword ptr [eax], edx
        mov    bx, word ptr [ecx + 0ch]
        cmp    ebx, edx
        jbe    SHORT loc_4_050B
        inc    dword ptr [eax + 4]
        jmp    SHORT loc_4_050B
loc_4_04E8:
        lea    edx, [esi + 0e2h]
        xor    eax, eax
        xor    ebx, ebx
        inc    dword ptr [esi + 10ah]
        mov    ax, word ptr [ecx + 0ch]
        add    eax, dword ptr [edx]
        mov    dword ptr [edx], eax
        mov    bx, word ptr [ecx + 0ch]
        cmp    ebx, eax
        jbe    SHORT loc_4_050B
loc_4_0508:
        inc    dword ptr [edx + 4]
loc_4_050B:
        mov    al, byte ptr [ecx + 0fh]
        inc    al
        mov    byte ptr [ecx + 0fh], al
        cmp    al, 3
        jne    SHORT loc_4_0538
        push   0
        mov    eax, dword ptr [esi + 197h]
        push   eax
        mov    ecx, dword ptr [edi + 10h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0529:
        dec    dword ptr [edi + 14h]
        dec    dword ptr [esi + 16eh]
        dec    dword ptr [esi + 11eh]
loc_4_0538:
        cmp    byte ptr [esi + 18ah], 0
        je     SHORT loc_4_0599
        mov    eax, dword ptr [esi + 19bh]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 195h]
        lea    edx, [esi + 1789h]
        push   edx
        mov    dword ptr [esi + 197h], eax
        inc    dword ptr [esi + 18dh]
        add    dword ptr [esi + 191h], ecx
        call   sub_4_1FF0
        mov    dword ptr [esi + 186h], 0
        mov    al, 1
        pop    edi
        mov    byte ptr [esi + 18bh], 1
        mov    byte ptr [esi + 18ah], 0
        mov    dword ptr [esi + 182h], 4
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_0599:
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 18bh], al
        mov    dword ptr [esi + 182h], eax
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
sub_4_05B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 197h]
        inc    dword ptr [esi + 16ah]
        mov    edi, dword ptr [eax + 2ch]
        add    eax, 24h
        inc    dword ptr [esi + 0c6h]
        mov    cl, byte ptr [eax + 0fh]
        inc    cl
        mov    byte ptr [eax + 0fh], cl
        cmp    cl, 3
        jne    SHORT loc_4_0601
        push   0c0000001h
        mov    eax, dword ptr [esi + 197h]
        push   eax
        mov    ecx, dword ptr [edi + 10h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_05F2:
        dec    dword ptr [edi + 14h]
        dec    dword ptr [esi + 16eh]
        dec    dword ptr [esi + 11eh]
loc_4_0601:
        cmp    byte ptr [esi + 18ah], 0
        je     SHORT loc_4_0661
        mov    eax, dword ptr [esi + 19bh]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 195h]
        lea    edx, [esi + 1789h]
        push   edx
        mov    dword ptr [esi + 197h], eax
        inc    dword ptr [esi + 18dh]
        add    dword ptr [esi + 191h], ecx
        call   sub_4_1FF0
        mov    dword ptr [esi + 186h], 0
        mov    al, 1
        pop    edi
        mov    byte ptr [esi + 18bh], 1
        mov    byte ptr [esi + 18ah], 0
        mov    dword ptr [esi + 182h], 4
        pop    esi
        pop    ebp
        ret    4
loc_4_0661:
        xor    eax, eax
        pop    edi
        mov    byte ptr [esi + 18bh], al
        mov    dword ptr [esi + 182h], eax
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_0680:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 7ah]
        test   edi, edi
        je     NEAR PTR loc_4_078E
        cmp    byte ptr [esi + 9dh], 0
        jne    NEAR PTR loc_4_078E
        mov    byte ptr [esi + 9dh], 1
        push   esi
        call   sub_4_07E0
        mov    ecx, dword ptr [edi + 2ch]
        lea    eax, [edi + 24h]
        lea    edx, [ebp - 14h]
        mov    dword ptr [ebp - 0ch], eax
        push   edx
        lea    eax, [ebp - 18h]
        push   eax
        mov    dword ptr [ebp - 10h], ecx
        push   0
        mov    dword ptr [esi + 82h], edi
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06D5:
        lea    ecx, [ebp - 4]
        lea    edx, [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 18h]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_06E6:
        cmp    dword ptr [ebp - 4], 0eh
        jae    SHORT loc_4_0703
        cmp    dword ptr [ebp - 4], 6
        jb     SHORT loc_4_0756
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   0
        push   eax
        push   ecx
        push   ecx
        jmp    SHORT loc_4_074B
loc_4_0703:
        cmp    dword ptr [ebp - 4], 0deh
        jae    SHORT loc_4_072E
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 4], eax
        je     SHORT loc_4_072E
        lea    ebx, [esi + 19fh]
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        push   0d0h
        push   0eh
        push   edi
        call   sub_4_0810
        jmp    SHORT loc_4_0738
loc_4_072E:
        sub    dword ptr [ebp - 4], 0eh
        mov    ebx, dword ptr [ebp - 8]
        add    ebx, 0eh
loc_4_0738:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 4]
        sub    eax, 0eh
        mov    edx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        push   ebx
        push   0eh
        push   edx
        push   edx
loc_4_074B:
        push   0
        mov    eax, dword ptr [esi + 6ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0756:
        mov    eax, dword ptr [ebp - 0ch]
        mov    cl, byte ptr [eax + 0fh]
        inc    cl
        mov    byte ptr [eax + 0fh], cl
        cmp    cl, 3
        jne    SHORT loc_4_0787
        push   0
        mov    eax, dword ptr [ebp - 10h]
        push   edi
        mov    ecx, dword ptr [eax + 10h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0775:
        mov    ecx, dword ptr [ebp - 10h]
        dec    dword ptr [ecx + 14h]
        dec    dword ptr [esi + 16eh]
        dec    dword ptr [esi + 11eh]
loc_4_0787:
        mov    byte ptr [esi + 9dh], 0
loc_4_078E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_07A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        inc    dword ptr [eax + 15ah]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 28h], 0
        cmp    dword ptr [eax + 7ah], 0
        jne    SHORT loc_4_07C2
        mov    dword ptr [eax + 7ah], ecx
        jmp    SHORT loc_4_07C8
loc_4_07C2:
        mov    edx, dword ptr [eax + 7eh]
        mov    dword ptr [edx + 28h], ecx
loc_4_07C8:
        pop    ebp
        mov    dword ptr [eax + 7eh], ecx
        inc    dword ptr [eax + 172h]
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_07E0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 7ah]
        add    eax, 24h
        cmp    dword ptr [eax + 4], 0
        jne    SHORT loc_4_07FA
        mov    dword ptr [ecx + 7eh], 0
loc_4_07FA:
        mov    eax, dword ptr [eax + 4]
        pop    ebp
        mov    dword ptr [ecx + 7ah], eax
        dec    dword ptr [ecx + 172h]
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_0810:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 10h], esi
        je     NEAR PTR loc_4_08D9
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 10h]
        push   ecx
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_083D:
        cmp    dword ptr [ebp - 10h], esi
        je     NEAR PTR loc_4_08D9
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0857:
        cmp    dword ptr [ebp + 10h], esi
        je     SHORT loc_4_08D4
        mov    edi, dword ptr [ebp + 0ch]
loc_4_085F:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_088B
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0872:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_08D4
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0889:
        jmp    SHORT loc_4_08CF
loc_4_088B:
        test   edi, edi
        je     SHORT loc_4_08A8
        cmp    edi, dword ptr [ebp - 4]
        jbe    SHORT loc_4_08A0
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_4_08CF
loc_4_08A0:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_4_08A8:
        mov    ebx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, esi
        cmp    ebx, eax
        jb     SHORT loc_4_08B6
        mov    ebx, eax
loc_4_08B6:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    ecx, dword ptr [ebp + 14h]
        push   ecx
        add    esi, ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_08C6:
        add    dword ptr [ebp + 14h], ebx
        add    dword ptr [ebp - 0ch], ebx
        sub    dword ptr [ebp - 4], ebx
loc_4_08CF:
        cmp    dword ptr [ebp + 10h], esi
        ja     SHORT loc_4_085F
loc_4_08D4:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_4_08D9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_08F0:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0ch], esi
        je     NEAR PTR loc_4_09BC
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 14h]
        push   ecx
        mov    edx, dword ptr [ebp + 10h]
        push   esi
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_091D:
        cmp    dword ptr [ebp - 14h], esi
        je     NEAR PTR loc_4_09BC
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0937:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0ch], esi
        mov    dword ptr [ebp - 10h], ecx
        je     SHORT loc_4_09B7
        mov    edi, dword ptr [ebp + 14h]
loc_4_0945:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_0971
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0958:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_09B7
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_096F:
        jmp    SHORT loc_4_09B2
loc_4_0971:
        test   edi, edi
        je     SHORT loc_4_098E
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_4_0986
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_4_09B2
loc_4_0986:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        xor    edi, edi
loc_4_098E:
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        sub    ebx, esi
        cmp    ebx, eax
        jb     SHORT loc_4_099C
        mov    ebx, eax
loc_4_099C:
        push   ebx
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        add    esi, ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_09AC:
        add    dword ptr [ebp - 10h], ebx
        sub    dword ptr [ebp - 4], ebx
loc_4_09B2:
        cmp    esi, dword ptr [ebp + 0ch]
        jb     SHORT loc_4_0945
loc_4_09B7:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_4_09BC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_09D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        inc    dword ptr [esi + 4ah]
        cmp    byte ptr [esi + 92h], 0
        jne    SHORT loc_4_0A25
        cmp    byte ptr [eax + 40h], 0
        jne    SHORT loc_4_0A1E
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ecx + 10h]
        test   edx, edx
        jl     SHORT loc_4_09FD
        cmp    edx, 1
        jle    SHORT loc_4_0A04
loc_4_09FD:
        mov    edi, 0c00000bbh
        jmp    SHORT loc_4_0A2A
loc_4_0A04:
        push   ecx
        mov    edi, 103h
        push   esi
        inc    dword ptr [eax + 14h]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx], 0
        call   sub_4_0A70
        jmp    SHORT loc_4_0A2A
loc_4_0A1E:
        mov    edi, 0c0010002h
        jmp    SHORT loc_4_0A2A
loc_4_0A25:
        mov    edi, 0c001000dh
loc_4_0A2A:
        cmp    byte ptr [esi + 15h], 0
        jne    SHORT loc_4_0A60
        cmp    dword ptr [esi + 4ah], 2
        jne    SHORT loc_4_0A3F
        cmp    byte ptr [esi + 92h], 0
        jne    SHORT loc_4_0A49
loc_4_0A3F:
        mov    eax, dword ptr [esi + 5ah]
        sub    eax, esi
        cmp    eax, 5ah
        je     SHORT loc_4_0A60
loc_4_0A49:
        push   0
        add    esi, 18h
        push   esi
        mov    byte ptr [esi - 3], 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0A58:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_4_0A60:
        mov    eax, edi
        dec    dword ptr [esi + 4ah]
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
sub_4_0A70:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 6eh], 0
        je     SHORT loc_4_0A8C
        mov    edx, dword ptr [eax + 72h]
        mov    ecx, dword ptr [ebp + 0ch]
        pop    ebp
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax + 72h], ecx
        ret    8
loc_4_0A8C:
        cmp    byte ptr [eax + 95h], 0
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 6eh], ecx
        mov    dword ptr [eax + 72h], ecx
        jne    SHORT loc_4_0AA4
        push   eax
        call   sub_4_0AB0
loc_4_0AA4:
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_0AB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
loc_4_0ABC:
        mov    edi, dword ptr [esi + 6eh]
        test   edi, edi
        je     NEAR PTR loc_4_0C91
        cmp    byte ptr [esi + 92h], 0
        jne    NEAR PTR loc_4_0CA1
        mov    byte ptr [esi + 95h], 1
        mov    eax, dword ptr [edi + 10h]
        cmp    eax, 4
        ja     NEAR PTR loc_4_0C3F
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_0AEE:
        push   0
        lea    eax, [edi + 24h]
        push   eax
        lea    ecx, [edi + 20h]
        push   ecx
        mov    edx, dword ptr [edi + 1ch]
        push   edx
        mov    eax, dword ptr [edi + 18h]
        push   eax
        mov    ecx, dword ptr [edi + 14h]
        push   ecx
        mov    edx, dword ptr [edi + 4]
        push   edx
        push   esi
        call   sub_4_0D60
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_4_0C3F
loc_4_0B16:
        lea    eax, [edi + 24h]
        lea    ecx, [edi + 20h]
        push   eax
        mov    edx, dword ptr [edi + 1ch]
        push   ecx
        mov    eax, dword ptr [edi + 18h]
        push   edx
        mov    ecx, dword ptr [edi + 14h]
        push   eax
        mov    edx, dword ptr [edi + 4]
        push   ecx
        push   edx
        push   esi
        call   sub_4_12F0
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_4_0C3F
loc_4_0B3C:
        push   1
        lea    eax, [edi + 24h]
        push   eax
        lea    ecx, [edi + 20h]
        push   ecx
        mov    edx, dword ptr [edi + 1ch]
        push   edx
        mov    eax, dword ptr [edi + 18h]
        push   eax
        mov    ecx, dword ptr [edi + 14h]
        push   ecx
        mov    edx, dword ptr [edi + 4]
        push   edx
        push   esi
        call   sub_4_0D60
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_4_0C3F
loc_4_0B64:
        mov    ebx, dword ptr [edi + 4]
        lea    eax, [ebx + 0ch]
        mov    ecx, dword ptr [ebx + 10h]
        push   eax
        mov    edx, dword ptr [ebx + 8]
        push   ecx
        push   ebx
        mov    eax, dword ptr [edx + 6ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0B7C:
        test   al, al
        jne    SHORT loc_4_0BB3
        push   0
        mov    eax, dword ptr [ebx + 10h]
        push   0c0000001h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0B90:
        push   8
        mov    eax, dword ptr [esi + 66h]
        push   1
        push   2
        push   0aa000002h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0BA4:
        add    esp, 14h
        push   0
        push   45h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0BB1:
        jmp    SHORT loc_4_0BD9
loc_4_0BB3:
        lea    eax, [esi + 4eh]
        push   0
        push   0
        mov    dword ptr [ebx], eax
        mov    ecx, dword ptr [esi + 52h]
        mov    dword ptr [ebx + 4], ecx
        mov    ecx, dword ptr [esi + 52h]
        mov    dword ptr [ecx], ebx
        mov    dword ptr [esi + 52h], ebx
        inc    dword ptr [esi + 56h]
        inc    dword ptr [ebx + 14h]
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0BD9:
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_4_0C3F
loc_4_0BE2:
        mov    ebx, dword ptr [edi + 4]
        push   0
        mov    ecx, dword ptr [esi + 6ah]
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0BF4:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_4_0C00
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_4_0C16
loc_4_0C00:
        mov    eax, 103h
        cmp    dword ptr [ebp - 4], eax
        je     SHORT loc_4_0C16
        cmp    dword ptr [ebp - 4], 0c001000eh
        jne    SHORT loc_4_0C16
        mov    dword ptr [ebp - 4], eax
loc_4_0C16:
        mov    byte ptr [ebx + 40h], 1
        dec    dword ptr [ebx + 14h]
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        lea    ecx, [esi + 5ah]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [ebx], ecx
        mov    edx, dword ptr [esi + 5eh]
        mov    dword ptr [ebx + 4], edx
        mov    edx, dword ptr [esi + 5eh]
        mov    dword ptr [edx], ebx
        mov    dword ptr [esi + 5eh], ebx
loc_4_0C3F:
        cmp    dword ptr [ebp - 4], 103h
        je     SHORT loc_4_0CA8
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 6eh], eax
        mov    eax, dword ptr [edi + 10h]
        test   eax, eax
        je     SHORT loc_4_0C78
        cmp    eax, 1
        je     SHORT loc_4_0C78
        cmp    eax, 2
        jne    NEAR PTR loc_4_0ABC
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 66h]
        push   eax
        push   edi
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0C70:
        dec    dword ptr [esi + 4ah]
        jmp    NEAR PTR loc_4_0ABC
loc_4_0C78:
        mov    ebx, dword ptr [edi + 4]
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   edi
        mov    ecx, dword ptr [ebx + 10h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0C89:
        dec    dword ptr [ebx + 14h]
        jmp    NEAR PTR loc_4_0ABC
loc_4_0C91:
        mov    byte ptr [esi + 95h], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_0CA1:
        mov    byte ptr [esi + 95h], 0
loc_4_0CA8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_0CD0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 4ah]
        cmp    byte ptr [esi + 92h], 0
        jne    SHORT loc_4_0D12
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 2
        je     SHORT loc_4_0CF4
        mov    edi, 0c00000bbh
        jmp    SHORT loc_4_0D17
loc_4_0CF4:
        push   eax
        mov    edi, 103h
        mov    dword ptr [eax + 4], 0
        push   esi
        mov    dword ptr [eax], 0
        inc    dword ptr [esi + 4ah]
        call   sub_4_0A70
        jmp    SHORT loc_4_0D17
loc_4_0D12:
        mov    edi, 0c001000dh
loc_4_0D17:
        cmp    byte ptr [esi + 15h], 0
        jne    SHORT loc_4_0D4D
        cmp    dword ptr [esi + 4ah], 2
        jne    SHORT loc_4_0D2C
        cmp    byte ptr [esi + 92h], 0
        jne    SHORT loc_4_0D36
loc_4_0D2C:
        mov    eax, dword ptr [esi + 5ah]
        sub    eax, esi
        cmp    eax, 5ah
        je     SHORT loc_4_0D4D
loc_4_0D36:
        push   0
        add    esi, 18h
        push   esi
        mov    byte ptr [esi - 3], 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0D45:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_4_0D4D:
        mov    eax, edi
        dec    dword ptr [esi + 4ah]
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_0D60:
        push   ebp
        mov    cl, byte ptr [esp + 24h]
        mov    ebp, esp
        sub    esp, 14h
        test   cl, cl
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_4_0D7D
        db  0BAh, 000h, 000h, 000h, 000h
        mov    edi, 34h
        jmp    SHORT loc_4_0D87
loc_4_0D7D:
        db  0BAh, 000h, 000h, 000h, 000h
        mov    edi, 18h
loc_4_0D87:
        xor    esi, esi
        test   edi, edi
        jle    SHORT loc_4_0DA0
        mov    ebx, edx
        mov    eax, dword ptr [ebp + 10h]
loc_4_0D92:
        cmp    dword ptr [ebx], eax
        je     SHORT loc_4_0DA3
        add    ebx, 4
        inc    esi
        cmp    edi, esi
        jg     SHORT loc_4_0D92
        jmp    SHORT loc_4_0DA3
loc_4_0DA0:
        mov    eax, dword ptr [ebp + 10h]
loc_4_0DA3:
        cmp    edi, esi
        jne    SHORT loc_4_0DBE
        mov    eax, dword ptr [ebp + 1ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    dword ptr [eax], 0
        mov    eax, 0c0010017h
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_4_0DBE:
        lea    ebx, [ebp - 8]
        mov    esi, 4
        cmp    eax, 20101h
        ja     SHORT loc_4_0DE8
        je     NEAR PTR loc_4_0FBF
        sub    eax, 10101h
        cmp    eax, 12h
        ja     NEAR PTR loc_4_120E
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_0DE8:
        cmp    eax, 20201h
        ja     SHORT loc_4_0E0A
        je     NEAR PTR loc_4_1005
        sub    eax, 20102h
        cmp    eax, 3
        ja     NEAR PTR loc_4_120E
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_0E0A:
        cmp    eax, 1010101h
        ja     SHORT loc_4_0E2C
        je     NEAR PTR loc_4_10E7
        sub    eax, 20202h
        cmp    eax, 0ch
        ja     NEAR PTR loc_4_120E
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_0E2C:
        cmp    eax, 1020101h
        ja     SHORT loc_4_0E5F
        je     NEAR PTR loc_4_119F
        cmp    eax, 1010102h
        je     NEAR PTR loc_4_10F7
        cmp    eax, 1010103h
        je     NEAR PTR loc_4_1107
        cmp    eax, 1010104h
        je     NEAR PTR loc_4_1196
        jmp    NEAR PTR loc_4_120E
loc_4_0E5F:
        cmp    eax, 1020201h
        ja     SHORT loc_4_0E87
        je     NEAR PTR loc_4_11C0
        cmp    eax, 1020102h
        je     NEAR PTR loc_4_11AA
        cmp    eax, 1020103h
        je     NEAR PTR loc_4_11B5
        jmp    NEAR PTR loc_4_120E
loc_4_0E87:
        sub    eax, 1020202h
        cmp    eax, 5
        ja     NEAR PTR loc_4_120E
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_0E9C:
        mov    ebx, edx
        lea    esi, [edi*4]
        jmp    NEAR PTR loc_4_120E
loc_4_0EAA:
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_4_120E
loc_4_0EB6:
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_4_120E
loc_4_0EC2:
        xor    eax, eax
        test   cl, cl
        mov    dword ptr [ebp - 8], eax
        je     NEAR PTR loc_4_120E
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 56h], eax
        jne    NEAR PTR loc_4_120E
        xor    esi, esi
        jmp    NEAR PTR loc_4_120E
loc_4_0EE2:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_4_120E
loc_4_0EEE:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_4_120E
loc_4_0EFA:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_4_120E
loc_4_0F06:
        mov    dword ptr [ebp - 8], 0bd4h
        jmp    NEAR PTR loc_4_120E
loc_4_0F12:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_4_120E
loc_4_0F1E:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_4_120E
loc_4_0F2A:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_4_120E
loc_4_0F36:
        push   3
        mov    edi, dword ptr [ebp + 8]
        add    edi, 6
        lea    eax, [ebp - 14h]
        push   edi
        lea    ebx, [ebp - 14h]
        push   eax
        mov    esi, 4
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_0F50:
        mov    byte ptr [ebp - 11h], 0
        jmp    NEAR PTR loc_4_120E
loc_4_0F59:
        db  0BBh, 000h, 000h, 000h, 000h
        mov    esi, 1eh
        jmp    NEAR PTR loc_4_120E
loc_4_0F68:
        test   cl, cl
        je     SHORT loc_4_0F77
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 76h]
        jmp    NEAR PTR loc_4_120B
loc_4_0F77:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 0ch]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_4_120E
loc_4_0F88:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_4_120E
loc_4_0F94:
        lea    ebx, [ebp - 2]
        mov    esi, 2
        mov    word ptr [ebp - 2], 300h
        jmp    NEAR PTR loc_4_120E
loc_4_0FA7:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_4_120E
loc_4_0FB3:
        mov    dword ptr [ebp - 8], 7
        jmp    NEAR PTR loc_4_120E
loc_4_0FBF:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0beh]
        jmp    NEAR PTR loc_4_120B
loc_4_0FCD:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0c2h]
        jmp    NEAR PTR loc_4_120B
loc_4_0FDB:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0c6h]
        jmp    NEAR PTR loc_4_120B
loc_4_0FE9:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0cah]
        jmp    NEAR PTR loc_4_120B
loc_4_0FF7:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 0ceh]
        jmp    NEAR PTR loc_4_120B
loc_4_1005:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 8
        add    ebx, 0d2h
        jmp    NEAR PTR loc_4_120E
loc_4_1018:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 102h]
        jmp    NEAR PTR loc_4_120B
loc_4_1026:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 8
        add    ebx, 0dah
        jmp    NEAR PTR loc_4_120E
loc_4_1039:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 106h]
        jmp    NEAR PTR loc_4_120B
loc_4_1047:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 8
        add    ebx, 0e2h
        jmp    NEAR PTR loc_4_120E
loc_4_105A:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 10ah]
        jmp    NEAR PTR loc_4_120B
loc_4_1068:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 8
        add    ebx, 0eah
        jmp    NEAR PTR loc_4_120E
loc_4_107B:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 10eh]
        jmp    NEAR PTR loc_4_120B
loc_4_1089:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 8
        add    ebx, 0f2h
        jmp    NEAR PTR loc_4_120E
loc_4_109C:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 112h]
        jmp    NEAR PTR loc_4_120B
loc_4_10AA:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 8
        add    ebx, 0fah
        jmp    NEAR PTR loc_4_120E
loc_4_10BD:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 116h]
        jmp    NEAR PTR loc_4_120B
loc_4_10CB:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 11ah]
        jmp    NEAR PTR loc_4_120B
loc_4_10D9:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 11eh]
        jmp    NEAR PTR loc_4_120B
loc_4_10E7:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 6
        add    ebx, 6
        jmp    NEAR PTR loc_4_120E
loc_4_10F7:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 6
        add    ebx, 0ch
        jmp    NEAR PTR loc_4_120E
loc_4_1107:
        test   cl, cl
        je     SHORT loc_4_113C
        mov    eax, dword ptr [ebp + 14h]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 18h]
        push   ecx
        mov    edi, dword ptr [ebp + 8]
        push   edx
        lea    ecx, [ebp - 10h]
        mov    eax, dword ptr [edi + 6ah]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1127:
        mov    ebx, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 0ch]
        lea    eax, [ecx + ecx*2]
        lea    esi, [eax*2]
        jmp    NEAR PTR loc_4_120E
loc_4_113C:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    ecx, [ebp - 0ch]
        push   ecx
        mov    edx, dword ptr [ebp + 18h]
        push   edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [edi + 6ah]
        mov    ecx, dword ptr [eax + 0ch]
        push   ecx
        lea    eax, [ebp - 10h]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_115F:
        cmp    dword ptr [ebp - 10h], 0
        mov    ebx, dword ptr [ebp + 14h]
        jne    SHORT loc_4_117A
        mov    eax, dword ptr [ebp - 0ch]
        lea    ecx, [eax + eax*2]
        lea    esi, [ecx*2]
        jmp    NEAR PTR loc_4_120E
loc_4_117A:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [edi + 6ah]
        mov    ecx, dword ptr [eax + 0ch]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_118A:
        lea    ecx, [eax + eax*2]
        lea    esi, [ecx*2]
        jmp    SHORT loc_4_120E
loc_4_1196:
        mov    dword ptr [ebp - 8], 10h
        jmp    SHORT loc_4_120E
loc_4_119F:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 122h]
        jmp    SHORT loc_4_120B
loc_4_11AA:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 126h]
        jmp    SHORT loc_4_120B
loc_4_11B5:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 12ah]
        jmp    SHORT loc_4_120B
loc_4_11C0:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 12eh]
        jmp    SHORT loc_4_120B
loc_4_11CB:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 132h]
        jmp    SHORT loc_4_120B
loc_4_11D6:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 136h]
        jmp    SHORT loc_4_120B
loc_4_11E1:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 13ah]
        jmp    SHORT loc_4_120B
loc_4_11EC:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 13eh]
        jmp    SHORT loc_4_120B
loc_4_11F7:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 142h]
        jmp    SHORT loc_4_120B
loc_4_1202:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 146h]
loc_4_120B:
        mov    dword ptr [ebp - 8], eax
loc_4_120E:
        cmp    esi, dword ptr [ebp + 18h]
        jbe    SHORT loc_4_1226
        mov    eax, dword ptr [ebp + 20h]
        pop    edi
        mov    dword ptr [eax], esi
        mov    eax, 0c0010014h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_4_1226:
        push   esi
        mov    eax, dword ptr [ebp + 14h]
        push   ebx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1231:
        mov    eax, dword ptr [ebp + 1ch]
        pop    edi
        mov    dword ptr [eax], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh, 064h, 024h, 000h
sub_4_12F0:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 4
        cmp    eax, 1010103h
        push   esi
        ja     SHORT loc_4_1325
        je     NEAR PTR loc_4_13F5
        cmp    eax, 1010eh
        je     SHORT loc_4_1331
        cmp    eax, 1010fh
        je     NEAR PTR loc_4_139B
        cmp    eax, 10112h
        je     NEAR PTR loc_4_13AD
loc_4_1325:
        mov    eax, 0c0010017h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_1331:
        cmp    dword ptr [ebp + 18h], 4
        je     SHORT loc_4_134C
        mov    eax, dword ptr [ebp + 20h]
        pop    esi
        mov    dword ptr [eax], 4
        mov    eax, 0c0010014h
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_134C:
        push   4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_135B:
        test   dword ptr [ebp - 4], 0f050h
        je     SHORT loc_4_1370
        mov    eax, 0c00000bbh
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_1370:
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0ch]
        mov    ecx, dword ptr [eax + 6ah]
        push   edx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_138B:
        mov    ecx, dword ptr [ebp + 1ch]
        pop    esi
        mov    dword ptr [ecx], 4
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_139B:
        mov    ecx, dword ptr [ebp + 1ch]
        xor    eax, eax
        pop    esi
        mov    dword ptr [ecx], 4
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_13AD:
        cmp    dword ptr [ebp + 18h], 4
        je     SHORT loc_4_13D1
        mov    ecx, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [ebp + 20h]
        pop    esi
        mov    dword ptr [ecx], 0
        mov    dword ptr [eax], 0
        mov    eax, 0c0010014h
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_13D1:
        push   4
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 41h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_13E3:
        mov    ecx, dword ptr [ebp + 1ch]
        xor    eax, eax
        pop    esi
        mov    dword ptr [ecx], 4
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_13F5:
        mov    ecx, 6
        mov    esi, dword ptr [ebp + 18h]
        mov    eax, esi
        sub    edx, edx
        div    ecx
        test   edx, edx
        je     SHORT loc_4_1413
        mov    eax, 0c0010015h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
loc_4_1413:
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    ecx, 6
        mov    eax, esi
        sub    edx, edx
        div    ecx
        push   eax
        mov    edx, dword ptr [ebp + 0ch]
        push   0
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        mov    edx, dword ptr [ecx + 6ah]
        push   eax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_143A:
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1450:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        cmp    byte ptr [esi + 92h], 0
        jne    SHORT loc_4_14B6
        add    esi, 1789h
        push   2
        push   esi
        call   sub_4_26B0
        mov    edi, dword ptr [ebp + 0ch]
        test   edi, 8
        je     SHORT loc_4_1486
        push   3
        push   esi
        call   sub_4_26B0
loc_4_1486:
        test   edi, 2
        je     SHORT loc_4_1496
        push   4
        push   esi
        call   sub_4_26B0
loc_4_1496:
        test   edi, 4
        je     SHORT loc_4_14A6
        push   5
        push   esi
        call   sub_4_26B0
loc_4_14A6:
        test   edi, 20h
        je     SHORT loc_4_14B6
        push   6
        push   esi
        call   sub_4_26B0
loc_4_14B6:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
loc_4_14C0:
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        cmp    byte ptr [esi + 92h], 0
        jne    SHORT loc_4_14FD
        add    esi, 1789h
        push   0
        push   0
        push   esi
        call   sub_4_2950
        mov    edi, dword ptr [ebp + 10h]
        test   edi, edi
        je     SHORT loc_4_14FD
        mov    ebx, dword ptr [ebp + 14h]
loc_4_14F0:
        push   ebx
        push   esi
        add    ebx, 6
        call   sub_4_2A30
        dec    edi
        jne    SHORT loc_4_14F0
loc_4_14FD:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    1ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_1510:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 14h]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_4_1520:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 4], 103h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 4ah]
        cmp    byte ptr [esi + 92h], 0
        jne    NEAR PTR loc_4_16B2
        cmp    byte ptr [edi + 40h], 0
        jne    NEAR PTR loc_4_16A9
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        inc    dword ptr [edi + 14h]
        push   0
        push   0
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1566:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_4_169D
        cmp    dword ptr [ebp - 8], 5eah
        ja     NEAR PTR loc_4_169D
        mov    ebx, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ebx + 30h], cx
        add    ebx, 24h
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx + 8], edi
        push   eax
        push   edi
        push   esi
        call   sub_4_1710
        test   byte ptr [edi + 41h], 2
        je     SHORT loc_4_15BB
        cmp    eax, 1
        jne    SHORT loc_4_15AA
        mov    eax, 2
        jmp    SHORT loc_4_15BB
loc_4_15AA:
        cmp    eax, 3
        jne    SHORT loc_4_15C5
        mov    dword ptr [ebp - 4], 0
        mov    eax, 4
loc_4_15BB:
        cmp    eax, 3
        jne    SHORT loc_4_15C5
        mov    eax, 1
loc_4_15C5:
        cmp    eax, 4
        jae    SHORT loc_4_15D6
        inc    dword ptr [esi + 11eh]
        inc    dword ptr [esi + 16eh]
loc_4_15D6:
        cmp    eax, 1
        jne    SHORT loc_4_1627
        mov    byte ptr [ebx + 0fh], 1
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        call   sub_4_17F0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        call   sub_4_07A0
        cmp    byte ptr [esi + 9ch], 0
        jne    NEAR PTR loc_4_16B9
        cmp    dword ptr [esi + 9eh], 0
        je     NEAR PTR loc_4_16B9
        cmp    byte ptr [esi + 98h], 0
        je     NEAR PTR loc_4_16B9
        push   0
        push   esi
        call   sub_4_1830
        jmp    NEAR PTR loc_4_16B9
loc_4_1627:
        cmp    eax, 2
        jne    SHORT loc_4_165F
        mov    byte ptr [ebx + 0fh], 2
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        call   sub_4_17F0
        cmp    byte ptr [esi + 9ch], 0
        jne    SHORT loc_4_16B9
        cmp    dword ptr [esi + 9eh], 0
        je     SHORT loc_4_16B9
        cmp    byte ptr [esi + 98h], 0
        je     SHORT loc_4_16B9
        push   0
        push   esi
        call   sub_4_1830
        jmp    SHORT loc_4_16B9
loc_4_165F:
        cmp    eax, 3
        jne    SHORT loc_4_16B9
        mov    byte ptr [ebx + 0fh], 2
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        call   sub_4_07A0
        lea    ecx, [esi + 0d2h]
        xor    edx, edx
        xor    eax, eax
        inc    dword ptr [esi + 0beh]
        inc    dword ptr [esi + 102h]
        mov    dx, word ptr [ebx + 0ch]
        add    edx, dword ptr [ecx]
        mov    dword ptr [ecx], edx
        mov    ax, word ptr [ebx + 0ch]
        cmp    eax, edx
        jbe    SHORT loc_4_16B9
        inc    dword ptr [ecx + 4]
        jmp    SHORT loc_4_16B9
loc_4_169D:
        mov    dword ptr [ebp - 4], 0c000009ah
        dec    dword ptr [edi + 14h]
        jmp    SHORT loc_4_16B9
loc_4_16A9:
        mov    dword ptr [ebp - 4], 0c0010002h
        jmp    SHORT loc_4_16B9
loc_4_16B2:
        mov    dword ptr [ebp - 4], 0c001000dh
loc_4_16B9:
        cmp    byte ptr [esi + 15h], 0
        jne    SHORT loc_4_16F3
        cmp    dword ptr [esi + 4ah], 2
        jne    SHORT loc_4_16CE
        cmp    byte ptr [esi + 92h], 0
        jne    SHORT loc_4_16D8
loc_4_16CE:
        mov    eax, dword ptr [esi + 5ah]
        sub    eax, esi
        cmp    eax, 5ah
        je     SHORT loc_4_16F3
loc_4_16D8:
        push   0
        add    esi, 18h
        push   esi
        mov    byte ptr [esi - 3], 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_16E7:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_16F3:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        dec    dword ptr [esi + 4ah]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1710:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        lea    ecx, [ebp - 8]
        lea    esi, [eax + 24h]
        push   0
        push   ecx
        push   0
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_172E:
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 4]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_173F:
        mov    edx, dword ptr [ebp - 4]
        mov    cl, byte ptr [edx]
        test   cl, 1
        je     SHORT loc_4_176C
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_4_1763
        cmp    cl, 0ffh
        jne    SHORT loc_4_1763
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_4_1763
        mov    byte ptr [esi + 0eh], 3
        add    esi, 0eh
        jmp    SHORT loc_4_17B4
loc_4_1763:
        mov    byte ptr [esi + 0eh], 2
        add    esi, 0eh
        jmp    SHORT loc_4_17B4
loc_4_176C:
        mov    byte ptr [esi + 0eh], 1
        add    esi, 0eh
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        add    ecx, 6
        mov    eax, dword ptr [eax + 2]
        cmp    edx, eax
        jbe    SHORT loc_4_178D
        mov    eax, 1
        jmp    SHORT loc_4_17AD
loc_4_178D:
        jae    SHORT loc_4_1796
        mov    eax, 0ffffffffh
        jmp    SHORT loc_4_17AD
loc_4_1796:
        mov    cx, word ptr [ecx]
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax]
        cmp    cx, ax
        jbe    SHORT loc_4_17AB
        mov    eax, 1
        jmp    SHORT loc_4_17AD
loc_4_17AB:
        sbb    eax, eax
loc_4_17AD:
        test   eax, eax
        jne    SHORT loc_4_17B4
        mov    byte ptr [esi], 4
loc_4_17B4:
        cmp    byte ptr [esi], 4
        jne    SHORT loc_4_17C5
        mov    eax, 3
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_17C5:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        mov    edx, dword ptr [ecx + 6ah]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_17D5:
        pop    esi
        cmp    al, 1
        mov    eax, 2
        mov    esp, ebp
        adc    eax, -1
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_17F0:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 24h], 0
        mov    edx, dword ptr [eax + 0a2h]
        test   edx, edx
        jne    SHORT loc_4_1813
        mov    dword ptr [eax + 9eh], ecx
        jmp    SHORT loc_4_1816
loc_4_1813:
        mov    dword ptr [edx + 24h], ecx
loc_4_1816:
        pop    ebp
        mov    dword ptr [eax + 0a2h], ecx
        inc    dword ptr [eax + 176h]
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1830:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 98h], 0
        je     SHORT loc_4_1899
        cmp    byte ptr [esi + 9ch], 0
        jne    SHORT loc_4_1899
        mov    eax, dword ptr [esi + 9eh]
        test   eax, eax
        je     SHORT loc_4_1899
        cmp    byte ptr [esi + 13h], 0
        jne    SHORT loc_4_1899
        mov    byte ptr [esi + 9ch], 1
        mov    cl, byte ptr [ebp + 0ch]
        push   ecx
        push   eax
        push   esi
        call   sub_4_18A0
        test   al, al
        je     SHORT loc_4_1892
        mov    eax, dword ptr [esi + 9eh]
        mov    eax, dword ptr [eax + 24h]
        mov    dword ptr [esi + 9eh], eax
        test   eax, eax
        jne    SHORT loc_4_188C
        mov    dword ptr [esi + 0a2h], 0
loc_4_188C:
        dec    dword ptr [esi + 176h]
loc_4_1892:
        mov    byte ptr [esi + 9ch], 0
loc_4_1899:
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_4_18A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 18ah], bl
        je     SHORT loc_4_18C1
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_18C1:
        lea    esi, [edi + 1789h]
        push   esi
        call   sub_4_1F90
        test   ax, ax
        je     SHORT loc_4_18DD
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_18DD:
        push   0
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   0
        push   0
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_18F0:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_192B
loc_4_18F6:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1907:
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        push   edx
        push   esi
        call   sub_4_1FB0
        add    ebx, dword ptr [ebp - 8]
        lea    ecx, [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1925:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_4_18F6
loc_4_192B:
        push   esi
        call   sub_4_1F70
        mov    word ptr [edi + 195h], bx
        push   esi
        call   sub_4_19C0
        cmp    byte ptr [edi + 18bh], 0
        mov    eax, dword ptr [ebp + 0ch]
        je     SHORT loc_4_1968
        push   esi
        mov    dword ptr [edi + 19bh], eax
        mov    byte ptr [edi + 18ah], 1
        call   sub_4_19D0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_1968:
        push   esi
        mov    dword ptr [edi + 197h], eax
        call   sub_4_19D0
        push   esi
        inc    dword ptr [edi + 18dh]
        mov    dword ptr [edi + 182h], 4
        mov    dword ptr [edi + 186h], 0
        mov    byte ptr [edi + 18bh], 1
        call   sub_4_19C0
        push   esi
        call   sub_4_1FF0
        push   esi
        call   sub_4_19D0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_19C0:
        cli
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_19D0:
        sti
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
loc_4_19E3:
        mov    edx, 2fdh
        in     al, dx
        test   al, 20h
        je     SHORT loc_4_19E3
        mov    edx, 2f8h
        mov    al, byte ptr [ebp + 0ch]
        out    dx, al
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_1A00:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        lea    eax, [ebp - 10h]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        lea    ecx, [ebp - 8]
        mov    edi, dword ptr [esi]
        push   ecx
        mov    edx, dword ptr [esi + 4]
        lea    eax, [ebp - 4]
        push   edx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1A22:
        mov    cl, byte ptr [ebp - 0eh]
        mov    byte ptr [edi + 17e2h], cl
        mov    dl, byte ptr [ebp - 0dh]
        mov    byte ptr [edi + 17e3h], dl
        mov    al, byte ptr [ebp - 0ch]
        mov    byte ptr [edi + 17e4h], al
        mov    cl, byte ptr [ebp - 0bh]
        mov    byte ptr [edi + 17e5h], cl
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_1A58
        mov    ax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1A58:
        mov    al, byte ptr [esi + 5ah]
        and    al, 0eh
        shr    al, 1
        and    eax, 0ffh
        cmp    eax, 7
        ja     SHORT loc_4_1A71
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_1A71:
        mov    ax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1A7D:
        xor    ax, ax
        pop    edi
        mov    dword ptr [esi + 0ch], 300h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1A8F:
        xor    ax, ax
        pop    edi
        mov    dword ptr [esi + 0ch], 1300h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1AA1:
        xor    ax, ax
        pop    edi
        mov    dword ptr [esi + 0ch], 2300h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1AB3:
        xor    ax, ax
        pop    edi
        mov    dword ptr [esi + 0ch], 3300h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1AC5:
        xor    ax, ax
        pop    edi
        mov    dword ptr [esi + 0ch], 4300h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1AD7:
        xor    ax, ax
        pop    edi
        mov    dword ptr [esi + 0ch], 5300h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1AE9:
        xor    ax, ax
        pop    edi
        mov    dword ptr [esi + 0ch], 6300h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1AFB:
        xor    ax, ax
        pop    edi
        mov    dword ptr [esi + 0ch], 7300h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h
sub_4_1B30:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [eax]
        xor    eax, eax
        mov    ax, word ptr [esi + 179bh]
        lea    edi, [esi + 14eh]
        shl    eax, 4
        mov    dword ptr [esi + 14ah], eax
        cmp    word ptr [esi + 17a1h], 4000h
        jne    SHORT loc_4_1B68
        push   4000h
        jmp    SHORT loc_4_1B6D
loc_4_1B68:
        push   10000h
loc_4_1B6D:
        push   eax
        lea    ecx, [ebp - 4]
        mov    eax, dword ptr [esi + 66h]
        push   eax
        push   edi
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1B7C:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_1B8E
        mov    ax, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1B8E:
        mov    eax, dword ptr [edi]
        pop    edi
        mov    dword ptr [esi + 179dh], eax
        pop    esi
        xor    ax, ax
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h
        db  066h, 08Bh, 04Ch, 024h, 010h, 08Bh, 0ECh, 083h, 0ECh, 004h, 080h, 0F1h, 001h, 08Dh, 045h, 0FCh
        db  056h, 050h, 08Bh, 075h, 014h, 056h, 08Bh, 045h, 008h, 051h, 08Bh, 04Dh, 00Ch, 051h, 08Bh, 010h
        db  08Bh, 04Ah, 066h, 051h, 0E8h, 000h, 000h, 000h, 000h, 08Bh, 055h, 0FCh, 08Bh, 04Dh, 018h, 089h
        db  011h, 083h, 03Eh, 001h, 05Eh, 01Bh, 0C9h, 08Bh, 0E5h, 0F7h, 0D9h, 066h, 08Bh, 0C1h, 05Dh, 0C2h
        db  014h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1C00:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        lea    ecx, [eax + eax*4]
        lea    edx, [ecx + ecx*4]
        lea    eax, [edx + edx*4]
        shl    eax, 3
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_1C19:
        xor    ax, ax
        pop    ebp
        ret    8
sub_4_1C20:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        mov    ebp, esp
        mov    al, byte ptr [esp + 10h]
        out    dx, al
        xor    ax, ax
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 054h
        db  024h, 00Ch, 08Bh, 0ECh, 08Bh, 044h, 024h, 010h, 025h, 0FFh, 0FFh, 000h, 000h, 05Dh, 066h, 0EFh
        db  066h, 033h, 0C0h, 0C2h, 00Ch, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 054h
        db  024h, 00Ch, 08Bh, 0ECh, 08Bh, 044h, 024h, 010h, 0EFh, 066h, 033h, 0C0h, 05Dh, 0C2h, 00Ch, 000h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1C80:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        in     al, dx
        mov    ebp, esp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 055h, 08Bh, 054h, 024h, 00Ch, 066h, 0EDh, 08Bh, 0ECh, 05Dh, 0C2h, 008h
        db  000h, 08Dh, 049h, 000h, 055h, 08Bh, 054h, 024h, 00Ch, 0EDh, 08Bh, 0ECh, 05Dh, 0C2h, 008h, 000h
        db  08Dh, 064h, 024h, 000h, 055h, 08Bh, 044h, 024h, 014h, 08Bh, 0ECh, 050h, 08Bh, 04Dh, 010h, 08Bh
        db  055h, 00Ch, 051h, 052h, 0E8h, 000h, 000h, 000h, 000h, 066h, 033h, 0C0h, 05Dh, 0C2h, 010h, 000h
        db  08Dh, 064h, 024h, 000h, 055h, 08Bh, 044h, 024h, 014h, 08Bh, 0ECh, 050h, 08Bh, 04Dh, 010h, 08Bh
        db  055h, 00Ch, 051h, 052h, 0E8h, 000h, 000h, 000h, 000h, 066h, 033h, 0C0h, 05Dh, 0C2h, 010h, 000h
        db  08Dh, 064h, 024h, 000h, 055h, 08Bh, 044h, 024h, 018h, 08Bh, 0ECh, 08Bh, 008h, 051h, 08Bh, 055h
        db  014h, 052h, 08Bh, 045h, 00Ch, 06Ah, 000h, 08Bh, 04Dh, 008h, 050h, 08Bh, 011h, 08Bh, 042h, 066h
        db  050h, 0E8h, 000h, 000h, 000h, 000h, 066h, 033h, 0C0h, 05Dh, 0C2h, 014h, 000h, 08Dh, 0A4h, 024h
        db  000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 08Bh, 008h, 05Dh, 0FFh, 081h
        db  07Eh, 001h, 000h, 000h, 0C2h, 004h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h
        db  000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 08Bh, 008h, 05Dh, 0C6h, 081h
        db  08Ch, 001h, 000h, 000h, 001h, 0C2h, 004h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h
        db  000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 06Ah, 000h, 08Bh, 048h, 00Ch
        db  083h, 0C1h, 00Fh, 051h, 050h, 0E8h, 0AAh, 0FEh, 0FFh, 0FFh, 05Dh, 0C2h, 004h, 000h, 08Dh, 09Bh
        db  000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 06Ah, 01Fh, 08Bh, 048h, 00Ch
        db  083h, 0C1h, 00Fh, 051h, 050h, 0E8h, 08Ah, 0FEh, 0FFh, 0FFh, 05Dh, 0C2h, 004h, 000h, 08Dh, 09Bh
        db  000h, 000h, 000h, 000h
sub_4_1DA0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0fh
        push   eax
        push   esi
        call   sub_4_1C20
        push   4
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 23h
        push   eax
        push   esi
        call   sub_4_1C20
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 7
        push   eax
        push   esi
        call   sub_4_1C80
        mov    byte ptr [esi + 3eh], al
        and    al, 1fh
        push   eax
        mov    byte ptr [esi + 3eh], al
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 7
        push   eax
        push   esi
        call   sub_4_1C20
        mov    al, byte ptr [esi + 3eh]
        pop    esi
        and    eax, 0ffff00ffh
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1E00:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_4_19C0
        push   62h
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        push   esi
        call   sub_4_1C20
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 7
        push   eax
        push   esi
        call   sub_4_1C80
        mov    bl, al
        push   22h
        mov    ecx, dword ptr [esi + 0ch]
        push   ecx
        push   esi
        call   sub_4_1C20
        push   esi
        call   sub_4_19D0
        mov    al, byte ptr [esi + 3ch]
        cmp    bl, al
        jne    SHORT loc_4_1E53
        mov    ax, 0ffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1E53:
        xor    ecx, ecx
        mov    cl, al
        shl    ecx, 8
        add    ecx, dword ptr [esi + 14h]
        mov    dl, byte ptr [ecx]
        mov    byte ptr [esi + 3fh], dl
        mov    al, byte ptr [ecx + 1]
        mov    byte ptr [esi + 3dh], al
        mov    ax, word ptr [ecx + 2]
        sub    ax, 4
        mov    word ptr [ebp - 2], ax
        cmp    ax, 5eah
        ja     SHORT loc_4_1EE5
        test   dl, 6
        jne    SHORT loc_4_1EE5
        and    eax, 0ffffh
        mov    edi, dword ptr [esi + 50h]
        mov    edx, ecx
        mov    dword ptr [ebp - 8], eax
        sub    edx, edi
        lea    eax, [edx + eax + 4]
        test   eax, eax
        jle    SHORT loc_4_1EB3
        mov    ebx, dword ptr [esi + 54h]
        inc    eax
        cdq
        sub    eax, edx
        sar    eax, 1
        test   eax, eax
        jle    SHORT loc_4_1EB3
loc_4_1EA4:
        mov    dx, word ptr [ebx]
        add    ebx, 2
        mov    word ptr [edi], dx
        add    edi, 2
        dec    eax
        jne    SHORT loc_4_1EA4
loc_4_1EB3:
        add    ecx, 4
        mov    esi, dword ptr [esi + 40h]
        mov    eax, dword ptr [ebp - 8]
        inc    eax
        cdq
        sub    eax, edx
        sar    eax, 1
        test   ax, ax
        je     SHORT loc_4_1ED8
loc_4_1EC8:
        mov    dx, word ptr [ecx]
        add    ecx, 2
        mov    word ptr [esi], dx
        add    esi, 2
        dec    ax
        jne    SHORT loc_4_1EC8
loc_4_1ED8:
        mov    ax, word ptr [ebp - 2]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_4_1EE5:
        push   esi
        call   sub_4_1F00
        mov    ax, 0ffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_1F00:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    al, byte ptr [ecx + 3dh]
        mov    byte ptr [ecx + 3ch], al
        cmp    byte ptr [ecx + 38h], al
        jne    SHORT loc_4_1F2C
        mov    al, byte ptr [ecx + 3ah]
        dec    al
        push   eax
        mov    eax, dword ptr [ecx + 0ch]
        add    eax, 3
        push   eax
        push   ecx
        call   sub_4_1C20
        xor    ax, ax
        pop    ebp
        ret    4
loc_4_1F2C:
        dec    al
        push   eax
        mov    eax, dword ptr [ecx + 0ch]
        add    eax, 3
        push   eax
        push   ecx
        call   sub_4_1C20
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_1F50:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   1fh
        mov    ecx, dword ptr [eax + 0ch]
        add    ecx, 0fh
        push   ecx
        push   eax
        call   sub_4_1C20
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_4_1F70:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        cmp    word ptr [eax + 4ch], 3ch
        mov    ebp, esp
        jae    SHORT loc_4_1F84
        mov    word ptr [eax + 4ch], 3ch
loc_4_1F84:
        xor    ax, ax
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_1F90:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 4ch], 0
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 44h]
        pop    ebp
        mov    dword ptr [eax + 48h], ecx
        xor    ax, ax
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_1FB0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    dx, word ptr [esp + 10h]
        mov    ebp, esp
        test   dx, dx
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 48h]
        je     SHORT loc_4_1FD7
        mov    di, dx
        mov    ecx, dword ptr [ebp + 0ch]
loc_4_1FCD:
        mov    bl, byte ptr [ecx]
        inc    ecx
        mov    byte ptr [esi], bl
        inc    esi
        dec    di
        jne    SHORT loc_4_1FCD
loc_4_1FD7:
        add    word ptr [eax + 4ch], dx
        pop    edi
        mov    dword ptr [eax + 48h], esi
        pop    esi
        xor    ax, ax
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_1FF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [esi + 4ch]
        mov    ebx, dword ptr [esi + 44h]
        movzx  edi, byte ptr [esi + 39h]
        shl    edi, 8
        inc    eax
        cdq
        add    edi, dword ptr [esi + 14h]
        sub    eax, edx
        sar    eax, 1
        test   ax, ax
        je     SHORT loc_4_2028
loc_4_2018:
        mov    cx, word ptr [ebx]
        add    ebx, 2
        mov    word ptr [edi], cx
        add    edi, 2
        dec    ax
        jne    SHORT loc_4_2018
loc_4_2028:
        mov    ax, word ptr [esi + 4ch]
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 5
        push   eax
        push   esi
        call   sub_4_1C20
        mov    ax, word ptr [esi + 4ch]
        shr    ax, 8
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 6
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 39h]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        add    eax, 4
        push   eax
        push   esi
        call   sub_4_1C20
        push   26h
        mov    ecx, dword ptr [esi + 0ch]
        push   ecx
        push   esi
        call   sub_4_1C20
        xor    byte ptr [esi + 39h], 0eh
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_4_2080:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   0
        mov    ecx, dword ptr [eax + 0ch]
        add    ecx, 0fh
        push   ecx
        push   eax
        call   sub_4_1C20
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_20A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   1fh
        mov    ecx, dword ptr [eax + 0ch]
        add    ecx, 0fh
        push   ecx
        push   eax
        call   sub_4_1C20
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_20C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    di, di
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0ah]
        add    al, 8
        push   eax
        push   96h
        push   esi
        call   sub_4_1C20
        push   101h
        push   esi
        call   sub_4_1C80
        cmp    al, 56h
        jne    SHORT loc_4_213F
        push   100h
        push   esi
        call   sub_4_1C80
        test   al, al
        je     SHORT loc_4_2108
        cmp    al, 2
        je     SHORT loc_4_2108
        cmp    al, 4
        je     SHORT loc_4_2108
        cmp    al, 6
        jne    SHORT loc_4_211C
loc_4_2108:
        mov    word ptr [esi + 18h], 4000h
        mov    di, 1
        mov    cx, word ptr [esi + 0ah]
        add    cl, 8
        mov    byte ptr [esi + 58h], cl
loc_4_211C:
        cmp    al, 1
        je     SHORT loc_4_212C
        cmp    al, 3
        je     SHORT loc_4_212C
        cmp    al, 5
        je     SHORT loc_4_212C
        cmp    al, 7
        jne    SHORT loc_4_213F
loc_4_212C:
        mov    word ptr [esi + 18h], 0
        mov    di, 1
        mov    ax, word ptr [esi + 0ah]
        add    al, 8
        mov    byte ptr [esi + 58h], al
loc_4_213F:
        push   0
        push   96h
        push   esi
        call   sub_4_1C20
        test   di, di
        jne    SHORT loc_4_215C
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_215C:
        mov    al, byte ptr [esi + 58h]
        push   eax
        push   96h
        push   esi
        call   sub_4_1C20
        push   81h
        push   102h
        push   esi
        call   sub_4_1C20
        push   103h
        push   esi
        call   sub_4_1C80
        shr    al, 1
        and    eax, 7
        dec    eax
        cmp    eax, 6
        ja     SHORT loc_4_2199
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_2199:
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_21A4:
        lea    edi, [esi + 0ch]
        mov    dword ptr [edi], 1300h
        jmp    SHORT loc_4_21EF
loc_4_21AF:
        lea    edi, [esi + 0ch]
        mov    dword ptr [edi], 2300h
        jmp    SHORT loc_4_21EF
loc_4_21BA:
        lea    edi, [esi + 0ch]
        mov    dword ptr [edi], 3300h
        jmp    SHORT loc_4_21EF
loc_4_21C5:
        lea    edi, [esi + 0ch]
        mov    dword ptr [edi], 4300h
        jmp    SHORT loc_4_21EF
loc_4_21D0:
        lea    edi, [esi + 0ch]
        mov    dword ptr [edi], 5300h
        jmp    SHORT loc_4_21EF
loc_4_21DB:
        lea    edi, [esi + 0ch]
        mov    dword ptr [edi], 6300h
        jmp    SHORT loc_4_21EF
loc_4_21E6:
        lea    edi, [esi + 0ch]
        mov    dword ptr [edi], 7300h
loc_4_21EF:
        push   103h
        push   esi
        call   sub_4_1C80
        shr    al, 4
        and    eax, 7
        cmp    eax, 7
        ja     SHORT loc_4_220C
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_220C:
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_2217:
        mov    word ptr [esi + 12h], 0c000h
        jmp    SHORT loc_4_2255
loc_4_221F:
        mov    word ptr [esi + 12h], 0c400h
        jmp    SHORT loc_4_2255
loc_4_2227:
        mov    word ptr [esi + 12h], 0c800h
        jmp    SHORT loc_4_2255
loc_4_222F:
        mov    word ptr [esi + 12h], 0cc00h
        jmp    SHORT loc_4_2255
loc_4_2237:
        mov    word ptr [esi + 12h], 0d000h
        jmp    SHORT loc_4_2255
loc_4_223F:
        mov    word ptr [esi + 12h], 0d400h
        jmp    SHORT loc_4_2255
loc_4_2247:
        mov    word ptr [esi + 12h], 0d800h
        jmp    SHORT loc_4_2255
loc_4_224F:
        mov    word ptr [esi + 12h], 0dc00h
loc_4_2255:
        push   esi
        call   sub_4_1B30
        test   ax, ax
        je     SHORT loc_4_226B
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_226B:
        push   104h
        push   esi
        call   sub_4_1C80
        and    eax, 3
        cmp    eax, 3
        ja     SHORT loc_4_2285
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_2285:
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_2290:
        mov    word ptr [esi + 10h], 3
        jmp    SHORT loc_4_22AE
loc_4_2298:
        mov    word ptr [esi + 10h], 7
        jmp    SHORT loc_4_22AE
loc_4_22A0:
        mov    word ptr [esi + 10h], 9
        jmp    SHORT loc_4_22AE
loc_4_22A8:
        mov    word ptr [esi + 10h], 0ch
loc_4_22AE:
        xor    eax, eax
        mov    edx, dword ptr [esi + 14h]
        mov    word ptr [esi + 1ah], ax
loc_4_22B7:
        mov    ecx, eax
        add    edx, 2
        inc    ax
        and    ecx, 0ffffh
        cmp    ax, 6
        lea    ebx, [ecx + esi]
        mov    cl, byte ptr [edx - 2]
        mov    byte ptr [ebx + 22h], cl
        mov    byte ptr [ebx + 1ch], cl
        jb     SHORT loc_4_22B7
        push   0
        push   96h
        push   esi
        call   sub_4_1C20
        mov    eax, dword ptr [edi]
        push   eax
        push   esi
        call   sub_4_1C80
        cmp    al, 0ffh
        jne    SHORT loc_4_22FB
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_22FB:
        push   21h
        mov    eax, dword ptr [edi]
        push   eax
        push   esi
        call   sub_4_1C20
        mov    eax, dword ptr [edi]
        push   eax
        push   esi
        call   sub_4_1C80
        cmp    al, 21h
        je     SHORT loc_4_2322
        cmp    al, 23h
        je     SHORT loc_4_2322
        mov    ax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_2322:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh, 064h, 024h, 000h
        db  08Dh, 064h, 024h, 000h
sub_4_2380:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [esi + 38h], 1
        mov    byte ptr [esi + 3ch], 2
        mov    dx, word ptr [esi + 18h]
        inc    edx
        mov    ecx, dword ptr [esi + 14h]
        shr    edx, 8
        lea    eax, [ecx + 100h]
        and    dl, 0feh
        mov    dword ptr [esi + 54h], eax
        sub    dl, 0ch
        xor    eax, eax
        mov    byte ptr [esi + 39h], dl
        sub    dl, 4
        mov    al, dl
        push   0
        shl    eax, 8
        mov    byte ptr [esi + 3ah], dl
        add    eax, ecx
        mov    dword ptr [esi + 50h], eax
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0fh
        push   eax
        push   esi
        call   sub_4_1C20
        push   21h
        mov    ecx, dword ptr [esi + 0ch]
        push   ecx
        push   esi
        call   sub_4_1C20
        push   49h
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0eh
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0ah
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0bh
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ch
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0ch
        push   eax
        push   esi
        call   sub_4_1C20
        push   4
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0dh
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 38h]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        inc    eax
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 38h]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        add    eax, 3
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 3ah]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        add    eax, 2
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ffh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 7
        push   eax
        push   esi
        call   sub_4_1C20
        push   61h
        mov    ecx, dword ptr [esi + 0ch]
        push   ecx
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 1ch]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        inc    eax
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 1dh]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        add    eax, 2
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 1eh]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        add    eax, 3
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 1fh]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        add    eax, 4
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 20h]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        add    eax, 5
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 21h]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        add    eax, 6
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 3ch]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        add    eax, 7
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 8
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 9
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0ah
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0bh
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0ch
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0dh
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0eh
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0fh
        push   eax
        push   esi
        call   sub_4_1C20
        push   22h
        mov    ecx, dword ptr [esi + 0ch]
        push   ecx
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0dh
        push   eax
        push   esi
        call   sub_4_1C20
        push   12h
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 20h
        push   eax
        push   esi
        call   sub_4_1C20
        push   4
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 23h
        push   eax
        push   esi
        call   sub_4_1C20
        push   4
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 22h
        push   eax
        push   esi
        call   sub_4_1C20
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_25C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        push   esi
        call   sub_4_1C80
        mov    bl, al
        push   21h
        and    bl, 4
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        push   esi
        call   sub_4_1C20
        push   2
        push   esi
        call   sub_4_1C00
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0ah
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0bh
        push   eax
        push   esi
        call   sub_4_1C20
        test   bl, bl
        jne    SHORT loc_4_2615
        mov    word ptr [esi + 4eh], 0
        jmp    SHORT loc_4_2642
loc_4_2615:
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 7
        push   eax
        push   esi
        call   sub_4_1C80
        and    ax, 0ffh
        mov    cl, byte ptr [esi + 3eh]
        mov    word ptr [esi + 4eh], 0
        and    ecx, 0ffff00ffh
        or     ax, cx
        test   al, 0ah
        jne    SHORT loc_4_2642
        mov    word ptr [esi + 4eh], 1
loc_4_2642:
        push   2
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0dh
        push   eax
        push   esi
        call   sub_4_1C20
        push   22h
        mov    ecx, dword ptr [esi + 0ch]
        push   ecx
        push   esi
        call   sub_4_1C20
        xor    ax, ax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_2670:
        push   ebp
        mov    ebp, esp
        push   esi
        push   10h
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 7
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0dh
        push   eax
        push   esi
        call   sub_4_1C20
        cmp    word ptr [esi + 4eh], 0
        je     SHORT loc_4_26A8
        push   26h
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        push   esi
        call   sub_4_1C20
loc_4_26A8:
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
sub_4_26B0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   0
        mov    di, word ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [esi + 34h], di
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0fh
        push   eax
        push   esi
        call   sub_4_1C20
        movsx  eax, di
        dec    eax
        cmp    eax, 5
        ja     NEAR PTR loc_4_2920
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_4_26E3:
        push   21h
        mov    eax, dword ptr [esi + 0ch]
        jmp    NEAR PTR loc_4_2919
loc_4_26ED:
        push   22h
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        jmp    NEAR PTR loc_4_2913
loc_4_2700:
        push   22h
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        push   esi
        call   sub_4_1C20
        push   4
        jmp    NEAR PTR loc_4_2913
loc_4_2713:
        push   62h
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        push   esi
        call   sub_4_1C20
        mov    al, byte ptr [esi + 28h]
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 8
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 29h]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        add    eax, 9
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 2ah]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        add    eax, 0ah
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 2bh]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        add    eax, 0bh
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 2ch]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        add    eax, 0ch
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 2dh]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        add    eax, 0dh
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 2eh]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        add    eax, 0eh
        push   eax
        push   esi
        call   sub_4_1C20
        mov    cl, byte ptr [esi + 2fh]
        mov    eax, dword ptr [esi + 0ch]
        push   ecx
        add    eax, 0fh
        push   eax
        push   esi
        call   sub_4_1C20
        push   22h
        mov    ecx, dword ptr [esi + 0ch]
        push   ecx
        push   esi
        call   sub_4_1C20
        push   0ch
        jmp    NEAR PTR loc_4_2913
loc_4_27BA:
        push   62h
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ffh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 8
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ffh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 9
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ffh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0ah
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ffh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0bh
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ffh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0ch
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ffh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0dh
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ffh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0eh
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ffh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0fh
        push   eax
        push   esi
        call   sub_4_1C20
        push   22h
        mov    ecx, dword ptr [esi + 0ch]
        push   ecx
        push   esi
        call   sub_4_1C20
        push   0ch
        jmp    NEAR PTR loc_4_2913
loc_4_2869:
        push   62h
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ffh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 8
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ffh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 9
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ffh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0ah
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ffh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0bh
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ffh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0ch
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ffh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0dh
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ffh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0eh
        push   eax
        push   esi
        call   sub_4_1C20
        push   0ffh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0fh
        push   eax
        push   esi
        call   sub_4_1C20
        push   22h
        mov    ecx, dword ptr [esi + 0ch]
        push   ecx
        push   esi
        call   sub_4_1C20
        push   1fh
loc_4_2913:
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0ch
loc_4_2919:
        push   eax
        push   esi
        call   sub_4_1C20
loc_4_2920:
        push   1fh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0fh
        push   eax
        push   esi
        call   sub_4_1C20
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_4_2950:
        push   ebp
        cmp    word ptr [esp + 10h], 0
        mov    ebp, esp
        push   esi
        jne    NEAR PTR loc_4_2A24
        mov    esi, dword ptr [ebp + 8]
        push   0
        lea    eax, [esi + 28h]
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 4], 0
        mov    byte ptr [eax + 6], 0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0fh
        push   eax
        push   esi
        call   sub_4_1C20
        push   62h
        mov    ecx, dword ptr [esi + 0ch]
        push   ecx
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 8
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 9
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0ah
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0bh
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0ch
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0dh
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0eh
        push   eax
        push   esi
        call   sub_4_1C20
        push   0
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0fh
        push   eax
        push   esi
        call   sub_4_1C20
        push   22h
        mov    ecx, dword ptr [esi + 0ch]
        push   ecx
        push   esi
        call   sub_4_1C20
        push   1fh
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 0fh
        push   eax
        push   esi
        call   sub_4_1C20
loc_4_2A24:
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_4_2A30:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   eax
        call   sub_4_2B40
        mov    bx, ax
        mov    edi, dword ptr [ebp + 8]
        push   0
        mov    eax, dword ptr [edi + 0ch]
        add    eax, 0fh
        push   eax
        push   edi
        call   sub_4_1C20
        push   62h
        mov    ecx, dword ptr [edi + 0ch]
        push   ecx
        push   edi
        call   sub_4_1C20
        mov    ax, bx
        sar    ax, 3
        movsx  esi, ax
        mov    eax, dword ptr [edi + 0ch]
        and    bl, 7
        add    eax, esi
        add    eax, 8
        push   eax
        push   edi
        call   sub_4_1C80
        mov    dl, 1
        mov    cl, bl
        shl    dl, cl
        or     dl, al
        mov    eax, dword ptr [edi + 0ch]
        push   edx
        add    eax, esi
        add    eax, 8
        push   eax
        push   edi
        call   sub_4_1C20
        mov    eax, dword ptr [edi + 0ch]
        add    eax, 8
        push   eax
        push   edi
        call   sub_4_1C80
        mov    byte ptr [edi + 28h], al
        mov    eax, dword ptr [edi + 0ch]
        add    eax, 9
        push   eax
        push   edi
        call   sub_4_1C80
        mov    byte ptr [edi + 29h], al
        mov    eax, dword ptr [edi + 0ch]
        add    eax, 0ah
        push   eax
        push   edi
        call   sub_4_1C80
        mov    byte ptr [edi + 2ah], al
        mov    eax, dword ptr [edi + 0ch]
        add    eax, 0bh
        push   eax
        push   edi
        call   sub_4_1C80
        mov    byte ptr [edi + 2bh], al
        mov    eax, dword ptr [edi + 0ch]
        add    eax, 0ch
        push   eax
        push   edi
        call   sub_4_1C80
        mov    byte ptr [edi + 2ch], al
        mov    eax, dword ptr [edi + 0ch]
        add    eax, 0dh
        push   eax
        push   edi
        call   sub_4_1C80
        mov    byte ptr [edi + 2dh], al
        mov    eax, dword ptr [edi + 0ch]
        add    eax, 0eh
        push   eax
        push   edi
        call   sub_4_1C80
        mov    byte ptr [edi + 2eh], al
        mov    eax, dword ptr [edi + 0ch]
        add    eax, 0fh
        push   eax
        push   edi
        call   sub_4_1C80
        push   22h
        mov    ecx, dword ptr [edi + 0ch]
        push   ecx
        mov    byte ptr [edi + 2fh], al
        push   edi
        call   sub_4_1C20
        push   1fh
        mov    eax, dword ptr [edi + 0ch]
        add    eax, 0fh
        push   eax
        push   edi
        call   sub_4_1C20
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_4_2B40:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        mov    ecx, dword ptr [eax]
        push   ebx
        mov    ax, word ptr [eax + 4]
        push   esi
        push   edi
        lea    edx, [ebp - 8]
        mov    word ptr [ebp - 2], 0
        mov    dword ptr [edx], ecx
        mov    word ptr [edx + 4], ax
        mov    eax, 0ffffffffh
loc_4_2B67:
        movsx  ecx, word ptr [ebp - 2]
        mov    si, 8
        lea    ecx, [ebp + ecx - 8]
loc_4_2B73:
        mov    edx, eax
        and    edx, 80000000h
        cmp    edx, 1
        mov    dl, byte ptr [ecx]
        sbb    edi, edi
        mov    bl, dl
        inc    di
        add    eax, eax
        shr    dl, 1
        and    bx, 1
        mov    byte ptr [ecx], dl
        xor    di, bx
        test   di, di
        je     SHORT loc_4_2BA3
        movsx  edx, di
        xor    eax, 4c11db7h
        or     eax, edx
loc_4_2BA3:
        dec    si
        jne    SHORT loc_4_2B73
        inc    word ptr [ebp - 2]
        cmp    word ptr [ebp - 2], 6
        jl     SHORT loc_4_2B67
        and    eax, 0ff000000h
        pop    edi
        shr    eax, 1ah
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0C2h, 004h, 000h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_2BE0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 4ah]
        cmp    byte ptr [esi + 92h], 0
        jne    SHORT loc_4_2C52
        cmp    byte ptr [edi + 40h], 0
        jne    SHORT loc_4_2C4B
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        inc    dword ptr [edi + 14h]
        test   ecx, ecx
        jne    SHORT loc_4_2C2F
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [esi + 82h]
        add    eax, 0eh
        mov    edx, dword ptr [ebp + 14h]
        push   eax
        push   ecx
        push   edx
        mov    eax, dword ptr [ebp + 18h]
        push   0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2C28:
        dec    dword ptr [edi + 14h]
        xor    edi, edi
        jmp    SHORT loc_4_2C57
loc_4_2C2F:
        push   0
        mov    edx, dword ptr [ebp + 18h]
        push   edx
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, ecx
        push   eax
        call   sub_4_08F0
        dec    dword ptr [edi + 14h]
        xor    edi, edi
        jmp    SHORT loc_4_2C57
loc_4_2C4B:
        mov    edi, 0c001000ch
        jmp    SHORT loc_4_2C57
loc_4_2C52:
        mov    edi, 0c001000dh
loc_4_2C57:
        cmp    byte ptr [esi + 15h], 0
        jne    SHORT loc_4_2C8D
        cmp    dword ptr [esi + 4ah], 2
        jne    SHORT loc_4_2C6C
        cmp    byte ptr [esi + 92h], 0
        jne    SHORT loc_4_2C76
loc_4_2C6C:
        mov    eax, dword ptr [esi + 5ah]
        sub    eax, esi
        cmp    eax, 5ah
        je     SHORT loc_4_2C8D
loc_4_2C76:
        push   0
        add    esi, 18h
        push   esi
        mov    byte ptr [esi - 3], 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2C85:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
loc_4_2C8D:
        mov    eax, edi
        dec    dword ptr [esi + 4ah]
        pop    edi
        pop    esi
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_2CA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 5ch
        mov    word ptr [ebp - 24h], 0
        mov    word ptr [ebp - 22h], 8
        mov    word ptr [ebp - 20h], 5600h
        mov    word ptr [ebp - 1eh], 5601h
        mov    word ptr [ebp - 1ch], 5602h
        mov    word ptr [ebp - 1ah], 5603h
        mov    word ptr [ebp - 18h], 5604h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    ecx, [ebp - 24h]
        push   ecx
        lea    edx, [ebp - 4]
        mov    word ptr [ebp - 16h], 5605h
        mov    word ptr [ebp - 14h], 5606h
        mov    word ptr [ebp - 12h], 5607h
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2CF5:
        push   -1
        lea    ecx, [ebp - 0ch]
        push   0
        push   8
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2D04:
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_4_2D16
        mov    eax, 0c000009ah
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_2D16:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx], eax
        mov    byte ptr [ebp - 5ch], 3
        mov    byte ptr [ebp - 5bh], 0
        db  0C7h, 045h, 0AAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0AEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0B2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0B6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0BAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0BEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C2h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        db  0A1h, 004h, 000h, 000h, 000h
        push   36h
        mov    dword ptr [ebp - 2eh], edx
        lea    ecx, [ebp - 5ch]
        mov    dword ptr [ebp - 2ah], eax
        push   ecx
        mov    edx, dword ptr [ebp - 0ch]
        push   edx
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    ecx, [ebp - 8]
        db  0C7h, 045h, 0C6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CEh, 000h, 000h, 000h, 000h
        lea    eax, [edx + 4]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2D98:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_4_2DA6
        xor    eax, eax
        mov    esp, ebp
        pop    ebp
        ret    8
loc_4_2DA6:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2DB1:
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_4_2DC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        lea    eax, [esi + 1789h]
        push   eax
        mov    dword ptr [esi + 1789h], esi
        mov    dword ptr [esi + 178dh], ebx
        call   sub_4_1A00
        test   ax, ax
        je     SHORT loc_4_2DF0
        mov    byte ptr [ebp + 14h], 1
loc_4_2DF0:
        lea    edi, [ebp - 2ch]
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    dword ptr [ebp - 2ch], eax
        mov    byte ptr [ebp - 28h], al
        mov    byte ptr [ebp - 27h], al
        mov    dword ptr [ebp - 26h], eax
        mov    dword ptr [ebp - 1ah], 5eah
        mov    dword ptr [ebp - 16h], 1
        mov    dword ptr [ebp - 0eh], 4
        lea    edx, [ebp - 2ch]
        mov    dword ptr [ebp - 1eh], eax
        lea    eax, [esi + 1795h]
        push   edx
        mov    dword ptr [ebp - 22h], 3
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 0ah], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 12h], ecx
        push   eax
        mov    ecx, dword ptr [esi]
        push   ebx
        lea    edx, [esi + 66h]
        push   esi
        push   ecx
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2E4D:
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    NEAR PTR loc_4_3138
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_4_2E88
        push   0
        mov    eax, dword ptr [esi + 66h]
        push   0aa00000ch
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2E6E:
        add    esp, 0ch
        mov    eax, dword ptr [esi + 66h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2E7A:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_2E88:
        lea    eax, [esi + 1789h]
        push   eax
        call   sub_4_20C0
        test   ax, ax
        je     SHORT loc_4_2ED0
        cmp    ax, 2
        push   0
        jne    SHORT loc_4_2EA8
        push   0aa000001h
        jmp    SHORT loc_4_2EAD
loc_4_2EA8:
        push   0aa000003h
loc_4_2EAD:
        mov    eax, dword ptr [esi + 66h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2EB6:
        add    esp, 0ch
        mov    eax, dword ptr [esi + 66h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2EC2:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_2ED0:
        lea    edi, [esi + 17abh]
        push   6
        push   edi
        lea    eax, [esi + 6]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2EE2:
        push   6
        lea    eax, [esi + 0ch]
        push   edi
        xor    ebx, ebx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_2EF0:
        lea    eax, [esi + 4eh]
        mov    dword ptr [esi + 56h], ebx
        mov    dword ptr [esi + 52h], eax
        mov    byte ptr [esi + 5], bl
        mov    dword ptr [eax], eax
        mov    byte ptr [esi + 12h], bl
        lea    eax, [esi + 5ah]
        mov    byte ptr [esi + 13h], bl
        mov    dword ptr [esi + 5eh], eax
        mov    byte ptr [esi + 15h], bl
        mov    dword ptr [eax], eax
        mov    byte ptr [esi + 16h], bl
        mov    byte ptr [esi + 17h], bl
        mov    dword ptr [esi + 7ah], ebx
        mov    dword ptr [esi + 7eh], ebx
        mov    dword ptr [esi + 86h], ebx
        mov    eax, 1
        mov    dword ptr [esi + 8ah], ebx
        mov    dword ptr [esi + 0b6h], ebx
        mov    dword ptr [esi + 0bah], ebx
        mov    byte ptr [esi + 96h], al
        mov    byte ptr [esi + 97h], al
        mov    byte ptr [esi + 98h], al
        mov    byte ptr [esi + 99h], al
        mov    byte ptr [esi + 9ah], bl
        mov    byte ptr [esi + 9bh], bl
        mov    byte ptr [esi + 9ch], bl
        mov    byte ptr [esi + 9dh], bl
        mov    dword ptr [esi + 9eh], ebx
        mov    dword ptr [esi + 0a2h], ebx
        mov    dword ptr [esi + 0a6h], ebx
        mov    dword ptr [esi + 0aah], ebx
        mov    dword ptr [esi + 0aeh], ebx
        mov    dword ptr [esi + 0b2h], ebx
        mov    dword ptr [esi + 4ah], eax
        mov    byte ptr [esi + 92h], bl
        lea    eax, [esi + 0beh]
        mov    byte ptr [esi + 93h], bl
        mov    byte ptr [esi + 94h], bl
        mov    dword ptr [esi + 8eh], ebx
        lea    edi, [esi + 0d2h]
        mov    byte ptr [esi + 4], bl
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax + 0ch], ebx
        mov    dword ptr [eax + 10h], ebx
        xor    eax, eax
        mov    ecx, 0ch
        rep stosd
        mov    ecx, 8
        lea    edi, [esi + 102h]
        rep stosd
        mov    ecx, 7
        lea    eax, [esi + 122h]
        lea    edi, [esi + 12eh]
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax + 8], ebx
        xor    eax, eax
        rep stosd
        mov    dword ptr [esi + 76h], ebx
        mov    dword ptr [esi + 18h], ebx
        mov    dword ptr [esi + 20h], esi
        mov    dword ptr [esi + 24h], ebx
        lea    ecx, [esi + 6ah]
        lea    edx, [esi + 62h]
        push   ecx
        lea    eax, [esi + 0ch]
        push   edx
        mov    dword ptr [esi + 2ch], esi
        db  0C7h, 046h, 01Ch, 000h, 000h, 000h, 000h
        db  0C7h, 046h, 028h, 000h, 000h, 000h, 000h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   10h
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3036:
        test   al, al
        jne    SHORT loc_4_3067
        push   2
        mov    eax, dword ptr [esi + 66h]
        push   ebx
        push   2
        push   0aa000002h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_304D:
        add    esp, 14h
        mov    eax, dword ptr [esi + 66h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3059:
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3067:
        xor    eax, eax
        lea    edi, [esi + 30h]
        mov    ax, word ptr [esi + 1799h]
        push   2
        push   1
        mov    ecx, dword ptr [esi + 66h]
        push   eax
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3092:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_4_30CD
        push   1
        mov    eax, dword ptr [esi + 66h]
        push   0
        push   2
        push   0aa000005h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_30AC:
        add    esp, 14h
        mov    eax, dword ptr [esi + 6ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_30B8:
        mov    eax, dword ptr [esi + 66h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_30C1:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_30CD:
        lea    eax, [esi + 1789h]
        push   eax
        call   sub_4_2380
        test   ax, ax
        je     SHORT loc_4_3104
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_30E4:
        mov    eax, dword ptr [esi + 6ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_30ED:
        mov    eax, dword ptr [esi + 66h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_30F6:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_4_3104:
        lea    eax, [esi + 789h]
        lea    ecx, [esi + 0f89h]
        lea    edx, [esi + 1789h]
        mov    dword ptr [esi + 17c9h], eax
        mov    dword ptr [esi + 17cdh], ecx
        push   edx
        add    esi, 24h
        call   sub_4_20A0
        push   3e8h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3136:
        xor    eax, eax
loc_4_3138:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3150:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 20h]
        cmp    byte ptr [esi + 5], 0
        je     SHORT loc_4_316E
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_316E:
        xor    edx, edx
        mov    ecx, dword ptr [ebp + 18h]
        cmp    ecx, edx
        je     SHORT loc_4_318B
        mov    eax, dword ptr [ebp + 14h]
loc_4_317A:
        cmp    dword ptr [eax], 0
        je     SHORT loc_4_3187
        add    eax, 4
        inc    edx
        cmp    ecx, edx
        ja     SHORT loc_4_317A
loc_4_3187:
        cmp    ecx, edx
        jne    SHORT loc_4_3198
loc_4_318B:
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_3198:
        mov    eax, dword ptr [ebp + 10h]
        push   -1
        push   0
        lea    ecx, [ebp - 4]
        push   45h
        mov    dword ptr [eax], edx
        push   ecx
        inc    dword ptr [esi + 4ah]
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_31AF:
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_4_3223
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx + 4], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 10h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 14h], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 40h], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 41h], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], esi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 28h], 3
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], ecx
        mov    dword ptr [eax + 18h], 0
        add    eax, 18h
        mov    edi, 103h
        mov    eax, dword ptr [ebp - 4]
        add    eax, 18h
        push   eax
        push   esi
        call   sub_4_0A70
        jmp    SHORT loc_4_323A
loc_4_3223:
        push   9
        mov    eax, dword ptr [esi + 66h]
        push   1
        push   2
        push   0aa000002h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3237:
        add    esp, 14h
loc_4_323A:
        cmp    byte ptr [esi + 15h], 0
        jne    SHORT loc_4_3272
        cmp    dword ptr [esi + 4ah], 2
        jne    SHORT loc_4_324F
        cmp    byte ptr [esi + 92h], 0
        jne    SHORT loc_4_3259
loc_4_324F:
        mov    eax, dword ptr [esi + 5ah]
        sub    eax, esi
        cmp    eax, 5ah
        je     SHORT loc_4_3272
loc_4_3259:
        push   0
        add    esi, 18h
        push   esi
        mov    byte ptr [esi - 3], 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3268:
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_4_3272:
        mov    eax, edi
        dec    dword ptr [esi + 4ah]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  090h
loc_4_3280:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   -1
        lea    eax, [ebp - 4]
        push   ebx
        mov    dword ptr [ebp - 8], ebx
        push   1807h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_329F:
        mov    dword ptr [ebp - 8], eax
        cmp    eax, ebx
        je     SHORT loc_4_32B4
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_32B4:
        mov    edi, dword ptr [ebp - 4]
        xor    eax, eax
        mov    ecx, 601h
        rep stosd
        stosw
        stosb
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_4_3840
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 4]
        push   esi
        lea    eax, [ebp - 0ch]
        push   eax
        mov    dword ptr [ecx], edx
        lea    ecx, [ebp - 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_32E8:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_330C
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   1807h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_32FE:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_330C:
        lea    eax, [ebp - 18h]
        lea    ecx, [ebp - 10h]
        push   eax
        lea    edx, [ebp - 8]
        push   ecx
        push   esi
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_331E:
        mov    cl, byte ptr [ebp - 16h]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [edx + 17e2h], cl
        mov    cl, byte ptr [ebp - 15h]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [edx + 17e3h], cl
        mov    cl, byte ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [edx + 17e4h], cl
        mov    cl, byte ptr [ebp - 13h]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [edx + 17e5h], cl
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 10h]
        dec    ax
        mov    word ptr [edx + 1793h], ax
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_4_3365
        mov    bl, 1
loc_4_3365:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_336E:
        push   ebx
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        push   esi
        push   ecx
        call   sub_4_2DC0
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_4_3394
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   1807h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3394:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_4_33A0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [esi + 4ah]
        cmp    byte ptr [edi + 40h], 0
        jne    SHORT loc_4_33DB
        mov    dword ptr [edi + 28h], 4
        mov    dword ptr [edi + 1ch], edi
        lea    eax, [edi + 18h]
        push   eax
        push   esi
        mov    dword ptr [eax], 0
        inc    dword ptr [edi + 14h]
        call   sub_4_0A70
        dec    dword ptr [edi + 14h]
        mov    edi, 103h
        jmp    SHORT loc_4_33E0
loc_4_33DB:
        mov    edi, 0c0010002h
loc_4_33E0:
        cmp    byte ptr [esi + 15h], 0
        jne    SHORT loc_4_3416
        cmp    dword ptr [esi + 4ah], 2
        jne    SHORT loc_4_33F5
        cmp    byte ptr [esi + 92h], 0
        jne    SHORT loc_4_33FF
loc_4_33F5:
        mov    eax, dword ptr [esi + 5ah]
        sub    eax, esi
        cmp    eax, 5ah
        je     SHORT loc_4_3416
loc_4_33FF:
        push   0
        add    esi, 18h
        push   esi
        mov    byte ptr [esi - 3], 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_340E:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_4_3416:
        mov    eax, edi
        dec    dword ptr [esi + 4ah]
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_3430:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 1789h]
        push   eax
        call   sub_4_2080
        push   esi
        lea    eax, [esi + 30h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3454:
        xor    eax, eax
        lea    edi, [esi + 6eh]
        mov    byte ptr [esi + 96h], al
        mov    byte ptr [esi + 97h], al
        mov    ecx, dword ptr [edi]
        mov    byte ptr [esi + 98h], al
        mov    byte ptr [esi + 99h], al
        test   ecx, ecx
        mov    byte ptr [esi + 92h], 1
        je     SHORT loc_4_34CB
        cmp    dword ptr [ecx + 10h], 4
        jne    SHORT loc_4_3490
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edi], eax
        mov    ecx, dword ptr [ecx + 4]
        dec    dword ptr [ecx + 14h]
        mov    ecx, dword ptr [edi]
loc_4_3490:
        test   ecx, ecx
        je     SHORT loc_4_34C4
loc_4_3494:
        mov    eax, dword ptr [ecx + 10h]
        cmp    eax, 4
        je     SHORT loc_4_34BC
        cmp    eax, 3
        je     SHORT loc_4_34BC
        mov    eax, dword ptr [ecx]
        push   0c001000dh
        push   ecx
        mov    dword ptr [edi], eax
        mov    ebx, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebx + 10h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_34B7:
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_4_34BE
loc_4_34BC:
        mov    edi, ecx
loc_4_34BE:
        mov    ecx, dword ptr [edi]
        test   ecx, ecx
        jne    SHORT loc_4_3494
loc_4_34C4:
        mov    byte ptr [esi + 95h], 0
loc_4_34CB:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_4_34E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 1789h]
        push   eax
        call   sub_4_2080
        xor    eax, eax
        mov    ecx, 1
        mov    word ptr [esi + 48h], ax
        mov    byte ptr [esi + 96h], cl
        mov    byte ptr [esi + 97h], cl
        mov    byte ptr [esi + 98h], cl
        mov    byte ptr [esi + 99h], cl
        mov    byte ptr [esi + 9ah], al
        mov    byte ptr [esi + 9bh], al
        mov    byte ptr [esi + 9ch], al
        mov    byte ptr [esi + 9dh], al
        cmp    byte ptr [esi + 18bh], cl
        jne    SHORT loc_4_3570
        mov    eax, dword ptr [esi + 197h]
        mov    edi, dword ptr [eax + 2ch]
        mov    cl, byte ptr [eax + 33h]
        lea    edx, [eax + 24h]
        inc    cl
        cmp    cl, 3
        mov    byte ptr [edx + 0fh], cl
        jne    SHORT loc_4_3569
        push   0c001000ch
        push   eax
        mov    eax, dword ptr [edi + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3560:
        dec    dword ptr [esi + 11eh]
        dec    dword ptr [edi + 14h]
loc_4_3569:
        mov    byte ptr [esi + 18bh], 0
loc_4_3570:
        cmp    byte ptr [esi + 18ah], 1
        jne    SHORT loc_4_35B1
        mov    eax, dword ptr [esi + 19bh]
        mov    edi, dword ptr [eax + 2ch]
        mov    cl, byte ptr [eax + 33h]
        lea    edx, [eax + 24h]
        inc    cl
        cmp    cl, 3
        mov    byte ptr [edx + 0fh], cl
        jne    SHORT loc_4_35AA
        push   0c001000ch
        push   eax
        mov    eax, dword ptr [edi + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_35A1:
        dec    dword ptr [esi + 11eh]
        dec    dword ptr [edi + 14h]
loc_4_35AA:
        mov    byte ptr [esi + 18ah], 0
loc_4_35B1:
        mov    ebx, dword ptr [esi + 9eh]
        test   ebx, ebx
        je     SHORT loc_4_35ED
loc_4_35BB:
        mov    edi, dword ptr [ebx + 2ch]
        mov    al, byte ptr [ebx + 33h]
        mov    ecx, ebx
        lea    edx, [ebx + 24h]
        inc    al
        mov    ebx, dword ptr [edx]
        mov    byte ptr [edx + 0fh], al
        cmp    al, 3
        jne    SHORT loc_4_35E9
        push   0c001000ch
        mov    eax, dword ptr [edi + 10h]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_35E0:
        dec    dword ptr [esi + 11eh]
        dec    dword ptr [edi + 14h]
loc_4_35E9:
        test   ebx, ebx
        jne    SHORT loc_4_35BB
loc_4_35ED:
        mov    ebx, dword ptr [esi + 7ah]
        test   ebx, ebx
        je     SHORT loc_4_3627
loc_4_35F4:
        mov    edi, dword ptr [ebx + 2ch]
        mov    al, byte ptr [ebx + 33h]
        mov    ecx, ebx
        lea    edx, [ebx + 24h]
        inc    al
        mov    ebx, dword ptr [edx + 4]
        mov    byte ptr [edx + 0fh], al
        cmp    al, 3
        jne    SHORT loc_4_3623
        push   0c001000ch
        mov    eax, dword ptr [edi + 10h]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_361A:
        dec    dword ptr [esi + 11eh]
        dec    dword ptr [edi + 14h]
loc_4_3623:
        test   ebx, ebx
        jne    SHORT loc_4_35F4
loc_4_3627:
        xor    eax, eax
        mov    dword ptr [esi + 82h], eax
        mov    dword ptr [esi + 7ah], eax
        mov    dword ptr [esi + 7eh], eax
        mov    dword ptr [esi + 86h], eax
        mov    dword ptr [esi + 8ah], eax
        mov    dword ptr [esi + 0aeh], eax
        mov    dword ptr [esi + 0b2h], eax
        mov    dword ptr [esi + 0a6h], eax
        mov    dword ptr [esi + 0aah], eax
        mov    dword ptr [esi + 9eh], eax
        push   2
        mov    dword ptr [esi + 0a2h], eax
        lea    ecx, [esi + 1789h]
        mov    dword ptr [esi + 0b6h], eax
        mov    dword ptr [esi + 0bah], eax
        mov    dword ptr [esi + 11eh], eax
        push   ecx
        add    esi, 1789h
        call   sub_4_26B0
        push   esi
        call   sub_4_20A0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_4_36A0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [edi + 4ah]
        cmp    byte ptr [edi + 92h], 0
        jne    SHORT loc_4_372B
        cmp    byte ptr [edi + 93h], 0
        jne    SHORT loc_4_372B
        cmp    byte ptr [ebx + 40h], 0
        jne    SHORT loc_4_3724
        mov    byte ptr [edi + 93h], 1
        cmp    byte ptr [edi + 94h], 0
        jne    SHORT loc_4_3714
        mov    esi, dword ptr [edi + 4eh]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, -4eh
        je     SHORT loc_4_3706
loc_4_36E3:
        push   0
        inc    dword ptr [esi + 14h]
        push   0
        mov    eax, dword ptr [esi + 10h]
        push   40010004h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_36F8:
        mov    eax, edi
        dec    dword ptr [esi + 14h]
        mov    esi, dword ptr [esi]
        sub    eax, esi
        cmp    eax, -4eh
        jne    SHORT loc_4_36E3
loc_4_3706:
        mov    byte ptr [edi + 93h], 0
        push   ebx
        push   edi
        call   sub_4_3430
loc_4_3714:
        mov    esi, 103h
        inc    dword ptr [ebx + 14h]
        mov    dword ptr [edi + 8eh], ebx
        jmp    SHORT loc_4_3730
loc_4_3724:
        mov    esi, 0c0010002h
        jmp    SHORT loc_4_3730
loc_4_372B:
        mov    esi, 0c001000dh
loc_4_3730:
        cmp    byte ptr [edi + 15h], 0
        jne    SHORT loc_4_3767
        cmp    dword ptr [edi + 4ah], 2
        jne    SHORT loc_4_3745
        cmp    byte ptr [edi + 92h], 0
        jne    SHORT loc_4_374F
loc_4_3745:
        mov    eax, dword ptr [edi + 5ah]
        sub    eax, edi
        cmp    eax, 5ah
        je     SHORT loc_4_3767
loc_4_374F:
        push   0
        add    edi, 18h
        push   edi
        mov    byte ptr [edi - 3], 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_375E:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_4_3767:
        mov    eax, esi
        dec    dword ptr [edi + 4ah]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3780:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 1789h]
        push   eax
        call   sub_4_2080
        mov    word ptr [esi + 48h], 0
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
loc_4_37A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], bl
        mov    byte ptr [esi + 5], 1
        lea    eax, [esi + 1789h]
        push   eax
        call   sub_4_2080
        lea    eax, [esi + 30h]
        lea    edi, [esi + 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_37CD:
        mov    eax, dword ptr [esi + 6ah]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_37D6:
        mov    byte ptr [ebp - 1], bl
loc_4_37D9:
        lea    eax, [ebp - 1]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_37E3:
        cmp    byte ptr [ebp - 1], bl
        je     SHORT loc_4_37D9
        mov    eax, dword ptr [esi + 66h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_37F1:
        push   0
        push   1807h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_37FE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_4_3810:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    ecx, [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3827:
        push   0
        mov    ecx, dword ptr [esi]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_4_3831:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_4_3840:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 152h], ecx
        mov    dword ptr [eax + 156h], ecx
        mov    dword ptr [eax + 15ah], ecx
        mov    dword ptr [eax + 15eh], ecx
        mov    dword ptr [eax + 162h], ecx
        mov    dword ptr [eax + 166h], ecx
        mov    dword ptr [eax + 16ah], ecx
        mov    dword ptr [eax + 16eh], ecx
        mov    dword ptr [eax + 172h], ecx
        mov    dword ptr [eax + 176h], ecx
        mov    dword ptr [eax + 17ah], ecx
        mov    dword ptr [eax + 17eh], ecx
        mov    byte ptr [eax + 18ah], cl
        mov    byte ptr [eax + 18bh], cl
        mov    word ptr [eax + 195h], cx
        mov    byte ptr [eax + 18ch], cl
        pop    ebp
        mov    dword ptr [eax + 18dh], ecx
        mov    dword ptr [eax + 191h], ecx
        mov    dword ptr [eax + 182h], ecx
        ret    4
_PTEXT ends
        end
