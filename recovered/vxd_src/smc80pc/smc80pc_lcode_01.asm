; smc80pc_lcode_01.asm -- SMC80PC object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
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
        call   sub_1_1B48
        cmp    dword ptr [ebp - 8], 0ch
        jb     NEAR PTR loc_1_01B7
        cmp    dword ptr [ebp - 8], 5eah
        ja     NEAR PTR loc_1_01B7
        cmp    byte ptr [esi + 172h], 0
        je     SHORT loc_1_0056
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0056:
        cmp    byte ptr [esi + 174h], 0
        je     SHORT loc_1_006D
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_006D:
        cmp    byte ptr [ebx + 20h], 0
        je     SHORT loc_1_0081
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0081:
        mov    eax, dword ptr [ebp - 4]
        push   edi
        push   ebx
        inc    dword ptr [ebx + 1ch]
        push   esi
        inc    dword ptr [esi + 10dh]
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [eax], 0
        call   sub_1_0210
        mov    ecx, dword ptr [ebp - 4]
        cmp    byte ptr [ecx + 9], 0
        je     SHORT loc_1_00D5
        cmp    dword ptr [esi + 125h], 0
        jne    SHORT loc_1_00C1
        mov    dword ptr [esi + 129h], edi
        mov    dword ptr [esi + 125h], edi
        jmp    NEAR PTR loc_1_015F
loc_1_00C1:
        mov    eax, dword ptr [esi + 129h]
        mov    dword ptr [eax + 24h], edi
        mov    dword ptr [esi + 129h], edi
        jmp    NEAR PTR loc_1_015F
loc_1_00D5:
        cmp    dword ptr [esi + 115h], 0
        je     SHORT loc_1_00E9
        mov    eax, dword ptr [esi + 119h]
        mov    dword ptr [eax + 24h], edi
        jmp    SHORT loc_1_0152
loc_1_00E9:
        cmp    dword ptr [esi + 11dh], 0
        jne    SHORT loc_1_0102
        xor    ebx, ebx
        mov    dword ptr [esi + 121h], edi
        mov    dword ptr [esi + 11dh], edi
        jmp    SHORT loc_1_0111
loc_1_0102:
        mov    ebx, dword ptr [esi + 121h]
        mov    dword ptr [ebx + 24h], edi
        mov    dword ptr [esi + 121h], edi
loc_1_0111:
        mov    byte ptr [esi + 2a1h], 0
        push   esi
        push   edi
        call   sub_1_13B0
        cmp    ax, 6
        jne    SHORT loc_1_015F
        test   ebx, ebx
        jne    SHORT loc_1_013F
        mov    dword ptr [esi + 121h], 0
        mov    dword ptr [esi + 11dh], 0
        jmp    SHORT loc_1_014C
loc_1_013F:
        mov    dword ptr [esi + 121h], ebx
        mov    dword ptr [ebx + 24h], 0
loc_1_014C:
        mov    dword ptr [esi + 115h], edi
loc_1_0152:
        mov    dword ptr [esi + 119h], edi
        mov    byte ptr [esi + 2a1h], 0
loc_1_015F:
        mov    edi, 103h
        mov    eax, dword ptr [esi + 10dh]
        dec    eax
        mov    dword ptr [esi + 10dh], eax
        jne    SHORT loc_1_01AC
        cmp    byte ptr [esi + 173h], 0
        je     SHORT loc_1_0187
        lea    eax, [esi + 28dh]
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_019B
loc_1_0187:
        cmp    dword ptr [esi + 125h], 0
        je     SHORT loc_1_01AC
        lea    eax, [esi + 28dh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_01AC
loc_1_019B:
        push   0
        add    esi, 181h
        push   esi
        mov    byte ptr [eax], 1
        call   sub_1_1AFA
loc_1_01AC:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_01B7:
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_01D0:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        push   ebx
        push   esi
        xor    esi, esi
        cmp    edx, esi
        je     SHORT loc_1_01F2
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
loc_1_01E5:
        mov    bl, byte ptr [eax + esi]
        cmp    byte ptr [ecx + esi], bl
        jne    SHORT loc_1_01FA
        inc    esi
        cmp    esi, edx
        jb     SHORT loc_1_01E5
loc_1_01F2:
        xor    eax, eax
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
loc_1_01FA:
        mov    eax, 0ffffffffh
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0210:
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
        call   sub_1_09F0
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, byte ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 0f9h]
        cmp    byte ptr [ecx + 3ch], dl
        mov    ebx, dword ptr [eax + 0ch]
        jne    SHORT loc_1_0263
        push   6
        lea    eax, [ebp - 8]
        push   eax
        add    ecx, 38h
        push   ecx
        call   sub_1_01D0
        test   eax, eax
        jne    SHORT loc_1_0263
        mov    eax, ebx
        and    al, 1
        mov    byte ptr [esi + 9], al
loc_1_0263:
        mov    eax, dword ptr [ebp + 0ch]
        test   byte ptr [eax + 71h], 2
        je     SHORT loc_1_0278
        mov    byte ptr [esi + 8], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0278:
        test   bl, 20h
        je     SHORT loc_1_0289
        mov    byte ptr [esi + 8], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0289:
        db  0A0h, 004h, 000h, 000h, 000h
        cmp    byte ptr [ebp - 4], al
        jne    SHORT loc_1_02B5
        push   6
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_01D0
        test   eax, eax
        jne    SHORT loc_1_02B5
        and    bl, 8
        mov    byte ptr [esi + 8], bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_02B5:
        test   byte ptr [ebp - 8], 1
        je     SHORT loc_1_02C9
        and    bl, 6
        mov    byte ptr [esi + 8], bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_02C9:
        cmp    byte ptr [esi + 9], 0
        je     SHORT loc_1_02DB
        mov    byte ptr [esi + 8], 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_02DB:
        mov    byte ptr [esi + 8], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_02F0:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 2eh]
        or     byte ptr [eax], 2
        mov    eax, dword ptr [ecx + 2eh]
        mov    byte ptr [eax + 10h], dl
        mov    eax, dword ptr [ecx + 2eh]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 10h]
        test   ecx, 0fffffff9h
        jne    SHORT loc_1_032A
        mov    cl, byte ptr [eax + 17h]
        test   cl, cl
        je     SHORT loc_1_032A
        cmp    cl, 0ffh
        je     SHORT loc_1_032A
        pop    ebp
        mov    byte ptr [eax + 1fh], dl
        mov    al, 1
        ret    4
loc_1_032A:
        xor    al, al
        pop    ebp
        ret    4
loc_1_0330:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 10dh]
        mov    byte ptr [esi + 290h], 1
loc_1_0347:
        cmp    byte ptr [esi + 28eh], bl
        jne    SHORT loc_1_035C
        mov    byte ptr [esi + 28eh], 1
        push   esi
        call   sub_1_09B0
loc_1_035C:
        cmp    byte ptr [esi + 28dh], bl
        jne    SHORT loc_1_0384
        mov    byte ptr [esi + 28dh], 1
        push   ebx
        push   ebx
        push   esi
        push   ebx
        call   sub_1_04B0
        cmp    dword ptr [esi + 125h], ebx
        je     SHORT loc_1_0384
        cmp    byte ptr [esi + 28dh], bl
        je     SHORT loc_1_0347
loc_1_0384:
        mov    eax, dword ptr [esi + 10dh]
        dec    eax
        mov    dword ptr [esi + 10dh], eax
        jne    SHORT loc_1_03CC
        cmp    byte ptr [esi + 173h], 0
        je     SHORT loc_1_03A5
        cmp    byte ptr [esi + 28dh], 0
        je     SHORT loc_1_03B7
loc_1_03A5:
        cmp    dword ptr [esi + 125h], 0
        je     SHORT loc_1_03CC
        cmp    byte ptr [esi + 28dh], 0
        jne    SHORT loc_1_03CC
loc_1_03B7:
        push   0
        lea    eax, [esi + 181h]
        mov    byte ptr [esi + 28dh], 1
        push   eax
        call   sub_1_1AFA
