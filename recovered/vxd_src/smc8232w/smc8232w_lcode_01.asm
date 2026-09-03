; smc8232w_lcode_01.asm -- SMC8232W object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  055h, 0B8h, 009h, 000h, 000h, 000h, 08Bh, 0ECh, 083h, 0ECh, 01Ch, 066h, 089h, 045h, 0F0h, 053h
        db  066h, 089h, 045h, 0F2h, 033h, 0DBh, 066h, 089h, 05Dh, 0E6h, 053h, 08Bh, 04Dh, 00Ch, 08Dh, 045h
        db  0E4h, 051h, 08Dh, 055h, 0F8h, 0C7h, 045h, 0F4h, 000h, 000h, 000h, 000h, 0C7h, 045h, 0E8h, 04Dh
        db  0A3h, 080h, 010h, 066h, 0C7h, 045h, 0E4h, 001h, 000h, 050h, 052h, 0E8h, 012h, 045h, 000h, 000h
        db  08Dh, 04Dh, 0E4h, 089h, 00Dh, 04Ah, 000h, 000h, 000h, 08Bh, 055h, 0F8h, 089h, 015h, 004h, 000h
        db  000h, 000h, 088h, 01Dh, 04Eh, 000h, 000h, 000h, 089h, 01Dh, 03Eh, 000h, 000h, 000h, 089h, 01Dh
        db  042h, 000h, 000h, 000h, 0C6h, 005h, 008h, 000h, 000h, 000h, 003h, 088h, 01Dh, 009h, 000h, 000h
        db  000h, 089h, 01Dh, 00Ah, 000h, 000h, 000h, 0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 0C7h, 005h, 012h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 016h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 01Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h
        db  005h, 01Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Bh, 045h, 0F0h, 08Bh, 04Dh, 0F4h, 0C7h
        db  005h, 022h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 026h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 0C7h, 005h, 02Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 02Eh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 032h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 06Ah, 036h, 0A3h, 036h, 000h, 000h, 000h, 068h, 008h, 000h, 000h, 000h, 08Dh, 045h, 0FCh
        db  068h, 000h, 000h, 000h, 000h, 089h, 00Dh, 03Ah, 000h, 000h, 000h, 08Bh, 055h, 0F8h, 052h, 068h
        db  000h, 000h, 000h, 000h, 050h, 0E8h, 054h, 044h, 000h, 000h, 039h, 05Dh, 0FCh, 074h, 014h, 053h
        db  08Bh, 045h, 0F8h, 050h, 0E8h, 03Fh, 044h, 000h, 000h, 08Bh, 045h, 0FCh, 05Bh, 08Bh, 0E5h, 05Dh
        db  0C2h, 008h, 000h, 08Bh, 045h, 0FCh, 05Bh, 08Bh, 0E5h, 05Dh, 0C2h, 008h, 000h, 08Dh, 049h, 000h
loc_1_0130:
        push   ebp
        mov    eax, 0dh
        mov    ebp, esp
        sub    esp, 1b4h
        mov    word ptr [ebp - 4ch], ax
        push   ebx
        mov    word ptr [ebp - 4ah], ax
        push   esi
        push   edi
        mov    eax, 4
        db  0C7h, 045h, 0B8h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0C8h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D0h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 3ch], ax
        mov    word ptr [ebp - 3ah], ax
        mov    eax, 0bh
loc_1_0170:
        mov    word ptr [ebp - 34h], ax
        xor    ebx, ebx
        mov    word ptr [ebp - 32h], ax
        mov    byte ptr [ebp - 2], bl
        mov    eax, 8
        mov    byte ptr [ebp - 1], bl
        mov    word ptr [ebp - 44h], ax
        mov    dword ptr [ebp - 18h], ebx
        mov    word ptr [ebp - 42h], ax
        mov    byte ptr [ebp - 2ch], bl
        lea    eax, [ebp - 2bh]
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        lea    edx, [ebp - 10h]
        push   edx
        mov    dword ptr [eax], ebx
        db  0C7h, 045h, 0C0h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 1ch], 10h
        mov    byte ptr [eax + 4], bl
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_46BC
        cmp    dword ptr [ebp - 0ch], ebx
        je     SHORT loc_1_01CD
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_01CD:
        push   0
        lea    eax, [ebp - 4ch]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_46C2
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_01F3
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebp - 1ch], ecx
loc_1_01F3:
        push   1
        lea    eax, [ebp - 44h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_46C2
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_022E
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, 3
        jne    SHORT loc_1_0221
        mov    byte ptr [ebp - 1], 1
        jmp    SHORT loc_1_022E
loc_1_0221:
        sub    eax, 2
        cmp    eax, 1
        sbb    al, al
        and    al, 2
        mov    byte ptr [ebp - 1], al
loc_1_022E:
        mov    eax, dword ptr [ebp - 10h]
        lea    ecx, [ebp - 24h]
        push   eax
        lea    edx, [ebp - 20h]
        push   ecx
        lea    eax, [ebp - 0ch]
        push   edx
        push   eax
        call   sub_1_45C4
        cmp    dword ptr [ebp - 24h], 6
        jne    SHORT loc_1_0265
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_0265
        mov    ecx, dword ptr [ebp - 20h]
        mov    eax, ecx
        mov    edx, dword ptr [ecx]
        mov    dword ptr [ebp - 2ch], edx
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [ebp - 28h], al
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [ebp - 27h], cl
loc_1_0265:
        lea    eax, [ebp - 1b4h]
        lea    ecx, [ebp - 18h]
        push   eax
        mov    edx, dword ptr [ebp + 0ch]
        push   ecx
        lea    eax, [ebp - 0ch]
        push   edx
        push   eax
        call   sub_1_45BE
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_028C
        mov    byte ptr [ebp - 2], 1
        mov    ebx, 0aa000004h
loc_1_028C:
        mov    esi, dword ptr [ebp - 18h]
        db  0A1h, 000h, 000h, 000h, 000h
        shl    si, 0ch
        push   eax
        or     si, 0c80h
        push   0
        push   332h
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_1_463E
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        je     SHORT loc_1_02BE
        mov    byte ptr [ebp - 2], 1
        mov    ebx, 0aa000002h
loc_1_02BE:
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 0cch
        rep stosd
        stosw
        lea    edi, [ebp - 74h]
        mov    ecx, 9
        rep stosd
        stosw
        mov    byte ptr [ebp - 70h], 1
        mov    ecx, esi
        mov    dword ptr [ebp - 74h], eax
        mov    eax, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        and    eax, 0ffffh
        mov    dword ptr [ebp - 52h], ecx
        mov    dword ptr [ebp - 6ah], 2
        shl    eax, 0ch
        lea    edx, [ebp - 74h]
        push   edx
loc_1_0300:
        mov    dword ptr [ebp - 5ah], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 56h], 40h
        mov    dword ptr [ebp - 66h], 64h
        mov    dword ptr [ebp - 62h], 5eah
        push   edx
        push   eax
        lea    eax, [edx + 4eh]
        push   eax
        call   sub_1_456A
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        je     SHORT loc_1_0361
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_46C8
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   332h
        push   eax
        call   sub_1_4644
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0361:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_1_03AC
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   ebx
        mov    ecx, dword ptr [eax + 4eh]
        push   ecx
        call   sub_1_4710
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        call   sub_1_46C8
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + 4eh]
        push   edx
        call   sub_1_4570
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   332h
        push   ecx
        call   sub_1_4644
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_03AC:
        mov    eax, dword ptr [ebp - 8]
        mov    cl, byte ptr [ebp - 1]
        mov    word ptr [eax + 4], si
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], cl
        mov    eax, dword ptr [ebp - 8]
        mov    dl, byte ptr [ebp - 18h]
        mov    byte ptr [eax + 1], dl
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 52h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 0c3h], 4000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2A80
        cmp    ax, 0ffffh
        jne    SHORT loc_1_042F
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0aa000004h
        mov    ecx, dword ptr [eax + 4eh]
        push   ecx
        call   sub_1_4710
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        call   sub_1_46C8
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + 4eh]
        push   edx
        call   sub_1_4570
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   332h
        push   ecx
        call   sub_1_4644
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_042F:
        cmp    ax, 2
        jne    SHORT loc_1_0488
        push   0
        lea    eax, [ebp - 3ch]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_46C2
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_045F
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 4]
        mov    word ptr [ecx + 0ah], dx
loc_1_045F:
        push   1
        lea    eax, [ebp - 34h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_46C2
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_0488
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 18h], edx
loc_1_0488:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_46C8
        lea    eax, [ebp - 2ch]
        mov    ecx, dword ptr [ebp - 1ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   2
        push   edx
        call   sub_1_04E0
        test   eax, eax
        je     SHORT loc_1_04D2
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4eh]
        push   ecx
        call   sub_1_4570
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   332h
        push   ecx
        call   sub_1_4644
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_04D2:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
sub_1_04E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        cmp    edi, 2
        mov    dword ptr [esi + 17ah], ebx
        jbe    SHORT loc_1_0523
        push   0
        mov    eax, dword ptr [esi + 4eh]
        push   0aa000002h
        push   eax
        call   sub_1_4710
        mov    dword ptr [ebp - 4], 0c000009ah
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_0523:
        xor    edx, edx
        db  0B8h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 172h], edx
        mov    dword ptr [esi + 21eh], eax
        mov    dword ptr [esi + 212h], edx
        mov    dword ptr [esi + 216h], eax
        db  0C7h, 086h, 06Ah, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 21ah], esi
        db  08Bh, 00Dh, 042h, 000h, 000h, 000h
        mov    dword ptr [esi + 16eh], ecx
        db  089h, 035h, 042h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 0ah]
        mov    cl, byte ptr [esi]
        test   cl, cl
        je     SHORT loc_1_0577
        cmp    cl, 2
        mov    ecx, 2
        jne    SHORT loc_1_057C
loc_1_0577:
        mov    ecx, 1
loc_1_057C:
        movsx  eax, al
        push   ecx
        push   0
        push   eax
        mov    ecx, dword ptr [esi + 4eh]
        push   eax
        lea    edx, [esi + 0cbh]
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   edx
        push   eax
        call   sub_1_4626
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_05C8
        push   0
        mov    eax, dword ptr [esi + 4eh]
        push   0aa000005h
        push   eax
        call   sub_1_4710
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_05C8:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 18h]
        mov    ax, word ptr [esi + 20h]
        mov    edx, dword ptr [esi + 4eh]
        shl    eax, 0ah
        push   eax
        push   ecx
        push   edx
        lea    eax, [esi + 1ch]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_470A
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_0605
        push   0
        mov    eax, dword ptr [esi + 4eh]
        push   0aa000001h
        push   eax
        call   sub_1_4710
        add    esp, 0ch
        jmp    NEAR PTR loc_1_07F5
loc_1_0605:
        xor    edx, edx
        mov    ecx, dword ptr [ebp + 14h]
loc_1_060A:
        mov    al, byte ptr [ecx + edx]
        inc    edx
        cmp    edx, 6
        mov    byte ptr [esi + edx + 23h], al
        jb     SHORT loc_1_060A
        mov    word ptr [esi + 3ah], di
        lea    eax, [esi + 1cbh]
        lea    ecx, [esi + 1d3h]
        lea    edx, [esi + 1cfh]
        mov    dword ptr [esi + 5fh], eax
        mov    dword ptr [esi + 8fh], ecx
        lea    eax, [esi + 1c7h]
        lea    ecx, [esi + 1d7h]
        mov    dword ptr [esi + 63h], edx
        mov    dword ptr [esi + 93h], eax
        lea    edx, [esi + 1dbh]
        lea    eax, [esi + 1dfh]
        mov    dword ptr [esi + 97h], ecx
        mov    dword ptr [esi + 67h], edx
loc_1_0660:
        lea    ecx, [esi + 1e3h]
        lea    edx, [esi + 1e7h]
        mov    dword ptr [esi + 6bh], eax
        mov    dword ptr [esi + 6fh], ecx
        lea    eax, [esi + 1ebh]
        lea    ecx, [esi + 1efh]
        mov    dword ptr [esi + 73h], edx
        mov    dword ptr [esi + 77h], eax
        lea    edx, [esi + 1f3h]
        lea    eax, [esi + 1f7h]
        mov    dword ptr [esi + 7bh], ecx
        mov    dword ptr [esi + 7fh], edx
        lea    ecx, [esi + 1fbh]
        lea    edx, [esi + 1ffh]
        mov    dword ptr [esi + 83h], eax
        mov    dword ptr [esi + 87h], ecx
        lea    eax, [esi + 1b7h]
        lea    ecx, [esi + 1abh]
        mov    dword ptr [esi + 8bh], edx
        mov    dword ptr [esi + 9bh], eax
        lea    edx, [esi + 1afh]
        lea    eax, [esi + 1b3h]
        mov    dword ptr [esi + 9fh], ecx
        mov    dword ptr [esi + 0a3h], edx
        lea    ecx, [esi + 1bbh]
        lea    edx, [esi + 1bfh]
        mov    dword ptr [esi + 0a7h], eax
        mov    dword ptr [esi + 0abh], ecx
        lea    eax, [esi + 1c3h]
        mov    dword ptr [esi + 0afh], edx
        lea    edi, [esi + 162h]
        mov    dword ptr [esi + 0b7h], eax
        mov    dword ptr [esi + 15eh], 0
        mov    word ptr [esi + 36h], 5eah
        mov    word ptr [esi + 38h], 100h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        push   ecx
        push   2
        push   300h
        push   edi
        call   sub_1_463E
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_1_0752
        push   2
        mov    eax, dword ptr [edi]
        or     byte ptr [esi + 43h], 4
        push   300h
        push   eax
        call   sub_1_4644
loc_1_0752:
        and    word ptr [esi + 42h], 0fbffh
        push   esi
        call   sub_1_2BD0
        test   ax, ax
        je     SHORT loc_1_079B
        push   0
        mov    eax, dword ptr [esi + 4eh]
        mov    byte ptr [esi + 203h], 1
        push   0aa000003h
        push   eax
        call   sub_1_4710
        add    esp, 0ch
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0792
        push   2
        mov    eax, dword ptr [edi]
        push   300h
        push   eax
        call   sub_1_4644
loc_1_0792:
        mov    dword ptr [ebp - 4], 0c0010006h
        jmp    SHORT loc_1_07F5
loc_1_079B:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_07B0
        push   2
        mov    eax, dword ptr [edi]
        push   300h
        push   eax
        call   sub_1_4644
loc_1_07B0:
        lea    eax, [esi + 15eh]
        lea    ecx, [esi + 166h]
        push   eax
        lea    edx, [esi + 24h]
loc_1_07C0:
        push   ecx
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        db  0E8h, 029h, 0F8h, 0FFh, 0FFh
        test   al, al
        jne    SHORT loc_1_085A
        push   0
        mov    eax, dword ptr [esi + 4eh]
        push   0aa000002h
        push   eax
        call   sub_1_4710
        mov    dword ptr [ebp - 4], 0c0000001h
        add    esp, 0ch
loc_1_07F5:
        lea    eax, [esi + 0cbh]
        push   eax
        call   sub_1_462C
        db  03Bh, 035h, 042h, 000h, 000h, 000h
        jne    SHORT loc_1_0820
        mov    eax, dword ptr [esi + 16eh]
        pop    edi
        pop    esi
        db  0A3h, 042h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_0820:
        db  08Bh, 00Dh, 042h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 16eh], esi
        je     SHORT loc_1_083C
loc_1_082E:
        mov    ecx, dword ptr [ecx + 16eh]
        cmp    dword ptr [ecx + 16eh], esi
        jne    SHORT loc_1_082E
loc_1_083C:
        mov    eax, dword ptr [ecx + 16eh]
        pop    edi
        pop    esi
        pop    ebx
        mov    edx, dword ptr [eax + 16eh]
        mov    dword ptr [ecx + 16eh], edx
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_085A:
        db  0B9h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 325h]
        push   1388h
        mov    dword ptr [esi + 321h], ecx
        push   eax
        mov    dword ptr [eax], 0
        mov    dword ptr [esi + 329h], ecx
        mov    dword ptr [esi + 32dh], esi
        call   sub_1_461A
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_08A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [eax], 0
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 10h]
        mov    edx, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ecx], 0ffffffffh
        lea    eax, [eax + edx*4]
loc_1_08C4:
        test   edx, edx
        je     SHORT loc_1_08D3
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_08C4
        mov    dword ptr [ecx], edx
loc_1_08D3:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_1_08E5
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_08E5:
        mov    esi, dword ptr [ebp + 20h]
        cmp    byte ptr [esi + 203h], 0
        je     SHORT loc_1_08FE
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_08FE:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   75h
        push   ecx
        call   sub_1_463E
        test   eax, eax
        je     SHORT loc_1_0935
        push   0
        mov    eax, dword ptr [esi + 4eh]
        push   0aa000002h
        push   eax
        call   sub_1_4710
        add    esp, 0ch
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_0935:
        inc    dword ptr [esi + 176h]
        cmp    dword ptr [esi + 172h], 0
        jne    SHORT loc_1_096F
        push   esi
        call   sub_1_2F10
        test   ax, ax
        je     SHORT loc_1_096F
        push   0
        mov    eax, dword ptr [esi + 4eh]
        push   0aa000003h
        push   eax
        call   sub_1_4710
        add    esp, 0ch
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_096F:
        mov    eax, dword ptr [esi + 172h]
        mov    ecx, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 172h], ecx
        mov    ecx, dword ptr [esi + 15eh]
        mov    eax, dword ptr [ebp - 4]
        add    eax, 14h
        push   eax
        push   edi
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   ecx
        db  0E8h, 0D1h, 0F7h, 0FFh, 0FFh
        test   al, al
        jne    SHORT loc_1_09D5
        push   0
        dec    dword ptr [esi + 176h]
        mov    eax, dword ptr [ebp - 4]
        push   0aa000002h
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esi + 172h], ecx
        mov    edx, dword ptr [esi + 4eh]
        push   edx
        call   sub_1_4710
        add    esp, 0ch
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_09D5:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], esi
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 16ah]
        mov    dword ptr [eax + 10h], edx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 28h]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], edx
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 20h], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, 0eeh
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], eax
        mov    dword ptr [esi + 20eh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], 1
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [esi + 176h]
        dec    eax
        mov    dword ptr [esi + 176h], eax
        jne    SHORT loc_1_0A67
        cmp    byte ptr [esi + 204h], 0
        je     SHORT loc_1_0A42
        lea    eax, [esi + 31eh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_0A56
loc_1_0A42:
        cmp    dword ptr [esi + 18eh], 0
        je     SHORT loc_1_0A67
        lea    eax, [esi + 31eh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_0A67
loc_1_0A56:
        push   0
        add    esi, 212h
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_461A
loc_1_0A67:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A80:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 172h]
        test   edx, edx
        je     SHORT loc_1_0AA3
loc_1_0A93:
        mov    esi, dword ptr [edx + 18h]
        cmp    ecx, esi
        jae    SHORT loc_1_0A9C
        mov    ecx, esi
loc_1_0A9C:
        mov    edx, dword ptr [edx + 0ch]
        test   edx, edx
        jne    SHORT loc_1_0A93
loc_1_0AA3:
        test   ecx, ecx
        jne    SHORT loc_1_0AAC
        mov    ecx, 0eeh
loc_1_0AAC:
        push   eax
        mov    dword ptr [eax + 20eh], ecx
        call   sub_1_2BA0
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0AC0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 20h], 0
        mov    esi, dword ptr [ebx + 8]
        je     SHORT loc_1_0ADE
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0ADE:
        push   0
        inc    dword ptr [esi + 176h]
        inc    dword ptr [ebx + 1ch]
        mov    eax, dword ptr [ebx + 14h]
        mov    ecx, dword ptr [esi + 15eh]
        push   eax
        push   ecx
        db  0E8h, 047h, 0F7h, 0FFh, 0FFh
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_0B1E
        mov    eax, dword ptr [ebx + 1ch]
        cmp    eax, 2
        je     SHORT loc_1_0B16
        mov    byte ptr [ebx + 20h], 1
        sub    dword ptr [ebx + 1ch], 2
        mov    edi, 103h
        jmp    SHORT loc_1_0B44
