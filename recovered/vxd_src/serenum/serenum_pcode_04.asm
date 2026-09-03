; serenum_pcode_04.asm -- SERENUM object 4 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

        db  083h, 0ECh, 004h, 089h, 054h, 024h, 000h, 06Ah, 000h, 06Ah, 001h, 0FFh, 074h, 024h, 008h, 0FFh
        db  074h, 024h, 00Ch, 0E8h, 064h, 017h, 000h, 000h, 083h, 0C4h, 010h, 06Ah, 000h, 0FFh, 074h, 024h
        db  004h, 0E8h, 086h, 017h, 000h, 000h, 083h, 0C4h, 00Ch, 0C3h
sub_4_002A:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 18h]
        mov    eax, esi
        dec    esi
        or     eax, eax
        je     SHORT loc_4_0057
        mov    eax, dword ptr [esp + 0ch]
        mov    dl, byte ptr [esp + 10h]
        mov    bl, byte ptr [esp + 14h]
loc_4_0043:
        mov    cl, byte ptr [eax]
        or     cl, cl
        je     SHORT loc_4_0057
        cmp    cl, dl
        jne    SHORT loc_4_004F
        mov    byte ptr [eax], bl
loc_4_004F:
        inc    eax
        mov    ecx, esi
        dec    esi
        or     ecx, ecx
        jne    SHORT loc_4_0043
loc_4_0057:
        pop    esi
        pop    ebx
        ret    10h
sub_4_005C:
        sub    esp, 168h
        push   esi
        push   edi
        db  0BEh, 000h, 000h, 000h, 000h
        lea    edi, [esp + 70h]
        movsd
        movsd
        movsd
        movsd
        sub    eax, eax
        movsb
        lea    edi, [esp + 81h]
        mov    ecx, 3bh
        rep stosd
        stosw
        stosb
        push   eax
        lea    eax, [esp + 10h]
        push   64h
        mov    esi, dword ptr [esp + 17ch]
        push   eax
        push   esi
        call   sub_4_176C
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_00A8
        sub    eax, eax
        jmp    NEAR PTR loc_4_01CA
loc_4_00A8:
        push   64h
        push   26h
        lea    eax, [esp + 14h]
        push   5ch
        push   eax
        call   sub_4_002A
        lea    eax, [esp + 0ch]
        push   eax
        lea    eax, [esp + 74h]
        push   eax
        call   sub_4_1597
        push   0
        push   esi
        call   sub_4_17B4
        add    esp, 8
        push   0
        lea    eax, [esp + 74h]
        push   esi
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        call   sub_4_1784
        add    esp, 10h
        cmp    eax, 2
        jne    SHORT loc_4_00F3
        sub    eax, eax
        jmp    NEAR PTR loc_4_01CA
loc_4_00F3:
        cmp    eax, 10h
        jne    SHORT loc_4_00FF
        sub    eax, eax
        jmp    NEAR PTR loc_4_01CA
loc_4_00FF:
        or     eax, eax
        je     SHORT loc_4_010A
        sub    eax, eax
        jmp    NEAR PTR loc_4_01CA
loc_4_010A:
        push   0
        push   esi
        call   sub_4_17BC
        add    esp, 8
        db  068h, 014h, 000h, 000h, 000h
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_0150
        push   0
        db  068h, 030h, 000h, 000h, 000h
        call   sub_4_1764
        add    esp, 4
        push   eax
        db  068h, 04Ch, 000h, 000h, 000h
        push   1
        db  068h, 068h, 000h, 000h, 000h
        push   0
        push   dword ptr [esp + 20h]
        call   sub_4_17CC
        add    esp, 1ch
loc_4_0150:
        db  068h, 074h, 000h, 000h, 000h
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_018B
        push   0
        db  068h, 07Ch, 000h, 000h, 000h
        call   sub_4_1764
        add    esp, 4
        push   eax
        db  068h, 084h, 000h, 000h, 000h
        push   1
        db  068h, 08Ch, 000h, 000h, 000h
        push   0
        push   dword ptr [esp + 20h]
        call   sub_4_17CC
        add    esp, 1ch
loc_4_018B:
        db  068h, 094h, 000h, 000h, 000h
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_01C6
        push   0
        db  068h, 09Ch, 000h, 000h, 000h
        call   sub_4_1764
        add    esp, 4
        push   eax
        db  068h, 0A4h, 000h, 000h, 000h
        push   1
        db  068h, 0ACh, 000h, 000h, 000h
        push   0
        push   dword ptr [esp + 20h]
        call   sub_4_17CC
        add    esp, 1ch
loc_4_01C6:
        mov    eax, dword ptr [esp + 8]
loc_4_01CA:
        pop    edi
        pop    esi
        add    esp, 168h
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_4_01E0:
        push   ebx
        push   esi
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
loc_4_01EA:
        mov    cl, byte ptr [eax]
        cmp    cl, 61h
        jl     SHORT loc_4_01FE
        cmp    cl, 7ah
        jg     SHORT loc_4_01FE
        movsx  edx, cl
        sub    edx, 20h
        jmp    SHORT loc_4_0201
loc_4_01FE:
        movsx  edx, cl
loc_4_0201:
        mov    bl, byte ptr [esi]
        cmp    bl, 61h
        jl     SHORT loc_4_0215
        cmp    bl, 7ah
        jg     SHORT loc_4_0215
        movsx  ecx, bl
        sub    ecx, 20h
        jmp    SHORT loc_4_0218
loc_4_0215:
        movsx  ecx, bl
loc_4_0218:
        cmp    edx, ecx
        jne    SHORT loc_4_0228
        or     bl, bl
        je     SHORT loc_4_0224
        inc    eax
        inc    esi
        jmp    SHORT loc_4_01EA
loc_4_0224:
        sub    eax, eax
        jmp    SHORT loc_4_0230
loc_4_0228:
        movsx  eax, byte ptr [eax]
        movsx  ecx, byte ptr [esi]
        sub    eax, ecx
loc_4_0230:
        pop    esi
        pop    ebx
        ret    8
sub_4_0235:
        sub    esp, 220h
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 20h]
        push   eax
        db  068h, 0FCh, 000h, 000h, 000h
        push   80000002h
        call   sub_4_1628
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_4_0490
        sub    ebx, ebx
        mov    esi, dword ptr [esp + 234h]
        mov    edi, dword ptr [esp + 238h]
