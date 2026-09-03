; smc8000w_lcode_01.asm -- SMC8000W object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

        db  055h, 033h, 0C9h, 08Bh, 0ECh, 056h, 08Bh, 045h, 008h, 08Bh, 090h, 05Bh, 001h, 000h, 000h, 085h
        db  0D2h, 074h, 010h, 08Bh, 072h, 018h, 03Bh, 0CEh, 073h, 002h, 08Bh, 0CEh, 08Bh, 052h, 00Ch, 085h
        db  0D2h, 075h, 0F0h, 085h, 0C9h, 075h, 005h, 0B9h, 0EEh, 000h, 000h, 000h, 050h, 089h, 088h, 0F7h
        db  001h, 000h, 000h, 083h, 0C1h, 01Dh, 0C1h, 0E9h, 004h, 066h, 089h, 048h, 03Ch, 0E8h, 03Eh, 01Bh
        db  000h, 000h, 05Eh, 05Dh, 0C2h, 004h, 000h, 08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0050:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 20h], 0
        mov    esi, dword ptr [ebx + 8]
        je     SHORT loc_1_006E
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_006E:
        push   0
        inc    dword ptr [esi + 15fh]
        inc    dword ptr [ebx + 1ch]
        mov    eax, dword ptr [ebx + 14h]
        mov    ecx, dword ptr [esi + 147h]
        push   eax
        push   ecx
        db  0E8h, 047h, 02Eh, 000h, 000h
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_1_00AE
        mov    eax, dword ptr [ebx + 1ch]
        cmp    eax, 2
        je     SHORT loc_1_00A6
        mov    byte ptr [ebx + 20h], 1
        sub    dword ptr [ebx + 1ch], 2
        mov    edi, 103h
        jmp    SHORT loc_1_00D4
loc_1_00A6:
        sub    eax, 2
        mov    dword ptr [ebx + 1ch], eax
        jmp    SHORT loc_1_00D4
loc_1_00AE:
        cmp    edi, 103h
        jne    SHORT loc_1_00C0
        mov    byte ptr [ebx + 20h], 1
        sub    dword ptr [ebx + 1ch], 2
        jmp    SHORT loc_1_00D4
loc_1_00C0:
        cmp    edi, 0c001000eh
        jne    SHORT loc_1_00D1
        mov    byte ptr [ebx + 20h], 1
        mov    edi, 103h
loc_1_00D1:
        dec    dword ptr [ebx + 1ch]
loc_1_00D4:
        cmp    dword ptr [ebx + 1ch], 0
        jne    SHORT loc_1_0147
        mov    eax, dword ptr [esi + 15bh]
        cmp    eax, ebx
        jne    SHORT loc_1_00EF
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esi + 15bh], eax
        jmp    SHORT loc_1_0102
loc_1_00EF:
        cmp    dword ptr [eax + 0ch], ebx
        je     SHORT loc_1_00FC
loc_1_00F4:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], ebx
        jne    SHORT loc_1_00F4
loc_1_00FC:
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_0102:
        mov    eax, dword ptr [esi + 1f7h]
        cmp    dword ptr [ebx + 18h], eax
        jne    SHORT loc_1_0113
        push   esi
        call   0
loc_1_0113:
        cmp    dword ptr [esi + 15bh], 0
        jne    SHORT loc_1_014C
        push   esi
        lea    eax, [esi + 0d5h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_1C62
        test   al, al
        jne    SHORT loc_1_014C
        push   0
        mov    eax, dword ptr [esi + 52h]
        push   0aa000003h
        push   eax
        call   sub_1_1D40
        add    esp, 0ch
        jmp    SHORT loc_1_014C
loc_1_0147:
        mov    edi, 103h
loc_1_014C:
        mov    eax, dword ptr [esi + 15fh]
        dec    eax
        mov    dword ptr [esi + 15fh], eax
        jne    SHORT loc_1_0194
        cmp    byte ptr [esi + 1edh], 0
        je     SHORT loc_1_016F
        lea    eax, [esi + 307h]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_0183
loc_1_016F:
        cmp    dword ptr [esi + 177h], 0
        je     SHORT loc_1_0194
        lea    eax, [esi + 307h]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_0194
loc_1_0183:
        push   0
        add    esi, 1fbh
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_1C4A
loc_1_0194:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_01A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 8]
        inc    dword ptr [edi + 1ch]
        inc    dword ptr [esi + 15fh]
        cmp    byte ptr [edi + 20h], 0
        je     SHORT loc_1_01C7
        mov    dword ptr [ebp - 4], 0c0010002h
        jmp    SHORT loc_1_0209
loc_1_01C7:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 10h]
        test   ecx, ecx
        jne    SHORT loc_1_01DE
        push   eax
        push   edi
        push   esi
        call   sub_1_0680
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_0209
loc_1_01DE:
        cmp    ecx, 1
        jne    SHORT loc_1_0202
        cmp    byte ptr [esi + 1ech], 0
        je     SHORT loc_1_01F5
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    SHORT loc_1_0209
loc_1_01F5:
        push   eax
        push   edi
        push   esi
        call   sub_1_06D0
        mov    dword ptr [ebp - 4], eax
        jmp    SHORT loc_1_0209
loc_1_0202:
        mov    dword ptr [ebp - 4], 10001h
loc_1_0209:
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_0279
        mov    eax, dword ptr [ebx + 15bh]
        cmp    edi, eax
        jne    SHORT loc_1_0226
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 15bh], eax
        jmp    SHORT loc_1_0239
loc_1_0226:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_0233
loc_1_022B:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_022B
loc_1_0233:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_0239:
        mov    eax, dword ptr [ebx + 1f7h]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_024A
        push   ebx
        call   0
loc_1_024A:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1BAC
        push   0
        push   75h
        push   edi
        call   sub_1_1C74
        cmp    dword ptr [ebx + 15bh], 0
        jne    SHORT loc_1_0279
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 0d5h
        push   ebx
        call   sub_1_1C62
loc_1_0279:
        mov    eax, dword ptr [esi + 15fh]
        dec    eax
        mov    dword ptr [esi + 15fh], eax
        jne    SHORT loc_1_02C1
        cmp    byte ptr [esi + 1edh], 0
        je     SHORT loc_1_029C
        lea    eax, [esi + 307h]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_02B0
loc_1_029C:
        cmp    dword ptr [esi + 177h], 0
        je     SHORT loc_1_02C1
        lea    eax, [esi + 307h]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_02C1
loc_1_02B0:
        push   0
        add    esi, 1fbh
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_1C4A
loc_1_02C1:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
sub_1_02D0:
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
        ja     SHORT loc_1_030D
        je     NEAR PTR loc_1_04C4
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_032E
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_030D:
        cmp    eax, 1010102h
        je     NEAR PTR loc_1_04E3
        cmp    eax, 1010103h
        je     NEAR PTR loc_1_0502
        cmp    eax, 1010104h
        je     NEAR PTR loc_1_05D9
loc_1_032E:
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_05E5
loc_1_033A:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_034F
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 5ch
        jmp    NEAR PTR loc_1_05E5
loc_1_034F:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 7ch
        jmp    NEAR PTR loc_1_05E5
loc_1_035E:
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 14h], 4
        cmp    byte ptr [ebx + 1ech], 0
        jne    SHORT loc_1_0378
        mov    dword ptr [ebp - 14h], 0
loc_1_0378:
        lea    esi, [ebp - 14h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_05E5
loc_1_0385:
        lea    esi, [ebp - 18h]
        mov    edi, 4
        jmp    NEAR PTR loc_1_05E5
loc_1_0392:
        mov    dword ptr [ebp - 8], 0eeh
        jmp    NEAR PTR loc_1_05E5
loc_1_039E:
        mov    dword ptr [ebp - 8], 5dch
        jmp    NEAR PTR loc_1_05E5
loc_1_03AA:
        mov    dword ptr [ebp - 8], 186a0h
        jmp    NEAR PTR loc_1_05E5
loc_1_03B6:
        mov    ebx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 3ah]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0cdh]
        imul   ecx, eax
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_05E5
loc_1_03D3:
        mov    ebx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 16h]
        xor    edx, edx
        mov    dx, word ptr [ebx + 3ah]
        xor    eax, eax
        shl    ecx, 0ah
        mov    ax, word ptr [ebx + 0cdh]
        imul   edx, eax
        sub    ecx, edx
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_05E5
loc_1_03FB:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 38h]
        jmp    NEAR PTR loc_1_05E2
loc_1_0409:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 38h]
        jmp    NEAR PTR loc_1_05E2
loc_1_0417:
        push   3
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 2ah
        lea    eax, [ebp - 8]
        push   ebx
        lea    esi, [ebp - 8]
        push   eax
        mov    edi, 4
        call   sub_1_2050
        and    dword ptr [ebp - 8], 0ffffff00h
        jmp    NEAR PTR loc_1_05E5
loc_1_043D:
        db  0BEh, 000h, 000h, 000h, 000h
        mov    edi, 0dh
        jmp    NEAR PTR loc_1_05E5
loc_1_044C:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_0466
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 147h]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_05E5
loc_1_0466:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 14h]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebp - 8], edx
        jmp    NEAR PTR loc_1_05E5
loc_1_0477:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_048B
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 1f7h]
        jmp    NEAR PTR loc_1_05E2
loc_1_048B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 8], ecx
        jmp    NEAR PTR loc_1_05E5
loc_1_0499:
        lea    esi, [ebp - 2]
        mov    edi, 2
        mov    word ptr [ebp - 2], 300h
        jmp    NEAR PTR loc_1_05E5
loc_1_04AC:
        mov    dword ptr [ebp - 8], 5eah
        jmp    NEAR PTR loc_1_05E5
loc_1_04B8:
        mov    dword ptr [ebp - 8], 0eh
        jmp    NEAR PTR loc_1_05E5
loc_1_04C4:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 2ah
        lea    eax, [ebp - 20h]
        push   ebx
        lea    esi, [ebp - 20h]
        push   eax
        mov    edi, 6
        call   sub_1_2050
        jmp    NEAR PTR loc_1_05E5
loc_1_04E3:
        push   6
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 24h
        lea    eax, [ebp - 20h]
        push   ebx
        lea    esi, [ebp - 20h]
        push   eax
        mov    edi, 6
        call   sub_1_2050
        jmp    NEAR PTR loc_1_05E5
loc_1_0502:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 5ch], 0
        je     SHORT loc_1_0517
        mov    dword ptr [ebp - 0ch], 0c00000bbh
        jmp    NEAR PTR loc_1_05E5
loc_1_0517:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_0574
        mov    ecx, dword ptr [ebx + 147h]
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [ebp - 10h], eax
        lea    ebx, [eax + eax*2]
        lea    edx, [ebx*2]
        cmp    dword ptr [ebp + 1ch], edx
        jae    SHORT loc_1_0549
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [eax], edx
        jmp    NEAR PTR loc_1_05E5
loc_1_0549:
        mov    eax, dword ptr [ebp + 18h]
        lea    edx, [ebp - 10h]
        push   eax
        mov    ebx, dword ptr [ebp + 1ch]
        push   edx
        lea    eax, [ebp - 0ch]
        push   ebx
        push   ecx
        push   eax
        db  0E8h, 091h, 02Eh, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 24h]
        lea    eax, [eax + eax*2]
        add    eax, eax
        cmp    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ecx], eax
        je     SHORT loc_1_05EB
        jmp    SHORT loc_1_05E5
loc_1_0574:
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebx + 147h]
        mov    ecx, dword ptr [eax + 14h]
        push   ecx
        push   edx
        db  0E8h, 069h, 02Dh, 000h, 000h
        lea    edx, [eax + eax*2]
        mov    dword ptr [ebp - 10h], eax
        lea    ecx, [edx*2]
        cmp    dword ptr [ebp + 1ch], ecx
        jae    SHORT loc_1_05A7
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], 0c0010014h
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_1_05E5
loc_1_05A7:
        mov    eax, dword ptr [ebp + 18h]
        lea    ecx, [ebp - 10h]
        push   eax
        mov    edx, dword ptr [ebp + 1ch]
        push   ecx
        mov    eax, dword ptr [ebp + 0ch]
        push   edx
        mov    ecx, dword ptr [eax + 14h]
        push   ecx
        mov    edx, dword ptr [ebx + 147h]
        push   edx
        lea    eax, [ebp - 0ch]
        push   eax
        db  0E8h, 066h, 02Dh, 000h, 000h
        mov    ecx, dword ptr [ebp - 10h]
        lea    eax, [ecx + ecx*2]
        mov    ecx, dword ptr [ebp + 24h]
        add    eax, eax
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_05E5
loc_1_05D9:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 163h]
loc_1_05E2:
        mov    dword ptr [ebp - 8], eax