loc_1_03CC:
        or     byte ptr [esi + 0aeh], 3fh
        push   esi
        call   sub_1_1020
        mov    byte ptr [esi + 290h], 0
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_03F0:
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
        mov    dword ptr [esi + 12dh], ebx
        mov    byte ptr [esi + 131h], 1
        push   0
        push   ebx
        call   sub_1_1B48
        cmp    dword ptr [ebp - 4], 6
        jb     SHORT loc_1_049B
        mov    edi, dword ptr [esi + 17dh]
        mov    eax, dword ptr [ebp - 4]
        add    edi, 0eh
        cmp    edi, eax
        jb     SHORT loc_1_0434
        mov    edi, eax
loc_1_0434:
        push   edi
        lea    eax, [esi + 191h]
        push   0
        push   ebx
        push   eax
        call   sub_1_09F0
        cmp    dword ptr [ebp - 4], 0eh
        jae    SHORT loc_1_0472
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0
        lea    ecx, [esi + 191h]
        push   0
        push   eax
        push   ecx
        mov    eax, dword ptr [esi + 0f9h]
        push   ecx
        push   esi
        push   eax
        call   sub_1_1C10
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0472:
        mov    eax, dword ptr [ebp - 4]
        sub    edi, 0eh
        sub    eax, 0eh
        lea    ecx, [esi + 19fh]
        push   eax
        push   edi
        push   ecx
        lea    eax, [esi + 191h]
        push   0eh
        mov    ecx, dword ptr [esi + 0f9h]
        push   eax
        push   eax
        push   esi
        push   ecx
        call   sub_1_1C10
loc_1_049B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_04B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 10dh]
        cmp    byte ptr [esi + 290h], 0
        jne    SHORT loc_1_04D7
        push   esi
        call   sub_1_10A0
        push   esi
        call   sub_1_1020
loc_1_04D7:
        cmp    dword ptr [esi + 125h], 0
        je     NEAR PTR loc_1_059D
loc_1_04E4:
        mov    ebx, dword ptr [esi + 125h]
        cmp    dword ptr [esi + 129h], ebx
        jne    SHORT loc_1_04FC
        mov    dword ptr [esi + 129h], 0
loc_1_04FC:
        mov    eax, dword ptr [ebx + 24h]
        push   ebx
        push   esi
        mov    dword ptr [esi + 125h], eax
        call   sub_1_03F0
        push   0
        inc    dword ptr [esi + 13ah]
        mov    edi, dword ptr [ebx + 28h]
        push   ebx
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1A62
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_0590
        mov    eax, dword ptr [ebx + 109h]
        cmp    edi, eax
        jne    SHORT loc_1_053D
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 109h], eax
        jmp    SHORT loc_1_0550
loc_1_053D:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_054A
loc_1_0542:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_0542
loc_1_054A:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_0550:
        mov    eax, dword ptr [ebx + 17dh]
        cmp    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_0561
        push   ebx
        db  0E8h, 03Fh, 004h, 000h, 000h
loc_1_0561:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1A5C
        push   0
        push   75h
        push   edi
        call   sub_1_1B24
        cmp    dword ptr [ebx + 109h], 0
        jne    SHORT loc_1_0590
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 0b0h
        push   ebx
        call   sub_1_1B12
loc_1_0590:
        cmp    dword ptr [esi + 125h], 0
        jne    NEAR PTR loc_1_04E4
loc_1_059D:
        push   esi
        call   sub_1_1100
        cmp    byte ptr [esi + 131h], 0
        je     SHORT loc_1_05BF
        mov    byte ptr [esi + 131h], 0
        mov    eax, dword ptr [esi + 0f9h]
        push   eax
        call   sub_1_1DF0
loc_1_05BF:
        cmp    byte ptr [esi + 173h], 0
        je     NEAR PTR loc_1_0981
        cmp    dword ptr [esi + 10dh], 1
        jne    NEAR PTR loc_1_0981
        push   esi
        lea    eax, [esi + 0b0h]
        mov    byte ptr [esi + 174h], 1
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_1_1B12
        mov    edi, dword ptr [esi + 109h]
        test   edi, edi
        je     NEAR PTR loc_1_068F
loc_1_0600:
        push   0
        inc    dword ptr [edi + 1ch]
        push   0
        mov    eax, dword ptr [edi]
        push   40010004h
        push   eax
        call   sub_1_1A86
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_0684
        mov    eax, dword ptr [ebx + 109h]
        cmp    eax, edi
        jne    SHORT loc_1_0631
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 109h], eax
        jmp    SHORT loc_1_0644
loc_1_0631:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_063E
loc_1_0636:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_0636
loc_1_063E:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_0644:
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [ebx + 17dh], eax
        jne    SHORT loc_1_0655
        push   ebx
        db  0E8h, 04Bh, 003h, 000h, 000h
loc_1_0655:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1A5C
        push   0
        push   75h
        push   edi
        call   sub_1_1B24
        cmp    dword ptr [ebx + 109h], 0
        jne    SHORT loc_1_0684
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 0b0h
        push   ebx
        call   sub_1_1B12
loc_1_0684:
        mov    edi, dword ptr [edi + 0ch]
        test   edi, edi
        jne    NEAR PTR loc_1_0600
loc_1_068F:
        push   esi
        db  0E8h, 0DBh, 010h, 000h, 000h
        test   ax, ax
        je     SHORT loc_1_06B6
        mov    byte ptr [esi + 172h], 1
        push   0
        mov    eax, dword ptr [esi + 5eh]
        push   0aa000003h
        push   eax
        call   sub_1_1BF0
        add    esp, 0ch
        jmp    SHORT loc_1_06BD
loc_1_06B6:
        mov    byte ptr [esi + 172h], 0
loc_1_06BD:
        cmp    dword ptr [esi + 11dh], 0
        je     SHORT loc_1_06F5
        mov    eax, dword ptr [esi + 115h]
        mov    ecx, dword ptr [esi + 121h]
        mov    dword ptr [ecx + 24h], eax
        mov    edx, dword ptr [esi + 11dh]
        mov    dword ptr [esi + 115h], edx
        mov    dword ptr [esi + 121h], 0
        mov    dword ptr [esi + 11dh], 0
loc_1_06F5:
        cmp    dword ptr [esi + 125h], 0
        je     SHORT loc_1_0719
        mov    eax, dword ptr [esi + 115h]
        mov    ecx, dword ptr [esi + 129h]
        mov    dword ptr [ecx + 24h], eax
        mov    edx, dword ptr [esi + 125h]
        mov    dword ptr [esi + 115h], edx
loc_1_0719:
        mov    dword ptr [esi + 129h], 0
        mov    dword ptr [esi + 125h], 0
        cmp    dword ptr [esi + 115h], 0
        je     NEAR PTR loc_1_07D7
loc_1_073A:
        mov    ecx, dword ptr [esi + 115h]
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [esi + 115h], eax
        mov    edi, dword ptr [ecx + 28h]
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_1A62
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_07CA
        mov    eax, dword ptr [ebx + 109h]
        cmp    eax, edi
        jne    SHORT loc_1_0777
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 109h], eax
        jmp    SHORT loc_1_078A
loc_1_0777:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_0784
loc_1_077C:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_077C
loc_1_0784:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_078A:
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [ebx + 17dh], eax
        jne    SHORT loc_1_079B
        push   ebx
        db  0E8h, 005h, 002h, 000h, 000h
loc_1_079B:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1A5C
        push   0
        push   75h
        push   edi
        call   sub_1_1B24
        cmp    dword ptr [ebx + 109h], 0
        jne    SHORT loc_1_07CA
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 0b0h
        push   ebx
        call   sub_1_1B12
loc_1_07CA:
        cmp    dword ptr [esi + 115h], 0
        jne    NEAR PTR loc_1_073A
loc_1_07D7:
        mov    dword ptr [esi + 119h], 0
        cmp    byte ptr [esi + 172h], 0
        jne    SHORT loc_1_07F0
        push   esi
        db  0E8h, 080h, 010h, 000h, 000h
loc_1_07F0:
        mov    byte ptr [esi + 174h], 0
        mov    edi, dword ptr [esi + 109h]
        test   edi, edi
        je     NEAR PTR loc_1_08CF