loc_4_026E:
        mov    dword ptr [esp + 14h], 101h
        mov    eax, ebx
        lea    ecx, [esp + 12ch]
        push   101h
        push   ecx
        push   eax
        inc    ebx
        push   dword ptr [esp + 2ch]
        call   sub_4_1638
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_0484
        push   0
        lea    eax, [esp + 2ch]
        push   100h
        push   eax
        push   dword ptr [esi]
        call   sub_4_176C
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_0484
        push   100h
        push   26h
        lea    eax, [esp + 30h]
        push   5ch
        push   eax
        call   sub_4_002A
        lea    eax, [esp + 12ch]
        db  068h, 00Ch, 001h, 000h, 000h
        push   eax
        call   sub_4_1597
        lea    eax, [esp + 28h]
        push   eax
        lea    eax, [esp + 130h]
        push   eax
        call   sub_4_1597
        lea    eax, [esp + 18h]
        push   eax
        lea    eax, [esp + 130h]
        push   eax
        push   dword ptr [esp + 28h]
        call   sub_4_1628
        add    esp, 0ch
        or     eax, eax
        jne    NEAR PTR loc_4_026E
        mov    dword ptr [esp + 14h], 101h
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        push   0
        push   0
        db  068h, 010h, 001h, 000h, 000h
        push   dword ptr [esp + 2ch]
        call   sub_4_1640
        add    esp, 18h
        or     eax, eax
        jne    NEAR PTR loc_4_0467
        lea    eax, [esp + 28h]
        push   edi
        push   eax
        call   sub_4_01E0
        or     eax, eax
        jne    NEAR PTR loc_4_0467
        mov    ebp, 1
        mov    byte ptr [esp + 28h], 0
        lea    eax, [esp + 28h]
        db  068h, 0FCh, 000h, 000h, 000h
        push   eax
        call   sub_4_1597
        lea    eax, [esp + 28h]
        db  068h, 018h, 001h, 000h, 000h
        push   eax
        call   sub_4_1597
        lea    eax, [esp + 12ch]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        call   sub_4_1597
        lea    eax, [esp + 1ch]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        push   80000005h
        call   sub_4_1628
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_4_03E8
        mov    dword ptr [esp + 14h], 4
        lea    eax, [esp + 14h]
        push   eax
        lea    eax, [esp + 28h]
        push   eax
        push   0
        push   0
        db  068h, 01Ch, 001h, 000h, 000h
        push   dword ptr [esp + 30h]
        call   sub_4_1640
        add    esp, 18h
        or     eax, eax
        jne    SHORT loc_4_03DC
        test   byte ptr [esp + 24h], 2
        je     SHORT loc_4_03DC
        sub    ebp, ebp
loc_4_03DC:
        push   dword ptr [esp + 1ch]
        call   sub_4_1630
        add    esp, 4
loc_4_03E8:
        or     ebp, ebp
        je     SHORT loc_4_0467
        mov    byte ptr [esp + 28h], 0
        lea    eax, [esp + 28h]
        db  068h, 02Ch, 001h, 000h, 000h
        push   eax
        call   sub_4_1597
        lea    eax, [esp + 12ch]
        push   eax
        lea    eax, [esp + 2ch]
        push   eax
        call   sub_4_1597
        push   0
        lea    eax, [esp + 2ch]
        push   dword ptr [esi]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_4_1784
        add    esp, 10h
        or     eax, eax
        jne    SHORT loc_4_0467
        mov    ebp, dword ptr [esi + 4]
        push   ebp
        call   sub_4_1728
        or     eax, eax
        je     SHORT loc_4_0450
loc_4_0439:
        mov    ecx, dword ptr [esp + 10h]
        cmp    dword ptr [eax], ecx
        je     SHORT loc_4_044C
        push   eax
        push   ebp
        call   sub_4_1738
        or     eax, eax
        jne    SHORT loc_4_0439
loc_4_044C:
        or     eax, eax
        jne    SHORT loc_4_0467
loc_4_0450:
        push   ebp
        call   sub_4_16E0
        or     eax, eax
        je     SHORT loc_4_0478
        mov    ecx, dword ptr [esp + 10h]
        mov    dword ptr [eax], ecx
        push   eax
        push   ebp
        call   sub_4_16F4
loc_4_0467:
        push   dword ptr [esp + 18h]
        call   sub_4_1630
        add    esp, 4
        jmp    NEAR PTR loc_4_026E
loc_4_0478:
        push   dword ptr [esp + 18h]
        call   sub_4_1630
        add    esp, 4
loc_4_0484:
        push   dword ptr [esp + 20h]
        call   sub_4_1630
        add    esp, 4
loc_4_0490:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 220h
        ret    8
loc_4_049D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 48h]
        call   sub_4_15E4
        or     eax, eax
        je     SHORT loc_4_04E0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 44h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        db  0CDh, 020h, 016h, 000h, 003h, 000h
        cmp    eax, 1
        jbe    SHORT loc_4_04E0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 44h]
        db  0C7h, 004h, 085h, 0BCh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_4_0538
loc_4_04E0:
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [eax + 0ch]
        add    eax, 0ch
        or     esi, esi
        je     SHORT loc_4_051A
        mov    dword ptr [eax], 0
        push   0
        push   0
        push   6
        push   esi
        call   sub_4_1678
        add    esp, 10h
        push   0
        push   0
        push   4
        push   esi
        call   sub_4_1678
        add    esp, 10h
        push   esi
        call   sub_4_1660
        add    esp, 4
loc_4_051A:
        mov    eax, dword ptr [ebp - 4]
        db  068h, 038h, 001h, 000h, 000h
        mov    eax, dword ptr [eax + 44h]
        db  0C7h, 004h, 085h, 0BCh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_4_0235
loc_4_0538:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_4_053D:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 2ch], 0
        push   dword ptr [esp]
        call   sub_4_160A
        add    esp, 4
        ret
sub_4_055C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        add    esi, 2ch
        push   dword ptr [esi]
        call   sub_4_16D0
        mov    dword ptr [esi], 0
        mov    edx, dword ptr [esp + 8]
        call   sub_4_053D
        pop    esi
        ret    4
loc_4_057E:
        sub    esp, 4
        mov    dword ptr [esp], edx
        push   0
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax]
        call   sub_4_17AC
        add    esp, 0ch
        ret
loc_4_0596:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   dword ptr [esi + 1ch]
        call   sub_4_16D0
        mov    dword ptr [esi + 1ch], 0
        push   esi
        call   sub_4_148C
        pop    esi
        ret
loc_4_05B2:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   dword ptr [esi + 24h]
        call   sub_4_16D0
        mov    dword ptr [esi + 24h], 0
        push   esi
        call   sub_4_148C
        pop    esi
        ret
loc_4_05CE:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 14h]
        cmp    eax, 4
        je     SHORT loc_4_05EA
        cmp    eax, 5
        je     SHORT loc_4_0651
        mov    eax, 1
        jmp    NEAR PTR loc_4_066D
loc_4_05EA:
        db  0A1h, 0A0h, 001h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_4_05F5
        jmp    SHORT loc_4_066B
loc_4_05F5:
        push   eax
        call   sub_4_1728
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_4_066B
        mov    esi, dword ptr [esp + 20h]
loc_4_0605:
        mov    edi, dword ptr [ebx + 4]
        push   edi
        call   sub_4_1728
        mov    ebp, eax
        or     ebp, ebp
        je     SHORT loc_4_063D
loc_4_0614:
        cmp    dword ptr [ebp], esi
        je     SHORT loc_4_0626
        push   ebp
        push   edi
        call   sub_4_1738
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_4_0614
loc_4_0626:
        or     ebp, ebp
        je     SHORT loc_4_063D
        cmp    dword ptr [ebp], esi
        jne    SHORT loc_4_063D
        push   ebp
        push   edi
        call   sub_4_1748
        push   ebp
        push   edi
        call   sub_4_1718
loc_4_063D:
        push   ebx
        db  0FFh, 035h, 0A0h, 001h, 000h, 000h
        call   sub_4_1738
        mov    ebx, eax
        or     ebx, ebx
        jne    SHORT loc_4_0605
        jmp    SHORT loc_4_066B
loc_4_0651:
        call   sub_4_175C
        cmp    eax, 20000000h
        mov    eax, 1
        jb     SHORT loc_4_066D
        mov    ecx, dword ptr [esp + 1ch]
        call   sub_4_0672
