; pccard_pcode_03_01.asm -- PCCARD object 3 segment _PTEXT  (part 1/2)
.386p
_PTEXT segment dword public use32 'PCODE'

sub_3_0000:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        push   0
        push   dword ptr [esp + 8]
        call   sub_3_5278
        add    esp, 8
        ret
sub_3_0019:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        db  0CDh, 020h, 000h, 000h, 033h, 000h
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 400h
        jb     NEAR PTR loc_3_00C4
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        je     NEAR PTR loc_3_00C4
        cmp    eax, 1
        je     SHORT loc_3_0052
        cmp    eax, 2
        je     SHORT loc_3_00C4
        mov    eax, 1
        jmp    SHORT loc_3_00C9
loc_3_0052:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_0062
        mov    eax, 21h
        jmp    SHORT loc_3_00C9
loc_3_0062:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        test   ecx, ecx
        je     SHORT loc_3_00B9
loc_3_006F:
        mov    edx, dword ptr [ecx + 10h]
        test   edx, edx
        je     SHORT loc_3_009A
loc_3_0076:
        cmp    dword ptr [edx + 8], eax
        je     SHORT loc_3_00A2
        mov    esi, dword ptr [edx + 2ch]
        test   esi, esi
        jne    SHORT loc_3_0089
        mov    esi, dword ptr [edx + 24h]
        test   esi, esi
        je     SHORT loc_3_0094
loc_3_0089:
        cmp    dword ptr [esi + 2ch], eax
        je     SHORT loc_3_00AD
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_3_0089
loc_3_0094:
        mov    edx, dword ptr [edx]
        test   edx, edx
        jne    SHORT loc_3_0076
loc_3_009A:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_3_006F
        jmp    SHORT loc_3_00B9
loc_3_00A2:
        push   edx
        call   sub_3_00FD
        add    esp, 4
        jmp    SHORT loc_3_00C9
loc_3_00AD:
        push   eax
        push   edx
        call   sub_3_0139
        add    esp, 8
        jmp    SHORT loc_3_00C9
loc_3_00B9:
        push   eax
        call   sub_3_01A2
        add    esp, 4
        jmp    SHORT loc_3_00C9
loc_3_00C4:
        mov    eax, 13h
loc_3_00C9:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_00D0:
        mov    eax, dword ptr [esp + 4]
        push   esi
        push   0
        push   0
        mov    esi, dword ptr [eax + 8]
        push   esi
        call   sub_3_5288
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_00FB
        push   3
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_3_5280
        add    esp, 10h
loc_3_00FB:
        pop    esi
        ret
sub_3_00FD:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 30h], 2
        jb     SHORT loc_3_011B
        push   3
        push   0
        push   0
        push   dword ptr [esi + 8]
        call   sub_3_5280
        add    esp, 10h
        jmp    SHORT loc_3_0137
loc_3_011B:
        push   esi
        call   sub_3_00D0
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_0137
        add    esi, 14h
        mov    ecx, dword ptr [esi]
        or     cl, 2
        mov    dword ptr [esi], ecx
        and    cl, 0feh
        mov    dword ptr [esi], ecx
loc_3_0137:
        pop    esi
        ret
sub_3_0139:
        push   ebp
        mov    ebp, esp
        sub    esp, 54h
        push   esi
        lea    eax, [ebp - 4]
        push   edi
        lea    ecx, [ebp - 54h]
        push   1
        xor    edi, edi
        push   eax
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], 50h
        push   ecx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        push   esi
        call   sub_3_5298
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_3_019C
        push   edi
        lea    eax, [ebp - 54h]
        push   2
        push   eax
        push   esi
        call   sub_3_5290
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_019C
        push   edi
        push   edi
        push   esi
        call   sub_3_5288
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_019C
        push   3
        push   edi
        push   edi
        push   esi
        call   sub_3_5280
        add    esp, 10h
loc_3_019C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_01A2:
        push   ebp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 58h
        test   ecx, ecx
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_3_01C3
loc_3_01B5:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 4], eax
        je     SHORT loc_3_01DE
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_3_01B5
loc_3_01C3:
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_01CB:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_3_01E8
        mov    eax, 2
        jmp    NEAR PTR loc_3_02A5
loc_3_01DE:
        mov    eax, 10h
        jmp    NEAR PTR loc_3_02A5
loc_3_01E8:
        mov    dword ptr [ebp - 8], 50h
        push   1
        xor    edi, edi
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 8]
        call   sub_3_5288
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    NEAR PTR loc_3_029A
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 8]
        call   sub_3_52B0
        add    esp, 0ch
        mov    ebx, eax
        cmp    ebx, edi
        jne    SHORT loc_3_029A
        push   1
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 58h]
        push   ecx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_3_5298
        add    esp, 1ch
        mov    ebx, eax
        cmp    ebx, edi
        jne    SHORT loc_3_029A
        push   edi
        lea    eax, [ebp - 58h]
        push   2
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_3_5290
        add    esp, 10h
        mov    ebx, eax
        cmp    ebx, edi
        jne    SHORT loc_3_029A
        push   edi
        lea    eax, [ebp - 4]
        push   edi
        push   edi
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_3_52A8
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_3_0296
        cmp    dword ptr [ebp - 4], 4
        jne    SHORT loc_3_0296
        push   edi
        lea    eax, [esi + 28h]
        push   8
        push   eax
        push   edi
        push   2
        push   dword ptr [ebp + 8]
        call   sub_3_52A0
        add    esp, 18h
        mov    ebx, eax
        cmp    ebx, edi
        jne    SHORT loc_3_029A
loc_3_0296:
        xor    eax, eax
        jmp    SHORT loc_3_02A5
loc_3_029A:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_02A0:
        add    esp, 4
        mov    eax, ebx
loc_3_02A5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_02AC:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        push   ebp
        test   esi, esi
        je     SHORT loc_3_02FC
loc_3_02BA:
        mov    edi, dword ptr [esi + 10h]
        test   edi, edi
        je     SHORT loc_3_02E9
loc_3_02C1:
        xor    bl, bl
        mov    ebp, dword ptr [edi + 18h]
        test   ebp, ebp
        je     SHORT loc_3_02DF
loc_3_02CA:
        cmp    bl, byte ptr [edi + 30h]
        jae    SHORT loc_3_02DF
        push   ebp
        inc    bl
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_02D7:
        add    esp, 4
        add    ebp, 28h
        jne    SHORT loc_3_02CA
loc_3_02DF:
        or     byte ptr [edi + 14h], 1
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_3_02C1
loc_3_02E9:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_02EF:
        add    esp, 4
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        jne    SHORT loc_3_02BA
loc_3_02FC:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_3_0317
loc_3_0305:
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_030B:
        add    esp, 4
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        jne    SHORT loc_3_0305
loc_3_0317:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_031E:
        push   esi
        push   0
        db  068h, 000h, 000h, 000h, 000h
        call   sub_3_1180
        add    esp, 8
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 008h
        jne    SHORT loc_3_03A5
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 1
        je     SHORT loc_3_0351
        cmp    eax, 2
        je     SHORT loc_3_035D
        cmp    eax, 3
        je     SHORT loc_3_0378
        cmp    eax, 4
        je     SHORT loc_3_0384
        jmp    SHORT loc_3_03A5
loc_3_0351:
        push   12ch
        push   1521h
        jmp    SHORT loc_3_039D
loc_3_035D:
        mov    esi, 96h
        push   esi
        push   70bh
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_036D:
        add    esp, 8
        push   esi
        push   548h
        jmp    SHORT loc_3_039D
loc_3_0378:
        push   12ch
        push   1521h
        jmp    SHORT loc_3_039D
loc_3_0384:
        mov    esi, 96h
        push   esi
        push   548h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0394:
        add    esp, 8
        push   esi
        push   70bh
loc_3_039D:
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_03A2:
        add    esp, 8
loc_3_03A5:
        pop    esi
        ret
sub_3_03A7:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        xor    ebx, ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_03B5:
        add    esp, 4
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     SHORT loc_3_03CF
        push   ebx
        push   eax
        call   sub_3_52B8
        add    esp, 8
        mov    byte ptr [esi + 48h], bl
        mov    dword ptr [esi + 8], ebx
loc_3_03CF:
        push   esi
        call   sub_3_50F5
        add    esp, 4
        pop    esi
        pop    ebx
        ret
sub_3_03DB:
        push   ebp
        and    byte ptr [esp + 13h], 7fh
        mov    ebp, esp
        sub    esp, 90h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi]
        mov    ax, word ptr [ebx + 10h]
        mov    word ptr [ebp - 0ch], ax
        lea    eax, [ebp - 0ch]
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0403:
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_3_0561
        test   byte ptr [ebp - 0ah], 80h
        je     NEAR PTR loc_3_0561
        mov    eax, dword ptr [esi + 8]
        test   eax, 100000h
        je     SHORT loc_3_0438
        cmp    dword ptr [ebp + 10h], 11h
        je     SHORT loc_3_0438
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0430:
        add    esp, 8
        jmp    NEAR PTR loc_3_0577
loc_3_0438:
        test   eax, 80000h
        je     SHORT loc_3_044B
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_3_10AE
        add    esp, 8
loc_3_044B:
        mov    ax, word ptr [ebx + 10h]
        mov    ecx, ebx
        mov    word ptr [ebp - 2], 0
        mov    word ptr [ebp - 4], ax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0460:
        test   eax, eax
        jne    NEAR PTR loc_3_0561
        cmp    byte ptr [ebx + 48h], 0
        je     NEAR PTR loc_3_0561
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_047C:
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_3_0561
        cmp    word ptr [ebp - 2], 0
        je     NEAR PTR loc_3_0561
        push   0
        lea    eax, [ebp - 90h]
        push   0
        push   eax
        push   ebx
        call   sub_3_394C
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_3_0561
        lea    edi, [ebx + 48h]
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        lea    edi, [ecx + ebx + 45h]
        cmp    byte ptr [edi], 5ch
        jne    NEAR PTR loc_3_0561
        lea    eax, [ebp - 90h]
        lea    ecx, [ebx + 48h]
        mov    edx, ecx
        mov    byte ptr [edi], 0
loc_3_04D7:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_3_04F7
        or     cl, cl
        je     SHORT loc_3_04F3
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_3_04F7
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_3_04D7
loc_3_04F3:
        xor    eax, eax
        jmp    SHORT loc_3_04FC
loc_3_04F7:
        sbb    eax, eax
        sbb    eax, -1
loc_3_04FC:
        mov    byte ptr [edi], 5ch
        test   eax, eax
        jne    SHORT loc_3_0561
        cmp    dword ptr [ebp + 10h], 7
        je     SHORT loc_3_0515
        cmp    dword ptr [ebp + 10h], 8
        je     SHORT loc_3_0515
        cmp    dword ptr [ebp + 10h], 11h
        jne    SHORT loc_3_054C
loc_3_0515:
        push   0
        lea    eax, [ebp - 10h]
        push   eax
        push   dword ptr [ebp + 0ch]
        call   sub_3_52C0
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_054C
        test   byte ptr [ebp - 10h], 8
        je     SHORT loc_3_054C
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0536:
        add    esp, 4
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_053F:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_0577
        or     byte ptr [ebx + 14h], 20h
        jmp    SHORT loc_3_0577
loc_3_054C:
        push   dword ptr [ebp + 0ch]
        push   esi
        call   sub_3_0E3D
        add    esp, 8
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_3_0579
loc_3_0561:
        push   ebx
        call   sub_3_03A7
        add    esp, 4
        and    byte ptr [ebx + 14h], 0feh
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0574:
        add    esp, 4
loc_3_0577:
        xor    eax, eax
loc_3_0579:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_0580:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        test   byte ptr [eax + 8], 4
        je     SHORT loc_3_0592
        mov    eax, 1
        jmp    SHORT loc_3_05BE
loc_3_0592:
        mov    eax, dword ptr [eax + 10h]
        test   eax, eax
        je     SHORT loc_3_05BC
loc_3_0599:
        xor    dl, dl
        mov    ecx, dword ptr [eax + 18h]
        test   ecx, ecx
        je     SHORT loc_3_05B6
        mov    bl, byte ptr [eax + 30h]
loc_3_05A5:
        cmp    dl, bl
        jae    SHORT loc_3_05B6
        test   byte ptr [ecx + 0ah], 10h
        jne    SHORT loc_3_05C0
        inc    dl
        add    ecx, 28h
        jne    SHORT loc_3_05A5
loc_3_05B6:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_3_0599
loc_3_05BC:
        xor    eax, eax
loc_3_05BE:
        pop    ebx
        ret
loc_3_05C0:
        mov    eax, 1
        jmp    SHORT loc_3_05BE
loc_3_05C7:
        push   ebp
        mov    ebp, esp
        push   dword ptr [esp + 10h]
        push   0
        call   sub_3_1FF8
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_0604
        mov    eax, dword ptr [eax]
        test   byte ptr [eax + 17h], 10h
        je     SHORT loc_3_0604
        mov    eax, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [eax + 2ch]
        test   ecx, ecx
        je     SHORT loc_3_0604
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   ecx
        add    esp, 14h
        jmp    SHORT loc_3_0609
loc_3_0604:
        mov    eax, 20h
loc_3_0609:
        pop    ebp
        ret
loc_3_060B:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 10h]
        push   edi
        call   sub_3_1F16
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_3_0C8F
        cmp    dword ptr [ebp + 14h], edi
        jne    NEAR PTR loc_3_0997
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        je     SHORT loc_3_0674
        cmp    eax, 2
        je     SHORT loc_3_06AC
        cmp    eax, 4
        je     NEAR PTR loc_3_06D6
        cmp    eax, 5
        je     NEAR PTR loc_3_06FC
        cmp    eax, 8
        je     NEAR PTR loc_3_0892
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   edi
        push   dword ptr [ebp + 0ch]
        push   eax
        push   ebx
        call   sub_3_1C3C
        add    esp, 18h
        jmp    NEAR PTR loc_3_0C94
loc_3_0674:
        cmp    dword ptr [ebp + 0ch], 1
        je     SHORT loc_3_0691
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0680:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_0691
        mov    eax, 13h
        jmp    NEAR PTR loc_3_0C94
loc_3_0691:
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   edi
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_3_1C3C
        add    esp, 18h
        jmp    NEAR PTR loc_3_0C94
loc_3_06AC:
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   edi
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_3_1C3C
        add    esp, 18h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_06C8:
        add    esp, 4
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        jmp    NEAR PTR loc_3_0C94
loc_3_06D6:
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   edi
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_3_1C3C
        add    esp, 18h
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_06F2:
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_3_0C94
loc_3_06FC:
        mov    esi, dword ptr [ebx + 10h]
        test   esi, esi
        je     NEAR PTR loc_3_0843
        xor    ebx, ebx
loc_3_0709:
        mov    eax, dword ptr [esi + 14h]
        test   eax, 10000000h
        jne    NEAR PTR loc_3_084A
        test   al, 1
        je     NEAR PTR loc_3_07F4
        and    al, 0b9h
        push   esi
        mov    dword ptr [esi + 14h], eax
        mov    dword ptr [ebp - 4], ebx
        call   sub_3_03A7
        mov    cx, word ptr [esi + 10h]
        add    esp, 4
        mov    word ptr [ebp - 0ch], cx
        lea    ecx, [ebp - 0ch]
        push   ecx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0742:
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_3_0839
        test   byte ptr [ebp - 0ah], 80h
        je     NEAR PTR loc_3_0839
        test   byte ptr [esi + 14h], 8
        je     NEAR PTR loc_3_0839
        mov    dword ptr [ebp - 10h], 13h
        mov    ecx, esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_076F:
        test   eax, eax
        jne    NEAR PTR loc_3_086B
        push   dword ptr [esi + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_077F:
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_3_086B
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        push   esi
        call   sub_3_3F1C
        add    esp, 0ch
        mov    dword ptr [ebp - 10h], eax
        cmp    eax, ebx
        jne    NEAR PTR loc_3_086B
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 18h]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 14h]
        and    al, 0feh
        mov    dword ptr [esi + 14h], eax
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_3_07C3
        test   al, 2
        je     SHORT loc_3_07CD
loc_3_07C3:
        push   edi
        push   esi
        call   sub_3_4F85
        add    esp, 8
loc_3_07CD:
        cmp    dword ptr [ebp - 4], ebx
        je     SHORT loc_3_0839
        test   byte ptr [esi + 14h], 2
        je     SHORT loc_3_0839
        push   esi
        call   sub_3_00D0
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_0862
        and    byte ptr [esi + 14h], 0feh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_07EF:
        add    esp, 4
        jmp    SHORT loc_3_0839
loc_3_07F4:
        mov    ax, word ptr [esi + 10h]
        mov    word ptr [ebp - 0ch], ax
        lea    eax, [ebp - 0ch]
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0806:
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_0839
        test   byte ptr [ebp - 0ah], 80h
        je     SHORT loc_3_0839
        lea    eax, [esi + 48h]
        cmp    byte ptr [eax], bl
        je     SHORT loc_3_0839
        push   ebx
        lea    ecx, [ebp - 4]
        push   edi
        push   eax
        push   ecx
        call   sub_3_52E0
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_0839
        push   ebx
        push   dword ptr [ebp - 4]
        call   sub_3_52B8
        add    esp, 8
loc_3_0839:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    NEAR PTR loc_3_0709
loc_3_0843:
        xor    eax, eax
        jmp    NEAR PTR loc_3_0C94
loc_3_084A:
        push   dword ptr [ebp + 18h]
        push   edi
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   esi
        call   sub_3_1C6D
        add    esp, 14h
        jmp    NEAR PTR loc_3_0C94
loc_3_0862:
        and    byte ptr [esi + 14h], 0fdh
        jmp    NEAR PTR loc_3_0C94
loc_3_086B:
        xor    edi, edi
        cmp    dword ptr [ebp - 4], edi
        je     SHORT loc_3_0881
        push   edi
        push   dword ptr [ebp - 4]
        call   sub_3_52B8
        add    esp, 8
        mov    dword ptr [ebp - 4], edi
loc_3_0881:
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0887:
        add    esp, 4
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_3_0C94
loc_3_0892:
        push   0
        db  068h, 000h, 000h, 000h, 000h
        call   sub_3_1180
        add    esp, 8
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 020h
        jne    NEAR PTR loc_3_0C8F
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 7fffffffh
        cmp    eax, 5
        je     SHORT loc_3_08E5
        cmp    eax, 7
        jb     SHORT loc_3_08CA
        cmp    eax, 8
        jbe    SHORT loc_3_0918
        cmp    eax, 11h
        je     SHORT loc_3_0918
loc_3_08CA:
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   edi
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_3_1C3C
        add    esp, 18h
        jmp    NEAR PTR loc_3_0C94
loc_3_08E5:
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   edi
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_3_1C3C
        add    esp, 18h
        push   ebx
        call   sub_3_0580
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_0911
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_090E:
        add    esp, 4
loc_3_0911:
        xor    eax, eax
        jmp    NEAR PTR loc_3_0C94
loc_3_0918:
        push   ebx
        call   sub_3_0580
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_0932
        test   byte ptr [ebp + 0ch], 1
        je     SHORT loc_3_0932
        xor    eax, eax
        jmp    NEAR PTR loc_3_0C94
loc_3_0932:
        test   byte ptr [ebx + 8], 10h
        je     SHORT loc_3_0941
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_093E:
        add    esp, 4
loc_3_0941:
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0947:
        add    esp, 4
        mov    esi, dword ptr [ebx + 10h]
        test   esi, esi
        je     SHORT loc_3_097C
loc_3_0951:
        cmp    dword ptr [esi + 8], 0
        jne    SHORT loc_3_0976
        mov    eax, dword ptr [esi + 18h]
        cmp    dword ptr [eax + 4], 0
        jne    SHORT loc_3_0976
        push   esi
        call   sub_3_03A7
        add    esp, 4
        and    byte ptr [esi + 14h], 0feh
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0973:
        add    esp, 4
loc_3_0976:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_3_0951
loc_3_097C:
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   edi
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_3_1C3C
        add    esp, 18h
        jmp    NEAR PTR loc_3_0C8B
