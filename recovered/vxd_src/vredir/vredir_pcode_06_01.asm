; vredir_pcode_06_01.asm -- VREDIR object 6 segment _PTEXT  (part 1/4)
.386p
_PTEXT segment dword public use32 'PCODE'

        db  055h, 08Bh, 04Ch, 024h, 00Ch, 066h, 08Bh, 041h, 026h, 08Bh, 0ECh, 066h, 085h, 0C0h, 074h, 010h
        db  08Bh, 045h, 008h, 05Dh, 066h, 0C7h, 040h, 01Ah, 061h, 009h, 0B8h, 061h, 009h, 000h, 000h, 0C3h
        db  066h, 040h, 080h, 061h, 033h, 0FCh, 066h, 089h, 041h, 026h, 08Bh, 045h, 008h, 050h, 089h, 048h
        db  020h, 0E8h, 000h, 000h, 000h, 000h, 08Bh, 0E5h, 05Dh, 0C3h, 08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_6_0040:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
loc_6_0049:
        mov    ecx, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [ecx + 30h]
        test   ecx, ecx
        je     SHORT loc_6_0061
        push   ecx
        push   esi
        call   0
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_0049
loc_6_0061:
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_0070:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi]
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [esi + 3ch], edi
        mov    ecx, dword ptr [eax + 14h]
        cmp    byte ptr [ecx + 48h], 4
        jne    SHORT loc_6_0096
        mov    byte ptr [esi + 4], 1
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0093:
        add    esp, 4
loc_6_0096:
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [esi + 4ch], 0
        mov    eax, dword ptr [eax + 14h]
        cmp    byte ptr [eax + 48h], 4
        je     SHORT loc_6_00D2
        cmp    dword ptr [eax + 1ch], 0
        je     SHORT loc_6_00BC
        push   esi
        call   sub_6_0DA0
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_6_010A
loc_6_00BC:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_00D2
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_00CB:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_010A
loc_6_00D2:
        mov    eax, dword ptr [esi + 3ch]
        push   0a1h
        push   esi
        mov    dword ptr [esi], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_00E2:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_00F1
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_00F1:
        mov    eax, dword ptr [esi + 3ch]
        push   eax
        push   esi
        call   sub_6_01E0
        add    esp, 8
        mov    eax, dword ptr [esi + 50h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0107:
        add    esp, 4
loc_6_010A:
        movsx  eax, word ptr [esi + 1ah]
        mov    dword ptr [esi], edi
        sub    edi, dword ptr [esi + 3ch]
        mov    dword ptr [esi], edi
        db  001h, 03Dh, 000h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  090h
sub_6_0120:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 20h]
        mov    eax, dword ptr [edi]
        mov    ebx, dword ptr [esi + 14h]
        cmp    dword ptr [ebx + 3ch], eax
        jne    SHORT loc_6_017A
        mov    ecx, dword ptr [edi + 24h]
        cmp    dword ptr [ebx + 40h], ecx
        jne    SHORT loc_6_017A
        test   byte ptr [ebx + 2ah], 10h
        jne    SHORT loc_6_017A
        mov    al, byte ptr [ebx + 48h]
        inc    al
        mov    byte ptr [ebx + 48h], al
        cmp    al, 4
        je     SHORT loc_6_015D
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0158:
        cmp    eax, dword ptr [ebx + 44h]
        jbe    SHORT loc_6_0192
loc_6_015D:
        mov    byte ptr [ebx + 48h], 4
        mov    byte ptr [edi + 4], 1
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_016B:
        add    esp, 4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_017A:
        mov    ecx, dword ptr [edi + 24h]
        mov    dword ptr [ebx + 3ch], eax
        mov    byte ptr [ebx + 48h], 1
        mov    dword ptr [ebx + 40h], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_018C:
        add    eax, 2
        mov    dword ptr [ebx + 44h], eax
loc_6_0192:
        mov    cl, byte ptr [esi + 34h]
        test   cl, 80h
        je     SHORT loc_6_01A3
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_01A3:
        mov    edx, dword ptr [edi + 24h]
        mov    eax, dword ptr [esi + 39h]
        cmp    edx, eax
        jb     SHORT loc_6_01BE
        or     eax, 0fffh
        cmp    eax, edx
        jb     SHORT loc_6_01BE
        and    cl, 9fh
        mov    byte ptr [esi + 34h], cl
        jmp    SHORT loc_6_01CD
loc_6_01BE:
        test   cl, 20h
        je     SHORT loc_6_01C9
        or     cl, 40h
        mov    byte ptr [esi + 34h], cl
loc_6_01C9:
        or     byte ptr [esi + 34h], 20h
loc_6_01CD:
        mov    eax, dword ptr [edi]
        pop    edi
        add    eax, edx
        mov    dword ptr [esi + 39h], eax
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_6_01E0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 60h
        push   ebx
        mov    dword ptr [ebp - 8], edx
        push   esi
        mov    dword ptr [ebp - 20h], edx
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 20h]
        mov    edi, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 18h], ecx
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [ebp - 1ch], eax
        mov    dword ptr [ebp - 14h], edx
        mov    ax, word ptr [eax + 1ah]
        mov    dword ptr [esi + 54h], edx
        mov    cl, al
        and    cl, 1
        test   al, 80h
        mov    byte ptr [ebp - 1], cl
        jne    SHORT loc_6_0258
        mov    ecx, dword ptr [ebp - 18h]
        cmp    dword ptr [ecx + 28h], edx
        je     SHORT loc_6_022A
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0227:
        add    esp, 4
loc_6_022A:
        mov    eax, dword ptr [edi + 10h]
        test   byte ptr [eax + 1ah], 20h
        je     SHORT loc_6_0258
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ax, word ptr [edi + 20h]
        sub    eax, 34h
        mov    dword ptr [ebp - 24h], eax
        mov    al, byte ptr [ecx + 1dh]
        and    al, 8
        cmp    al, 1
        sbb    eax, eax
        and    eax, 7
        add    eax, 3
        imul   eax, dword ptr [ebp - 24h]
        mov    dword ptr [ebp - 14h], eax
loc_6_0258:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 3ch]
        mov    ax, word ptr [edi + 20h]
        mov    dword ptr [ebp - 0ch], ecx
        sub    eax, 64h
        mov    dword ptr [ebp - 54h], ecx
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 58h], ecx
loc_6_0270:
        mov    dword ptr [ebp - 5ch], 0
        mov    dword ptr [ebp - 60h], 0
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_6_028D
        mov    byte ptr [ebp - 1], 0
        mov    edi, dword ptr [ebp - 10h]
        jmp    SHORT loc_6_02D2
loc_6_028D:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0293:
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_6_02D2
        mov    byte ptr [ebp - 1], 0
        mov    edi, dword ptr [ebp - 10h]
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_02A9:
        add    esp, 4
        mov    dword ptr [ebp - 20h], eax
        test   eax, eax
        je     NEAR PTR loc_6_0514
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 20h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_02C4:
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     NEAR PTR loc_6_0514
loc_6_02D2:
        cmp    dword ptr [ebp - 58h], edi
        jae    SHORT loc_6_02DA
        mov    edi, dword ptr [ebp - 58h]
loc_6_02DA:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_6_0330
        cmp    edi, 33h
        jb     SHORT loc_6_0330
        mov    ecx, dword ptr [ebp - 18h]
        cmp    dword ptr [ecx + 28h], 0
        jne    SHORT loc_6_0330
        mov    eax, dword ptr [esi + 50h]
        mov    ecx, dword ptr [esi + 24h]
        push   eax
        mov    edx, dword ptr [ebp - 18h]
        push   edi
        xor    eax, eax
        mov    ax, word ptr [edx + 35h]
        push   edi
        push   ecx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   0a1ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0312:
        add    esp, 20h
        test   eax, eax
        jne    SHORT loc_6_0330
        mov    eax, dword ptr [esi + 34h]
        test   eax, eax
        je     SHORT loc_6_0330
        mov    dword ptr [ebp - 5ch], eax
        cmp    eax, edi
        je     NEAR PTR loc_6_0492
        jmp    NEAR PTR loc_6_048B
loc_6_0330:
        cmp    dword ptr [ebp - 10h], edi
        jae    NEAR PTR loc_6_0425
        mov    ecx, dword ptr [ebp - 18h]
        cmp    dword ptr [ecx + 28h], 0
        jne    NEAR PTR loc_6_041D
        cmp    dword ptr [ebp - 14h], 0
        je     NEAR PTR loc_6_041D
        cmp    dword ptr [ebp - 14h], edi
        jae    SHORT loc_6_0358
        mov    edi, dword ptr [ebp - 14h]
loc_6_0358:
        lea    eax, [ebp - 60h]
        mov    edx, dword ptr [esi + 50h]
        push   edx
        mov    dword ptr [esi + 48h], eax
        push   edi
        mov    eax, dword ptr [esi + 24h]
        push   edi
        mov    dword ptr [esi + 38h], edi
        push   edi
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [esi + 40h], 0
        mov    dword ptr [esi + 34h], 0
        push   eax
        xor    eax, eax
        mov    ax, word ptr [ecx + 35h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   481bh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0399:
        add    esp, 28h
        test   eax, eax
        je     SHORT loc_6_03BB
        cmp    word ptr [esi + 1ah], 0fbh
        jne    NEAR PTR loc_6_051A
        mov    dword ptr [ebp - 14h], 0
        mov    word ptr [esi + 1ah], 0
        jmp    SHORT loc_6_03C9
loc_6_03BB:
        mov    eax, dword ptr [esi + 38h]
        cmp    edi, eax
        je     SHORT loc_6_03C9
        or     dword ptr [esi + 54h], 1
        mov    dword ptr [ebp - 0ch], eax
loc_6_03C9:
        mov    eax, dword ptr [esi + 38h]
        mov    ecx, dword ptr [ebp - 24h]
        sub    eax, dword ptr [esi + 40h]
        add    ecx, ecx
        cmp    eax, ecx
        jae    SHORT loc_6_03E9
        mov    eax, dword ptr [ebp - 1ch]
        cmp    word ptr [eax + 8dh], 2
        jb     NEAR PTR loc_6_0492
loc_6_03E9:
        mov    eax, dword ptr [ebp - 1ch]
        mov    word ptr [eax + 8dh], 0
        mov    cl, byte ptr [eax + 1dh]
        test   cl, 8
        je     SHORT loc_6_0409
        mov    dword ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_6_0492
loc_6_0409:
        or     cl, 8
        mov    eax, dword ptr [ebp - 1ch]
        mov    edx, dword ptr [ebp - 24h]
        mov    byte ptr [eax + 1dh], cl
        lea    ecx, [edx + edx*2]
        mov    dword ptr [ebp - 14h], ecx
        jmp    SHORT loc_6_0492
loc_6_041D:
        cmp    dword ptr [ebp - 10h], edi
        jae    SHORT loc_6_0425
        mov    edi, dword ptr [ebp - 10h]
loc_6_0425:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_6_0430
        mov    eax, dword ptr [ebp - 8]
        jmp    SHORT loc_6_0433
loc_6_0430:
        mov    eax, dword ptr [esi + 50h]
loc_6_0433:
        push   eax
        mov    edx, dword ptr [esi + 24h]
        push   edi
        mov    eax, dword ptr [ebp - 58h]
        push   eax
        mov    ecx, dword ptr [ebp - 18h]
        push   edx
        xor    eax, eax
        mov    ax, word ptr [ecx + 35h]
        push   edi
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   480ah
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0458:
        add    esp, 24h
        test   eax, eax
        jne    NEAR PTR loc_6_051A
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_6_0481
        mov    eax, dword ptr [esi + 34h]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        mov    edx, dword ptr [esi + 50h]
        push   0
        push   ecx
        push   0
        push   edx
        call   sub_6_D640
        add    esp, 14h
loc_6_0481:
        mov    eax, dword ptr [esi + 34h]
        mov    dword ptr [ebp - 5ch], eax
        cmp    eax, edi
        je     SHORT loc_6_0492
loc_6_048B:
        or     dword ptr [esi + 54h], 1
        mov    dword ptr [ebp - 0ch], eax
loc_6_0492:
        lea    ebx, [ebp - 60h]
        xor    edi, edi
        cmp    dword ptr [ebp - 60h], edi
        jne    SHORT loc_6_04AA
loc_6_049C:
        mov    edi, dword ptr [ebx + 4]
        cmp    dword ptr [ebx], edi
        je     SHORT loc_6_04AA
        add    ebx, 8
        cmp    dword ptr [ebx], edi
        je     SHORT loc_6_049C
loc_6_04AA:
        mov    eax, dword ptr [esi + 3ch]
        add    dword ptr [esi + 24h], edi
        sub    eax, edi
        mov    dword ptr [esi + 3ch], eax
        je     SHORT loc_6_051A
        push   edi
        mov    eax, dword ptr [esi + 50h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_04C1:
        add    esp, 8
        cmp    dword ptr [ebp - 0ch], edi
        jbe    SHORT loc_6_051A
        cmp    word ptr [esi + 1ah], 0
        jne    SHORT loc_6_051A
        test   edi, edi
        je     NEAR PTR loc_6_0270
        lea    ecx, [ebp - 58h]
        mov    eax, dword ptr [ebx]
        sub    eax, edi
        sub    dword ptr [ebp - 0ch], edi
        mov    dword ptr [ebp - 58h], eax
        mov    eax, dword ptr [ebx + 4]
        sub    eax, edi
        mov    dword ptr [ebp - 54h], eax
        cmp    eax, dword ptr [ebp - 58h]
        je     NEAR PTR loc_6_0270
loc_6_04F6:
        mov    edx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebx + 0ch]
        add    ebx, 8
        add    ecx, 8
        sub    edx, edi
        sub    eax, edi
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], eax
        cmp    eax, edx
        jne    SHORT loc_6_04F6
        jmp    NEAR PTR loc_6_0270
loc_6_0514:
        mov    word ptr [esi + 1ah], 8
loc_6_051A:
        cmp    dword ptr [ebp - 20h], 0
        je     SHORT loc_6_052C
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0529:
        add    esp, 4
loc_6_052C:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_6_053E
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_053B:
        add    esp, 4
loc_6_053E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_0550:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ecx + 1ch]
        mov    edx, dword ptr [ecx + 34h]
        mov    eax, dword ptr [esi + 34h]
        add    dword ptr [esi + 40h], edx
        add    eax, edx
        mov    dword ptr [esi + 34h], eax
        cmp    dword ptr [esi + 38h], eax
        jbe    SHORT loc_6_057D
        cmp    dword ptr [ecx + 20h], 0
        jne    SHORT loc_6_057D
        test   edx, edx
        je     SHORT loc_6_057D
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_057D:
        mov    eax, 1
        pop    esi
        and    word ptr [ecx + 2], 0ffefh
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_6_0590:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        mov    dword ptr [ebp - 1ch], eax
        push   esi
        mov    dword ptr [ebp - 20h], eax
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], 800h
        mov    edi, dword ptr [esi + 20h]
        mov    ebx, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 18h], ecx
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebx + 10h]
        mov    ax, word ptr [eax + 1ah]
        test   al, 80h
        jne    SHORT loc_6_05E2
        test   al, 20h
        je     SHORT loc_6_05ED
        mov    dword ptr [ebp - 1ch], 80h
        mov    dword ptr [ebp - 8], 1eh
        mov    dword ptr [ebp - 20h], 1
        jmp    SHORT loc_6_05ED
loc_6_05E2:
        test   al, 2
        je     SHORT loc_6_05ED
        mov    dword ptr [ebp - 8], 21dh
loc_6_05ED:
        or     byte ptr [edi + 33h], 8
        mov    dword ptr [esi + 4ch], 0
        cmp    dword ptr [edi + 28h], 0
        mov    dword ptr [esi + 3ch], ecx
        mov    ecx, dword ptr [edi + 14h]
        mov    dword ptr [ebp - 4], ecx
        je     SHORT loc_6_0610
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_060D:
        add    esp, 4
loc_6_0610:
        cmp    dword ptr [esi], 0
        jne    SHORT loc_6_0631
        mov    eax, dword ptr [esi + 24h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [esi + 4], 1
        push   esi
        mov    dword ptr [ecx + 30h], eax
        or     byte ptr [ecx + 2ah], 2
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_062C:
        add    esp, 4
        jmp    SHORT loc_6_0668
loc_6_0631:
        mov    eax, dword ptr [esi + 20h]
        mov    ecx, dword ptr [eax + 14h]
        cmp    dword ptr [ecx + 1ch], 0
        je     SHORT loc_6_064E
        push   esi
        call   sub_6_0DA0
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_6_0779
loc_6_064E:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_0668
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_065D:
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_6_0779
loc_6_0668:
        mov    eax, dword ptr [esi + 3ch]
        push   0a0h
        push   esi
        mov    dword ptr [esi], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0678:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_068A
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_068A:
        mov    eax, dword ptr [esi + 24h]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 30h]
        cmp    eax, ecx
        jbe    SHORT loc_6_06BC
        sub    eax, ecx
        cmp    eax, 0fa000h
        jbe    SHORT loc_6_06B5
        mov    ecx, 0fa000h
        sub    edx, edx
        div    ecx
        shl    eax, 2
        lea    edx, [eax + eax*4]
        lea    ecx, [edx + edx*4]
        mov    dword ptr [ebp - 10h], ecx
loc_6_06B5:
        mov    dword ptr [ebp - 0ch], 2800h
loc_6_06BC:
        xor    eax, eax
        mov    ax, word ptr [ebx + 22h]
        mov    ebx, dword ptr [ebp - 4]
        sub    eax, 30h
        mov    dword ptr [ebp - 14h], eax
loc_6_06CB:
        test   byte ptr [esi + 19h], 10h
        je     SHORT loc_6_06D8
        mov    eax, dword ptr [esi + 50h]
        mov    ebx, dword ptr [eax]
        jmp    SHORT loc_6_06F2
loc_6_06D8:
        cmp    dword ptr [esi + 3ch], 0
        je     SHORT loc_6_06F2
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_06E4:
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_6_06F2
        mov    ebx, 100h
loc_6_06F2:
        mov    ecx, dword ptr [esi + 3ch]
        cmp    ebx, ecx
        jbe    SHORT loc_6_06FB
        mov    ebx, ecx