loc_1_0805:
        inc    dword ptr [edi + 1ch]
        cmp    byte ptr [esi + 172h], 0
        je     SHORT loc_1_0822
        push   0
        mov    eax, dword ptr [edi]
        push   0
        push   40010007h
        push   eax
        call   sub_1_1A86
loc_1_0822:
        mov    eax, 0
        push   4
        cmp    byte ptr [esi + 172h], 1
        adc    eax, -1
        and    eax, 0c0000001h
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [edi]
        push   40010005h
        push   ecx
        call   sub_1_1A86
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_1A8C
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_08C4
        mov    eax, dword ptr [ebx + 109h]
        cmp    eax, edi
        jne    SHORT loc_1_0871
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 109h], eax
        jmp    SHORT loc_1_0884
loc_1_0871:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_087E
loc_1_0876:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_0876
loc_1_087E:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_0884:
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [ebx + 17dh], eax
        jne    SHORT loc_1_0895
        push   ebx
        db  0E8h, 00Bh, 001h, 000h, 000h
loc_1_0895:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1A5C
        push   0
        push   75h
        push   edi
        call   sub_1_1B24
        cmp    dword ptr [ebx + 109h], 0
        jne    SHORT loc_1_08C4
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 0b0h
        push   ebx
        call   sub_1_1B12
loc_1_08C4:
        mov    edi, dword ptr [edi + 0ch]
        test   edi, edi
        jne    NEAR PTR loc_1_0805
loc_1_08CF:
        mov    eax, 0
        mov    ecx, dword ptr [esi + 175h]
        cmp    byte ptr [esi + 172h], 1
        adc    eax, -1
        mov    edx, dword ptr [ecx]
        and    eax, 0c0000001h
        push   eax
        push   edx
        call   sub_1_1A6E
        mov    ebx, dword ptr [esi + 175h]
        mov    edi, dword ptr [ebx + 8]
        dec    dword ptr [ebx + 1ch]
        jne    SHORT loc_1_0974
        mov    ecx, dword ptr [edi + 109h]
        cmp    ebx, ecx
        jne    SHORT loc_1_0915
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [edi + 109h], eax
        jmp    SHORT loc_1_0934
loc_1_0915:
        mov    eax, dword ptr [esi + 175h]
        cmp    dword ptr [ecx + 0ch], eax
        je     SHORT loc_1_0928
loc_1_0920:
        mov    ecx, dword ptr [ecx + 0ch]
        cmp    dword ptr [ecx + 0ch], eax
        jne    SHORT loc_1_0920
loc_1_0928:
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 0ch], eax
        mov    ebx, dword ptr [esi + 175h]
loc_1_0934:
        mov    eax, dword ptr [ebx + 18h]
        cmp    dword ptr [edi + 17dh], eax
        jne    SHORT loc_1_0945
        push   edi
        db  0E8h, 05Bh, 000h, 000h, 000h
loc_1_0945:
        push   0
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1A5C
        push   0
        push   75h
        push   ebx
        call   sub_1_1B24
        cmp    dword ptr [edi + 109h], 0
        jne    SHORT loc_1_0974
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        add    edi, 0b0h
        push   edi
        call   sub_1_1B12
loc_1_0974:
        mov    byte ptr [esi + 173h], 0
        push   esi
        db  0E8h, 0EFh, 00Eh, 000h, 000h
loc_1_0981:
        mov    byte ptr [esi + 28dh], 0
        cmp    byte ptr [esi + 290h], 0
        jne    SHORT loc_1_099D
        push   esi
        call   sub_1_10E0
        push   esi
        call   sub_1_1020
loc_1_099D:
        mov    al, byte ptr [ebp - 4]
loc_1_09A0:
        pop    edi
        dec    dword ptr [esi + 10dh]
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  090h
sub_1_09B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 174h], 0
        je     SHORT loc_1_09CC
        mov    byte ptr [esi + 28eh], 0
        pop    esi
        pop    ebp
        ret    4
loc_1_09CC:
        mov    byte ptr [esi + 2a1h], 0
        push   esi
        call   sub_1_1260
        mov    byte ptr [esi + 28eh], 0
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_09F0:
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
        call   sub_1_1B48
        mov    ebx, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp - 4], esi
        je     SHORT loc_1_0A44
loc_1_0A15:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_1B6C
        mov    eax, dword ptr [ebp - 8]
        add    eax, edi
        cmp    ebx, eax
        jb     SHORT loc_1_0A44
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_1B90
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0A15
loc_1_0A44:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0A55
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_0A55:
        mov    eax, ebx
        sub    eax, edi
        sub    edi, ebx
        add    dword ptr [ebp - 0ch], eax
        add    dword ptr [ebp - 8], edi
        mov    edi, dword ptr [ebp + 14h]
loc_1_0A64:
        mov    eax, dword ptr [ebp - 8]
        add    eax, esi
        cmp    eax, edi
        jbe    SHORT loc_1_0A73
        mov    ebx, edi
        sub    ebx, esi
        jmp    SHORT loc_1_0A76
loc_1_0A73:
        mov    ebx, dword ptr [ebp - 8]
loc_1_0A76:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, esi
        add    esi, ebx
        push   eax
        call   sub_1_1F00
        cmp    edi, esi
        je     SHORT loc_1_0AB2
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_1B90
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_0ABD
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_1B6C
        jmp    SHORT loc_1_0A64
loc_1_0AB2:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
loc_1_0ABD:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0AD0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        mov    edi, dword ptr [eax + 8]
        add    esi, 0eh
        cmp    dword ptr [edi + 12dh], 0
        je     SHORT loc_1_0B5C
        push   0
        lea    eax, [ebp - 4]
        push   eax
        mov    ecx, dword ptr [ebp + 18h]
        push   0
        mov    ebx, esi
        push   0
        push   ecx
        call   sub_1_1B48
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_0B4A
loc_1_0B0A:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_1B6C
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [edi + 12dh]
        push   ecx
        mov    eax, dword ptr [ebp - 0ch]
        push   ebx
        push   edx
        push   eax
        call   sub_1_09F0
        add    ebx, eax
        cmp    eax, dword ptr [ebp - 8]
        jb     SHORT loc_1_0B4A
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_1B90
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0B0A
loc_1_0B4A:
        sub    ebx, esi
        xor    eax, eax
        mov    esi, dword ptr [ebp + 1ch]
        pop    edi
        mov    dword ptr [esi], ebx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_0B5C:
        cmp    byte ptr [edi + 131h], 0
        je     SHORT loc_1_0B9C
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    ecx, dword ptr [ebp + 14h]
        push   esi
        push   ecx
        mov    esi, dword ptr [ebp + 1ch]
        push   esi
        call   sub_1_1440
        test   ax, ax
        je     SHORT loc_1_0B91
        mov    eax, 0c0000001h
        pop    edi
        mov    dword ptr [esi], 0
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_0B91:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_0B9C:
        mov    eax, 0c0010013h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 2a1h], 0
        je     NEAR PTR loc_1_0CB9
        cmp    byte ptr [esi + 0cfh], 0
        je     NEAR PTR loc_1_0CB9
        push   esi
        call   sub_1_1060
        mov    byte ptr [esi + 2a1h], 0
        push   0
        mov    eax, dword ptr [esi + 5eh]
        push   0aa000003h
        push   eax
        call   sub_1_1BF0
        add    esp, 0ch
        cmp    dword ptr [esi + 11dh], 0
        je     SHORT loc_1_0C31
        xor    edi, edi
loc_1_0BFE:
        mov    edx, dword ptr [esi + 11dh]
        lea    ecx, [edx + 24h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 11dh], eax
        test   eax, eax
        jne    SHORT loc_1_0C19
        mov    dword ptr [esi + 121h], edi
loc_1_0C19:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   edx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1A62
        dec    dword ptr [ebx + 1ch]
        cmp    dword ptr [esi + 11dh], edi
        jne    SHORT loc_1_0BFE
loc_1_0C31:
        cmp    dword ptr [esi + 115h], 0
        je     SHORT loc_1_0C6F
        xor    edi, edi
