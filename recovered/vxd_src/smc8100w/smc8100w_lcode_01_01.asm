; smc8100w_lcode_01_01.asm -- SMC8100W object 1 segment _LTEXT  (part 1/3)
.386p
_LTEXT segment dword public use32 'LCODE'

        db  055h, 0B8h, 009h, 000h, 000h, 000h, 08Bh, 0ECh, 083h, 0ECh, 018h, 066h, 089h, 045h, 0E8h, 053h
        db  066h, 089h, 045h, 0EAh, 033h, 0DBh, 066h, 089h, 05Dh, 0F0h, 053h, 08Bh, 04Dh, 00Ch, 08Dh, 045h
        db  0F0h, 051h, 08Dh, 055h, 0F8h, 0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h, 066h, 0C7h, 045h, 0F2h
        db  001h, 000h, 066h, 0C7h, 045h, 0F4h, 0C6h, 06Eh, 050h, 052h, 0E8h, 0BBh, 0D6h, 000h, 000h, 08Dh
        db  04Dh, 0F0h, 089h, 00Dh, 04Ah, 000h, 000h, 000h, 08Bh, 055h, 0F8h, 089h, 015h, 004h, 000h, 000h
        db  000h, 088h, 01Dh, 04Eh, 000h, 000h, 000h, 089h, 01Dh, 03Eh, 000h, 000h, 000h, 089h, 01Dh, 042h
        db  000h, 000h, 000h, 0C6h, 005h, 008h, 000h, 000h, 000h, 003h, 088h, 01Dh, 009h, 000h, 000h, 000h
        db  089h, 01Dh, 00Ah, 000h, 000h, 000h, 0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 012h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 016h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0C7h, 005h, 01Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h
        db  01Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Bh, 045h, 0E8h, 08Bh, 04Dh, 0ECh, 0C7h, 005h
        db  022h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 026h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 0C7h, 005h, 02Ah, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 02Eh, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0C7h, 005h, 032h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  06Ah, 036h, 0A3h, 036h, 000h, 000h, 000h, 068h, 008h, 000h, 000h, 000h, 08Dh, 045h, 0FCh, 068h
        db  000h, 000h, 000h, 000h, 089h, 00Dh, 03Ah, 000h, 000h, 000h, 08Bh, 055h, 0F8h, 052h, 068h, 000h
        db  000h, 000h, 000h, 050h, 0E8h, 0FDh, 0D5h, 000h, 000h, 039h, 05Dh, 0FCh, 074h, 014h, 053h, 08Bh
        db  045h, 0F8h, 050h, 0E8h, 0E8h, 0D5h, 000h, 000h, 08Bh, 045h, 0FCh, 05Bh, 08Bh, 0E5h, 05Dh, 0C2h
        db  008h, 000h, 08Bh, 045h, 0FCh, 05Bh, 08Bh, 0E5h, 05Dh, 0C2h, 008h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0130:
        push   ebp
        mov    eax, 7
        mov    ebp, esp
        sub    esp, 74h
        mov    word ptr [ebp - 44h], ax
        push   ebx
        mov    word ptr [ebp - 42h], ax
        push   esi
        push   edi
        mov    eax, 4
        db  0C7h, 045h, 0C0h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D0h, 000h, 000h, 000h, 000h
        db  0C7h, 045h, 0D8h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 34h], ax
        mov    word ptr [ebp - 32h], ax
        mov    eax, 0bh
        db  0C7h, 045h, 0B8h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 2ch], ax
        mov    word ptr [ebp - 2ah], ax
        mov    eax, 8
        mov    word ptr [ebp - 4ch], ax
        xor    ebx, ebx
        mov    word ptr [ebp - 4ah], ax
        mov    byte ptr [ebp - 1], bl
        mov    eax, 12h
        xor    esi, esi
        mov    word ptr [ebp - 3ch], ax
        mov    byte ptr [ebp - 2], bl
        mov    word ptr [ebp - 3ah], ax
        mov    byte ptr [ebp - 24h], bl
        lea    eax, [ebp - 23h]
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        lea    edx, [ebp - 10h]
        push   edx
        mov    dword ptr [eax], ebx
        db  0C7h, 045h, 0C8h, 000h, 000h, 000h, 000h
        mov    word ptr [ebp - 4], 280h
        mov    byte ptr [eax + 4], bl
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_D864
        cmp    dword ptr [ebp - 0ch], ebx
        je     SHORT loc_1_01DC
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_01DC:
        push   1
        lea    eax, [ebp - 4ch]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_D86A
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_020C
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        sub    eax, 3
        cmp    eax, 1
        sbb    al, al
        neg    al
        mov    byte ptr [ebp - 2], al
loc_1_020C:
        cmp    byte ptr [ebp - 2], 1
        je     SHORT loc_1_0239
        push   1
        lea    eax, [ebp - 44h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_D86A
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_0239
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [eax + 4]
        mov    word ptr [ebp - 4], cx
loc_1_0239:
        push   1
        lea    eax, [ebp - 3ch]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_D86A
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_025C
        mov    eax, dword ptr [ebp - 14h]
        mov    esi, dword ptr [eax + 4]
loc_1_025C:
        mov    eax, dword ptr [ebp - 10h]
        lea    ecx, [ebp - 1ch]
        push   eax
        lea    edx, [ebp - 18h]
        push   ecx
        lea    eax, [ebp - 0ch]
        push   edx
        push   eax
        call   sub_1_D76C
        cmp    dword ptr [ebp - 1ch], 6
        jne    SHORT loc_1_02A5
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_02A5
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, eax
        mov    cl, byte ptr [eax]
        mov    byte ptr [ebp - 24h], cl
        mov    dl, byte ptr [eax + 1]
        mov    byte ptr [ebp - 23h], dl
        mov    cl, byte ptr [eax + 2]
        mov    byte ptr [ebp - 22h], cl
        mov    dl, byte ptr [eax + 3]
        mov    byte ptr [ebp - 21h], dl
        mov    cl, byte ptr [eax + 4]
        mov    byte ptr [ebp - 20h], cl
        mov    al, byte ptr [eax + 5]
        mov    byte ptr [ebp - 1fh], al
loc_1_02A5:
        xor    edi, edi
loc_1_02A7:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 8]
        push   eax
        push   edi
        push   47ah
        push   ecx
        call   sub_1_D7E6
        mov    dword ptr [ebp - 0ch], eax
        cmp    eax, edi
        je     SHORT loc_1_02CE
        mov    byte ptr [ebp - 1], 1
        mov    ebx, 0aa000002h
        jmp    SHORT loc_1_02A7
loc_1_02CE:
        mov    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    ecx, 11eh
        rep stosd
        stosw
        cmp    byte ptr [ebp - 1], al
        jne    SHORT loc_1_030C
        cmp    byte ptr [ebp - 2], 1
        jne    SHORT loc_1_030C
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_5E93
        test   ax, ax
        je     SHORT loc_1_030C
        mov    byte ptr [ebp - 1], 1
        mov    word ptr [ebp - 4], 0
        mov    ebx, 0aa00000ah
loc_1_030C:
        lea    edi, [ebp - 74h]
        xor    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    al, byte ptr [ebp - 2]
        mov    dword ptr [ebp - 5eh], 1
        dec    al
        mov    ecx, dword ptr [ebp - 4]
        cmp    al, 1
        mov    dword ptr [ebp - 56h], 20h
        sbb    eax, eax
        lea    edx, [ebp - 74h]
        and    eax, 2
        and    ecx, 0ffffh
        inc    eax
        push   edx
        mov    edx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 6ah], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 5ah], ecx
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        db  0A1h, 000h, 000h, 000h, 000h
        push   edx
        push   eax
        lea    eax, [edx + 4ch]
        push   eax
        call   sub_1_D712
        mov    dword ptr [ebp - 0ch], eax
        test   eax, eax
        je     SHORT loc_1_0391
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_D870
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   47ah
        push   eax
        call   sub_1_D7EC
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0391:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_03DC
        push   1
        mov    eax, dword ptr [ebp - 8]
        push   ebx
        mov    ecx, dword ptr [eax + 4ch]
        push   ecx
        call   sub_1_D8B8
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        call   sub_1_D870
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + 4ch]
        push   edx
        call   sub_1_D718
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   47ah
        push   ecx
        call   sub_1_D7EC
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_03DC:
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 4], ax
        mov    al, byte ptr [ebp - 2]
        mov    edx, dword ptr [ebp - 8]
        test   esi, esi
        mov    byte ptr [edx], al
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 50h], 1
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 40h], 6
        mov    eax, dword ptr [ebp - 8]
        je     SHORT loc_1_0416
        mov    word ptr [eax + 204h], si
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 36h], si
        jmp    SHORT loc_1_0429
loc_1_0416:
        mov    ecx, 3e80h
        mov    word ptr [eax + 36h], cx
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax + 204h], cx
loc_1_0429:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_62E9
        cmp    ax, 0ffffh
        jne    SHORT loc_1_0481
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   0aa000004h
        mov    ecx, dword ptr [eax + 4ch]
        push   ecx
        call   sub_1_D8B8
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        call   sub_1_D870
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx + 4ch]
        push   edx
        call   sub_1_D718
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   47ah
        push   ecx
        call   sub_1_D7EC
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0481:
        cmp    ax, 2
        jne    SHORT loc_1_04DA
        push   0
        lea    eax, [ebp - 34h]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_D86A
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_04B1
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 4]
        mov    word ptr [ecx + 0ah], dx
loc_1_04B1:
        push   1
        lea    eax, [ebp - 2ch]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        lea    edx, [ebp - 14h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_D86A
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_04DA
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 18h], edx
loc_1_04DA:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_D870
        lea    eax, [ebp - 24h]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   10h
        push   2
        push   ecx
        call   sub_1_0530
        test   eax, eax
        je     SHORT loc_1_0522
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4ch]
        push   ecx
        call   sub_1_D718
        push   0
        mov    ecx, dword ptr [ebp - 8]
        push   47ah
        push   ecx
        call   sub_1_D7EC
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0522:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
sub_1_0530:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 276h], eax
        cmp    edi, 2
        jbe    SHORT loc_1_0574
        push   0
        mov    eax, dword ptr [esi + 4ch]
        push   0aa000002h
        push   eax
        call   sub_1_D8B8
        mov    dword ptr [ebp - 4], 0c000009ah
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_0574:
        xor    ebx, ebx
        db  0B8h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 26dh], ebx
        mov    dword ptr [esi + 327h], eax
        mov    dword ptr [esi + 31bh], ebx
        mov    dword ptr [esi + 31fh], eax
        db  0C7h, 086h, 065h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 323h], esi
        db  08Bh, 00Dh, 042h, 000h, 000h, 000h
        mov    dword ptr [esi + 269h], ecx
        db  089h, 035h, 042h, 000h, 000h, 000h
        movsx  eax, byte ptr [esi + 0ah]
        cmp    byte ptr [esi], 1
        sbb    ecx, ecx
        add    ecx, 2
        push   ecx
        push   ebx
        push   eax
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 4ch]
        push   esi
        lea    edx, [esi + 20ch]
        db  068h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        push   eax
        push   edx
        push   ecx
        call   sub_1_D7CE
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_1_0607
        push   ebx
        mov    eax, dword ptr [esi + 4ch]
        push   0aa000005h
        push   eax
        call   sub_1_D8B8
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_0607:
        xor    eax, eax
        mov    ecx, dword ptr [esi + 18h]
        mov    ax, word ptr [esi + 20h]
        mov    edx, dword ptr [esi + 4ch]
        shl    eax, 0ah
        push   eax
        push   ecx
        push   edx
        lea    eax, [esi + 1ch]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_D8B2
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_0644
        push   0
        mov    eax, dword ptr [esi + 4ch]
        push   0aa000001h
        push   eax
        call   sub_1_D8B8
        add    esp, 0ch
        jmp    NEAR PTR loc_1_0827
loc_1_0644:
        xor    edx, edx
        mov    eax, dword ptr [ebp + 14h]
loc_1_0649:
        mov    cl, byte ptr [edx + eax]
        inc    edx
        cmp    edx, 6
        mov    byte ptr [edx + esi + 23h], cl
        jb     SHORT loc_1_0649
        mov    word ptr [esi + 38h], 100h
        or     byte ptr [esi + 200h], 40h
        mov    word ptr [esi + 3ah], di
        lea    eax, [esi + 2cbh]
        lea    ecx, [esi + 2d3h]
        lea    edx, [esi + 2cfh]
        mov    dword ptr [esi + 1c8h], eax
        mov    dword ptr [esi + 1d0h], ecx
        lea    eax, [esi + 2c7h]
        lea    ecx, [esi + 2d7h]
        mov    dword ptr [esi + 1cch], edx
        mov    dword ptr [esi + 1d4h], eax
        lea    edx, [esi + 2dbh]
        lea    eax, [esi + 2dfh]
        mov    dword ptr [esi + 1d8h], ecx
        mov    dword ptr [esi + 1a0h], edx
        lea    ecx, [esi + 2e3h]
        lea    edx, [esi + 2e7h]
        mov    dword ptr [esi + 1a4h], eax
        mov    dword ptr [esi + 1a8h], ecx
        lea    eax, [esi + 2ebh]
        lea    ecx, [esi + 2efh]
        mov    dword ptr [esi + 1ach], edx
        mov    dword ptr [esi + 1b0h], eax
        lea    edx, [esi + 2f3h]
        lea    eax, [esi + 2f7h]
        mov    dword ptr [esi + 1b4h], ecx
        mov    dword ptr [esi + 1b8h], edx
        lea    ecx, [esi + 2fbh]
        lea    edx, [esi + 2ffh]
        mov    dword ptr [esi + 1bch], eax
        mov    dword ptr [esi + 1c0h], ecx
        lea    eax, [esi + 2b7h]
        lea    ecx, [esi + 2abh]
        mov    dword ptr [esi + 1c4h], edx
        mov    dword ptr [esi + 1dch], eax
        lea    edx, [esi + 2afh]
        lea    eax, [esi + 2b3h]
        mov    dword ptr [esi + 1e0h], ecx
        mov    dword ptr [esi + 1e4h], edx
        lea    ecx, [esi + 2bbh]
        lea    edx, [esi + 2bfh]
        lea    ebx, [esi + 25dh]
        mov    dword ptr [esi + 1e8h], eax
        lea    eax, [esi + 2c3h]
        mov    dword ptr [esi + 1ech], ecx
        mov    dword ptr [esi + 1f0h], edx
        mov    dword ptr [esi + 1f8h], eax
        push   esi
        mov    dword ptr [ebx], 0
        db  0C7h, 086h, 0ABh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        mov    word ptr [esi + 3eh], 0
        call   sub_1_5FE8
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_1_07A1
        cmp    eax, 13h
        jne    NEAR PTR loc_1_0827
loc_1_07A1:
        push   esi
        call   sub_1_2B50
        test   ax, ax
        je     SHORT loc_1_07C8
        push   0
        mov    eax, dword ptr [esi + 4ch]
        push   0aa000003h
        push   eax
        call   sub_1_D8B8
        mov    dword ptr [ebp - 4], 0c0010006h
        add    esp, 0ch
        jmp    SHORT loc_1_0827
loc_1_07C8:
        xor    edi, edi
loc_1_07CA:
        cmp    word ptr [esi + 40h], 2
        je     SHORT loc_1_07E4
        push   3e8h
        inc    edi
        call   sub_1_D7E0
        cmp    edi, 3e8h
        jb     SHORT loc_1_07CA
loc_1_07E4:
        push   ebx
        lea    eax, [esi + 261h]
        push   eax
        lea    ecx, [esi + 24h]
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0809:
        test   al, al
        jne    SHORT loc_1_088C
        push   0
        mov    eax, dword ptr [esi + 4ch]
        push   0aa000002h
        push   eax
        call   sub_1_D8B8
        mov    dword ptr [ebp - 4], 0c0000001h
        add    esp, 0ch
loc_1_0827:
        lea    eax, [esi + 20ch]
        push   eax
        call   sub_1_D7D4
        db  03Bh, 035h, 042h, 000h, 000h, 000h
        jne    SHORT loc_1_0852
        mov    eax, dword ptr [esi + 269h]
        pop    edi
        pop    esi
        db  0A3h, 042h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_0852:
        db  08Bh, 00Dh, 042h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 269h], esi
        je     SHORT loc_1_086E
loc_1_0860:
        mov    ecx, dword ptr [ecx + 269h]
        cmp    dword ptr [ecx + 269h], esi
        jne    SHORT loc_1_0860
loc_1_086E:
        mov    eax, dword ptr [ecx + 269h]
        pop    edi
        pop    esi
        pop    ebx
        mov    edx, dword ptr [eax + 269h]
        mov    dword ptr [ecx + 269h], edx
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_088C:
        db  0B8h, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        mov    dword ptr [esi + 45ah], eax
        mov    dword ptr [esi + 45eh], ecx
        mov    dword ptr [esi + 462h], eax
        mov    dword ptr [esi + 466h], esi
        db  0B8h, 000h, 000h, 000h, 000h
        pop    edi
        mov    dword ptr [esi + 46ah], eax
        mov    dword ptr [esi + 46eh], ecx
        mov    dword ptr [esi + 472h], eax
        mov    dword ptr [esi + 476h], esi
        xor    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_08E0:
        push   ebp
        mov    ecx, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    edx, dword ptr [ebp + 18h]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [esi], 0
        mov    dword ptr [ecx], 0ffffffffh
        lea    eax, [eax + edx*4]
