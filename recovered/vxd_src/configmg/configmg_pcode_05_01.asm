; configmg_pcode_05_01.asm -- CONFIGMG object 5 segment _PTEXT  (part 1/4)
.386p
_PTEXT segment dword public use32 'PCODE'

CONFIGMG_PM_API:
        mov    ax, 3408h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        mov    esi, eax
        movzx  ebx, word ptr [ebp + 1ch]
        mov    eax, 20h
        cmp    ebx, 6dh
        jae    SHORT loc_5_007B
        db  00Fh, 0B7h, 014h, 05Dh, 000h, 000h, 000h, 000h
        cmp    dx, -1
        je     SHORT loc_5_007B
        mov    edi, edx
        shr    edx, 4
        and    edi, 0fh
        mov    ecx, edi
        je     SHORT loc_5_0072
        shl    edi, 2
        add    esi, edi
        ror    edx, cl
loc_5_003B:
        mov    eax, dword ptr [esi]
        sub    esi, 4
        rol    edx, 1
        test   edx, 1
        je     SHORT loc_5_006F
        or     eax, eax
        je     SHORT loc_5_006F
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 3ch]
        mov    word ptr [ebp + 14h], ax
        shr    eax, 10h
        mov    word ptr [ebp + 3ch], ax
        mov    ax, 3c14h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        pop    dword ptr [ebp + 3ch]
        pop    dword ptr [ebp + 14h]
loc_5_006F:
        push   eax
        loop   loc_5_003B
loc_5_0072:
        db  0FFh, 014h, 09Dh, 000h, 000h, 000h, 000h
        add    esp, edi
loc_5_007B:
        mov    edx, dword ptr [ebp + 1ch]
        and    edx, 0ffffh
        mov    dx, ax
        sub    ax, 1
        cmc
        lahf
        mov    dword ptr [ebp + 1ch], edx
        mov    byte ptr [ebp + 2ch], ah
        ret
sub_5_0094:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [eax + 4]
        and    ebx, 0ffffh
        push   eax
        mov    esi, dword ptr [eax]
        mov    eax, 20h
        cmp    ebx, 6dh
        jae    SHORT loc_5_00DA
        db  00Fh, 0B7h, 00Ch, 05Dh, 000h, 000h, 000h, 000h
        cmp    cx, -1
        je     SHORT loc_5_00DA
        and    ecx, 0fh
        mov    edi, ecx
        shl    edi, 2
        mov    eax, edi
        sub    esp, edi
        mov    edi, esp
        rep movsd
        mov    edi, eax
        db  0FFh, 014h, 09Dh, 000h, 000h, 000h, 000h
        add    esp, edi
loc_5_00DA:
        pop    ebx
        mov    dword ptr [ebx + 8], eax
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_00E2:
        lea    eax, [esp + 4]
        push   eax
        push   14h
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   0
        db  0CDh, 020h, 053h, 001h, 001h, 000h
        add    esp, 10h
        ret
loc_5_00FB:
        mov    ebx, dword ptr [esi + 0ch]
        sub    ebx, 80000000h
        jb     NEAR PTR loc_5_019D
        cmp    ebx, 6dh
        jae    NEAR PTR loc_5_019D
        mov    eax, 20h
        push   esi
        cmp    dword ptr [esi + 1ch], 4
        jne    SHORT loc_5_018B
        cmp    dword ptr [esi + 14h], 4
        jne    SHORT loc_5_018B
        db  00Fh, 0B7h, 03Ch, 05Dh, 000h, 000h, 000h, 000h
        cmp    di, -1
        je     SHORT loc_5_018B
        and    edi, 0fh
        shl    edi, 2
        mov    esi, dword ptr [esi + 10h]
        push   0
        push   0
        push   0
        push   4
        push   esi
        db  0CDh, 020h, 073h, 001h, 001h, 000h
        add    esp, 14h
        or     eax, eax
        mov    eax, 3
        je     SHORT loc_5_018B
        mov    esi, dword ptr [esi]
        push   0
        push   0
        push   0
        push   edi
        push   esi
        db  0CDh, 020h, 073h, 001h, 001h, 000h
        add    esp, 14h
        or     eax, eax
        mov    eax, 3
        je     SHORT loc_5_018B
        mov    ecx, edi
        mov    eax, edi
        shr    ecx, 2
        sub    esp, edi
        mov    edi, esp
        rep movsd
        mov    esi, eax
        db  0FFh, 014h, 09Dh, 000h, 000h, 000h, 000h
        add    esp, esi
loc_5_018B:
        pop    esi
        mov    ecx, dword ptr [esi + 18h]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi + 20h]
        xor    eax, eax
        mov    dword ptr [ecx], 4
        ret
loc_5_019D:
        add    ebx, 80000000h
        cmp    ebx, 0
        je     SHORT loc_5_01B6
        cmp    ebx, -1
        je     SHORT loc_5_01B3
        mov    eax, 0ffffffffh
        ret
loc_5_01B3:
        xor    eax, eax
        ret
loc_5_01B6:
        xor    eax, eax
        mov    ecx, 400h
        ret
loc_5_01BE:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_01D8
        push   dword ptr [esp + 4]
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_5_D024
        jmp    SHORT loc_5_01EA
loc_5_01D8:
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_020F
        mov    ecx, dword ptr [esp + 4]
        db  0E9h, 000h, 000h, 000h, 000h
loc_5_01EA:
        ret
sub_5_01EB:
        push   esi
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        call   dword ptr [esi + 10h]
        add    esp, 0ch
        mov    dword ptr [esi + 0ch], eax
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        pop    esi
        ret
sub_5_020F:
        push   esi
        push   edi
        mov    edi, ecx
        xor    esi, esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    NEAR PTR loc_5_02B4
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_0245
        push   9
        push   esi
        push   esi
        push   esi
        push   esi
        push   esi
        push   1
        push   4
        call   sub_5_D068
        add    esp, 20h
        db  0A3h, 000h, 000h, 000h, 000h
        cmp    eax, esi
        je     SHORT loc_5_02BD
loc_5_0245:
        push   4000h
        db  0A1h, 000h, 000h, 000h, 000h
        add    eax, 4000h
        push   eax
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_D060
        add    esp, 10h
loc_5_0263:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_D050
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_5_0292
        push   1
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0281:
        add    esp, 0ch
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        push   eax
        call   sub_5_D03C
        jmp    SHORT loc_5_0263
loc_5_0292:
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 004h
        je     SHORT loc_5_02BD
        db  0A1h, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        push   ecx
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_D034
        add    esp, 8
        jmp    SHORT loc_5_02BD
loc_5_02B4:
        push   edi
        call   sub_5_01EB
        add    esp, 4
loc_5_02BD:
        pop    edi
        pop    esi
        ret
loc_5_02C0:
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_020F
        push   dword ptr [esp + 8]
        call   sub_5_D070
        ret
sub_5_02D3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    edi, ecx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_02E5:
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_5_02F3
        mov    eax, dword ptr [ebp + 10h]
        jmp    NEAR PTR loc_5_041F
loc_5_02F3:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 10h], edi
        mov    dword ptr [esi], ebx
        lea    edi, [esi + 0ch]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [edi], eax
        call   sub_5_D0B8
        test   al, 1
        je     NEAR PTR loc_5_040D
        db  0F6h, 005h, 000h, 000h, 000h, 000h, 008h
        jne    NEAR PTR loc_5_040D
        test   al, 2
        jne    NEAR PTR loc_5_040D
        call   sub_5_D0AC
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_0390
        xor    ebx, ebx
        xor    ecx, ecx
        db  0CDh, 020h, 029h, 000h, 001h, 000h
        mov    dword ptr [ebp - 0ch], ecx
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_5_0359
        mov    dword ptr [edi], 16h
        jmp    NEAR PTR loc_5_0414
loc_5_0359:
        push   0
        call   sub_5_D09C
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_5_0414
        push   0
        db  0B9h, 000h, 000h, 000h, 000h
        push   ebx
        mov    edx, esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0379:
        test   eax, eax
        je     SHORT loc_5_0385
        push   1
        push   ebx
        call   sub_5_D090
loc_5_0385:
        push   ebx
        call   sub_5_D084
        jmp    NEAR PTR loc_5_0414
loc_5_0390:
        db  0A1h, 000h, 000h, 000h, 000h
        test   byte ptr [eax], 20h
        jne    SHORT loc_5_03A2
        mov    dword ptr [edi], 16h
        jmp    SHORT loc_5_0414
loc_5_03A2:
        mov    eax, dword ptr [eax + 8]
        mov    ax, word ptr [eax + 28h]
        mov    word ptr [ebp - 2], ax
        mov    ax, word ptr [ebp - 2]
        lar    eax, eax
        mov    dword ptr [ebp - 0ch], eax
        test   byte ptr [ebp - 0ah], 40h
        jne    SHORT loc_5_03E2
        push   esi
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 8], 1
        push   7fffh
        push   219h
        push   eax
        push   80000068h
        call   sub_5_D07C
        add    esp, 14h
        jmp    SHORT loc_5_0414
loc_5_03E2:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_0414
        push   esi
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 8], 1
        push   7fffh
        push   219h
        push   eax
        push   68h
        call   sub_5_00E2
        add    esp, 14h
        jmp    SHORT loc_5_0414
loc_5_040D:
        mov    ecx, esi
        call   sub_5_020F
loc_5_0414:
        mov    edi, dword ptr [edi]
        mov    ecx, esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_041D:
        mov    eax, edi
loc_5_041F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_5_0428:
        xor    eax, eax
        ret
sub_5_042B:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        push   ebp
        xor    edi, edi
        dec    ebx
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [eax - 24h], ecx
        jae    SHORT loc_5_0446
        mov    dword ptr [edx], edi
        jmp    SHORT loc_5_048F
loc_5_0446:
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   ebx, ebx
        je     SHORT loc_5_046B
loc_5_0450:
        lea    eax, [edi + ebx]
        shr    eax, 1
        mov    ebp, eax
        imul   ebp, ebp, 24h
        cmp    dword ptr [ebp + esi], ecx
        jb     SHORT loc_5_0464
        mov    ebx, eax
        jmp    SHORT loc_5_0467
loc_5_0464:
        lea    edi, [eax + 1]
loc_5_0467:
        cmp    ebx, edi
        jne    SHORT loc_5_0450
loc_5_046B:
        imul   edi, edi, 24h
        lea    eax, [edi + esi]
        mov    esi, dword ptr [eax]
        sub    esi, ecx
        cmp    esi, 1
        sbb    esi, esi
        neg    esi
        mov    dword ptr [edx], esi
        test   esi, esi
        je     SHORT loc_5_048F
        cmp    dword ptr [eax - 24h], ecx
        jne    SHORT loc_5_048F
loc_5_0487:
        sub    eax, 24h
        cmp    dword ptr [eax - 24h], ecx
        je     SHORT loc_5_0487
loc_5_048F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_0494:
        push   ebx
        push   esi
        mov    eax, dword ptr [edx + 0ch]
        push   edi
        push   ebp
        mov    ebx, edx
        mov    edx, dword ptr [ecx + 1ch]
        mov    edi, ecx
        mov    ebp, dword ptr [eax + 4]
        mov    ecx, ebp
        call   sub_5_1D59
        test   eax, eax
        jne    SHORT loc_5_04B4
        xor    eax, eax
        jmp    SHORT loc_5_0511
loc_5_04B4:
        mov    esi, dword ptr [edi + 8]
        test   esi, 8
        jne    SHORT loc_5_04C6
        mov    eax, 1
        jmp    SHORT loc_5_0511
loc_5_04C6:
        cmp    dword ptr [ebx + 14h], -1
        jne    SHORT loc_5_04E8
        mov    ecx, dword ptr [ebp + 38h]
        test   ecx, ecx
        je     SHORT loc_5_04E1
        mov    edx, dword ptr [ebx + 4]
        call   sub_5_184D
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_04E8
loc_5_04E1:
        mov    eax, 1
        jmp    SHORT loc_5_0511
loc_5_04E8:
        test   esi, 10h
        je     SHORT loc_5_04F5
        add    ebx, 1ch
        jmp    SHORT loc_5_04FB
loc_5_04F5:
        mov    eax, dword ptr [ebx + 0ch]
        mov    ebx, dword ptr [eax + 4]
loc_5_04FB:
        push   ebx
        mov    edx, 7
        mov    ecx, edi
        call   sub_5_0B16
        sub    eax, 13h
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_5_0511:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_0516:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    eax, dword ptr [ecx + 0ch]
        push   ebp
        mov    edi, ecx
        mov    ecx, dword ptr [eax + 4]
        call   sub_5_1D31
        mov    ebp, eax
        lea    edx, [esp + 10h]
        mov    ecx, dword ptr [edi + 4]
        call   sub_5_042B
        mov    esi, eax
        cmp    dword ptr [esp + 10h], ebx
        je     SHORT loc_5_0580
loc_5_0542:
        mov    eax, dword ptr [esi + 10h]
        test   eax, eax
        je     SHORT loc_5_057E
        cmp    eax, ebp
        jae    SHORT loc_5_0555
        test   byte ptr [esi + 8], 2
        je     SHORT loc_5_055D
        cmp    eax, ebp
loc_5_0555:
        jne    SHORT loc_5_056A
        test   byte ptr [esi + 8], 2
        je     SHORT loc_5_056A
loc_5_055D:
        mov    edx, edi
        mov    ecx, esi
        call   sub_5_0494
        test   eax, eax
        jne    SHORT loc_5_057E
loc_5_056A:
        add    esi, 24h
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_0580
        mov    eax, dword ptr [esi]
        cmp    dword ptr [edi + 4], eax
        je     SHORT loc_5_0542
        jmp    SHORT loc_5_0580
loc_5_057E:
        mov    ebx, esi
loc_5_0580:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_5_058A:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        test   edi, edi
        je     SHORT loc_5_05BA
loc_5_0595:
        mov    ebx, dword ptr [edi + 10h]
        test   ebx, ebx
        je     SHORT loc_5_05B4
loc_5_059C:
        test   byte ptr [ebx + 5], 80h
        jne    SHORT loc_5_05AD
        mov    ecx, ebx
        call   sub_5_0516
        test   eax, eax
        je     SHORT loc_5_05C3
loc_5_05AD:
        mov    ebx, dword ptr [ebx + 10h]
        test   ebx, ebx
        jne    SHORT loc_5_059C
loc_5_05B4:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_5_0595
loc_5_05BA:
        mov    eax, 1
loc_5_05BF:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_05C3:
        mov    edx, 5
        mov    ecx, esi
        call   sub_5_731F
        xor    eax, eax
        jmp    SHORT loc_5_05BF
sub_5_05D3:
        sub    esp, 30h
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        add    edi, 24h
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_0625
        mov    ebx, 100000h
loc_5_05F1:
        push   ebx
        mov    ecx, dword ptr [edi + 1ch]
        push   0
        push   ebx
        push   0
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_061A
        lea    ecx, [esp + 0ch]
        push   esi
        mov    edx, dword ptr [edi]
        call   sub_5_08C9
        mov    ecx, eax
        call   sub_5_0516
        cmp    eax, edi
        je     SHORT loc_5_0631
loc_5_061A:
        add    edi, 24h
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_05F1
loc_5_0625:
        mov    eax, 1
loc_5_062A:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 30h
        ret
loc_5_0631:
        xor    eax, eax
        jmp    SHORT loc_5_062A
sub_5_0635:
        db  0A1h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        add    eax, 24h
        cmp    edx, eax
        je     SHORT loc_5_0653
loc_5_0647:
        cmp    dword ptr [eax + 4], ecx
        je     SHORT loc_5_0655
        add    eax, 24h
        cmp    edx, eax
        jne    SHORT loc_5_0647
loc_5_0653:
        xor    eax, eax
loc_5_0655:
        ret
sub_5_0656:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   esi
        mov    dword ptr [ebp - 4], edx
        push   edi
        mov    dword ptr [ebp - 0ch], ecx
        test   byte ptr [ebp + 14h], 1
        je     SHORT loc_5_0676
        xor    edi, edi
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp + 10h], eax
        jmp    SHORT loc_5_0680
loc_5_0676:
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_5_1D31
        mov    edi, eax
loc_5_0680:
        call   sub_5_675F
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    SHORT loc_5_06A9
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_5_069F
        mov    edx, 3
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_5_731F
loc_5_069F:
        mov    eax, 2
        jmp    NEAR PTR loc_5_07D2
loc_5_06A9:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_06BA
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_5_070D
loc_5_06BA:
        db  0A1h, 000h, 000h, 000h, 000h
        push   201h
        inc    eax
        imul   eax, eax, 24h
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_5_D0C0
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_06F8
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_5_06EE
        mov    edx, 3
        mov    ecx, dword ptr [ebp + 10h]
        call   sub_5_731F
loc_5_06EE:
        mov    eax, 2
        jmp    NEAR PTR loc_5_07D2
loc_5_06F8:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        imul   ecx, ecx, 24h
        add    eax, ecx
        db  0A3h, 000h, 000h, 000h, 000h
loc_5_070D:
        lea    edx, [ebp - 10h]
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_042B
        cmp    dword ptr [ebp - 10h], 0
        mov    esi, eax
        je     SHORT loc_5_0737
loc_5_0720:
        cmp    dword ptr [esi + 10h], edi
        jbe    SHORT loc_5_0790
        add    esi, 24h
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_0737
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [esi], eax
        je     SHORT loc_5_0720
loc_5_0737:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, 24h
        sub    eax, esi
        cdq
        idiv   ecx
        imul   eax, ecx
        push   eax
        lea    ecx, [esi + 24h]
        mov    edx, esi
        call   sub_5_5B5D
        db  083h, 005h, 000h, 000h, 000h, 000h, 024h
        mov    dword ptr [esi + 4], 0
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_5_07AA
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_5_0774:
        test   ecx, ecx
        je     SHORT loc_5_078D
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_0635
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_5_079B
loc_5_078D:
        inc    ecx
        jmp    SHORT loc_5_0774
loc_5_0790:
        test   edi, edi
        jne    SHORT loc_5_0737
        mov    eax, 8
        jmp    SHORT loc_5_07D2
loc_5_079B:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [eax], ecx
        inc    ecx
        db  089h, 00Dh, 000h, 000h, 000h, 000h
loc_5_07AA:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 8], ecx
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [esi + 10h], edi
        mov    dword ptr [esi + 14h], edx
        mov    dword ptr [esi + 18h], eax
        mov    edx, dword ptr [ebp - 8]
        xor    eax, eax
        mov    dword ptr [esi + 1ch], ecx
        mov    dword ptr [esi + 20h], edx
loc_5_07D2:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    10h
sub_5_07DA:
        push   esi
        mov    edx, 5
        push   edi
        push   0
        mov    edi, ecx
        call   sub_5_0B16
        mov    esi, dword ptr [edi + 20h]
        mov    ecx, esi
        call   sub_5_684A
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_0808
        push   0
        push   esi
        call   sub_5_D0C8
        add    esp, 8
loc_5_0808:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, 24h
        sub    eax, edi
        cdq
        idiv   ecx
        imul   eax, ecx
        sub    eax, 24h
        lea    edx, [edi + 24h]
        push   eax
        mov    ecx, edi
        call   sub_5_5B5D
        pop    edi
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        pop    esi
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        db  083h, 02Dh, 000h, 000h, 000h, 000h, 024h
        ret
sub_5_083E:
        call   sub_5_0635
        test   eax, eax
        jne    SHORT loc_5_084B
        xor    eax, eax
        jmp    SHORT loc_5_0857
loc_5_084B:
        mov    ecx, eax
        call   sub_5_07DA
        mov    eax, 1
loc_5_0857:
        ret
sub_5_0858:
        push   esi
        push   edi
        mov    esi, ecx
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        add    edi, 24h
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_0887
loc_5_086D:
        cmp    dword ptr [edi + 1ch], esi
        jne    SHORT loc_5_087C
        mov    ecx, edi
        sub    edi, 24h
        call   sub_5_07DA
