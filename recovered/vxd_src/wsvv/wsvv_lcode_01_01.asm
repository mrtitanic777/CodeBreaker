; wsvv_lcode_01_01.asm -- WSVV object 1 segment _LTEXT  (part 1/5)
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    esi, dword ptr [eax + 84h]
        mov    eax, dword ptr [esi + 4ch]
        sub    eax, esi
        cmp    eax, 4ch
        je     NEAR PTR loc_1_0161
        call   sub_1_2880
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_0031
        mov    edi, 2747h
        jmp    NEAR PTR loc_1_0166
loc_1_0031:
        push   esi
        call   sub_1_0F09
        mov    ebx, eax
        mov    eax, dword ptr [esp + 18h]
        add    esp, 4
        dec    dword ptr [eax + 48h]
        push   edi
        inc    dword ptr [esi + 38h]
        push   dword ptr [esp + 18h]
        call   sub_1_2960
        mov    eax, dword ptr [esp + 24h]
        add    esp, 8
        mov    dword ptr [edi + 68h], eax
        mov    ebp, dword ptr [ebx + 58h]
        mov    dword ptr [edi + 88h], ebx
        lea    eax, [ebx + 1ch]
        mov    dword ptr [edi + 14h], eax
        mov    dword ptr [edi + 20h], 18h
        lea    eax, [ebx + 34h]
        mov    dword ptr [edi + 18h], eax
        mov    dword ptr [edi + 24h], 18h
        mov    dword ptr [edi + 8ch], ebp
        mov    dword ptr [ebp], edi
        mov    dword ptr [ebx + 10h], edi
        push   ebx
        call   sub_1_074D
        add    esp, 4
        test   byte ptr [edi + 40h], 8
        je     SHORT loc_1_00A5
        push   1
        push   2
        push   ebx
        call   sub_1_07CD
        add    esp, 0ch
loc_1_00A5:
        test   byte ptr [edi + 40h], 10h
        je     SHORT loc_1_00B8
        push   1
        push   3
        push   ebx
        call   sub_1_07CD
        add    esp, 0ch
loc_1_00B8:
        test   byte ptr [edi + 80h], 8
        je     SHORT loc_1_00CE
        push   1
        push   1
        push   ebx
        call   sub_1_07CD
        add    esp, 0ch
loc_1_00CE:
        cmp    dword ptr [ebx + 18h], 60000002h
        jne    SHORT loc_1_00DE
        mov    dword ptr [ebx + 18h], 60000003h
loc_1_00DE:
        push   0
        push   2
        push   edi
        call   sub_1_F3AC
        mov    eax, dword ptr [esp + 20h]
        add    esp, 0ch
        mov    ecx, dword ptr [eax + 50h]
        and    ecx, 0fffffff7h
        mov    dword ptr [eax + 50h], ecx
        mov    eax, dword ptr [esi + 4ch]
        sub    eax, esi
        cmp    eax, 4ch
        jne    SHORT loc_1_010C
        mov    eax, dword ptr [esp + 14h]
        and    ecx, 0fffffff7h
        mov    dword ptr [eax + 50h], ecx
loc_1_010C:
        mov    eax, dword ptr [ebp + 2ch]
        sub    eax, ebp
        cmp    eax, 2ch
        je     SHORT loc_1_013D
        mov    eax, dword ptr [ebp + 34h]
        sub    eax, ebp
        cmp    eax, 34h
        je     SHORT loc_1_012D
        push   0
        push   1
        push   edi
        call   sub_1_F3AC
        add    esp, 0ch
loc_1_012D:
        mov    eax, dword ptr [ebp + 3ch]
        sub    eax, ebp
        cmp    eax, 3ch
        je     SHORT loc_1_0157
        push   0
        push   4
        jmp    SHORT loc_1_014E
loc_1_013D:
        test   byte ptr [ebx + 1bh], 10h
        je     SHORT loc_1_0157
        mov    dword ptr [edi + 4ch], 7
        push   0
        push   20h
loc_1_014E:
        push   edi
        call   sub_1_F3AC
        add    esp, 0ch
loc_1_0157:
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [eax], edi
        sub    edi, edi
        jmp    SHORT loc_1_0166
loc_1_0161:
        mov    edi, 2733h
loc_1_0166:
        push   esi
        call   sub_1_0E7C
        add    esp, 4
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0180:
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        mov    ebp, dword ptr [esp + 40h]
        cmp    ebp, 18h
        je     SHORT loc_1_019C
        mov    edi, 271eh
        jmp    NEAR PTR loc_1_032C
loc_1_019C:
        push   ebp
        push   dword ptr [esp + 40h]
        call   sub_1_F270
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_01B7
        mov    edi, 271eh
        jmp    NEAR PTR loc_1_032C
loc_1_01B7:
        lea    edi, [esp + 1ch]
        mov    esi, eax
        mov    ecx, 6
        rep movsd
        push   ebp
        push   eax
        call   sub_1_F2B5
        mov    esi, dword ptr [esp + 40h]
        add    esp, 8
        push   esi
        call   sub_1_29BC
        movzx  eax, word ptr [esp + 20h]
        mov    dword ptr [esp + 18h], eax
        add    esp, 4
        mov    eax, dword ptr [esi + 2ch]
        mov    dword ptr [esp + 10h], eax
        mov    eax, dword ptr [esi + 30h]
        mov    dword ptr [esp + 18h], eax
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_2F7E
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        jne    NEAR PTR loc_1_0323
        test   byte ptr [esi + 40h], 20h
        jne    SHORT loc_1_0241
        lea    eax, [esp + 1ch]
        push   dword ptr [esp + 10h]
        push   eax
        call   sub_1_0B56
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_0241
        push   esi
        call   sub_1_29C4
        add    esp, 4
        mov    edi, 2740h
        jmp    NEAR PTR loc_1_032C
loc_1_0241:
        call   sub_1_09F0
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_025F
        push   esi
        call   sub_1_29C4
        add    esp, 4
        mov    edi, 2747h
        jmp    NEAR PTR loc_1_032C
loc_1_025F:
        cmp    dword ptr [esi + 2ch], 2
        jne    SHORT loc_1_02A4
        push   0
        call   sub_1_1760
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_0291
        push   edi
        call   sub_1_0B13
        add    esp, 4
        push   esi
        call   sub_1_29C4
        add    esp, 4
        mov    edi, 2747h
        jmp    NEAR PTR loc_1_032C
loc_1_0291:
        mov    dword ptr [edi + 54h], ebx
        mov    dword ptr [esi + 8ch], ebx
        push   esi
        push   ebx
        call   sub_1_17E3
        add    esp, 8
loc_1_02A4:
        movzx  eax, word ptr [esp + 1eh]
        push   eax
        lea    eax, [esp + 28h]
        push   dword ptr [esp + 24h]
        push   eax
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 20h]
        push   dword ptr [esp + 28h]
        push   esi
        push   edi
        call   sub_1_0C44
        add    esp, 20h
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_02E6
        mov    dword ptr [edi + 18h], esi
        mov    dword ptr [esi + 4ch], 2
        push   esi
        call   sub_1_29C4
        add    esp, 4
        sub    edi, edi
        jmp    SHORT loc_1_032C
loc_1_02E6:
        or     ebx, ebx
        je     SHORT loc_1_0304
        push   ebx
        call   sub_1_17C1
        add    esp, 4
        mov    dword ptr [edi + 54h], 0
        mov    dword ptr [esi + 8ch], 0
loc_1_0304:
        push   edi
        call   sub_1_0B13
        add    esp, 4
        push   esi
        call   sub_1_29C4
        add    esp, 4
        push   ebp
        call   sub_1_2C80
        add    esp, 4
        mov    edi, eax
        jmp    SHORT loc_1_032C
loc_1_0323:
        push   esi
        call   sub_1_29C4
        add    esp, 4
loc_1_032C:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 24h
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0340:
        sub    eax, eax
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0350:
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 44h]
        and    dword ptr [ebx + 50h], 0ffffffefh
        cmp    dword ptr [ebx + 4ch], 1
        jne    SHORT loc_1_03C5
        mov    ebp, dword ptr [esp + 48h]
        mov    word ptr [esp + 28h], 12h
        mov    ax, word ptr [ebp + 2]
        lea    esi, [ebp + 8]
        mov    word ptr [esp + 2ah], ax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [esp + 2ch], eax
        lea    edi, [esp + 30h]
        movsd
        movsd
        movsd
        movsd
        lea    eax, [esp + 28h]
        push   18h
        push   eax
        push   ebx
        call   sub_1_0180
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_1_05BE
        cmp    dword ptr [ebx + 2ch], 2
        jne    SHORT loc_1_03BF
        mov    dword ptr [ebx + 54h], 0
        push   0
        push   2
        push   ebx
        call   sub_1_F3AC
        add    esp, 0ch
        jmp    SHORT loc_1_03C9
loc_1_03BF:
        and    dword ptr [ebx + 54h], 0ffffffefh
        jmp    SHORT loc_1_03C9
loc_1_03C5:
        mov    ebp, dword ptr [esp + 48h]
loc_1_03C9:
        cmp    dword ptr [esp + 4ch], 18h
        jne    NEAR PTR loc_1_05B9
        push   dword ptr [esp + 4ch]
        push   ebp
        call   sub_1_F270
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_03EA
        jmp    NEAR PTR loc_1_05B9
loc_1_03EA:
        lea    edi, [esp + 10h]
        mov    esi, eax
        mov    ecx, 6
        rep movsd
        push   dword ptr [esp + 4ch]
        push   eax
        call   sub_1_F2B5
        add    esp, 8
        cmp    dword ptr [ebx + 2ch], 2
        jne    NEAR PTR loc_1_053C
        mov    eax, dword ptr [ebx + 88h]
        cmp    byte ptr [esp + 18h], 0
        jne    NEAR PTR loc_1_04AA
        cmp    byte ptr [esp + 19h], 0
        jne    NEAR PTR loc_1_04AA
        cmp    byte ptr [esp + 1ah], 0
        jne    SHORT loc_1_04AA
        cmp    byte ptr [esp + 1bh], 0
        jne    SHORT loc_1_04AA
        cmp    byte ptr [esp + 1ch], 0
        jne    SHORT loc_1_04AA
        cmp    byte ptr [esp + 1dh], 0
        jne    SHORT loc_1_04AA
        cmp    byte ptr [esp + 1eh], 0
        jne    SHORT loc_1_04AA
        cmp    byte ptr [esp + 1fh], 0
        jne    SHORT loc_1_04AA
        cmp    byte ptr [esp + 20h], 0
        jne    SHORT loc_1_04AA
        cmp    byte ptr [esp + 21h], 0
        jne    SHORT loc_1_04AA
        cmp    byte ptr [esp + 22h], 0
        jne    SHORT loc_1_04AA
        cmp    byte ptr [esp + 23h], 0
        jne    SHORT loc_1_04AA
        cmp    byte ptr [esp + 24h], 0
        jne    SHORT loc_1_04AA
        cmp    byte ptr [esp + 25h], 0
        jne    SHORT loc_1_04AA
        cmp    byte ptr [esp + 26h], 0
        jne    SHORT loc_1_04AA
        cmp    byte ptr [esp + 27h], 0
        jne    SHORT loc_1_04AA
        or     eax, eax
        je     SHORT loc_1_04A3
        push   eax
        call   sub_1_06D1
        add    esp, 4
        mov    dword ptr [ebx + 88h], 0
loc_1_04A3:
        sub    eax, eax
        jmp    NEAR PTR loc_1_05BE
loc_1_04AA:
        mov    ecx, dword ptr [ebx + 84h]
        mov    dx, word ptr [esp + 10h]
        cmp    word ptr [ecx + 1ch], dx
        jne    SHORT loc_1_0532
        or     eax, eax
        jne    SHORT loc_1_04FD
        push   0
        call   sub_1_05D0
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_04D7
        mov    eax, 2747h
        jmp    NEAR PTR loc_1_05BE
loc_1_04D7:
        mov    dword ptr [ebx + 88h], eax
        mov    dword ptr [eax + 10h], ebx
        mov    ecx, dword ptr [ebx + 84h]
        mov    dword ptr [eax + 14h], ecx
        lea    ecx, [eax + 34h]
        mov    dword ptr [eax + 18h], 60000004h
        mov    dword ptr [ebx + 18h], ecx
        mov    dword ptr [ebx + 24h], 18h
loc_1_04FD:
        lea    esi, [esp + 18h]
        lea    edi, [eax + 3ch]
        movsd
        movsd
        movsd
        movsd
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [eax + 38h], ecx
        mov    dword ptr [ebx + 4ch], 6
        and    dword ptr [ebx + 54h], 0fffffffdh
        and    dword ptr [ebx + 50h], 0fffffffdh
        push   0
        push   2
        push   ebx
        call   sub_1_F3AC
        add    esp, 0ch
        sub    eax, eax
        jmp    NEAR PTR loc_1_05BE
loc_1_0532:
        mov    eax, 2726h
        jmp    NEAR PTR loc_1_05BE
loc_1_053C:
        cmp    word ptr [esp + 10h], 12h
        jne    SHORT loc_1_05B2
        push   ebx
        call   sub_1_29BC
        add    esp, 4
        call   sub_1_2DE0
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_0568
        push   ebx
        call   sub_1_29C4
        add    esp, 4
        mov    eax, 2747h
        jmp    SHORT loc_1_05BE
loc_1_0568:
        lea    eax, [esp + 10h]
        push   eax
        push   edi
        push   ebx
        call   sub_1_29E2
        add    esp, 0ch
        mov    esi, eax
        or     esi, esi
        je     SHORT loc_1_05A2
        cmp    esi, 0ffh
        je     SHORT loc_1_05A2
        push   edi
        call   sub_1_2E44
        add    esp, 4
        push   ebx
        call   sub_1_29C4
        add    esp, 4
        push   esi
        call   sub_1_2C80
        add    esp, 4
        jmp    SHORT loc_1_05BE
loc_1_05A2:
        push   ebx
        call   sub_1_29C4
        add    esp, 4
        mov    eax, 2733h
        jmp    SHORT loc_1_05BE
loc_1_05B2:
        mov    eax, 2726h
        jmp    SHORT loc_1_05BE
loc_1_05B9:
        mov    eax, 271eh
loc_1_05BE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_05D0:
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_1_05EE
        push   0
        call   sub_1_1760
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_05F0
        sub    eax, eax
        jmp    SHORT loc_1_0651
loc_1_05EE:
        sub    ebx, ebx
loc_1_05F0:
        call   sub_1_F3A4
        push   5ch
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_0616
        or     ebx, ebx
        je     SHORT loc_1_0612
        push   ebx
        call   sub_1_17C1
        add    esp, 4
loc_1_0612:
        sub    eax, eax
        jmp    SHORT loc_1_0651
loc_1_0616:
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 17h
        rep stosd
        mov    dword ptr [esi + 18h], 1
        mov    dword ptr [esi + 58h], ebx
        db  0C7h, 006h, 03Ch, 038h, 001h, 000h
        db  0A1h, 040h, 038h, 001h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 040h, 038h, 001h, 000h
        push   esi
        mov    dword ptr [eax], esi
        db  089h, 035h, 040h, 038h, 001h, 000h
        call   sub_1_06C9
        add    esp, 4
        mov    eax, esi
loc_1_0651:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0655:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        sub    edi, edi
        mov    eax, dword ptr [esi + 58h]
        cmp    eax, edi
        je     SHORT loc_1_0670
        push   eax
        call   sub_1_17C1
        add    esp, 4
        mov    dword ptr [esi + 58h], edi
loc_1_0670:
        cmp    dword ptr [esi + 4ch], 0
        je     SHORT loc_1_06BD
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [esi + 18h], 7
        test   eax, 40000000h
        je     SHORT loc_1_06AD
        push   0
        push   esi
        call   sub_1_08BC
        add    esp, 8
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_06AD
        cmp    edi, 0ffh
        je     SHORT loc_1_06C6
        push   1
        push   esi
        call   sub_1_08BC
        add    esp, 8
        mov    edi, eax
loc_1_06AD:
        cmp    edi, 0ffh
        je     SHORT loc_1_06C6
        push   esi
        call   sub_1_093C
        jmp    SHORT loc_1_06C3
loc_1_06BD:
        push   esi
        call   sub_1_0986
loc_1_06C3:
        add    esp, 4
loc_1_06C6:
        pop    edi
        pop    esi
        ret
sub_1_06C9:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 8]
        ret
sub_1_06D1:
        mov    edx, dword ptr [esp + 4]
        sub    eax, eax
        mov    ecx, dword ptr [edx + 8]
        dec    ecx
        mov    dword ptr [edx + 8], ecx
        jne    SHORT loc_1_06EE
        push   edx
        call   sub_1_0655
        add    esp, 4
        mov    eax, 1
loc_1_06EE:
        ret
sub_1_06EF:
        sub    esp, 10h
        push   esi
        push   edi
        lea    edi, [esp + 8]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    esi, dword ptr [esp + 1ch]
        lea    eax, [esp + 8]
        push   esi
        push   eax
        db  0A1h, 058h, 02Ah, 001h, 000h
        call   dword ptr [eax + 8]
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_0747
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi + 4ch], eax
        mov    edi, dword ptr [esp + 20h]
        mov    dword ptr [esi + 14h], edi
        push   edi
        call   sub_1_0B31
        add    esp, 4
        lea    eax, [esp + 8]
        push   dword ptr [edi + 10h]
        push   eax
        db  0A1h, 058h, 02Ah, 001h, 000h
        call   dword ptr [eax + 10h]
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_0747
        sub    eax, eax
loc_1_0747:
        pop    edi
        pop    esi
        add    esp, 10h
        ret
sub_1_074D:
        sub    esp, 4ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 2ch], 4655424eh
        lea    eax, [esp + 30h]
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 28h], 26h
        mov    dword ptr [esp + 20h], eax
        mov    dword ptr [esp + 24h], 0
        lea    edi, [esp + 0ch]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    ebx, dword ptr [esp + 5ch]
        push   1
        mov    eax, dword ptr [ebx + 4ch]
        mov    dword ptr [esp + 10h], eax
        lea    eax, [esp + 2ch]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        lea    eax, [esp + 18h]
        push   3
        push   eax
        db  0A1h, 058h, 02Ah, 001h, 000h
        call   dword ptr [eax + 3ch]
        add    esp, 14h
        mov    word ptr [ebx + 1ch], 12h
        lea    esi, [esp + 3ch]
        lea    edi, [ebx + 24h]
        movsd
        movsd
        movsd
        movsd
        mov    eax, dword ptr [esp + 4ch]
        mov    dword ptr [ebx + 20h], eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
sub_1_07CD:
        sub    esp, 28h
        push   edi
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp + 18h], 400h
        lea    edi, [esp + 8]
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 20h], 200h
        mov    dword ptr [esp + 24h], 300h
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [esp + 28h], eax
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    eax, dword ptr [esp + 30h]
        push   4
        mov    eax, dword ptr [eax + 4ch]
        mov    dword ptr [esp + 0ch], eax
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        db  0A1h, 058h, 02Ah, 001h, 000h
        call   dword ptr [eax + 4ch]
        add    esp, 10h
        pop    edi
        add    esp, 28h
        ret
sub_1_0837:
        sub    esp, 24h
        push   edi
        mov    dword ptr [esp + 14h], 400h
        mov    dword ptr [esp + 18h], 0
        mov    dword ptr [esp + 1ch], 200h
        lea    edi, [esp + 4]
        mov    dword ptr [esp + 20h], 300h
        mov    eax, dword ptr [esp + 30h]
        mov    dword ptr [esp + 24h], eax
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    eax, dword ptr [esp + 2ch]
        push   dword ptr [esp + 38h]
        mov    eax, dword ptr [eax + 4ch]
        mov    dword ptr [esp + 8], eax
        lea    eax, [esp + 18h]
        push   dword ptr [esp + 38h]
        push   eax
        lea    eax, [esp + 10h]
        push   eax
        db  0A1h, 058h, 02Ah, 001h, 000h
        call   dword ptr [eax + 4ch]
        add    esp, 10h
        pop    edi
        add    esp, 24h
        ret
sub_1_089A:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 18h]
        mov    dword ptr [eax + 18h], 6
        test   ecx, 40000000h
        je     SHORT loc_1_08BB
        push   1
        push   eax
        call   sub_1_08BC
        add    esp, 8
loc_1_08BB:
        ret
sub_1_08BC:
        sub    esp, 14h
        mov    edx, dword ptr [esp + 18h]
        mov    ecx, dword ptr [edx + 10h]
        or     ecx, ecx
        je     SHORT loc_1_0902
        cmp    dword ptr [esp + 1ch], 0
        jne    SHORT loc_1_0902
        cmp    word ptr [ecx + 10h], 0
        je     SHORT loc_1_08F4
        movzx  eax, word ptr [ecx + 12h]
        imul   eax, eax, 3e8h
        mov    dword ptr [esp], eax
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0fffffffeh
        add    ax, 4
        jmp    SHORT loc_1_090E
loc_1_08F4:
        mov    dword ptr [esp], 0
        mov    ax, 4
        jmp    SHORT loc_1_090E
loc_1_0902:
        mov    dword ptr [esp], 0
        mov    ax, 2
loc_1_090E:
        mov    ecx, dword ptr [edx + 4ch]
        mov    dword ptr [esp + 4], ecx
        db  0C7h, 044h, 024h, 008h, 09Eh, 009h, 000h, 000h
        mov    dword ptr [esp + 0ch], edx
        push   0
        push   0
        push   eax
        lea    eax, [esp + 0ch]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        db  0A1h, 058h, 02Ah, 001h, 000h
        call   dword ptr [eax + 1ch]
        add    esp, 28h
        ret
sub_1_093C:
        sub    esp, 10h
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esi + 4ch]
        mov    dword ptr [esp + 8], eax
        lea    eax, [esp + 8]
        db  0C7h, 044h, 024h, 00Ch, 0BAh, 009h, 000h, 000h
        mov    dword ptr [esp + 10h], esi
        push   eax
        db  0A1h, 058h, 02Ah, 001h, 000h
        call   dword ptr [eax + 0ch]
        add    esp, 4
        mov    edi, eax
        cmp    edi, 0ffh
        je     SHORT loc_1_097E
        push   0
        push   edi
        push   esi
        call   sub_1_09BA
        add    esp, 0ch
loc_1_097E:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 10h
        ret
sub_1_0986:
        mov    eax, dword ptr [esp + 4]
        push   eax
        mov    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    dword ptr [ecx + 4], edx
        call   sub_1_F396
        add    esp, 4
        ret
loc_1_099E:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0ch]
        or     ecx, 2
        mov    dword ptr [eax + 0ch], ecx
        test   cl, 1
        je     SHORT loc_1_09B9
        push   eax
        call   sub_1_093C
        add    esp, 4
loc_1_09B9:
        ret
sub_1_09BA:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    dword ptr [esi + 4ch], 0
        mov    eax, dword ptr [esi + 14h]
        or     eax, eax
        je     SHORT loc_1_09D6
        push   eax
        call   sub_1_0B39
        add    esp, 4