loc_1_0904:
        test   edx, edx
        je     SHORT loc_1_0913
        sub    eax, 4
        dec    edx
        cmp    dword ptr [eax], 1
        jne    SHORT loc_1_0904
        mov    dword ptr [ecx], edx
loc_1_0913:
        cmp    dword ptr [ecx], -1
        jne    SHORT loc_1_0925
        mov    eax, 0c0010019h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_0925:
        db  0A1h, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4]
        push   eax
        push   0
        push   75h
        push   ecx
        call   sub_1_D7E6
        test   eax, eax
        je     SHORT loc_1_0949
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_0949:
        mov    edi, dword ptr [ebp + 20h]
        inc    dword ptr [edi + 272h]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], edi
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [edi + 265h]
        mov    dword ptr [eax + 10h], edx
        mov    edx, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 28h]
        mov    dword ptr [ecx], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 20h], 0
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, 0f2h
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], eax
        mov    dword ptr [edi + 317h], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edx], ecx
        cmp    word ptr [edi + 40h], 1
        je     NEAR PTR loc_1_0A22
        mov    byte ptr [edi + 271h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi + 456h], eax
        cmp    word ptr [edi + 40h], 2
        je     SHORT loc_1_0A22
        push   edi
        call   sub_1_2B50
        and    eax, 0ffffh
        test   eax, eax
        je     SHORT loc_1_09FD
        mov    byte ptr [edi + 303h], 1
        mov    esi, 0fa0h
loc_1_09D2:
        push   3e8h
        call   sub_1_D7E0
        dec    esi
        jne    SHORT loc_1_09D2
        push   7530h
        add    edi, 45eh
        push   edi
        call   sub_1_D7C2
        mov    eax, 103h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_09FD:
        push   7530h
        add    edi, 45eh
        push   edi
        mov    byte ptr [edi - 15bh], 0
        call   sub_1_D7C2
        mov    eax, 103h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_0A22:
        mov    eax, dword ptr [edi + 26dh]
        test   eax, eax
        jne    SHORT loc_1_0A81
        push   edi
        call   sub_1_2F78
        test   ax, ax
        je     SHORT loc_1_0A5C
        push   7530h
        add    edi, 45eh
        push   edi
        mov    byte ptr [edi - 15bh], 1
        call   sub_1_D7C2
        mov    eax, 103h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_0A5C:
        push   7530h
        add    edi, 45eh
        push   edi
        mov    byte ptr [edi - 15bh], 0
        call   sub_1_D7C2
        mov    eax, 103h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_0A81:
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0ch], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [edi + 26dh], ecx
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [edi + 25dh]
        add    eax, 14h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax]
        push   ecx
        push   eax
        push   edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0AAA:
        test   al, al
        jne    SHORT loc_1_0AD3
        dec    dword ptr [edi + 272h]
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0ch]
        mov    eax, 0c0000001h
        mov    dword ptr [edi + 26dh], ecx
        pop    edi
        mov    dword ptr [esi], 0c0010010h
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
loc_1_0AD3:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 1ch], 1
        mov    eax, dword ptr [edi + 272h]
        dec    eax
        mov    dword ptr [edi + 272h], eax
        jne    SHORT loc_1_0B25
        cmp    byte ptr [edi + 304h], 0
        je     SHORT loc_1_0B00
        lea    eax, [edi + 43dh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_0B14
loc_1_0B00:
        cmp    dword ptr [edi + 28ah], 0
        je     SHORT loc_1_0B25
        lea    eax, [edi + 43dh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_0B25
loc_1_0B14:
        push   0
        add    edi, 31bh
        push   edi
        mov    byte ptr [eax], 1
        call   sub_1_D7C2
loc_1_0B25:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    24h
        db  090h
sub_1_0B30:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 456h]
        mov    eax, dword ptr [edi + 26dh]
        lea    ecx, [esi + 14h]
        mov    dword ptr [esi + 0ch], eax
        push   ecx
        mov    eax, dword ptr [edi + 25dh]
        mov    dword ptr [edi + 26dh], esi
        mov    edx, dword ptr [esi]
        push   edx
        push   esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0B61:
        test   al, al
        jne    SHORT loc_1_0B9A
        push   0
        dec    dword ptr [edi + 272h]
        mov    eax, dword ptr [esi + 0ch]
        push   0c0010007h
        mov    byte ptr [edi + 271h], 0
        mov    dword ptr [edi + 26dh], eax
        mov    ecx, dword ptr [esi]
        push   ecx
        call   sub_1_D71E
        push   0
        push   75h
        push   esi
        call   sub_1_D7EC
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_0B9A:
        mov    byte ptr [edi + 271h], 0
        mov    dword ptr [esi + 1ch], 1
        push   0
        dec    dword ptr [edi + 272h]
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_D71E
        pop    edi
        pop    esi
        pop    ebp
        ret    4
sub_1_0BC0:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 26dh]
        test   edx, edx
        je     SHORT loc_1_0BE3
loc_1_0BD3:
        mov    esi, dword ptr [edx + 18h]
        cmp    ecx, esi
        jae    SHORT loc_1_0BDC
        mov    ecx, esi
loc_1_0BDC:
        mov    edx, dword ptr [edx + 0ch]
        test   edx, edx
        jne    SHORT loc_1_0BD3
loc_1_0BE3:
        test   ecx, ecx
        jne    SHORT loc_1_0BEC
        mov    ecx, 0f2h
loc_1_0BEC:
        pop    esi
        mov    dword ptr [eax + 317h], ecx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0C00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 20h], 0
        mov    esi, dword ptr [ebx + 8]
        je     SHORT loc_1_0C1E
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_0C1E:
        inc    dword ptr [esi + 272h]
        inc    dword ptr [ebx + 1ch]
        mov    eax, dword ptr [esi + 26dh]
        cmp    ebx, eax
        jne    SHORT loc_1_0C3C
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 26dh], eax
        jmp    SHORT loc_1_0C4F
loc_1_0C3C:
        cmp    dword ptr [eax + 0ch], ebx
        je     SHORT loc_1_0C49
loc_1_0C41:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], ebx
        jne    SHORT loc_1_0C41
loc_1_0C49:
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_0C4F:
        push   0
        mov    dword ptr [esi + 452h], ebx
        mov    eax, dword ptr [ebx + 14h]
        mov    ecx, dword ptr [esi + 25dh]
        push   eax
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0C67:
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_0C8C
        mov    eax, dword ptr [ebx + 1ch]
        cmp    eax, 2
        je     SHORT loc_1_0C84
        mov    byte ptr [ebx + 20h], 1
        sub    dword ptr [ebx + 1ch], 2
        mov    edi, 103h
        jmp    SHORT loc_1_0CB2
loc_1_0C84:
        sub    eax, 2
        mov    dword ptr [ebx + 1ch], eax
        jmp    SHORT loc_1_0CB2
loc_1_0C8C:
        cmp    edi, 103h
        jne    SHORT loc_1_0C9E
        mov    byte ptr [ebx + 20h], 1
        sub    dword ptr [ebx + 1ch], 2
        jmp    SHORT loc_1_0CB2
loc_1_0C9E:
        cmp    edi, 0c001000eh
        jne    SHORT loc_1_0CAF
        mov    byte ptr [ebx + 20h], 1
        mov    edi, 103h
loc_1_0CAF:
        dec    dword ptr [ebx + 1ch]
loc_1_0CB2:
        cmp    dword ptr [ebx + 1ch], 0
        jne    SHORT loc_1_0CE4
        mov    eax, dword ptr [esi + 317h]
        cmp    dword ptr [ebx + 18h], eax
        jne    SHORT loc_1_0CC9
        push   esi
        call   sub_1_0BC0
loc_1_0CC9:
        cmp    dword ptr [esi + 26dh], 0
        jne    SHORT loc_1_0CE9
        push   esi
        lea    eax, [esi + 20ch]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_D7DA
loc_1_0CE4:
        mov    edi, 103h
loc_1_0CE9:
        cmp    byte ptr [esi + 451h], 0
        jne    SHORT loc_1_0D0A
        push   7530h
        lea    eax, [esi + 45eh]
        mov    byte ptr [esi + 451h], 1
        push   eax
        call   sub_1_D7C2
loc_1_0D0A:
        dec    dword ptr [esi + 272h]
        jne    SHORT loc_1_0D4B
        cmp    byte ptr [esi + 304h], 0
        je     SHORT loc_1_0D26
        lea    eax, [esi + 43dh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_0D3A
loc_1_0D26:
        cmp    dword ptr [esi + 28ah], 0
        je     SHORT loc_1_0D4B
        lea    eax, [esi + 43dh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_0D4B
loc_1_0D3A:
        push   0
        add    esi, 31bh
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_D7C2
loc_1_0D4B:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [edi + 1ch]
        inc    dword ptr [esi + 272h]
        cmp    byte ptr [edi + 20h], 0
        je     SHORT loc_1_0D87
        mov    dword ptr [ebp - 4], 0c0010002h
        jmp    SHORT loc_1_0DC9
loc_1_0D87:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 10h]
        test   ecx, ecx
        jne    SHORT loc_1_0D9E
        push   eax
        push   edi
        push   esi
        call   sub_1_11D0
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_0DC9
loc_1_0D9E:
        cmp    ecx, 1
        jne    SHORT loc_1_0DC2
        cmp    byte ptr [esi + 303h], 0
        je     SHORT loc_1_0DB5
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    SHORT loc_1_0DC9
loc_1_0DB5:
        push   eax
        push   edi
        push   esi
        call   sub_1_1220
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_0DC9
loc_1_0DC2:
        mov    dword ptr [ebp - 4], 10001h
loc_1_0DC9:
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_0E2B
        mov    eax, dword ptr [ebx + 26dh]
        cmp    edi, eax
        jne    SHORT loc_1_0DE6
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 26dh], eax
        jmp    SHORT loc_1_0DFF
loc_1_0DE6:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_0DF3
loc_1_0DEB:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_0DEB
loc_1_0DF3:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [ebx + 452h], edi
loc_1_0DFF:
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [ebx + 317h], eax
        jne    SHORT loc_1_0E10
        push   ebx
        call   sub_1_0BC0
loc_1_0E10:
        cmp    dword ptr [ebx + 26dh], 0
        jne    SHORT loc_1_0E2B
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 20ch
        push   ebx
        call   sub_1_D7DA
loc_1_0E2B:
        mov    eax, dword ptr [esi + 272h]
        dec    eax
        mov    dword ptr [esi + 272h], eax
        jne    SHORT loc_1_0E73
        cmp    byte ptr [esi + 304h], 0
        je     SHORT loc_1_0E4E
        lea    eax, [esi + 43dh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_0E62
loc_1_0E4E:
        cmp    dword ptr [esi + 28ah], 0
        je     SHORT loc_1_0E73
        lea    eax, [esi + 43dh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_0E73
loc_1_0E62:
        push   0
        add    esi, 31bh
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_D7C2
loc_1_0E73:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_1_0E80:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        lea    ecx, [ebp - 8]
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        push   edi
        xor    esi, esi
        mov    edi, 4
        mov    dword ptr [ebp - 0ch], esi
        mov    dword ptr [ebp - 10h], 1
        cmp    eax, 2010101h
        ja     SHORT loc_1_0EBE
        je     NEAR PTR loc_1_107F
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_0ECF
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_0EBE:
        sub    eax, 2010102h
        cmp    eax, 3
        ja     SHORT loc_1_0ECF
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_0ECF:
        mov    esi, 0c00000bbh
        jmp    NEAR PTR loc_1_112E
loc_1_0ED9:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_0EEE
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edi, 60h
        jmp    NEAR PTR loc_1_112E
loc_1_0EEE:
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edi, 7ch
        jmp    NEAR PTR loc_1_112E
loc_1_0EFD:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], 4
        cmp    byte ptr [eax + 303h], 0
        jne    SHORT loc_1_0F17
        mov    dword ptr [ebp - 0ch], 0
loc_1_0F17:
        lea    ecx, [ebp - 0ch]
        mov    edi, 4
        jmp    NEAR PTR loc_1_112E
loc_1_0F24:
        lea    ecx, [ebp - 10h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_112E
loc_1_0F31:
        mov    dword ptr [ebp - 8], 0f2h
        jmp    NEAR PTR loc_1_112E
loc_1_0F3D:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 36h]
        and    eax, 0ffffh
        sub    eax, 0eh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_112E
loc_1_0F54:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 44h]
        test   al, 6
        jne    SHORT loc_1_0F6B
        mov    dword ptr [ebp - 8], 9c40h
        jmp    NEAR PTR loc_1_112E
loc_1_0F6B:
        mov    dword ptr [ebp - 8], 27100h
        jmp    NEAR PTR loc_1_112E
loc_1_0F77:
        mov    eax, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [eax + 204h]
        xor    ebx, ebx
        mov    bx, word ptr [eax + 3ah]
        imul   edx, ebx
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_112E
loc_1_0F94:
        mov    eax, dword ptr [ebp + 8]
        xor    ebx, ebx
        mov    bx, word ptr [eax + 16h]
        xor    edx, edx
        shl    ebx, 0ah
        mov    dx, word ptr [eax + 204h]
        mov    ax, word ptr [eax + 3ah]
        and    eax, 0ffffh
        imul   edx, eax
        sub    ebx, edx
        mov    dword ptr [ebp - 8], ebx
        jmp    NEAR PTR loc_1_112E
loc_1_0FBF:
        mov    eax, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [eax + 36h]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_112E
loc_1_0FD0:
        mov    eax, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [eax + 36h]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_112E
loc_1_0FE1:
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edi, 0dh
        jmp    NEAR PTR loc_1_112E
loc_1_0FF0:
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edi, 0eh
        jmp    NEAR PTR loc_1_112E
loc_1_0FFF:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_1019
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 25dh]
        mov    ebx, dword ptr [edx + 10h]
        mov    dword ptr [ebp - 8], ebx
        jmp    NEAR PTR loc_1_112E
loc_1_1019:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 14h]
        mov    ebx, dword ptr [edx + 8]
        mov    dword ptr [ebp - 8], ebx
        jmp    NEAR PTR loc_1_112E
loc_1_102A:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_1041
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 317h]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_112E
loc_1_1041:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_112E
loc_1_104F:
        lea    ecx, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 300h
        jmp    NEAR PTR loc_1_112E
loc_1_1062:
        mov    eax, dword ptr [ebp + 8]
        xor    edx, edx
        mov    dx, word ptr [eax + 36h]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_112E
loc_1_1073:
        mov    dword ptr [ebp - 8], 0eh
        jmp    NEAR PTR loc_1_112E
loc_1_107F:
        push   6
        mov    eax, dword ptr [ebp + 8]
        add    eax, 2ah
        lea    ecx, [ebp - 18h]
        push   eax
        mov    edi, 6
        push   ecx
        call   sub_1_D8CC
        lea    ecx, [ebp - 18h]
        jmp    NEAR PTR loc_1_112E
loc_1_109E:
        push   6
        mov    eax, dword ptr [ebp + 8]
        add    eax, 24h
        lea    ecx, [ebp - 18h]
        push   eax
        mov    edi, 6
        push   ecx
        call   sub_1_D8CC
        lea    ecx, [ebp - 18h]
        jmp    SHORT loc_1_112E
loc_1_10BA:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_10CE
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 14h]
        mov    ebx, dword ptr [edx + 0ch]
        mov    dword ptr [ebp - 8], ebx
        jmp    SHORT loc_1_10DA
loc_1_10CE:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 9fh]
        mov    dword ptr [ebp - 8], edx
loc_1_10DA:
        mov    edx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        shl    eax, 10h
        and    edx, 0ff00h
        or     edx, eax
        mov    ebx, dword ptr [ebp - 8]
        shl    edx, 8
        mov    eax, dword ptr [ebp - 8]
        shr    ebx, 10h
        and    eax, 0ff0000h
        or     eax, ebx
        shr    eax, 8
        or     edx, eax
        mov    dword ptr [ebp - 8], edx
        jmp    SHORT loc_1_112E
loc_1_1108:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 99h]
        mov    dword ptr [ebp - 8], edx
        jmp    SHORT loc_1_112E
loc_1_1116:
        mov    dword ptr [ebp - 8], 1
        mov    eax, dword ptr [ebp + 8]
        cmp    word ptr [eax + 40h], 1
        jne    SHORT loc_1_112E
        mov    dword ptr [ebp - 8], 0