loc_1_0C3C:
        mov    edx, dword ptr [esi + 115h]
        lea    ecx, [edx + 24h]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [esi + 115h], eax
        test   eax, eax
        jne    SHORT loc_1_0C57
        mov    dword ptr [esi + 119h], edi
loc_1_0C57:
        mov    ebx, dword ptr [ecx + 4]
        push   edi
        push   edx
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_1A62
        dec    dword ptr [ebx + 1ch]
        cmp    dword ptr [esi + 115h], edi
        jne    SHORT loc_1_0C3C
loc_1_0C6F:
        mov    byte ptr [esi + 2a1h], 0
        mov    byte ptr [esi + 0cfh], 0
        push   esi
        db  0E8h, 0EDh, 00Ah, 000h, 000h
        test   ax, ax
        je     SHORT loc_1_0CAA
        mov    byte ptr [esi + 172h], 1
        push   0
        mov    eax, dword ptr [esi + 5eh]
        push   0aa000003h
        push   eax
        call   sub_1_1BF0
        add    esp, 0ch
        push   esi
        call   sub_1_1020
        jmp    SHORT loc_1_0CD2
loc_1_0CAA:
        mov    byte ptr [esi + 172h], 0
        push   esi
        call   sub_1_1020
        jmp    SHORT loc_1_0CD2
loc_1_0CB9:
        cmp    dword ptr [esi + 11dh], 0
        jne    SHORT loc_1_0CCB
        cmp    dword ptr [esi + 115h], 0
        je     SHORT loc_1_0CD2
loc_1_0CCB:
        mov    byte ptr [esi + 2a1h], 1
loc_1_0CD2:
        push   30d40h
        add    esi, 295h
        push   esi
        call   sub_1_1AFA
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0CF0:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        push   esi
        mov    eax, dword ptr [edx + 2eh]
        or     byte ptr [eax], 2
        mov    cx, word ptr [edx + 0cbh]
        mov    word ptr [edx + 0cdh], cx
        lea    eax, [ecx + 1]
        mov    word ptr [edx + 0cbh], ax
        cmp    word ptr [edx + 4eh], ax
        jne    SHORT loc_1_0D26
        mov    word ptr [edx + 0cbh], 0
loc_1_0D26:
        mov    ax, word ptr [edx + 0cbh]
        movsx  esi, ax
        cmp    dword ptr [edx + esi*4 + 0d4h], 2
        jne    SHORT loc_1_0D3F
        cmp    ax, cx
        jne    SHORT loc_1_0D48
loc_1_0D3F:
        mov    word ptr [edx + 0cbh], 0ffffh
loc_1_0D48:
        movsx  eax, cx
        mov    bx, word ptr [edx + eax*4 + 0d0h]
        mov    al, byte ptr [edx + 0a8h]
        imul   cl
        shr    al, 8
        mov    ecx, dword ptr [edx + 2eh]
        inc    al
        mov    byte ptr [ecx + 14h], al
        mov    ecx, dword ptr [edx + 2eh]
        pop    esi
        mov    byte ptr [ecx + 15h], bl
        mov    ecx, dword ptr [edx + 2eh]
        mov    byte ptr [ecx + 16h], bh
        mov    ecx, dword ptr [edx + 2eh]
        mov    byte ptr [edx + 0cfh], 1
        mov    byte ptr [ecx + 10h], 6
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0D90:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        push   0
        mov    di, word ptr [esi + 0a8h]
        mov    ecx, dword ptr [ebp + 0ch]
        movsx  eax, word ptr [esi + 0c9h]
        imul   edi, eax
        add    edi, dword ptr [esi + 2ah]
        lea    eax, [ebp - 4]
        push   eax
        xor    ebx, ebx
        push   0
        push   0
        push   ecx
        call   sub_1_1B48
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_1_0E04
loc_1_0DCC:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_1B6C
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 0ch]
        push   ecx
        push   edx
        push   edi
        call   sub_1_1F00
        add    edi, dword ptr [ebp - 8]
        add    ebx, dword ptr [ebp - 8]
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_1B90
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_0DCC
loc_1_0E04:
        movsx  eax, word ptr [esi + 0c9h]
        cmp    ebx, 3ch
        jae    SHORT loc_1_0E24
        mov    dword ptr [esi + eax*4 + 0d0h], 3ch
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_0E24:
        pop    edi
        mov    dword ptr [esi + eax*4 + 0d0h], ebx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0E40:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [esi + 0b0h]
        push   eax
        call   sub_1_1B12
        mov    cl, byte ptr [esi + 0c8h]
        mov    eax, dword ptr [esi + 2eh]
        mov    byte ptr [ebp - 1], cl
        mov    bl, byte ptr [eax + 13h]
        inc    bl
        cmp    byte ptr [esi + 0f3h], bl
        jne    SHORT loc_1_0E7D
        mov    bl, byte ptr [esi + 0f2h]
loc_1_0E7D:
        xor    edi, edi
loc_1_0E7F:
        cmp    bl, byte ptr [ebp - 1]
        je     NEAR PTR loc_1_0F50
loc_1_0E88:
        xor    ecx, ecx
        xor    eax, eax
        mov    cx, word ptr [esi + 4ch]
        mov    al, bl
        imul   ecx, eax
        add    ecx, dword ptr [esi + 8]
        mov    dword ptr [ebp - 8], edi
        mov    ax, word ptr [ecx + 2]
        mov    word ptr [ebp - 8], ax
        cmp    dword ptr [ebp - 8], 600h
        ja     NEAR PTR loc_1_0FB3
        mov    bl, byte ptr [ecx + 1]
        cmp    byte ptr [esi + 0f2h], bl
        ja     SHORT loc_1_0F21
        cmp    byte ptr [esi + 0f3h], bl
        jb     SHORT loc_1_0F21
        add    ecx, 4
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 4
        push   esi
        push   eax
        mov    dword ptr [esi + 0ech], ecx
        mov    dword ptr [esi + 0f4h], eax
        call   sub_1_1850
        mov    dl, byte ptr [esi + 0f3h]
        cmp    bl, dl
        jb     SHORT loc_1_0EF1
        sub    bl, dl
        add    bl, byte ptr [esi + 0f2h]
loc_1_0EF1:
        cmp    byte ptr [esi + 0f2h], bl
        jne    SHORT loc_1_0F03
        dec    dl
        mov    ecx, dword ptr [esi + 2eh]
        mov    byte ptr [ecx + 13h], dl
        jmp    SHORT loc_1_0F0C
loc_1_0F03:
        lea    ecx, [ebx - 1]
        mov    edx, dword ptr [esi + 2eh]
        mov    byte ptr [edx + 13h], cl
loc_1_0F0C:
        cmp    ax, 5
        jne    NEAR PTR loc_1_0E7F
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0F21:
        mov    cl, byte ptr [ebp - 1]
        mov    al, byte ptr [esi + 0f3h]
        cmp    cl, al
        jbe    SHORT loc_1_0F36
        sub    cl, al
        add    cl, byte ptr [esi + 0f2h]
loc_1_0F36:
        cmp    byte ptr [esi + 0f2h], cl
        jne    SHORT loc_1_0F48
        dec    al
        mov    ecx, dword ptr [esi + 2eh]
        mov    byte ptr [ecx + 13h], al
        jmp    SHORT loc_1_0F50
loc_1_0F48:
        dec    cl
        mov    eax, dword ptr [esi + 2eh]
        mov    byte ptr [eax + 13h], cl
loc_1_0F50:
        mov    eax, dword ptr [esi + 2eh]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        lea    ecx, [esi + 0b0h]
        mov    byte ptr [eax + 17h], 1
        push   ecx
        call   sub_1_1B12
        mov    cl, byte ptr [esi + 0c8h]
        mov    edx, dword ptr [esi + 2eh]
        mov    byte ptr [ebp - 1], cl
        mov    bl, byte ptr [edx + 13h]
        inc    bl
        cmp    byte ptr [esi + 0f3h], bl
        jne    SHORT loc_1_0F88
        mov    bl, byte ptr [esi + 0f2h]
