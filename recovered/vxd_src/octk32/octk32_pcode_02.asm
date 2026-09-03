; octk32_pcode_02.asm -- OCTK32 object 2 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        dec    eax
        cmp    eax, 0eh
        ja     SHORT loc_2_007A
        db  0FFh, 024h, 085h, 080h, 000h, 000h, 000h
loc_2_0014:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 545h]
        push   eax
        call   sub_2_2270
        jmp    SHORT loc_2_0028
loc_2_0025:
        mov    esi, dword ptr [ebp + 8]
loc_2_0028:
        lea    eax, [esi + 515h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0034:
        jmp    SHORT loc_2_0039
loc_2_0036:
        mov    esi, dword ptr [ebp + 8]
loc_2_0039:
        push   1
        push   1
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0043:
        jmp    SHORT loc_2_0048
loc_2_0045:
        mov    esi, dword ptr [ebp + 8]
loc_2_0048:
        mov    eax, dword ptr [esi + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0054:
        jmp    SHORT loc_2_0059
loc_2_0056:
        mov    esi, dword ptr [ebp + 8]
loc_2_0059:
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        jmp    SHORT loc_2_006D
loc_2_006A:
        mov    esi, dword ptr [ebp + 8]
loc_2_006D:
        push   0
        push   683h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_007A:
        pop    esi
        pop    ebp
        ret    8
        db  090h, 014h, 000h, 000h, 000h, 025h, 000h, 000h, 000h, 025h, 000h, 000h, 000h, 036h, 000h, 000h
        db  000h, 025h, 000h, 000h, 000h, 056h, 000h, 000h, 000h, 056h, 000h, 000h, 000h, 045h, 000h, 000h
        db  000h, 07Ah, 000h, 000h, 000h, 07Ah, 000h, 000h, 000h, 07Ah, 000h, 000h, 000h, 045h, 000h, 000h
        db  000h, 06Ah, 000h, 000h, 000h, 056h, 000h, 000h, 000h, 06Ah, 000h, 000h, 000h, 08Dh, 064h, 024h
        db  000h
sub_2_00C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        db  080h, 03Dh, 01Ch, 000h, 000h, 000h, 000h
        jne    SHORT loc_2_0130
        push   4
        mov    esi, dword ptr [ebp + 8]
        push   0fffd9h
        mov    eax, dword ptr [esi + 4ffh]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_00EE:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0144
        mov    eax, dword ptr [ebp - 4]
        cmp    byte ptr [eax], 45h
        jne    SHORT loc_2_011F
        cmp    byte ptr [eax + 1], 49h
        jne    SHORT loc_2_011F
        cmp    byte ptr [eax + 2], 53h
        jne    SHORT loc_2_011F
        cmp    byte ptr [eax + 3], 41h
        jne    SHORT loc_2_011F
        db  0C7h, 005h, 018h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        mov    dword ptr [esi + 46h], 1
loc_2_011F:
        db  0C6h, 005h, 01Ch, 000h, 000h, 000h, 001h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_0130:
        db  083h, 03Dh, 018h, 000h, 000h, 000h, 001h
        jne    SHORT loc_2_0144
        mov    esi, dword ptr [ebp + 8]
        db  0A1h, 018h, 000h, 000h, 000h
        mov    dword ptr [esi + 46h], eax
loc_2_0144:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_2_0150:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        mov    word ptr [ebp - 10h], 0a20h
        mov    word ptr [ebp - 0eh], 0a24h
        mov    word ptr [ebp - 0ch], 0a50h
        mov    word ptr [ebp - 0ah], 0a54h
        sub    bl, bl
        mov    word ptr [ebp - 8], 1a20h
        mov    word ptr [ebp - 6], 1a24h
        mov    word ptr [ebp - 4], 1a50h
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 2], 1a54h
        mov    eax, dword ptr [esi + 4f7h]
        cmp    eax, 7
        jbe    SHORT loc_2_01D2
        mov    dword ptr [esi + 4bah], 0aa00000ah
        mov    dword ptr [esi + 4beh], 5
        mov    dword ptr [esi + 4c2h], 2
        mov    dword ptr [esi + 4c6h], 1000h
        mov    bl, 1
        mov    dword ptr [esi + 4cah], eax
        mov    dword ptr [esi + 4f7h], 0ffh
loc_2_01D2:
        mov    eax, dword ptr [esi + 4f7h]
        mov    eax, dword ptr [ebp + eax*2 - 10h]
        and    eax, 0ffffh
        mov    dword ptr [esi + 4fbh], eax
        push   esi
        call   sub_2_15E0
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        je     SHORT loc_2_024D
        cmp    eax, 6
        mov    eax, dword ptr [ebp + 0ch]
        jne    SHORT loc_2_0216
        mov    cl, byte ptr [eax]
        test   cl, 40h
        je     SHORT loc_2_0216
        test   cl, 80h
        jne    SHORT loc_2_0216
        push   6
        push   eax
        lea    eax, [esi + 50h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0214:
        jmp    SHORT loc_2_024D
loc_2_0216:
        mov    dword ptr [esi + 4bah], 0aa000009h
        mov    dword ptr [esi + 4beh], 5
        mov    dword ptr [esi + 4c2h], 2
        mov    dword ptr [esi + 4c6h], 1200h
        mov    bl, 1
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    dword ptr [esi + 4cah], eax
loc_2_024D:
        mov    eax, dword ptr [esi + 3ah]
        or     eax, eax
        je     SHORT loc_2_029C
        cmp    eax, 200h
        jb     SHORT loc_2_0262
        cmp    eax, 1fd0h
        jbe    SHORT loc_2_029C
loc_2_0262:
        mov    dword ptr [esi + 4bah], 0aa00000ah
        mov    dword ptr [esi + 4beh], 5
        mov    dword ptr [esi + 4c2h], 2
        mov    dword ptr [esi + 4c6h], 1700h
        mov    bl, 1
        mov    eax, dword ptr [esi + 3ah]
        mov    dword ptr [esi + 4cah], eax
        mov    dword ptr [esi + 3ah], 0
loc_2_029C:
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_02B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        db  0A1h, 084h, 000h, 000h, 000h
        push   eax
        lea    eax, [ebp - 4]
        push   0
        push   683h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_02CF:
        or     eax, eax
        jne    NEAR PTR loc_2_0857
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 1a0h
        rep stosd
        stosw
        stosb
        mov    ecx, dword ptr [ebp - 4]
        db  0A1h, 008h, 000h, 000h, 000h
        mov    dword ptr [ecx + 503h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4f7h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 3eh], 0bh
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 24h]
        mov    dword ptr [ecx + 0c3h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 28h]
        mov    dword ptr [ecx + 0c7h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 2d5h], 3f8h
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 2ch]
        mov    word ptr [ecx + 2d7h], ax
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp + 30h]
        mov    byte ptr [eax + 0e6h], cl
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp + 34h]
        mov    byte ptr [eax + 39h], cl
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 535h], 0f0h
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 38h]
        mov    dword ptr [ecx + 3ah], eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_0150
        mov    bl, al
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_2100
        lea    eax, [ebp - 0ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_1920
        or     al, al
        je     SHORT loc_2_03A5
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_2_14F0
loc_2_03A5:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, eax
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        db  0C7h, 000h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        db  0A1h, 004h, 000h, 000h, 000h
        mov    dword ptr [ecx + 4], eax
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 004h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 3eh]
        cmp    eax, 7
        je     SHORT loc_2_0416
        cmp    eax, 9
        je     SHORT loc_2_0416
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4f7h]
        cmp    ecx, 1fh
        mov    eax, 1
        ja     SHORT loc_2_0409
        shl    eax, cl
        db  009h, 005h, 010h, 000h, 000h, 000h
        jmp    SHORT loc_2_0429
loc_2_0409:
        sub    cl, 20h
        shl    eax, cl
        db  009h, 005h, 014h, 000h, 000h, 000h
        jmp    SHORT loc_2_0429
loc_2_0416:
        mov    eax, dword ptr [ebp - 4]
        mov    edx, 1
        mov    cl, byte ptr [eax + 2fh]
        shl    edx, cl
        db  009h, 015h, 01Dh, 000h, 000h, 000h
loc_2_0429:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 42h]
        mov    dword ptr [ecx + 46h], eax
        lea    edi, [ebp - 40h]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    byte ptr [ebp - 3ch], al
        mov    dword ptr [ebp - 40h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 42h]
        or     eax, eax
        je     SHORT loc_2_0466
        cmp    eax, 1
        je     SHORT loc_2_0470
        cmp    eax, 2
        je     SHORT loc_2_0480
        mov    dword ptr [ebp - 36h], 1
        jmp    SHORT loc_2_048B
loc_2_0466:
        mov    dword ptr [ebp - 36h], 1
        jmp    SHORT loc_2_048B
        db  090h
loc_2_0470:
        mov    dword ptr [ebp - 36h], 2
        jmp    SHORT loc_2_048B
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0480:
        mov    dword ptr [ebp - 36h], 3
        mov    byte ptr [ebp - 3bh], 1