loc_1_112E:
        test   esi, esi
        jne    SHORT loc_1_1160
        mov    eax, dword ptr [ebp + 1ch]
        cmp    eax, edi
        jae    SHORT loc_1_1150
        sub    edi, eax
        mov    esi, 0c0010014h
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [eax], edi
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_1_1150:
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   ecx
        push   eax
        call   sub_1_D8CC
        mov    eax, dword ptr [ebp + 24h]
        add    dword ptr [eax], edi
loc_1_1160:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  090h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh, 064h, 024h
        db  000h, 08Dh, 064h, 024h, 000h
sub_1_11D0:
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
        call   sub_1_0E80
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1220:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        xor    edi, edi
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 4
        mov    eax, dword ptr [esi + 14h]
        je     SHORT loc_1_1254
        mov    eax, 0c0010014h
        mov    dword ptr [esi + 20h], edi
        mov    dword ptr [esi + 24h], edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1254:
        cmp    eax, 2010103h
        ja     SHORT loc_1_127C
        je     NEAR PTR loc_1_1369
        cmp    eax, 1010eh
        je     SHORT loc_1_1299
        cmp    eax, 1010fh
        je     SHORT loc_1_12DA
        cmp    eax, 10112h
        je     NEAR PTR loc_1_1341
        jmp    SHORT loc_1_1287
loc_1_127C:
        cmp    eax, 2010104h
        je     NEAR PTR loc_1_137A
loc_1_1287:
        mov    edi, 0c00000bbh
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_138B
loc_1_1299:
        push   4
        lea    eax, [ebp - 0ch]
        push   ecx
        push   eax
        call   sub_1_D8CC
        test   byte ptr [ebp - 0ch], 46h
        je     SHORT loc_1_12C3
        mov    dword ptr [esi + 20h], 4
        mov    dword ptr [esi + 24h], 0
        mov    edi, 0c00000bbh
        jmp    NEAR PTR loc_1_138B
loc_1_12C3:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    ebx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   ebx
        call   sub_1_13B0
        jmp    NEAR PTR loc_1_1389
loc_1_12DA:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_D8CC
        cmp    dword ptr [ebp - 4], 0f2h
        ja     SHORT loc_1_133A
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebx + 317h]
        cmp    dword ptr [ebp - 4], edx
        jbe    SHORT loc_1_130B
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ebx + 317h], eax
        jmp    SHORT loc_1_132B
loc_1_130B:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ecx + 18h]
        cmp    edx, eax
        jne    SHORT loc_1_1328
        cmp    dword ptr [ebp - 4], eax
        jae    SHORT loc_1_1328
        mov    eax, dword ptr [ebp - 4]
        push   ebx
        mov    dword ptr [ecx + 18h], eax
        call   sub_1_0BC0
        jmp    SHORT loc_1_132E
loc_1_1328:
        mov    eax, dword ptr [ebp - 4]
loc_1_132B:
        mov    dword ptr [ecx + 18h], eax
loc_1_132E:
        mov    eax, dword ptr [ebx + 317h]
        mov    word ptr [ebx + 3ch], ax
        jmp    SHORT loc_1_138B
loc_1_133A:
        mov    edi, 0c0010014h
        jmp    SHORT loc_1_138B
loc_1_1341:
        cmp    dword ptr [ebp - 8], 4
        je     SHORT loc_1_1356
        mov    edi, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_1_138B
loc_1_1356:
        push   4
        xor    edi, edi
        push   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 71h
        push   ecx
        call   sub_1_D8CC
        jmp    SHORT loc_1_138B
loc_1_1369:
        push   ecx
        mov    ebx, dword ptr [ebp + 8]
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        push   ebx
        call   sub_1_29C0
        jmp    SHORT loc_1_1389
loc_1_137A:
        push   ecx
        mov    ebx, dword ptr [ebp + 8]
        push   esi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        push   ebx
        call   sub_1_2990
loc_1_1389:
        mov    edi, eax
loc_1_138B:
        test   edi, edi
        jne    SHORT loc_1_139C
        mov    dword ptr [esi + 24h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], eax
loc_1_139C:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_13B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_1_13E5
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
        mov    edx, dword ptr [esi + 14h]
        push   edx
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [esi + 1ch]
        mov    ecx, dword ptr [eax + 25dh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_13DD:
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    10h
loc_1_13E5:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
        db  090h
sub_1_13F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 0ch], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        lea    edx, [ebp - 8]
        mov    eax, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        push   edx
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 10h], ecx
        lea    eax, [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 14h]
        push   eax
        mov    edx, dword ptr [ebp - 10h]
        push   ecx
        mov    eax, dword ptr [esi + 14h]
        push   edx
        mov    ebx, dword ptr [ebp + 8]
        push   1
        push   eax
        push   0
        push   ebx
        call   sub_1_0E80
        cmp    eax, 0c00000bbh
        mov    edi, eax
        jne    NEAR PTR loc_1_14E9
        xor    edi, edi
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 2010105h
        ja     SHORT loc_1_1463
        je     SHORT loc_1_14BB
        sub    eax, 20101h
        cmp    eax, 4
        ja     SHORT loc_1_1463
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_1463:
        mov    edi, 0c00000bbh
        jmp    SHORT loc_1_14D0
loc_1_146A:
        mov    eax, dword ptr [ebx + 2a3h]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_14D0
loc_1_1475:
        mov    eax, dword ptr [ebx + 2a7h]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_14D0
loc_1_1480:
        mov    eax, dword ptr [ebx + 2abh]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_14D0
loc_1_148B:
        cmp    byte ptr [ebx + 50h], 0
        je     SHORT loc_1_1498
        mov    edi, 0c00000bbh
        jmp    SHORT loc_1_14D0
loc_1_1498:
        mov    eax, dword ptr [ebx + 2cbh]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_14D0
loc_1_14A3:
        cmp    byte ptr [ebx + 50h], 0
        je     SHORT loc_1_14B0
        mov    edi, 0c00000bbh
        jmp    SHORT loc_1_14D0
loc_1_14B0:
        mov    eax, dword ptr [ebx + 2cfh]
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_14D0
loc_1_14BB:
        mov    dword ptr [ebp - 4], 1
        cmp    word ptr [ebx + 40h], 1
        jne    SHORT loc_1_14D0
        mov    dword ptr [ebp - 4], 0
loc_1_14D0:
        cmp    dword ptr [ebp - 14h], 4
        jb     SHORT loc_1_14E9
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        call   sub_1_D8CC
        add    dword ptr [ebp - 8], 4
loc_1_14E9:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], ecx
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1520:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        mov    eax, dword ptr [ecx + 14h]
        cmp    eax, 10113h
        ja     SHORT loc_1_1547
        je     SHORT loc_1_1568
        cmp    eax, 10101h
        jb     SHORT loc_1_1563
        cmp    eax, 10111h
        jbe    SHORT loc_1_1568
        jmp    SHORT loc_1_1563
loc_1_1547:
        cmp    eax, 2010105h
        ja     SHORT loc_1_1563
        cmp    eax, 2010101h
        jae    SHORT loc_1_1568
        cmp    eax, 20101h
        jb     SHORT loc_1_1563
        cmp    eax, 20104h
        jbe    SHORT loc_1_1568
loc_1_1563:
        mov    esi, 0c00000bbh
loc_1_1568:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 272h]
        test   esi, esi
        jne    SHORT loc_1_157E
        push   ecx
        push   edi
        call   sub_1_13F0
        mov    esi, eax
loc_1_157E:
        mov    eax, dword ptr [edi + 272h]
        dec    eax
        mov    dword ptr [edi + 272h], eax
        jne    SHORT loc_1_15C6
        cmp    byte ptr [edi + 304h], 0
        je     SHORT loc_1_15A1
        lea    eax, [edi + 43dh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_15B5
loc_1_15A1:
        cmp    dword ptr [edi + 28ah], 0
        je     SHORT loc_1_15C6
        lea    eax, [edi + 43dh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_15C6
loc_1_15B5:
        push   0
        add    edi, 31bh
        push   edi
        mov    byte ptr [eax], 1
        call   sub_1_D7C2
loc_1_15C6:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_15D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_64BD
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 45eh]
        push   eax
        push   ecx
        call   sub_1_D7C8
        lea    ecx, [ebp - 1]
        lea    edx, [esi + 46eh]
        push   ecx
        push   edx
        call   sub_1_D7C8
        push   9c4h
        call   sub_1_D7E0
        mov    byte ptr [esi + 43fh], 1
        db  03Bh, 035h, 042h, 000h, 000h, 000h
        jne    SHORT loc_1_1626
        mov    eax, dword ptr [esi + 269h]
        db  0A3h, 042h, 000h, 000h, 000h
        jmp    SHORT loc_1_1654
loc_1_1626:
        db  08Bh, 00Dh, 042h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 269h], esi
        je     SHORT loc_1_1642
loc_1_1634:
        mov    ecx, dword ptr [ecx + 269h]
        cmp    dword ptr [ecx + 269h], esi
        jne    SHORT loc_1_1634
loc_1_1642:
        mov    eax, dword ptr [ecx + 269h]
        mov    edx, dword ptr [eax + 269h]
        mov    dword ptr [ecx + 269h], edx
loc_1_1654:
        lea    eax, [esi + 20ch]
        push   eax
        call   sub_1_D7D4
        mov    eax, dword ptr [esi + 25dh]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_166C:
        mov    eax, dword ptr [esi + 4ch]
        push   eax
        call   sub_1_D718
        push   0
        push   47ah
        push   esi
        call   sub_1_D7EC
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1690:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_D70C
        push   0
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        call   sub_1_D700
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_16C0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0ch
        mov    dword ptr [ebp - 4], 103h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        mov    edi, dword ptr [ebp + 0ch]
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 0ch]
        lea    ebx, [edi + 24h]
        push   ecx
        push   0
        push   0
        push   edi
        call   sub_1_D810
        cmp    dword ptr [ebp - 8], 0ch
        jb     NEAR PTR loc_1_18B9
        xor    eax, eax
        mov    ax, word ptr [esi + 36h]
        cmp    eax, dword ptr [ebp - 8]
        jb     NEAR PTR loc_1_18B9
        cmp    byte ptr [esi + 305h], 0
        je     SHORT loc_1_171F
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_171F:
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 20h], 0
        je     SHORT loc_1_1736
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1736:
        mov    eax, dword ptr [ebp + 8]
        push   edi
        push   eax
        push   esi
        inc    dword ptr [eax + 1ch]
        inc    dword ptr [esi + 272h]
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx], 0
        call   sub_1_1910
        cmp    byte ptr [ebx + 9], 0
        je     SHORT loc_1_1787
        cmp    dword ptr [esi + 28ah], 0
        jne    SHORT loc_1_1773
        mov    dword ptr [esi + 28eh], edi
        mov    dword ptr [esi + 28ah], edi
        jmp    NEAR PTR loc_1_1847
loc_1_1773:
        mov    eax, dword ptr [esi + 28eh]
        mov    dword ptr [eax + 24h], edi
        mov    dword ptr [esi + 28eh], edi
        jmp    NEAR PTR loc_1_1847
loc_1_1787:
        cmp    dword ptr [esi + 27ah], 0
        je     SHORT loc_1_17A4
        mov    eax, dword ptr [esi + 27eh]
        mov    dword ptr [eax + 24h], edi
        mov    dword ptr [esi + 27eh], edi
        jmp    NEAR PTR loc_1_1841
loc_1_17A4:
        cmp    dword ptr [esi + 282h], 0
        jne    SHORT loc_1_17BD
        xor    ebx, ebx
        mov    dword ptr [esi + 286h], edi
        mov    dword ptr [esi + 282h], edi
        jmp    SHORT loc_1_17CC
loc_1_17BD:
        mov    ebx, dword ptr [esi + 286h]
        mov    dword ptr [ebx + 24h], edi
        mov    dword ptr [esi + 286h], edi
loc_1_17CC:
        push   esi
        push   edi
        call   sub_1_3760
        and    eax, 0ffffh
        mov    dword ptr [ebp - 4], eax
        cmp    eax, 6
        jne    SHORT loc_1_1813
        test   ebx, ebx
        jne    SHORT loc_1_17FA
        mov    dword ptr [esi + 286h], 0
        mov    dword ptr [esi + 282h], 0
        jmp    SHORT loc_1_1807
loc_1_17FA:
        mov    dword ptr [esi + 286h], ebx
        mov    dword ptr [ebx + 24h], 0
loc_1_1807:
        mov    dword ptr [esi + 27ah], edi
        mov    dword ptr [esi + 27eh], edi
loc_1_1813:
        cmp    dword ptr [ebp - 4], 8
        jne    SHORT loc_1_1847
        mov    byte ptr [esi + 303h], 1
        push   0
        push   0
        push   esi
        push   0
        call   sub_1_29F0
        mov    ax, word ptr [esi + 40h]
        xor    ecx, ecx
loc_1_1832:
        cmp    ax, 1
        je     SHORT loc_1_1841
        inc    ecx
        cmp    ecx, 0fffffh
        jb     SHORT loc_1_1832
loc_1_1841:
        cmp    dword ptr [ebp - 4], 8
        je     SHORT loc_1_189F
loc_1_1847:
        mov    edi, 103h
        mov    eax, dword ptr [esi + 272h]
        dec    eax
        mov    dword ptr [esi + 272h], eax
        jne    SHORT loc_1_1894
        cmp    byte ptr [esi + 304h], 0
        je     SHORT loc_1_186F
        lea    eax, [esi + 43dh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_1883
loc_1_186F:
        cmp    dword ptr [esi + 28ah], 0
        je     SHORT loc_1_1894
        lea    eax, [esi + 43dh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_1894
loc_1_1883:
        push   0
        add    esi, 31bh
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_D7C2
loc_1_1894:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_189F:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        dec    dword ptr [esi + 272h]
        pop    esi
        pop    ebx
        dec    dword ptr [eax + 1ch]
        mov    eax, 0c0000001h
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_18B9:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_18D0:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        push   ebx
        push   esi
        xor    esi, esi
        cmp    edx, esi
        je     SHORT loc_1_18F2
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
loc_1_18E5:
        mov    bl, byte ptr [eax + esi]
        cmp    byte ptr [ecx + esi], bl
        jne    SHORT loc_1_18FA
        inc    esi
        cmp    esi, edx
        jb     SHORT loc_1_18E5
loc_1_18F2:
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_18FA:
        mov    eax, 0ffffffffh
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1910:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        lea    esi, [eax + 24h]
        push   6
        lea    ecx, [ebp - 8]
        mov    byte ptr [esi + 9], 0
        mov    byte ptr [esi + 8], 0
        push   2
        push   eax
        push   ecx
        call   sub_1_2120
        mov    edi, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp - 4]
        mov    eax, dword ptr [edi + 25dh]
        cmp    byte ptr [edi + 28h], cl
        mov    ebx, dword ptr [eax + 10h]
        jne    SHORT loc_1_1973
        push   6
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [edi + 24h]
        push   ecx
        call   sub_1_18D0
        test   eax, eax
        jne    SHORT loc_1_1973
        mov    eax, ebx
        and    al, 1
        mov    byte ptr [esi + 9], al
        je     SHORT loc_1_1973
        mov    byte ptr [esi + 8], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1973:
        mov    eax, dword ptr [ebp + 0ch]
        test   byte ptr [eax + 71h], 2
        je     SHORT loc_1_1989
        mov    byte ptr [esi + 8], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1989:
        test   bl, 20h
        je     SHORT loc_1_199B
        mov    byte ptr [esi + 8], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_199B:
        mov    al, byte ptr [ebp - 4]
        db  038h, 005h, 004h, 000h, 000h, 000h
        jne    SHORT loc_1_19C9
        push   6
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_18D0
        test   eax, eax
        jne    SHORT loc_1_19C9
        and    bl, 8
        pop    edi
        mov    byte ptr [esi + 8], bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_19C9:
        test   byte ptr [ebp - 8], 80h
        je     SHORT loc_1_1A4D
        test   bh, 20h
        je     SHORT loc_1_19E1
        mov    byte ptr [esi + 8], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_19E1:
        test   bh, 40h
        je     SHORT loc_1_1A10
        cmp    byte ptr [ebp - 8], 0c0h
        jne    SHORT loc_1_1A10
        cmp    byte ptr [ebp - 7], 0
        jne    SHORT loc_1_1A10
        mov    ax, word ptr [edi + 9fh]
        test   word ptr [ebp - 6], ax
        jne    SHORT loc_1_1A0C
        mov    ax, word ptr [edi + 0a1h]
        test   word ptr [ebp - 4], ax
        je     SHORT loc_1_1A10
loc_1_1A0C:
        mov    byte ptr [esi + 8], 1
loc_1_1A10:
        test   bh, 10h
        je     SHORT loc_1_1A51
        mov    al, byte ptr [ebp - 8]
        and    eax, 0ffff00ffh
        cmp    word ptr [edi + 97h], ax
        jne    SHORT loc_1_1A51
        mov    ax, word ptr [ebp - 6]
        cmp    word ptr [edi + 99h], ax
        jne    SHORT loc_1_1A51
        mov    ax, word ptr [ebp - 4]
        cmp    word ptr [edi + 9bh], ax
        jne    SHORT loc_1_1A51
        mov    byte ptr [esi + 8], 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1A4D:
        mov    byte ptr [esi + 8], 0
loc_1_1A51:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1A60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 20h], 0
        mov    esi, dword ptr [edi + 8]
        je     SHORT loc_1_1A81
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1A81:
        cmp    byte ptr [esi + 304h], 0
        je     SHORT loc_1_1A94
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1A94:
        cmp    byte ptr [esi + 305h], 0
        je     SHORT loc_1_1AA7
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1AA7:
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 46eh]
        push   eax
        push   ecx
        call   sub_1_D7C8
        inc    dword ptr [edi + 1ch]
        inc    dword ptr [esi + 272h]
        mov    byte ptr [esi + 304h], 1
        mov    dword ptr [esi + 30bh], edi
        mov    eax, dword ptr [esi + 272h]
        dec    eax
        mov    dword ptr [esi + 272h], eax
        jne    SHORT loc_1_1B15
        cmp    byte ptr [esi + 304h], 0
        je     SHORT loc_1_1AF0
        lea    eax, [esi + 43dh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_1B04
loc_1_1AF0:
        cmp    dword ptr [esi + 28ah], 0
        je     SHORT loc_1_1B15
        lea    eax, [esi + 43dh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_1B15
loc_1_1B04:
        push   0
        add    esi, 31bh
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_D7C2
loc_1_1B15:
        mov    eax, 103h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1B30:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        xor    edi, edi
        push   esi
        call   sub_1_31A0
        test   ax, ax
        je     SHORT loc_1_1B79
        mov    ebx, 1
loc_1_1B4F:
        test   edi, edi
        jne    SHORT loc_1_1B96
        push   0
        xor    edi, edi
        push   0
        mov    byte ptr [esi + 303h], bl
        push   esi
        push   0
        call   sub_1_29F0
        mov    ax, word ptr [esi + 40h]
loc_1_1B6B:
        cmp    ax, bx
        je     SHORT loc_1_1B82
        inc    edi
        cmp    edi, 0fffffh
        jb     SHORT loc_1_1B6B
loc_1_1B79:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
loc_1_1B82:
        push   esi
        call   sub_1_31A0
        test   ax, ax
        jne    SHORT loc_1_1B4F
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
loc_1_1B96:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1BB0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 1ch]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1BC0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        call   sub_1_2A60
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1BE0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 272h]
loc_1_1BF0:
        mov    byte ptr [esi + 450h], bl
        cmp    byte ptr [esi + 43dh], bl
        jne    SHORT loc_1_1C1E
        mov    byte ptr [esi + 43dh], 1
        push   ebx
        push   ebx
        push   esi
        push   ebx
        call   sub_1_1D50
        cmp    dword ptr [esi + 28ah], ebx
        je     SHORT loc_1_1C1E
        cmp    byte ptr [esi + 43dh], bl
        je     SHORT loc_1_1BF0