loc_1_0B16:
        sub    eax, 2
        mov    dword ptr [ebx + 1ch], eax
        jmp    SHORT loc_1_0B44
loc_1_0B1E:
        cmp    edi, 103h
        jne    SHORT loc_1_0B30
        mov    byte ptr [ebx + 20h], 1
        sub    dword ptr [ebx + 1ch], 2
        jmp    SHORT loc_1_0B44
loc_1_0B30:
        cmp    edi, 0c001000eh
        jne    SHORT loc_1_0B41
        mov    byte ptr [ebx + 20h], 1
        mov    edi, 103h
loc_1_0B41:
        dec    dword ptr [ebx + 1ch]
loc_1_0B44:
        cmp    dword ptr [ebx + 1ch], 0
        jne    SHORT loc_1_0BB7
        mov    eax, dword ptr [esi + 172h]
        cmp    ebx, eax
        jne    SHORT loc_1_0B5F
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 172h], eax
        jmp    SHORT loc_1_0B72
loc_1_0B5F:
        cmp    dword ptr [eax + 0ch], ebx
        je     SHORT loc_1_0B6C
loc_1_0B64:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], ebx
        jne    SHORT loc_1_0B64
loc_1_0B6C:
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_0B72:
        mov    eax, dword ptr [ebx + 18h]
        cmp    dword ptr [esi + 20eh], eax
        jne    SHORT loc_1_0B83
        push   esi
        call   sub_1_0A80
loc_1_0B83:
        cmp    dword ptr [esi + 172h], 0
        jne    SHORT loc_1_0BBC
        push   esi
        lea    eax, [esi + 0cbh]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_4632
        test   al, al
        jne    SHORT loc_1_0BBC
        push   0
        mov    eax, dword ptr [esi + 4eh]
        push   0aa000003h
        push   eax
        call   sub_1_4710
        add    esp, 0ch
        jmp    SHORT loc_1_0BBC
loc_1_0BB7:
        mov    edi, 103h
loc_1_0BBC:
        mov    eax, dword ptr [esi + 176h]
        dec    eax
        mov    dword ptr [esi + 176h], eax
        jne    SHORT loc_1_0C04
        cmp    byte ptr [esi + 204h], 0
        je     SHORT loc_1_0BDF
        lea    eax, [esi + 31eh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_0BF3
loc_1_0BDF:
        cmp    dword ptr [esi + 18eh], 0
        je     SHORT loc_1_0C04
        lea    eax, [esi + 31eh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_0C04
loc_1_0BF3:
        push   0
        add    esi, 212h
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_461A
loc_1_0C04:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0C10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [edi + 1ch]
        inc    dword ptr [esi + 176h]
        cmp    byte ptr [edi + 20h], 0
        je     SHORT loc_1_0C37
        mov    dword ptr [ebp - 4], 0c0010002h
        jmp    SHORT loc_1_0C79
loc_1_0C37:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 10h]
        test   ecx, ecx
        jne    SHORT loc_1_0C4E
        push   eax
        push   edi
        push   esi
        call   sub_1_10E0
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_0C79
loc_1_0C4E:
        cmp    ecx, 1
        jne    SHORT loc_1_0C72
        cmp    byte ptr [esi + 203h], 0
        je     SHORT loc_1_0C65
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    SHORT loc_1_0C79
loc_1_0C65:
        push   eax
        push   edi
        push   esi
        call   sub_1_1130
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_0C79
loc_1_0C72:
        mov    dword ptr [ebp - 4], 10001h
loc_1_0C79:
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_0CE9
        mov    eax, dword ptr [ebx + 172h]
        cmp    eax, edi
        jne    SHORT loc_1_0C96
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 172h], eax
        jmp    SHORT loc_1_0CA9
loc_1_0C96:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_0CA3
loc_1_0C9B:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_0C9B
loc_1_0CA3:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_0CA9:
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [ebx + 20eh], eax
        jne    SHORT loc_1_0CBA
        push   ebx
        call   sub_1_0A80
loc_1_0CBA:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_457C
        push   0
        push   75h
        push   edi
        call   sub_1_4644
        cmp    dword ptr [ebx + 172h], 0
        jne    SHORT loc_1_0CE9
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 0cbh
        push   ebx
        call   sub_1_4632
loc_1_0CE9:
        mov    eax, dword ptr [esi + 176h]
        dec    eax
        mov    dword ptr [esi + 176h], eax
        jne    SHORT loc_1_0D31
        cmp    byte ptr [esi + 204h], 0
        je     SHORT loc_1_0D0C
        lea    eax, [esi + 31eh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_0D20
loc_1_0D0C:
        cmp    dword ptr [esi + 18eh], 0
        je     SHORT loc_1_0D31
        lea    eax, [esi + 31eh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_0D31
loc_1_0D20:
        push   0
        add    esi, 212h
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_461A
loc_1_0D31:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_1_0D40:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        mov    dword ptr [ebp - 18h], eax
        push   esi
        mov    dword ptr [ebp - 0ch], eax
        push   edi
        lea    esi, [ebp - 8]
        mov    edi, 4
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1010101h
        ja     SHORT loc_1_0D7D
        je     NEAR PTR loc_1_0F34
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_0D9E
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_0D7D:
        cmp    eax, 1010102h
        je     NEAR PTR loc_1_0F53
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_0F72
        cmp    eax, 1010104h
        je     NEAR PTR loc_1_1037
loc_1_0D9E:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_1043
loc_1_0DAA:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_0DBF
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 5ch
        jmp    NEAR PTR loc_1_1043
loc_1_0DBF:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 7ch
        jmp    NEAR PTR loc_1_1043
loc_1_0DCE:
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 14h], 4
        cmp    byte ptr [ebx + 203h], 0
        jne    SHORT loc_1_0DE8
        mov    dword ptr [ebp - 14h], 0
loc_1_0DE8:
        lea    esi, [ebp - 14h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_1043
loc_1_0DF5:
        lea    esi, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_1043
loc_1_0E02:
        mov    dword ptr [ebp - 8], 0eeh
        jmp    NEAR PTR loc_1_1043
loc_1_0E0E:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_1043
loc_1_0E1A:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_1_1043
loc_1_0E26:
        mov    ebx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 3ah]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0c3h]
        imul   ecx, eax
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_1043
loc_1_0E43:
        mov    ebx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 16h]
        xor    edx, edx
        mov    dx, word ptr [ebx + 3ah]
        xor    eax, eax
        shl    ecx, 0ah
        mov    ax, word ptr [ebx + 0c3h]
        imul   edx, eax
        sub    ecx, edx
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_1043
loc_1_0E6B:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 38h]
        jmp    NEAR PTR loc_1_1040
loc_1_0E79:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 38h]
        jmp    NEAR PTR loc_1_1040
loc_1_0E87:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 2ah
        lea    eax, [ebp - 8]
        push   ebx
        lea    esi, [ebp - 8]
        push   eax
        mov    edi, 4
        call   sub_1_4A20
        and    dword ptr [ebp - 8], 0ffffff00h
        jmp    NEAR PTR loc_1_1043
loc_1_0EAD:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 0dh
        jmp    NEAR PTR loc_1_1043
loc_1_0EBC:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_0ED6
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 15eh]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_1043
loc_1_0ED6:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 14h]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_1043
loc_1_0EE7:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_0EFB
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 20eh]
        jmp    NEAR PTR loc_1_1040
loc_1_0EFB:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_1043
loc_1_0F09:
        lea    esi, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 300h
        jmp    NEAR PTR loc_1_1043
loc_1_0F1C:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_1043
loc_1_0F28:
        mov    dword ptr [ebp - 8], 0eh
        jmp    NEAR PTR loc_1_1043
loc_1_0F34:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 2ah
        lea    eax, [ebp - 20h]
        push   ebx
        lea    esi, [ebp - 20h]
        push   eax
        mov    edi, 6
        call   sub_1_4A20
        jmp    NEAR PTR loc_1_1043
loc_1_0F53:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 24h
        lea    eax, [ebp - 20h]
        push   ebx
        lea    esi, [ebp - 20h]
        push   eax
        mov    edi, 6
        call   sub_1_4A20
        jmp    NEAR PTR loc_1_1043
loc_1_0F72:
        cmp    byte ptr [ebp + 14h], 0
        mov    ebx, dword ptr [ebp + 8]
        je     SHORT loc_1_0FD2
        mov    ecx, dword ptr [ebx + 15eh]
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    ebx, [eax + eax*2]
        lea    edx, [ebx*2]
        cmp    edx, dword ptr [ebp + 1ch]
        jbe    SHORT loc_1_0FA7
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [eax], edx
        jmp    NEAR PTR loc_1_1043
loc_1_0FA7:
        mov    eax, dword ptr [ebp + 18h]
        lea    edx, [ebp - 10h]
        push   eax
        mov    ebx, dword ptr [ebp + 1ch]
        push   edx
        lea    eax, [ebp - 0ch]
        push   ebx
        push   ecx
        push   eax
        db  0E8h, 0A3h, 0F7h, 0FFh, 0FFh
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ecx], eax
        je     SHORT loc_1_1049
        jmp    SHORT loc_1_1043
loc_1_0FD2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebx + 15eh]
        mov    ecx, dword ptr [eax + 14h]
        push   ecx
        push   edx
        db  0E8h, 07Bh, 0F6h, 0FFh, 0FFh
        lea    edx, [eax + eax*2]
        mov    dword ptr [ebp - 10h], eax
        lea    ecx, [edx*2]
        cmp    ecx, dword ptr [ebp + 1ch]
        jbe    SHORT loc_1_1005
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_1_1043
loc_1_1005:
        mov    eax, dword ptr [ebp + 18h]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp + 1ch]
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        push   edx
        mov    ecx, dword ptr [eax + 14h]
        push   ecx
        mov    edx, dword ptr [ebx + 15eh]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 078h, 0F6h, 0FFh, 0FFh
        mov    ecx, dword ptr [ebp - 10h]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 24h]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_1043
loc_1_1037:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 17ah]
loc_1_1040:
        mov    dword ptr [ebp - 8], eax
loc_1_1043:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_1082
loc_1_1049:
        cmp    dword ptr [ebp + 10h], 1010103h
        je     SHORT loc_1_1082
        cmp    edi, dword ptr [ebp + 1ch]
        jbe    SHORT loc_1_1072
        sub    edi, dword ptr [ebp + 1ch]
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_1_1072:
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   esi
        push   eax
        call   sub_1_4A20
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_1_1082:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Bh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh, 064h
        db  024h, 000h
sub_1_10E0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edx
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], ecx
        mov    eax, dword ptr [esi + 1ch]
        mov    edx, dword ptr [esi + 18h]
        push   eax
        push   edx
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 14h]
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   eax
        push   edx
        call   sub_1_0D40
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1130:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        xor    ebx, ebx
        mov    edi, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1010103h
        ja     SHORT loc_1_116D
        je     NEAR PTR loc_1_127C
        cmp    eax, 1010eh
        je     SHORT loc_1_117F
        cmp    eax, 1010fh
        je     SHORT loc_1_11DA
        cmp    eax, 10112h
        je     NEAR PTR loc_1_1255
loc_1_116D:
        mov    ebx, 0c00000bbh
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_12B7
loc_1_117F:
        cmp    edi, 4
        je     SHORT loc_1_1196
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_12B7
loc_1_1196:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_4A20
        test   dword ptr [ebp - 8], 0f050h
        je     SHORT loc_1_11C3
        mov    dword ptr [esi + 20h], 4
        mov    dword ptr [esi + 24h], 0
        mov    ebx, 0c00000bbh
        jmp    NEAR PTR loc_1_12B7
loc_1_11C3:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_1_12D0
        jmp    NEAR PTR loc_1_12B5
loc_1_11DA:
        cmp    edi, 4
        je     SHORT loc_1_11F1
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_12B7
loc_1_11F1:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_4A20
        cmp    dword ptr [ebp - 4], 0eeh
        ja     SHORT loc_1_124E
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 20eh]
        cmp    dword ptr [ebp - 4], eax
        jbe    SHORT loc_1_1228
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 20eh], eax
        mov    dword ptr [ecx + 18h], eax
        jmp    NEAR PTR loc_1_12B7
loc_1_1228:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    eax, dword ptr [ecx + 18h]
        jne    SHORT loc_1_1246
        mov    eax, dword ptr [ecx + 18h]
        cmp    dword ptr [ebp - 4], eax
        jae    SHORT loc_1_1246
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 18h], eax
        call   sub_1_0A80
        jmp    SHORT loc_1_12B7
loc_1_1246:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        jmp    SHORT loc_1_12B7
loc_1_124E:
        mov    ebx, 0c0010014h
        jmp    SHORT loc_1_12B7
loc_1_1255:
        cmp    edi, 4
        je     SHORT loc_1_1269
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_1_12B7
loc_1_1269:
        push   4
        xor    ebx, ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 71h
        push   ecx
        call   sub_1_4A20
        jmp    SHORT loc_1_12B7
loc_1_127C:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     SHORT loc_1_129A
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_1_12B7
loc_1_129A:
        push   ecx
        mov    eax, edi
        mov    ecx, 6
        sub    edx, edx
        div    ecx
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        call   sub_1_1310
loc_1_12B5:
        mov    ebx, eax
loc_1_12B7:
        test   ebx, ebx
        jne    SHORT loc_1_12C5
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [esi + 20h], edi
loc_1_12C5:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_1_12D0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_1_1305
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
        mov    edx, dword ptr [esi + 14h]
        push   edx
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [esi + 1ch]
        mov    ecx, dword ptr [eax + 15eh]
        push   ecx
        db  0E8h, 0E3h, 0F2h, 0FFh, 0FFh
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    10h
loc_1_1305:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
        db  090h
sub_1_1310:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_1_1349
        push   1
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    ecx, dword ptr [ebp + 14h]
        push   ecx
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        inc    dword ptr [esi + 1ch]
        mov    edx, dword ptr [ecx + 15eh]
        push   edx
        db  0E8h, 0BFh, 0EFh, 0FFh, 0FFh
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    14h
loc_1_1349:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1360:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 10h], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        lea    ecx, [ebp - 0ch]
        lea    edx, [ebp - 10h]
        mov    edi, dword ptr [esi + 1ch]
        mov    eax, dword ptr [esi + 18h]
        push   ecx
        mov    dword ptr [ebp - 14h], eax
        push   edx
        mov    ecx, dword ptr [esi + 14h]
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        push   eax
        push   1
        push   ecx
        push   0
        push   ebx
        call   sub_1_0D40
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0c00000bbh
        jne    NEAR PTR loc_1_1440
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1020101h
        ja     SHORT loc_1_13D0
        je     SHORT loc_1_140F
        sub    eax, 20101h
        cmp    eax, 4
        ja     SHORT loc_1_13DE
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_13D0:
        cmp    eax, 1020102h
        je     SHORT loc_1_1417
        cmp    eax, 1020103h
        je     SHORT loc_1_141F
loc_1_13DE:
        mov    dword ptr [ebp - 8], 0c00000bbh
        jmp    SHORT loc_1_1428
loc_1_13E7:
        mov    eax, dword ptr [ebx + 1a3h]
        jmp    SHORT loc_1_1425
loc_1_13EF:
        mov    eax, dword ptr [ebx + 1a7h]
        jmp    SHORT loc_1_1425
loc_1_13F7:
        mov    eax, dword ptr [ebx + 1abh]
        jmp    SHORT loc_1_1425
loc_1_13FF:
        mov    eax, dword ptr [ebx + 1cbh]
        jmp    SHORT loc_1_1425
loc_1_1407:
        mov    eax, dword ptr [ebx + 1cfh]
        jmp    SHORT loc_1_1425
loc_1_140F:
        mov    eax, dword ptr [ebx + 1c7h]
        jmp    SHORT loc_1_1425
loc_1_1417:
        mov    eax, dword ptr [ebx + 1afh]
        jmp    SHORT loc_1_1425
loc_1_141F:
        mov    eax, dword ptr [ebx + 1b3h]
loc_1_1425:
        mov    dword ptr [ebp - 4], eax
loc_1_1428:
        cmp    edi, 4
        jb     SHORT loc_1_1440
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 14h]
        push   ecx
        call   sub_1_4A20
        add    dword ptr [ebp - 0ch], 4
loc_1_1440:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        pop    edi
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 24h], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 08Dh, 064h, 024h, 000h
loc_1_1470:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        mov    eax, dword ptr [ecx + 14h]
        cmp    eax, 10113h
        ja     SHORT loc_1_1497
        je     SHORT loc_1_14C8
        cmp    eax, 10101h
        jb     SHORT loc_1_14C3
        cmp    eax, 10111h
        jbe    SHORT loc_1_14C8
        jmp    SHORT loc_1_14C3
loc_1_1497:
        cmp    eax, 1010104h
        ja     SHORT loc_1_14B5
        cmp    eax, 1010101h
        jae    SHORT loc_1_14C8
        cmp    eax, 20101h
        jb     SHORT loc_1_14C3
        cmp    eax, 20105h
        jbe    SHORT loc_1_14C8
        jmp    SHORT loc_1_14C3
loc_1_14B5:
        cmp    eax, 1020101h
        jb     SHORT loc_1_14C3
        cmp    eax, 1020103h
        jbe    SHORT loc_1_14C8
loc_1_14C3:
        mov    esi, 0c00000bbh
loc_1_14C8:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 176h]
        test   esi, esi
        jne    SHORT loc_1_14DE
        push   ecx
        push   edi
        call   sub_1_1360
        mov    esi, eax