loc_5_087C:
        add    edi, 24h
        db  03Bh, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_086D
loc_5_0887:
        pop    edi
        pop    esi
        ret
sub_5_088A:
        push   esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        add    esi, 24h
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_08C7
loc_5_089C:
        mov    eax, dword ptr [esi + 8]
        test   eax, 20000000h
        je     SHORT loc_5_08BC
        and    eax, 0dfffffffh
        push   0
        mov    edx, 9
        mov    ecx, esi
        mov    dword ptr [esi + 8], eax
        call   sub_5_0B16
loc_5_08BC:
        add    esi, 24h
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_089C
loc_5_08C7:
        pop    esi
        ret
sub_5_08C9:
        push   esi
        xor    eax, eax
        push   edi
        mov    esi, ecx
        mov    edi, esi
        mov    ecx, 0ch
        rep stosd
        mov    ecx, dword ptr [esp + 0ch]
        lea    eax, [esi + 1ch]
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [esi + 0ch], eax
        mov    eax, esi
        mov    dword ptr [esi + 20h], ecx
        pop    edi
        mov    dword ptr [esi + 2ch], esi
        mov    dword ptr [esi + 14h], 0ffffffffh
        mov    dword ptr [esi + 18h], 4c52574dh
        pop    esi
        ret    4
sub_5_0900:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        test   eax, eax
        push   edi
        mov    esi, edx
        push   ebp
        je     SHORT loc_5_0959
        mov    edi, dword ptr [eax + 10h]
        test   edi, edi
        je     SHORT loc_5_0959
loc_5_0915:
        test   byte ptr [edi + 5], 80h
        jne    SHORT loc_5_0952
        mov    ecx, edi
        call   sub_5_0516
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_5_0952
        mov    ebp, dword ptr [ebx + 20h]
        mov    eax, dword ptr [ebp]
        test   eax, eax
        je     SHORT loc_5_0937
        cmp    dword ptr [eax + 8], esi
        je     SHORT loc_5_0952
loc_5_0937:
        mov    edx, esi
        mov    ecx, ebp
        call   sub_5_6791
        test   eax, eax
        je     SHORT loc_5_0965
        mov    ecx, dword ptr [ebp]
        mov    eax, dword ptr [esp + 18h]
        or     byte ptr [ebx + 0bh], 40h
        mov    dword ptr [ecx + 0ch], eax
loc_5_0952:
        mov    edi, dword ptr [edi + 10h]
        test   edi, edi
        jne    SHORT loc_5_0915
loc_5_0959:
        mov    eax, 1
loc_5_095E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_5_0965:
        xor    eax, eax
        jmp    SHORT loc_5_095E
sub_5_0969:
        push   esi
        push   edi
        push   edx
        mov    edi, edx
        mov    edx, ecx
        mov    esi, ecx
        xor    ecx, ecx
        push   dword ptr [esi + 38h]
        call   sub_5_0900
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_5_0999
        push   edi
        mov    edx, esi
        push   edi
        mov    ecx, 1
        call   sub_5_0900
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_5_0999:
        pop    edi
        pop    esi
        ret
sub_5_099C:
        push   esi
        xor    eax, eax
        push   edi
        mov    esi, ecx
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        add    edi, 24h
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_09D8
loc_5_09B3:
        test   byte ptr [edi + 0bh], 40h
        je     SHORT loc_5_09CD
        mov    edx, esi
        mov    ecx, edi
        push   dword ptr [edi + 20h]
        call   sub_5_0B16
        test   eax, eax
        jne    SHORT loc_5_09D8
        or     byte ptr [edi + 0bh], 80h
loc_5_09CD:
        add    edi, 24h
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_09B3
loc_5_09D8:
        pop    edi
        pop    esi
        ret
sub_5_09DB:
        push   esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        add    esi, 24h
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_0A30
loc_5_09ED:
        lea    ecx, [esi + 8]
        mov    eax, dword ptr [ecx]
        test   eax, 40000000h
        je     SHORT loc_5_0A25
        and    eax, 0bfffffffh
        mov    dword ptr [ecx], eax
        test   eax, 80000000h
        je     SHORT loc_5_0A1D
        and    eax, 7fffffffh
        mov    edx, 3
        push   dword ptr [esi + 20h]
        mov    dword ptr [ecx], eax
        mov    ecx, esi
        call   sub_5_0B16
loc_5_0A1D:
        mov    ecx, dword ptr [esi + 20h]
        call   sub_5_684A
loc_5_0A25:
        add    esi, 24h
        db  03Bh, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_09ED
loc_5_0A30:
        pop    esi
        ret
sub_5_0A32:
        push   esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        sub    esi, 24h
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_0A87
loc_5_0A44:
        lea    ecx, [esi + 8]
        mov    eax, dword ptr [ecx]
        test   eax, 40000000h
        je     SHORT loc_5_0A7C
        and    eax, 0bfffffffh
        mov    edx, 2
        push   dword ptr [esi + 20h]
        mov    dword ptr [ecx], eax
        and    eax, 7fffffffh
        mov    dword ptr [ecx], eax
        or     eax, 20000000h
        mov    dword ptr [ecx], eax
        mov    ecx, esi
        call   sub_5_0B16
        mov    ecx, dword ptr [esi + 20h]
        call   sub_5_684A
loc_5_0A7C:
        sub    esi, 24h
        db  039h, 035h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_0A44
loc_5_0A87:
        pop    esi
        ret
sub_5_0A89:
        sub    esp, 0ch
        mov    eax, dword ptr [ecx + 20h]
        mov    dword ptr [esp], ecx
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, edx
        mov    edi, dword ptr [eax]
        test   edi, edi
        je     SHORT loc_5_0B0E
loc_5_0A9F:
        mov    eax, dword ptr [edi + 8]
        mov    edx, dword ptr [edi + 0ch]
        lea    ebx, [esp + 14h]
        mov    dword ptr [esp + 18h], edx
        mov    ecx, dword ptr [eax + 38h]
        mov    dword ptr [esp + 14h], ecx
loc_5_0AB4:
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     SHORT loc_5_0AFD
        mov    ebp, dword ptr [eax + 10h]
        test   ebp, ebp
        je     SHORT loc_5_0AFD
loc_5_0AC1:
        mov    ecx, ebp
        call   sub_5_0516
        cmp    eax, dword ptr [esp + 10h]
        je     SHORT loc_5_0AF6
        test   esi, esi
        je     SHORT loc_5_0AE6
        mov    eax, dword ptr [ebp + 4]
        test   ah, 80h
        jne    SHORT loc_5_0AE6
        or     ah, 80h
        or     byte ptr [ebp], 1
        mov    dword ptr [ebp + 4], eax
        jmp    SHORT loc_5_0AF6
loc_5_0AE6:
        mov    eax, dword ptr [ebp]
        test   al, 1
        je     SHORT loc_5_0AF6
        and    byte ptr [ebp + 5], 7fh
        and    al, 0feh
        mov    dword ptr [ebp], eax
loc_5_0AF6:
        mov    ebp, dword ptr [ebp + 10h]
        test   ebp, ebp
        jne    SHORT loc_5_0AC1
loc_5_0AFD:
        add    ebx, 4
        lea    eax, [esp + 1ch]
        cmp    ebx, eax
        jb     SHORT loc_5_0AB4
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_5_0A9F
loc_5_0B0E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0ch
        ret
sub_5_0B16:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        test   byte ptr [esi + 8], 8
        jne    SHORT loc_5_0B39
        cmp    edi, 1
        jbe    SHORT loc_5_0B2D
        cmp    edi, 6
        jne    SHORT loc_5_0B39
loc_5_0B2D:
        mov    edx, 1
        mov    ecx, esi
        call   sub_5_0A89
loc_5_0B39:
        push   dword ptr [esp + 10h]
        push   dword ptr [esi + 1ch]
        push   dword ptr [esi + 18h]
        push   edi
        call   dword ptr [esi + 14h]
        add    esp, 10h
        mov    ebx, eax
        test   byte ptr [esi + 8], 8
        jne    SHORT loc_5_0B65
        cmp    edi, 1
        jbe    SHORT loc_5_0B5C
        cmp    edi, 6
        jne    SHORT loc_5_0B65
loc_5_0B5C:
        xor    edx, edx
        mov    ecx, esi
        call   sub_5_0A89
loc_5_0B65:
        cmp    ebx, 1
        jne    SHORT loc_5_0B91
        cmp    edi, 3
        jbe    SHORT loc_5_0B88
        cmp    edi, 4
        je     SHORT loc_5_0B8C
        cmp    edi, 5
        jb     SHORT loc_5_0B91
        cmp    edi, 7
        jbe    SHORT loc_5_0B88
        cmp    edi, 8
        je     SHORT loc_5_0B8C
        cmp    edi, 9
        jne    SHORT loc_5_0B91
loc_5_0B88:
        xor    ebx, ebx
        jmp    SHORT loc_5_0B91
loc_5_0B8C:
        mov    ebx, 13h
loc_5_0B91:
        test   ebx, ebx
        je     SHORT loc_5_0BA4
        cmp    ebx, 2
        je     SHORT loc_5_0BA4
        cmp    ebx, 13h
        je     SHORT loc_5_0BA4
        mov    ebx, 13h
loc_5_0BA4:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_5_0BAC:
        push   ebp
        mov    ebp, esp
        sub    esp, 314h
        mov    eax, 20000h
        push   ebx
        mov    byte ptr [ebp - 1], 1
        push   esi
        push   edi
        mov    esi, ecx
        push   0
        push   eax
        push   0
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     NEAR PTR loc_5_0D02
        mov    edi, dword ptr [esi + 20h]
        lea    eax, [ebp - 14h]
        push   eax
        xor    edx, edx
        mov    ecx, edi
        call   sub_5_8D6E
        test   byte ptr [ebp - 14h], 80h
        je     NEAR PTR loc_5_0D02
        xor    ebx, ebx
        mov    esi, dword ptr [edi + 24h]
        mov    dword ptr [ebp - 0ch], 0
        cmp    esi, ebx
        je     SHORT loc_5_0C2C
loc_5_0C00:
        push   0
        mov    ecx, esi
        push   8
        push   0
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_0C16
        inc    ebx
        jmp    SHORT loc_5_0C25
loc_5_0C16:
        mov    eax, dword ptr [esi + 0ch]
        test   eax, eax
        je     SHORT loc_5_0C25
        cmp    eax, 1dh
        je     SHORT loc_5_0C25
        inc    dword ptr [ebp - 0ch]
loc_5_0C25:
        mov    esi, dword ptr [esi + 28h]
        test   esi, esi
        jne    SHORT loc_5_0C00
loc_5_0C2C:
        xor    esi, esi
        cmp    dword ptr [ebp - 0ch], esi
        je     NEAR PTR loc_5_0D02
        test   ebx, ebx
        je     NEAR PTR loc_5_0D02
        push   0eh
        call   sub_5_6710
        add    esp, 4
        lea    ecx, [ebp - 114h]
        push   eax
        push   ecx
        call   sub_5_D0FC
        add    esp, 8
        lea    ecx, [ebp - 10h]
        mov    dword ptr [ebp - 10h], 100h
        push   esi
        lea    edx, [ebp - 114h]
        push   ecx
        push   edx
        push   1
        xor    edx, edx
        db  068h, 000h, 000h, 000h, 000h
        mov    ecx, edi
        call   sub_5_87D6
        lea    ecx, [ebp - 114h]
        push   ecx
        push   15h
        call   sub_5_6710
        add    esp, 4
        lea    ecx, [ebp - 314h]
        push   eax
        push   ecx
        call   sub_5_D0FC
        add    esp, 0ch
        lea    ecx, [ebp - 8]
        push   ecx
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   2
        call   sub_5_D0F4
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_5_0CE7
        push   1
        lea    eax, [ebp - 1]
        push   eax
        push   3
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_5_D0EC
        add    esp, 18h
        push   dword ptr [ebp - 8]
        call   sub_5_D0E4
        add    esp, 4
        push   5350574dh
        call   sub_5_D0DC
        add    esp, 4
loc_5_0CE7:
        push   1
        lea    edx, [ebp - 314h]
        xor    ecx, ecx
        call   sub_5_5EA8
        push   0
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_D0D0
loc_5_0D02:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_0D09:
        push   1
        push   edx
        push   9
        push   edx
        push   3
        mov    edx, ecx
        mov    ecx, 1
        call   sub_5_CEA8
        ret
sub_5_0D1E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    eax, dword ptr [ecx + 3ch]
        mov    esi, ecx
        test   eax, eax
        je     NEAR PTR loc_5_0DB8
        mov    eax, dword ptr [eax + 10h]
        test   eax, eax
        je     SHORT loc_5_0DB8
loc_5_0D39:
        cmp    dword ptr [eax + 4], 1
        jne    SHORT loc_5_0D45
        test   byte ptr [eax + 28h], 1
        je     SHORT loc_5_0D4E
loc_5_0D45:
        mov    eax, dword ptr [eax + 10h]
        test   eax, eax
        jne    SHORT loc_5_0D39
        jmp    SHORT loc_5_0DB8
loc_5_0D4E:
        lea    eax, [ebp - 4]
        xor    edx, edx
        push   eax
        mov    ecx, esi
        call   sub_5_8D6E
        test   byte ptr [ebp - 3], 1
        jne    SHORT loc_5_0DB8
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_0D71
        mov    eax, 1
        jmp    SHORT loc_5_0DBA
loc_5_0D71:
        lea    eax, [ebp - 8]
        db  0BAh, 000h, 000h, 000h, 000h
        push   eax
        db  0B9h, 000h, 000h, 000h, 000h
        push   4
        mov    dword ptr [ebp - 8], esi
        push   0
        call   sub_5_1240
        cmp    eax, 20h
        jne    SHORT loc_5_0D97
        mov    eax, 1
        jmp    SHORT loc_5_0DBA
loc_5_0D97:
        cmp    eax, 22h
        jne    SHORT loc_5_0DB8
        push   0
        lea    eax, [ebp - 4]
        push   4
        xor    edx, edx
        push   eax
        mov    ecx, esi
        or     byte ptr [ebp - 3], 1
        push   3
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_886D
loc_5_0DB8:
        xor    eax, eax
loc_5_0DBA:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_5_0DBF:
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [esp + 0ch]
        push   esi
        mov    edx, edi
        db  068h, 000h, 000h, 000h, 000h
        mov    ecx, 2
        call   sub_5_1A48
        push   8000000h
        mov    ecx, edi
        push   esi
        call   sub_5_1923
        push   esi
        mov    ecx, edi
        push   8
        push   esi
        push   8
        call   sub_5_18E3
        test   eax, eax
        mov    eax, 1
        je     SHORT loc_5_0E25
        xor    esi, esi
        mov    edx, 8003h
        push   esi
        mov    ecx, edi
        call   sub_5_6048
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        mov    edx, edi
        push   -1
        xor    ecx, ecx
        push   eax
        push   0ah
        call   sub_5_CEA8
        mov    eax, 1
loc_5_0E25:
        pop    edi
        pop    esi
        ret
sub_5_0E28:
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [esp + 0ch]
        push   esi
        mov    edx, edi
        db  068h, 000h, 000h, 000h, 000h
        mov    ecx, 3
        call   sub_5_1A48
        push   8000000h
        mov    ecx, edi
        push   esi
        call   sub_5_1923
        push   esi
        mov    ecx, edi
        push   8
        push   esi
        push   8
        call   sub_5_18E3
        test   eax, eax
        mov    eax, 1
        je     SHORT loc_5_0E8E
        xor    esi, esi
        mov    edx, 8002h
        push   esi
        mov    ecx, edi
        call   sub_5_6048
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        mov    edx, edi
        push   -1
        xor    ecx, ecx
        push   eax
        push   9
        call   sub_5_CEA8
        mov    eax, 1
loc_5_0E8E:
        pop    edi
        pop    esi
        ret
sub_5_0E91:
        push   esi
        push   ecx
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_5_0E28
        add    esp, 4
        db  089h, 035h, 000h, 000h, 000h, 000h
        pop    esi
        ret
loc_5_0EB3:
        push   esi
        mov    eax, 8000000h
        push   edi
        push   eax
        mov    edi, dword ptr [esp + 10h]
        push   0
        push   eax
        mov    ecx, edi
        push   0
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_0ED9
        mov    eax, 1
        jmp    NEAR PTR loc_5_0F8F
loc_5_0ED9:
        db  068h, 000h, 000h, 000h, 000h
        mov    edx, edi
        db  068h, 000h, 000h, 000h, 000h
        mov    ecx, 2
        call   sub_5_1A48
        test   eax, eax
        je     NEAR PTR loc_5_0F8D
        push   0
        mov    ecx, edi
        push   8
        push   0
        push   8
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_0F3A
        mov    ecx, edi
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        call   sub_5_0D1E
        cmp    eax, 1
        sbb    esi, esi
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        neg    esi
        test   esi, esi
        je     SHORT loc_5_0F36
        push   8000000h
        mov    ecx, edi
        push   0
        call   sub_5_1906
loc_5_0F36:
        mov    eax, esi
        jmp    SHORT loc_5_0F8F
loc_5_0F3A:
        push   1
        mov    edx, 8001h
        mov    ecx, edi
        call   sub_5_6048
        test   eax, eax
        je     SHORT loc_5_0F7F
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ecx, edi
        call   sub_5_0D09
        test   eax, eax
        je     SHORT loc_5_0F7F
        mov    ecx, edi
        call   sub_5_0D1E
        test   eax, eax
        je     SHORT loc_5_0F92
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   0
        mov    edx, edi
        push   -1
        xor    ecx, ecx
        push   eax
        push   9
        call   sub_5_CEA8
loc_5_0F7F:
        push   0
        mov    edx, 8002h
        mov    ecx, edi
        call   sub_5_6048
loc_5_0F8D:
        xor    eax, eax
loc_5_0F8F:
        pop    edi
        pop    esi
        ret
loc_5_0F92:
        push   8000000h
        mov    ecx, edi
        push   0
        call   sub_5_1906
        mov    eax, 1
        jmp    SHORT loc_5_0F8F
sub_5_0FA7:
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        mov    edx, ecx
        mov    ecx, 2
        call   sub_5_CBF2
        db  089h, 035h, 000h, 000h, 000h, 000h
        pop    esi
        ret
sub_5_0FD2:
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        mov    edx, ecx
        mov    ecx, 2
        call   sub_5_CBE2
        db  089h, 035h, 000h, 000h, 000h, 000h
        pop    esi
        ret
        db  056h, 068h, 000h, 000h, 000h, 000h, 08Bh, 035h, 000h, 000h, 000h, 000h, 089h, 015h, 000h, 000h
        db  000h, 000h, 08Bh, 0D1h, 0B9h, 003h, 000h, 000h, 000h, 0E8h, 0CCh, 0BBh, 000h, 000h, 089h, 035h
        db  000h, 000h, 000h, 000h, 05Eh, 0C3h
loc_5_101E:
        mov    ecx, dword ptr [esp + 4]
        cmp    dword ptr [ecx + 0ch], 1fh
        jne    SHORT loc_5_102D
        call   sub_5_7024
loc_5_102D:
        mov    eax, 1
        ret
sub_5_1033:
        sub    esp, 4
        mov    edx, 1
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        db  068h, 000h, 000h, 000h, 000h
        mov    ecx, esi
        call   sub_5_1B18
        test   eax, eax
        jne    SHORT loc_5_1055
        xor    edi, edi
        jmp    SHORT loc_5_109B
loc_5_1055:
        push   0
        mov    ecx, esi
        push   40h
        push   0
        push   40h
        call   sub_5_18E3
        test   eax, eax
        mov    eax, 1
        je     SHORT loc_5_1078
        db  0A1h, 000h, 000h, 000h, 000h
        and    eax, 8
        shr    eax, 2