loc_1_09D6:
        push   esi
        call   sub_1_0986
        add    esp, 4
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_09F0:
        push   esi
        push   edi
        call   sub_1_F3A4
        push   58h
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        mov    eax, 0
        je     SHORT loc_1_0A54
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 16h
        rep stosd
        mov    dword ptr [esi + 14h], 1
        lea    eax, [esi + 44h]
        mov    dword ptr [esi + 48h], eax
        mov    dword ptr [eax], eax
        lea    eax, [esi + 4ch]
        mov    dword ptr [esi + 50h], eax
        mov    dword ptr [eax], eax
        db  0C7h, 006h, 078h, 036h, 001h, 000h
        db  0A1h, 07Ch, 036h, 001h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 07Ch, 036h, 001h, 000h
        push   esi
        mov    dword ptr [eax], esi
        db  089h, 035h, 07Ch, 036h, 001h, 000h
        call   sub_1_0B0B
        add    esp, 4
        mov    eax, esi
loc_1_0A54:
        pop    edi
        pop    esi
        ret
sub_1_0A57:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        inc    dword ptr [esi + 8]
        mov    edi, dword ptr [esi + 14h]
        mov    dword ptr [esi + 14h], 4
        mov    eax, dword ptr [esi + 44h]
        sub    eax, esi
        cmp    eax, 44h
        je     SHORT loc_1_0A90
loc_1_0A74:
        push   esi
        call   sub_1_0F2D
        add    esp, 4
        push   eax
        call   sub_1_06D1
        add    esp, 4
        mov    eax, dword ptr [esi + 44h]
        sub    eax, esi
        cmp    eax, 44h
        jne    SHORT loc_1_0A74
loc_1_0A90:
        mov    eax, dword ptr [esi + 4ch]
        sub    eax, esi
        cmp    eax, 4ch
        je     SHORT loc_1_0AB6
loc_1_0A9A:
        push   esi
        call   sub_1_0F09
        add    esp, 4
        push   eax
        call   sub_1_06D1
        add    esp, 4
        mov    eax, dword ptr [esi + 4ch]
        sub    eax, esi
        cmp    eax, 4ch
        jne    SHORT loc_1_0A9A
loc_1_0AB6:
        mov    eax, dword ptr [esi + 54h]
        or     eax, eax
        je     SHORT loc_1_0ACD
        push   eax
        call   sub_1_17C1
        add    esp, 4
        mov    dword ptr [esi + 54h], 0
loc_1_0ACD:
        dec    dword ptr [esi + 8]
        cmp    dword ptr [esi + 34h], 0
        jne    SHORT loc_1_0B08
        cmp    edi, 40000003h
        jne    SHORT loc_1_0AF2
        push   0
        push   0
        push   0
        db  0A1h, 058h, 02Ah, 001h, 000h
        push   dword ptr [esi + 10h]
        call   dword ptr [eax + 38h]
        add    esp, 10h
loc_1_0AF2:
        cmp    dword ptr [esi + 10h], 0
        push   esi
        je     SHORT loc_1_0B00
        call   sub_1_0F54
        jmp    SHORT loc_1_0B05
loc_1_0B00:
        call   sub_1_0F9E
loc_1_0B05:
        add    esp, 4
loc_1_0B08:
        pop    edi
        pop    esi
        ret
sub_1_0B0B:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 8]
        ret
sub_1_0B13:
        mov    edx, dword ptr [esp + 4]
        sub    eax, eax
        mov    ecx, dword ptr [edx + 8]
        dec    ecx
        mov    dword ptr [edx + 8], ecx
        jne    SHORT loc_1_0B30
        push   edx
        call   sub_1_0A57
        add    esp, 4
        mov    eax, 1
loc_1_0B30:
        ret
sub_1_0B31:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 34h]
        ret
sub_1_0B39:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 34h]
        dec    eax
        mov    dword ptr [ecx + 34h], eax
        jne    SHORT loc_1_0B55
        cmp    dword ptr [ecx + 8], 0
        jne    SHORT loc_1_0B55
        push   ecx
        call   sub_1_0A57
        add    esp, 4
loc_1_0B55:
        ret
sub_1_0B56:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 015h, 078h, 036h, 001h, 000h
        db  081h, 0FAh, 078h, 036h, 001h, 000h
        je     NEAR PTR loc_1_0C37
        mov    eax, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        mov    esi, 4
loc_1_0B78:
        cmp    dword ptr [edx + 14h], esi
        je     NEAR PTR loc_1_0C29
        mov    edi, dword ptr [edx + 18h]
        cmp    dword ptr [edi + 2ch], ecx
        jne    NEAR PTR loc_1_0C29
        mov    bl, byte ptr [eax + 8]
        cmp    byte ptr [edx + 24h], bl
        jne    NEAR PTR loc_1_0C29
        mov    bl, byte ptr [eax + 9]
        cmp    byte ptr [edx + 25h], bl
        jne    NEAR PTR loc_1_0C29
        mov    bl, byte ptr [eax + 0ah]
        cmp    byte ptr [edx + 26h], bl
        jne    SHORT loc_1_0C29
        mov    bl, byte ptr [eax + 0bh]
        cmp    byte ptr [edx + 27h], bl
        jne    SHORT loc_1_0C29
        mov    bl, byte ptr [eax + 0ch]
        cmp    byte ptr [edx + 28h], bl
        jne    SHORT loc_1_0C29
        mov    bl, byte ptr [eax + 0dh]
        cmp    byte ptr [edx + 29h], bl
        jne    SHORT loc_1_0C29
        mov    bl, byte ptr [eax + 0eh]
        cmp    byte ptr [edx + 2ah], bl
        jne    SHORT loc_1_0C29
        mov    bl, byte ptr [eax + 0fh]
        cmp    byte ptr [edx + 2bh], bl
        jne    SHORT loc_1_0C29
        mov    bl, byte ptr [eax + 10h]
        cmp    byte ptr [edx + 2ch], bl
        jne    SHORT loc_1_0C29
        mov    bl, byte ptr [edx + 2dh]
        cmp    byte ptr [eax + 11h], bl
        jne    SHORT loc_1_0C29
        mov    bl, byte ptr [eax + 12h]
        cmp    byte ptr [edx + 2eh], bl
        jne    SHORT loc_1_0C29
        mov    bl, byte ptr [edx + 2fh]
        cmp    byte ptr [eax + 13h], bl
        jne    SHORT loc_1_0C29
        mov    bl, byte ptr [edx + 30h]
        cmp    byte ptr [eax + 14h], bl
        jne    SHORT loc_1_0C29
        mov    bl, byte ptr [edx + 31h]
        cmp    byte ptr [eax + 15h], bl
        jne    SHORT loc_1_0C29
        mov    bl, byte ptr [edx + 32h]
        cmp    byte ptr [eax + 16h], bl
        jne    SHORT loc_1_0C29
        mov    bl, byte ptr [edx + 33h]
        cmp    byte ptr [eax + 17h], bl
        jne    SHORT loc_1_0C29
        mov    ebx, dword ptr [eax + 4]
        mov    edi, dword ptr [edx + 20h]
        cmp    ebx, edi
        je     SHORT loc_1_0C3D
        cmp    edi, -1
        je     SHORT loc_1_0C3D
        cmp    ebx, -1
        je     SHORT loc_1_0C3D
loc_1_0C29:
        mov    edx, dword ptr [edx]
        db  081h, 0FAh, 078h, 036h, 001h, 000h
        jne    NEAR PTR loc_1_0B78
loc_1_0C37:
        sub    eax, eax
loc_1_0C39:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0C3D:
        mov    eax, 1
        jmp    SHORT loc_1_0C39
sub_1_0C44:
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 12h], 1
        mov    byte ptr [esp + 13h], 0
        mov    dword ptr [esp + 24h], 1
        lea    edi, [esp + 2ch]
        mov    word ptr [esp + 28h], 1ah
        mov    word ptr [esp + 2ah], 15h
        mov    eax, dword ptr [esp + 64h]
        mov    dword ptr [esp + 3ch], eax
        mov    eax, dword ptr [esp + 68h]
        mov    dword ptr [esp + 40h], eax
        sub    eax, eax
        mov    esi, dword ptr [esp + 60h]
        movsd
        movsd
        movsd
        movsd
        lea    edi, [esp + 14h]
        stosd
        stosd
        stosd
        stosd
        lea    eax, [esp + 12h]
        mov    esi, dword ptr [esp + 5ch]
        push   eax
        lea    eax, [esp + 28h]
        push   esi
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        db  0A1h, 058h, 02Ah, 001h, 000h
        call   dword ptr [eax]
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_1_0D64
        mov    edi, dword ptr [esp + 4ch]
        mov    ebx, dword ptr [esp + 14h]
        mov    dword ptr [edi + 14h], 40000002h
        mov    dword ptr [edi + 10h], ebx
        mov    ebp, dword ptr [esp + 54h]
        mov    word ptr [edi + 1ch], bp
        push   edi
        call   sub_1_0D6C
        mov    ecx, dword ptr [esp + 54h]
        add    esp, 4
        mov    dword ptr [ecx + 84h], edi
        lea    eax, [edi + 1ch]
        mov    dword ptr [ecx + 14h], eax
        mov    dword ptr [ecx + 20h], 18h
        mov    dword ptr [ecx + 28h], ebp
        mov    eax, dword ptr [esp + 58h]
        mov    dword ptr [ecx + 2ch], eax
        mov    dword ptr [ecx + 30h], esi
        push   edi
        db  068h, 095h, 010h, 000h, 000h
        push   1
        db  0A1h, 058h, 02Ah, 001h, 000h
        push   ebx
        call   dword ptr [eax + 38h]
        add    esp, 10h
        push   edi
        db  068h, 011h, 011h, 000h, 000h
        push   2
        db  0A1h, 058h, 02Ah, 001h, 000h
        push   ebx
        call   dword ptr [eax + 38h]
        add    esp, 10h
        push   edi
        db  068h, 014h, 011h, 000h, 000h
        push   3
        db  0A1h, 058h, 02Ah, 001h, 000h
        push   ebx
        call   dword ptr [eax + 38h]
        add    esp, 10h
        push   edi
        db  068h, 06Bh, 011h, 000h, 000h
        push   5
        db  0A1h, 058h, 02Ah, 001h, 000h
        push   ebx
        call   dword ptr [eax + 38h]
        add    esp, 10h
        push   edi
        db  068h, 0C2h, 011h, 000h, 000h
        push   4
        db  0A1h, 058h, 02Ah, 001h, 000h
        push   ebx
        call   dword ptr [eax + 38h]
        add    esp, 10h
        sub    eax, eax
loc_1_0D64:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 38h
        ret
sub_1_0D6C:
        sub    esp, 4ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [esp + 2ch], 4655424eh
        lea    eax, [esp + 30h]
        mov    dword ptr [esp + 1ch], 0
        mov    dword ptr [esp + 28h], 26h
        mov    dword ptr [esp + 20h], eax
        mov    dword ptr [esp + 24h], 0
        lea    edi, [esp + 0ch]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    ebx, dword ptr [esp + 5ch]
        push   0
        mov    eax, dword ptr [ebx + 10h]
        mov    dword ptr [esp + 10h], eax
        lea    eax, [esp + 2ch]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        lea    eax, [esp + 18h]
        push   3
        push   eax
        db  0A1h, 058h, 02Ah, 001h, 000h
        call   dword ptr [eax + 3ch]
        add    esp, 14h
        mov    word ptr [ebx + 1ch], 12h
        mov    eax, dword ptr [esp + 50h]
        mov    word ptr [ebx + 1eh], ax
        mov    eax, dword ptr [esp + 4ch]
        lea    esi, [esp + 3ch]
        mov    dword ptr [ebx + 20h], eax
        lea    edi, [ebx + 24h]
        movsd
        movsd
        movsd
        movsd
        mov    eax, dword ptr [esp + 4ch]
        mov    dword ptr [ebx + 20h], eax
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4ch
        ret
sub_1_0DFB:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   1
        call   sub_1_05D0
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_1_0E16
        mov    eax, 1
        jmp    SHORT loc_1_0E77
loc_1_0E16:
        mov    esi, dword ptr [esp + 14h]
        push   esi
        cmp    dword ptr [esi + 34h], 1
        sbb    ebp, ebp
        neg    ebp
        push   edi
        call   sub_1_06EF
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_0E3F
        push   edi
        call   sub_1_0655
        add    esp, 4
        mov    eax, ebx
        jmp    SHORT loc_1_0E77
loc_1_0E3F:
        mov    ebx, dword ptr [esi + 18h]
        mov    dword ptr [edi + 10h], ebx
        push   edi
        push   esi
        call   sub_1_0EE8
        add    esp, 8
        push   ebx
        push   dword ptr [edi + 58h]
        call   sub_1_17E3
        add    esp, 8
        or     ebp, ebp
        je     SHORT loc_1_0E75
        push   esi
        db  068h, 0D0h, 00Fh, 000h, 000h
        push   0
        db  0A1h, 058h, 02Ah, 001h, 000h
        push   dword ptr [esi + 10h]
        call   dword ptr [eax + 38h]
        add    esp, 10h
loc_1_0E75:
        sub    eax, eax
loc_1_0E77:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0E7C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        sub    eax, eax
        mov    ecx, dword ptr [esi + 38h]
        mov    ebx, dword ptr [esi + 3ch]
        mov    ebp, dword ptr [esi + 40h]
        cmp    ecx, eax
        je     SHORT loc_1_0EC3
        cmp    ebp, ebx
        jbe    SHORT loc_1_0EC3
        mov    edi, ebp
        sub    edi, ebx
        cmp    edi, ecx
        jb     SHORT loc_1_0EA1
        mov    edi, ecx
loc_1_0EA1:
        or     edi, edi
        je     SHORT loc_1_0EB8
loc_1_0EA5:
        push   esi
        call   sub_1_0DFB
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_0EB8
        dec    edi
        inc    ebx
        or     edi, edi
        jne    SHORT loc_1_0EA5
loc_1_0EB8:
        mov    dword ptr [esi + 38h], edi
        mov    dword ptr [esi + 3ch], ebx
        mov    dword ptr [esi + 40h], ebp
        jmp    SHORT loc_1_0EC5
loc_1_0EC3:
        sub    eax, eax
loc_1_0EC5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0ECA:
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 8]
        lea    esi, [edx + 50h]
        lea    eax, [ecx + 4ch]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ecx + 50h]
        mov    dword ptr [edx + 54h], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx + 50h], esi
        pop    esi
        ret
sub_1_0EE8:
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 8]
        lea    esi, [edx + 50h]
        lea    eax, [ecx + 44h]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ecx + 48h]
        mov    dword ptr [edx + 54h], eax
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx + 48h], esi
        inc    dword ptr [ecx + 3ch]
        pop    esi
        ret
sub_1_0F09:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 4ch]
        lea    esi, [ecx + 4ch]
        mov    edx, eax
        sub    edx, ecx
        cmp    edx, 4ch
        jne    SHORT loc_1_0F21
        sub    eax, eax
        jmp    SHORT loc_1_0F2B
loc_1_0F21:
        mov    ecx, dword ptr [eax]
        sub    eax, 50h
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [esi], ecx
loc_1_0F2B:
        pop    esi
        ret
sub_1_0F2D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 44h]
        lea    edx, [esi + 44h]
        mov    ecx, eax
        sub    ecx, esi
        cmp    ecx, 44h
        jne    SHORT loc_1_0F45
        sub    eax, eax
        jmp    SHORT loc_1_0F52
loc_1_0F45:
        mov    ecx, dword ptr [eax]
        sub    eax, 50h
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [edx], ecx
        dec    dword ptr [esi + 3ch]
loc_1_0F52:
        pop    esi
        ret
sub_1_0F54:
        sub    esp, 10h
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [esp + 8], eax
        lea    eax, [esp + 8]
        db  0C7h, 044h, 024h, 00Ch, 0B6h, 00Fh, 000h, 000h
        mov    dword ptr [esp + 10h], esi
        push   eax
        db  0A1h, 058h, 02Ah, 001h, 000h
        call   dword ptr [eax + 4]
        add    esp, 4
        mov    edi, eax
        cmp    edi, 0ffh
        je     SHORT loc_1_0F96
        push   0
        push   edi
        push   esi
        call   sub_1_0FB6
        add    esp, 0ch
loc_1_0F96:
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 10h
        ret
sub_1_0F9E:
        mov    eax, dword ptr [esp + 4]
        push   eax
        mov    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    dword ptr [ecx + 4], edx
        call   sub_1_F396
        add    esp, 4
        ret
sub_1_0FB6:
        mov    eax, dword ptr [esp + 4]
        push   eax
        mov    dword ptr [eax + 10h], 0
        call   sub_1_0F9E
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h
loc_1_0FD0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 18h]
        mov    eax, dword ptr [ebx + 18h]
        mov    dword ptr [esp + 10h], eax
        cmp    dword ptr [ebx + 14h], 4
        je     NEAR PTR loc_1_1088
        mov    eax, dword ptr [ebx + 44h]
        sub    eax, ebx
        cmp    eax, 44h
        je     NEAR PTR loc_1_1088
        cmp    dword ptr [esp + 10h], 0
        je     NEAR PTR loc_1_1088
        cmp    dword ptr [esp + 1ch], 22h
        jb     SHORT loc_1_1088
        mov    eax, dword ptr [esp + 20h]
        cmp    word ptr [eax + 6], 15h
        jne    SHORT loc_1_1088
        push   ebx
        call   sub_1_0F2D
        mov    ebp, eax
        add    esp, 4
        mov    dword ptr [ebp + 18h], 60000002h
        mov    word ptr [ebp + 34h], 12h
        mov    esi, dword ptr [esp + 20h]
        add    esi, 8
        lea    edi, [ebp + 3ch]
        movsd
        movsd
        movsd
        movsd
        mov    eax, dword ptr [esp + 20h]
        push   ebp
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp + 38h], eax
        mov    eax, dword ptr [esp + 14h]
        inc    dword ptr [eax + 48h]
        call   sub_1_06C9
        add    esp, 4
        push   ebx
        call   sub_1_0B0B
        mov    eax, dword ptr [esp + 3ch]
        add    esp, 4
        db  0C7h, 000h, 0EBh, 012h, 000h, 000h
        mov    dword ptr [eax + 4], ebp
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        mov    eax, dword ptr [esp + 34h]
        mov    dword ptr [eax], ebp
        mov    eax, 18h
        jmp    SHORT loc_1_108D
loc_1_1088:
        mov    eax, 0c00000d0h
loc_1_108D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_1095:
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edx + 0ch]
        or     eax, 1
        mov    dword ptr [edx + 0ch], eax
        test   al, 2
        je     SHORT loc_1_10B2
        push   edx
        call   sub_1_0655
        add    esp, 4
        jmp    SHORT loc_1_110D
loc_1_10B2:
        mov    ecx, dword ptr [edx + 18h]
        cmp    ecx, 7
        je     SHORT loc_1_110D
        mov    eax, dword ptr [esp + 20h]
        and    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 3fffffffh
        add    eax, 6
        mov    dword ptr [edx + 18h], eax
        cmp    ecx, 60000002h
        je     SHORT loc_1_110D
        mov    esi, dword ptr [edx + 10h]
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_10F2
        mov    eax, dword ptr [esi + 8ch]
        mov    ecx, dword ptr [eax + 2ch]
        sub    ecx, eax
        cmp    ecx, 2ch
        jne    SHORT loc_1_110D
loc_1_10F2:
        push   0
        push   20h
        push   esi
        call   sub_1_F3AC
        add    esp, 0ch
        push   2746h
        push   esi
        call   sub_1_F3B2
        add    esp, 8
loc_1_110D:
        sub    eax, eax
        pop    esi
        ret
loc_1_1111:
        sub    eax, eax
        ret
loc_1_1114:
        push   esi
        mov    esi, dword ptr [esp + 1ch]
        mov    edx, dword ptr [esp + 0ch]
        mov    dword ptr [esi], 0
        test   byte ptr [edx + 1bh], 20h
        je     SHORT loc_1_1158
        mov    eax, dword ptr [edx + 10h]
        test   byte ptr [eax + 40h], 80h
        jne    SHORT loc_1_1158
        db  068h, 000h, 02Ah, 001h, 000h
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 24h]
        push   dword ptr [edx + 58h]
        call   sub_1_135D
        add    esp, 14h
        cmp    eax, 1
        sbb    eax, eax
        add    eax, 18h
        jmp    SHORT loc_1_1169
loc_1_1158:
        push   edx
        call   sub_1_089A
        add    esp, 4
        sub    eax, eax
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [esi], ecx
loc_1_1169:
        pop    esi
        ret
loc_1_116B:
        push   esi
        mov    esi, dword ptr [esp + 1ch]
        mov    edx, dword ptr [esp + 0ch]
        mov    dword ptr [esi], 0
        test   byte ptr [edx + 1bh], 20h
        je     SHORT loc_1_11AF
        mov    eax, dword ptr [edx + 10h]
        test   byte ptr [eax + 40h], 80h
        jne    SHORT loc_1_11AF
        db  068h, 004h, 02Ah, 001h, 000h
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 24h]
        push   dword ptr [edx + 58h]
        call   sub_1_135D
        add    esp, 14h
        cmp    eax, 1
        sbb    eax, eax
        add    eax, 18h
        jmp    SHORT loc_1_11C0
loc_1_11AF:
        push   edx
        call   sub_1_089A
        add    esp, 4
        sub    eax, eax
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [esi], ecx
loc_1_11C0:
        pop    esi
        ret
loc_1_11C2:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0ch]
        mov    eax, dword ptr [edx + 18h]
        or     eax, eax
        je     NEAR PTR loc_1_1292
        mov    ecx, dword ptr [eax + 88h]
        or     ecx, ecx
        je     NEAR PTR loc_1_1292
        mov    al, byte ptr [ecx + 3ch]
        add    ecx, 34h
        mov    esi, dword ptr [esp + 14h]
        cmp    byte ptr [esi + 8], al
        jne    NEAR PTR loc_1_1284
        mov    al, byte ptr [ecx + 9]
        cmp    byte ptr [esi + 9], al
        jne    NEAR PTR loc_1_1284
        mov    al, byte ptr [ecx + 0ah]
        cmp    byte ptr [esi + 0ah], al
        jne    SHORT loc_1_1284
        mov    al, byte ptr [ecx + 0bh]
        cmp    byte ptr [esi + 0bh], al
        jne    SHORT loc_1_1284
        mov    al, byte ptr [ecx + 0ch]
        cmp    byte ptr [esi + 0ch], al
        jne    SHORT loc_1_1284
        mov    al, byte ptr [ecx + 0dh]
        cmp    byte ptr [esi + 0dh], al
        jne    SHORT loc_1_1284
        mov    al, byte ptr [ecx + 0eh]
        cmp    byte ptr [esi + 0eh], al
        jne    SHORT loc_1_1284
        mov    al, byte ptr [ecx + 0fh]
        cmp    byte ptr [esi + 0fh], al
        jne    SHORT loc_1_1284
        mov    al, byte ptr [ecx + 10h]
        cmp    byte ptr [esi + 10h], al
        jne    SHORT loc_1_1284
        mov    al, byte ptr [ecx + 11h]
        cmp    byte ptr [esi + 11h], al
        jne    SHORT loc_1_1284
        mov    al, byte ptr [ecx + 12h]
        cmp    byte ptr [esi + 12h], al
        jne    SHORT loc_1_1284
        mov    al, byte ptr [ecx + 13h]
        cmp    byte ptr [esi + 13h], al
        jne    SHORT loc_1_1284
        mov    al, byte ptr [ecx + 14h]
        cmp    byte ptr [esi + 14h], al
        jne    SHORT loc_1_1284
        mov    al, byte ptr [ecx + 15h]
        cmp    byte ptr [esi + 15h], al
        jne    SHORT loc_1_1284
        mov    al, byte ptr [ecx + 16h]
        cmp    byte ptr [esi + 16h], al
        jne    SHORT loc_1_1284
        mov    al, byte ptr [ecx + 17h]
        cmp    byte ptr [esi + 17h], al
        jne    SHORT loc_1_1284
        mov    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [esi + 18h]
        cmp    eax, ecx
        je     SHORT loc_1_1296
        cmp    eax, -1
        je     SHORT loc_1_1296
        cmp    ecx, -1
        je     SHORT loc_1_1296