loc_6_06FB:
        mov    dword ptr [esi + 38h], 0
        mov    dword ptr [esi + 44h], 0
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_6_07F5
        cmp    ebx, dword ptr [ebp - 14h]
        jbe    NEAR PTR loc_6_07FD
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [esi + 50h]
        push   eax
        mov    edx, dword ptr [ebp - 20h]
        push   ecx
        mov    eax, dword ptr [ebp - 1ch]
        push   ebx
        mov    ecx, dword ptr [esi + 24h]
        push   edx
        mov    dword ptr [esi + 34h], ebx
        push   eax
        push   ecx
        push   ebx
        xor    eax, eax
        mov    ax, word ptr [edi + 35h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        or     eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_074E:
        add    esp, 2ch
        test   eax, eax
        je     NEAR PTR loc_6_086B
        movsx  eax, word ptr [esi + 1ah]
        cmp    eax, 47h
        jg     SHORT loc_6_07C4
        je     NEAR PTR loc_6_07ED
        cmp    eax, -3
        je     SHORT loc_6_07D4
loc_6_076D:
        mov    eax, dword ptr [esi + 50h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0776:
        add    esp, 4
loc_6_0779:
        mov    ax, word ptr [esi + 1ah]
        cmp    ax, 27h
        je     NEAR PTR loc_6_089C
        cmp    ax, 70h
        je     NEAR PTR loc_6_089C
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, eax
        mov    dword ptr [esi], eax
        sub    ecx, dword ptr [esi + 3ch]
        mov    eax, dword ptr [esi + 24h]
        mov    dword ptr [esi], ecx
        db  001h, 00Dh, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [ecx + 30h], eax
        jae    NEAR PTR loc_6_08A8
        pop    edi
        mov    dword ptr [ecx + 30h], eax
        or     byte ptr [ecx + 2ah], 2
        movsx  eax, word ptr [esi + 1ah]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_07C4:
        cmp    eax, 0fah
        jl     SHORT loc_6_076D
        cmp    eax, 0fch
        jle    SHORT loc_6_07DB
        jmp    SHORT loc_6_076D
loc_6_07D4:
        mov    dword ptr [esi + 34h], 0
loc_6_07DB:
        mov    dword ptr [ebp - 8], 0
loc_6_07E2:
        mov    word ptr [esi + 1ah], 0
        mov    ebx, dword ptr [esi + 34h]
        jmp    SHORT loc_6_086B
loc_6_07ED:
        cmp    dword ptr [esi + 34h], 0
        jne    SHORT loc_6_07E2
        jmp    SHORT loc_6_07DB
loc_6_07F5:
        cmp    ebx, dword ptr [ebp - 14h]
        jbe    SHORT loc_6_07FD
        mov    ebx, dword ptr [ebp - 14h]
loc_6_07FD:
        test   byte ptr [edi + 34h], 8
        je     SHORT loc_6_0836
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [esi + 50h]
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [edi + 35h]
        push   edx
        push   ebx
        mov    dword ptr [esi + 34h], ebx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        or     eax, 8c1h
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_082A:
        add    esp, 1ch
        test   eax, eax
        je     SHORT loc_6_086B
        jmp    NEAR PTR loc_6_076D
loc_6_0836:
        mov    edx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [esi + 50h]
        push   edx
        push   eax
        push   ebx
        xor    eax, eax
        mov    ax, word ptr [edi + 35h]
        push   ecx
        mov    ecx, dword ptr [esi + 24h]
        push   ecx
        push   ebx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 0ch]
        or     eax, 480bh
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0860:
        add    esp, 28h
        test   eax, eax
        jne    NEAR PTR loc_6_076D
loc_6_086B:
        mov    eax, dword ptr [esi + 34h]
        mov    ecx, dword ptr [esi + 3ch]
        add    dword ptr [esi + 24h], eax
        sub    ecx, eax
        mov    dword ptr [esi + 3ch], ecx
        je     NEAR PTR loc_6_076D
        cmp    ebx, eax
        jne    NEAR PTR loc_6_076D
        sub    eax, dword ptr [esi + 38h]
        mov    ecx, dword ptr [esi + 50h]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0894:
        add    esp, 8
        jmp    NEAR PTR loc_6_06CB
loc_6_089C:
        mov    word ptr [esi + 1ah], 27h
        mov    dword ptr [esi], 0
loc_6_08A8:
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_08C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   0a1h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_08D2:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_08DE
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_08DE:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_08E4:
        add    esp, 4
        mov    eax, dword ptr [esi + 3ch]
        push   eax
        push   esi
        call   sub_6_01E0
        add    esp, 8
        mov    eax, dword ptr [esi + 50h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_08FD:
        add    esp, 4
        cmp    word ptr [esi + 1ah], 0
        je     SHORT loc_6_090C
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_090C:
        mov    eax, dword ptr [esi]
        sub    eax, dword ptr [esi + 3ch]
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_0920:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0932:
        add    esp, 4
        cmp    dword ptr [esi], 800h
        jae    SHORT loc_6_095E
        mov    eax, dword ptr [esi + 20h]
        test   byte ptr [eax + 34h], 40h
        je     SHORT loc_6_095E
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_094F:
        add    esp, 4
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_095E:
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], 1000h
        mov    ecx, dword ptr [edi + 8]
        mov    eax, dword ptr [edi + 1ch]
        sub    ebx, ecx
        cmp    eax, ebx
        jbe    SHORT loc_6_098A
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [edi + 1ch], ebx
        cmp    word ptr [edi + 20h], 0
        jne    SHORT loc_6_099F
        mov    word ptr [edi + 20h], ax
        jmp    SHORT loc_6_099F
loc_6_098A:
        lea    eax, [edi + 20h]
        mov    ax, word ptr [edi + 20h]
        test   ax, ax
        je     SHORT loc_6_0999
        movzx  ebx, ax
loc_6_0999:
        mov    word ptr [edi + 20h], 1000h
loc_6_099F:
        add    ecx, ebx
        mov    eax, dword ptr [edi + 10h]
        push   eax
        lea    edx, [ebp - 24h]
        push   edx
        mov    dword ptr [esi + 24h], ecx
        call   sub_6_D620
        add    esp, 8
        lea    ecx, [ebp - 24h]
        mov    dword ptr [esi + 50h], ecx
        test   ebx, ebx
        je     SHORT loc_6_09C8
        push   ebx
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_09C5:
        add    esp, 8
loc_6_09C8:
        mov    eax, dword ptr [ebp - 4]
        sub    eax, ebx
        push   eax
        mov    dword ptr [esi + 3ch], eax
        push   esi
        call   sub_6_01E0
        add    esp, 8
        lea    ecx, [ebp - 24h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_09E3:
        mov    ax, word ptr [esi + 1ah]
        add    esp, 4
        test   ax, ax
        jne    SHORT loc_6_0A2C
        cmp    dword ptr [esi + 3ch], 0
        je     SHORT loc_6_0A00
        mov    ecx, dword ptr [esi + 24h]
        sub    cx, word ptr [edi + 8]
        mov    word ptr [edi + 20h], cx
loc_6_0A00:
        test   ax, ax
        jne    SHORT loc_6_0A2C
        mov    eax, dword ptr [edi + 8]
        cmp    dword ptr [esi + 24h], eax
        je     SHORT loc_6_0A2C
        cmp    byte ptr [edi + 22h], 0
        jne    SHORT loc_6_0A2C
        test   byte ptr [esi + 54h], 1
        je     SHORT loc_6_0A23
        mov    eax, dword ptr [ebp - 4]
        sub    eax, dword ptr [esi + 3ch]
        cmp    eax, ebx
        je     SHORT loc_6_0A2C
loc_6_0A23:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_0A2C:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0A32:
        add    esp, 4
        cmp    word ptr [esi + 1ah], 21h
        jne    SHORT loc_6_0A5C
        mov    ecx, dword ptr [esi + 1ch]
        mov    ax, word ptr [ecx + 1ch]
        add    ecx, 1ch
        test   al, 40h
        jne    SHORT loc_6_0A5C
        or     al, 40h
        push   esi
        mov    byte ptr [esi + 4], 0
        mov    word ptr [ecx], ax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0A59:
        add    esp, 4
loc_6_0A5C:
        mov    eax, 1
        pop    edi
        mov    word ptr [esi + 1ah], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_6_0A70:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 10h]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ecx + 14h]
        mov    edx, dword ptr [eax + 8]
        mov    eax, dword ptr [ecx + 2ch]
        mov    dword ptr [ebp - 14h], edx
        mov    dword ptr [edi + 1ch], edx
        mov    dl, byte ptr [ecx + 30h]
        mov    dword ptr [edi + 8], eax
        xor    eax, eax
        mov    byte ptr [edi + 5], dl
        mov    ax, word ptr [ecx + 35h]
        lea    edx, [esi + 14h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 0ch], eax
        xor    eax, eax
        mov    dword ptr [edi + 50h], edx
        mov    edx, dword ptr [ecx + 10h]
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 1ch], eax
        mov    ax, word ptr [edx + 1ah]
        test   al, 80h
        jne    SHORT loc_6_0AE3
        test   al, 20h
        je     SHORT loc_6_0AEE
        mov    dword ptr [ebp - 18h], 80h
        mov    dword ptr [ebp - 8], 0a1eh
        mov    dword ptr [ebp - 1ch], 1
        jmp    SHORT loc_6_0AEE
loc_6_0AE3:
        test   al, 2
        je     SHORT loc_6_0AEE
        mov    dword ptr [ebp - 8], 0a1dh
loc_6_0AEE:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0AF4:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_6_0B00
        mov    eax, 100h
loc_6_0B00:
        mov    ebx, dword ptr [esi + 0ch]
        cmp    eax, ebx
        jae    SHORT loc_6_0B09
        mov    ebx, eax
loc_6_0B09:
        xor    ecx, ecx
        mov    eax, dword ptr [ebp - 14h]
        mov    cx, word ptr [eax + 22h]
        mov    dword ptr [edi + 34h], ebx
        sub    ecx, 30h
        xor    eax, eax
        mov    dword ptr [ebp - 10h], ecx
        mov    dword ptr [edi + 38h], eax
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [edi + 44h], eax
        mov    dword ptr [edi + 24h], ecx
        cmp    dword ptr [ebp - 10h], ebx
        jae    SHORT loc_6_0B70
        test   byte ptr [esi + 31h], 1
        jne    SHORT loc_6_0B68
        cmp    dword ptr [ebp - 8], eax
        je     SHORT loc_6_0B68
        lea    eax, [esi + 14h]
        mov    edx, dword ptr [ebp - 1ch]
        push   eax
        push   ebx
        push   edx
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        push   ecx
        push   ebx
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0B60:
        add    esp, 2ch
        cmp    eax, -3
        jne    SHORT loc_6_0BC6
loc_6_0B68:
        cmp    dword ptr [ebp - 10h], ebx
        jae    SHORT loc_6_0B70
        mov    ebx, dword ptr [ebp - 10h]
loc_6_0B70:
        mov    ecx, dword ptr [ebp - 4]
        test   byte ptr [ecx + 34h], 8
        je     SHORT loc_6_0B9F
        add    esi, 14h
        mov    eax, dword ptr [ebp - 0ch]
        push   esi
        mov    dword ptr [edi + 34h], ebx
        push   ebx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   8c1h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0B9A:
        add    esp, 1ch
        jmp    SHORT loc_6_0BC6
loc_6_0B9F:
        lea    eax, [esi + 14h]
        mov    ecx, dword ptr [esi + 8]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ebx
        push   ebx
        push   ecx
        push   ebx
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   480bh
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0BC3:
        add    esp, 28h
loc_6_0BC6:
        test   eax, eax
        je     SHORT loc_6_0BE4
        push   edi
        jge    SHORT loc_6_0BDC
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0BD2:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_0BDC:
        call   sub_6_0BF0
        add    esp, 4
loc_6_0BE4:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_6_0BF0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ecx + 34h]
        mov    edi, dword ptr [ecx + 30h]
        mov    edx, dword ptr [ecx + 38h]
        mov    esi, dword ptr [edi + 10h]
        push   ecx
        mov    dword ptr [ebp - 8], edx
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp - 4], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0C18:
        add    esp, 4
        cmp    eax, 70h
        je     SHORT loc_6_0C24
        test   ebx, ebx
        jne    SHORT loc_6_0C29
loc_6_0C24:
        mov    eax, 27h
loc_6_0C29:
        test   eax, eax
        je     SHORT loc_6_0CA9
        cmp    eax, 47h
        je     SHORT loc_6_0CA1
        cmp    eax, 0fah
        jl     SHORT loc_6_0C40
        cmp    eax, 0fch
        jle    SHORT loc_6_0CA5
loc_6_0C40:
        mov    ecx, dword ptr [esi]
        mov    word ptr [ecx + 37h], ax
        mov    edx, dword ptr [ecx + 14h]
        and    byte ptr [edx + 2ah], 0c7h
loc_6_0C4D:
        lea    eax, [esi + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0C56:
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0C62:
        add    esp, 4
        mov    eax, dword ptr [esi + 10h]
        test   eax, eax
        je     SHORT loc_6_0C81
loc_6_0C6C:
        mov    ecx, dword ptr [eax]
        push   eax
        mov    dword ptr [esi + 10h], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0C77:
        add    esp, 4
        mov    eax, dword ptr [esi + 10h]
        test   eax, eax
        jne    SHORT loc_6_0C6C
loc_6_0C81:
        cmp    byte ptr [esi + 30h], 0
        mov    dword ptr [esi], 0
        je     SHORT loc_6_0C9A
        mov    byte ptr [esi + 30h], 0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0C97:
        add    esp, 4
loc_6_0C9A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_0CA1:
        test   ebx, ebx
        jne    SHORT loc_6_0CA9
loc_6_0CA5:
        or     byte ptr [esi + 31h], 1
loc_6_0CA9:
        sub    dword ptr [esi + 0ch], ebx
        je     SHORT loc_6_0C4D
        add    dword ptr [esi + 8], ebx
        add    esi, 14h
        sub    ebx, dword ptr [ebp - 8]
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0CBE:
        add    esp, 8
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0CC7:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_0CE0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 4], 0
        je     NEAR PTR loc_6_0D95
        mov    edi, dword ptr [esi + 20h]
        mov    eax, dword ptr [edi + 14h]
        test   byte ptr [eax + 2ah], 10h
        jne    NEAR PTR loc_6_0D8C
        test   byte ptr [edi + 31h], 20h
        je     SHORT loc_6_0D11
        mov    ecx, dword ptr [eax + 34h]
        cmp    dword ptr [eax + 30h], ecx
        je     SHORT loc_6_0D8C
loc_6_0D11:
        mov    ebx, dword ptr [esi + 1ch]
        mov    eax, dword ptr [ebx + 55h]
        cmp    dword ptr [ebx + 59h], eax
        jne    SHORT loc_6_0D26
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0D21:
        cmp    eax, dword ptr [ebx + 5dh]
        jbe    SHORT loc_6_0D8C
loc_6_0D26:
        mov    byte ptr [esi + 4], 0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0D30:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_6_0D95
        test   byte ptr [edi + 31h], 20h
        jne    SHORT loc_6_0D5F
        mov    eax, dword ptr [esi + 24h]
        push   eax
        push   2
        xor    eax, eax
        mov    ax, word ptr [edi + 35h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   4812h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0D5A:
        add    esp, 18h
        jmp    SHORT loc_6_0D68
loc_6_0D5F:
        mov    eax, dword ptr [edi + 14h]
        mov    ecx, dword ptr [eax + 30h]
        add    dword ptr [esi + 24h], ecx
loc_6_0D68:
        mov    ebx, dword ptr [esi + 1ch]
        mov    eax, dword ptr [ebx + 55h]
        mov    dword ptr [ebx + 59h], eax
        mov    eax, dword ptr [edi + 14h]
        mov    ecx, dword ptr [eax + 30h]
        mov    dword ptr [eax + 34h], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0D7F:
        inc    eax
        pop    edi
        mov    dword ptr [ebx + 5dh], eax
        movsx  eax, word ptr [esi + 1ah]
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_0D8C:
        mov    eax, dword ptr [edi + 14h]
        mov    ecx, dword ptr [eax + 30h]
        add    dword ptr [esi + 24h], ecx
loc_6_0D95:
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Bh, 0FFh
sub_6_0DA0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        mov    edx, dword ptr [ecx + 24h]
        mov    eax, dword ptr [ecx + 3ch]
        mov    edi, dword ptr [ecx + 20h]
        lea    esi, [eax + edx - 1]
        mov    eax, dword ptr [edi + 14h]
        mov    edi, dword ptr [eax + 1ch]
        test   edi, edi
        je     SHORT loc_6_0DEB
loc_6_0DC0:
        cmp    dword ptr [edi + 0ch], edx
        jb     SHORT loc_6_0DE5
        cmp    dword ptr [edi + 8], esi
        ja     SHORT loc_6_0DEB
        cmp    dword ptr [ecx + 4ch], 0
        jne    SHORT loc_6_0DD3
        mov    dword ptr [ecx + 4ch], edi
loc_6_0DD3:
        mov    eax, dword ptr [edi + 10h]
        cmp    dword ptr [ecx + 8], eax
        jne    SHORT loc_6_0DF1
        mov    ax, word ptr [ecx + 6]
        cmp    word ptr [edi + 14h], ax
        jne    SHORT loc_6_0DF1
loc_6_0DE5:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_6_0DC0
loc_6_0DEB:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_0DF1:
        mov    eax, 21h
        pop    edi
        mov    word ptr [ecx + 1ah], 21h
        pop    esi
        pop    ebp
        ret
loc_6_0E00:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        jne    SHORT loc_6_0E20
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        pop    ebp
        ret
loc_6_0E20:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_6_0E84
loc_6_0E2C:
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [eax + 0ch]
        test   edi, edi
        je     SHORT loc_6_0E73
loc_6_0E36:
        mov    ebx, dword ptr [edi + 30h]
        test   ebx, ebx
        je     SHORT loc_6_0E68
        mov    esi, ebx
loc_6_0E3F:
        mov    ecx, dword ptr [esi + 28h]
        test   ecx, ecx
        je     SHORT loc_6_0E61
        mov    al, byte ptr [ecx + 16h]
        test   al, 40h
        je     SHORT loc_6_0E5C
        or     byte ptr [esi + 34h], 10h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0E57:
        add    esp, 4
        jmp    SHORT loc_6_0E61
loc_6_0E5C:
        or     al, 40h
        mov    byte ptr [ecx + 16h], al
loc_6_0E61:
        mov    esi, dword ptr [esi + 8]
        cmp    ebx, esi
        jne    SHORT loc_6_0E3F
loc_6_0E68:
        mov    edi, dword ptr [edi + 4]
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_6_0E36
loc_6_0E73:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 4], ecx
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_0E2C
loc_6_0E84:
        push   64h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0E8F:
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_0EA0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 20h]
        test   eax, eax
        jne    SHORT loc_6_0ED7
        mov    ecx, dword ptr [esi + 10h]
        or     byte ptr [esi + 2], 1
        mov    eax, dword ptr [ecx + 76h]
        test   eax, eax
        je     SHORT loc_6_0ED2
        push   0
        mov    edx, dword ptr [esi + 38h]
        sub    edx, esi
        sub    edx, 3ch
        push   edx
        push   ecx
        push   esi
        call   dword ptr [eax + 30h]
        add    esp, 10h
        pop    esi
        pop    ebp
        ret
loc_6_0ED2:
        mov    eax, 37h
loc_6_0ED7:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_6_0EE0:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        cmp    eax, 0ffh
        jg     SHORT loc_6_0F06
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx], al
        je     SHORT loc_6_0F01
loc_6_0EF5:
        cmp    byte ptr [ecx], 0
        je     SHORT loc_6_0F01
        add    ecx, 2
        cmp    byte ptr [ecx], al
        jne    SHORT loc_6_0EF5
loc_6_0F01:
        xor    eax, eax
        mov    al, byte ptr [ecx + 1]