loc_1_1C1E:
        mov    eax, dword ptr [esi + 272h]
        dec    eax
        mov    dword ptr [esi + 272h], eax
        jne    SHORT loc_1_1C66
        cmp    byte ptr [esi + 304h], 0
        je     SHORT loc_1_1C3F
        cmp    byte ptr [esi + 43dh], 0
        je     SHORT loc_1_1C51
loc_1_1C3F:
        cmp    dword ptr [esi + 28ah], 0
        je     SHORT loc_1_1C66
        cmp    byte ptr [esi + 43dh], 0
        jne    SHORT loc_1_1C66
loc_1_1C51:
        push   0
        add    esi, 31bh
        push   esi
        mov    byte ptr [esi + 122h], 1
        call   sub_1_D7C2
loc_1_1C66:
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h
sub_1_1C70:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        mov    dword ptr [esi + 292h], ebx
        mov    byte ptr [esi + 296h], 1
        push   0
        push   0
        push   ebx
        call   sub_1_D810
        cmp    dword ptr [ebp - 4], 6
        jb     NEAR PTR loc_1_1D40
        mov    edi, dword ptr [esi + 317h]
        mov    eax, dword ptr [ebp - 4]
        add    edi, 0eh
        cmp    edi, eax
        jb     SHORT loc_1_1CBA
        mov    edi, eax
loc_1_1CBA:
        push   edi
        lea    eax, [esi + 32bh]
        push   0
        push   ebx
        push   eax
        call   sub_1_2120
        test   byte ptr [esi + 333h], 80h
        je     SHORT loc_1_1CE1
        mov    cl, byte ptr [esi + 339h]
        and    ecx, 1fh
        add    ecx, 0eh
        jmp    SHORT loc_1_1CE6
loc_1_1CE1:
        mov    ecx, 0eh
loc_1_1CE6:
        cmp    dword ptr [ebp - 4], 0eh
        mov    dword ptr [esi + 29fh], ecx
        jae    SHORT loc_1_1D19
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        lea    ecx, [esi + 32bh]
        push   0
        mov    edx, dword ptr [esi + 25dh]
        push   eax
        push   ecx
        push   esi
        push   edx
        call   sub_1_D260
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1D19:
        mov    eax, dword ptr [ebp - 4]
        sub    edi, ecx
        sub    eax, ecx
        push   eax
        push   edi
        lea    eax, [ecx + esi]
        mov    edx, dword ptr [esi + 25dh]
        add    eax, 32bh
        push   eax
        push   ecx
        lea    ecx, [esi + 32bh]
        push   ecx
        push   esi
        push   edx
        call   sub_1_D260
loc_1_1D40:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1D50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        inc    dword ptr [esi + 272h]
        call   sub_1_2A60
        cmp    dword ptr [esi + 28ah], 0
        je     NEAR PTR loc_1_1E26
loc_1_1D72:
        mov    byte ptr [esi + 450h], 0
        mov    ebx, dword ptr [esi + 28ah]
        cmp    dword ptr [esi + 28eh], ebx
        jne    SHORT loc_1_1D91
        mov    dword ptr [esi + 28eh], 0
loc_1_1D91:
        mov    eax, dword ptr [ebx + 24h]
        push   ebx
        push   esi
        mov    dword ptr [esi + 28ah], eax
        call   sub_1_1C70
        inc    dword ptr [esi + 2a3h]
        mov    edi, dword ptr [ebx + 28h]
        mov    eax, dword ptr [ebx + 2eh]
        push   eax
        push   ebx
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_D72A
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_1E19
        mov    eax, dword ptr [ebx + 26dh]
        cmp    edi, eax
        jne    SHORT loc_1_1DD4
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 26dh], eax
        jmp    SHORT loc_1_1DED
loc_1_1DD4:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_1DE1
loc_1_1DD9:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_1DD9
loc_1_1DE1:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [ebx + 452h], edi
loc_1_1DED:
        mov    eax, dword ptr [ebx + 317h]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_1DFE
        push   ebx
        call   sub_1_0BC0
loc_1_1DFE:
        cmp    dword ptr [ebx + 26dh], 0
        jne    SHORT loc_1_1E19
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 20ch
        push   ebx
        call   sub_1_D7DA
loc_1_1E19:
        cmp    dword ptr [esi + 28ah], 0
        jne    NEAR PTR loc_1_1D72
loc_1_1E26:
        mov    byte ptr [esi + 450h], 0
        push   esi
        call   sub_1_3800
        and    eax, 0ffffh
        cmp    eax, 8
        je     NEAR PTR loc_1_1EF4
        cmp    dword ptr [esi + 28ah], 0
        jne    NEAR PTR loc_1_1D72
        cmp    byte ptr [esi + 296h], 0
        je     SHORT loc_1_1E6A
        mov    byte ptr [esi + 296h], 0
        mov    eax, dword ptr [esi + 25dh]
        push   eax
        call   sub_1_D4B0
loc_1_1E6A:
        cmp    byte ptr [esi + 304h], 0
        je     NEAR PTR loc_1_20F4
        cmp    dword ptr [esi + 272h], 1
        jne    NEAR PTR loc_1_20F4
        cmp    byte ptr [esi + 305h], 0
        jne    NEAR PTR loc_1_20F4
        push   esi
        lea    eax, [esi + 20ch]
        mov    byte ptr [esi + 304h], 0
        mov    byte ptr [esi + 305h], 1
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_D7DA
        mov    edi, dword ptr [esi + 26dh]
        test   edi, edi
        je     NEAR PTR loc_1_1F6B
loc_1_1EBF:
        push   0
        inc    dword ptr [edi + 1ch]
        push   0
        mov    eax, dword ptr [edi]
        push   40010004h
        push   eax
        call   sub_1_D74E
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    NEAR PTR loc_1_1F60
        mov    eax, dword ptr [ebx + 26dh]
        cmp    eax, edi
        jne    SHORT loc_1_1F1B
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 26dh], eax
        jmp    SHORT loc_1_1F34
loc_1_1EF4:
        mov    byte ptr [esi + 303h], 1
        push   0
        push   0
        push   esi
        push   0
        call   sub_1_29F0
        mov    byte ptr [esi + 43dh], 0
        pop    edi
        dec    dword ptr [esi + 272h]
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_1F1B:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_1F28
loc_1_1F20:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_1F20
loc_1_1F28:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [ebx + 452h], edi
loc_1_1F34:
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [ebx + 317h], eax
        jne    SHORT loc_1_1F45
        push   ebx
        call   sub_1_0BC0
loc_1_1F45:
        cmp    dword ptr [ebx + 26dh], 0
        jne    SHORT loc_1_1F60
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 20ch
        push   ebx
        call   sub_1_D7DA
loc_1_1F60:
        mov    edi, dword ptr [edi + 0ch]
        test   edi, edi
        jne    NEAR PTR loc_1_1EBF
loc_1_1F6B:
        push   esi
        mov    edi, 0bb8h
        call   sub_1_3130
loc_1_1F76:
        push   3e8h
        call   sub_1_D7E0
        dec    edi
        jne    SHORT loc_1_1F76
        push   esi
        xor    edi, edi
        call   sub_1_2A60
loc_1_1F8B:
        push   esi
        call   sub_1_2B50
        and    eax, 0ffffh
        test   eax, eax
        je     SHORT loc_1_1FBB
        mov    byte ptr [esi + 303h], 1
        test   edi, edi
        jne    SHORT loc_1_1FC2
        mov    ebx, 0bb8h
        mov    edi, ebx
loc_1_1FAC:
        push   3e8h
        call   sub_1_D7E0
        dec    ebx
        jne    SHORT loc_1_1FAC
        jmp    SHORT loc_1_1F8B
loc_1_1FBB:
        mov    byte ptr [esi + 303h], 0
loc_1_1FC2:
        push   7530h
        lea    eax, [esi + 45eh]
        push   eax
        call   sub_1_D7C2
        cmp    dword ptr [esi + 282h], 0
        je     SHORT loc_1_200B
        mov    eax, dword ptr [esi + 27ah]
        mov    ecx, dword ptr [esi + 286h]
        mov    dword ptr [ecx + 24h], eax
        mov    edx, dword ptr [esi + 282h]
        mov    dword ptr [esi + 27ah], edx
        mov    dword ptr [esi + 286h], 0
        mov    dword ptr [esi + 282h], 0
loc_1_200B:
        cmp    dword ptr [esi + 28ah], 0
        je     SHORT loc_1_202F
        mov    eax, dword ptr [esi + 27ah]
        mov    ecx, dword ptr [esi + 28eh]
        mov    dword ptr [ecx + 24h], eax
        mov    edx, dword ptr [esi + 28ah]
        mov    dword ptr [esi + 27ah], edx
loc_1_202F:
        mov    dword ptr [esi + 28eh], 0
        mov    dword ptr [esi + 28ah], 0
        cmp    dword ptr [esi + 27ah], 0
        je     NEAR PTR loc_1_20DF
loc_1_2050:
        mov    ecx, dword ptr [esi + 27ah]
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [esi + 27ah], eax
        mov    edi, dword ptr [ecx + 28h]
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_D72A
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_20D2
        mov    eax, dword ptr [ebx + 26dh]
        cmp    eax, edi
        jne    SHORT loc_1_208D
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 26dh], eax
        jmp    SHORT loc_1_20A6
loc_1_208D:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_209A
loc_1_2092:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_2092
loc_1_209A:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [ebx + 452h], edi
loc_1_20A6:
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [ebx + 317h], eax
        jne    SHORT loc_1_20B7
        push   ebx
        call   sub_1_0BC0
loc_1_20B7:
        cmp    dword ptr [ebx + 26dh], 0
        jne    SHORT loc_1_20D2
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 20ch
        push   ebx
        call   sub_1_D7DA
loc_1_20D2:
        cmp    dword ptr [esi + 27ah], 0
        jne    NEAR PTR loc_1_2050
loc_1_20DF:
        xor    eax, eax
        pop    edi
        mov    dword ptr [esi + 27eh], eax
        mov    byte ptr [esi + 43dh], al
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_1_20F4:
        push   esi
        lea    eax, [esi + 20ch]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_D7DA
        mov    byte ptr [esi + 43dh], 0
        pop    edi
        dec    dword ptr [esi + 272h]
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2120:
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
        call   sub_1_D810
        mov    ebx, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp - 4], esi
        je     SHORT loc_1_2174
loc_1_2145:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_D834
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     SHORT loc_1_2174
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_D858
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_2145
loc_1_2174:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_2185
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_2185:
        mov    eax, ebx
        sub    eax, edi
        sub    edi, ebx
        add    dword ptr [ebp - 0ch], eax
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 14h]
loc_1_2194:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    SHORT loc_1_21A3
        mov    ebx, edi
        sub    ebx, esi
        jmp    SHORT loc_1_21A6
loc_1_21A3:
        mov    ebx, dword ptr [ebp - 8]
loc_1_21A6:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        add    esi, ebx
        push   eax
        call   sub_1_D8CC
        cmp    esi, edi
        je     SHORT loc_1_21E2
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_D858
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_21ED
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_D834
        jmp    SHORT loc_1_2194
loc_1_21E2:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_21ED:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_2200:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 14h
        mov    ecx, dword ptr [eax + 8]
        push   ebx
        push   esi
        mov    dword ptr [ebp - 10h], ecx
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        add    esi, dword ptr [ecx + 29fh]
        cmp    dword ptr [ecx + 292h], 0
        je     SHORT loc_1_2296
        push   0
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        push   0
        mov    edi, esi
        push   0
        push   ecx
        call   sub_1_D810
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_2284
        xor    ebx, ebx
loc_1_2242:
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 14h]
        push   eax
        mov    edx, dword ptr [ebp - 8]
        push   ecx
        push   edx
        call   sub_1_D834
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 10h]
        push   ecx
        push   edi
        mov    eax, dword ptr [edx + 292h]
        mov    ecx, dword ptr [ebp - 14h]
        push   eax
        push   ecx
        call   sub_1_2120
        add    edi, eax
        cmp    eax, dword ptr [ebp - 0ch]
        jb     SHORT loc_1_2284
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ecx
        call   sub_1_D858
        cmp    dword ptr [ebp - 8], ebx
        jne    SHORT loc_1_2242
loc_1_2284:
        sub    edi, esi
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], edi
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_2296:
        mov    eax, dword ptr [ebp - 10h]
        cmp    byte ptr [eax + 296h], 0
        je     SHORT loc_1_22FE
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        push   ecx
        mov    edx, dword ptr [ebp + 14h]
        push   esi
        mov    eax, dword ptr [ebp + 1ch]
        push   edx
        push   eax
        call   sub_1_3590
        mov    word ptr [ebp - 2], ax
        test   ax, ax
        je     SHORT loc_1_22F3
        int3
        mov    eax, dword ptr [ebp + 1ch]
        cmp    word ptr [ebp - 2], 8
        mov    dword ptr [eax], 0
        jne    SHORT loc_1_22E5
        mov    eax, dword ptr [ebp - 10h]
        push   0
        push   0
        push   eax
        mov    byte ptr [eax + 303h], 1
        push   0
        call   sub_1_29F0
loc_1_22E5:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_22F3:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_22FE:
        mov    eax, 0c0010013h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 064h, 024h, 000h
loc_1_2310:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        mov    byte ptr [esi + 451h], 1
        call   sub_1_3130
        test   ax, ax
        jne    SHORT loc_1_2348
        push   7530h
        lea    eax, [esi + 45eh]
        push   eax
        call   sub_1_D7C2
        mov    byte ptr [esi + 303h], 0
        mov    al, 1
        pop    esi
        pop    ebp
        ret    4
loc_1_2348:
        push   4e20h
        add    esi, 45eh
        push   esi
        call   sub_1_D7C2
        xor    al, al
        pop    esi
        pop    ebp
        ret    4
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 450h], 0
        je     NEAR PTR loc_1_2476
        mov    edi, dword ptr [esi + 27ah]
        test   edi, edi
        je     NEAR PTR loc_1_2476
        cmp    byte ptr [esi + 304h], 0
        jne    NEAR PTR loc_1_248B
        cmp    byte ptr [esi + 305h], 0
        jne    NEAR PTR loc_1_248B
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [esi + 27ah], eax
        test   eax, eax
        jne    SHORT loc_1_23B8
        mov    dword ptr [esi + 27eh], 0