loc_1_1284:
        mov    ecx, dword ptr [esp + 2ch]
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        jmp    SHORT loc_1_12E8
loc_1_1292:
        mov    esi, dword ptr [esp + 14h]
loc_1_1296:
        mov    ecx, dword ptr [esp + 2ch]
        mov    dword ptr [ecx], 0
        test   byte ptr [edx + 17h], 40h
        je     SHORT loc_1_12E3
        db  068h, 000h, 02Ah, 001h, 000h
        push   0
        push   0
        push   dword ptr [esp + 34h]
        push   dword ptr [edx + 54h]
        call   sub_1_135D
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_1_12E3
        add    esi, 8
        lea    edi, [eax + 40h]
        mov    ecx, 6
        rep movsd
        movsw
        mov    ecx, dword ptr [esp + 34h]
        add    eax, 8ah
        mov    dword ptr [ecx], eax
        mov    eax, 18h
        jmp    SHORT loc_1_12E8
loc_1_12E3:
        mov    eax, 17h
loc_1_12E8:
        pop    edi
        pop    esi
        ret
loc_1_12EB:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [esp + 14h], 0
        mov    edi, dword ptr [esi + 14h]
        mov    ebx, dword ptr [esi + 10h]
        jne    SHORT loc_1_1318
        push   esi
        push   edi
        call   sub_1_0ECA
        add    esp, 8
        push   0
        push   8
        push   ebx
        call   sub_1_F3AC
        add    esp, 0ch
        jmp    SHORT loc_1_1347
loc_1_1318:
        cmp    dword ptr [esi + 18h], 60000002h
        jne    SHORT loc_1_1347
        mov    ecx, dword ptr [esi + 54h]
        lea    eax, [esi + 50h]
        mov    dword ptr [esi + 18h], 1
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        push   esi
        push   dword ptr [ebx + 84h]
        call   sub_1_0EE8
        add    esp, 8
        dec    dword ptr [ebx + 48h]
loc_1_1347:
        push   edi
        call   sub_1_0B13
        add    esp, 4
        push   esi
        call   sub_1_06D1
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_135D:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        or     esi, esi
        je     SHORT loc_1_138D
        cmp    dword ptr [esi + 1ch], 0
        ja     SHORT loc_1_138D
        mov    eax, dword ptr [esi + 10h]
        cmp    dword ptr [esi + 18h], eax
        ja     SHORT loc_1_138D
        cmp    dword ptr [esi + 14h], 10h
        ja     SHORT loc_1_138D
        mov    ebx, dword ptr [esp + 14h]
        push   ebx
        call   sub_1_12F70
        add    esp, 4
        mov    edi, eax
        jmp    SHORT loc_1_1393
loc_1_138D:
        sub    edi, edi
        mov    ebx, dword ptr [esp + 14h]
loc_1_1393:
        or     edi, edi
        je     SHORT loc_1_13E1
        mov    dword ptr [edi + 18h], esi
        push   dword ptr [esp + 18h]
        call   sub_1_2DBB
        add    esp, 4
        or     dword ptr [edi + 1ch], eax
        push   dword ptr [esi]
        call   sub_1_29BC
        mov    ecx, dword ptr [esp + 20h]
        add    esp, 4
        or     ecx, ecx
        jne    SHORT loc_1_13C1
        lea    ecx, [edi + 8ah]
loc_1_13C1:
        mov    eax, dword ptr [edi + 28h]
        add    eax, 0ch
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], ebx
        db  0C7h, 041h, 008h, 0F4h, 013h, 000h, 000h
        mov    dword ptr [ecx + 0ch], edi
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, edi
        jmp    SHORT loc_1_13F0
loc_1_13E1:
        add    dword ptr [esi + 1ch], ebx
        mov    eax, dword ptr [esp + 18h]
        and    eax, 60h
        mov    dword ptr [esi + 20h], eax
        sub    eax, eax
loc_1_13F0:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_13F4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        mov    esi, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 1ch]
        and    eax, 1
        mov    edi, dword ptr [esi]
        cmp    eax, 1
        sbb    ebx, ebx
        and    ebx, 0fffffffdh
        add    ebx, 4
        cmp    dword ptr [esp + 18h], 0
        jne    SHORT loc_1_143F
        mov    dword ptr [ebp + 18h], edi
        push   dword ptr [esp + 1ch]
        push   ebp
        call   sub_1_1324F
        add    esp, 8
        push   ebp
        push   esi
        call   sub_1_1833
        mov    eax, dword ptr [esp + 24h]
        add    esp, 8
        add    dword ptr [esi + 18h], eax
        inc    dword ptr [esi + 14h]
        jmp    SHORT loc_1_144C
loc_1_143F:
        or     dword ptr [edi + 3ch], 1
        push   ebp
        call   sub_1_1301F
        add    esp, 4
loc_1_144C:
        push   edi
        call   sub_1_29C4
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_1470
        push   dword ptr [esp + 18h]
        call   sub_1_2C80
        add    esp, 4
        push   eax
        push   ebx
        push   edi
        call   sub_1_F3AC
        add    esp, 0ch
loc_1_1470:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1480:
        sub    esp, 8
        push   esi
        push   edi
        sub    esi, esi
        mov    eax, dword ptr [esp + 18h]
        cmp    eax, 4004667fh
        je     SHORT loc_1_149E
        cmp    eax, 40047307h
        je     SHORT loc_1_14B5
        jmp    NEAR PTR loc_1_1565
loc_1_149E:
        push   dword ptr [esp + 14h]
        call   sub_1_1572
        mov    ecx, dword ptr [esp + 20h]
        add    esp, 4
        mov    dword ptr [ecx], eax
        jmp    NEAR PTR loc_1_156A
loc_1_14B5:
        mov    edi, dword ptr [esp + 14h]
        cmp    dword ptr [edi + 2ch], 1
        jne    NEAR PTR loc_1_1565
        test   byte ptr [edi + 40h], 10h
        je     NEAR PTR loc_1_1565
        mov    eax, dword ptr [edi + 88h]
        or     eax, eax
        jne    SHORT loc_1_14E6
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], 1
        jmp    NEAR PTR loc_1_156A
loc_1_14E6:
        lea    ecx, [esp + 8]
        push   8
        push   ecx
        push   5
        push   eax
        call   sub_1_0837
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_1509
        push   eax
        call   sub_1_2C80
        add    esp, 4
        mov    esi, eax
        jmp    SHORT loc_1_156A
loc_1_1509:
        cmp    dword ptr [esp + 8], 0
        jne    SHORT loc_1_1523
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_1_1528
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], 1
        jmp    SHORT loc_1_156A
loc_1_1523:
        cmp    dword ptr [esp + 0ch], 0
loc_1_1528:
        jle    SHORT loc_1_1536
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], 0
        jmp    SHORT loc_1_156A
loc_1_1536:
        mov    eax, dword ptr [edi + 8ch]
        or     eax, eax
        jne    SHORT loc_1_154C
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], 1
        jmp    SHORT loc_1_156A
loc_1_154C:
        mov    ecx, dword ptr [esp + 0ch]
        neg    ecx
        cmp    dword ptr [eax + 18h], ecx
        mov    eax, 0
        setbe  al
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_1_156A
loc_1_1565:
        mov    esi, 2726h
loc_1_156A:
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 8
        ret
sub_1_1572:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        sub    eax, eax
        mov    edi, dword ptr [esi + 8ch]
        cmp    edi, eax
        jne    SHORT loc_1_1586
        jmp    SHORT loc_1_15C9
loc_1_1586:
        cmp    dword ptr [esi + 2ch], 1
        jne    SHORT loc_1_15B1
        mov    eax, dword ptr [edi + 18h]
        test   byte ptr [esi + 40h], 10h
        jne    SHORT loc_1_15C0
        mov    edx, dword ptr [edi + 3ch]
        mov    ecx, edi
        sub    ecx, edx
        cmp    ecx, -3ch
        je     SHORT loc_1_15C0
loc_1_15A1:
        sub    eax, dword ptr [edx + 18h]
        mov    edx, dword ptr [edx]
        mov    ecx, edi
        sub    ecx, edx
        cmp    ecx, -3ch
        jne    SHORT loc_1_15A1
        jmp    SHORT loc_1_15C0
loc_1_15B1:
        mov    ecx, dword ptr [edi + 2ch]
        mov    edx, ecx
        sub    edx, edi
        cmp    edx, 2ch
        je     SHORT loc_1_15C0
        mov    eax, dword ptr [ecx + 20h]
loc_1_15C0:
        mov    ecx, dword ptr [esi + 38h]
        cmp    eax, ecx
        jbe    SHORT loc_1_15C9
        mov    eax, ecx
loc_1_15C9:
        pop    edi
        pop    esi
        ret
        db  000h, 000h, 000h, 000h
loc_1_15D0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        push   esi
        call   sub_1_29BC
        add    esp, 4
        mov    edi, dword ptr [esi + 84h]
        mov    ebx, dword ptr [esp + 14h]
        add    dword ptr [edi + 38h], ebx
        mov    dword ptr [edi + 40h], ebx
        push   edi
        call   sub_1_0E7C
        add    esp, 4
        mov    eax, dword ptr [edi + 44h]
        sub    eax, edi
        cmp    eax, 44h
        jne    SHORT loc_1_1613
        push   esi
        call   sub_1_29C4
        add    esp, 4
        mov    eax, 2747h
        jmp    SHORT loc_1_1639
loc_1_1613:
        mov    dword ptr [edi + 14h], 40000003h
        mov    dword ptr [esi + 4ch], 3
        movzx  eax, bl
        mov    dword ptr [esi + 44h], eax
        mov    dword ptr [esi + 54h], 37h
        push   esi
        call   sub_1_29C4
        add    esp, 4
        sub    eax, eax
loc_1_1639:
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h
sub_1_1640:
        mov    ecx, dword ptr [esp + 0ch]
        or     ecx, ecx
        jne    SHORT loc_1_1654
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0c000009ah
        jmp    SHORT loc_1_169A
loc_1_1654:
        mov    edx, dword ptr [ecx + 8]
        or     edx, edx
        jne    SHORT loc_1_1667
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax], 0c000009ah
        jmp    SHORT loc_1_169A
loc_1_1667:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [edx], 0
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edx + 8], ecx
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edx + 0ch], eax
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [edx + 10h], 4655424eh
        mov    dword ptr [eax], 0
loc_1_169A:
        ret    14h
sub_1_169D:
        push   esi
        mov    esi, dword ptr [esp + 10h]
        mov    eax, esi
        imul   eax, eax, 14h
        add    eax, 11h
        push   eax
        call   sub_1_F390
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_16CD
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], 0c000009ah
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [ecx], 0
        jmp    SHORT loc_1_1700
loc_1_16CD:
        mov    dword ptr [eax + 0ch], 14h
        lea    edx, [eax + 10h]
        mov    dword ptr [eax + 8], edx
        cmp    esi, 1
        jbe    SHORT loc_1_16EA
        dec    esi
loc_1_16E0:
        lea    ecx, [edx + 14h]
        mov    dword ptr [edx], ecx
        mov    edx, ecx
        dec    esi
        jne    SHORT loc_1_16E0
loc_1_16EA:
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [edx], 0
        mov    dword ptr [ecx], 0
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [ecx], eax
loc_1_1700:
        pop    esi
        ret    0ch
sub_1_1704:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     SHORT loc_1_171B
        mov    ecx, dword ptr [eax + 8]
        or     ecx, ecx
        je     SHORT loc_1_171B
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 8], eax
loc_1_171B:
        ret    4
sub_1_171E:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        jne    SHORT loc_1_172D
        mov    eax, 0c000009ah
        jmp    SHORT loc_1_1738
loc_1_172D:
        push   eax
        call   sub_1_F396
        add    esp, 4
        sub    eax, eax
loc_1_1738:
        ret    4
sub_1_173B:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     SHORT loc_1_1759
        mov    ecx, dword ptr [esp + 8]
        or     ecx, ecx
        je     SHORT loc_1_1750
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
loc_1_1750:
        mov    ecx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
loc_1_1759:
        ret    0ch
        db  000h, 000h, 000h, 000h
sub_1_1760:
        push   edi
        call   sub_1_F3A4
        push   44h
        call   sub_1_F390
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_1_177A
        sub    eax, eax
        jmp    SHORT loc_1_17BF
loc_1_177A:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 11h
        rep stosd
        lea    eax, [edx + 24h]
        mov    dword ptr [edx + 28h], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 2ch]
        mov    dword ptr [edx + 30h], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 34h]
        mov    dword ptr [edx + 38h], eax
        mov    dword ptr [eax], eax
        lea    eax, [edx + 3ch]
        mov    dword ptr [edx + 40h], eax
        mov    dword ptr [eax], eax
        mov    ecx, dword ptr [esp + 8]
        or     ecx, ecx
        je     SHORT loc_1_17BD
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [ecx + 10h]
        mov    dword ptr [edx + 10h], eax
loc_1_17BD:
        mov    eax, edx
loc_1_17BF:
        pop    edi
        ret
sub_1_17C1:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_18C9
        add    esp, 4
        push   esi
        call   sub_1_190E
        add    esp, 4
        push   esi
        call   sub_1_192D
        add    esp, 4
        pop    esi
        ret
sub_1_17E3:
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx], eax
        mov    ecx, dword ptr [eax + 34h]
        mov    dword ptr [edx + 4], ecx
        mov    eax, dword ptr [eax + 38h]
        mov    dword ptr [edx + 10h], eax
        ret
sub_1_17FA:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 8]
        lea    edx, [esi + 8]
        lea    eax, [ecx + 24h]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 28h]
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 28h], edx
        db  0C7h, 006h, 04Ch, 036h, 001h, 000h
        db  0A1h, 050h, 036h, 001h, 000h
        mov    dword ptr [esi + 4], eax
        db  0A1h, 050h, 036h, 001h, 000h
        mov    dword ptr [eax], esi
        db  089h, 035h, 050h, 036h, 001h, 000h
        pop    esi
        ret
sub_1_1833:
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 8]
        test   byte ptr [edx + 1ch], 1
        je     SHORT loc_1_1847
        lea    esi, [ecx + 3ch]
        jmp    SHORT loc_1_184A
loc_1_1847:
        lea    esi, [ecx + 34h]
loc_1_184A:
        lea    eax, [ecx + 2ch]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 30h]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
        mov    dword ptr [ecx + 30h], edx
        mov    dword ptr [edx + 8], esi
        lea    ecx, [edx + 8]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [eax], ecx
        mov    dword ptr [esi + 4], ecx
        pop    esi
        ret
sub_1_186D:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 24h]
        lea    esi, [ecx + 24h]
        mov    edx, eax
        sub    edx, ecx
        cmp    edx, 24h
        jne    SHORT loc_1_1885
        sub    eax, eax
        jmp    SHORT loc_1_1899
loc_1_1885:
        mov    ecx, dword ptr [eax]
        sub    eax, 8
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [esi], ecx
        mov    ecx, dword ptr [eax + 4]
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
loc_1_1899:
        pop    esi
        ret
sub_1_189B:
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 2ch]
        lea    esi, [ecx + 2ch]
        mov    edx, eax
        sub    edx, ecx
        cmp    edx, 2ch
        jne    SHORT loc_1_18B3
        sub    eax, eax
        jmp    SHORT loc_1_18C7
loc_1_18B3:
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [esi], ecx
        mov    edx, dword ptr [eax + 0ch]
        lea    ecx, [eax + 8]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edx], ecx
        mov    dword ptr [ecx + 4], edx
loc_1_18C7:
        pop    esi
        ret
sub_1_18C9:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        mov    eax, dword ptr [edi + 24h]
        sub    eax, edi
        cmp    eax, 24h
        je     SHORT loc_1_190A
        mov    ebx, dword ptr [edi]
loc_1_18DC:
        push   edi
        call   sub_1_186D
        add    esp, 4
        push   eax
        call   sub_1_1301F
        add    esp, 4
        mov    eax, dword ptr [edi + 24h]
        sub    eax, edi
        sub    eax, 24h
        cmp    eax, 1
        sbb    esi, esi
        neg    esi
        push   ebx
        call   sub_1_29C4
        add    esp, 4
        or     esi, esi
        je     SHORT loc_1_18DC
loc_1_190A:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_190E:
        push   esi
        mov    esi, dword ptr [esp + 8]
loc_1_1913:
        push   esi
        call   sub_1_189B
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_192B
        push   eax
        call   sub_1_1301F
        add    esp, 4
        jmp    SHORT loc_1_1913
loc_1_192B:
        pop    esi
        ret
sub_1_192D:
        push   dword ptr [esp + 4]
        call   sub_1_F396
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_1_1940:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 1ch]
        sub    esi, esi
        push   dword ptr [esp + 1ch]
        call   sub_1_F270
        add    esp, 8
        mov    edi, eax
        cmp    edi, esi
        je     NEAR PTR loc_1_19EF
        cmp    dword ptr [esp + 2ch], esi
        je     NEAR PTR loc_1_19EF
        mov    eax, dword ptr [esp + 14h]
        and    dword ptr [eax + 54h], 0fffffffeh
        cmp    dword ptr [eax + 2ch], 2
        jne    SHORT loc_1_19D2
        mov    ebx, dword ptr [esp + 24h]
        cmp    ebx, esi
        je     SHORT loc_1_19AF
        mov    ebp, dword ptr [esp + 28h]
        cmp    ebp, esi
        je     SHORT loc_1_19AF
        mov    esi, dword ptr [ebp]
        push   esi
        push   ebx
        call   sub_1_F270
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_1_19A2
        mov    ebp, 271eh
        jmp    SHORT loc_1_19F8
loc_1_19A2:
        cmp    dword ptr [ebp], 18h
        je     SHORT loc_1_19B3
        mov    ebp, 271eh
        jmp    SHORT loc_1_19F8
loc_1_19AF:
        sub    ebx, ebx
        sub    ebp, ebp
loc_1_19B3:
        push   dword ptr [esp + 2ch]
        push   ebp
        push   ebx
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        push   edi
        push   dword ptr [esp + 2ch]
        call   sub_1_1C21
        add    esp, 1ch
        mov    ebp, eax
        jmp    SHORT loc_1_19F8
loc_1_19D2:
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 24h]
        push   dword ptr [esp + 24h]
        push   edi
        push   dword ptr [esp + 24h]
        call   sub_1_1A1A
        add    esp, 14h
        mov    ebp, eax
        jmp    SHORT loc_1_19F4
loc_1_19EF:
        mov    ebp, 271eh
loc_1_19F4:
        mov    ebx, dword ptr [esp + 24h]
loc_1_19F8:
        push   dword ptr [esp + 1ch]
        push   edi
        call   sub_1_F2B5
        add    esp, 8
        or     esi, esi
        je     SHORT loc_1_1A13
        push   esi
        push   ebx
        call   sub_1_F2B5
        add    esp, 8
loc_1_1A13:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_1A1A:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 28h], 0
        mov    esi, dword ptr [esp + 30h]
        mov    dword ptr [esp + 18h], 0
        mov    edi, dword ptr [esi + 8ch]
        mov    eax, dword ptr [esp + 3ch]
        and    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 1ch], eax
        push   edi
        push   esi
        call   sub_1_1D84
        add    esp, 8
        mov    ecx, dword ptr [edi + 2ch]
        lea    edx, [edi + 2ch]
        mov    eax, ecx
        sub    eax, edi
        cmp    eax, 2ch
        je     NEAR PTR loc_1_1B42
        mov    eax, dword ptr [esi + 40h]
        and    eax, 10h
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 20h], eax
        or     eax, eax
        je     SHORT loc_1_1A8D
        mov    dword ptr [esp + 14h], edx
        mov    eax, dword ptr [ecx + 1ch]
        and    eax, 1
        mov    dword ptr [esp + 24h], eax
        jmp    SHORT loc_1_1AA0
loc_1_1A8D:
        test   byte ptr [esp + 3ch], 1
        je     SHORT loc_1_1A99
        lea    eax, [edi + 3ch]
        jmp    SHORT loc_1_1A9C
loc_1_1A99:
        lea    eax, [edi + 34h]
loc_1_1A9C:
        mov    dword ptr [esp + 14h], eax
loc_1_1AA0:
        mov    eax, dword ptr [esp + 14h]
        mov    ebx, dword ptr [eax]
        cmp    eax, ebx
        je     NEAR PTR loc_1_1B39
loc_1_1AAE:
        cmp    dword ptr [esp + 20h], 0
        je     SHORT loc_1_1AC5
        mov    ebp, ebx
        mov    eax, dword ptr [ebx + 1ch]
        and    eax, 1
        cmp    eax, dword ptr [esp + 24h]
        je     SHORT loc_1_1AC8
        jmp    SHORT loc_1_1B39
loc_1_1AC5:
        lea    ebp, [ebx - 8]
loc_1_1AC8:
        push   dword ptr [esp + 1ch]
        mov    ebx, dword ptr [ebx]
        push   dword ptr [esp + 3ch]
        push   ebp
        push   dword ptr [esp + 40h]
        call   sub_1_130AB
        mov    dword ptr [esp + 20h], eax
        add    esp, 10h
        mov    eax, dword ptr [ebp + 30h]
        cmp    dword ptr [ebp + 28h], eax
        jne    SHORT loc_1_1B1D
        mov    eax, dword ptr [ebp + 34h]
        cmp    dword ptr [ebp + 2ch], eax
        jne    SHORT loc_1_1B1D
        mov    eax, dword ptr [ebp + 20h]
        sub    dword ptr [edi + 18h], eax
        dec    dword ptr [edi + 14h]
        mov    ecx, dword ptr [ebp + 4]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        lea    eax, [ebp + 8]
        push   ebp
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        call   sub_1_1301F
        add    esp, 4
loc_1_1B1D:
        mov    eax, dword ptr [esp + 10h]
        add    dword ptr [esp + 18h], eax
        sub    dword ptr [esp + 38h], eax
        je     SHORT loc_1_1B39
        add    dword ptr [esp + 34h], eax
        cmp    dword ptr [esp + 14h], ebx
        jne    NEAR PTR loc_1_1AAE
loc_1_1B39:
        cmp    dword ptr [esp + 18h], 0
        jne    SHORT loc_1_1B72
        jmp    SHORT loc_1_1B6A
loc_1_1B42:
        mov    eax, dword ptr [esi + 88h]
        or     eax, eax
        je     SHORT loc_1_1B6A
        cmp    dword ptr [eax + 18h], 40000005h
        jne    SHORT loc_1_1B6A
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_1B72
        push   0
        push   20h
        push   esi
        call   sub_1_F3AC
        add    esp, 0ch
        jmp    SHORT loc_1_1B72
loc_1_1B6A:
        mov    dword ptr [esp + 28h], 2733h
loc_1_1B72:
        cmp    dword ptr [esp + 1ch], 0
        jne    SHORT loc_1_1BA4
        call   sub_1_2117
        push   edi
        push   esi
        call   sub_1_1D84
        add    esp, 8
        mov    eax, dword ptr [edi + 3ch]
        sub    eax, edi
        cmp    eax, 3ch
        jne    SHORT loc_1_1B96
        and    dword ptr [esi + 50h], 0fffffffbh
