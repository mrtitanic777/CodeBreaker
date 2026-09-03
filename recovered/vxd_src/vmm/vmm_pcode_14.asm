; vmm_pcode_14.asm -- VMM object 14 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_14_0000:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_14_0015:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 023h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    8
loc_14_002A:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 024h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    0ch
loc_14_003F:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 025h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    18h
loc_14_0054:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 026h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_14_0069:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 033h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    18h
loc_14_007E:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 034h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    14h
loc_14_0093:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 048h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    14h
loc_14_00A8:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 04Ah, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    14h
loc_14_00BD:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 049h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    0ch
loc_14_00D2:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 04Bh, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_14_00E7:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 04Eh, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    1ch
loc_14_00FC:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 04Fh, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_14_0111:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 04Dh, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    18h
loc_14_0126:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 04Ch, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    18h
loc_14_013B:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 050h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    28h
loc_14_0150:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 051h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    20h
loc_14_0165:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 052h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    20h
loc_14_017A:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 055h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    0ch
loc_14_018F:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 071h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    10h
loc_14_01A4:
        pop    ebx
        pop    esi
        pop    edi
        db  0CDh, 020h, 077h, 001h, 001h, 000h
        mov    dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   ebx
        ret    1ch
loc_14_01B9:
        pop    eax
        pop    esi
        pop    edi
        mov    dword ptr [esi + 1ch], eax
        db  0E8h, 09Fh, 053h, 0F2h, 0FFh
        xchg   dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   eax
        ret    20h
loc_14_01D0:
        pop    eax
        pop    esi
        pop    edi
        mov    dword ptr [esi + 1ch], eax
        db  0E8h, 0F5h, 05Ah, 0F2h, 0FFh
        xchg   dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   eax
        ret    14h
loc_14_01E7:
        pop    eax
        pop    esi
        pop    edi
        mov    dword ptr [esi + 1ch], eax
        db  0E8h, 02Fh, 05Ch, 0F2h, 0FFh
        xchg   dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   eax
        ret    10h
loc_14_01FE:
        pop    eax
        pop    esi
        pop    edi
        mov    dword ptr [esi + 1ch], eax
        db  0E8h, 034h, 05Dh, 0F2h, 0FFh
        xchg   dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   eax
        ret    14h
loc_14_0215:
        pop    eax
        pop    esi
        pop    edi
        mov    dword ptr [esi + 1ch], eax
        db  0E8h, 084h, 055h, 0F2h, 0FFh
        xchg   dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   eax
        ret    18h
loc_14_022C:
        pop    eax
        pop    esi
        pop    edi
        mov    dword ptr [esi + 1ch], eax
        db  0E8h, 01Eh, 073h, 0F2h, 0FFh
        xchg   dword ptr [esi + 1ch], eax
        mov    dword ptr [esi + 14h], edx
        push   edi
        push   esi
        push   eax
        ret    8
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_14_0250:
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, 0ffffffffh
        mov    ebx, dword ptr [esp + 30h]
        mov    ebp, ebx
        and    ebp, 20000000h
        and    ebx, 0dfffffffh
        mov    eax, ebx
        and    eax, 60000h
        cmp    eax, ebx
        jne    NEAR PTR loc_14_0357
        shr    ebx, 10h
        mov    edi, dword ptr [esp + 2ch]
        shr    edi, 10h
        push   dword ptr [esp + 24h]
        db  0E8h, 020h, 036h, 0F2h, 0FFh
        mov    dword ptr [esp + 18h], eax
        lea    eax, [esp + 10h]
        push   eax
        or     edi, 0fffffff9h
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        db  0E8h, 047h, 036h, 0F2h, 0FFh
        or     eax, eax
        je     NEAR PTR loc_14_0347
        test   byte ptr [eax + 4], 10h
        je     SHORT loc_14_02BF
        or     ebp, ebp
        je     NEAR PTR loc_14_0347
loc_14_02BF:
        mov    eax, dword ptr [esp + 28h]
        lea    ecx, [eax*4]
        mov    dword ptr [esp + 14h], ecx
        mov    ebp, dword ptr [esp + 10h]
        add    ebp, ecx
        cmp    ebp, dword ptr [esp + 10h]
        jbe    SHORT loc_14_02F3
        mov    eax, 4