loc_1_14DE:
        mov    eax, dword ptr [edi + 176h]
        dec    eax
        mov    dword ptr [edi + 176h], eax
        jne    SHORT loc_1_1526
        cmp    byte ptr [edi + 204h], 0
        je     SHORT loc_1_1501
        lea    eax, [edi + 31eh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_1515
loc_1_1501:
        cmp    dword ptr [edi + 18eh], 0
        je     SHORT loc_1_1526
        lea    eax, [edi + 31eh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_1526
loc_1_1515:
        push   0
        add    edi, 212h
        push   edi
        mov    byte ptr [eax], 1
        call   sub_1_461A
loc_1_1526:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_1530:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_2B90
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 325h]
        push   eax
        mov    edi, 300h
        push   ecx
        call   sub_1_4620
loc_1_1556:
        push   2710h
        call   sub_1_4638
        dec    edi
        jne    SHORT loc_1_1556
        mov    byte ptr [esi + 320h], 1
        db  03Bh, 035h, 042h, 000h, 000h, 000h
        jne    SHORT loc_1_157F
        mov    eax, dword ptr [esi + 16eh]
        db  0A3h, 042h, 000h, 000h, 000h
        jmp    SHORT loc_1_15AD
loc_1_157F:
        db  08Bh, 00Dh, 042h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 16eh], esi
        je     SHORT loc_1_159B
loc_1_158D:
        mov    ecx, dword ptr [ecx + 16eh]
        cmp    dword ptr [ecx + 16eh], esi
        jne    SHORT loc_1_158D
loc_1_159B:
        mov    eax, dword ptr [ecx + 16eh]
        mov    edx, dword ptr [eax + 16eh]
        mov    dword ptr [ecx + 16eh], edx
loc_1_15AD:
        lea    eax, [esi + 0cbh]
        push   eax
        call   sub_1_462C
        mov    eax, dword ptr [esi + 15eh]
        push   eax
        db  0E8h, 05Bh, 0EBh, 0FFh, 0FFh
        mov    eax, dword ptr [esi + 4eh]
        push   eax
        call   sub_1_4570
        push   0
        push   332h
        push   esi
        call   sub_1_4644
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_15F0:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_4564
        push   0
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        call   sub_1_4558
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1620:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        lea    ecx, [ebp - 8]
        mov    esi, dword ptr [ebx + 8]
        push   ecx
        lea    eax, [edi + 24h]
        push   0
        push   0
        mov    dword ptr [ebp - 4], eax
        push   0
        push   edi
        call   sub_1_4668
        cmp    dword ptr [ebp - 8], 0ch
        jb     NEAR PTR loc_1_17D6
        cmp    dword ptr [ebp - 8], 5eah
        ja     NEAR PTR loc_1_17D6
        cmp    byte ptr [esi + 203h], 0
        je     SHORT loc_1_1676
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1676:
        cmp    byte ptr [esi + 205h], 0
        je     SHORT loc_1_168D
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_168D:
        cmp    byte ptr [ebx + 20h], 0
        je     SHORT loc_1_16A1
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_16A1:
        mov    eax, dword ptr [ebp - 4]
        push   edi
        push   ebx
        inc    dword ptr [ebx + 1ch]
        push   esi
        inc    dword ptr [esi + 176h]
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax], 0
        call   sub_1_1830
        mov    ecx, dword ptr [ebp - 4]
        cmp    byte ptr [ecx + 9], 0
        je     SHORT loc_1_16F5
        cmp    dword ptr [esi + 18eh], 0
        jne    SHORT loc_1_16E1
        mov    dword ptr [esi + 192h], edi
        mov    dword ptr [esi + 18eh], edi
        jmp    NEAR PTR loc_1_177E
loc_1_16E1:
        mov    eax, dword ptr [esi + 192h]
        mov    dword ptr [eax + 24h], edi
        mov    dword ptr [esi + 192h], edi
        jmp    NEAR PTR loc_1_177E
loc_1_16F5:
        cmp    dword ptr [esi + 17eh], 0
        je     SHORT loc_1_170F
        mov    eax, dword ptr [esi + 182h]
        mov    dword ptr [eax + 24h], edi
        mov    dword ptr [esi + 182h], edi
        jmp    SHORT loc_1_177E
loc_1_170F:
        cmp    dword ptr [esi + 186h], 0
        jne    SHORT loc_1_1728
        xor    ebx, ebx
        mov    dword ptr [esi + 18ah], edi
        mov    dword ptr [esi + 186h], edi
        jmp    SHORT loc_1_1737
loc_1_1728:
        mov    ebx, dword ptr [esi + 18ah]
        mov    dword ptr [ebx + 24h], edi
        mov    dword ptr [esi + 18ah], edi
loc_1_1737:
        push   esi
        push   edi
        call   sub_1_3410
        cmp    ax, 6
        jne    SHORT loc_1_177E
        test   ebx, ebx
        jne    SHORT loc_1_175E
        mov    dword ptr [esi + 18ah], 0
        mov    dword ptr [esi + 186h], 0
        jmp    SHORT loc_1_176B
loc_1_175E:
        mov    dword ptr [esi + 18ah], ebx
        mov    dword ptr [ebx + 24h], 0
loc_1_176B:
        mov    dword ptr [esi + 17eh], edi
        mov    dword ptr [esi + 182h], edi
        mov    byte ptr [esi + 331h], 0
loc_1_177E:
        mov    edi, 103h
        mov    eax, dword ptr [esi + 176h]
        dec    eax
        mov    dword ptr [esi + 176h], eax
        jne    SHORT loc_1_17CB
        cmp    byte ptr [esi + 204h], 0
        je     SHORT loc_1_17A6
        lea    eax, [esi + 31eh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_17BA
loc_1_17A6:
        cmp    dword ptr [esi + 18eh], 0
        je     SHORT loc_1_17CB
        lea    eax, [esi + 31eh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_17CB
loc_1_17BA:
        push   0
        add    esi, 212h
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_461A
loc_1_17CB:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_17D6:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_17F0:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        push   ebx
        push   esi
        xor    esi, esi
        cmp    edx, esi
        je     SHORT loc_1_1812
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
loc_1_1805:
        mov    bl, byte ptr [eax + esi]
        cmp    byte ptr [ecx + esi], bl
        jne    SHORT loc_1_181A
        inc    esi
        cmp    esi, edx
        jb     SHORT loc_1_1805
loc_1_1812:
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_181A:
        mov    eax, 0ffffffffh
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1830:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        lea    esi, [eax + 24h]
        push   6
        push   0
        lea    ecx, [ebp - 8]
        mov    byte ptr [esi + 9], 0
        mov    byte ptr [esi + 8], 0
        push   eax
        push   ecx
        call   sub_1_20C0
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, byte ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 15eh]
        cmp    byte ptr [ecx + 28h], dl
        mov    ebx, dword ptr [eax + 0ch]
        jne    SHORT loc_1_1883
        push   6
        lea    eax, [ebp - 8]
        push   eax
        add    ecx, 24h
        push   ecx
        call   sub_1_17F0
        test   eax, eax
        jne    SHORT loc_1_1883
        mov    eax, ebx
        and    al, 1
        mov    byte ptr [esi + 9], al
loc_1_1883:
        mov    eax, dword ptr [ebp + 0ch]
        test   byte ptr [eax + 71h], 2
        je     SHORT loc_1_1898
        mov    byte ptr [esi + 8], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1898:
        test   bl, 20h
        je     SHORT loc_1_18A9
        mov    byte ptr [esi + 8], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_18A9:
        db  0A0h, 004h, 000h, 000h, 000h
        cmp    byte ptr [ebp - 4], al
        jne    SHORT loc_1_18D5
        push   6
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_17F0
        test   eax, eax
        jne    SHORT loc_1_18D5
        and    bl, 8
        mov    byte ptr [esi + 8], bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_18D5:
        test   byte ptr [ebp - 8], 1
        je     SHORT loc_1_18E9
        and    bl, 6
        mov    byte ptr [esi + 8], bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_18E9:
        cmp    byte ptr [esi + 9], 0
        je     SHORT loc_1_18FB
        mov    byte ptr [esi + 8], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_18FB:
        mov    byte ptr [esi + 8], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1910:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        cmp    byte ptr [eax + 20h], 0
        je     SHORT loc_1_1929
        mov    eax, 0c0010002h
        pop    ebp
        ret    4
loc_1_1929:
        cmp    byte ptr [ecx + 204h], 0
        je     SHORT loc_1_1938
        xor    eax, eax
        pop    ebp
        ret    4
loc_1_1938:
        inc    dword ptr [eax + 1ch]
        inc    dword ptr [ecx + 176h]
        mov    byte ptr [ecx + 204h], 1
        mov    dword ptr [ecx + 206h], eax
        mov    eax, dword ptr [ecx + 176h]
        dec    eax
        mov    dword ptr [ecx + 176h], eax
        jne    SHORT loc_1_1996
        cmp    byte ptr [ecx + 204h], 0
        je     SHORT loc_1_1971
        lea    eax, [ecx + 31eh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_1985
loc_1_1971:
        cmp    dword ptr [ecx + 18eh], 0
        je     SHORT loc_1_1996
        lea    eax, [ecx + 31eh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_1996
loc_1_1985:
        push   0
        add    ecx, 212h
        push   ecx
        mov    byte ptr [eax], 1
        call   sub_1_461A
loc_1_1996:
        mov    eax, 103h
        pop    ebp
        ret    4
        db  090h
loc_1_19A0:
        xor    eax, eax
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_19B0:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        call   sub_1_2FE0
        cmp    ax, 1
        pop    ebp
        mov    eax, 0
        adc    eax, -1
        and    eax, 0c0000001h
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_19E0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 1ch]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_19F0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_2DF0
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1A10:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 176h]
        cmp    byte ptr [esi + 31eh], bl
        jne    SHORT loc_1_1A38
loc_1_1A28:
        mov    byte ptr [esi + 31eh], 1
        push   ebx
        push   ebx
        push   esi
        push   ebx
        call   sub_1_1B70
loc_1_1A38:
        cmp    byte ptr [esi + 31fh], bl
        jne    SHORT loc_1_1A5D
        mov    byte ptr [esi + 31fh], 1
        push   esi
        call   sub_1_2080
        cmp    dword ptr [esi + 18eh], ebx
        je     SHORT loc_1_1A5D
        cmp    byte ptr [esi + 31eh], bl
        je     SHORT loc_1_1A28
loc_1_1A5D:
        mov    eax, dword ptr [esi + 176h]
        dec    eax
        mov    dword ptr [esi + 176h], eax
        jne    SHORT loc_1_1AA5
        cmp    byte ptr [esi + 204h], 0
        je     SHORT loc_1_1A7E
        cmp    byte ptr [esi + 31eh], 0
        je     SHORT loc_1_1A90
loc_1_1A7E:
        cmp    dword ptr [esi + 18eh], 0
        je     SHORT loc_1_1AA5
        cmp    byte ptr [esi + 31eh], 0
        jne    SHORT loc_1_1AA5
loc_1_1A90:
        push   0
        add    esi, 212h
        push   esi
        mov    byte ptr [esi + 10ch], 1
        call   sub_1_461A
loc_1_1AA5:
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
sub_1_1AB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        mov    dword ptr [esi + 196h], ebx
        mov    byte ptr [esi + 19ah], 1
        push   0
        push   ebx
        call   sub_1_4668
        cmp    dword ptr [ebp - 4], 6
        jb     SHORT loc_1_1B5B
        mov    edi, dword ptr [esi + 20eh]
        mov    eax, dword ptr [ebp - 4]
        add    edi, 0eh
        cmp    edi, eax
        jb     SHORT loc_1_1AF4
        mov    edi, eax
loc_1_1AF4:
        push   edi
        lea    eax, [esi + 222h]
        push   0
        push   ebx
        push   eax
        call   sub_1_20C0
        cmp    dword ptr [ebp - 4], 0eh
        jae    SHORT loc_1_1B32
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        lea    ecx, [esi + 222h]
        push   0
        push   eax
        push   ecx
        mov    eax, dword ptr [esi + 15eh]
        push   ecx
        push   esi
        push   eax
        call   sub_1_4730
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1B32:
        mov    eax, dword ptr [ebp - 4]
        sub    edi, 0eh
        sub    eax, 0eh
        lea    ecx, [esi + 230h]
        push   eax
        push   edi
        push   ecx
        lea    eax, [esi + 222h]
        push   0eh
        mov    ecx, dword ptr [esi + 15eh]
        push   eax
        push   eax
        push   esi
        push   ecx
        call   sub_1_4730
loc_1_1B5B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B70:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        inc    dword ptr [esi + 176h]
        call   sub_1_2E50
        push   esi
        lea    eax, [esi + 0cbh]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_4632
        cmp    dword ptr [esi + 18eh], 0
        je     NEAR PTR loc_1_1C60
loc_1_1BA7:
        mov    ebx, dword ptr [esi + 18eh]
        cmp    dword ptr [esi + 192h], ebx
        jne    SHORT loc_1_1BBF
        mov    dword ptr [esi + 192h], 0
loc_1_1BBF:
        mov    eax, dword ptr [ebx + 24h]
        push   ebx
        push   esi
        mov    dword ptr [esi + 18eh], eax
        call   sub_1_1AB0
        push   0
        inc    dword ptr [esi + 1a3h]
        mov    edi, dword ptr [ebx + 28h]
        push   ebx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_4582
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_1C53
        mov    eax, dword ptr [ebx + 172h]
        cmp    edi, eax
        jne    SHORT loc_1_1C00
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 172h], eax
        jmp    SHORT loc_1_1C13
loc_1_1C00:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_1C0D
loc_1_1C05:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_1C05
loc_1_1C0D:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_1C13:
        mov    eax, dword ptr [ebx + 20eh]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_1C24
        push   ebx
        call   sub_1_0A80
loc_1_1C24:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_457C
        push   0
        push   75h
        push   edi
        call   sub_1_4644
        cmp    dword ptr [ebx + 172h], 0
        jne    SHORT loc_1_1C53
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 0cbh
        push   ebx
        call   sub_1_4632
loc_1_1C53:
        cmp    dword ptr [esi + 18eh], 0
        jne    NEAR PTR loc_1_1BA7
loc_1_1C60:
        push   esi
        call   sub_1_3070
        cmp    byte ptr [esi + 19ah], 0
        je     SHORT loc_1_1C82
        mov    byte ptr [esi + 19ah], 0
        mov    eax, dword ptr [esi + 15eh]
        push   eax
        call   sub_1_4910
loc_1_1C82:
        cmp    byte ptr [esi + 204h], 0
        je     NEAR PTR loc_1_2044
        cmp    dword ptr [esi + 176h], 1
        jne    NEAR PTR loc_1_2044
        push   esi
        lea    eax, [esi + 0cbh]
        mov    byte ptr [esi + 205h], 1
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_4632
        mov    edi, dword ptr [esi + 172h]
        test   edi, edi
        je     NEAR PTR loc_1_1D52
loc_1_1CC3:
        push   0
        inc    dword ptr [edi + 1ch]
        push   0
        mov    eax, dword ptr [edi]
        push   40010004h
        push   eax
        call   sub_1_45A6
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_1D47
        mov    eax, dword ptr [ebx + 172h]
        cmp    edi, eax
        jne    SHORT loc_1_1CF4
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 172h], eax
        jmp    SHORT loc_1_1D07
loc_1_1CF4:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_1D01
loc_1_1CF9:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_1CF9
loc_1_1D01:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_1D07:
        mov    eax, dword ptr [ebx + 20eh]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_1D18
        push   ebx
        call   sub_1_0A80
loc_1_1D18:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_457C
        push   0
        push   75h
        push   edi
        call   sub_1_4644
        cmp    dword ptr [ebx + 172h], 0
        jne    SHORT loc_1_1D47
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 0cbh
        push   ebx
        call   sub_1_4632
loc_1_1D47:
        mov    edi, dword ptr [edi + 0ch]
        test   edi, edi
        jne    NEAR PTR loc_1_1CC3
loc_1_1D52:
        push   esi
        call   sub_1_2BD0
        test   ax, ax
        je     SHORT loc_1_1D79
        mov    byte ptr [esi + 203h], 1
        push   0
        mov    eax, dword ptr [esi + 4eh]
        push   0aa000003h
        push   eax
        call   sub_1_4710
        add    esp, 0ch
        jmp    SHORT loc_1_1D80
loc_1_1D79:
        mov    byte ptr [esi + 203h], 0
loc_1_1D80:
        cmp    dword ptr [esi + 186h], 0
        je     SHORT loc_1_1DB8
        mov    eax, dword ptr [esi + 17eh]
        mov    ecx, dword ptr [esi + 18ah]
        mov    dword ptr [ecx + 24h], eax
        mov    edx, dword ptr [esi + 186h]
        mov    dword ptr [esi + 17eh], edx
        mov    dword ptr [esi + 18ah], 0
        mov    dword ptr [esi + 186h], 0
loc_1_1DB8:
        cmp    dword ptr [esi + 18eh], 0
        je     SHORT loc_1_1DDC
        mov    eax, dword ptr [esi + 17eh]
        mov    ecx, dword ptr [esi + 192h]
        mov    dword ptr [ecx + 24h], eax
        mov    edx, dword ptr [esi + 18eh]
        mov    dword ptr [esi + 17eh], edx
loc_1_1DDC:
        mov    dword ptr [esi + 192h], 0
        mov    dword ptr [esi + 18eh], 0
        cmp    dword ptr [esi + 17eh], 0
        je     NEAR PTR loc_1_1E9A
loc_1_1DFD:
        mov    ecx, dword ptr [esi + 17eh]
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [esi + 17eh], eax
        mov    edi, dword ptr [ecx + 28h]
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_4582
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_1E8D
        mov    eax, dword ptr [ebx + 172h]
        cmp    edi, eax
        jne    SHORT loc_1_1E3A
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 172h], eax
        jmp    SHORT loc_1_1E4D
loc_1_1E3A:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_1E47
loc_1_1E3F:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_1E3F
loc_1_1E47:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_1E4D:
        mov    eax, dword ptr [ebx + 20eh]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_1E5E
        push   ebx
        call   sub_1_0A80
loc_1_1E5E:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_457C
        push   0
        push   75h
        push   edi
        call   sub_1_4644
        cmp    dword ptr [ebx + 172h], 0
        jne    SHORT loc_1_1E8D
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 0cbh
        push   ebx
        call   sub_1_4632
loc_1_1E8D:
        cmp    dword ptr [esi + 17eh], 0
        jne    NEAR PTR loc_1_1DFD
loc_1_1E9A:
        mov    dword ptr [esi + 182h], 0
        cmp    byte ptr [esi + 203h], 0
        jne    SHORT loc_1_1EB3
        push   esi
        call   sub_1_2F10
loc_1_1EB3:
        mov    byte ptr [esi + 205h], 0
        mov    edi, dword ptr [esi + 172h]
        test   edi, edi
        je     NEAR PTR loc_1_1F92
loc_1_1EC8:
        inc    dword ptr [edi + 1ch]
        cmp    byte ptr [esi + 203h], 0
        je     SHORT loc_1_1EE5
        push   0
        mov    eax, dword ptr [edi]
        push   0
        push   40010007h
        push   eax
        call   sub_1_45A6