loc_6_0F06:
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_6_0F10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebx + 1ch]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebp - 4], ecx
        test   eax, 80000000h
        je     SHORT loc_6_0F37
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_0F37:
        cmp    eax, 8
        jne    SHORT loc_6_0F4E
        mov    eax, 5
        pop    edi
        mov    word ptr [ebx + 1ah], 5
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_0F4E:
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0F54:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_0F66
        movsx  eax, word ptr [ebx + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_0F66:
        mov    edx, dword ptr [ebx + 1ch]
        mov    eax, dword ptr [edx + 10h]
        test   word ptr [eax + 1ah], 108h
        je     SHORT loc_6_0F9F
        test   byte ptr [ebx + 3], 10h
        je     SHORT loc_6_0F9F
        mov    eax, dword ptr [ebx + 0ch]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2]
        mov    esi, dword ptr [ebx + 28h]
        lea    edi, [ecx + eax]
        xor    eax, eax
        mov    ax, word ptr [edi]
        add    edi, 2
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_6_0F9F:
        mov    ecx, dword ptr [ebp - 4]
        inc    dword ptr [edx + 55h]
        test   byte ptr [ecx + 1ch], 80h
        jne    SHORT loc_6_100F
        cmp    dword ptr [ecx + 72h], 0
        je     SHORT loc_6_100F
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_0FB6:
        mov    ecx, dword ptr [ebp - 4]
        cmp    eax, dword ptr [ecx + 76h]
        ja     SHORT loc_6_100F
        mov    eax, dword ptr [ecx + 55h]
        cmp    dword ptr [ecx + 65h], eax
        jne    SHORT loc_6_100F
        mov    eax, dword ptr [ecx + 72h]
        mov    esi, dword ptr [ebx + 0ch]
        add    esi, 4
        xor    edx, edx
        mov    dx, word ptr [eax]
        lea    edi, [eax + 4]
        sub    edx, 4
        mov    ecx, edx
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_6_100F
        mov    ecx, edx
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_6_100F
        mov    cx, word ptr [eax]
        mov    eax, dword ptr [ebx + 0ch]
        cmp    word ptr [eax], cx
        jne    SHORT loc_6_100F
        mov    eax, dword ptr [ebx + 1ch]
        pop    edi
        mov    word ptr [ebx + 1ah], 2
        pop    esi
        dec    dword ptr [eax + 55h]
        mov    eax, 2
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_100F:
        mov    eax, dword ptr [ebx + 1ch]
        mov    eax, dword ptr [eax + 72h]
        test   eax, eax
        je     SHORT loc_6_102C
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_101F:
        add    esp, 4
        mov    eax, dword ptr [ebx + 1ch]
        mov    dword ptr [eax + 72h], 0
loc_6_102C:
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [ebx]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   6
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1043:
        add    esp, 14h
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_6_1082
        mov    byte ptr [ebx + 4], 0
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1056:
        add    esp, 4
        mov    eax, dword ptr [ebx + 0ch]
        mov    word ptr [ebx + 1ah], 0
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1068:
        add    esp, 4
        mov    ecx, dword ptr [ebx + 1ch]
        mov    edi, ecx
        mov    dword ptr [ecx + 72h], eax
        mov    eax, dword ptr [edi + 55h]
        mov    dword ptr [edi + 65h], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_107E:
        inc    eax
        mov    dword ptr [edi + 76h], eax
loc_6_1082:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
loc_6_1090:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    dword ptr [ebx], 8
        jne    SHORT loc_6_10AE
        mov    eax, 5
        pop    edi
        mov    word ptr [ebx + 1ah], 5
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_10AE:
        mov    eax, dword ptr [ebx + 1ch]
        push   ebx
        inc    dword ptr [eax + 55h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_10BA:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_10CA
        movsx  eax, word ptr [ebx + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_10CA:
        mov    eax, dword ptr [ebx + 1ch]
        mov    ecx, dword ptr [eax + 10h]
        test   word ptr [ecx + 1ah], 108h
        je     SHORT loc_6_1103
        test   byte ptr [ebx + 3], 10h
        je     SHORT loc_6_1103
        mov    eax, dword ptr [ebx + 10h]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2]
        mov    esi, dword ptr [ebx + 28h]
        lea    edi, [ecx + eax]
        xor    eax, eax
        mov    ax, word ptr [edi]
        add    edi, 2
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_6_1103:
        mov    eax, dword ptr [ebx + 10h]
        mov    ecx, dword ptr [ebx + 0ch]
        push   eax
        push   ecx
        xor    eax, eax
        mov    ax, word ptr [ebx]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   7
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_111E:
        add    esp, 18h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_1130:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 1ch]
        test   byte ptr [edi + 1ch], 80h
        jne    SHORT loc_6_117F
        cmp    byte ptr [edi + 14h], 1
        jne    SHORT loc_6_117F
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_114C:
        cmp    eax, dword ptr [edi + 45h]
        ja     SHORT loc_6_117F
        mov    cx, word ptr [edi + 4bh]
        xor    eax, eax
        mov    ax, word ptr [edi + 49h]
        mov    word ptr [esi + 18h], cx
        mov    dword ptr [esi + 24h], eax
        mov    word ptr [esi + 1ah], 0
        xor    eax, eax
        mov    ax, word ptr [edi + 4dh]
        mov    dword ptr [esi], eax
        xor    eax, eax
        mov    ax, word ptr [edi + 4fh]
        pop    edi
        mov    dword ptr [esi + 28h], eax
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_117F:
        mov    dword ptr [edi + 45h], 0
        db  068h, 000h, 000h, 000h, 000h
        push   4080h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1196:
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_6_11A0:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_6_11B7
        xor    ecx, ecx
loc_6_11AB:
        cmp    byte ptr [eax + 2], cl
        je     SHORT loc_6_11BC
        mov    eax, dword ptr [eax + 18h]
        test   eax, eax
        jne    SHORT loc_6_11AB
loc_6_11B7:
        mov    ax, 100h
        ret
loc_6_11BC:
        movzx  ax, byte ptr [eax + 63h]
        or     ah, 1
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_11D0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [eax + 10h]
        test   byte ptr [ecx + 1ah], 20h
        jne    SHORT loc_6_11F3
        mov    eax, 32h
        pop    edi
        mov    word ptr [esi + 1ah], 32h
        pop    esi
        pop    ebp
        ret
loc_6_11F3:
        push   41h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_11FB:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_120A
        movsx  eax, word ptr [esi + 1ah]
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_120A:
        mov    eax, dword ptr [esi + 50h]
        mov    ecx, dword ptr [esi]
        push   eax
        push   ecx
        push   ecx
        mov    eax, dword ptr [esi + 20h]
        push   60h
        xor    ecx, ecx
        mov    cx, word ptr [eax + 35h]
        push   53h
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        push   4827h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1230:
        add    esp, 24h
        mov    edi, eax
        mov    ecx, dword ptr [esi + 50h]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_123E:
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_1250:
        push   esi
        push   edi
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        test   edi, edi
        je     SHORT loc_6_127F
loc_6_125C:
        mov    esi, dword ptr [edi + 0ch]
        test   esi, esi
        je     SHORT loc_6_1274
loc_6_1263:
        push   esi
        call   sub_6_12D0
        add    esp, 4
        mov    esi, dword ptr [esi + 4]
        cmp    dword ptr [edi + 0ch], esi
        jne    SHORT loc_6_1263
loc_6_1274:
        mov    edi, dword ptr [edi + 4]
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_125C
loc_6_127F:
        push   36eh
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1289:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_6_1295
        xor    eax, eax
        pop    edi
        pop    esi
        ret
loc_6_1295:
        mov    word ptr [eax], 6953h
        lea    ecx, [eax + 12h]
        pop    edi
        mov    dword ptr [eax + 0eh], ecx
        mov    byte ptr [eax + 3], 0
        mov    byte ptr [eax + 2], 1
        pop    esi
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_6_12B0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax], 6973h
        mov    ebp, esp
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_12C2:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_12D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0ch]
        test   eax, eax
        je     SHORT loc_6_12F9
        xor    ebx, ebx
loc_6_12E2:
        cmp    byte ptr [eax + 2], bl
        jne    SHORT loc_6_130A
        mov    edi, dword ptr [eax + 6]
        push   eax
        call   sub_6_12B0
        add    esp, 4
        mov    eax, edi
        test   edi, edi
        jne    SHORT loc_6_12E2
loc_6_12F9:
        mov    eax, 1
        pop    edi
        mov    dword ptr [esi + 0ch], 0
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_130A:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_1320:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_132E:
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [edx + 0ch]
        add    edx, 0ch
        test   eax, eax
        je     NEAR PTR loc_6_1424
loc_6_1342:
        mov    esi, dword ptr [eax + 0eh]
        lea    edi, [ebp - 10h]
        mov    ecx, dword ptr [esi + 1]
        inc    esi
        mov    dword ptr [edi], ecx
        mov    ebx, dword ptr [esi + 4]
        mov    dword ptr [edi + 4], ebx
        mov    cx, word ptr [esi + 8]
        mov    word ptr [edi + 8], cx
        mov    ecx, 0bh
        mov    bl, byte ptr [esi + 0ah]
        mov    byte ptr [edi + 0ah], bl
loc_6_1367:
        mov    bl, byte ptr [esi]
        cmp    bl, 3eh
        je     SHORT loc_6_1373
        cmp    bl, 22h
        jne    SHORT loc_6_1376
loc_6_1373:
        mov    byte ptr [esi], 3fh
loc_6_1376:
        inc    esi
        dec    ecx
        jne    SHORT loc_6_1367
        mov    ebx, dword ptr [eax + 0eh]
        mov    ecx, 11h
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, ebx
        repe cmpsb
        je     SHORT loc_6_13B9
        lea    edx, [ebp - 10h]
        inc    ebx
        mov    ecx, dword ptr [edx]
        mov    dword ptr [ebx], ecx
        mov    esi, dword ptr [edx + 4]
        mov    dword ptr [ebx + 4], esi
        mov    di, word ptr [edx + 8]
        mov    word ptr [ebx + 8], di
        mov    cl, byte ptr [edx + 0ah]
        lea    edx, [eax + 6]
        mov    byte ptr [ebx + 0ah], cl
        mov    eax, dword ptr [edx]
        test   eax, eax
        jne    SHORT loc_6_1342
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_13B9:
        lea    esi, [ebp - 10h]
        mov    edi, dword ptr [eax + 0eh]
        inc    edi
        mov    ecx, dword ptr [esi]
        mov    dword ptr [edi], ecx
        mov    ebx, dword ptr [esi + 4]
        mov    dword ptr [edi + 4], ebx
        mov    cx, word ptr [esi + 8]
        mov    word ptr [edi + 8], cx
        mov    cl, byte ptr [eax + 3]
        mov    bl, byte ptr [esi + 0ah]
        add    dword ptr [eax + 0eh], 2bh
        dec    cl
        mov    byte ptr [edi + 0ah], bl
        mov    byte ptr [eax + 3], cl
        je     SHORT loc_6_13F8
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0ah], ecx
        jb     SHORT loc_6_13F8
        pop    edi
        inc    byte ptr [eax + 2]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_13F8:
        mov    ebx, dword ptr [ebp - 0ch]
        lea    esi, [ebp - 10h]
        mov    edi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [edi], ecx
        mov    cx, word ptr [esi + 8]
        mov    dword ptr [edi + 4], ebx
        mov    word ptr [edi + 8], cx
        mov    bl, byte ptr [esi + 0ah]
        mov    esi, dword ptr [eax + 6]
        push   eax
        mov    byte ptr [edi + 0ah], bl
        mov    dword ptr [edx], esi
        call   sub_6_12B0
        add    esp, 4
loc_6_1424:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_6_1430:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        xor    esi, esi
        mov    ecx, dword ptr [ebx + 1ch]
        mov    edx, dword ptr [ebx + 14h]
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ecx + 10h]
        mov    dword ptr [ebp - 14h], edx
        mov    dword ptr [ebp - 24h], esi
        mov    dword ptr [ebp - 18h], eax
        test   byte ptr [ecx + 1ch], 80h
        mov    dword ptr [ebp - 20h], edx
        je     SHORT loc_6_1475
        mov    eax, dword ptr [ecx + 72h]
        test   eax, eax
        je     SHORT loc_6_1475
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1469:
        add    esp, 4
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 72h], esi
        mov    dword ptr [ecx + 76h], esi
loc_6_1475:
        mov    ecx, dword ptr [ebp - 8]
        test   byte ptr [ecx + 1ch], 80h
        je     SHORT loc_6_14A7
        mov    dword ptr [ecx + 76h], 0
        push   4ffh
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1490:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_14A7
        mov    ecx, dword ptr [eax + 20h]
        push   eax
        mov    word ptr [ebx + 1ah], cx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_14A4:
        add    esp, 4
loc_6_14A7:
        mov    ax, word ptr [ebx + 1ah]
        test   ax, ax
        je     SHORT loc_6_14BA
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        movsx  eax, ax
        pop    ebp
        ret
loc_6_14BA:
        cmp    byte ptr [ebx + 4], 0
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebp - 28h], eax
        je     NEAR PTR loc_6_154E
        mov    eax, dword ptr [ebp - 14h]
        mov    cl, byte ptr [eax + 11h]
        cmp    cl, 0ffh
        jne    SHORT loc_6_14EF
        mov    eax, dword ptr [ebp - 18h]
        test   byte ptr [eax + 1bh], 4
        je     SHORT loc_6_14EF
        mov    eax, 12h
        pop    edi
        mov    word ptr [ebx + 1ah], 12h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_14EF:
        mov    eax, dword ptr [ebp - 14h]
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [eax], cl
        lea    ecx, [ebp - 38h]
        mov    byte ptr [ebp - 38h], 0
        push   ecx
        push   eax
        push   edx
        call   sub_6_1320
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    NEAR PTR loc_6_17C8
        cmp    byte ptr [ebp - 38h], 0
        je     SHORT loc_6_1539
        lea    edx, [ebp - 38h]
        mov    ecx, dword ptr [ebp - 14h]
        inc    ecx
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    esi, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], esi
        mov    di, word ptr [edx + 8]
        mov    word ptr [ecx + 8], di
        mov    al, byte ptr [edx + 0ah]
        mov    byte ptr [ecx + 0ah], al
loc_6_1539:
        db  0C7h, 043h, 00Ch, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 28h], 0
        mov    dword ptr [ebp - 24h], 800h
loc_6_154E:
        mov    ecx, dword ptr [ebp - 8]
        mov    si, word ptr [ecx + 1ch]
        and    si, 80h
        je     SHORT loc_6_1565
        mov    dword ptr [ebp - 10h], 1
        jmp    SHORT loc_6_1589
loc_6_1565:
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [ecx + 20h]
        mov    edi, 2bh
        sub    eax, 28h
        sub    edx, edx
        div    edi
        mov    dword ptr [ebp - 10h], eax
        cmp    eax, 14h
        jle    SHORT loc_6_1589
        mov    dword ptr [ebp - 10h], 14h
loc_6_1589:
        cmp    byte ptr [ebx + 4], 0
        jne    NEAR PTR loc_6_1667
        test   si, si
        jne    NEAR PTR loc_6_1667
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + 72h], 0
        je     NEAR PTR loc_6_1667
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_15AE:
        mov    ecx, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ecx + 76h]
        ja     NEAR PTR loc_6_1667
        mov    eax, dword ptr [ecx + 55h]
        cmp    dword ptr [ecx + 65h], eax
        jne    NEAR PTR loc_6_1667
        mov    edx, dword ptr [ebx + 0ch]
        mov    ecx, dword ptr [ecx + 72h]
        add    edx, 4
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [ebp - 0ch], 0
        lea    eax, [ecx + 4]
        mov    esi, edx
        mov    cx, word ptr [ecx]
        mov    edi, eax
        mov    word ptr [ebp - 0ch], cx
        sub    dword ptr [ebp - 0ch], 4
        mov    ecx, dword ptr [ebp - 0ch]
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_6_1667
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_6_1667
        mov    ecx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebx + 0ch]
        mov    si, word ptr [ecx]
        mov    cx, word ptr [edi]
        mov    word ptr [ebp - 4], cx
        cmp    si, cx
        jne    SHORT loc_6_1626
        mov    eax, 12h
        pop    edi
        mov    word ptr [ebx + 1ah], 12h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_1626:
        xor    di, di
        xor    ecx, ecx
        sub    si, 4
        mov    word ptr [ebp - 0ah], si
        mov    si, word ptr [ebp - 4]
        sub    si, 4
        cmp    si, word ptr [ebp - 0ah]
        jbe    SHORT loc_6_1667
loc_6_1641:
        cmp    word ptr [eax], 0
        jne    SHORT loc_6_164D
        cmp    cx, word ptr [ebp - 0ah]
        je     SHORT loc_6_168C
loc_6_164D:
        mov    si, word ptr [edx]
        add    cx, si
        movzx  esi, si
        add    di, word ptr [eax]
        add    edx, esi
        xor    esi, esi
        mov    si, word ptr [eax]
        add    eax, esi
        cmp    di, cx
        je     SHORT loc_6_1641
loc_6_1667:
        call   sub_6_1250
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebx + 48h], eax
        test   eax, eax
        je     SHORT loc_6_169E
        push   35ch
        add    eax, 12h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1684:
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        jmp    SHORT loc_6_16CB
loc_6_168C:
        mov    eax, 3
        pop    edi
        mov    word ptr [ebx + 1ah], 3
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_169E:
        push   61h
        mov    dword ptr [ebx], 2bh
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_16AC:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_16BE
        movsx  eax, word ptr [ebx + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_16BE:
        mov    dword ptr [ebp - 10h], 1
        mov    eax, dword ptr [ebx + 50h]
        mov    dword ptr [ebp - 0ch], eax
loc_6_16CB:
        test   byte ptr [ebx + 3], 20h
        je     SHORT loc_6_1727
        push   208h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_16DB:
        add    esp, 4
        mov    dword ptr [ebp - 1ch], eax
        test   eax, eax
        je     SHORT loc_6_1727
        mov    eax, dword ptr [ebx + 10h]
        mov    ecx, dword ptr [ebp - 1ch]
        push   1
        mov    dword ptr [ebp - 2ch], eax
        push   1
        mov    dword ptr [ebx + 10h], ecx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_16FB:
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_6_170F
        mov    edi, dword ptr [ebx + 0ch]
        mov    esi, dword ptr [ebp - 1ch]
        mov    ecx, 82h
        rep movsd
loc_6_170F:
        mov    eax, dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp - 1ch]
        push   ecx
        mov    dword ptr [ebx + 10h], eax
        mov    word ptr [ebx + 1ah], 0
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1724:
        add    esp, 4
loc_6_1727:
        mov    eax, dword ptr [ebp - 18h]
        test   byte ptr [eax + 1bh], 4
        je     SHORT loc_6_173C
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        call   sub_6_18F0
        add    esp, 4
loc_6_173C:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_6_174B
        mov    cl, byte ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [edx + 4], cl