loc_3_0997:
        push   0
        lea    ecx, [ebp - 14h]
        push   dword ptr [ebp + 14h]
        push   ecx
        call   sub_3_52D8
        add    esp, 0ch
        cmp    dword ptr [ebp - 14h], edi
        je     SHORT loc_3_09C8
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   edi
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_3_1C3C
        add    esp, 18h
        jmp    NEAR PTR loc_3_0C94
loc_3_09C8:
        push   dword ptr [ebp + 14h]
        push   ebx
        call   sub_3_1F31
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_3_0A0D
        push   dword ptr [ebp + 14h]
        push   ebx
        call   sub_3_1FF8
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_0A0D
        mov    ecx, dword ptr [eax]
        test   byte ptr [ecx + 17h], 10h
        je     SHORT loc_3_0A0D
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   edi
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        push   eax
        call   sub_3_1D6C
        add    esp, 18h
        jmp    NEAR PTR loc_3_0C94
loc_3_0A0D:
        xor    edi, edi
        test   esi, esi
        je     SHORT loc_3_0A16
        mov    edi, dword ptr [esi + 18h]
loc_3_0A16:
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     SHORT loc_3_0A47
        cmp    eax, 1
        je     SHORT loc_3_0A58
        cmp    eax, 2
        je     NEAR PTR loc_3_0AAE
        cmp    eax, 4
        je     SHORT loc_3_0AAE
        cmp    eax, 6
        je     NEAR PTR loc_3_0B12
        cmp    eax, 8
        je     NEAR PTR loc_3_0B39
        jmp    NEAR PTR loc_3_0C8F
loc_3_0A47:
        push   dword ptr [ebp + 14h]
        push   edi
        call   sub_3_0CDF
        add    esp, 8
        jmp    NEAR PTR loc_3_0C94
loc_3_0A58:
        test   edi, edi
        je     SHORT loc_3_0A6D
        push   dword ptr [ebp + 14h]
        push   edi
        call   sub_3_0E3D
        add    esp, 8
        jmp    NEAR PTR loc_3_0C94
loc_3_0A6D:
        push   0
        push   dword ptr [ebp + 14h]
        push   ebx
        call   sub_3_5124
        add    esp, 0ch
        test   eax, eax
        je     NEAR PTR loc_3_0C8F
        cmp    byte ptr [eax + 48h], 0
        jne    NEAR PTR loc_3_0C8F
        mov    ecx, dword ptr [eax + 14h]
        test   cl, 10h
        jne    NEAR PTR loc_3_0C8F
        or     cl, 10h
        push   2
        mov    dword ptr [eax + 14h], ecx
        call   sub_3_031E
        add    esp, 4
        jmp    NEAR PTR loc_3_0C8B
loc_3_0AAE:
        test   edi, edi
        je     SHORT loc_3_0AC6
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 14h]
        push   edi
        call   sub_3_104E
        add    esp, 0ch
        jmp    NEAR PTR loc_3_0C94
loc_3_0AC6:
        push   0
        push   dword ptr [ebp + 14h]
        push   ebx
        call   sub_3_5124
        add    esp, 0ch
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_3_0B0B
        cmp    dword ptr [ebp + 8], 4
        jne    SHORT loc_3_0B0B
        cmp    dword ptr [esi + 8], 0
        je     SHORT loc_3_0AFB
        cmp    byte ptr [esi + 48h], 0
        jne    SHORT loc_3_0AFB
        test   byte ptr [esi + 14h], 2
        je     SHORT loc_3_0AFB
        push   esi
        call   sub_3_03A7
        add    esp, 4
loc_3_0AFB:
        push   dword ptr [ebp + 14h]
        push   ebx
        call   sub_3_518B
        and    byte ptr [esi + 14h], 0efh
        add    esp, 8
loc_3_0B0B:
        xor    eax, eax
        jmp    NEAR PTR loc_3_0C94
loc_3_0B12:
        test   edi, edi
        je     SHORT loc_3_0B26
        push   edi
        call   sub_3_4106
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_3_0C94
loc_3_0B26:
        push   dword ptr [ebp + 14h]
        push   ebx
        call   sub_3_51B2
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_3_0C94
loc_3_0B39:
        test   esi, esi
        je     NEAR PTR loc_3_0C8F
        push   0
        db  068h, 000h, 000h, 000h, 000h
        call   sub_3_1180
        add    esp, 8
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 020h
        jne    NEAR PTR loc_3_0C8F
        mov    ecx, dword ptr [esi + 8]
        cmp    dword ptr [edi + 4], ecx
        jne    NEAR PTR loc_3_0C8F
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 7fffffffh
        dec    eax
        cmp    eax, 10h
        ja     NEAR PTR loc_3_0C8F
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_3_0B82:
        test   byte ptr [edi + 0ah], 2
        jne    NEAR PTR loc_3_0C8F
        test   esi, esi
        je     NEAR PTR loc_3_0C8B
        push   0
        push   ecx
        call   sub_3_52D0
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_3_0BB9
        push   0
        push   dword ptr [ebx + 4]
        call   sub_3_52C8
        add    esp, 8
        mov    eax, edi
        jmp    NEAR PTR loc_3_0C94
loc_3_0BB9:
        push   esi
        call   sub_3_03A7
        add    esp, 4
        jmp    NEAR PTR loc_3_0C8B
loc_3_0BC7:
        test   byte ptr [edi + 0ah], 10h
        je     SHORT loc_3_0BD7
        push   1
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0BD5:
        jmp    SHORT loc_3_0BE0
loc_3_0BD7:
        push   dword ptr [ebp + 14h]
        push   edi
        call   sub_3_10AE
loc_3_0BE0:
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_3_0C94
loc_3_0BEA:
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 14h]
        push   edi
        call   sub_3_03DB
        add    esp, 0ch
        jmp    NEAR PTR loc_3_0C94
loc_3_0BFE:
        lea    ecx, [edi + 8]
        mov    eax, dword ptr [ecx]
        and    eax, 0ffefffffh
        mov    dword ptr [ecx], eax
        or     eax, 20000h
        mov    dword ptr [ecx], eax
        test   eax, 80000h
        jne    SHORT loc_3_0C29
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 14h]
        push   edi
        call   sub_3_03DB
        add    esp, 0ch
        jmp    SHORT loc_3_0C94
loc_3_0C29:
        xor    eax, eax
        jmp    SHORT loc_3_0C94
loc_3_0C2D:
        xor    eax, eax
        lea    esi, [edi + 8]
        mov    ecx, dword ptr [esi]
        or     ecx, 20000h
        test   ecx, 80000h
        mov    dword ptr [esi], ecx
        jne    SHORT loc_3_0C5B
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 14h]
        push   edi
        and    ecx, 0ffefffffh
        mov    dword ptr [esi], ecx
        call   sub_3_03DB
        add    esp, 0ch
loc_3_0C5B:
        test   eax, eax
        jne    SHORT loc_3_0C94
        or     byte ptr [esi + 2], 10h
        jmp    SHORT loc_3_0C94
loc_3_0C65:
        lea    ecx, [edi + 8]
        mov    eax, dword ptr [ecx]
        and    eax, 0ffefffffh
        mov    dword ptr [ecx], eax
        or     eax, 20000h
        mov    dword ptr [ecx], eax
        test   eax, 80000h
        je     SHORT loc_3_0C8B
        push   dword ptr [ebp + 14h]
        push   edi
        call   sub_3_10AE
        add    esp, 8
loc_3_0C8B:
        xor    eax, eax
        jmp    SHORT loc_3_0C94
loc_3_0C8F:
        mov    eax, 1
loc_3_0C94:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
sub_3_0CDF:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     NEAR PTR loc_3_0E34
        push   1
        lea    ecx, [ebp - 0ch]
        push   dword ptr [ebp + 0ch]
        push   ecx
        call   sub_3_5300
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_3_0E34
        xor    edi, edi
loc_3_0D0E:
        mov    eax, dword ptr [ebp - 0ch]
        xor    ebx, ebx
        push   edi
        lea    ecx, [ebp - 8]
        push   ecx
        lea    edx, [ebp - 14h]
        push   897ch
        mov    dword ptr [ebp - 4], eax
        push   eax
        push   edx
        call   sub_3_52F8
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_3_0E04
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edi
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_3_52F8
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_3_0DF4
loc_3_0D52:
        test   ebx, ebx
        jne    NEAR PTR loc_3_0DF4
        mov    ecx, 40000000h
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 1
        je     SHORT loc_3_0D7A
        cmp    eax, 2
        je     SHORT loc_3_0D95
        cmp    eax, 4
        je     SHORT loc_3_0DB3
        cmp    eax, 897ch
        je     SHORT loc_3_0DC9
        jmp    SHORT loc_3_0DD7
loc_3_0D7A:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 24h]
        or     eax, 40000004h
        push   eax
        push   dword ptr [ebp - 8]
        push   ecx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0D90:
        add    esp, 10h
        jmp    SHORT loc_3_0DD5
loc_3_0D95:
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 18h], 40h
        je     SHORT loc_3_0DA3
        mov    ecx, 40000008h
loc_3_0DA3:
        push   ecx
        push   dword ptr [ebp - 8]
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0DAE:
        add    esp, 10h
        jmp    SHORT loc_3_0DD5
loc_3_0DB3:
        push   40000020h
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0DC4:
        add    esp, 10h
        jmp    SHORT loc_3_0DD5
loc_3_0DC9:
        push   dword ptr [ebp - 4]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0DD2:
        add    esp, 8
loc_3_0DD5:
        mov    ebx, eax
loc_3_0DD7:
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edi
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_3_52F8
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_3_0D52
loc_3_0DF4:
        push   20000000h
        push   edi
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0E01:
        add    esp, 10h
loc_3_0E04:
        mov    eax, dword ptr [ebp - 0ch]
        push   edi
        push   eax
        lea    ecx, [ebp - 0ch]
        push   ecx
        mov    dword ptr [ebp - 10h], eax
        call   sub_3_52F0
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        test   ebx, ebx
        je     SHORT loc_3_0E2B
        push   edi
        push   dword ptr [ebp - 10h]
        call   sub_3_52E8
        add    esp, 8
loc_3_0E2B:
        cmp    dword ptr [ebp - 4], edi
        je     NEAR PTR loc_3_0D0E
loc_3_0E34:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_0E3D:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        test   edi, edi
        jne    SHORT loc_3_0E57
        mov    eax, 1
        jmp    NEAR PTR loc_3_1047
loc_3_0E57:
        mov    esi, dword ptr [edi]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 8], eax
        jne    SHORT loc_3_0E80
        mov    ecx, esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0E68:
        test   eax, eax
        je     SHORT loc_3_0E80
        push   3
        call   sub_3_031E
        add    esp, 4
        mov    eax, 13h
        jmp    NEAR PTR loc_3_1047
loc_3_0E80:
        cmp    byte ptr [esi + 30h], 2
        jb     SHORT loc_3_0E9E
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 8], eax
        jne    SHORT loc_3_0E9E
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0E94:
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_3_1047
loc_3_0E9E:
        push   2
        lea    ecx, [ebp - 14h]
        push   dword ptr [ebp + 0ch]
        push   ecx
        call   sub_3_5300
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_3_1022
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 10h]
        push   897ch
        push   dword ptr [ebp - 14h]
        push   edx
        call   sub_3_52F8
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_3_1022
        mov    eax, dword ptr [ebp - 14h]
        xor    ebx, ebx
        push   ebx
        lea    ecx, [ebp - 8]
        push   ecx
        lea    edx, [ebp - 4]
        push   ebx
        mov    dword ptr [ebp - 4], eax
        push   eax
        push   edx
        call   sub_3_52F8
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_3_0F71
loc_3_0EF8:
        test   ebx, ebx
        jne    SHORT loc_3_0F71
        xor    ecx, ecx
        test   byte ptr [esi + 14h], 2
        je     SHORT loc_3_0F09
        mov    ecx, 80000000h
loc_3_0F09:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 1
        je     SHORT loc_3_0F1D
        cmp    eax, 2
        je     SHORT loc_3_0F2E
        cmp    eax, 4
        je     SHORT loc_3_0F41
        jmp    SHORT loc_3_0F56
loc_3_0F1D:
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edx + 24h]
        or     al, 4
        or     ecx, eax
        push   ecx
        push   dword ptr [ebp - 8]
        push   edx
        jmp    SHORT loc_3_0F4B
loc_3_0F2E:
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 18h], 40h
        je     SHORT loc_3_0F3A
        or     cl, 8
loc_3_0F3A:
        push   ecx
        push   dword ptr [ebp - 8]
        push   eax
        jmp    SHORT loc_3_0F4B
loc_3_0F41:
        or     cl, 20h
        push   ecx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
loc_3_0F4B:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0F51:
        add    esp, 10h
        mov    ebx, eax
loc_3_0F56:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   0
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_3_52F8
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_3_0EF8
loc_3_0F71:
        test   byte ptr [esi + 14h], 2
        jne    SHORT loc_3_0F9F
        test   ebx, ebx
        jne    SHORT loc_3_0F9F
        mov    ax, word ptr [esi + 10h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    word ptr [ecx], ax
        push   dword ptr [ebp - 10h]
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_0F8E:
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_3_0F9F
        mov    eax, dword ptr [esi + 14h]
        and    al, 0feh
        mov    dword ptr [esi + 14h], eax
loc_3_0F9F:
        mov    dword ptr [ebp - 0ch], 0
        test   ebx, ebx
        jne    SHORT loc_3_100F
        mov    eax, dword ptr [esi + 14h]
        test   al, 10h
        jne    SHORT loc_3_0FC0
        or     al, 10h
        push   2
        mov    dword ptr [esi + 14h], eax
        call   sub_3_031E
        add    esp, 4
loc_3_0FC0:
        push   dword ptr [ebp + 0ch]
        call   sub_3_1281
        add    esp, 4
        cmp    eax, -1
        je     SHORT loc_3_0FE1
        test   eax, eax
        je     SHORT loc_3_1029
        push   0
        push   eax
        call   sub_3_5308
        add    esp, 8
        jmp    SHORT loc_3_1029
loc_3_0FE1:
        mov    al, byte ptr [edi + 1eh]
        cmp    al, 9
        jne    SHORT loc_3_0FF9
        push   0
        push   1e8480h
        call   sub_3_5308
        add    esp, 8
        jmp    SHORT loc_3_1029
loc_3_0FF9:
        cmp    al, 4
        jne    SHORT loc_3_1029
        push   0
        call   sub_3_1212
        push   eax
        call   sub_3_5308
        add    esp, 8
        jmp    SHORT loc_3_1029
loc_3_100F:
        push   3
        call   sub_3_031E
        mov    dword ptr [ebp - 0ch], 13h
        add    esp, 4
        jmp    SHORT loc_3_1029
loc_3_1022:
        mov    dword ptr [ebp - 0ch], 1
loc_3_1029:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 8], eax
        jne    SHORT loc_3_103A
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1037:
        add    esp, 4
loc_3_103A:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_3_1044
        or     byte ptr [edi + 0ah], 8
loc_3_1044:
        mov    eax, dword ptr [ebp - 0ch]
loc_3_1047:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_104E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        test   edi, edi
        je     SHORT loc_3_10A7
        mov    ebx, dword ptr [edi]
        push   1
        push   100000h
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_3_5310
        add    esp, 0ch
        push   esi
        push   edi
        call   sub_3_10AE
        add    esp, 8
        cmp    dword ptr [ebx + 8], esi
        jne    SHORT loc_3_1091
        or     byte ptr [ebx + 14h], 1
        cmp    dword ptr [ebp + 10h], 4
        jne    SHORT loc_3_1091
        xor    eax, eax
        mov    byte ptr [ebx + 48h], al
        mov    dword ptr [ebx + 8], eax
loc_3_1091:
        cmp    dword ptr [edi + 4], esi
        jne    SHORT loc_3_10A7
        cmp    dword ptr [ebp + 10h], 4
        jne    SHORT loc_3_10A7
        mov    dword ptr [edi + 4], 0
        and    byte ptr [edi + 0ah], 0fdh
loc_3_10A7:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_3_10AE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [esi]
        cmp    dword ptr [ebx + 8], edi
        jne    SHORT loc_3_10CE
        cmp    byte ptr [ebx + 30h], 2
        jae    NEAR PTR loc_3_1177
loc_3_10CE:
        and    byte ptr [esi + 0ah], 0e7h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_10D8:
        add    esp, 4
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_10E1:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_10EC
        or     byte ptr [ebx + 14h], 20h
loc_3_10EC:
        mov    eax, dword ptr [ebx + 40h]
        test   eax, eax
        je     SHORT loc_3_110A
        cmp    dword ptr [ebx + 8], edi
        jne    SHORT loc_3_110A
        push   0
        push   eax
        call   sub_3_52B8
        mov    dword ptr [ebx + 40h], 0
        add    esp, 8
loc_3_110A:
        push   2
        lea    eax, [ebp - 0ch]
        push   edi
        push   eax
        call   sub_3_5300
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_1177
        mov    eax, dword ptr [ebp - 0ch]
        xor    edi, edi
        push   edi
        lea    ecx, [ebp - 8]
        push   ecx
        lea    edx, [ebp - 4]
        push   edi
        mov    dword ptr [ebp - 4], eax
        push   eax
        push   edx
        call   sub_3_52F8
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_3_1177
loc_3_113C:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 1
        jb     SHORT loc_3_115E
        cmp    eax, 2
        jbe    SHORT loc_3_114E
        cmp    eax, 4
        jne    SHORT loc_3_115E
loc_3_114E:
        push   edi
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_115B:
        add    esp, 10h
loc_3_115E:
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edi
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_3_52F8
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_3_113C
loc_3_1177:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_1180:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        test   edi, edi
        je     SHORT loc_3_1193
        mov    dword ptr [edi], eax
loc_3_1193:
        mov    esi, dword ptr [ebp + 0ch]
        xor    eax, eax
        test   esi, esi
        je     SHORT loc_3_119F
        mov    dword ptr [esi + 8], eax
loc_3_119F:
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   80000002h
        call   sub_3_5328
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_120C
        test   edi, edi
        je     SHORT loc_3_11DD
        mov    dword ptr [ebp - 8], 4
        lea    ecx, [ebp - 8]
        push   ecx
        push   edi
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_3_5320
        add    esp, 18h
loc_3_11DD:
        test   esi, esi
        je     SHORT loc_3_1201
        mov    dword ptr [ebp - 8], 0ch
        lea    ecx, [ebp - 8]
        push   ecx
        push   esi
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_3_5320
        add    esp, 18h
loc_3_1201:
        push   dword ptr [ebp - 4]
        call   sub_3_5318
        add    esp, 4
loc_3_120C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_1212:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        mov    dword ptr [ebp - 4], 5dch
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   80000002h
        call   sub_3_5328
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_1267
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 0ch], 4
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_3_5320
        add    esp, 18h
        push   dword ptr [ebp - 8]
        call   sub_3_5318
        add    esp, 4
loc_3_1267:
        mov    eax, 2710h
        cmp    dword ptr [ebp - 4], eax
        jbe    SHORT loc_3_1274
        mov    dword ptr [ebp - 4], eax
loc_3_1274:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        imul   eax, eax, 3e8h
        pop    ebp
        ret
sub_3_1281:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   1
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 8], 4
        push   ecx
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   dword ptr [ebp + 8]
        call   sub_3_5298
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_3_12CE
        cmp    dword ptr [ebp - 8], 4
        jne    SHORT loc_3_12CE
        mov    eax, 2710h
        cmp    dword ptr [ebp - 4], eax
        jbe    SHORT loc_3_12C3
        mov    dword ptr [ebp - 4], eax
loc_3_12C3:
        mov    eax, dword ptr [ebp - 4]
        imul   eax, eax, 3e8h
        jmp    SHORT loc_3_12D3
loc_3_12CE:
        mov    eax, 0ffffffffh
loc_3_12D3:
        mov    esp, ebp
        pop    ebp
        ret