loc_1_1EE5:
        mov    eax, 0
        push   4
        cmp    byte ptr [esi + 203h], 1
        adc    eax, -1
        and    eax, 0c0000001h
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [edi]
        push   40010005h
        push   ecx
        call   sub_1_45A6
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_45AC
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_1F87
        mov    eax, dword ptr [ebx + 172h]
        cmp    edi, eax
        jne    SHORT loc_1_1F34
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 172h], eax
        jmp    SHORT loc_1_1F47
loc_1_1F34:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_1F41
loc_1_1F39:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_1F39
loc_1_1F41:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_1F47:
        mov    eax, dword ptr [ebx + 20eh]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_1F58
        push   ebx
        call   sub_1_0A80
loc_1_1F58:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_457C
        push   0
        push   75h
        push   edi
        call   sub_1_4644
        cmp    dword ptr [ebx + 172h], 0
        jne    SHORT loc_1_1F87
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 0cbh
        push   ebx
        call   sub_1_4632
loc_1_1F87:
        mov    edi, dword ptr [edi + 0ch]
        test   edi, edi
        jne    NEAR PTR loc_1_1EC8
loc_1_1F92:
        mov    eax, 0
        mov    ecx, dword ptr [esi + 206h]
        cmp    byte ptr [esi + 203h], 1
        adc    eax, -1
        mov    edx, dword ptr [ecx]
        and    eax, 0c0000001h
        push   eax
        push   edx
        call   sub_1_458E
        mov    ebx, dword ptr [esi + 206h]
        mov    edi, dword ptr [ebx + 8]
        dec    dword ptr [ebx + 1ch]
        jne    SHORT loc_1_2037
        mov    ecx, dword ptr [edi + 172h]
        cmp    ecx, ebx
        jne    SHORT loc_1_1FD8
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [edi + 172h], eax
        jmp    SHORT loc_1_1FF7
loc_1_1FD8:
        mov    eax, dword ptr [esi + 206h]
        cmp    dword ptr [ecx + 0ch], eax
        je     SHORT loc_1_1FEB
loc_1_1FE3:
        mov    ecx, dword ptr [ecx + 0ch]
        cmp    dword ptr [ecx + 0ch], eax
        jne    SHORT loc_1_1FE3
loc_1_1FEB:
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    ebx, dword ptr [esi + 206h]
loc_1_1FF7:
        mov    eax, dword ptr [edi + 20eh]
        cmp    dword ptr [ebx + 18h], eax
        jne    SHORT loc_1_2008
        push   edi
        call   sub_1_0A80
loc_1_2008:
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_457C
        push   0
        push   75h
        push   ebx
        call   sub_1_4644
        cmp    dword ptr [edi + 172h], 0
        jne    SHORT loc_1_2037
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        add    edi, 0cbh
        push   edi
        call   sub_1_4632
loc_1_2037:
        mov    byte ptr [esi + 204h], 0
        push   esi
        call   sub_1_2F10
loc_1_2044:
        mov    byte ptr [esi + 31eh], 0
        push   esi
        call   sub_1_2ED0
        push   esi
        lea    eax, [esi + 0cbh]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_4632
        pop    edi
        dec    dword ptr [esi + 176h]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2080:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 205h], 0
        je     SHORT loc_1_209C
        mov    byte ptr [esi + 31fh], 0
        pop    esi
        pop    ebp
        ret    4
loc_1_209C:
        mov    byte ptr [esi + 331h], 0
        push   esi
        call   sub_1_3290
        mov    byte ptr [esi + 31fh], 0
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_20C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        xor    esi, esi
        xor    edi, edi
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        push   esi
        push   ecx
        call   sub_1_4668
        mov    ebx, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp - 4], esi
        je     SHORT loc_1_2114
loc_1_20E5:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_468C
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    eax, ebx
        ja     SHORT loc_1_2114
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_46B0
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_20E5
loc_1_2114:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_2125
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_2125:
        mov    eax, ebx
        sub    eax, edi
        sub    edi, ebx
        add    dword ptr [ebp - 0ch], eax
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 14h]
loc_1_2134:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    SHORT loc_1_2143
        mov    ebx, edi
        sub    ebx, esi
        jmp    SHORT loc_1_2146
loc_1_2143:
        mov    ebx, dword ptr [ebp - 8]
loc_1_2146:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        add    esi, ebx
        push   eax
        call   sub_1_4A20
        cmp    esi, edi
        je     SHORT loc_1_2182
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_46B0
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_218D
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_468C
        jmp    SHORT loc_1_2134
loc_1_2182:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_218D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_21A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebx + 8]
        add    esi, 0eh
        cmp    dword ptr [edi + 196h], 0
        je     SHORT loc_1_222B
        push   0
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        push   0
        mov    ebx, esi
        push   0
        push   ecx
        call   sub_1_4668
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_2219
loc_1_21D9:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_468C
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [edi + 196h]
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   ebx
        push   edx
        push   eax
        call   sub_1_20C0
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 8]
        jb     SHORT loc_1_2219
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_46B0
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_21D9
loc_1_2219:
        sub    ebx, esi
        mov    ecx, dword ptr [ebp + 1ch]
        xor    eax, eax
        pop    edi
        pop    esi
        mov    dword ptr [ecx], ebx
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_222B:
        cmp    byte ptr [edi + 19ah], 0
        je     SHORT loc_1_22AB
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    ecx, dword ptr [ebp + 14h]
        push   esi
        mov    edx, dword ptr [ebp + 1ch]
        push   ecx
        push   edx
        call   sub_1_34C0
        test   ax, ax
        je     SHORT loc_1_22A0
        cmp    ax, 13h
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], 0
        jne    SHORT loc_1_2292
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edi + 111h], eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [edi + 115h], ecx
        mov    dword ptr [edi + 119h], esi
        mov    dword ptr [edi + 11dh], edx
        mov    dword ptr [edi + 121h], eax
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_2292:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_22A0:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_22AB:
        mov    eax, 0c0010013h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_22C0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_2F90
        cmp    ax, 1
        pop    ebp
        sbb    cl, cl
        neg    cl
        mov    al, cl
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_22E0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 331h], 0
        je     NEAR PTR loc_1_23E9
        cmp    byte ptr [esi + 0ebh], 0
        je     NEAR PTR loc_1_23E9
        push   esi
        call   sub_1_2DF0
        mov    byte ptr [esi + 331h], 0
        push   0
        mov    eax, dword ptr [esi + 4eh]
        push   0aa000003h
        push   eax
        call   sub_1_4710
        add    esp, 0ch
        cmp    dword ptr [esi + 186h], 0
        je     SHORT loc_1_2361
        xor    edi, edi
loc_1_232E:
        mov    edx, dword ptr [esi + 186h]
        lea    eax, [edx + 24h]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 186h], ecx
        test   ecx, ecx
        jne    SHORT loc_1_2349
        mov    dword ptr [esi + 18ah], edi
loc_1_2349:
        mov    ebx, dword ptr [eax + 4]
        push   edi
        push   edx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_4582
        dec    dword ptr [ebx + 1ch]
        cmp    dword ptr [esi + 186h], edi
        jne    SHORT loc_1_232E
loc_1_2361:
        cmp    dword ptr [esi + 17eh], 0
        je     SHORT loc_1_239F
        xor    edi, edi
loc_1_236C:
        mov    edx, dword ptr [esi + 17eh]
        lea    ecx, [edx + 24h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 17eh], eax
        test   eax, eax
        jne    SHORT loc_1_2387
        mov    dword ptr [esi + 182h], edi
loc_1_2387:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   edx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_4582
        dec    dword ptr [ebx + 1ch]
        cmp    dword ptr [esi + 17eh], edi
        jne    SHORT loc_1_236C
loc_1_239F:
        mov    byte ptr [esi + 331h], 0
        mov    byte ptr [esi + 0ebh], 0
        push   esi
        call   sub_1_2BD0
        test   ax, ax
        je     SHORT loc_1_23D4
        mov    byte ptr [esi + 203h], 1
        push   0
        mov    eax, dword ptr [esi + 4eh]
        push   0aa000003h
        push   eax
        call   sub_1_4710
        add    esp, 0ch
        jmp    SHORT loc_1_23DB
loc_1_23D4:
        mov    byte ptr [esi + 203h], 0
loc_1_23DB:
        push   esi
        call   sub_1_2F10
        push   esi
        call   sub_1_2DC0
        jmp    SHORT loc_1_2402
loc_1_23E9:
        cmp    dword ptr [esi + 186h], 0
        jne    SHORT loc_1_23FB
        cmp    dword ptr [esi + 17eh], 0
        je     SHORT loc_1_2402
loc_1_23FB:
        mov    byte ptr [esi + 331h], 1
loc_1_2402:
        push   1388h
        add    esi, 325h
        push   esi
        call   sub_1_461A
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2420:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   1
        mov    dx, word ptr [esi + 4]
        xor    edi, edi
        add    edx, 20h
        in     al, dx
        mov    bl, al
        or     al, 80h
        out    dx, al
        call   sub_1_4638
        mov    di, word ptr [esi + 4]
        mov    al, bl
        lea    edx, [edi + 20h]
        xor    ecx, ecx
        out    dx, al
        mov    eax, dword ptr [esi + 18h]
        shr    eax, 0dh
        and    al, 3fh
        or     al, 40h
        out    dx, al
        xor    edx, edx
        xor    eax, eax
loc_1_245C:
        cmp    byte ptr [edx + esi + 24h], al
        je     SHORT loc_1_2464
        mov    cl, 1
loc_1_2464:
        inc    edx
        cmp    edx, 6
        jb     SHORT loc_1_245C
        lea    edx, [edi + 24h]
        in     al, dx
        mov    bl, al
        and    al, 7fh
        out    dx, al
        lea    edx, [edi + 21h]
        mov    al, 46h
        out    dx, al
loc_1_2479:
        in     al, dx
        test   al, 40h
        jne    SHORT loc_1_2479
        xor    edi, edi
loc_1_2480:
        xor    eax, eax
        inc    edi
        mov    ax, word ptr [esi + 4]
        cmp    edi, 6
        lea    edx, [eax + edi + 27h]
        in     al, dx
        mov    byte ptr [edi + esi + 29h], al
        jb     SHORT loc_1_2480
        xor    edx, edx
        mov    al, bl
        mov    dx, word ptr [esi + 4]
        add    edx, 21h
        test   cl, cl
        out    dx, al
        jne    SHORT loc_1_24B7
        xor    ecx, ecx
loc_1_24A7:
        mov    dl, byte ptr [ecx + esi + 2ah]
        lea    eax, [ecx + esi]
        inc    ecx
        mov    byte ptr [eax + 24h], dl
        cmp    ecx, 6
        jb     SHORT loc_1_24A7
loc_1_24B7:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 0ah]
        in     al, dx
        or     al, 40h
        out    dx, al
        lea    edx, [ecx + 20h]
        in     al, dx
        or     al, 40h
        out    dx, al
        lea    edx, [ecx + 30h]
        mov    al, 1
        out    dx, al
        lea    edx, [ecx + 3bh]
        xor    al, al
        out    dx, al
        out    dx, al
        lea    edx, [ecx + 37h]
        in     al, dx
        test   al, 80h
        jne    SHORT loc_1_24E7
        push   2
        call   sub_1_4638
loc_1_24E7:
        xor    edx, edx
        mov    al, 49h
        mov    dx, word ptr [esi + 4]
        push   esi
        add    edx, 3eh
        xor    edi, edi
        out    dx, al
        call   sub_1_2FE0
        mov    di, word ptr [esi + 4]
        mov    al, 4
        mov    cl, byte ptr [esi + 152h]
        lea    ebx, [edi + 3dh]
        mov    edx, ebx
        out    dx, al
        lea    edx, [edi + 31h]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 32h]
        mov    al, byte ptr [esi + 153h]
        out    dx, al
        lea    edx, [edi + 33h]
        mov    al, cl
        out    dx, al
        mov    edx, ebx
        xor    al, al
        out    dx, al
        lea    edx, [edi + 37h]
        mov    al, 0ffh
        out    dx, al
        lea    edx, [edi + 3fh]
        mov    al, 3fh
        mov    byte ptr [esi + 0c9h], 3fh
        out    dx, al
        lea    edx, [edi + 30h]
        mov    al, 41h
        out    dx, al
        xor    ebx, ebx
loc_1_2543:
        lea    edx, [ebx + edi + 31h]
        mov    al, byte ptr [ebx + esi + 24h]
        out    dx, al
        inc    ebx
        cmp    ebx, 6
        jb     SHORT loc_1_2543
        lea    eax, [ecx + 1]
        lea    edx, [edi + 37h]
        out    dx, al
        xor    ecx, ecx
        and    eax, 0ffh
        mov    cx, word ptr [esi + 38h]
        xor    ebx, ebx
        imul   ecx, eax
        add    ecx, dword ptr [esi + 1ch]
        lea    edi, [esi + 0cbh]
        mov    dword ptr [esi + 154h], ecx
loc_1_2578:
        push   esi
        mov    byte ptr [esi + 13ch], bl
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_1_4632
        xor    ecx, ecx
        xor    eax, eax
        mov    ax, word ptr [esi + 20h]
        test   eax, 3fffffh
        jbe    SHORT loc_1_25AF
loc_1_2599:
        mov    eax, dword ptr [esi + 1ch]
        inc    ecx
        mov    byte ptr [eax + ecx - 1], 0
        xor    eax, eax
        mov    ax, word ptr [esi + 20h]
        shl    eax, 0ah
        cmp    eax, ecx
        ja     SHORT loc_1_2599
loc_1_25AF:
        inc    ebx
        cmp    ebx, 3
        jb     SHORT loc_1_2578
        mov    byte ptr [esi + 13ch], 0
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_1_4632
        mov    si, word ptr [esi + 4]
        mov    al, 1
        and    esi, 0ffffh
        lea    edx, [esi + 30h]
        lea    ecx, [esi + 24h]
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    bl, al
        or     al, 80h
        out    dx, al
        lea    edx, [esi + 2dh]
        in     al, dx
        or     al, 4ch
        out    dx, al
        lea    edx, [esi + 25h]
        in     al, dx
        or     al, 80h
        out    dx, al
        lea    edx, [esi + 26h]
        mov    al, 1
        out    dx, al
        lea    edx, [esi + 2bh]
        xor    al, al
        out    dx, al
        mov    al, bl
        mov    edx, ecx
        and    al, 7fh
        pop    edi
        out    dx, al
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_2610:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    si, word ptr [ecx + 0e7h]
        mov    word ptr [ecx + 0e9h], si
        lea    eax, [esi + 1]
        mov    word ptr [ecx + 0e7h], ax
        cmp    word ptr [ecx + 3ah], ax
        jne    SHORT loc_1_2641
        mov    word ptr [ecx + 0e7h], 0
loc_1_2641:
        movsx  eax, word ptr [ecx + 0e7h]
        cmp    dword ptr [ecx + eax*4 + 0f4h], 2
        je     SHORT loc_1_265B
        mov    word ptr [ecx + 0e7h], 0ffffh
loc_1_265B:
        movsx  eax, si
        mov    bx, word ptr [ecx + eax*4 + 0ech]
        xor    edi, edi
        mov    di, word ptr [ecx + 4]
        mov    ax, word ptr [ecx + 0c3h]
        lea    edx, [edi + 34h]
        imul   ax, si
        mov    al, ah
        out    dx, al
        lea    edx, [edi + 35h]
        mov    al, bl
        out    dx, al
        lea    edx, [edi + 36h]
        mov    al, bh
        out    dx, al
        lea    edx, [edi + 30h]
        mov    al, 26h
        mov    byte ptr [ecx + 0ebh], 1
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_26A0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0c3h]
        lea    edx, [ebp - 8]
        movsx  ecx, word ptr [esi + 0e5h]
        imul   eax, ecx
        add    eax, dword ptr [esi + 1ch]
        lea    ecx, [ebp - 20h]
        push   ecx
        xor    ebx, ebx
        push   edx
        mov    dword ptr [ebp - 18h], eax
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        push   ebx
        push   eax
        call   sub_1_4668
        mov    ecx, dword ptr [esi + 125h]
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        mov    dword ptr [ebp - 10h], ebx
        add    edx, 30h
        mov    byte ptr [ebp - 1], bl
        in     al, dx
        mov    dword ptr [ebp - 14h], ecx
        test   al, 4
        jne    SHORT loc_1_26FE
        mov    byte ptr [ebp - 1], 1
        mov    al, 10h
        out    dx, al
loc_1_26FE:
        xor    ebx, ebx
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 0cbh]
        push   eax
        mov    byte ptr [esi + 13ch], bl
        call   sub_1_4632
        cmp    dword ptr [ebp - 8], ebx
        je     NEAR PTR loc_1_2816
loc_1_2721:
        xor    edi, edi
        lea    eax, [ebp - 0ch]
        push   eax
        lea    ecx, [ebp - 1ch]
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        push   edx
        call   sub_1_468C
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 1ch]
        push   eax
        mov    edx, dword ptr [ebp - 18h]
        mov    word ptr [esi + 4ah], 5
        push   ecx
        push   edx
        call   sub_1_4A20
        cmp    dword ptr [esi + 100h], ebx
        je     SHORT loc_1_2798
loc_1_2753:
        mov    eax, edi
        inc    edi
        cmp    eax, 0ffffh
        jae    SHORT loc_1_2798
        xor    ecx, ecx
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 0bh]
        in     al, dx
        test   al, 10h
        je     SHORT loc_1_2790
        lea    edx, [ecx + 0ah]
        mov    dword ptr [esi + 100h], ebx
        in     al, dx
        and    al, 7fh
        out    dx, al
        mov    eax, dword ptr [esi + 104h]
        inc    eax
        mov    dword ptr [esi + 104h], eax
        cmp    eax, 31h
        jne    SHORT loc_1_2790
        mov    dword ptr [esi + 104h], ebx
loc_1_2790:
        cmp    dword ptr [esi + 100h], ebx
        jne    SHORT loc_1_2753
loc_1_2798:
        mov    eax, dword ptr [ebp - 0ch]
        add    dword ptr [ebp - 18h], eax
        add    dword ptr [ebp - 10h], eax
        cmp    byte ptr [ebp - 1], bl
        je     SHORT loc_1_2800
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 14h], ecx
        jae    SHORT loc_1_2800
        movsx  eax, word ptr [esi + 0e5h]
        mov    dword ptr [esi + eax*4 + 0f4h], 2
        cmp    word ptr [esi + 0e7h], -1
        jne    SHORT loc_1_2800
        mov    ax, word ptr [esi + 0e5h]
        cmp    word ptr [esi + 0e9h], -1
        mov    word ptr [esi + 0e7h], ax
        jne    SHORT loc_1_2800
        or     byte ptr [esi + 42h], 20h
        movsx  eax, ax
        mov    ecx, dword ptr [ebp - 20h]
        push   esi
        mov    dword ptr [esi + eax*4 + 0ech], ecx
        call   sub_1_2610
        mov    dword ptr [ebp - 14h], 0ffffffffh