loc_1_1B96:
        mov    eax, dword ptr [edi + 34h]
        sub    eax, edi
        cmp    eax, 34h
        jne    SHORT loc_1_1BA4
        and    dword ptr [esi + 50h], 0fffffffeh
loc_1_1BA4:
        cmp    dword ptr [esi + 58h], 0
        je     SHORT loc_1_1C0B
        mov    ecx, dword ptr [esp + 3ch]
        and    ecx, 1
        je     SHORT loc_1_1BBD
        mov    eax, dword ptr [edi + 3ch]
        sub    eax, edi
        sub    eax, 3ch
        jmp    SHORT loc_1_1BC5
loc_1_1BBD:
        mov    eax, dword ptr [edi + 34h]
        sub    eax, edi
        sub    eax, 34h
loc_1_1BC5:
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        or     eax, eax
        jne    SHORT loc_1_1BED
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_1_1C0B
        mov    eax, dword ptr [esi + 88h]
        cmp    dword ptr [eax + 18h], 40000005h
        jne    SHORT loc_1_1C0B
loc_1_1BED:
        cmp    ecx, 1
        sbb    eax, eax
        and    eax, 0fffffffdh
        add    eax, 4
        mov    ecx, eax
        not    ecx
        and    dword ptr [esi + 50h], ecx
        push   0
        push   eax
        push   esi
        call   sub_1_F3AC
        add    esp, 0ch
loc_1_1C0B:
        mov    eax, dword ptr [esp + 40h]
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 28h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
sub_1_1C21:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    dword ptr [esp + 10h], 0
        mov    ebx, dword ptr [eax + 8ch]
        push   ebx
        push   dword ptr [esp + 1ch]
        call   sub_1_1D84
        add    esp, 8
        mov    ebp, dword ptr [ebx + 2ch]
        mov    eax, ebp
        sub    eax, ebx
        cmp    eax, 2ch
        je     NEAR PTR loc_1_1D19
        mov    ecx, dword ptr [esp + 28h]
        or     ecx, ecx
        je     SHORT loc_1_1C83
        mov    eax, dword ptr [esp + 18h]
        lea    esi, [ebp + 40h]
        mov    eax, dword ptr [eax + 28h]
        mov    word ptr [ecx], ax
        lea    edi, [ecx + 8]
        movsd
        movsd
        movsd
        movsd
        mov    eax, dword ptr [ebp + 50h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 2ch]
        mov    dword ptr [eax], 18h
loc_1_1C83:
        mov    esi, dword ptr [esp + 24h]
        mov    eax, esi
        and    eax, 2
        push   eax
        push   dword ptr [esp + 24h]
        push   ebp
        push   dword ptr [esp + 28h]
        call   sub_1_130AB
        mov    ecx, dword ptr [esp + 40h]
        add    esp, 10h
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 30h]
        cmp    dword ptr [ebp + 28h], eax
        jne    SHORT loc_1_1CE0
        mov    eax, dword ptr [ebp + 34h]
        cmp    dword ptr [ebp + 2ch], eax
        jne    SHORT loc_1_1CE0
        mov    eax, dword ptr [ebp + 20h]
        sub    dword ptr [ebx + 18h], eax
        dec    dword ptr [ebx + 14h]
        mov    ecx, dword ptr [ebp + 4]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        lea    eax, [ebp + 8]
        push   ebp
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        call   sub_1_1301F
        add    esp, 4
        jmp    SHORT loc_1_1D12
loc_1_1CE0:
        mov    eax, dword ptr [ebp + 20h]
        sub    dword ptr [ebx + 18h], eax
        dec    dword ptr [ebx + 14h]
        mov    ecx, dword ptr [ebp + 4]
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebp + 0ch]
        lea    eax, [ebp + 8]
        push   ebp
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        call   sub_1_1301F
        mov    dword ptr [esp + 14h], 2738h
        add    esp, 4
loc_1_1D12:
        call   sub_1_2117
        jmp    SHORT loc_1_1D25
loc_1_1D19:
        mov    dword ptr [esp + 10h], 2733h
        mov    esi, dword ptr [esp + 24h]
loc_1_1D25:
        test   esi, 2
        jne    SHORT loc_1_1D4C
        mov    eax, dword ptr [ebx + 2ch]
        sub    eax, ebx
        cmp    eax, 2ch
        jne    SHORT loc_1_1D3F
        mov    eax, dword ptr [esp + 18h]
        and    dword ptr [eax + 50h], 0fffffffeh
loc_1_1D3F:
        push   ebx
        push   dword ptr [esp + 1ch]
        call   sub_1_1D84
        add    esp, 8
loc_1_1D4C:
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [eax + 58h], 0
        je     SHORT loc_1_1D78
        mov    eax, dword ptr [ebx + 2ch]
        sub    eax, ebx
        cmp    eax, 2ch
        je     SHORT loc_1_1D78
        mov    eax, dword ptr [esp + 18h]
        push   0
        push   1
        and    dword ptr [eax + 50h], 0fffffffeh
        push   dword ptr [esp + 20h]
        call   sub_1_F3AC
        add    esp, 0ch
loc_1_1D78:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_1D84:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 28h]
        mov    eax, dword ptr [edi + 84h]
        test   byte ptr [eax + 17h], 40h
        je     NEAR PTR loc_1_1E9C
        mov    ebp, dword ptr [esp + 2ch]
        mov    ebx, dword ptr [ebp + 1ch]
        or     ebx, ebx
        je     NEAR PTR loc_1_1E9C
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp + 18h], eax
        ja     NEAR PTR loc_1_1E9C
        cmp    dword ptr [ebp + 14h], 10h
        ja     NEAR PTR loc_1_1E9C
        push   ebx
        call   sub_1_12F70
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     NEAR PTR loc_1_1E9C
        mov    dword ptr [esi + 18h], edi
        push   dword ptr [ebp + 20h]
        add    ebp, 20h
        call   sub_1_2DBB
        add    esp, 4
        or     dword ptr [esi + 1ch], eax
        db  0C7h, 044h, 024h, 018h, 0A4h, 01Eh, 000h, 000h
        mov    dword ptr [esp + 1ch], esi
        push   edi
        call   sub_1_29BC
        add    esp, 4
        cmp    dword ptr [edi + 2ch], 2
        jne    SHORT loc_1_1E47
        mov    eax, dword ptr [edi + 84h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 14h], eax
        lea    eax, [esi + 38h]
        mov    dword ptr [esi + 6eh], eax
        mov    eax, dword ptr [esi + 28h]
        add    eax, 0ch
        push   eax
        mov    dword ptr [esi + 6ah], 22h
        lea    eax, [esp + 14h]
        push   eax
        push   ebx
        lea    eax, [esi + 72h]
        push   eax
        lea    eax, [esi + 5ah]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        db  0A1h, 058h, 02Ah, 001h, 000h
        call   dword ptr [eax + 34h]
        add    esp, 18h
        jmp    SHORT loc_1_1E71
loc_1_1E47:
        mov    eax, dword ptr [edi + 88h]
        mov    eax, dword ptr [eax + 4ch]
        mov    dword ptr [esp + 14h], eax
        mov    eax, dword ptr [esi + 28h]
        add    eax, 0ch
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 1ch]
        push   ebp
        push   eax
        db  0A1h, 058h, 02Ah, 001h, 000h
        call   dword ptr [eax + 28h]
        add    esp, 10h
loc_1_1E71:
        or     eax, eax
        je     SHORT loc_1_1E90
        cmp    eax, 0ffh
        je     SHORT loc_1_1E9C
        push   esi
        call   sub_1_1301F
        add    esp, 4
        push   edi
        call   sub_1_29C4
        add    esp, 4
        jmp    SHORT loc_1_1E9C
loc_1_1E90:
        push   ebx
        push   0
        push   esi
        call   sub_1_1EA4
        add    esp, 0ch
loc_1_1E9C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret
sub_1_1EA4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 14h]
        mov    esi, dword ptr [ebp + 18h]
        mov    eax, dword ptr [ebp + 1ch]
        and    eax, 1
        mov    edi, dword ptr [esi + 8ch]
        cmp    eax, 1
        sbb    ebx, ebx
        and    ebx, 0fffffffdh
        add    ebx, 4
        cmp    dword ptr [esp + 18h], 0
        jne    SHORT loc_1_1EFE
        mov    dword ptr [edi + 20h], 0
        mov    dword ptr [edi + 1ch], 0
        push   dword ptr [esp + 1ch]
        push   ebp
        call   sub_1_1324F
        add    esp, 8
        push   ebp
        push   edi
        call   sub_1_1833
        mov    eax, dword ptr [esp + 24h]
        add    esp, 8
        add    dword ptr [edi + 18h], eax
        inc    dword ptr [edi + 14h]
        jmp    SHORT loc_1_1F0B
loc_1_1EFE:
        or     dword ptr [esi + 3ch], 1
        push   ebp
        call   sub_1_1301F
        add    esp, 4
loc_1_1F0B:
        push   dword ptr [esp + 18h]
        call   sub_1_2C80
        add    esp, 4
        push   eax
        push   ebx
        push   esi
        call   sub_1_F3AC
        add    esp, 0ch
        push   esi
        call   sub_1_29C4
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_1F30:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 2ch]
        cmp    dword ptr [ebx + 2ch], 2
        jne    NEAR PTR loc_1_2013
        mov    eax, dword ptr [esp + 3ch]
        or     eax, eax
        jne    SHORT loc_1_1F85
        cmp    dword ptr [ebx + 88h], 0
        je     SHORT loc_1_1F7B
        mov    ecx, dword ptr [ebx + 18h]
        lea    edi, [ebx + 0a4h]
        lea    esi, [ecx + 8]
        movsd
        movsd
        movsd
        movsd
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebx + 0b4h], eax
        movzx  eax, word ptr [ecx + 2]
        mov    dword ptr [ebx + 0b8h], eax
        jmp    SHORT loc_1_1FCF
loc_1_1F7B:
        mov    edi, 2749h
        jmp    NEAR PTR loc_1_210D
loc_1_1F85:
        mov    ebp, dword ptr [esp + 40h]
        cmp    ebp, 18h
        jne    NEAR PTR loc_1_2108
        push   ebp
        push   eax
        call   sub_1_F270
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_1_1FA5
        jmp    NEAR PTR loc_1_2108
loc_1_1FA5:
        lea    esi, [eax + 8]
        lea    edi, [ebx + 0a4h]
        movsd
        movsd
        movsd
        movsd
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 0b4h], ecx
        movzx  ecx, word ptr [eax + 2]
        mov    dword ptr [ebx + 0b8h], ecx
        push   ebp
        push   eax
        call   sub_1_F2B5
        add    esp, 8
loc_1_1FCF:
        mov    dword ptr [ebx + 0bch], 1
        cmp    dword ptr [ebx + 4ch], 1
        jne    SHORT loc_1_201D
        mov    word ptr [esp + 10h], 12h
        lea    edi, [esp + 18h]
        sub    eax, eax
        stosd
        stosd
        stosd
        stosd
        mov    dword ptr [esp + 14h], 0ffffffffh
        lea    eax, [esp + 10h]
        push   18h
        push   eax
        push   ebx
        call   sub_1_0180
        add    esp, 0ch
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_201D
        jmp    NEAR PTR loc_1_210D
loc_1_2013:
        mov    dword ptr [ebx + 0bch], 0
loc_1_201D:
        mov    esi, dword ptr [esp + 34h]
        push   esi
        push   dword ptr [esp + 34h]
        call   sub_1_F270
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        je     NEAR PTR loc_1_20FE
        mov    edi, dword ptr [esp + 44h]
        or     edi, edi
        je     NEAR PTR loc_1_20FE
        mov    eax, dword ptr [ebx + 8ch]
        mov    ecx, dword ptr [eax + 0ch]
        cmp    dword ptr [eax + 4], ecx
        jb     SHORT loc_1_2063
        cmp    dword ptr [eax + 8], 10h
        ja     SHORT loc_1_2063
        push   esi
        call   sub_1_12F70
        add    esp, 4
        jmp    SHORT loc_1_2065
loc_1_2063:
        sub    eax, eax
loc_1_2065:
        or     eax, eax
        je     SHORT loc_1_208D
        push   0
        push   edi
        push   eax
        push   dword ptr [esp + 44h]
        push   esi
        push   ebp
        push   ebx
        call   sub_1_2213
        add    esp, 1ch
        mov    edi, eax
        push   esi
        push   ebp
        call   sub_1_F2B5
        add    esp, 8
        jmp    NEAR PTR loc_1_210D
loc_1_208D:
        and    dword ptr [ebx + 50h], 0fffffffdh
        test   byte ptr [ebx + 41h], 1
        jne    SHORT loc_1_20B0
        cmp    dword ptr [ebx + 58h], 0
        je     SHORT loc_1_20F5
        or     dword ptr [ebx + 80h], 4
        push   esi
        push   ebp
        call   sub_1_F2B5
        add    esp, 8
        jmp    SHORT loc_1_20CD
loc_1_20B0:
        mov    dword ptr [ebx + 98h], ebp
        mov    dword ptr [ebx + 9ch], esi
        mov    eax, dword ptr [esp + 38h]
        mov    dword ptr [ebx + 0a0h], eax
        or     dword ptr [ebx + 80h], 2
loc_1_20CD:
        db  0C7h, 083h, 090h, 000h, 000h, 000h, 02Ch, 036h, 001h, 000h
        lea    ecx, [ebx + 90h]
        db  0A1h, 030h, 036h, 001h, 000h
        mov    dword ptr [ebx + 94h], eax
        db  0A1h, 030h, 036h, 001h, 000h
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 030h, 036h, 001h, 000h
loc_1_20F5:
        mov    dword ptr [edi], esi
        mov    edi, 2733h
        jmp    SHORT loc_1_210D
loc_1_20FE:
        push   esi
        push   ebp
        call   sub_1_F2B5
        add    esp, 8
loc_1_2108:
        mov    edi, 271eh
loc_1_210D:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
sub_1_2117:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 035h, 02Ch, 036h, 001h, 000h
        db  081h, 0FEh, 02Ch, 036h, 001h, 000h
        je     NEAR PTR loc_1_220B
        mov    edi, 100h
        mov    ebx, 10h
loc_1_213A:
        lea    ebp, [esi - 90h]
        mov    esi, dword ptr [esi]
        test   byte ptr [ebp + 80h], 4
        jne    NEAR PTR loc_1_21D4
        test   dword ptr [ebp + 40h], edi
        je     NEAR PTR loc_1_21E8
        mov    eax, dword ptr [ebp + 8ch]
        mov    ecx, dword ptr [eax + 4]
        cmp    dword ptr [eax + 0ch], ecx
        ja     SHORT loc_1_217B
        cmp    dword ptr [eax + 8], ebx
        ja     SHORT loc_1_217B
        push   dword ptr [ebp + 9ch]
        call   sub_1_12F70
        add    esp, 4
        jmp    SHORT loc_1_217D
loc_1_217B:
        sub    eax, eax
loc_1_217D:
        or     eax, eax
        je     SHORT loc_1_21FF
        mov    edx, dword ptr [ebp + 94h]
        lea    ecx, [ebp + 90h]
        push   1
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [edx], ecx
        mov    dword ptr [ecx + 4], edx
        lea    ecx, [esp + 14h]
        and    dword ptr [ebp + 80h], 0fffffffdh
        push   ecx
        push   eax
        push   dword ptr [ebp + 0a0h]
        push   dword ptr [ebp + 9ch]
        push   dword ptr [ebp + 98h]
        push   ebp
        call   sub_1_2213
        add    esp, 1ch
        push   dword ptr [ebp + 9ch]
        push   dword ptr [ebp + 98h]
        call   sub_1_F2B5
        add    esp, 8
        jmp    SHORT loc_1_21FF
loc_1_21D4:
        push   0
        push   2
        push   ebp
        call   sub_1_F3AC
        add    esp, 0ch
        and    dword ptr [ebp + 80h], 0fffffffbh
loc_1_21E8:
        mov    ecx, dword ptr [ebp + 94h]
        mov    eax, dword ptr [ebp + 90h]
        add    ebp, 90h
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
loc_1_21FF:
        db  081h, 0FEh, 02Ch, 036h, 001h, 000h
        jne    NEAR PTR loc_1_213A
loc_1_220B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_2213:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], 0
        mov    ebp, dword ptr [esp + 30h]
        mov    ebx, dword ptr [esp + 20h]
        mov    dword ptr [ebp + 18h], ebx
        push   dword ptr [esp + 2ch]
        call   sub_1_2D98
        add    esp, 4
        lea    ecx, [ebp + 38h]
        or     dword ptr [ebp + 1ch], eax
        mov    dword ptr [ecx], 1
        mov    word ptr [ebp + 3ch], 1ah
        mov    word ptr [ebp + 3eh], 15h
        lea    esi, [ebx + 0a4h]
        mov    eax, dword ptr [ebx + 0b4h]
        mov    dword ptr [ebp + 50h], eax
        mov    eax, dword ptr [ebx + 0b8h]
        mov    dword ptr [ebp + 54h], eax
        lea    edi, [ebp + 40h]
        movsd
        movsd
        movsd
        movsd
        mov    dword ptr [ebp + 6eh], ecx
        mov    dword ptr [ebp + 6ah], 22h
        mov    eax, dword ptr [ebx + 8ch]
        mov    ecx, dword ptr [esp + 28h]
        add    dword ptr [eax + 0ch], ecx
        push   ecx
        inc    dword ptr [eax + 8]
        push   dword ptr [esp + 28h]
        push   ebp
        call   sub_1_13196
        lea    eax, [esp + 24h]
        add    esp, 0ch
        push   eax
        lea    eax, [esp + 18h]
        push   1
        push   eax
        push   ebp
        call   sub_1_22EE
        mov    ecx, dword ptr [esp + 44h]
        add    esp, 10h
        mov    dword ptr [ecx], eax
        cmp    eax, -1
        jne    SHORT loc_1_22C8
        push   dword ptr [esp + 14h]
        call   sub_1_2C80
        mov    dword ptr [esp + 14h], eax
        add    esp, 4
loc_1_22C8:
        cmp    dword ptr [esp + 38h], 0
        je     SHORT loc_1_22E2
        and    dword ptr [ebx + 54h], 0fffffffdh
        push   dword ptr [esp + 10h]
        push   2
        push   ebx
        call   sub_1_F3AC
        add    esp, 0ch
loc_1_22E2:
        mov    eax, dword ptr [esp + 10h]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_22EE:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 2ch]
        mov    esi, dword ptr [edi + 18h]
        push   esi
        mov    eax, dword ptr [esi + 8ch]
        mov    dword ptr [esp + 14h], eax
        call   sub_1_29BC
        mov    eax, dword ptr [esp + 3ch]
        add    esp, 4
        mov    dword ptr [eax], 0
        db  0C7h, 044h, 024h, 01Ch, 03Ch, 024h, 000h, 000h
        mov    dword ptr [esp + 20h], edi
        mov    ebx, dword ptr [edi + 20h]
        db  0A1h, 078h, 037h, 001h, 000h
        inc    eax
        db  0A3h, 078h, 037h, 001h, 000h
        mov    ecx, dword ptr [edi + 10h]
        cmp    dword ptr [esi + 2ch], 2
        jne    SHORT loc_1_2369
        mov    eax, dword ptr [esi + 84h]
        add    ecx, 0ch
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [esp + 18h], eax
        lea    eax, [esp + 14h]
        push   ecx
        push   eax
        lea    eax, [edi + 5ah]
        push   ebx
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        db  0A1h, 058h, 02Ah, 001h, 000h
        call   dword ptr [eax + 30h]
        add    esp, 14h
        jmp    SHORT loc_1_238E
loc_1_2369:
        mov    eax, dword ptr [esi + 88h]
        add    ecx, 0ch
        mov    eax, dword ptr [eax + 4ch]
        mov    dword ptr [esp + 18h], eax
        push   ecx
        push   ebx
        lea    eax, [esp + 20h]
        push   dword ptr [edi + 1ch]
        push   eax
        db  0A1h, 058h, 02Ah, 001h, 000h
        call   dword ptr [eax + 2ch]
        add    esp, 10h
loc_1_238E:
        mov    ebp, eax
        mov    eax, dword ptr [esp + 30h]
        cmp    ebp, 0ffh
        mov    dword ptr [eax], ebp
        db  0A1h, 078h, 037h, 001h, 000h
        je     SHORT loc_1_23A4
        dec    eax
loc_1_23A4:
        db  0A3h, 078h, 037h, 001h, 000h
        or     ebp, ebp
        je     SHORT loc_1_23CA
        cmp    ebp, 1
        je     SHORT loc_1_23D5
        cmp    ebp, 0ffh
        je     SHORT loc_1_240C
        mov    ebx, 0ffffffffh
        push   esi
        call   sub_1_29C4
        add    esp, 4
        jmp    SHORT loc_1_240C
loc_1_23CA:
        push   esi
        call   sub_1_29C4
        add    esp, 4
        jmp    SHORT loc_1_240C
loc_1_23D5:
        db  0A3h, 078h, 037h, 001h, 000h
        or     eax, eax
        jne    SHORT loc_1_23EE
        mov    ebx, 0ffffffffh
        push   esi
        call   sub_1_29C4
        add    esp, 4
        jmp    SHORT loc_1_240C
loc_1_23EE:
        mov    eax, dword ptr [esp + 38h]
        cmp    dword ptr [esp + 34h], 0
        mov    dword ptr [eax], 1
        je     SHORT loc_1_240C
        push   edi
        push   dword ptr [esp + 14h]
        call   sub_1_17FA
        add    esp, 8
loc_1_240C:
        or     ebp, ebp
        je     SHORT loc_1_2415
        cmp    ebx, -1
        jne    SHORT loc_1_2432
loc_1_2415:
        mov    eax, dword ptr [esi + 8ch]
        mov    ecx, dword ptr [edi + 20h]
        sub    dword ptr [eax + 0ch], ecx
        dec    dword ptr [eax + 8]
        push   edi
        call   sub_1_1301F
        add    esp, 4
        call   sub_1_2117
loc_1_2432:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
loc_1_243C:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esi + 18h]
        db  0FFh, 00Dh, 078h, 037h, 001h, 000h
        call   sub_1_2479
        mov    eax, dword ptr [edi + 8ch]
        mov    ecx, dword ptr [esi + 20h]
        sub    dword ptr [eax + 0ch], ecx
        dec    dword ptr [eax + 8]
        push   esi
        call   sub_1_1301F
        add    esp, 4
        call   sub_1_2117
        push   edi
        call   sub_1_29C4
        add    esp, 4
        pop    edi
        pop    esi
        ret
sub_1_2479:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  0BFh, 04Ch, 036h, 001h, 000h
        sub    ebx, ebx