loc_4_066B:
        sub    eax, eax
loc_4_066D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_4_0672:
        sub    esp, 10ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 10h], ecx
        mov    dword ptr [esp + 14h], 0
        mov    dword ptr [esp + 20h], 0c8h
        lea    eax, [esp + 20h]
        push   0
        push   eax
        lea    eax, [esp + 5ch]
        push   eax
        push   1
        db  068h, 040h, 001h, 000h, 000h
        push   0
        push   dword ptr [esp + 28h]
        call   sub_4_17C4
        add    esp, 1ch
        or     eax, eax
        je     SHORT loc_4_06BC
        sub    eax, eax
        jmp    NEAR PTR loc_4_08DC
loc_4_06BC:
        push   dword ptr [esp + 10h]
        call   sub_4_14C0
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_4_06D2
        sub    eax, eax
        jmp    NEAR PTR loc_4_08DC
loc_4_06D2:
        movsx  eax, byte ptr [esp + 57h]
        sub    eax, 30h
        push   2
        mov    dword ptr [esi + 48h], eax
        lea    eax, [esp + 20h]
        push   dword ptr [esp + 14h]
        push   eax
        call   sub_4_179C
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_4_06FB
        sub    eax, eax
        jmp    NEAR PTR loc_4_08DC
loc_4_06FB:
        push   0
        push   0
        push   4
        lea    eax, [esp + 24h]
        push   dword ptr [esp + 28h]
        push   eax
        call   sub_4_17A4
        add    esp, 14h
        or     eax, eax
        je     SHORT loc_4_071D
        sub    eax, eax
        jmp    NEAR PTR loc_4_08DC
loc_4_071D:
        mov    eax, dword ptr [esp + 18h]
        mov    bx, word ptr [eax + 2]
        and    ebx, 0fh
        mov    dword ptr [esi + 44h], ebx
        db  083h, 03Ch, 09Dh, 0BCh, 000h, 000h, 000h, 000h
        je     SHORT loc_4_074C
        push   esi
        push   2710h
        db  068h, 07Eh, 005h, 000h, 000h
        call   sub_4_16B4
        sub    eax, eax
        jmp    NEAR PTR loc_4_08DC
loc_4_074C:
        push   dword ptr [esi + 4]
        call   sub_4_1728
        mov    edi, eax
        or     edi, edi
        je     SHORT loc_4_077B
loc_4_075A:
        push   edi
        push   dword ptr [esi + 4]
        call   sub_4_1738
        mov    ebp, eax
        push   0
        push   dword ptr [edi]
        call   sub_4_178C
        add    esp, 8
        or     eax, eax
        jne    SHORT loc_4_07A3
        mov    edi, ebp
        or     ebp, ebp
        jne    SHORT loc_4_075A
loc_4_077B:
        mov    edi, dword ptr [esi + 40h]
        test   edi, 1
        jne    SHORT loc_4_07AA
        or     edi, 1
        mov    dword ptr [esi + 40h], edi
        push   ebx
        call   sub_4_15BB
        or     eax, eax
        je     SHORT loc_4_07C9
        or     edi, 2
        mov    dword ptr [esi + 40h], edi
        sub    eax, eax
        jmp    NEAR PTR loc_4_08DC
loc_4_07A3:
        sub    eax, eax
        jmp    NEAR PTR loc_4_08DC
loc_4_07AA:
        test   edi, 2
        je     SHORT loc_4_07C9
        push   ebx
        call   sub_4_15BB
        or     eax, eax
        je     SHORT loc_4_07C3
        sub    eax, eax
        jmp    NEAR PTR loc_4_08DC
loc_4_07C3:
        and    edi, 0fffffffdh
        mov    dword ptr [esi + 40h], edi
loc_4_07C9:
        call   sub_4_16A8
        push   eax
        lea    eax, [esp + 58h]
        push   eax
        call   sub_4_1648
        add    esp, 8
        mov    edi, eax
        cmp    edi, -0ah
        jb     SHORT loc_4_07EA
        sub    eax, eax
        jmp    NEAR PTR loc_4_08DC
loc_4_07EA:
        push   0
        push   0
        push   0
        push   100h
        push   0
        push   edi
        call   sub_4_1658
        add    esp, 18h
        mov    ecx, dword ptr [esi + 44h]
        mov    eax, dword ptr [esp + 10h]
        db  089h, 004h, 08Dh, 0BCh, 000h, 000h, 000h
        push   0
        mov    dword ptr [esi + 0ch], edi
        push   dword ptr [esp + 14h]
        lea    eax, [esp + 1ch]
        push   dword ptr [esp + 18h]
        push   eax
        call   sub_4_1774
        add    esp, 10h
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_4_083A
        push   esi
        call   sub_4_005C
        jmp    NEAR PTR loc_4_08D7
loc_4_083A:
        mov    dword ptr [esp + 24h], 30h
        mov    dword ptr [esp + 28h], 4b0h
        mov    dword ptr [esp + 2ch], 1
        mov    byte ptr [esp + 3ah], 7
        mov    byte ptr [esp + 3bh], 0
        mov    byte ptr [esp + 3ch], 0
        lea    eax, [esp + 24h]
        push   -1
        push   eax
        push   dword ptr [esi + 0ch]
        call   sub_4_1650
        add    esp, 0ch
        or     eax, eax
        jne    SHORT loc_4_0888
        mov    dword ptr [esi + 2ch], 0
        push   esi
        call   sub_4_055C
        sub    eax, eax
        jmp    SHORT loc_4_08DC
loc_4_0888:
        push   0
        push   0
        push   5
        push   edi
        call   sub_4_1678
        add    esp, 10h
        push   0
        push   0
        push   4
        push   edi
        call   sub_4_1678
        add    esp, 10h
        push   1
        push   edi
        call   sub_4_1680
        add    esp, 8
        push   esi
        push   0c8h
        db  068h, 0E7h, 008h, 000h, 000h
        call   sub_4_16B4
        mov    dword ptr [esi + 10h], eax
        push   esi
        push   2710h
        db  068h, 03Dh, 005h, 000h, 000h
        call   sub_4_16B4
        mov    dword ptr [esi + 2ch], eax
loc_4_08D7:
        mov    eax, 1
loc_4_08DC:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10ch
        ret
loc_4_08E7:
        sub    esp, 8
        push   esi
        mov    dword ptr [esp + 4], edx
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 10h], 0
        mov    esi, dword ptr [esp + 4]
        add    esi, 0ch
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     SHORT loc_4_0961
        lea    ecx, [esp + 8]
        push   ecx
        push   eax
        call   sub_4_1670
        add    esp, 8
        test   byte ptr [esp + 8], 20h
        jne    SHORT loc_4_0927
        push   dword ptr [esp + 4]
        call   sub_4_055C
        jmp    SHORT loc_4_0961
loc_4_0927:
        mov    esi, dword ptr [esi]
        push   0
        push   0
        push   4
        push   esi
        call   sub_4_1678
        add    esp, 10h
        push   0
        push   0
        push   6
        push   esi
        call   sub_4_1678
        add    esp, 10h
        push   dword ptr [esp + 4]
        push   0c8h
        db  068h, 066h, 009h, 000h, 000h
        call   sub_4_16B4
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 14h], eax
loc_4_0961:
        pop    esi
        add    esp, 8
        ret