loc_2_048B:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_2_04B0
        mov    eax, dword ptr [ebp - 4]
        cmp    word ptr [eax + 0ch], 0
        jne    SHORT loc_2_04A4
        mov    dword ptr [ebp - 2ah], 1
        jmp    SHORT loc_2_04B7
loc_2_04A4:
        mov    dword ptr [ebp - 2ah], 2
        jmp    SHORT loc_2_04B7
        db  08Dh, 049h, 000h
loc_2_04B0:
        mov    dword ptr [ebp - 2ah], 0
loc_2_04B7:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 26h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 22h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 1ah], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 16h], eax
        lea    eax, [ebp - 40h]
        push   eax
        push   esi
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [eax + 503h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4ffh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0518:
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        jne    NEAR PTR loc_2_07D0
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_2_05A7
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4c2h], 2
        jne    SHORT loc_2_0560
        mov    ecx, dword ptr [eax + 4bah]
        mov    eax, dword ptr [eax + 4c6h]
        push   eax
        push   ecx
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        mov    eax, dword ptr [eax + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0555:
        add    esp, 14h
        jmp    SHORT loc_2_0590
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0560:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4bah]
        mov    eax, dword ptr [eax + 4cah]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4c6h]
        push   eax
        push   ecx
        push   3
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        mov    eax, dword ptr [eax + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_058D:
        add    esp, 18h
loc_2_0590:
        push   0ch
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    34h
loc_2_05A7:
        or     bl, bl
        je     SHORT loc_2_0610
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4c2h], 2
        jne    SHORT loc_2_05E0
        mov    ecx, dword ptr [eax + 4bah]
        mov    eax, dword ptr [eax + 4c6h]
        push   eax
        push   ecx
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        mov    eax, dword ptr [eax + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_05D7:
        add    esp, 14h
        jmp    SHORT loc_2_0610
        db  08Dh, 064h, 024h, 000h
loc_2_05E0:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4bah]
        mov    eax, dword ptr [eax + 4cah]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4c6h]
        push   eax
        push   ecx
        push   3
        mov    eax, dword ptr [ebp - 4]
        push   ecx
        mov    eax, dword ptr [eax + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_060D:
        add    esp, 18h
loc_2_0610:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 46h], 0
        jne    SHORT loc_2_061F
        push   eax
        call   sub_2_00C0