loc_1_05E5:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_0624
loc_1_05EB:
        cmp    dword ptr [ebp + 10h], 1010103h
        je     SHORT loc_1_0624
        cmp    dword ptr [ebp + 1ch], edi
        jae    SHORT loc_1_0614
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
loc_1_0614:
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   esi
        push   eax
        call   sub_1_2050
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], edi
loc_1_0624:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0680:
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
        call   sub_1_02D0
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 20h], ecx
        mov    dword ptr [esi + 24h], edx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_06D0:
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
        ja     SHORT loc_1_070D
        je     NEAR PTR loc_1_081C
        cmp    eax, 1010eh
        je     SHORT loc_1_071F
        cmp    eax, 1010fh
        je     SHORT loc_1_077A
        cmp    eax, 10112h
        je     NEAR PTR loc_1_07F5
loc_1_070D:
        mov    ebx, 0c00000bbh
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_0857
loc_1_071F:
        cmp    edi, 4
        je     SHORT loc_1_0736
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_0857
loc_1_0736:
        push   4
        lea    eax, [ebp - 8]
        push   ecx
        push   eax
        call   sub_1_2050
        test   dword ptr [ebp - 8], 0f050h
        je     SHORT loc_1_0763
        mov    dword ptr [esi + 20h], 4
        mov    dword ptr [esi + 24h], 0
        mov    ebx, 0c00000bbh
        jmp    NEAR PTR loc_1_0857
loc_1_0763:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   esi
        push   ecx
        push   edx
        call   sub_1_0870
        jmp    NEAR PTR loc_1_0855
loc_1_077A:
        cmp    edi, 4
        je     SHORT loc_1_0791
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    NEAR PTR loc_1_0857
loc_1_0791:
        push   4
        lea    eax, [ebp - 4]
        push   ecx
        push   eax
        call   sub_1_2050
        cmp    dword ptr [ebp - 4], 0eeh
        ja     SHORT loc_1_07EE
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [edx + 1f7h]
        cmp    dword ptr [ebp - 4], eax
        jbe    SHORT loc_1_07C8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 1f7h], eax
        mov    dword ptr [ecx + 18h], eax
        jmp    NEAR PTR loc_1_0857
loc_1_07C8:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 18h], eax
        jne    SHORT loc_1_07E6
        mov    eax, dword ptr [ecx + 18h]
        cmp    dword ptr [ebp - 4], eax
        jae    SHORT loc_1_07E6
        mov    eax, dword ptr [ebp - 4]
        push   edx
        mov    dword ptr [ecx + 18h], eax
        call   0
        jmp    SHORT loc_1_0857
loc_1_07E6:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        jmp    SHORT loc_1_0857
loc_1_07EE:
        mov    ebx, 0c0010014h
        jmp    SHORT loc_1_0857
loc_1_07F5:
        cmp    edi, 4
        je     SHORT loc_1_0809
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_1_0857
loc_1_0809:
        push   4
        xor    ebx, ebx
        push   ecx
        mov    ecx, dword ptr [ebp + 0ch]
        add    ecx, 71h
        push   ecx
        call   sub_1_2050
        jmp    SHORT loc_1_0857
loc_1_081C:
        mov    ebx, 6
        mov    eax, edi
        sub    edx, edx
        div    ebx
        test   edx, edx
        je     SHORT loc_1_083A
        mov    ebx, 0c0010014h
        xor    eax, eax
        mov    dword ptr [esi + 20h], eax
        mov    dword ptr [esi + 24h], eax
        jmp    SHORT loc_1_0857
loc_1_083A:
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
        call   sub_1_08B0
loc_1_0855:
        mov    ebx, eax
loc_1_0857:
        test   ebx, ebx
        jne    SHORT loc_1_0865
        mov    dword ptr [esi + 24h], 0
        mov    dword ptr [esi + 20h], edi
loc_1_0865:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_1_0870:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_1_08A5
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    ecx, dword ptr [ebp + 10h]
        push   ecx
        mov    edx, dword ptr [esi + 14h]
        push   edx
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [esi + 1ch]
        mov    ecx, dword ptr [eax + 147h]
        push   ecx
        db  0E8h, 0D3h, 029h, 000h, 000h
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    10h
loc_1_08A5:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    10h
        db  090h
sub_1_08B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 20h], 0
        jne    SHORT loc_1_08E9
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
        mov    edx, dword ptr [ecx + 147h]
        push   edx
        db  0E8h, 0AFh, 026h, 000h, 000h
        dec    dword ptr [esi + 1ch]
        pop    esi
        pop    ebp
        ret    14h
loc_1_08E9:
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0900:
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
        call   sub_1_02D0
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0c00000bbh
        jne    NEAR PTR loc_1_09E0
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1020101h
        ja     SHORT loc_1_0970
        je     SHORT loc_1_09AF
        sub    eax, 20101h
        cmp    eax, 4
        ja     SHORT loc_1_097E
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_0970:
        cmp    eax, 1020102h
        je     SHORT loc_1_09B7
        cmp    eax, 1020103h
        je     SHORT loc_1_09BF
loc_1_097E:
        mov    dword ptr [ebp - 8], 0c00000bbh
        jmp    SHORT loc_1_09C8
loc_1_0987:
        mov    eax, dword ptr [ebx + 18ch]
        jmp    SHORT loc_1_09C5
loc_1_098F:
        mov    eax, dword ptr [ebx + 190h]
        jmp    SHORT loc_1_09C5
loc_1_0997:
        mov    eax, dword ptr [ebx + 194h]
        jmp    SHORT loc_1_09C5
loc_1_099F:
        mov    eax, dword ptr [ebx + 1b4h]
        jmp    SHORT loc_1_09C5
loc_1_09A7:
        mov    eax, dword ptr [ebx + 1b8h]
        jmp    SHORT loc_1_09C5
loc_1_09AF:
        mov    eax, dword ptr [ebx + 1b0h]
        jmp    SHORT loc_1_09C5
loc_1_09B7:
        mov    eax, dword ptr [ebx + 198h]
        jmp    SHORT loc_1_09C5
loc_1_09BF:
        mov    eax, dword ptr [ebx + 19ch]
loc_1_09C5:
        mov    dword ptr [ebp - 4], eax
loc_1_09C8:
        cmp    edi, 4
        jb     SHORT loc_1_09E0
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp - 14h]
        push   ecx
        call   sub_1_2050
        add    dword ptr [ebp - 0ch], 4
loc_1_09E0:
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
loc_1_0A10:
        push   ebp
        mov    ecx, dword ptr [esp + 0ch]
        mov    ebp, esp
        push   esi
        push   edi
        xor    esi, esi
        mov    eax, dword ptr [ecx + 14h]
        cmp    eax, 10113h
        ja     SHORT loc_1_0A37
        je     SHORT loc_1_0A68
        cmp    eax, 10101h
        jb     SHORT loc_1_0A63
        cmp    eax, 10111h
        jbe    SHORT loc_1_0A68
        jmp    SHORT loc_1_0A63
loc_1_0A37:
        cmp    eax, 1010104h
        ja     SHORT loc_1_0A55
        cmp    eax, 1010101h
        jae    SHORT loc_1_0A68
        cmp    eax, 20101h
        jb     SHORT loc_1_0A63
        cmp    eax, 20105h
        jbe    SHORT loc_1_0A68
        jmp    SHORT loc_1_0A63
loc_1_0A55:
        cmp    eax, 1020101h
        jb     SHORT loc_1_0A63
        cmp    eax, 1020103h
        jbe    SHORT loc_1_0A68
loc_1_0A63:
        mov    esi, 0c00000bbh
loc_1_0A68:
        mov    edi, dword ptr [ebp + 8]
        inc    dword ptr [edi + 15fh]
        test   esi, esi
        jne    SHORT loc_1_0A7E
        push   ecx
        push   edi
        call   sub_1_0900
        mov    esi, eax
loc_1_0A7E:
        mov    eax, dword ptr [edi + 15fh]
        dec    eax
        mov    dword ptr [edi + 15fh], eax
        jne    SHORT loc_1_0AC6
        cmp    byte ptr [edi + 1edh], 0
        je     SHORT loc_1_0AA1
        lea    eax, [edi + 307h]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_0AB5
loc_1_0AA1:
        cmp    dword ptr [edi + 177h], 0
        je     SHORT loc_1_0AC6
        lea    eax, [edi + 307h]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_0AC6
loc_1_0AB5:
        push   0
        add    edi, 1fbh
        push   edi
        mov    byte ptr [eax], 1
        call   sub_1_1C4A
loc_1_0AC6:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_0AD0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  0E8h, 08Fh, 010h, 000h, 000h
        lea    eax, [ebp - 1]
        lea    ecx, [esi + 30eh]
        push   eax
        mov    edi, 300h
        push   ecx
        call   sub_1_1C50
loc_1_0AF6:
        push   2710h
        call   sub_1_1C68
        dec    edi
        jne    SHORT loc_1_0AF6
        mov    byte ptr [esi + 309h], 1
        db  03Bh, 035h, 042h, 000h, 000h, 000h
        jne    SHORT loc_1_0B1F
        mov    eax, dword ptr [esi + 157h]
        db  0A3h, 042h, 000h, 000h, 000h
        jmp    SHORT loc_1_0B4D
loc_1_0B1F:
        db  08Bh, 00Dh, 042h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 157h], esi
        je     SHORT loc_1_0B3B
loc_1_0B2D:
        mov    ecx, dword ptr [ecx + 157h]
        cmp    dword ptr [ecx + 157h], esi
        jne    SHORT loc_1_0B2D
loc_1_0B3B:
        mov    eax, dword ptr [ecx + 157h]
        mov    edx, dword ptr [eax + 157h]
        mov    dword ptr [ecx + 157h], edx
loc_1_0B4D:
        lea    eax, [esi + 0d5h]
        push   eax
        call   sub_1_1C5C
        mov    eax, dword ptr [esi + 147h]
        push   eax
        db  0E8h, 04Bh, 022h, 000h, 000h
        mov    eax, dword ptr [esi + 52h]
        push   eax
        call   sub_1_1BA0
        push   0
        push   31bh
        push   esi
        call   sub_1_1C74
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0B90:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_1_1B94
        push   0
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        push   ecx
        call   sub_1_1B88
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0BC0:
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
        call   sub_1_1C98
        cmp    dword ptr [ebp - 8], 0ch
        jb     NEAR PTR loc_1_0DBB
        cmp    dword ptr [ebp - 8], 5eah
        ja     NEAR PTR loc_1_0DBB
        cmp    byte ptr [esi + 1ech], 0
        je     SHORT loc_1_0C16
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0C16:
        cmp    byte ptr [esi + 1eeh], 0
        je     SHORT loc_1_0C2D
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0C2D:
        cmp    byte ptr [ebx + 20h], 0
        je     SHORT loc_1_0C41
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0C41:
        mov    eax, dword ptr [ebp - 4]
        push   edi
        push   ebx
        inc    dword ptr [ebx + 1ch]
        push   esi
        inc    dword ptr [esi + 15fh]
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax], 0
        call   sub_1_0E10
        mov    ecx, dword ptr [ebp - 4]
        cmp    byte ptr [ecx + 9], 0
        je     SHORT loc_1_0C95
        cmp    dword ptr [esi + 177h], 0
        jne    SHORT loc_1_0C81
        mov    dword ptr [esi + 17bh], edi
        mov    dword ptr [esi + 177h], edi
        jmp    NEAR PTR loc_1_0D1F
loc_1_0C81:
        mov    eax, dword ptr [esi + 17bh]
        mov    dword ptr [eax + 24h], edi
        mov    dword ptr [esi + 17bh], edi
        jmp    NEAR PTR loc_1_0D1F
loc_1_0C95:
        cmp    dword ptr [esi + 167h], 0
        je     SHORT loc_1_0CA9
        mov    eax, dword ptr [esi + 16bh]
        mov    dword ptr [eax + 24h], edi
        jmp    SHORT loc_1_0D12
loc_1_0CA9:
        cmp    dword ptr [esi + 16fh], 0
        jne    SHORT loc_1_0CC2
        xor    ebx, ebx
        mov    dword ptr [esi + 173h], edi
        mov    dword ptr [esi + 16fh], edi
        jmp    SHORT loc_1_0CD1
loc_1_0CC2:
        mov    ebx, dword ptr [esi + 173h]
        mov    dword ptr [ebx + 24h], edi
        mov    dword ptr [esi + 173h], edi
loc_1_0CD1:
        mov    byte ptr [esi + 31ah], 0
        push   esi
        push   edi
        db  0E8h, 081h, 018h, 000h, 000h
        cmp    ax, 6
        jne    SHORT loc_1_0D1F
        test   ebx, ebx
        jne    SHORT loc_1_0CFF
        mov    dword ptr [esi + 173h], 0
        mov    dword ptr [esi + 16fh], 0
        jmp    SHORT loc_1_0D0C
loc_1_0CFF:
        mov    dword ptr [esi + 173h], ebx
        mov    dword ptr [ebx + 24h], 0
loc_1_0D0C:
        mov    dword ptr [esi + 167h], edi
loc_1_0D12:
        mov    dword ptr [esi + 16bh], edi
        mov    byte ptr [esi + 31ah], 0