loc_4_0966:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0ch]
        or     eax, eax
        je     SHORT loc_4_09AC
        push   0
        push   0
        push   5
        push   eax
        call   sub_4_1678
        add    esp, 10h
        push   dword ptr [esp]
        push   0c8h
        db  068h, 0B0h, 009h, 000h, 000h
        call   sub_4_16B4
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ecx + 18h], eax
loc_4_09AC:
        add    esp, 4
        ret
loc_4_09B0:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 18h], 0
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0ch]
        or     eax, eax
        je     SHORT loc_4_0A10
        push   0
        push   0
        push   3
        push   eax
        call   sub_4_1678
        add    esp, 10h
        push   dword ptr [esp]
        push   0c8h
        db  068h, 014h, 00Ah, 000h, 000h
        call   sub_4_16B4
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ecx + 1ch], eax
        push   dword ptr [esp]
        db  068h, 096h, 005h, 000h, 000h
        mov    eax, dword ptr [esp + 8]
        push   1
        push   dword ptr [eax + 0ch]
        call   sub_4_1690
        add    esp, 10h
loc_4_0A10:
        add    esp, 4
        ret
loc_4_0A14:
        sub    esp, 4
        push   esi
        mov    dword ptr [esp + 4], edx
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 1ch], 0
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0ch]
        or     eax, eax
        je     SHORT loc_4_0A80
        push   0
        push   0
        push   -1
        push   eax
        call   sub_4_1690
        mov    eax, dword ptr [esp + 14h]
        add    esp, 10h
        mov    esi, dword ptr [eax + 0ch]
        push   0
        push   0
        push   6
        push   esi
        call   sub_4_1678
        add    esp, 10h
        push   0
        push   0
        push   4
        push   esi
        call   sub_4_1678
        add    esp, 10h
        push   dword ptr [esp + 4]
        push   0c8h
        db  068h, 085h, 00Ah, 000h, 000h
        call   sub_4_16B4
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 20h], eax
loc_4_0A80:
        pop    esi
        add    esp, 4
        ret
loc_4_0A85:
        sub    esp, 4
        push   esi
        mov    dword ptr [esp + 4], edx
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [esp + 4]
        mov    esi, dword ptr [eax + 0ch]
        or     esi, esi
        je     SHORT loc_4_0AF5
        push   0
        push   0
        push   3
        push   esi
        call   sub_4_1678
        add    esp, 10h
        push   0
        push   0
        push   5
        push   esi
        call   sub_4_1678
        add    esp, 10h
        push   dword ptr [esp + 4]
        push   0c8h
        db  068h, 0FAh, 00Ah, 000h, 000h
        call   sub_4_16B4
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 24h], eax
        push   dword ptr [esp + 4]
        db  068h, 0B2h, 005h, 000h, 000h
        mov    eax, dword ptr [esp + 0ch]
        push   1
        push   dword ptr [eax + 0ch]
        call   sub_4_1690
        add    esp, 10h
loc_4_0AF5:
        pop    esi
        add    esp, 4
        ret
loc_4_0AFA:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0ch]
        or     eax, eax
        je     SHORT loc_4_0B2F
        push   0
        push   0
        push   -1
        push   eax
        call   sub_4_1690
        add    esp, 10h
        push   dword ptr [esp]
        call   sub_4_055C
loc_4_0B2F:
        add    esp, 4
        ret
sub_4_0B33:
        push   1
        db  068h, 0CEh, 005h, 000h, 000h
        push   ecx
        call   sub_4_1794
        add    esp, 0ch
        ret
sub_4_0B44:
        push   ebx
        push   esi
        push   edi
        mov    ecx, 64h
        mov    esi, dword ptr [esp + 10h]
        mov    eax, esi
        cdq
        idiv   ecx
        mov    ebx, eax
        cmp    ebx, 0ah
        jl     SHORT loc_4_0B71
        mov    ecx, 0ah
        mov    eax, ebx
        cdq
        idiv   ecx
        mov    edi, dword ptr [esp + 14h]
        add    al, 30h
        mov    byte ptr [edi], al
        inc    edi
        jmp    SHORT loc_4_0B75
loc_4_0B71:
        mov    edi, dword ptr [esp + 14h]
loc_4_0B75:
        mov    ecx, 0ah
        mov    eax, ebx
        cdq
        idiv   ecx
        add    dl, 30h
        mov    byte ptr [edi], dl
        inc    edi
        mov    ecx, 64h
        mov    byte ptr [edi], 2eh
        inc    edi
        mov    eax, esi
        cdq
        idiv   ecx
        mov    esi, edx
        mov    ecx, 0ah
        mov    eax, esi
        cdq
        idiv   ecx
        add    al, 30h
        mov    byte ptr [edi], al
        inc    edi
        mov    eax, esi
        cdq
        idiv   ecx
        add    dl, 30h
        mov    byte ptr [edi], dl
        mov    byte ptr [edi + 1], 0
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_4_0BB8:
        push   ebx
        push   esi
        mov    ecx, dword ptr [esp + 0ch]
        mov    eax, 1
        mov    esi, dword ptr [ecx + 3ch]
        mov    edx, dword ptr [esp + 10h]
        mov    bl, byte ptr [esi]
loc_4_0BCC:
        cmp    bl, 28h
        je     SHORT loc_4_0BE2
        cmp    bl, 8
        je     SHORT loc_4_0BE2
        mov    byte ptr [edx], bl
        inc    edx
        mov    bl, byte ptr [eax + esi]
        inc    eax
        cmp    eax, 11h
        jl     SHORT loc_4_0BCC
loc_4_0BE2:
        dec    eax
        mov    dword ptr [ecx + 38h], eax
        mov    byte ptr [edx], 0
        pop    esi
        pop    ebx
        ret    8
sub_4_0BEE:
        mov    al, byte ptr [esp + 4]
        cmp    al, 30h
        jl     SHORT loc_4_0C02
        cmp    al, 39h
        jg     SHORT loc_4_0C02
        movsx  eax, al
        sub    eax, 30h
        jmp    SHORT loc_4_0C27
loc_4_0C02:
        cmp    al, 41h
        jl     SHORT loc_4_0C12
        cmp    al, 46h
        jg     SHORT loc_4_0C12
        movsx  eax, al
        sub    eax, 37h
        jmp    SHORT loc_4_0C27
loc_4_0C12:
        cmp    al, 61h
        jl     SHORT loc_4_0C22
        cmp    al, 66h
        jg     SHORT loc_4_0C22
        movsx  eax, al
        sub    eax, 57h
        jmp    SHORT loc_4_0C27
loc_4_0C22:
        mov    eax, 0ffffffffh
loc_4_0C27:
        ret    4
sub_4_0C2A:
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 2ch], 0
        je     NEAR PTR loc_4_0DAD
        mov    esi, dword ptr [esp + 28h]
        or     esi, esi
        je     NEAR PTR loc_4_0DAD
        mov    eax, dword ptr [esp + 2ch]
        mov    byte ptr [eax], 0
        mov    edi, dword ptr [esi + 38h]
        mov    ebp, dword ptr [esi + 3ch]
        mov    bl, byte ptr [edi + ebp]
        or     bl, bl
        jne    SHORT loc_4_0C61
        jmp    NEAR PTR loc_4_0DAD
loc_4_0C61:
        inc    edi
        cmp    edi, 100h
        jge    SHORT loc_4_0C81