loc_2_061F:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_2_19E0
        or     al, al
        jne    SHORT loc_2_0665
        push   0c000001fh
        push   5
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0aa000002h
        mov    eax, dword ptr [eax + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0649:
        add    esp, 14h
        push   8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    34h
loc_2_0665:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 20h]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 52dh]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   1
        mov    eax, dword ptr [eax + 20h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 24h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 4ffh]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 515h
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_06B2:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_2_06F3
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0c0000018h
        push   5
        push   3
        mov    eax, dword ptr [ebp - 4]
        push   0aa000005h
        mov    eax, dword ptr [eax + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_06D9:
        add    esp, 18h
        push   4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    34h
loc_2_06F3:
        push   1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_06FE:
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     SHORT loc_2_0740
        push   eax
        push   0c0000008h
        push   5
        push   3
        mov    eax, dword ptr [ebp - 4]
        push   0aa000003h
        mov    eax, dword ptr [eax + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0723:
        add    esp, 18h
        push   5
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    34h
        db  08Dh, 049h, 000h
loc_2_0740:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 545h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 531h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 50h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_2_21A0
        or     al, al
        jne    SHORT loc_2_07B0
        push   0c000001ch
        push   5
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   0aa000002h
        mov    eax, dword ptr [eax + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0793:
        add    esp, 14h
        push   2
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    34h
        db  090h
loc_2_07B0:
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [eax + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_07C5:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    34h
loc_2_07D0:
        mov    byte ptr [ebp - 3ch], 0
        mov    dword ptr [ebp - 40h], 0
        mov    dword ptr [ebp - 2ah], 0
        lea    eax, [ebp - 40h]
        push   eax
        push   esi
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [eax + 503h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4ffh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0801:
        or     eax, eax
        jne    SHORT loc_2_0840
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0c000000ch
        push   5
        push   3
        mov    eax, dword ptr [ebp - 4]
        push   0aa000002h
        mov    eax, dword ptr [eax + 4ffh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0826:
        add    esp, 18h
        push   0ch
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    34h
loc_2_0840:
        push   0eh
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   0
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    34h
loc_2_0857:
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    34h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_0870:
        push   ebp
        mov    ebp, esp
        sub    esp, 74h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        lea    eax, [ebp - 2bh]
        mov    byte ptr [ebp - 2ch], 0
        mov    dword ptr [eax], 0
        mov    byte ptr [eax + 4], 0
        mov    dword ptr [ebp - 18h], 0
        mov    word ptr [ebp - 5ch], 8
        mov    word ptr [ebp - 5ah], 8
        db  0C7h, 045h, 0A8h, 090h, 000h, 000h, 000h
        mov    word ptr [ebp - 54h], 0dh
        mov    word ptr [ebp - 52h], 0dh
        db  0C7h, 045h, 0B0h, 0A8h, 000h, 000h, 000h
        mov    word ptr [ebp - 4ch], 0ah
        mov    word ptr [ebp - 4ah], 0ah
        db  0C7h, 045h, 0B8h, 0C4h, 000h, 000h, 000h
        mov    word ptr [ebp - 44h], 8
        mov    word ptr [ebp - 42h], 8
        db  0C7h, 045h, 0C0h, 0D8h, 000h, 000h, 000h
        mov    word ptr [ebp - 34h], 0ch
        mov    word ptr [ebp - 32h], 0ch
        db  0C7h, 045h, 0D0h, 054h, 001h, 000h, 000h
        sub    edi, edi
        mov    word ptr [ebp - 3ch], 0dh
        mov    word ptr [ebp - 3ah], 0dh
        db  0C7h, 045h, 0C8h, 070h, 001h, 000h, 000h
        mov    dword ptr [ebp - 1ch], 0
        sub    bl, bl
        mov    dword ptr [ebp - 20h], 0
        mov    dword ptr [ebp - 24h], 0
        mov    byte ptr [ebp - 1], 1
        mov    esi, dword ptr [ebp + 0ch]
        cmp    esi, edi
        je     NEAR PTR loc_2_0A97
        push   esi
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0940:
        cmp    dword ptr [ebp - 8], edi
        je     SHORT loc_2_0951
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_0951:
        push   0
        lea    eax, [ebp - 5ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0968:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0977
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 1ch], eax
loc_2_0977:
        push   0
        lea    eax, [ebp - 54h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_098E:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_099A
        mov    eax, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [eax + 4]
loc_2_099A:
        push   0
        lea    eax, [ebp - 4ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_09B1:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_09C0
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 20h], eax
loc_2_09C0:
        push   0
        lea    eax, [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_09D7:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_09E6
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 24h], eax
loc_2_09E6:
        push   2
        lea    eax, [ebp - 3ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_09FD:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0A16
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    al, byte ptr [eax]
        sub    al, 79h
        cmp    al, 1
        sbb    al, al
        neg    al
        mov    byte ptr [ebp - 1], al
loc_2_0A16:
        push   2
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0A2D:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0A43
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    al, byte ptr [eax]
        sub    al, 79h
        cmp    al, 1
        sbb    bl, bl
        neg    bl
loc_2_0A43:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0A58:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0A90
        cmp    dword ptr [ebp - 18h], 6
        jne    SHORT loc_2_0A97
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 2]
        or     al, byte ptr [ecx + 4]
        or     al, byte ptr [ecx + 5]
        or     al, byte ptr [ecx + 1]
        or     al, byte ptr [ecx + 3]
        or     al, byte ptr [ecx]
        je     SHORT loc_2_0A97
        push   6
        mov    eax, ecx
        push   eax
        lea    eax, [ebp - 2ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0A8B:
        jmp    SHORT loc_2_0A97
        db  08Dh, 049h, 000h
loc_2_0A90:
        mov    dword ptr [ebp - 18h], 0
loc_2_0A97:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0AA0:
        push   edi
        mov    al, byte ptr [ebp - 1]
        push   eax
        push   ebx
        push   100h
        push   80h
        push   3f8h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        lea    eax, [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_2_02B0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
sub_2_0AE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 50h
        mov    word ptr [ebp - 18h], 2
        mov    word ptr [ebp - 16h], 0
        mov    dword ptr [ebp - 14h], 12013e80h
        mov    dword ptr [ebp - 10h], 120126c0h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0B13:
        mov    eax, dword ptr [ebp - 4]
        db  0A3h, 00Ch, 000h, 000h, 000h
        db  0C7h, 005h, 018h, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h
        db  0C6h, 005h, 01Ch, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    byte ptr [ebp - 50h], 3
        mov    byte ptr [ebp - 4fh], 0
        db  0C7h, 045h, 0B6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0BAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0BEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CAh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0CEh, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D2h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D6h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0DAh, 000h, 000h, 000h, 000h
        db  0A1h, 088h, 001h, 000h, 000h
        db  08Bh, 00Dh, 08Ch, 001h, 000h, 000h
        mov    dword ptr [ebp - 22h], eax
        lea    eax, [ebp - 50h]
        mov    dword ptr [ebp - 1eh], ecx
        push   36h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        db  068h, 008h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0BBC:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_2_0BCB
        mov    eax, dword ptr [ebp - 8]
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0BCB:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_0BD6:
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0BF0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4bah], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 4beh], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 4c2h], eax
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [ecx + 4c6h], eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx + 4cah], eax
        pop    ebp
        ret    18h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0C30:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        mov    word ptr [ebp - 10h], 240h
        mov    word ptr [ebp - 0eh], 280h
        mov    word ptr [ebp - 0ch], 2c0h
        mov    word ptr [ebp - 0ah], 300h
        sub    ax, ax
        mov    word ptr [ebp - 8], 320h
        mov    word ptr [ebp - 6], 340h
        mov    word ptr [ebp - 4], 360h
        mov    dx, word ptr [ebp + 0ch]
        and    dx, 0fff0h
        sub    dx, 10h
loc_2_0C70:
        mov    ecx, eax
        and    ecx, 0ffffh
        cmp    word ptr [ebp + ecx*2 - 10h], dx
        je     SHORT loc_2_0C90
        inc    ax
        cmp    ax, 7
        jb     SHORT loc_2_0C70
        sub    al, al
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
loc_2_0C90:
        in     al, dx
        and    eax, 0ffh
        and    eax, 0fffffffah
        cmp    eax, 22h
        je     SHORT loc_2_0CA6
        sub    al, al
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_0CA6:
        add    dx, 3
        in     al, dx
        and    eax, 0ffh
        and    al, 0c0h
        cmp    al, 40h
        jne    SHORT loc_2_0CC0
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_2_0CC0:
        sub    al, al
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0CD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 3eh], 0bh
        je     SHORT loc_2_0CF0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_0CF0:
        mov    di, word ptr [esi + 12h]
        mov    dword ptr [esi + 42h], 0
        lea    edx, [edi + 3]
        in     al, dx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    cl, 7
loc_2_0D05:
        mov    bl, cl
        xor    bl, al
        mov    al, bl
        out    dx, al
        in     al, dx
        in     al, dx
        xor    al, cl
        mov    bl, al
        xor    bl, byte ptr [ebp - 1]
        and    ebx, 0ffh
        test   ebx, 0ffffffbfh
        jne    SHORT loc_2_0D27
        dec    cl
        jne    SHORT loc_2_0D05
loc_2_0D27:
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        or     cl, cl
        jne    SHORT loc_2_0D49
        add    di, 2
        mov    word ptr [esi + 12h], di
        mov    dword ptr [esi + 3eh], 1
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0D49:
        lea    edx, [edi + 2]
        in     al, dx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    cl, 3fh
        and    al, cl
        mov    byte ptr [ebp - 2], al
        out    dx, al
loc_2_0D59:
        mov    bl, cl
        xor    bl, al
        mov    al, bl
        out    dx, al
        in     al, dx
        in     al, dx
        xor    al, cl
        cmp    al, byte ptr [ebp - 2]
        jne    SHORT loc_2_0D6D
        dec    cl
        jne    SHORT loc_2_0D59
loc_2_0D6D:
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        or     cl, cl
        jne    SHORT loc_2_0DC6
        inc    dx
        in     al, dx
        mov    cl, al
        sub    ebx, ebx
loc_2_0D7C:
        in     al, dx
        cmp    al, cl
        jne    SHORT loc_2_0DA4
        inc    ebx
        cmp    ebx, 0ah
        jl     SHORT loc_2_0D7C
        test   al, 6
        jne    SHORT loc_2_0DB0
        mov    dword ptr [esi + 3eh], 5
        mov    dword ptr [esi + 42h], 1
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0DA4:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_2_0DB0:
        test   al, 4
        je     SHORT loc_2_0DC6
        mov    dword ptr [esi + 3eh], 0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0DC6:
        lea    edx, [edi + 1]
        in     al, dx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    cl, 7
loc_2_0DD0:
        mov    bl, cl
        xor    bl, al
        mov    al, bl
        out    dx, al
        in     al, dx
        in     al, dx
        xor    al, cl
        mov    bl, al
        xor    bl, byte ptr [ebp - 1]
        mov    edi, ebx
        and    edi, 0ffh
        test   edi, 0ffffffbfh
        jne    SHORT loc_2_0DF4
        dec    cl
        jne    SHORT loc_2_0DD0
loc_2_0DF4:
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        or     cl, cl
        je     SHORT loc_2_0E07
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0E07:
        mov    cl, byte ptr [ebp - 1]
        mov    al, cl
        sub    edi, edi
        mov    edi, edi
loc_2_0E10:
        xor    cl, 40h
        xor    al, 40h
        out    dx, al
        in     al, dx
        in     al, dx
        cmp    al, cl
        jne    SHORT loc_2_0E22
        inc    edi
        cmp    edi, 8
        jl     SHORT loc_2_0E10
loc_2_0E22:
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        cmp    edi, 8
        jne    SHORT loc_2_0E44
        mov    dword ptr [esi + 3eh], 2
        mov    dword ptr [esi + 42h], 1
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_0E44:
        mov    al, byte ptr [ebp - 1]
        and    al, 0bfh
        out    dx, al
        mov    dword ptr [esi + 3eh], 3
        mov    dword ptr [esi + 42h], 2
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0E70:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    cx, word ptr [eax + 10h]
        lea    eax, [ecx + 4]
        mov    word ptr [ebp - 0ch], ax
        mov    dx, ax
        in     al, dx
        mov    bl, al
        lea    edx, [ecx + 5]
        mov    word ptr [ebp - 8], dx
        in     al, dx
        mov    si, ax
        and    esi, 0ffff00ffh
        shl    si, 8
        mov    al, bl
        and    eax, 0ffff00ffh
        or     si, ax
        test   si, 1
        je     SHORT loc_2_0EE0
        mov    eax, esi
        and    eax, 0ffffh
        push   eax
        push   0c0000001h
        push   1
        push   3
        push   0aa000004h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0BF0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_0EE0:
        lea    edx, [ecx + 2]
        in     al, dx
        lea    edx, [ecx + 3]
        in     al, dx
        sub    edi, edi
loc_2_0EEA:
        mov    dx, word ptr [ebp - 0ch]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    dx, word ptr [ebp - 8]
        in     al, dx
        mov    bl, al
        and    ebx, 0ffff00ffh
        shl    bx, 8
        mov    al, byte ptr [ebp - 2]
        and    eax, 0ffff00ffh
        or     bx, ax
        mov    eax, esi
        mov    edx, ebx
        and    edx, 0ffffh
        and    eax, 0ffffh
        sub    edx, eax
        cmp    edx, 2
        jne    SHORT loc_2_0F70
        lea    edx, [ecx + 0ah]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        lea    edx, [ecx + 0bh]
        in     al, dx
        mov    si, ax
        and    esi, 0ffff00ffh
        shl    si, 8
        mov    al, byte ptr [ebp - 1]
        and    eax, 0ffff00ffh
        or     si, ax
        cmp    bx, si
        jne    SHORT loc_2_0FA0
        mov    dx, cx
        in     al, dx
        lea    edx, [ecx + 1]
        in     al, dx
        lea    edx, [ecx + 2]
        in     al, dx
        lea    edx, [ecx + 3]
        in     al, dx
        inc    edi
        cmp    edi, 8
        jl     SHORT loc_2_0EEA
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_2_0F70:
        mov    eax, ebx
        and    eax, 0ffffh
        push   eax
        push   0c0000002h
        push   1
        push   3
        push   0aa000004h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_2_0BF0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_0FA0:
        push   0
        push   0c0000012h
        push   1
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   0aa000004h
        mov    eax, dword ptr [eax + 4ffh]
        push   eax
        call   sub_2_0BF0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0FD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 0bh
        mov    dword ptr [esi + 3eh], ebx
        mov    ax, word ptr [esi + 0ah]
        mov    word ptr [ebp - 4], ax
        cmp    ax, 20h
        jne    SHORT loc_2_103E
        mov    dx, word ptr [esi + 12h]
        inc    dx
        sub    di, di
        mov    ecx, 4
loc_2_1000:
        in     al, dx
        and    ax, 0ffh
        shr    al, 4
        and    ax, 0ffh
        shl    di, 4
        or     ax, di
        mov    di, ax
        dec    ecx
        jne    SHORT loc_2_1000
        sub    eax, eax
        add    eax, 0
loc_2_1020:
        cmp    di, 9436h
        je     SHORT loc_2_1094
        mov    cx, di
        shr    cx, 0ch
        shl    di, 4
        or     cx, di
        mov    di, cx
        inc    eax
        cmp    eax, 4
        jl     SHORT loc_2_1020
loc_2_103E:
        cmp    word ptr [ebp - 4], 20h
        je     NEAR PTR loc_2_10D3
        mov    dx, word ptr [esi + 12h]
        add    dx, 2
        sub    cx, cx
        mov    edi, 4
loc_2_1059:
        in     al, dx
        and    ax, 0ffh
        shr    al, 4
        and    ax, 0ffh
        shl    cx, 4
        or     ax, cx
        mov    cx, ax
        dec    edi
        jne    SHORT loc_2_1059
        sub    eax, eax
loc_2_1074:
        cmp    cx, 9246h
        je     SHORT loc_2_10C3
        mov    di, cx
        shr    di, 0ch
        shl    cx, 4
        or     di, cx
        mov    cx, di
        inc    eax
        cmp    eax, 4
        jl     SHORT loc_2_1074
        jmp    SHORT loc_2_10D3
loc_2_1094:
        mov    ebx, 6
        mov    cx, word ptr [esi + 12h]
        mov    dword ptr [esi + 42h], 0
        lea    edx, [ecx + 1]
        in     al, dx
        and    al, 0f7h
        out    dx, al
        mov    dx, cx
        mov    al, 0feh
        out    dx, al
        lea    edx, [ecx + 2]
        in     al, dx
        or     al, 40h
        out    dx, al
        mov    ecx, 14h
loc_2_10BD:
        in     al, dx
        dec    ecx
        jne    SHORT loc_2_10BD
        jmp    SHORT loc_2_10D3
loc_2_10C3:
        mov    word ptr [esi + 12h], dx
        mov    dword ptr [esi + 42h], 0
        mov    ebx, 4
loc_2_10D3:
        push   esi
        call   sub_2_0E70
        or     al, al
        je     NEAR PTR loc_2_116B
        cmp    ebx, 0bh
        je     SHORT loc_2_1111
        mov    dword ptr [esi + 3eh], ebx
loc_2_10E9:
        mov    eax, dword ptr [esi + 3eh]
        cmp    eax, 5
        je     SHORT loc_2_113C
        push   eax
        push   0c0000011h
        push   1
        push   3
        push   0c100000dh
        push   esi
        call   sub_2_0BF0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1111:
        push   esi
        call   sub_2_0CD0
        or     al, al
        jne    SHORT loc_2_10E9
        push   0
        push   0c0000005h
        push   2
        push   2
        push   0aa000003h
        push   esi
        call   sub_2_0BF0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_113C:
        or     eax, eax
        jne    SHORT loc_2_1160
        push   eax
        push   0c0000011h
        push   1
        push   3
        push   0c100000dh
        push   esi
        call   sub_2_0BF0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1160:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_116B:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1180:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        and    dword ptr [esi + 0b7h], 0fffffffeh
        call   sub_2_0CD0
        or     al, al
        je     SHORT loc_2_1215
        mov    byte ptr [esi + 1eh], 0
        push   esi
        call   sub_2_2080
        or     al, al
        je     SHORT loc_2_11B4
        or     dword ptr [esi + 0b7h], 1
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    4
loc_2_11B4:
        mov    eax, dword ptr [esi + 3eh]
        cmp    eax, 5
        je     SHORT loc_2_11DB
        push   eax
        push   0c0000011h
        push   1
        push   3
        push   0c100000dh
        push   esi
        call   sub_2_0BF0
        mov    eax, 0c0000001h
        pop    esi
        pop    ebp
        ret    4
loc_2_11DB:
        or     eax, eax
        push   eax
        push   0c0000011h
        push   1
        push   3
        jne    SHORT loc_2_1200
        push   0c100000dh
        push   esi
        call   sub_2_0BF0
        mov    eax, 0c0000001h
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_1200:
        push   0c100000eh
        push   esi
        call   sub_2_0BF0
        mov    eax, 0c0000001h
        pop    esi
        pop    ebp
        ret    4
loc_2_1215:
        push   0
        push   0c0000005h
        push   2
        push   2
        push   0aa000003h
        push   esi
        call   sub_2_0BF0
        mov    eax, 0c0000001h
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1240:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        sub    si, si
        sub    ebx, ebx
        mov    edi, dword ptr [ebp + 8]
loc_2_1251:
        push   2710h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_125B:
        mov    cx, si
        mov    si, word ptr [edi + 10h]
        lea    edx, [esi + 6]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        lea    edx, [esi + 7]
        in     al, dx
        mov    si, ax
        and    esi, 0ffff00ffh
        shl    si, 8
        mov    al, byte ptr [ebp - 1]
        and    eax, 0ffff00ffh
        or     si, ax
        mov    word ptr [edi + 0b1h], si
        cmp    cx, si
        je     SHORT loc_2_1298
        test   si, 40h
        jne    SHORT loc_2_12E0
loc_2_1298:
        add    ebx, 2710h
        cmp    ebx, 5b8d80h
        jle    SHORT loc_2_1251
        test   si, 30h
        je     SHORT loc_2_12C9
        mov    eax, esi
        and    eax, 0ffffh
        push   eax
        push   0c0000017h
        push   4
        push   3
        push   0c1000001h
        push   edi
        call   sub_2_0BF0
loc_2_12C9:
        and    dword ptr [edi + 0b7h], 0fffffffch
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_12E0:
        or     dword ptr [edi + 0b7h], 2
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1300:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    ax, 0c000h
        cmp    dword ptr [edx + 3eh], 9
        jne    SHORT loc_2_1322
        cmp    byte ptr [edx + 18h], 0
        je     SHORT loc_2_1326
        mov    ax, 0c800h
        jmp    SHORT loc_2_1326
loc_2_1322:
        mov    ax, 0d000h
loc_2_1326:
        mov    edi, dword ptr [ebp + 10h]
        mov    dx, word ptr [edi + 4]
        mov    ch, dl
        mov    cl, dh
        or     cx, cx
        je     NEAR PTR loc_2_1443
        and    eax, 0ffffh
        mov    dword ptr [ebp - 10h], eax
loc_2_1342:
        mov    ax, word ptr [edi]
        mov    dx, ax
        and    edx, 0ffh
        shl    edx, 8
        mov    dword ptr [ebp - 0ch], edx
        mov    dx, ax
        shr    dx, 8
        mov    esi, edx
        and    esi, 0ffffh
        or     esi, dword ptr [ebp - 0ch]
        and    esi, dword ptr [ebp - 10h]
        cmp    esi, 1
        sbb    bl, bl
        neg    bl
        or     bl, bl
        je     SHORT loc_2_13A0
        shl    al, 8
        or     al, dl
        mov    edx, dword ptr [ebp + 8]
        mov    si, word ptr [edx + 10h]
        lea    edx, [esi + 0ch]
        out    dx, al
        lea    edx, [esi + 0dh]
        mov    al, byte ptr [ebp - 0bh]
        out    dx, al
        mov    ax, word ptr [edi + 2]
        shl    al, 8
        or     al, byte ptr [edi + 3]
        lea    edx, [esi + 4]
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, byte ptr [edi + 2]
        out    dx, al
loc_2_13A0:
        test   cl, 1
        je     SHORT loc_2_13F0
        shr    cx, 1
        mov    dx, word ptr [edi + 6]
        mov    ah, dl
        mov    al, dh
        mov    word ptr [ebp - 2], ax
        or     bl, bl
        je     SHORT loc_2_13E8
        mov    ax, cx
        dec    cx
        or     ax, ax
        je     SHORT loc_2_13E8
        mov    edx, dword ptr [ebp + 8]
        mov    si, word ptr [edx + 10h]
        lea    ebx, [esi + 2]
        add    si, 3
loc_2_13D0:
        mov    dx, bx
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    dx, si
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    ax, cx
        dec    cx
        or     ax, ax
        jne    SHORT loc_2_13D0
loc_2_13E8:
        add    edi, 8
        jmp    SHORT loc_2_1432
        db  08Dh, 049h, 000h
loc_2_13F0:
        lea    esi, [edi + 6]
        mov    eax, ecx
        and    eax, 0fffeh
        lea    edi, [esi + eax]
        cmp    edi, esi
        jbe    SHORT loc_2_1432
loc_2_1401:
        or     bl, bl
        je     SHORT loc_2_142B
        mov    cx, word ptr [esi]
        mov    ax, cx
        shl    al, 8
        mov    edx, dword ptr [ebp + 8]
        or     al, ch
        mov    dx, word ptr [edx + 10h]
        mov    word ptr [ebp - 8], dx
        add    dx, 2
        out    dx, al
        mov    dx, word ptr [ebp - 8]
        add    dx, 3
        mov    al, cl
        out    dx, al
loc_2_142B:
        add    esi, 2
        cmp    edi, esi
        ja     SHORT loc_2_1401
loc_2_1432:
        mov    ax, word ptr [edi + 4]
        mov    ch, al
        mov    cl, ah
        or     cx, cx
        jne    NEAR PTR loc_2_1342
loc_2_1443:
        mov    edx, dword ptr [ebp + 8]
        mov    al, 1
        mov    cx, word ptr [edx + 10h]
        pop    edi
        lea    edx, [ecx + 0ch]
        out    dx, al
        lea    edx, [ecx + 0dh]
        sub    al, al
        out    dx, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
sub_2_1460:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, 80h
        mov    dx, word ptr [esi + 10h]
        add    dx, 8
        out    dx, al
        push   2710h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_147C:
        mov    cx, word ptr [esi + 10h]
        add    cx, 8
        mov    dx, cx
        in     al, dx
        and    al, 64h
        or     al, 1
        cmp    dword ptr [esi + 35h], 9c40h
        jne    SHORT loc_2_1497
        or     al, 2
loc_2_1497:
        mov    dx, cx
        out    dx, al
        push   2710h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_14A5:
        mov    cx, word ptr [esi + 10h]
        add    cx, 9
        mov    dx, cx
        sub    al, al
        out    dx, al
        push   2710h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_14BD:
        db  068h, 002h, 000h, 000h, 000h
        push   0a00h
        push   esi
        call   sub_2_1300
        mov    cx, word ptr [esi + 10h]
        add    cx, 8
        mov    dx, cx
        in     al, dx
        and    al, 0bfh
        or     al, 8
        out    dx, al
        push   esi
        call   sub_2_1240
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_14F0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [esi], 0c0000001h
        push   edi
        call   sub_2_1180
        or     eax, eax
        jne    NEAR PTR loc_2_15CC
        push   edi
        call   sub_2_1460
        or     eax, eax
        jne    NEAR PTR loc_2_15B6
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1523:
        cmp    dword ptr [edi + 42h], 0
        lea    ecx, [edi + 52dh]
        jne    SHORT loc_2_1537
        mov    dword ptr [ecx], 1
        jmp    SHORT loc_2_153D
loc_2_1537:
        mov    dword ptr [ecx], 2
loc_2_153D:
        mov    eax, dword ptr [edi + 3eh]
        cmp    eax, 7
        je     SHORT loc_2_154A
        cmp    eax, 9
        jne    SHORT loc_2_1556
loc_2_154A:
        mov    eax, dword ptr [edi + 20h]
        mov    dword ptr [edi + 24h], eax
        mov    dword ptr [ecx], 2
loc_2_1556:
        cmp    dword ptr [edi + 35h], 27100h
        mov    eax, 1fd0h
        je     SHORT loc_2_156D
        mov    eax, 1178h
        mov    byte ptr [edi + 39h], 0
loc_2_156D:
        mov    ecx, dword ptr [edi + 3ah]
        or     ecx, ecx
        je     SHORT loc_2_1578
        cmp    eax, ecx
        jae    SHORT loc_2_157B
loc_2_1578:
        mov    dword ptr [edi + 3ah], eax
loc_2_157B:
        cmp    dword ptr [edi + 42h], 0
        jne    SHORT loc_2_158F
        mov    eax, dword ptr [edi + 0c3h]
        cmp    dword ptr [edi + 3ah], eax
        jbe    SHORT loc_2_158F
        mov    dword ptr [edi + 3ah], eax
loc_2_158F:
        cmp    word ptr [edi + 54h], 0
        jne    SHORT loc_2_15AA
        cmp    dword ptr [edi + 50h], 0
        jne    SHORT loc_2_15AA
        mov    ax, word ptr [edi + 4eh]
        mov    word ptr [edi + 54h], ax
        mov    eax, dword ptr [edi + 4ah]
        mov    dword ptr [edi + 50h], eax
loc_2_15AA:
        mov    dword ptr [esi], 0
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_2_15B6:
        push   0
        push   0c0000007h
        push   1
        push   2
        push   0c1000009h
        push   edi
        call   sub_2_0BF0
loc_2_15CC:
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_15E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 70h
        mov    word ptr [ebp - 70h], 0a20h
        mov    word ptr [ebp - 6eh], 0a30h
        mov    word ptr [ebp - 6ch], 0a18h
        mov    word ptr [ebp - 6ah], 0a18h
        mov    word ptr [ebp - 68h], 0ch
        mov    word ptr [ebp - 66h], 0a30h
        mov    word ptr [ebp - 64h], 10h
        mov    word ptr [ebp - 62h], 0a24h
        mov    word ptr [ebp - 60h], 0a40h
        mov    word ptr [ebp - 5eh], 0a28h
        mov    word ptr [ebp - 5ch], 0a24h
        mov    word ptr [ebp - 5ah], 0ch
        mov    word ptr [ebp - 58h], 0a40h
        mov    word ptr [ebp - 56h], 10h
        mov    word ptr [ebp - 54h], 0a50h
        mov    word ptr [ebp - 52h], 0a60h
        mov    word ptr [ebp - 50h], 0a58h
        mov    word ptr [ebp - 4eh], 0a50h
        mov    word ptr [ebp - 4ch], 4
        mov    word ptr [ebp - 4ah], 0a58h
        mov    word ptr [ebp - 48h], 18h
        mov    word ptr [ebp - 46h], 0a54h
        mov    word ptr [ebp - 44h], 0a70h
        mov    word ptr [ebp - 42h], 0a80h
        mov    word ptr [ebp - 40h], 0a54h
        mov    word ptr [ebp - 3eh], 4
        mov    word ptr [ebp - 3ch], 0a70h
        mov    word ptr [ebp - 3ah], 18h
        mov    word ptr [ebp - 38h], 1a20h
        mov    word ptr [ebp - 36h], 1a30h
        mov    word ptr [ebp - 34h], 1a18h
        mov    word ptr [ebp - 32h], 1a18h
        mov    word ptr [ebp - 30h], 0ch
        mov    word ptr [ebp - 2eh], 1a30h
        mov    word ptr [ebp - 2ch], 10h
        mov    word ptr [ebp - 2ah], 1a24h
        mov    word ptr [ebp - 28h], 1a40h
        mov    word ptr [ebp - 26h], 1a28h
        mov    word ptr [ebp - 24h], 1a24h
        mov    word ptr [ebp - 22h], 0ch
        mov    word ptr [ebp - 20h], 1a40h
        mov    word ptr [ebp - 1eh], 10h
        mov    word ptr [ebp - 1ch], 1a50h
        mov    word ptr [ebp - 1ah], 1a60h
        mov    word ptr [ebp - 18h], 1a58h
        mov    word ptr [ebp - 16h], 1a50h
        mov    word ptr [ebp - 14h], 4
        mov    word ptr [ebp - 12h], 1a58h
        mov    word ptr [ebp - 10h], 18h
        mov    word ptr [ebp - 0eh], 1a54h
        mov    word ptr [ebp - 0ch], 1a70h
        mov    word ptr [ebp - 0ah], 1a80h
        mov    word ptr [ebp - 8], 1a54h
        mov    word ptr [ebp - 6], 4
        mov    word ptr [ebp - 4], 1a70h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ebp - 2], 18h
        cmp    dword ptr [ecx + 3eh], 0bh
        jne    NEAR PTR loc_2_17F7
        mov    edx, dword ptr [ecx + 4f7h]
        mov    eax, edx
        shl    edx, 3
        sub    edx, eax
        mov    ax, word ptr [ebp + edx*2 - 6eh]
        mov    word ptr [ecx + 10h], ax
        mov    eax, dword ptr [ecx + 4f7h]
        mov    edx, eax
        shl    eax, 3
        sub    eax, edx
        mov    ax, word ptr [ebp + eax*2 - 70h]
        mov    word ptr [ecx + 12h], ax
        mov    eax, dword ptr [ecx + 4f7h]
        mov    edx, eax
        shl    eax, 3
        sub    eax, edx
        mov    ax, word ptr [ebp + eax*2 - 6ch]
        mov    word ptr [ecx + 14h], ax
        mov    eax, dword ptr [ecx + 4f7h]
        mov    edx, eax
        shl    eax, 3
        sub    eax, edx
        mov    ax, word ptr [ebp + eax*2 - 6ch]
        add    ax, 4
        mov    word ptr [ecx + 16h], ax
        mov    edx, dword ptr [ecx + 4f7h]
        mov    eax, edx
        shl    edx, 3
        sub    edx, eax
        mov    ax, word ptr [ebp + edx*2 - 6ah]
        mov    word ptr [ecx + 8], ax
        mov    eax, dword ptr [ecx + 4f7h]
        mov    edx, eax
        shl    eax, 3
        sub    eax, edx
        mov    ax, word ptr [ebp + eax*2 - 66h]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ecx + 4f7h]
        mov    edx, eax
        shl    eax, 3
        sub    eax, edx
        mov    ax, word ptr [ebp + eax*2 - 68h]
        mov    word ptr [ecx + 0ah], ax
        mov    eax, dword ptr [ecx + 4f7h]
        mov    edx, eax
        shl    eax, 3
        sub    eax, edx
        mov    ax, word ptr [ebp + eax*2 - 64h]
        mov    word ptr [ecx + 0eh], ax
loc_2_17F7:
        mov    byte ptr [ecx + 29h], 0
        mov    dword ptr [ecx + 0ceh], 400h
        mov    dword ptr [ecx + 0d2h], 20000h
        mov    dword ptr [ecx + 4e7h], 0ah
        mov    dword ptr [ecx + 4ebh], 0ah
        mov    dword ptr [ecx + 4efh], 4b0h
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1840:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, 1
        mov    dword ptr [ebp - 4], 0
        mov    ebx, dword ptr [ebp + 8]
        cmp    ebx, 1fh
        ja     SHORT loc_2_1880
        mov    cl, bl
        mov    esi, dword ptr [ebp + 10h]
        shl    eax, cl
        test   dword ptr [esi + 10h], eax
        je     SHORT loc_2_1898
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
loc_2_1880:
        lea    ecx, [ebx - 20h]
        mov    esi, dword ptr [ebp + 10h]
        shl    eax, cl
        test   dword ptr [esi + 14h], eax
        je     SHORT loc_2_1898
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_1898:
        cmp    ebx, 1bh
        jbe    SHORT loc_2_18C0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 4fbh]
        push   ecx
        push   eax
        call   sub_2_0C30
        or     al, al
        je     SHORT loc_2_18C0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h
loc_2_18C0:
        cmp    ebx, 1fh
        ja     SHORT loc_2_18FA
        lea    edx, [ebp - 8]
loc_2_18C8:
        mov    eax, dword ptr [edx]
        cmp    ebx, eax
        jne    SHORT loc_2_18DD
        mov    edi, 1
        mov    cl, byte ptr [edx + 4]
        shl    edi, cl
        test   dword ptr [esi + 10h], edi
        jne    SHORT loc_2_1905
loc_2_18DD:
        cmp    dword ptr [edx + 4], ebx
        jne    SHORT loc_2_18F0
        mov    edi, 1
        mov    cl, al
        shl    edi, cl
        test   dword ptr [esi + 10h], edi
        jne    SHORT loc_2_1910
loc_2_18F0:
        add    edx, 8
        lea    eax, [ebp]
        cmp    edx, eax
        jb     SHORT loc_2_18C8
loc_2_18FA:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_1905:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_2_1910:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
sub_2_1920:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [edi], 0
        mov    ecx, dword ptr [esi + 4f7h]
        cmp    ecx, 7
        ja     SHORT loc_2_19A0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        push   ecx
        call   sub_2_1840
        or     al, al
        jne    SHORT loc_2_1963
        push   esi
        call   sub_2_15E0
        push   esi
        call   sub_2_0FD0
        or     al, al
        je     SHORT loc_2_19A0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
loc_2_1963:
        mov    dword ptr [edi], 0c001001eh
        mov    dword ptr [esi + 4bah], 0aa00000dh
        mov    dword ptr [esi + 4beh], 5
        mov    eax, dword ptr [esi + 4f7h]
        mov    dword ptr [esi + 4c6h], eax
        sub    al, al
        mov    dword ptr [esi + 4c2h], 2
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
        db  005h, 000h, 000h, 000h, 000h
loc_2_19A0:
        mov    dword ptr [edi], 0c0010006h
        mov    dword ptr [esi + 4bah], 0aa000004h
        mov    dword ptr [esi + 4beh], 5
        mov    eax, dword ptr [esi + 4f7h]
        mov    dword ptr [esi + 4c6h], eax
        sub    al, al
        mov    dword ptr [esi + 4c2h], 2
        pop    edi
        pop    esi
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_19E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 54h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 85h], 0
        lea    ebx, [esi + 95h]
        mov    dword ptr [esi + 91h], 0
        mov    dword ptr [ebx], 0
        cmp    dword ptr [esi + 3eh], 9
        jne    SHORT loc_2_1A20
        mov    dword ptr [esi + 91h], 4eh
        jmp    SHORT loc_2_1A27
        db  08Bh, 0FFh