loc_1_2487:
        db  039h, 03Dh, 04Ch, 036h, 001h, 000h
        je     NEAR PTR loc_1_2515
        db  08Bh, 035h, 04Ch, 036h, 001h, 000h
        mov    eax, esi
        lea    ecx, [esi + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [eax + 4], edi
        db  0A3h, 04Ch, 036h, 001h, 000h
        mov    eax, dword ptr [ecx]
        mov    eax, dword ptr [eax]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        lea    eax, [esp + 10h]
        mov    ebp, dword ptr [esi + 18h]
        push   eax
        lea    eax, [esp + 18h]
        push   ebx
        push   eax
        push   esi
        call   sub_1_22EE
        add    esp, 10h
        cmp    eax, -1
        je     SHORT loc_1_250C
        cmp    dword ptr [esp + 10h], ebx
        je     SHORT loc_1_2487
        mov    eax, dword ptr [esi + 18h]
        mov    edx, dword ptr [eax + 8ch]
        db  0A1h, 04Ch, 036h, 001h, 000h
        mov    dword ptr [esi], eax
        lea    ecx, [esi + 8]
        db  0C7h, 046h, 004h, 04Ch, 036h, 001h, 000h
        db  0A1h, 04Ch, 036h, 001h, 000h
        add    edx, 24h
        mov    dword ptr [eax + 4], esi
        db  089h, 035h, 04Ch, 036h, 001h, 000h
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        mov    dword ptr [esi + 0ch], edx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_1_2515
loc_1_250C:
        push   ebp
        call   sub_1_2B23
        add    esp, 4
loc_1_2515:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
        db  000h, 000h, 000h
loc_1_2520:
        push   esi
        push   edi
        sub    esi, esi
        mov    eax, dword ptr [esp + 10h]
        cmp    eax, esi
        je     SHORT loc_1_253D
        cmp    eax, 1
        je     SHORT loc_1_2579
        cmp    eax, 2
        je     SHORT loc_1_253D
        mov    esi, 2726h
        jmp    SHORT loc_1_2579
loc_1_253D:
        mov    edi, dword ptr [esp + 0ch]
        cmp    dword ptr [edi + 2ch], 1
        jne    SHORT loc_1_2579
        mov    eax, dword ptr [edi + 8ch]
        or     eax, eax
        je     SHORT loc_1_2579
        mov    ecx, dword ptr [eax + 2ch]
        sub    ecx, eax
        cmp    ecx, 2ch
        je     SHORT loc_1_2579
        push   dword ptr [edi + 88h]
        call   sub_1_089A
        add    esp, 4
        push   2745h
        push   20h
        push   edi
        call   sub_1_F3AC
        add    esp, 0ch
loc_1_2579:
        mov    eax, esi
        pop    edi
        pop    esi
        ret
        db  000h, 000h
loc_1_2580:
        call   sub_1_2880
        or     eax, eax
        jne    SHORT loc_1_258B
        sub    eax, eax
loc_1_258B:
        ret
loc_1_258C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 84h]
        or     eax, eax
        je     SHORT loc_1_25A7
        cmp    dword ptr [eax + 18h], esi
        jne    SHORT loc_1_25A7
        mov    dword ptr [eax + 18h], 0
loc_1_25A7:
        cmp    word ptr [esi + 10h], 0
        je     SHORT loc_1_25B5
        cmp    word ptr [esi + 12h], 0
        je     SHORT loc_1_25C5
loc_1_25B5:
        mov    eax, dword ptr [esi + 8ch]
        mov    ecx, dword ptr [eax + 2ch]
        sub    ecx, eax
        cmp    ecx, 2ch
        je     SHORT loc_1_25E1
loc_1_25C5:
        push   esi
        call   sub_1_2B23
        add    esp, 4
        mov    eax, dword ptr [esi + 88h]
        or     eax, eax
        je     SHORT loc_1_25E1
        push   eax
        call   sub_1_089A
        add    esp, 4
loc_1_25E1:
        push   esi
        call   sub_1_2B3B
        add    esp, 4
        push   esi
        call   sub_1_29C4
        add    esp, 4
        sub    eax, eax
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2600:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 1ch]
        push   ebx
        call   sub_1_29BC
        add    esp, 4
        sub    esi, esi
        cmp    dword ptr [esp + 20h], 0ffffh
        je     SHORT loc_1_262A
        mov    esi, 2726h
        jmp    NEAR PTR loc_1_2757
loc_1_262A:
        mov    eax, dword ptr [esp + 24h]
        cmp    eax, 2
        je     SHORT loc_1_264F
        cmp    eax, 7001h
        je     SHORT loc_1_2693
        cmp    eax, 7002h
        je     NEAR PTR loc_1_2724
        mov    esi, 273ah
        jmp    NEAR PTR loc_1_2757
loc_1_264F:
        cmp    dword ptr [ebx + 2ch], 2
        jne    SHORT loc_1_265F
        mov    esi, 273ah
        jmp    NEAR PTR loc_1_2757
loc_1_265F:
        mov    eax, dword ptr [esp + 2ch]
        mov    ebp, dword ptr [esp + 28h]
        mov    edx, dword ptr [eax]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [ebx + 80h]
        and    eax, 8
        cmp    eax, 1
        sbb    al, al
        inc    al
        mov    byte ptr [ebp], al
        jmp    NEAR PTR loc_1_2757
loc_1_2693:
        mov    eax, dword ptr [esp + 2ch]
        mov    ebp, dword ptr [esp + 28h]
        mov    edx, dword ptr [eax]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [ebx + 14h]
        mov    eax, dword ptr [eax + 4]
        cmp    eax, -1
        jne    SHORT loc_1_26CE
        db  083h, 03Dh, 0ECh, 036h, 001h, 000h, 000h
        je     NEAR PTR loc_1_2757
        db  0A1h, 0F0h, 036h, 001h, 000h
        jmp    SHORT loc_1_26D7
loc_1_26CE:
        push   eax
        call   sub_1_EE10
        add    esp, 4
loc_1_26D7:
        or     eax, eax
        je     SHORT loc_1_2757
        mov    ecx, dword ptr [eax + 14h]
        or     ecx, ecx
        je     SHORT loc_1_2757
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        push   ecx
        call   sub_1_10879
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_2700
        mov    eax, 1
        jmp    SHORT loc_1_271F
loc_1_2700:
        cmp    dword ptr [esp + 10h], 2
        jne    SHORT loc_1_270E
        mov    eax, 6
        jmp    SHORT loc_1_271F
loc_1_270E:
        cmp    dword ptr [esp + 10h], 1
        mov    eax, 0ah
        je     SHORT loc_1_271F
        mov    eax, 2
loc_1_271F:
        mov    byte ptr [ebp], al
        jmp    SHORT loc_1_2757
loc_1_2724:
        mov    eax, dword ptr [esp + 2ch]
        mov    edx, dword ptr [eax]
        cmp    edx, 4
        jne    SHORT loc_1_2741
        mov    eax, dword ptr [ebx + 8ch]
        mov    ebp, dword ptr [esp + 28h]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp], eax
        jmp    SHORT loc_1_2757
loc_1_2741:
        mov    ebp, dword ptr [esp + 28h]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
loc_1_2757:
        push   ebx
        call   sub_1_29C4
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_276A:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    esi, dword ptr [esp + 14h]
        push   esi
        call   sub_1_29BC
        mov    ebp, dword ptr [esp + 28h]
        add    esp, 4
        mov    ebx, dword ptr [esp + 20h]
        push   ebp
        push   ebx
        call   sub_1_285F
        add    esp, 8
        cmp    eax, 1
        sbb    ecx, ecx
        neg    ecx
        cmp    dword ptr [esp + 18h], 0ffffh
        je     SHORT loc_1_27AA
        mov    edi, 2726h
        jmp    NEAR PTR loc_1_284F
loc_1_27AA:
        mov    eax, dword ptr [esp + 1ch]
        cmp    eax, 8
        je     SHORT loc_1_27D1
        cmp    eax, 100h
        je     SHORT loc_1_27F1
        cmp    eax, 1001h
        je     SHORT loc_1_2811
        cmp    eax, 1002h
        je     SHORT loc_1_2822
        cmp    eax, 8000h
        je     SHORT loc_1_2833
        jmp    SHORT loc_1_284F
loc_1_27D1:
        cmp    dword ptr [esi + 2ch], 2
        jne    SHORT loc_1_27D9
        jmp    SHORT loc_1_284A
loc_1_27D9:
        mov    eax, dword ptr [esi + 88h]
        or     eax, eax
        je     SHORT loc_1_284F
        push   ecx
        push   2
        push   eax
        call   sub_1_07CD
        add    esp, 0ch
        jmp    SHORT loc_1_284F
loc_1_27F1:
        cmp    dword ptr [esi + 2ch], 2
        jne    SHORT loc_1_27F9
        jmp    SHORT loc_1_284A
loc_1_27F9:
        mov    eax, dword ptr [esi + 88h]
        or     eax, eax
        je     SHORT loc_1_284F
        push   ecx
        push   3
        push   eax
        call   sub_1_07CD
        add    esp, 0ch
        jmp    SHORT loc_1_284F
loc_1_2811:
        movzx  eax, word ptr [ebx]
        mov    ecx, dword ptr [esi + 8ch]
        mov    dword ptr [esi + 34h], eax
        mov    dword ptr [ecx + 4], eax
        jmp    SHORT loc_1_284F
loc_1_2822:
        movzx  eax, word ptr [ebx]
        mov    ecx, dword ptr [esi + 8ch]
        mov    dword ptr [esi + 38h], eax
        mov    dword ptr [ecx + 10h], eax
        jmp    SHORT loc_1_284F
loc_1_2833:
        cmp    ebp, 4
        jb     SHORT loc_1_284A
        cmp    word ptr [ebx], 4d2h
        jne    SHORT loc_1_284A
        or     dword ptr [esi + 80h], 1
        sub    edi, edi
        jmp    SHORT loc_1_284F
loc_1_284A:
        mov    edi, 273ah
loc_1_284F:
        push   esi
        call   sub_1_29C4
        add    esp, 4
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_285F:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
loc_1_2867:
        mov    eax, ecx
        dec    ecx
        or     eax, eax
        je     SHORT loc_1_287A
        mov    eax, edx
        inc    edx
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_2867
        sub    eax, eax
        jmp    SHORT loc_1_287F
loc_1_287A:
        mov    eax, 1
loc_1_287F:
        ret
sub_1_2880:
        push   esi
        push   edi
        call   sub_1_F3A4
        push   0c0h
        call   sub_1_F390
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        mov    eax, 0
        je     SHORT loc_1_28E2
        mov    edi, esi
        sub    eax, eax
        mov    ecx, 30h
        rep stosd
        mov    dword ptr [esi + 38h], 2710h
        mov    dword ptr [esi + 34h], 2710h
        db  0C7h, 046h, 074h, 034h, 038h, 001h, 000h
        lea    ecx, [esi + 74h]
        db  0A1h, 038h, 038h, 001h, 000h
        mov    dword ptr [esi + 78h], eax
        db  0A1h, 038h, 038h, 001h, 000h
        push   esi
        mov    dword ptr [eax], ecx
        db  089h, 00Dh, 038h, 038h, 001h, 000h
        call   sub_1_29BC
        add    esp, 4
        mov    eax, esi
loc_1_28E2:
        pop    edi
        pop    esi
        ret
sub_1_28E5:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 80h]
        test   al, 2
        je     SHORT loc_1_2911
        and    eax, 0fffffffdh
        mov    dword ptr [esi + 80h], eax
        push   dword ptr [esi + 9ch]
        push   dword ptr [esi + 98h]
        call   sub_1_F2B5
        add    esp, 8
loc_1_2911:
        mov    dword ptr [esi + 8ch], 0
        mov    eax, dword ptr [esi + 88h]
        or     eax, eax
        je     SHORT loc_1_2938
        push   eax
        call   sub_1_06D1
        add    esp, 4
        mov    dword ptr [esi + 88h], 0
loc_1_2938:
        mov    eax, dword ptr [esi + 84h]
        or     eax, eax
        je     SHORT loc_1_2955
        push   eax
        call   sub_1_0B13
        add    esp, 4
        mov    dword ptr [esi + 84h], 0
loc_1_2955:
        push   esi
        call   sub_1_2B53
        add    esp, 4
        pop    esi
        ret
sub_1_2960:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 14h]
        mov    edi, edx
        mov    ebx, dword ptr [esp + 10h]
        mov    esi, ebx
        mov    ecx, 1dh
        rep movsd
        mov    dword ptr [edx + 50h], 0
        mov    dword ptr [edx + 4ch], 6
        mov    dword ptr [edx + 54h], 10h
        lea    eax, [edx + 8]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [eax], eax
        mov    eax, dword ptr [ebx + 80h]
        mov    dword ptr [edx + 80h], eax
        mov    eax, dword ptr [ebx + 84h]
        or     eax, eax
        mov    dword ptr [edx + 84h], eax
        je     SHORT loc_1_29B8
        push   eax
        call   sub_1_0B0B
        add    esp, 4
loc_1_29B8:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_29BC:
        mov    eax, dword ptr [esp + 4]
        inc    dword ptr [eax + 7ch]
        ret
sub_1_29C4:
        mov    ecx, dword ptr [esp + 4]
        sub    eax, eax
        mov    edx, dword ptr [ecx + 7ch]
        dec    edx
        mov    dword ptr [ecx + 7ch], edx
        jne    SHORT loc_1_29E1
        push   ecx
        call   sub_1_28E5
        add    esp, 4
        mov    eax, 1
loc_1_29E1:
        ret
sub_1_29E2:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 30h]
        mov    ebx, dword ptr [esp + 2ch]
        mov    eax, dword ptr [ebp + 52h]
        mov    dword ptr [esp + 10h], eax
        push   dword ptr [ebx + 84h]
        push   dword ptr [esp + 14h]
        call   sub_1_06EF
        add    esp, 8
        or     eax, eax
        jne    NEAR PTR loc_1_2B1B
        mov    eax, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 34h]
        mov    dword ptr [eax + 10h], ebx
        mov    dword ptr [ebp + 30h], 1
        add    esi, 8
        mov    word ptr [ebp + 34h], 1ah
        mov    word ptr [ebp + 36h], 15h
        mov    eax, dword ptr [esi - 4]
        mov    dword ptr [ebp + 48h], eax
        lea    edi, [ebp + 38h]
        movzx  eax, word ptr [esi - 6]
        mov    dword ptr [ebp + 4ch], eax
        movsd
        movsd
        movsd
        movsd
        push   ebx
        call   sub_1_29BC
        mov    eax, dword ptr [esp + 14h]
        add    esp, 4
        mov    eax, dword ptr [eax + 58h]
        mov    dword ptr [ebx + 8ch], eax
        push   ebx
        push   eax
        call   sub_1_17E3
        mov    eax, dword ptr [esp + 18h]
        add    esp, 8
        mov    dword ptr [ebx + 88h], eax
        mov    dword ptr [ebx + 4ch], 5
        and    dword ptr [ebx + 50h], 0fffffffdh
        mov    esi, dword ptr [eax + 18h]
        mov    dword ptr [eax + 18h], 60000004h
        push   eax
        call   sub_1_06C9
        mov    dword ptr [esp + 18h], 0
        mov    eax, dword ptr [esp + 14h]
        add    esp, 4
        mov    eax, dword ptr [eax + 4ch]
        mov    dword ptr [esp + 18h], eax
        db  0C7h, 044h, 024h, 01Ch, 08Fh, 02Bh, 000h, 000h
        mov    dword ptr [esp + 20h], ebp
        lea    eax, [ebp + 18h]
        push   eax
        lea    eax, [esp + 18h]
        push   ebp
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        db  0A1h, 058h, 02Ah, 001h, 000h
        call   dword ptr [eax + 18h]
        add    esp, 10h
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_1_2B0C
        cmp    edi, 0ffh
        je     SHORT loc_1_2B19
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [ebx + 4ch], 2
        mov    dword ptr [eax + 18h], esi
        mov    dword ptr [ebx + 88h], 0
        mov    dword ptr [ebx + 8ch], 0
        push   eax
        call   sub_1_06D1
        add    esp, 4
        push   ebx
        call   sub_1_29C4
        add    esp, 4
        jmp    SHORT loc_1_2B19
loc_1_2B0C:
        push   0
        push   0
        push   ebp
        call   sub_1_2B8F
        add    esp, 0ch
loc_1_2B19:
        mov    eax, edi
loc_1_2B1B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret
sub_1_2B23:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 8ch]
        or     eax, eax
        je     SHORT loc_1_2B3A
        push   eax
        call   sub_1_18C9
        add    esp, 4
loc_1_2B3A:
        ret
sub_1_2B3B:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 8ch]
        or     eax, eax
        je     SHORT loc_1_2B52
        push   eax
        call   sub_1_190E
        add    esp, 4
loc_1_2B52:
        ret
sub_1_2B53:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [edx + 78h]
        lea    eax, [edx + 74h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    eax, dword ptr [edx + 80h]
        test   al, 2
        jne    SHORT loc_1_2B72
        test   al, 4
        je     SHORT loc_1_2B85
loc_1_2B72:
        mov    ecx, dword ptr [edx + 94h]
        lea    eax, [edx + 90h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
loc_1_2B85:
        push   edx
        call   sub_1_F396
        add    esp, 4
        ret
sub_1_2B8F:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        cmp    dword ptr [esp + 18h], 0
        mov    ebx, dword ptr [eax + 52h]
        mov    ebp, dword ptr [ebx + 10h]
        jne    SHORT loc_1_2C18
        cmp    dword ptr [ebx + 18h], 60000004h
        jne    SHORT loc_1_2C18
        mov    dword ptr [ebp + 4ch], 6
        mov    esi, eax
        add    esi, 38h
        lea    edi, [ebx + 3ch]
        movsd
        movsd
        movsd
        movsd
        mov    eax, dword ptr [esp + 14h]
        mov    eax, dword ptr [eax + 48h]
        mov    dword ptr [ebx + 38h], eax
        lea    eax, [ebx + 1ch]
        mov    dword ptr [ebp + 14h], eax
        mov    dword ptr [ebp + 20h], 18h
        lea    eax, [ebx + 34h]
        mov    dword ptr [ebp + 18h], eax
        mov    dword ptr [ebp + 24h], 18h
        push   ebx
        call   sub_1_074D
        add    esp, 4
        test   byte ptr [ebp + 40h], 8
        je     SHORT loc_1_2C00
        push   1
        push   2
        push   ebx
        call   sub_1_07CD
        add    esp, 0ch
loc_1_2C00:
        test   byte ptr [ebp + 80h], 8
        je     SHORT loc_1_2C3C
        push   1
        push   1
        push   ebx
        call   sub_1_07CD
        add    esp, 0ch
        jmp    SHORT loc_1_2C3C
loc_1_2C18:
        mov    dword ptr [ebp + 4ch], 2
        mov    dword ptr [ebp + 88h], 0
        mov    dword ptr [ebp + 8ch], 0
        push   ebx
        call   sub_1_06D1
        add    esp, 4
loc_1_2C3C:
        push   dword ptr [esp + 14h]
        call   sub_1_2E44
        add    esp, 4
        push   ebp
        call   sub_1_29C4
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_2C7B
        push   dword ptr [esp + 18h]
        call   sub_1_2C80
        add    esp, 4
        push   eax
        push   10h
        push   ebp
        call   sub_1_F3AC
        add    esp, 0ch
        cmp    dword ptr [esp + 18h], 0
        jne    SHORT loc_1_2C7B
        mov    dword ptr [ebp + 54h], 8
loc_1_2C7B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_2C80:
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 0feh
        jg     SHORT loc_1_2C99
        je     SHORT loc_1_2CB9
        cmp    eax, 22h
        ja     SHORT loc_1_2C99
        db  0FFh, 024h, 085h, 00Ch, 02Dh, 000h, 000h
loc_1_2C99:
        mov    eax, 2726h
        jmp    SHORT loc_1_2D0B
loc_1_2CA0:
        sub    eax, eax
        jmp    SHORT loc_1_2D0B
loc_1_2CA4:
        mov    eax, 2747h
        jmp    SHORT loc_1_2D0B
loc_1_2CAB:
        mov    eax, 2740h
        jmp    SHORT loc_1_2D0B
loc_1_2CB2:
        mov    eax, 2741h
        jmp    SHORT loc_1_2D0B
loc_1_2CB9:
        mov    eax, 2726h
        jmp    SHORT loc_1_2D0B
loc_1_2CC0:
        mov    eax, 2738h
        jmp    SHORT loc_1_2D0B
loc_1_2CC7:
        mov    eax, 274dh
        jmp    SHORT loc_1_2D0B
loc_1_2CCE:
        mov    eax, 2745h
        jmp    SHORT loc_1_2D0B
loc_1_2CD5:
        mov    eax, 2746h
        jmp    SHORT loc_1_2D0B
loc_1_2CDC:
        mov    eax, 274ch
        jmp    SHORT loc_1_2D0B
loc_1_2CE3:
        mov    eax, 2775h
        jmp    SHORT loc_1_2D0B
loc_1_2CEA:
        mov    eax, 2743h
        jmp    SHORT loc_1_2D0B
loc_1_2CF1:
        mov    eax, 2751h
        jmp    SHORT loc_1_2D0B
loc_1_2CF8:
        mov    eax, 2757h
        jmp    SHORT loc_1_2D0B
loc_1_2CFF:
        mov    eax, 2714h
        jmp    SHORT loc_1_2D0B
loc_1_2D06:
        mov    eax, 271eh
loc_1_2D0B:
        ret
        db  0A0h, 02Ch, 000h, 000h, 0A4h, 02Ch, 000h, 000h, 0ABh, 02Ch, 000h, 000h, 0B2h, 02Ch, 000h, 000h
        db  0A4h, 02Ch, 000h, 000h, 099h, 02Ch, 000h, 000h, 0B2h, 02Ch, 000h, 000h, 0B9h, 02Ch, 000h, 000h
        db  099h, 02Ch, 000h, 000h, 0C0h, 02Ch, 000h, 000h, 0B9h, 02Ch, 000h, 000h, 0B9h, 02Ch, 000h, 000h
        db  099h, 02Ch, 000h, 000h, 099h, 02Ch, 000h, 000h, 0C7h, 02Ch, 000h, 000h, 0B9h, 02Ch, 000h, 000h
        db  0B9h, 02Ch, 000h, 000h, 0B9h, 02Ch, 000h, 000h, 0B9h, 02Ch, 000h, 000h, 0CEh, 02Ch, 000h, 000h
        db  0D5h, 02Ch, 000h, 000h, 0DCh, 02Ch, 000h, 000h, 0E3h, 02Ch, 000h, 000h, 099h, 02Ch, 000h, 000h
        db  099h, 02Ch, 000h, 000h, 0B9h, 02Ch, 000h, 000h, 0B9h, 02Ch, 000h, 000h, 0EAh, 02Ch, 000h, 000h
        db  0F1h, 02Ch, 000h, 000h, 0EAh, 02Ch, 000h, 000h, 0F8h, 02Ch, 000h, 000h, 0B9h, 02Ch, 000h, 000h
        db  0FFh, 02Ch, 000h, 000h, 006h, 02Dh, 000h, 000h, 0FFh, 02Ch, 000h, 000h
sub_1_2D98:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 4]
        test   cl, 1
        je     SHORT loc_1_2DA8
        mov    eax, 20h
loc_1_2DA8:
        test   ch, 80h
        je     SHORT loc_1_2DB0
        or     eax, 40h
loc_1_2DB0:
        test   cl, 4
        je     SHORT loc_1_2DBA
        or     eax, 200h
loc_1_2DBA:
        ret
sub_1_2DBB:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 4]
        test   cl, 40h
        je     SHORT loc_1_2DCB
        mov    eax, 1
loc_1_2DCB:
        test   cl, 10h
        je     SHORT loc_1_2DD5
        or     eax, 8000h
loc_1_2DD5:
        ret
        db  0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2DE0:
        push   esi
        push   edi
        push   1
        call   sub_1_05D0
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_2DF6
        sub    eax, eax
        jmp    SHORT loc_1_2E41