loc_1_0F88:
        cmp    bl, byte ptr [ebp - 1]
        jne    NEAR PTR loc_1_0E88
        cmp    byte ptr [esi + 0f0h], 0
        je     SHORT loc_1_0FA8
        mov    eax, dword ptr [esi + 2eh]
        mov    byte ptr [eax + 17h], 10h
        mov    byte ptr [esi + 0f0h], 0
loc_1_0FA8:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0FB3:
        mov    eax, 0ffffh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0FD0:
        push   ebp
        mov    ecx, 10h
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 20h], cx
        mov    byte ptr [eax], 6
        mov    dword ptr [eax + 9ch], 2a3h
        mov    dword ptr [eax + 0a0h], 330000h
        mov    dword ptr [eax + 10h], 0
        mov    word ptr [eax + 32h], cx
        pop    ebp
        mov    word ptr [eax + 58h], 6
        mov    word ptr [eax + 5ah], 2
        mov    word ptr [eax + 56h], cx
        mov    ax, 1
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1020:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        cmp    byte ptr [eax + 0ach], 1
        je     SHORT loc_1_1037
        xor    ax, ax
        pop    ebp
        ret    4
loc_1_1037:
        mov    ecx, dword ptr [eax + 2eh]
        pop    ebp
        mov    byte ptr [ecx + 10h], 0
        mov    dl, byte ptr [eax + 0aeh]
        mov    ecx, dword ptr [eax + 2eh]
        xor    ax, ax
        mov    byte ptr [ecx + 1fh], dl
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1060:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    eax, dword ptr [esp + 8]
        pop    ebp
        mov    ecx, dword ptr [eax + 2eh]
        mov    byte ptr [ecx + 10h], dl
        mov    ecx, dword ptr [eax + 2eh]
        xor    ax, ax
        mov    byte ptr [ecx + 1fh], dl
        ret    4
        db  08Dh, 064h, 024h, 000h, 055h, 08Bh, 044h, 024h, 008h, 08Bh, 0ECh, 080h, 0A0h, 0AEh, 000h, 000h
        db  000h, 0F5h, 066h, 033h, 0C0h, 05Dh, 0C2h, 004h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h
        db  000h, 000h, 000h, 000h
sub_1_10A0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        and    byte ptr [eax + 0aeh], 0cah
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 055h, 08Bh, 044h, 024h, 008h
        db  08Bh, 0ECh, 080h, 088h, 0AEh, 000h, 000h, 000h, 00Ah, 066h, 033h, 0C0h, 05Dh, 0C2h, 004h, 000h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_10E0:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        or     byte ptr [eax + 0aeh], 35h
        xor    ax, ax
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1100:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 1
        push   ebx
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, 0
        mov    eax, dword ptr [esi + 2eh]
        or     byte ptr [eax], 2
        mov    eax, dword ptr [esi + 2eh]
        mov    cl, byte ptr [eax + 17h]
        and    cl, 35h
        je     NEAR PTR loc_1_121F
        mov    byte ptr [ebp - 1], bl
loc_1_112C:
        mov    eax, dword ptr [esi + 2eh]
        mov    byte ptr [eax + 10h], bl
        xor    eax, eax
        mov    edx, dword ptr [esi + 2eh]
        mov    al, byte ptr [edx + 10h]
        test   eax, 0fffffff9h
        jne    NEAR PTR loc_1_120C
        cmp    byte ptr [edx + 17h], 0ffh
        je     NEAR PTR loc_1_120C
        test   cl, 10h
        je     SHORT loc_1_116E
        mov    byte ptr [esi + 0f0h], 1
        push   esi
        call   sub_1_0E40
        test   eax, eax
        je     NEAR PTR loc_1_11FB
        jmp    NEAR PTR loc_1_121F
loc_1_116E:
        test   cl, 20h
        je     SHORT loc_1_11A0
        mov    eax, dword ptr [esi + 74h]
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 1dh]
        add    dword ptr [eax], ecx
        mov    eax, dword ptr [esi + 2eh]
        mov    edx, dword ptr [esi + 68h]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 1eh]
        add    dword ptr [edx], ecx
        mov    edx, dword ptr [esi + 6ch]
        mov    eax, dword ptr [esi + 2eh]
        xor    ecx, ecx
        mov    cl, byte ptr [eax + 1fh]
        add    dword ptr [edx], ecx
        mov    eax, dword ptr [esi + 2eh]
        mov    byte ptr [eax + 17h], 20h
        jmp    SHORT loc_1_11FB
loc_1_11A0:
        test   cl, 1
        je     SHORT loc_1_11B1
        push   esi
        call   sub_1_0E40
        test   eax, eax
        je     SHORT loc_1_11FB
        jmp    SHORT loc_1_121F
loc_1_11B1:
        test   cl, 4
        je     SHORT loc_1_11FB
        mov    cl, byte ptr [edx + 1ch]
        test   cl, 2
        je     SHORT loc_1_11C3
        mov    eax, dword ptr [esi + 68h]
        inc    dword ptr [eax]
loc_1_11C3:
        test   cl, 4
        je     SHORT loc_1_11D8
        mov    edx, dword ptr [esi + 74h]
        mov    eax, dword ptr [esi + 2eh]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        add    dword ptr [edx], eax
loc_1_11D8:
        test   cl, 8
        je     SHORT loc_1_11E2
        mov    eax, dword ptr [esi + 78h]
        inc    dword ptr [eax]
loc_1_11E2:
        test   cl, 10h
        je     SHORT loc_1_11F4
        mov    ecx, dword ptr [esi + 6ch]
        mov    eax, dword ptr [esi + 2eh]
        xor    edx, edx
        mov    dl, byte ptr [eax + 1fh]
        add    dword ptr [ecx], edx
loc_1_11F4:
        mov    eax, dword ptr [esi + 2eh]
        mov    byte ptr [eax + 17h], 4
loc_1_11FB:
        mov    eax, dword ptr [esi + 2eh]
        mov    cl, byte ptr [eax + 17h]
        and    cl, 35h
        jne    NEAR PTR loc_1_112C
        jmp    SHORT loc_1_121F
loc_1_120C:
        push   0
        mov    eax, dword ptr [esi + 5eh]
        push   0aa000003h
        push   eax
        call   sub_1_1BF0
        add    esp, 0ch
loc_1_121F:
        cmp    byte ptr [esi + 0afh], 0
        je     SHORT loc_1_123A
        mov    byte ptr [esi + 0afh], 0
        xor    ax, ax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_123A:
        mov    eax, 0
        pop    esi
        cmp    byte ptr [ebp - 1], 1
        adc    eax, -1
        pop    ebx
        and    ax, 3
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1260:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 2eh]
        mov    cl, byte ptr [eax + 17h]
        and    cl, 1ah
        test   cl, 0ah
        je     NEAR PTR loc_1_139E
        mov    edi, 0ffffffffh
loc_1_1280:
        mov    eax, dword ptr [esi + 2eh]
        xor    edx, edx
        mov    byte ptr [eax + 10h], 0
        mov    eax, dword ptr [esi + 2eh]
        mov    dl, byte ptr [eax + 10h]
        test   edx, 0fffffff9h
        jne    NEAR PTR loc_1_138B
        add    eax, 17h
        cmp    byte ptr [eax], 0ffh
        je     NEAR PTR loc_1_138B
        test   cl, 2
        jne    SHORT loc_1_1328
        test   cl, 8
        je     NEAR PTR loc_1_136F
        mov    byte ptr [eax], 8
        mov    eax, dword ptr [esi + 2eh]
        mov    al, byte ptr [eax + 14h]
        mov    bl, al
        and    bl, 8
        je     SHORT loc_1_12CD
        mov    eax, dword ptr [esi + 80h]
        jmp    SHORT loc_1_12D7
loc_1_12CD:
        test   al, 20h
        je     SHORT loc_1_12D9
        mov    eax, dword ptr [esi + 98h]
loc_1_12D7:
        inc    dword ptr [eax]