loc_1_23B8:
        push   esi
        mov    ebx, 1
        push   edi
        call   sub_1_3760
        and    eax, 0ffffh
loc_1_23C9:
        cmp    eax, 6
        je     SHORT loc_1_23F0
        test   eax, eax
        je     SHORT loc_1_241A
        cmp    eax, 8
        jne    SHORT loc_1_244E
        push   0
        mov    byte ptr [esi + 303h], bl
        push   0
        push   esi
        push   0
        call   sub_1_29F0
        mov    eax, 6
        jmp    SHORT loc_1_23C9
loc_1_23F0:
        mov    byte ptr [esi + 303h], 1
        mov    eax, dword ptr [esi + 27ah]
        test   eax, eax
        je     SHORT loc_1_240C
        mov    dword ptr [edi + 24h], eax
        mov    dword ptr [esi + 27ah], edi
        jmp    SHORT loc_1_244E
loc_1_240C:
        mov    dword ptr [esi + 27ah], edi
        mov    dword ptr [esi + 27eh], edi
        jmp    SHORT loc_1_244E
loc_1_241A:
        mov    byte ptr [esi + 450h], 0
        cmp    dword ptr [esi + 282h], 0
        jne    SHORT loc_1_2438
        mov    dword ptr [esi + 286h], edi
        mov    dword ptr [esi + 282h], edi
        jmp    SHORT loc_1_2447
loc_1_2438:
        mov    eax, dword ptr [esi + 286h]
        mov    dword ptr [eax + 24h], edi
        mov    dword ptr [esi + 286h], edi
loc_1_2447:
        mov    dword ptr [edi + 24h], 0
loc_1_244E:
        add    esi, 444h
        lea    eax, [ebp - 1]
        push   eax
        push   esi
        mov    byte ptr [esi + 0ch], 1
        call   sub_1_D7C8
        push   2710h
        push   esi
        call   sub_1_D7C2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_2476:
        push   2710h
        add    esi, 444h
        push   esi
        mov    byte ptr [esi + 0ch], 1
        call   sub_1_D7C2
loc_1_248B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_24A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [esi + 26dh]
        mov    al, byte ptr [esi + 271h]
        cmp    al, 1
        jne    SHORT loc_1_2512
        cmp    word ptr [esi + 40h], 1
        je     SHORT loc_1_250A
        push   esi
        call   sub_1_2B50
        test   ax, ax
        jne    NEAR PTR loc_1_2789
        mov    eax, dword ptr [esi + 307h]
        test   eax, eax
        lea    ecx, [eax + 1]
        mov    dword ptr [esi + 307h], ecx
        jne    NEAR PTR loc_1_2789
        push   7530h
        add    esi, 45eh
        push   esi
        mov    byte ptr [esi - 15bh], 0
        call   sub_1_D7C2
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_250A:
        cmp    al, 1
        je     NEAR PTR loc_1_2789
loc_1_2512:
        cmp    byte ptr [esi + 451h], 1
        jne    NEAR PTR loc_1_260B
        mov    byte ptr [esi + 451h], 0
        cmp    dword ptr [esi + 282h], 0
        je     SHORT loc_1_2568
        xor    edi, edi
loc_1_2531:
        mov    edx, dword ptr [esi + 282h]
        lea    ecx, [edx + 24h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 282h], eax
        test   eax, eax
        jne    SHORT loc_1_254C
        mov    dword ptr [esi + 286h], edi
loc_1_254C:
        mov    ebx, dword ptr [ecx + 4]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_D72A
        dec    dword ptr [ebx + 1ch]
        cmp    dword ptr [esi + 282h], edi
        jne    SHORT loc_1_2531
loc_1_2568:
        cmp    dword ptr [esi + 27ah], 0
        je     SHORT loc_1_25AA
        xor    edi, edi
loc_1_2573:
        mov    edx, dword ptr [esi + 27ah]
        lea    ecx, [edx + 24h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 27ah], eax
        test   eax, eax
        jne    SHORT loc_1_258E
        mov    dword ptr [esi + 27eh], edi
loc_1_258E:
        mov    ebx, dword ptr [ecx + 4]
        push   0c001000ch
        push   edx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_D72A
        dec    dword ptr [ebx + 1ch]
        cmp    dword ptr [esi + 27ah], edi
        jne    SHORT loc_1_2573
loc_1_25AA:
        mov    eax, dword ptr [esi + 452h]
        xor    ebx, ebx
        push   ebx
        mov    dword ptr [eax + 1ch], ebx
        mov    byte ptr [esi + 451h], bl
        mov    byte ptr [esi + 303h], bl
        mov    eax, dword ptr [esi + 452h]
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_D724
        push   ebx
        mov    ecx, dword ptr [esi + 452h]
        push   ebx
        push   40010007h
        mov    edx, dword ptr [ecx]
        push   edx
        call   sub_1_D74E
        mov    ecx, dword ptr [esi + 452h]
        mov    edx, dword ptr [ecx]
        push   edx
        call   sub_1_D754
        push   ebx
        mov    ecx, dword ptr [esi + 452h]
        push   75h
        push   ecx
        call   sub_1_D7EC
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_260B:
        mov    al, byte ptr [esi + 305h]
        cmp    al, 1
        jne    NEAR PTR loc_1_2779
        cmp    word ptr [esi + 40h], 1
        je     SHORT loc_1_262B
        test   al, al
        je     SHORT loc_1_262B
        mov    byte ptr [esi + 303h], 1
loc_1_262B:
        test   edi, edi
        je     NEAR PTR loc_1_26D5
loc_1_2633:
        mov    eax, 0
        push   4
        inc    dword ptr [edi + 1ch]
        cmp    byte ptr [esi + 303h], 1
        adc    eax, -1
        and    eax, 0c0000001h
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [edi]
        push   40010005h
        push   ecx
        call   sub_1_D74E
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_D754
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_26CA
        mov    eax, dword ptr [ebx + 26dh]
        cmp    edi, eax
        jne    SHORT loc_1_2685
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 26dh], eax
        jmp    SHORT loc_1_269E
loc_1_2685:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_2692
loc_1_268A:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_268A
loc_1_2692:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
        mov    dword ptr [ebx + 452h], edi
loc_1_269E:
        mov    eax, dword ptr [ebx + 317h]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_26AF
        push   ebx
        call   sub_1_0BC0
loc_1_26AF:
        cmp    dword ptr [ebx + 26dh], 0
        jne    SHORT loc_1_26CA
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 20ch
        push   ebx
        call   sub_1_D7DA
loc_1_26CA:
        mov    edi, dword ptr [edi + 0ch]
        test   edi, edi
        jne    NEAR PTR loc_1_2633
loc_1_26D5:
        mov    byte ptr [esi + 305h], 0
        mov    eax, 0
        mov    ecx, dword ptr [esi + 30bh]
        cmp    byte ptr [esi + 303h], 1
        adc    eax, -1
        mov    edx, dword ptr [ecx]
        and    eax, 0c0000001h
        push   eax
        push   edx
        call   sub_1_D736
        mov    ecx, dword ptr [esi + 30bh]
        mov    edi, dword ptr [ecx + 8]
        dec    dword ptr [ecx + 1ch]
        jne    SHORT loc_1_2773
        mov    edx, dword ptr [edi + 26dh]
        cmp    ecx, edx
        jne    SHORT loc_1_2722
        mov    eax, dword ptr [ecx + 0ch]
        mov    dword ptr [edi + 26dh], eax
        jmp    SHORT loc_1_2747
loc_1_2722:
        mov    eax, dword ptr [esi + 30bh]
        cmp    dword ptr [edx + 0ch], eax
        je     SHORT loc_1_2735
loc_1_272D:
        mov    edx, dword ptr [edx + 0ch]
        cmp    dword ptr [edx + 0ch], eax
        jne    SHORT loc_1_272D
loc_1_2735:
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [edx + 0ch], eax
        mov    ecx, dword ptr [esi + 30bh]
        mov    dword ptr [edi + 452h], ecx
loc_1_2747:
        mov    eax, dword ptr [ecx + 18h]
        cmp    dword ptr [edi + 317h], eax
        jne    SHORT loc_1_2758
        push   edi
        call   sub_1_0BC0
loc_1_2758:
        cmp    dword ptr [edi + 26dh], 0
        jne    SHORT loc_1_2773
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        add    edi, 20ch
        push   edi
        call   sub_1_D7DA
loc_1_2773:
        dec    dword ptr [esi + 272h]
loc_1_2779:
        mov    byte ptr [esi + 303h], 0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_2789:
        mov    byte ptr [esi + 303h], 0
        mov    byte ptr [esi + 271h], 0
        mov    word ptr [esi + 40h], 0fh
        push   0
        mov    eax, dword ptr [esi + 456h]
        push   0c0010007h
        mov    ecx, dword ptr [eax]
        push   ecx
        call   sub_1_D71E
        push   0
        mov    ecx, dword ptr [esi + 456h]
        push   75h
        push   ecx
        call   sub_1_D7EC
        pop    edi
        dec    dword ptr [esi + 272h]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_27E0:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [eax + 8]
        inc    dword ptr [edi + 272h]
        cmp    byte ptr [edi + 305h], 0
        je     SHORT loc_1_280B
        mov    ebx, 0c001000dh
        jmp    NEAR PTR loc_1_289F
loc_1_280B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, eax
        shr    ecx, 18h
        mov    byte ptr [ebp - 4], cl
        mov    ecx, eax
        shr    ecx, 10h
        mov    byte ptr [ebp - 3], cl
        mov    byte ptr [ebp - 2], ah
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [edi + 9fh], al
        mov    cl, byte ptr [ebp - 3]
        mov    byte ptr [edi + 0a0h], cl
        mov    dl, byte ptr [ebp - 2]
        mov    byte ptr [edi + 0a1h], dl
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [edi + 0a2h], al
loc_1_2848:
        push   edi
        call   sub_1_3410
        test   ax, ax
        je     SHORT loc_1_289D
        cmp    ax, 8
        mov    ebx, 0c0000001h
        jne    SHORT loc_1_289F
        test   esi, esi
        jne    SHORT loc_1_288C
        push   0
        xor    esi, esi
        mov    byte ptr [edi + 303h], 1
        push   0
        push   edi
        push   0
        call   sub_1_29F0
        mov    ax, word ptr [edi + 40h]
loc_1_287B:
        cmp    ax, 1
        je     SHORT loc_1_2848
        inc    esi
        cmp    esi, 0fffffh
        jb     SHORT loc_1_287B
        jmp    SHORT loc_1_289F
loc_1_288C:
        mov    eax, ebx
        dec    dword ptr [edi + 272h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_289D:
        xor    ebx, ebx
loc_1_289F:
        mov    eax, ebx
        dec    dword ptr [edi + 272h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_28B0:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 8]
        xor    ebx, ebx
        inc    dword ptr [esi + 272h]
        cmp    byte ptr [esi + 305h], bl
        je     SHORT loc_1_28DA
        mov    edi, 0c001000dh
        jmp    NEAR PTR loc_1_2973
loc_1_28DA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, eax
        shr    ecx, 18h
        mov    byte ptr [ebp - 4], cl
        mov    ecx, eax
        shr    ecx, 10h
        mov    byte ptr [ebp - 3], cl
        mov    byte ptr [ebp - 2], ah
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [esi + 99h], al
        mov    cl, byte ptr [ebp - 3]
        mov    byte ptr [esi + 9ah], cl
        mov    dl, byte ptr [ebp - 2]
        mov    byte ptr [esi + 9bh], dl
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 9ch], al
loc_1_2917:
        push   esi
        call   sub_1_3440
        cmp    ax, 1
        sbb    eax, eax
        inc    ax
        je     SHORT loc_1_2971
        cmp    ax, 8
        mov    edi, 0c0000001h
        jne    SHORT loc_1_2973
        test   ebx, ebx
        jne    SHORT loc_1_2960
        push   0
        xor    ebx, ebx
        mov    byte ptr [esi + 303h], 1
        push   0
        push   esi
        push   0
        call   sub_1_29F0
        mov    ax, word ptr [esi + 40h]
loc_1_294F:
        cmp    ax, 1
        je     SHORT loc_1_2917
        inc    ebx
        cmp    ebx, 0ffffh
        jb     SHORT loc_1_294F
        jmp    SHORT loc_1_2973
loc_1_2960:
        mov    eax, edi
        dec    dword ptr [esi + 272h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_2971:
        xor    edi, edi
loc_1_2973:
        mov    eax, edi
        dec    dword ptr [esi + 272h]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2990:
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, esp
        push   esi
        push   1
        push   eax
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
        mov    edx, dword ptr [esi + 14h]
        push   edx
        mov    eax, dword ptr [esi + 8]
        inc    dword ptr [esi + 1ch]
        mov    ecx, dword ptr [eax + 25dh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_29B8:
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    10h
sub_1_29C0:
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, esp
        push   esi
        push   1
        push   eax
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
        mov    edx, dword ptr [esi + 14h]
        push   edx
        mov    eax, dword ptr [esi + 8]
        inc    dword ptr [esi + 1ch]
        mov    ecx, dword ptr [eax + 25dh]
        push   ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_29E8:
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    10h
sub_1_29F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 272h]
        cmp    byte ptr [esi + 303h], 1
        jne    SHORT loc_1_2A4F
        cmp    byte ptr [esi + 305h], 0
        jne    SHORT loc_1_2A4F
        cmp    byte ptr [esi + 304h], 0
        jne    SHORT loc_1_2A4F
        cmp    dword ptr [esi + 26dh], 0
        je     SHORT loc_1_2A4F
        mov    byte ptr [esi + 303h], 0
        mov    byte ptr [esi + 305h], 1
        mov    byte ptr [esi + 306h], 1
        push   esi
        call   sub_1_2B50
        test   ax, ax
        je     SHORT loc_1_2A4F
        mov    byte ptr [esi + 305h], 0
        mov    byte ptr [esi + 306h], 0
loc_1_2A4F:
        dec    dword ptr [esi + 272h]
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2A60:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        mov    ax, word ptr [ecx + 46h]
        cmp    eax, 2
        je     SHORT loc_1_2A7C
        cmp    eax, 3
        je     SHORT loc_1_2A96
        cmp    eax, 5
        jne    SHORT loc_1_2AA3
loc_1_2A7C:
        xor    edx, edx
        mov    al, 32h
        mov    dx, word ptr [ecx + 4]
        pop    ebp
        mov    byte ptr [ecx + 198h], 32h
        add    edx, 10h
        out    dx, al
        xor    ax, ax
        ret    4
loc_1_2A96:
        xor    edx, edx
        mov    dx, word ptr [ecx + 4]
        add    edx, 5
        in     al, dx
        and    al, 0fbh
        out    dx, al
loc_1_2AA3:
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2AB0:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        mov    ax, word ptr [ecx + 46h]
        cmp    eax, 2
        je     SHORT loc_1_2AD3
        cmp    eax, 3
        je     SHORT loc_1_2AF8
        cmp    eax, 5
        je     SHORT loc_1_2B1D
        xor    ax, ax
        pop    ebp
        ret    4
loc_1_2AD3:
        mov    byte ptr [ecx + 198h], 12h
        mov    cx, word ptr [ecx + 4]
        mov    al, 12h
        and    ecx, 0ffffh
        lea    edx, [ecx + 10h]
        pop    ebp
        out    dx, al
        lea    edx, [ecx + 4]
        in     al, dx
        or     al, 80h
        out    dx, al
        xor    ax, ax
        ret    4
loc_1_2AF8:
        mov    byte ptr [ecx + 198h], 12h
        mov    cx, word ptr [ecx + 4]
        mov    al, 12h
        and    ecx, 0ffffh
        lea    edx, [ecx + 4]
        pop    ebp
        out    dx, al
        lea    edx, [ecx + 5]
        in     al, dx
        or     al, 4
        out    dx, al
        xor    ax, ax
        ret    4
loc_1_2B1D:
        mov    byte ptr [ecx + 198h], 12h
        mov    cx, word ptr [ecx + 4]
        mov    al, 12h
        and    ecx, 0ffffh
        lea    edx, [ecx + 10h]
        pop    ebp
        out    dx, al
        lea    edx, [ecx + 5]
        in     al, dx
        or     al, 4
        out    dx, al
        xor    ax, ax
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2B50:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2A60
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 40h], 3
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 20h]
        and    eax, 0ffffh
        shl    eax, 0ah
        dec    eax
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0b0h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 19eh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 97h]
        and    eax, 0ffffh
        test   al, 80h
        jne    NEAR PTR loc_1_2BC4
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 97h]
        and    eax, 0ffffh
        or     eax, 0c0h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 97h], ax
loc_1_2BC4:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 99h]
        and    eax, 0ffffh
        test   al, 80h
        jne    NEAR PTR loc_1_2BF9
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 99h]
        and    eax, 0ffffh
        or     eax, 80h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 99h], ax