loc_5_1078:
        mov    ecx, 1
        mov    edx, esi
        push   ecx
        push   0
        push   -1
        push   eax
        push   ecx
        call   sub_5_CEA8
        mov    edi, eax
        db  068h, 000h, 000h, 000h, 000h
        xor    edx, edx
        mov    ecx, esi
        call   sub_5_1B18
loc_5_109B:
        test   edi, edi
        je     SHORT loc_5_1106
        push   0
        mov    ecx, esi
        push   48h
        call   sub_5_1906
        lea    eax, [esp + 8]
        xor    edx, edx
        push   eax
        mov    ecx, esi
        call   sub_5_8D6E
        test   byte ptr [esp + 9], 2
        je     SHORT loc_5_10CD
        push   400000h
        mov    ecx, esi
        push   0
        call   sub_5_1906
loc_5_10CD:
        mov    ecx, esi
        call   sub_5_0BAC
        push   0
        mov    edx, 8000h
        mov    ecx, esi
        call   sub_5_6048
        push   0
        mov    edx, esi
        db  068h, 000h, 000h, 000h, 000h
        mov    ecx, 2
        call   sub_5_1A48
        call   sub_5_7B5F
        call   sub_5_73CA
        mov    eax, 1
        jmp    SHORT loc_5_1136
loc_5_1106:
        mov    ecx, esi
        call   sub_5_0BAC
        push   0
        mov    ecx, 40000h
        push   ecx
        push   0
        push   ecx
        mov    ecx, esi
        call   sub_5_18E3
        test   eax, eax
        mov    eax, 0
        jne    SHORT loc_5_1136
        mov    edx, 0ah
        mov    ecx, esi
        call   sub_5_731F
        xor    eax, eax
loc_5_1136:
        pop    edi
        pop    esi
        add    esp, 4
        ret
loc_5_113C:
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    edx, esi
        db  068h, 000h, 000h, 000h, 000h
        mov    ecx, 2
        call   sub_5_1A48
        push   edi
        mov    ecx, esi
        push   8
        push   edi
        push   8
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_11D6
        xor    edi, edi
        mov    edx, 8004h
        push   edi
        mov    ecx, esi
        call   sub_5_6048
        push   edi
        mov    eax, 400h
        push   edi
        mov    ecx, esi
        push   -1
        push   edi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        mov    edx, esi
        cmp    eax, 1
        sbb    eax, eax
        xor    ecx, ecx
        inc    eax
        push   eax
        push   2
        call   sub_5_CEA8
        push   edi
        mov    ecx, esi
        push   8
        call   sub_5_1923
        push   edi
        mov    eax, 100000h
        push   eax
        mov    ecx, esi
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_11CA
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 040h
        jne    SHORT loc_5_11CA
        mov    ecx, esi
        call   sub_5_1CB6
loc_5_11CA:
        mov    ecx, esi
        call   sub_5_0BAC
        call   sub_5_73CA
loc_5_11D6:
        mov    eax, 1
        pop    edi
        pop    esi
        ret
sub_5_11DE:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        mov    esi, 1
        mov    edx, esi
        call   sub_5_0FA7
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_5_1205
        mov    edx, esi
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_0FD2
        mov    eax, esi
loc_5_1205:
        pop    esi
        ret
loc_5_1207:
        xor    edx, edx
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_0FA7
        test   eax, eax
        mov    eax, 0
        je     SHORT loc_5_122B
        xor    edx, edx
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_0FD2
        mov    eax, 1
loc_5_122B:
        ret
sub_5_122C:
        mov    edx, dword ptr [esp + 4]
        db  068h, 000h, 000h, 000h, 000h
        mov    ecx, 2
        call   sub_5_CBE2
        ret
sub_5_1240:
        push   ebp
        mov    ebp, esp
        sub    esp, 194h
        push   ebx
        mov    dword ptr [ebp - 8], edx
        push   esi
        mov    ebx, ecx
        push   edi
        db  0BEh, 000h, 000h, 000h, 000h
        lea    edi, [ebp - 194h]
        xor    eax, eax
        movsd
        movsd
        movsw
        lea    edi, [ebp - 18ah]
        mov    ecx, 1dh
        rep stosd
        stosw
        db  0BEh, 000h, 000h, 000h, 000h
        lea    edi, [ebp - 14h]
        movsd
        movsd
        movsb
        db  0BEh, 000h, 000h, 000h, 000h
        lea    edi, [ebp - 94h]
        movsb
        lea    edi, [ebp - 93h]
        mov    edx, 1fh
        mov    ecx, edx
        rep stosd
        stosw
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ecx, edx
        stosb
        lea    edi, [ebp - 114h]
        movsb
        lea    edi, [ebp - 113h]
        rep stosd
        stosw
        stosb
        xor    edi, edi
        call   sub_5_C73C
        mov    word ptr [ebp - 2], ax
        mov    esi, dword ptr [ebp + 8]
        cmp    esi, edi
        je     SHORT loc_5_12CE
        cmp    dword ptr [esi + 4], edi
        jne    NEAR PTR loc_5_13CD
loc_5_12CE:
        lea    ecx, [ebp - 94h]
        mov    edx, dword ptr [ebp - 8]
        call   sub_5_C486
        test   ebx, ebx
        jne    SHORT loc_5_12E8
        lea    ecx, [ebp - 14h]
        jmp    NEAR PTR loc_5_138A
loc_5_12E8:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_131A
        db  068h, 000h, 000h, 000h, 000h
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   2
        call   sub_5_D0F4
        add    esp, 10h
        test   eax, eax
        je     SHORT loc_5_131A
        mov    ecx, dword ptr [ebp - 2]
        call   sub_5_C77D
        xor    eax, eax
        jmp    NEAR PTR loc_5_1400
loc_5_131A:
        mov    edi, 80h
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 194h]
        push   ecx
        db  0A1h, 000h, 000h, 000h, 000h
        push   0
        mov    dword ptr [ebp - 8], edi
        push   0
        push   ebx
        push   eax
        call   sub_5_D11C
        add    esp, 18h
        lea    ecx, [ebp - 114h]
        mov    edx, ebx
        mov    dword ptr [ebp - 8], edi
        lea    edi, [ebp - 8]
        call   sub_5_C486
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 114h]
        call   sub_5_C44E
        push   edi
        lea    eax, [ebp - 94h]
        push   eax
        lea    ecx, [ebp - 114h]
        push   0
        push   0
        push   ecx
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        call   sub_5_D11C
        add    esp, 18h
        lea    ecx, [ebp - 194h]
loc_5_138A:
        test   esi, esi
        jne    SHORT loc_5_13A8
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        lea    ebx, [ebp - 94h]
        push   ebx
        push   ecx
        call   sub_5_D104
        add    esp, 10h
        mov    edi, eax
        jmp    SHORT loc_5_13CD
loc_5_13A8:
        push   ecx
        call   sub_5_D114
        add    esp, 4
        mov    edi, eax
        cmp    edi, 20h
        jb     SHORT loc_5_13CD
        lea    eax, [ebp - 94h]
        mov    dword ptr [esi], edi
        push   eax
        push   edi
        call   sub_5_D10C
        add    esp, 8
        mov    dword ptr [esi + 4], eax
loc_5_13CD:
        test   esi, esi
        je     SHORT loc_5_13F6
        mov    ecx, dword ptr [esi + 4]
        test   ecx, ecx
        je     SHORT loc_5_13F6
        push   dword ptr [ebp + 10h]
        push   dword ptr [ebp + 0ch]
        push   ecx
        add    esi, 8
        push   0
        mov    ebx, dword ptr [esi]
        inc    ebx
        mov    dword ptr [esi], ebx
        call   sub_5_D104
        add    esp, 10h
        mov    edi, eax
        dec    ebx
        mov    dword ptr [esi], ebx
loc_5_13F6:
        mov    ecx, dword ptr [ebp - 2]
        call   sub_5_C77D
        mov    eax, edi
loc_5_1400:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
sub_5_1409:
        push   edi
        mov    eax, dword ptr [ecx]
        mov    edi, ecx
        test   eax, eax
        je     SHORT loc_5_142D
        cmp    dword ptr [edi + 8], 0
        jne    SHORT loc_5_1428
        push   eax
        call   sub_5_D124
        add    esp, 4
        xor    eax, eax
        stosd
        stosd
        stosd
        jmp    SHORT loc_5_142D
loc_5_1428:
        call   sub_5_7B5F
loc_5_142D:
        pop    edi
        ret
loc_5_142F:
        push   esi
        xor    esi, esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_144E
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_D0E4
        add    esp, 4
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_5_144E:
        pop    esi
        ret
sub_5_1450:
        cmp    edx, 1
        sbb    eax, eax
        sub    ecx, 4
        and    eax, 80000000h
        push   eax
        push   0
        push   574d5950h
        push   18h
        push   ecx
        call   sub_5_D12C
        add    esp, 14h
        ret
sub_5_1471:
        push   80000000h
        push   18h
        push   4c52574dh
        push   4
        push   ecx
        call   sub_5_D12C
        add    esp, 14h
        ret
sub_5_1489:
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        xor    edx, edx
        mov    ecx, dword ptr [esi]
        sub    ecx, 1ch
        call   sub_5_1471
        test   eax, eax
        je     SHORT loc_5_14A2
        sub    dword ptr [esi], 1ch
loc_5_14A2:
        pop    edi
        pop    esi
        ret
sub_5_14A5:
        push   201h
        push   18h
        call   sub_5_D134
        add    esp, 8
        test   eax, eax
        je     SHORT loc_5_14C1
        mov    dword ptr [eax], 574d5950h
        add    eax, 4
loc_5_14C1:
        ret
sub_5_14C2:
        push   esi
        push   201h
        lea    eax, [ecx + 1ch]
        mov    esi, ecx
        push   eax
        call   sub_5_D134
        add    esp, 8
        test   eax, eax
        je     SHORT loc_5_14E4
        mov    dword ptr [eax + 18h], 4c52574dh
        mov    dword ptr [eax + 14h], esi
loc_5_14E4:
        pop    esi
        ret
sub_5_14E6:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    edi, ecx
        xor    esi, esi
        call   sub_5_14A5
        mov    dword ptr [ebp - 4], eax
        cmp    eax, esi
        jne    SHORT loc_5_1508
        xor    eax, eax
        jmp    NEAR PTR loc_5_158F
loc_5_1508:
        push   14h
        mov    edx, edi
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_5B5D
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [edi + 10h]
        test   edi, edi
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 10h], 0
        je     SHORT loc_5_158C
loc_5_1534:
        mov    eax, dword ptr [edi + 14h]
        push   201h
        add    eax, 1ch
        push   eax
        mov    dword ptr [ebp - 8], eax
        call   sub_5_D134
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_5_1584
        mov    edx, edi
        mov    ecx, ebx
        push   dword ptr [ebp - 8]
        call   sub_5_5B5D
        mov    eax, dword ptr [ebp - 4]
        test   esi, esi
        mov    dword ptr [ebx + 10h], 0
        mov    dword ptr [ebx + 0ch], eax
        je     SHORT loc_5_1573
        mov    dword ptr [esi + 10h], ebx
        jmp    SHORT loc_5_1579
loc_5_1573:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 10h], ebx
loc_5_1579:
        mov    esi, ebx
        mov    edi, dword ptr [edi + 10h]
        test   edi, edi
        jne    SHORT loc_5_1534
        jmp    SHORT loc_5_158C
loc_5_1584:
        lea    ecx, [ebp - 4]
        call   sub_5_1596
loc_5_158C:
        mov    eax, dword ptr [ebp - 4]
loc_5_158F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_1596:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [ecx]
        mov    edi, ecx
        test   ebx, ebx
        je     SHORT loc_5_15E8
        mov    esi, dword ptr [ebx + 10h]
        test   esi, esi
        je     SHORT loc_5_15CA
        xor    ebp, ebp
loc_5_15AB:
        mov    eax, esi
        mov    dword ptr [esi + 18h], ebp
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        mov    esi, dword ptr [esi + 10h]
        jne    SHORT loc_5_15C6
        push   ebp
        push   eax
        call   sub_5_D0C8
        add    esp, 8
loc_5_15C6:
        test   esi, esi
        jne    SHORT loc_5_15AB
loc_5_15CA:
        lea    eax, [ebx - 4]
        xor    edx, edx
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [eax], edx
        mov    dword ptr [edi], ecx
        jne    SHORT loc_5_15E8
        push   edx
        push   eax
        call   sub_5_D0C8
        add    esp, 8
loc_5_15E8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_15ED:
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ecx]
        mov    edi, ecx
        test   esi, esi
        je     SHORT loc_5_160D
loc_5_15FA:
        lea    ecx, [esp + 8]
        mov    dword ptr [esp + 8], esi
        mov    esi, dword ptr [esi]
        call   sub_5_1596
        test   esi, esi
        jne    SHORT loc_5_15FA
loc_5_160D:
        mov    dword ptr [edi], 0
        pop    edi
        pop    esi
        add    esp, 4
        ret
sub_5_1619:
        push   esi
        mov    esi, dword ptr [ecx]
        cmp    edx, esi
        jne    SHORT loc_5_1624
        mov    eax, ecx
        jmp    SHORT loc_5_1643
loc_5_1624:
        cmp    dword ptr [esi], 0
        je     SHORT loc_5_1636
loc_5_1629:
        mov    eax, dword ptr [esi]
        cmp    edx, eax
        je     SHORT loc_5_1636
        cmp    dword ptr [eax], 0
        mov    esi, eax
        jne    SHORT loc_5_1629
loc_5_1636:
        mov    eax, 0
        cmp    dword ptr [esi], 1
        adc    eax, -1
        and    eax, esi
loc_5_1643:
        pop    esi
        ret
sub_5_1645:
        lea    eax, [ecx + 10h]
        mov    ecx, dword ptr [eax]
        cmp    ecx, edx
        je     SHORT loc_5_1673
        cmp    dword ptr [ecx + 10h], 0
        je     SHORT loc_5_1663
loc_5_1654:
        mov    eax, dword ptr [ecx + 10h]
        cmp    eax, edx
        je     SHORT loc_5_1663
        cmp    dword ptr [eax + 10h], 0
        mov    ecx, eax
        jne    SHORT loc_5_1654
loc_5_1663:
        add    ecx, 10h
        mov    eax, 0
        cmp    dword ptr [ecx], 1
        adc    eax, -1
        and    eax, ecx
loc_5_1673:
        ret
sub_5_1674:
        mov    eax, dword ptr [esp + 8]
        push   ebx
        push   edi
        mov    dword ptr [edx + 4], ecx
        mov    edi, dword ptr [esp + 0ch]
        lea    ebx, [ecx + eax*4 + 30h]
        mov    dword ptr [edx + 0ch], edi
        mov    dword ptr [edx + 8], eax
        cmp    dword ptr [ebx], 0
        je     SHORT loc_5_16A9
loc_5_1690:
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ecx + 0ch]
        cmp    edi, eax
        jb     SHORT loc_5_16A9
        jne    SHORT loc_5_16A2
        cmp    dword ptr [esp + 14h], 0
        jne    SHORT loc_5_16A9
loc_5_16A2:
        cmp    dword ptr [ecx], 0
        mov    ebx, ecx
        jne    SHORT loc_5_1690
loc_5_16A9:
        mov    eax, dword ptr [ebx]
        pop    edi
        mov    dword ptr [edx], eax
        mov    dword ptr [ebx], edx
        pop    ebx
        ret    0ch
sub_5_16B4:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        call   sub_5_14A5
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_5_16DA
        mov    edx, ebx
        mov    ecx, esi
        push   dword ptr [esp + 14h]
        push   dword ptr [esp + 14h]
        push   edi
        call   sub_5_1674
        jmp    SHORT loc_5_16E6
loc_5_16DA:
        mov    edx, 3
        mov    ecx, esi
        call   sub_5_731F
loc_5_16E6:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_5_16EE:
        push   esi
        mov    edx, dword ptr [ecx]
        mov    esi, ecx
        mov    eax, dword ptr [edx + 8]
        mov    ecx, dword ptr [edx + 4]
        lea    ecx, [ecx + eax*4 + 30h]
        call   sub_5_1619
        mov    ecx, eax
        call   sub_5_1596
        mov    dword ptr [esi], 0
        pop    esi
        ret
sub_5_1711:
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    edi, ecx
        mov    ecx, dword ptr [esp + 18h]
        call   sub_5_14C2
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_5_1738
        mov    edx, 3
        mov    ecx, dword ptr [ebx + 4]
        call   sub_5_731F
        xor    eax, eax
        jmp    SHORT loc_5_175B
loc_5_1738:
        mov    eax, dword ptr [esp + 10h]
        lea    ecx, [esi + 1ch]
        mov    edx, dword ptr [esp + 14h]
        mov    dword ptr [esi + 4], eax
        push   dword ptr [esp + 18h]
        mov    dword ptr [esi + 0ch], ebx
        call   sub_5_5B5D
        mov    ecx, dword ptr [edi]
        mov    eax, esi
        mov    dword ptr [esi + 10h], ecx
        mov    dword ptr [edi], esi
loc_5_175B:
        pop    edi
        pop    esi
        pop    ebx
        ret    0ch
sub_5_1761:
        push   esi
        mov    esi, ecx
        lea    ecx, [ecx + 10h]
        cmp    dword ptr [ecx], 0
        je     SHORT loc_5_1776
loc_5_176C:
        mov    ecx, dword ptr [ecx]
        add    ecx, 10h
        cmp    dword ptr [ecx], 0
        jne    SHORT loc_5_176C
loc_5_1776:
        push   dword ptr [esp + 0ch]
        push   dword ptr [esp + 0ch]
        push   edx
        mov    edx, esi
        call   sub_5_1711
        pop    esi
        ret    8
sub_5_178A:
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        push   ebp
        mov    esi, ecx
        mov    ebp, dword ptr [esp + 18h]
        cmp    dword ptr [esi + 14h], ebp
        jne    SHORT loc_5_17EA
        mov    edi, esi
loc_5_179D:
        push   ebp
        lea    ecx, [edi + 1ch]
        mov    edx, dword ptr [esp + 18h]
        mov    dword ptr [edi + 4], ebx
        call   sub_5_5B5D
        cmp    edi, esi
        je     SHORT loc_5_17E1
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [esi + 10h]
        mov    edx, esi
        mov    dword ptr [edi + 0ch], eax
        mov    dword ptr [edi + 10h], ecx
        mov    ecx, dword ptr [esi + 0ch]
        call   sub_5_1645
        mov    dword ptr [eax], edi
        xor    eax, eax
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        mov    dword ptr [esi + 18h], eax
        jne    SHORT loc_5_17E1
        push   eax
        push   esi
        call   sub_5_D0C8
        add    esp, 8
loc_5_17E1:
        mov    eax, edi
loc_5_17E3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_5_17EA:
        mov    ecx, ebp
        call   sub_5_14C2
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_5_179D
        xor    eax, eax
        jmp    SHORT loc_5_17E3
sub_5_17FB:
        push   esi
        push   edi
        mov    edi, dword ptr [ecx + 0ch]
        mov    edx, ecx
        mov    esi, ecx
        mov    ecx, edi
        call   sub_5_1645
        mov    ecx, dword ptr [esi + 10h]
        mov    dword ptr [eax], ecx
        mov    ecx, edi
        sub    ecx, eax
        cmp    ecx, -10h
        je     SHORT loc_5_181C
        lea    edi, [eax + 0ch]
loc_5_181C:
        xor    eax, eax
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        mov    dword ptr [esi + 18h], eax
        jne    SHORT loc_5_1834
        push   eax
        push   esi
        call   sub_5_D0C8
        add    esp, 8
loc_5_1834:
        mov    eax, edi
        pop    edi
        pop    esi
        ret
sub_5_1839:
        test   edx, edx
        je     SHORT loc_5_1847
        mov    eax, 0
        cmp    dword ptr [ecx + 4], edx
        jne    SHORT loc_5_184C