loc_14_02DF:
        mov    ecx, dword ptr [esp + 10h]
        test   byte ptr [ecx + 1], 2
        je     SHORT loc_14_0347
        add    dword ptr [esp + 10h], eax
        cmp    ebp, dword ptr [esp + 10h]
        ja     SHORT loc_14_02DF
loc_14_02F3:
        lea    eax, [esp + 1ch]
        push   eax
        push   dword ptr [esp + 2ch]
        push   dword ptr [esp + 2ch]
        db  0E8h, 0E9h, 01Eh, 0F2h, 0FFh
        mov    ecx, dword ptr [esp + 14h]
        sub    dword ptr [esp + 10h], ecx
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [ecx]
        and    esi, 6
        shl    esi, 10h
        cmp    ebp, ecx
        jbe    SHORT loc_14_0338
loc_14_031D:
        mov    ecx, dword ptr [esp + 10h]
        mov    edx, dword ptr [esp + 10h]
        mov    ecx, dword ptr [ecx]
        and    ecx, edi
        or     ecx, ebx
        mov    dword ptr [edx], ecx
        add    dword ptr [esp + 10h], 4
        cmp    ebp, dword ptr [esp + 10h]
        ja     SHORT loc_14_031D
loc_14_0338:
        push   dword ptr [esp + 1ch]
        push   eax
        db  0E8h, 01Ah, 01Fh, 0F2h, 0FFh
        db  0E8h, 0C7h, 033h, 0F2h, 0FFh
loc_14_0347:
        mov    eax, dword ptr [esp + 18h]
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
loc_14_0357:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 10h
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_14_0370:
        push   edi
        mov    edx, dword ptr [esp + 0ch]
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 400h
        rep stosd
        mov    eax, edx
        pop    edi
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_14_0390:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 20h], 1ch
        je     SHORT loc_14_03A5
        sub    eax, eax
        jmp    NEAR PTR loc_14_051A
loc_14_03A5:
        mov    ebp, dword ptr [esp + 18h]
        mov    ebx, ebp
        shr    ebx, 0ch
        mov    eax, ebx
        mov    esi, dword ptr [esp + 1ch]
        shl    eax, 0ch
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 18h], 20000h
        push   ebx
        db  0E8h, 0EAh, 034h, 0F2h, 0FFh
        mov    edi, eax
        mov    dword ptr [esp + 10h], 0fffffffeh
        lea    eax, [esp + 10h]
        push   eax
        push   1
        push   ebx
        db  0E8h, 013h, 035h, 0F2h, 0FFh
        or     eax, eax
        jne    SHORT loc_14_0457
        mov    dword ptr [esi + 10h], 10000h
        mov    dword ptr [esi + 8], 0
        cmp    ebp, 400000h
        jae    SHORT loc_14_0410
        mov    dword ptr [esi + 4], 0
        mov    eax, 400h
        sub    eax, ebx
        shl    eax, 0ch
        mov    dword ptr [esi + 0ch], eax
        jmp    NEAR PTR loc_14_0509
loc_14_0410:
        cmp    ebp, 0ffbfffffh
        jbe    SHORT loc_14_0429
        mov    dword ptr [esi + 4], 0ffc00000h
        neg    ebp
        mov    dword ptr [esi + 0ch], ebp
        jmp    NEAR PTR loc_14_0509
loc_14_0429:
        mov    ecx, dword ptr [edi + 8]
loc_14_042C:
        mov    ecx, dword ptr [ecx + 8]
        mov    eax, dword ptr [ecx + 4]
        shr    eax, 0ch
        add    eax, dword ptr [ecx]
        cmp    eax, ebx
        jb     SHORT loc_14_042C
        mov    eax, dword ptr [ecx + 4]
        and    eax, 0fffff000h
        mov    dword ptr [esi + 4], eax
        mov    ecx, dword ptr [ecx]
        sub    ecx, ebx
        shl    ecx, 0ch
        add    ecx, eax
        mov    dword ptr [esi + 0ch], ecx
        jmp    NEAR PTR loc_14_0509