loc_4_0C6A:
        cmp    bl, 28h
        je     SHORT loc_4_0C90
        cmp    bl, 8
        je     SHORT loc_4_0C81
        mov    bl, byte ptr [ebp + edi]
        inc    edi
        cmp    edi, 100h
        jl     SHORT loc_4_0C6A
loc_4_0C81:
        cmp    bl, 28h
        je     SHORT loc_4_0C90
        cmp    bl, 8
        je     SHORT loc_4_0C90
        jmp    NEAR PTR loc_4_0DAD
loc_4_0C90:
        mov    al, 28h
        sub    al, bl
        mov    byte ptr [esp + 13h], al
        movsx  ecx, al
        movsx  eax, byte ptr [ebp + edi + 9]
        add    eax, ecx
        cmp    eax, 29h
        je     NEAR PTR loc_4_0D45
        movsx  eax, bl
        mov    dword ptr [esp + 1ch], eax
        mov    dword ptr [esp + 14h], edi
        cmp    edi, 100h
        jge    SHORT loc_4_0CE5
loc_4_0CBE:
        mov    eax, 29h
        movsx  edx, bl
        sub    eax, edx
        cmp    eax, ecx
        je     SHORT loc_4_0CE5
        mov    eax, dword ptr [esp + 14h]
        mov    bl, byte ptr [ebp + eax]
        inc    dword ptr [esp + 14h]
        movsx  eax, bl
        cmp    dword ptr [esp + 14h], 100h
        jl     SHORT loc_4_0CBE
loc_4_0CE5:
        mov    edx, dword ptr [esp + 14h]
        add    edx, ebp
        movsx  eax, byte ptr [edx - 3]
        movsx  edx, byte ptr [edx - 2]
        mov    dword ptr [esp + 20h], edx
        add    eax, ecx
        add    dword ptr [esp + 20h], ecx
        push   eax
        call   sub_4_0BEE
        or     eax, eax
        jge    SHORT loc_4_0D0C
        jmp    NEAR PTR loc_4_0DAD
loc_4_0D0C:
        push   dword ptr [esp + 20h]
        call   sub_4_0BEE
        or     eax, eax
        jge    SHORT loc_4_0D1E
        jmp    NEAR PTR loc_4_0DAD
loc_4_0D1E:
        mov    al, byte ptr [esp + 14h]
        movsx  ecx, bl
        sub    ecx, dword ptr [esp + 1ch]
        cmp    ecx, 1
        je     SHORT loc_4_0D30
        jmp    SHORT loc_4_0DAD
loc_4_0D30:
        movsx  ecx, al
        mov    al, 29h
        sub    al, byte ptr [esp + 13h]
        mov    byte ptr [ebp + ecx - 3], al
        mov    eax, dword ptr [esi + 3ch]
        mov    byte ptr [ecx + eax - 2], 0
loc_4_0D45:
        mov    edx, dword ptr [esi + 3ch]
        lea    eax, [edi + edx]
        mov    cl, byte ptr [eax]
        cmp    cl, 3fh
        jg     SHORT loc_4_0DAD
        cmp    byte ptr [eax + 1], 3fh
        jg     SHORT loc_4_0DAD
        and    ecx, 3fh
        shl    ecx, 6
        inc    edi
        mov    al, byte ptr [edx + edi]
        and    eax, 3fh
        or     ecx, eax
        inc    edi
        push   dword ptr [esp + 2ch]
        push   ecx
        call   sub_4_0B44
        mov    edx, edi
        cmp    edi, 100h
        jge    SHORT loc_4_0DA6
loc_4_0D7C:
        cmp    byte ptr [esp + 13h], 0
        je     SHORT loc_4_0DA6
        mov    eax, dword ptr [esi + 3ch]
        mov    cl, byte ptr [esp + 13h]
        add    byte ptr [eax + edx], cl
        mov    eax, dword ptr [esi + 3ch]
        inc    edx
        mov    al, byte ptr [eax + edx - 1]
        cmp    al, 29h
        jne    SHORT loc_4_0D9E
        mov    byte ptr [esp + 13h], 0
loc_4_0D9E:
        cmp    edx, 100h
        jl     SHORT loc_4_0D7C
loc_4_0DA6:
        mov    dword ptr [esi + 38h], edi
        sub    eax, eax
        jmp    SHORT loc_4_0DB2
loc_4_0DAD:
        mov    eax, 1
loc_4_0DB2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 14h
        ret    8
sub_4_0DBC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 1ch]
        or     edi, edi
        je     NEAR PTR loc_4_0E62
        mov    esi, dword ptr [esp + 18h]
        or     esi, esi
        je     NEAR PTR loc_4_0E62
        mov    ebp, dword ptr [esi + 38h]
        mov    ebx, dword ptr [esi + 3ch]
        mov    al, byte ptr [ebp + ebx]
        mov    byte ptr [edi], al
        inc    ebp
        inc    edi
        mov    al, byte ptr [ebx + ebp]
        mov    byte ptr [edi], al
        inc    ebp
        inc    edi
        mov    al, byte ptr [ebx + ebp]
        mov    byte ptr [edi], al
        inc    ebp
        inc    edi
        mov    al, byte ptr [ebx + ebp]
        mov    byte ptr [esp + 13h], al
        inc    ebp
        push   dword ptr [esp + 13h]
        call   sub_4_0BEE
        or     eax, eax
        jl     SHORT loc_4_0E13
        mov    al, byte ptr [esp + 13h]
        mov    byte ptr [edi], al
        inc    edi
loc_4_0E13:
        mov    al, byte ptr [ebx + ebp]
        mov    byte ptr [esp + 13h], al
        inc    ebp
        push   dword ptr [esp + 13h]
        call   sub_4_0BEE
        or     eax, eax
        jl     SHORT loc_4_0E2F
        mov    al, byte ptr [esp + 13h]
        mov    byte ptr [edi], al
        inc    edi
loc_4_0E2F:
        mov    al, byte ptr [ebx + ebp]
        mov    byte ptr [esp + 13h], al
        inc    ebp
        push   dword ptr [esp + 13h]
        call   sub_4_0BEE
        or     eax, eax
        jl     SHORT loc_4_0E4B
        mov    al, byte ptr [esp + 13h]
        mov    byte ptr [edi], al
        inc    edi
loc_4_0E4B:
        mov    bl, byte ptr [ebx + ebp]
        inc    ebp
        push   ebx
        call   sub_4_0BEE
        or     eax, eax
        jl     SHORT loc_4_0E5C
        mov    byte ptr [edi], bl
        inc    edi
loc_4_0E5C:
        mov    byte ptr [edi], 0
        mov    dword ptr [esi + 38h], ebp
loc_4_0E62:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
sub_4_0E6C:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        or     esi, esi
        je     SHORT loc_4_0EDA
        cmp    dword ptr [esp + 18h], 0
        je     SHORT loc_4_0EDA
        mov    eax, dword ptr [esp + 18h]
        mov    byte ptr [esi], 0
        mov    edi, dword ptr [eax + 38h]
        mov    ecx, edi
        inc    edi
        mov    eax, dword ptr [eax + 3ch]
        mov    dword ptr [esp + 10h], eax
        cmp    byte ptr [ecx + eax], 5ch
        jne    SHORT loc_4_0EDA
        mov    bl, byte ptr [eax + edi]
        inc    edi
        sub    ebp, ebp