loc_5_1847:
        mov    eax, 1
loc_5_184C:
        ret
sub_5_184D:
        push   esi
        push   edi
        mov    esi, edx
        mov    edi, dword ptr [ecx + 10h]
        test   edi, edi
        jne    SHORT loc_5_185C
        xor    eax, eax
        jmp    SHORT loc_5_1874
loc_5_185C:
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_1839
        test   eax, eax
        mov    eax, edi
        jne    SHORT loc_5_1874
        mov    edx, esi
        mov    ecx, edi
        call   sub_5_1877
loc_5_1874:
        pop    edi
        pop    esi
        ret
sub_5_1877:
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
loc_5_187D:
        mov    esi, dword ptr [esi + 10h]
        test   esi, esi
        je     SHORT loc_5_1896
        mov    edx, edi
        mov    ecx, esi
        call   sub_5_1839
        test   eax, eax
        je     SHORT loc_5_187D
        mov    eax, esi
loc_5_1893:
        pop    edi
        pop    esi
        ret
loc_5_1896:
        xor    eax, eax
        jmp    SHORT loc_5_1893
sub_5_189A:
        test   ecx, ecx
        jne    SHORT loc_5_18A2
        xor    eax, eax
        jmp    SHORT loc_5_18B5
loc_5_18A2:
        mov    eax, dword ptr [ecx + 10h]
        test   eax, eax
        je     SHORT loc_5_18B5
loc_5_18A9:
        test   edx, edx
        je     SHORT loc_5_18B5
        mov    eax, dword ptr [eax + 10h]
        dec    edx
        test   eax, eax
        jne    SHORT loc_5_18A9
loc_5_18B5:
        ret
sub_5_18B6:
        push   ebp
        mov    eax, dword ptr [ecx + 8]
        mov    ebp, esp
        mov    edx, dword ptr [ecx + 88h]
        sub    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    esp, ebp
        pop    ebp
        ret
sub_5_18CF:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 88h], edx
        ret    8
sub_5_18E3:
        push   ebp
        mov    ebp, esp
        call   sub_5_18B6
        and    eax, dword ptr [ebp + 8]
        and    edx, dword ptr [ebp + 0ch]
        cmp    edx, dword ptr [ebp + 14h]
        jne    SHORT loc_5_1900
        cmp    eax, dword ptr [ebp + 10h]
        mov    eax, 1
        je     SHORT loc_5_1902
loc_5_1900:
        xor    eax, eax
loc_5_1902:
        pop    ebp
        ret    10h
sub_5_1906:
        push   esi
        mov    esi, ecx
        call   sub_5_18B6
        or     eax, dword ptr [esp + 8]
        or     edx, dword ptr [esp + 0ch]
        push   edx
        mov    ecx, esi
        push   eax
        call   sub_5_18CF
        pop    esi
        ret    8
sub_5_1923:
        push   esi
        push   edi
        mov    edi, ecx
        call   sub_5_18B6
        mov    ecx, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
        not    ecx
        not    esi
        and    eax, ecx
        and    edx, esi
        push   edx
        mov    ecx, edi
        push   eax
        call   sub_5_18CF
        pop    edi
        pop    esi
        ret    8
sub_5_194A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cmp    ecx, 0c0000000h
        mov    dword ptr [ebp - 4], ecx
        jb     NEAR PTR loc_5_1A24
        cmp    ecx, 0ffbfffffh
        ja     NEAR PTR loc_5_1A24
        test   cl, 3
        jne    NEAR PTR loc_5_1A24
        xor    edx, edx
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_1989
        mov    eax, 1
        jmp    NEAR PTR loc_5_1A26
loc_5_1989:
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_1450
        test   eax, eax
        je     SHORT loc_5_19A1
        mov    eax, 2
        jmp    NEAR PTR loc_5_1A26
loc_5_19A1:
        lea    ecx, [ebp - 4]
        xor    edx, edx
        call   sub_5_1489
        test   eax, eax
        je     SHORT loc_5_19B6
        mov    eax, 3
        jmp    SHORT loc_5_1A26
loc_5_19B6:
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_8317
        test   eax, eax
        je     SHORT loc_5_19CB
        mov    eax, 4
        jmp    SHORT loc_5_1A26
loc_5_19CB:
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_8335
        test   eax, eax
        je     SHORT loc_5_19E0
        mov    eax, 5
        jmp    SHORT loc_5_1A26
loc_5_19E0:
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_6723
        test   eax, eax
        je     SHORT loc_5_19F5
        mov    eax, 6
        jmp    SHORT loc_5_1A26
loc_5_19F5:
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_6741
        test   eax, eax
        je     SHORT loc_5_1A0A
        mov    eax, 7
        jmp    SHORT loc_5_1A26
loc_5_1A0A:
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_1471
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 8
        jmp    SHORT loc_5_1A26
loc_5_1A24:
        xor    eax, eax
loc_5_1A26:
        mov    esp, ebp
        pop    ebp
        ret
loc_5_1A2A:
        mov    ecx, dword ptr [esp + 4]
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 74h], eax
        mov    eax, 1
        jne    SHORT loc_5_1A47
        push   ecx
        db  0FFh, 015h, 000h, 000h, 000h, 000h
        add    esp, 4
loc_5_1A47:
        ret
sub_5_1A48:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        db  08Bh, 01Dh, 000h, 000h, 000h, 000h
        push   edi
        mov    esi, ecx
        db  08Bh, 03Dh, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        lea    edx, [ebp - 8]
        push   edx
        push   0
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        call   sub_5_CB5A
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    SHORT loc_5_1AAE
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        jne    SHORT loc_5_1AAE
        db  068h, 000h, 000h, 000h, 000h
        lea    ecx, [esi + 4]
        push   0
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_5_CB5A
loc_5_1AAE:
        mov    eax, dword ptr [ebp - 4]
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        pop    edi
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_5_1AC6:
        cmp    edx, 1
        sbb    eax, eax
        and    eax, 80000000h
        push   eax
        push   0
        push   59504c52h
        push   8ch
        push   ecx
        call   sub_5_D12C
        add    esp, 14h
        ret
sub_5_1AE7:
        push   ebx
        push   esi
        mov    esi, ecx
        push   0
        mov    ebx, 20000h
        push   ebx
        push   0
        push   ebx
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_1B13
loc_5_1AFF:
        mov    esi, dword ptr [esi + 20h]
        push   0
        push   ebx
        mov    ecx, esi
        push   0
        push   ebx
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_1AFF
loc_5_1B13:
        mov    eax, esi
        pop    esi
        pop    ebx
        ret
sub_5_1B18:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 010h
        push   ebx
        push   esi
        push   edi
        mov    esi, edx
        je     SHORT loc_5_1B33
        mov    eax, 1
        jmp    SHORT loc_5_1B8A
loc_5_1B33:
        mov    ebx, dword ptr [ecx + 4]
        cmp    byte ptr [ebx], 0
        jne    SHORT loc_5_1B42
        mov    eax, 1
        jmp    SHORT loc_5_1B8A
loc_5_1B42:
        xor    ecx, ecx
        xor    edx, edx
        db  0CDh, 020h, 000h, 000h, 001h, 000h
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 40ah
        mov    eax, 1
        jb     SHORT loc_5_1B8A
        push   0
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        dec    ecx
        cmp    esi, 1
        sbb    eax, eax
        push   ecx
        push   ebx
        and    eax, 3
        push   dword ptr [ebp + 8]
        push   eax
        call   sub_5_D13C
        add    esp, 14h
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_5_1B8A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_1B93:
        push   esi
        mov    ecx, 1000h
        mov    esi, dword ptr [esp + 8]
        push   0
        push   ecx
        push   0
        push   ecx
        mov    ecx, esi
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_1BB5
        mov    eax, 1
        jmp    SHORT loc_5_1BDA
loc_5_1BB5:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esi + 4]
        push   eax
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_C3FC
        test   eax, eax
        mov    eax, 1
        je     SHORT loc_5_1BDA
        xor    eax, eax
        db  089h, 035h, 000h, 000h, 000h, 000h
loc_5_1BDA:
        pop    esi
        ret
sub_5_1BDC:
        push   esi
        mov    esi, ecx
        call   sub_5_C3C4
        test   eax, eax
        jne    SHORT loc_5_1BEC
        xor    eax, eax
        jmp    SHORT loc_5_1C1C
loc_5_1BEC:
        inc    eax
        db  068h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ecx, 2
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_5_CBE2
        db  0A1h, 000h, 000h, 000h, 000h
loc_5_1C1C:
        pop    esi
        ret
sub_5_1C1E:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    ecx, edx
        push   ebp
        mov    edi, edx
        add    esi, 8ch
        call   sub_5_C3C4
        mov    ebp, eax
        push   201h
        lea    eax, [esi + ebp]
        inc    eax
        push   eax
        call   sub_5_D134
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     SHORT loc_5_1C6C
        lea    ecx, [esi + ebx]
        push   ebp
        mov    edx, edi
        mov    dword ptr [ebx + 4], ecx
        call   sub_5_5B5D
        mov    eax, 1
        mov    dword ptr [ebx], 59504c52h
        mov    dword ptr [ebx + 78h], eax
        mov    dword ptr [ebx + 7ch], eax
loc_5_1C6C:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_1C73:
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        mov    ecx, edi
        call   sub_5_1D31
        lea    ecx, [eax*4 + 4]
        mov    edx, esi
        call   sub_5_1C1E
        test   eax, eax
        je     SHORT loc_5_1CB3
        mov    ecx, dword ptr [edi + 24h]
        mov    dword ptr [eax + 20h], edi
        test   ecx, ecx
        je     SHORT loc_5_1CB0
        cmp    dword ptr [ecx + 28h], 0
        je     SHORT loc_5_1CAB
loc_5_1CA2:
        mov    ecx, dword ptr [ecx + 28h]
        cmp    dword ptr [ecx + 28h], 0
        jne    SHORT loc_5_1CA2
loc_5_1CAB:
        mov    dword ptr [ecx + 28h], eax
        jmp    SHORT loc_5_1CB3
loc_5_1CB0:
        mov    dword ptr [edi + 24h], eax
loc_5_1CB3:
        pop    edi
        pop    esi
        ret
sub_5_1CB6:
        push   edi
        push   80000000h
        mov    edi, ecx
        push   0
        call   sub_5_1923
        push   0
        mov    eax, 20000h
        push   eax
        mov    ecx, edi
        push   0
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_1D27
        mov    edx, dword ptr [edi + 38h]
        test   edx, edx
        je     SHORT loc_5_1D16
        mov    ecx, edi
        call   sub_5_058A
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, edi
        mov    edx, dword ptr [eax + 38h]
        call   sub_5_0969
        test   eax, eax
        je     SHORT loc_5_1D11
        mov    ecx, 1
        call   sub_5_099C
        test   eax, eax
        jne    SHORT loc_5_1D11
        call   sub_5_0A32
        jmp    SHORT loc_5_1D16
loc_5_1D11:
        call   sub_5_09DB
loc_5_1D16:
        push   10000000h
        mov    ecx, edi
        push   800h
        call   sub_5_1923
loc_5_1D27:
        lea    ecx, [edi + 38h]
        call   sub_5_15ED
        pop    edi
        ret
sub_5_1D31:
        xor    eax, eax
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        cmp    edx, ecx
        je     SHORT loc_5_1D45
loc_5_1D3D:
        inc    eax
        mov    ecx, dword ptr [ecx + 20h]
        cmp    edx, ecx
        jne    SHORT loc_5_1D3D
loc_5_1D45:
        ret
sub_5_1D46:
        push   esi
        mov    esi, ecx
        mov    ecx, edx
        call   sub_5_1D31
        lea    eax, [esi + eax*4 + 88h]
        pop    esi
        ret
sub_5_1D59:
        test   ecx, ecx
        je     SHORT loc_5_1D68
loc_5_1D5D:
        cmp    edx, ecx
        je     SHORT loc_5_1D6B
        mov    ecx, dword ptr [ecx + 20h]
        test   ecx, ecx
        jne    SHORT loc_5_1D5D
loc_5_1D68:
        xor    eax, eax
loc_5_1D6A:
        ret
loc_5_1D6B:
        mov    eax, 1
        jmp    SHORT loc_5_1D6A
sub_5_1D72:
        push   ebx
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    eax, edx
        push   esi
        push   edi
        mov    ebx, edx
        mov    edi, ecx
        je     SHORT loc_5_1DB2
        cmp    eax, edi
        je     SHORT loc_5_1DB2
        mov    esi, 1
        mov    edx, esi
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_1DB2
        mov    edx, esi
        mov    ecx, ebx
        call   sub_5_1AC6
        test   eax, eax
        je     SHORT loc_5_1DB2
        mov    edx, ebx
        mov    ecx, edi
        call   sub_5_1D59
        test   eax, eax
        mov    eax, esi
        jne    SHORT loc_5_1DB4
loc_5_1DB2:
        xor    eax, eax
loc_5_1DB4:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_1DB8:
        mov    eax, 1
        ret
sub_5_1DBE:
        push   ebp
        mov    ebp, esp
        push   esi
        cmp    ecx, 13h
        mov    esi, ecx
        ja     SHORT loc_5_1DDB
        je     SHORT loc_5_1E44
        test   esi, esi
        je     SHORT loc_5_1E44
        cmp    esi, 1
        je     SHORT loc_5_1DF6
        cmp    esi, 2
        je     SHORT loc_5_1DFA
        jmp    SHORT loc_5_1E3F
loc_5_1DDB:
        cmp    esi, 17h
        je     SHORT loc_5_1E3F
        cmp    esi, 22h
        je     SHORT loc_5_1E09
        cmp    esi, 24h
        je     SHORT loc_5_1E10
        cmp    esi, 29h
        je     SHORT loc_5_1E23
        cmp    esi, 2bh
        je     SHORT loc_5_1E32
        jmp    SHORT loc_5_1E3F
loc_5_1DF6:
        mov    esi, edx
        jmp    SHORT loc_5_1E44
loc_5_1DFA:
        mov    edx, 3
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_731F
        jmp    SHORT loc_5_1E44
loc_5_1E09:
        mov    edx, 0eh
        jmp    SHORT loc_5_1E37
loc_5_1E10:
        push   0
        mov    ecx, dword ptr [ebp + 8]
        push   8
        call   sub_5_1923
        mov    edx, 18h
        jmp    SHORT loc_5_1E37
loc_5_1E23:
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_1CB6
        mov    edx, 18h
        jmp    SHORT loc_5_1E37
loc_5_1E32:
        mov    edx, 1eh
loc_5_1E37:
        mov    ecx, dword ptr [ebp + 8]
        call   sub_5_731F
loc_5_1E3F:
        mov    esi, 13h
loc_5_1E44:
        mov    eax, esi
        pop    esi
        pop    ebp
        ret    4
sub_5_1E4B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        cmp    ecx, 3
        mov    edi, edx
        mov    esi, ecx
        je     SHORT loc_5_1E63
        cmp    esi, 8
        je     SHORT loc_5_1EAB
        xor    ebx, ebx
        jmp    SHORT loc_5_1EBE
loc_5_1E63:
        cmp    edi, 1
        je     SHORT loc_5_1E6C
        xor    ebx, ebx
        jmp    SHORT loc_5_1EBE
loc_5_1E6C:
        push   0
        mov    eax, 4000h
        push   eax
        mov    ecx, dword ptr [ebp + 0ch]
        push   0
        push   eax
        call   sub_5_18E3
        push   0
        cmp    eax, 1
        sbb    ebx, ebx
        push   8
        push   0
        mov    ecx, dword ptr [ebp + 0ch]
        neg    ebx
        push   8
        call   sub_5_18E3
        and    ebx, eax
        and    ebx, dword ptr [ebp + 10h]
        cmp    ebx, 1
        mov    ebx, 0
        adc    ebx, -1
        and    ebx, 13h
        jmp    SHORT loc_5_1EBE
loc_5_1EAB:
        cmp    edi, 9
        jb     SHORT loc_5_1EB5
        cmp    edi, 10h
        jbe    SHORT loc_5_1EB9
loc_5_1EB5:
        xor    ebx, ebx
        jmp    SHORT loc_5_1EBE
loc_5_1EB9:
        mov    ebx, 13h
loc_5_1EBE:
        mov    ecx, dword ptr [ebp + 8]
        xor    edx, edx
        cmp    dword ptr [ecx], edx
        je     SHORT loc_5_1EFE
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax], edx
        je     SHORT loc_5_1EFE
        cmp    dword ptr [ebp + 10h], edx
        jne    SHORT loc_5_1EE8
        mov    eax, dword ptr [ecx + 1ch]
        test   eax, eax
        je     SHORT loc_5_1EFE
        push   edx
        push   dword ptr [ebp + 0ch]
        push   ecx
        push   edi
        push   esi
        call   eax
        add    esp, 14h
        jmp    SHORT loc_5_1F03
loc_5_1EE8:
        mov    eax, dword ptr [ecx + 18h]
        test   eax, eax
        je     SHORT loc_5_1EFE
        push   0
        push   dword ptr [ecx + 14h]
        push   ecx
        push   edi
        push   esi
        call   eax
        add    esp, 14h
        jmp    SHORT loc_5_1F03
loc_5_1EFE:
        mov    eax, 1
loc_5_1F03:
        mov    edx, ebx
        mov    ecx, eax
        push   dword ptr [ebp + 0ch]
        call   sub_5_1DBE
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0ch
sub_5_1F16:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 14h]
        test   edi, 1
        je     SHORT loc_5_1F39
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   2000h
        mov    ecx, esi
        call   sub_5_1906
        jmp    SHORT loc_5_1F3C
loc_5_1F39:
        mov    esi, dword ptr [ebp + 8]
loc_5_1F3C:
        test   edi, 2
        je     SHORT loc_5_1F52
        push   0
        mov    ecx, esi
        push   4000h
        call   sub_5_1906
loc_5_1F52:
        mov    eax, dword ptr [ebp + 0ch]
        test   eax, eax
        mov    dword ptr [esi + 18h], eax
        jne    SHORT loc_5_1F63
        db  0C7h, 046h, 018h, 000h, 000h, 000h, 000h
loc_5_1F63:
        mov    eax, dword ptr [ebp + 10h]
        push   0
        push   22h
        mov    ecx, esi
        mov    dword ptr [esi + 14h], eax
        call   sub_5_1906
        push   0
        mov    ecx, esi
        push   8
        call   sub_5_1923
        test   edi, 4
        je     SHORT loc_5_1F8E
        call   sub_5_7B5F
        jmp    SHORT loc_5_1FAC
loc_5_1F8E:
        push   200000h
        xor    edi, edi
        push   edi
        mov    ecx, esi
        call   sub_5_1906
        push   edi
        db  0B9h, 000h, 000h, 000h, 000h
        push   edi
        mov    edx, esi
        push   edi
        call   sub_5_02D3
loc_5_1FAC:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_5_1FB0:
        mov    edx, 16h
        mov    ecx, dword ptr [esp + 4]
        jmp    NEAR PTR sub_5_731F
sub_5_1FBE:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_5_122C
        add    esp, 4
        mov    ecx, 20000h
        push   0
        push   ecx
        push   0
        push   ecx
        mov    ecx, esi
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_1FFA
        push   90000000h
        mov    ecx, esi
        push   800h
        call   sub_5_1923
        mov    ecx, esi
        call   sub_5_1CB6
loc_5_1FFA:
        mov    ecx, esi
        call   sub_5_7024
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_5_2013
        push   esi
        call   sub_5_79CB
        add    esp, 4
        jmp    SHORT loc_5_2018
loc_5_2013:
        call   sub_5_73CA
loc_5_2018:
        pop    esi
        ret