sub_3_12D7:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 58h
        mov    word ptr [ebp - 2], dx
        xor    eax, eax
        push   ebx
        mov    ecx, 9
        push   esi
        push   edi
        lea    edi, [ebp - 48h]
        rep stosd
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 46h], 14h
        mov    esi, dword ptr [eax + 0ch]
        mov    word ptr [ebp - 48h], 1
        cmp    dword ptr [esi + 30h], edx
        jne    NEAR PTR loc_3_163D
        lea    eax, [ebp - 18h]
        lea    ecx, [ebp - 8]
        push   eax
        push   ecx
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_131D:
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_3_1332
        mov    dword ptr [ebp - 8], 1000h
        mov    dword ptr [ebp - 18h], 0ffffffh
loc_3_1332:
        xor    edi, edi
        lea    ecx, [ebp - 0ch]
        push   edi
        push   dword ptr [esi + 4]
        push   ecx
        call   sub_3_52D8
        add    esp, 0ch
        lea    ecx, [ebp - 4ch]
        push   edi
        push   edi
        push   edi
        push   ecx
        push   dword ptr [ebp - 0ch]
        call   sub_3_52A8
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_3_1360
        cmp    dword ptr [ebp - 4ch], 4
        je     SHORT loc_3_13AF
loc_3_1360:
        xor    edi, edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], edi
        cmp    eax, edi
        je     SHORT loc_3_13AF
        cmp    eax, dword ptr [ebp - 18h]
        jae    SHORT loc_3_13AF
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 8], ecx
        jne    SHORT loc_3_13AF
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   ecx
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_138C:
        add    esp, 10h
        lea    eax, [esi + 30h]
        push   1
        push   4
        push   eax
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        push   dword ptr [esi + 4]
        call   sub_3_5330
        add    esp, 1ch
        jmp    NEAR PTR loc_3_163D
loc_3_13AF:
        lea    eax, [ebp - 58h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        call   sub_3_1180
        add    esp, 8
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 004h
        jne    SHORT loc_3_13F6
        cmp    dword ptr [ebp - 50h], 0
        je     SHORT loc_3_13F6
        mov    eax, dword ptr [ebp - 50h]
        mov    ecx, dword ptr [ebp - 58h]
        mov    edx, dword ptr [ebp - 54h]
        mov    dword ptr [ebp - 34h], eax
        lea    eax, [ebp - 48h]
        mov    dword ptr [ebp - 30h], ecx
        push   eax
        mov    dword ptr [ebp - 2ch], edx
        push   dword ptr [ebp - 0ch]
        push   esi
        call   sub_3_1662
        add    esp, 0ch
        jmp    NEAR PTR loc_3_163D
loc_3_13F6:
        push   1
        lea    eax, [ebp - 24h]
        push   eax
        lea    ecx, [ebp - 10h]
        mov    dword ptr [ebp - 24h], 4
        push   ecx
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   dword ptr [esi + 4]
        call   sub_3_5298
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_3_1446
        cmp    dword ptr [ebp - 24h], 4
        jne    SHORT loc_3_1446
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_3_1446
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 10h]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        add    eax, dword ptr [ebp - 10h]
        dec    eax
        db  0A3h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_3_1452
loc_3_1446:
        mov    dword ptr [ebp - 10h], 0
        db  0BFh, 00Ch, 000h, 000h, 000h
loc_3_1452:
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_3_15CA
        mov    word ptr [ebp - 1ah], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 10h]
        mov    word ptr [ebp - 1ch], ax
        cmp    dword ptr [edi], 0
        je     NEAR PTR loc_3_15CA
loc_3_1476:
        mov    ebx, dword ptr [edi]
loc_3_1478:
        cmp    dword ptr [edi + 4], ebx
        jbe    NEAR PTR loc_3_15BE
        cmp    ebx, dword ptr [ebp - 18h]
        jae    NEAR PTR loc_3_15BE
        test   ebx, ebx
        je     NEAR PTR loc_3_15BE
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        add    eax, ebx
        dec    eax
        push   eax
        push   ebx
        push   1
        call   sub_3_210B
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_3_15A8
        push   0
        push   dword ptr [ebp - 8]
        push   ebx
        call   sub_3_5340
        add    esp, 0ch
        cmp    eax, -1
        je     NEAR PTR loc_3_15A0
        push   eax
        push   dword ptr [ebp - 8]
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_14D1:
        add    esp, 10h
        lea    eax, [ebp - 1ch]
        push   eax
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_14E0:
        add    esp, 8
        mov    dword ptr [ebp - 14h], eax
        test   eax, eax
        jne    SHORT loc_3_152E
        cmp    word ptr [ebp - 1ah], 0
        je     SHORT loc_3_152E
        push   dword ptr [ebp + 8]
        call   sub_3_37AC
        mov    word ptr [ebp - 2], ax
        add    esp, 4
        mov    dword ptr [ebp - 20h], 0
loc_3_1507:
        push   dword ptr [ebp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_150F:
        add    esp, 4
        push   dword ptr [ebp + 8]
        call   sub_3_37AC
        add    esp, 4
        cmp    ax, word ptr [ebp - 2]
        jne    SHORT loc_3_152E
        inc    dword ptr [ebp - 20h]
        cmp    dword ptr [ebp - 20h], 8
        jl     SHORT loc_3_1507
        jmp    SHORT loc_3_1535
loc_3_152E:
        mov    dword ptr [ebp - 14h], 19h
loc_3_1535:
        push   0
        push   0
        push   0
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1541:
        add    esp, 10h
        cmp    dword ptr [ebp - 14h], 0
        jne    SHORT loc_3_15A0
        mov    eax, dword ptr [ebp - 8]
        lea    ecx, [ebp - 48h]
        push   ecx
        mov    dword ptr [ebp - 34h], eax
        add    eax, ebx
        mov    dword ptr [ebp - 30h], ebx
        push   dword ptr [ebp - 0ch]
        push   esi
        dec    eax
        mov    dword ptr [ebp - 2ch], eax
        call   sub_3_1662
        add    esp, 0ch
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [esi + 30h], 0
        je     SHORT loc_3_15A0
        push   dword ptr [ebp + 8]
        call   sub_3_37AC
        add    esp, 4
        cmp    ax, word ptr [ebp - 2]
        je     NEAR PTR loc_3_1644
        cmp    dword ptr [ebp - 14h], 0
        je     SHORT loc_3_159A
        push   0
        push   dword ptr [ebp - 14h]
        call   sub_3_52B8
        add    esp, 8
loc_3_159A:
        mov    ebx, dword ptr [edi + 4]
        sub    ebx, dword ptr [ebp - 8]
loc_3_15A0:
        add    ebx, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_3_1478
loc_3_15A8:
        mov    ebx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    ebx, eax
        dec    ecx
        not    ecx
        dec    ebx
        and    ebx, ecx
        cmp    eax, ebx
        jbe    NEAR PTR loc_3_1478
loc_3_15BE:
        add    edi, 0ch
        cmp    dword ptr [edi], 0
        jne    NEAR PTR loc_3_1476
loc_3_15CA:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_3_15F2
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 34h], eax
        mov    dword ptr [ebp - 30h], ecx
        add    eax, ecx
        lea    ecx, [ebp - 48h]
        dec    eax
        push   ecx
        push   dword ptr [ebp - 0ch]
        push   esi
        mov    dword ptr [ebp - 2ch], eax
        call   sub_3_1662
        add    esp, 0ch
loc_3_15F2:
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [eax + 0ch]
        add    edi, 30h
        cmp    dword ptr [edi], 0
        jne    SHORT loc_3_1623
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 18h]
        lea    edx, [ebp - 48h]
        mov    dword ptr [ebp - 34h], eax
        push   edx
        mov    dword ptr [ebp - 2ch], ecx
        push   dword ptr [ebp - 0ch]
        push   esi
        mov    dword ptr [ebp - 30h], 0c0000h
        call   sub_3_1662
        add    esp, 0ch
loc_3_1623:
        xor    eax, eax
        cmp    dword ptr [edi], eax
        jne    SHORT loc_3_163D
        push   eax
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        call   sub_3_5338
        add    esp, 10h
loc_3_163D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1644:
        push   1
        lea    eax, [esi + 30h]
        push   4
        push   eax
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   dword ptr [esi + 4]
        call   sub_3_5330
        add    esp, 1ch
        jmp    SHORT loc_3_163D
sub_3_1662:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 8], 1
        mov    dword ptr [esi + 10h], 0ffffffffh
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 1
        jbe    SHORT loc_3_1695
loc_3_1684:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_3_1695
        shl    dword ptr [ebp - 8], 1
        shl    dword ptr [esi + 10h], 1
        cmp    dword ptr [ebp - 8], eax
        jb     SHORT loc_3_1684
loc_3_1695:
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], edi
        test   edi, edi
        jne    SHORT loc_3_16B7
        push   0
        lea    eax, [ebp - 0ch]
        push   0
        push   eax
        call   sub_3_5358
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_3_17BE
loc_3_16B7:
        push   1
        lea    ecx, [ebp - 4]
        push   dword ptr [ebp - 0ch]
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        call   sub_3_52E0
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_3_17BE
        xor    ebx, ebx
        push   ebx
        mov    dword ptr [ebp - 8], ebx
        push   ebx
        push   1
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_16E3:
        add    esp, 4
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        push   dword ptr [ebp - 4]
        call   sub_3_5330
        add    esp, 1ch
        test   eax, eax
        jne    SHORT loc_3_171C
        push   ebx
        lea    eax, [ebp - 8]
        push   4
        push   eax
        push   3
        db  068h, 000h, 000h, 000h, 000h
        push   ebx
        push   dword ptr [ebp - 4]
        call   sub_3_5330
        add    esp, 1ch
        test   eax, eax
        je     SHORT loc_3_1740
loc_3_171C:
        push   0
        push   dword ptr [ebp - 4]
        call   sub_3_52B8
        add    esp, 8
        lea    ecx, [ebp - 4]
        push   1
        push   dword ptr [ebp - 0ch]
        push   0
        push   ecx
        call   sub_3_52E0
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_17BE
loc_3_1740:
        xor    ebx, ebx
        lea    ecx, [ebp - 10h]
        push   ebx
        push   ebx
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_3_5350
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_1787
        push   ebx
        lea    ecx, [ebp - 14h]
        push   24h
        push   esi
        push   1
        push   dword ptr [ebp - 10h]
        push   ecx
        call   sub_3_5348
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_3_1787
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_3_5280
        add    esp, 10h
        jmp    SHORT loc_3_178A
loc_3_1787:
        mov    esi, dword ptr [ebp + 8]
loc_3_178A:
        mov    eax, dword ptr [esi + 30h]
        test   eax, eax
        jne    SHORT loc_3_17A0
        push   0
        push   dword ptr [ebp - 4]
        call   sub_3_52B8
        add    esp, 8
        jmp    SHORT loc_3_17BE
loc_3_17A0:
        test   edi, edi
        jne    SHORT loc_3_17B9
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 34h]
        db  0A3h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esi + 38h]
        db  0A3h, 000h, 000h, 000h, 000h
loc_3_17B9:
        mov    eax, dword ptr [ebp - 4]
        jmp    SHORT loc_3_17C0
loc_3_17BE:
        xor    eax, eax
loc_3_17C0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_17C7:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0ch
        cmp    eax, 1
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 14h]
        je     SHORT loc_3_17F6
        cmp    eax, 2
        je     SHORT loc_3_185C
        cmp    eax, 3
        je     NEAR PTR loc_3_186A
        cmp    eax, 4
        je     SHORT loc_3_185C
        mov    eax, 1
        jmp    SHORT loc_3_186C
loc_3_17F6:
        push   2
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp + 10h]
        push   ecx
        call   sub_3_5300
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_186C
        mov    eax, dword ptr [ebp - 8]
        push   0
        lea    ecx, [ebp - 0ch]
        lea    edx, [ebp - 4]
        push   ecx
        mov    dword ptr [ebp - 4], eax
        push   1
        push   eax
        push   edx
        call   sub_3_52F8
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_3_186A
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    edi, dword ptr [eax + 4]
        mov    ebx, dword ptr [eax + 8]
        sub    ebx, edi
        inc    ebx
        push   ebx
        push   edi
        call   sub_3_5340
        add    esp, 0ch
        cmp    eax, -1
        jne    SHORT loc_3_184E
        mov    eax, 13h
        jmp    SHORT loc_3_186C
loc_3_184E:
        push   eax
        push   ebx
        push   edi
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1857:
        add    esp, 10h
        jmp    SHORT loc_3_186A
loc_3_185C:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1867:
        add    esp, 10h
loc_3_186A:
        xor    eax, eax
loc_3_186C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1873:
        mov    eax, 1
        ret
sub_3_1879:
        push   esi
        push   0
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi + 0ch]
        or     byte ptr [esi + 14h], 1
        push   dword ptr [eax + 4]
        call   sub_3_52C8
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_189F
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_189C:
        add    esp, 4
loc_3_189F:
        pop    esi
        ret
loc_3_18A1:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 10h]
        call   sub_3_1F79
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     NEAR PTR loc_3_1AE4
        mov    esi, dword ptr [ebp + 14h]
        cmp    esi, dword ptr [ebp + 10h]
        jne    NEAR PTR loc_3_1974
        cmp    dword ptr [ebp + 8], 5
        jne    NEAR PTR loc_3_1AE4
        xor    bl, bl
        mov    esi, dword ptr [edi + 18h]
        test   esi, esi
        je     SHORT loc_3_1952
loc_3_18DE:
        cmp    byte ptr [edi + 30h], bl
        jbe    SHORT loc_3_1952
        mov    dword ptr [ebp - 8], 0
        mov    ax, word ptr [edi + 10h]
        mov    word ptr [ebp - 10h], ax
        lea    eax, [ebp - 10h]
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_18FC:
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_193E
        test   byte ptr [ebp - 0eh], 80h
        je     SHORT loc_3_193E
        test   byte ptr [edi + 14h], 8
        je     SHORT loc_3_193E
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1915:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_3_1959
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 10h]
        push   esi
        call   sub_3_4058
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_3_1959
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 4], eax
        jmp    SHORT loc_3_194B
loc_3_193E:
        push   0
        push   dword ptr [esi + 4]
        call   sub_3_52B8
        add    esp, 8
loc_3_194B:
        inc    bl
        add    esi, 28h
        jne    SHORT loc_3_18DE
loc_3_1952:
        xor    eax, eax
        jmp    NEAR PTR loc_3_1AE9
loc_3_1959:
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_3_196C
        push   0
        push   dword ptr [ebp - 8]
        call   sub_3_52B8
        add    esp, 8
loc_3_196C:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_3_1AE9
loc_3_1974:
        push   0
        lea    eax, [ebp - 14h]
        push   esi
        push   eax
        call   sub_3_52D8
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, dword ptr [ebp - 14h]
        jne    NEAR PTR loc_3_1AE4
        push   esi
        push   edi
        call   sub_3_1FCF
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_3_19A9
        mov    eax, 13h
        jmp    NEAR PTR loc_3_1AE9
loc_3_19A9:
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     SHORT loc_3_19CE
        cmp    eax, 1
        je     SHORT loc_3_19DD
        cmp    eax, 2
        je     SHORT loc_3_19EC
        cmp    eax, 4
        je     SHORT loc_3_19EC
        cmp    eax, 6
        je     SHORT loc_3_19FE
        cmp    eax, 8
        je     SHORT loc_3_1A0E
        jmp    NEAR PTR loc_3_1AE4
loc_3_19CE:
        push   esi
        push   edi
        call   sub_3_0CDF
        add    esp, 8
        jmp    NEAR PTR loc_3_1AE9
loc_3_19DD:
        push   esi
        push   edi
        call   sub_3_0E3D
        add    esp, 8
        jmp    NEAR PTR loc_3_1AE9
loc_3_19EC:
        push   dword ptr [ebp + 8]
        push   esi
        push   edi
        call   sub_3_104E
        add    esp, 0ch
        jmp    NEAR PTR loc_3_1AE9
loc_3_19FE:
        push   edi
        call   sub_3_4106
        add    esp, 4
        xor    eax, eax
        jmp    NEAR PTR loc_3_1AE9
loc_3_1A0E:
        push   0
        db  068h, 000h, 000h, 000h, 000h
        call   sub_3_1180
        add    esp, 8
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 020h
        jne    NEAR PTR loc_3_1AE4
        mov    ecx, dword ptr [ebp + 0ch]
        mov    eax, ecx
        and    eax, 7fffffffh
        sub    eax, 5
        cmp    eax, 0ch
        ja     NEAR PTR loc_3_1AE4
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_3_1A47:
        test   byte ptr [edi + 0ah], 10h
        je     SHORT loc_3_1A57
        push   1
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1A55:
        jmp    SHORT loc_3_1A5E
loc_3_1A57:
        push   esi
        push   edi
        call   sub_3_10AE
loc_3_1A5E:
        add    esp, 8
        xor    eax, eax
        jmp    NEAR PTR loc_3_1AE9
loc_3_1A68:
        push   ecx
        push   esi
        push   edi
        call   sub_3_03DB
        add    esp, 0ch
        jmp    SHORT loc_3_1AE9
loc_3_1A75:
        mov    eax, dword ptr [edi + 8]
        and    eax, 0ffefffffh
        mov    dword ptr [edi + 8], eax
        test   eax, 80000h
        jne    SHORT loc_3_1A94
        push   ecx
        push   esi
        push   edi
        call   sub_3_03DB
        add    esp, 0ch
        jmp    SHORT loc_3_1AE9
loc_3_1A94:
        xor    eax, eax
        jmp    SHORT loc_3_1AE9
loc_3_1A98:
        xor    eax, eax
        lea    ebx, [edi + 8]
        mov    edx, dword ptr [ebx]
        test   edx, 80000h
        jne    SHORT loc_3_1ABA
        and    edx, 0ffefffffh
        push   ecx
        push   esi
        mov    dword ptr [ebx], edx
        push   edi
        call   sub_3_03DB
        add    esp, 0ch
loc_3_1ABA:
        test   eax, eax
        jne    SHORT loc_3_1AE9
        or     byte ptr [ebx + 2], 10h
        jmp    SHORT loc_3_1AE9
loc_3_1AC4:
        mov    eax, dword ptr [edi + 8]
        and    eax, 0ffefffffh
        mov    dword ptr [edi + 8], eax
        test   eax, 80000h
        je     SHORT loc_3_1AE0
        push   esi
        push   edi
        call   sub_3_10AE
        add    esp, 8
loc_3_1AE0:
        xor    eax, eax
        jmp    SHORT loc_3_1AE9
loc_3_1AE4:
        mov    eax, 1
loc_3_1AE9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_3_1B24:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 14h
        cmp    eax, 1
        push   ebx
        push   esi
        push   edi
        jbe    SHORT loc_3_1B65
        cmp    eax, 2
        jb     SHORT loc_3_1B5B
        cmp    eax, 3
        jbe    NEAR PTR loc_3_1C2A
        cmp    eax, 4
        je     NEAR PTR loc_3_1C2E
        cmp    eax, 5
        je     NEAR PTR loc_3_1C2A
        cmp    eax, 6
        je     SHORT loc_3_1B65
loc_3_1B5B:
        mov    esi, 1
        jmp    NEAR PTR loc_3_1C33
loc_3_1B65:
        xor    esi, esi
        push   6
        push   dword ptr [ebp + 10h]
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_3_5300
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_1B9B
        push   esi
        lea    eax, [ebp - 14h]
        push   eax
        lea    edx, [ebp - 10h]
        push   4
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_3_52F8
        add    esp, 14h
        test   eax, eax
        je     SHORT loc_3_1BCB
        jmp    NEAR PTR loc_3_1C2E
loc_3_1B9B:
        push   2
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp + 10h]
        push   ecx
        call   sub_3_5300
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_1C2E
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        lea    edx, [ebp - 10h]
        push   4
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_3_52F8
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_3_1C2E
loc_3_1BCB:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 14h]
        mov    bx, word ptr [eax + 2]
        mov    edi, dword ptr [ecx]
        test   edi, edi
        je     SHORT loc_3_1C33