loc_1_12D9:
        mov    byte ptr [esi + 0cfh], 0
        mov    byte ptr [esi + 2a1h], 0
        movsx  eax, word ptr [esi + 0cdh]
        mov    dword ptr [esi + eax*4 + 0d4h], 0
        mov    word ptr [esi + 0cdh], di
        cmp    word ptr [esi + 0cbh], di
        je     SHORT loc_1_130F
        push   esi
        call   sub_1_0CF0
loc_1_130F:
        push   esi
        mov    eax, 0ch
        cmp    bl, 1
        adc    eax, edi
        push   eax
        call   sub_1_1690
        cmp    ax, 5
        je     SHORT loc_1_139E
        jmp    SHORT loc_1_136F
loc_1_1328:
        mov    byte ptr [eax], 2
        mov    byte ptr [esi + 0cfh], 0
        mov    byte ptr [esi + 2a1h], 0
        movsx  eax, word ptr [esi + 0cdh]
        mov    dword ptr [esi + eax*4 + 0d4h], 0
        mov    word ptr [esi + 0cdh], di
        cmp    word ptr [esi + 0cbh], di
        je     SHORT loc_1_1361
        push   esi
        call   sub_1_0CF0
loc_1_1361:
        push   esi
        push   0
        call   sub_1_1690
        cmp    ax, 5
        je     SHORT loc_1_139E
loc_1_136F:
        mov    eax, dword ptr [esi + 2eh]
        mov    cl, byte ptr [eax + 17h]
        and    cl, 1ah
        test   cl, 0ah
        jne    NEAR PTR loc_1_1280
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_138B:
        push   0
        mov    eax, dword ptr [esi + 5eh]
        push   0aa000003h
        push   eax
        call   sub_1_1BF0
        add    esp, 0ch
loc_1_139E:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_13B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    si, word ptr [edi + 0c9h]
        movsx  eax, si
        lea    ebx, [edi + eax*4 + 0d4h]
        cmp    dword ptr [ebx], 0
        je     SHORT loc_1_13DA
        mov    ax, 6
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_13DA:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx], 1
        push   eax
        push   edi
        call   sub_1_0D90
        mov    ax, word ptr [edi + 0c9h]
        inc    ax
        mov    word ptr [edi + 0c9h], ax
        cmp    ax, 1
        jne    SHORT loc_1_1409
        mov    word ptr [edi + 0c9h], 0
loc_1_1409:
        mov    dword ptr [ebx], 2
        cmp    word ptr [edi + 0cbh], -1
        jne    SHORT loc_1_1430
        cmp    word ptr [edi + 0cdh], -1
        mov    word ptr [edi + 0cbh], si
        jne    SHORT loc_1_1430
        push   edi
        call   sub_1_0CF0
loc_1_1430:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1440:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 10h
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 18h]
        push   edi
        mov    edx, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ecx + 0f4h]
        mov    esi, dword ptr [ebp + 0ch]
        lea    ecx, [esi + edx]
        cmp    ecx, eax
        jbe    SHORT loc_1_146D
        sub    eax, edx
        mov    esi, eax
loc_1_146D:
        mov    eax, dword ptr [ebp + 18h]
        push   0
        mov    ecx, dword ptr [ebp + 14h]
        mov    edi, dword ptr [eax + 0ech]
        add    edi, edx
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        push   ecx
        call   sub_1_1B48
        lea    ecx, [ebp - 10h]
        lea    edx, [ebp - 0ch]
        push   ecx
        mov    eax, dword ptr [ebp - 4]
        push   edx
        push   eax
        call   sub_1_1B6C
        mov    dword ptr [ebp - 8], 0
        test   esi, esi
        je     NEAR PTR loc_1_1537
loc_1_14AB:
        mov    eax, dword ptr [ebp - 10h]
        mov    ebx, esi
        sub    eax, dword ptr [ebp - 8]
        cmp    eax, esi
        ja     SHORT loc_1_14B9
        mov    ebx, eax
loc_1_14B9:
        mov    eax, dword ptr [ebp + 18h]
        lea    ecx, [ebx + edi]
        mov    eax, dword ptr [eax + 0dch]
        cmp    ecx, eax
        jbe    SHORT loc_1_14CD
        mov    ebx, eax
        sub    ebx, edi
loc_1_14CD:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   edi
        sub    esi, ebx
        push   eax
        add    edi, ebx
        call   sub_1_1F00
        mov    eax, dword ptr [ebp + 8]
        add    dword ptr [eax], ebx
        test   esi, esi
        je     SHORT loc_1_1537
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [eax + 0dch], edi
        jne    SHORT loc_1_14F6
        mov    edi, dword ptr [eax + 0d8h]
loc_1_14F6:
        add    dword ptr [ebp - 0ch], ebx
        add    dword ptr [ebp - 8], ebx
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, dword ptr [ebp - 10h]
        jne    SHORT loc_1_152F
        lea    eax, [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_1B90
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_1537
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp - 4]
        push   ecx
        push   edx
        call   sub_1_1B6C
        mov    dword ptr [ebp - 8], 0
loc_1_152F:
        test   esi, esi
        jne    NEAR PTR loc_1_14AB
loc_1_1537:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1550:
        push   ebp
        mov    eax, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    dword ptr [ebp - 8], eax
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   edi
        mov    esi, dword ptr [ebp + 14h]
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 0f4h]
        lea    edx, [edi + eax]
        cmp    edx, ecx
        jbe    SHORT loc_1_157A
        sub    ecx, eax
        mov    edi, ecx
loc_1_157A:
        mov    ebx, dword ptr [esi + 0ech]
        test   edi, edi
        je     SHORT loc_1_15C6
loc_1_1584:
        mov    eax, dword ptr [esi + 0dch]
        lea    ecx, [edi + ebx]
        mov    dword ptr [ebp - 4], edi
        cmp    ecx, eax
        jbe    SHORT loc_1_1599
        sub    eax, ebx
        mov    dword ptr [ebp - 4], eax
loc_1_1599:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        push   eax
        push   ebx
        push   ecx
        call   sub_1_1F00
        add    ebx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        add    dword ptr [ebp + 10h], ecx
        sub    edi, ecx
        je     SHORT loc_1_15C6
        cmp    dword ptr [esi + 0dch], ebx
        jne    SHORT loc_1_15C2
        mov    ebx, dword ptr [esi + 0d8h]
loc_1_15C2:
        test   edi, edi
        jne    SHORT loc_1_1584
loc_1_15C6:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_15E0:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx + 2eh]
        or     byte ptr [eax], 2
        mov    eax, dword ptr [ecx + 2eh]
        mov    byte ptr [eax + 10h], 40h
        mov    eax, dword ptr [ecx + 2eh]
        pop    ebp
        mov    al, byte ptr [eax + 17h]
        mov    byte ptr [ecx + 0c8h], al
        mov    ecx, dword ptr [ecx + 2eh]
        mov    al, 1
        mov    byte ptr [ecx + 10h], 0
        ret    4
        db  08Dh, 049h, 000h, 055h, 08Bh, 054h, 024h, 008h, 08Bh, 0ECh, 053h, 08Bh, 042h, 02Eh, 0B3h, 018h
        db  080h, 008h, 002h, 08Bh, 042h, 02Eh, 0C6h, 040h, 010h, 040h, 033h, 0C0h, 08Bh, 04Ah, 02Eh, 08Ah
        db  0C3h, 0FEh, 0C3h, 080h, 0FBh, 01Fh, 0C6h, 004h, 008h, 0FFh, 076h, 0EEh, 08Bh, 042h, 02Eh, 05Bh
        db  05Dh, 0C6h, 040h, 010h, 000h, 0B0h, 001h, 0C2h, 004h, 000h, 08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h
        db  000h, 000h, 000h, 055h, 08Bh, 054h, 024h, 008h, 08Bh, 0ECh, 053h, 08Bh, 042h, 02Eh, 0B3h, 018h
        db  080h, 008h, 002h, 08Bh, 042h, 02Eh, 0C6h, 040h, 010h, 040h, 033h, 0C0h, 08Bh, 04Ah, 02Eh, 08Ah
        db  0C3h, 0FEh, 0C3h, 080h, 0FBh, 01Fh, 0C6h, 004h, 008h, 000h, 076h, 0EEh, 08Bh, 042h, 02Eh, 05Bh
        db  05Dh, 0C6h, 040h, 010h, 000h, 0B0h, 001h, 0C2h, 004h, 000h, 08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h
        db  000h, 000h, 000h