loc_5_201A:
        push   esi
        mov    eax, 400h
        mov    esi, dword ptr [esp + 8]
        push   edi
        xor    edi, edi
        mov    ecx, esi
        push   edi
        push   eax
        push   edi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_206E
        mov    eax, 1000000h
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_206E
        push   edi
        push   esi
        call   sub_5_1FBE
        add    esp, 8
        xor    edx, edx
        mov    ecx, 0fh
        push   edi
        push   esi
        push   dword ptr [esi + 20h]
        call   sub_5_1E4B
        push   esi
        call   sub_5_79CB
        add    esp, 4
loc_5_206E:
        pop    edi
        pop    esi
        ret
loc_5_2071:
        sub    esp, 0cch
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0d8h]
        push   edi
        push   ebp
        mov    ecx, esi
        xor    ebx, ebx
        call   sub_5_8CCD
        push   ebx
        xor    edx, edx
        push   esi
        mov    ecx, 6
        push   dword ptr [esi + 20h]
        call   sub_5_1E4B
        push   ebx
        mov    eax, 80h
        push   eax
        mov    ecx, dword ptr [esi + 20h]
        push   ebx
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_210A
        mov    ebp, dword ptr [esi + 4]
        mov    ecx, ebp
        call   sub_5_C3C4
        lea    ecx, [esp + 10h]
        push   eax
        mov    edx, ebp
        call   sub_5_5B5D
        mov    edx, 5ch
        mov    ecx, ebp
        call   sub_5_C3E0
        lea    ecx, [ebp + eax + 1]
        mov    edi, eax
        mov    edx, 5ch
        call   sub_5_C3E0
        lea    eax, [edi + eax + 1]
        push   1
        lea    edx, [esp + 14h]
        mov    ecx, esi
        mov    byte ptr [esp + eax + 14h], bl
        call   sub_5_21CF
        lea    edx, [esp + edi + 11h]
        cmp    byte ptr [edx], 2ah
        jne    SHORT loc_5_210A
        push   1
        mov    ecx, esi
        call   sub_5_21CF
loc_5_210A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0cch
        ret
loc_5_2115:
        push   esi
        xor    edx, edx
        mov    esi, dword ptr [esp + 8]
        push   1
        push   esi
        mov    ecx, 10h
        push   esi
        call   sub_5_1E4B
        push   0
        xor    edx, edx
        push   esi
        mov    ecx, 10h
        push   esi
        call   sub_5_1E4B
        pop    esi
        ret
loc_5_213C:
        push   ebp
        mov    ecx, 11h
        mov    ebp, esp
        push   dword ptr [esp + 10h]
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0ch]
        call   sub_5_1E4B
        pop    ebp
        ret
sub_5_2158:
        push   ebx
        push   esi
        push   edi
        mov    esi, edx
        mov    edi, ecx
loc_5_215F:
        mov    edx, 2ch
        mov    ecx, edi
        call   sub_5_C3E0
        cmp    eax, -1
        je     SHORT loc_5_218F
        lea    ebx, [eax + edi]
        mov    edx, esi
        push   dword ptr [esp + 10h]
        mov    ecx, edi
        mov    byte ptr [ebx], 0
        call   sub_5_C3FC
        mov    byte ptr [ebx], 2ch
        test   eax, eax
        jne    SHORT loc_5_219E
        lea    edi, [ebx + 1]
        jmp    SHORT loc_5_215F
loc_5_218F:
        mov    edx, esi
        mov    ecx, edi
        push   dword ptr [esp + 10h]
        call   sub_5_C3FC
        jmp    SHORT loc_5_21A3
loc_5_219E:
        mov    eax, 1
loc_5_21A3:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_5_21A9:
        push   esi
        mov    esi, ecx
        call   sub_5_C430
        cmp    byte ptr [esi], 0
        je     SHORT loc_5_21CD
loc_5_21B6:
        mov    al, byte ptr [esi]
        cmp    al, 20h
        jle    SHORT loc_5_21C4
        cmp    al, 7fh
        jg     SHORT loc_5_21C4
        cmp    al, 2ch
        jne    SHORT loc_5_21C7
loc_5_21C4:
        mov    byte ptr [esi], 5fh
loc_5_21C7:
        inc    esi
        cmp    byte ptr [esi], 0
        jne    SHORT loc_5_21B6
loc_5_21CD:
        pop    esi
        ret
sub_5_21CF:
        push   ebp
        mov    ebp, esp
        sub    esp, 404h
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    ecx, edx
        mov    edi, edx
        call   sub_5_C3C4
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_5_22A6
        mov    eax, 3feh
        push   0
        sub    eax, ebx
        lea    ecx, [ebp - 404h]
        cmp    dword ptr [ebp + 8], 0
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        push   1
        db  0B8h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_2219
        db  0B8h, 000h, 000h, 000h, 000h
loc_5_2219:
        push   eax
        xor    edx, edx
        mov    ecx, esi
        call   sub_5_87D6
        test   eax, eax
        je     SHORT loc_5_222E
        cmp    eax, 25h
        je     SHORT loc_5_2253
        jmp    SHORT loc_5_22A6
loc_5_222E:
        inc    ebx
        lea    ecx, [ebp - 404h]
        push   ebx
        mov    edx, edi
        call   sub_5_2158
        test   eax, eax
        jne    SHORT loc_5_22A6
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 404h]
        call   sub_5_C44E
        jmp    SHORT loc_5_225A
loc_5_2253:
        mov    byte ptr [ebp - 404h], 0
loc_5_225A:
        lea    ecx, [ebp - 404h]
        call   sub_5_C3C4
        mov    ebx, eax
        lea    ecx, [ebp - 404h]
        mov    edx, edi
        call   sub_5_C44E
        lea    ecx, [ebp + ebx - 404h]
        call   sub_5_21A9
        xor    eax, eax
        lea    ecx, [ebp - 404h]
        push   eax
        cmp    dword ptr [ebp + 8], eax
        push   eax
        push   ecx
        push   1
        db  0B8h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_229C
        db  0B8h, 000h, 000h, 000h, 000h
loc_5_229C:
        push   eax
        xor    edx, edx
        mov    ecx, esi
        call   sub_5_886D
loc_5_22A6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_22AF:
        push   ebp
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    edx, [ebp - 4]
        xor    esi, esi
        push   esi
        push   eax
        db  0FFh, 035h, 000h, 000h, 000h, 000h
        push   edx
        call   _CONFIGMG_Create_DevNode
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_5_22FE
        push   esi
        push   esi
        push   dword ptr [ebp - 4]
        call   _CONFIGMG_Register_Enumerator
        add    esp, 0ch
        mov    ecx, dword ptr [ebp - 4]
        push   esi
        push   800h
        call   sub_5_1906
        push   esi
        push   esi
        push   esi
        push   dword ptr [ebp - 4]
        call   sub_5_1F16
        add    esp, 10h
loc_5_22FE:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_5_2303:
        mov    ecx, dword ptr [esp + 4]
        db  0A1h, 000h, 000h, 000h, 000h
        cmp    dword ptr [ecx + 0ch], eax
        jne    SHORT loc_5_2316
        call   sub_5_7024
loc_5_2316:
        mov    eax, 1
        ret
sub_5_231C:
        db  0BAh, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        jmp    NEAR PTR sub_5_CC30
loc_5_232E:
        push   esi
        mov    eax, 40000h
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [esp + 0ch]
        push   esi
        push   eax
        mov    ecx, edi
        push   esi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    NEAR PTR loc_5_23D7
        cmp    dword ptr [edi + 40h], esi
        jne    NEAR PTR loc_5_23D7
        push   esi
        mov    eax, 4000h
        push   eax
        mov    ecx, edi
        push   esi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_23D7
        push   esi
        mov    eax, 1000h
        push   eax
        mov    ecx, edi
        push   esi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_23D7
        push   esi
        mov    eax, 20000h
        push   eax
        mov    ecx, edi
        push   esi
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_23D7
        cmp    dword ptr [edi + 38h], esi
        je     SHORT loc_5_23D7
        mov    eax, dword ptr [edi + 34h]
        test   eax, eax
        je     SHORT loc_5_23D7
        cmp    dword ptr [eax + 0ch], 7fffh
        ja     SHORT loc_5_23D7
        cmp    dword ptr [edi + 3ch], esi
        je     SHORT loc_5_23C1
        mov    eax, 800000h
        mov    ecx, edi
        push   eax
        push   esi
        push   eax
        push   esi
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_23D7
loc_5_23C1:
        push   edi
        call   sub_5_11DE
        add    esp, 4
        test   eax, eax
        je     SHORT loc_5_23D7
        push   edi
        call   sub_5_9014
        add    esp, 4
loc_5_23D7:
        mov    eax, 1
        pop    edi
        pop    esi
        ret
sub_5_23DF:
        cmp    dword ptr [esp + 4], 0
        jne    SHORT loc_5_2412
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_240A
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, 1
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        call   sub_5_CC30
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
loc_5_240A:
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_5_2469
loc_5_2412:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_5_2469
        cmp    eax, 1
        jne    SHORT loc_5_2463
        push   0
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  080h, 025h, 001h, 000h, 000h, 000h, 0FDh
        push   20h
        call   sub_5_1906
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_79CB
        add    esp, 4
        mov    ecx, 1ah
        db  080h, 00Dh, 001h, 000h, 000h, 000h, 002h
        call   sub_5_231C
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_79CB
        add    esp, 4
loc_5_2463:
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
loc_5_2469:
        ret
sub_5_246A:
        mov    eax, dword ptr [ecx + 10h]
        test   eax, eax
        je     SHORT loc_5_2487
loc_5_2471:
        mov    ecx, dword ptr [eax + 4]
        and    ch, 7fh
        cmp    ecx, 4
        jne    SHORT loc_5_2480
        or     byte ptr [eax + 1ch], 1
loc_5_2480:
        mov    eax, dword ptr [eax + 10h]
        test   eax, eax
        jne    SHORT loc_5_2471
loc_5_2487:
        ret
sub_5_2488:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, ecx
        mov    ecx, dword ptr [ecx + 38h]
        test   ecx, ecx
        je     SHORT loc_5_24E1
        mov    edx, 2
        call   sub_5_14E6
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_5_250C
        mov    ecx, eax
        call   sub_5_246A
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, edi
        call   sub_5_0969
        test   eax, eax
        je     SHORT loc_5_250C
        mov    ecx, 6
        call   sub_5_099C
        test   eax, eax
        je     SHORT loc_5_24D4
        call   sub_5_09DB
        jmp    SHORT loc_5_250C
loc_5_24D4:
        call   sub_5_0A32
        lea    ecx, [ebp - 4]
        call   sub_5_1596
loc_5_24E1:
        add    edi, 30h
        mov    esi, 7
loc_5_24E9:
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     SHORT loc_5_2506
loc_5_24F2:
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_246A
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 4], ecx
        test   ecx, ecx
        jne    SHORT loc_5_24F2
loc_5_2506:
        add    edi, 4
        dec    esi
        jne    SHORT loc_5_24E9
loc_5_250C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_2512:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   ecx, ecx
        jne    SHORT loc_5_2529
        mov    edx, dword ptr [esp + 8]
        db  0A1h, 000h, 000h, 000h, 000h
        mov    dword ptr [eax], edx
        jmp    SHORT loc_5_2549
loc_5_2529:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esp + 8]
        cmp    dword ptr [eax], edx
        jae    SHORT loc_5_253D
        mov    eax, 0eah
        jmp    SHORT loc_5_254B
loc_5_253D:
        push   edx
        mov    dword ptr [eax], edx
        mov    edx, dword ptr [esp + 8]
        call   sub_5_5B5D
loc_5_2549:
        xor    eax, eax
loc_5_254B:
        ret
loc_5_254C:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 4]
        mov    ecx, esi
        call   sub_5_C3C4
        inc    eax
        push   eax
        push   esi
        call   sub_5_2512
        add    esp, 8
        pop    esi
        ret
loc_5_2568:
        mov    eax, dword ptr [esp + 4]
        push   4
        add    eax, 0ch
        push   eax
        call   sub_5_2512
        add    esp, 8
        ret
loc_5_257B:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 4
        mov    ecx, dword ptr [eax + 8]
        push   4
        mov    dword ptr [esp + 4], ecx
        lea    ecx, [esp + 4]
        push   ecx
        call   sub_5_2512
        add    esp, 0ch
        ret
loc_5_2599:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 4
        lea    edx, [esp]
        push   esi
        push   edi
        mov    ecx, dword ptr [eax + 38h]
        call   sub_5_8BD1
        mov    edi, eax
        test   edi, edi
        jne    SHORT loc_5_25BB
        mov    eax, 0eh
        jmp    SHORT loc_5_25E0
loc_5_25BB:
        push   dword ptr [esp + 8]
        push   edi
        call   sub_5_2512
        add    esp, 8
        mov    esi, eax
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        jne    SHORT loc_5_25DE
        push   0
        push   edi
        call   sub_5_D0C8
        add    esp, 8
loc_5_25DE:
        mov    eax, esi
loc_5_25E0:
        pop    edi
        pop    esi
        add    esp, 4
        ret
loc_5_25E6:
        mov    eax, dword ptr [esp + 4]
        push   4
        add    eax, 24h
        push   eax
        call   sub_5_2512
        add    esp, 8
        ret
loc_5_25F9:
        mov    eax, dword ptr [esp + 4]
        push   4
        add    eax, 28h
        push   eax
        call   sub_5_2512
        add    esp, 8
        ret
loc_5_260C:
        mov    eax, dword ptr [esp + 4]
        push   4
        add    eax, 20h
        push   eax
        call   sub_5_2512
        add    esp, 8
        ret
loc_5_261F:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        mov    ecx, dword ptr [esp + 8]
        call   sub_5_1AC6
        test   eax, eax
        mov    eax, 3fah
        je     SHORT loc_5_2650
        mov    eax, dword ptr [ebp + 18h]
        mov    ecx, dword ptr [ebp + 14h]
        db  0A3h, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 0ch]
        push   dword ptr [ebp + 8]
        call   dword ptr [eax + 4]
loc_5_2650:
        mov    esp, ebp
        pop    ebp
        ret
sub_5_2654:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 14h
        push   esi
        mov    dword ptr [ebp - 10h], eax
        push   edi
        mov    esi, ecx
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], eax
        db  0C7h, 045h, 0ECh, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 4], 1
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 001h
        je     SHORT loc_5_26CE
        db  0BAh, 000h, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, edi
        call   sub_5_C486
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, edi
        call   sub_5_C44E
        push   esi
        mov    ecx, edi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_5_C3C4
        add    eax, edi
        push   eax
        call   sub_5_D0FC
        add    esp, 0ch
        lea    eax, [esi + 70h]
        lea    ecx, [ebp - 14h]
        push   eax
        push   7
        db  068h, 000h, 000h, 000h, 000h
        push   ecx
        push   esi
        push   edi
        call   sub_5_D144
        add    esp, 18h
loc_5_26CE:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_5_26D4:
        push   esi
        db  0A1h, 000h, 000h, 000h, 000h
        test   ah, 1
        mov    esi, ecx
        je     SHORT loc_5_2705
        test   al, 8
        jne    SHORT loc_5_2705
        mov    eax, dword ptr [esi + 70h]
        test   eax, eax
        je     SHORT loc_5_2705
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_D14C
        add    esp, 8
        push   dword ptr [esi + 70h]
        call   sub_5_D0E4
        add    esp, 4
loc_5_2705:
        pop    esi
        ret
sub_5_2707:
        push   ebx
        push   esi
        push   edi
        mov    esi, edx
        push   ebp
        mov    edi, ecx
        call   sub_5_09DB
        mov    edi, dword ptr [edi]
        test   edi, edi
        je     SHORT loc_5_273B
        xor    ebx, ebx
loc_5_271C:
        mov    ecx, dword ptr [edi + 8]
        mov    edx, dword ptr [edi + 0ch]
        test   edx, edx
        mov    dword ptr [ecx + 48h], ebx
        je     SHORT loc_5_2735
        mov    dword ptr [ecx + 48h], edx
        call   sub_5_0969
        test   eax, eax
        je     SHORT loc_5_2790
loc_5_2735:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_5_271C
loc_5_273B:
        xor    edx, edx
        xor    edi, edi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        add    ecx, 24h
        db  03Bh, 00Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_2774
loc_5_2750:
        mov    eax, dword ptr [ecx + 20h]
        cmp    dword ptr [eax], 0
        je     SHORT loc_5_2769
        test   byte ptr [ecx + 0bh], 80h
        jne    SHORT loc_5_2769
        mov    eax, dword ptr [ecx + 0ch]
        cmp    edx, eax
        ja     SHORT loc_5_2769
        mov    edi, ecx
        mov    edx, eax
loc_5_2769:
        add    ecx, 24h
        db  03Bh, 00Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_2750
loc_5_2774:
        test   edi, edi
        je     SHORT loc_5_2797
        mov    edx, esi
        mov    ecx, edi
        push   dword ptr [edi + 20h]
        call   sub_5_0B16
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_279B
        or     byte ptr [edi + 0bh], 80h
        jmp    SHORT loc_5_273B
loc_5_2790:
        mov    eax, 2
        jmp    SHORT loc_5_27C2
loc_5_2797:
        xor    eax, eax
        jmp    SHORT loc_5_27C2
loc_5_279B:
        mov    ebp, dword ptr [edi]
        mov    esi, dword ptr [edi + 1ch]
loc_5_27A0:
        mov    edx, dword ptr [edi + 1ch]
        mov    ecx, esi
        call   sub_5_1D59
        test   eax, eax
        je     SHORT loc_5_27B1
        inc    dword ptr [edi + 0ch]
loc_5_27B1:
        add    edi, 24h
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_27C0
        cmp    dword ptr [edi], ebp
        je     SHORT loc_5_27A0
loc_5_27C0:
        mov    eax, ebx
loc_5_27C2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_27C7:
        push   ebx
        push   esi
        mov    ebx, dword ptr [ecx + 8]
        push   edi
        push   ebp
        mov    esi, ecx
        lea    edi, [ebx + 38h]
        mov    ecx, edi
        call   sub_5_1596
        mov    eax, dword ptr [esi + 0ch]
        push   90000000h
        push   800h
        mov    ecx, ebx
        mov    dword ptr [edi], eax
        call   sub_5_1906
        mov    esi, dword ptr [esi + 0ch]
        mov    dword ptr [esi + 8], 2
        add    esi, 10h
        cmp    dword ptr [esi], 0
        je     SHORT loc_5_2848
loc_5_2802:
        mov    eax, dword ptr [esi]
        mov    ebx, dword ptr [eax + 8]
        test   ebx, ebx
        je     SHORT loc_5_283E
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 004h
        mov    ebp, dword ptr [eax + 10h]
        jne    SHORT loc_5_2822
        push   0
        push   eax
        call   sub_5_D0C8
        add    esp, 8
loc_5_2822:
        lea    ecx, [ebx + 1ch]
        mov    edx, dword ptr [edi]
        push   dword ptr [ebx + 14h]
        push   ecx
        mov    ecx, esi
        mov    dword ptr [esi], ebp
        push   dword ptr [ebx + 4]
        call   sub_5_1711
        test   eax, eax
        je     SHORT loc_5_2852
        mov    dword ptr [eax + 8], ebx
loc_5_283E:
        mov    esi, dword ptr [esi]
        add    esi, 10h
        cmp    dword ptr [esi], 0
        jne    SHORT loc_5_2802
loc_5_2848:
        mov    eax, 1
loc_5_284D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_2852:
        xor    eax, eax
        jmp    SHORT loc_5_284D
sub_5_2856:
        mov    eax, dword ptr [edx]
        test   eax, eax
        je     SHORT loc_5_2867
loc_5_285C:
        cmp    dword ptr [eax + 8], ecx
        je     SHORT loc_5_286A
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_5_285C
loc_5_2867:
        xor    eax, eax
loc_5_2869:
        ret
loc_5_286A:
        mov    eax, 1
        jmp    SHORT loc_5_2869