loc_4_0EA2:
        cmp    edi, 100h
        jge    SHORT loc_4_0ECF
        cmp    bl, 5ch
        je     SHORT loc_4_0ECF
        cmp    bl, 29h
        je     SHORT loc_4_0ECF
        inc    ebp
        push   ebx
        call   sub_4_0BEE
        or     eax, eax
        jl     SHORT loc_4_0ECF
        mov    byte ptr [esi], bl
        inc    esi
        mov    eax, dword ptr [esp + 10h]
        mov    bl, byte ptr [edi + eax]
        inc    edi
        cmp    ebp, 8
        jl     SHORT loc_4_0EA2
loc_4_0ECF:
        mov    byte ptr [esi], 0
        mov    eax, dword ptr [esp + 18h]
        dec    edi
        mov    dword ptr [eax + 38h], edi
loc_4_0EDA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
sub_4_0EE4:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 14h]
        or     edx, edx
        je     SHORT loc_4_0F35
        mov    ecx, dword ptr [esp + 10h]
        or     ecx, ecx
        je     SHORT loc_4_0F35
        mov    byte ptr [edx], 0
        mov    edi, dword ptr [ecx + 38h]
        mov    esi, edi
        inc    edi
        mov    eax, dword ptr [ecx + 3ch]
        cmp    byte ptr [esi + eax], 5ch
        jne    SHORT loc_4_0F35
        mov    bl, byte ptr [eax + edi]
        inc    edi
        cmp    edi, 100h
        jge    SHORT loc_4_0F2E
loc_4_0F15:
        cmp    bl, 5ch
        je     SHORT loc_4_0F2E
        cmp    bl, 29h
        je     SHORT loc_4_0F2E
        mov    byte ptr [edx], bl
        inc    edx
        mov    bl, byte ptr [eax + edi]
        inc    edi
        cmp    edi, 100h
        jl     SHORT loc_4_0F15
loc_4_0F2E:
        mov    byte ptr [edx], 0
        dec    edi
        mov    dword ptr [ecx + 38h], edi
loc_4_0F35:
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_4_0F3B:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 14h]
        or     eax, eax
        je     SHORT loc_4_0F8C
        mov    edi, dword ptr [esp + 10h]
        or     edi, edi
        je     SHORT loc_4_0F8C
        mov    byte ptr [eax], 0
        mov    ecx, dword ptr [edi + 38h]
        mov    esi, ecx
        inc    ecx
        mov    edx, dword ptr [edi + 3ch]
        cmp    byte ptr [esi + edx], 5ch
        jne    SHORT loc_4_0F8C
        mov    bl, byte ptr [ecx + edx]
        inc    ecx
        cmp    ecx, 100h
        jge    SHORT loc_4_0F85
loc_4_0F6C:
        cmp    bl, 5ch
        je     SHORT loc_4_0F85
        cmp    bl, 29h
        je     SHORT loc_4_0F85
        mov    byte ptr [eax], bl
        inc    eax
        mov    bl, byte ptr [ecx + edx]
        inc    ecx
        cmp    ecx, 100h
        jl     SHORT loc_4_0F6C
loc_4_0F85:
        mov    byte ptr [eax], 0
        dec    ecx
        mov    dword ptr [edi + 38h], ecx
loc_4_0F8C:
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_4_0F92:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 14h]
        or     ecx, ecx
        je     SHORT loc_4_0FE3
        mov    eax, dword ptr [esp + 10h]
        or     eax, eax
        je     SHORT loc_4_0FE3
        mov    byte ptr [ecx], 0
        mov    edi, dword ptr [eax + 38h]
        mov    esi, edi
        inc    edi
        mov    edx, dword ptr [eax + 3ch]
        cmp    byte ptr [esi + edx], 5ch
        jne    SHORT loc_4_0FE3
        mov    bl, byte ptr [edx + edi]
        inc    edi
        cmp    edi, 100h
        jge    SHORT loc_4_0FDC
loc_4_0FC3:
        cmp    bl, 5ch
        je     SHORT loc_4_0FDC
        cmp    bl, 29h
        je     SHORT loc_4_0FDC
        mov    byte ptr [ecx], bl
        inc    ecx
        mov    bl, byte ptr [edx + edi]
        inc    edi
        cmp    edi, 100h
        jl     SHORT loc_4_0FC3
loc_4_0FDC:
        mov    byte ptr [ecx], 0
        dec    edi
        mov    dword ptr [eax + 38h], edi
loc_4_0FE3:
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_0FE9:
        sub    esp, 908h
        push   ebx
        push   esi
        push   edi
        db  0BEh, 04Ch, 001h, 000h, 000h
        lea    edi, [esp + 110h]
        movsd
        movsd
        movsb
        lea    edi, [esp + 119h]
        sub    eax, eax
        mov    ecx, 3dh
        rep stosd
        stosw
        stosb
        mov    esi, dword ptr [esp + 918h]
        cmp    dword ptr [esi + 3ch], -1
        je     NEAR PTR loc_4_13C6
        mov    ebx, dword ptr [esi]
        lea    eax, [esp + 710h]
        push   eax
        push   esi
        call   sub_4_0BB8
        mov    edi, eax
        cmp    edi, 10h
        jg     NEAR PTR loc_4_13B2
        lea    eax, [esp + 410h]
        push   eax
        push   esi
        call   sub_4_0C2A
        or     eax, eax
        jne    NEAR PTR loc_4_13B2
        lea    eax, [esp + 610h]
        push   eax
        push   esi
        call   sub_4_0DBC
        lea    eax, [esp + 610h]
        push   eax
        call   sub_4_1764
        add    esp, 4
        cmp    eax, 7
        jne    NEAR PTR loc_4_13B2
        lea    eax, [esp + 610h]
        push   eax
        lea    eax, [esp + 114h]
        push   eax
        call   sub_4_1597
        lea    eax, [esp + 10h]
        push   eax
        push   esi
        call   sub_4_0E6C
        lea    eax, [esp + 10h]
        push   eax
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_10C4
        lea    eax, [esp + 10h]
        push   eax
        call   sub_4_1764
        add    esp, 4
        cmp    eax, 8
        jne    NEAR PTR loc_4_13B2
loc_4_10C4:
        lea    eax, [esp + 210h]
        push   eax
        push   esi
        call   sub_4_0EE4
        lea    eax, [esp + 210h]
        push   eax
        call   sub_4_1764
        add    esp, 4
        cmp    eax, 20h
        ja     NEAR PTR loc_4_13B2
        mov    byte ptr [esp + 810h], 2ah
        lea    eax, [esp + 811h]
        push   eax
        push   esi
        call   sub_4_0F3B
        lea    eax, [esp + 811h]
        push   eax
        call   sub_4_1764
        add    esp, 4
        cmp    eax, 28h
        ja     NEAR PTR loc_4_13B2
        lea    eax, [esp + 310h]
        push   eax
        push   esi
        call   sub_4_0F92
        lea    eax, [esp + 310h]
        push   eax
        call   sub_4_1764
        add    esp, 4
        cmp    eax, 28h
        ja     NEAR PTR loc_4_13B2
        lea    eax, [esp + 110h]
        db  068h, 058h, 001h, 000h, 000h
        push   eax
        call   sub_4_1597
        push   0
        lea    eax, [esp + 514h]
        push   64h
        push   eax
        push   ebx
        call   sub_4_176C
        add    esp, 10h
        or     eax, eax
        jne    NEAR PTR loc_4_13B2
        push   64h
        push   26h
        lea    eax, [esp + 518h]
        push   5ch
        push   eax
        call   sub_4_002A
        lea    eax, [esp + 510h]
        push   eax
        lea    eax, [esp + 114h]
        push   eax
        call   sub_4_1597
        push   0
        push   ebx
        call   sub_4_17B4
        add    esp, 8
        push   0
        lea    eax, [esp + 114h]
        push   ebx
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        call   sub_4_1784
        add    esp, 10h
        cmp    eax, 2
        je     NEAR PTR loc_4_13B2
        cmp    eax, 10h
        je     NEAR PTR loc_4_13B2
        or     eax, eax
        jne    NEAR PTR loc_4_13B2
        push   0
        push   ebx
        call   sub_4_17BC
        add    esp, 8
        mov    ebx, dword ptr [esi + 4]
        push   ebx
        call   sub_4_1728
        or     eax, eax
        je     SHORT loc_4_1204