loc_1_2800:
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_46B0
        cmp    dword ptr [ebp - 8], ebx
        jne    NEAR PTR loc_1_2721
loc_1_2816:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_2828
        xor    edx, edx
        mov    al, 8
        mov    dx, word ptr [esi + 4]
        add    edx, 30h
        out    dx, al
loc_1_2828:
        cmp    dword ptr [ebp - 10h], 3ch
        movsx  eax, word ptr [esi + 0e5h]
        jae    SHORT loc_1_2849
        mov    dword ptr [esi + eax*4 + 0ech], 3ch
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2849:
        mov    ecx, dword ptr [ebp - 10h]
        pop    edi
        mov    dword ptr [esi + eax*4 + 0ech], ecx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h
sub_1_2860:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 4]
        push   esi
        add    edx, 38h
        mov    al, byte ptr [esi + 3ch]
        out    dx, al
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 0cbh]
        push   eax
        call   sub_1_4632
        mov    cl, byte ptr [esi + 0e3h]
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        mov    byte ptr [ebp - 3], cl
        add    edx, 33h
        in     al, dx
        lea    ebx, [eax + 1]
        cmp    byte ptr [esi + 153h], bl
        ja     SHORT loc_1_28AF
        mov    bl, byte ptr [esi + 152h]
loc_1_28AF:
        mov    di, word ptr [ebp - 2]
loc_1_28B3:
        cmp    bl, byte ptr [ebp - 3]
        je     NEAR PTR loc_1_29E0
loc_1_28BC:
        mov    al, bl
        push   esi
        shr    al, 5
        db  068h, 000h, 000h, 000h, 000h
        lea    ecx, [esi + 0cbh]
        mov    byte ptr [esi + 13dh], al
        push   ecx
        mov    byte ptr [esi + 13ch], al
        call   sub_1_4632
        mov    al, bl
        xor    ecx, ecx
        mov    cx, word ptr [esi + 38h]
        and    eax, 1fh
        imul   eax, ecx
        add    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 8], 0
        mov    cx, word ptr [eax + 2]
        mov    dl, byte ptr [eax]
        mov    word ptr [ebp - 8], cx
        mov    cl, byte ptr [eax + 1]
        mov    byte ptr [ebp - 2], cl
        cmp    byte ptr [esi + 152h], cl
        ja     SHORT loc_1_2987
        cmp    byte ptr [esi + 153h], cl
        jb     SHORT loc_1_2987
        test   dl, 1
        mov    byte ptr [esi + 0e4h], bl
        je     SHORT loc_1_2941
        add    eax, 4
        push   esi
        mov    dword ptr [esi + 14ch], eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 4
        push   eax
        mov    dword ptr [esi + 158h], eax
        call   sub_1_3A90
        mov    di, ax
loc_1_2941:
        mov    bl, byte ptr [ebp - 2]
        mov    al, byte ptr [esi + 153h]
        cmp    bl, al
        jbe    SHORT loc_1_2956
        sub    bl, al
        add    bl, byte ptr [esi + 152h]
loc_1_2956:
        cmp    byte ptr [esi + 152h], bl
        jne    SHORT loc_1_2965
        and    eax, 0ffh
        jmp    SHORT loc_1_2969
loc_1_2965:
        xor    eax, eax
        mov    al, bl
loc_1_2969:
        dec    eax
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 33h
        out    dx, al
        cmp    di, 5
        jne    NEAR PTR loc_1_28B3
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2987:
        mov    bl, byte ptr [ebp - 3]
        mov    cl, byte ptr [esi + 153h]
        cmp    bl, cl
        jbe    SHORT loc_1_299C
        sub    bl, cl
        add    bl, byte ptr [esi + 152h]
loc_1_299C:
        mov    al, bl
        push   esi
        shr    al, 5
        db  068h, 000h, 000h, 000h, 000h
        lea    ecx, [esi + 0cbh]
        mov    byte ptr [esi + 13dh], al
        push   ecx
        mov    byte ptr [esi + 13ch], al
        call   sub_1_4632
        cmp    byte ptr [esi + 152h], bl
        jne    SHORT loc_1_29D1
        xor    eax, eax
        mov    al, byte ptr [esi + 153h]
        jmp    SHORT loc_1_29D5
loc_1_29D1:
        xor    eax, eax
        mov    al, bl
loc_1_29D5:
        dec    eax
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 33h
        out    dx, al
loc_1_29E0:
        xor    edx, edx
        mov    al, 1
        mov    dx, word ptr [esi + 4]
        push   esi
        add    edx, 37h
        db  068h, 000h, 000h, 000h, 000h
        out    dx, al
        lea    eax, [esi + 0cbh]
        push   eax
        call   sub_1_4632
        mov    cl, byte ptr [esi + 0e3h]
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        mov    byte ptr [ebp - 3], cl
        add    edx, 33h
        in     al, dx
        lea    ebx, [eax + 1]
        cmp    byte ptr [esi + 153h], bl
        ja     SHORT loc_1_2A22
        mov    bl, byte ptr [esi + 152h]
loc_1_2A22:
        mov    al, bl
        xor    ecx, ecx
        mov    cx, word ptr [esi + 38h]
        and    eax, 1fh
        imul   eax, ecx
        add    eax, dword ptr [esi + 1ch]
        mov    dword ptr [esi + 154h], eax
        mov    al, bl
        shr    al, 5
        cmp    bl, byte ptr [ebp - 3]
        mov    byte ptr [esi + 13fh], al
        jne    NEAR PTR loc_1_28BC
        cmp    byte ptr [esi + 150h], 0
        je     SHORT loc_1_2A6E
        xor    ecx, ecx
        mov    al, 10h
        mov    cx, word ptr [esi + 4]
        mov    byte ptr [esi + 150h], 0
        lea    edx, [ecx + 37h]
        out    dx, al
        lea    edx, [ecx + 33h]
        in     al, dx
        out    dx, al
loc_1_2A6E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2A80:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 4]
        in     al, dx
        mov    edi, edx
        mov    byte ptr [ebp - 2], al
        and    edi, 0ffffh
        shl    word ptr [ebp - 2], 8
        lea    edx, [edi + 1]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        cmp    word ptr [ebp - 2], 4da3h
        je     SHORT loc_1_2ABD
        mov    ax, 0ffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2ABD:
        lea    edx, [edi + 2]
        in     al, dx
        lea    edx, [edi + 3]
        mov    byte ptr [ebp - 2], al
        in     al, dx
        shl    word ptr [ebp - 2], 8
        mov    byte ptr [ebp - 2], al
        cmp    word ptr [ebp - 2], 8010h
        je     SHORT loc_1_2AE5
        mov    ax, 0ffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2AE5:
        xor    eax, eax
        lea    edx, [edi + 0ah]
        mov    ax, word ptr [ebp - 2]
        mov    ebx, 0fba75320h
        mov    dword ptr [esi + 0bbh], eax
        in     al, dx
        mov    cl, al
        in     al, dx
        and    cl, 7
        shl    cl, 2
        shr    ebx, cl
        and    bx, 0fh
        mov    cl, al
        mov    word ptr [esi + 0ah], bx
        and    cl, 0f0h
        lea    edx, [edi + 8]
        cmp    cl, 1
        in     al, dx
        mov    ecx, 0
        adc    ecx, -1
        and    cx, 20h
        lea    edx, [edi + 9]
        mov    word ptr [esi + 0ch], cx
        mov    cl, al
        in     al, dx
        and    ecx, 0fch
        and    eax, 0f0h
        mov    ch, 1
        mov    ah, 6
        mov    word ptr [esi + 3ah], 2
        mov    word ptr [esi + 0c3h], 600h
        mov    word ptr [esi + 16h], 20h
        mov    word ptr [esi + 20h], 8
        mov    word ptr [esi + 4ch], 1fffh
        shl    ecx, 0bh
        shl    eax, 9
        mov    dword ptr [esi + 18h], ecx
        pop    edi
        mov    dword ptr [esi + 0eh], eax
        xor    eax, eax
        mov    word ptr [esi + 42h], ax
        mov    dword ptr [esi + 6], eax
        mov    ax, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2B90:
        xor    ax, ax
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2BA0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        cmp    word ptr [eax + 3ch], 4
        mov    ebp, esp
        jae    SHORT loc_1_2BB4
        mov    word ptr [eax + 3ch], 4
loc_1_2BB4:
        xor    edx, edx
        pop    ebp
        mov    dx, word ptr [eax + 4]
        mov    al, byte ptr [eax + 3ch]
        add    edx, 38h
        out    dx, al
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2BD0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_4380
        push   esi
        call   sub_1_2DF0
        mov    eax, 0ffffffffh
        xor    ecx, ecx
        mov    word ptr [esi + 0e9h], ax
        mov    byte ptr [esi + 0ebh], cl
        mov    word ptr [esi + 0e5h], cx
        mov    byte ptr [esi + 151h], cl
        mov    byte ptr [esi + 129h], 3
        mov    dword ptr [esi + 125h], 180h
        cmp    word ptr [esi + 3ah], 3
        mov    word ptr [esi + 0e7h], ax
        jbe    SHORT loc_1_2C2B
        mov    word ptr [esi + 3ah], 2
loc_1_2C2B:
        xor    eax, eax
        cmp    word ptr [esi + 3ah], ax
        jbe    SHORT loc_1_2C4E
        lea    ecx, [esi + 0f4h]
        xor    edx, edx
loc_1_2C3B:
        mov    dword ptr [ecx], edx
        add    ecx, 4
        inc    eax
        xor    edi, edi
        mov    dword ptr [ecx - 0ch], edx
        mov    di, word ptr [esi + 3ah]
        cmp    edi, eax
        ja     SHORT loc_1_2C3B
loc_1_2C4E:
        xor    ecx, ecx
        xor    eax, eax
        mov    cx, word ptr [esi + 0c3h]
        mov    edi, dword ptr [esi + 1ch]
        mov    ax, word ptr [esi + 3ah]
        mov    edx, dword ptr [esi + 1ch]
        mov    byte ptr [esi + 0e3h], 0
        imul   ecx, eax
        mov    ax, word ptr [esi + 16h]
        add    edi, ecx
        mov    ecx, eax
        mov    dword ptr [esi + 12ch], edi
        and    ecx, 0ffffh
        mov    dword ptr [esi + 138h], edx
        mov    ebx, ecx
        shl    ebx, 0ah
        add    edx, ebx
        mov    dword ptr [esi + 134h], edx
        xor    edx, edx
        mov    dx, word ptr [esi + 20h]
        shl    edx, 0ah
        add    edx, dword ptr [esi + 1ch]
        mov    dword ptr [esi + 130h], edx
        sub    dx, dx
        div    word ptr [esi + 20h]
        mov    byte ptr [esi + 13eh], al
        mov    eax, edi
        sub    eax, dword ptr [esi + 1ch]
        xor    edi, edi
        mov    di, word ptr [esi + 38h]
        and    ecx, 3fffffh
        cdq
        idiv   edi
        mov    byte ptr [esi + 152h], al
        cmp    ecx, 40h
        jne    SHORT loc_1_2CDB
        mov    byte ptr [esi + 153h], 0ffh
        jmp    SHORT loc_1_2CEC
loc_1_2CDB:
        mov    eax, ebx
        cdq
        and    edx, 0ffh
        add    eax, edx
        mov    byte ptr [esi + 153h], ah
loc_1_2CEC:
        xor    eax, eax
        mov    dword ptr [esi + 154h], eax
        mov    dword ptr [esi + 115h], eax
        mov    dword ptr [esi + 119h], eax
        mov    dword ptr [esi + 11dh], eax
        mov    dword ptr [esi + 121h], eax
        mov    dword ptr [esi + 10ch], eax
        mov    dword ptr [esi + 100h], eax
        mov    dword ptr [esi + 0fch], eax
        mov    dword ptr [esi + 108h], 32h
        cmp    word ptr [esi + 3ch], 4
        mov    dword ptr [esi + 104h], eax
        jae    SHORT loc_1_2D3B
        mov    word ptr [esi + 3ch], 4
loc_1_2D3B:
        xor    edx, edx
        mov    al, byte ptr [esi + 3ch]
        mov    dx, word ptr [esi + 4]
        push   esi
        mov    word ptr [esi + 12ah], 187h
        add    edx, 38h
        out    dx, al
        call   sub_1_2420
        test   al, al
        jne    SHORT loc_1_2D6D
        mov    ax, 9
        pop    edi
        mov    byte ptr [esi + 0c7h], 4
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2D6D:
        cmp    byte ptr [esi + 110h], 1
        je     SHORT loc_1_2D83
        push   esi
        call   sub_1_3B50
        mov    byte ptr [esi + 110h], 1
loc_1_2D83:
        push   esi
        call   sub_1_3FD0
        push   esi
        call   sub_1_2420
        test   al, al
        jne    SHORT loc_1_2DA5
        mov    ax, 9
        pop    edi
        mov    byte ptr [esi + 0c7h], 4
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2DA5:
        xor    ax, ax
        pop    edi
        mov    byte ptr [esi + 0c7h], 2
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2DC0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    byte ptr [eax + 0c7h], 1
        je     SHORT loc_1_2DD7
        xor    ax, ax
        pop    ebp
        ret    4
loc_1_2DD7:
        xor    edx, edx
        pop    ebp
        mov    dx, word ptr [eax + 4]
        add    edx, 0bh
        in     al, dx
        or     al, 80h
        out    dx, al
        xor    ax, ax
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_2DF0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    dx, word ptr [eax + 4]
        pop    ebp
        add    edx, 0bh
        in     al, dx
        and    al, 7fh
        out    dx, al
        xor    ax, ax
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 056h, 08Ah, 088h, 0C9h
        db  000h, 000h, 000h, 033h, 0F6h, 066h, 08Bh, 070h, 004h, 080h, 0E1h, 0F5h, 088h, 088h, 0C9h, 000h
        db  000h, 000h, 08Dh, 056h, 030h, 032h, 0C0h, 0EEh, 08Dh, 056h, 03Fh, 08Ah, 0C1h, 0EEh, 066h, 033h
        db  0C0h, 05Eh, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h
        db  024h, 000h, 000h, 000h, 000h
sub_1_2E50:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    cl, byte ptr [eax + 0c9h]
        xor    esi, esi
        mov    si, word ptr [eax + 4]
        and    cl, 0cah
        mov    byte ptr [eax + 0c9h], cl
        lea    edx, [esi + 30h]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 3fh]
        mov    al, cl
        out    dx, al
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h
        db  08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 056h, 08Ah, 088h, 0C9h, 000h, 000h, 000h, 033h, 0F6h, 066h
        db  08Bh, 070h, 004h, 080h, 0C9h, 00Ah, 088h, 088h, 0C9h, 000h, 000h, 000h, 08Dh, 056h, 030h, 032h
        db  0C0h, 0EEh, 08Dh, 056h, 03Fh, 08Ah, 0C1h, 0EEh, 066h, 033h, 0C0h, 05Eh, 05Dh, 0C2h, 004h, 000h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2ED0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    cl, byte ptr [eax + 0c9h]
        xor    esi, esi
        mov    si, word ptr [eax + 4]
        or     cl, 35h
        mov    byte ptr [eax + 0c9h], cl
        lea    edx, [esi + 30h]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 3fh]
        mov    al, cl
        out    dx, al
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2F10:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 0c7h]
        cmp    al, 1
        jne    SHORT loc_1_2F29
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
loc_1_2F29:
        cmp    al, 3
        jne    SHORT loc_1_2F38
        push   esi
        call   sub_1_2BD0
        test   ax, ax
        jne    SHORT loc_1_2F86
loc_1_2F38:
        cmp    byte ptr [esi + 0c7h], 2
        je     SHORT loc_1_2F4A
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_2F4A:
        xor    ecx, ecx
        mov    al, 2
        mov    cx, word ptr [esi + 4]
        test   byte ptr [esi + 0bfh], 10h
        lea    edx, [ecx + 30h]
        out    dx, al
        lea    edx, [ecx + 3dh]
        je     SHORT loc_1_2F66
        mov    al, 1
        jmp    SHORT loc_1_2F68
loc_1_2F66:
        xor    al, al
loc_1_2F68:
        out    dx, al
        mov    byte ptr [esi + 0c7h], 1
        push   esi
        call   sub_1_2DC0
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 26h
        in     al, dx
        or     al, 1
        out    dx, al
        xor    ax, ax
loc_1_2F86:
        pop    esi
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_2F90:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0c7h], 1
        je     SHORT loc_1_2FA9
        mov    ax, 0ah
        pop    esi
        pop    ebp
        ret    4
loc_1_2FA9:
        xor    edx, edx
        push   esi
        mov    dx, word ptr [esi + 4]
        add    edx, 26h
        in     al, dx
        and    al, 0feh
        out    dx, al
        call   sub_1_2DF0
        xor    edx, edx
        mov    al, 1
        mov    byte ptr [esi + 0c7h], 3
        mov    dx, word ptr [esi + 4]
        pop    esi
        add    edx, 30h
        pop    ebp
        out    dx, al
        xor    ax, ax
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2FE0:
        push   ebp
        xor    cl, cl
        mov    ebp, esp
        xor    edx, edx
        push   esi
        mov    al, 20h
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 4]
        mov    edi, 0
        add    edx, 30h
        out    dx, al
        mov    eax, dword ptr [esi + 15eh]
        test   eax, eax
        je     SHORT loc_1_3008
        mov    edi, dword ptr [eax + 0ch]
loc_1_3008:
        test   edi, 6
        je     SHORT loc_1_3024
        push   esi
        lea    eax, [esi + 0cbh]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_4632
        mov    cl, 8
loc_1_3024:
        test   edi, 8
        je     SHORT loc_1_302F
        or     cl, 4
loc_1_302F:
        test   edi, 20h
        je     SHORT loc_1_303A
        or     cl, 1ch
loc_1_303A:
        or     cl, 1
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        mov    al, cl
        add    edx, 3ch
        pop    edi
        out    dx, al
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 066h, 0B8h
        db  080h, 000h, 0C2h, 004h, 000h, 08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3070:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        xor    edx, edx
        push   ebx
        push   esi
        mov    ebx, 1
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 4]
        mov    byte ptr [ebp - 1], bl
        add    edx, 37h
        in     al, dx
        and    al, 35h
        je     NEAR PTR loc_1_3254
        mov    byte ptr [ebp - 1], 0
loc_1_309A:
        test   al, 10h
        je     SHORT loc_1_30AF
        push   esi
        mov    byte ptr [esi + 150h], bl
        call   sub_1_2860
        jmp    NEAR PTR loc_1_3242