loc_6_174B:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        push   eax
        lea    edx, [ecx + ecx*4]
        lea    eax, [ecx + edx*4]
        lea    edx, [ecx + eax*2]
        mov    eax, dword ptr [ebx + 0ch]
        push   edx
        mov    ecx, dword ptr [ebp - 28h]
        push   eax
        mov    edx, dword ptr [ebp - 10h]
        push   ecx
        mov    eax, dword ptr [ebp - 24h]
        push   edx
        or     eax, 4081h
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_177C:
        add    esp, 20h
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1788:
        add    esp, 4
        cmp    word ptr [ebx + 1ah], 0
        je     SHORT loc_6_17AF
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_6_17A4
        mov    edx, dword ptr [ebp - 4]
        push   edx
        call   sub_6_12B0
        add    esp, 4
loc_6_17A4:
        movsx  eax, word ptr [ebx + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_17AF:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_6_188A
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 6], eax
        mov    dword ptr [ecx + 0ch], edx
loc_6_17C8:
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edx + 0eh]
        add    eax, 2ah
        cmp    byte ptr [eax], 20h
        jne    SHORT loc_6_17DF
        push   eax
        call   sub_6_18D0
        add    esp, 4
loc_6_17DF:
        mov    eax, dword ptr [ebp - 18h]
        test   byte ptr [eax + 1bh], 4
        je     SHORT loc_6_1806
        mov    edx, dword ptr [ebp - 4]
        cmp    byte ptr [edx + 4], 0ah
        jb     SHORT loc_6_1806
        cmp    byte ptr [edx + 5], 0ah
        jae    SHORT loc_6_1806
        cmp    byte ptr [edx + 3], 1
        jne    SHORT loc_6_1806
        mov    eax, dword ptr [edx + 0eh]
        mov    byte ptr [eax + 11h], 0ffh
        jmp    SHORT loc_6_1811
loc_6_1806:
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edx + 0eh]
        mov    cl, byte ptr [eax]
        mov    byte ptr [eax + 11h], cl
loc_6_1811:
        mov    eax, dword ptr [ebp - 14h]
        push   2bh
        mov    cl, byte ptr [eax + 13h]
        mov    dl, byte ptr [eax + 14h]
        mov    byte ptr [ebp - 0ah], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ebp - 9], dl
        mov    eax, dword ptr [ecx + 0eh]
        push   eax
        mov    edx, dword ptr [ebx + 14h]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1832:
        mov    word ptr [ebx + 1ah], ax
        add    esp, 0ch
        mov    cl, byte ptr [ebp - 0ah]
        mov    edx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [edx + 13h], cl
        mov    cl, byte ptr [ebp - 9]
        push   eax
        mov    eax, dword ptr [ebx + 14h]
        mov    byte ptr [edx + 14h], cl
        add    eax, 16h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1858:
        add    esp, 8
        xor    eax, eax
loc_6_185D:
        mov    ecx, dword ptr [ebp - 20h]
        lea    ecx, [eax + ecx + 1]
        mov    dl, byte ptr [ecx]
        cmp    dl, 3eh
        je     SHORT loc_6_1870
        cmp    dl, 22h
        jne    SHORT loc_6_1873
loc_6_1870:
        mov    byte ptr [ecx], 3fh
loc_6_1873:
        inc    eax
        cmp    eax, 0bh
        jb     SHORT loc_6_185D
        mov    edx, dword ptr [ebp - 4]
        pop    edi
        movsx  eax, word ptr [ebx + 1ah]
        pop    esi
        dec    byte ptr [edx + 2]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_188A:
        mov    esi, dword ptr [ebx + 14h]
        xor    eax, eax
        add    esi, 1eh
loc_6_1892:
        mov    ecx, dword ptr [ebp - 20h]
        lea    ecx, [eax + ecx + 1]
        mov    dl, byte ptr [ecx]
        cmp    dl, 3eh
        je     SHORT loc_6_18A5
        cmp    dl, 22h
        jne    SHORT loc_6_18A8
loc_6_18A5:
        mov    byte ptr [ecx], 3fh
loc_6_18A8:
        inc    eax
        cmp    eax, 0bh
        jb     SHORT loc_6_1892
        add    esi, 0ch
        cmp    byte ptr [esi], 20h
        jne    SHORT loc_6_18BF
        push   esi
        call   sub_6_18D0
        add    esp, 4
loc_6_18BF:
        movsx  eax, word ptr [ebx + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_6_18D0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
loc_6_18D7:
        mov    byte ptr [eax], 0
        dec    eax
        cmp    byte ptr [eax], 20h
        je     SHORT loc_6_18D7
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_18F0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        xor    edx, edx
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 2]
        add    ecx, eax
        cmp    word ptr [ecx], dx
        jbe    SHORT loc_6_1933
        lea    esi, [ecx + 2]
loc_6_1909:
        xor    eax, eax
        mov    ax, word ptr [esi]
        cmp    eax, 2eh
        je     SHORT loc_6_191A
        cmp    eax, 3fh
        je     SHORT loc_6_1921
        jmp    SHORT loc_6_1926
loc_6_191A:
        mov    word ptr [esi], 22h
        jmp    SHORT loc_6_1926
loc_6_1921:
        mov    word ptr [esi], 3eh
loc_6_1926:
        add    esi, 2
        inc    edx
        xor    eax, eax
        mov    ax, word ptr [ecx]
        cmp    eax, edx
        jg     SHORT loc_6_1909
loc_6_1933:
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_1940:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi], 424d53ffh
        jne    NEAR PTR loc_6_19E0
        cmp    byte ptr [esi + 4], 24h
        jne    NEAR PTR loc_6_19E0
        cmp    word ptr [esi + 1eh], 0ffffh
        jne    SHORT loc_6_19E0
        mov    dx, word ptr [esi + 18h]
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, eax
        test   ecx, ecx
        je     SHORT loc_6_1982
loc_6_1975:
        cmp    word ptr [ecx + 1eh], dx
        je     SHORT loc_6_1987
        mov    ecx, dword ptr [ecx + 4]
        cmp    ecx, eax
        jne    SHORT loc_6_1975
loc_6_1982:
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_1987:
        mov    edx, dword ptr [ecx + 30h]
        test   edx, edx
        je     SHORT loc_6_19A1
        mov    cx, word ptr [esi + 25h]
        mov    eax, edx
loc_6_1994:
        cmp    word ptr [edx + 35h], cx
        je     SHORT loc_6_19A6
        mov    edx, dword ptr [edx + 8]
        cmp    eax, edx
        jne    SHORT loc_6_1994
loc_6_19A1:
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_19A6:
        mov    eax, dword ptr [edx + 14h]
        mov    cl, byte ptr [eax + 2ah]
        and    cl, 0f7h
        cmp    word ptr [eax + 24h], 0
        mov    byte ptr [eax + 2ah], cl
        jne    SHORT loc_6_19BF
        and    cl, 0efh
        mov    byte ptr [eax + 2ah], cl
loc_6_19BF:
        cmp    word ptr [eax + 26h], 0
        jne    SHORT loc_6_19CA
        and    byte ptr [eax + 2ah], 0dfh
loc_6_19CA:
        and    byte ptr [edx + 33h], 0fch
        or     byte ptr [edx + 34h], 1
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_19D8:
        add    esp, 4
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
loc_6_19E0:
        mov    eax, 1
        pop    esi
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_6_19F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 38h]
        mov    edi, dword ptr [eax + 20h]
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [edi + 14h]
        mov    ebx, dword ptr [edx + 1ch]
        test   ebx, ebx
        je     SHORT loc_6_1A51
loc_6_1A0F:
        mov    dl, byte ptr [ebx + 16h]
        test   dl, 80h
        jne    SHORT loc_6_1A47
        or     dl, 80h
        add    eax, 0ah
        mov    byte ptr [ebx + 16h], dl
        mov    edx, dword ptr [ebx + 10h]
        mov    word ptr [eax - 0ah], dx
        mov    edx, dword ptr [ebx + 8]
        mov    dword ptr [eax - 8], edx
        mov    edx, dword ptr [ebx + 0ch]
        sub    edx, dword ptr [ebx + 8]
        inc    edx
        mov    dword ptr [eax - 4], edx
        mov    dx, word ptr [ecx + 0fh]
        inc    dx
        mov    word ptr [ecx + 0fh], dx
        cmp    dx, 6eh
        jae    SHORT loc_6_1A4D
loc_6_1A47:
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    SHORT loc_6_1A0F
loc_6_1A4D:
        test   ebx, ebx
        jne    SHORT loc_6_1A64
loc_6_1A51:
        and    byte ptr [edi + 34h], 0feh
        or     byte ptr [ecx + 7], 2
        cmp    word ptr [ecx + 0fh], 0
        jne    SHORT loc_6_1A64
        or     byte ptr [esi + 2], 2
loc_6_1A64:
        pop    edi
        mov    dword ptr [esi + 38h], eax
        mov    eax, dword ptr [ebp + 8]
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  090h
sub_6_1A70:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   18eh
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1A83:
        add    esp, 4
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        jne    SHORT loc_6_1AA2
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, 8
        mov    word ptr [ecx + 1ah], 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_1AA2:
        mov    ebx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        add    ebx, 104h
        add    eax, 132h
        mov    edi, eax
        mov    ecx, 17h
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        rep stosd
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0ch]
        mov    edi, dword ptr [ebp - 8]
        mov    ax, word ptr [edx + 4]
        lea    esi, [eax + edx]
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        add    esi, 6
        sub    eax, 2
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        xor    eax, eax
        lea    esi, [edx + 6]
        mov    ax, word ptr [edx + 4]
        mov    edi, dword ptr [ebp - 0ch]
        sub    eax, 2
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 002h
        je     SHORT loc_6_1B25
        mov    ecx, dword ptr [ebp + 8]
        test   byte ptr [ecx + 18h], 8
        jne    SHORT loc_6_1B25
        mov    word ptr [ecx + 1ah], 35h
        jmp    NEAR PTR loc_6_1FFC
loc_6_1B25:
        mov    esi, dword ptr [ebp - 0ch]
        push   1
        add    esi, 2
        push   2eh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1B35:
        add    esp, 4
        add    eax, eax
        push   eax
        push   esi
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1B42:
        mov    byte ptr [eax + ebx], 0
        mov    eax, dword ptr [ebp - 0ch]
        add    esp, 14h
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [eax], 5ch
        je     SHORT loc_6_1B5F
        mov    word ptr [ecx + 1ah], 35h
        jmp    NEAR PTR loc_6_1FFC
loc_6_1B5F:
        cmp    byte ptr [ecx + 4], 4
        je     SHORT loc_6_1BA4
        mov    eax, ebx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_6_1B6D:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    SHORT loc_6_1B8D
        or     dl, dl
        je     SHORT loc_6_1B89
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    SHORT loc_6_1B8D
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    SHORT loc_6_1B6D
loc_6_1B89:
        xor    eax, eax
        jmp    SHORT loc_6_1B92
loc_6_1B8D:
        sbb    eax, eax
        sbb    eax, -1
loc_6_1B92:
        test   eax, eax
        jne    SHORT loc_6_1BA4
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ah], 83ah
        jmp    NEAR PTR loc_6_1FFC
loc_6_1BA4:
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 18h]
        and    ax, 10h
        cmp    ax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        xor    eax, eax
        mov    ax, word ptr [ecx + 6]
        push   eax
        push   ebx
        call   sub_6_21F0
        add    esp, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_6_1BF3
        mov    ax, word ptr [ecx + 18h]
        and    ax, 10h
        cmp    ax, 1
        sbb    eax, eax
        and    eax, 0fffff78ah
        add    eax, 8cah
        mov    word ptr [ecx + 1ah], ax
        jmp    NEAR PTR loc_6_1FFC
loc_6_1BF3:
        mov    eax, dword ptr [ecx + 0ch]
        xor    edx, edx
        mov    dx, word ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        sub    edx, 2
        lea    esi, [eax + 6]
        mov    edi, dword ptr [ecx + 62h]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [ebp - 4]
        cmp    byte ptr [edx + 2ch], 0
        je     SHORT loc_6_1C3F
        xor    edx, edx
        mov    edi, dword ptr [ebp - 4]
        mov    dx, word ptr [eax + 4]
        add    edi, 3ch
        sub    edx, 2
        lea    esi, [eax + 6]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
loc_6_1C3F:
        mov    ecx, dword ptr [ebp + 8]
        push   1
        push   2eh
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    dword ptr [ecx], 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1C55:
        add    esp, 4
        add    eax, eax
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1C65:
        add    esp, 14h
        mov    byte ptr [eax + ebx], 0
        cmp    eax, 2dh
        jbe    SHORT loc_6_1C7F
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ah], 35h
        jmp    NEAR PTR loc_6_1FED
loc_6_1C7F:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 5
        mov    esi, ebx
        repe cmpsb
        je     SHORT loc_6_1CDA
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 9
        mov    esi, ebx
        repe cmpsb
        je     SHORT loc_6_1CDA
        mov    ecx, dword ptr [ebp + 8]
        test   byte ptr [ecx + 18h], 1
        jne    SHORT loc_6_1CD3
        mov    ecx, dword ptr [ecx + 0ch]
        xor    eax, eax
        mov    ax, word ptr [ecx + 4]
        xor    edx, edx
        mov    dx, word ptr [ecx + eax + 4]
        add    edx, eax
        xor    eax, eax
        mov    ax, word ptr [edx + ecx + 4]
        test   eax, eax
        je     SHORT loc_6_1CD3
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ah], 57h
        jmp    NEAR PTR loc_6_1FED
loc_6_1CD3:
        mov    ecx, dword ptr [ebp + 8]
        inc    dword ptr [ecx]
        jmp    SHORT loc_6_1D57
loc_6_1CDA:
        mov    ecx, dword ptr [ebp + 8]
        test   byte ptr [ecx + 18h], 1
        jne    SHORT loc_6_1CEE
        mov    word ptr [ecx + 1ah], 57h
        jmp    NEAR PTR loc_6_1FED
loc_6_1CEE:
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 4], 0
        jne    SHORT loc_6_1CFB
        mov    byte ptr [ecx + 4], 4
loc_6_1CFB:
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 4], 4
        je     SHORT loc_6_1D0F
        mov    word ptr [ecx + 1ah], 42h
        jmp    NEAR PTR loc_6_1FED
loc_6_1D0F:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, ebx
        rep movsd
        mov    ecx, eax
        push   1
        and    ecx, 3
        rep movsb
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1D54:
        add    esp, 10h
loc_6_1D57:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        inc    eax
        mov    ebx, dword ptr [ecx + 0ch]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], eax
        test   ebx, ebx
        je     SHORT loc_6_1DBB
loc_6_1D6F:
        lea    eax, [ebx + 7bh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1D78:
        add    esp, 4
        mov    esi, eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1D86:
        add    esp, 4
        cmp    esi, eax
        jne    SHORT loc_6_1DB0
        lea    esi, [ebx + 7bh]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1D96:
        add    esp, 4
        add    eax, eax
        mov    ecx, eax
        mov    edi, dword ptr [ebp - 8]
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_6_1DAE
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
loc_6_1DAE:
        je     SHORT loc_6_1DEF
loc_6_1DB0:
        mov    ebx, dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [ecx + 0ch], ebx
        jne    SHORT loc_6_1D6F
loc_6_1DBB:
        mov    ecx, dword ptr [ebp + 8]
        test   byte ptr [ecx + 18h], 10h
        jne    SHORT loc_6_1E3A
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1DCF:
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 10h], eax
        db  0A1h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_1E48
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1DE7:
        add    esp, 4
        jmp    NEAR PTR loc_6_1D57
loc_6_1DEF:
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 4]
        test   cl, cl
        je     SHORT loc_6_1E03
        mov    al, byte ptr [ebx + 14h]
        test   al, al
        jne    SHORT loc_6_1E1D
        mov    byte ptr [ebx + 14h], cl
loc_6_1E03:
        mov    ecx, dword ptr [ebp + 8]
        test   byte ptr [ecx + 18h], 10h
        je     SHORT loc_6_1E2F
        cmp    dword ptr [ebx + 2ch], 0
        jne    SHORT loc_6_1E2F
        mov    word ptr [ecx + 1ah], 8cah
        jmp    NEAR PTR loc_6_1FED
loc_6_1E1D:
        cmp    al, cl
        je     SHORT loc_6_1E03
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ah], 42h
        jmp    NEAR PTR loc_6_1FED
loc_6_1E2F:
        and    word ptr [ebx + 1ch], 0fff7h
        jmp    NEAR PTR loc_6_1F3A
loc_6_1E3A:
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ah], 8cah
        jmp    NEAR PTR loc_6_1FED
loc_6_1E48:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1E51:
        add    esp, 4
        lea    eax, [eax*2 + 2]
        mov    dword ptr [ebp - 10h], eax
        add    eax, 7dh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1E67:
        add    esp, 4
        mov    ebx, eax
        xor    eax, eax
        test   ebx, ebx
        je     NEAR PTR loc_6_1FD6
        mov    edx, dword ptr [ebp - 10h]
        lea    edi, [ebx + 7bh]
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    ecx, [ebx + 35h]
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], eax
        mov    dl, byte ptr [esi + 4]
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], eax
        mov    cx, word ptr [esi + 18h]
        mov    byte ptr [ebx + 14h], dl
        mov    byte ptr [ebx + 35h], 20h
        mov    word ptr [ebx + 1ch], 80h
        mov    word ptr [ebx + 26h], 37h
        and    cl, 20h
        mov    byte ptr [ebx + 34h], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 10h], ecx
        mov    dword ptr [ebx + 2ch], eax
        db  066h, 08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 18h], eax
        mov    word ptr [ebx + 24h], cx
        mov    dword ptr [ebx + 0ch], eax
        inc    cx
        mov    dword ptr [ebx + 30h], eax
        db  066h, 089h, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 45h], eax
        mov    ecx, 0ffffffh
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 61h], ecx
        mov    dword ptr [ebx + 59h], ecx
        mov    dword ptr [ebx + 65h], ecx
        mov    dword ptr [ebx + 6dh], eax
        mov    ecx, dword ptr [edx + 0ch]
        mov    dword ptr [ebx + 5dh], eax
        test   ecx, ecx
        je     SHORT loc_6_1F19
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [ebx + 8], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [ecx + 8], ebx
        mov    dword ptr [eax + 4], ebx
        jmp    SHORT loc_6_1F25
loc_6_1F19:
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 8], ebx
        mov    dword ptr [ebx + 4], ebx
        mov    dword ptr [ecx + 0ch], ebx
loc_6_1F25:
        db  066h, 0FFh, 005h, 000h, 000h, 000h, 000h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1F37:
        add    esp, 4
loc_6_1F3A:
        mov    ax, word ptr [ebx + 1ch]
        mov    ecx, dword ptr [ebp + 8]
        and    ax, 0fffdh
        mov    dword ptr [ecx + 1ch], ebx
        test   byte ptr [ecx + 18h], 1
        mov    word ptr [ebx + 1ch], ax
        je     SHORT loc_6_1F58
        or     al, 4
        mov    word ptr [ebx + 1ch], ax
loc_6_1F58:
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        call   sub_6_2030
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_6_1FB8
        cmp    dword ptr [ebx + 2ch], 0
        jne    SHORT loc_6_1F77
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 20h]
        mov    dword ptr [ebx + 2ch], eax