loc_3_1BDB:
        mov    eax, dword ptr [edi + 0ch]
        push   0
        push   0
        lea    ecx, [ebp - 4]
        push   4
        mov    dword ptr [ebp - 4], eax
        push   eax
        push   ecx
        call   sub_3_52F8
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_3_1C22
        cmp    dword ptr [ebp + 8], 0
        mov    eax, dword ptr [ebp - 4]
        jne    SHORT loc_3_1C1C
        mov    dword ptr [ebp - 0ch], 1
        movzx  edx, word ptr [eax + 4]
        mov    cl, bl
        shl    dword ptr [ebp - 0ch], cl
        test   dword ptr [ebp - 0ch], edx
        je     SHORT loc_3_1C2E
        mov    word ptr [eax + 2], bx
        jmp    SHORT loc_3_1C22
loc_3_1C1C:
        cmp    word ptr [eax + 2], bx
        jne    SHORT loc_3_1C2E
loc_3_1C22:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_3_1BDB
        jmp    SHORT loc_3_1C33
loc_3_1C2A:
        xor    esi, esi
        jmp    SHORT loc_3_1C33
loc_3_1C2E:
        mov    esi, 13h
loc_3_1C33:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_1C3C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 28h]
        test   ecx, ecx
        je     SHORT loc_3_1C66
        test   byte ptr [eax + 8], 20h
        je     SHORT loc_3_1C66
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        call   ecx
        add    esp, 14h
        jmp    SHORT loc_3_1C6B
loc_3_1C66:
        mov    eax, 1
loc_3_1C6B:
        pop    ebp
        ret
sub_3_1C6D:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 14h]
        mov    eax, dword ptr [edi]
        test   al, 1
        je     SHORT loc_3_1CCE
        mov    cx, word ptr [esi + 10h]
        and    al, 0f9h
        mov    word ptr [ebp - 8], cx
        mov    dword ptr [edi], eax
        lea    ecx, [ebp - 8]
        push   ecx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1C98:
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_1CCE
        test   byte ptr [ebp - 6], 80h
        je     SHORT loc_3_1CCE
        test   byte ptr [edi], 8
        je     SHORT loc_3_1CCE
        mov    ecx, esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1CB1:
        test   eax, eax
        jne    SHORT loc_3_1CC4
        push   dword ptr [esi + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1CBD:
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_1CCE
loc_3_1CC4:
        mov    eax, 13h
        jmp    NEAR PTR loc_3_1D65
loc_3_1CCE:
        push   dword ptr [ebp + 18h]
        mov    edi, dword ptr [ebp + 14h]
        push   edi
        push   edi
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [esi + 0ch]
        call   sub_3_1C3C
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_3_1D65
        mov    bl, 1
        push   0
        push   edi
        lea    eax, [ebp - 28h]
        push   eax
        call   sub_3_5368
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_1D26
loc_3_1D00:
        movzx  eax, bl
        shl    eax, 2
        push   0
        push   dword ptr [ebp + eax - 2ch]
        lea    edx, [ebp + eax - 28h]
        push   edx
        call   sub_3_5360
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_3_1D2D
        inc    bl
        cmp    bl, 8
        jb     SHORT loc_3_1D00
        jmp    SHORT loc_3_1D2D
loc_3_1D26:
        mov    dword ptr [ebp - 28h], 0
loc_3_1D2D:
        push   ebx
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1D34:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_1D42
        mov    eax, 13h
        jmp    SHORT loc_3_1D65
loc_3_1D42:
        xor    cl, cl
        mov    edi, dword ptr [esi + 18h]
        test   edi, edi
        je     SHORT loc_3_1D63
        mov    dl, byte ptr [esi + 30h]
loc_3_1D4E:
        cmp    cl, dl
        jae    SHORT loc_3_1D63
        movzx  eax, cl
        inc    cl
        mov    ebx, dword ptr [ebp + eax*4 - 28h]
        mov    dword ptr [edi + 4], ebx
        add    edi, 28h
        jne    SHORT loc_3_1D4E
loc_3_1D63:
        xor    eax, eax
loc_3_1D65:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_1D6C:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        mov    esi, dword ptr [edi]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [esi + 0ch]
        call   sub_3_1C3C
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_3_1DA1
        cmp    eax, 1
        jne    NEAR PTR loc_3_1F0F
loc_3_1DA1:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 1
        je     SHORT loc_3_1DCE
        cmp    eax, 2
        je     NEAR PTR loc_3_1ECB
        cmp    eax, 4
        je     NEAR PTR loc_3_1EC4
        cmp    eax, 6
        je     NEAR PTR loc_3_1F0B
        mov    edi, 1
        jmp    NEAR PTR loc_3_1F0D
loc_3_1DCE:
        lea    eax, [ebp - 1ah]
        lea    ecx, [ebp - 1bh]
        push   eax
        lea    edx, [ebp - 1ch]
        push   ecx
        push   edx
        push   edi
        call   sub_3_4C20
        add    esp, 10h
        lea    ecx, [ebp - 2]
        lea    eax, [ebp - 3]
        lea    edx, [ebp - 1]
        push   ecx
        push   eax
        push   edx
        push   dword ptr [edi]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1DF6:
        add    esp, 10h
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    NEAR PTR loc_3_1E94
        cmp    byte ptr [ebp - 1ch], 0
        jne    SHORT loc_3_1E10
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebp - 1ch], al
loc_3_1E10:
        cmp    byte ptr [ebp - 1bh], 0
        jne    SHORT loc_3_1E1C
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [ebp - 1bh], al
loc_3_1E1C:
        cmp    byte ptr [ebp - 1ah], 0
        jne    SHORT loc_3_1E28
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [ebp - 1ah], al
loc_3_1E28:
        mov    ax, word ptr [esi + 10h]
        xor    ebx, ebx
        mov    word ptr [ebp - 20h], ax
        mov    byte ptr [ebp - 14h], bl
        mov    word ptr [ebp - 1eh], bx
        mov    byte ptr [ebp - 13h], bl
        mov    byte ptr [ebp - 12h], bl
        mov    byte ptr [ebp - 11h], bl
        mov    byte ptr [ebp - 10h], bl
        mov    byte ptr [ebp - 0fh], bl
        lea    eax, [ebp - 20h]
        mov    dword ptr [ebp - 0eh], ebx
        mov    byte ptr [ebp - 19h], 4
        mov    dword ptr [ebp - 18h], 0ffffffffh
        push   eax
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1E60:
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    eax, ebx
        jne    SHORT loc_3_1E6E
        and    byte ptr [esi + 14h], 0feh
loc_3_1E6E:
        mov    al, byte ptr [ebp - 1ch]
        cmp    byte ptr [ebp - 1], al
        je     SHORT loc_3_1E94
        push   dword ptr [ebp + 1ch]
        push   dword ptr [ebp + 18h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [esi + 0ch]
        call   sub_3_1C3C
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_3_1F0F
loc_3_1E94:
        xor    edi, edi
        cmp    dword ptr [ebp - 8], edi
        jne    SHORT loc_3_1EB3
        mov    eax, dword ptr [esi + 14h]
        test   al, 10h
        jne    SHORT loc_3_1F0D
        or     al, 10h
        push   2
        mov    dword ptr [esi + 14h], eax
        call   sub_3_031E
        add    esp, 4
        jmp    SHORT loc_3_1F0D
loc_3_1EB3:
        push   3
        mov    edi, 13h
        call   sub_3_031E
        add    esp, 4
        jmp    SHORT loc_3_1F0D
loc_3_1EC4:
        mov    dword ptr [edi + 4], 0
loc_3_1ECB:
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1ED1:
        add    esp, 4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_1EDA:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_1EE5
        or     byte ptr [esi + 14h], 20h
loc_3_1EE5:
        xor    cl, cl
        mov    eax, dword ptr [esi + 18h]
        test   eax, eax
        je     SHORT loc_3_1F02
        mov    dl, byte ptr [esi + 30h]
loc_3_1EF1:
        cmp    cl, dl
        jae    SHORT loc_3_1F02
        cmp    dword ptr [eax + 4], 0
        jne    SHORT loc_3_1F02
        inc    cl
        add    eax, 28h
        jne    SHORT loc_3_1EF1
loc_3_1F02:
        cmp    byte ptr [esi + 30h], cl
        ja     SHORT loc_3_1F0B
        or     byte ptr [esi + 14h], 1
loc_3_1F0B:
        xor    edi, edi
loc_3_1F0D:
        mov    eax, edi
loc_3_1F0F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_1F16:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_3_1F2E
        mov    ecx, dword ptr [esp + 4]
loc_3_1F23:
        cmp    dword ptr [eax + 4], ecx
        je     SHORT loc_3_1F30
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_3_1F23
loc_3_1F2E:
        xor    eax, eax
loc_3_1F30:
        ret
sub_3_1F31:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        push   0
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0ch]
        lea    eax, [ebp - 4]
        push   dword ptr [esi + 4]
        push   edi
        push   eax
        call   sub_3_5370
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_1F6C
        mov    eax, dword ptr [esi + 10h]
        test   eax, eax
        je     SHORT loc_3_1F6C
loc_3_1F5C:
        cmp    dword ptr [eax + 8], edi
        jne    SHORT loc_3_1F66
        cmp    eax, dword ptr [ebp - 4]
        je     SHORT loc_3_1F74
loc_3_1F66:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_3_1F5C
loc_3_1F6C:
        xor    eax, eax
loc_3_1F6E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1F74:
        mov    eax, dword ptr [ebp - 4]
        jmp    SHORT loc_3_1F6E
sub_3_1F79:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   esi
        push   eax
        call   sub_3_5370
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_1FC3
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_3_1FC3
loc_3_1FA0:
        mov    ecx, dword ptr [eax + 10h]
        test   ecx, ecx
        je     SHORT loc_3_1FBD
loc_3_1FA7:
        cmp    ecx, dword ptr [ebp - 4]
        jne    SHORT loc_3_1FB7
        cmp    dword ptr [ecx + 8], esi
        jne    SHORT loc_3_1FB7
        cmp    byte ptr [ecx + 30h], 2
        jae    SHORT loc_3_1FCA
loc_3_1FB7:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_3_1FA7
loc_3_1FBD:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_3_1FA0
loc_3_1FC3:
        xor    eax, eax
loc_3_1FC5:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_3_1FCA:
        mov    eax, dword ptr [ebp - 4]
        jmp    SHORT loc_3_1FC5
sub_3_1FCF:
        push   ebx
        xor    dl, dl
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 18h]
        test   eax, eax
        je     SHORT loc_3_1FF4
        mov    bl, byte ptr [ecx + 30h]
        mov    ecx, dword ptr [esp + 0ch]
loc_3_1FE4:
        cmp    dl, bl
        jae    SHORT loc_3_1FF4
        cmp    dword ptr [eax + 4], ecx
        je     SHORT loc_3_1FF6
        inc    dl
        add    eax, 28h
        jne    SHORT loc_3_1FE4
loc_3_1FF4:
        xor    eax, eax
loc_3_1FF6:
        pop    ebx
        ret
sub_3_1FF8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        test   edi, edi
        je     SHORT loc_3_2046
        mov    ecx, dword ptr [esp + 14h]
        mov    esi, dword ptr [esp + 18h]
loc_3_200E:
        test   ecx, ecx
        je     SHORT loc_3_2016
        cmp    edi, ecx
        jne    SHORT loc_3_2040
loc_3_2016:
        mov    ebp, dword ptr [edi + 10h]
        test   ebp, ebp
        je     SHORT loc_3_2040
loc_3_201D:
        xor    bl, bl
        mov    eax, dword ptr [ebp + 18h]
        test   eax, eax
        je     SHORT loc_3_2039
        mov    dl, byte ptr [ebp + 30h]
loc_3_2029:
        cmp    bl, dl
        jae    SHORT loc_3_2039
        cmp    dword ptr [eax + 4], esi
        je     SHORT loc_3_2048
        inc    bl
        add    eax, 28h
        jne    SHORT loc_3_2029
loc_3_2039:
        mov    ebp, dword ptr [ebp]
        test   ebp, ebp
        jne    SHORT loc_3_201D
loc_3_2040:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_3_200E
loc_3_2046:
        xor    eax, eax
loc_3_2048:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_204D:
        push   ebx
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   esi
        mov    ecx, edx
        push   edi
        mov    esi, 0ffffffffh
        test   edx, edx
        je     SHORT loc_3_207E
loc_3_2061:
        mov    edi, dword ptr [ecx + 10h]
        test   edi, edi
        je     SHORT loc_3_2078
loc_3_2068:
        movzx  eax, word ptr [edi + 10h]
        cmp    eax, esi
        jle    SHORT loc_3_2072
        mov    esi, eax
loc_3_2072:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_3_2068
loc_3_2078:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_3_2061
loc_3_207E:
        inc    esi
        lea    eax, [esi*4]
        cmp    eax, dword ptr [esp + 14h]
        jbe    SHORT loc_3_2094
        or     esi, 10000h
        jmp    SHORT loc_3_2105
loc_3_2094:
        mov    ebx, dword ptr [esp + 10h]
        test   esi, esi
        jle    SHORT loc_3_20A4
        mov    edi, ebx
        xor    eax, eax
        mov    ecx, esi
        rep stosd
loc_3_20A4:
        test   edx, edx
        je     SHORT loc_3_2105
loc_3_20A8:
        mov    ecx, dword ptr [edx + 10h]
        test   ecx, ecx
        je     SHORT loc_3_20FF
loc_3_20AF:
        mov    eax, dword ptr [ecx + 8]
        test   eax, eax
        jne    SHORT loc_3_20C0
        mov    eax, dword ptr [ecx + 18h]
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        je     SHORT loc_3_20C7
loc_3_20C0:
        movzx  edi, word ptr [ecx + 10h]
        mov    dword ptr [ebx + edi*4], eax
loc_3_20C7:
        cmp    byte ptr [ecx + 48h], 0
        jne    SHORT loc_3_20F9
        test   byte ptr [ecx + 14h], 2
        je     SHORT loc_3_20F9
        mov    eax, dword ptr [ecx + 2ch]
        test   eax, eax
        jne    SHORT loc_3_20E1
        mov    eax, dword ptr [ecx + 24h]
        test   eax, eax
        je     SHORT loc_3_20F9
loc_3_20E1:
        cmp    dword ptr [eax + 2ch], 0
        jne    SHORT loc_3_20EF
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_3_20E1
        jmp    SHORT loc_3_20F9
loc_3_20EF:
        movzx  edi, word ptr [ecx + 10h]
        mov    eax, dword ptr [eax + 2ch]
        mov    dword ptr [ebx + edi*4], eax
loc_3_20F9:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_3_20AF
loc_3_20FF:
        mov    edx, dword ptr [edx]
        test   edx, edx
        jne    SHORT loc_3_20A8
loc_3_2105:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_210B:
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        lea    edi, [esi + eax - 1]
        cmp    dword ptr [ebp + 10h], edi
        jb     SHORT loc_3_217D
        push   0
        lea    ecx, [ebp - 10h]
        push   dword ptr [ebp + 8]
        push   0
        push   4
        push   ecx
        call   sub_3_5388
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_3_217D
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        push   dword ptr [ebp - 10h]
        call   sub_3_5380
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_3_217D
loc_3_2159:
        cmp    dword ptr [ebp - 8], edi
        ja     SHORT loc_3_2188
        cmp    dword ptr [ebp - 4], esi
        jae    SHORT loc_3_218C
        push   0
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0ch]
        push   edx
        call   sub_3_5378
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_3_2159
loc_3_217D:
        mov    eax, 0ffffffffh
loc_3_2182:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_3_2188:
        xor    eax, eax
        jmp    SHORT loc_3_2182
loc_3_218C:
        cmp    dword ptr [ebp - 4], -1
        je     SHORT loc_3_217D
        mov    eax, dword ptr [ebp - 4]
        inc    eax
        jmp    SHORT loc_3_2182
sub_3_2198:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 40h], ecx
        jne    SHORT loc_3_221B
        push   ecx
        mov    eax, dword ptr [edi + 0ch]
        push   dword ptr [eax + 4]
        push   ecx
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_3_52E0
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_3_21CD
        mov    eax, 13h
        jmp    NEAR PTR loc_3_23AA
loc_3_21CD:
        push   2
        xor    esi, esi
        push   esi
        lea    ecx, [ebp - 18h]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_3_5350
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_223C
        push   esi
        push   esi
        push   esi
        push   dword ptr [ebp - 4]
        call   sub_3_5280
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_223C
        push   esi
        lea    ecx, [ebp - 1ch]
        push   dword ptr [ebp - 4]
        push   ecx
        lea    edx, [ebp - 10h]
        push   edx
        call   sub_3_5398
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_223C
        test   byte ptr [ebp - 10h], 8
        je     SHORT loc_3_223C
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi + 40h], eax
loc_3_221B:
        push   0
        lea    eax, [ebp - 4]
        push   dword ptr [edi + 40h]
        push   0
        push   eax
        call   sub_3_52E0
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_3_2253
        mov    eax, 13h
        jmp    NEAR PTR loc_3_23AA
loc_3_223C:
        push   0
        push   dword ptr [ebp - 4]
        call   sub_3_52B8
        add    esp, 8
        mov    eax, 13h
        jmp    NEAR PTR loc_3_23AA
loc_3_2253:
        xor    ebx, ebx
        lea    ecx, [ebp - 0ch]
        mov    dword ptr [ebp - 14h], 13h
        push   ebx
        push   ebx
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_3_5350
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_3_239A
        push   ebx
        mov    esi, dword ptr [ebp + 10h]
        push   dword ptr [ebp + 14h]
        push   esi
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 0ch]
        lea    edx, [ebp - 8]
        push   edx
        call   sub_3_5348
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_3_239A
        push   ebx
        push   ebx
        push   ebx
        push   dword ptr [ebp - 4]
        call   sub_3_5280
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_3_239A
        push   ebx
        lea    ecx, [ebp - 1ch]
        push   dword ptr [ebp - 4]
        push   ecx
        lea    edx, [ebp - 10h]
        push   edx
        call   sub_3_5398
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_3_239A
        test   byte ptr [ebp - 10h], 8
        je     NEAR PTR loc_3_239A
        push   2
        lea    ecx, [ebp - 18h]
        push   dword ptr [edi + 40h]
        push   ecx
        call   sub_3_5300
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_3_239A
        push   2
        lea    ecx, [ebp - 0ch]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_3_5300
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_3_239A
        push   ebx
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 8]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 0ch]
        push   eax
        call   sub_3_52F8
        add    esp, 14h
        test   eax, eax
        jne    SHORT loc_3_239A
        push   ebx
        lea    ecx, [ebp - 24h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 18h]
        push   ecx
        call   sub_3_5348
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_3_239A
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 1
        je     SHORT loc_3_2351
        cmp    eax, 2
        je     SHORT loc_3_2362
        cmp    eax, 4
        je     SHORT loc_3_2377
        jmp    SHORT loc_3_2382
loc_3_2351:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi + 8], edx
        jmp    SHORT loc_3_2382
loc_3_2362:
        mov    eax, dword ptr [ebp - 8]
        mov    cx, word ptr [eax + 4]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [esi + 4], cx
        mov    word ptr [esi + 6], ax
        jmp    SHORT loc_3_2382
loc_3_2377:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 2]
        mov    word ptr [esi + 2], ax
loc_3_2382:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_3_5390
        mov    dword ptr [ebp - 14h], 0
        add    esp, 0ch
loc_3_239A:
        push   0
        push   dword ptr [ebp - 4]
        call   sub_3_52B8
        add    esp, 8
        mov    eax, dword ptr [ebp - 14h]
loc_3_23AA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_23B1:
        push   18h
        push   dword ptr [esp + 0ch]
        push   2
        push   dword ptr [esp + 10h]
        call   sub_3_2198
        add    esp, 10h
        ret
sub_3_23C6:
        push   8
        push   dword ptr [esp + 0ch]
        push   4
        push   dword ptr [esp + 10h]
        call   sub_3_2198
        add    esp, 10h
        ret
sub_3_23DB:
        push   24h
        push   dword ptr [esp + 0ch]
        push   1
        push   dword ptr [esp + 10h]
        call   sub_3_2198
        add    esp, 10h
        ret