loc_2_1A20:
        add    dword ptr [esi + 85h], 4eh
loc_2_1A27:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [esi + 85h]
        push   eax
        lea    eax, [esi + 89h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1A42:
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_2_1A54
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1A54:
        mov    eax, dword ptr [esi + 89h]
        mov    dword ptr [esi + 8dh], eax
        mov    edx, dword ptr [esi + 85h]
        mov    edi, eax
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        push   2000h
        lea    eax, [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1A8E:
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_2_1AB0
        push   0
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1A9F:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1AB0:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 2a9h], eax
        cmp    dword ptr [esi + 3eh], 9
        mov    eax, dword ptr [esi + 89h]
        jne    SHORT loc_2_1B10
        mov    dword ptr [esi + 99h], eax
        mov    eax, dword ptr [esi + 8dh]
        mov    dword ptr [esi + 9dh], eax
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [esi + 91h]
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1AEC:
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_2_1B04
        mov    dword ptr [ebx], 0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1B04:
        mov    edx, dword ptr [ebx]
        mov    ecx, edx
        jmp    SHORT loc_2_1B34
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_2_1B10:
        add    eax, 4eh
        mov    dword ptr [esi + 99h], eax
        mov    eax, dword ptr [esi + 8dh]
        add    eax, 4eh
        mov    dword ptr [esi + 9dh], eax
        mov    ecx, dword ptr [esi + 89h]
        mov    edx, dword ptr [esi + 8dh]
loc_2_1B34:
        mov    dword ptr [esi + 65h], ecx
        lea    eax, [ecx + 6]
        mov    dword ptr [esi + 69h], eax
        lea    eax, [ecx + 0eh]
        mov    dword ptr [esi + 6dh], eax
        add    ecx, 2eh
        mov    dword ptr [esi + 71h], ecx
        mov    dword ptr [esi + 75h], edx
        lea    eax, [edx + 6]
        mov    dword ptr [esi + 79h], eax
        lea    eax, [edx + 0eh]
        mov    dword ptr [esi + 7dh], eax
        lea    eax, [edx + 2eh]
        mov    dword ptr [esi + 81h], eax
        add    edx, 3ch
        mov    eax, edx
        shr    eax, 10h
        mov    cx, dx
        and    ecx, 0ffh
        shl    ecx, 10h
        mov    cl, al
        shl    ecx, 8
        shr    dx, 8
        and    edx, 0ffffh
        shl    edx, 10h
        or     ecx, edx
        shr    ax, 8
        and    eax, 0ffffh
        or     ecx, eax
        mov    eax, dword ptr [esi + 6dh]
        push   0
        mov    dword ptr [eax + 1ch], ecx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1BA2:
        lea    edi, [esi + 629h]
        push   0
        push   1
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1BB6:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_1BD1
        push   1
        push   1
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1BC6:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1BD1:
        mov    dword ptr [esi + 645h], 0
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [esi + 62dh]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1BEE:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_1C10
        push   1
        push   1
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1BFE:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1C10:
        mov    dword ptr [esi + 645h], 1
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        lea    eax, [esi + 639h]
        push   4bh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1C30:
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_2_1C50
        push   1
        push   1
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1C41:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_2_1C50:
        mov    dword ptr [esi + 645h], 2
        lea    ebx, [esi + 631h]
        push   1
        push   ebx
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1C6C:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_1C90
        push   1
        push   1
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1C7C:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_2_1C90:
        mov    dword ptr [esi + 645h], 3
        push   4bh
        mov    eax, dword ptr [esi + 639h]
        push   eax
        mov    eax, dword ptr [ebx]
        lea    edi, [esi + 635h]
        push   eax
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1CB6:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_2_1CD1
        push   1
        push   1
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1CC6:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1CD1:
        mov    dword ptr [esi + 645h], 4
        mov    byte ptr [ebp - 54h], 10h
        mov    byte ptr [ebp - 53h], 40h
        mov    byte ptr [ebp - 52h], 0c0h
        mov    byte ptr [ebp - 51h], 0
        mov    byte ptr [ebp - 50h], 0
        mov    byte ptr [ebp - 4fh], 0
        mov    byte ptr [ebp - 4eh], 20h
        mov    byte ptr [ebp - 4dh], 0
        mov    byte ptr [ebp - 4ch], 80h
        mov    byte ptr [ebp - 4bh], 0
        mov    byte ptr [ebp - 4ah], 0
        mov    byte ptr [ebp - 49h], 0
        mov    byte ptr [ebp - 48h], 0
        mov    byte ptr [ebp - 47h], 0
        mov    byte ptr [ebp - 46h], 0c2h
        mov    byte ptr [ebp - 45h], 70h
        mov    byte ptr [ebp - 44h], 0f4h
        mov    byte ptr [ebp - 43h], 0f4h
        mov    byte ptr [ebp - 42h], 3
        mov    byte ptr [ebp - 41h], 0
        mov    byte ptr [ebp - 40h], 38h
        mov    byte ptr [ebp - 3fh], 84h
        mov    byte ptr [ebp - 3eh], 1
        mov    byte ptr [ebp - 3dh], 0
        mov    byte ptr [ebp - 3ch], 0ah
        mov    byte ptr [ebp - 3bh], 40h
        mov    byte ptr [ebp - 3ah], 3
        mov    byte ptr [ebp - 39h], 0
        mov    byte ptr [ebp - 38h], 0
        mov    byte ptr [ebp - 37h], 0
        mov    byte ptr [ebp - 36h], 0
        mov    byte ptr [ebp - 35h], 0
        mov    byte ptr [ebp - 34h], 0
        mov    byte ptr [ebp - 33h], 0
        mov    byte ptr [ebp - 32h], 6
        mov    byte ptr [ebp - 31h], 0c0h
        mov    byte ptr [ebp - 30h], 1
        mov    byte ptr [ebp - 2fh], 0
        mov    byte ptr [ebp - 2eh], 1
        mov    byte ptr [ebp - 2dh], 0
        mov    byte ptr [ebp - 2ch], 6
        mov    byte ptr [ebp - 2bh], 0c0h
        mov    byte ptr [ebp - 2ah], 2
        mov    byte ptr [ebp - 29h], 80h
        mov    byte ptr [ebp - 28h], 0
        mov    byte ptr [ebp - 27h], 0
        mov    byte ptr [ebp - 26h], 0ch
        mov    byte ptr [ebp - 25h], 0c0h
        mov    byte ptr [ebp - 24h], 0bh
        mov    byte ptr [ebp - 23h], 0
        mov    byte ptr [ebp - 22h], 0
        mov    byte ptr [ebp - 21h], 0
        mov    byte ptr [ebp - 20h], 0
        mov    byte ptr [ebp - 1fh], 0
        mov    byte ptr [ebp - 1eh], 0
        mov    byte ptr [ebp - 1dh], 0
        mov    byte ptr [ebp - 1ch], 0
        mov    byte ptr [ebp - 1bh], 0
        mov    byte ptr [ebp - 1ah], 12h
        mov    byte ptr [ebp - 19h], 40h
        mov    byte ptr [ebp - 18h], 0ch
        mov    byte ptr [ebp - 17h], 0
        mov    byte ptr [ebp - 16h], 32h
        mov    byte ptr [ebp - 15h], 30h
        mov    byte ptr [ebp - 14h], 32h
        mov    byte ptr [ebp - 13h], 30h
        mov    byte ptr [ebp - 12h], 32h
        mov    byte ptr [ebp - 11h], 30h
        mov    byte ptr [ebp - 10h], 39h
        sub    eax, eax
        mov    byte ptr [ebp - 0fh], 36h
        mov    byte ptr [ebp - 0eh], 58h
        mov    byte ptr [ebp - 0dh], 35h
        mov    byte ptr [ebp - 0ch], 37h
        mov    byte ptr [ebp - 0bh], 36h
        mov    byte ptr [ebp - 0ah], 0
        lea    esp, [esp]
loc_2_1E10:
        mov    cl, byte ptr [ebp + eax - 54h]
        mov    edx, dword ptr [esi + 639h]
        mov    byte ptr [eax + edx], cl
        inc    eax
        cmp    eax, 4bh
        jb     SHORT loc_2_1E10
        mov    ecx, dword ptr [esi + 639h]
        mov    ax, word ptr [esi + 54h]
        mov    word ptr [ecx + 35h], ax
        mov    ecx, dword ptr [esi + 639h]
        mov    eax, dword ptr [esi + 50h]
        mov    dword ptr [ecx + 31h], eax
        mov    ax, word ptr [esi + 54h]
        mov    ecx, dword ptr [esi + 639h]
        lea    ebx, [esi + 67fh]
        mov    word ptr [ecx + 0ch], ax
        mov    ecx, dword ptr [esi + 639h]
        mov    eax, dword ptr [esi + 50h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esi + 639h]
        or     byte ptr [eax + 8], 80h
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        push   20h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1E77:
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_2_1E93
        push   1
        push   1
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1E88:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1E93:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 18h], 8002h
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 14h], 1
        mov    dword ptr [esi + 645h], 5
        mov    eax, dword ptr [edi]
        push   eax
        mov    eax, dword ptr [esi + 62dh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1EC3:
        mov    dword ptr [esi + 645h], 6
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_1EE0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    si, word ptr [eax + 12h]
        add    si, 2
        mov    dx, si
        in     al, dx
        and    al, 0cfh
        or     al, byte ptr [ebp + 0ch]
        mov    bl, al
        out    dx, al
        push   1388h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1F06:
        or     bl, 20h
        mov    dx, si
        mov    al, bl
        out    dx, al
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1F20:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 12h]
        add    ax, 2
        mov    word ptr [ebp - 2], ax
        mov    al, byte ptr [ebp + 0ch]
        and    ax, 3fh
        or     ax, 180h
        mov    word ptr [ebp - 4], ax
        mov    dx, word ptr [ebp - 2]
        in     al, dx
        and    al, 0fh
        or     al, 60h
        out    dx, al
        push   0
        push   esi
        call   sub_2_1EE0
        mov    bx, 100h
        mov    edi, 9