loc_1_30AF:
        test   al, 20h
        je     SHORT loc_1_3108
        xor    edx, edx
        mov    ecx, dword ptr [esi + 93h]
        mov    dx, word ptr [esi + 4]
        add    edx, 3dh
        in     al, dx
        mov    edi, eax
        xor    edx, edx
        and    edi, 0ffh
        add    dword ptr [ecx], edi
        mov    ecx, dword ptr [esi + 5fh]
        mov    dx, word ptr [esi + 4]
        add    edx, 3eh
        in     al, dx
        mov    edi, eax
        xor    edx, edx
        and    edi, 0ffh
        add    dword ptr [ecx], edi
        mov    ecx, dword ptr [esi + 63h]
        mov    dx, word ptr [esi + 4]
        add    edx, 3fh
        in     al, dx
        and    eax, 0ffh
        xor    edx, edx
        add    dword ptr [ecx], eax
        mov    al, 20h
        mov    dx, word ptr [esi + 4]
        add    edx, 37h
        jmp    NEAR PTR loc_1_3241
loc_1_3108:
        test   bl, al
        je     SHORT loc_1_3117
        push   esi
        call   sub_1_2860
        jmp    NEAR PTR loc_1_3242
loc_1_3117:
        test   al, 40h
        je     NEAR PTR loc_1_31DB
        test   byte ptr [esi + 43h], 4
        je     SHORT loc_1_3135
        xor    edx, edx
        mov    al, 40h
        mov    dx, word ptr [esi + 4]
        add    edx, 37h
        jmp    NEAR PTR loc_1_3241
loc_1_3135:
        mov    al, byte ptr [esi + 13fh]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    ecx, [esi + 0cbh]
        push   ecx
        mov    byte ptr [esi + 13ch], al
        call   sub_1_4632
        mov    ecx, dword ptr [esi + 154h]
        mov    dx, word ptr [ecx + 10h]
        mov    word ptr [ebp - 8], dx
        mov    eax, dword ptr [ebp - 8]
        mov    al, ah
        mov    ah, dl
        mov    ecx, eax
        and    ecx, 0ffffh
        mov    dword ptr [ebp - 8], ecx
        cmp    ecx, 5dch
        jbe    SHORT loc_1_318B
        xor    edx, edx
        mov    al, 40h
        mov    dx, word ptr [esi + 4]
        add    edx, 37h
        jmp    NEAR PTR loc_1_3241
loc_1_318B:
        add    dword ptr [ebp - 8], 12h
        cmp    dword ptr [ebp - 8], 40h
        jae    SHORT loc_1_319C
        mov    dword ptr [ebp - 8], 40h
loc_1_319C:
        cmp    dword ptr [ebp - 8], 100h
        jae    SHORT loc_1_31AA
        mov    cl, byte ptr [esi + 3ch]
        jmp    SHORT loc_1_31B3
loc_1_31AA:
        mov    ecx, dword ptr [ebp - 8]
        lea    ecx, [ecx + ecx*2]
        shr    ecx, 4
loc_1_31B3:
        xor    edi, edi
        mov    al, 40h
        mov    di, word ptr [esi + 4]
        lea    edx, [edi + 37h]
        out    dx, al
        lea    edx, [edi + 38h]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 37h]
        in     al, dx
        test   bl, al
        je     SHORT loc_1_31D5
        push   esi
        call   sub_1_2860
        jmp    SHORT loc_1_3242
loc_1_31D5:
        test   al, 4
        je     SHORT loc_1_3242
        jmp    SHORT loc_1_31DF
loc_1_31DB:
        test   al, 4
        je     SHORT loc_1_3242
loc_1_31DF:
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 3ch
        in     al, dx
        test   al, 2
        mov    cl, al
        je     SHORT loc_1_31F4
        mov    eax, dword ptr [esi + 5fh]
        inc    dword ptr [eax]
loc_1_31F4:
        test   cl, 4
        je     SHORT loc_1_3210
        xor    edx, edx
        mov    edi, dword ptr [esi + 93h]
        mov    dx, word ptr [esi + 4]
        add    edx, 3dh
        in     al, dx
        and    eax, 0ffh
        add    dword ptr [edi], eax
loc_1_3210:
        test   cl, 8
        je     SHORT loc_1_321D
        mov    eax, dword ptr [esi + 97h]
        inc    dword ptr [eax]
loc_1_321D:
        test   cl, 10h
        je     SHORT loc_1_3236
        xor    edx, edx
        mov    ecx, dword ptr [esi + 63h]
        mov    dx, word ptr [esi + 4]
        add    edx, 3fh
        in     al, dx
        and    eax, 0ffh
        add    dword ptr [ecx], eax
loc_1_3236:
        xor    edx, edx
        mov    al, 4
        mov    dx, word ptr [esi + 4]
        add    edx, 37h
loc_1_3241:
        out    dx, al
loc_1_3242:
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 37h
        in     al, dx
        and    al, 35h
        jne    NEAR PTR loc_1_309A
loc_1_3254:
        cmp    byte ptr [esi + 0cah], 0
        je     SHORT loc_1_3270
        xor    ax, ax
        pop    edi
        mov    byte ptr [esi + 0cah], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3270:
        mov    eax, 0
        pop    edi
        cmp    byte ptr [ebp - 1], 1
        adc    eax, -1
        pop    esi
        and    ax, 3
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3290:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 4]
        add    edx, 37h
        in     al, dx
        and    al, 1ah
        test   al, 0ah
        je     NEAR PTR loc_1_3403
loc_1_32AD:
        test   al, 2
        jne    NEAR PTR loc_1_339C
        test   al, 8
        je     NEAR PTR loc_1_33EF
        xor    ecx, ecx
        mov    al, 8
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 37h]
        out    dx, al
        lea    edx, [ecx + 34h]
        in     al, dx
        mov    bl, al
        and    bl, 8
        je     SHORT loc_1_32DC
        mov    eax, dword ptr [esi + 9fh]
        jmp    SHORT loc_1_32E6
loc_1_32DC:
        test   al, 20h
        je     SHORT loc_1_32E8
        mov    eax, dword ptr [esi + 0b7h]
loc_1_32E6:
        inc    dword ptr [eax]
loc_1_32E8:
        dec    byte ptr [esi + 129h]
        jne    SHORT loc_1_335E
        mov    byte ptr [esi + 129h], 3
        cmp    byte ptr [esi + 0ebh], 0
        je     NEAR PTR loc_1_33EF
        mov    byte ptr [esi + 0ebh], 0
        mov    byte ptr [esi + 331h], 0
        movsx  eax, word ptr [esi + 0e9h]
        mov    dword ptr [esi + eax*4 + 0f4h], 0
        mov    word ptr [esi + 0e9h], 0ffffh
        cmp    word ptr [esi + 0e7h], -1
        je     SHORT loc_1_333D
        push   esi
        call   sub_1_2610
loc_1_333D:
        push   esi
        mov    eax, 0ch
        cmp    bl, 1
        adc    eax, -1
        push   eax
        call   sub_1_38D0
        cmp    ax, 5
        je     NEAR PTR loc_1_3403
        jmp    NEAR PTR loc_1_33EF
loc_1_335E:
        xor    edi, edi
        xor    eax, eax
        mov    di, word ptr [esi + 4]
        movsx  ebx, word ptr [esi + 0e9h]
        mov    cx, word ptr [esi + ebx*4 + 0ech]
        lea    edx, [edi + 34h]
        mov    ax, word ptr [esi + 0c3h]
        imul   eax, ebx
        add    eax, dword ptr [esi + 18h]
        mov    al, ah
        out    dx, al
        lea    edx, [edi + 35h]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 36h]
        mov    al, ch
        out    dx, al
        lea    edx, [edi + 30h]
        mov    al, 26h
        out    dx, al
        jmp    SHORT loc_1_33EF
loc_1_339C:
        xor    edx, edx
        mov    al, 2
        mov    byte ptr [esi + 129h], 3
        mov    dx, word ptr [esi + 4]
        add    edx, 37h
        out    dx, al
        movsx  eax, word ptr [esi + 0e9h]
        mov    byte ptr [esi + 0ebh], 0
        mov    dword ptr [esi + eax*4 + 0f4h], 0
        mov    word ptr [esi + 0e9h], 0ffffh
        cmp    word ptr [esi + 0e7h], -1
        je     SHORT loc_1_33E1
        push   esi
        call   sub_1_2610
loc_1_33E1:
        push   esi
        push   0
        call   sub_1_38D0
        cmp    ax, 5
        je     SHORT loc_1_3403
loc_1_33EF:
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 37h
        in     al, dx
        and    al, 1ah
        test   al, 0ah
        jne    NEAR PTR loc_1_32AD
loc_1_3403:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_1_3410:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    di, word ptr [esi + 0e5h]
        movsx  eax, di
        lea    ebx, [esi + eax*4 + 0f4h]
        cmp    dword ptr [ebx], 0
        je     SHORT loc_1_343A
        mov    ax, 6
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_343A:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx], 1
        push   eax
        push   esi
        call   sub_1_26A0
        mov    ax, word ptr [esi + 0e5h]
        inc    ax
        mov    word ptr [esi + 0e5h], ax
        cmp    ax, 2
        jne    SHORT loc_1_3469
        mov    word ptr [esi + 0e5h], 0
loc_1_3469:
        mov    ax, word ptr [esi + 42h]
        test   al, 20h
        jne    SHORT loc_1_34A2
        mov    dword ptr [ebx], 2
        cmp    word ptr [esi + 0e7h], -1
        jne    SHORT loc_1_34AA
        cmp    word ptr [esi + 0e9h], -1
        mov    word ptr [esi + 0e7h], di
        jne    SHORT loc_1_34AA
        push   esi
        call   sub_1_2610
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_34A2:
        and    ax, 0ffdfh
        mov    word ptr [esi + 42h], ax
loc_1_34AA:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_34C0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 18h
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 10h]
        push   edi
        mov    esi, dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi + 158h]
        add    ecx, edi
        cmp    ecx, eax
        jbe    SHORT loc_1_34ED
        sub    eax, dword ptr [ebp + 10h]
        mov    edi, eax
loc_1_34ED:
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        mov    ecx, dword ptr [ebp + 14h]
        push   0
        mov    dword ptr [ebp - 4], edi
        push   0
        push   ecx
        call   sub_1_4668
        lea    ecx, [ebp - 18h]
        lea    edx, [ebp - 14h]
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   edx
        push   eax
        call   sub_1_468C
        mov    cl, byte ptr [esi + 13dh]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    edx, [esi + 0cbh]
        push   edx
        mov    byte ptr [esi + 13ch], cl
        mov    dword ptr [ebp - 8], 0
        call   sub_1_4632
        mov    ebx, dword ptr [esi + 14ch]
        xor    eax, eax
        add    ebx, dword ptr [ebp + 10h]
        mov    al, byte ptr [esi + 0e4h]
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 10h]
        test   edi, edi
        lea    edx, [eax + ecx + 4]
        mov    dword ptr [ebp - 10h], edx
        je     NEAR PTR loc_1_369C
loc_1_355F:
        mov    eax, dword ptr [ebp - 18h]
        mov    edi, dword ptr [ebp - 4]
        sub    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 4], eax
        jb     SHORT loc_1_356F
        mov    edi, eax
loc_1_356F:
        mov    eax, dword ptr [esi + 130h]
        lea    ecx, [edi + ebx]
        cmp    ecx, eax
        jbe    SHORT loc_1_3580
        mov    edi, eax
        sub    edi, ebx
loc_1_3580:
        push   edi
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [esi + 0fch], 0
        mov    word ptr [esi + 4ah], 5
        push   ebx
        push   eax
        add    ebx, edi
        call   sub_1_4A20
        mov    eax, dword ptr [ebp + 8]
        add    dword ptr [ebp - 10h], edi
        add    dword ptr [ebp - 8], edi
        sub    dword ptr [ebp - 4], edi
        add    dword ptr [eax], edi
        xor    edi, edi
        mov    ecx, dword ptr [esi + 0fch]
loc_1_35B3:
        test   ecx, ecx
        je     SHORT loc_1_35E6
        mov    eax, edi
        inc    edi
        cmp    eax, 0ffffh
        jae    SHORT loc_1_35E6
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0bh
        in     al, dx
        test   al, 10h
        je     SHORT loc_1_35B3
        mov    dword ptr [esi + 0fch], 0
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0ah
        in     al, dx
        and    al, 7fh
        out    dx, al
loc_1_35E6:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_369C
        mov    eax, dword ptr [ebp - 8]
        add    dword ptr [ebp - 14h], eax
        cmp    eax, dword ptr [ebp - 18h]
        jne    SHORT loc_1_362A
        lea    eax, [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        push   ecx
        call   sub_1_46B0
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_369C
        lea    eax, [ebp - 18h]
        lea    ecx, [ebp - 14h]
        push   eax
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        push   edx
        call   sub_1_468C
        mov    dword ptr [ebp - 8], 0
loc_1_362A:
        cmp    dword ptr [esi + 130h], ebx
        ja     SHORT loc_1_3676
        mov    al, byte ptr [esi + 13ch]
        inc    al
        mov    byte ptr [esi + 13ch], al
        cmp    byte ptr [esi + 13eh], al
        jne    SHORT loc_1_365E
        mov    byte ptr [esi + 13dh], 0
        mov    byte ptr [esi + 13ch], 0
        mov    ebx, dword ptr [esi + 12ch]
        jmp    SHORT loc_1_3664
loc_1_365E:
        mov    ebx, dword ptr [esi + 138h]
loc_1_3664:
        push   esi
        lea    eax, [esi + 0cbh]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_4632
loc_1_3676:
        mov    eax, dword ptr [esi + 1ch]
        add    eax, dword ptr [ebp - 10h]
        cmp    eax, dword ptr [esi + 134h]
        jb     SHORT loc_1_3692
        xor    eax, eax
        mov    al, byte ptr [esi + 152h]
        shl    eax, 8
        mov    dword ptr [ebp - 10h], eax
loc_1_3692:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_355F
loc_1_369C:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_36B0:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    ecx, dword ptr [ebp + 0ch]
        push   esi
        mov    dword ptr [ebp - 8], eax
        push   edi
        mov    esi, dword ptr [ebp + 14h]
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 158h]
        lea    edx, [edi + ecx]
        cmp    edx, eax
        jbe    SHORT loc_1_36DA
        sub    eax, ecx
        mov    edi, eax
loc_1_36DA:
        mov    ebx, dword ptr [esi + 14ch]
        mov    al, byte ptr [esi + 13dh]
        push   esi
        lea    ecx, [esi + 0cbh]
        db  068h, 000h, 000h, 000h, 000h
        mov    byte ptr [esi + 13ch], al
        push   ecx
        call   sub_1_4632
        test   edi, edi
        je     NEAR PTR loc_1_378A
loc_1_3706:
        mov    eax, dword ptr [esi + 130h]
        lea    ecx, [edi + ebx]
        mov    dword ptr [ebp - 4], edi
        cmp    ecx, eax
        jbe    SHORT loc_1_371B
        sub    eax, ebx
        mov    dword ptr [ebp - 4], eax
loc_1_371B:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ebx
        push   ecx
        call   sub_1_4A20
        add    ebx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    dword ptr [ebp + 10h], ecx
        sub    edi, ecx
        je     SHORT loc_1_378A
        cmp    dword ptr [esi + 130h], ebx
        ja     SHORT loc_1_3782
        mov    al, byte ptr [esi + 13ch]
        inc    al
        mov    byte ptr [esi + 13ch], al
        cmp    byte ptr [esi + 13eh], al
        jne    SHORT loc_1_376A
        mov    byte ptr [esi + 13dh], 0
        mov    byte ptr [esi + 13ch], 0
        mov    ebx, dword ptr [esi + 12ch]
        jmp    SHORT loc_1_3770
loc_1_376A:
        mov    ebx, dword ptr [esi + 138h]
loc_1_3770:
        push   esi
        lea    eax, [esi + 0cbh]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_4632
loc_1_3782:
        test   edi, edi
        jne    NEAR PTR loc_1_3706
loc_1_378A:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_37A0:
        push   ebp
        mov    al, 60h
        mov    ebp, esp
        push   esi
        xor    esi, esi
        mov    ecx, dword ptr [ebp + 8]
        mov    si, word ptr [ecx + 4]
        lea    edx, [esi + 30h]
        out    dx, al
        lea    edx, [esi + 37h]
        in     al, dx
        xor    edx, edx
        mov    byte ptr [ecx + 0e3h], al
        mov    dx, word ptr [ecx + 4]
        mov    al, 20h
        add    edx, 30h
        pop    esi
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
loc_1_37D0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 4]
        add    edx, 8
        in     al, dx
        mov    bl, al
        mov    al, byte ptr [ecx + 13ch]
        and    bl, 0fch
        or     al, bl
        pop    ebx
        out    dx, al
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3800:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        xor    edi, edi
        mov    di, word ptr [eax + 4]
        mov    cl, 18h
        mov    al, 60h
        lea    esi, [edi + 30h]
        mov    edx, esi
        out    dx, al
loc_1_3819:
        xor    eax, eax
        mov    al, cl
        inc    cl
        cmp    cl, 1fh
        lea    edx, [eax + edi + 20h]
        mov    al, 0ffh
        out    dx, al
        jbe    SHORT loc_1_3819
        mov    edx, esi
        mov    al, 20h
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 056h
        db  057h, 033h, 0FFh, 066h, 08Bh, 078h, 004h, 0B1h, 018h, 0B0h, 060h, 08Dh, 077h, 030h, 08Bh, 0D6h
        db  0EEh, 033h, 0C0h, 08Ah, 0C1h, 0FEh, 0C1h, 08Dh, 054h, 038h, 020h, 032h, 0C0h, 0EEh, 080h, 0F9h
        db  01Fh, 076h, 0EEh, 08Bh, 0D6h, 0B0h, 020h, 0EEh, 0B0h, 001h, 05Fh, 05Eh, 05Dh, 0C2h, 004h, 000h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 055h, 08Bh, 04Ch, 024h, 00Ch, 066h, 083h, 07Ch
        db  024h, 008h, 001h, 08Bh, 0ECh, 08Bh, 091h, 015h, 001h, 000h, 000h, 08Bh, 081h, 021h, 001h, 000h
        db  000h, 0C7h, 081h, 015h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 08Bh, 000h, 08Bh, 089h, 011h
        db  001h, 000h, 000h, 050h, 0B8h, 000h, 000h, 000h, 000h, 083h, 0D0h, 0FFh, 025h, 001h, 000h, 000h
        db  0C0h, 050h, 052h, 051h, 0E8h, 0C7h, 00Ch, 000h, 000h, 066h, 033h, 0C0h, 05Dh, 0C2h, 008h, 000h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_38D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 186h]
        test   ecx, ecx
        jne    SHORT loc_1_38ED
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_38ED:
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [esi + 186h], eax
        test   eax, eax
        jne    SHORT loc_1_3904
        mov    dword ptr [esi + 18ah], 0
loc_1_3904:
        cmp    byte ptr [ecx + 2ch], 0
        mov    edi, dword ptr [ecx + 28h]
        je     SHORT loc_1_393F
        cmp    dword ptr [esi + 18eh], 0
        jne    SHORT loc_1_3924
        mov    dword ptr [esi + 192h], ecx
        mov    dword ptr [esi + 18eh], ecx
        jmp    SHORT loc_1_3933
loc_1_3924:
        mov    eax, dword ptr [esi + 192h]
        mov    dword ptr [eax + 24h], ecx
        mov    dword ptr [esi + 192h], ecx