loc_4_11ED:
        mov    ecx, dword ptr [esp + 0ch]
        cmp    dword ptr [eax], ecx
        je     SHORT loc_4_1200
        push   eax
        push   ebx
        call   sub_4_1738
        or     eax, eax
        jne    SHORT loc_4_11ED
loc_4_1200:
        or     eax, eax
        jne    SHORT loc_4_121F
loc_4_1204:
        push   ebx
        call   sub_4_16E0
        or     eax, eax
        je     NEAR PTR loc_4_13B2
        mov    ecx, dword ptr [esp + 0ch]
        mov    dword ptr [eax], ecx
        push   eax
        push   ebx
        call   sub_4_16F4
loc_4_121F:
        lea    eax, [esp + 310h]
        push   eax
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_1263
        lea    eax, [esp + 310h]
        push   0
        push   eax
        call   sub_4_1764
        add    esp, 4
        push   eax
        lea    eax, [esp + 318h]
        push   eax
        push   1
        db  068h, 05Ch, 001h, 000h, 000h
        push   0
        push   dword ptr [esp + 24h]
        call   sub_4_17CC
        add    esp, 1ch
loc_4_1263:
        lea    eax, [esp + 210h]
        push   eax
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_12A7
        lea    eax, [esp + 210h]
        push   0
        push   eax
        call   sub_4_1764
        add    esp, 4
        push   eax
        lea    eax, [esp + 218h]
        push   eax
        push   1
        db  068h, 068h, 001h, 000h, 000h
        push   0
        push   dword ptr [esp + 24h]
        call   sub_4_17CC
        add    esp, 1ch
loc_4_12A7:
        lea    eax, [esp + 811h]
        push   eax
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_130F
        cmp    byte ptr [esp + 811h], 2ah
        jne    SHORT loc_4_12CE
        lea    eax, [esp + 811h]
        jmp    SHORT loc_4_12D5
loc_4_12CE:
        lea    eax, [esp + 810h]
loc_4_12D5:
        push   eax
        call   sub_4_1764
        add    esp, 4
        cmp    byte ptr [esp + 811h], 2ah
        lea    ecx, [esp + 811h]
        je     SHORT loc_4_12F6
        lea    ecx, [esp + 810h]
loc_4_12F6:
        push   0
        push   eax
        push   ecx
        push   1
        db  068h, 070h, 001h, 000h, 000h
        push   0
        push   dword ptr [esp + 24h]
        call   sub_4_17CC
        add    esp, 1ch
loc_4_130F:
        lea    eax, [esp + 10h]
        push   eax
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_134A
        lea    eax, [esp + 10h]
        push   0
        push   eax
        call   sub_4_1764
        add    esp, 4
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        push   1
        db  068h, 080h, 001h, 000h, 000h
        push   0
        push   dword ptr [esp + 24h]
        call   sub_4_17CC
        add    esp, 1ch
loc_4_134A:
        lea    eax, [esp + 410h]
        push   eax
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     SHORT loc_4_138E
        lea    eax, [esp + 410h]
        push   0
        push   eax
        call   sub_4_1764
        add    esp, 4
        push   eax
        lea    eax, [esp + 418h]
        push   eax
        push   1
        db  068h, 090h, 001h, 000h, 000h
        push   0
        push   dword ptr [esp + 24h]
        call   sub_4_17CC
        add    esp, 1ch
loc_4_138E:
        or     edi, edi
        je     SHORT loc_4_13B2
        push   0
        lea    eax, [esp + 714h]
        push   edi
        push   eax
        push   3
        db  068h, 098h, 001h, 000h, 000h
        push   0
        push   dword ptr [esp + 24h]
        call   sub_4_17CC
        add    esp, 1ch
loc_4_13B2:
        push   0
        push   dword ptr [esi + 3ch]
        call   sub_4_16A0
        add    esp, 8
        mov    dword ptr [esi + 3ch], 0ffffffffh
loc_4_13C6:
        push   esi
        call   sub_4_055C
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 908h
        ret
loc_4_13D6:
        sub    esp, 8
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        cmp    dword ptr [eax + 0ch], 0
        je     NEAR PTR loc_4_1488
        push   0
        push   100h
        call   sub_4_1698
        mov    ecx, dword ptr [esp + 8]
        add    esp, 8
        mov    dword ptr [ecx + 3ch], eax
        mov    eax, dword ptr [esp]
        cmp    dword ptr [eax + 3ch], 0
        je     SHORT loc_4_147F
        lea    eax, [esp + 4]
        push   eax
        mov    eax, dword ptr [esp + 4]
        push   0
        push   dword ptr [eax + 0ch]
        call   sub_4_1668
        mov    eax, dword ptr [esp + 0ch]
        add    esp, 0ch
        add    eax, 34h
        push   eax
        mov    eax, dword ptr [esp + 4]
        push   100h
        push   dword ptr [eax + 3ch]
        push   dword ptr [eax + 0ch]
        call   sub_4_1688
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_4_1463
        mov    eax, dword ptr [esp]
        cmp    dword ptr [eax + 34h], 0
        je     SHORT loc_4_1463
        push   0
        push   dword ptr [esp + 4]
        db  068h, 0E9h, 00Fh, 000h, 000h
        call   sub_4_17D4
        add    esp, 0ch
        jmp    SHORT loc_4_1488
loc_4_1463:
        mov    eax, dword ptr [esp]
        push   0
        push   dword ptr [eax + 3ch]
        call   sub_4_16A0
        mov    eax, dword ptr [esp + 8]
        add    esp, 8
        mov    dword ptr [eax + 3ch], 0ffffffffh
loc_4_147F:
        push   dword ptr [esp]
        call   sub_4_055C
loc_4_1488:
        add    esp, 8
        ret
sub_4_148C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi + 2ch]
        call   sub_4_16D0
        mov    dword ptr [esi + 2ch], 0
        push   esi
        push   8fch
        db  068h, 0D6h, 013h, 000h, 000h
        call   sub_4_16B4
        mov    dword ptr [esi + 30h], eax
        pop    esi
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_4_14C0:
        push   esi
        push   edi
        db  0A1h, 0A0h, 001h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_4_14E4
        push   4ch
        push   4
        call   sub_4_1704
        db  0A3h, 0A0h, 001h, 000h, 000h
        or     eax, eax
        jne    SHORT loc_4_14E4
        sub    eax, eax
        jmp    NEAR PTR loc_4_156D