sub_3_23F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], 13h
        push   esi
        push   edi
        xor    esi, esi
        cmp    dword ptr [eax + 40h], esi
        je     NEAR PTR loc_3_257C
        push   2
        lea    edx, [ebp - 1ch]
        push   dword ptr [eax + 40h]
        push   edx
        call   sub_3_5300
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_3_257C
        mov    eax, dword ptr [ebp - 1ch]
        push   esi
        lea    ecx, [ebp - 10h]
        mov    dword ptr [ebp - 4], eax
        push   ecx
        lea    ecx, [ebp - 4]
        push   dword ptr [ebp + 0ch]
        push   eax
        push   ecx
        call   sub_3_52F8
        add    esp, 14h
        test   eax, eax
        jne    NEAR PTR loc_3_257C
        mov    edi, dword ptr [ebp + 10h]
        xor    ebx, ebx
loc_3_244F:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    eax, 1
        je     SHORT loc_3_2463
        cmp    eax, 2
        je     SHORT loc_3_2476
        cmp    eax, 4
        je     SHORT loc_3_248D
        jmp    SHORT loc_3_249F
loc_3_2463:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        cmp    dword ptr [edi + 4], ecx
        jne    SHORT loc_3_249F
        mov    eax, dword ptr [eax + 8]
        cmp    dword ptr [edi + 8], eax
        jmp    SHORT loc_3_2498
loc_3_2476:
        mov    eax, dword ptr [ebp - 4]
        mov    cx, word ptr [eax + 4]
        cmp    word ptr [edi + 4], cx
        jne    SHORT loc_3_249F
        mov    ax, word ptr [eax + 6]
        cmp    word ptr [edi + 6], ax
        jmp    SHORT loc_3_2498
loc_3_248D:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2]
        cmp    word ptr [edi + 2], ax
loc_3_2498:
        jne    SHORT loc_3_249F
        mov    esi, 1
loc_3_249F:
        mov    dword ptr [ebp - 8], ebx
        test   esi, esi
        je     NEAR PTR loc_3_2535
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        lea    edx, [ebp - 8]
        push   dword ptr [eax + 40h]
        push   ebx
        push   edx
        call   sub_3_52E0
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_2535
        push   2
        lea    ecx, [ebp - 14h]
        push   ebx
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_3_5350
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_2535
        push   ebx
        push   ebx
        push   ebx
        push   dword ptr [ebp - 8]
        call   sub_3_5280
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_2535
        push   ebx
        lea    ecx, [ebp - 20h]
        push   dword ptr [ebp - 8]
        push   ecx
        lea    edx, [ebp - 18h]
        push   edx
        call   sub_3_5398
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_2535
        test   byte ptr [ebp - 18h], 8
        je     SHORT loc_3_2535
        push   ebx
        lea    ecx, [ebp - 24h]
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 14h]
        push   ecx
        call   sub_3_5348
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_3_2535
        push   ebx
        push   dword ptr [ebp - 8]
        call   sub_3_52B8
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_2567
loc_3_2535:
        cmp    dword ptr [ebp - 8], ebx
        je     SHORT loc_3_2546
        push   ebx
        push   dword ptr [ebp - 8]
        call   sub_3_52B8
        add    esp, 8
loc_3_2546:
        push   ebx
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 4]
        push   dword ptr [ebp + 0ch]
        push   dword ptr [ebp - 4]
        push   eax
        call   sub_3_52F8
        add    esp, 14h
        test   eax, eax
        je     NEAR PTR loc_3_244F
        jmp    SHORT loc_3_257C
loc_3_2567:
        xor    esi, esi
        lea    ecx, [ebp - 4]
        push   esi
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_3_5390
        add    esp, 0ch
        mov    dword ptr [ebp - 0ch], esi
loc_3_257C:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_2586:
        push   18h
        push   dword ptr [esp + 0ch]
        push   2
        push   dword ptr [esp + 10h]
        call   sub_3_23F0
        add    esp, 10h
        ret
sub_3_259B:
        push   8
        push   dword ptr [esp + 0ch]
        push   4
        push   dword ptr [esp + 10h]
        call   sub_3_23F0
        add    esp, 10h
        ret
sub_3_25B0:
        push   24h
        push   dword ptr [esp + 0ch]
        push   1
        push   dword ptr [esp + 10h]
        call   sub_3_23F0
        add    esp, 10h
        ret
sub_3_25C5:
        push   ebx
        push   esi
        push   edi
        push   37h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_25CF:
        add    esp, 4
        mov    esi, eax
        mov    eax, 19h
        test   esi, esi
        je     SHORT loc_3_262F
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 10h]
        push   ebx
        push   esi
        lea    eax, [edi + 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_25EE:
        mov    ax, word ptr [edi + 10h]
        mov    ecx, dword ptr [esp + 24h]
        mov    word ptr [esi + 4], ax
        add    esp, 0ch
        mov    word ptr [esi + 6], bx
        mov    eax, dword ptr [esp + 14h]
        mov    byte ptr [esi + 8], bl
        mov    byte ptr [esi + 9], 1
        mov    dl, byte ptr [ecx + 0eh]
        mov    byte ptr [esi + 0ah], dl
        mov    dword ptr [esi + 0ch], ebx
        mov    dword ptr [esi + 10h], eax
        mov    ebx, dword ptr [ecx + 4]
        mov    dword ptr [esi + 14h], ebx
        mov    dx, word ptr [ecx + 2]
        mov    word ptr [esi + 18h], dx
        mov    ax, word ptr [ecx]
        mov    word ptr [esi + 1ah], ax
        xor    eax, eax
loc_3_262F:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_2633:
        mov    ecx, dword ptr [esp + 4]
        lea    edx, [ecx + 0ch]
        mov    eax, dword ptr [edx]
        mov    eax, dword ptr [eax + 18h]
        db  0A3h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [edx]
        mov    eax, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Dh
        db  0A3h, 00Ah, 000h, 000h, 000h
        mov    edx, dword ptr [edx]
        mov    al, byte ptr [edx + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [ecx + 12h]
        db  088h, 00Dh, 002h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_266D:
        test   eax, eax
        mov    al, 0
        jne    SHORT loc_3_2678
        db  0A0h, 004h, 000h, 000h, 000h
loc_3_2678:
        ret
sub_3_2679:
        mov    ecx, dword ptr [esp + 4]
        lea    edx, [ecx + 0ch]
        mov    eax, dword ptr [edx]
        mov    eax, dword ptr [eax + 18h]
        db  0A3h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [edx]
        mov    eax, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Dh
        db  0A3h, 00Ah, 000h, 000h, 000h
        mov    edx, dword ptr [edx]
        mov    al, byte ptr [edx + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [ecx + 12h]
        db  088h, 00Dh, 002h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_26B3:
        test   eax, eax
        mov    al, 19h
        jne    SHORT loc_3_26DB
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Eh
        mov    al, byte ptr [esp + 8]
        db  0A2h, 004h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_26CE:
        mov    cl, 0
        cmp    eax, 1
        adc    cl, 0ffh
        and    cl, 19h
        mov    al, cl
loc_3_26DB:
        ret
sub_3_26DC:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        lea    ecx, [esi + 0ch]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 18h]
        db  089h, 015h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    edi, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 084h
        db  089h, 03Dh, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0ch]
        db  0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_271B:
        test   eax, eax
        jne    SHORT loc_3_274B
        xor    al, al
        db  066h, 083h, 03Dh, 00Eh, 000h, 000h, 000h, 000h
        jbe    SHORT loc_3_274B
        mov    dl, byte ptr [esp + 10h]
loc_3_272F:
        movzx  ecx, al
        db  038h, 014h, 04Dh, 010h, 000h, 000h, 000h
        je     SHORT loc_3_2750
        inc    al
        db  00Fh, 0B7h, 03Dh, 00Eh, 000h, 000h, 000h
        movzx  ecx, al
        cmp    ecx, edi
        jb     SHORT loc_3_272F
loc_3_274B:
        mov    al, 19h
loc_3_274D:
        pop    edi
        pop    esi
        ret
loc_3_2750:
        mov    cl, 11h
        imul   cl
        push   eax
        push   esi
        call   sub_3_2679
        add    esp, 8
        jmp    SHORT loc_3_274D
sub_3_2760:
        push   ebp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2773:
        test   al, 80h
        mov    eax, 19h
        je     SHORT loc_3_27BA
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_278F:
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0ch]
        lea    ecx, [ebp - 4]
        mov    al, byte ptr [eax]
        push   ecx
        inc    esi
        mov    byte ptr [edi], al
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_27AD:
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        mov    byte ptr [edi + 1], al
        xor    eax, eax
loc_3_27BA:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_27C0:
        push   ebp
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_27D2:
        test   al, 80h
        mov    eax, 19h
        je     SHORT loc_3_2817
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_27EE:
        add    esp, 0ch
        mov    cl, byte ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        lea    edx, [ebp - 4]
        push   edx
        inc    esi
        push   esi
        mov    byte ptr [eax], cl
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_280A:
        mov    eax, dword ptr [ebp - 4]
        add    esp, 0ch
        mov    cl, byte ptr [ebp + 0dh]
        mov    byte ptr [eax], cl
        xor    eax, eax
loc_3_2817:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_281C:
        mov    al, byte ptr [esp + 4]
        or     al, al
        lahf
        and    ah, 4
        mov    byte ptr [esp + 4], ah
        cmp    byte ptr [esp + 4], 1
        sbb    eax, eax
        neg    eax
        ret
sub_3_2834:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 2]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_3_2760
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_28A3
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 1], al
        jne    SHORT loc_3_28A3
        push   dword ptr [ebp - 2]
        call   sub_3_281C
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_28A3
        lea    eax, [ebp - 4]
        add    esi, 2
        push   eax
        push   esi
        call   sub_3_2760
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_28A3
        mov    al, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 3], al
        jne    SHORT loc_3_28A3
        mov    cx, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0ch]
        and    cx, 0ff00h
        mov    ax, word ptr [ebp - 2]
        and    ax, 0ffh
        add    cx, ax
        mov    eax, 1
        mov    word ptr [edx], cx
        jmp    SHORT loc_3_28A5
loc_3_28A3:
        xor    eax, eax
loc_3_28A5:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_28AA:
        push   9090h
        push   dword ptr [esp + 8]
        call   sub_3_27C0
        add    esp, 8
        test   eax, eax
        mov    eax, 0
        jne    SHORT loc_3_28D4
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 8]
        call   sub_3_2834
        add    esp, 8
loc_3_28D4:
        ret
sub_3_28D5:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   esi
        push   edi
        lea    esi, [ebp - 1ch]
        mov    edi, 0bh
loc_3_28E5:
        lea    eax, [ebp - 20h]
        push   eax
        push   edi
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_28F5:
        add    esp, 0ch
        mov    eax, dword ptr [ebp - 20h]
        inc    edi
        mov    al, byte ptr [eax]
        mov    byte ptr [esi], al
        inc    esi
        cmp    edi, 24h
        jb     SHORT loc_3_28E5
        cmp    word ptr [ebp - 1ch], 200h
        jne    SHORT loc_3_2963
        cmp    byte ptr [ebp - 1ah], 0
        je     SHORT loc_3_2963
        cmp    word ptr [ebp - 19h], 1
        jne    SHORT loc_3_2963
        cmp    byte ptr [ebp - 17h], 1
        je     SHORT loc_3_2927
        cmp    byte ptr [ebp - 17h], 2
        jne    SHORT loc_3_2963
loc_3_2927:
        xor    eax, eax
        cmp    word ptr [ebp - 16h], ax
        je     SHORT loc_3_2963
        test   byte ptr [ebp - 16h], 0fh
        jne    SHORT loc_3_2963
        cmp    word ptr [ebp - 14h], ax
        jne    SHORT loc_3_2940
        cmp    dword ptr [ebp - 7], eax
        je     SHORT loc_3_2963
loc_3_2940:
        xor    eax, eax
        cmp    word ptr [ebp - 11h], ax
        je     SHORT loc_3_2963
        cmp    word ptr [ebp - 0fh], ax
        je     SHORT loc_3_2963
        cmp    word ptr [ebp - 0dh], ax
        je     SHORT loc_3_2963
        cmp    byte ptr [ebp - 12h], 0f0h
        jb     SHORT loc_3_2963
        movzx  eax, word ptr [ebp - 14h]
        shl    eax, 9
        jmp    SHORT loc_3_2965
loc_3_2963:
        xor    eax, eax
loc_3_2965:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_296B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 2]
        push   edi
        mov    esi, 4000h
        push   eax
        push   esi
        call   sub_3_2760
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_29EE
        xor    edi, edi
loc_3_298B:
        push   edi
        push   esi
        call   sub_3_27C0
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_29EE
        lea    eax, [ebp - 6]
        push   eax
        push   esi
        call   sub_3_2760
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_29EE
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_3_2760
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_29EE
        cmp    word ptr [ebp - 6], di
        jne    SHORT loc_3_29F6
        cmp    word ptr [ebp - 4], di
        je     SHORT loc_3_29F6
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_3_27C0
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_29EE
        add    esi, 4000h
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_3_2760
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_298B
loc_3_29EE:
        xor    eax, eax
loc_3_29F0:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_3_29F6:
        mov    eax, esi
        jmp    SHORT loc_3_29F0
sub_3_29FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        push   edi
        push   eax
        push   0
        call   sub_3_28AA
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_2A3B
        push   78h
        push   dword ptr [ebp + 8]
        call   sub_3_26DC
        add    esp, 8
        lea    eax, [ebp - 2]
        push   eax
        push   0
        call   sub_3_28AA
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_2A3B
        xor    eax, eax
        jmp    NEAR PTR loc_3_2AD1
loc_3_2A3B:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        db  0BEh, 000h, 000h, 000h, 000h
        cmp    word ptr [ebp - 2], ax
        je     SHORT loc_3_2A5E
loc_3_2A4C:
        cmp    word ptr [esi], 0
        je     SHORT loc_3_2AB3
        mov    ax, word ptr [ebp - 2]
        add    esi, 0fh
        cmp    word ptr [esi], ax
        jne    SHORT loc_3_2A4C
loc_3_2A5E:
        mov    bx, word ptr [esi + 0ch]
        mov    eax, dword ptr [esi + 8]
        mov    edi, eax
        mov    dword ptr [ebp - 8], eax
loc_3_2A6A:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_3_2834
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_2A85
        mov    ax, word ptr [ebp - 2]
        cmp    word ptr [ebp - 4], ax
        je     SHORT loc_3_2AB9
loc_3_2A85:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_3_28AA
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_2AB9
        mov    ax, word ptr [ebp - 2]
        cmp    word ptr [ebp - 4], ax
        jne    SHORT loc_3_2AB9
        push   ebx
        push   edi
        call   sub_3_27C0
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_2AB7
        add    edi, dword ptr [ebp - 8]
        jmp    SHORT loc_3_2A6A
loc_3_2AB3:
        xor    eax, eax
        jmp    SHORT loc_3_2AD1
loc_3_2AB7:
        xor    edi, edi
loc_3_2AB9:
        push   ebx
        push   0
        call   sub_3_27C0
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_2ACA
        xor    edi, edi
loc_3_2ACA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], esi
        mov    eax, edi
loc_3_2AD1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_2AD8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2AED:
        add    esp, 4
        lea    eax, [esi + 28h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2AF9:
        add    esp, 4
        lea    eax, [esi + 2ch]
        xor    edi, edi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2B07:
        add    esp, 4
        lea    eax, [ebp - 2]
        push   eax
        push   edi
        call   sub_3_2760
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_3_2C25
        push   9090h
        push   edi
        call   sub_3_27C0
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_3_2C25
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_3_2760
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_3_2C25
        mov    ecx, esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2B51:
        cmp    byte ptr [ebp - 2], 0e9h
        mov    di, ax
        je     SHORT loc_3_2B60
        cmp    byte ptr [ebp - 2], 0ebh
        jne    SHORT loc_3_2BA6
loc_3_2B60:
        mov    bx, di
        and    bx, 1
        jne    SHORT loc_3_2B71
        cmp    word ptr [ebp - 4], 9090h
        jne    SHORT loc_3_2BA6
loc_3_2B71:
        push   dword ptr [ebp - 2]
        push   0
        call   sub_3_27C0
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_3_2C25
        push   esi
        call   sub_3_28D5
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_2BA6
        test   bx, bx
        db  0B9h, 0B4h, 000h, 000h, 000h
        jne    SHORT loc_3_2BA2
        db  0B9h, 0A5h, 000h, 000h, 000h
loc_3_2BA2:
        push   ecx
        push   eax
        jmp    SHORT loc_3_2C1A
loc_3_2BA6:
        test   di, 1
        je     SHORT loc_3_2BB4
        mov    eax, 1ah
        jmp    SHORT loc_3_2C25
loc_3_2BB4:
        cmp    word ptr [ebp - 4], 9090h
        push   esi
        jne    SHORT loc_3_2BEA
        call   sub_3_296B
        add    esp, 4
        mov    edi, eax
        push   dword ptr [ebp - 2]
        push   0
        call   sub_3_27C0
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_2C25
        test   edi, edi
        jne    SHORT loc_3_2BE3
        mov    eax, 19h
        jmp    SHORT loc_3_2C25
loc_3_2BE3:
        db  068h, 0A5h, 000h, 000h, 000h
        jmp    SHORT loc_3_2C19
loc_3_2BEA:
        call   sub_3_2633
        add    esp, 4
        mov    bl, al
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        call   sub_3_29FA
        add    esp, 8
        mov    edi, eax
        push   ebx
        push   esi
        call   sub_3_2679
        add    esp, 8
        mov    eax, 19h
        test   edi, edi
        je     SHORT loc_3_2C25
        push   dword ptr [ebp - 8]
loc_3_2C19:
        push   edi
loc_3_2C1A:
        push   esi
        call   sub_3_25C5
        add    esp, 0ch
        xor    eax, eax
loc_3_2C25:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_2C2C:
        push   esi
        push   edi
        mov    esi, 19h
        db  068h, 000h, 000h, 000h, 000h
        mov    edi, dword ptr [esp + 10h]
        push   edi
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2C42:
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_2CAD
        db  0A1h, 000h, 000h, 000h, 000h
        and    word ptr [eax + 14h], 0fffdh
        db  0A1h, 000h, 000h, 000h, 000h
        or     byte ptr [eax + 14h], 8
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [ecx + 16h], 0
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2C75:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_2C9F
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2C87:
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_2C9F
        push   edi
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        call   sub_3_2AD8
        add    esp, 4
        mov    esi, eax
loc_3_2C9F:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2CAA:
        add    esp, 4
loc_3_2CAD:
        mov    eax, esi
        pop    edi
        pop    esi
        ret
loc_3_2CB2:
        db  0A0h, 0A9h, 000h, 000h, 000h
        and    al, 0fh
        cmp    al, 1
        jne    SHORT loc_3_2D20
        db  083h, 03Dh, 006h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_2D20
        db  0F6h, 005h, 00Eh, 000h, 000h, 000h, 001h
        je     SHORT loc_3_2D20
        db  080h, 03Dh, 00Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_3_2D20
        db  066h, 0A1h, 00Ch, 000h, 000h, 000h
        db  066h, 039h, 005h, 011h, 000h, 000h, 000h
        jne    SHORT loc_3_2D20
        db  066h, 083h, 03Dh, 00Fh, 000h, 000h, 000h, 008h
        jne    SHORT loc_3_2D20
        db  066h, 0A1h, 00Eh, 000h, 000h, 000h
        db  066h, 039h, 005h, 015h, 000h, 000h, 000h
        jne    SHORT loc_3_2D20
        db  066h, 083h, 03Dh, 013h, 000h, 000h, 000h, 002h
        jne    SHORT loc_3_2D20
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        mov    byte ptr [ecx + 1eh], 4
        mov    byte ptr [eax], 1
        mov    eax, 13h
        jmp    SHORT loc_3_2D22
loc_3_2D20:
        xor    eax, eax
loc_3_2D22:
        ret
sub_3_2D23:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 0
        lea    eax, [ebp - 1]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 8]
        call   sub_3_4320
        movzx  eax, byte ptr [ebp - 1]
        add    esp, 0ch
        mov    esp, ebp
        pop    ebp
        ret
sub_3_2D49:
        push   esi
        db  0A0h, 0A9h, 000h, 000h, 000h
        push   edi
        and    al, 0fh
        cmp    al, 1
        jne    SHORT loc_3_2DBB
        db  083h, 03Dh, 006h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_2DBB
        db  0F6h, 005h, 00Eh, 000h, 000h, 000h, 001h
        je     SHORT loc_3_2DBB
        db  080h, 03Dh, 00Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_3_2DBB
        db  0B8h, 000h, 000h, 000h, 000h