loc_1_0D1F:
        mov    edi, 103h
        mov    eax, dword ptr [esi + 15fh]
        test   byte ptr [esi + 0c5h], 8
        je     SHORT loc_1_0D6E
        dec    eax
        mov    dword ptr [esi + 15fh], eax
        cmp    byte ptr [esi + 307h], 0
        jne    SHORT loc_1_0DB0
        test   eax, eax
        jne    SHORT loc_1_0DB0
        cmp    dword ptr [esi + 177h], 0
        je     SHORT loc_1_0DB0
        mov    byte ptr [esi + 307h], 1
        push   0
        push   0
        push   esi
        push   0
        call   sub_1_1180
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0D6E:
        dec    eax
        mov    dword ptr [esi + 15fh], eax
        jne    SHORT loc_1_0DB0
        cmp    byte ptr [esi + 1edh], 0
        je     SHORT loc_1_0D8B
        lea    eax, [esi + 307h]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_0D9F
loc_1_0D8B:
        cmp    dword ptr [esi + 177h], 0
        je     SHORT loc_1_0DB0
        lea    eax, [esi + 307h]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_0DB0
loc_1_0D9F:
        push   0
        add    esi, 1fbh
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_1C4A
loc_1_0DB0:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0DBB:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0DD0:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        push   ebx
        push   esi
        xor    esi, esi
        cmp    edx, esi
        je     SHORT loc_1_0DF2
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
loc_1_0DE5:
        mov    bl, byte ptr [ecx + esi]
        cmp    byte ptr [esi + eax], bl
        jne    SHORT loc_1_0DFA
        inc    esi
        cmp    esi, edx
        jb     SHORT loc_1_0DE5
loc_1_0DF2:
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_0DFA:
        mov    eax, 0ffffffffh
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0E10:
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
        call   sub_1_1700
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, byte ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 147h]
        cmp    byte ptr [ecx + 28h], dl
        mov    ebx, dword ptr [eax + 0ch]
        jne    SHORT loc_1_0E63
        push   6
        lea    eax, [ebp - 8]
        push   eax
        add    ecx, 24h
        push   ecx
        call   sub_1_0DD0
        test   eax, eax
        jne    SHORT loc_1_0E63
        mov    eax, ebx
        and    al, 1
        mov    byte ptr [esi + 9], al
loc_1_0E63:
        mov    eax, dword ptr [ebp + 0ch]
        test   byte ptr [eax + 71h], 2
        je     SHORT loc_1_0E78
        mov    byte ptr [esi + 8], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0E78:
        test   bl, 20h
        je     SHORT loc_1_0E89
        mov    byte ptr [esi + 8], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0E89:
        db  0A0h, 004h, 000h, 000h, 000h
        cmp    byte ptr [ebp - 4], al
        jne    SHORT loc_1_0EB5
        push   6
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_0DD0
        test   eax, eax
        jne    SHORT loc_1_0EB5
        and    bl, 8
        mov    byte ptr [esi + 8], bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0EB5:
        test   byte ptr [ebp - 8], 1
        je     SHORT loc_1_0EC9
        and    bl, 6
        mov    byte ptr [esi + 8], bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0EC9:
        cmp    byte ptr [esi + 9], 0
        je     SHORT loc_1_0EDB
        mov    byte ptr [esi + 8], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0EDB:
        mov    byte ptr [esi + 8], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0EF0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        cmp    byte ptr [eax + 20h], 0
        je     SHORT loc_1_0F09
        mov    eax, 0c0010002h
        pop    ebp
        ret    4
loc_1_0F09:
        cmp    byte ptr [ecx + 1edh], 0
        je     SHORT loc_1_0F18
        xor    eax, eax
        pop    ebp
        ret    4
loc_1_0F18:
        inc    dword ptr [eax + 1ch]
        inc    dword ptr [ecx + 15fh]
        mov    byte ptr [ecx + 1edh], 1
        mov    dword ptr [ecx + 1efh], eax
        mov    eax, dword ptr [ecx + 15fh]
        dec    eax
        mov    dword ptr [ecx + 15fh], eax
        jne    SHORT loc_1_0F76
        cmp    byte ptr [ecx + 1edh], 0
        je     SHORT loc_1_0F51
        lea    eax, [ecx + 307h]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_0F65
loc_1_0F51:
        cmp    dword ptr [ecx + 177h], 0
        je     SHORT loc_1_0F76
        lea    eax, [ecx + 307h]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_0F76
loc_1_0F65:
        push   0
        add    ecx, 1fbh
        push   ecx
        mov    byte ptr [eax], 1
        call   sub_1_1C4A
loc_1_0F76:
        mov    eax, 103h
        pop    ebp
        ret    4
        db  090h
loc_1_0F80:
        xor    eax, eax
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0F90:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 8]
        push   ecx
        db  0E8h, 020h, 010h, 000h, 000h
        cmp    ax, 1
        pop    ebp
        mov    eax, 0
        adc    eax, -1
        and    eax, 0c0000001h
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0FC0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        dec    dword ptr [eax + 1ch]
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_0FD0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        db  0E8h, 0B3h, 00Dh, 000h, 000h
        mov    al, 1
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0FF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    bl, bl
        mov    esi, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 15fh]
loc_1_1000:
        cmp    byte ptr [esi + 307h], 0
        jne    SHORT loc_1_101E
loc_1_1009:
        mov    byte ptr [esi + 307h], 1
        push   0
        push   0
        push   esi
        push   0
        call   sub_1_1180
        mov    bl, al
loc_1_101E:
        cmp    byte ptr [esi + 308h], 0
        jne    SHORT loc_1_104A
        mov    byte ptr [esi + 308h], 1
        push   esi
        call   sub_1_16C0
        cmp    dword ptr [esi + 177h], 0
        je     SHORT loc_1_1046
        cmp    byte ptr [esi + 307h], 0
        je     SHORT loc_1_1009
loc_1_1046:
        test   bl, bl
        jne    SHORT loc_1_1000
loc_1_104A:
        mov    eax, dword ptr [esi + 15fh]
        dec    eax
        mov    dword ptr [esi + 15fh], eax
        jne    SHORT loc_1_1092
        cmp    byte ptr [esi + 1edh], 0
        je     SHORT loc_1_106B
        cmp    byte ptr [esi + 307h], 0
        je     SHORT loc_1_107D
loc_1_106B:
        cmp    dword ptr [esi + 177h], 0
        je     SHORT loc_1_1092
        cmp    byte ptr [esi + 307h], 0
        jne    SHORT loc_1_1092
loc_1_107D:
        push   0
        lea    eax, [esi + 1fbh]
        mov    byte ptr [esi + 307h], 1
        push   eax
        call   sub_1_1C4A
loc_1_1092:
        cmp    byte ptr [esi + 146h], 0
        je     SHORT loc_1_10AD
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        add    esi, 0d5h
        push   esi
        call   sub_1_1C62
loc_1_10AD:
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_10C0:
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
        mov    dword ptr [esi + 17fh], ebx
        mov    byte ptr [esi + 183h], 1
        push   0
        push   ebx
        call   sub_1_1C98
        cmp    dword ptr [ebp - 4], 6
        jb     SHORT loc_1_116B
        mov    edi, dword ptr [esi + 1f7h]
        mov    eax, dword ptr [ebp - 4]
        add    edi, 0eh
        cmp    edi, eax
        jb     SHORT loc_1_1104
        mov    edi, eax
loc_1_1104:
        push   edi
        lea    eax, [esi + 20bh]
        push   0
        push   ebx
        push   eax
        call   sub_1_1700
        cmp    dword ptr [ebp - 4], 0eh
        jae    SHORT loc_1_1142
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        lea    ecx, [esi + 20bh]
        push   0
        push   eax
        push   ecx
        mov    eax, dword ptr [esi + 147h]
        push   ecx
        push   esi
        push   eax
        call   sub_1_1D60
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1142:
        mov    eax, dword ptr [ebp - 4]
        sub    edi, 0eh
        sub    eax, 0eh
        lea    ecx, [esi + 219h]
        push   eax
        push   edi
        push   ecx
        lea    eax, [esi + 20bh]
        push   0eh
        mov    ecx, dword ptr [esi + 147h]
        push   eax
        push   eax
        push   esi
        push   ecx
        call   sub_1_1D60
loc_1_116B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1180:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        inc    dword ptr [esi + 15fh]
        cmp    byte ptr [esi + 146h], 0
        je     SHORT loc_1_11AA
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 0d5h]
        push   eax
        jmp    SHORT loc_1_11BC
loc_1_11AA:
        db  0E8h, 041h, 00Ch, 000h, 000h
        lea    ecx, [esi + 0d5h]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
loc_1_11BC:
        call   sub_1_1C62
        cmp    dword ptr [esi + 177h], 0
        je     NEAR PTR loc_1_1287
loc_1_11CE:
        mov    edi, dword ptr [esi + 177h]
        cmp    dword ptr [esi + 17bh], edi
        jne    SHORT loc_1_11E6
        mov    dword ptr [esi + 17bh], 0
loc_1_11E6:
        mov    eax, dword ptr [edi + 24h]
        push   edi
        push   esi
        mov    dword ptr [esi + 177h], eax
        call   sub_1_10C0
        push   0
        inc    dword ptr [esi + 18ch]
        mov    ebx, dword ptr [edi + 28h]
        push   edi
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1BB2
        mov    edi, dword ptr [ebx + 8]
        dec    dword ptr [ebx + 1ch]
        jne    SHORT loc_1_127A
        mov    eax, dword ptr [edi + 15bh]
        cmp    ebx, eax
        jne    SHORT loc_1_1227
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [edi + 15bh], eax
        jmp    SHORT loc_1_123A
loc_1_1227:
        cmp    dword ptr [eax + 0ch], ebx
        je     SHORT loc_1_1234
loc_1_122C:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], ebx
        jne    SHORT loc_1_122C
loc_1_1234:
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_123A:
        mov    eax, dword ptr [edi + 1f7h]
        cmp    dword ptr [ebx + 18h], eax
        jne    SHORT loc_1_124B
        push   edi
        call   0
loc_1_124B:
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1BAC
        push   0
        push   75h
        push   ebx
        call   sub_1_1C74
        cmp    dword ptr [edi + 15bh], 0
        jne    SHORT loc_1_127A
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        add    edi, 0d5h
        push   edi
        call   sub_1_1C62
loc_1_127A:
        cmp    dword ptr [esi + 177h], 0
        jne    NEAR PTR loc_1_11CE
loc_1_1287:
        push   esi
        db  0E8h, 0F3h, 00Dh, 000h, 000h
        sub    ax, 12h
        cmp    ax, 1
        sbb    al, al
        neg    al
        cmp    byte ptr [esi + 183h], 0
        mov    byte ptr [ebp - 4], al
        je     SHORT loc_1_12B8
        mov    byte ptr [esi + 183h], 0
        mov    eax, dword ptr [esi + 147h]
        push   eax
        call   sub_1_1F40
loc_1_12B8:
        cmp    byte ptr [esi + 1edh], 0
        je     NEAR PTR loc_1_167A
        cmp    dword ptr [esi + 15fh], 1
        jne    NEAR PTR loc_1_167A
        push   esi
        lea    eax, [esi + 0d5h]
        mov    byte ptr [esi + 1eeh], 1
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_1C62
        mov    edi, dword ptr [esi + 15bh]
        test   edi, edi
        je     NEAR PTR loc_1_1388
loc_1_12F9:
        push   0
        inc    dword ptr [edi + 1ch]
        push   0
        mov    eax, dword ptr [edi]
        push   40010004h
        push   eax
        call   sub_1_1BD6
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_137D
        mov    eax, dword ptr [ebx + 15bh]
        cmp    eax, edi
        jne    SHORT loc_1_132A
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 15bh], eax
        jmp    SHORT loc_1_133D
loc_1_132A:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_1337
loc_1_132F:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_132F
loc_1_1337:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_133D:
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [ebx + 1f7h], eax
        jne    SHORT loc_1_134E
        push   ebx
        call   0
loc_1_134E:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1BAC
        push   0
        push   75h
        push   edi
        call   sub_1_1C74
        cmp    dword ptr [ebx + 15bh], 0
        jne    SHORT loc_1_137D
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 0d5h
        push   ebx
        call   sub_1_1C62
loc_1_137D:
        mov    edi, dword ptr [edi + 0ch]
        test   edi, edi
        jne    NEAR PTR loc_1_12F9
loc_1_1388:
        push   esi
        db  0E8h, 022h, 008h, 000h, 000h
        test   ax, ax
        je     SHORT loc_1_13AF
        mov    byte ptr [esi + 1ech], 1
        push   0
        mov    eax, dword ptr [esi + 52h]
        push   0aa000003h
        push   eax
        call   sub_1_1D40
        add    esp, 0ch
        jmp    SHORT loc_1_13B6
loc_1_13AF:
        mov    byte ptr [esi + 1ech], 0