loc_4_14E4:
        db  0A3h, 0A0h, 001h, 000h, 000h
        push   eax
        call   sub_4_1728
        mov    edi, eax
        or     edi, edi
        mov    esi, dword ptr [esp + 0ch]
        je     SHORT loc_4_151D
loc_4_14F9:
        cmp    dword ptr [edi], esi
        je     SHORT loc_4_1511
        push   edi
        db  0FFh, 035h, 0A0h, 001h, 000h, 000h
        call   sub_4_1738
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_4_14F9
        jmp    SHORT loc_4_151D
loc_4_1511:
        cmp    dword ptr [edi + 3ch], -1
        jne    SHORT loc_4_1519
        jmp    SHORT loc_4_156B
loc_4_1519:
        sub    eax, eax
        jmp    SHORT loc_4_156D
loc_4_151D:
        or     edi, edi
        jne    SHORT loc_4_156B
        db  0FFh, 035h, 0A0h, 001h, 000h, 000h
        call   sub_4_16E0
        mov    edi, eax
        or     edi, edi
        jne    SHORT loc_4_1536
        sub    eax, eax
        jmp    SHORT loc_4_156D
loc_4_1536:
        mov    dword ptr [edi + 3ch], 0ffffffffh
        mov    dword ptr [edi], esi
        mov    dword ptr [edi + 40h], 0
        push   4
        push   4
        call   sub_4_1704
        mov    dword ptr [edi + 4], eax
        or     eax, eax
        push   edi
        db  0FFh, 035h, 0A0h, 001h, 000h, 000h
        jne    SHORT loc_4_1566
        call   sub_4_1718
        sub    eax, eax
        jmp    SHORT loc_4_156D
loc_4_1566:
        call   sub_4_16F4
loc_4_156B:
        mov    eax, edi
loc_4_156D:
        pop    edi
        pop    esi
        ret    4
sub_4_1572:
        mov    eax, 1
        ret
sub_4_1578:
        cmp    dword ptr [esp + 8], 0
        mov    eax, 1
        jne    SHORT loc_4_158D
        mov    ecx, dword ptr [esp + 4]
        call   sub_4_0B33
loc_4_158D:
        ret    8
loc_4_1590:
        mov    eax, 400h
        clc
        ret
sub_4_1597:
        push   dword ptr [esp + 4]
        db  0CDh, 020h, 07Ch, 001h, 001h, 000h
        add    esp, 4
        push   -1
        push   dword ptr [esp + 0ch]
        push   edx
        db  0CDh, 020h, 07Bh, 001h, 001h, 000h
        add    esp, 0ch
        mov    eax, dword ptr [esp + 4]
        ret    8
sub_4_15BB:
        mov    eax, dword ptr [esp + 4]
        mov    edx, eax
        db  0CDh, 020h, 00Bh, 000h, 003h, 000h
        jb     SHORT loc_4_15E1
        and    ecx, 14h
        xor    ecx, 14h
        mov    eax, ecx
        jecxz  loc_4_15E1
        movzx  ecx, byte ptr [5fch]
        bt     ecx, edx
        jb     SHORT loc_4_15E1
        xor    eax, eax
loc_4_15E1:
        ret    4
sub_4_15E4:
        db  0CDh, 020h, 000h, 000h, 00Ch, 000h
        cmp    eax, 400h
        jb     SHORT loc_4_1605
        db  0CDh, 020h, 007h, 000h, 00Ch, 000h
        cmp    ah, 2
        jne    SHORT loc_4_1605
        movzx  edx, dl
        cmp    edx, dword ptr [esp + 4]
        je     SHORT loc_4_1607
loc_4_1605:
        xor    eax, eax
loc_4_1607:
        ret    4
sub_4_160A:
        push   esi
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 4bh
        db  0BEh, 09Dh, 004h, 000h, 000h
        mov    edx, dword ptr [esp + 8]
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        pop    esi
        ret    4
        db  000h
sub_4_1628:
        db  0CDh, 020h, 048h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_4_1630:
        db  0CDh, 020h, 049h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_4_1638:
        db  0CDh, 020h, 04Ch, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_4_1640:
        db  0CDh, 020h, 051h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_4_1648:
        db  0CDh, 020h, 004h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_4_1650:
        db  0CDh, 020h, 005h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_4_1658:
        db  0CDh, 020h, 007h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_4_1660:
        db  0CDh, 020h, 009h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_4_1668:
        db  0CDh, 020h, 00Bh, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_4_1670:
        db  0CDh, 020h, 00Ch, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_4_1678:
        db  0CDh, 020h, 00Eh, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_4_1680:
        db  0CDh, 020h, 00Fh, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_4_1688:
        db  0CDh, 020h, 013h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_4_1690:
        db  0CDh, 020h, 017h, 080h, 02Bh, 000h
        add    byte ptr [eax], al
sub_4_1698:
        db  0CDh, 020h, 04Fh, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_4_16A0:
        db  0CDh, 020h, 051h, 080h, 001h, 000h
        add    byte ptr [eax], al
sub_4_16A8:
        push   ebx
        db  0CDh, 020h, 003h, 000h, 001h, 000h
        mov    eax, ebx
        pop    ebx
        ret
        db  000h
sub_4_16B4:
        push   esi
        mov    edx, dword ptr [esp + 10h]
        mov    eax, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret    0ch
        db  000h, 000h, 000h
sub_4_16D0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret    4
        db  000h
sub_4_16E0:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 09Dh, 000h, 001h, 000h
        mov    esi, ecx
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  000h
sub_4_16F4:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 09Eh, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  000h
sub_4_1704:
        pop    edx
        pop    eax
        pop    ecx
        push   esi
        db  0CDh, 020h, 09Bh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        cmc
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
sub_4_1718:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A2h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  000h
sub_4_1728:
        pop    edx
        mov    ecx, esi
        pop    esi
        db  0CDh, 020h, 0A3h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  000h, 000h
sub_4_1738:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A4h, 000h, 001h, 000h
        mov    esi, ecx
        jmp    edx
        db  000h
sub_4_1748:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        db  0CDh, 020h, 0A1h, 000h, 001h, 000h
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
        db  000h, 000h
sub_4_175C:
        db  0CDh, 020h, 011h, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_4_1764:
        db  0CDh, 020h, 07Ch, 081h, 001h, 000h
        add    byte ptr [eax], al
sub_4_176C:
        db  0CDh, 020h, 007h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_1774:
        db  0CDh, 020h, 009h, 080h, 033h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 00Ah, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_1784:
        db  0CDh, 020h, 00Bh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_178C:
        db  0CDh, 020h, 00Dh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_1794:
        db  0CDh, 020h, 00Fh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_179C:
        db  0CDh, 020h, 01Ah, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_17A4:
        db  0CDh, 020h, 01Fh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_17AC:
        db  0CDh, 020h, 033h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_17B4:
        db  0CDh, 020h, 035h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_17BC:
        db  0CDh, 020h, 037h, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_17C4:
        db  0CDh, 020h, 03Eh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_17CC:
        db  0CDh, 020h, 03Fh, 080h, 033h, 000h
        add    byte ptr [eax], al
sub_4_17D4:
        db  0CDh, 020h, 04Eh, 080h, 033h, 025h
_PTEXT ends
        end