loc_1_3933:
        mov    dword ptr [ecx + 24h], 0
        jmp    NEAR PTR loc_1_39E1
loc_1_393F:
        mov    ax, word ptr [ebp + 8]
        test   ax, ax
        jne    SHORT loc_1_3950
        inc    dword ptr [esi + 1a3h]
        jmp    SHORT loc_1_3956
loc_1_3950:
        inc    dword ptr [esi + 1abh]
loc_1_3956:
        cmp    ax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0c0000001h
        push   eax
        push   ecx
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_4582
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_39E1
        mov    eax, dword ptr [ebx + 172h]
        cmp    edi, eax
        jne    SHORT loc_1_398E
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 172h], eax
        jmp    SHORT loc_1_39A1
loc_1_398E:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_399B
loc_1_3993:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_3993
loc_1_399B:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_39A1:
        mov    eax, dword ptr [ebx + 20eh]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_39B2
        push   ebx
        call   sub_1_0A80
loc_1_39B2:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_457C
        push   0
        push   75h
        push   edi
        call   sub_1_4644
        cmp    dword ptr [ebx + 172h], 0
        jne    SHORT loc_1_39E1
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 0cbh
        push   ebx
        call   sub_1_4632
loc_1_39E1:
        mov    edi, dword ptr [esi + 17eh]
        test   edi, edi
        je     NEAR PTR loc_1_3A85
        cmp    byte ptr [esi + 204h], 0
        jne    NEAR PTR loc_1_3A85
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [esi + 17eh], eax
        test   eax, eax
        jne    SHORT loc_1_3A13
        mov    dword ptr [esi + 182h], 0
loc_1_3A13:
        push   esi
        push   edi
        call   sub_1_3410
        cmp    ax, 6
        jne    SHORT loc_1_3A53
        mov    eax, dword ptr [esi + 17eh]
        test   eax, eax
        je     SHORT loc_1_3A3D
        mov    dword ptr [edi + 24h], eax
        xor    ax, ax
        mov    dword ptr [esi + 17eh], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_3A3D:
        xor    ax, ax
        mov    dword ptr [esi + 17eh], edi
        mov    dword ptr [esi + 182h], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_3A53:
        test   ax, ax
        jne    SHORT loc_1_3A85
        cmp    dword ptr [esi + 186h], 0
        jne    SHORT loc_1_3A6F
        mov    dword ptr [esi + 18ah], edi
        mov    dword ptr [esi + 186h], edi
        jmp    SHORT loc_1_3A7E
loc_1_3A6F:
        mov    eax, dword ptr [esi + 18ah]
        mov    dword ptr [eax + 24h], edi
        mov    dword ptr [esi + 18ah], edi
loc_1_3A7E:
        mov    dword ptr [edi + 24h], 0
loc_1_3A85:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_1_3A90:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [edi + 19ah], 1
        mov    dword ptr [edi + 196h], 0
        mov    ebx, dword ptr [edi + 20eh]
        add    ebx, 0eh
        cmp    ebx, esi
        jb     SHORT loc_1_3ABC
        mov    ebx, esi
loc_1_3ABC:
        push   edi
        lea    eax, [edi + 222h]
        push   eax
        push   0
        push   ebx
        call   sub_1_36B0
        test   ax, ax
        je     SHORT loc_1_3ADB
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_3ADB:
        inc    dword ptr [edi + 1a7h]
        cmp    esi, 0eh
        jae    SHORT loc_1_3B11
        cmp    esi, 6
        jb     SHORT loc_1_3B37
        push   0
        lea    eax, [edi + 222h]
        push   0
        mov    edx, dword ptr [edi + 15eh]
        push   0
        push   esi
        push   eax
        push   eax
        push   edi
        push   edx
        call   sub_1_4730
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_3B11:
        sub    esi, 0eh
        sub    ebx, 0eh
        push   esi
        lea    eax, [edi + 230h]
        push   ebx
        lea    ecx, [edi + 222h]
        push   eax
        push   0eh
        push   ecx
        mov    eax, dword ptr [edi + 15eh]
        push   ecx
        push   edi
        push   eax
        call   sub_1_4730
loc_1_3B37:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3B50:
        push   ebp
        mov    edx, 0c80h
        in     eax, dx
        mov    ebp, esp
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        cmp    edx, esi
        je     SHORT loc_1_3B7E
        db  0B9h, 000h, 000h, 000h, 000h
loc_1_3B70:
        cmp    edx, eax
        je     SHORT loc_1_3B9E
        mov    edx, dword ptr [ecx + 4]
        add    ecx, 4
        test   edx, edx
        jne    SHORT loc_1_3B70
loc_1_3B7E:
        mov    edi, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [edi + 4]
        add    edx, 25h
        in     al, dx
        test   al, 4
        je     SHORT loc_1_3BB0
        mov    word ptr [edi + 4ah], 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3B9E:
        mov    edi, dword ptr [ebp + 8]
        mov    word ptr [edi + 4ah], 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3BB0:
        cmp    dword ptr [edi + 162h], 0
        jne    SHORT loc_1_3BC8
        mov    word ptr [edi + 4ah], 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3BC8:
        push   3
        lea    eax, [ebp - 1ch]
        push   eax
        lea    ecx, [ebp - 24h]
        push   ecx
        call   sub_1_466E
        cmp    dword ptr [ebp - 24h], 0
        je     SHORT loc_1_3BEC
        mov    word ptr [edi + 4ah], 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3BEC:
        push   100h
        mov    eax, dword ptr [edi + 162h]
        push   eax
        mov    ecx, dword ptr [ebp - 1ch]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 24h]
        push   eax
        call   sub_1_467A
        cmp    dword ptr [ebp - 24h], 0
        je     SHORT loc_1_3C27
        mov    word ptr [edi + 4ah], 5
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_4674
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3C27:
        push   100h
        mov    eax, dword ptr [edi + 162h]
        add    eax, 100h
        mov    ecx, dword ptr [ebp - 1ch]
        push   eax
        lea    edx, [ebp - 18h]
        push   ecx
        lea    eax, [ebp - 24h]
        push   edx
        push   eax
        call   sub_1_467A
        cmp    dword ptr [ebp - 24h], 0
        je     SHORT loc_1_3C70
        mov    word ptr [edi + 4ah], 5
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_4686
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_4674
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3C70:
        push   100h
        mov    eax, dword ptr [edi + 162h]
        add    eax, 200h
        mov    ecx, dword ptr [ebp - 1ch]
        push   eax
        lea    edx, [ebp - 20h]
        push   ecx
        lea    eax, [ebp - 24h]
        push   edx
        push   eax
        call   sub_1_467A
        cmp    dword ptr [ebp - 24h], 0
        je     SHORT loc_1_3CC2
        mov    word ptr [edi + 4ah], 5
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_4686
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_4686
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_4674
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3CC2:
        lea    eax, [ebp - 28h]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp - 14h]
        push   ecx
        push   edx
        call   sub_1_468C
        lea    ecx, [ebp - 28h]
        lea    edx, [ebp - 0ch]
        push   ecx
        mov    eax, dword ptr [ebp - 20h]
        push   edx
        push   eax
        call   sub_1_468C
        lea    ecx, [ebp - 28h]
        lea    edx, [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp - 18h]
        push   edx
        push   eax
        call   sub_1_468C
        xor    ecx, ecx
loc_1_3CF7:
        mov    eax, dword ptr [ebp - 10h]
        add    ecx, 4
        mov    dword ptr [eax + ecx - 4], 0a5a5a5a5h
        mov    edx, dword ptr [ebp - 0ch]
        cmp    ecx, 100h
        mov    dword ptr [edx + ecx - 4], 5a5a5a5ah
        mov    ebx, dword ptr [edi + 1ch]
        mov    dword ptr [ebx + ecx - 4], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + ecx - 4], 0
        jb     SHORT loc_1_3CF7
        xor    ecx, ecx
        xor    al, al
        mov    cx, word ptr [edi + 4]
        lea    edx, [ecx + 30h]
        out    dx, al
        lea    edx, [ecx + 3fh]
        out    dx, al
        lea    edx, [ecx + 3ch]
        mov    al, 4
        out    dx, al
        mov    eax, dword ptr [edi + 1ch]
        mov    ecx, 47474747h
        mov    dword ptr [eax + 200h], 0ffffffffh
        mov    eax, dword ptr [edi + 1ch]
        mov    word ptr [eax + 204h], 0ffffh
        xor    eax, eax
loc_1_3D64:
        mov    edx, dword ptr [edi + 1ch]
        add    eax, 4
        cmp    eax, 400h
        mov    dword ptr [edx + eax + 20ch], ecx
        jb     SHORT loc_1_3D64
        mov    dword ptr [ebp - 4], 1
loc_1_3D7F:
        test   esi, esi
        jne    NEAR PTR loc_1_3E2D
        test   byte ptr [ebp - 4], 1
        je     SHORT loc_1_3D95
        mov    eax, dword ptr [ebp - 14h]
        mov    ebx, dword ptr [ebp - 10h]
        jmp    SHORT loc_1_3D9B
loc_1_3D95:
        mov    eax, dword ptr [ebp - 20h]
        mov    ebx, dword ptr [ebp - 0ch]
loc_1_3D9B:
        push   1
        push   eax
        push   edi
        call   sub_1_3E80
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_1_3E15
        mov    eax, dword ptr [edi + 1ch]
        cmp    word ptr [eax], 0
        jne    SHORT loc_1_3E10
        mov    ecx, 1
        add    ebx, 4
        add    eax, 4
loc_1_3DBE:
        mov    edx, dword ptr [ebx]
        cmp    dword ptr [eax], edx
        jne    SHORT loc_1_3E28
        add    ebx, 4
        add    eax, 4
        inc    ecx
        cmp    ecx, 3eh
        jb     SHORT loc_1_3DBE
        push   0
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        push   edi
        call   sub_1_3E80
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_1_3E15
        mov    eax, dword ptr [ebp - 8]
        cmp    word ptr [eax], 0
        jne    SHORT loc_1_3E10
        mov    edx, 1
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 4
        mov    eax, dword ptr [edi + 1ch]
        add    eax, 4
loc_1_3DFC:
        mov    ebx, dword ptr [ecx]
        cmp    dword ptr [eax], ebx
        jne    SHORT loc_1_3E10
        add    ecx, 4
        add    eax, 4
        inc    edx
        cmp    edx, 3eh
        jb     SHORT loc_1_3DFC
        jmp    SHORT loc_1_3E15
loc_1_3E10:
        mov    esi, 1
loc_1_3E15:
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [ebp - 4]
        cmp    eax, 7d0h
        jb     NEAR PTR loc_1_3D7F
        jmp    SHORT loc_1_3E2D
loc_1_3E28:
        mov    esi, 1
loc_1_3E2D:
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_4686
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_4686
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        call   sub_1_4686
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_4674
        test   esi, esi
        je     SHORT loc_1_3E64
        mov    word ptr [edi + 4ah], 5
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3E64:
        mov    word ptr [edi + 4ah], 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3E80:
        push   ebp
        mov    ebp, esp
        sub    esp, 84h
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        lea    ecx, [ebp - 84h]
        push   edi
        xor    esi, esi
        push   eax
        mov    edx, dword ptr [ebp + 0ch]
        push   ecx
        push   edx
        call   sub_1_4692
        mov    edi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [edi + 4]
        lea    edx, [ecx + 0bh]
        in     al, dx
        and    al, 0bfh
        out    dx, al
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 1
        jne    SHORT loc_1_3EDA
        lea    edx, [ecx + 10h]
        mov    eax, dword ptr [ebp - 84h]
        out    dx, eax
        push   edi
        call   sub_1_3F40
        xor    edx, edx
        mov    eax, 280fah
        mov    dx, word ptr [edi + 4]
        add    edx, 14h
        jmp    SHORT loc_1_3EFF
loc_1_3EDA:
        test   eax, eax
        jne    SHORT loc_1_3F00
        lea    edx, [ecx + 10h]
        mov    eax, dword ptr [ebp - 84h]
        add    eax, 2
        push   edi
        out    dx, eax
        call   sub_1_3F40
        xor    edx, edx
        mov    eax, 2c0fah
        mov    dx, word ptr [edi + 4]
        add    edx, 14h
loc_1_3EFF:
        out    dx, eax
loc_1_3F00:
        xor    ebx, ebx
        xor    ecx, ecx
        mov    cx, word ptr [edi + 4]
        lea    edx, [ecx + 0bh]
loc_1_3F0B:
        in     al, dx
        inc    ebx
        cmp    ebx, 0ffffh
        ja     SHORT loc_1_3F1B
        test   al, 2
        jne    SHORT loc_1_3F0B
        jmp    SHORT loc_1_3F20
loc_1_3F1B:
        mov    esi, 1
loc_1_3F20:
        lea    edx, [ecx + 0ah]
        pop    edi
        in     al, dx
        and    al, 7fh
        out    dx, al
        mov    eax, esi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3F40:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        mov    di, word ptr [esi + 4]
        lea    ecx, [edi + 37h]
        mov    edx, ecx
        in     al, dx
        test   al, 75h
        mov    bl, al
        je     SHORT loc_1_3F8B
        mov    al, 75h
        out    dx, al
        lea    edx, [edi + 33h]
        in     al, dx
        inc    al
        cmp    al, byte ptr [esi + 153h]
        jb     SHORT loc_1_3F72
        mov    al, byte ptr [esi + 152h]
loc_1_3F72:
        cmp    byte ptr [esi + 152h], al
        jne    SHORT loc_1_3F84
        xor    eax, eax
        mov    al, byte ptr [esi + 153h]
        jmp    SHORT loc_1_3F89
loc_1_3F84:
        and    eax, 0ffh
loc_1_3F89:
        dec    eax
        out    dx, al
loc_1_3F8B:
        lea    esi, [edi + 30h]
        mov    edx, esi
        in     al, dx
        test   al, 4
        jne    SHORT loc_1_3FBC
        test   bl, 1ah
        je     SHORT loc_1_3F9F
        mov    edx, ecx
        mov    al, 1ah
        out    dx, al
loc_1_3F9F:
        lea    edx, [edi + 3dh]
        mov    al, 4
        out    dx, al
        lea    edx, [edi + 34h]
        mov    al, 2
        out    dx, al
        lea    edx, [edi + 35h]
        xor    al, al
        out    dx, al
        lea    edx, [edi + 36h]
        mov    al, 4
        out    dx, al
        mov    edx, esi
        mov    al, 26h
        out    dx, al
loc_1_3FBC:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3FD0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        xor    esi, esi
        mov    si, word ptr [ecx + 4]
        lea    edx, [esi + 9]
        in     al, dx
        test   al, 8
        je     SHORT loc_1_4006
        lea    ecx, [esi + 24h]
        mov    edx, ecx
        in     al, dx
        mov    bl, al
        or     al, 80h
        out    dx, al
        lea    edx, [esi + 2dh]
        in     al, dx
        and    al, 0fch
        pop    esi
        out    dx, al
        mov    edx, ecx
        mov    al, bl
        out    dx, al
        xor    eax, eax
        pop    ebx
        pop    ebp
        ret    4
loc_1_4006:
        push   ecx
        call   sub_1_4020
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4020:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    cx, word ptr [eax + 4]
        mov    ebp, esp
        add    cx, 20h
        sub    esp, 4
        mov    al, 80h
        push   ebx
        push   esi
        push   edi
        movsx  edi, cx
        lea    esi, [ecx + 10h]
        lea    ecx, [edi + 1dh]
        mov    dx, si
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    dx, si
        mov    byte ptr [ebp - 2], al
        xor    al, al
        lea    ebx, [edi + 4]
        out    dx, al
        mov    edx, ecx
        mov    eax, ecx
        xor    al, al
        out    dx, al
        mov    edx, ebx
        in     al, dx
        mov    cl, al
        mov    byte ptr [ebp - 1], al
        and    cl, 0c3h
        or     cl, 80h
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 0dh]
        in     al, dx
        and    al, 0fdh
        or     al, 1
        out    dx, al
        mov    edx, ebx
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    dx, si
        mov    al, 0c0h
        out    dx, al
        lea    edx, [edi + 1fh]
        in     al, dx
        mov    dx, si
        mov    byte ptr [ebp - 3], al
        xor    al, al
        out    dx, al
        test   byte ptr [ebp - 3], 4
        je     SHORT loc_1_40A7
        mov    edx, ebx
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 0dh]
        in     al, dx
        or     al, 3
        out    dx, al
        mov    edx, ebx
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        jmp    SHORT loc_1_40B0
loc_1_40A7:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_40D0
loc_1_40B0:
        mov    dx, si
        xor    al, al
        out    dx, al
        lea    edx, [edi + 1dh]
        mov    al, byte ptr [ebp - 2]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, al
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_40D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    cx, word ptr [edi + 4]
        add    cx, 20h
        movsx  esi, cx
        lea    eax, [ecx + 10h]
        lea    ecx, [esi + 1dh]
        mov    dx, ax
        mov    word ptr [ebp - 4], ax
        mov    al, 80h
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    dx, word ptr [ebp - 4]
        xor    al, al
        out    dx, al
        mov    edx, ecx
        out    dx, al
        lea    edx, [esi + 4]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    al, 0c3h
        or     al, 80h
        out    dx, al
        lea    edx, [esi + 0dh]
        mov    byte ptr [ebp - 8], al
        in     al, dx
        and    al, 0fdh
        or     al, 1
        out    dx, al
        lea    edx, [esi + 4]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    edx, esi
        push   66h
        in     al, dx
        or     al, 40h
        db  068h, 000h, 000h, 000h, 000h
        out    dx, al
        lea    edx, [esi + 5]
        mov    al, byte ptr [edi + 15ch]
        out    dx, al
        mov    edx, dword ptr [edi + 1ch]
        push   edx
        call   sub_1_4A20
        lea    edx, [esi + 5]
        mov    al, byte ptr [edi + 15dh]
        out    dx, al
        lea    edi, [esi + 17h]
        mov    al, 0ffh
        mov    edx, edi
        out    dx, al
        mov    dx, word ptr [ebp - 4]
        in     al, dx
        and    eax, 0ffh
        and    al, 3bh
        out    dx, al
        lea    edx, [esi + 14h]
        xor    al, al
        out    dx, al
        lea    edx, [esi + 15h]
        out    dx, al
        lea    edx, [esi + 16h]
        mov    al, 4
        out    dx, al
        mov    dx, word ptr [ebp - 4]
        xor    ebx, ebx
        in     al, dx
        and    eax, 0ffh
        or     al, 6
        out    dx, al
loc_1_4181:
        push   64h
        call   sub_1_4638
        mov    edx, edi
        in     al, dx
        test   al, 0ah
        jne    SHORT loc_1_4198
        inc    ebx
        cmp    ebx, 0bb8h
        jl     SHORT loc_1_4181