loc_1_13B6:
        cmp    dword ptr [esi + 16fh], 0
        je     SHORT loc_1_13EE
        mov    eax, dword ptr [esi + 167h]
        mov    ecx, dword ptr [esi + 173h]
        mov    dword ptr [ecx + 24h], eax
        mov    edx, dword ptr [esi + 16fh]
        mov    dword ptr [esi + 167h], edx
        mov    dword ptr [esi + 173h], 0
        mov    dword ptr [esi + 16fh], 0
loc_1_13EE:
        cmp    dword ptr [esi + 177h], 0
        je     SHORT loc_1_1412
        mov    eax, dword ptr [esi + 167h]
        mov    ecx, dword ptr [esi + 17bh]
        mov    dword ptr [ecx + 24h], eax
        mov    edx, dword ptr [esi + 177h]
        mov    dword ptr [esi + 167h], edx
loc_1_1412:
        mov    dword ptr [esi + 17bh], 0
        mov    dword ptr [esi + 177h], 0
        cmp    dword ptr [esi + 167h], 0
        je     NEAR PTR loc_1_14D0
loc_1_1433:
        mov    ecx, dword ptr [esi + 167h]
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [esi + 167h], eax
        mov    edi, dword ptr [ecx + 28h]
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_1BB2
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_14C3
        mov    eax, dword ptr [ebx + 15bh]
        cmp    eax, edi
        jne    SHORT loc_1_1470
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 15bh], eax
        jmp    SHORT loc_1_1483
loc_1_1470:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_147D
loc_1_1475:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_1475
loc_1_147D:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_1483:
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [ebx + 1f7h], eax
        jne    SHORT loc_1_1494
        push   ebx
        call   0
loc_1_1494:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1BAC
        push   0
        push   75h
        push   edi
        call   sub_1_1C74
        cmp    dword ptr [ebx + 15bh], 0
        jne    SHORT loc_1_14C3
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 0d5h
        push   ebx
        call   sub_1_1C62
loc_1_14C3:
        cmp    dword ptr [esi + 167h], 0
        jne    NEAR PTR loc_1_1433
loc_1_14D0:
        mov    dword ptr [esi + 16bh], 0
        cmp    byte ptr [esi + 1ech], 0
        jne    SHORT loc_1_14E9
        push   esi
        db  0E8h, 0A7h, 009h, 000h, 000h
loc_1_14E9:
        mov    byte ptr [esi + 1eeh], 0
        mov    edi, dword ptr [esi + 15bh]
        test   edi, edi
        je     NEAR PTR loc_1_15C8
loc_1_14FE:
        inc    dword ptr [edi + 1ch]
        cmp    byte ptr [esi + 1ech], 0
        je     SHORT loc_1_151B
        push   0
        mov    eax, dword ptr [edi]
        push   0
        push   40010007h
        push   eax
        call   sub_1_1BD6
loc_1_151B:
        mov    eax, 0
        push   4
        cmp    byte ptr [esi + 1ech], 1
        adc    eax, -1
        and    eax, 0c0000001h
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [edi]
        push   40010005h
        push   ecx
        call   sub_1_1BD6
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_1BDC
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_15BD
        mov    eax, dword ptr [ebx + 15bh]
        cmp    eax, edi
        jne    SHORT loc_1_156A
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 15bh], eax
        jmp    SHORT loc_1_157D
loc_1_156A:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_1577
loc_1_156F:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_156F
loc_1_1577:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_157D:
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [ebx + 1f7h], eax
        jne    SHORT loc_1_158E
        push   ebx
        call   0
loc_1_158E:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1BAC
        push   0
        push   75h
        push   edi
        call   sub_1_1C74
        cmp    dword ptr [ebx + 15bh], 0
        jne    SHORT loc_1_15BD
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 0d5h
        push   ebx
        call   sub_1_1C62
loc_1_15BD:
        mov    edi, dword ptr [edi + 0ch]
        test   edi, edi
        jne    NEAR PTR loc_1_14FE
loc_1_15C8:
        mov    eax, 0
        mov    ecx, dword ptr [esi + 1efh]
        cmp    byte ptr [esi + 1ech], 1
        adc    eax, -1
        mov    edx, dword ptr [ecx]
        and    eax, 0c0000001h
        push   eax
        push   edx
        call   sub_1_1BBE
        mov    ebx, dword ptr [esi + 1efh]
        mov    edi, dword ptr [ebx + 8]
        dec    dword ptr [ebx + 1ch]
        jne    SHORT loc_1_166D
        mov    ecx, dword ptr [edi + 15bh]
        cmp    ebx, ecx
        jne    SHORT loc_1_160E
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [edi + 15bh], eax
        jmp    SHORT loc_1_162D
loc_1_160E:
        mov    eax, dword ptr [esi + 1efh]
        cmp    dword ptr [ecx + 0ch], eax
        je     SHORT loc_1_1621
loc_1_1619:
        mov    ecx, dword ptr [ecx + 0ch]
        cmp    dword ptr [ecx + 0ch], eax
        jne    SHORT loc_1_1619
loc_1_1621:
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    ebx, dword ptr [esi + 1efh]
loc_1_162D:
        mov    eax, dword ptr [edi + 1f7h]
        cmp    dword ptr [ebx + 18h], eax
        jne    SHORT loc_1_163E
        push   edi
        call   0
loc_1_163E:
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1BAC
        push   0
        push   75h
        push   ebx
        call   sub_1_1C74
        cmp    dword ptr [edi + 15bh], 0
        jne    SHORT loc_1_166D
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        add    edi, 0d5h
        push   edi
        call   sub_1_1C62
loc_1_166D:
        mov    byte ptr [esi + 1edh], 0
        push   esi
        db  0E8h, 016h, 008h, 000h, 000h
loc_1_167A:
        mov    byte ptr [esi + 307h], 0
        cmp    byte ptr [esi + 146h], 0
        push   esi
        jne    SHORT loc_1_1691
        db  0E8h, 0C0h, 007h, 000h, 000h
        push   esi
loc_1_1691:
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 0d5h]
        push   eax
        call   sub_1_1C62
        mov    al, byte ptr [ebp - 4]
        pop    edi
        dec    dword ptr [esi + 15fh]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_16C0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 1eeh], 0
        je     SHORT loc_1_16DC
        mov    byte ptr [esi + 308h], 0
        pop    esi
        pop    ebp
        ret    4
loc_1_16DC:
        mov    byte ptr [esi + 31ah], 0
        push   esi
        db  0E8h, 0A7h, 00Ch, 000h, 000h
        mov    byte ptr [esi + 308h], 0
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1700:
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
        call   sub_1_1C98
        mov    ebx, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp - 4], esi
        je     SHORT loc_1_1754
loc_1_1725:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_1CBC
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     SHORT loc_1_1754
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_1CE0
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1725
loc_1_1754:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1765
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_1765:
        mov    eax, ebx
        sub    eax, edi
        sub    edi, ebx
        add    dword ptr [ebp - 0ch], eax
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 14h]
loc_1_1774:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    SHORT loc_1_1783
        mov    ebx, edi
        sub    ebx, esi
        jmp    SHORT loc_1_1786
loc_1_1783:
        mov    ebx, dword ptr [ebp - 8]
loc_1_1786:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        add    esi, ebx
        push   eax
        call   sub_1_2050
        cmp    esi, edi
        je     SHORT loc_1_17C2
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_1CE0
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_17CD
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_1CBC
        jmp    SHORT loc_1_1774
loc_1_17C2:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_17CD:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_17E0:
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
        cmp    dword ptr [edi + 17fh], 0
        je     SHORT loc_1_186B
        push   0
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        push   0
        mov    ebx, esi
        push   0
        push   ecx
        call   sub_1_1C98
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1859
loc_1_1819:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_1CBC
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [edi + 17fh]
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   ebx
        push   edx
        push   eax
        call   sub_1_1700
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 8]
        jb     SHORT loc_1_1859
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_1CE0
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_1819
loc_1_1859:
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
loc_1_186B:
        cmp    byte ptr [edi + 183h], 0
        je     SHORT loc_1_18EB
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    ecx, dword ptr [ebp + 14h]
        push   esi
        mov    edx, dword ptr [ebp + 1ch]
        push   ecx
        push   edx
        db  0E8h, 089h, 00Dh, 000h, 000h
        test   ax, ax
        je     SHORT loc_1_18E0
        cmp    ax, 13h
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], 0
        jne    SHORT loc_1_18D2
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebp + 18h]
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edi + 105h], eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [edi + 109h], ecx
        mov    dword ptr [edi + 10dh], esi
        mov    dword ptr [edi + 111h], edx
        mov    dword ptr [edi + 115h], eax
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_18D2:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_18E0:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_18EB:
        mov    eax, 0c0010013h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1900:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   eax
        db  0E8h, 033h, 006h, 000h, 000h
        cmp    ax, 1
        pop    ebp
        sbb    cl, cl
        neg    cl
        mov    al, cl
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_1920:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 31ah], 0
        je     NEAR PTR loc_1_1A29
        cmp    byte ptr [esi + 0f4h], 0
        je     NEAR PTR loc_1_1A29
        push   esi
        db  0E8h, 047h, 004h, 000h, 000h
        mov    byte ptr [esi + 31ah], 0
        push   0
        mov    eax, dword ptr [esi + 52h]
        push   0aa000003h
        push   eax
        call   sub_1_1D40
        add    esp, 0ch
        cmp    dword ptr [esi + 16fh], 0
        je     SHORT loc_1_19A1
        xor    edi, edi
loc_1_196E:
        mov    edx, dword ptr [esi + 16fh]
        lea    ecx, [edx + 24h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 16fh], eax
        test   eax, eax
        jne    SHORT loc_1_1989
        mov    dword ptr [esi + 173h], edi
loc_1_1989:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   edx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1BB2
        dec    dword ptr [ebx + 1ch]
        cmp    dword ptr [esi + 16fh], edi
        jne    SHORT loc_1_196E
loc_1_19A1:
        cmp    dword ptr [esi + 167h], 0
        je     SHORT loc_1_19DF
        xor    edi, edi
loc_1_19AC:
        mov    edx, dword ptr [esi + 167h]
        lea    ecx, [edx + 24h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 167h], eax
        test   eax, eax
        jne    SHORT loc_1_19C7
        mov    dword ptr [esi + 16bh], edi
loc_1_19C7:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   edx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1BB2
        dec    dword ptr [ebx + 1ch]
        cmp    dword ptr [esi + 167h], edi
        jne    SHORT loc_1_19AC
loc_1_19DF:
        mov    byte ptr [esi + 31ah], 0
        mov    byte ptr [esi + 0f4h], 0
        push   esi
        db  0E8h, 0BDh, 001h, 000h, 000h
        test   ax, ax
        je     SHORT loc_1_1A14
        mov    byte ptr [esi + 1ech], 1
        push   0
        mov    eax, dword ptr [esi + 52h]
        push   0aa000003h
        push   eax
        call   sub_1_1D40
        add    esp, 0ch
        jmp    SHORT loc_1_1A1B
loc_1_1A14:
        mov    byte ptr [esi + 1ech], 0
loc_1_1A1B:
        push   esi
        db  0E8h, 06Fh, 004h, 000h, 000h
        push   esi
        db  0E8h, 009h, 003h, 000h, 000h
        jmp    SHORT loc_1_1A42
loc_1_1A29:
        cmp    dword ptr [esi + 16fh], 0
        jne    SHORT loc_1_1A3B
        cmp    dword ptr [esi + 167h], 0
        je     SHORT loc_1_1A42
loc_1_1A3B:
        mov    byte ptr [esi + 31ah], 1
loc_1_1A42:
        push   1388h
        add    esi, 30eh
        push   esi
        call   sub_1_1C4A
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1A60:
        db  0A0h, 000h, 000h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_1A85
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_1A87
        push   0
        push   0
        db  0E8h, 080h, 0E5h, 0FFh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_1A87
loc_1_1A85:
        clc
        ret
loc_1_1A87:
        stc
        ret
SMC8000W_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_1A98
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_1A98:
        clc
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0CCh, 0CCh
sub_1_1B08:
        push   esi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep outsb
        pop    esi
        ret    0ch
sub_1_1B1C:
        push   esi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep outsw
        pop    esi
        ret    0ch
        db  056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh
        db  05Eh, 0C2h, 00Ch, 000h
sub_1_1B45:
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep insb
        pop    edi
        ret    0ch
sub_1_1B59:
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep insw
        pop    edi
        ret    0ch
        db  057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh
        db  05Fh, 0C2h, 00Ch, 000h
sub_1_1B82:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_1B88:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_1B8E:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_1B94:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_1B9A:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_1BA0:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_1BAC:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_1BB2:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_1BBE:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_1BCA:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_1BD0:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_1BD6:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_1BDC:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
sub_1_1BE8:
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_1BF4:
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
sub_1_1C4A:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_1C50:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_1C56:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_1C5C:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
sub_1_1C62:
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_1C68:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_1C6E:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_1C74:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_1C98:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_1CBC:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_1CE0:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_1CEC:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_1CF2:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_1CF8:
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
sub_1_1D3A:
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_1D40:
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
sub_1_1D60:
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
        call   sub_1_1D90
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_1D90:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_1E30
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_1DD0
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_1DC5
        cmp    al, 0ffh
        jne    SHORT loc_1_1DC5
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_1DC5
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_1E37
loc_1_1DC5:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_1E37
        db  08Bh, 0FFh