loc_1_2BF9:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 9dh]
        and    eax, 0ffffh
        test   al, 0c0h
        jne    NEAR PTR loc_1_2C2E
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 9dh]
        and    eax, 0ffffh
        or     eax, 0c0h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 9dh], ax
loc_1_2C2E:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 9fh]
        and    eax, 0ff7fh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 9fh], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 91h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_2C6A
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 91h], 7fffh
loc_1_2C6A:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 93h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_2C8D
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 93h], 6
loc_1_2C8D:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6A9B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_6B03
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_946E
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_2CCE
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_2F71
loc_1_2CCE:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_94DC
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_2CFD
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_2F71
loc_1_2CFD:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_92CB
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_984F
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_2D47
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 12h
        jmp    NEAR PTR loc_1_2F71
loc_1_2D47:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9572
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_2D76
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_2F71
loc_1_2D76:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 42h]
        and    eax, 0fffdh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 42h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 42h]
        and    eax, 0ffffh
        or     eax, 1
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 42h], ax
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_2DAE
loc_1_2DAA:
        inc    word ptr [ebp - 0ch]
loc_1_2DAE:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 6
        jge    NEAR PTR loc_1_2DEE
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        lea    edx, [eax + ecx + 8]
        in     al, dx
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 2ah], al
        jmp    NEAR PTR loc_1_2DAA
loc_1_2DEE:
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_2DFD
loc_1_2DF9:
        inc    word ptr [ebp - 0ch]
loc_1_2DFD:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 6
        jge    NEAR PTR loc_1_2E2F
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 24h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_2E2F
        jmp    NEAR PTR loc_1_2DF9
loc_1_2E2F:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 6
        jne    NEAR PTR loc_1_2E84
        mov    word ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_2E4F
loc_1_2E4B:
        inc    word ptr [ebp - 0ch]
loc_1_2E4F:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        cmp    eax, 6
        jge    NEAR PTR loc_1_2E84
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + ecx + 2ah]
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 24h], al
        jmp    NEAR PTR loc_1_2E4B
loc_1_2E84:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_939D
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8E75
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_2EBC
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_2F71
loc_1_2EBC:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9A6F
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_2EEB
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_2F71
loc_1_2EEB:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_939D
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_8E75
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_2F23
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_2F71
loc_1_2F23:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2AB0
        push   7ff6h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9265
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_2F60
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, word ptr [ebp - 8]
        jmp    NEAR PTR loc_1_2F71
loc_1_2F60:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        xor    ax, ax
        jmp    NEAR PTR loc_1_2F71
loc_1_2F71:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_2F78:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 40h]
        cmp    ax, 1
        jne    SHORT loc_1_2F91
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
loc_1_2F91:
        cmp    ax, 2
        je     SHORT loc_1_2FA0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_2FA0:
        push   esi
        call   sub_1_2A60
        push   esi
        call   sub_1_67FC
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        push   esi
        call   sub_1_9300
        push   0
        push   esi
        call   sub_1_9103
        test   ax, ax
        je     SHORT loc_1_2FD8
        push   esi
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_2FD8:
        push   0
        push   esi
        call   sub_1_917D
        test   ax, ax
        je     SHORT loc_1_2FFA
        push   esi
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_2FFA:
        push   1
        push   esi
        call   sub_1_9103
        test   ax, ax
        je     SHORT loc_1_301C
        push   esi
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_301C:
        push   1
        push   esi
        call   sub_1_917D
        test   ax, ax
        je     SHORT loc_1_303E
        push   esi
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_303E:
        mov    byte ptr [esi + 19eh], 1
        push   esi
        call   sub_1_A6C9
        test   ax, ax
        push   esi
        je     SHORT loc_1_3065
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_3065:
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        push   esi
        call   sub_1_2A60
        push   esi
        call   sub_1_67FC
        mov    byte ptr [esi + 19eh], 0
        mov    word ptr [esi + 3eh], 0
        push   esi
        call   sub_1_31A0
        mov    ax, word ptr [esi + 200h]
        and    ax, 7
        cmp    ax, 1
        jne    SHORT loc_1_30C4
        push   1
        push   1
        push   esi
        call   sub_1_6B39
        test   ax, ax
        je     SHORT loc_1_3112
        push   esi
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_30C4:
        cmp    ax, 2
        jne    SHORT loc_1_30EE
        push   2
        push   1
        push   esi
        call   sub_1_6B39
        test   ax, ax
        je     SHORT loc_1_3112
        push   esi
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_30EE:
        push   0
        push   2
        push   esi
        call   sub_1_6B39
        test   ax, ax
        je     SHORT loc_1_3112
        push   esi
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    ax, 7
        pop    esi
        pop    ebp
        ret    4
loc_1_3112:
        push   esi
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3130:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi + 40h], 1
        je     SHORT loc_1_314D
        mov    word ptr [esi + 40h], 5
        mov    ax, 0ah
        pop    esi
        pop    ebp
        ret    4
loc_1_314D:
        push   esi
        call   sub_1_2A60
        push   esi
        call   sub_1_67FC
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        push   esi
        call   sub_1_9300
        push   esi
        call   sub_1_9201
        test   ax, ax
        push   esi
        je     SHORT loc_1_3189
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        mov    word ptr [esi + 40h], 5
        mov    ax, 0ah
        pop    esi
        pop    ebp
        ret    4
loc_1_3189:
        call   sub_1_682C
        push   esi
        call   sub_1_2AB0
        xor    ax, ax
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_31A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 0
        mov    eax, dword ptr [esi + 25dh]
        test   eax, eax
        je     SHORT loc_1_31BE
        mov    edi, dword ptr [eax + 10h]
loc_1_31BE:
        test   edi, 10h
        lea    ecx, [esi + 3eh]
        je     SHORT loc_1_31D5
        or     byte ptr [esi + 19ch], 0c0h
        or     byte ptr [ecx], 8
        jmp    SHORT loc_1_31E3
loc_1_31D5:
        and    word ptr [esi + 19ch], 0ffbfh
        and    word ptr [ecx], 0fff7h
loc_1_31E3:
        test   edi, 8000h
        je     SHORT loc_1_31F9
        or     byte ptr [esi + 19bh], 4
        or     word ptr [ecx], 120h
        jmp    SHORT loc_1_3207
loc_1_31F9:
        and    word ptr [esi + 19ah], 0fbffh
        and    word ptr [ecx], 0fedfh
loc_1_3207:
        test   edi, 20h
        je     SHORT loc_1_321B
        or     byte ptr [esi + 19bh], 8
        or     byte ptr [ecx], 4
        jmp    SHORT loc_1_3229
loc_1_321B:
        and    word ptr [esi + 19ah], 0f7ffh
        and    word ptr [ecx], 0fffbh
loc_1_3229:
        test   edi, 2000h
        je     SHORT loc_1_323D
        or     byte ptr [esi + 19ch], 10h
        or     byte ptr [ecx], 40h
        jmp    SHORT loc_1_324B
loc_1_323D:
        and    word ptr [esi + 19ch], 0ffefh
        and    word ptr [ecx], 0ffbfh
loc_1_324B:
        test   edi, 4000h
        je     SHORT loc_1_3277
        push   esi
        call   sub_1_A3FF
        mov    bx, ax
        test   bx, bx
        je     NEAR PTR loc_1_32F4
        push   esi
        call   sub_1_682C
        mov    ax, bx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3277:
        mov    ax, word ptr [esi + 9dh]
        mov    word ptr [esi + 9dh], 0
        mov    cx, word ptr [esi + 9fh]
        push   esi
        mov    word ptr [esi + 9fh], 0
        mov    dx, word ptr [esi + 0a1h]
        mov    word ptr [esi + 0a1h], 0
        mov    word ptr [ebp - 8], ax
        mov    word ptr [ebp - 6], cx
        mov    word ptr [ebp - 4], dx
        call   sub_1_A3FF
        mov    bx, ax
        mov    cx, word ptr [ebp - 8]
        mov    ax, word ptr [ebp - 6]
        mov    dx, word ptr [ebp - 4]
        mov    word ptr [esi + 9dh], cx
        mov    word ptr [esi + 9fh], ax
        mov    word ptr [esi + 0a1h], dx
        test   bx, bx
        je     SHORT loc_1_32F4
        push   esi
        call   sub_1_682C
        mov    ax, bx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_32F4:
        test   edi, 1000h
        je     SHORT loc_1_3320
        push   esi
        call   sub_1_A564
        mov    di, ax
        test   di, di
        je     NEAR PTR loc_1_3392
        push   esi
        call   sub_1_682C
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3320:
        mov    di, word ptr [esi + 97h]
        mov    word ptr [esi + 97h], 0
        mov    ax, word ptr [esi + 99h]
        push   esi
        mov    word ptr [esi + 99h], 0
        mov    cx, word ptr [esi + 9bh]
        mov    word ptr [esi + 9bh], 0
        mov    word ptr [ebp - 6], ax
        mov    word ptr [ebp - 4], cx
        call   sub_1_A564
        mov    cx, word ptr [ebp - 6]
        mov    dx, word ptr [ebp - 4]
        mov    word ptr [esi + 97h], di
        mov    word ptr [esi + 99h], cx
        mov    word ptr [esi + 9bh], dx
        test   bx, bx
        je     SHORT loc_1_3392
        push   esi
        call   sub_1_682C
        mov    ax, bx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3392:
        push   esi
        call   sub_1_67FC
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        push   esi
        call   sub_1_9300
        lea    eax, [esi + 19ah]
        push   eax
        push   0a001h
        push   esi
        call   sub_1_7B23
        mov    di, ax
        test   di, di
        je     SHORT loc_1_33CE
        push   esi
        call   sub_1_682C
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_33CE:
        lea    eax, [esi + 19ch]
        push   eax
        push   0a101h
        push   esi
        call   sub_1_7B23
        mov    di, ax
        push   esi
        test   di, di
        je     SHORT loc_1_33FA
        call   sub_1_682C
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_33FA:
        call   sub_1_682C
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_3410:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 25dh]
        test   eax, eax
        je     SHORT loc_1_3426
        mov    eax, dword ptr [eax + 10h]
        jmp    SHORT loc_1_3428
loc_1_3426:
        xor    eax, eax
loc_1_3428:
        test   ah, 40h
        je     SHORT loc_1_3437
        push   ecx
        call   sub_1_A3FF
        pop    ebp
        ret    4
loc_1_3437:
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_3440:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 25dh]
        test   eax, eax
        je     SHORT loc_1_3456
        mov    eax, dword ptr [eax + 10h]
        jmp    SHORT loc_1_3458
loc_1_3456:
        xor    eax, eax
loc_1_3458:
        test   ah, 10h
        je     SHORT loc_1_3467
        push   ecx
        call   sub_1_A3FF
        pop    ebp
        ret    4
loc_1_3467:
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_3470:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        mov    dword ptr [ebp - 0ch], eax
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 14h]
        mov    bx, word ptr [esi + 0bah]
        push   esi
        call   sub_1_67FC
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        push   esi
        call   sub_1_9300
        push   ebx
        push   esi
        call   sub_1_8AF5
        mov    edi, eax
        mov    dword ptr [ebp - 18h], eax
        mov    ax, word ptr [esi + 20h]
        shl    ax, 0ah
        cmp    dword ptr [ebp + 8], 0
        mov    word ptr [ebp - 2], ax
        je     NEAR PTR loc_1_356E
        mov    eax, ebx
        and    eax, 0ffffh
        lea    ecx, [esi + eax*4]
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        mov    dword ptr [ebp - 10h], ecx
        add    ecx, 190h
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 8], ecx
loc_1_34DB:
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, dword ptr [eax]
        mov    eax, ebx
        sub    eax, edi
        cmp    eax, dword ptr [ebp + 8]
        jae    SHORT loc_1_34EE
        sub    bx, di
        jmp    SHORT loc_1_34F2
loc_1_34EE:
        mov    bx, word ptr [ebp + 8]
loc_1_34F2:
        push   edi
        push   esi
        call   sub_1_9300
        mov    eax, edi
        mov    ecx, dword ptr [ebp - 14h]
        sub    ax, word ptr [esi + 1ch]
        and    ax, word ptr [esi + 0b0h]
        mov    edx, eax
        and    edx, 0ffffh
        sub    ecx, edx
        mov    edx, ebx
        and    edx, 0ffffh
        cmp    ecx, edx
        jge    SHORT loc_1_3526
        mov    bx, word ptr [ebp - 2]
        sub    bx, ax
loc_1_3526:
        mov    ecx, edi
        xor    eax, eax
        mov    ax, word ptr [esi + 0b0h]
        sub    ecx, dword ptr [esi + 1ch]
        and    ecx, eax
        and    ebx, 0ffffh
        mov    eax, dword ptr [esi + 1ch]
        push   ebx
        add    eax, ecx
        add    edi, ebx
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        call   sub_1_D8CC
        mov    eax, dword ptr [ebp - 8]
        sub    dword ptr [ebp + 8], ebx
        add    dword ptr [ebp - 0ch], ebx
        cmp    dword ptr [eax], edi
        ja     SHORT loc_1_3564
        mov    eax, dword ptr [ebp - 10h]
        mov    edi, dword ptr [eax + 188h]
loc_1_3564:
        cmp    dword ptr [ebp + 8], 0
        jne    NEAR PTR loc_1_34DB
loc_1_356E:
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        push   esi
        call   sub_1_9300
        push   esi
        call   sub_1_682C
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_3590:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 18h]
        mov    ax, word ptr [esi + 0bah]
        push   esi
        mov    word ptr [ebp - 4], ax
        mov    dword ptr [ecx], 0
        call   sub_1_67FC
        mov    ecx, dword ptr [esi + 1ch]
        push   ecx
        push   esi
        call   sub_1_9300
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        push   esi
        call   sub_1_8AF5
        mov    edi, dword ptr [ebp + 10h]
        push   0
        add    edi, eax
        lea    ecx, [ebp - 14h]
        push   ecx
        mov    dword ptr [ebp - 18h], eax
        mov    ax, word ptr [esi + 20h]
        push   0
        shl    ax, 0ah
        push   0
        mov    word ptr [ebp - 2], ax
        mov    edx, dword ptr [ebp + 14h]
        push   edx
        call   sub_1_D810
        cmp    dword ptr [ebp - 14h], 0
        jne    SHORT loc_1_3613
        mov    ecx, dword ptr [ebp - 18h]
        push   ecx
        push   esi
        call   sub_1_9300
        push   esi
        call   sub_1_682C
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_3613:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp - 14h]
        push   ecx
        push   edx
        call   sub_1_D834
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_373B
loc_1_362E:
        cmp    dword ptr [ebp - 8], 0
        jbe    NEAR PTR loc_1_3701
loc_1_3638:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_3731
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ebx, dword ptr [esi + eax*4 + 190h]
        lea    ecx, [esi + eax*4]
        mov    eax, ebx
        sub    eax, edi
        mov    dword ptr [ebp - 0ch], ecx
        cmp    eax, dword ptr [ebp + 0ch]
        jae    SHORT loc_1_3665
        sub    bx, di
        jmp    SHORT loc_1_3669
loc_1_3665:
        mov    bx, word ptr [ebp + 0ch]
loc_1_3669:
        push   edi
        push   esi
        call   sub_1_9300
        mov    eax, edi
        xor    ecx, ecx
        sub    ax, word ptr [esi + 1ch]
        and    ax, word ptr [esi + 0b0h]
        mov    cx, word ptr [ebp - 2]
        mov    edx, eax
        and    edx, 0ffffh
        sub    ecx, edx
        mov    edx, ebx
        and    edx, 0ffffh
        cmp    ecx, edx
        jge    SHORT loc_1_36A0
        mov    bx, word ptr [ebp - 2]
        sub    bx, ax
loc_1_36A0:
        mov    eax, ebx
        and    eax, 0ffffh
        cmp    eax, dword ptr [ebp - 8]
        jbe    SHORT loc_1_36B0
        mov    bx, word ptr [ebp - 8]
loc_1_36B0:
        mov    ecx, edi
        xor    eax, eax
        mov    ax, word ptr [esi + 0b0h]
        sub    ecx, dword ptr [esi + 1ch]
        and    ecx, eax
        and    ebx, 0ffffh
        mov    eax, dword ptr [esi + 1ch]
        push   ebx
        add    eax, ecx
        add    edi, ebx
        push   eax
        mov    ecx, dword ptr [ebp - 10h]
        push   ecx
        call   sub_1_D8CC
        mov    eax, dword ptr [ebp - 0ch]
        sub    dword ptr [ebp - 8], ebx
        sub    dword ptr [ebp + 0ch], ebx
        add    dword ptr [ebp - 10h], ebx
        cmp    dword ptr [eax + 190h], edi
        ja     SHORT loc_1_36F2
        mov    edi, dword ptr [eax + 188h]
loc_1_36F2:
        mov    eax, dword ptr [ebp + 8]
        add    dword ptr [eax], ebx
        cmp    dword ptr [ebp - 8], 0
        ja     NEAR PTR loc_1_3638