loc_5_2871:
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    ecx, esi
        push   200000h
        call   sub_5_1923
        mov    eax, 8000000h
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_28CE
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_28BC
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ecx, esi
        call   sub_5_2856
        test   eax, eax
        jne    SHORT loc_5_28BC
        mov    ecx, esi
        call   sub_5_0E91
        jmp    SHORT loc_5_28CE
loc_5_28BC:
        xor    edx, edx
        mov    ecx, esi
        call   sub_5_0FD2
        push   esi
        call   sub_5_122C
        add    esp, 4
loc_5_28CE:
        mov    eax, 1
        pop    edi
        pop    esi
        ret
sub_5_28D6:
        push   ebx
        db  0A1h, 000h, 000h, 000h, 000h
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [eax]
        test   esi, esi
        je     SHORT loc_5_2929
        xor    edi, edi
        mov    ebx, 800000h
loc_5_28EC:
        mov    ebp, dword ptr [esi + 8]
        push   ebx
        push   edi
        mov    ecx, ebp
        push   ebx
        mov    dword ptr [ebp + 48h], edi
        push   edi
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_2923
        mov    eax, dword ptr [ebp + 38h]
        test   eax, eax
        je     SHORT loc_5_2923
        mov    ecx, dword ptr [eax + 10h]
        test   ecx, ecx
        je     SHORT loc_5_2923
loc_5_290F:
        mov    eax, dword ptr [ecx + 8]
        test   eax, eax
        je     SHORT loc_5_291C
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], eax
loc_5_291C:
        mov    ecx, dword ptr [ecx + 10h]
        test   ecx, ecx
        jne    SHORT loc_5_290F
loc_5_2923:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_5_28EC
loc_5_2929:
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, 1
        call   sub_5_CC30
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_293D:
        push   esi
        call   sub_5_0A32
        call   sub_5_28D6
        db  0A1h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [eax]
        test   esi, esi
        je     SHORT loc_5_2960
loc_5_2953:
        mov    ecx, esi
        call   sub_5_27C7
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_5_2953
loc_5_2960:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_684A
        pop    esi
        ret
sub_5_296D:
        push   esi
        push   edi
        mov    esi, ecx
        test   esi, esi
        je     SHORT loc_5_2990
        mov    edi, dword ptr [esi]
        test   edi, edi
        je     SHORT loc_5_2989
loc_5_297B:
        lea    ecx, [edi + 0ch]
        call   sub_5_1596
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_5_297B
loc_5_2989:
        mov    ecx, esi
        call   sub_5_684A
loc_5_2990:
        pop    edi
        pop    esi
        ret
sub_5_2993:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        push   ebp
        je     SHORT loc_5_29B3
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_29B3
        mov    eax, 13h
        jmp    NEAR PTR loc_5_2A62
loc_5_29B3:
        xor    edx, edx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_2707
        test   eax, eax
        jne    NEAR PTR loc_5_2A62
        call   sub_5_675F
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_29DD
        mov    eax, 2
        jmp    NEAR PTR loc_5_2A62
loc_5_29DD:
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ebp, dword ptr [eax]
        test   ebp, ebp
        je     SHORT loc_5_2A19
loc_5_29E8:
        mov    edi, dword ptr [ebp + 0ch]
        test   edi, edi
        je     SHORT loc_5_2A12
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, ebx
        call   sub_5_6791
        test   eax, eax
        je     SHORT loc_5_2A67
        mov    esi, dword ptr [ebx]
        mov    edx, 0beefdeadh
        mov    ecx, edi
        call   sub_5_14E6
        mov    dword ptr [esi + 0ch], eax
        test   eax, eax
        je     SHORT loc_5_2A67
loc_5_2A12:
        mov    ebp, dword ptr [ebp]
        test   ebp, ebp
        jne    SHORT loc_5_29E8
loc_5_2A19:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_296D
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    edx, 1
        db  089h, 01Dh, 000h, 000h, 000h, 000h
        mov    esi, edx
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  03Bh, 005h, 000h, 000h, 000h, 000h
        lea    eax, [ecx - 1]
        adc    esi, -1
        cmp    eax, edx
        sbb    eax, eax
        neg    eax
        and    esi, eax
        cmp    ecx, edx
        sbb    eax, eax
        neg    eax
        or     esi, eax
        cmp    esi, edx
        sbb    eax, eax
        and    eax, 13h
loc_5_2A62:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_5_2A67:
        mov    ecx, ebx
        call   sub_5_296D
        mov    eax, 2
        jmp    SHORT loc_5_2A62
sub_5_2A75:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   ebp
        test   esi, esi
        jne    SHORT loc_5_2A8C
        call   sub_5_2993
        jmp    NEAR PTR loc_5_2B9F
loc_5_2A8C:
        mov    edi, dword ptr [esi + 8]
        xor    ebx, ebx
        push   ebx
        mov    eax, 200000h
        push   eax
        mov    ecx, edi
        push   ebx
        mov    dword ptr [esi + 0ch], ebx
        push   eax
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_2ACB
        mov    ecx, edi
        call   sub_5_05D3
        test   eax, eax
        je     SHORT loc_5_2ACB
        mov    ecx, dword ptr [esi]
        call   sub_5_2A75
        test   eax, eax
        je     NEAR PTR loc_5_2B9F
        cmp    eax, 2
        je     NEAR PTR loc_5_2B9F
loc_5_2ACB:
        mov    ebx, dword ptr [edi + 34h]
        mov    eax, dword ptr [ebx + 0ch]
        mov    dword ptr [esp + 14h], eax
loc_5_2AD5:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_C7F6
        test   eax, eax
        jne    NEAR PTR loc_5_2B86
        mov    ebp, dword ptr [ebx + 0ch]
        db  039h, 02Dh, 000h, 000h, 000h, 000h
        jb     NEAR PTR loc_5_2B8D
        db  0A1h, 000h, 000h, 000h, 000h
        sub    eax, dword ptr [esp + 14h]
        add    eax, ebp
        db  039h, 005h, 000h, 000h, 000h, 000h
        jbe    NEAR PTR loc_5_2B94
        push   100000h
        db  03Bh, 02Dh, 000h, 000h, 000h, 000h
        sbb    ecx, ecx
        push   0
        inc    ecx
        db  0A3h, 000h, 000h, 000h, 000h
        db  001h, 00Dh, 000h, 000h, 000h, 000h
        mov    dword ptr [esi + 0ch], ebx
        mov    ecx, edi
        call   sub_5_1906
        mov    ecx, dword ptr [esi]
        call   sub_5_2A75
        push   100000h
        mov    ecx, edi
        mov    dword ptr [esp + 14h], eax
        push   0
        call   sub_5_1923
        db  03Bh, 02Dh, 000h, 000h, 000h, 000h
        sbb    eax, eax
        inc    eax
        db  029h, 005h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 14h]
        sub    eax, ebp
        db  001h, 005h, 000h, 000h, 000h, 000h
        cmp    dword ptr [esp + 10h], 0
        je     SHORT loc_5_2B9B
        cmp    dword ptr [esp + 10h], 2
        je     SHORT loc_5_2B9B
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    NEAR PTR loc_5_2AD5
        mov    eax, 13h
        jmp    SHORT loc_5_2B9F
loc_5_2B86:
        mov    eax, 13h
        jmp    SHORT loc_5_2B9F
loc_5_2B8D:
        mov    eax, 13h
        jmp    SHORT loc_5_2B9F
loc_5_2B94:
        mov    eax, 13h
        jmp    SHORT loc_5_2B9F
loc_5_2B9B:
        mov    eax, dword ptr [esp + 10h]
loc_5_2B9F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
sub_5_2BA7:
        push   edi
        mov    eax, 800000h
        mov    edi, ecx
        push   eax
        push   0
        push   eax
        push   0
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_2BE0
        mov    eax, dword ptr [edi + 38h]
        test   eax, eax
        je     SHORT loc_5_2BE0
        mov    eax, dword ptr [eax + 10h]
        test   eax, eax
        je     SHORT loc_5_2BE0
loc_5_2BCC:
        cmp    dword ptr [eax + 8], 0
        je     SHORT loc_5_2BD9
        mov    dword ptr [eax + 4], 8000h
loc_5_2BD9:
        mov    eax, dword ptr [eax + 10h]
        test   eax, eax
        jne    SHORT loc_5_2BCC
loc_5_2BE0:
        pop    edi
        ret
sub_5_2BE2:
        push   esi
        push   edi
        mov    edi, dword ptr [ecx + 8]
        mov    esi, ecx
        db  039h, 03Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_2C03
        push   0
        mov    eax, 200000h
        push   eax
        mov    ecx, edi
        push   0
        push   eax
        call   sub_5_18E3
loc_5_2C03:
        mov    ecx, edi
        call   sub_5_1D31
        pop    edi
        mov    dword ptr [esi + 10h], eax
        pop    esi
        ret
loc_5_2C10:
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    ecx, esi
        push   2008h
        push   edi
        push   23408h
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_2C99
        mov    eax, dword ptr [esi + 34h]
        test   eax, eax
        je     SHORT loc_5_2C99
        cmp    dword ptr [eax + 0ch], 7fffh
        ja     SHORT loc_5_2C99
        cmp    dword ptr [esi + 38h], edi
        je     SHORT loc_5_2C99
        cmp    dword ptr [esi + 40h], edi
        jne    SHORT loc_5_2C99
        cmp    dword ptr [esi + 3ch], edi
        je     SHORT loc_5_2C62
        mov    eax, 800000h
        mov    ecx, esi
        push   eax
        push   edi
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_2C99
loc_5_2C62:
        mov    eax, 8000000h
        mov    ecx, esi
        push   eax
        push   0
        push   eax
        push   0
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_2C85
        xor    edx, edx
        mov    ecx, esi
        call   sub_5_0FA7
        test   eax, eax
        je     SHORT loc_5_2C99
loc_5_2C85:
        mov    ecx, esi
        call   sub_5_2BA7
        mov    eax, 1
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_5_2C9B
loc_5_2C99:
        xor    eax, eax
loc_5_2C9B:
        pop    edi
        pop    esi
        ret
sub_5_2C9E:
        test   ecx, ecx
        je     SHORT loc_5_2CBF
        mov    edx, ecx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_731F
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_296D
        call   sub_5_28D6
loc_5_2CBF:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_684A
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_296D
        call   sub_5_09DB
        db  080h, 025h, 001h, 000h, 000h, 000h, 0BFh
        db  0FFh, 00Dh, 000h, 000h, 000h, 000h
        ret
sub_5_2CE8:
        xor    eax, eax
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        db  089h, 015h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0A3h, 000h, 000h, 000h, 000h
        xor    edx, edx
        db  0CDh, 020h, 007h, 000h, 005h, 000h
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edx, 4c4b40h
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_5_C7CC
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [eax]
        call   sub_5_2A75
        test   eax, eax
        jne    SHORT loc_5_2D3F
        call   sub_5_293D
        xor    ecx, ecx
        jmp    SHORT loc_5_2D74
loc_5_2D3F:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 0FFh
        jae    SHORT loc_5_2D65
        mov    edx, 1
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_2707
        test   eax, eax
        jne    SHORT loc_5_2D65
        call   sub_5_293D
        xor    ecx, ecx
        jmp    SHORT loc_5_2D74
loc_5_2D65:
        cmp    eax, 2
        mov    eax, 0
        jne    SHORT loc_5_2D7E
        mov    ecx, 3
loc_5_2D74:
        call   sub_5_2C9E
        mov    eax, 1
loc_5_2D7E:
        ret
sub_5_2D7F:
        push   esi
        mov    edx, 20000h
        push   edi
        mov    esi, ecx
        push   0
        push   edx
        push   0
        push   edx
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_2DA3
        mov    ecx, esi
        call   sub_5_6238
        jmp    NEAR PTR loc_5_2F35
loc_5_2DA3:
        xor    edi, edi
        mov    eax, 800000h
        push   eax
        mov    ecx, esi
        push   edi
        db  089h, 03Dh, 000h, 000h, 000h, 000h
        push   eax
        push   edi
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_2DD6
        cmp    dword ptr [esi + 38h], edi
        je     SHORT loc_5_2DD6
        push   80000000h
        mov    ecx, esi
        push   edi
        call   sub_5_1906
        jmp    NEAR PTR loc_5_2F35
loc_5_2DD6:
        mov    eax, dword ptr [esi + 34h]
        test   eax, eax
        jne    SHORT loc_5_2DF0
        push   80000000h
        mov    ecx, esi
        push   0
        call   sub_5_1906
        jmp    NEAR PTR loc_5_2F35
loc_5_2DF0:
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 040h
        je     SHORT loc_5_2E03
        call   sub_5_7B5F
        jmp    NEAR PTR loc_5_2F35
loc_5_2E03:
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  080h, 00Dh, 001h, 000h, 000h, 000h, 040h
        cmp    dword ptr [eax + 0ch], 7fffh
        jbe    SHORT loc_5_2E29
        mov    ecx, 1bh
        jmp    NEAR PTR loc_5_2F30
loc_5_2E29:
        mov    edx, esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_6791
        test   eax, eax
        jne    SHORT loc_5_2E44
        mov    ecx, 3
        jmp    NEAR PTR loc_5_2F30
loc_5_2E44:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_5_2E4A:
        add    ecx, 24h
        db  03Bh, 00Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_2E6F
        test   byte ptr [ecx + 8], 4
        je     SHORT loc_5_2E62
        mov    eax, 40000000h
        jmp    SHORT loc_5_2E67
loc_5_2E62:
        mov    eax, 80000000h
loc_5_2E67:
        sub    eax, dword ptr [ecx + 10h]
        mov    dword ptr [ecx + 0ch], eax
        jmp    SHORT loc_5_2E4A
loc_5_2E6F:
        mov    ecx, esi
        call   sub_5_2BA7
        push   0
        mov    ecx, esi
        push   200000h
        call   sub_5_1906
        mov    edx, 3fffh
        xor    ecx, ecx
        call   sub_5_2CE8
        test   eax, eax
        jne    NEAR PTR loc_5_2F35
        db  0A1h, 000h, 000h, 000h, 000h
        mov    ecx, dword ptr [eax]
        call   sub_5_2BE2
        db  0BAh, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_5_6812
        test   eax, eax
        jne    SHORT loc_5_2EC9
        mov    ecx, 3
        jmp    SHORT loc_5_2F30
loc_5_2EC9:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_2F0C
        db  0A1h, 000h, 000h, 000h, 000h
        mov    esi, dword ptr [eax]
        test   esi, esi
        je     SHORT loc_5_2EEA
loc_5_2EDD:
        mov    ecx, esi
        call   sub_5_2BE2
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_5_2EDD
loc_5_2EEA:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        call   sub_5_692A
        mov    edx, 3fffh
        xor    ecx, ecx
        call   sub_5_2CE8
        test   eax, eax
        je     SHORT loc_5_2F0C
        mov    eax, 1
        jmp    SHORT loc_5_2F37
loc_5_2F0C:
        mov    edx, 7fffh
        mov    ecx, 4000h
        call   sub_5_2CE8
        test   eax, eax
        je     SHORT loc_5_2F2B
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        sbb    eax, eax
        inc    eax
        jmp    SHORT loc_5_2F37
loc_5_2F2B:
        mov    ecx, 0ch
loc_5_2F30:
        call   sub_5_2C9E
loc_5_2F35:
        xor    eax, eax
loc_5_2F37:
        pop    edi
        pop    esi
        ret
sub_5_2F3A:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0F6h, 005h, 001h, 000h, 000h, 000h, 040h
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        je     SHORT loc_5_2F5E
        call   sub_5_7B5F
        jmp    NEAR PTR loc_5_309A
loc_5_2F5E:
        mov    edx, esi
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0FFh, 005h, 000h, 000h, 000h, 000h
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  080h, 00Dh, 001h, 000h, 000h, 000h, 040h
        call   sub_5_6791
        test   eax, eax
        jne    SHORT loc_5_2F93
        mov    ecx, 3
        call   sub_5_2C9E
        xor    eax, eax
        jmp    NEAR PTR loc_5_309F
loc_5_2F93:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
loc_5_2F99:
        add    ecx, 24h
        db  039h, 00Dh, 000h, 000h, 000h, 000h
        je     SHORT loc_5_2FBE
        test   byte ptr [ecx + 8], 4
        je     SHORT loc_5_2FB1
        mov    eax, 40000000h
        jmp    SHORT loc_5_2FB6
loc_5_2FB1:
        mov    eax, 80000000h
loc_5_2FB6:
        sub    eax, dword ptr [ecx + 10h]
        mov    dword ptr [ecx + 0ch], eax
        jmp    SHORT loc_5_2F99
loc_5_2FBE:
        db  0BAh, 000h, 000h, 000h, 000h
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        call   sub_5_6812
        test   eax, eax
        jne    SHORT loc_5_2FED
        mov    ecx, 3
        call   sub_5_2C9E
        xor    eax, eax
        jmp    NEAR PTR loc_5_309F
loc_5_2FED:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_3004
        xor    ecx, ecx
        call   sub_5_2C9E
        xor    eax, eax
        jmp    NEAR PTR loc_5_309F
loc_5_3004:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     SHORT loc_5_302C
        db  08Bh, 015h, 000h, 000h, 000h, 000h
loc_5_3016:
        cmp    dword ptr [eax + 8], esi
        jne    SHORT loc_5_3020
        mov    dword ptr [eax + 0ch], edi
        jmp    SHORT loc_5_3026
loc_5_3020:
        mov    ebx, dword ptr [edx + 38h]
        mov    dword ptr [eax + 0ch], ebx
loc_5_3026:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_5_3016
loc_5_302C:
        mov    edx, 1
        call   sub_5_2707
        test   eax, eax
        je     SHORT loc_5_304A
        call   sub_5_28D6
        xor    ecx, ecx
        call   sub_5_2C9E
        xor    eax, eax
        jmp    SHORT loc_5_309F
loc_5_304A:
        db  0A1h, 000h, 000h, 000h, 000h
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        mov    edi, dword ptr [eax]
        test   edi, edi
        je     SHORT loc_5_3089
loc_5_305F:
        mov    esi, dword ptr [edi + 8]
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_3083
        push   90000000h
        mov    ecx, esi
        push   800h
        call   sub_5_1923
        lea    ecx, [esi + 38h]
        call   sub_5_15ED
loc_5_3083:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_5_305F
loc_5_3089:
        call   sub_5_0A32
        call   sub_5_28D6
        xor    ecx, ecx
        call   sub_5_2C9E
loc_5_309A:
        mov    eax, 1
loc_5_309F:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_5_30A3:
        movzx  eax, cx
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   edx
        call   sub_5_D0FC
        add    esp, 0ch
        ret
sub_5_30B6:
        push   ebp
        mov    ebp, esp
        sub    esp, 9ch
        mov    dword ptr [ebp - 4], 4fh
        push   ebx
        push   esi
        push   edi
        db  0BEh, 000h, 000h, 000h, 000h
        mov    ebx, dword ptr [ebp + 0ch]
        mov    edi, ebx
        movsb
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_5_3175
        db  0BEh, 000h, 000h, 000h, 000h
        lea    edi, [ebp - 9ch]
        movsd
        movsd
        movsd
        movsb
        lea    edx, [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        lea    edi, [ebp - 0ch]
        call   sub_5_30A3
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    ecx, 0ffffffffh
        lea    edi, [ebp - 9ch]
        sub    eax, eax
        repne scasb
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 5ch]
        push   eax
        lea    edx, [ebp - 9ch]
        push   ecx
        push   0
        push   0
        push   edx
        push   dword ptr [ebp + 10h]
        call   sub_5_D11C
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_5_3175
        xor    edi, edi
        lea    esi, [ebx + 1]
        cmp    byte ptr [ebp - 5ch], 0
        mov    al, byte ptr [ebp - 5ch]
        mov    byte ptr [ebx], al
        lea    eax, [ebp - 5bh]
        je     SHORT loc_5_3171