loc_6_1F77:
        mov    eax, dword ptr [ebx + 2ch]
        mov    ecx, dword ptr [ebp + 8]
        db  0C7h, 041h, 010h, 000h, 000h, 000h, 000h
        test   byte ptr [ecx + 18h], 8
        mov    dword ptr [ecx + 20h], eax
        je     SHORT loc_6_1F95
        mov    word ptr [ecx + 6], 0ffh
        jmp    SHORT loc_6_1FA7
loc_6_1F95:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 76h]
        mov    ecx, dword ptr [ebp + 8]
        movzx  ax, byte ptr [edx + 5ah]
        mov    word ptr [ecx + 6], ax
loc_6_1FA7:
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        movzx  ax, byte ptr [ecx + 19h]
        mov    word ptr [edx + 18h], ax
        jmp    SHORT loc_6_1FED
loc_6_1FB8:
        cmp    dword ptr [ebx + 2ch], 0
        jne    SHORT loc_6_1FED
        mov    dword ptr [ebx + 28h], 0
        mov    ax, word ptr [ebx + 1ch]
        test   ah, 1
        jne    SHORT loc_6_1FED
        or     al, 8
        mov    word ptr [ebx + 1ch], ax
        jmp    SHORT loc_6_1FED
loc_6_1FD6:
        mov    ecx, dword ptr [ebp + 8]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [ecx + 1ah], 54h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_1FEA:
        add    esp, 4
loc_6_1FED:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [ecx + 6], 0
        dec    byte ptr [eax + 2]
loc_6_1FFC:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2005:
        add    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        movsx  eax, word ptr [ecx + 1ah]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 033h, 0C0h, 0C3h, 08Dh, 09Bh, 000h
        db  000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_2030:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ebx, dword ptr [edx + 1ch]
        push   edi
        test   byte ptr [ebx + 1ch], 80h
        jne    SHORT loc_6_204A
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_204A:
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    esi, dword ptr [edx + 28h]
        test   esi, esi
        je     SHORT loc_6_20A5
        lea    edx, [ebx + 35h]
        mov    edi, esi
        mov    ecx, 0ffffffffh
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx + 0ch], eax
        sub    eax, eax
        repne scasb
        not    ecx
        lea    eax, [ecx - 1]
        cmp    eax, 0fh
        jbe    SHORT loc_6_207E
        mov    eax, 0fh
loc_6_207E:
        mov    edi, edx
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        cmp    byte ptr [edx], 20h
        jne    SHORT loc_6_20A1
        cmp    byte ptr [ebx + 36h], 0
        jne    SHORT loc_6_20A1
        and    word ptr [ebx + 1ch], 0ffdfh
        jmp    SHORT loc_6_20A5
loc_6_20A1:
        or     byte ptr [ebx + 1ch], 20h
loc_6_20A5:
        mov    edx, dword ptr [ebp + 8]
        test   byte ptr [edx + 18h], 8
        jne    SHORT loc_6_20E5
        push   4ffh
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_20B9:
        mov    cx, word ptr [eax + 20h]
        add    esp, 8
        cmp    cx, 56h
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx + 1ah], cx
        jne    SHORT loc_6_20DC
        mov    ecx, dword ptr [ebx + 10h]
        test   byte ptr [ecx + 1ah], 10h
        je     SHORT loc_6_20DC
        mov    word ptr [edx + 1ah], 963h
loc_6_20DC:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_20E2:
        add    esp, 4
loc_6_20E5:
        mov    edx, dword ptr [ebp + 8]
        pop    edi
        movsx  eax, word ptr [edx + 1ah]
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_2100:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 1ch]
        mov    ax, word ptr [edi + 1ch]
        inc    dword ptr [edi + 55h]
        test   al, 2
        jne    SHORT loc_6_2125
        mov    eax, 5
        pop    edi
        mov    word ptr [esi + 1ah], 5
        pop    esi
        pop    ebp
        ret
loc_6_2125:
        cmp    byte ptr [edi + 14h], 4
        jne    SHORT loc_6_2139
        test   al, 10h
        jne    SHORT loc_6_2139
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2134:
        add    eax, 2
        jmp    SHORT loc_6_2144
loc_6_2139:
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_213E:
        db  003h, 005h, 000h, 000h, 000h, 000h
loc_6_2144:
        mov    dword ptr [edi + 28h], eax
        mov    ax, word ptr [edi + 1ch]
        test   ah, 1
        jne    SHORT loc_6_2156
        or     al, 8
        mov    word ptr [edi + 1ch], ax
loc_6_2156:
        xor    eax, eax
        test   byte ptr [edi + 1ch], 4
        mov    dword ptr [edi + 2ch], eax
        je     SHORT loc_6_216F
        cmp    byte ptr [edi + 14h], 4
        jne    SHORT loc_6_216F
        mov    word ptr [esi + 1ah], ax
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_216F:
        push   esi
        call   sub_6_3290
        add    esp, 4
        xor    eax, eax
        mov    word ptr [esi + 1ah], 0
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_2190:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        mov    ecx, edi
        test   edi, edi
        je     SHORT loc_6_21E1
loc_6_21A4:
        lea    edx, [ecx + 2ch]
        mov    esi, dword ptr [ebp + 8]
loc_6_21AA:
        mov    bl, byte ptr [edx]
        cmp    bl, byte ptr [esi]
        jne    SHORT loc_6_21CA
        or     bl, bl
        je     SHORT loc_6_21C6
        mov    bl, byte ptr [edx + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    SHORT loc_6_21CA
        add    edx, 2
        add    esi, 2
        or     bl, bl
        jne    SHORT loc_6_21AA
loc_6_21C6:
        xor    edx, edx
        jmp    SHORT loc_6_21CF
loc_6_21CA:
        sbb    edx, edx
        sbb    edx, -1
loc_6_21CF:
        test   edx, edx
        jne    SHORT loc_6_21DA
        xor    edx, edx
        mov    dl, byte ptr [ecx + 7eh]
        add    eax, edx
loc_6_21DA:
        mov    ecx, dword ptr [ecx + 4]
        cmp    ecx, edi
        jne    SHORT loc_6_21A4
loc_6_21E1:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_21F0:
        push   ebp
        mov    ecx, 0ffffffffh
        mov    ebp, esp
        sub    esp, 8
        sub    eax, eax
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        repne scasb
        not    ecx
        mov    dword ptr [ebp - 8], ecx
loc_6_220D:
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        inc    edi
        test   esi, esi
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_6_22C0
loc_6_2228:
        lea    eax, [esi + 2ch]
        mov    edx, dword ptr [ebp + 8]
loc_6_222E:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_6_224E
        or     cl, cl
        je     SHORT loc_6_224A
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_6_224E
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_6_222E
loc_6_224A:
        xor    eax, eax
        jmp    SHORT loc_6_2253
loc_6_224E:
        sbb    eax, eax
        sbb    eax, -1
loc_6_2253:
        test   eax, eax
        je     SHORT loc_6_229D
        mov    eax, dword ptr [esi + 5eh]
        mov    edx, dword ptr [ebp + 8]
loc_6_225D:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_6_227D
        or     cl, cl
        je     SHORT loc_6_2279
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_6_227D
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_6_225D
loc_6_2279:
        xor    eax, eax
        jmp    SHORT loc_6_2282
loc_6_227D:
        sbb    eax, eax
        sbb    eax, -1
loc_6_2282:
        test   eax, eax
        je     SHORT loc_6_229D
        cmp    dword ptr [esi + 66h], 0
        je     SHORT loc_6_22B1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2295:
        add    esp, 4
        cmp    eax, dword ptr [esi + 66h]
        jne    SHORT loc_6_22B1
loc_6_229D:
        cmp    ebx, 0ffh
        je     SHORT loc_6_22EF
        mov    ecx, dword ptr [esi + 76h]
        xor    eax, eax
        mov    al, byte ptr [ecx + 5ah]
        cmp    eax, ebx
        je     SHORT loc_6_22EF
loc_6_22B1:
        mov    esi, dword ptr [esi + 4]
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_6_2228
loc_6_22C0:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_6_22FB
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_22D1:
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_6_2304
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_22E7:
        add    esp, 4
        jmp    NEAR PTR loc_6_220D
loc_6_22EF:
        mov    eax, esi
        pop    edi
        inc    byte ptr [esi + 2]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_22FB:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_2304:
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [eax + eax*2]
        add    eax, 91h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2315:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     NEAR PTR loc_6_2423
        mov    word ptr [eax], 6553h
        push   1
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_232F:
        add    esp, 4
        mov    ecx, dword ptr [ebp - 4]
        test   eax, eax
        mov    dword ptr [ecx + 24h], eax
        je     NEAR PTR loc_6_2410
        mov    byte ptr [ecx + 1dh], 80h
        lea    edx, [ecx + 8fh]
        mov    byte ptr [ecx + 7dh], bl
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 62h], edx
        mov    byte ptr [ecx + 2], 1
        lea    edx, [edx + eax*2 + 2]
        mov    edi, dword ptr [ebp + 8]
        sub    eax, eax
        mov    dword ptr [ecx + 5eh], edx
        mov    byte ptr [ecx + 18h], 1
        mov    dword ptr [ecx + 85h], 0ffffffffh
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, edx
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    eax, dword ptr [ebp - 8]
        dec    eax
        cmp    eax, 0fh
        jg     SHORT loc_6_23D7
        mov    ecx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ecx + 5eh]
        mov    eax, edi
        cmp    byte ptr [edi], 0
        je     SHORT loc_6_23B4
loc_6_23A4:
        cmp    byte ptr [eax], 2eh
        je     SHORT loc_6_23AF
        inc    eax
        cmp    byte ptr [eax], 0
        jne    SHORT loc_6_23A4
loc_6_23AF:
        cmp    byte ptr [eax], 0
        jne    SHORT loc_6_23D7
loc_6_23B4:
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp - 4]
        add    edi, 2ch
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_6_23D7:
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        test   edx, edx
        je     SHORT loc_6_23F8
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [edx + 8], ecx
        mov    dword ptr [eax + 4], ecx
        jmp    SHORT loc_6_23FE
loc_6_23F8:
        mov    dword ptr [ecx + 8], ecx
        mov    dword ptr [ecx + 4], ecx
loc_6_23FE:
        db  066h, 0FFh, 005h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_6_2423
loc_6_2410:
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2419:
        mov    dword ptr [ebp - 4], 0
        add    esp, 4
loc_6_2423:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_242E:
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_6_2440:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        xor    eax, eax
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 14h]
        mov    word ptr [ebx + 1], 0ffh
        mov    edi, dword ptr [esi + 1ch]
        mov    al, byte ptr [esi + 5]
        mov    byte ptr [ebx], 0ah
        db  08Bh, 00Ch, 085h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [ebp - 8], ecx
        mov    word ptr [ebx + 5], ax
        mov    ecx, dword ptr [ebp + 10h]
        movzx  dx, byte ptr [ecx + 18h]
        mov    word ptr [ebx + 7], dx
        lea    eax, [ecx + 2ch]
        push   eax
        call   sub_6_2190
        mov    word ptr [ebx + 9], ax
        add    esp, 4
        mov    dword ptr [ebx + 11h], 0
        mov    ecx, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp - 8], 0
        mov    edx, dword ptr [ecx + 28h]
        mov    dword ptr [ebx + 0bh], edx
        je     NEAR PTR loc_6_257F
        mov    cl, byte ptr [ecx + 19h]
        test   cl, 1
        je     NEAR PTR loc_6_2534
        cmp    byte ptr [esi + 5], 2
        jne    SHORT loc_6_24DD
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [eax + 28h]
        test   esi, esi
        je     SHORT loc_6_24D5
        lea    edi, [ebx + 17h]
        mov    ecx, 6
        rep movsd
        mov    word ptr [ebx + 0fh], 18h
        jmp    SHORT loc_6_253E
loc_6_24D5:
        mov    word ptr [ebx + 0fh], 0
        jmp    SHORT loc_6_253E
loc_6_24DD:
        test   byte ptr [edi + 1ch], 20h
        db  0BEh, 000h, 000h, 000h, 000h
        je     SHORT loc_6_24ED
        lea    esi, [edi + 35h]
        jmp    SHORT loc_6_24FC
loc_6_24ED:
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 0ch]
        test   byte ptr [eax + 1ch], 20h
        je     SHORT loc_6_24FC
        lea    esi, [eax + 35h]
loc_6_24FC:
        cmp    byte ptr [esi], 0
        jne    SHORT loc_6_2507
        mov    esi, dword ptr [ebp - 8]
        add    esi, 5
loc_6_2507:
        test   cl, 2
        mov    word ptr [ebx + 0fh], 18h
        je     SHORT loc_6_2528
        lea    eax, [ebx + 17h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 6ah
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2523:
        add    esp, 0ch
        jmp    SHORT loc_6_253E
loc_6_2528:
        lea    edi, [ebx + 17h]
        mov    ecx, 6
        rep movsd
        jmp    SHORT loc_6_253E
loc_6_2534:
        mov    byte ptr [ebx + 17h], 0
        mov    word ptr [ebx + 0fh], 1
loc_6_253E:
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, 0ffffffffh
        add    esi, 13h
        sub    eax, eax
        mov    edi, esi
        repne scasb
        mov    ax, word ptr [ebx + 0fh]
        not    ecx
        mov    edx, ecx
        lea    ecx, [eax + ecx]
        movzx  edi, ax
        mov    word ptr [ebx + 15h], cx
        lea    eax, [edi + ebx + 17h]
        mov    edi, eax
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        add    eax, edx
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 38h], eax
        jmp    SHORT loc_6_259C
loc_6_257F:
        xor    eax, eax
        lea    ecx, [ebx + 19h]
        mov    edx, dword ptr [ebp + 0ch]
        mov    byte ptr [ebx + 17h], al
        mov    word ptr [ebx + 0fh], 1
        mov    byte ptr [ebx + 18h], al
        mov    dword ptr [edx + 38h], ecx
        mov    word ptr [ebx + 15h], 2
loc_6_259C:
        mov    eax, dword ptr [ebp + 10h]
        test   byte ptr [eax + 1ah], 8
        je     SHORT loc_6_2616
        cmp    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [edx + 38h]
        mov    dword ptr [ebp - 4], eax
        je     SHORT loc_6_25CD
        mov    esi, dword ptr [ebp - 8]
        add    esi, 30h
        cmp    byte ptr [esi], 0
        je     SHORT loc_6_25CD
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, ecx
        jmp    SHORT loc_6_25E4
loc_6_25CD:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, ecx
        db  0BEh, 000h, 000h, 000h, 000h
loc_6_25E4:
        shr    ecx, 2
        mov    edi, dword ptr [ebp - 4]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, 6
        add    dword ptr [ebp - 4], eax
        mov    edi, dword ptr [ebp - 4]
        rep movsd
        add    ax, 18h
        mov    ecx, dword ptr [ebp - 4]
        add    word ptr [ebx + 15h], ax
        add    ecx, 18h
        mov    dword ptr [edx + 38h], ecx
loc_6_2616:
        mov    eax, dword ptr [edx + 38h]
        pop    edi
        sub    eax, edx
        pop    esi
        sub    ax, 3ch
        mov    word ptr [ebx + 3], ax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_6_2630:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        xor    eax, eax
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 14h]
        mov    word ptr [ebx + 1], 0ffh
        mov    edi, dword ptr [esi + 1ch]
        mov    al, byte ptr [esi + 5]
        mov    edx, dword ptr [ebp + 10h]
        mov    byte ptr [ebx], 0dh
        db  08Bh, 00Ch, 085h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 18h]
        mov    word ptr [ebx + 5], ax
        mov    dword ptr [ebp - 8], ecx
        movzx  cx, byte ptr [edx + 18h]
        mov    word ptr [ebx + 7], cx
        lea    eax, [edx + 2ch]
        push   eax
        call   sub_6_2190
        mov    word ptr [ebx + 9], ax
        add    esp, 4
        mov    dword ptr [ebx + 13h], 0
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [ebx + 17h], 0
        mov    ecx, dword ptr [edx + 28h]
        mov    dword ptr [ebx + 0bh], ecx
        mov    cx, word ptr [edx + 1ah]
        mov    ax, cx
        and    ax, 200h
        cmp    ax, 1
        mov    eax, 0
        adc    eax, -1
        and    cx, 3
        and    eax, 4
        cmp    cx, 1
        mov    dword ptr [ebx + 17h], eax
        sbb    ecx, ecx
        inc    ecx
        or     ecx, eax
        cmp    dword ptr [ebp - 8], 0
        mov    dword ptr [ebx + 17h], ecx
        je     NEAR PTR loc_6_287B
        mov    cl, byte ptr [edx + 19h]
        test   cl, 1
        je     NEAR PTR loc_6_2814
        cmp    byte ptr [esi + 5], 2
        jne    SHORT loc_6_272A
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [eax + 28h]
        test   esi, esi
        je     SHORT loc_6_26F9
        lea    edi, [ebx + 1dh]
        mov    ecx, 6
        rep movsd
        mov    word ptr [ebx + 0fh], 18h
        jmp    SHORT loc_6_26FF
loc_6_26F9:
        mov    word ptr [ebx + 0fh], 0
loc_6_26FF:
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 8]
        mov    ax, word ptr [ebx + 0fh]
        mov    esi, dword ptr [ecx + 2ch]
        test   esi, esi
        lea    edi, [eax + ebx + 1dh]
        je     SHORT loc_6_2722
        mov    ecx, 6
        rep movsd
        mov    word ptr [ebx + 11h], 18h
        jmp    SHORT loc_6_2785
loc_6_2722:
        mov    word ptr [ebx + 11h], 0
        jmp    SHORT loc_6_2785
loc_6_272A:
        test   byte ptr [edi + 1ch], 20h
        db  0BEh, 000h, 000h, 000h, 000h
        je     SHORT loc_6_273A
        lea    esi, [edi + 35h]
        jmp    SHORT loc_6_2749
loc_6_273A:
        mov    edx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [edx + 0ch]
        test   byte ptr [eax + 1ch], 20h
        je     SHORT loc_6_2749
        lea    esi, [eax + 35h]
loc_6_2749:
        cmp    byte ptr [esi], 0
        jne    SHORT loc_6_2754
        mov    esi, dword ptr [ebp - 8]
        add    esi, 5
loc_6_2754:
        test   cl, 2
        mov    word ptr [ebx + 0fh], 18h
        mov    word ptr [ebx + 11h], 0
        je     SHORT loc_6_277B
        lea    eax, [ebx + 1dh]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 6ah
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2776:
        add    esp, 0ch
        jmp    SHORT loc_6_2785
loc_6_277B:
        lea    edi, [ebx + 1dh]
        mov    ecx, 6
        rep movsd