sub_1_1690:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [esi + 11dh]
        mov    eax, dword ptr [ecx + 24h]
        mov    dword ptr [esi + 11dh], eax
        test   eax, eax
        jne    SHORT loc_1_16B6
        mov    dword ptr [esi + 121h], 0
loc_1_16B6:
        cmp    byte ptr [ecx + 2ch], 0
        mov    edi, dword ptr [ecx + 28h]
        je     SHORT loc_1_16F1
        cmp    dword ptr [esi + 125h], 0
        jne    SHORT loc_1_16D6
        mov    dword ptr [esi + 129h], ecx
        mov    dword ptr [esi + 125h], ecx
        jmp    SHORT loc_1_16E5
loc_1_16D6:
        mov    eax, dword ptr [esi + 129h]
        mov    dword ptr [eax + 24h], ecx
        mov    dword ptr [esi + 129h], ecx
loc_1_16E5:
        mov    dword ptr [ecx + 24h], 0
        jmp    NEAR PTR loc_1_1793
loc_1_16F1:
        mov    ax, word ptr [ebp + 8]
        test   ax, ax
        jne    SHORT loc_1_1702
        inc    dword ptr [esi + 13ah]
        jmp    SHORT loc_1_1708
loc_1_1702:
        inc    dword ptr [esi + 142h]
loc_1_1708:
        cmp    ax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0c0000001h
        push   eax
        push   ecx
        mov    ecx, dword ptr [edi]
        push   ecx
        call   sub_1_1A62
        mov    ebx, dword ptr [edi + 8]
        dec    dword ptr [edi + 1ch]
        jne    SHORT loc_1_1793
        mov    eax, dword ptr [ebx + 109h]
        cmp    edi, eax
        jne    SHORT loc_1_1740
        mov    eax, dword ptr [edi + 0ch]
        mov    dword ptr [ebx + 109h], eax
        jmp    SHORT loc_1_1753
loc_1_1740:
        cmp    dword ptr [eax + 0ch], edi
        je     SHORT loc_1_174D
loc_1_1745:
        mov    eax, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_1745
loc_1_174D:
        mov    ecx, dword ptr [edi + 0ch]
        mov    dword ptr [eax + 0ch], ecx
loc_1_1753:
        mov    eax, dword ptr [edi + 18h]
        cmp    dword ptr [ebx + 17dh], eax
        jne    SHORT loc_1_1764
        push   ebx
        db  0E8h, 03Ch, 0F2h, 0FFh, 0FFh
loc_1_1764:
        push   0
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1A5C
        push   0
loc_1_1770:
        push   75h
        push   edi
        call   sub_1_1B24
        cmp    dword ptr [ebx + 109h], 0
        jne    SHORT loc_1_1793
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        add    ebx, 0b0h
        push   ebx
        call   sub_1_1B12
loc_1_1793:
        mov    edi, dword ptr [esi + 115h]
        test   edi, edi
        je     NEAR PTR loc_1_1837
        cmp    byte ptr [esi + 173h], 0
        jne    NEAR PTR loc_1_1837
        mov    eax, dword ptr [edi + 24h]
        mov    dword ptr [esi + 115h], eax
        test   eax, eax
        jne    SHORT loc_1_17C5
        mov    dword ptr [esi + 119h], 0
loc_1_17C5:
        push   esi
        push   edi
        call   sub_1_13B0
        cmp    ax, 6
        jne    SHORT loc_1_1805
        mov    eax, dword ptr [esi + 115h]
        test   eax, eax
        je     SHORT loc_1_17EF
        mov    dword ptr [edi + 24h], eax
        xor    ax, ax
        mov    dword ptr [esi + 115h], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_17EF:
        xor    ax, ax
        mov    dword ptr [esi + 115h], edi
        mov    dword ptr [esi + 119h], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_1805:
        test   ax, ax
        jne    SHORT loc_1_1837
        cmp    dword ptr [esi + 11dh], 0
        jne    SHORT loc_1_1821
        mov    dword ptr [esi + 121h], edi
        mov    dword ptr [esi + 11dh], edi
        jmp    SHORT loc_1_1830
loc_1_1821:
        mov    eax, dword ptr [esi + 121h]
        mov    dword ptr [eax + 24h], edi
        mov    dword ptr [esi + 121h], edi
loc_1_1830:
        mov    dword ptr [edi + 24h], 0
loc_1_1837:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1850:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [edi + 131h], 1
        mov    dword ptr [edi + 12dh], 0
        mov    ebx, dword ptr [edi + 17dh]
        add    ebx, 0eh
        cmp    ebx, esi
        jb     SHORT loc_1_187C
        mov    ebx, esi
loc_1_187C:
        push   edi
        lea    eax, [edi + 191h]
        push   eax
        push   0
        push   ebx
        call   sub_1_1550
        test   ax, ax
        je     SHORT loc_1_189B
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_189B:
        inc    dword ptr [edi + 13eh]
        cmp    esi, 0eh
        jae    SHORT loc_1_18D1
        cmp    esi, 6
        jb     SHORT loc_1_18F7
        push   0
        lea    eax, [edi + 191h]
        push   0
        mov    edx, dword ptr [edi + 0f9h]
        push   0
        push   esi
        push   eax
        push   eax
        push   edi
        push   edx
        call   sub_1_1C10
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_18D1:
        sub    esi, 0eh
        sub    ebx, 0eh
        push   esi
        lea    eax, [edi + 19fh]
        push   ebx
        lea    ecx, [edi + 191h]
        push   eax
        push   0eh
        push   ecx
        mov    eax, dword ptr [edi + 0f9h]
        push   ecx
        push   edi
        push   eax
        call   sub_1_1C10
loc_1_18F7:
        xor    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1910:
        db  0A0h, 000h, 000h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_1935
        db  0FEh, 005h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_1937
        push   0
        push   0
        db  0E8h, 0D0h, 0E6h, 0FFh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_1937
loc_1_1935:
        clc
        ret
loc_1_1937:
        stc
        ret
SMC80PC_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_1948
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_1948:
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
        db  010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_1A32:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_1A38:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_1A3E:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_1A44:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_1A4A:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_1A50:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_1A5C:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_1A62:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_1A6E:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_1A7A:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_1A80:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_1A86:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_1A8C:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_1AA4:
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
sub_1_1AEC:
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        int3
        int3
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_1AFA:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_1B00:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_1B06:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_1B0C:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
sub_1_1B12:
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_1B18:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_1B1E:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_1B24:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_1B48:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_1B6C:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_1B90:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_1B9C:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_1BA2:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_1BA8:
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
sub_1_1BEA:
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_1BF0:
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
sub_1_1C10:
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
        call   sub_1_1C40
        pop    ebp
        ret    20h
        db  08Dh, 064h, 024h, 000h
sub_1_1C40:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 18h], 0eh
        jb     NEAR PTR loc_1_1CE0
        mov    edx, dword ptr [ebp + 10h]
        mov    al, byte ptr [edx]
        test   al, 1
        je     SHORT loc_1_1C80
        cmp    dword ptr [edx + 2], -1
        jne    SHORT loc_1_1C75
        cmp    al, 0ffh
        jne    SHORT loc_1_1C75
        cmp    byte ptr [edx + 1], 0ffh
        jne    SHORT loc_1_1C75
        mov    dword ptr [ebp - 4], 8
        jmp    SHORT loc_1_1CE7
loc_1_1C75:
        mov    dword ptr [ebp - 4], 2
        jmp    SHORT loc_1_1CE7
        db  08Bh, 0FFh