loc_1_3701:
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_373B
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_3731
        lea    eax, [ebp - 14h]
        mov    ecx, dword ptr [ebp - 14h]
        push   eax
        push   ecx
        call   sub_1_D858
        cmp    dword ptr [ebp - 14h], 0
        je     SHORT loc_1_373B
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp - 14h]
        push   ecx
        push   edx
        call   sub_1_D834
loc_1_3731:
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_362E
loc_1_373B:
        mov    ecx, dword ptr [ebp - 18h]
        push   ecx
        push   esi
        call   sub_1_9300
        push   esi
        call   sub_1_682C
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3760:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    word ptr [esi + 40h], 1
        je     SHORT loc_1_3780
        mov    ax, 6
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3780:
        push   esi
        call   sub_1_67FC
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        push   esi
        call   sub_1_9300
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 8]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   ecx
        push   0
        push   0
        push   edi
        call   sub_1_D810
        mov    ecx, dword ptr [ebp - 4]
        push   ecx
        push   1
        push   esi
        call   sub_1_6C63
        cmp    eax, -1
        mov    ebx, eax
        jne    SHORT loc_1_37CB
        push   esi
        call   sub_1_682C
        mov    ax, 6
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_37CB:
        mov    eax, dword ptr [ebx + 0eh]
        mov    ecx, dword ptr [eax + 0ch]
        push   ecx
        push   edi
        push   esi
        call   sub_1_7096
        push   1
        push   ebx
        push   esi
        call   sub_1_6F4B
        mov    di, ax
        push   esi
        call   sub_1_682C
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3800:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 1ch], 0
        mov    word ptr [ebp - 0ch], 0ff00h
        mov    word ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9300
loc_1_3834:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0d0h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0cch]
        mov    al, byte ptr [ecx + eax*2]
        and    eax, 0ffh
        test   al, 0f0h
        jne    NEAR PTR loc_1_4458
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 5
        je     NEAR PTR loc_1_4458
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0d0h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0cch]
        mov    al, byte ptr [ecx + eax*2]
        mov    byte ptr [ebp - 14h], al
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0d0h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0cch]
        mov    al, byte ptr [ecx + eax*2 + 1]
        mov    byte ptr [ebp - 18h], al
        mov    eax, 1
        mov    cl, byte ptr [ebp - 14h]
        shl    eax, cl
        mov    ecx, dword ptr [ebp - 1ch]
        and    ecx, 0ffffh
        or     eax, ecx
        mov    word ptr [ebp - 1ch], ax
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_439A
loc_1_38C1:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 19eh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_3A44
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        mov    dword ptr [ebp - 24h], eax
        jmp    NEAR PTR loc_1_39D8
loc_1_38ED:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8dh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 40h], 3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CED8
        mov    word ptr [ebp - 8], ax
        jmp    NEAR PTR loc_1_3A10
loc_1_3914:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8dh], 1
        jmp    NEAR PTR loc_1_3A10
loc_1_3925:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8dh], 2
        jmp    NEAR PTR loc_1_3A10
loc_1_3936:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8dh], 3
        jmp    NEAR PTR loc_1_3A10
loc_1_3947:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8dh], 4
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 40h], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CED8
        mov    word ptr [ebp - 8], ax
        jmp    NEAR PTR loc_1_3A10
loc_1_396E:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8dh], 5
        jmp    NEAR PTR loc_1_3A10
loc_1_397F:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8dh], 6
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 40h], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CED8
        mov    word ptr [ebp - 8], ax
        jmp    NEAR PTR loc_1_3A10
loc_1_39A6:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8dh], 7
        jmp    NEAR PTR loc_1_3A10
loc_1_39B7:
        jmp    NEAR PTR loc_1_3A10
loc_1_39BC:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
        db  0E9h, 03Dh, 000h, 000h, 000h, 0E9h, 038h, 000h, 000h, 000h
loc_1_39D8:
        cmp    dword ptr [ebp - 24h], 8
        ja     NEAR PTR loc_1_39BC
        mov    eax, dword ptr [ebp - 24h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_3A10:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_3A3B
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        cmp    eax, 8
        jge    NEAR PTR loc_1_3A3B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D0D8
        mov    word ptr [ebp - 8], ax
loc_1_3A3B:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
loc_1_3A44:
        jmp    NEAR PTR loc_1_43E9
loc_1_3A49:
        jmp    NEAR PTR loc_1_43E9
loc_1_3A4E:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 19eh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_1_3A64
loc_1_3A64:
        jmp    NEAR PTR loc_1_43E9
loc_1_3A69:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        test   al, 4
        je     NEAR PTR loc_1_3AA8
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A07D
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_3AA8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_3AA8:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        test   al, 2
        je     NEAR PTR loc_1_3AE7
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A07D
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_3AE7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_3AE7:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        test   al, 1
        je     NEAR PTR loc_1_3B37
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 5
        je     NEAR PTR loc_1_3B37
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A07D
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_3B37
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_3B37:
        jmp    NEAR PTR loc_1_43E9
loc_1_3B3C:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        test   al, 4
        je     NEAR PTR loc_1_3BD7
        mov    eax, dword ptr [ebp - 1ch]
        and    eax, 0ffffh
        or     eax, 800h
        mov    word ptr [ebp - 1ch], ax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 186h], 0
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A07D
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_3B98
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_3B98:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_3BAD
        jmp    NEAR PTR loc_1_43E9
loc_1_3BAD:
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A1D0
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_3BD7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_3BD7:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        test   al, 2
        je     NEAR PTR loc_1_3C61
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 184h], 0
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A07D
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_3C22
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_3C22:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_3C37
        jmp    NEAR PTR loc_1_43E9
loc_1_3C37:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A1D0
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_3C61
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_3C61:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        test   al, 1
        je     NEAR PTR loc_1_3CE6
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 182h], 0
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A07D
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_3CAC
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_3CAC:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_3CE6
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A1D0
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_3CE6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_3CE6:
        jmp    NEAR PTR loc_1_43E9
loc_1_3CEB:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 199h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0bah], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A250
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_3D31
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_3D31:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        test   al, 80h
        je     NEAR PTR loc_1_3D7D
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9103
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_3D68
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_3D68:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 51h], 0
        je     NEAR PTR loc_1_3D7D
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 51h]
        inc    dword ptr [eax]
loc_1_3D7D:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        test   al, 40h
        je     NEAR PTR loc_1_3DC9
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_917D
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_3DB4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_3DB4:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 51h], 0
        je     NEAR PTR loc_1_3DC9
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 51h]
        inc    dword ptr [eax]
loc_1_3DC9:
        jmp    NEAR PTR loc_1_43E9
loc_1_3DCE:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 199h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0bah], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A250
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_3E14
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_3E14:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        test   al, 80h
        je     NEAR PTR loc_1_3E60
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9103
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_3E4B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_3E4B:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 51h], 0
        je     NEAR PTR loc_1_3E60
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 51h]
        inc    dword ptr [eax]
loc_1_3E60:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        test   al, 40h
        je     NEAR PTR loc_1_3EAC
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_917D
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_3E97
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_3E97:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 51h], 0
        je     NEAR PTR loc_1_3EAC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 51h]
        inc    dword ptr [eax]
loc_1_3EAC:
        jmp    NEAR PTR loc_1_43E9
loc_1_3EB1:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 199h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0bah], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A250
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_3EF7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_3EF7:
        jmp    NEAR PTR loc_1_43E9
loc_1_3EFC:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 199h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0bah], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_A250
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        cmp    eax, 8
        jne    NEAR PTR loc_1_3F42
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_3F42:
        jmp    NEAR PTR loc_1_43E9
loc_1_3F47:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        test   al, 40h
        je     NEAR PTR loc_1_3F69
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 5dh], 0
        je     NEAR PTR loc_1_3F69
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5dh]
        inc    dword ptr [eax]
loc_1_3F69:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        test   al, 80h
        je     NEAR PTR loc_1_3FCA
        mov    eax, dword ptr [ebp + 8]
        inc    byte ptr [eax + 199h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 199h]
        and    eax, 0ffh
        cmp    eax, 3
        jl     NEAR PTR loc_1_3FB5
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 199h], 0
        mov    eax, dword ptr [ebp - 1ch]
        and    eax, 0f7ffh
        mov    word ptr [ebp - 1ch], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_B2C4
loc_1_3FB5:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 59h], 0
        je     NEAR PTR loc_1_3FCA
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 59h]
        inc    dword ptr [eax]
loc_1_3FCA:
        jmp    NEAR PTR loc_1_43E9
loc_1_3FCF:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_4006
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    word ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        jmp    NEAR PTR loc_1_41F8
loc_1_4006:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        cmp    eax, 7
        jne    NEAR PTR loc_1_41F8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d2h]
        mov    ax, word ptr [eax + 0ah]
        and    eax, 0ffffh
        cmp    eax, 5412h
        jne    NEAR PTR loc_1_41F8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 20h]
        and    eax, 0ffffh
        test   al, 1
        je     NEAR PTR loc_1_4061
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9B60
loc_1_4061:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 20h]
        and    eax, 0ffffh
        test   al, 2
        je     NEAR PTR loc_1_40CF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 8fh], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8dh]
        and    eax, 0ffffh
        or     eax, 8000h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 8dh], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D0D8
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
loc_1_40CF:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 20h]
        and    eax, 0ffffh
        test   al, 4
        je     NEAR PTR loc_1_41A6
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 89h], 0
        je     NEAR PTR loc_1_41A6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 18h]
        and    eax, 0ffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0e4h]
        mov    cx, word ptr [ecx + 18h]
        and    ecx, 0ffffh
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 89h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 1ah]
        and    eax, 0ffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0e4h]
        mov    cx, word ptr [ecx + 1ah]
        and    ecx, 0ffffh
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 89h]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 1ch]
        and    eax, 0ffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0e4h]
        mov    cx, word ptr [ecx + 1ch]
        and    ecx, 0ffffh
        sar    ecx, 8
        or     eax, ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 89h]
        mov    word ptr [ecx + 4], ax
loc_1_41A6:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_1_41F8
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e4h]
        mov    ax, word ptr [eax + 20h]
        and    eax, 0ffffh
        test   al, 8
        je     NEAR PTR loc_1_41F8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_BC8F
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_41F8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_41F8:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0e0h], 0
        jmp    NEAR PTR loc_1_43E9
loc_1_4209:
        xor    eax, eax
        mov    al, byte ptr [ebp - 18h]
        mov    dword ptr [ebp - 28h], eax
        jmp    NEAR PTR loc_1_42E9
loc_1_4216:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8dh], 8
        jmp    NEAR PTR loc_1_4319
loc_1_4227:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8dh], 9
        jmp    NEAR PTR loc_1_4319
loc_1_4238:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8dh], 0ah
        jmp    NEAR PTR loc_1_4319
loc_1_4249:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8dh], 0bh
        jmp    NEAR PTR loc_1_4319
loc_1_425A:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8dh], 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 19ah]
        and    eax, 0ffffh
        test   ah, 80h
        je     NEAR PTR loc_1_42A6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9201
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_42A6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_42A6:
        jmp    NEAR PTR loc_1_4319
loc_1_42AB:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8dh], 0dh
        jmp    NEAR PTR loc_1_4319
loc_1_42BC:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8dh], 0eh
        jmp    NEAR PTR loc_1_4319
loc_1_42CD:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
        db  0E9h, 035h, 000h, 000h, 000h, 0E9h, 030h, 000h, 000h, 000h
loc_1_42E9:
        cmp    dword ptr [ebp - 28h], 6
        ja     NEAR PTR loc_1_42CD
        mov    eax, dword ptr [ebp - 28h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4319:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 8dh]
        and    eax, 7fffh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 8dh], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_D0D8
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        jmp    NEAR PTR loc_1_43E9
loc_1_4356:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 40h], 2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_CED8
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_67FC
        jmp    NEAR PTR loc_1_43E9
loc_1_4383:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
        db  0E9h, 04Fh, 000h, 000h, 000h
loc_1_439A:
        dec    dword ptr [ebp - 20h]
        cmp    dword ptr [ebp - 20h], 0dh
        ja     NEAR PTR loc_1_4383
        mov    eax, dword ptr [ebp - 20h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_43E9:
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0d0h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0cch]
        mov    al, byte ptr [ecx + eax*2]
        and    eax, 0ffh
        or     al, 0f0h
        mov    ecx, dword ptr [ebp + 8]
        movsx  ecx, word ptr [ecx + 0d0h]
        mov    edx, dword ptr [ebp + 8]
        mov    edx, dword ptr [edx + 0cch]
        mov    byte ptr [edx + ecx*2], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0d0h]
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 0d0h]
        mov    eax, dword ptr [ebp + 8]
        movsx  eax, word ptr [eax + 0d0h]
        cmp    eax, 20h
        jne    NEAR PTR loc_1_4453
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0d0h], 0
loc_1_4453:
        jmp    NEAR PTR loc_1_3834
loc_1_4458:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9D17
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        test   ah, ah
        jne    NEAR PTR loc_1_44A1
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_9D8E
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_1_44A1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        mov    ax, 8
        jmp    NEAR PTR loc_1_44B2
loc_1_44A1:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_682C
        xor    ax, ax
        jmp    NEAR PTR loc_1_44B2
loc_1_44B2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        db  000h, 000h, 000h
sub_1_44BC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cmp    byte ptr [ebp + 14h], 0
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 18h]
        mov    dword ptr [edi], 0
        je     SHORT loc_1_4537
        cmp    byte ptr [ebp + 10h], 0
        je     SHORT loc_1_44E0
        mov    dword ptr [edi], 8
loc_1_44E0:
        mov    esi, dword ptr [ebp + 0ch]
        lea    edx, [esi + 0eh]
        in     al, dx
        test   al, 0f8h
        jne    NEAR PTR loc_1_465E
        mov    eax, dword ptr [edi]
        or     eax, 95h
        cmp    byte ptr [ebp + 10h], 0
        mov    dword ptr [edi], eax
        je     SHORT loc_1_4505
        or     eax, 800000h
        jmp    SHORT loc_1_450A
loc_1_4505:
        or     eax, 400040h
loc_1_450A:
        lea    edx, [esi + 1]
        mov    dword ptr [edi], eax
        in     al, dx
        mov    ecx, dword ptr [edi]
        and    al, 0ch
        out    dx, al
        in     al, dx
        or     ecx, 4250000h
        test   al, 1
        mov    dword ptr [edi], ecx
        je     NEAR PTR loc_1_465E
        or     ecx, 80000h
        mov    dword ptr [edi], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_4537:
        mov    esi, dword ptr [ebp + 0ch]
        xor    ebx, ebx
        lea    edx, [esi + 0eh]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        and    al, 1eh
        shr    al, 1
        mov    bl, al
        test   ebx, ebx
        je     NEAR PTR loc_1_465E
        cmp    byte ptr [ebp + 10h], 0
        je     SHORT loc_1_455E
        mov    dword ptr [edi], 8
loc_1_455E:
        push   edi
        mov    al, byte ptr [ebp + 10h]
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        call   sub_1_466C
        test   byte ptr [ebp - 4], 1
        je     SHORT loc_1_4578
        or     dword ptr [edi], 2
        jmp    SHORT loc_1_4585
loc_1_4578:
        cmp    ebx, 1
        je     SHORT loc_1_4582
        or     dword ptr [edi], 3
        jmp    SHORT loc_1_4585
loc_1_4582:
        or     dword ptr [edi], 1
loc_1_4585:
        cmp    ebx, 2
        jb     SHORT loc_1_45AA
        test   byte ptr [ebp - 4], 20h
        je     SHORT loc_1_45AA
        mov    ecx, dword ptr [edi]
        mov    eax, ecx
        and    al, 12h
        cmp    al, 12h
        je     SHORT loc_1_45A2
        mov    eax, ecx
        and    al, 13h
        cmp    al, 13h
        jne    SHORT loc_1_45AA
loc_1_45A2:
        or     ecx, 200000h
        mov    dword ptr [edi], ecx
loc_1_45AA:
        cmp    ebx, 3
        jb     SHORT loc_1_45F6
        cmp    byte ptr [ebp + 10h], 0
        jne    SHORT loc_1_45F0
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        and    dword ptr [edi], 0ffd0ffb0h
        call   sub_1_4EE3
        test   eax, eax
        je     SHORT loc_1_45D1
        or     dword ptr [edi], 1000000h
        jmp    SHORT loc_1_45D7
loc_1_45D1:
        or     dword ptr [edi], 400000h
loc_1_45D7:
        lea    eax, [ebp - 4]
        mov    cl, byte ptr [ebp + 10h]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        push   esi
        push   edx
        call   sub_1_484C
        mov    ecx, dword ptr [ebp - 4]
        or     dword ptr [edi], ecx
        jmp    SHORT loc_1_4606
loc_1_45F0:
        or     dword ptr [edi], 800000h