loc_6_2785:
        mov    esi, dword ptr [ebp - 8]
        mov    ecx, 0ffffffffh
        add    esi, 13h
        sub    eax, eax
        mov    edi, esi
        repne scasb
        not    ecx
        mov    eax, ecx
        xor    edx, edx
        mov    dx, word ptr [ebx + 11h]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 0fh]
        add    ecx, edx
        mov    edx, dword ptr [ebp + 10h]
        lea    edi, [ecx + ebx + 1dh]
        mov    cx, word ptr [edx + 1ah]
        mov    dword ptr [ebp - 4], edi
        test   ch, 2
        je     SHORT loc_6_27EA
        test   cl, 80h
        je     SHORT loc_6_27EA
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_27EA
        mov    ecx, edi
        push   1
        sub    ecx, dword ptr [ebp + 0ch]
        push   eax
        and    ecx, 1
        push   esi
        add    word ptr [ebx + 1bh], cx
        add    dword ptr [ebp - 4], ecx
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_27E5:
        add    esp, 10h
        jmp    SHORT loc_6_27FB
loc_6_27EA:
        mov    edi, dword ptr [ebp - 4]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_6_27FB:
        mov    cx, word ptr [ebx + 0fh]
        add    cx, word ptr [ebx + 11h]
        add    cx, ax
        mov    word ptr [ebx + 1bh], cx
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, eax
        jmp    NEAR PTR loc_6_2894
loc_6_2814:
        xor    eax, eax
        mov    esi, dword ptr [ebp - 8]
        add    esi, 13h
        mov    byte ptr [ebx + 1dh], al
        mov    edi, esi
        mov    ecx, 0ffffffffh
        mov    word ptr [ebx + 11h], 1
        mov    word ptr [ebx + 0fh], 1
        mov    byte ptr [ebx + 1eh], al
        sub    eax, eax
        repne scasb
        not    ecx
        mov    edx, ecx
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 11h]
        lea    eax, [edx + 2]
        mov    word ptr [ebx + 1bh], ax
        xor    eax, eax
        mov    ax, word ptr [ebx + 0fh]
        add    eax, ecx
        mov    ecx, edx
        shr    ecx, 2
        lea    edi, [eax + ebx + 1dh]
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        xor    ecx, ecx
        xor    eax, eax
        mov    cx, word ptr [ebx + 0fh]
        mov    ax, word ptr [ebx + 11h]
        add    ecx, eax
        add    ecx, edx
        add    ecx, ebx
        add    ecx, 1dh
        jmp    SHORT loc_6_2894
loc_6_287B:
        xor    eax, eax
        lea    ecx, [ebx + 1dh]
        mov    word ptr [ecx], ax
        add    ecx, 2
        mov    word ptr [ebx + 1bh], 2
        mov    word ptr [ebx + 0fh], ax
        mov    word ptr [ebx + 11h], ax
loc_6_2894:
        mov    edi, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 10h]
        mov    ax, word ptr [edx + 1ah]
        mov    dword ptr [edi + 38h], ecx
        test   ah, 2
        je     NEAR PTR loc_6_2973
        test   al, 80h
        je     NEAR PTR loc_6_2973
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_6_2973
        mov    edi, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [edi + 38h]
        mov    dword ptr [ebp - 4], esi
        je     SHORT loc_6_28EC
        mov    edx, dword ptr [ebp - 8]
        add    edx, 30h
        cmp    byte ptr [edx], 0
        je     SHORT loc_6_28EC
        mov    edi, edx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        push   1
        repne scasb
        not    ecx
        push   ecx
        push   edx
        jmp    SHORT loc_6_2904
loc_6_28EC:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        push   1
        repne scasb
        not    ecx
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
loc_6_2904:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_290A:
        add    esp, 10h
        add    esi, eax
        push   1
        push   0ch
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_291E:
        add    esp, 10h
        add    esi, eax
        mov    word ptr [esi], 0
        add    esi, 2
        push   1
        db  0BFh, 000h, 000h, 000h, 000h
        push   0ch
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        db  081h, 0C1h, 000h, 000h, 000h, 000h
        push   ecx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_294C:
        add    esp, 10h
        add    esi, eax
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, esi
        sub    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi + 38h], esi
        inc    ax
        sub    esi, edi
        sub    si, 3ch
        pop    edi
        add    word ptr [ebx + 1bh], ax
        mov    word ptr [ebx + 3], si
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_2973:
        mov    edi, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 8], 0
        mov    edx, dword ptr [edi + 38h]
        je     SHORT loc_6_299B
        mov    esi, dword ptr [ebp - 8]
        add    esi, 30h
        cmp    byte ptr [esi], 0
        je     SHORT loc_6_299B
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, ecx
        jmp    SHORT loc_6_29B2
loc_6_299B:
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, ecx
        db  0BEh, 000h, 000h, 000h, 000h
loc_6_29B2:
        shr    ecx, 2
        mov    edi, edx
        rep movsd
        mov    ecx, eax
        add    edx, eax
        and    ecx, 3
        rep movsb
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, edx
        mov    ecx, 6
        add    edx, 18h
        rep movsd
        add    ax, 18h
        mov    edi, dword ptr [ebp + 0ch]
        add    word ptr [ebx + 1bh], ax
        mov    dword ptr [edi + 38h], edx
        sub    edx, edi
        pop    edi
        sub    dx, 3ch
        pop    esi
        mov    word ptr [ebx + 3], dx
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_2A00:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        add    eax, 5ch
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [eax], 0
        mov    byte ptr [ecx + 40h], 70h
        push   ebx
        push   esi
        push   edi
        lea    ebx, [eax + 3]
        inc    ebx
        mov    edi, dword ptr [ebp + 0ch]
        add    edi, 2ch
        mov    ecx, 0ffffffffh
        mov    byte ptr [ebx - 1], 4
        mov    word ptr [ebx], 5c5ch
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        lea    edi, [ebx + 2]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        mov    esi, dword ptr [ebp + 10h]
        push   1
        add    esi, 7bh
        push   2eh
        not    ecx
        push   esi
        lea    ebx, [ecx + ebx - 1]
        mov    byte ptr [ebx], 5ch
        inc    ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2A75:
        add    esp, 4
        add    eax, eax
        mov    edi, ebx
        push   eax
        push   esi
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2A84:
        add    esp, 14h
        mov    ecx, 0ffffffffh
        mov    byte ptr [ebx + eax], 0
        sub    eax, eax
        repne scasb
        not    ecx
        add    ebx, ecx
        mov    eax, dword ptr [ebp + 10h]
        inc    ebx
        test   byte ptr [eax + 1ch], 20h
        mov    byte ptr [ebx - 1], 4
        mov    byte ptr [ebx], 0
        je     SHORT loc_6_2ACB
        lea    edi, [eax + 35h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, ebx
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_6_2ACB:
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        add    ebx, ecx
        mov    eax, dword ptr [ebp + 10h]
        inc    ebx
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 14h]
        sub    eax, eax
        lea    edx, [ecx + ecx*2]
        mov    ecx, 0ffffffffh
        mov    byte ptr [ebx - 1], 4
        db  08Dh, 03Ch, 055h, 000h, 000h, 000h, 000h
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, ebx
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        mov    eax, dword ptr [ebp + 8]
        lea    ecx, [ecx + ebx - 1]
        inc    ecx
        pop    edi
        pop    esi
        mov    dword ptr [eax + 38h], ecx
        sub    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        sub    cx, 3
        pop    ebx
        mov    word ptr [eax + 1], cx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_6_2B40:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ecx, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 0ch], ecx
        mov    dword ptr [ebp - 18h], eax
        mov    edx, dword ptr [ecx + 10h]
        mov    byte ptr [ebp - 1], al
        cmp    byte ptr [esi + 5], 3
        mov    dword ptr [ebp - 10h], edx
        jb     SHORT loc_6_2B7C
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, 5
        pop    edi
        pop    esi
        mov    dword ptr [ebx + 20h], 5
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_2B7C:
        mov    edx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [edx + 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2B88:
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp - 10h]
        and    word ptr [ebx + 2], 0bfffh
        test   byte ptr [edx + 1dh], 80h
        je     SHORT loc_6_2BCF
        push   ebx
        push   edx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2BA5:
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_6_2BCF
        mov    eax, dword ptr [ebx + 20h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 26h], ax
        mov    edx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [edx + 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2BC2:
        add    esp, 4
        mov    eax, dword ptr [ebx + 20h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_2BCF:
        xor    eax, eax
        mov    edx, dword ptr [ebp - 10h]
        mov    al, byte ptr [esi + 5]
        cmp    word ptr [edx + eax*2 + 7fh], 0ffffh
        jne    NEAR PTR loc_6_2C6C
        push   ebx
        mov    eax, dword ptr [edx + 76h]
        call   dword ptr [eax + 44h]
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_6_2C15
        mov    eax, dword ptr [ebx + 20h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    word ptr [ecx + 26h], ax
        mov    edx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [edx + 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2C08:
        add    esp, 4
        mov    eax, dword ptr [ebx + 20h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_2C15:
        mov    byte ptr [ebx + 40h], 73h
        mov    edx, dword ptr [ebp - 10h]
        or     byte ptr [ebx + 45h], 10h
        mov    dx, word ptr [edx + 1ah]
        test   dh, 2
        je     SHORT loc_6_2C3B
        test   dl, 80h
        je     SHORT loc_6_2C3B
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_2C3B
        or     byte ptr [ebx + 47h], 80h
loc_6_2C3B:
        mov    edi, dword ptr [ebx + 38h]
        push   eax
        test   dh, 1
        movzx  cx, byte ptr [esi + 5]
        mov    word ptr [ebx + 58h], cx
        mov    ecx, edi
        je     SHORT loc_6_2C5D
        push   edi
        mov    edx, dword ptr [ebp - 10h]
        push   edx
        push   ebx
        push   esi
        call   sub_6_2630
        jmp    SHORT loc_6_2C69
loc_6_2C5D:
        push   ecx
        mov    edx, dword ptr [ebp - 10h]
        push   edx
        push   ebx
        push   esi
        call   sub_6_2440
loc_6_2C69:
        add    esp, 14h
loc_6_2C6C:
        mov    ecx, dword ptr [ebp - 0ch]
        test   byte ptr [ecx + 1ch], 80h
        je     NEAR PTR loc_6_2F99
        mov    dword ptr [ecx + 45h], 0
        mov    eax, dword ptr [ecx + 69h]
        test   eax, eax
        je     SHORT loc_6_2C9A
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2C8D:
        add    esp, 4
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 69h], 0
loc_6_2C9A:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [ecx + 72h]
        test   eax, eax
        je     SHORT loc_6_2CB7
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2CAA:
        add    esp, 4
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 72h], 0
loc_6_2CB7:
        mov    eax, 0ffffffh
        mov    ecx, dword ptr [ebp - 0ch]
        xor    esi, esi
        mov    edx, dword ptr [ebp - 10h]
        mov    dword ptr [ecx + 61h], eax
        mov    dword ptr [ecx + 59h], eax
        mov    dword ptr [ecx + 65h], eax
        mov    dword ptr [ecx + 6dh], esi
        mov    ax, word ptr [edx + 1ah]
        mov    dword ptr [ecx + 5dh], esi
        test   ah, 2
        je     SHORT loc_6_2CEC
        test   al, 80h
        je     SHORT loc_6_2CEC
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_2CEC
        or     byte ptr [ebx + 47h], 80h
loc_6_2CEC:
        mov    ecx, dword ptr [ebp - 0ch]
        test   al, 20h
        mov    word ptr [ecx + 1eh], 0ffffh
        je     NEAR PTR loc_6_2F77
        lea    edx, [ebx + 5ch]
        mov    eax, dword ptr [ebx + 38h]
        mov    ecx, eax
        mov    dword ptr [ebp - 14h], edx
        sub    ecx, ebx
        cmp    ecx, 5ch
        jne    SHORT loc_6_2D15
        mov    byte ptr [ebx + 40h], 75h
        jmp    SHORT loc_6_2D1F
loc_6_2D15:
        mov    edx, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 14h], eax
        mov    byte ptr [edx + 1], 75h
loc_6_2D1F:
        mov    edx, dword ptr [ebp - 14h]
        xor    ecx, ecx
        mov    word ptr [edx + 3], cx
        mov    esi, dword ptr [ebp - 10h]
        mov    word ptr [edx + 1], 0ffh
        mov    al, byte ptr [esi + 19h]
        mov    byte ptr [edx], 4
        mov    word ptr [edx + 5], 2
        test   al, 1
        je     SHORT loc_6_2D55
        mov    word ptr [edx + 7], 1
        add    edx, 7
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 8], edx
        mov    byte ptr [eax + 0bh], cl
        jmp    SHORT loc_6_2DD1
loc_6_2D55:
        test   al, 2
        je     SHORT loc_6_2D8D
        mov    ecx, dword ptr [ebp - 0ch]
        test   byte ptr [ecx + 1ch], 20h
        je     SHORT loc_6_2D8D
        mov    edx, dword ptr [ebp - 14h]
        lea    eax, [ecx + 35h]
        mov    word ptr [edx + 7], 18h
        add    edx, 7
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 8], edx
        add    ecx, 0bh
        push   ecx
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 6ah
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2D88:
        add    esp, 0ch
        jmp    SHORT loc_6_2DD1
loc_6_2D8D:
        mov    edx, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 0ch]
        add    edx, 7
        add    esi, 35h
        mov    edi, esi
        mov    ecx, 0ffffffffh
        sub    eax, eax
        mov    dword ptr [ebp - 8], edx
        repne scasb
        mov    edi, esi
        sub    eax, eax
        not    ecx
        mov    word ptr [edx], cx
        mov    ecx, 0ffffffffh
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp - 14h]
        add    edi, 0bh
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
loc_6_2DD1:
        xor    eax, eax
        mov    edx, dword ptr [ebp - 8]
        mov    ax, word ptr [edx]
        mov    ecx, dword ptr [ebp - 14h]
        mov    esi, dword ptr [ebp - 10h]
        lea    edx, [eax + ecx + 0bh]
        mov    ax, word ptr [esi + 1ah]
        mov    dword ptr [ebp - 8], edx
        test   ah, 2
        je     NEAR PTR loc_6_2E96
        test   al, 80h
        je     NEAR PTR loc_6_2E96
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     NEAR PTR loc_6_2E96
        add    dword ptr [ebp - 8], 2
        mov    word ptr [edx], 5ch
        lea    ecx, [esi + 3ch]
        push   ecx
        mov    dword ptr [ebp - 1ch], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2E1D:
        add    esp, 4
        add    eax, eax
        mov    edi, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp - 1ch]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    ecx, dword ptr [ebp - 1ch]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2E3F:
        add    esp, 4
        mov    edx, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp - 0ch]
        lea    ecx, [edx + eax*2]
        mov    word ptr [ecx], 5ch
        add    esi, 7bh
        push   esi
        mov    dword ptr [ebp - 8], ecx
        add    dword ptr [ebp - 8], 2
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2E60:
        add    esp, 4
        lea    eax, [eax*2 + 2]
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [ebp - 8]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2E84:
        add    esp, 4
        mov    edx, dword ptr [ebp - 8]
        lea    ecx, [edx + eax*2 + 2]
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_6_2F1C
loc_6_2E96:
        mov    edx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 10h]
        mov    word ptr [edx], 5c5ch
        add    edi, 2ch
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        lea    edi, [edx + 2]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        mov    edx, dword ptr [ebp - 8]
        repne scasb
        not    ecx
        mov    esi, dword ptr [ebp - 0ch]
        push   1
        lea    eax, [ecx + edx - 1]
        add    esi, 7bh
        push   2eh
        mov    dword ptr [ebp - 8], eax
        push   esi
        mov    byte ptr [eax], 5ch
        inc    dword ptr [ebp - 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2EF2:
        add    esp, 4
        add    eax, eax
        mov    edx, dword ptr [ebp - 8]
        push   eax
        push   esi
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2F02:
        add    esp, 14h
        mov    edx, dword ptr [ebp - 8]
        mov    edi, edx
        mov    ecx, 0ffffffffh
        mov    byte ptr [edx + eax], 0
        sub    eax, eax
        repne scasb
        not    ecx
        add    dword ptr [ebp - 8], ecx
loc_6_2F1C:
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ecx + 14h]
        mov    ecx, 0ffffffffh
        lea    edx, [eax + eax*2]
        sub    eax, eax
        db  08Dh, 03Ch, 055h, 000h, 000h, 000h, 000h
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [ebp - 8]
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    edi, dword ptr [ebp - 8]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        mov    edx, dword ptr [ebp - 8]
        repne scasb
        not    ecx
        lea    eax, [ecx + edx - 1]
        mov    edx, dword ptr [ebp - 14h]
        inc    eax
        mov    dword ptr [ebx + 38h], eax
        sub    eax, dword ptr [ebp - 14h]
        sub    ax, 0bh
        mov    word ptr [edx + 9], ax
        jmp    SHORT loc_6_2F99
loc_6_2F77:
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    byte ptr [ecx + 14h], 0
        jne    SHORT loc_6_2F88
        mov    byte ptr [ebp - 1], 1
        mov    byte ptr [ecx + 14h], 1
loc_6_2F88:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 10h]
        push   ecx
        push   edx
        push   ebx
        call   sub_6_2A00
        add    esp, 0ch
loc_6_2F99:
        mov    eax, dword ptr [ebx + 38h]
        sub    eax, ebx
        cmp    eax, 5ch
        je     NEAR PTR loc_6_3098
        mov    esi, 20h
loc_6_2FAC:
        push   ebx
        call   sub_6_0EA0
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
        test   eax, eax
        jne    SHORT loc_6_2FC8
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_2FC2:
        add    esp, 4
        mov    dword ptr [ebp - 18h], eax
loc_6_2FC8:
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_6_3098
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ax, word ptr [ecx + 1eh]
        cmp    ax, 0ffffh
        je     SHORT loc_6_3015
        mov    word ptr [ebx + 54h], ax
        push   ebx
        mov    byte ptr [ebx + 40h], 71h
        lea    eax, [ebx + 5ch]
        mov    word ptr [eax + 1], 0
        mov    byte ptr [eax], 0
        add    eax, 3
        mov    dword ptr [ebx + 38h], eax
        call   sub_6_0EA0
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_6_300F
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_300C:
        add    esp, 4
loc_6_300F:
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebx + 20h], eax
loc_6_3015:
        mov    edx, dword ptr [ebp - 10h]
        test   word ptr [edx + 1ah], si
        jne    SHORT loc_6_3059
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_6_3059
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    byte ptr [ecx + 14h], 1
        jne    SHORT loc_6_3059
        cmp    dword ptr [ebp - 18h], 42h
        jne    SHORT loc_6_305F
        mov    byte ptr [ecx + 14h], 2
        mov    dword ptr [ebx + 20h], 0
        push   ecx
        push   edx
        push   ebx
        call   sub_6_2A00
        add    esp, 0ch
        mov    eax, dword ptr [ebx + 38h]
        sub    eax, ebx
        cmp    eax, 5ch
        jne    NEAR PTR loc_6_2FAC
        jmp    SHORT loc_6_3098
loc_6_3059:
        cmp    dword ptr [ebp - 18h], 42h
        je     SHORT loc_6_3065
loc_6_305F:
        cmp    dword ptr [ebp - 18h], 43h
        jne    SHORT loc_6_3087