loc_5_315F:
        cmp    edi, 50h
        jge    SHORT loc_5_3171
        inc    edi
        mov    ecx, esi
        inc    esi
        mov    dl, byte ptr [eax]
        inc    eax
        mov    byte ptr [ecx], dl
        test   dl, dl
        jne    SHORT loc_5_315F
loc_5_3171:
        mov    byte ptr [ebx + 4fh], 0
loc_5_3175:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_317C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        xor    edi, edi
        db  081h, 0C2h, 000h, 000h, 000h, 000h
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   edi
        mov    ecx, esi
        mov    dword ptr [ebp - 4], edi
        call   sub_5_8613
        test   eax, eax
        je     SHORT loc_5_31EC
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   80000002h
        call   sub_5_D154
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_31EC
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 0ch], 4
        push   eax
        push   ecx
        push   edi
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_5_D11C
        add    esp, 18h
        push   dword ptr [ebp - 8]
        call   sub_5_D0E4
        add    esp, 4
loc_5_31EC:
        test   byte ptr [ebp - 4], 2
        je     SHORT loc_5_31FD
        push   esi
        call   sub_5_9014
        add    esp, 4
        jmp    SHORT loc_5_325E
loc_5_31FD:
        mov    edi, 40000000h
        xor    ebx, ebx
        push   edi
        mov    ecx, esi
        push   ebx
        push   edi
        push   ebx
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_322E
        test   byte ptr [ebp - 4], 1
        jne    SHORT loc_5_322E
        push   ebx
        push   esi
        call   _CONFIGMG_Enable_DevNode
        add    esp, 8
        mov    ecx, esi
        push   edi
        push   ebx
        call   sub_5_1923
        jmp    SHORT loc_5_325E
loc_5_322E:
        mov    edi, 40000000h
        xor    ebx, ebx
        push   edi
        mov    ecx, esi
        push   ebx
        push   edi
        push   ebx
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_325E
        test   byte ptr [ebp - 4], 1
        je     SHORT loc_5_325E
        push   1
        push   esi
        call   _CONFIGMG_Disable_DevNode
        add    esp, 8
        mov    ecx, esi
        push   edi
        push   ebx
        call   sub_5_1906
loc_5_325E:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_326A:
        push   ebp
        mov    ebp, esp
        sub    esp, 118h
        push   ebx
        lea    edx, [ebp - 0ch]
        push   esi
        xor    ebx, ebx
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], 5
        call   sub_5_30A3
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   80000002h
        call   sub_5_D154
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_32E2
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 18h]
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_5_32E2
        push   5
        lea    eax, [ebp - 0ch]
        push   eax
        push   1
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_D0EC
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_5_32E7
loc_5_32E2:
        mov    ebx, 1dh
loc_5_32E7:
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 118h]
        call   sub_5_C486
        lea    edx, [ebp - 0ch]
        lea    ecx, [ebp - 118h]
        call   sub_5_C44E
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 118h]
        push   eax
        push   ecx
        push   80000002h
        call   sub_5_D164
        add    esp, 0ch
        push   80000005h
        lea    edi, [ebp - 0ch]
        push   dword ptr [ebp - 4]
        call   sub_5_D15C
        add    esp, 8
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        mov    edx, 0ffffffffh
        mov    ecx, edx
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        db  0BFh, 007h, 000h, 000h, 000h
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        db  0BEh, 000h, 000h, 000h, 000h
        db  0BFh, 000h, 000h, 000h, 000h
        mov    ecx, edx
        sub    eax, eax
        repne scasb
        add    edi, edx
        mov    ecx, edx
        movsw
        db  0BFh, 000h, 000h, 000h, 000h
        sub    eax, eax
        repne scasb
        not    ecx
        add    ecx, edx
        db  0BAh, 000h, 000h, 000h, 000h
        db  089h, 00Dh, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        call   sub_5_CC30
        xor    eax, eax
        lea    ecx, [ebp - 18h]
        mov    esi, 4
loc_5_33A9:
        shl    eax, 4
        mov    dl, byte ptr [ecx]
        movsx  edi, dl
        cmp    dl, 39h
        mov    edx, 37h
        jg     SHORT loc_5_33C0
        mov    edx, 30h
loc_5_33C0:
        sub    eax, edx
        inc    ecx
        add    eax, edi
        dec    esi
        jne    SHORT loc_5_33A9
        xor    esi, esi
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_341D
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_33D8:
        add    esp, 4
        db  039h, 035h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_33FA
        push   esi
        db  0B9h, 000h, 000h, 000h, 000h
        push   esi
        mov    edx, 1
        db  089h, 035h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_33FA:
        xor    esi, esi
        db  0B9h, 000h, 000h, 000h, 000h
        push   esi
        mov    edx, 18h
        push   esi
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_340D:
        push   esi
        db  0B9h, 000h, 000h, 000h, 000h
        push   esi
        xor    edx, edx
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_341B:
        jmp    SHORT loc_5_3442
loc_5_341D:
        xor    eax, eax
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_3436
        push   1
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_5_23DF
        add    esp, 4
loc_5_3436:
        push   0
        push   18h
        call   sub_5_5FFE
        add    esp, 8
loc_5_3442:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_344B:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        mov    dword ptr [ebp - 8], ecx
        push   esi
        push   edi
        mov    esi, edx
        xor    edi, edi
        cmp    dword ptr [ebp + 8], edi
        jne    NEAR PTR loc_5_34F3
        mov    ecx, esi
        call   sub_5_1BDC
        test   eax, eax
        jne    NEAR PTR loc_5_35B6
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], edi
        mov    dword ptr [ebp - 0ch], 4
        push   ecx
        push   edi
        push   edi
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_5_D11C
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_5_34B9
        cmp    eax, 2
        je     NEAR PTR loc_5_35B6
        cmp    eax, 3f4h
        je     NEAR PTR loc_5_35B6
        mov    edi, 1dh
        jmp    NEAR PTR loc_5_35B6
loc_5_34B9:
        test   byte ptr [ebp - 3], 10h
        jne    NEAR PTR loc_5_35B6
        push   4
        lea    eax, [ebp - 4]
        or     byte ptr [ebp - 3], 4
        push   eax
        push   3
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_5_D0EC
        add    esp, 18h
        test   eax, eax
        je     NEAR PTR loc_5_35B6
        mov    edi, 1dh
        jmp    NEAR PTR loc_5_35B6
loc_5_34F3:
        cmp    byte ptr [esi], 0
        je     SHORT loc_5_3504
        db  0BAh, 000h, 000h, 000h, 000h
        mov    ecx, esi
        call   sub_5_C44E
loc_5_3504:
        mov    ecx, esi
        call   sub_5_C3C4
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 14h], eax
loc_5_3515:
        test   edi, edi
        jne    NEAR PTR loc_5_35B6
        mov    eax, dword ptr [ebp - 14h]
        push   100h
        add    eax, esi
        push   eax
        mov    dword ptr [ebp - 4], eax
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        call   sub_5_D16C
        add    esp, 10h
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_359E
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_C430
        cmp    dword ptr [ebp + 8], 3
        jne    SHORT loc_5_3568
        push   0
        db  0A1h, 000h, 000h, 000h, 000h
        push   0
        push   0
        push   0
        push   esi
        push   eax
        call   sub_5_D11C
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_5_35AB
loc_5_3568:
        lea    eax, [ebp - 10h]
        push   eax
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 8]
        call   sub_5_D154
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_35A6
        mov    eax, dword ptr [ebp + 8]
        mov    edx, esi
        dec    eax
        mov    ecx, dword ptr [ebp - 10h]
        push   eax
        call   sub_5_344B
        push   dword ptr [ebp - 10h]
        mov    edi, eax
        call   sub_5_D0E4
        add    esp, 4
        jmp    SHORT loc_5_35AB
loc_5_359E:
        cmp    ebx, 103h
        je     SHORT loc_5_35AB
loc_5_35A6:
        mov    edi, 1dh
loc_5_35AB:
        inc    dword ptr [ebp - 0ch]
        test   ebx, ebx
        je     NEAR PTR loc_5_3515
loc_5_35B6:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_35C1:
        push   ebp
        mov    ebp, esp
        sub    esp, 204h
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        db  068h, 000h, 000h, 000h, 000h
        mov    edi, 80000002h
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_5_D154
        add    esp, 0ch
        mov    ebx, eax
        test   eax, eax
        jne    SHORT loc_5_3637
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   edi
        call   sub_5_D154
        add    esp, 0ch
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_5_3629
        push   3
        lea    edx, [ebp - 204h]
        mov    byte ptr [ebp - 204h], 0
        mov    ecx, dword ptr [ebp - 4]
        call   sub_5_344B
        push   dword ptr [ebp - 4]
        mov    esi, eax
        call   sub_5_D0E4
        add    esp, 4
loc_5_3629:
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_D0E4
        add    esp, 4
loc_5_3637:
        test   ebx, ebx
        je     SHORT loc_5_3640
        mov    esi, 1dh
loc_5_3640:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_3649:
        push   ebp
        mov    ebp, esp
        sub    esp, 158h
        push   ebx
        mov    dword ptr [ebp - 4], edx
        push   esi
        lea    eax, [ebp - 8]
        push   edi
        mov    esi, ecx
        xor    edi, edi
        push   eax
        push   8
        db  0BAh, 000h, 000h, 000h, 000h
        push   edi
        db  0B9h, 000h, 000h, 000h, 000h
        mov    dword ptr [ebp - 8], 0fh
        call   sub_5_1240
        cmp    eax, 21h
        jb     SHORT loc_5_36A6
        cmp    eax, 29h
        ja     SHORT loc_5_36A6
        lea    edi, [eax - 20h]
        xor    ecx, ecx
        mov    eax, dword ptr [esi]
        cmp    eax, ecx
        jle    SHORT loc_5_3703
        add    esi, 4
loc_5_3691:
        movzx  edx, word ptr [esi]
        cmp    edx, edi
        je     SHORT loc_5_36A2
        add    esi, 2
        inc    ecx
        cmp    ecx, eax
        jl     SHORT loc_5_3691
        jmp    SHORT loc_5_3703
loc_5_36A2:
        mov    edi, ecx
        jmp    SHORT loc_5_3703
loc_5_36A6:
        push   dword ptr [ebp + 8]
        mov    dx, word ptr [esi + edi*2 + 4]
        lea    ecx, [ebp - 58h]
        push   ecx
        lea    ebx, [ebp - 158h]
        push   edx
        call   sub_5_30B6
        add    esp, 0ch
        lea    ecx, [ebp - 58h]
        push   ecx
        push   1
        call   sub_5_6710
        add    esp, 4
        lea    ecx, [ebp - 158h]
        push   eax
        push   ecx
        call   sub_5_D0FC
        add    esp, 0ch
        push   1
        push   2
        call   sub_5_6710
        add    esp, 4
        mov    edx, ebx
        mov    ecx, eax
        call   sub_5_5ECA
        test   eax, eax
        jne    SHORT loc_5_3703
        inc    edi
        cmp    dword ptr [esi], edi
        jg     SHORT loc_5_36FF
        xor    edi, edi
loc_5_36FF:
        test   eax, eax
        je     SHORT loc_5_36A6
loc_5_3703:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_5_370E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0a0h
        push   esi
        lea    eax, [ebp - 0a0h]
        push   edi
        mov    esi, edx
        mov    di, 1
        mov    edx, ecx
        mov    ecx, eax
        call   sub_5_C486
        lea    ecx, [ebp - 0a0h]
        call   sub_5_C430
loc_5_3739:
        push   esi
        lea    eax, [ebp - 50h]
        push   eax
        push   edi
        call   sub_5_30B6
        add    esp, 0ch
        lea    ecx, [ebp - 50h]
        call   sub_5_C430
        lea    eax, [ebp - 0a0h]
        lea    edx, [ebp - 50h]
loc_5_3758:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    SHORT loc_5_3778
        or     cl, cl
        je     SHORT loc_5_3774
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    SHORT loc_5_3778
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    SHORT loc_5_3758
loc_5_3774:
        xor    eax, eax
        jmp    SHORT loc_5_377D
loc_5_3778:
        sbb    eax, eax
        sbb    eax, -1
loc_5_377D:
        test   eax, eax
        je     SHORT loc_5_3791
        inc    di
        cmp    di, 9
        jb     SHORT loc_5_3739
        xor    eax, eax
loc_5_378B:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_5_3791:
        mov    eax, 1
        jmp    SHORT loc_5_378B
sub_5_3798:
        sub    esp, 54h
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        push   ebp
        xor    ebp, ebp
        test   edx, edx
        jne    NEAR PTR loc_5_3843
        push   3
        call   sub_5_6710
        add    esp, 4
        mov    edi, eax
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
        mov    edi, ebx
        mov    ecx, 0ffffffffh
        sub    eax, eax
        mov    edx, dword ptr [esp + 68h]
        repne scasb
        not    ecx
        dec    ecx
        mov    dword ptr [esp + 10h], ecx
        mov    ecx, ebx
        call   sub_5_370E
        test   eax, eax
        je     SHORT loc_5_386A
loc_5_37F7:
        inc    ebp
        lea    eax, [esp + 14h]
        push   ebp
        db  068h, 000h, 000h, 000h, 000h
        push   eax
        call   sub_5_D0FC
        lea    edi, [esp + 20h]
        add    esp, 0ch
        mov    ecx, 0ffffffffh
        sub    eax, eax
        repne scasb
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        shr    ecx, 2
        mov    esi, edi
        mov    edi, dword ptr [esp + 10h]
        add    edi, ebx
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        mov    edx, dword ptr [esp + 68h]
        mov    ecx, ebx
        call   sub_5_370E
        test   eax, eax
        jne    SHORT loc_5_37F7
        jmp    SHORT loc_5_386A
loc_5_3843:
        push   4
        call   sub_5_6710
        add    esp, 4
        mov    esi, eax
loc_5_384F:
        inc    ebp
        push   ebp
        push   esi
        push   ebx
        call   sub_5_D0FC
        mov    edx, dword ptr [esp + 74h]
        add    esp, 0ch
        mov    ecx, ebx
        call   sub_5_370E
        test   eax, eax
        jne    SHORT loc_5_384F
loc_5_386A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 54h
        ret    4
sub_5_3874:
        sub    esp, 4
        test   ecx, ecx
        je     SHORT loc_5_38B6
        cmp    ecx, 8000h
        je     SHORT loc_5_38CA
        cmp    ecx, 8001h
        je     SHORT loc_5_38D6
loc_5_388B:
        cmp    ecx, 1
        mov    dword ptr [esp], ecx
        sbb    eax, eax
        lea    ecx, [esp]
        inc    eax
        push   ecx
        push   4
        db  0BAh, 000h, 000h, 000h, 000h
        push   0
        db  0B9h, 000h, 000h, 000h, 000h
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_5_1240
loc_5_38B2:
        add    esp, 4
        ret
loc_5_38B6:
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 001h
        je     SHORT loc_5_38B2
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_38B2
        jmp    SHORT loc_5_388B
loc_5_38CA:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    SHORT loc_5_38B2
loc_5_38D6:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_5_38B2
loc_5_38E2:
        push   ebp
        mov    ebp, esp
        sub    esp, 12ch
        mov    dword ptr [ebp - 0ch], 6eh
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        cmp    dword ptr [ebp + 10h], esi
        jne    SHORT loc_5_3907
        mov    dword ptr [ebp + 0ch], 1
        xor    edi, edi
        jmp    SHORT loc_5_3913
loc_5_3907:
        mov    edi, dword ptr [ebp + 8]
        test   edi, edi
        jne    SHORT loc_5_3913
        mov    edi, 0fffffff0h
loc_5_3913:
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   80000002h
        call   sub_5_D154
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_5_3C02
        lea    eax, [ebp - 0ch]
        lea    ecx, [ebp - 9ch]
        push   eax
        xor    ebx, ebx
        push   ecx
        push   ebx
        push   ebx
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_5_D11C
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_5_3BF7
        cmp    dword ptr [ebp - 0ch], 6eh
        jne    NEAR PTR loc_5_3BF7
        mov    ecx, 8000h
        call   sub_5_3874
        mov    ecx, 3
        call   sub_5_3874
        cmp    word ptr [ebp - 9ch], bx
        mov    dword ptr [ebp - 2ch], ebx
        jbe    SHORT loc_5_39C6
loc_5_3982:
        movzx  eax, bx
        imul   eax, eax, 0ch
        cmp    dword ptr [ebp + eax - 9ah], edi
        jne    SHORT loc_5_39BB
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp + eax - 96h], ecx
        jne    SHORT loc_5_39BB
        mov    ax, word ptr [ebp + eax - 90h]
        mov    ecx, dword ptr [ebp - 2ch]
        mov    word ptr [ebp + ecx*2 - 28h], ax
        mov    eax, 1
        mov    cl, bl
        inc    dword ptr [ebp - 2ch]
        shl    eax, cl
        or     esi, eax
loc_5_39BB:
        inc    bx
        cmp    bx, word ptr [ebp - 9ch]
        jb     SHORT loc_5_3982
loc_5_39C6:
        mov    eax, dword ptr [ebp - 2ch]
        test   eax, eax
        je     SHORT loc_5_3A18
        cmp    eax, 1
        je     NEAR PTR loc_5_3B94
        lea    ecx, [ebp - 2ch]
        mov    edx, esi
        push   dword ptr [ebp - 8]
        call   sub_5_3649
        movzx  esi, word ptr [ebp + eax*2 - 28h]
        lea    edx, [ebp - 4]
        mov    ecx, esi
        call   sub_5_44B6
        test   byte ptr [ebp - 4], 4
        je     SHORT loc_5_3A06
        and    byte ptr [ebp - 4], 0fbh
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, esi
        call   sub_5_4572
loc_5_3A06:
        test   dword ptr [ebp - 4], 80000003h
        je     NEAR PTR loc_5_3BC6
        jmp    NEAR PTR loc_5_3BBF
loc_5_3A18:
        mov    si, 1
loc_5_3A1C:
        xor    ax, ax
        cmp    word ptr [ebp - 9ch], ax
        jbe    SHORT loc_5_3A4A
loc_5_3A28:
        movzx  ecx, ax
        imul   ecx, ecx, 0ch
        cmp    word ptr [ebp + ecx - 90h], si
        je     SHORT loc_5_3A43
        inc    ax
        cmp    word ptr [ebp - 9ch], ax
        ja     SHORT loc_5_3A28
loc_5_3A43:
        cmp    word ptr [ebp - 9ch], ax
loc_5_3A4A:
        je     SHORT loc_5_3A54
        inc    si
        cmp    si, 9
        jb     SHORT loc_5_3A1C
loc_5_3A54:
        cmp    word ptr [ebp - 9ch], 9
        jb     SHORT loc_5_3A6D
        mov    word ptr [ebp - 9ch], 9
        mov    ax, 8
        jmp    SHORT loc_5_3A7B
loc_5_3A6D:
        mov    ax, word ptr [ebp - 9ch]
        inc    word ptr [ebp - 9ch]
loc_5_3A7B:
        movzx  eax, ax
        imul   eax, eax, 0ch
        mov    ecx, dword ptr [ebp + 0ch]
        push   6eh
        mov    dword ptr [ebp + eax - 9ah], edi
        mov    dword ptr [ebp + eax - 96h], ecx
        xor    ecx, ecx
        mov    word ptr [ebp + eax - 92h], cx
        mov    word ptr [ebp + eax - 90h], si
        lea    eax, [ebp - 9ch]
        push   eax
        push   3
        push   ecx
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_5_D0EC
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_5_3AE5
        push   dword ptr [ebp - 8]
        call   sub_5_D0E4
        add    esp, 4
        mov    ecx, 8001h
        call   sub_5_3874
        xor    ecx, ecx
        call   sub_5_3874
        jmp    NEAR PTR loc_5_3C02
loc_5_3AE5:
        mov    ax, si
        xor    di, di