loc_14_0457:
        mov    ecx, dword ptr [esp + 10h]
        mov    edx, dword ptr [eax + 4]
        mov    ecx, dword ptr [ecx]
        and    ecx, 206h
        and    edx, 0fffff3ffh
        shr    edx, 0ah
        mov    ebx, dword ptr [eax]
        lea    ebx, [edx + ebx*4 - 800000h]
        sub    edx, edx
        cmp    ebx, dword ptr [esp + 10h]
        jbe    SHORT loc_14_04A2
loc_14_0480:
        mov    ebp, dword ptr [esp + 10h]
        mov    ebp, dword ptr [ebp]
        and    ebp, 206h
        cmp    ebp, ecx
        jne    SHORT loc_14_04A2
        add    edx, 1000h
        add    dword ptr [esp + 10h], 4
        cmp    ebx, dword ptr [esp + 10h]
        ja     SHORT loc_14_0480
loc_14_04A2:
        mov    ebx, dword ptr [eax + 4]
        mov    eax, ebx
        and    eax, 0fffff000h
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 0ch], edx
        mov    eax, ecx
        and    eax, 200h
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 1000h
        add    eax, 1000h
        mov    dword ptr [esi + 10h], eax
        test   bl, 2
        je     SHORT loc_14_04E3
        and    ebx, 1
        cmp    ebx, 1
        sbb    eax, eax
        and    eax, 0fffffffeh
        add    eax, 4
        mov    dword ptr [esi + 8], eax
        jmp    SHORT loc_14_04EA
loc_14_04E3:
        mov    dword ptr [esi + 8], 1
loc_14_04EA:
        test   cl, 4
        je     SHORT loc_14_0502
        and    ecx, 2
        cmp    ecx, 1
        sbb    eax, eax
        and    eax, 0fffffffeh
        add    eax, 4
        mov    dword ptr [esi + 14h], eax
        jmp    SHORT loc_14_0509
loc_14_0502:
        mov    dword ptr [esi + 14h], 1
loc_14_0509:
        push   dword ptr [edi + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        add    esp, 4
        mov    eax, 1ch
loc_14_051A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_14_0530:
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 18h], 1
        mov    ebp, dword ptr [esp + 30h]
        cmp    ebp, 400h
        jae    SHORT loc_14_0555
        mov    eax, dword ptr [1200ech]
        shr    eax, 0ch
        add    ebp, eax
loc_14_0555:
        cmp    dword ptr [esp + 38h], 400h
        jae    SHORT loc_14_056B
        mov    eax, dword ptr [1200ech]
        shr    eax, 0ch
        add    dword ptr [esp + 38h], eax
loc_14_056B:
        cmp    ebp, 7ffffh
        ja     SHORT loc_14_058A
        cmp    dword ptr [esp + 38h], 7ffffh
        ja     SHORT loc_14_058A
        db  0A1h, 03Ch, 0F8h, 000h, 000h
        cmp    eax, dword ptr [esp + 34h]
        jbe    SHORT loc_14_0598
        jmp    SHORT loc_14_0590
loc_14_058A:
        cmp    dword ptr [esp + 38h], ebp
        jbe    SHORT loc_14_0598
loc_14_0590:
        mov    eax, ebp
        mov    esi, dword ptr [esp + 38h]
        jmp    SHORT loc_14_059E
loc_14_0598:
        mov    eax, dword ptr [esp + 38h]
        mov    esi, ebp
loc_14_059E:
        push   eax
        db  0E8h, 00Ch, 033h, 0F2h, 0FFh
        mov    dword ptr [esp + 20h], eax
        push   esi
        db  0E8h, 002h, 033h, 0F2h, 0FFh
        mov    dword ptr [esp + 1ch], eax
        lea    eax, [esp + 10h]
        push   eax
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 40h]
        db  0E8h, 02Ch, 033h, 0F2h, 0FFh
        or     eax, eax
        je     NEAR PTR loc_14_06DF
        lea    eax, [esp + 28h]
        push   eax
        mov    ebx, 400h
        push   dword ptr [esp + 40h]
        push   dword ptr [esp + 40h]
        db  0E8h, 00Bh, 01Ch, 0F2h, 0FFh
        mov    dword ptr [esp + 24h], eax
        mov    eax, ebp
        and    eax, 3ffh
        sub    ebx, eax
        mov    eax, dword ptr [esp + 3ch]
        cmp    ebx, eax
        jb     SHORT loc_14_05FA
        mov    ebx, eax