loc_1_2DF6:
        call   sub_1_F3A4
        push   56h
        call   sub_1_F390
        add    esp, 4
        mov    edx, eax
        or     edx, edx
        jne    SHORT loc_1_2E18
        push   esi
        call   sub_1_0655
        add    esp, 4
        sub    eax, eax
        jmp    SHORT loc_1_2E41
loc_1_2E18:
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 15h
        rep stosd
        stosw
        mov    dword ptr [edx + 10h], 22h
        lea    eax, [edx + 30h]
        mov    dword ptr [edx + 14h], eax
        mov    dword ptr [edx + 28h], 22h
        mov    dword ptr [edx + 2ch], eax
        mov    dword ptr [edx + 52h], esi
        mov    eax, edx
loc_1_2E41:
        pop    edi
        pop    esi
        ret
sub_1_2E44:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 52h]
        or     eax, eax
        je     SHORT loc_1_2E60
        push   eax
        call   sub_1_06D1
        add    esp, 4
        mov    dword ptr [esi + 52h], 0
loc_1_2E60:
        push   esi
        call   sub_1_2E6B
        add    esp, 4
        pop    esi
        ret
sub_1_2E6B:
        push   dword ptr [esp + 4]
        call   sub_1_F396
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2E80:
        sub    esp, 10h
        push   ebx
        mov    dword ptr [esp + 10h], 0
        mov    dword ptr [esp + 0ch], 0
        mov    dword ptr [esp + 8], 0
        lea    eax, [esp + 4]
        mov    dword ptr [esp + 4], 0
        mov    bl, byte ptr [esp + 20h]
        push   eax
        lea    eax, [esp + 0ch]
        push   eax
        lea    eax, [esp + 14h]
        push   ebx
        push   eax
        lea    eax, [esp + 20h]
        push   dword ptr [esp + 2ch]
        push   eax
        push   dword ptr [esp + 30h]
        push   3
        db  068h, 098h, 02Ah, 001h, 000h
        call   sub_1_3089
        add    esp, 24h
        or     eax, eax
        jne    SHORT loc_1_2F34
        lea    eax, [esp + 4]
        push   eax
        lea    eax, [esp + 0ch]
        push   eax
        lea    eax, [esp + 14h]
        push   ebx
        push   eax
        lea    eax, [esp + 20h]
        push   dword ptr [esp + 2ch]
        push   eax
        push   dword ptr [esp + 30h]
        push   3
        db  068h, 0A4h, 02Ah, 001h, 000h
        call   sub_1_3089
        add    esp, 24h
        or     eax, eax
        jne    SHORT loc_1_2F77
        lea    eax, [esp + 4]
        push   eax
        lea    eax, [esp + 0ch]
        push   eax
        lea    eax, [esp + 14h]
        push   ebx
        push   eax
        lea    eax, [esp + 20h]
        push   dword ptr [esp + 2ch]
        push   eax
        push   dword ptr [esp + 30h]
        push   3
        db  068h, 0B0h, 02Ah, 001h, 000h
        call   sub_1_3089
        add    esp, 24h
loc_1_2F34:
        or     eax, eax
        jne    SHORT loc_1_2F77
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_2F46
        mov    eax, 273fh
        jmp    SHORT loc_1_2F79
loc_1_2F46:
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_1_2F54
        mov    eax, 273ch
        jmp    SHORT loc_1_2F79
loc_1_2F54:
        cmp    dword ptr [esp + 8], 0
        jne    SHORT loc_1_2F62
        mov    eax, 273bh
        jmp    SHORT loc_1_2F79
loc_1_2F62:
        cmp    dword ptr [esp + 4], 0
        je     SHORT loc_1_2F70
        mov    eax, 2739h
        jmp    SHORT loc_1_2F79
loc_1_2F70:
        mov    eax, 2726h
        jmp    SHORT loc_1_2F79
loc_1_2F77:
        sub    eax, eax
loc_1_2F79:
        pop    ebx
        add    esp, 10h
        ret
sub_1_2F7E:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 10h]
        push   eax
        sub    esi, esi
        lea    eax, [esp + 18h]
        mov    edi, dword ptr [esp + 30h]
        push   eax
        lea    eax, [esp + 20h]
        push   dword ptr [edi]
        mov    ebx, dword ptr [esp + 34h]
        push   eax
        lea    eax, [esp + 2ch]
        push   dword ptr [ebx]
        mov    ebp, dword ptr [esp + 38h]
        push   eax
        push   dword ptr [ebp]
        push   3
        db  068h, 098h, 02Ah, 001h, 000h
        call   sub_1_3089
        add    esp, 24h
        cmp    eax, esi
        je     SHORT loc_1_2FC8
        db  0B9h, 098h, 02Ah, 001h, 000h
        jmp    SHORT loc_1_3030
loc_1_2FC8:
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 20h]
        push   dword ptr [edi]
        push   eax
        lea    eax, [esp + 2ch]
        push   dword ptr [ebx]
        push   eax
        push   dword ptr [ebp]
        push   3
        db  068h, 0A4h, 02Ah, 001h, 000h
        call   sub_1_3089
        add    esp, 24h
        or     eax, eax
        je     SHORT loc_1_2FFD
        db  0B9h, 0A4h, 02Ah, 001h, 000h
        jmp    SHORT loc_1_3030
loc_1_2FFD:
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 20h]
        push   dword ptr [edi]
        push   eax
        lea    eax, [esp + 2ch]
        push   dword ptr [ebx]
        push   eax
        push   dword ptr [ebp]
        push   3
        db  068h, 0B0h, 02Ah, 001h, 000h
        call   sub_1_3089
        add    esp, 24h
        or     eax, eax
        je     SHORT loc_1_3044
        db  0B9h, 0B0h, 02Ah, 001h, 000h
loc_1_3030:
        movzx  eax, byte ptr [ecx]
        mov    dword ptr [ebp], eax
        movzx  eax, byte ptr [ecx + 1]
        mov    dword ptr [ebx], eax
        movzx  eax, byte ptr [ecx + 2]
        mov    dword ptr [edi], eax
        jmp    SHORT loc_1_307F
loc_1_3044:
        cmp    dword ptr [esp + 1ch], 0
        jne    SHORT loc_1_3052
        mov    esi, 273fh
        jmp    SHORT loc_1_307F
loc_1_3052:
        cmp    dword ptr [esp + 18h], 0
        jne    SHORT loc_1_3060
        mov    esi, 273ch
        jmp    SHORT loc_1_307F
loc_1_3060:
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_1_306E
        mov    esi, 273bh
        jmp    SHORT loc_1_307F
loc_1_306E:
        cmp    dword ptr [esp + 10h], 0
        mov    esi, 2739h
        jne    SHORT loc_1_307F
        mov    esi, 2726h
loc_1_307F:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
sub_1_3089:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        mov    eax, esi
        dec    esi
        or     eax, eax
        je     SHORT loc_1_3106
        mov    edi, dword ptr [esp + 14h]
        mov    cl, byte ptr [esp + 1ch]
        mov    dl, byte ptr [esp + 24h]
        mov    bl, byte ptr [esp + 2ch]
loc_1_30A8:
        cmp    cl, byte ptr [edi]
        jne    SHORT loc_1_30B6
        mov    eax, dword ptr [esp + 20h]
        mov    dword ptr [eax], 1
loc_1_30B6:
        cmp    dl, byte ptr [edi + 1]
        jne    SHORT loc_1_30C5
        mov    eax, dword ptr [esp + 28h]
        mov    dword ptr [eax], 1
loc_1_30C5:
        mov    al, byte ptr [edi + 2]
        cmp    bl, al
        jne    SHORT loc_1_30D7
        mov    ebp, dword ptr [esp + 30h]
        mov    dword ptr [ebp], 1
loc_1_30D7:
        cmp    cl, byte ptr [edi]
        jne    SHORT loc_1_30FC
        cmp    dl, byte ptr [edi + 1]
        jne    SHORT loc_1_30EF
        cmp    bl, al
        je     SHORT loc_1_30EF
        mov    ebp, dword ptr [esp + 34h]
        mov    dword ptr [ebp], 1
loc_1_30EF:
        cmp    cl, byte ptr [edi]
        jne    SHORT loc_1_30FC
        cmp    dl, byte ptr [edi + 1]
        jne    SHORT loc_1_30FC
        cmp    bl, al
        je     SHORT loc_1_310D
loc_1_30FC:
        add    edi, 3
        mov    eax, esi
        dec    esi
        or     eax, eax
        jne    SHORT loc_1_30A8
loc_1_3106:
        sub    eax, eax
loc_1_3108:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_310D:
        mov    eax, 1
        jmp    SHORT loc_1_3108
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3120:
        db  0A1h, 064h, 036h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_3133
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 064h, 036h, 001h, 000h
        jmp    SHORT loc_1_313D
loc_1_3133:
        push   24h
        call   sub_1_F390
        add    esp, 4
loc_1_313D:
        ret
sub_1_313E:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 00Dh, 064h, 036h, 001h, 000h
        mov    dword ptr [eax], ecx
        db  0A3h, 064h, 036h, 001h, 000h
        ret
sub_1_3150:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, edx
        and    ecx, 0ffffh
        db  039h, 00Dh, 070h, 036h, 001h, 000h
        jbe    SHORT loc_1_317B
        db  0A1h, 07Ch, 037h, 001h, 000h
        mov    eax, dword ptr [eax + ecx*4]
        or     eax, eax
        je     SHORT loc_1_317B
        movzx  ecx, word ptr [eax + 48h]
        shr    edx, 10h
        cmp    ecx, edx
        je     SHORT loc_1_317D
loc_1_317B:
        sub    eax, eax
loc_1_317D:
        ret
sub_1_317E:
        push   ebx
        push   esi
        push   edi
loc_1_3181:
        db  08Bh, 01Dh, 070h, 036h, 001h, 000h
        sub    eax, eax
        db  08Bh, 035h, 07Ch, 037h, 001h, 000h
        db  08Bh, 00Dh, 058h, 038h, 001h, 000h
        cmp    eax, ebx
        je     SHORT loc_1_31AD
loc_1_3199:
        cmp    ecx, ebx
        jne    SHORT loc_1_319F
        sub    ecx, ecx
loc_1_319F:
        cmp    dword ptr [esi + ecx*4], 0
        je     SHORT loc_1_31AB
        inc    ecx
        inc    eax
        cmp    eax, ebx
        jb     SHORT loc_1_3199
loc_1_31AB:
        cmp    eax, ebx
loc_1_31AD:
        jb     SHORT loc_1_3210
        cmp    ebx, 100h
        je     NEAR PTR loc_1_3246
        add    ebx, 10h
        cmp    ebx, 100h
        jb     SHORT loc_1_31CB
        mov    ebx, 100h
loc_1_31CB:
        lea    eax, [ebx*4]
        push   eax
        call   sub_1_F390
        add    esp, 4
        or     eax, eax
        db  08Bh, 00Dh, 070h, 036h, 001h, 000h
        db  08Bh, 015h, 07Ch, 037h, 001h, 000h
        je     SHORT loc_1_3246
        shl    ecx, 2
        mov    edi, eax
        mov    esi, edx
        shr    ecx, 2
        rep movsd
        db  0A3h, 07Ch, 037h, 001h, 000h
        db  089h, 01Dh, 070h, 036h, 001h, 000h
        push   edx
        call   sub_1_F396
        add    esp, 4
        jmp    NEAR PTR loc_1_3181
loc_1_3210:
        mov    eax, dword ptr [esp + 10h]
        db  066h, 08Bh, 015h, 018h, 038h, 001h, 000h
        mov    word ptr [eax + 48h], dx
        mov    byte ptr [eax + 43h], cl
        mov    dword ptr [esi + ecx*4], eax
        lea    eax, [ecx + 1]
        db  0A3h, 058h, 038h, 001h, 000h
        movzx  eax, dx
        shl    eax, 10h
        and    ecx, 0ffffh
        or     eax, ecx
        inc    dx
        db  066h, 089h, 015h, 018h, 038h, 001h, 000h
        jmp    SHORT loc_1_324B
loc_1_3246:
        mov    eax, 0ffffffffh
loc_1_324B:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_324F:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        mov    edi, ecx
        mov    edx, dword ptr [ebx + 4]
        lea    esi, [edx + 28h]
        movsd
        movsd
        movsd
        movsd
        mov    eax, dword ptr [edx + 38h]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     SHORT loc_1_3282
        cmp    byte ptr [eax + 42h], 51h
        jne    SHORT loc_1_3282
        mov    dword ptr [ebx + 4], eax
        mov    eax, 1
        jmp    SHORT loc_1_32DF
loc_1_3282:
        mov    edx, dword ptr [ebx]
loc_1_3284:
        or     eax, eax
        je     SHORT loc_1_329C
        mov    ecx, 51h
loc_1_328D:
        cmp    byte ptr [eax + 42h], cl
        je     SHORT loc_1_3298
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_328D
loc_1_3298:
        or     eax, eax
        jne    SHORT loc_1_32C4
loc_1_329C:
        db  08Dh, 00Ch, 095h, 0D8h, 038h, 001h, 000h
loc_1_32A3:
        add    ecx, 4
        inc    edx
        db  081h, 0F9h, 018h, 039h, 001h, 000h
        jae    SHORT loc_1_32BB
        cmp    dword ptr [ecx], 0
        je     SHORT loc_1_32A3
        db  08Bh, 004h, 095h, 0D8h, 038h, 001h, 000h
loc_1_32BB:
        cmp    edx, 10h
        jne    SHORT loc_1_3284
        or     eax, eax
        je     SHORT loc_1_32D0
loc_1_32C4:
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx], edx
        mov    eax, 1
        jmp    SHORT loc_1_32DF
loc_1_32D0:
        mov    dword ptr [ebx], 0
        mov    dword ptr [ebx + 4], 0
        sub    eax, eax
loc_1_32DF:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_32E3:
        push   ebx
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        mov    esi, dword ptr [edx]
        or     esi, esi
        mov    eax, dword ptr [edx + 4]
        jne    SHORT loc_1_333D
        or     eax, eax
        jne    SHORT loc_1_333D
        mov    eax, dword ptr [esp + 10h]
        db  08Dh, 00Ch, 0B5h, 0D8h, 038h, 001h, 000h
        mov    dword ptr [eax], 1
loc_1_3307:
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     SHORT loc_1_3321
        mov    ebx, 51h
loc_1_3312:
        cmp    byte ptr [eax + 42h], bl
        je     SHORT loc_1_331D
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_3312
loc_1_331D:
        or     eax, eax
        jne    SHORT loc_1_3331
loc_1_3321:
        add    ecx, 4
        inc    esi
        db  081h, 0F9h, 018h, 039h, 001h, 000h
        jb     SHORT loc_1_3307
        or     eax, eax
        je     SHORT loc_1_3361
loc_1_3331:
        mov    dword ptr [edx], esi
        mov    dword ptr [edx + 4], eax
        mov    eax, 1
        jmp    SHORT loc_1_3363
loc_1_333D:
        cmp    esi, 10h
        jae    SHORT loc_1_3357
        db  08Bh, 00Ch, 0B5h, 0D8h, 038h, 001h, 000h
        or     ecx, ecx
        je     SHORT loc_1_3357
loc_1_334D:
        cmp    ecx, eax
        je     SHORT loc_1_3366
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_334D
loc_1_3357:
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], 0
loc_1_3361:
        sub    eax, eax
loc_1_3363:
        pop    esi
        pop    ebx
        ret
loc_1_3366:
        cmp    byte ptr [ecx + 42h], 51h
        jne    SHORT loc_1_3357
        mov    eax, dword ptr [esp + 10h]
        mov    dword ptr [eax], 1
        mov    eax, 1
        jmp    SHORT loc_1_3363
sub_1_337D:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        movzx  ecx, byte ptr [esi + 3]
        movzx  eax, byte ptr [esi + 2]
        add    ecx, eax
        movzx  eax, byte ptr [esi + 1]
        add    ecx, eax
        movzx  eax, byte ptr [esi]
        add    ecx, eax
        and    ecx, 0fh
        db  08Bh, 004h, 08Dh, 0D8h, 038h, 001h, 000h
        or     eax, eax
        je     NEAR PTR loc_1_3552
        mov    edi, dword ptr [esp + 18h]
        mov    dl, byte ptr [esp + 1ch]
        mov    ebx, dword ptr [esp + 20h]
        mov    ebp, 0ffffffffh
loc_1_33BD:
        cmp    eax, ebx
        jbe    NEAR PTR loc_1_3548
        mov    cl, byte ptr [esi]
        cmp    byte ptr [eax + 28h], cl
        jne    NEAR PTR loc_1_3456
        mov    cl, byte ptr [esi + 1]
        cmp    byte ptr [eax + 29h], cl
        jne    SHORT loc_1_3456
        mov    cl, byte ptr [esi + 2]
        cmp    byte ptr [eax + 2ah], cl
        jne    SHORT loc_1_3456
        mov    cl, byte ptr [esi + 3]
        cmp    byte ptr [eax + 2bh], cl
        jne    SHORT loc_1_3456
        mov    cl, byte ptr [eax + 2ch]
        cmp    byte ptr [esi + 4], cl
        jne    SHORT loc_1_3456
        mov    cl, byte ptr [eax + 2dh]
        cmp    byte ptr [esi + 5], cl
        jne    SHORT loc_1_3456
        mov    cl, byte ptr [eax + 2eh]
        cmp    byte ptr [esi + 6], cl
        jne    SHORT loc_1_3456
        mov    cl, byte ptr [eax + 2fh]
        cmp    byte ptr [esi + 7], cl
        jne    SHORT loc_1_3456
        mov    cl, byte ptr [eax + 30h]
        cmp    byte ptr [esi + 8], cl
        jne    SHORT loc_1_3456
        mov    cl, byte ptr [esi + 9]
        cmp    byte ptr [eax + 31h], cl
        jne    SHORT loc_1_3456
        mov    cl, byte ptr [esi + 0ah]
        cmp    byte ptr [eax + 32h], cl
        jne    SHORT loc_1_3456
        mov    cl, byte ptr [esi + 0bh]
        cmp    byte ptr [eax + 33h], cl
        jne    SHORT loc_1_3456
        mov    cl, byte ptr [esi + 0ch]
        cmp    byte ptr [eax + 34h], cl
        jne    SHORT loc_1_3456
        mov    cl, byte ptr [esi + 0dh]
        cmp    byte ptr [eax + 35h], cl
        jne    SHORT loc_1_3456
        mov    cl, byte ptr [esi + 0eh]
        cmp    byte ptr [eax + 36h], cl
        jne    SHORT loc_1_3456
        mov    cl, byte ptr [esi + 0fh]
        cmp    byte ptr [eax + 37h], cl
        jne    SHORT loc_1_3456
        cmp    dword ptr [eax + 38h], edi
        jne    SHORT loc_1_3456
        cmp    byte ptr [eax + 42h], dl
        je     NEAR PTR loc_1_3554
loc_1_3456:
        mov    cl, byte ptr [esi]
        cmp    byte ptr [eax + 28h], cl
        jne    NEAR PTR loc_1_34E3
        mov    cl, byte ptr [esi + 1]
        cmp    byte ptr [eax + 29h], cl
        jne    SHORT loc_1_34E3
        mov    cl, byte ptr [esi + 2]
        cmp    byte ptr [eax + 2ah], cl
        jne    SHORT loc_1_34E3
        mov    cl, byte ptr [esi + 3]
        cmp    byte ptr [eax + 2bh], cl
        jne    SHORT loc_1_34E3
        mov    cl, byte ptr [eax + 2ch]
        cmp    byte ptr [esi + 4], cl
        jne    SHORT loc_1_34E3
        mov    cl, byte ptr [eax + 2dh]
        cmp    byte ptr [esi + 5], cl
        jne    SHORT loc_1_34E3
        mov    cl, byte ptr [eax + 2eh]
        cmp    byte ptr [esi + 6], cl
        jne    SHORT loc_1_34E3
        mov    cl, byte ptr [eax + 2fh]
        cmp    byte ptr [esi + 7], cl
        jne    SHORT loc_1_34E3
        mov    cl, byte ptr [eax + 30h]
        cmp    byte ptr [esi + 8], cl
        jne    SHORT loc_1_34E3
        mov    cl, byte ptr [esi + 9]
        cmp    byte ptr [eax + 31h], cl
        jne    SHORT loc_1_34E3
        mov    cl, byte ptr [esi + 0ah]
        cmp    byte ptr [eax + 32h], cl
        jne    SHORT loc_1_34E3
        mov    cl, byte ptr [esi + 0bh]
        cmp    byte ptr [eax + 33h], cl
        jne    SHORT loc_1_34E3
        mov    cl, byte ptr [esi + 0ch]
        cmp    byte ptr [eax + 34h], cl
        jne    SHORT loc_1_34E3
        mov    cl, byte ptr [esi + 0dh]
        cmp    byte ptr [eax + 35h], cl
        jne    SHORT loc_1_34E3
        mov    cl, byte ptr [esi + 0eh]
        cmp    byte ptr [eax + 36h], cl
        jne    SHORT loc_1_34E3
        mov    cl, byte ptr [esi + 0fh]
        cmp    byte ptr [eax + 37h], cl
        jne    SHORT loc_1_34E3
        cmp    dword ptr [eax + 38h], ebp
        jne    SHORT loc_1_34E3
        cmp    byte ptr [eax + 42h], dl
        je     SHORT loc_1_3554
loc_1_34E3:
        mov    cl, byte ptr [esi]
        cmp    byte ptr [eax + 28h], cl
        jne    SHORT loc_1_3548
        mov    cl, byte ptr [esi + 1]
        cmp    byte ptr [eax + 29h], cl
        jne    SHORT loc_1_3548
        mov    cl, byte ptr [esi + 2]
        cmp    byte ptr [eax + 2ah], cl
        jne    SHORT loc_1_3548
        mov    cl, byte ptr [esi + 3]
        cmp    byte ptr [eax + 2bh], cl
        jne    SHORT loc_1_3548
        mov    cl, byte ptr [eax + 2ch]
        cmp    byte ptr [esi + 4], cl
        jne    SHORT loc_1_3548
        mov    cl, byte ptr [eax + 2dh]
        cmp    byte ptr [esi + 5], cl
        jne    SHORT loc_1_3548
        mov    cl, byte ptr [eax + 2eh]
        cmp    byte ptr [esi + 6], cl
        jne    SHORT loc_1_3548
        mov    cl, byte ptr [eax + 2fh]
        cmp    byte ptr [esi + 7], cl
        jne    SHORT loc_1_3548
        mov    cl, byte ptr [eax + 30h]
        cmp    byte ptr [esi + 8], cl
        jne    SHORT loc_1_3548
        mov    cl, byte ptr [esi + 9]
        cmp    byte ptr [eax + 31h], cl
        jne    SHORT loc_1_3548
        mov    cl, byte ptr [esi + 0ah]
        cmp    byte ptr [eax + 32h], cl
        jne    SHORT loc_1_3548
        mov    cl, byte ptr [esi + 0bh]
        cmp    byte ptr [eax + 33h], cl
        jne    SHORT loc_1_3548
        mov    cl, byte ptr [esi + 0ch]
        cmp    byte ptr [eax + 34h], cl
loc_1_3548:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    NEAR PTR loc_1_33BD
loc_1_3552:
        sub    eax, eax