loc_6_3065:
        mov    edx, dword ptr [ebp - 10h]
        test   byte ptr [edx + 1ah], 20h
        jne    SHORT loc_6_3087
        mov    ecx, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ecx + 14h]
        cmp    al, 4
        je     SHORT loc_6_307C
        cmp    al, 3
        jne    SHORT loc_6_3087
loc_6_307C:
        mov    eax, 32h
        mov    dword ptr [ebx + 20h], eax
        mov    dword ptr [ebp - 18h], eax
loc_6_3087:
        mov    ecx, dword ptr [ebp - 0ch]
        test   byte ptr [ecx + 1ch], 80h
        je     SHORT loc_6_3098
        mov    ax, word ptr [ebp - 18h]
        mov    word ptr [ecx + 26h], ax
loc_6_3098:
        mov    edx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [edx + 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_30A4:
        add    esp, 4
        mov    eax, dword ptr [ebp - 18h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_30C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_30D5:
        add    esp, 4
        test   byte ptr [esi + 1ah], 20h
        je     SHORT loc_6_3137
        mov    edx, 80h
        test   byte ptr [esi + 1dh], dl
        jne    SHORT loc_6_3137
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        lea    ecx, [esi + eax*2 + 7fh]
        mov    bx, word ptr [ecx]
        mov    dword ptr [ebp - 4], ecx
        cmp    bx, 0ffffh
        je     SHORT loc_6_3137
        mov    al, byte ptr [esi + 7eh]
        dec    al
        mov    byte ptr [esi + 7eh], al
        je     SHORT loc_6_314A
        mov    eax, dword ptr [esi + 0ch]
        mov    edi, dword ptr [ebp + 8]
        test   eax, eax
        mov    dword ptr [edi + 1ch], eax
        je     SHORT loc_6_314A
        test   word ptr [eax + 1ch], dx
        mov    word ptr [ecx], 0ffffh
        jne    SHORT loc_6_3137
        movzx  eax, bx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   974h
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3134:
        add    esp, 10h
loc_6_3137:
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3140:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_314A:
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, eax
        test   ecx, ecx
        je     NEAR PTR loc_6_3212
        mov    edx, 4
loc_6_315C:
        test   word ptr [ecx + 1ch], dx
        je     SHORT loc_6_3167
        cmp    byte ptr [ecx + 14h], dl
        je     SHORT loc_6_3173
loc_6_3167:
        mov    ecx, dword ptr [ecx + 4]
        cmp    eax, ecx
        jne    SHORT loc_6_315C
        jmp    NEAR PTR loc_6_3212
loc_6_3173:
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [edi + 1ch], ecx
        xor    ecx, ecx
        cmp    byte ptr [esi + 3], cl
        jbe    SHORT loc_6_31F7
        mov    byte ptr [esi + 2ch], cl
        mov    dword ptr [ebp - 8], ecx
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_6_31F7
        db  0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h
loc_6_319B:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ebx, dword ptr [eax]
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_31A5:
        lea    edi, [eax + 1388h]
        test   byte ptr [ebx + 2], 1
        je     SHORT loc_6_31E5
loc_6_31B1:
        cmp    dword ptr [ebx + 10h], esi
        jne    SHORT loc_6_31E5
        db  081h, 07Bh, 024h, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_31E5
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_31C4:
        cmp    eax, edi
        ja     SHORT loc_6_31D5
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_31CD:
        test   byte ptr [ebx + 2], 1
        jne    SHORT loc_6_31B1
        jmp    SHORT loc_6_31E5
loc_6_31D5:
        mov    dword ptr [ebx + 20h], 43h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_31E2:
        add    esp, 4
loc_6_31E5:
        add    dword ptr [ebp - 0ch], 4
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        jb     SHORT loc_6_319B
loc_6_31F7:
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_6_3290
        add    esp, 4
        mov    eax, dword ptr [ebp - 10h]
        mov    edi, dword ptr [ebp + 8]
        mov    word ptr [edi + 1ah], 0
        mov    dword ptr [edi + 1ch], eax
loc_6_3212:
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx], 0ffffh
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3223:
        add    esp, 4
        mov    edi, dword ptr [ebp + 8]
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3230:
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_6_3240:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 2]
        mov    ebp, esp
        and    cx, 0fffeh
        push   esi
        mov    word ptr [eax + 2], cx
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [eax + 1ch]
        test   cl, 4
        je     SHORT loc_6_3265
        mov    esi, 1
loc_6_3265:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_326B:
        add    esp, 4
        mov    eax, dword ptr [edi + 1ch]
        push   eax
        push   0
        call   sub_6_3390
        add    esp, 8
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3282:
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_6_3290:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 1ch]
        test   byte ptr [edi + 1ch], 8
        mov    edx, dword ptr [edi + 10h]
        mov    dword ptr [ebp - 8], edx
        je     NEAR PTR loc_6_337E
        mov    cx, word ptr [edi + 7bh]
        mov    word ptr [ebp - 2], cx
        test   cx, cx
        je     NEAR PTR loc_6_337E
        mov    word ptr [edi + 7bh], 0
        push   edi
        call   sub_6_12D0
        add    esp, 4
        push   esi
        call   sub_6_0040
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_6_336F
        test   byte ptr [edi + 1ch], 80h
        jne    SHORT loc_6_335C
        mov    bl, byte ptr [esi + 5]
loc_6_32E9:
        mov    al, byte ptr [esi + 5]
        xor    ecx, ecx
        mov    cl, al
        mov    edx, dword ptr [ebp - 8]
        cmp    word ptr [edx + ecx*2 + 7fh], 0ffffh
        jne    SHORT loc_6_330E
        inc    al
        mov    byte ptr [esi + 5], al
        cmp    al, 3
        jb     SHORT loc_6_3309
        mov    byte ptr [esi + 5], 0
loc_6_3309:
        cmp    byte ptr [esi + 5], bl
        jne    SHORT loc_6_32E9
loc_6_330E:
        cmp    dword ptr [esi + 30h], 0
        jne    SHORT loc_6_332C
        db  068h, 000h, 000h, 000h, 000h
        push   871h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3324:
        add    esp, 0ch
        mov    byte ptr [esi + 5], bl
        jmp    SHORT loc_6_335C
loc_6_332C:
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   871h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3341:
        add    esp, 10h
        mov    byte ptr [esi + 5], bl
        test   eax, eax
        jne    SHORT loc_6_3357
        mov    eax, 0ffffffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_3357:
        cmp    eax, -3
        je     SHORT loc_6_336F
loc_6_335C:
        push   edi
        push   esi
        call   sub_6_3390
        add    esp, 8
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_336F:
        mov    cx, word ptr [ebp - 2]
        mov    word ptr [edi + 7bh], cx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_337E:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_3390:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [edi + 10h]
        mov    ecx, dword ptr [edi + 4]
        cmp    ecx, edi
        jne    SHORT loc_6_33B1
        mov    dword ptr [esi + 0ch], 0
        jmp    SHORT loc_6_33C2
loc_6_33B1:
        mov    eax, dword ptr [edi + 8]
        cmp    dword ptr [esi + 0ch], edi
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [eax + 4], ecx
        jne    SHORT loc_6_33C2
        mov    dword ptr [esi + 0ch], ecx
loc_6_33C2:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        test   ax, ax
        je     SHORT loc_6_33D5
        dec    ax
        db  066h, 0A3h, 000h, 000h, 000h, 000h
loc_6_33D5:
        mov    eax, dword ptr [edi + 18h]
        test   eax, eax
        je     SHORT loc_6_33E5
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_33E2:
        add    esp, 4
loc_6_33E5:
        mov    eax, dword ptr [edi + 69h]
        test   eax, eax
        je     SHORT loc_6_33FC
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_33F2:
        mov    dword ptr [edi + 69h], 0
        add    esp, 4
loc_6_33FC:
        mov    eax, dword ptr [edi + 72h]
        test   eax, eax
        je     SHORT loc_6_3413
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3409:
        mov    dword ptr [edi + 72h], 0
        add    esp, 4
loc_6_3413:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3419:
        add    esp, 4
        mov    ecx, dword ptr [esi + 0ch]
        mov    eax, ecx
        test   eax, eax
        je     SHORT loc_6_343D
        mov    bl, 1
        mov    edx, 80h
loc_6_342C:
        test   word ptr [eax + 1ch], dx
        je     SHORT loc_6_343B
        mov    eax, dword ptr [eax + 4]
        cmp    eax, ecx
        jne    SHORT loc_6_342C
        jmp    SHORT loc_6_343D
loc_6_343B:
        xor    bl, bl
loc_6_343D:
        test   byte ptr [esi + 1dh], 80h
        jne    SHORT loc_6_345E
        test   ecx, ecx
        je     SHORT loc_6_344B
        test   bl, bl
        je     SHORT loc_6_345E
loc_6_344B:
        cmp    byte ptr [esi + 2], 0
        jne    SHORT loc_6_345E
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_345B:
        add    esp, 8
loc_6_345E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_3470:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3481:
        add    esp, 4
        push   esi
        push   0
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_348C:
        add    esp, 8
        xor    eax, eax
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_34A0:
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_34AA:
        add    esp, 4
        ret
        db  08Bh, 0FFh
sub_6_34B0:
        push   ebp
        mov    ebp, esp
        push   1
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_34BA:
        add    esp, 4
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   3e8h
        db  068h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_34D9:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_6_34E0:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_34F1:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_6_3502
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3502:
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_3510:
        xor    eax, eax
        ret
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_3520:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 10h]
        mov    ax, word ptr [ecx + 1ah]
        mov    edx, dword ptr [ecx + 76h]
        or     al, 80h
        test   byte ptr [edx + 3], 2
        mov    word ptr [ecx + 1ah], ax
        je     SHORT loc_6_3544
        or     ah, 8
        mov    word ptr [ecx + 1ah], ax
loc_6_3544:
        db  0A1h, 000h, 000h, 000h, 000h
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_6_3550:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        test   byte ptr [ecx + 1dh], 40h
        push   esi
        push   edi
        je     SHORT loc_6_358B
        xor    esi, esi
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        cmp    edx, esi
        je     SHORT loc_6_358B
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ebx, 1
loc_6_3576:
        mov    eax, dword ptr [edi]
        test   word ptr [eax + 2], bx
        je     SHORT loc_6_3583
        cmp    dword ptr [eax + 10h], ecx
        je     SHORT loc_6_358D
loc_6_3583:
        add    edi, 4
        inc    esi
        cmp    edx, esi
        ja     SHORT loc_6_3576
loc_6_358B:
        xor    eax, eax
loc_6_358D:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_35A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 44h]
        dec    byte ptr [edi + 5bh]
        cmp    byte ptr [esi], 96h
        jne    SHORT loc_6_35BA
        dec    byte ptr [edi + 5ch]
loc_6_35BA:
        mov    al, byte ptr [esi + 1]
        test   al, al
        je     SHORT loc_6_35CF
        sub    al, 6
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        push   eax
        jmp    NEAR PTR loc_6_370B
loc_6_35CF:
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 22h
        jb     NEAR PTR loc_6_36DC
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 1eh]
        cmp    ax, 0ffffh
        jne    SHORT loc_6_360F
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_35FE:
        add    esp, 8
        push   eax
        call   sub_6_1940
        add    esp, 8
        jmp    NEAR PTR loc_6_371A
loc_6_360F:
        test   ax, ax
        jne    SHORT loc_6_3629
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        call   sub_6_3750
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_6_3709
loc_6_3629:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax], 424d53ffh
        jne    NEAR PTR loc_6_3709
        mov    cx, word ptr [eax + 1eh]
        xor    eax, eax
        mov    ax, cx
        and    eax, 3fh
        je     NEAR PTR loc_6_3709
        db  08Bh, 01Ch, 085h, 0FCh, 0FFh, 0FFh, 0FFh
        test   ebx, ebx
        je     NEAR PTR loc_6_3709
        cmp    word ptr [ebx], cx
        jne    NEAR PTR loc_6_371A
        cmp    dword ptr [ebx + 10h], 0
        je     NEAR PTR loc_6_371A
        test   byte ptr [ebx + 2], 1
        je     NEAR PTR loc_6_371A
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 4]
        push   eax
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], 0
        push   ecx
        push   edx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_368F:
        add    esp, 10h
        test   eax, eax
        je     NEAR PTR loc_6_371A
        mov    ecx, dword ptr [ebx + 30h]
        test   ecx, ecx
        je     SHORT loc_6_36D1
        cmp    dword ptr [ebx + 34h], 0
        je     SHORT loc_6_36D1
        xor    eax, eax
        mov    edx, dword ptr [ebp - 8]
        mov    ax, word ptr [esi + 8]
        sub    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        push   ecx
        call   sub_6_D670
        add    esp, 10h
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        sub    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebx + 34h], eax
loc_6_36D1:
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_36D7:
        add    esp, 4
        jmp    SHORT loc_6_371A
loc_6_36DC:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_6_3709
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_36EE:
        add    esp, 8
        push   eax
        call   sub_6_3550
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_3709
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3704:
        add    esp, 4
        jmp    SHORT loc_6_371A
loc_6_3709:
        push   1
loc_6_370B:
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        push   eax
        push   edi
        call   sub_6_34E0
        add    esp, 0ch
loc_6_371A:
        mov    eax, dword ptr [esi + 4ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3723:
        add    esp, 4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_372C:
        add    esp, 4
        cmp    byte ptr [edi + 5ch], 0
        je     SHORT loc_6_373D
        mov    al, byte ptr [edi + 5bh]
        cmp    byte ptr [edi + 5dh], al
        jbe    SHORT loc_6_3746
loc_6_373D:
        push   edi
        call   sub_6_4190
        add    esp, 4
loc_6_3746:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h
sub_6_3750:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_6_3795
        db  0BEh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
loc_6_3768:
        mov    edx, dword ptr [esi]
        cmp    dword ptr [edx + 10h], 0
        je     SHORT loc_6_3789
        mov    edi, dword ptr [edx + 10h]
        mov    bl, byte ptr [eax + 30h]
        mov    edi, dword ptr [edi + 76h]
        cmp    byte ptr [edi + 5ah], bl
        jne    SHORT loc_6_3789
        mov    edi, dword ptr [edx + 10h]
        mov    bl, byte ptr [edi + 7ah]
        cmp    byte ptr [eax + 2], bl
        je     SHORT loc_6_379F
loc_6_3789:
        add    esi, 4
        inc    ecx
        db  03Bh, 00Dh, 000h, 000h, 000h, 000h
        jb     SHORT loc_6_3768
loc_6_3795:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_6_379F:
        mov    cx, word ptr [edx]
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 1eh], cx
        pop    edi
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_6_37B0:
        push   ebp
        mov    ebp, esp
        cli
        db  0A1h, 000h, 000h, 000h, 000h
        xor    edx, edx
        test   eax, eax
        je     SHORT loc_6_37E6
        mov    ecx, dword ptr [ebp + 8]
loc_6_37C2:
        cmp    eax, ecx
        je     SHORT loc_6_37CF
        mov    edx, eax
        mov    eax, dword ptr [eax + 4ch]
        test   eax, eax
        jne    SHORT loc_6_37C2
loc_6_37CF:
        test   eax, eax
        je     SHORT loc_6_37E6
        mov    ecx, dword ptr [eax + 4ch]
        test   edx, edx
        je     SHORT loc_6_37E0
        mov    dword ptr [edx + 4ch], ecx
        sti
        pop    ebp
        ret
loc_6_37E0:
        db  089h, 00Dh, 000h, 000h, 000h, 000h
loc_6_37E6:
        sti
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_37F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        xor    eax, eax
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 44h]
        mov    al, byte ptr [edi]
        sub    eax, 90h
        mov    dword ptr [ebp - 8], ecx
        cmp    eax, 26h
        ja     NEAR PTR loc_6_3AB2
        xor    ecx, ecx
        db  08Ah, 088h, 000h, 000h, 000h, 000h
        db  0FFh, 024h, 08Dh, 000h, 000h, 000h, 000h
loc_6_3823:
        mov    ebx, dword ptr [edi + 40h]
        mov    cl, byte ptr [edi + 2]
        push   edi
        mov    byte ptr [ebp - 1], cl
        call   sub_6_37B0
        add    esp, 4
        mov    ecx, dword ptr [ebp - 8]
        dec    byte ptr [ebx + 2]
        mov    al, byte ptr [ecx + 5eh]
        not    al
        push   edi
        and    byte ptr [ebx + 7bh], al
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3849:
        add    esp, 4
        xor    eax, eax
        mov    al, byte ptr [edi + 1]
        test   eax, eax
        je     SHORT loc_6_3898
        cmp    eax, 12h
        je     SHORT loc_6_3866
        cmp    eax, 14h
        je     SHORT loc_6_387E
        cmp    word ptr [ebx + 1eh], 0
        jne    SHORT loc_6_387E
loc_6_3866:
        cmp    dword ptr [ebx + 76h], 0
        jne    SHORT loc_6_387E
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_6_0EE0
        mov    word ptr [ebx + 1eh], ax
        add    esp, 8
loc_6_387E:
        cmp    byte ptr [ebx + 7bh], 0
        jne    NEAR PTR loc_6_3AB2
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_388E:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_3898:
        mov    edx, dword ptr [ebx + 76h]
        test   edx, edx
        je     SHORT loc_6_38BB
        mov    al, byte ptr [ebx + 7ch]
        mov    ecx, dword ptr [ebp - 8]
        cmp    byte ptr [ecx + 5fh], al
        jb     SHORT loc_6_38BB
        push   0
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        push   eax
        push   ecx
        call   dword ptr [ecx + 2ch]
        add    esp, 0ch
        jmp    SHORT loc_6_387E
loc_6_38BB:
        mov    al, byte ptr [ebx + 7ah]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 2], al
        mov    eax, dword ptr [ebx + 72h]
        mov    dword ptr [ebx + 72h], 0
        mov    cl, byte ptr [ecx + 5fh]
        test   edx, edx
        mov    byte ptr [ebx + 7ch], cl
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebx + 76h], ecx
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [ebx + 7ah], cl
        je     SHORT loc_6_38F2
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        push   eax
        push   edx
        call   dword ptr [edx + 2ch]
        add    esp, 0ch
loc_6_38F2:
        lea    eax, [edi + 19h]
        cmp    byte ptr [eax], 20h
        jne    SHORT loc_6_3900
loc_6_38FA:
        dec    eax
        cmp    byte ptr [eax], 20h
        je     SHORT loc_6_38FA