loc_2_1F62:
        mov    ax, bx
        and    ax, word ptr [ebp - 4]
        cmp    ax, 1
        cmc
        sbb    al, al
        and    al, 10h
        push   eax
        push   esi
        call   sub_2_1EE0
        shr    bx, 1
        dec    edi
        jne    SHORT loc_2_1F62
        push   1388h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1F89:
        mov    dx, word ptr [ebp - 2]
        in     al, dx
        sub    di, di
        mov    ebx, 0fh
        mov    ax, dx
        inc    ax
        mov    word ptr [ebp - 8], ax
        nop
loc_2_1FA0:
        push   1388h
        db  0E8h, 000h, 000h, 000h, 000h
loc_2_1FAA:
        push   0
        push   esi
        call   sub_2_1EE0
        mov    dx, word ptr [ebp - 8]
        in     al, dx
        and    ax, 0ffh
        and    ax, 80h
        cmp    ax, 1
        mov    cl, bl
        sbb    eax, eax
        inc    eax
        shl    ax, cl
        or     di, ax
        dec    ebx
        jns    SHORT loc_2_1FA0
        mov    dx, word ptr [ebp - 2]
        in     al, dx
        and    al, 2fh
        out    dx, al
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_1FF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    word ptr [ebp - 2], 0
        lea    edi, [esi + 4ah]
        sub    ebx, ebx