loc_1_1DD0:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_1E20
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_1DF6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_1DFD
loc_1_1DF6:
        mov    dword ptr [ebp - 10h], 1
loc_1_1DFD:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_1E10
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_1E3A
        db  08Dh, 064h, 024h, 000h
loc_1_1E10:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_1E3A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1E20:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_1E3A
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1E30:
        mov    dword ptr [ebp - 4], 20h
loc_1_1E37:
        mov    esi, dword ptr [ebp + 8]
loc_1_1E3A:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_1F31
loc_1_1E45:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    SHORT loc_1_1E9D
        test   al, 2
        je     SHORT loc_1_1E89
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 0E6h, 015h, 000h, 000h
        or     al, al
        je     SHORT loc_1_1E89
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    SHORT loc_1_1E9D
loc_1_1E89:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_1E94
        test   bl, 4
        jne    SHORT loc_1_1E9D
loc_1_1E94:
        test   bl, 20h
        je     NEAR PTR loc_1_1F26
loc_1_1E9D:
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
        call   sub_1_1BCA
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_1F26
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_1EDF
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_1EDF:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_1EF0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_1EF6
        db  08Bh, 0FFh
loc_1_1EF0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_1EF6:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_1F05
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1BD0
loc_1_1F05:
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
        call   sub_1_1C74
        mov    edi, ebx
        jmp    SHORT loc_1_1F29
loc_1_1F26:
        mov    edi, dword ptr [edi + 10h]
loc_1_1F29:
        or     edi, edi
        jne    NEAR PTR loc_1_1E45
loc_1_1F31:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1F40:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1F50
        pop    ebp
        ret    4
sub_1_1F50:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_1FCE
loc_1_1F60:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_1FC7
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1BD0
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_1FC7
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_1F8C
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_1F8C:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_1FA0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_1FA6
        db  005h, 000h, 000h, 000h, 000h
loc_1_1FA0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_1FA6:
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
        call   sub_1_1C74
        mov    edi, ebx
        jmp    SHORT loc_1_1FCA
loc_1_1FC7:
        mov    edi, dword ptr [edi + 10h]
loc_1_1FCA:
        or     edi, edi
        jne    SHORT loc_1_1F60
loc_1_1FCE:
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
        je     SHORT loc_1_1FF5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1FF5:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_2031
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_2022
        cmp    bl, 0ffh
        jne    SHORT loc_1_2022
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_2022
        test   al, 8
        je     SHORT loc_1_201B
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_201B:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_2022:
        test   al, 4
        jne    SHORT loc_1_202A
        test   al, 2
        je     SHORT loc_1_2049
loc_1_202A:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_2031:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_2049
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_2049
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_2049:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
sub_1_2050:
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
sub_1_2070:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_2090
        or     ecx, ecx
        je     SHORT loc_1_20A6
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_20A4
loc_1_2090:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_20A4
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_20A4:
        rep movsb
loc_1_20A6:
        popfd
        pop    edi
        pop    esi
        ret    0ch
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 053h, 04Dh, 043h, 038h
        db  030h, 030h, 030h, 057h, 000h, 040h, 000h, 054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 0CCh, 0CCh, 0CCh
sub_1_2100:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   0fa0h
        mov    dx, word ptr [esi + 4]
        xor    edi, edi
        in     al, dx
        mov    bl, al
        or     al, 80h
        out    dx, al
        and    bl, 7fh
        call   sub_1_1C68
        mov    di, word ptr [esi + 4]
        mov    al, bl
        mov    edx, edi
        mov    ebx, dword ptr [esi + 0c5h]
        out    dx, al
        test   bl, 18h
        je     SHORT loc_1_217D
        in     al, dx
        cmp    byte ptr [esi + 146h], 0
        mov    cl, al
        je     SHORT loc_1_2152
        and    cl, 3fh
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 6]
        in     al, dx
        or     al, 10h
        jmp    SHORT loc_1_2191
loc_1_2152:
        test   bh, 8
        je     SHORT loc_1_2160
        lea    edx, [edi + 6]
        in     al, dx
        mov    cl, al
        and    al, 0cfh
        out    dx, al
loc_1_2160:
        test   ebx, 8000000h
        jne    SHORT loc_1_2174
        and    cl, 3fh
        mov    edx, edi
        or     cl, 40h
        mov    al, cl
        jmp    SHORT loc_1_2191
loc_1_2174:
        and    cl, 3fh
        mov    edx, edi
        mov    al, cl
        jmp    SHORT loc_1_2191
loc_1_217D:
        mov    eax, dword ptr [esi + 18h]
        shr    eax, 0dh
        and    al, 3fh
        test   ebx, 8000000h
        jne    SHORT loc_1_2192
        or     al, 40h
        mov    edx, edi
loc_1_2191:
        out    dx, al
loc_1_2192:
        xor    cl, cl
        xor    eax, eax
        xor    edx, edx
loc_1_2198:
        cmp    byte ptr [esi + eax + 24h], dl
        je     SHORT loc_1_21A0
        mov    cl, 1
loc_1_21A0:
        inc    eax
        cmp    eax, 6
        jb     SHORT loc_1_2198
        xor    edi, edi
loc_1_21A8:
        xor    eax, eax
        inc    edi
        mov    ax, word ptr [esi + 4]
        cmp    edi, 6
        lea    edx, [eax + edi + 7]
        in     al, dx
        mov    byte ptr [esi + edi + 29h], al
        jb     SHORT loc_1_21A8
        test   cl, cl
        jne    SHORT loc_1_21D3
        xor    eax, eax
loc_1_21C3:
        mov    dl, byte ptr [esi + eax + 2ah]
        lea    ecx, [esi + eax]
        inc    eax
        mov    byte ptr [ecx + 24h], dl
        cmp    eax, 6
        jb     SHORT loc_1_21C3
loc_1_21D3:
        test   byte ptr [esi + 0c8h], 8
        je     SHORT loc_1_21E9
        xor    edx, edx
        mov    al, 1
        mov    dx, word ptr [esi + 4]
        add    edx, 10h
        jmp    SHORT loc_1_21F4
loc_1_21E9:
        xor    edx, edx
        mov    al, 21h
        mov    dx, word ptr [esi + 4]
        add    edx, 10h
loc_1_21F4:
        out    dx, al
        xor    ecx, ecx
        xor    al, al
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 1ah]
        out    dx, al
        lea    edx, [ecx + 1bh]
        out    dx, al
        lea    edx, [ecx + 17h]
        in     al, dx
        test   al, 80h
        jne    SHORT loc_1_221F
        mov    edi, 400h
loc_1_2212:
        push   3e8h
        call   sub_1_1C68
        dec    edi
        jne    SHORT loc_1_2212
loc_1_221F:
        mov    ecx, dword ptr [esi + 0c5h]
        mov    eax, ecx
        and    eax, 8000000h
        cmp    eax, 1
        sbb    al, al
        and    al, 8
        add    al, 40h
        test   ecx, 80008h
        je     SHORT loc_1_223F
        or     al, 1
loc_1_223F:
        xor    edx, edx
        push   esi
        mov    dx, word ptr [esi + 4]
        xor    edi, edi
        add    edx, 1eh
        out    dx, al
        db  0E8h, 06Fh, 0FDh, 0FFh, 0FFh
        mov    di, word ptr [esi + 4]
        mov    al, 4
        mov    cl, byte ptr [esi + 13ah]
        lea    edx, [edi + 1dh]
        out    dx, al
        lea    edx, [edi + 11h]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 12h]
        mov    al, byte ptr [esi + 13bh]
        out    dx, al
        lea    edx, [edi + 13h]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 17h]
        mov    al, 0ffh
        out    dx, al
        mov    byte ptr [esi + 0d3h], 3fh
        mov    ebx, dword ptr [esi + 0c5h]
        and    ebx, 8000000h
        je     SHORT loc_1_229F
        test   byte ptr [esi + 49h], 4
        jne    SHORT loc_1_229F
        mov    byte ptr [esi + 0d3h], 7fh
loc_1_229F:
        lea    edx, [edi + 1fh]
        mov    al, byte ptr [esi + 0d3h]
        out    dx, al
        lea    edx, [edi + 10h]
        test   ebx, ebx
        je     SHORT loc_1_22B4
        mov    al, 41h
        jmp    SHORT loc_1_22B6
loc_1_22B4:
        mov    al, 61h
loc_1_22B6:
        out    dx, al
        xor    ebx, ebx
loc_1_22B9:
        lea    edx, [edi + ebx + 11h]
        mov    al, byte ptr [esi + ebx + 24h]
        out    dx, al
        inc    ebx
        cmp    ebx, 6
        jb     SHORT loc_1_22B9
        lea    eax, [ecx + 1]
        lea    edx, [edi + 17h]
        out    dx, al
        xor    ecx, ecx
        and    eax, 0ffh
        mov    cx, word ptr [esi + 38h]
        imul   ecx, eax
        add    ecx, dword ptr [esi + 1ch]
        cmp    byte ptr [esi + 146h], 0
        mov    dword ptr [esi + 13ch], ecx
        je     SHORT loc_1_231E
        lea    ecx, [edi + 7]
        xor    al, al
        mov    edx, ecx
        out    dx, al
        out    dx, al
        mov    ax, word ptr [esi + 20h]
        test   al, 7fh
        jbe    SHORT loc_1_2316
        shl    ax, 9
        lea    edx, [edi + 2]
        mov    edi, eax
        and    edi, 0ffffh
loc_1_230F:
        xor    eax, eax
        dec    edi
        out    dx, ax
        jne    SHORT loc_1_230F
loc_1_2316:
        mov    edx, ecx
        xor    al, al
        out    dx, al
        out    dx, al
        jmp    SHORT loc_1_2346
loc_1_231E:
        xor    ecx, ecx
        xor    eax, eax
        mov    ax, word ptr [esi + 20h]
        test   eax, 3fffffh
        jbe    SHORT loc_1_2346
loc_1_232D:
        mov    eax, dword ptr [esi + 1ch]
        inc    ecx
        mov    byte ptr [eax + ecx - 1], 0
        mov    ax, word ptr [esi + 20h]
        and    eax, 0ffffh
        shl    eax, 8
        cmp    eax, ecx
        ja     SHORT loc_1_232D
loc_1_2346:
        mov    edi, dword ptr [esi + 0c5h]
        mov    ebx, edi
        and    ebx, 8000000h
        je     SHORT loc_1_2363
        xor    edx, edx
        mov    al, 1
        mov    dx, word ptr [esi + 4]
        add    edx, 10h
        jmp    SHORT loc_1_236E
loc_1_2363:
        xor    edx, edx
        mov    al, 21h
        mov    dx, word ptr [esi + 4]
        add    edx, 10h
loc_1_236E:
        out    dx, al
        mov    ecx, edi
        and    ecx, 8
        je     SHORT loc_1_237E
        test   edi, 3c00000h
        je     SHORT loc_1_238C
loc_1_237E:
        mov    eax, edi
        and    eax, 3c00000h
        cmp    eax, 800000h
        jne    SHORT loc_1_2399
loc_1_238C:
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 5
        in     al, dx
        or     al, 4
        out    dx, al
loc_1_2399:
        test   edi, 50h
        jne    SHORT loc_1_23C0
        mov    eax, dword ptr [esi + 18h]
        pop    edi
        shr    eax, 0dh
        and    al, 3fh
        or     al, 40h
        mov    byte ptr [esi + 145h], al
        mov    byte ptr [esi + 144h], al
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_23C0:
        test   ecx, ecx
        je     SHORT loc_1_23DB
        mov    al, 1
        pop    edi
        mov    byte ptr [esi + 144h], 0
        mov    byte ptr [esi + 145h], 0
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_23DB:
        test   edi, 10h
        je     SHORT loc_1_2452
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 5
        test   ebx, ebx
        in     al, dx
        je     SHORT loc_1_242B
        test   edi, 80000h
        je     SHORT loc_1_2406
        mov    cl, al
        or     cl, 80h
        mov    byte ptr [esi + 144h], cl
        jmp    SHORT loc_1_241C
loc_1_2406:
        cmp    word ptr [esi + 0ah], 8
        jbe    SHORT loc_1_2416
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_2416:
        mov    byte ptr [esi + 144h], al
loc_1_241C:
        pop    edi
        mov    byte ptr [esi + 145h], al
        mov    al, 1
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_242B:
        and    al, 1fh
        mov    ecx, dword ptr [esi + 18h]
        shr    ecx, 3
        or     al, cl
        test   edi, 80000h
        je     SHORT loc_1_2448
        mov    cl, al
        or     cl, 80h
        mov    byte ptr [esi + 144h], cl
loc_1_2448:
        lea    ecx, [esi + 145h]
        mov    byte ptr [ecx], al
        jmp    SHORT loc_1_2462