loc_3_2D76:
        mov    dx, word ptr [eax]
        db  0BEh, 011h, 000h, 000h, 000h
loc_3_2D7E:
        cmp    word ptr [esi], dx
        jne    SHORT loc_3_2D96
        mov    di, 1
        mov    cl, byte ptr [eax + 2]
        shl    di, cl
        db  066h, 085h, 03Dh, 023h, 000h, 000h, 000h
        jne    SHORT loc_3_2DAD
loc_3_2D96:
        add    esi, 4
        db  081h, 0FEh, 021h, 000h, 000h, 000h
        jb     SHORT loc_3_2D7E
        add    eax, 3
        db  03Dh, 00Ch, 000h, 000h, 000h
        jb     SHORT loc_3_2D76
        jmp    SHORT loc_3_2DBB
loc_3_2DAD:
        mov    eax, dword ptr [esp + 10h]
        mov    byte ptr [eax], 1
        mov    eax, 13h
        jmp    SHORT loc_3_2DBD
loc_3_2DBB:
        xor    eax, eax
loc_3_2DBD:
        pop    edi
        pop    esi
        ret
sub_3_2DC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 0
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_3_4320
        add    esp, 0ch
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_3_2DEA
        mov    byte ptr [esi + 1eh], 2
loc_3_2DEA:
        movzx  eax, byte ptr [ebp - 1]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_3_2DF3:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        xor    edi, edi
        mov    dword ptr [ebp - 4], esi
loc_3_2E03:
        test   edi, edi
        jne    SHORT loc_3_2E4F
        db  0BAh, 011h, 000h, 000h, 000h
        mov    ebx, dword ptr [ebp - 4]
        imul   ebx, ebx, 3
        db  066h, 08Bh, 083h, 000h, 000h, 000h, 000h
loc_3_2E19:
        cmp    word ptr [edx], ax
        jne    SHORT loc_3_2E34
        mov    si, 1
        db  08Ah, 08Bh, 002h, 000h, 000h, 000h
        shl    si, cl
        db  066h, 085h, 035h, 023h, 000h, 000h, 000h
        jne    SHORT loc_3_2E41
loc_3_2E34:
        add    edx, 4
        db  081h, 0FAh, 021h, 000h, 000h, 000h
        jb     SHORT loc_3_2E19
        jmp    SHORT loc_3_2E46
loc_3_2E41:
        mov    edi, 1
loc_3_2E46:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 4
        jb     SHORT loc_3_2E03
loc_3_2E4F:
        db  0A0h, 0A9h, 000h, 000h, 000h
        and    al, 0fh
        cmp    al, 1
        jne    NEAR PTR loc_3_2FAF
        xor    eax, eax
        db  039h, 005h, 006h, 000h, 000h, 000h
        je     NEAR PTR loc_3_2FAF
        db  0F6h, 005h, 00Eh, 000h, 000h, 000h, 001h
        je     NEAR PTR loc_3_2FAF
        db  038h, 005h, 00Dh, 000h, 000h, 000h
        je     NEAR PTR loc_3_2FAF
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  066h, 039h, 005h, 023h, 000h, 000h, 000h
        je     SHORT loc_3_2E9D
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 002h, 000h
loc_3_2E9D:
        db  0C6h, 005h, 005h, 000h, 000h, 000h, 002h
        db  0C6h, 005h, 00Ah, 000h, 000h, 000h, 000h
        db  0F6h, 005h, 00Eh, 000h, 000h, 000h, 002h
        je     SHORT loc_3_2ED8
        push   0
        lea    eax, [ebp - 4]
        push   eax
        call   sub_3_1180
        add    esp, 8
        test   byte ptr [ebp - 4], 10h
        jne    SHORT loc_3_2ED8
        db  0F6h, 005h, 0AAh, 000h, 000h, 000h, 008h
        je     SHORT loc_3_2ED8
        db  080h, 00Dh, 00Ah, 000h, 000h, 000h, 008h
loc_3_2ED8:
        db  080h, 025h, 00Eh, 000h, 000h, 000h, 0FBh
        db  0C6h, 005h, 00Bh, 000h, 000h, 000h, 000h
        db  0C6h, 005h, 00Ch, 000h, 000h, 000h, 000h
        db  080h, 025h, 025h, 000h, 000h, 000h, 07Fh
        test   edi, edi
        je     SHORT loc_3_2F22
        db  066h, 089h, 035h, 023h, 000h, 000h, 000h
        push   2000h
        db  068h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [ebp + 0ch]
        push   dword ptr [esi]
        push   dword ptr [ebp + 8]
        call   sub_3_44F4
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_3_2F25
        jmp    NEAR PTR loc_3_2FB1
loc_3_2F22:
        mov    esi, dword ptr [ebp + 0ch]
loc_3_2F25:
        db  066h, 083h, 03Dh, 023h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_2F38
        db  066h, 0C7h, 005h, 023h, 000h, 000h, 000h, 0FFh, 0FFh
loc_3_2F38:
        cmp    edi, 1
        sbb    edi, edi
        and    edi, 2000h
        add    edi, 3000h
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [esi]
        push   dword ptr [ebp + 8]
        call   sub_3_44F4
        add    esp, 10h
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_3_2FB1
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   dword ptr [ecx]
        db  0E8h, 000h, 000h, 000h, 000h
loc_3_2F70:
        db  00Fh, 0B7h, 00Dh, 023h, 000h, 000h, 000h
        add    esp, 8
        test   dword ptr [ebp - 8], ecx
        je     SHORT loc_3_2FAB
        mov    eax, dword ptr [ebp - 8]
        add    edi, 800h
        db  080h, 00Dh, 025h, 000h, 000h, 000h, 080h
        db  066h, 021h, 005h, 023h, 000h, 000h, 000h
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [esi]
        push   dword ptr [ebp + 8]
        call   sub_3_44F4
        add    esp, 10h
        mov    ebx, eax
loc_3_2FAB:
        mov    eax, ebx
        jmp    SHORT loc_3_2FB1
loc_3_2FAF:
        xor    eax, eax
loc_3_2FB1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_2FB8:
        lea    eax, [esp + 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [esp + 0ch]
        call   sub_3_4320
        add    esp, 0ch
        ret
sub_3_2FCF:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    edi, 1
        mov    al, byte ptr [esi]
        and    al, 78h
        shr    al, 2
        movzx  edx, al
        test   byte ptr [esi], 80h
        db  066h, 08Bh, 08Ah, 000h, 000h, 000h, 000h
        je     SHORT loc_3_3024
        mov    bx, 0ah
        mov    ax, cx
        sub    dx, dx
        div    bx
        movzx  eax, ax
        movzx  edx, cx
        imul   eax, eax, 0ah
        cmp    eax, edx
        jne    SHORT loc_3_3024
        mov    al, byte ptr [esi + 1]
        and    al, 7fh
        cmp    al, 64h
        jae    SHORT loc_3_3024
        imul   cx, bx
        movzx  ax, al
        add    cx, ax
        mov    edi, 2
loc_3_3024:
        xor    ebx, ebx
        mov    bl, byte ptr [esi]
        and    ebx, 7
        sub    ebx, edi
        sub    ebx, 4
        test   ebx, ebx
        jle    SHORT loc_3_303D
loc_3_3034:
        imul   cx, cx, 0ah
        dec    ebx
        jne    SHORT loc_3_3034
        test   ebx, ebx
loc_3_303D:
        jge    SHORT loc_3_3054
        neg    ebx
loc_3_3041:
        mov    si, 0ah
        mov    ax, cx
        sub    dx, dx
        div    si
        mov    cx, ax
        dec    ebx
        jne    SHORT loc_3_3041
loc_3_3054:
        mov    al, cl
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_305A:
        push   ebp
        mov    al, byte ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        and    al, 3
        mov    byte ptr [ebp - 8], al
        je     NEAR PTR loc_3_3103
loc_3_3077:
        mov    al, byte ptr [esi]
        xor    ebx, ebx
        inc    esi
        mov    byte ptr [ebp - 1], al
loc_3_307F:
        mov    edx, 1
        mov    cl, bl
        shl    edx, cl
        movzx  eax, byte ptr [ebp - 1]
        test   eax, edx
        je     SHORT loc_3_30F0
        mov    al, byte ptr [esi]
        mov    byte ptr [ebp - 4], al
        test   byte ptr [esi], 80h
        je     SHORT loc_3_30A2
        inc    esi
        mov    al, byte ptr [esi]
        mov    byte ptr [ebp - 3], al
        jmp    SHORT loc_3_30A6
loc_3_30A2:
        mov    byte ptr [ebp - 3], 0
loc_3_30A6:
        test   byte ptr [esi], 80h
        je     SHORT loc_3_30B1
loc_3_30AB:
        inc    esi
        test   byte ptr [esi], 80h
        jne    SHORT loc_3_30AB
loc_3_30B1:
        inc    esi
        test   ebx, ebx
        jne    SHORT loc_3_30F0
        lea    eax, [ebp - 4]
        push   eax
        call   sub_3_2FCF
        add    esp, 4
        mov    byte ptr [ebp - 4], al
        test   edi, edi
        je     SHORT loc_3_30D5
        cmp    edi, 1
        je     SHORT loc_3_30DE
        cmp    edi, 2
        je     SHORT loc_3_30E7
        jmp    SHORT loc_3_30F0
loc_3_30D5:
        mov    cl, byte ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 2], cl
loc_3_30DE:
        mov    cl, byte ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 3], cl
loc_3_30E7:
        mov    cl, byte ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 4], cl
loc_3_30F0:
        inc    ebx
        cmp    ebx, 7
        jl     SHORT loc_3_307F
        movzx  eax, byte ptr [ebp - 8]
        inc    edi
        cmp    eax, edi
        jg     NEAR PTR loc_3_3077
loc_3_3103:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_310C:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    al, byte ptr [esp + 0ch]
        and    eax, 30h
        je     SHORT loc_3_314D
        cmp    eax, 10h
        je     SHORT loc_3_3158
        cmp    eax, 20h
        je     SHORT loc_3_3166
        cmp    eax, 30h
        je     SHORT loc_3_3176
loc_3_3129:
        mov    eax, dword ptr [ebp + 8]
loc_3_312C:
        xor    edx, edx
        mov    ecx, 0c0h
        mov    dl, byte ptr [ebp + 0ch]
        and    edx, ecx
        je     SHORT loc_3_3185
        cmp    edx, 40h
        je     SHORT loc_3_3190
        cmp    edx, 80h
        je     SHORT loc_3_319C
        cmp    edx, ecx
        je     SHORT loc_3_31AA
        jmp    SHORT loc_3_31B5
loc_3_314D:
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [edx], 0
        jmp    SHORT loc_3_3129
loc_3_3158:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 10h]
        movzx  ecx, byte ptr [eax]
        inc    eax
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_3_312C
loc_3_3166:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 10h]
        movzx  ecx, word ptr [eax]
        add    eax, 2
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_3_312C
loc_3_3176:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [eax]
        add    eax, 4
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_3_312C
loc_3_3185:
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], 0
        jmp    SHORT loc_3_31B5
loc_3_3190:
        movzx  ecx, byte ptr [eax]
        inc    ecx
        mov    edx, dword ptr [ebp + 14h]
        inc    eax
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_3_31B5
loc_3_319C:
        movzx  ecx, word ptr [eax]
        inc    ecx
        mov    edx, dword ptr [ebp + 14h]
        add    eax, 2
        mov    dword ptr [edx], ecx
        jmp    SHORT loc_3_31B5
loc_3_31AA:
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [ebp + 14h]
        inc    ecx
        add    eax, 4
        mov    dword ptr [edx], ecx
loc_3_31B5:
        pop    ebp
        ret
sub_3_31B7:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    dl, byte ptr [ebp + 0ch]
        mov    al, dl
        and    eax, 18h
        je     NEAR PTR loc_3_3256
        cmp    eax, 8
        je     NEAR PTR loc_3_3264
        cmp    eax, 10h
        je     NEAR PTR loc_3_3275
        cmp    eax, 18h
        je     NEAR PTR loc_3_3288
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 10h]
loc_3_31EE:
        shl    dword ptr [esi], 8
        xor    ebx, ebx
        mov    bl, dl
        and    ebx, 60h
        je     NEAR PTR loc_3_32A4
        cmp    ebx, 20h
        je     NEAR PTR loc_3_32B2
        cmp    ebx, 40h
        je     NEAR PTR loc_3_32C0
        cmp    ebx, 60h
        je     NEAR PTR loc_3_32D0
        mov    esi, dword ptr [ebp + 18h]
loc_3_321C:
        shl    dword ptr [esi], 8
        test   dl, 80h
        je     NEAR PTR loc_3_332E
        test   ebx, ebx
        je     NEAR PTR loc_3_32E9
        cmp    ebx, 20h
        je     NEAR PTR loc_3_32F7
        cmp    ebx, 40h
        je     NEAR PTR loc_3_3305
        cmp    ebx, 60h
        je     NEAR PTR loc_3_3315
        mov    edx, dword ptr [ebp + 14h]
loc_3_324E:
        shl    dword ptr [edx], 8
        jmp    NEAR PTR loc_3_3337
loc_3_3256:
        mov    esi, dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi], 0
        jmp    SHORT loc_3_31EE
loc_3_3264:
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 10h]
        movzx  ecx, byte ptr [eax]
        inc    eax
        mov    dword ptr [esi], ecx
        jmp    NEAR PTR loc_3_31EE
loc_3_3275:
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 10h]
        movzx  ecx, word ptr [eax]
        add    eax, 2
        mov    dword ptr [esi], ecx
        jmp    NEAR PTR loc_3_31EE
loc_3_3288:
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 10h]
        movzx  edi, byte ptr [eax + 2]
        movzx  ecx, word ptr [eax]
        shl    edi, 10h
        add    eax, 3
        add    edi, ecx
        mov    dword ptr [esi], edi
        jmp    NEAR PTR loc_3_31EE
loc_3_32A4:
        mov    esi, dword ptr [ebp + 18h]
        mov    dword ptr [esi], 0
        jmp    NEAR PTR loc_3_321C
loc_3_32B2:
        movzx  ecx, byte ptr [eax]
        mov    esi, dword ptr [ebp + 18h]
        inc    eax
        mov    dword ptr [esi], ecx
        jmp    NEAR PTR loc_3_321C
loc_3_32C0:
        movzx  ecx, word ptr [eax]
        mov    esi, dword ptr [ebp + 18h]
        add    eax, 2
        mov    dword ptr [esi], ecx
        jmp    NEAR PTR loc_3_321C
loc_3_32D0:
        movzx  edi, byte ptr [eax + 2]
        movzx  ecx, word ptr [eax]
        shl    edi, 10h
        mov    esi, dword ptr [ebp + 18h]
        add    edi, ecx
        add    eax, 3
        mov    dword ptr [esi], edi
        jmp    NEAR PTR loc_3_321C
loc_3_32E9:
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], 0
        jmp    NEAR PTR loc_3_324E
loc_3_32F7:
        movzx  ecx, byte ptr [eax]
        mov    edx, dword ptr [ebp + 14h]
        inc    eax
        mov    dword ptr [edx], ecx
        jmp    NEAR PTR loc_3_324E
loc_3_3305:
        movzx  ecx, word ptr [eax]
        mov    edx, dword ptr [ebp + 14h]
        add    eax, 2
        mov    dword ptr [edx], ecx
        jmp    NEAR PTR loc_3_324E
loc_3_3315:
        movzx  ecx, byte ptr [eax + 2]
        movzx  edx, word ptr [eax]
        shl    ecx, 10h
        add    eax, 3
        add    ecx, edx
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], ecx
        jmp    NEAR PTR loc_3_324E
loc_3_332E:
        mov    edx, dword ptr [ebp + 14h]
        mov    dword ptr [edx], 0ffffffffh
loc_3_3337:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_3_333C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 18h
        test   byte ptr [eax], 80h
        push   ebx
        push   esi
        push   edi
        je     SHORT loc_3_335C
        inc    eax
        mov    esi, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [eax]
        mov    byte ptr [esi + 0a9h], cl
        jmp    SHORT loc_3_335F
loc_3_335C:
        mov    esi, dword ptr [ebp + 0ch]
loc_3_335F:
        inc    eax
        mov    cl, byte ptr [eax]
        inc    eax
        mov    byte ptr [ebp - 1], cl
        push   dword ptr [ebp - 1]
        push   esi
        push   eax
        call   sub_3_305A
        add    esp, 0ch
        test   byte ptr [ebp - 1], 4
        je     SHORT loc_3_33A6
        mov    cl, byte ptr [eax]
        inc    eax
        mov    dl, cl
        and    dl, 3
        cmp    dl, 3
        je     SHORT loc_3_3392
        test   byte ptr [eax], 80h
        je     SHORT loc_3_3391
loc_3_338B:
        inc    eax
        test   byte ptr [eax], 80h
        jne    SHORT loc_3_338B
loc_3_3391:
        inc    eax
loc_3_3392:
        and    cl, 1ch
        cmp    cl, 1ch
        je     SHORT loc_3_33A6
        test   byte ptr [eax], 80h
        je     SHORT loc_3_33A5
loc_3_339F:
        inc    eax
        test   byte ptr [eax], 80h
        jne    SHORT loc_3_339F
loc_3_33A5:
        inc    eax
loc_3_33A6:
        test   byte ptr [ebp - 1], 8
        je     NEAR PTR loc_3_3482
        lea    edi, [esi + 0fh]
        mov    ecx, 4
        mov    edx, edi
        xor    ebx, ebx
loc_3_33BC:
        mov    dword ptr [esi + 1fh], ebx
        mov    word ptr [edx], bx
        add    edx, 4
        mov    word ptr [edx - 2], bx
        dec    ecx
        jne    SHORT loc_3_33BC
        mov    bl, byte ptr [eax]
        inc    eax
        test   bl, 80h
        movzx  ecx, bl
        mov    dword ptr [esi + 1fh], ecx
        je     NEAR PTR loc_3_3460
        mov    dword ptr [ebp - 10h], 0
        mov    cl, byte ptr [eax]
        inc    eax
        mov    byte ptr [ebp - 2], cl
        and    cl, 0fh
        movzx  ecx, cl
        mov    dword ptr [ebp - 18h], ecx
loc_3_33F4:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 0ch]
        push   ecx
        push   edx
        push   dword ptr [ebp - 2]
        push   eax
        call   sub_3_310C
        add    esp, 10h
        cmp    dword ptr [ebp - 10h], 4
        jge    SHORT loc_3_3450
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_3_3423
        mov    cl, bl
        mov    edx, 1
        and    cl, 1fh
        shl    edx, cl
        mov    dword ptr [ebp - 8], edx
loc_3_3423:
        cmp    dword ptr [ebp - 8], 0ffffh
        ja     SHORT loc_3_3450
        cmp    dword ptr [ebp - 0ch], 0ffffh
        ja     SHORT loc_3_3450
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, dword ptr [ebp - 0ch]
        cmp    ecx, 10000h
        ja     SHORT loc_3_3450
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 0ch]
        mov    word ptr [edi], cx
        mov    word ptr [edi + 2], dx
loc_3_3450:
        add    edi, 4
        inc    dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    ecx, dword ptr [ebp - 18h]
        jle    SHORT loc_3_33F4
        jmp    SHORT loc_3_3482
loc_3_3460:
        mov    cl, bl
        and    cl, 1fh
        je     SHORT loc_3_3482
        mov    edx, 1
        shl    edx, cl
        mov    dword ptr [ebp - 8], edx
        cmp    edx, 0ffffh
        ja     SHORT loc_3_347C
        mov    word ptr [edi], dx
loc_3_347C:
        mov    word ptr [esi + 11h], 0
loc_3_3482:
        test   byte ptr [ebp - 1], 10h
        je     SHORT loc_3_34B0
        mov    cl, byte ptr [eax]
        inc    eax
        test   cl, 10h
        movzx  edx, cl
        mov    dword ptr [esi + 25h], edx
        je     SHORT loc_3_34A2
        mov    cx, word ptr [eax]
        add    eax, 2
        mov    word ptr [esi + 23h], cx
        jmp    SHORT loc_3_34B0