loc_1_1C80:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0ch], 20h
        je     SHORT loc_1_1CD0
        mov    edx, dword ptr [ebp + 10h]
        lea    ecx, [esi + 2ch]
        mov    eax, dword ptr [edx + 2]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_1CA6
        mov    ax, word ptr [edx]
        cmp    word ptr [ecx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_1CAD
loc_1_1CA6:
        mov    dword ptr [ebp - 10h], 1
loc_1_1CAD:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_1CC0
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_1CEA
        db  08Dh, 064h, 024h, 000h
loc_1_1CC0:
        mov    dword ptr [ebp - 4], 20h
        jmp    SHORT loc_1_1CEA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1CD0:
        mov    dword ptr [ebp - 4], 1
        jmp    SHORT loc_1_1CEA
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1CE0:
        mov    dword ptr [ebp - 4], 20h
loc_1_1CE7:
        mov    esi, dword ptr [ebp + 8]
loc_1_1CEA:
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     NEAR PTR loc_1_1DE1
loc_1_1CF5:
        mov    ebx, dword ptr [edi + 8]
        mov    eax, ebx
        and    eax, dword ptr [ebp - 4]
        test   al, 9
        jne    SHORT loc_1_1D4D
        test   al, 2
        je     SHORT loc_1_1D39
        lea    eax, [ebp - 0ch]
        push   eax
        mov    edx, dword ptr [ebp + 10h]
        push   edx
        mov    eax, dword ptr [esi + 4]
        push   eax
        mov    eax, dword ptr [esi + 24h]
        push   eax
        db  0E8h, 046h, 004h, 000h, 000h
        or     al, al
        je     SHORT loc_1_1D39
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], ecx
        mov    edx, 1
        mov    cl, byte ptr [edi + 19h]
        shl    edx, cl
        mov    ecx, dword ptr [ebp - 8]
        test   dword ptr [eax + ecx*4], edx
        jne    SHORT loc_1_1D4D
loc_1_1D39:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_1_1D44
        test   bl, 4
        jne    SHORT loc_1_1D4D
loc_1_1D44:
        test   bl, 20h
        je     NEAR PTR loc_1_1DD6
loc_1_1D4D:
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
        call   sub_1_1A7A
        mov    byte ptr [edi + 18h], 1
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_1DD6
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_1D8F
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_1D8F:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_1DA0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_1DA6
        db  08Bh, 0FFh
loc_1_1DA0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_1DA6:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_1DB5
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1A80
loc_1_1DB5:
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
        call   sub_1_1B24
        mov    edi, ebx
        jmp    SHORT loc_1_1DD9
loc_1_1DD6:
        mov    edi, dword ptr [edi + 10h]
loc_1_1DD9:
        or     edi, edi
        jne    NEAR PTR loc_1_1CF5
loc_1_1DE1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1DF0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1E00
        pop    ebp
        ret    4
sub_1_1E00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 10h]
        or     edi, edi
        je     SHORT loc_1_1E7E
loc_1_1E10:
        cmp    byte ptr [edi + 18h], 0
        je     SHORT loc_1_1E77
        mov    byte ptr [edi + 18h], 0
        inc    dword ptr [edi + 0ch]
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_1A80
        mov    eax, dword ptr [edi + 0ch]
        dec    eax
        mov    dword ptr [edi + 0ch], eax
        jne    SHORT loc_1_1E77
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_1E3C
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_1E3C:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_1E50
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_1E56
        db  005h, 000h, 000h, 000h, 000h
loc_1_1E50:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 10h], eax
loc_1_1E56:
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
        call   sub_1_1B24
        mov    edi, ebx
        jmp    SHORT loc_1_1E7A
loc_1_1E77:
        mov    edi, dword ptr [edi + 10h]
loc_1_1E7A:
        or     edi, edi
        jne    SHORT loc_1_1E10
loc_1_1E7E:
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
        je     SHORT loc_1_1EA5
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1EA5:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    bl, byte ptr [ecx]
        test   bl, 1
        je     SHORT loc_1_1EE1
        cmp    dword ptr [ecx + 2], -1
        jne    SHORT loc_1_1ED2
        cmp    bl, 0ffh
        jne    SHORT loc_1_1ED2
        cmp    byte ptr [ecx + 1], 0ffh
        jne    SHORT loc_1_1ED2
        test   al, 8
        je     SHORT loc_1_1ECB
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1ECB:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
loc_1_1ED2:
        test   al, 4
        jne    SHORT loc_1_1EDA
        test   al, 2
        je     SHORT loc_1_1EF9
loc_1_1EDA:
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1EE1:
        mov    eax, dword ptr [edx + 2eh]
        cmp    dword ptr [ecx + 2], eax
        jne    SHORT loc_1_1EF9
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx + 2ch], ax
        jne    SHORT loc_1_1EF9
        mov    al, 1
        pop    ebx
        pop    ebp
        ret    8
loc_1_1EF9:
        sub    al, al
        pop    ebx
        pop    ebp
        ret    8
sub_1_1F00:
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
sub_1_1F20:
        push   esi
        push   edi
        pushfd
        mov    ecx, dword ptr [esp + 18h]
        mov    esi, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        cmp    esi, edi
        jae    SHORT loc_1_1F40
        or     ecx, ecx
        je     SHORT loc_1_1F56
        std
        dec    ecx
        add    esi, ecx
        add    edi, ecx
        inc    ecx
        jmp    SHORT loc_1_1F54
loc_1_1F40:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 4
        jb     SHORT loc_1_1F54
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        rep movsd
        mov    cl, al
loc_1_1F54:
        rep movsb
loc_1_1F56:
        popfd
        pop    edi
        pop    esi
        ret    0ch
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 053h, 04Dh, 043h, 038h
        db  030h, 050h, 043h, 020h, 000h, 040h, 000h, 054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 0CCh, 0CCh, 0CCh, 040h, 028h, 023h, 029h, 020h, 053h, 04Dh, 043h, 038h, 030h, 050h, 043h
        db  02Eh, 056h, 058h, 044h, 020h, 076h, 031h, 02Eh, 030h, 030h, 062h, 020h, 031h, 031h, 02Fh, 032h
        db  031h, 02Fh, 039h, 034h, 00Ah, 020h, 028h, 043h, 029h, 020h, 053h, 074h, 061h, 06Eh, 064h, 061h
        db  072h, 064h, 020h, 04Dh, 069h, 063h, 072h, 06Fh, 073h, 079h, 073h, 074h, 065h, 06Dh, 073h, 020h
        db  043h, 06Fh, 072h, 070h, 02Ch, 020h, 031h, 039h, 039h, 034h, 000h, 000h, 040h, 028h, 023h, 029h
        db  020h, 055h, 04Dh, 05Fh, 038h, 030h, 031h, 036h, 077h, 069h, 06Eh, 039h, 035h, 05Fh, 073h, 032h
        db  02Eh, 03Fh, 03Fh, 05Fh, 076h, 031h, 02Eh, 030h, 031h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2074:
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
loc_1_20F0:
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
        push   eax
        inc    ebx
        db  000h
loc_1_2154:
        push   ebx
        dec    ebp
        inc    ebx
        cmp    byte ptr [eax], dh
        push   eax
        inc    ebx
        add    byte ptr [ebp + 41h], cl
        pop    eax
        dec    ebp
loc_1_2160:
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
loc_1_216C:
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
        add    byte ptr [ebp + 45h], cl
        dec    ebp
        dec    edi
        push   edx
        pop    ecx
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
        dec    ebp
        inc    ebp
        dec    ebp
        dec    edi
        push   edx
        pop    ecx
        inc    edx
        inc    ecx
        push   ebx
        inc    ebp
        add    byte ptr [eax], al
loc_1_21B4:
        push   ebx
        dec    ebp
        inc    ebx
        and    byte ptr [ecx + 64h], al
        popad
        db  070h, 074h
        db  065h, 072h, 02Eh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    eax
        sub    byte ptr [ebx], ah
        sub    dword ptr [eax], esp
        dec    esp
        dec    ebp
        pop    edi
        cmp    byte ptr [eax], dh
        xor    dword ptr [esi], esi
        pop    edi
        db  073h, 032h
        aas
        aas
        pop    edi
        db  076h, 031h
        xor    byte ptr cs:[ecx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
_LTEXT ends
        end