loc_1_2452:
        lea    ecx, [esi + 145h]
        mov    byte ptr [esi + 144h], 1
        mov    byte ptr [ecx], 1
loc_1_2462:
        test   edi, 80000h
        je     SHORT loc_1_2474
        or     byte ptr [esi + 144h], 40h
        or     byte ptr [ecx], 40h
loc_1_2474:
        xor    edx, edx
        mov    al, byte ptr [ecx]
        mov    dx, word ptr [esi + 4]
        pop    edi
        add    edx, 5
        pop    esi
        out    dx, al
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2490:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 0f0h]
        mov    word ptr [esi + 0f2h], cx
        lea    eax, [ecx + 1]
        mov    word ptr [esi + 0f0h], ax
        cmp    word ptr [esi + 3ah], ax
        jne    SHORT loc_1_24C0
        mov    word ptr [esi + 0f0h], 0
loc_1_24C0:
        movsx  eax, word ptr [esi + 0f0h]
        cmp    dword ptr [esi + eax*4 + 0fdh], 2
        je     SHORT loc_1_24DA
        mov    word ptr [esi + 0f0h], 0ffffh
loc_1_24DA:
        xor    edi, edi
        xor    eax, eax
        mov    di, word ptr [esi + 4]
        movsx  ebx, cx
        mov    cx, word ptr [esi + ebx*4 + 0f5h]
        lea    edx, [edi + 14h]
        mov    ax, word ptr [esi + 0cdh]
        imul   eax, ebx
        mov    al, ah
        out    dx, al
        lea    edx, [edi + 15h]
        mov    al, cl
        out    dx, al
        lea    edx, [edi + 16h]
        mov    al, ch
        out    dx, al
        cmp    byte ptr [esi + 138h], 0
        je     SHORT loc_1_252C
        test   dword ptr [esi + 0c5h], 8100000h
        jne    SHORT loc_1_252C
        mov    byte ptr [esi + 139h], 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_252C:
        lea    edx, [edi + 10h]
        mov    al, 26h
        mov    byte ptr [esi + 0f4h], 1
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
sub_1_2540:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 34h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0cdh]
        lea    edx, [ebp - 24h]
        movsx  ecx, word ptr [esi + 0eeh]
        imul   eax, ecx
        add    eax, dword ptr [esi + 1ch]
        lea    ecx, [ebp - 34h]
        push   ecx
        xor    ebx, ebx
        push   edx
        mov    dword ptr [ebp - 1ch], eax
        push   ebx
        mov    eax, dword ptr [ebp + 0ch]
        push   ebx
        push   eax
        call   sub_1_1C98
        mov    eax, dword ptr [esi + 119h]
        mov    dword ptr [ebp - 28h], ebx
        mov    byte ptr [ebp - 3], bl
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [esi + 0c5h]
        and    eax, 8080000h
        cmp    eax, 8080000h
        jne    SHORT loc_1_25B0
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 10h
        in     al, dx
        test   al, 4
        jne    SHORT loc_1_25B0
        mov    byte ptr [ebp - 3], 1
        mov    al, 10h
        out    dx, al
loc_1_25B0:
        cmp    dword ptr [ebp - 24h], 0
        je     NEAR PTR loc_1_28A7
loc_1_25BA:
        lea    eax, [ebp - 18h]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 24h]
        push   ecx
        push   edx
        call   sub_1_1CBC
        mov    ecx, dword ptr [esi + 0c5h]
        test   cl, 8
        mov    dword ptr [ebp - 20h], ecx
        je     NEAR PTR loc_1_2789
        test   ecx, 800000h
        jne    NEAR PTR loc_1_2789
        mov    eax, dword ptr [ebp - 0ch]
        xor    ecx, ecx
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp - 1ch]
        and    al, 3
        mov    cl, al
        mov    byte ptr [ebp - 2], al
        mov    eax, dword ptr [ebp - 1ch]
        mov    edi, dword ptr [ebp - 0ch]
        sub    eax, ecx
        mov    dword ptr [ebp - 10h], eax
        mov    ebx, dword ptr [eax]
        mov    eax, 4
        mov    dword ptr [ebp - 8], ebx
loc_1_2610:
        sub    eax, ecx
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 18h], eax
        jae    SHORT loc_1_2620
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 14h], eax
loc_1_2620:
        xor    eax, eax
        cmp    dword ptr [ebp - 14h], eax
        je     SHORT loc_1_2650
loc_1_2627:
        mov    cl, byte ptr [ebp - 2]
        mov    edx, 0ffh
        add    cl, al
        inc    edi
        shl    cl, 3
        inc    eax
        shl    edx, cl
        mov    ebx, edx
        not    ebx
        and    dword ptr [ebp - 8], ebx
        xor    ebx, ebx
        mov    bl, byte ptr [edi - 1]
        shl    ebx, cl
        and    ebx, edx
        or     dword ptr [ebp - 8], ebx
        cmp    dword ptr [ebp - 14h], eax
        ja     SHORT loc_1_2627
loc_1_2650:
        test   byte ptr [ebp - 1dh], 8
        je     SHORT loc_1_266F
        xor    ecx, ecx
        mov    al, byte ptr [esi + 144h]
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 5]
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 40h
        out    dx, al
loc_1_266F:
        mov    edx, dword ptr [ebp - 10h]
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [edx], eax
        test   byte ptr [esi + 0c8h], 8
        je     SHORT loc_1_2696
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0bfh
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 145h]
        out    dx, al
loc_1_2696:
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 0ch], edi
        add    dword ptr [ebp - 10h], 4
        sub    ecx, dword ptr [ebp - 14h]
        mov    edi, ecx
        shr    edi, 2
        test   edi, edi
        je     SHORT loc_1_2702
loc_1_26AC:
        test   byte ptr [esi + 0c8h], 8
        je     SHORT loc_1_26CE
        xor    ebx, ebx
        mov    al, byte ptr [esi + 144h]
        mov    bx, word ptr [esi + 4]
        lea    edx, [ebx + 5]
        out    dx, al
        mov    edx, ebx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 40h
        out    dx, al
loc_1_26CE:
        mov    eax, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 10h]
        mov    ebx, dword ptr [eax]
        mov    dword ptr [edx], ebx
        test   byte ptr [esi + 0c8h], 8
        je     SHORT loc_1_26F7
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0bfh
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 145h]
        out    dx, al
loc_1_26F7:
        add    dword ptr [ebp - 10h], 4
        add    dword ptr [ebp - 0ch], 4
        dec    edi
        jne    SHORT loc_1_26AC
loc_1_2702:
        test   cl, 3
        je     SHORT loc_1_277E
        and    cl, 3
        mov    edi, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], 0
        mov    edx, 0
        je     SHORT loc_1_2735
        xor    eax, eax
        mov    al, cl
loc_1_271F:
        lea    ecx, [edx*8]
        xor    ebx, ebx
        mov    bl, byte ptr [edi]
        inc    edi
        shl    ebx, cl
        inc    edx
        or     dword ptr [ebp - 8], ebx
        cmp    edx, eax
        jb     SHORT loc_1_271F
loc_1_2735:
        test   byte ptr [esi + 0c8h], 8
        je     SHORT loc_1_2757
        xor    ecx, ecx
        mov    al, byte ptr [esi + 144h]
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 5]
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 40h
        out    dx, al
loc_1_2757:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 10h]
        mov    dword ptr [edx], eax
        test   byte ptr [esi + 0c8h], 8
        je     SHORT loc_1_277E
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0bfh
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 145h]
        out    dx, al
loc_1_277E:
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_2827
loc_1_2789:
        cmp    byte ptr [esi + 146h], 0
        je     SHORT loc_1_27D8
        lea    edi, [esi + 0d5h]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_1_1C62
        push   1
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        mov    edx, dword ptr [ebp - 1ch]
        push   edx
        push   esi
        call   sub_1_2C10
        cmp    byte ptr [esi + 307h], 0
        je     SHORT loc_1_27CA
        cmp    byte ptr [esi + 308h], 0
        jne    SHORT loc_1_2827
loc_1_27CA:
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_1_1C62
        jmp    SHORT loc_1_2827
loc_1_27D8:
        test   byte ptr [ebp - 1dh], 8
        je     SHORT loc_1_27F7
        xor    ecx, ecx
        mov    al, byte ptr [esi + 144h]
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 5]
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     al, 40h
        out    dx, al
loc_1_27F7:
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 1ch]
        push   ecx
        push   edx
        call   sub_1_2050
        test   byte ptr [esi + 0c8h], 8
        je     SHORT loc_1_2827
        xor    edx, edx
        mov    al, byte ptr [ebp - 1]
        mov    dx, word ptr [esi + 4]
        and    al, 0bfh
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 145h]
        out    dx, al
loc_1_2827:
        mov    eax, dword ptr [ebp - 18h]
        add    dword ptr [ebp - 1ch], eax
        add    dword ptr [ebp - 28h], eax
        cmp    byte ptr [ebp - 3], 0
        je     SHORT loc_1_2890
        mov    eax, dword ptr [ebp - 28h]
        cmp    dword ptr [ebp - 2ch], eax
        jae    SHORT loc_1_2890
        movsx  eax, word ptr [esi + 0eeh]
        mov    dword ptr [esi + eax*4 + 0fdh], 2
        cmp    word ptr [esi + 0f0h], -1
        jne    SHORT loc_1_2890
        mov    ax, word ptr [esi + 0eeh]
        cmp    word ptr [esi + 0f2h], -1
        mov    word ptr [esi + 0f0h], ax
        jne    SHORT loc_1_2890
        or     byte ptr [esi + 48h], 20h
        movsx  eax, ax
        mov    ecx, dword ptr [ebp - 34h]
        push   esi
        mov    dword ptr [esi + eax*4 + 0f5h], ecx
        call   sub_1_2490
        mov    dword ptr [ebp - 2ch], 0ffffffffh
loc_1_2890:
        lea    eax, [ebp - 24h]
        mov    ecx, dword ptr [ebp - 24h]
        push   eax
        push   ecx
        call   sub_1_1CE0
        cmp    dword ptr [ebp - 24h], 0
        jne    NEAR PTR loc_1_25BA
loc_1_28A7:
        cmp    byte ptr [ebp - 3], 0
        je     SHORT loc_1_28B9
        xor    edx, edx
        mov    al, 8
        mov    dx, word ptr [esi + 4]
        add    edx, 10h
        out    dx, al
loc_1_28B9:
        cmp    dword ptr [ebp - 28h], 3ch
        jae    SHORT loc_1_28DA
        movsx  eax, word ptr [esi + 0eeh]
        pop    edi
        mov    dword ptr [esi + eax*4 + 0f5h], 3ch
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_28DA:
        movsx  ecx, word ptr [esi + 0eeh]
        mov    eax, dword ptr [ebp - 28h]
        pop    edi
        mov    dword ptr [esi + ecx*4 + 0f5h], eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2900:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, word ptr [esi + 4]
        push   esi
        add    edx, 18h
        mov    al, byte ptr [esi + 3ch]
        out    dx, al
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 0d5h]
        push   eax
        call   sub_1_1C62
        mov    cl, byte ptr [esi + 0edh]
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        mov    byte ptr [ebp - 4], cl
        add    edx, 13h
        in     al, dx
        lea    ecx, [eax + 1]
        cmp    byte ptr [esi + 13bh], cl
        ja     SHORT loc_1_294F
        mov    cl, byte ptr [esi + 13ah]
loc_1_294F:
        cmp    cl, byte ptr [ebp - 4]
        je     NEAR PTR loc_1_2B3F
loc_1_2958:
        mov    ebx, dword ptr [esi + 0c5h]
        mov    eax, ebx
        and    al, 7
        cmp    al, 1
        jne    SHORT loc_1_29CD
        xor    edx, edx
        mov    edi, ecx
        mov    dx, word ptr [esi + 38h]
        and    edi, 0ffh
        imul   edi, edx
        add    edi, dword ptr [esi + 1ch]
        mov    al, byte ptr [edi]
        mov    byte ptr [ebp - 2], al
        mov    cl, byte ptr [edi + 1]
        mov    dword ptr [ebp - 0ch], 0
        mov    byte ptr [ebp - 1], cl
        mov    bx, word ptr [edi + 2]
        mov    word ptr [ebp - 0ch], bx
        sub    dword ptr [ebp - 0ch], edi
        jns    SHORT loc_1_29BB
        xor    ecx, ecx
        xor    eax, eax
        mov    cl, byte ptr [esi + 13bh]
        mov    al, byte ptr [esi + 13ah]
        sub    ecx, eax
        xor    ebx, ebx
        mov    bl, byte ptr [ebp - 1]
        add    ecx, ebx
        dec    ecx
        imul   ecx, edx
        sub    ecx, edi
        mov    dword ptr [ebp - 0ch], ecx
loc_1_29BB:
        cmp    byte ptr [ebp - 0ch], 0fch
        jbe    NEAR PTR loc_1_2A95
        add    dword ptr [ebp - 0ch], edx
        jmp    NEAR PTR loc_1_2A95