loc_3_34A2:
        mov    dx, 1
        and    cl, 0fh
        shl    dx, cl
        mov    word ptr [esi + 23h], dx
loc_3_34B0:
        mov    dl, byte ptr [ebp - 1]
        and    dl, 60h
        je     SHORT loc_3_34CA
        lea    ecx, [esi + 29h]
        mov    ebx, 8
        xor    edi, edi
loc_3_34C2:
        mov    dword ptr [ecx], edi
        add    ecx, 10h
        dec    ebx
        jne    SHORT loc_3_34C2
loc_3_34CA:
        movzx  ecx, dl
        cmp    ecx, 20h
        je     SHORT loc_3_34E1
        cmp    ecx, 40h
        je     SHORT loc_3_3501
        cmp    ecx, 60h
        je     SHORT loc_3_3523
        jmp    NEAR PTR loc_3_358C
loc_3_34E1:
        mov    dword ptr [esi + 31h], 0ffffffffh
        movzx  ecx, word ptr [eax]
        shl    ecx, 8
        add    eax, 2
        mov    dword ptr [esi + 29h], ecx
        xor    ecx, ecx
        mov    dword ptr [esi + 2dh], ecx
        mov    dword ptr [esi + 35h], ecx
        jmp    NEAR PTR loc_3_358C
loc_3_3501:
        movzx  ecx, word ptr [eax]
        shl    ecx, 8
        add    eax, 2
        movzx  edx, word ptr [eax]
        shl    edx, 8
        add    eax, 2
        mov    dword ptr [esi + 29h], ecx
        mov    dword ptr [esi + 2dh], edx
        add    ecx, edx
        mov    dword ptr [esi + 35h], edx
        mov    dword ptr [esi + 31h], ecx
        jmp    SHORT loc_3_358C
loc_3_3523:
        mov    cl, byte ptr [eax]
        xor    edi, edi
        inc    eax
        lea    ebx, [esi + 2dh]
        mov    byte ptr [ebp - 2], cl
        and    cl, 7
        movzx  ecx, cl
        mov    dword ptr [ebp - 10h], ecx
loc_3_3537:
        lea    ecx, [ebp - 14h]
        lea    edx, [ebp - 0ch]
        push   ecx
        push   edx
        lea    ecx, [ebp - 8]
        push   ecx
        push   dword ptr [ebp - 2]
        push   eax
        call   sub_3_31B7
        add    esp, 14h
        cmp    edi, 8
        jge    SHORT loc_3_3583
        cmp    dword ptr [ebp - 0ch], -1
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebx - 4], ecx
        jne    SHORT loc_3_356F
        mov    dword ptr [ebx], 0
        mov    dword ptr [ebx + 4], 0ffffffffh
        jmp    SHORT loc_3_357D
loc_3_356F:
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx], ecx
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ebx + 4], ecx
loc_3_357D:
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ebx + 8], ecx
loc_3_3583:
        add    ebx, 10h
        inc    edi
        cmp    edi, dword ptr [ebp - 10h]
        jle    SHORT loc_3_3537
loc_3_358C:
        test   byte ptr [ebp - 1], 80h
        je     SHORT loc_3_359A
        mov    al, byte ptr [eax]
        mov    byte ptr [esi + 0aah], al
loc_3_359A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_35A1:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        inc    eax
        mov    esi, dword ptr [esp + 10h]
        mov    bl, byte ptr [eax]
        push   ebx
        inc    eax
        push   esi
        push   eax
        call   sub_3_305A
        add    esp, 0ch
        test   bl, 8
        je     SHORT loc_3_35C0
        inc    eax
loc_3_35C0:
        test   bl, 10h
        je     SHORT loc_3_35ED
        mov    cl, byte ptr [eax]
        inc    eax
        test   cl, 10h
        movzx  edx, cl
        mov    dword ptr [esi + 25h], edx
        je     SHORT loc_3_35DF
        mov    cx, word ptr [eax]
        add    eax, 2
        mov    word ptr [esi + 23h], cx
        jmp    SHORT loc_3_35ED
loc_3_35DF:
        mov    dx, 1
        and    cl, 0fh
        shl    dx, cl
        mov    word ptr [esi + 23h], dx
loc_3_35ED:
        test   bl, 20h
        je     SHORT loc_3_35F3
        inc    eax
loc_3_35F3:
        test   bl, 80h
        je     SHORT loc_3_3600
        mov    al, byte ptr [eax]
        mov    byte ptr [esi + 0aah], al
loc_3_3600:
        pop    esi
        pop    ebx
        ret
sub_3_3603:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        push   esi
        add    edx, 14h
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    al, byte ptr [edx]
        mov    bl, al
        and    ebx, 40h
        jne    SHORT loc_3_3630
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ebp, dword ptr [esp + 18h]
        mov    edi, ebp
        mov    ecx, 2bh
        rep movsd
        movsw
        movsb
        jmp    SHORT loc_3_3634
loc_3_3630:
        mov    ebp, dword ptr [esp + 18h]
loc_3_3634:
        and    al, 3fh
        push   ebp
        push   edx
        mov    byte ptr [ebp + 0dh], al
        cmp    dword ptr [esp + 24h], 0
        je     SHORT loc_3_3649
        call   sub_3_35A1
        jmp    SHORT loc_3_364E
loc_3_3649:
        call   sub_3_333C
loc_3_364E:
        add    esp, 8
        test   ebx, ebx
        je     SHORT loc_3_3666
        db  0BFh, 000h, 000h, 000h, 000h
        mov    esi, ebp
        mov    ecx, 2bh
        rep movsd
        movsw
        movsb
loc_3_3666:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_3_366B:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        add    ecx, 14h
        movzx  edx, word ptr [ecx - 4]
        push   ebx
        cmp    edx, 4
        push   esi
        push   edi
        jl     NEAR PTR loc_3_377F
        mov    bl, byte ptr [ecx]
        xor    eax, eax
        mov    al, bl
        sub    edx, 2
        and    eax, 3
        and    bl, 3ch
        shr    bl, 2
        add    ecx, 2
        movzx  esi, bl
        lea    edi, [eax + 1]
        cmp    edi, edx
        jg     NEAR PTR loc_3_377F
        test   eax, eax
        je     SHORT loc_3_36BD
        cmp    eax, 1
        je     SHORT loc_3_36C2
        cmp    eax, 2
        je     SHORT loc_3_36C7
        cmp    eax, 3
        je     SHORT loc_3_36D5
        jmp    SHORT loc_3_36DC
loc_3_36BD:
        movzx  ebx, byte ptr [ecx]
        jmp    SHORT loc_3_36D7
loc_3_36C2:
        movzx  ebx, word ptr [ecx]
        jmp    SHORT loc_3_36D7
loc_3_36C7:
        movzx  ebx, byte ptr [ecx + 2]
        movzx  edi, word ptr [ecx]
        shl    ebx, 10h
        add    ebx, edi
        jmp    SHORT loc_3_36D7
loc_3_36D5:
        mov    ebx, dword ptr [ecx]
loc_3_36D7:
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [edi], ebx
loc_3_36DC:
        lea    edi, [eax + 1]
        lea    ecx, [eax + ecx + 1]
        sub    edx, edi
        lea    eax, [esi + 1]
        cmp    eax, edx
        jg     NEAR PTR loc_3_377F
        test   esi, esi
        jne    SHORT loc_3_36FA
        movzx  ax, byte ptr [ecx]
        jmp    SHORT loc_3_36FD
loc_3_36FA:
        mov    ax, word ptr [ecx]
loc_3_36FD:
        mov    edi, dword ptr [ebp + 10h]
        lea    ecx, [esi + ecx + 1]
        mov    word ptr [edi], ax
        lea    eax, [esi + 1]
        sub    edx, eax
        xor    esi, esi
        mov    eax, dword ptr [ebp + 14h]
loc_3_3711:
        cmp    edx, 3
        jl     SHORT loc_3_377F
        mov    bl, byte ptr [ecx]
        cmp    bl, 0ffh
        je     SHORT loc_3_377F
        cmp    bl, 0c0h
        jne    SHORT loc_3_3769
        mov    bl, byte ptr [ecx + 2]
        shr    bl, 6
        movzx  edi, bl
        lea    ebx, [edi + 3]
        cmp    ebx, edx
        jg     SHORT loc_3_377F
        test   edi, edi
        je     SHORT loc_3_3747
        cmp    edi, 1
        je     SHORT loc_3_374F
        cmp    edi, 2
        je     SHORT loc_3_3755
        cmp    edi, 3
        je     SHORT loc_3_3764
        jmp    SHORT loc_3_3769
loc_3_3747:
        movzx  ebx, byte ptr [ecx + 2]
        mov    dword ptr [eax], ebx
        jmp    SHORT loc_3_3769
loc_3_374F:
        movzx  edi, word ptr [ecx + 2]
        jmp    SHORT loc_3_3767
loc_3_3755:
        movzx  edi, byte ptr [ecx + 4]
        movzx  ebx, word ptr [ecx + 2]
        shl    edi, 10h
        add    edi, ebx
        jmp    SHORT loc_3_3767
loc_3_3764:
        mov    edi, dword ptr [ecx + 2]
loc_3_3767:
        mov    dword ptr [eax], edi
loc_3_3769:
        movzx  edi, byte ptr [ecx + 1]
        add    eax, 4
        inc    esi
        lea    ebx, [edi + 2]
        lea    ecx, [edi + ecx + 2]
        sub    edx, ebx
        cmp    esi, 4
        jl     SHORT loc_3_3711
loc_3_377F:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_3_3784:
        push   ebp
        mov    edx, dword ptr [esp + 10h]
        mov    ebp, esp
        test   edx, edx
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        jle    SHORT loc_3_37A5
        mov    ecx, dword ptr [ebp + 0ch]
loc_3_3796:
        mov    bl, byte ptr [ecx]
        mov    byte ptr [eax], bl
        inc    eax
        test   bl, bl
        je     SHORT loc_3_37A5
        dec    edx
        inc    ecx
        test   edx, edx
        jg     SHORT loc_3_3796
loc_3_37A5:
        mov    byte ptr [eax - 1], 0
        pop    ebx
        pop    ebp
        ret
sub_3_37AC:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 18h]
        xor    eax, eax
        push   1ah
        push   esi
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], eax
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_37F9
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_37F9
        lea    eax, [ebp - 1ch]
        lea    ecx, [ebp - 2]
        push   eax
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        db  068h, 000h, 000h, 000h, 000h
        call   sub_3_366B
        add    esp, 10h
loc_3_37F9:
        push   0ffh
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_3_38DD
        db  0BFh, 014h, 000h, 000h, 000h
loc_3_3814:
        db  0B8h, 000h, 000h, 000h, 000h
        db  08Ah, 00Dh, 000h, 000h, 000h, 000h
        db  038h, 00Dh, 010h, 000h, 000h, 000h
        je     SHORT loc_3_3837
loc_3_3827:
        cmp    byte ptr [eax], 0ffh
        je     SHORT loc_3_3837
        inc    eax
        db  08Ah, 00Dh, 010h, 000h, 000h, 000h
        cmp    byte ptr [eax], cl
        jne    SHORT loc_3_3827
loc_3_3837:
        mov    bl, byte ptr [eax]
        cmp    bl, 0ffh
        je     NEAR PTR loc_3_38CC
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_38CC
        db  0F6h, 005h, 006h, 000h, 000h, 000h, 001h
        db  00Fh, 0B7h, 00Dh, 010h, 000h, 000h, 000h
        jne    SHORT loc_3_388C
        db  0A1h, 00Ch, 000h, 000h, 000h
        add    eax, 4
        cmp    eax, dword ptr [ebp - 8]
        ja     SHORT loc_3_388C
        lea    eax, [ecx*2 + 4]
        db  003h, 005h, 00Ch, 000h, 000h, 000h
        cmp    eax, dword ptr [ebp - 8]
        jbe    SHORT loc_3_388C
        mov    ecx, dword ptr [ebp - 8]
        db  02Bh, 00Dh, 00Ch, 000h, 000h, 000h
        sub    ecx, 4
        shr    ecx, 1
loc_3_388C:
        cmp    bl, 15h
        jne    SHORT loc_3_38BC
        db  0B8h, 016h, 000h, 000h, 000h
        db  08Dh, 099h, 014h, 000h, 000h, 000h
loc_3_389C:
        cmp    eax, ebx
        jae    SHORT loc_3_38B4
        mov    edx, eax
        inc    eax
        cmp    byte ptr [edx], 0
        jne    SHORT loc_3_389C
loc_3_38A8:
        cmp    eax, ebx
        jae    SHORT loc_3_38B4
        mov    edx, eax
        inc    eax
        cmp    byte ptr [edx], 0
        jne    SHORT loc_3_38A8
loc_3_38B4:
        sub    eax, edi
        cmp    ecx, eax
        jbe    SHORT loc_3_38BC
        mov    ecx, eax
loc_3_38BC:
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        push   ecx
        push   edi
        call   sub_3_5420
        add    esp, 10h
loc_3_38CC:
        push   esi
        call   sub_3_4CDD
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_3_3814
loc_3_38DD:
        mov    ax, word ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_38E8:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_3_393F
        mov    edx, dword ptr [esp + 10h]
loc_3_38F9:
        mov    ecx, dword ptr [esi + 10h]
        test   ecx, ecx
        je     SHORT loc_3_3939
loc_3_3900:
        cmp    edx, ecx
        je     SHORT loc_3_3933
        lea    eax, [ecx + 48h]
        lea    edi, [edx + 48h]
loc_3_390A:
        mov    bl, byte ptr [eax]
        cmp    bl, byte ptr [edi]
        jne    SHORT loc_3_392A
        or     bl, bl
        je     SHORT loc_3_3926
        mov    bl, byte ptr [eax + 1]
        cmp    bl, byte ptr [edi + 1]
        jne    SHORT loc_3_392A
        add    eax, 2
        add    edi, 2
        or     bl, bl
        jne    SHORT loc_3_390A
loc_3_3926:
        xor    eax, eax
        jmp    SHORT loc_3_392F
loc_3_392A:
        sbb    eax, eax
        sbb    eax, -1
loc_3_392F:
        test   eax, eax
        je     SHORT loc_3_3948
loc_3_3933:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_3_3900
loc_3_3939:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_3_38F9
loc_3_393F:
        mov    eax, 1
loc_3_3944:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_3948:
        xor    eax, eax
        jmp    SHORT loc_3_3944
sub_3_394C:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 44h
        db  0B8h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        mov    byte ptr [ebp - 44h], cl
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], ecx
        db  038h, 00Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_3_397B
loc_3_396D:
        mov    dl, byte ptr [eax]
        inc    dword ptr [ebp - 4]
        inc    eax
        mov    byte ptr [esi], dl
        inc    esi
        cmp    byte ptr [eax], 0
        jne    SHORT loc_3_396D
loc_3_397B:
        db  0BBh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   15h
        mov    edi, dword ptr [eax + 18h]
        push   edi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_3A0B
        push   edi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_3A0B
        db  066h, 083h, 03Dh, 010h, 000h, 000h, 000h, 002h
        jbe    SHORT loc_3_3A0B
        db  080h, 03Dh, 016h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_3A0B
        db  0BBh, 016h, 000h, 000h, 000h
        mov    edx, 0ffffffffh
        db  00Fh, 0B7h, 005h, 010h, 000h, 000h, 000h
        mov    edi, ebx
        mov    ecx, edx
        db  0C6h, 080h, 014h, 000h, 000h, 000h, 000h
        sub    eax, eax
        repne scasb
        not    ecx
        add    ecx, edx
        movzx  eax, cx
        db  00Fh, 0B7h, 00Dh, 010h, 000h, 000h, 000h
        sub    ecx, 2
        cmp    eax, ecx
        jge    SHORT loc_3_3A07
        push   40h
        mov    edi, ebx
        mov    ecx, edx
        sub    eax, eax
        repne scasb
        not    ecx
        add    ecx, ebx
        lea    eax, [ebp - 44h]
        push   ecx
        push   eax
        call   sub_3_3784
        add    esp, 0ch
        jmp    SHORT loc_3_3A0B
loc_3_3A07:
        mov    byte ptr [ebp - 44h], 0
loc_3_3A0B:
        cmp    dword ptr [ebp - 4], 74h
        jge    SHORT loc_3_3A2F
loc_3_3A11:
        mov    al, byte ptr [ebx]
        test   al, al
        je     SHORT loc_3_3A2F
        cmp    al, 20h
        jl     SHORT loc_3_3A25
        cmp    al, 7fh
        jge    SHORT loc_3_3A25
        inc    ebx
        mov    byte ptr [esi], al
        inc    esi
        jmp    SHORT loc_3_3A26
loc_3_3A25:
        inc    ebx
loc_3_3A26:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 74h
        jl     SHORT loc_3_3A11
loc_3_3A2F:
        mov    byte ptr [esi], 2dh
        inc    esi
        cmp    byte ptr [ebp - 44h], 0
        je     SHORT loc_3_3A66
        cmp    dword ptr [ebp - 4], 78h
        lea    eax, [ebp - 44h]
        jge    SHORT loc_3_3A62
loc_3_3A42:
        mov    cl, byte ptr [eax]
        test   cl, cl
        je     SHORT loc_3_3A62
        cmp    cl, 20h
        jl     SHORT loc_3_3A58
        cmp    cl, 7fh
        jge    SHORT loc_3_3A58
        inc    eax
        mov    byte ptr [esi], cl
        inc    esi
        jmp    SHORT loc_3_3A59
loc_3_3A58:
        inc    eax
loc_3_3A59:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 78h
        jl     SHORT loc_3_3A42
loc_3_3A62:
        mov    byte ptr [esi], 2dh
        inc    esi
loc_3_3A66:
        mov    eax, dword ptr [ebp + 10h]
        test   eax, eax
        je     SHORT loc_3_3A72
        lea    ecx, [esi - 1]
        mov    dword ptr [eax], ecx
loc_3_3A72:
        push   dword ptr [ebp + 8]
        call   sub_3_37AC
        add    esp, 4
        push   eax
        push   esi
        call   sub_3_4D76
        add    esp, 8
        mov    esi, eax
        cmp    dword ptr [ebp + 14h], 0
        jne    SHORT loc_3_3A96
        xor    eax, eax
        mov    byte ptr [esi], 0
        jmp    SHORT loc_3_3ABF
loc_3_3A96:
        xor    edi, edi
        mov    byte ptr [esi], 5ch
        mov    byte ptr [esi + 2], 0
        inc    esi
loc_3_3AA0:
        push   dword ptr [ebp + 8]
        lea    eax, [edi + 30h]
        mov    byte ptr [esi], al
        call   sub_3_38E8
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_3AC6
        inc    edi
        cmp    edi, 0ah
        jl     SHORT loc_3_3AA0
        mov    eax, 13h
loc_3_3ABF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_3AC6:
        xor    eax, eax
        jmp    SHORT loc_3_3ABF
sub_3_3ACA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, byte ptr [esi + 1eh]
        test   eax, eax
        je     SHORT loc_3_3AFC
        cmp    eax, 2
        je     SHORT loc_3_3B2A
        cmp    eax, 4
        je     NEAR PTR loc_3_3C47
        cmp    eax, 6
        je     NEAR PTR loc_3_3C4E
        xor    eax, eax
        jmp    NEAR PTR loc_3_3C59
loc_3_3AFC:
        movzx  eax, byte ptr [ebp + 0ch]
        cmp    eax, 2
        je     SHORT loc_3_3B11
        cmp    eax, 4
        je     SHORT loc_3_3B1C
        xor    eax, eax
        jmp    NEAR PTR loc_3_3C59
loc_3_3B11:
        push   esi
        call   sub_3_2DC0
        add    esp, 4
        jmp    SHORT loc_3_3B35
loc_3_3B1C:
        push   esi
        call   sub_3_2D23
        add    esp, 4
        jmp    NEAR PTR loc_3_3C59
loc_3_3B2A:
        mov    al, byte ptr [ebp + 0ch]
        sub    al, 2
        cmp    al, 1
        sbb    eax, eax
        neg    eax