loc_2_2007:
        lea    eax, [ebx + 3ch]
        push   eax
        push   esi
        call   sub_2_1F20
        mov    word ptr [edi], ax
        add    edi, 2
        mov    ch, al
        mov    cl, ah
        add    word ptr [ebp - 2], cx
        inc    ebx
        cmp    ebx, 3
        jb     SHORT loc_2_2007
        push   3fh
        push   esi
        call   sub_2_1F20
        cmp    word ptr [esi + 54h], 0
        jne    SHORT loc_2_2048
        cmp    dword ptr [esi + 50h], 0
        jne    SHORT loc_2_2048
        mov    cx, word ptr [esi + 4eh]
        mov    word ptr [esi + 54h], cx
        mov    ecx, dword ptr [esi + 4ah]
        mov    dword ptr [esi + 50h], ecx
loc_2_2048:
        cmp    word ptr [esi + 4eh], 0
        jne    SHORT loc_2_2055
        cmp    dword ptr [esi + 4ah], 0
        je     SHORT loc_2_206A
loc_2_2055:
        mov    ch, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        cmp    cx, ax
        jne    SHORT loc_2_206A
        test   byte ptr [esi + 4ah], 80h
        jne    SHORT loc_2_206A
        mov    byte ptr [esi + 56h], 1