loc_1_45F6:
        push   edi
        mov    al, byte ptr [ebp + 10h]
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   esi
        push   ecx
        call   sub_1_4C32
loc_1_4606:
        cmp    ebx, 4
        jb     SHORT loc_1_464A
        mov    ecx, dword ptr [edi]
        or     ecx, 20h
        test   ecx, 1000000h
        mov    dword ptr [edi], ecx
        je     SHORT loc_1_464A
        lea    edx, [esi + 7]
        and    ecx, 0fc3fffffh
        in     al, dx
        shr    al, 4
        mov    dword ptr [edi], ecx
        cmp    al, 2
        jne    SHORT loc_1_463E
        or     ecx, 9000000h
        mov    dword ptr [edi], ecx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_463E:
        cmp    al, 3
        jne    SHORT loc_1_464A
        or     ecx, 1000000h
        mov    dword ptr [edi], ecx
loc_1_464A:
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4E39
        test   al, al
        je     SHORT loc_1_465E
        or     dword ptr [edi], 100000h
loc_1_465E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
sub_1_466C:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4EE3
        test   eax, eax
        je     NEAR PTR loc_1_4699
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 10h
        jmp    NEAR PTR loc_1_4845
loc_1_4699:
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 7
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 7
        mov    al, 35h
        out    dx, al
        mov    edx, dword ptr [ebp + 0ch]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 7
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 35h
        jne    NEAR PTR loc_1_46F7
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 7
        mov    al, 3ah
        out    dx, al
        mov    edx, dword ptr [ebp + 0ch]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 7
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 3ah
        jne    NEAR PTR loc_1_46F7
        mov    byte ptr [ebp - 0ch], 1
loc_1_46F7:
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 7
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        test   eax, eax
        je     NEAR PTR loc_1_4726
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        test   eax, eax
        je     NEAR PTR loc_1_4721
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 10h
loc_1_4721:
        jmp    NEAR PTR loc_1_4845
loc_1_4726:
        mov    dword ptr [ebp - 10h], 1
        jmp    NEAR PTR loc_1_4735
loc_1_4732:
        inc    dword ptr [ebp - 10h]
loc_1_4735:
        cmp    dword ptr [ebp - 10h], 6
        jae    NEAR PTR loc_1_4775
        mov    eax, dword ptr [ebp - 10h]
        add    eax, dword ptr [ebp + 0ch]
        mov    edx, eax
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp + 0ch]
        lea    edx, [ecx + edx + 8]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        cmp    eax, ecx
        je     NEAR PTR loc_1_4770
        jmp    NEAR PTR loc_1_4775
loc_1_4770:
        jmp    NEAR PTR loc_1_4732
loc_1_4775:
        cmp    dword ptr [ebp - 10h], 6
        jne    NEAR PTR loc_1_4799
        mov    eax, dword ptr [ebp - 10h]
        add    eax, dword ptr [ebp + 0ch]
        mov    edx, eax
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp + 0ch]
        lea    edx, [ecx + edx + 8]
        in     al, dx
        mov    byte ptr [ebp - 8], al
loc_1_4799:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_47B0
        jmp    NEAR PTR loc_1_4845
loc_1_47B0:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        test   eax, eax
        je     NEAR PTR loc_1_47C8
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 10h
        jmp    NEAR PTR loc_1_4845
loc_1_47C8:
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 1
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ebp + 0ch]
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        xor    al, 1
        inc    edx
        out    dx, al
        mov    edx, dword ptr [ebp + 0ch]
        in     al, dx
        mov    byte ptr [ebp - 14h], al
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 14h], al
        mov    al, byte ptr [ebp - 14h]
        and    eax, 1
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        cmp    eax, ecx
        jne    NEAR PTR loc_1_483D
        mov    edx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [ebp - 8]
        and    eax, 0feh
        inc    edx
        out    dx, al
        mov    ecx, dword ptr [ebp + 14h]
        or     dword ptr [ecx], 40h
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 1
        je     NEAR PTR loc_1_4838
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 80000h
loc_1_4838:
        jmp    NEAR PTR loc_1_4845
loc_1_483D:
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        mov    al, byte ptr [ebp - 8]
        out    dx, al
loc_1_4845:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_484C:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4EE3
        test   eax, eax
        je     NEAR PTR loc_1_489E
        mov    byte ptr [ebp - 8], 4ah
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        mov    al, byte ptr [ebp - 8]
        out    dx, al
loc_1_487F:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 40h
        je     NEAR PTR loc_1_4899
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        jmp    NEAR PTR loc_1_487F
loc_1_4899:
        jmp    NEAR PTR loc_1_492D
loc_1_489E:
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0ch
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     al, 2
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 3
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0fh
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     al, 0a0h
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 3
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0ch
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     al, 12h
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
loc_1_4913:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 10h
        je     NEAR PTR loc_1_492D
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        jmp    NEAR PTR loc_1_4913
loc_1_492D:
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        test   eax, eax
        je     NEAR PTR loc_1_4940
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 8
loc_1_4940:
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 9
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0c0h
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 40h
        je     NEAR PTR loc_1_496B
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 80h
loc_1_496B:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 80h
        je     NEAR PTR loc_1_4981
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 100h
loc_1_4981:
        mov    al, byte ptr [ebp - 8]
        and    eax, 18h
        cmp    al, 8
        jne    NEAR PTR loc_1_49ED
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 40h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4EE3
        test   eax, eax
        je     NEAR PTR loc_1_49CF
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 4
        in     al, dx
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 10h
        je     NEAR PTR loc_1_49CA
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 80000h
loc_1_49CA:
        jmp    NEAR PTR loc_1_49ED
loc_1_49CF:
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 1
        je     NEAR PTR loc_1_49ED
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 80000h
loc_1_49ED:
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 8
        in     al, dx
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 7
        jne    NEAR PTR loc_1_4A0F
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 1
        jmp    NEAR PTR loc_1_4A47
loc_1_4A0F:
        mov    al, byte ptr [ebp - 8]
        and    eax, 7
        cmp    al, 2
        jne    NEAR PTR loc_1_4A28
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 3
        jmp    NEAR PTR loc_1_4A47
loc_1_4A28:
        mov    al, byte ptr [ebp - 8]
        and    eax, 7
        cmp    al, 3
        jne    NEAR PTR loc_1_4A41
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 4
        jmp    NEAR PTR loc_1_4A47
loc_1_4A41:
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 2
loc_1_4A47:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 18h
        je     NEAR PTR loc_1_4A5D
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 200000h
loc_1_4A5D:
        mov    al, byte ptr [ebp - 8]
        and    eax, 0e0h
        cmp    al, 40h
        jne    NEAR PTR loc_1_4A7B
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 20000h
        jmp    NEAR PTR loc_1_4B3F
loc_1_4A7B:
        mov    al, byte ptr [ebp - 8]
        and    eax, 0e0h
        cmp    al, 60h
        jne    NEAR PTR loc_1_4ACE
        mov    eax, dword ptr [ebp + 14h]
        test   byte ptr [eax], 40h
        jne    NEAR PTR loc_1_4AA5
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 30000h
        jmp    NEAR PTR loc_1_4AC9
loc_1_4AA5:
        mov    eax, dword ptr [ebp + 14h]
        test   byte ptr [eax + 2], 8
        jne    NEAR PTR loc_1_4AC0
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 20000h
        jmp    NEAR PTR loc_1_4AC9
loc_1_4AC0:
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 30000h
loc_1_4AC9:
        jmp    NEAR PTR loc_1_4B3F
loc_1_4ACE:
        mov    al, byte ptr [ebp - 8]
        and    eax, 0e0h
        cmp    al, 80h
        jne    NEAR PTR loc_1_4AEC
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 40000h
        jmp    NEAR PTR loc_1_4B3F
loc_1_4AEC:
        mov    al, byte ptr [ebp - 8]
        and    eax, 0e0h
        cmp    al, 0a0h
        jne    NEAR PTR loc_1_4B3F
        mov    eax, dword ptr [ebp + 14h]
        test   byte ptr [eax], 40h
        jne    NEAR PTR loc_1_4B16
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 50000h
        jmp    NEAR PTR loc_1_4B3A
loc_1_4B16:
        mov    eax, dword ptr [ebp + 14h]
        test   byte ptr [eax + 2], 8
        jne    NEAR PTR loc_1_4B31
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 40000h
        jmp    NEAR PTR loc_1_4B3A
loc_1_4B31:
        mov    eax, dword ptr [ebp + 14h]
        or     dword ptr [eax], 50000h
loc_1_4B3A:
        jmp    NEAR PTR loc_1_4B3F
loc_1_4B3F:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4EE3
        test   eax, eax
        je     NEAR PTR loc_1_4B9C
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 4
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 43h
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 4
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    byte ptr [ebp - 8], 46h
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        mov    al, byte ptr [ebp - 8]
        out    dx, al
loc_1_4B7D:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 40h
        je     NEAR PTR loc_1_4B97
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        jmp    NEAR PTR loc_1_4B7D
loc_1_4B97:
        jmp    NEAR PTR loc_1_4C2B
loc_1_4B9C:
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0ch
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     al, 2
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 3
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0fh
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     al, 80h
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 3
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 0ch
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     al, 10h
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
loc_1_4C11:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   al, 10h
        je     NEAR PTR loc_1_4C2B
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 8], al
        jmp    NEAR PTR loc_1_4C11
loc_1_4C2B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_4C32:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 14h], 2
        jae    NEAR PTR loc_1_4CE0
        xor    eax, eax
        mov    al, byte ptr [ebp + 10h]
        test   eax, eax
        je     NEAR PTR loc_1_4C60
        mov    eax, dword ptr [ebp + 18h]
        or     dword ptr [eax], 30000h
        jmp    NEAR PTR loc_1_4CDB
loc_1_4C60:
        mov    eax, dword ptr [ebp + 18h]
        test   byte ptr [eax], 40h
        je     NEAR PTR loc_1_4C95
        mov    eax, dword ptr [ebp + 18h]
        test   byte ptr [eax + 2], 8
        je     NEAR PTR loc_1_4C87
        mov    eax, dword ptr [ebp + 18h]
        or     dword ptr [eax], 30000h
        jmp    NEAR PTR loc_1_4C90
loc_1_4C87:
        mov    eax, dword ptr [ebp + 18h]
        or     dword ptr [eax], 20000h
loc_1_4C90:
        jmp    NEAR PTR loc_1_4CDB
loc_1_4C95:
        mov    eax, dword ptr [ebp + 18h]
        test   byte ptr [eax], 10h
        jne    NEAR PTR loc_1_4CAF
        mov    eax, dword ptr [ebp + 18h]
        or     dword ptr [eax], 20000h
        jmp    NEAR PTR loc_1_4CDB
loc_1_4CAF:
        mov    edx, dword ptr [ebp + 0ch]
        inc    edx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 8
        je     NEAR PTR loc_1_4CD2
        mov    eax, dword ptr [ebp + 18h]
        or     dword ptr [eax], 40000h
        jmp    NEAR PTR loc_1_4CDB
loc_1_4CD2:
        mov    eax, dword ptr [ebp + 18h]
        or     dword ptr [eax], 20000h
loc_1_4CDB:
        jmp    NEAR PTR loc_1_4E32
loc_1_4CE0:
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 0eh
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp + 18h]
        test   byte ptr [eax], 8
        je     NEAR PTR loc_1_4D1F
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 40h
        je     NEAR PTR loc_1_4D11
        mov    eax, dword ptr [ebp + 18h]
        or     dword ptr [eax], 50000h
        jmp    NEAR PTR loc_1_4D1A
loc_1_4D11:
        mov    eax, dword ptr [ebp + 18h]
        or     dword ptr [eax], 30000h
loc_1_4D1A:
        jmp    NEAR PTR loc_1_4E32
loc_1_4D1F:
        mov    eax, dword ptr [ebp + 18h]
        mov    eax, dword ptr [eax]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_4DBE
loc_1_4D31:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 40h
        je     NEAR PTR loc_1_4D4C
        mov    eax, dword ptr [ebp + 18h]
        or     dword ptr [eax], 40000h
        jmp    NEAR PTR loc_1_4D55
loc_1_4D4C:
        mov    eax, dword ptr [ebp + 18h]
        or     dword ptr [eax], 20000h
loc_1_4D55:
        jmp    NEAR PTR loc_1_4E32
loc_1_4D5A:
        mov    eax, dword ptr [ebp + 18h]
        test   byte ptr [eax + 2], 8
        je     NEAR PTR loc_1_4D90
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 40h
        je     NEAR PTR loc_1_4D82
        mov    eax, dword ptr [ebp + 18h]
        or     dword ptr [eax], 50000h
        jmp    NEAR PTR loc_1_4D8B
loc_1_4D82:
        mov    eax, dword ptr [ebp + 18h]
        or     dword ptr [eax], 30000h
loc_1_4D8B:
        jmp    NEAR PTR loc_1_4DB4
loc_1_4D90:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 40h
        je     NEAR PTR loc_1_4DAB
        mov    eax, dword ptr [ebp + 18h]
        or     dword ptr [eax], 40000h
        jmp    NEAR PTR loc_1_4DB4
loc_1_4DAB:
        mov    eax, dword ptr [ebp + 18h]
        or     dword ptr [eax], 20000h
loc_1_4DB4:
        jmp    NEAR PTR loc_1_4E32
loc_1_4DB9:
        jmp    NEAR PTR loc_1_4E32
loc_1_4DBE:
        dec    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], 41h
        ja     NEAR PTR loc_1_4E32
        mov    eax, dword ptr [ebp - 8]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
        db  003h, 001h, 001h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
        db  003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
        db  003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h, 003h
        db  003h, 002h
loc_1_4E32:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
sub_1_4E39:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 10h
        in     al, dx
        mov    byte ptr [ebp - 0ch], al
        mov    al, byte ptr [ebp - 0ch]
        and    eax, 0fbh
        mov    byte ptr [ebp - 0ch], al
        mov    edx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [ebp - 0ch]
        and    eax, 3fh
        or     al, 80h
        add    edx, 10h
        out    dx, al
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 1dh
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [ebp - 0ch]
        and    eax, 3fh
        add    edx, 10h
        out    dx, al
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 1dh
        mov    al, 18h
        out    dx, al
        mov    edx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [ebp - 0ch]
        and    eax, 3fh
        or     al, 80h
        add    edx, 10h
        out    dx, al
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 1dh
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [ebp - 0ch]
        and    eax, 3fh
        add    edx, 10h
        out    dx, al
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 1dh
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 10h
        mov    al, byte ptr [ebp - 0ch]
        out    dx, al
        mov    al, byte ptr [ebp - 8]
        and    eax, 18h
        cmp    al, 18h
        je     NEAR PTR loc_1_4ED5
        mov    al, 1
        jmp    NEAR PTR loc_1_4ED7
loc_1_4ED5:
        xor    al, al
loc_1_4ED7:
        jmp    NEAR PTR loc_1_4EDC
loc_1_4EDC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_4EE3:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 4
        in     al, dx
        mov    byte ptr [ebp - 10h], al
        mov    al, byte ptr [ebp - 10h]
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 10h]
        and    eax, 0c3h
        mov    byte ptr [ebp - 10h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        or     al, 80h
        mov    byte ptr [ebp - 10h], al
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 4
        mov    al, byte ptr [ebp - 10h]
        out    dx, al
        mov    byte ptr [ebp - 0ch], 0
        mov    al, byte ptr [ebp - 0ch]
        mov    byte ptr [ebp - 8], al
        mov    byte ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_4F31
loc_1_4F2E:
        inc    byte ptr [ebp - 10h]
loc_1_4F31:
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 6
        jge    NEAR PTR loc_1_4FA4
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        lea    edx, [eax + ecx + 8]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 4
        in     al, dx
        mov    byte ptr [ebp - 0ch], al
        mov    al, byte ptr [ebp - 0ch]
        and    eax, 0c3h
        mov    byte ptr [ebp - 0ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        xor    al, 80h
        mov    byte ptr [ebp - 0ch], al
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 4
        mov    al, byte ptr [ebp - 0ch]
        out    dx, al
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 10h]
        mov    edx, dword ptr [ebp + 0ch]
        lea    edx, [ecx + edx + 8]
        in     al, dx
        mov    byte ptr [ebp - 0ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        cmp    eax, ecx
        je     NEAR PTR loc_1_4F9F
        jmp    NEAR PTR loc_1_4FA4
loc_1_4F9F:
        jmp    NEAR PTR loc_1_4F2E
loc_1_4FA4:
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        cmp    eax, 6
        jne    NEAR PTR loc_1_4FC3
        mov    edx, dword ptr [ebp + 0ch]
        add    edx, 4
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        xor    eax, eax
        jmp    NEAR PTR loc_1_4FDC
loc_1_4FC3:
        mov    edx, dword ptr [ebp + 0ch]
        mov    al, byte ptr [ebp - 4]
        and    eax, 0c3h
        add    edx, 4
        out    dx, al
        mov    eax, 1
        jmp    NEAR PTR loc_1_4FDC
loc_1_4FDC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
_LTEXT ends
        end