loc_1_29CD:
        xor    edi, edi
        xor    eax, eax
        mov    di, word ptr [esi + 38h]
        mov    al, cl
        mov    dword ptr [ebp - 0ch], 0
        imul   edi, eax
        add    edi, dword ptr [esi + 1ch]
        cmp    byte ptr [esi + 146h], 0
        je     SHORT loc_1_2A34
        push   esi
        lea    eax, [esi + 0d5h]
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_1C62
        push   0
        lea    eax, [ebp - 0ch]
        push   2
        lea    ecx, [edi + 2]
        push   eax
        push   ecx
        push   esi
        call   sub_1_2C10
        push   0
        lea    ecx, [ebp - 2]
        push   1
        push   ecx
        push   edi
        push   esi
        call   sub_1_2C10
        push   0
        lea    ecx, [ebp - 1]
        push   1
        lea    edx, [edi + 1]
        push   ecx
        push   edx
        push   esi
        call   sub_1_2C10
        jmp    SHORT loc_1_2A82
loc_1_2A34:
        and    ebx, 8000000h
        je     SHORT loc_1_2A55
        xor    ecx, ecx
        mov    al, byte ptr [esi + 144h]
        mov    cx, word ptr [esi + 4]
        lea    edx, [ecx + 5]
        out    dx, al
        mov    edx, ecx
        in     al, dx
        mov    byte ptr [ebp - 3], al
        or     al, 40h
        out    dx, al
loc_1_2A55:
        mov    ax, word ptr [edi + 2]
        test   ebx, ebx
        mov    word ptr [ebp - 0ch], ax
        mov    cl, byte ptr [edi]
        mov    byte ptr [ebp - 2], cl
        mov    dl, byte ptr [edi + 1]
        mov    byte ptr [ebp - 1], dl
        je     SHORT loc_1_2A82
        xor    edx, edx
        mov    al, byte ptr [ebp - 3]
        mov    dx, word ptr [esi + 4]
        and    al, 0bfh
        out    dx, al
        add    edx, 5
        mov    al, byte ptr [esi + 145h]
        out    dx, al
loc_1_2A82:
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [esi + 13ah], al
        ja     SHORT loc_1_2B05
        cmp    byte ptr [esi + 13bh], al
        jb     SHORT loc_1_2B05
loc_1_2A95:
        test   byte ptr [ebp - 2], 1
        je     SHORT loc_1_2ABB
        add    edi, 4
        mov    eax, dword ptr [ebp - 0ch]
        sub    eax, 4
        push   esi
        push   eax
        mov    dword ptr [esi + 134h], edi
        mov    dword ptr [esi + 140h], eax
        call   sub_1_2F40
        mov    word ptr [ebp - 6], ax
loc_1_2ABB:
        mov    cl, byte ptr [ebp - 1]
        mov    al, byte ptr [esi + 13bh]
        cmp    cl, al
        jbe    SHORT loc_1_2AD3
        mov    cl, byte ptr [esi + 13ah]
        sub    cl, al
        add    cl, byte ptr [ebp - 1]
loc_1_2AD3:
        cmp    byte ptr [esi + 13ah], cl
        jne    SHORT loc_1_2AE2
        and    eax, 0ffh
        jmp    SHORT loc_1_2AE6
loc_1_2AE2:
        xor    eax, eax
        mov    al, cl
loc_1_2AE6:
        dec    eax
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 13h
        out    dx, al
        cmp    word ptr [ebp - 6], 5
        jne    NEAR PTR loc_1_294F
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2B05:
        mov    cl, byte ptr [ebp - 4]
        mov    dl, byte ptr [esi + 13bh]
        mov    al, cl
        mov    byte ptr [ebp - 1], cl
        cmp    cl, dl
        jbe    SHORT loc_1_2B21
        mov    al, byte ptr [esi + 13ah]
        sub    al, dl
        add    al, cl
loc_1_2B21:
        cmp    byte ptr [esi + 13ah], al
        jne    SHORT loc_1_2B2F
        xor    eax, eax
        mov    al, dl
        jmp    SHORT loc_1_2B34
loc_1_2B2F:
        and    eax, 0ffh
loc_1_2B34:
        dec    eax
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        add    edx, 13h
        out    dx, al
loc_1_2B3F:
        xor    edx, edx
        mov    al, 1
        mov    dx, word ptr [esi + 4]
        push   esi
        add    edx, 17h
        db  068h, 000h, 000h, 000h, 000h
        out    dx, al
        lea    eax, [esi + 0d5h]
        push   eax
        call   sub_1_1C62
        mov    cl, byte ptr [esi + 0edh]
        xor    edx, edx
        mov    dx, word ptr [esi + 4]
        mov    byte ptr [ebp - 4], cl
        add    edx, 13h
        in     al, dx
        lea    ecx, [eax + 1]
        cmp    byte ptr [esi + 13bh], cl
        ja     SHORT loc_1_2B81
        mov    cl, byte ptr [esi + 13ah]
loc_1_2B81:
        xor    eax, eax
        xor    edx, edx
        mov    ax, word ptr [esi + 38h]
        mov    dl, cl
        imul   eax, edx
        add    eax, dword ptr [esi + 1ch]
        mov    dword ptr [esi + 13ch], eax
        cmp    cl, byte ptr [ebp - 4]
        jne    NEAR PTR loc_1_2958
        cmp    byte ptr [esi + 138h], 0
        je     SHORT loc_1_2BFE
        xor    ecx, ecx
        mov    al, 10h
        test   dword ptr [esi + 0c5h], 8100000h
        mov    cx, word ptr [esi + 4]
        mov    byte ptr [esi + 138h], 0
        lea    edx, [ecx + 17h]
        out    dx, al
        je     SHORT loc_1_2BD6
        lea    edx, [ecx + 13h]
        pop    edi
        in     al, dx
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, al
        pop    ebp
        ret    4
loc_1_2BD6:
        mov    al, 0ffh
        out    dx, al
        lea    edx, [ecx + 1dh]
        xor    al, al
        out    dx, al
        cmp    byte ptr [esi + 139h], 0
        mov    al, 22h
        je     SHORT loc_1_2BF3
        mov    byte ptr [esi + 139h], 0
        mov    al, 26h
loc_1_2BF3:
        mov    byte ptr [esi + 31ah], 0
        lea    edx, [ecx + 10h]
        out    dx, al
loc_1_2BFE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2C10:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 14h]
        test   ebx, ebx
        jne    SHORT loc_1_2C26
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
loc_1_2C26:
        mov    esi, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cx, word ptr [esi + 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 10h]
        lea    edx, [ecx + 7]
        out    dx, al
        test   edi, 1
        mov    al, ah
        out    dx, al
        je     SHORT loc_1_2C57
        cmp    byte ptr [ebp + 18h], 0
        lea    edx, [ecx + 2]
        je     SHORT loc_1_2C52
        mov    al, byte ptr [edi]
        out    dx, al
        jmp    SHORT loc_1_2C55
loc_1_2C52:
        in     al, dx
        mov    byte ptr [edi], al
loc_1_2C55:
        dec    ebx
        inc    edi
loc_1_2C57:
        test   ebx, ebx
        jne    SHORT loc_1_2C64
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
loc_1_2C64:
        cmp    byte ptr [ebp + 18h], 0
        je     SHORT loc_1_2CC8
        cmp    ebx, 1
        jbe    SHORT loc_1_2CAE
        mov    eax, ebx
        shr    eax, 1
        push   eax
        push   edi
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        add    eax, 2
        push   eax
        call   sub_1_1B1C
        test   bl, 1
        je     NEAR PTR loc_1_2D19
        and    ebx, 0fffffffeh
        push   1
        add    edi, ebx
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   edi
        add    eax, 2
        push   eax
        call   sub_1_1B08
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
loc_1_2CAE:
        push   ebx
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   edi
        add    eax, 2
        push   eax
        call   sub_1_1B08
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
loc_1_2CC8:
        cmp    ebx, 1
        jbe    SHORT loc_1_2D08
        mov    eax, ebx
        shr    eax, 1
        push   eax
        push   edi
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        add    eax, 2
        push   eax
        call   sub_1_1B59
        test   bl, 1
        je     SHORT loc_1_2D19
        and    ebx, 0fffffffeh
        push   1
        add    edi, ebx
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   edi
        add    eax, 2
        push   eax
        call   sub_1_1B45
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
loc_1_2D08:
        push   ebx
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   edi
        add    eax, 2
        push   eax
        call   sub_1_1B45
loc_1_2D19:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh
        db  04Ch, 024h, 00Ch, 066h, 083h, 07Ch, 024h, 008h, 001h, 08Bh, 0ECh, 08Bh, 091h, 009h, 001h, 000h
        db  000h, 08Bh, 081h, 015h, 001h, 000h, 000h, 0C7h, 081h, 009h, 001h, 000h, 000h, 000h, 000h, 000h
        db  000h, 08Bh, 000h, 08Bh, 089h, 005h, 001h, 000h, 000h, 050h, 0B8h, 000h, 000h, 000h, 000h, 083h
        db  0D0h, 0FFh, 025h, 001h, 000h, 000h, 0C0h, 050h, 052h, 051h, 0E8h, 047h, 0EEh, 0FFh, 0FFh, 066h
        db  033h, 0C0h, 05Dh, 0C2h, 008h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2D80:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 16fh]
        test   ecx, ecx
        jne    SHORT loc_1_2D9D
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2D9D:
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [esi + 16fh], eax
        test   eax, eax
        jne    SHORT loc_1_2DB4
        mov    dword ptr [esi + 173h], 0
loc_1_2DB4:
        cmp    byte ptr [ecx + 2ch], 0
        mov    edi, dword ptr [ecx + 28h]
        je     SHORT loc_1_2DEF
        cmp    dword ptr [esi + 177h], 0
        jne    SHORT loc_1_2DD4
        mov    dword ptr [esi + 17bh], ecx
        mov    dword ptr [esi + 177h], ecx
        jmp    SHORT loc_1_2DE3
loc_1_2DD4:
        mov    eax, dword ptr [esi + 17bh]
        mov    dword ptr [eax + 24h], ecx
        mov    dword ptr [esi + 17bh], ecx
loc_1_2DE3:
        mov    dword ptr [ecx + 24h], 0
        jmp    NEAR PTR loc_1_2E91
loc_1_2DEF:
        mov    ax, word ptr [ebp + 8]
        test   ax, ax
        jne    SHORT loc_1_2E00
        inc    dword ptr [esi + 18ch]
        jmp    SHORT loc_1_2E06
loc_1_2E00:
        inc    dword ptr [esi + 194h]
loc_1_2E06:
        cmp    ax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0c0000001h
        push   eax
        push   ecx
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_1BB2
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_2E91
        mov    eax, dword ptr [ebx + 15bh]
        cmp    edi, eax
        jne    SHORT loc_1_2E3E
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 15bh], eax
        jmp    SHORT loc_1_2E51
loc_1_2E3E:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_2E4B
loc_1_2E43:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_2E43
loc_1_2E4B:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_2E51:
        mov    eax, dword ptr [ebx + 1f7h]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_2E62
        push   ebx
        call   0
loc_1_2E62:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1BAC
        push   0
        push   75h
        push   edi
        call   sub_1_1C74
        cmp    dword ptr [ebx + 15bh], 0
        jne    SHORT loc_1_2E91
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 0d5h
        push   ebx
        call   sub_1_1C62
loc_1_2E91:
        mov    edi, dword ptr [esi + 167h]
        test   edi, edi
        je     NEAR PTR loc_1_2F35
        cmp    byte ptr [esi + 1edh], 0
        jne    NEAR PTR loc_1_2F35
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [esi + 167h], eax
        test   eax, eax
        jne    SHORT loc_1_2EC3
        mov    dword ptr [esi + 16bh], 0
loc_1_2EC3:
        push   esi
        push   edi
        db  0E8h, 096h, 0F6h, 0FFh, 0FFh
        cmp    ax, 6
        jne    SHORT loc_1_2F03
loc_1_2ED0:
        mov    eax, dword ptr [esi + 167h]
        test   eax, eax
        je     SHORT loc_1_2EED
        mov    dword ptr [edi + 24h], eax
        xor    ax, ax
        mov    dword ptr [esi + 167h], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2EED:
        xor    ax, ax
        mov    dword ptr [esi + 167h], edi
        mov    dword ptr [esi + 16bh], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2F03:
        test   ax, ax
        jne    SHORT loc_1_2F35
        cmp    dword ptr [esi + 16fh], 0
        jne    SHORT loc_1_2F1F
        mov    dword ptr [esi + 173h], edi
        mov    dword ptr [esi + 16fh], edi
        jmp    SHORT loc_1_2F2E
loc_1_2F1F:
        mov    eax, dword ptr [esi + 173h]
        mov    dword ptr [eax + 24h], edi
        mov    dword ptr [esi + 173h], edi
loc_1_2F2E:
        mov    dword ptr [edi + 24h], 0