loc_5_3AEB:
        mov    cx, ax
        movzx  ebx, di
        and    cx, 0fh
        cmp    cx, 0ah
        sbb    dl, dl
        shr    ax, 4
        and    dl, 0f9h
        inc    di
        add    dl, 37h
        add    dl, cl
        lea    ecx, [ebp - 11h]
        sub    ecx, ebx
        cmp    di, 4
        mov    byte ptr [ecx], dl
        jb     SHORT loc_5_3AEB
        xor    ebx, ebx
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 0dch]
        mov    byte ptr [ebp - 10h], bl
        call   sub_5_C486
        lea    edx, [ebp - 14h]
        lea    ecx, [ebp - 0dch]
        call   sub_5_C44E
        push   dword ptr [ebp - 8]
        cmp    dword ptr [ebp + 10h], 1
        sbb    edx, edx
        lea    ecx, [ebp - 12ch]
        inc    edx
        call   sub_5_3798
        lea    ecx, [ebp - 12ch]
        call   sub_5_C3C4
        push   eax
        lea    ecx, [ebp - 12ch]
        push   ecx
        lea    edx, [ebp - 0dch]
        movzx  esi, si
        push   1
        push   ebx
        push   edx
        push   dword ptr [ebp - 8]
        call   sub_5_D0EC
        add    esp, 18h
        mov    edx, 5
        mov    ecx, esi
        call   sub_5_4572
        db  080h, 00Dh, 000h, 000h, 000h, 000h, 020h
        call   sub_5_7B5F
        push   ebx
        jmp    SHORT loc_5_3BC8
loc_5_3B94:
        movzx  esi, word ptr [ebp - 28h]
        lea    edx, [ebp - 4]
        mov    ecx, esi
        call   sub_5_44B6
        test   byte ptr [ebp - 4], 4
        je     SHORT loc_5_3BB6
        and    byte ptr [ebp - 4], 0fbh
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, esi
        call   sub_5_4572
loc_5_3BB6:
        test   dword ptr [ebp - 4], 80000003h
        je     SHORT loc_5_3BC6
loc_5_3BBF:
        db  080h, 00Dh, 000h, 000h, 000h, 000h, 020h
loc_5_3BC6:
        push   0
loc_5_3BC8:
        push   esi
        call   _CONFIGMG_Set_HW_Prof
        add    esp, 8
        mov    esi, eax
        push   dword ptr [ebp - 8]
        call   sub_5_D0E4
        add    esp, 4
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3BE9:
        mov    ecx, 8001h
        call   sub_5_3874
        mov    eax, esi
        jmp    SHORT loc_5_3C07
loc_5_3BF7:
        push   dword ptr [ebp - 8]
        call   sub_5_D0E4
        add    esp, 4
loc_5_3C02:
        mov    eax, 1dh
loc_5_3C07:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_3C0E:
        sub    esp, 100h
        test   ecx, ecx
        push   esi
        je     SHORT loc_5_3C1D
        push   0fh
        jmp    SHORT loc_5_3C1F
loc_5_3C1D:
        push   11h
loc_5_3C1F:
        call   sub_5_6710
        lea    ecx, [esp + 8]
        add    esp, 4
        push   eax
        push   ecx
        call   sub_5_D0FC
        lea    esi, [esp + 0ch]
        add    esp, 8
        push   0
        push   10h
        call   sub_5_6710
        add    esp, 4
        mov    edx, esi
        mov    ecx, eax
        call   sub_5_5EA8
        pop    esi
        add    esp, 100h
        ret
loc_5_3C56:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        cmp    byte ptr [eax], 0
        je     NEAR PTR loc_5_3D92
        lea    eax, [ebp - 4]
        xor    edx, edx
        push   eax
        mov    ecx, esi
        call   sub_5_8D6E
        test   eax, eax
        je     SHORT loc_5_3C8F
        test   byte ptr [ebp - 3], 4
        je     SHORT loc_5_3C8F
        mov    edi, 2
        jmp    NEAR PTR loc_5_3D1F
loc_5_3C8F:
        db  0BAh, 000h, 000h, 000h, 000h
        db  0B9h, 000h, 000h, 000h, 000h
        xor    edi, edi
        call   sub_5_C486
        db  0B9h, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [esi + 4]
        xor    ebx, ebx
        call   sub_5_C44E
        db  039h, 01Dh, 024h, 000h, 000h, 000h
        jbe    SHORT loc_5_3D1F
        db  0C7h, 045h, 0FCh, 000h, 000h, 000h, 000h
loc_5_3CBE:
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ecx]
        call   sub_5_D154
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_3D12
        lea    eax, [ebp - 14h]
        lea    ecx, [ebp - 0ch]
        mov    dword ptr [ebp - 14h], 4
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 8]
        call   sub_5_D11C
        add    esp, 18h
        mov    dword ptr [ebp - 10h], eax
        push   dword ptr [ebp - 8]
        call   sub_5_D0E4
        add    esp, 4
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_5_3D12
        or     edi, dword ptr [ebp - 0ch]
loc_5_3D12:
        add    dword ptr [ebp - 4], 4
        inc    ebx
        db  039h, 01Dh, 024h, 000h, 000h, 000h
        ja     SHORT loc_5_3CBE
loc_5_3D1F:
        test   edi, 2
        je     SHORT loc_5_3D4A
        push   esi
        call   sub_5_11DE
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_5_3D3F
        mov    ecx, esi
        call   sub_5_3C0E
        xor    eax, eax
        jmp    SHORT loc_5_3D97
loc_5_3D3F:
        push   esi
        call   sub_5_9014
        add    esp, 4
        jmp    SHORT loc_5_3D92
loc_5_3D4A:
        test   edi, 1
        je     SHORT loc_5_3D92
        mov    eax, 40000000h
        mov    ecx, esi
        push   eax
        push   0
        push   eax
        push   0
        call   sub_5_18E3
        test   eax, eax
        jne    SHORT loc_5_3D92
        push   0
        push   esi
        call   _CONFIGMG_Disable_DevNode
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_5_3D87
        push   40000000h
        mov    ecx, esi
        push   0
        call   sub_5_1906
        jmp    SHORT loc_5_3D92
loc_5_3D87:
        mov    ecx, esi
        call   sub_5_3C0E
        xor    eax, eax
        jmp    SHORT loc_5_3D97
loc_5_3D92:
        mov    eax, 1
loc_5_3D97:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_5_3D9E:
        sub    esp, 4
        mov    eax, 40000000h
        push   esi
        push   eax
        mov    esi, dword ptr [esp + 10h]
        push   0
        push   eax
        mov    ecx, esi
        push   0
        call   sub_5_18E3
        test   eax, eax
        je     SHORT loc_5_3DF8
        lea    ecx, [esp + 4]
        push   0
        push   ecx
        push   0
        push   dword ptr [esi + 4]
        call   _CONFIGMG_Get_HW_Prof_Flags
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_5_3DF8
        test   byte ptr [esp + 4], 1
        jne    SHORT loc_5_3DF8
        push   0
        push   esi
        call   _CONFIGMG_Enable_DevNode
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_5_3DF8
        push   40000000h
        mov    ecx, esi
        push   0
        call   sub_5_1923
loc_5_3DF8:
        mov    eax, 1
        pop    esi
        add    esp, 4
        ret
loc_5_3E02:
        push   ebp
        mov    ebp, esp
        sub    esp, 4d8h
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    dword ptr [ebp - 4], ebx
        mov    byte ptr [ebp - 4d8h], bl
        cmp    dword ptr [ebp + 10h], ebx
        je     SHORT loc_5_3E38
        push   ebx
        call   sub_5_23DF
        add    esp, 4
        xor    eax, eax
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        jmp    NEAR PTR loc_5_409A
loc_5_3E38:
        push   1
        push   17h
        call   sub_5_5FFE
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_5_3E4D
        mov    ebx, 13h
loc_5_3E4D:
        test   ebx, ebx
        jne    NEAR PTR loc_5_4072
        lea    eax, [ebp - 4]
        mov    esi, 80000002h
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_5_D154
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_3EB0
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 0d8h]
        mov    dword ptr [ebp - 10h], 6eh
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_5_3EB0
        lea    eax, [ebp - 8]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_5_D154
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_3EB5
loc_5_3EB0:
        mov    ebx, 1dh
loc_5_3EB5:
        test   ebx, ebx
        jne    NEAR PTR loc_5_4072
        db  0C7h, 005h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        xor    esi, esi
        cmp    word ptr [ebp - 0d8h], si
        jbe    NEAR PTR loc_5_3F8D
        lea    edi, [ebp - 0cch]
loc_5_3EDC:
        mov    cx, word ptr [edi]
        lea    edx, [ebp - 18h]
        call   sub_5_30A3
        db  0A1h, 024h, 000h, 000h, 000h
        lea    ecx, [ebp - 18h]
        db  08Dh, 004h, 085h, 000h, 000h, 000h, 000h
        push   eax
        push   ecx
        push   dword ptr [ebp - 8]
        call   sub_5_D154
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_3F7A
        lea    edx, [ebp - 0ch]
        db  0FFh, 005h, 024h, 000h, 000h, 000h
        movzx  ecx, word ptr [edi]
        call   sub_5_44B6
        test   eax, eax
        jne    SHORT loc_5_3F7A
        test   dword ptr [ebp - 0ch], 80000003h
        je     SHORT loc_5_3F7A
        cmp    byte ptr [ebp - 4d8h], 0
        jne    SHORT loc_5_3F45
        push   13h
        call   sub_5_6710
        add    esp, 4
        lea    ecx, [ebp - 4d8h]
        mov    edx, eax
        call   sub_5_C486
loc_5_3F45:
        push   dword ptr [ebp - 4]
        mov    dx, word ptr [edi]
        lea    ecx, [ebp - 68h]
        mov    byte ptr [ebp - 68h], 0
        push   ecx
        push   edx
        call   sub_5_30B6
        add    esp, 0ch
        lea    edx, [ebp - 68h]
        lea    ecx, [ebp - 4d8h]
        call   sub_5_C44E
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 4d8h]
        call   sub_5_C44E
loc_5_3F7A:
        add    edi, 0ch
        inc    esi
        movzx  eax, word ptr [ebp - 0d8h]
        cmp    eax, esi
        ja     NEAR PTR loc_5_3EDC
loc_5_3F8D:
        push   dword ptr [ebp - 8]
        call   sub_5_D0E4
        add    esp, 4
        cmp    byte ptr [ebp - 4d8h], 0
        je     SHORT loc_5_3FB8
        push   14h
        call   sub_5_6710
        add    esp, 4
        lea    ecx, [ebp - 4d8h]
        mov    edx, eax
        call   sub_5_C44E
loc_5_3FB8:
        db  083h, 03Dh, 024h, 000h, 000h, 000h, 000h
        jb     NEAR PTR loc_5_4072
        db  083h, 03Dh, 024h, 000h, 000h, 000h, 001h
        jne    SHORT loc_5_3FF3
        push   0
        push   6
        call   sub_5_6710
        add    esp, 4
        mov    esi, eax
        push   7
        call   sub_5_6710
        add    esp, 4
        mov    edx, esi
        mov    ecx, eax
        call   sub_5_5ECA
        test   eax, eax
        je     SHORT loc_5_4048
loc_5_3FF3:
        cmp    byte ptr [ebp - 4d8h], 0
        je     SHORT loc_5_401B
        push   0
        lea    esi, [ebp - 4d8h]
        push   12h
        call   sub_5_6710
        add    esp, 4
        mov    edx, esi
        mov    ecx, eax
        call   sub_5_5ECA
        test   eax, eax
        je     SHORT loc_5_4048
loc_5_401B:
        mov    ecx, 8000h
        call   sub_5_3874
        mov    ecx, 1
        call   sub_5_3874
        db  068h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        mov    ecx, 3
        call   sub_5_CBE2
        test   eax, eax
        jne    SHORT loc_5_404D
loc_5_4048:
        mov    ebx, 13h
loc_5_404D:
        xor    esi, esi
        db  039h, 035h, 024h, 000h, 000h, 000h
        jbe    SHORT loc_5_4072
        db  0BFh, 000h, 000h, 000h, 000h
loc_5_405C:
        push   dword ptr [edi]
        call   sub_5_D0E4
        add    esp, 4
        add    edi, 4
        inc    esi
        db  039h, 035h, 024h, 000h, 000h, 000h
        ja     SHORT loc_5_405C
loc_5_4072:
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        cmp    ebx, 13h
        jne    SHORT loc_5_408E
        push   -1
        call   sub_5_40A1
        add    esp, 4
        jmp    SHORT loc_5_4098
loc_5_408E:
        mov    ecx, 2
        call   sub_5_3874
loc_5_4098:
        mov    eax, ebx
loc_5_409A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_5_40A1:
        mov    ecx, 8000h
        call   sub_5_3874
        mov    ecx, 3
        call   sub_5_3874
        db  0BAh, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        call   sub_5_CC30
        xor    eax, eax
        db  039h, 005h, 000h, 000h, 000h, 000h
        je     SHORT loc_5_40DA
        push   1
        db  0A3h, 000h, 000h, 000h, 000h
        call   sub_5_23DF
        add    esp, 4
loc_5_40DA:
        push   0
        push   19h
        call   sub_5_5FFE
        add    esp, 8
        cmp    dword ptr [esp + 4], -1
        je     SHORT loc_5_40F6
        mov    ecx, dword ptr [esp + 4]
        call   sub_5_3C0E
loc_5_40F6:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_4101:
        mov    ecx, 8001h
        call   sub_5_3874
        xor    eax, eax
        ret
sub_5_410E:
        push   ebp
        mov    eax, edx
        mov    ebp, esp
        sub    esp, 110h
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        je     SHORT loc_5_4188
        lea    edx, [ebp - 0ch]
        mov    ecx, eax
        call   sub_5_30A3
loc_5_412C:
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 110h]
        call   sub_5_C486
        lea    edx, [ebp - 0ch]
        lea    ecx, [ebp - 110h]
        call   sub_5_C44E
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 110h]
        call   sub_5_C44E
        lea    ecx, [ebp - 110h]
        mov    edx, esi
        call   sub_5_C44E
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 110h]
        push   eax
        push   ecx
        push   80000002h
        call   sub_5_D154
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_41EB
        jmp    NEAR PTR loc_5_423B
loc_5_4188:
        lea    eax, [ebp - 4]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   80000002h
        call   sub_5_D154
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_41AC
        mov    eax, 1dh
        jmp    NEAR PTR loc_5_4246
loc_5_41AC:
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 0ch]
        mov    dword ptr [ebp - 10h], 5
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 18h
        mov    ebx, eax
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        test   ebx, ebx
        je     NEAR PTR loc_5_412C
        mov    eax, 1dh
        jmp    SHORT loc_5_4246
loc_5_41EB:
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 0bh]
        mov    dword ptr [ebp - 10h], 4
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 18h
        mov    edi, eax
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        test   edi, edi
        jne    SHORT loc_5_4229
        mov    eax, dword ptr [ebp - 0bh]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        jmp    SHORT loc_5_4244
loc_5_4229:
        cmp    edi, 3f4h
        je     SHORT loc_5_423B
        mov    eax, 1dh
        cmp    edi, 2
        jne    SHORT loc_5_4246
loc_5_423B:
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], 0
loc_5_4244:
        xor    eax, eax
loc_5_4246:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_5_424F:
        push   ebp
        mov    ebp, esp
        sub    esp, 120h
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   80000002h
        call   sub_5_D154
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_427F
        mov    eax, 1dh
        jmp    NEAR PTR loc_5_44AF
loc_5_427F:
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 20h]
        mov    dword ptr [ebp - 10h], 5
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 18h
        mov    edi, eax
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        test   edi, edi
        je     SHORT loc_5_42BD
        mov    eax, 1dh
        jmp    NEAR PTR loc_5_44AF
loc_5_42BD:
        xor    esi, esi
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    ecx, esi
        je     SHORT loc_5_42FF
        lea    edx, [ebp - 18h]
        call   sub_5_30A3
        lea    eax, [ebp - 18h]
        lea    ecx, [ebp - 20h]
loc_5_42D4:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    SHORT loc_5_42F4
        or     dl, dl
        je     SHORT loc_5_42F0
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    SHORT loc_5_42F4
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    SHORT loc_5_42D4
loc_5_42F0:
        xor    eax, eax
        jmp    SHORT loc_5_42F9
loc_5_42F4:
        sbb    eax, eax
        sbb    eax, -1
loc_5_42F9:
        test   eax, eax
        jne    SHORT loc_5_430F
        jmp    SHORT loc_5_430A
loc_5_42FF:
        lea    edx, [ebp - 20h]
        lea    ecx, [ebp - 18h]
        call   sub_5_C486
loc_5_430A:
        mov    esi, 1
loc_5_430F:
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 120h]
        call   sub_5_C486
        lea    edx, [ebp - 18h]
        lea    ecx, [ebp - 120h]
        call   sub_5_C44E
        db  0BAh, 000h, 000h, 000h, 000h
        lea    ecx, [ebp - 120h]
        call   sub_5_C44E
        lea    ecx, [ebp - 120h]
        mov    edx, dword ptr [ebp + 8]
        call   sub_5_C44E
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 120h]
        push   eax
        push   ecx
        push   80000002h
        call   sub_5_D164
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_5_4371
        mov    eax, 1dh
        jmp    NEAR PTR loc_5_44AF
loc_5_4371:
        lea    eax, [ebp - 10h]
        lea    ecx, [ebp - 0ch]
        mov    dword ptr [ebp - 10h], 4
        push   eax
        push   ecx
        push   0
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_D11C
        add    esp, 18h
        cmp    eax, 3f4h
        je     SHORT loc_5_43B9
        cmp    eax, 2
        je     SHORT loc_5_43B9
        test   eax, eax
        je     SHORT loc_5_43C0
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        mov    eax, 1dh
        jmp    NEAR PTR loc_5_44AF
loc_5_43B9:
        mov    dword ptr [ebp - 0ch], 0
loc_5_43C0:
        mov    ebx, dword ptr [ebp - 0ch]
        mov    edi, dword ptr [ebp + 10h]
        xor    ebx, edi
        je     SHORT loc_5_4400
        push   4
        lea    eax, [ebp - 0ch]
        push   eax
        mov    dword ptr [ebp - 0ch], edi
        push   3
        push   0
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp - 4]
        call   sub_5_D0EC
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_5_4400
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        mov    eax, 1dh
        jmp    NEAR PTR loc_5_44AF
loc_5_4400:
        push   dword ptr [ebp - 4]
        call   sub_5_D0E4
        add    esp, 4
        test   esi, esi
        je     NEAR PTR loc_5_44AD
        test   ebx, ebx
        je     NEAR PTR loc_5_44AD
        test   bl, 1
        je     SHORT loc_5_445D
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp + 8]
        push   ecx
        call   _CONFIGMG_Locate_DevNode
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_445D
        test   edi, 1
        push   0
        push   dword ptr [ebp - 8]
        je     SHORT loc_5_4455
        call   _CONFIGMG_Disable_DevNode
        add    esp, 8
        test   eax, eax
        je     SHORT loc_5_445D
        mov    eax, 22h
        jmp    SHORT loc_5_44AF
loc_5_4455:
        call   _CONFIGMG_Enable_DevNode
        add    esp, 8
loc_5_445D:
        test   bl, 2
        je     SHORT loc_5_44AD
        test   edi, 2
        je     SHORT loc_5_44A2
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp + 8]
        push   ecx
        call   _CONFIGMG_Locate_DevNode
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_5_44AD
        push   dword ptr [ebp - 8]
        call   sub_5_11DE
        add    esp, 4
        test   eax, eax
        je     SHORT loc_5_449B
        push   dword ptr [ebp - 8]
        call   sub_5_9014
        add    esp, 4
        jmp    SHORT loc_5_44AD
loc_5_449B:
        mov    eax, 22h
        jmp    SHORT loc_5_44AF
loc_5_44A2:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_44AD:
        xor    eax, eax
loc_5_44AF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
_PTEXT ends
        end