loc_1_3554:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_3559:
        mov    edx, dword ptr [esp + 4]
        movzx  ecx, byte ptr [edx + 28h]
        movzx  eax, byte ptr [edx + 29h]
        add    ecx, eax
        movzx  eax, byte ptr [edx + 2ah]
        add    ecx, eax
        movzx  eax, byte ptr [edx + 2bh]
        add    ecx, eax
        and    ecx, 0fh
        db  08Dh, 004h, 08Dh, 0D8h, 038h, 001h, 000h
        mov    ecx, dword ptr [eax]
        or     ecx, ecx
        je     SHORT loc_1_358F
loc_1_3583:
        cmp    ecx, edx
        ja     SHORT loc_1_358F
        mov    eax, ecx
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_3583
loc_1_358F:
        mov    dword ptr [edx], ecx
        mov    dword ptr [eax], edx
        ret
sub_1_3594:
        mov    edx, dword ptr [esp + 4]
        movzx  ecx, byte ptr [edx + 28h]
        movzx  eax, byte ptr [edx + 29h]
        add    ecx, eax
        movzx  eax, byte ptr [edx + 2ah]
        add    ecx, eax
        movzx  eax, byte ptr [edx + 2bh]
        add    ecx, eax
        and    ecx, 0fh
        db  08Dh, 00Ch, 08Dh, 0D8h, 038h, 001h, 000h
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     SHORT loc_1_35D0
loc_1_35BE:
        cmp    eax, edx
        je     SHORT loc_1_35CC
        mov    ecx, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_35BE
        jmp    SHORT loc_1_35D0
loc_1_35CC:
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx], eax
loc_1_35D0:
        ret
sub_1_35D1:
        db  0A1h, 034h, 036h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_35EE
        mov    ecx, dword ptr [esp + 4]
loc_1_35DE:
        cmp    ecx, -1
        je     SHORT loc_1_35F1
        cmp    dword ptr [eax + 0ch], ecx
        je     SHORT loc_1_35F1
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    SHORT loc_1_35DE
loc_1_35EE:
        sub    eax, eax
loc_1_35F0:
        ret
loc_1_35F1:
        mov    byte ptr [eax + 18h], 1
        jmp    SHORT loc_1_35F0
sub_1_35F7:
        push   edi
        push   69h
        call   sub_1_F390
        add    esp, 4
        mov    edx, eax
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 1ah
        rep stosd
        stosb
        db  0A1h, 034h, 036h, 001h, 000h
        db  089h, 015h, 034h, 036h, 001h, 000h
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [edx + 0ch], eax
        mov    dword ptr [edx + 10h], 0e2h
        mov    byte ptr [edx + 1ah], 0
        mov    byte ptr [edx + 18h], 0
        mov    byte ptr [edx + 30h], 1
        db  0A1h, 074h, 036h, 001h, 000h
        mov    dword ptr [edx + 35h], eax
        mov    dword ptr [edx + 31h], eax
        mov    dword ptr [edx + 5dh], 0
        db  0A1h, 0ECh, 037h, 001h, 000h
        mov    dword ptr [edx + 61h], eax
        db  0A1h, 088h, 037h, 001h, 000h
        mov    dword ptr [edx + 65h], eax
        lea    eax, [edx + 45h]
        mov    dword ptr [edx + 39h], 0
        mov    dword ptr [edx + 55h], 32h
        mov    dword ptr [edx + 4dh], 0
        mov    dword ptr [edx + 3dh], 0
        mov    dword ptr [edx + 41h], 0
        mov    dword ptr [eax], eax
        mov    dword ptr [edx + 49h], eax
        mov    eax, edx
        pop    edi
        ret
        db  056h, 08Bh, 074h, 024h, 008h, 00Bh, 0F6h, 074h, 036h, 08Bh, 00Dh, 034h, 036h, 001h, 000h, 08Bh
        db  0D1h, 08Bh, 0C1h, 00Bh, 0C9h, 074h, 028h, 03Bh, 0C6h, 074h, 00Ah, 08Bh, 0D0h, 08Bh, 000h, 00Bh
        db  0C0h, 075h, 0F4h, 0EBh, 01Ah, 03Bh, 0C8h, 075h, 009h, 08Bh, 000h, 0A3h, 034h, 036h, 001h, 000h
        db  0EBh, 004h, 08Bh, 006h, 089h, 002h, 056h, 0E8h, 0D2h, 0BCh, 000h, 000h, 083h, 0C4h, 004h, 05Eh
        db  0C3h
loc_1_36C9:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_C657
        db  08Bh, 035h, 034h, 036h, 001h, 000h
        or     esi, esi
        je     NEAR PTR loc_1_3966
loc_1_36E3:
        cmp    byte ptr [esi + 18h], 1
        jne    NEAR PTR loc_1_395C
        mov    al, byte ptr [esi + 30h]
        cmp    al, 1
        je     SHORT loc_1_36FC
        cmp    al, 4
        jne    NEAR PTR loc_1_395C
loc_1_36FC:
        mov    eax, dword ptr [esi + 4dh]
        or     eax, eax
        je     SHORT loc_1_3732
        dec    eax
        mov    dword ptr [esi + 4dh], eax
        jne    SHORT loc_1_3732
        inc    dword ptr [esi + 51h]
        db  081h, 03Dh, 09Ch, 035h, 001h, 000h, 09Ch, 035h, 001h, 000h
        je     SHORT loc_1_371D
        call   sub_1_BB33
loc_1_371D:
        push   0
        push   0
        push   0
        push   0
        push   dword ptr [esi + 0ch]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
loc_1_3732:
        mov    eax, dword ptr [esi + 39h]
        or     eax, eax
        je     NEAR PTR loc_1_395C
        dec    eax
        mov    dword ptr [esi + 39h], eax
        jne    NEAR PTR loc_1_395C
        mov    ecx, dword ptr [esi + 59h]
        mov    eax, dword ptr [esi + 5dh]
        add    eax, ecx
        mov    dword ptr [esi + 5dh], eax
        cmp    dword ptr [esi + 61h], eax
        jbe    SHORT loc_1_376E
        cmp    dword ptr [esi + 65h], eax
        ja     SHORT loc_1_3766
        mov    eax, dword ptr [esi + 45h]
        sub    eax, esi
        cmp    eax, 45h
        je     SHORT loc_1_376E
loc_1_3766:
        mov    dword ptr [esi + 39h], ecx
        jmp    NEAR PTR loc_1_395C
loc_1_376E:
        mov    dword ptr [esi + 5dh], 0
        call   sub_1_A7B0
        mov    ebx, eax
        mov    edi, dword ptr [ebx + 4]
        mov    dword ptr [ebx], 0
        mov    dword ptr [ebx + 0ch], 3ch
        mov    byte ptr [edi], 0feh
        mov    cx, word ptr [edi + 33h]
        and    cx, 7fffh
        mov    word ptr [edi + 33h], cx
        mov    eax, dword ptr [esi + 45h]
        sub    eax, esi
        cmp    eax, 45h
        db  08Bh, 015h, 048h, 036h, 001h, 000h
        db  08Bh, 02Dh, 06Ch, 037h, 001h, 000h
        je     SHORT loc_1_37BB
        or     cx, 8000h
        mov    word ptr [edi + 33h], cx
loc_1_37BB:
        cmp    dword ptr [esi + 3dh], 0
        je     SHORT loc_1_37CA
        mov    dword ptr [esi + 41h], 0
        jmp    SHORT loc_1_37D0
loc_1_37CA:
        lea    ecx, [esi + 41h]
        inc    dword ptr [esi + 41h]
loc_1_37D0:
        mov    dword ptr [esi + 3dh], 0
        cmp    byte ptr [esi + 30h], 4
        jne    SHORT loc_1_37EB
        mov    ebp, 4
        mov    dword ptr [esi + 41h], 0
        jmp    SHORT loc_1_380D
loc_1_37EB:
        cmp    dword ptr [esi + 41h], ebp
        jb     SHORT loc_1_3808
        cmp    eax, 45h
        jne    SHORT loc_1_3808
        cmp    dword ptr [esi + 65h], edx
        jb     SHORT loc_1_3808
        mov    ebp, 3
        mov    dword ptr [esi + 41h], 0
        jmp    SHORT loc_1_380D
loc_1_3808:
        mov    ebp, 2
loc_1_380D:
        push   ebx
        movzx  eax, byte ptr [edi + 1ch]
        movzx  ecx, byte ptr [edi + 0bh]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 13h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 14h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 3]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 15h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 4]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 5]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 16h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 6]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 17h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 7]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 18h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 8]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 19h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 9]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 1ah]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0ah]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 1bh]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0ch]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 1dh]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0dh]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 1eh]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0eh]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 1fh]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 0fh]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 20h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 21h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 10h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 11h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 22h]
        add    ecx, eax
        movzx  eax, byte ptr [edi + 12h]
        add    ecx, eax
        add    ecx, 8c00h
        push   ecx
        call   sub_1_4380
        add    esp, 8
        not    ax
        mov    word ptr [edi + 37h], ax
        push   ebp
        push   0
        push   3ch
        push   ebx
        push   dword ptr [esi + 0ch]
        push   12h
        call   sub_1_D7E0
        add    esp, 18h
        dec    dword ptr [esi + 3dh]
        cmp    eax, 3ch
        je     SHORT loc_1_395C
        cmp    dword ptr [esi + 59h], 7bh
        jne    SHORT loc_1_3929
        mov    dword ptr [esi + 39h], 7bh
        mov    dword ptr [esi + 59h], 7bh
        cmp    dword ptr [esi + 41h], 0
        jbe    SHORT loc_1_391E
        mov    dword ptr [esi + 41h], 0
loc_1_391E:
        push   ebx
        call   sub_1_A7C2
        add    esp, 4
        jmp    SHORT loc_1_395C
loc_1_3929:
        push   2
        push   ebp
        push   0
        push   0
        push   0
        push   3ch
        push   ebx
        push   esi
        push   dword ptr [esi + 0ch]
        call   sub_1_3A0F
        add    esp, 24h
        mov    dword ptr [esi + 39h], 7bh
        mov    dword ptr [esi + 59h], 7bh
        cmp    dword ptr [esi + 41h], 0
        jbe    SHORT loc_1_395C
        mov    dword ptr [esi + 41h], 0
loc_1_395C:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    NEAR PTR loc_1_36E3
loc_1_3966:
        push   0
        db  0FFh, 035h, 044h, 039h, 001h, 000h
        db  068h, 080h, 039h, 000h, 000h
        call   sub_1_129CC
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_3980:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        db  0C7h, 045h, 0F8h, 0C9h, 036h, 000h, 000h
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 10h], 0bh
        call   sub_1_129A4
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp - 18h]
        mov    ebx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 0ch]
        mov    esi, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        mov    dword ptr [ebp - 1ch], esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  056h, 057h, 0FFh, 074h, 024h, 00Ch, 0E8h, 0F0h, 0FBh, 0FFh, 0FFh, 083h, 0C4h, 004h, 08Bh, 0F8h
        db  00Bh, 0FFh, 074h, 022h, 08Dh, 077h, 045h, 08Bh, 00Eh, 08Bh, 0C1h, 02Bh, 0C7h, 083h, 0F8h, 045h
        db  074h, 014h, 08Bh, 001h, 089h, 006h, 089h, 070h, 004h, 051h, 0E8h, 039h, 0F7h, 0FFh, 0FFh, 083h
        db  0C4h, 004h, 00Bh, 0FFh, 075h, 0E1h, 05Fh, 05Eh, 0C3h
sub_1_3A0F:
        call   sub_1_3120
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax + 18h], ecx
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [eax + 10h], ecx
        mov    ecx, dword ptr [esp + 1ch]
        mov    dword ptr [eax + 1ch], ecx
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 14h], ecx
        mov    ecx, dword ptr [esp + 20h]
        mov    dword ptr [eax + 0ch], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 20h], ecx
        mov    ecx, dword ptr [esp + 24h]
        test   cl, 1
        je     SHORT loc_1_3A64
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [edx + 45h]
        add    edx, 45h
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [edx], eax
        jmp    SHORT loc_1_3A83
loc_1_3A64:
        test   cl, 2
        je     SHORT loc_1_3A6B
        jmp    SHORT loc_1_3A6F
loc_1_3A6B:
        or     ecx, ecx
        jne    SHORT loc_1_3A83
loc_1_3A6F:
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [edx + 49h]
        add    edx, 45h
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], edx
loc_1_3A83:
        ret
sub_1_3A84:
        push   9
        call   sub_1_F390
        mov    cl, byte ptr [esp + 0ch]
        add    esp, 4
        mov    byte ptr [eax + 4], cl
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 5], ecx
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 2ch]
        mov    dword ptr [ecx + 2ch], eax
        mov    dword ptr [eax], edx
        dec    dword ptr [ecx + 10h]
        ret
sub_1_3AAC:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 2ch]
        mov    eax, dword ptr [esp + 0ch]
        or     eax, eax
        je     SHORT loc_1_3AC1
        mov    dword ptr [eax], 0
loc_1_3AC1:
        or     ecx, ecx
        je     SHORT loc_1_3AD4
        mov    al, byte ptr [esp + 8]
loc_1_3AC9:
        cmp    byte ptr [ecx + 4], al
        je     SHORT loc_1_3ADA
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    SHORT loc_1_3AC9
loc_1_3AD4:
        mov    eax, 1
loc_1_3AD9:
        ret
loc_1_3ADA:
        sub    eax, eax
        jmp    SHORT loc_1_3AD9
sub_1_3ADE:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_1_129F8
        imul   eax, eax, 0dh
        mov    ecx, 0e1h
        sub    edx, edx
        div    ecx
        mov    esi, dword ptr [esp + 14h]
        lea    edi, [edx + 1]
        mov    ebx, edi
        cmp    edi, ecx
        ja     SHORT loc_1_3B31
loc_1_3B02:
        cmp    ebx, 1
        jb     SHORT loc_1_3B22
        cmp    ebx, 0e1h
        ja     SHORT loc_1_3B22
        lea    eax, [esp + 0ch]
        push   eax
        push   ebx
        push   esi
        call   sub_1_3AAC
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_3B2D
loc_1_3B22:
        inc    ebx
        cmp    ebx, 0e1h
        jbe    SHORT loc_1_3B02
        jmp    SHORT loc_1_3B31
loc_1_3B2D:
        mov    eax, ebx
        jmp    SHORT loc_1_3B61
loc_1_3B31:
        mov    ebx, 1
        cmp    edi, ebx
        jbe    SHORT loc_1_3B5F
loc_1_3B3A:
        cmp    ebx, 1
        jb     SHORT loc_1_3B5A
        cmp    ebx, 0e1h
        ja     SHORT loc_1_3B5A
        lea    eax, [esp + 0ch]
        push   eax
        push   ebx
        push   esi
        call   sub_1_3AAC
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_3B68
loc_1_3B5A:
        inc    ebx
        cmp    ebx, edi
        jb     SHORT loc_1_3B3A
loc_1_3B5F:
        sub    eax, eax
loc_1_3B61:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_3B68:
        mov    eax, ebx
        jmp    SHORT loc_1_3B61
sub_1_3B6C:
        push   ebx
        push   esi
        mov    ecx, dword ptr [esp + 0ch]
        mov    bl, byte ptr [esp + 10h]
        mov    eax, dword ptr [ecx + 2ch]
        mov    edx, eax
        mov    esi, edx
        or     edx, edx
        je     SHORT loc_1_3B8E
loc_1_3B81:
        cmp    byte ptr [esi + 4], bl
        je     SHORT loc_1_3B8E
        mov    edx, esi
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_3B81
loc_1_3B8E:
        or     esi, esi
        je     SHORT loc_1_3BB2
        cmp    byte ptr [esi + 4], bl
        jne    SHORT loc_1_3BB2
        inc    dword ptr [ecx + 10h]
        cmp    edx, esi
        jne    SHORT loc_1_3BA5
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx + 2ch], eax
        jmp    SHORT loc_1_3BA9
loc_1_3BA5:
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
loc_1_3BA9:
        push   esi
        call   sub_1_F396
        add    esp, 4
loc_1_3BB2:
        pop    esi
        pop    ebx
        ret
sub_1_3BB5:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0ch]
        sub    edi, edi
        lea    esi, [eax + 4]
        mov    eax, dword ptr [eax]
        cmp    eax, edi
        jle    SHORT loc_1_3BDE
        mov    ecx, 15h
loc_1_3BCB:
        cmp    word ptr [esi + 2], cx
        je     SHORT loc_1_3BE3
        movzx  edx, word ptr [esi]
        add    edx, 4
        add    esi, edx
        inc    edi
        cmp    edi, eax
        jl     SHORT loc_1_3BCB
loc_1_3BDE:
        sub    al, al
loc_1_3BE0:
        pop    edi
        pop    esi
        ret
loc_1_3BE3:
        cmp    word ptr [esi], 1ah
        jb     SHORT loc_1_3BDE
        mov    ecx, dword ptr [esi + 14h]
        add    esi, 4
        mov    eax, dword ptr [esp + 14h]
        mov    edi, dword ptr [esp + 10h]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esi + 14h]
        mov    dword ptr [eax], ecx
        mov    al, 1
        movsd
        movsd
        movsd
        movsd
        jmp    SHORT loc_1_3BE0
sub_1_3C0A:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [ecx], 0
        or     eax, eax
        je     SHORT loc_1_3C2C
        cmp    byte ptr [eax], 0
        je     SHORT loc_1_3C2C
loc_1_3C1E:
        cmp    byte ptr [eax], 1
        jne    SHORT loc_1_3C26
        mov    byte ptr [ecx], 1
loc_1_3C26:
        inc    eax
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_3C1E
loc_1_3C2C:
        ret
loc_1_3C2D:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  083h, 03Dh, 054h, 036h, 001h, 000h, 000h
        jne    SHORT loc_1_3C64
        db  068h, 073h, 052h, 000h, 000h
        push   12h
        call   sub_1_E730
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_3C5A
        mov    eax, 1bh
        jmp    NEAR PTR loc_1_3D9C
loc_1_3C5A:
        db  0C7h, 005h, 054h, 036h, 001h, 000h, 001h, 000h, 000h, 000h
loc_1_3C64:
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 24h]
        push   eax
        push   dword ptr [esp + 40h]
        call   sub_1_3BB5
        add    esp, 10h
        or     al, al
        jne    SHORT loc_1_3C8D
        mov    eax, 3
        jmp    NEAR PTR loc_1_3D9C
loc_1_3C8D:
        lea    eax, [esp + 13h]
        push   eax
        push   dword ptr [esp + 40h]
        call   sub_1_3C0A
        add    esp, 8
        push   8eh
        call   sub_1_F390
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_1_3CBE
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, 23h
        rep stosd
        stosw
loc_1_3CBE:
        mov    ebx, dword ptr [esp + 38h]
        push   0
        push   ebx
        lea    eax, [esp + 24h]
        push   dword ptr [esp + 1ch]
        push   eax
        call   sub_1_337D
        add    esp, 10h
        cmp    dword ptr [esp + 14h], -1
        je     SHORT loc_1_3CFF
        or     eax, eax
        je     SHORT loc_1_3D19
        cmp    byte ptr [esp + 13h], 0
        jne    SHORT loc_1_3D19
        or     ebp, ebp
        je     SHORT loc_1_3CF5
        push   ebp
        call   sub_1_F396
        add    esp, 4
loc_1_3CF5:
        mov    eax, 2
        jmp    NEAR PTR loc_1_3D9C
loc_1_3CFF:
        push   dword ptr [esp + 14h]
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_3D97
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp + 3ch], eax
loc_1_3D19:
        or     ebp, ebp
        je     SHORT loc_1_3D90
        lea    eax, [ebp + 10h]
        mov    dword ptr [eax], eax
        mov    dword ptr [ebp + 14h], eax
        lea    eax, [ebp + 20h]
        lea    esi, [esp + 1ch]
        mov    dword ptr [eax], eax
        mov    dword ptr [ebp + 24h], eax
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ebp + 38h], eax
        lea    edi, [ebp + 28h]
        movsd
        movsd
        movsd
        movsd
        cmp    ebx, 51h
        mov    byte ptr [ebp + 42h], bl
        jne    SHORT loc_1_3D52
        db  0A1h, 058h, 037h, 001h, 000h
        inc    eax
        db  0A3h, 058h, 037h, 001h, 000h
loc_1_3D52:
        mov    word ptr [ebp + 40h], 80h
        mov    word ptr [ebp + 40h], 88h
        push   ebp
        call   sub_1_317E
        add    esp, 4
        mov    esi, eax
        cmp    esi, -1
        push   ebp
        jne    SHORT loc_1_3D7E
        call   sub_1_F396
        add    esp, 4
        mov    eax, 1
        jmp    SHORT loc_1_3D9C
loc_1_3D7E:
        call   sub_1_3559
        mov    eax, dword ptr [esp + 34h]
        add    esp, 4
        mov    dword ptr [eax], esi
        sub    eax, eax
        jmp    SHORT loc_1_3D9C
loc_1_3D90:
        mov    eax, 1
        jmp    SHORT loc_1_3D9C
loc_1_3D97:
        mov    eax, 4
loc_1_3D9C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
sub_1_3DA4:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        sub    esi, esi
        cmp    byte ptr [edi + 42h], 51h
        db  0A1h, 058h, 037h, 001h, 000h
        jne    SHORT loc_1_3DC1
        cmp    eax, esi
        je     SHORT loc_1_3DC1
        dec    eax
loc_1_3DC1:
        db  0A3h, 058h, 037h, 001h, 000h
        or     eax, eax
        jne    SHORT loc_1_3E38
        push   dword ptr [edi + 38h]
        call   sub_1_5230
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_3E38
        mov    ebx, dword ptr [edi + 38h]
        cmp    ebx, -1
        jne    SHORT loc_1_3DE4
        mov    ebx, dword ptr [edi + 3ch]
loc_1_3DE4:
        push   ebx
        call   sub_1_35D1
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_3E01
        cmp    byte ptr [eax + 1ah], 1
        jne    SHORT loc_1_3E01
        push   ebx
        push   12h
        call   sub_1_EA12
        jmp    SHORT loc_1_3E35
loc_1_3E01:
        push   ebx
        call   sub_1_EE10
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_3E2C
        mov    ecx, dword ptr [eax + 14h]
        or     ecx, ecx
        je     SHORT loc_1_3E2C
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   ecx
        call   sub_1_10879
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_1_3E38
loc_1_3E2C:
        push   1
        push   12h
        call   sub_1_F02D
loc_1_3E35:
        add    esp, 8
loc_1_3E38:
        test   byte ptr [edi + 40h], 20h
        je     SHORT loc_1_3E49
        mov    ecx, dword ptr [edi + 18h]
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [eax], ecx
loc_1_3E49:
        db  0A1h, 07Ch, 037h, 001h, 000h
        movzx  ecx, byte ptr [edi + 43h]
        mov    dword ptr [eax + ecx*4], 0
        push   edi
        call   sub_1_3594
        add    esp, 4
        mov    ebx, dword ptr [edi + 4ah]
        or     ebx, ebx
        je     SHORT loc_1_3EAC
        sub    eax, eax
        mov    ecx, 1
loc_1_3E70:
        mov    edx, dword ptr [ebx]
        cmp    edx, eax
        je     SHORT loc_1_3EA0
        cmp    byte ptr [edx + 0bch], al
        je     SHORT loc_1_3E94
        test   byte ptr [edx + 0beh], cl
        jne    SHORT loc_1_3E94
        inc    dword ptr [edx + 0a6h]
        mov    dword ptr [edx + 0feh], esi
        mov    esi, edx