loc_1_2F35:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_1_2F40:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [edi + 183h], 1
        mov    dword ptr [edi + 17fh], 0
        mov    ebx, dword ptr [edi + 1f7h]
        add    ebx, 0eh
        cmp    ebx, esi
        jb     SHORT loc_1_2F6C
        mov    ebx, esi
loc_1_2F6C:
        push   edi
        lea    eax, [edi + 20bh]
        push   eax
        push   0
        push   ebx
        db  0E8h, 0C4h, 0F9h, 0FFh, 0FFh
        test   ax, ax
        je     SHORT loc_1_2F8B
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2F8B:
        inc    dword ptr [edi + 190h]
        cmp    esi, 0eh
        jae    SHORT loc_1_2FC1
        cmp    esi, 6
        jb     SHORT loc_1_2FE7
        push   0
        lea    eax, [edi + 20bh]
        push   0
        mov    edx, dword ptr [edi + 147h]
        push   0
        push   esi
        push   eax
        push   eax
        push   edi
        push   edx
        call   sub_1_1D60
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_2FC1:
        sub    esi, 0eh
        sub    ebx, 0eh
        push   esi
        lea    eax, [edi + 219h]
        push   ebx
        lea    ecx, [edi + 20bh]
        push   eax
        push   0eh
        push   ecx
        mov    eax, dword ptr [edi + 147h]
        push   ecx
        push   edi
        push   eax
        call   sub_1_1D60
loc_1_2FE7:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3000:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    word ptr [edi + 40h], 0
        test   byte ptr [edi + 0c9h], 8
        jne    SHORT loc_1_301F
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_301F:
        mov    ecx, dword ptr [edi + 0c5h]
        test   ecx, 8000000h
        jne    SHORT loc_1_3035
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_3035:
        mov    ax, word ptr [edi + 3eh]
        test   ax, ax
        je     SHORT loc_1_3070
        test   al, 1
        jne    SHORT loc_1_304A
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_304A:
        test   al, 4
        je     SHORT loc_1_305D
        and    ax, 2
        cmp    ax, 1
        sbb    esi, esi
        add    esi, 3
        jmp    SHORT loc_1_3096
loc_1_305D:
        test   al, 2
        je     SHORT loc_1_3068
        mov    esi, 1
        jmp    SHORT loc_1_3096
loc_1_3068:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_3070:
        and    cl, 7
        test   cl, 3
        je     SHORT loc_1_3080
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_3080:
        test   cl, 2
        je     SHORT loc_1_308C
        mov    esi, 2
        jmp    SHORT loc_1_3096
loc_1_308C:
        test   cl, 4
        je     SHORT loc_1_30CD
        mov    esi, 3
loc_1_3096:
        test   esi, 1
        je     SHORT loc_1_30B1
        push   edi
        call   sub_1_30E0
        cmp    esi, 1
        jne    SHORT loc_1_30B1
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_30B1:
        test   byte ptr [edi + 40h], 2
        je     SHORT loc_1_30BF
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_30BF:
        push   edi
        call   sub_1_3180
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_30CD:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_30E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    cx, word ptr [esi + 4]
        mov    al, 80h
        lea    edi, [ecx + 10h]
        movsx  ecx, cx
        mov    dx, di
        out    dx, al
        lea    edx, [ecx + 1dh]
        in     al, dx
        mov    dx, di
        mov    byte ptr [ebp - 2], al
        xor    al, al
        out    dx, al
        lea    edx, [ecx + 1dh]
        out    dx, al
        lea    edx, [ecx + 4]
        in     al, dx
        mov    bl, al
        mov    byte ptr [ebp - 1], al
        and    bl, 0c3h
        or     bl, 80h
        mov    al, bl
        out    dx, al
        lea    edx, [ecx + 0dh]
        in     al, dx
        and    al, 0fdh
        or     al, 1
        out    dx, al
        lea    edx, [ecx + 4]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    dx, di
        mov    al, 0c0h
        out    dx, al
        lea    edx, [ecx + 1fh]
        in     al, dx
        mov    dx, di
        mov    byte ptr [ebp - 3], al
        xor    al, al
        out    dx, al
        test   byte ptr [ebp - 3], 4
        je     SHORT loc_1_314E
        or     byte ptr [esi + 40h], 2
        jmp    SHORT loc_1_3166
loc_1_314E:
        lea    edx, [ecx + 4]
        mov    al, bl
        out    dx, al
        lea    edx, [ecx + 0dh]
        in     al, dx
        or     byte ptr [esi + 40h], 1
        or     al, 3
        out    dx, al
        lea    edx, [ecx + 4]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
loc_1_3166:
        mov    dx, di
        xor    al, al
        out    dx, al
        lea    edx, [ecx + 1dh]
        mov    al, byte ptr [ebp - 2]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, al
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_3180:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, word ptr [esi + 4]
        lea    eax, [edi + 10h]
        movsx  ebx, di
        mov    dx, ax
        mov    word ptr [ebp - 6], ax
        mov    al, 80h
        out    dx, al
        lea    edx, [ebx + 1dh]
        in     al, dx
        mov    dx, word ptr [ebp - 6]
        mov    byte ptr [ebp - 3], al
        xor    al, al
        out    dx, al
        lea    eax, [ebx + 1dh]
        mov    edx, eax
        xor    al, al
        out    dx, al
        lea    edx, [ebx + 4]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    al, 0c3h
        or     al, 80h
        out    dx, al
        lea    edx, [ebx + 0dh]
        mov    byte ptr [ebp - 0ch], al
        in     al, dx
        and    al, 0fdh
        mov    ecx, dword ptr [esi + 1ch]
        or     al, 1
        out    dx, al
        cmp    byte ptr [esi + 146h], 0
        lea    eax, [ebx + 4]
        mov    edx, eax
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        je     SHORT loc_1_31F5
        push   1
        push   66h
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   esi
        call   sub_1_2C10
        jmp    SHORT loc_1_323F
loc_1_31F5:
        test   byte ptr [esi + 0c8h], 8
        mov    edx, ebx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        je     SHORT loc_1_3217
        lea    edx, [ebx + 5]
        mov    al, byte ptr [esi + 144h]
        out    dx, al
        mov    edx, ebx
        in     al, dx
        mov    byte ptr [ebp - 2], al
        or     al, 40h
        out    dx, al
loc_1_3217:
        push   66h
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_1_2050
        test   byte ptr [esi + 0c8h], 8
        je     SHORT loc_1_323F
        mov    al, byte ptr [ebp - 2]
        mov    edx, ebx
        and    al, 0bfh
        out    dx, al
        lea    edx, [ebx + 5]
        mov    al, byte ptr [esi + 145h]
        out    dx, al
loc_1_323F:
        lea    edx, [ebx + 17h]
        mov    al, 0ffh
        out    dx, al
        push   400h
        push   0
        push   edi
        call   sub_1_32D0
        xor    edi, edi
loc_1_3254:
        push   3e8h
        call   sub_1_1C68
        lea    edx, [ebx + 17h]
        in     al, dx
        test   al, 7fh
        jne    SHORT loc_1_326F
        inc    edi
        cmp    edi, 300h
        jl     SHORT loc_1_3254
loc_1_326F:
        mov    cx, word ptr [esi + 40h]
        lea    edx, [ebx + 14h]
        in     al, dx
        and    cx, 0fff8h
        test   al, 10h
        mov    word ptr [esi + 40h], cx
        jne    SHORT loc_1_328D
        or     cl, 4
        mov    word ptr [esi + 40h], cx
        jmp    SHORT loc_1_32AB
loc_1_328D:
        or     cl, 1
        lea    eax, [ebx + 4]
        mov    word ptr [esi + 40h], cx
        mov    edx, eax
        mov    al, byte ptr [ebp - 0ch]
        out    dx, al
        lea    edx, [ebx + 0dh]
        in     al, dx
        or     al, 3
        out    dx, al
        lea    edx, [ebx + 4]
        mov    al, byte ptr [ebp - 1]
        out    dx, al
loc_1_32AB:
        mov    dx, word ptr [ebp - 6]
        xor    al, al
        out    dx, al
        lea    edx, [ebx + 1dh]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_32D0:
        push   ebp
        mov    cx, word ptr [esp + 8]
        mov    ebp, esp
        push   esi
        lea    esi, [ecx + 10h]
        movsx  ecx, cx
        mov    dx, si
        in     al, dx
        and    eax, 0ffh
        and    al, 3bh
        out    dx, al
        lea    edx, [ecx + 14h]
        mov    al, byte ptr [ebp + 0ch]
        out    dx, al
        lea    edx, [ecx + 15h]
        mov    eax, dword ptr [ebp + 10h]
        out    dx, al
        lea    edx, [ecx + 16h]
        mov    al, ah
        out    dx, al
        mov    dx, si
        xor    ecx, ecx
        in     al, dx
        mov    cl, al
        mov    al, cl
        pop    esi
        or     al, 2
        pop    ebp
        out    dx, al
        mov    al, cl
        or     al, 4
        out    dx, al
        xor    eax, eax
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 040h, 028h, 023h, 029h, 020h, 053h, 04Dh, 043h
        db  038h, 030h, 030h, 030h, 057h, 02Eh, 033h, 038h, 036h, 020h, 076h, 065h, 072h, 073h, 069h, 06Fh
        db  06Eh, 020h, 031h, 02Eh, 031h, 031h, 064h, 020h, 030h, 031h, 02Fh, 031h, 038h, 02Fh, 039h, 035h
        db  00Ah, 043h, 06Fh, 070h, 079h, 072h, 069h, 067h, 068h, 074h, 020h, 028h, 043h, 029h, 020h, 053h
        db  074h, 061h, 06Eh, 064h, 061h, 072h, 064h, 020h, 04Dh, 069h, 063h, 072h, 06Fh, 073h, 079h, 073h
        db  074h, 065h, 06Dh, 073h, 020h, 043h, 06Fh, 072h, 070h, 02Ch, 020h, 031h, 039h, 039h, 033h, 000h
        db  040h, 028h, 023h, 029h, 020h, 055h, 04Dh, 05Fh, 038h, 034h, 031h, 036h, 05Fh, 04Eh, 044h, 049h
        db  053h, 033h, 05Fh, 073h, 032h, 02Eh, 032h, 05Fh, 076h, 031h, 02Eh, 031h, 031h, 064h, 000h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_33F4:
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
loc_1_3450:
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
loc_1_3470:
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
        cmp    byte ptr [eax], dh
        xor    byte ptr [eax], dh
        push   edi
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_34D8:
        push   ebx
        dec    ebp
        inc    ebx
        cmp    byte ptr [eax], dh
        xor    byte ptr [eax], dh
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
loc_1_34EC:
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
loc_1_3504:
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
loc_1_3540:
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
        inc    edx
        db  075h, 073h
        push   esp
        db  079h, 070h
        add    byte ptr gs:[edx + 75h], al
        jae    SHORT loc_1_35AC
        jns    SHORT loc_1_35CA
        add    byte ptr gs:[ebp + 65h], cl
        imul   esp, dword ptr fs:[ecx + 54h], 657079h
        add    byte ptr [eax], al
        dec    ebp
        imul   esp, dword ptr fs:[ecx + 54h], 657079h
        add    byte ptr [eax], al
        dec    ebp
        popad
        db  078h, 069h
        insd
        db  075h, 06Dh
        push   eax
        popad
        arpl   word ptr [ebx + 65h], bp
        db  074h, 053h
        imul   edi, dword ptr [edx + 65h], 4d000000h
        popad
        db  078h, 069h
        insd
        db  075h, 06Dh
        push   eax
        popad
        arpl   word ptr [ebx + 65h], bp
        db  074h, 053h
        imul   edi, dword ptr [edx + 65h], 53000000h
        dec    ebp
        inc    ebx
        and    byte ptr [ecx + 64h], al
        popad
        jo     SHORT loc_1_3619
        db  065h, 072h, 02Eh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_35AC:
        add    byte ptr [edx], al
        add    eax, dword ptr [eax + 6]
        add    al, byte ptr [ebx]
        add    eax, 0f0b0a07h
loc_1_35BC:
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    al, al
        add    byte ptr [eax], al
        add    byte ptr [ebx + 4dh], dl
loc_1_35CA:
        inc    ebx
        and    byte ptr [ecx + 75h], al
        db  074h, 06Fh
        and    byte ptr [ebp + 65h], cl
        imul   esp, dword ptr fs:[ecx + 20h], 65746544h
        arpl   word ptr [eax + 54h], si
        db  065h, 073h, 074h
        and    byte ptr [esi + 72h], al
        popad
        insd
        and    byte ptr cs:[eax + ebp*2 + 61h], dl
        outsb
        imul   esp, dword ptr [eax], 79h
        outsd
        jne    SHORT loc_1_3614
        outsw
        jb     SHORT loc_1_3618
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
loc_1_3614:
        pop    edx
        push   eax
        dec    ebp
        inc    edx
loc_1_3618:
        inc    edi
loc_1_3619:
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