loc_3_3B35:
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_3_3C14
        cmp    byte ptr [esi + 1eh], 2
        jne    NEAR PTR loc_3_3C14
        push   22h
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_3B94
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_3B94
loc_3_3B68:
        db  080h, 03Dh, 014h, 000h, 000h, 000h, 001h
        jb     SHORT loc_3_3B7A
        db  080h, 03Dh, 014h, 000h, 000h, 000h, 003h
        jbe    SHORT loc_3_3BE1
loc_3_3B7A:
        push   esi
        call   sub_3_4CDD
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_3B94
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_3B68
loc_3_3B94:
        push   15h
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_3_3C40
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_3_3C40
        xor    ebx, ebx
        xor    edi, edi
        db  00Fh, 0B7h, 005h, 010h, 000h, 000h, 000h
        db  0B9h, 016h, 000h, 000h, 000h
        db  005h, 014h, 000h, 000h, 000h
        cmp    eax, ecx
        jbe    SHORT loc_3_3C14
loc_3_3BD1:
        mov    dl, byte ptr [ecx]
        test   dl, dl
        jne    SHORT loc_3_3BEC
        inc    ebx
        cmp    ebx, 4
        jge    SHORT loc_3_3C14
        xor    edi, edi
        jmp    SHORT loc_3_3C39
loc_3_3BE1:
        mov    byte ptr [esi + 1eh], 9
        mov    eax, 1
        jmp    SHORT loc_3_3C59
loc_3_3BEC:
        cmp    dl, 0ffh
        je     SHORT loc_3_3C14
        cmp    dl, 61h
        jb     SHORT loc_3_3C00
        cmp    dl, 7ah
        ja     SHORT loc_3_3C00
        sub    dl, 20h
        mov    byte ptr [ecx], dl
loc_3_3C00:
        test   edi, edi
        je     SHORT loc_3_3C1B
        mov    dl, byte ptr [ecx]
        cmp    byte ptr [edi], dl
        jne    SHORT loc_3_3C19
        inc    edi
        cmp    byte ptr [edi], 0
        jne    SHORT loc_3_3C39
        mov    byte ptr [esi + 1eh], 9
loc_3_3C14:
        mov    eax, dword ptr [ebp - 4]
        jmp    SHORT loc_3_3C59
loc_3_3C19:
        xor    edi, edi
loc_3_3C1B:
        mov    dl, byte ptr [ecx]
        db  038h, 015h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_3C2C
        db  0BFh, 001h, 000h, 000h, 000h
        jmp    SHORT loc_3_3C39
loc_3_3C2C:
        db  038h, 015h, 000h, 000h, 000h, 000h
        jne    SHORT loc_3_3C39
        db  0BFh, 001h, 000h, 000h, 000h
loc_3_3C39:
        inc    ecx
        cmp    eax, ecx
        ja     SHORT loc_3_3BD1
        jmp    SHORT loc_3_3C14
loc_3_3C40:
        mov    eax, 1
        jmp    SHORT loc_3_3C59
loc_3_3C47:
        mov    al, byte ptr [ebp + 0ch]
        sub    al, 4
        jmp    SHORT loc_3_3C53
loc_3_3C4E:
        mov    al, byte ptr [ebp + 0ch]
        sub    al, 6
loc_3_3C53:
        cmp    al, 1
        sbb    eax, eax
        neg    eax
loc_3_3C59:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_3_3C60:
        db  0F6h, 005h, 0A9h, 000h, 000h, 000h, 00Fh
        je     SHORT loc_3_3C84
        db  083h, 03Dh, 006h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_3CAD
        db  0F6h, 005h, 00Eh, 000h, 000h, 000h, 001h
        je     SHORT loc_3_3CAD
        db  080h, 03Dh, 00Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_3_3CAD
loc_3_3C84:
        db  0B8h, 00Fh, 000h, 000h, 000h
loc_3_3C89:
        cmp    word ptr [eax], 0
        jne    SHORT loc_3_3CB0
        add    eax, 4
        db  03Dh, 01Fh, 000h, 000h, 000h
        jb     SHORT loc_3_3C89
        db  0B8h, 029h, 000h, 000h, 000h
loc_3_3C9E:
        cmp    dword ptr [eax], 0
        jne    SHORT loc_3_3CBE
        add    eax, 10h
        db  03Dh, 0A9h, 000h, 000h, 000h
        jb     SHORT loc_3_3C9E
loc_3_3CAD:
        xor    eax, eax
loc_3_3CAF:
        ret
loc_3_3CB0:
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [eax], 1
        mov    eax, 13h
        jmp    SHORT loc_3_3CAF
loc_3_3CBE:
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [eax], 1
        mov    eax, 13h
        jmp    SHORT loc_3_3CAF
sub_3_3CCC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        push   15h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_3D37
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_3D37
        db  0BFh, 016h, 000h, 000h, 000h
        mov    edx, 0ffffffffh
        mov    ecx, edx
        sub    eax, eax
        repne scasb
        not    ecx
        add    ecx, edx
        je     SHORT loc_3_3D37
        db  08Dh, 0B9h, 017h, 000h, 000h, 000h
        sub    eax, eax
        mov    ecx, edx
        repne scasb
        not    ecx
        add    ecx, edx
        je     SHORT loc_3_3D37
        mov    byte ptr [ebp - 1], 0
        lea    eax, [ebp - 1]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_3_4320
        movzx  eax, byte ptr [ebp - 1]
        add    esp, 0ch
        jmp    SHORT loc_3_3D39
loc_3_3D37:
        xor    eax, eax
loc_3_3D39:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_3_3D3F:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        add    eax, 30h
        sub    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    byte ptr [eax], 1
        push   ebx
        push   esi
        push   edi
        jbe    NEAR PTR loc_3_3E4E
        mov    eax, dword ptr [ebp + 8]
        push   15h
        mov    esi, dword ptr [eax + 18h]
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_3_3E4E
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_3_3E4E
        db  0BFh, 016h, 000h, 000h, 000h
        mov    edx, 0ffffffffh
        mov    ecx, edx
        sub    eax, eax
        repne scasb
        not    ecx
        add    ecx, edx
        je     NEAR PTR loc_3_3E4E
        db  08Dh, 0B9h, 017h, 000h, 000h, 000h
        sub    eax, eax
        mov    ecx, edx
        repne scasb
        not    ecx
        add    ecx, edx
        je     NEAR PTR loc_3_3E4E
        push   6
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_3_3E4E
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_3E4E
        db  080h, 03Dh, 014h, 000h, 000h, 000h, 001h
        jae    SHORT loc_3_3DE7
        db  0C6h, 005h, 014h, 000h, 000h, 000h, 001h
        jmp    SHORT loc_3_3DF7
loc_3_3DE7:
        db  080h, 03Dh, 014h, 000h, 000h, 000h, 008h
        jbe    SHORT loc_3_3DF7
        db  0C6h, 005h, 014h, 000h, 000h, 000h, 008h
loc_3_3DF7:
        mov    eax, dword ptr [ebp - 8]
        mov    bl, byte ptr [eax]
        db  038h, 01Dh, 014h, 000h, 000h, 000h
        je     SHORT loc_3_3E09
        mov    byte ptr [eax], 1
        jmp    SHORT loc_3_3E4E
loc_3_3E09:
        mov    byte ptr [ebp - 1], 0
        mov    byte ptr [ebp - 5], 0
        test   bl, bl
        je     SHORT loc_3_3E48
loc_3_3E15:
        lea    eax, [ebp - 1]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_3_4320
        add    esp, 0ch
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_3_3E3A
        add    esi, 28h
        inc    byte ptr [ebp - 5]
        cmp    byte ptr [ebp - 5], bl
        jb     SHORT loc_3_3E15
        jmp    SHORT loc_3_3E48
loc_3_3E3A:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [esi + 1eh], 0ah
        db  0C7h, 040h, 03Ch, 000h, 000h, 000h, 000h
loc_3_3E48:
        movzx  eax, byte ptr [ebp - 1]
        jmp    SHORT loc_3_3E50
loc_3_3E4E:
        xor    eax, eax
loc_3_3E50:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_3E57:
        mov    eax, dword ptr [esp + 8]
        movzx  ecx, byte ptr [eax]
        cmp    ecx, 1
        je     SHORT loc_3_3E6B
        cmp    ecx, 2
        je     SHORT loc_3_3E86
loc_3_3E68:
        xor    eax, eax
loc_3_3E6A:
        ret
loc_3_3E6B:
        mov    al, byte ptr [eax + 1]
        push   eax
        push   dword ptr [esp + 8]
        call   sub_3_3ACA
        add    esp, 8
        test   eax, eax
        je     SHORT loc_3_3E68
        mov    eax, 1
        jmp    SHORT loc_3_3E6A
loc_3_3E86:
        push   dword ptr [esp + 4]
        call   sub_3_3CCC
        add    esp, 4
        test   eax, eax
        je     SHORT loc_3_3E68
        mov    eax, 1
        jmp    SHORT loc_3_3E6A
sub_3_3E9D:
        push   ebx
        xor    ecx, ecx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        push   21h
        push   edi
        mov    eax, dword ptr [edi]
        lea    esi, [edi + 1eh]
        mov    dword ptr [eax + 3ch], ecx
        mov    byte ptr [esi], cl
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_3EDC
        push   edi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_3EDC
        db  080h, 03Dh, 014h, 000h, 000h, 000h, 009h
        jae    SHORT loc_3_3EDC
        db  0A0h, 014h, 000h, 000h, 000h
        mov    byte ptr [esi], al
loc_3_3EDC:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        db  0BBh, 000h, 000h, 000h, 000h
        je     SHORT loc_3_3F00
loc_3_3EEA:
        push   ebx
        push   edi
        call   sub_3_3E57
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_3F09
        add    ebx, 3
        cmp    byte ptr [ebx], 0
        jne    SHORT loc_3_3EEA
loc_3_3F00:
        mov    eax, 13h
loc_3_3F05:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_3_3F09:
        mov    eax, dword ptr [edi]
        cmp    byte ptr [esi], 0
        mov    dword ptr [eax + 3ch], ebx
        jne    SHORT loc_3_3F18
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [esi], al
loc_3_3F18:
        xor    eax, eax
        jmp    SHORT loc_3_3F05
sub_3_3F1C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 30h], 1
        jb     SHORT loc_3_3F67
        push   esi
        call   sub_3_3D3F
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_3F4A
        push   dword ptr [esi + 18h]
        call   sub_3_3E9D
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_3F67
loc_3_3F4A:
        lea    edi, [esi + 48h]
        push   1
        lea    eax, [esi + 44h]
        push   eax
        push   edi
        push   esi
        call   sub_3_394C
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_3_3F67
        or     byte ptr [esi + 14h], 4
        jmp    SHORT loc_3_3F71
loc_3_3F67:
        lea    edi, [esi + 48h]
        or     byte ptr [esi + 14h], 2
        mov    byte ptr [edi], 0
loc_3_3F71:
        push   0
        mov    ebx, dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   edi
        push   ebx
        call   sub_3_52E0
        add    esp, 10h
        cmp    eax, 10h
        jne    SHORT loc_3_3F91
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebx], eax
        xor    eax, eax
        jmp    SHORT loc_3_3FE3
loc_3_3F91:
        test   eax, eax
        jne    SHORT loc_3_3FE0
        mov    ebx, dword ptr [ebx]
        push   0
        push   esi
        push   dword ptr [ebp + 0ch]
        push   ebx
        call   sub_3_5430
        mov    cx, word ptr [esi + 10h]
        add    esp, 10h
        mov    word ptr [ebp - 4], cx
        push   0
        mov    word ptr [ebp - 2], 0
        lea    ecx, [ebp - 4]
        push   ecx
        push   4
        push   8
        push   ebx
        call   sub_3_5428
        add    esp, 14h
        cmp    byte ptr [edi], 0
        je     SHORT loc_3_3FDC
        push   ebx
        push   dword ptr [esi + 18h]
        call   sub_3_4AF9
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_3_3FE0
loc_3_3FDC:
        xor    eax, eax
        jmp    SHORT loc_3_3FE3
loc_3_3FE0:
        mov    byte ptr [edi], 0
loc_3_3FE3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_3FEA:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    ecx, dword ptr [esi]
        xor    edi, edi
        mov    edx, dword ptr [esp + 14h]
        lea    eax, [ecx + 48h]
loc_3_3FFC:
        mov    bl, byte ptr [eax]
        test   bl, bl
        je     SHORT loc_3_4011
        cmp    dword ptr [ecx + 44h], eax
        jbe    SHORT loc_3_4011
        inc    eax
        inc    edi
        mov    byte ptr [edx], bl
        inc    edx
        cmp    edi, 6fh
        jl     SHORT loc_3_3FFC
loc_3_4011:
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        db  0B8h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_402A
loc_3_401F:
        mov    bl, byte ptr [eax]
        inc    eax
        mov    byte ptr [edx], bl
        inc    edx
        cmp    byte ptr [eax], 0
        jne    SHORT loc_3_401F
loc_3_402A:
        mov    al, byte ptr [esi + 1fh]
        add    al, 30h
        mov    byte ptr [edx], al
        inc    edx
        mov    eax, dword ptr [esp + 18h]
        test   eax, eax
        je     SHORT loc_3_403C
        mov    dword ptr [eax], edx
loc_3_403C:
        mov    eax, dword ptr [ecx + 44h]
        cmp    byte ptr [eax], 0
        je     SHORT loc_3_404F
loc_3_4044:
        mov    cl, byte ptr [eax]
        inc    eax
        mov    byte ptr [edx], cl
        inc    edx
        cmp    byte ptr [eax], 0
        jne    SHORT loc_3_4044
loc_3_404F:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    byte ptr [edx], 0
        pop    ebx
        ret
sub_3_4058:
        push   ebp
        mov    ebp, esp
        sub    esp, 84h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi]
        cmp    byte ptr [ebx + 48h], 0
        je     NEAR PTR loc_3_40FA
        push   esi
        call   sub_3_3E9D
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_3_40FA
        push   0
        lea    eax, [ebp - 84h]
        push   eax
        push   esi
        call   sub_3_3FEA
        add    esp, 0ch
        lea    eax, [ebp - 84h]
        mov    edi, dword ptr [ebp + 10h]
        push   0
        push   dword ptr [ebp + 0ch]
        push   eax
        push   edi
        call   sub_3_52E0
        add    esp, 10h
        cmp    eax, 10h
        jne    SHORT loc_3_40B8
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi], eax
        xor    eax, eax
        jmp    SHORT loc_3_40FF
loc_3_40B8:
        test   eax, eax
        jne    SHORT loc_3_40FF
        mov    edi, dword ptr [edi]
        push   0
        push   ebx
        push   dword ptr [ebp + 0ch]
        push   edi
        call   sub_3_5430
        mov    cx, word ptr [ebx + 10h]
        add    esp, 10h
        mov    word ptr [ebp - 4], cx
        push   0
        mov    word ptr [ebp - 2], 0
        lea    ecx, [ebp - 4]
        push   ecx
        push   4
        push   8
        push   edi
        call   sub_3_5428
        add    esp, 14h
        push   edi
        push   esi
        call   sub_3_4AF9
        add    esp, 8
        jmp    SHORT loc_3_40FF
loc_3_40FA:
        mov    eax, 13h
loc_3_40FF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_3_4106:
        push   ebp
        mov    ebp, esp
        sub    esp, 8ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi]
        lea    edx, [ebx + 48h]
        mov    al, byte ptr [edx]
        mov    byte ptr [ebp - 8], al
        test   al, al
        je     NEAR PTR loc_3_42E5
        lea    edi, [edx + 7]
        db  0B9h, 000h, 000h, 000h, 000h
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_3_4145
loc_3_4138:
        mov    al, byte ptr [ecx]
        cmp    byte ptr [edi], al
        jne    SHORT loc_3_4145
        inc    ecx
        inc    edi
        cmp    byte ptr [ecx], 0
        jne    SHORT loc_3_4138
loc_3_4145:
        mov    eax, dword ptr [ebx + 44h]
        test   eax, eax
        je     NEAR PTR loc_3_425D
        cmp    byte ptr [ecx], 0
        je     NEAR PTR loc_3_425D
        mov    ecx, dword ptr [esi + 4]
        cmp    dword ptr [ebx + 8], ecx
        jne    SHORT loc_3_4189
        cmp    byte ptr [ebp - 8], 0
        lea    edi, [ebp - 8ch]
        je     SHORT loc_3_4178
loc_3_416D:
        mov    cl, byte ptr [edx]
        inc    edx
        mov    byte ptr [edi], cl
        inc    edi
        cmp    byte ptr [edx], 0
        jne    SHORT loc_3_416D
loc_3_4178:
        sub    eax, ebx
        mov    byte ptr [edi], 0
        lea    ecx, [ebp + eax - 0d4h]
        mov    dword ptr [ebp - 4], ecx
        jmp    SHORT loc_3_419D
loc_3_4189:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 8ch]
        push   eax
        push   ecx
        push   esi
        call   sub_3_3FEA
        add    esp, 0ch
loc_3_419D:
        mov    eax, dword ptr [ebp - 4]
        cmp    byte ptr [eax], 2dh
        jne    SHORT loc_3_41DD
        xor    edx, edx
        lea    edi, [ebp - 85h]
        push   edx
        mov    ecx, 0ffffffffh
        mov    byte ptr [eax], dl
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        lea    eax, [ebp - 85h]
        push   ecx
        push   eax
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   dword ptr [esi + 4]
        call   sub_3_5330
        add    esp, 1ch
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], 2dh
loc_3_41DD:
        mov    edi, dword ptr [ebp - 4]
        add    edi, 5
        cmp    byte ptr [edi], 5ch
        jne    SHORT loc_3_4202
        push   0
        lea    eax, [ebp - 8ch]
        push   eax
        mov    byte ptr [edi], 0
        push   dword ptr [esi + 4]
        call   sub_3_5438
        add    esp, 0ch
        mov    byte ptr [edi], 5ch
loc_3_4202:
        mov    dx, word ptr [esi + 1ah]
        cmp    dx, 0ffffh
        je     SHORT loc_3_425D
        xor    ecx, ecx
        lea    eax, [ebp - 8ch]
loc_3_4215:
        cmp    byte ptr [eax], 0
        je     SHORT loc_3_4226
        cmp    dword ptr [ebp - 4], eax
        jbe    SHORT loc_3_4226
        inc    eax
        inc    ecx
        cmp    ecx, 6ah
        jl     SHORT loc_3_4215
loc_3_4226:
        push   edx
        mov    byte ptr [eax], 2dh
        inc    eax
        push   eax
        call   sub_3_4D76
        mov    cx, word ptr [esi + 1ch]
        add    esp, 8
        mov    byte ptr [eax], 2dh
        push   ecx
        inc    eax
        push   eax
        call   sub_3_4D76
        add    esp, 8
        lea    ecx, [ebp - 8ch]
        mov    byte ptr [eax], 0
        push   0
        push   ecx
        push   dword ptr [esi + 4]
        call   sub_3_5438
        add    esp, 0ch
loc_3_425D:
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [ebx + 8], eax
        jne    SHORT loc_3_427B
        cmp    byte ptr [ebx + 30h], 2
        jb     SHORT loc_3_427B
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_3_5438
        add    esp, 0ch
loc_3_427B:
        movzx  eax, byte ptr [esi + 1eh]
        xor    edx, edx
        db  08Bh, 01Ch, 085h, 000h, 000h, 000h, 000h
        cmp    byte ptr [ebx], dl
        je     SHORT loc_3_42B0
        push   edx
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   ebx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   dword ptr [esi + 4]
        call   sub_3_5330
        add    esp, 1ch
loc_3_42B0:
        movzx  eax, byte ptr [esi + 1eh]
        xor    edx, edx
        db  08Bh, 01Ch, 085h, 000h, 000h, 000h, 000h
        cmp    byte ptr [ebx], dl
        je     SHORT loc_3_42E5
        push   edx
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        push   ecx
        push   ebx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        push   dword ptr [esi + 4]
        call   sub_3_5330
        add    esp, 1ch
loc_3_42E5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
_PTEXT ends
        end