loc_6_3900:
        add    edi, 0ah
        mov    ecx, 0ffffffffh
        mov    byte ptr [eax + 1], 0
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        shr    ecx, 2
        mov    esi, edi
        lea    edi, [ebx + 2ch]
        rep movsd
        mov    ecx, edx
        push   1
        and    ecx, 3
        rep movsb
        lea    edi, [ebx + 2ch]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        lea    eax, [ebx + 2ch]
        lea    edx, [ebx + 3eh]
        not    ecx
        dec    ecx
        push   ecx
        push   eax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3946:
        add    esp, 10h
        lea    ecx, [ebp - 1ch]
        lea    edx, [ebx + 2ch]
        sub    eax, eax
        push   ecx
        mov    edi, edx
        push   edx
        mov    ecx, 0ffffffffh
        mov    word ptr [ebx + 3ch], 5ch
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        call   sub_6_D930
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 1ch], 0
        mov    dl, byte ptr [ecx + 5ah]
        lea    eax, [ebp - 1ch]
        push   eax
        lea    ecx, [ebx + 2ch]
        push   ecx
        mov    edi, ecx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        mov    byte ptr [ebp - 1bh], dl
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        call   sub_6_D9A0
        add    esp, 0ch
        cmp    byte ptr [ebx + 7ch], 1
        ja     NEAR PTR loc_6_387E
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_39AA:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_39B4:
        push   0
        xor    eax, eax
        mov    al, byte ptr [edi + 2]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_6_34E0
        add    esp, 0ch
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_39CE:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_39D8:
        mov    bl, byte ptr [edi + 40h]
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
        cmp    bl, 1
        ja     SHORT loc_6_3A52
        mov    dl, byte ptr [edi + 1]
        test   dl, dl
        jne    SHORT loc_6_3A1C
        xor    eax, eax
        mov    dl, byte ptr [edi + 3]
        mov    al, bl
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        mov    byte ptr [eax + ecx + 63h], dl
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3A02:
        add    esp, 8
        test   bl, bl
        jne    SHORT loc_6_3A7D
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ecx + 10h]
        mov    edx, dword ptr [eax]
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3A17:
        add    esp, 4
        jmp    SHORT loc_6_3A7D
loc_6_3A1C:
        mov    ecx, dword ptr [ebp - 8]
        cmp    word ptr [ecx + 58h], 0
        jne    SHORT loc_6_3A7D
        xor    eax, eax
        mov    al, dl
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_6_0EE0
        add    esp, 8
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 58h], ax
        cmp    ax, 34h
        jne    SHORT loc_6_3A7D
        cmp    bl, 1
        jne    SHORT loc_6_3A7D
        mov    word ptr [ecx + 58h], 17e5h
        jmp    SHORT loc_6_3A7D
loc_6_3A52:
        cmp    byte ptr [edi + 1], 0
        jne    SHORT loc_6_3A66
        mov    al, byte ptr [edi + 3]
        xor    edx, edx
        mov    dl, bl
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [edx + ecx + 63h], al
loc_6_3A66:
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [edi + 1]
        push   eax
        xor    eax, eax
        mov    al, bl
        push   eax
        push   ecx
        call   sub_6_C840
        add    esp, 0ch
loc_6_3A7D:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3A83:
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_3A8D:
        xor    eax, eax
        mov    ecx, dword ptr [edi + 40h]
        mov    al, byte ptr [edi + 1]
        mov    edx, dword ptr [ebp - 8]
        push   eax
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
        push   edx
        call   sub_6_C930
        add    esp, 0ch
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3AAF:
        add    esp, 4
loc_6_3AB2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 005h, 001h, 005h, 005h
        db  005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h
        db  005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 005h, 002h, 003h, 005h, 005h, 005h
        db  005h, 004h, 005h, 000h, 000h, 000h, 000h
loc_6_3B00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    edi, ebx
        push   eax
        and    edi, 7fh
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3B17:
        add    esp, 4
        mov    esi, eax
        and    ebx, 80h
        jne    SHORT loc_6_3B29
        mov    byte ptr [esi], 31h
        jmp    SHORT loc_6_3B33
loc_6_3B29:
        db  0C7h, 046h, 02Ch, 000h, 000h, 000h, 000h
        mov    byte ptr [esi], 0b1h
loc_6_3B33:
        db  08Bh, 004h, 0BDh, 000h, 000h, 000h, 000h
        lea    ecx, [esi + 1ah]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 8], edx
        push   esi
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, dword ptr [ebp + 8]
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 40h], edi
        mov    byte ptr [ecx + edi + 63h], 0ffh
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3B6A:
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_6_3B77
        test   ebx, ebx
        jne    SHORT loc_6_3B86
loc_6_3B77:
        push   esi
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3B83:
        add    esp, 4
loc_6_3B86:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 049h, 000h
loc_6_3B90:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, 0ffffffffh
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        add    edi, 2ch
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [ebp - 4], ecx
        cmp    ecx, 0fh
        jle    SHORT loc_6_3BBF
        mov    eax, 35h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_3BBF:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    byte ptr [eax + 63h], 0ffh
        jne    SHORT loc_6_3BE8
        db  0A0h, 000h, 000h, 000h, 000h
loc_6_3BCD:
        test   al, al
        je     NEAR PTR loc_6_3CB8
        call   sub_6_4220
        mov    eax, dword ptr [ebp + 0ch]
        cmp    byte ptr [eax + 63h], 0ffh
        db  0A0h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_3BCD
loc_6_3BE8:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3BF1:
        lea    ecx, [eax + 0ah]
        mov    byte ptr [eax], 90h
        add    esp, 4
        mov    ebx, eax
        mov    eax, 20202020h
        db  0C7h, 043h, 02Ch, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx], eax
        mov    edi, ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, eax
        shr    ecx, 2
        add    esi, 2ch
        rep movsd
        mov    ecx, eax
        db  0BAh, 000h, 000h, 000h, 000h
        and    ecx, 3
        rep movsb
        lea    ecx, [ebx + 1ah]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    esi, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], esi
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    edx, dword ptr [edx + 0ch]
        mov    dword ptr [ecx + 0ch], edx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    byte ptr [ebx + 2ah], 0
        mov    al, byte ptr [ecx + 4]
        mov    edx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 5eh]
        mov    byte ptr [ebx + 2bh], al
        mov    dword ptr [ebx + 40h], edx
        inc    byte ptr [edx + 2]
        mov    dword ptr [ebx + 4], 0
        or     byte ptr [edx + 7bh], cl
        cli
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 4ch], eax
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        sti
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3C83:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_6_3CAF
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        push   ebx
        mov    cl, byte ptr [edx + 5eh]
        not    cl
        dec    byte ptr [eax + 2]
        and    byte ptr [eax + 7bh], cl
        call   sub_6_37B0
        add    esp, 4
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3CAC:
        add    esp, 4
loc_6_3CAF:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_3CB8:
        mov    eax, 34h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_3CD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, 0ffffffffh
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        add    edi, 2ch
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [ebp - 4], ecx
        cmp    ecx, 0fh
        jle    SHORT loc_6_3CFF
        mov    eax, 35h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_3CFF:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    byte ptr [eax + 63h], 0ffh
        jne    SHORT loc_6_3D28
        db  0A0h, 000h, 000h, 000h, 000h
loc_6_3D0D:
        test   al, al
        je     NEAR PTR loc_6_3E16
        call   sub_6_4220
        mov    eax, dword ptr [ebp + 0ch]
        cmp    byte ptr [eax + 63h], 0ffh
        db  0A0h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_3D0D
loc_6_3D28:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3D31:
        lea    ecx, [eax + 0ah]
        mov    byte ptr [eax], 90h
        add    esp, 4
        mov    ebx, eax
        mov    eax, 20202020h
        db  0C7h, 043h, 02Ch, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx], eax
        mov    edi, ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, eax
        shr    ecx, 2
        add    esi, 2ch
        rep movsd
        mov    ecx, eax
        db  0BAh, 000h, 000h, 000h, 000h
        and    ecx, 3
        rep movsb
        mov    byte ptr [ebx + 0ah], 2ah
        lea    ecx, [ebx + 1ah]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [edx + 0ch]
        mov    al, byte ptr [eax + 4]
        mov    dword ptr [ecx + 0ch], edx
        xor    ecx, ecx
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ebx + 2ah], cl
        mov    byte ptr [ebx + 2bh], al
        mov    dword ptr [ebx + 4], ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx + 40h], edx
        inc    byte ptr [edx + 2]
        mov    dl, byte ptr [eax + 5eh]
        mov    eax, dword ptr [ebp + 8]
        or     byte ptr [eax + 7bh], dl
        cli
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebx + 4ch], edx
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        lea    edx, [eax + 66h]
        sti
        cmp    dword ptr [edx], ecx
        jne    SHORT loc_6_3DD2
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 5eh]
loc_6_3DD2:
        push   0
        push   0
        push   edx
        push   ecx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3DDE:
        xor    ecx, ecx
        mov    cl, al
        mov    dword ptr [ebp - 4], ecx
        test   ecx, ecx
        je     SHORT loc_6_3E0C
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        push   ebx
        mov    cl, byte ptr [edx + 5eh]
        not    cl
        dec    byte ptr [eax + 2]
        and    byte ptr [eax + 7bh], cl
        call   sub_6_37B0
        add    esp, 4
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3E09:
        add    esp, 4
loc_6_3E0C:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_3E16:
        mov    eax, 34h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_3E30:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cli
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_6_3E59
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
loc_6_3E46:
        cmp    dword ptr [esi + 40h], eax
        jne    SHORT loc_6_3E50
        cmp    dword ptr [esi + 44h], ecx
        je     SHORT loc_6_3E5C
loc_6_3E50:
        mov    esi, dword ptr [esi + 4ch]
        test   esi, esi
        jne    SHORT loc_6_3E46
        jmp    SHORT loc_6_3E5C
loc_6_3E59:
        mov    ecx, dword ptr [ebp + 0ch]
loc_6_3E5C:
        sti
        test   esi, esi
        je     SHORT loc_6_3E88
        push   ecx
        call   sub_6_4170
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_6_3E88
        push   edi
        mov    byte ptr [edi], 35h
        mov    dword ptr [edi + 4], esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3E7C:
        add    esp, 4
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3E85:
        add    esp, 4
loc_6_3E88:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Bh, 0FFh
loc_6_3E90:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3E9F:
        mov    byte ptr [eax], 92h
        add    esp, 4
        db  0C7h, 040h, 02Ch, 000h, 000h, 000h, 000h
        push   eax
        mov    edi, eax
        mov    al, byte ptr [ebp + 0ch]
        mov    dword ptr [edi + 48h], 0
        mov    byte ptr [edi + 2], al
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3EC1:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_6_3ED3
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3ED0:
        add    esp, 4
loc_6_3ED3:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_3EE0:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [eax + 76h]
        test   edi, edi
        jne    SHORT loc_6_3F00
        mov    eax, 37h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_3F00:
        mov    al, byte ptr [edi + 3]
        xor    ecx, ecx
        and    al, 1
        mov    esi, dword ptr [ebp + 8]
        mov    cl, al
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [esi + 14h], eax
        mov    ecx, dword ptr [ebp + 14h]
        test   eax, eax
        mov    dword ptr [esi + 18h], ecx
        je     SHORT loc_6_3F40
        cmp    byte ptr [edi + 5ch], 0
        jne    SHORT loc_6_3F40
        push   edi
        call   sub_6_4190
        add    esp, 4
        cmp    byte ptr [edi + 5bh], 0
        jne    SHORT loc_6_3F40
        mov    eax, 36h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_3F40:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_3F46:
        mov    byte ptr [eax], 94h
        add    esp, 4
        mov    dword ptr [eax + 48h], 0
        mov    ebx, eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    al, byte ptr [eax + 7ah]
        mov    byte ptr [ebx + 2], al
        mov    ax, word ptr [esi + 2]
        test   al, 2
        je     SHORT loc_6_3F76
        or     al, 20h
        mov    dword ptr [ebx + 48h], esi
        mov    dword ptr [ebp - 4], 0
        mov    word ptr [esi + 2], ax
loc_6_3F76:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_6_3FE8
        mov    cx, word ptr [ebp + 10h]
        lea    eax, [esi + 3ch]
        cmp    dword ptr [ebp + 14h], 0
        mov    word ptr [ebx + 8], cx
        mov    dword ptr [ebx + 4], eax
        je     SHORT loc_6_3FF9
        mov    eax, dword ptr [ebp + 14h]
        add    eax, dword ptr [ebp + 10h]
        cmp    eax, 480h
        ja     SHORT loc_6_3FCB
        add    cx, word ptr [ebp + 14h]
        mov    eax, dword ptr [ebp + 14h]
        mov    word ptr [ebx + 8], cx
        push   eax
        push   0
        mov    edx, dword ptr [esi + 28h]
        push   edx
        mov    eax, dword ptr [ebp + 10h]
        add    eax, esi
        add    eax, 3ch
        push   eax
        call   sub_6_D6A0
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_6_3FF9
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_3FCB:
        mov    ax, word ptr [ebp + 14h]
        mov    ecx, dword ptr [esi + 28h]
        mov    word ptr [ebx + 0ah], ax
        mov    byte ptr [ebx], 97h
        mov    dword ptr [ebp - 4], 0
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [ebx + 0ch], edx
        jmp    SHORT loc_6_3FF9
loc_6_3FE8:
        mov    ax, word ptr [ebp + 14h]
        mov    ecx, dword ptr [esi + 28h]
        mov    word ptr [ebx + 8], ax
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [ebx + 4], edx
loc_6_3FF9:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_6_40B4
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_400B:
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     NEAR PTR loc_6_40B4
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 4]
        mov    word ptr [ebx + 0ah], ax
        mov    dl, byte ptr [edi + 63h]
        mov    dword ptr [ebx + 4ch], esi
        mov    dword ptr [ebx + 0ch], ecx
        push   ebx
        mov    byte ptr [ebx + 3], dl
        mov    byte ptr [ebx], 0c8h
        inc    byte ptr [edi + 5bh]
        db  0C7h, 043h, 02Ch, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4041:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     NEAR PTR loc_6_40EA
        push   esi
        dec    byte ptr [edi + 5bh]
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4058:
        mov    dword ptr [ebx + 4ch], 0
        add    esp, 4
        cmp    dword ptr [ebp - 4], 3
        je     SHORT loc_6_409C
        cmp    byte ptr [ebx + 1], 3
        je     SHORT loc_6_409C
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4074:
        add    esp, 4
        cmp    dword ptr [ebp - 4], 37h
        jne    SHORT loc_6_4092
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4088:
        mov    dword ptr [ebp - 4], 0
        add    esp, 8
loc_6_4092:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_6_409C:
        db  0A0h, 000h, 000h, 000h, 000h
        push   edi
        mov    byte ptr [edi + 5dh], al
        mov    byte ptr [ebx], 94h
        and    byte ptr [edi + 3], 0feh
        call   sub_6_4190
        add    esp, 4
loc_6_40B4:
        db  0C7h, 043h, 02Ch, 000h, 000h, 000h, 000h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_40C1:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_6_40E8
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_40D0:
        add    esp, 4
        cmp    esi, 37h
        jne    SHORT loc_6_40E8
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        xor    esi, esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_40E5:
        add    esp, 8
loc_6_40E8:
        mov    eax, esi
loc_6_40EA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_6_4100:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [edi + 76h]
        test   eax, eax
        jne    SHORT loc_6_4118
        mov    eax, 37h
        pop    edi
        pop    esi
        pop    ebp
        ret
loc_6_4118:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_411E:
        mov    byte ptr [eax], 95h
        add    esp, 4
        mov    esi, eax
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [esi + 8], cx
        mov    al, byte ptr [edi + 7ah]
        mov    edx, dword ptr [ebp + 10h]
        mov    byte ptr [esi + 2], al
        mov    eax, dword ptr [ebp + 8]
        push   esi
        db  0C7h, 046h, 02Ch, 000h, 000h, 000h, 000h
        or     byte ptr [eax + 2], 20h
        mov    dword ptr [esi + 48h], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4153:
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_6_4165
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4162:
        add    esp, 4
loc_6_4165:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_6_4170:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    dword ptr [eax + 0ch], 0
        jne    SHORT loc_6_4181
        xor    eax, eax
        pop    ebp
        ret
loc_6_4181:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4187:
        mov    esp, ebp
        pop    ebp
        ret
        db  005h, 000h, 000h, 000h, 000h
sub_6_4190:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
loc_6_4199:
        cmp    byte ptr [edi + 5ch], 0
        je     SHORT loc_6_41A7
        mov    al, byte ptr [edi + 5bh]
        cmp    byte ptr [edi + 5dh], al
        jbe    SHORT loc_6_4214
loc_6_41A7:
        cmp    byte ptr [edi + 63h], 0ffh
        je     SHORT loc_6_4214
        push   0
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_41B5:
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_6_4214
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_41C4:
        mov    dword ptr [eax + 4ch], esi
        mov    byte ptr [eax], 96h
        add    esp, 4
        mov    ebx, eax
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi + 8]
        mov    word ptr [ebx + 8], cx
        mov    dl, byte ptr [edi + 63h]
        push   ebx
        mov    dword ptr [ebx + 4], eax
        mov    byte ptr [ebx + 3], dl
        inc    byte ptr [edi + 5bh]
        db  0C7h, 043h, 02Ch, 000h, 000h, 000h, 000h
        inc    byte ptr [edi + 5ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_41F5:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_6_4199
        push   esi
        dec    byte ptr [edi + 5bh]
        dec    byte ptr [edi + 5ch]
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4208:
        add    esp, 4
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4211:
        add    esp, 4
loc_6_4214:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_4220:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_6_4237
loc_6_4229:
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_422E:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_4229
loc_6_4237:
        ret
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_6_4240:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_424F:
        mov    byte ptr [eax], 0b0h
        add    esp, 4
        mov    esi, eax
        mov    ecx, dword ptr [ebp + 0ch]
        test   cl, 5
        je     SHORT loc_6_4262
        mov    byte ptr [esi], 0b6h
loc_6_4262:
        db  08Bh, 03Ch, 08Dh, 000h, 000h, 000h, 000h
        lea    edx, [esi + 1ah]
        mov    eax, dword ptr [edi]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [edx + 4], eax
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [edi + 8]
        push   esi
        mov    edi, dword ptr [edi + 0ch]
        mov    dword ptr [edx + 8], eax
        mov    dword ptr [edx + 0ch], edi
        db  0C7h, 046h, 02Ch, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 40h], ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4298:
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_6_42B0
        push   esi
        db  0FEh, 00Dh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_42AD:
        add    esp, 4
loc_6_42B0:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_6_42C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_6_CA40
        add    esp, 4
        push   esi
        call   sub_6_BBC0
        add    esp, 4
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        test   edi, edi
        je     SHORT loc_6_4324
        db  0A0h, 000h, 000h, 000h, 000h
loc_6_42EA:
        test   byte ptr [edi + 14h], 1
        je     SHORT loc_6_4304
loc_6_42F0:
        test   al, 4
        jne    SHORT loc_6_4304
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_42F9:
        test   byte ptr [edi + 14h], 1
        db  0A0h, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_42F0
loc_6_4304:
        cmp    dword ptr [edi + 76h], esi
        jne    SHORT loc_6_4319
        push   1
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_6_4311:
        add    esp, 8
        db  0A0h, 000h, 000h, 000h, 000h
loc_6_4319:
        mov    edi, dword ptr [edi + 4]
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_6_42EA
loc_6_4324:
        xor    edi, edi
        mov    ebx, 0ffh
loc_6_432B:
        cmp    byte ptr [esi + edi + 63h], bl
        je     SHORT loc_6_4339
        push   edi
        push   esi
        call   dword ptr [esi + 20h]
        add    esp, 8
loc_6_4339:
        inc    edi
        cmp    edi, 5
        jl     SHORT loc_6_432B
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_PTEXT ends
        end