loc_2_206A:
        mov    word ptr [esi + 57h], ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2080:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 12h]
        mov    byte ptr [esi + 56h], 0
        cmp    dword ptr [esi + 3eh], 5
        je     SHORT loc_2_20A0
        sub    al, al
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_2_20A0:
        lea    edx, [edi + 1]
        in     al, dx
        mov    cl, al
        and    ecx, 7
        db  08Bh, 00Ch, 08Dh, 024h, 000h, 000h, 000h
        mov    dword ptr [esi + 20h], ecx
        mov    cl, al
        and    cl, 38h
        shr    cl, 1
        and    ecx, 0ffh
        lea    edx, [edi + 3]
        db  08Bh, 089h, 044h, 000h, 000h, 000h
        mov    dword ptr [esi + 1ah], ecx
        in     al, dx
        and    al, 1
        cmp    al, 1
        mov    byte ptr [esi + 18h], 0
        sbb    eax, eax
        and    eax, 0fffe2b40h
        add    eax, 27100h
        mov    dword ptr [esi + 35h], eax
        push   esi
        call   sub_2_1FF0
        mov    al, byte ptr [esi + 56h]
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2100:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        lea    ecx, [eax + 53dh]
        mov    dword ptr [eax + 541h], ecx
        mov    dword ptr [ecx], ecx
        mov    dword ptr [eax + 539h], 0
        mov    dword ptr [eax + 54dh], 0
        mov    dword ptr [eax + 549h], 0
        mov    dword ptr [eax + 561h], 8
        mov    byte ptr [eax + 55fh], 0
        mov    dword ptr [eax + 589h], 9
        mov    byte ptr [eax + 587h], 0
        mov    dword ptr [eax + 5b1h], 0bh
        mov    byte ptr [eax + 5afh], 0
        mov    dword ptr [eax + 601h], 4
        mov    byte ptr [eax + 5ffh], 0
        mov    dword ptr [eax + 5d9h], 0ah
        mov    byte ptr [eax + 5d7h], 0
        mov    dword ptr [eax + 667h], 0bh
        mov    byte ptr [eax + 665h], 0
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_2_21A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   edi
        db  0A1h, 07Ch, 077h, 000h, 000h
        push   eax
        push   0
        lea    eax, [ebp - 4]
        push   32h
        push   eax
        db  0E8h, 0ECh, 0E6h, 0FDh, 0FFh
        mov    ecx, dword ptr [ebp + 20h]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], edx
        or     eax, eax
        je     SHORT loc_2_21D0
        sub    al, al
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  090h
loc_2_21D0:
        mov    edi, dword ptr [ebp - 4]
        sub    eax, eax
        mov    ecx, 0ch
        rep stosd
        stosw
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 18h]
        mov    dword ptr [eax + 2eh], 0ffffffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [edx]
        mov    byte ptr [ecx + 18h], al
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [ecx + 19h], al
        add    ecx, 18h
        mov    al, byte ptr [edx + 2]
        mov    byte ptr [ecx + 2], al
        mov    al, byte ptr [edx + 3]
        mov    byte ptr [ecx + 3], al
        mov    al, byte ptr [edx + 4]
        mov    byte ptr [ecx + 4], al
        mov    al, byte ptr [edx + 5]
        mov    byte ptr [ecx + 5], al
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 1eh], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 22h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 26h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx + 2ah], eax
        mov    al, 1
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2270:
        push   ebp
        mov    ebp, esp
        push   0
        push   32h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 02Ch, 0E6h, 0FDh, 0FFh
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2290:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 2eh], 0
        jne    SHORT loc_2_22A9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_22A9:
        db  0A1h, 07Ch, 077h, 000h, 000h
        push   eax
        push   0
        push   1fh
        lea    eax, [ebp - 4]
        push   eax
        db  0E8h, 0EAh, 0E5h, 0FDh, 0FFh
        or     eax, eax
        je     SHORT loc_2_22C9
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_2_22C9:
        sub    ecx, ecx
        mov    eax, dword ptr [esi + 2eh]
        mov    edi, edi