loc_1_3E94:
        mov    dword ptr [edx + 102h], eax
        mov    dword ptr [edx + 112h], eax
loc_1_3EA0:
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx], eax
        mov    ebx, dword ptr [ebx + 8]
        cmp    ebx, eax
        jne    SHORT loc_1_3E70
loc_1_3EAC:
        mov    eax, dword ptr [edi + 20h]
        lea    ebx, [edi + 20h]
        sub    eax, edi
        cmp    eax, 20h
        je     SHORT loc_1_3EE2
loc_1_3EB9:
        mov    ebp, dword ptr [ebx]
        push   0
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ebx], eax
        mov    dword ptr [eax + 4], ebx
        push   7
        push   dword ptr [ebp + 2ch]
        call   dword ptr [ebp + 28h]
        add    esp, 0ch
        push   ebp
        call   sub_1_BACB
        add    esp, 4
        mov    eax, dword ptr [ebx]
        sub    eax, edi
        cmp    eax, 20h
        jne    SHORT loc_1_3EB9
loc_1_3EE2:
        mov    eax, dword ptr [edi + 10h]
        lea    ebx, [edi + 10h]
        sub    eax, edi
        cmp    eax, 10h
        je     SHORT loc_1_3F28
loc_1_3EEF:
        mov    ebp, dword ptr [ebx]
        push   0
        mov    eax, dword ptr [ebp]
        mov    dword ptr [ebx], eax
        mov    dword ptr [eax + 4], ebx
        push   7
        push   dword ptr [ebp + 2ch]
        call   dword ptr [ebp + 28h]
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 24h]
        or     eax, eax
        je     SHORT loc_1_3F16
        push   eax
        call   sub_1_BA30
        add    esp, 4
loc_1_3F16:
        push   ebp
        call   sub_1_BA9B
        add    esp, 4
        mov    eax, dword ptr [ebx]
        sub    eax, edi
        cmp    eax, 10h
        jne    SHORT loc_1_3EEF
loc_1_3F28:
        push   edi
        call   sub_1_F396
        add    esp, 4
        or     esi, esi
        je     SHORT loc_1_3F59
loc_1_3F35:
        dec    dword ptr [esi + 0a6h]
        or     dword ptr [esi + 60h], 40000000h
        mov    edi, dword ptr [esi + 0feh]
        push   40h
        push   esi
        call   sub_1_4B72
        add    esp, 8
        mov    esi, edi
        or     edi, edi
        jne    SHORT loc_1_3F35
loc_1_3F59:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_1_3F61:
        db  0A1h, 078h, 038h, 001h, 000h
        or     eax, eax
        je     SHORT loc_1_3F73
        mov    ecx, dword ptr [eax + 10h]
        db  089h, 00Dh, 078h, 038h, 001h, 000h
loc_1_3F73:
        ret
sub_1_3F74:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 00Dh, 078h, 038h, 001h, 000h
        mov    dword ptr [eax + 10h], ecx
        db  0A3h, 078h, 038h, 001h, 000h
        ret
loc_1_3F87:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [esp + 10h]
        push   dword ptr [ebx]
        call   sub_1_3150
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_3FA3
        jmp    NEAR PTR loc_1_4042
loc_1_3FA3:
        mov    ax, word ptr [esi + 40h]
        test   al, 40h
        jne    SHORT loc_1_3FCF
        cmp    word ptr [esi + 46h], 0
        jne    SHORT loc_1_3FCF
        or     ax, 40h
        mov    word ptr [esi + 40h], ax
        and    ax, 0ff7fh
        mov    word ptr [esi + 40h], ax
        push   esi
        call   sub_1_3DA4
        add    esp, 4
        sub    eax, eax
        jmp    SHORT loc_1_4047
loc_1_3FCF:
        test   al, 1
        jne    SHORT loc_1_4042
        mov    edi, dword ptr [esi + 0ch]
        call   sub_1_3F61
        or     eax, eax
        je     SHORT loc_1_403B
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 10h], ecx
        mov    ecx, dword ptr [ebx + 8]
        mov    dword ptr [eax + 14h], ecx
        mov    cx, word ptr [esi + 40h]
        and    cx, 0fffdh
        mov    word ptr [esi + 40h], cx
        or     cx, 1
        mov    word ptr [esi + 40h], cx
        and    cx, 0ff7fh
        mov    word ptr [esi + 40h], cx
        mov    dword ptr [esi + 0ch], eax
        mov    dword ptr [eax], 0
        or     edi, edi
        je     SHORT loc_1_4034
loc_1_4016:
        push   0
        push   7
        push   dword ptr [edi + 14h]
        call   dword ptr [edi + 10h]
        add    esp, 0ch
        mov    eax, edi
        mov    edi, dword ptr [edi]
        push   eax
        call   sub_1_3F74
        add    esp, 4
        or     edi, edi
        jne    SHORT loc_1_4016
loc_1_4034:
        mov    eax, 0ffh
        jmp    SHORT loc_1_4047
loc_1_403B:
        mov    eax, 1
        jmp    SHORT loc_1_4047
loc_1_4042:
        mov    eax, 6
loc_1_4047:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_404B:
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_1_4059
        mov    eax, 0bh
        jmp    SHORT loc_1_409B
loc_1_4059:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 8]
        dec    ecx
        cmp    ecx, 6
        ja     SHORT loc_1_406C
        db  0FFh, 024h, 08Dh, 09Ch, 040h, 000h, 000h
loc_1_406C:
        mov    eax, 0bh
        jmp    SHORT loc_1_409B
loc_1_4073:
        mov    eax, 0bh
        jmp    SHORT loc_1_409B
loc_1_407A:
        mov    eax, 0bh
        jmp    SHORT loc_1_409B
loc_1_4081:
        mov    ecx, dword ptr [esp + 10h]
        cmp    byte ptr [ecx], 0
        mov    ecx, dword ptr [esp + 4]
        je     SHORT loc_1_4095
        or     word ptr [ecx + 40h], 8
        jmp    SHORT loc_1_409B
loc_1_4095:
        and    word ptr [ecx + 40h], 0fff7h
loc_1_409B:
        ret
        db  073h, 040h, 000h, 000h, 07Ah, 040h, 000h, 000h, 07Ah, 040h, 000h, 000h, 081h, 040h, 000h, 000h
        db  073h, 040h, 000h, 000h, 07Ah, 040h, 000h, 000h, 07Ah, 040h, 000h, 000h
sub_1_40B8:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        push   dword ptr [edi]
        call   sub_1_3150
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_40D3
        jmp    NEAR PTR loc_1_4178
loc_1_40D3:
        mov    ax, word ptr [esi + 40h]
        test   al, 80h
        je     NEAR PTR loc_1_4178
        test   al, 40h
        jne    SHORT loc_1_412A
        cmp    word ptr [esi + 46h], 0
        jne    SHORT loc_1_412A
        or     ax, 40h
        push   dword ptr [esp + 18h]
        mov    word ptr [esi + 40h], ax
        push   dword ptr [esp + 18h]
        push   dword ptr [esp + 18h]
        push   esi
        call   sub_1_404B
        add    esp, 10h
        mov    edi, eax
        mov    ax, word ptr [esi + 40h]
        test   al, 7
        jne    SHORT loc_1_411D
        and    ax, 0ffbfh
        mov    word ptr [esi + 40h], ax
        mov    eax, edi
        jmp    SHORT loc_1_417D
loc_1_411D:
        push   esi
        call   sub_1_423D
        add    esp, 4
        mov    eax, edi
        jmp    SHORT loc_1_417D
loc_1_412A:
        call   sub_1_3F61
        or     eax, eax
        je     SHORT loc_1_4171
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [eax + 10h], ecx
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [eax + 14h], ecx
        mov    ecx, dword ptr [esp + 10h]
        add    esi, 0ch
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esp + 14h]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax + 0ch], ecx
        or     word ptr [esi + 34h], 2
        cmp    dword ptr [esi], 0
        je     SHORT loc_1_4168
loc_1_4161:
        mov    esi, dword ptr [esi]
        cmp    dword ptr [esi], 0
        jne    SHORT loc_1_4161
loc_1_4168:
        mov    dword ptr [esi], eax
        mov    eax, 0ffh
        jmp    SHORT loc_1_417D
loc_1_4171:
        mov    eax, 1
        jmp    SHORT loc_1_417D
loc_1_4178:
        mov    eax, 6
loc_1_417D:
        pop    edi
        pop    esi
        ret
loc_1_4180:
        push   dword ptr [esp + 4]
        call   sub_1_3150
        add    esp, 4
        or     eax, eax
        je     NEAR PTR loc_1_421F
        test   byte ptr [eax + 40h], 80h
        je     NEAR PTR loc_1_421F
        sub    edx, edx
        mov    ecx, dword ptr [esp + 8]
        cmp    ecx, 5
        ja     SHORT loc_1_41B0
        db  0FFh, 024h, 08Dh, 025h, 042h, 000h, 000h
loc_1_41B0:
        mov    edx, 0ah
        jmp    SHORT loc_1_421B
loc_1_41B7:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 5ah], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 5eh], ecx
        jmp    SHORT loc_1_421B
loc_1_41C7:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 62h], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 66h], ecx
        jmp    SHORT loc_1_421B
loc_1_41D7:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 6ah], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 6eh], ecx
        jmp    SHORT loc_1_421B
loc_1_41E7:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 72h], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 76h], ecx
        jmp    SHORT loc_1_421B
loc_1_41F7:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 7ah], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 7eh], ecx
        jmp    SHORT loc_1_421B
loc_1_4207:
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax + 82h], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax + 86h], ecx
loc_1_421B:
        mov    eax, edx
        jmp    SHORT loc_1_4224
loc_1_421F:
        mov    eax, 6
loc_1_4224:
        ret
        db  0B7h, 041h, 000h, 000h, 0C7h, 041h, 000h, 000h, 0D7h, 041h, 000h, 000h, 0E7h, 041h, 000h, 000h
        db  0F7h, 041h, 000h, 000h, 007h, 042h, 000h, 000h
sub_1_423D:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        test   byte ptr [esi + 40h], 7
        je     NEAR PTR loc_1_42FB
loc_1_424D:
        mov    edi, dword ptr [esi + 0ch]
        mov    ax, word ptr [esi + 40h]
        test   al, 1
        jne    NEAR PTR loc_1_4304
        test   al, 2
        je     SHORT loc_1_429F
loc_1_4260:
        mov    edi, dword ptr [esi + 0ch]
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 0ch], eax
        or     eax, eax
        jne    SHORT loc_1_4272
        and    word ptr [esi + 40h], 0fffdh
loc_1_4272:
        push   dword ptr [edi + 0ch]
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        push   esi
        call   sub_1_404B
        add    esp, 10h
        push   0
        push   eax
        push   dword ptr [edi + 14h]
        call   dword ptr [edi + 10h]
        add    esp, 0ch
        push   edi
        call   sub_1_3F74
        add    esp, 4
        test   byte ptr [esi + 40h], 2
        jne    SHORT loc_1_4260
loc_1_429F:
        mov    dx, word ptr [esi + 40h]
        test   dl, 4
        je     SHORT loc_1_42F1
        mov    edi, dword ptr [esi + 10h]
        lea    ecx, [esi + 10h]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 10h
        je     SHORT loc_1_42E8
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        inc    word ptr [esi + 46h]
        and    dx, 0ffbfh
        mov    word ptr [esi + 40h], dx
        push   edi
        push   esi
        call   sub_1_BCEE
        add    esp, 8
        mov    ax, word ptr [esi + 46h]
        dec    ax
        mov    word ptr [esi + 46h], ax
        jne    SHORT loc_1_42FB
        or     word ptr [esi + 40h], 40h
        jmp    SHORT loc_1_42F1
loc_1_42E8:
        and    dx, 0fffbh
        mov    word ptr [esi + 40h], dx
loc_1_42F1:
        test   byte ptr [esi + 40h], 7
        jne    NEAR PTR loc_1_424D
loc_1_42FB:
        and    word ptr [esi + 40h], 0ffbfh
loc_1_4301:
        pop    edi
        pop    esi
        ret
loc_1_4304:
        push   esi
        call   sub_1_3DA4
        add    esp, 4
        push   0
        push   0
        push   dword ptr [edi + 14h]
        call   dword ptr [edi + 10h]
        add    esp, 0ch
        push   edi
        call   sub_1_3F74
        add    esp, 4
        jmp    SHORT loc_1_4301
sub_1_4325:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 46h]
        dec    ax
        mov    word ptr [ecx + 46h], ax
        jne    SHORT loc_1_4349
        mov    ax, word ptr [ecx + 40h]
        test   al, 40h
        jne    SHORT loc_1_4349
        test   al, 7
        je     SHORT loc_1_4349
        or     ax, 40h
        mov    word ptr [ecx + 40h], ax
loc_1_4349:
        ret
sub_1_434A:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 46h]
        dec    ax
        mov    word ptr [ecx + 46h], ax
        jne    SHORT loc_1_4377
        mov    ax, word ptr [ecx + 40h]
        test   al, 40h
        jne    SHORT loc_1_4377
        test   al, 7
        je     SHORT loc_1_4377
        or     ax, 40h
        mov    word ptr [ecx + 40h], ax
        push   ecx
        call   sub_1_423D
        add    esp, 4
loc_1_4377:
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_4380:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, 3ch
        sub    ebp, ebp
        sub    esi, esi
        mov    ecx, dword ptr [esp + 14h]
        mov    eax, ecx
        shl    eax, 10h
        mov    edx, ecx
        shr    edx, 10h
        or     eax, edx
        add    eax, ecx
        shr    eax, 10h
        mov    edi, dword ptr [esp + 18h]
loc_1_43A6:
        test   ebp, 1
        je     SHORT loc_1_43C3
        mov    ecx, eax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    eax, 8
        or     ecx, eax
        mov    eax, ecx
        xor    esi, 1
loc_1_43C3:
        mov    ecx, dword ptr [edi + 4]
        add    ecx, ebx
        push   dword ptr [edi + 0ch]
        push   ecx
        push   eax
        call   sub_1_F151
        add    esp, 0ch
        sub    ebx, ebx
        mov    ebp, dword ptr [edi + 0ch]
        mov    edi, dword ptr [edi]
        cmp    edi, ebx
        jne    SHORT loc_1_43A6
        cmp    esi, ebx
        je     SHORT loc_1_43F6
        mov    ecx, eax
        and    ecx, 0ffh
        shl    ecx, 8
        shr    eax, 8
        or     ecx, eax
        mov    eax, ecx
loc_1_43F6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
        db  056h, 08Bh, 04Ch, 024h, 008h, 08Bh, 0C1h, 0C1h, 0E0h, 010h, 08Bh, 0D1h, 0C1h, 0EAh, 010h, 00Bh
        db  0C2h, 003h, 0C1h, 0C1h, 0E8h, 010h, 08Bh, 074h, 024h, 00Ch, 0FFh, 076h, 00Ch, 0FFh, 076h, 008h
        db  050h, 0E8h, 030h, 0ADh, 000h, 000h, 083h, 0C4h, 00Ch, 08Bh, 036h, 00Bh, 0F6h, 075h, 0EBh, 05Eh
        db  0C3h
sub_1_442C:
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    eax, eax
        mov    ebx, dword ptr [esp + 24h]
        cmp    ebx, eax
        je     NEAR PTR loc_1_44D9
        mov    dword ptr [esp + 10h], eax
        mov    ecx, dword ptr [ebx + 0ch]
        mov    edx, dword ptr [esp + 20h]
        mov    dword ptr [esp + 14h], ecx
        mov    ecx, dword ptr [edx + 0ch]
        sub    ecx, dword ptr [esp + 2ch]
        mov    dword ptr [esp + 18h], ecx
loc_1_445B:
        mov    ebp, dword ptr [esp + 18h]
        mov    ecx, dword ptr [esp + 14h]
        cmp    ebp, ecx
        jb     SHORT loc_1_4469
        mov    ebp, ecx
loc_1_4469:
        mov    ecx, dword ptr [esp + 28h]
        cmp    ebp, ecx
        jb     SHORT loc_1_4473
        mov    ebp, ecx
loc_1_4473:
        mov    esi, dword ptr [edx + 8]
        mov    edi, dword ptr [ebx + 4]
        add    edi, dword ptr [esp + 10h]
        mov    ecx, ebp
        shr    ecx, 2
        add    esi, dword ptr [esp + 2ch]
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        add    eax, ebp
        sub    dword ptr [esp + 18h], ebp
        jne    SHORT loc_1_44AE
        mov    edx, dword ptr [edx]
        or     edx, edx
        je     SHORT loc_1_44D9
        mov    dword ptr [esp + 2ch], 0
        mov    ecx, dword ptr [edx + 0ch]
        mov    dword ptr [esp + 18h], ecx
        jmp    SHORT loc_1_44B2
loc_1_44AE:
        add    dword ptr [esp + 2ch], ebp
loc_1_44B2:
        sub    dword ptr [esp + 14h], ebp
        jne    SHORT loc_1_44CF
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     SHORT loc_1_44D9
        mov    dword ptr [esp + 10h], 0
        mov    ecx, dword ptr [ebx + 0ch]
        mov    dword ptr [esp + 14h], ecx
        jmp    SHORT loc_1_44D3
loc_1_44CF:
        add    dword ptr [esp + 10h], ebp
loc_1_44D3:
        sub    dword ptr [esp + 28h], ebp
        jne    SHORT loc_1_445B
loc_1_44D9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_1_44E1:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 18h]
        mov    edx, dword ptr [esp + 20h]
        cmp    dword ptr [eax + 0ch], edx
        ja     SHORT loc_1_44FC
loc_1_44F2:
        sub    edx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [eax + 0ch], edx
        jbe    SHORT loc_1_44F2
loc_1_44FC:
        cmp    dword ptr [esp + 1ch], 0
        je     SHORT loc_1_453B
loc_1_4503:
        mov    ebp, dword ptr [eax + 0ch]
        sub    ebp, edx
        mov    ebx, dword ptr [esp + 1ch]
        cmp    ebx, ebp
        jb     SHORT loc_1_4512
        mov    ebx, ebp
loc_1_4512:
        mov    esi, dword ptr [eax + 8]
        mov    edi, dword ptr [esp + 14h]
        add    esi, edx
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd
        mov    ecx, ebx
        and    ecx, 3
        rep movsb
        cmp    ebp, ebx
        jne    SHORT loc_1_4531
        mov    eax, dword ptr [eax]
        sub    edx, edx
loc_1_4531:
        add    dword ptr [esp + 14h], ebx
        sub    dword ptr [esp + 1ch], ebx
        jne    SHORT loc_1_4503
loc_1_453B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_4540:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 24h]
        mov    eax, dword ptr [esp + 18h]
        mov    esi, dword ptr [ecx]
        mov    edx, dword ptr [eax + 4]
        add    edx, esi
        mov    ecx, dword ptr [eax + 0ch]
        sub    ecx, esi
        mov    dword ptr [esp + 10h], ecx
        mov    ebx, dword ptr [esp + 20h]
loc_1_4563:
        mov    ebp, dword ptr [esp + 10h]
        cmp    ebp, ebx
        jb     SHORT loc_1_456D
        mov    ebp, ebx
loc_1_456D:
        mov    edi, edx
        mov    esi, dword ptr [esp + 1ch]
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd
        mov    ecx, ebp
        and    ecx, 3
        rep movsb
        add    edx, ebp
        add    dword ptr [esp + 1ch], ebp
        sub    ebx, ebp
        je     SHORT loc_1_459F
        sub    dword ptr [esp + 10h], ebp
        jne    SHORT loc_1_4563
        mov    eax, dword ptr [eax]
        mov    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax + 0ch]
        mov    dword ptr [esp + 10h], ecx
        jmp    SHORT loc_1_4563
loc_1_459F:
        mov    ecx, dword ptr [esp + 24h]
        sub    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_45B0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 1ch]
        mov    ebx, dword ptr [esp + 20h]
        mov    ebp, dword ptr [edx + 4]
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [esp + 10h], eax
loc_1_45C9:
        mov    eax, dword ptr [esp + 10h]
        cmp    eax, ebx
        jb     SHORT loc_1_45D3
        mov    eax, ebx
loc_1_45D3:
        mov    edi, dword ptr [esp + 18h]
        mov    esi, ebp
        mov    ecx, eax
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        add    dword ptr [esp + 18h], eax
        sub    ebx, eax
        je     SHORT loc_1_4607
        sub    dword ptr [esp + 10h], eax
        jne    SHORT loc_1_4603
        mov    edx, dword ptr [edx]
        mov    ebp, dword ptr [edx + 4]
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [esp + 10h], eax
        jmp    SHORT loc_1_45C9
loc_1_4603:
        add    ebp, eax
        jmp    SHORT loc_1_45C9
loc_1_4607:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_460F:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [edi], 1
        mov    word ptr [edi + 6], 15h
        add    edi, 4
        mov    esi, dword ptr [esp + 10h]
        mov    word ptr [edi], 1ah
        mov    dword ptr [edi + 14h], eax
        add    edi, 4
        movsd
        movsd
        movsd
        movsd
        pop    edi
        pop    esi
        ret
sub_1_463E:
        push   esi
        sub    esi, esi
        mov    ecx, dword ptr [esp + 8]
        cmp    ecx, esi
        je     SHORT loc_1_467F
        mov    dword ptr [ecx], esi
        mov    eax, dword ptr [ecx + 10h]
        cmp    eax, esi
        je     SHORT loc_1_467F
        cmp    eax, 22h
        jb     SHORT loc_1_4673
        push   dword ptr [esp + 10h]
        mov    dword ptr [ecx + 10h], 22h
        push   dword ptr [esp + 10h]
        push   dword ptr [ecx + 14h]
        call   sub_1_460F
        add    esp, 0ch
        jmp    SHORT loc_1_467F
loc_1_4673:
        mov    dword ptr [ecx + 10h], 0
        mov    esi, 1ah
loc_1_467F:
        mov    eax, esi
        pop    esi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4690:
        ret
loc_1_4691:
        ret
sub_1_4692:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 18h]
        mov    ebx, dword ptr [esi + 1ch]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_1_46B0
        push   esi
        push   eax
        call   sub_1_4C3F
        add    esp, 8
loc_1_46B0:
        push   esi
        call   sub_1_F396
        add    esp, 4
        push   0
        push   0
        push   ebx
        call   edi
        add    esp, 0ch
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_46C7:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 14h]
        sub    ebp, ebp
        mov    edi, dword ptr [esi + 18h]
        mov    ebx, dword ptr [esi + 1ch]
        test   byte ptr [esi + 0dh], 1
        mov    word ptr [esi + 0eh], 1
        jne    SHORT loc_1_46FA
        mov    eax, dword ptr [esi + 4]
        cmp    eax, ebp
        je     SHORT loc_1_46F4
        push   esi
        push   eax
        call   sub_1_4C3F
        add    esp, 8
loc_1_46F4:
        and    byte ptr [esi + 0dh], 0fdh
        jmp    SHORT loc_1_46FF
loc_1_46FA:
        mov    ebp, 1
loc_1_46FF:
        mov    word ptr [esi + 0eh], 0
        or     ebp, ebp
        je     SHORT loc_1_4714
        push   esi
        call   sub_1_4692
        add    esp, 4
        jmp    SHORT loc_1_471E
loc_1_4714:
        push   0
        push   0
        push   ebx
        call   edi
        add    esp, 0ch
loc_1_471E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
_LTEXT ends
        end