loc_1_4198:
        lea    eax, [esi + 14h]
        mov    edx, eax
        in     al, dx
        mov    edx, edi
        mov    byte ptr [ebp - 2], al
        mov    al, 0ffh
        xor    ebx, ebx
        out    dx, al
        mov    dx, word ptr [ebp - 4]
        mov    al, 21h
        out    dx, al
loc_1_41AF:
        push   0ah
        call   sub_1_4638
        mov    edx, edi
        in     al, dx
        test   al, 80h
        jne    SHORT loc_1_41C6
        inc    ebx
        cmp    ebx, 0a0h
        jl     SHORT loc_1_41AF
loc_1_41C6:
        test   byte ptr [ebp - 2], 10h
        je     SHORT loc_1_41E1
        lea    edx, [esi + 4]
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        lea    edx, [esi + 0dh]
        in     al, dx
        or     al, 3
        out    dx, al
        lea    edx, [esi + 4]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
loc_1_41E1:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 4]
        add    di, 20h
        mov    dx, di
        lea    eax, [edi + 10h]
        mov    word ptr [ebp - 2], ax
        push   1
        in     al, dx
        mov    bl, al
        and    bl, 3fh
        mov    al, bl
        or     al, 80h
        out    dx, al
        call   sub_1_4638
        mov    dx, di
        mov    al, bl
        out    dx, al
        mov    eax, dword ptr [esi + 18h]
        shr    eax, 0dh
        and    al, 3fh
        or     al, 40h
        out    dx, al
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 0ah
        in     al, dx
        or     al, 40h
        out    dx, al
        mov    cl, al
        xor    ebx, ebx
        in     al, dx
        mov    al, cl
        out    dx, al
        movsx  edi, di
        mov    dx, word ptr [ebp - 2]
        mov    al, 21h
        out    dx, al
loc_1_4250:
        lea    edx, [edi + 17h]
        in     al, dx
        test   al, 80h
        jne    SHORT loc_1_4268
        push   1
        inc    ebx
        call   sub_1_4638
        cmp    ebx, 0a0h
        jb     SHORT loc_1_4250
loc_1_4268:
        cmp    ebx, 0a0h
        jb     SHORT loc_1_427E
        mov    eax, 9
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_427E:
        lea    edx, [edi + 1eh]
        mov    al, 49h
        out    dx, al
        xor    edx, edx
        xor    al, al
        mov    dx, word ptr [esi + 4]
        add    edx, 3bh
        out    dx, al
        out    dx, al
        lea    edx, [edi + 1dh]
        mov    al, 4
        out    dx, al
        mov    ax, word ptr [esi + 3ah]
        cmp    ax, 2
        ja     SHORT loc_1_42A6
        test   ax, ax
        jne    SHORT loc_1_42AC
loc_1_42A6:
        mov    word ptr [esi + 3ah], 2
loc_1_42AC:
        mov    cx, word ptr [esi + 3ah]
        lea    edx, [edi + 11h]
        imul   cx, cx, 600h
        mov    al, ch
        out    dx, al
        mov    ax, word ptr [esi + 16h]
        lea    edx, [edi + 12h]
        shl    al, 2
        out    dx, al
        lea    edx, [edi + 13h]
        mov    al, cl
        out    dx, al
        lea    eax, [ecx + 1]
        lea    edx, [edi + 17h]
        out    dx, al
        mov    eax, edx
        lea    ebx, [edi + 4]
        mov    al, 0ffh
        out    dx, al
        lea    edx, [edi + 1fh]
        mov    al, byte ptr [esi + 0c9h]
        out    dx, al
        mov    edx, ebx
        in     al, dx
        mov    cl, al
        or     cl, 80h
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 0bh]
        xor    al, al
        out    dx, al
        lea    edx, [edi + 0dh]
        in     al, dx
        or     al, 6ch
        out    dx, al
        lea    edx, [edi + 5]
        in     al, dx
        or     al, 80h
        out    dx, al
        mov    dx, word ptr [ebp - 2]
        mov    al, 21h
        out    dx, al
        mov    edx, edi
        mov    al, 40h
        out    dx, al
        in     al, dx
        mov    al, 40h
        lea    edi, [esi + 0cbh]
        out    dx, al
        mov    al, cl
        mov    edx, ebx
        and    al, 7fh
        xor    ebx, ebx
        out    dx, al
loc_1_4323:
        push   esi
        mov    byte ptr [esi + 13ch], bl
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_1_4632
        xor    ecx, ecx
        xor    eax, eax
        mov    ax, word ptr [esi + 20h]
        test   eax, 3fffffh
        jbe    SHORT loc_1_435A
loc_1_4344:
        mov    eax, dword ptr [esi + 1ch]
        inc    ecx
        mov    byte ptr [eax + ecx - 1], 0
        xor    eax, eax
        mov    ax, word ptr [esi + 20h]
        shl    eax, 0ah
        cmp    eax, ecx
        ja     SHORT loc_1_4344
loc_1_435A:
        inc    ebx
        cmp    ebx, 3
        jb     SHORT loc_1_4323
        mov    byte ptr [esi + 13ch], 0
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_1_4632
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_4380:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 27h]
        in     al, dx
        cmp    al, 21h
        je     NEAR PTR loc_1_441C
        lea    edx, [ecx + 0bh]
        mov    edi, 12ch
        in     al, dx
        mov    bl, al
        or     al, 8
        out    dx, al
loc_1_43A9:
        push   3e8h
        call   sub_1_4638
        dec    edi
        jne    SHORT loc_1_43A9
        xor    ecx, ecx
        mov    al, bl
        mov    cx, word ptr [esi + 4]
        and    al, 0f7h
        xor    edi, edi
        lea    edx, [ecx + 0bh]
        out    dx, al
        lea    edx, [ecx + 21h]
        in     al, dx
        test   al, 40h
        je     SHORT loc_1_43F2
loc_1_43CE:
        cmp    edi, 12ch
        jge    SHORT loc_1_43F2
        xor    edx, edx
        push   3e8h
        mov    dx, word ptr [esi + 4]
        inc    edi
        add    edx, 21h
        in     al, dx
        mov    bl, al
        call   sub_1_4638
        test   bl, 40h
        jne    SHORT loc_1_43CE
loc_1_43F2:
        mov    dx, word ptr [esi + 4]
        mov    esi, edx
        and    esi, 0ffffh
        in     al, dx
        lea    edx, [esi + 1]
        lea    ecx, [esi + 24h]
        in     al, dx
        lea    edx, [esi + 25h]
        mov    al, 84h
        out    dx, al
        mov    edx, ecx
        mov    al, 0a0h
        out    dx, al
        lea    edx, [esi + 2dh]
        mov    al, 6dh
        out    dx, al
        mov    edx, ecx
        mov    al, 20h
        out    dx, al
loc_1_441C:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4430:
        db  0A0h, 000h, 000h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_4455
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_4457
        push   0
        push   0
        call   0
        cmp    eax, 0
        jne    SHORT loc_1_4457
loc_1_4455:
        clc
        ret
loc_1_4457:
        stc
        ret
SMC8232W_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_4468
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_4468:
        clc
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0CCh, 0CCh, 056h, 08Bh
        db  04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh, 05Eh, 0C2h
        db  00Ch, 000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh
        db  0F3h, 066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch
        db  08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h
        db  08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch, 000h, 057h
        db  08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h, 06Dh
        db  05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h
        db  010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h, 0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_4558:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_4564:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_456A:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_4570:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_457C:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_4582:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_458E:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_459A:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_45A0:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_45A6:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_45AC:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
sub_1_45BE:
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_45C4:
        db  0CDh, 020h, 044h, 080h, 028h, 000h
        db  0CDh, 020h, 048h, 080h, 028h, 000h
        db  0CDh, 020h, 049h, 080h, 028h, 000h
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 057h, 080h, 028h, 000h
        db  0CDh, 020h, 058h, 080h, 028h, 000h
        db  0CDh, 020h, 059h, 080h, 028h, 000h
        db  0CDh, 020h, 05Ah, 080h, 028h, 000h
        db  0CDh, 020h, 05Bh, 080h, 028h, 000h
        db  0CDh, 020h, 05Ch, 080h, 028h, 000h
        db  0CDh, 020h, 05Dh, 080h, 028h, 000h
        db  0CDh, 020h, 05Eh, 080h, 028h, 000h
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        int3
        int3
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_461A:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_4620:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_4626:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_462C:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
sub_1_4632:
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_4638:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_463E:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_4644:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_4668:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
sub_1_466E:
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
sub_1_4674:
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
sub_1_467A:
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
sub_1_4686:
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_468C:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
sub_1_4692:
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_46B0:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_46BC:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_46C2:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_46C8:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
        db  0CDh, 020h, 042h, 080h, 028h, 000h
        db  0CDh, 020h, 012h, 080h, 028h, 000h
        db  0CDh, 020h, 013h, 080h, 028h, 000h
        db  0CDh, 020h, 014h, 080h, 028h, 000h
        db  0CDh, 020h, 015h, 080h, 028h, 000h
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
sub_1_470A:
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_4710:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        int3
        int3
        int3
        int3
        int3
        int3
        int3
        int3
        int3
        int3
        int3
        int3
        int3
        int3
sub_1_4730:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4760
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_4760:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_4800
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_47A0
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_4795
        cmp    al, 0ffh
        jne    SHORT loc_1_4795
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_4795
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_4807
loc_1_4795:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_4807
        db  08Bh, 0FFh
loc_1_47A0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_47F0
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_47C6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_47CD
loc_1_47C6:
        mov    dword ptr [ebp - 10h], 1
loc_1_47CD:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_47E0
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_480A
        db  08Dh, 064h, 024h, 000h
loc_1_47E0:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_480A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_47F0:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_480A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_4800:
        mov    dword ptr [ebp - 4], 20h
loc_1_4807:
        mov    esi, dword ptr [ebp + 8]
loc_1_480A:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_4901
loc_1_4815:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    SHORT loc_1_486D
        test   al, 2
        je     SHORT loc_1_4859
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 086h, 0BFh, 0FFh, 0FFh
        or     al, al
        je     SHORT loc_1_4859
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    SHORT loc_1_486D
loc_1_4859:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_4864
        test   bl, 4
        jne    SHORT loc_1_486D
loc_1_4864:
        test   bl, 20h
        je     NEAR PTR loc_1_48F6
loc_1_486D:
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [ebp + 24h]
        push   eax
        mov    eax, dword ptr [ebp + 20h]
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_459A
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_48F6
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_48AF
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_48AF:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_48C0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_48C6
        db  08Bh, 0FFh
loc_1_48C0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_48C6:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_48D5
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_45A0
loc_1_48D5:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_4644
        mov    edi, ebx
        jmp    SHORT loc_1_48F9
loc_1_48F6:
        mov    edi, dword ptr [edi + 10h]
loc_1_48F9:
        or     edi, edi
        jne    NEAR PTR loc_1_4815
loc_1_4901:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_4910:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4920
        pop    ebp
        ret    4
sub_1_4920:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_499E
loc_1_4930:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_4997
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_45A0
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_4997
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_495C
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_495C:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_4970
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_4976
        db  005h, 000h, 000h, 000h, 000h
loc_1_4970:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_4976:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 1ch]
        mov    eax, 1
        mov    cl, byte ptr [edi + 19h]
        shl    eax, cl
        or     dword ptr [esi + 28h], eax
        push   0
        push   1ah
        push   edi
        call   sub_1_4644
        mov    edi, ebx
        jmp    SHORT loc_1_499A
loc_1_4997:
        mov    edi, dword ptr [edi + 10h]
loc_1_499A:
        or     edi, edi
        jne    SHORT loc_1_4930
loc_1_499E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 0ch]
        test   al, 20h
        je     SHORT loc_1_49C5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_49C5:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_4A01
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_49F2
        cmp    bl, 0ffh
        jne    SHORT loc_1_49F2
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_49F2
        test   al, 8
        je     SHORT loc_1_49EB
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_49EB:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_49F2:
        test   al, 4
        jne    SHORT loc_1_49FA
        test   al, 2
        je     SHORT loc_1_4A19
loc_1_49FA:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_4A01:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_4A19
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_4A19
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_4A19:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
sub_1_4A20:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        rep movsd
        mov    cl, al
        rep movsb
        pop    edi
        pop    esi
        ret    0ch
sub_1_4A40:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_4A60
        or     ecx, ecx
        je     SHORT loc_1_4A76
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_4A74
loc_1_4A60:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_4A74
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_4A74:
        rep movsb
loc_1_4A76:
        popfd
        pop    edi
        pop    esi
        ret    0ch
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 053h, 04Dh, 043h, 038h
        db  032h, 033h, 032h, 057h, 000h, 040h, 000h, 054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 0CCh, 0CCh, 0CCh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 040h, 028h, 023h, 029h
        db  020h, 053h, 04Dh, 043h, 038h, 032h, 033h, 032h, 057h, 02Eh, 056h, 058h, 044h, 020h, 076h, 065h
        db  072h, 073h, 069h, 06Fh, 06Eh, 020h, 031h, 02Eh, 030h, 036h, 061h, 020h, 030h, 033h, 02Fh, 031h
        db  036h, 02Fh, 039h, 035h, 00Ah, 043h, 06Fh, 070h, 079h, 072h, 069h, 067h, 068h, 074h, 020h, 028h
        db  043h, 029h, 020h, 053h, 074h, 061h, 06Eh, 064h, 061h, 072h, 064h, 020h, 04Dh, 069h, 063h, 072h
        db  06Fh, 073h, 079h, 073h, 074h, 065h, 06Dh, 073h, 020h, 043h, 06Fh, 072h, 070h, 02Ch, 020h, 031h
        db  039h, 039h, 034h, 000h, 040h, 028h, 023h, 029h, 020h, 055h, 04Dh, 05Fh, 038h, 032h, 033h, 032h
        db  05Fh, 04Eh, 044h, 049h, 053h, 033h, 05Fh, 073h, 032h, 02Eh, 032h, 05Fh, 076h, 031h, 02Eh, 030h
        db  036h, 061h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4BA4:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    byte ptr [edx], al
        add    dword ptr [edx], eax
        add    byte ptr [ebx], al
        add    dword ptr [edx], eax
        add    byte ptr [ecx + eax], al
        add    al, byte ptr [eax]
        add    eax, 1000201h
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    al, byte ptr [ecx]
        add    eax, dword ptr [ecx]
        add    al, byte ptr [ecx]
loc_1_4C20:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [ecx + eax], eax
        add    dword ptr [ecx], eax
        push   ebx
        dec    ebp
        inc    ebx
        cmp    byte ptr [edx], dh
        xor    esi, dword ptr [edx]
        push   edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4C88:
        push   ebx
        dec    ebp
        inc    ebx
        cmp    byte ptr [edx], dh
        xor    esi, dword ptr [edx]
        push   edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        dec    ecx
        dec    edi
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        dec    ebp
        inc    ecx
        pop    eax
        dec    ebp
        push   ebp
        dec    esp
        push   esp
        dec    ecx
        inc    ebx
        inc    ecx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4CB4:
        dec    ebp
        inc    ecx
        pop    eax
        dec    ebp
        push   ebp
        dec    esp
        push   esp
        dec    ecx
        inc    ebx
        inc    ecx
        push   ebx
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        push   esp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        dec    ecx
        push   edx
        push   ecx
        add    byte ptr [ecx + 52h], cl
        push   ecx
        add    byte ptr [edx + 41h], dl
        dec    ebp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
loc_1_4CF0:
        push   edx
        inc    ecx
        dec    ebp
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        push   ebx
        dec    esp
        dec    edi
        push   esp
        dec    esi
        push   ebp
        dec    ebp
        inc    edx
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        push   ebx
        dec    esp
        dec    edi
        push   esp
        dec    esi
        push   ebp
        dec    ebp
        inc    edx
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        inc    edx
        db  075h, 073h
        push   esp
        db  079h, 070h
        add    byte ptr gs:[edx + 75h], al
        jae    SHORT loc_1_4D74
        db  079h, 070h
        add    byte ptr gs:[ebp + 65h], cl
        imul   esp, dword ptr fs:[ecx + 54h], 657079h
        add    byte ptr [eax], al
        dec    ebp
        imul   esp, dword ptr fs:[ecx + 54h], 657079h
        add    byte ptr [eax], al
        dec    ebp
        popad
        js     SHORT loc_1_4DA9
        insd
        db  075h, 06Dh
        push   eax
        popad
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_4D9D
        imul   edi, dword ptr [edx + 65h], 4d000000h
        popad
        db  078h, 069h
        insd
        db  075h, 06Dh
        push   eax
        popad
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_4DB1
        imul   edi, dword ptr [edx + 65h], 53000000h
        dec    ebp
        inc    ebx
        and    byte ptr [ecx + 64h], al
        popad
        jo     SHORT loc_1_4DE1
        jb     NEAR PTR loc_1_4D9E
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4D74:
        add    byte ptr [edx], al
        add    eax, dword ptr [eax + 6]
        add    al, byte ptr [ebx]
        add    eax, 0f0b0a07h
loc_1_4D84:
        and    eax, 10f130d4h
        mov    dword ptr [23350911h], eax
        add    byte ptr [4002335h], al
        dec    ebp
        leave
        stosb
        rcr    dword ptr [edi + 33h], 70h
        add    dword ptr [eax], edx
loc_1_4D9D:
        lodsb
loc_1_4D9E:
        add    byte ptr [edx + 22h], al
        db  0F0h, 0C0h, 081h, 022h, 0F0h, 0C0h, 071h, 00Eh
loc_1_4DA9:
        adc    dword ptr [5110e19h], edx
        and    dword ptr [eax], edx
loc_1_4DB1:
        lodsb
        add    byte ptr [eax + 5], cl
        xchg   edx, eax
        add    eax, dword ptr [ecx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4DBC:
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 4dh], dl
        inc    ebx
        and    byte ptr [ecx + 75h], al
        db  074h, 06Fh
        and    byte ptr [ebp + 65h], cl
        imul   esp, dword ptr fs:[ecx + 20h], 65746544h
        arpl   word ptr [eax + 54h], si
        db  065h, 073h
loc_1_4DE1:
        db  074h, 020h
        inc    esi
        db  072h, 061h
        insd
        and    byte ptr cs:[eax + ebp*2 + 61h], dl
        outsb
        imul   esp, dword ptr [eax], 79h
        outsd
        jne    SHORT loc_1_4E14
        outsw
        jb     SHORT loc_1_4E18
        db  075h, 073h
        imul   ebp, dword ptr [esi + 67h], 434d5320h
        and    byte ptr [eax + 72h], dh
        outsd
        db  064h, 075h, 063h
        db  074h, 073h
        and    byte ptr cs:[524b5357h], ch
        push   esp
        inc    esp
        inc    esp
loc_1_4E14:
        pop    edx
        push   eax
        dec    ebp
        inc    edx
loc_1_4E18:
        inc    edi
        push   ebx
        inc    esi
        dec    ebp
        push   esp
        dec    edx
        inc    edi
        push   esi
        push   eax
        sub    eax, 0ffff0000h
        db  0FFh, 0FFh
_LTEXT ends
        end