loc_14_05FA:
        cmp    ebp, 7ffffh
        ja     SHORT loc_14_062B
        mov    ecx, ebp
        and    ecx, 0fffffcffh
        mov    eax, dword ptr [esp + 34h]
        shr    ecx, 8
        add    ecx, dword ptr [eax]
        push   ecx
        db  0E8h, 087h, 058h, 0F2h, 0FFh
        mov    esi, eax
        mov    dword ptr [esp + 14h], esi
        mov    eax, ebp
        and    eax, 3ffh
        lea    esi, [esi + eax*4]
        jmp    SHORT loc_14_063A
loc_14_062B:
        mov    dword ptr [esp + 14h], 0
        lea    esi, [ebp*4 - 800000h]
loc_14_063A:
        lea    edi, [esi + ebx*4]
        cmp    edi, esi
        jbe    SHORT loc_14_06A0
loc_14_0641:
        mov    eax, dword ptr [esi]
        and    ah, 0ah
        cmp    ah, 2
        jne    SHORT loc_14_0672
        mov    eax, dword ptr [esp + 10h]
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     SHORT loc_14_0672
        test   ah, 2
        jne    SHORT loc_14_0672
        push   dword ptr [esp + 10h]
        push   esi
        call   sub_14_070B
        add    esi, 4
        add    dword ptr [esp + 10h], 4
        cmp    edi, esi
        ja     SHORT loc_14_0641
        jmp    SHORT loc_14_06A0
loc_14_0672:
        mov    eax, dword ptr [esp + 10h]
        push   20000000h
        add    eax, 800000h
        sar    eax, 2
        sub    eax, dword ptr [esp + 3ch]
        push   eax
        push   dword ptr [esp + 40h]
        db  0E8h, 01Dh, 017h, 0F2h, 0FFh
        mov    ebx, dword ptr [esp + 48h]
        mov    dword ptr [esp + 24h], 0
        add    esp, 0ch
loc_14_06A0:
        cmp    dword ptr [esp + 14h], 0
        je     SHORT loc_14_06B0
        push   dword ptr [esp + 14h]
        db  0E8h, 031h, 058h, 0F2h, 0FFh
loc_14_06B0:
        sub    dword ptr [esp + 3ch], ebx
        je     SHORT loc_14_06D2
        add    ebp, ebx
        mov    ebx, dword ptr [esp + 3ch]
        cmp    ebx, 400h
        jb     NEAR PTR loc_14_05FA
        mov    ebx, 400h
        jmp    NEAR PTR loc_14_05FA
loc_14_06D2:
        push   dword ptr [esp + 28h]
        push   dword ptr [esp + 28h]
        db  0E8h, 07Dh, 01Bh, 0F2h, 0FFh
loc_14_06DF:
        mov    eax, dword ptr [esp + 20h]
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        mov    eax, dword ptr [esp + 20h]
        add    esp, 4
        push   dword ptr [eax + 10h]
        db  0FFh, 015h, 0ECh, 0F7h, 000h, 000h
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 1ch
        ret
sub_14_070B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        db  0E8h, 0ADh, 035h, 0F2h, 0FFh
        inc    word ptr [eax]
        mov    word ptr [eax + 4], 0
        mov    edx, dword ptr [esi]
        test   dl, 1
        je     SHORT loc_14_072D
        mov    ecx, dword ptr [eax + 6]
        inc    word ptr [ecx + 0ah]
loc_14_072D:
        mov    ecx, dword ptr [esp + 0ch]
        and    edx, 0ffffff9fh
        mov    dword ptr [ecx], edx
        push   eax
        db  0E8h, 081h, 02Fh, 0F2h, 0FFh
        pop    esi
        ret    8
_PTEXT ends
        end