loc_2_22D0:
        mov    edx, 1
        shl    edx, cl
        test   eax, edx
        jne    SHORT loc_2_22E3
        inc    ecx
        cmp    ecx, 20h
        jb     SHORT loc_2_22D0
        jmp    SHORT loc_2_22F2
loc_2_22E3:
        mov    dword ptr [ebp - 8], ecx
        mov    eax, 1
        shl    eax, cl
        not    eax
        and    dword ptr [esi + 2eh], eax
loc_2_22F2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 14h]
        mov    dword ptr [eax + 10h], ecx
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_2_2308
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], ecx
loc_2_2308:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [eax + 1ch], cl
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1dh], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1eh], 0
        mov    eax, dword ptr [ebp - 4]
        pop    esi
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        mov    al, 1
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  090h
sub_2_2370:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_2690
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_2_2399
        cmp    ebx, 103h
        jne    SHORT loc_2_23B1
loc_2_2399:
        push   0
        db  068h, 078h, 077h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_2_2460
        or     eax, eax
        je     SHORT loc_2_23B1
        mov    ebx, eax
loc_2_23B1:
        or     ebx, ebx
        je     SHORT loc_2_23BD
        cmp    ebx, 103h
        jne    SHORT loc_2_23E8
loc_2_23BD:
        cmp    byte ptr [edi + 1dh], 0
        je     SHORT loc_2_23E8
        dec    dword ptr [esi + 0ch]
        mov    byte ptr [edi + 1dh], 0
        cmp    dword ptr [esi + 0ch], 0
        jne    SHORT loc_2_23E8
        push   0
        db  068h, 078h, 077h, 000h, 000h
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   edi
        push   esi
        call   sub_2_2500
        or     eax, eax
        je     SHORT loc_2_23E8
        mov    ebx, eax
loc_2_23E8:
        or     ebx, ebx
        je     SHORT loc_2_23F4
        cmp    ebx, 103h
        jne    SHORT loc_2_2455
loc_2_23F4:
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_2_2450
        mov    eax, dword ptr [edi + 10h]
        or     eax, eax
        je     SHORT loc_2_240A
        mov    ecx, dword ptr [edi + 14h]
        mov    dword ptr [eax + 14h], ecx
loc_2_240A:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_2_2419
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_2_241F
loc_2_2419:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 14h], eax
loc_2_241F:
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_2_242E
        mov    eax, dword ptr [edi + 4]
        push   eax
        db  0E8h, 0DAh, 0E3h, 0FDh, 0FFh
loc_2_242E:
        mov    eax, 1
        mov    cl, byte ptr [edi + 1ch]
        shl    eax, cl
        or     dword ptr [esi + 2eh], eax
        push   0
        push   1fh
        push   edi
        db  0E8h, 067h, 0E4h, 0FDh, 0FFh
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_2_2450:
        mov    ebx, 0c001000eh
loc_2_2455:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_2_2460:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        xor    edx, edx
        mov    ebx, dword ptr [esi + 4]
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 14h]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 2]
        mov    dl, byte ptr [eax]
        shl    edx, 10h
        or     ecx, edx
        shl    ecx, 8
        xor    edx, edx
        mov    dl, byte ptr [eax + 1]
        shl    edx, 10h
        or     ecx, edx
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        or     ecx, eax
        mov    eax, ecx
        mov    dword ptr [edi + 0ch], eax
        mov    eax, dword ptr [esi + 14h]
        mov    dword ptr [esi + 4], 0
        or     eax, eax
        je     SHORT loc_2_24C0
loc_2_24B3:
        mov    ecx, dword ptr [eax + 0ch]
        or     dword ptr [esi + 4], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_24B3
loc_2_24C0:
        mov    ecx, dword ptr [esi + 4]
        cmp    ecx, ebx
        je     SHORT loc_2_24F4
        mov    al, byte ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 1eh]
        or     eax, eax
        je     SHORT loc_2_24F6
        cmp    eax, 103h
        je     SHORT loc_2_24F6
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0ch], ecx
        mov    dword ptr [esi + 4], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_24F4:
        sub    eax, eax
loc_2_24F6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  090h
sub_2_2500:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        xor    eax, eax
        mov    ecx, dword ptr [esi + 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [ebp + 14h]
        xor    edx, edx
        mov    dl, byte ptr [ecx]
        mov    al, byte ptr [ecx + 2]
        shl    edx, 10h
        or     edx, eax
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [ecx + 1]
        shl    eax, 10h
        or     edx, eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 3]
        or     edx, eax
        mov    dword ptr [ebp - 4], edx
        db  0A0h, 078h, 077h, 000h, 000h
        cmp    byte ptr [ecx], al
        jne    SHORT loc_2_25A8
        db  0A0h, 079h, 077h, 000h, 000h
        cmp    byte ptr [ecx + 1], al
        jne    SHORT loc_2_25A8
        db  0A0h, 07Ah, 077h, 000h, 000h
        cmp    byte ptr [ecx + 2], al
        jne    SHORT loc_2_25A8
        db  0A0h, 07Bh, 077h, 000h, 000h
        cmp    byte ptr [ecx + 3], al
        jne    SHORT loc_2_25A8
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_2_2589
        dec    dword ptr [esi + 0ch]
        mov    byte ptr [ebx + 1dh], 0
        cmp    dword ptr [esi + 0ch], 0
        je     NEAR PTR loc_2_2623
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_2589:
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_2_259D
        mov    eax, 0c001001ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_259D:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_25A8:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi + 8], eax
        jne    SHORT loc_2_25E0
        mov    ebx, dword ptr [ebp + 0ch]
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_2_25C4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_25C4:
        cmp    dword ptr [esi + 0ch], 0
        je     SHORT loc_2_2623
        mov    byte ptr [ebx + 1dh], 1
        inc    dword ptr [esi + 0ch]
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
loc_2_25E0:
        mov    eax, dword ptr [esi + 0ch]
        cmp    eax, 1
        jbe    SHORT loc_2_25F6
        mov    eax, 0c001001ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_25F6:
        mov    ebx, dword ptr [ebp + 0ch]
        jne    SHORT loc_2_2610
        cmp    byte ptr [ebx + 1dh], 0
        jne    SHORT loc_2_2610
        mov    eax, 0c001001ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  090h
loc_2_2610:
        cmp    eax, 1
        jne    SHORT loc_2_2623
        cmp    byte ptr [ebx + 1dh], 0
        je     SHORT loc_2_2623
        dec    eax
        mov    dword ptr [esi + 0ch], eax
        mov    byte ptr [ebx + 1dh], 0
loc_2_2623:
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 8], ecx
        mov    al, byte ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebx]
        push   eax
        push   ecx
        push   edi
        call   dword ptr [esi + 22h]
        or     eax, eax
        je     SHORT loc_2_2656
        cmp    eax, 103h
        je     SHORT loc_2_2656
        mov    dword ptr [esi + 8], edi
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 0ch], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_2656:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_2_2670
        mov    byte ptr [ebx + 1dh], 0
        mov    dword ptr [esi + 0ch], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_2670:
        mov    byte ptr [ebx + 1dh], 1
        mov    dword ptr [esi + 0ch], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_2690:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [esi + 10h]
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        mov    dword ptr [esi + 10h], 0
        je     SHORT loc_2_26CD
        lea    esp, [esp]
loc_2_26C0:
        mov    ecx, dword ptr [eax + 8]
        or     dword ptr [esi + 10h], ecx
        mov    eax, dword ptr [eax + 10h]
        or     eax, eax
        jne    SHORT loc_2_26C0
loc_2_26CD:
        mov    ecx, dword ptr [esi + 10h]
        cmp    ebx, ecx
        je     SHORT loc_2_2701
        mov    al, byte ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        push   ecx
        push   ebx
        call   dword ptr [esi + 26h]
        or     eax, eax
        je     SHORT loc_2_2703
        cmp    eax, 103h
        je     SHORT loc_2_2703
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [esi + 10h], ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_2_2701:
        sub    eax, eax
loc_2_2703:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h
_PTEXT ends
        end
