; tlnk3_pcode_05.asm -- TLNK3 object 5 segment _PTEXT
.386p
_PTEXT segment dword public use32 'PCODE'

loc_5_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 103h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_002C:
        cmp    dword ptr [ebp - 0ch], 0eh
        jb     NEAR PTR loc_5_0048
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 123h], ecx
        jae    NEAR PTR loc_5_0052
loc_5_0048:
        mov    eax, 0c001000fh
        jmp    NEAR PTR loc_5_01E9
loc_5_0052:
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1b5h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_0131
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 24h
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_0125
        test   byte ptr [ebp + 0ch], 1
        je     NEAR PTR loc_5_009A
loc_5_009A:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 14h]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 12bh], 0
        jne    NEAR PTR loc_5_00CD
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 12bh], eax
        jmp    NEAR PTR loc_5_00DC
loc_5_00CD:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 12fh]
        mov    dword ptr [ecx + 24h], eax
loc_5_00DC:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 12fh], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 18h]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 10fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0120
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 10fh], 0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_2BB4
loc_5_0120:
        jmp    NEAR PTR loc_5_012C
loc_5_0125:
        mov    dword ptr [ebp - 10h], 0c0010002h
loc_5_012C:
        jmp    NEAR PTR loc_5_0192
loc_5_0131:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0153
        mov    dword ptr [ebp - 10h], 0c001000dh
        jmp    NEAR PTR loc_5_0192
loc_5_0153:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1b3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0175
        mov    dword ptr [ebp - 10h], 0c0000001h
        jmp    NEAR PTR loc_5_0192
loc_5_0175:
        push   2
        push   0bh
        push   2
        push   0aa000006h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_018F:
        add    esp, 14h
loc_5_0192:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        dec    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp - 1ch]
        cmp    dword ptr [eax + 59h], 0
        jne    NEAR PTR loc_5_01E1
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_01D3
        mov    eax, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ebp - 1ch]
        add    ecx, 65h
        cmp    dword ptr [eax + 65h], ecx
        je     NEAR PTR loc_5_01E1
loc_5_01D3:
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_5_37FB
        jmp    NEAR PTR loc_5_01E1
loc_5_01E1:
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_5_01E9
loc_5_01E9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
loc_5_01F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 40h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1b5h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_0499
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_048D
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 11ah]
        mov    word ptr [ebp - 18h], ax
        mov    dword ptr [ebp - 28h], 0
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], 0
        jmp    NEAR PTR loc_5_0260
loc_5_0260:
        jmp    NEAR PTR loc_5_0265
loc_5_0265:
        push   0
        lea    eax, [ebp - 34h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_027A:
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_5_047B
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0295:
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        add    eax, dword ptr [ecx + 0f5h]
        add    eax, 2
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 11ch]
        and    eax, 0ffffh
        add    eax, dword ptr [ebp - 1ch]
        add    eax, 6
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        mov    al, ch
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 11ch]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    dword ptr [ebp - 14h], eax
loc_5_0308:
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [ebp - 28h], eax
        jae    NEAR PTR loc_5_0473
        cmp    dword ptr [ebp - 24h], 0
        jne    NEAR PTR loc_5_0350
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_032B:
        cmp    dword ptr [ebp - 34h], 0
        jne    NEAR PTR loc_5_033A
        jmp    NEAR PTR loc_5_0473
loc_5_033A:
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 34h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_034B:
        jmp    NEAR PTR loc_5_0308
loc_5_0350:
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_5_03C0
        mov    eax, dword ptr [ebp - 1ch]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 18h], ax
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_5_0379
        jmp    NEAR PTR loc_5_0473
loc_5_0379:
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        add    eax, dword ptr [ecx + 0f5h]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 6
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 14h], ax
        sub    eax, eax
        mov    ah, byte ptr [ebp - 14h]
        mov    al, byte ptr [ebp - 13h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_5_0308
loc_5_03C0:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_5_0408
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp - 14h], eax
        jae    NEAR PTR loc_5_03F1
        xor    eax, eax
        sub    eax, dword ptr [ebp - 14h]
        neg    eax
        sub    dword ptr [ebp + 10h], eax
        mov    dword ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_5_0308
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_03F1:
        mov    eax, dword ptr [ebp + 10h]
        add    dword ptr [ebp - 2ch], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 10h]
        neg    eax
        sub    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp + 10h], 0
loc_5_0408:
        mov    eax, dword ptr [ebp + 14h]
        sub    eax, dword ptr [ebp - 28h]
        mov    dword ptr [ebp - 3ch], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 24h]
        cmp    eax, ecx
        jb     NEAR PTR loc_5_0421
        mov    eax, ecx
loc_5_0421:
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp - 3ch]
        mov    ecx, dword ptr [ebp - 38h]
        cmp    eax, ecx
        jb     NEAR PTR loc_5_0434
        mov    eax, ecx
loc_5_0434:
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0448:
        mov    eax, dword ptr [ebp - 38h]
        add    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp - 38h]
        add    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp - 38h]
        add    dword ptr [ebp - 28h], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 38h]
        neg    eax
        sub    dword ptr [ebp - 14h], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 38h]
        neg    eax
        sub    dword ptr [ebp - 24h], eax
        jmp    NEAR PTR loc_5_0308
loc_5_0473:
        mov    eax, dword ptr [ebp - 28h]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
loc_5_047B:
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_5_0494
loc_5_048D:
        mov    dword ptr [ebp - 8], 0c001000ch
loc_5_0494:
        jmp    NEAR PTR loc_5_04FA
loc_5_0499:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_04BB
        mov    dword ptr [ebp - 8], 0c001000dh
        jmp    NEAR PTR loc_5_04FA
loc_5_04BB:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1b4h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_04DD
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_5_04FA
loc_5_04DD:
        push   1
        push   0bh
        push   2
        push   0aa000006h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_04F7:
        add    esp, 14h
loc_5_04FA:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 40h], eax
        mov    eax, dword ptr [ebp - 40h]
        dec    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp - 40h]
        cmp    dword ptr [eax + 59h], 0
        jne    NEAR PTR loc_5_0549
        mov    eax, dword ptr [ebp - 40h]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_053B
        mov    eax, dword ptr [ebp - 40h]
        mov    ecx, dword ptr [ebp - 40h]
        add    ecx, 65h
        cmp    dword ptr [eax + 65h], ecx
        je     NEAR PTR loc_5_0549
loc_5_053B:
        mov    eax, dword ptr [ebp - 40h]
        push   eax
        call   sub_5_37FB
        jmp    NEAR PTR loc_5_0549
loc_5_0549:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_5_0551
loc_5_0551:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    18h
sub_5_0558:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_5_0580
        jmp    NEAR PTR loc_5_0695
loc_5_0580:
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0595:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_5_05A4
        jmp    NEAR PTR loc_5_0695
loc_5_05A4:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05B5:
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp - 0ch], eax
        jae    NEAR PTR loc_5_068D
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_5_05FD
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05D8:
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_5_05E7
        jmp    NEAR PTR loc_5_068D
loc_5_05E7:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_05F8:
        jmp    NEAR PTR loc_5_05B5
loc_5_05FD:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_5_0645
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp + 0ch], eax
        jbe    NEAR PTR loc_5_062E
        xor    eax, eax
        sub    eax, dword ptr [ebp - 10h]
        neg    eax
        sub    dword ptr [ebp + 0ch], eax
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_5_05B5
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_062E:
        mov    eax, dword ptr [ebp + 0ch]
        add    dword ptr [ebp - 8], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 0ch]
        neg    eax
        sub    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp + 0ch], 0
loc_5_0645:
        mov    eax, dword ptr [ebp + 10h]
        sub    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    eax, ecx
        jb     NEAR PTR loc_5_0658
        mov    eax, ecx
loc_5_0658:
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_066C:
        mov    eax, dword ptr [ebp - 18h]
        add    dword ptr [ebp + 14h], eax
        mov    eax, dword ptr [ebp - 18h]
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 18h]
        add    dword ptr [ebp - 0ch], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 18h]
        neg    eax
        sub    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_5_05B5
loc_5_068D:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
loc_5_0695:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_5_06C4
        jmp    NEAR PTR loc_5_07E5
loc_5_06C4:
        push   0
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_06D9:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_5_06E8
        jmp    NEAR PTR loc_5_07E5
loc_5_06E8:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_06F9:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
loc_5_06FF:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 10h], eax
        jae    NEAR PTR loc_5_07DD
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_5_0747
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0722:
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_5_0731
        jmp    NEAR PTR loc_5_07DD
loc_5_0731:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0742:
        jmp    NEAR PTR loc_5_06FF
loc_5_0747:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_5_078F
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [ebp - 0ch], eax
        jae    NEAR PTR loc_5_0778
        xor    eax, eax
        sub    eax, dword ptr [ebp - 0ch]
        neg    eax
        sub    dword ptr [ebp + 14h], eax
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_06FF
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_0778:
        mov    eax, dword ptr [ebp + 14h]
        add    dword ptr [ebp - 14h], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 14h]
        neg    eax
        sub    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp + 14h], 0
loc_5_078F:
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    eax, ecx
        jb     NEAR PTR loc_5_07AE
        mov    eax, ecx
loc_5_07AE:
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_07C2:
        mov    eax, dword ptr [ebp - 1ch]
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 1ch]
        add    dword ptr [ebp - 10h], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 1ch]
        neg    eax
        sub    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_5_06FF
loc_5_07DD:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
loc_5_07E5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 14h], 0
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_5_0814
        jmp    NEAR PTR loc_5_0A05
loc_5_0814:
        push   0
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0829:
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_5_0838
        jmp    NEAR PTR loc_5_0A05
loc_5_0838:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0849:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_085E:
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_5_086D
        jmp    NEAR PTR loc_5_0A05
loc_5_086D:
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_087E:
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp + 10h], eax
        jbe    NEAR PTR loc_5_09FD
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_5_08C6
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08A1:
        cmp    dword ptr [ebp - 24h], 0
        jne    NEAR PTR loc_5_08B0
        jmp    NEAR PTR loc_5_09FD
loc_5_08B0:
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08C1:
        jmp    NEAR PTR loc_5_087E
loc_5_08C6:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_5_0902
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08DD:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_5_08EC
        jmp    NEAR PTR loc_5_09FD
loc_5_08EC:
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_08FD:
        jmp    NEAR PTR loc_5_087E
loc_5_0902:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_5_094A
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp + 0ch], eax
        jbe    NEAR PTR loc_5_0933
        xor    eax, eax
        sub    eax, dword ptr [ebp - 10h]
        neg    eax
        sub    dword ptr [ebp + 0ch], eax
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_5_087E
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_0933:
        mov    eax, dword ptr [ebp + 0ch]
        add    dword ptr [ebp - 1ch], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 0ch]
        neg    eax
        sub    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp + 0ch], 0
loc_5_094A:
        cmp    dword ptr [ebp + 18h], 0
        je     NEAR PTR loc_5_0992
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [ebp + 18h], eax
        jbe    NEAR PTR loc_5_097B
        xor    eax, eax
        sub    eax, dword ptr [ebp - 4]
        neg    eax
        sub    dword ptr [ebp + 18h], eax
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_087E
        db  0E9h, 017h, 000h, 000h, 000h
loc_5_097B:
        mov    eax, dword ptr [ebp + 18h]
        add    dword ptr [ebp - 18h], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp + 18h]
        neg    eax
        sub    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp + 18h], 0
loc_5_0992:
        mov    eax, dword ptr [ebp + 10h]
        sub    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    eax, ecx
        jb     NEAR PTR loc_5_09AB
        mov    eax, ecx
loc_5_09AB:
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp - 28h]
        cmp    eax, ecx
        jb     NEAR PTR loc_5_09BE
        mov    eax, ecx
loc_5_09BE:
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_09D2:
        mov    eax, dword ptr [ebp - 28h]
        add    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 28h]
        add    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 28h]
        add    dword ptr [ebp - 14h], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 28h]
        neg    eax
        sub    dword ptr [ebp - 4], eax
        xor    eax, eax
        sub    eax, dword ptr [ebp - 28h]
        neg    eax
        sub    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_5_087E
loc_5_09FD:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
loc_5_0A05:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    18h
loc_5_0A0C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 44h]
        and    eax, 24h
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 4]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 44h]
        and    eax, 0dbh
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx]
        mov    byte ptr [ecx + 44h], al
        mov    al, 1
        jmp    NEAR PTR loc_5_0A4A
loc_5_0A4A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_5_0A51:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 44h]
        and    eax, 18h
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 4]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    al, byte ptr [eax + 44h]
        and    eax, 0e7h
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx]
        mov    byte ptr [ecx + 44h], al
        mov    al, 1
        jmp    NEAR PTR loc_5_0A8F
loc_5_0A8F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_5_0A96:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ach], 1
        mov    al, 1
        jmp    NEAR PTR loc_5_0AB6
loc_5_0AB6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_5_0ABD:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 44h], 0
        mov    al, 1
        jmp    NEAR PTR loc_5_0ADA
loc_5_0ADA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_5_0AE1:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f1h]
        mov    al, byte ptr [eax + 1e09h]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 2ah]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_0B42
        mov    eax, dword ptr [ebp - 4]
        inc    byte ptr [eax + 18h]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 18h]
        and    eax, 0ffh
        cmp    eax, 0ffh
        jne    NEAR PTR loc_5_0B42
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 55h]
        inc    edx
        xor    al, al
        out    dx, al
        xor    al, al
        jmp    NEAR PTR loc_5_0D22
loc_5_0B42:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        not    eax
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0f1h]
        mov    byte ptr [ecx + 1e29h], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1b5h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0B90
        mov    al, byte ptr [ebp - 8]
        and    eax, 0dfh
        mov    ecx, dword ptr [ebp - 4]
        or     al, byte ptr [ecx + 45h]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 45h], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 20h
        mov    byte ptr [ebp - 8], al
        jmp    NEAR PTR loc_5_0BA8
loc_5_0B90:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 45h]
        and    eax, 0ffh
        or     al, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 45h], 0
loc_5_0BA8:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 44h]
        and    eax, 0ffh
        or     al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 44h], al
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1ach]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0BDC
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1adh], 1
loc_5_0BDC:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1b2h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0C96
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f1h]
        mov    al, byte ptr [eax + 1e02h]
        mov    byte ptr [ebp - 0ch], al
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f1h]
        mov    al, byte ptr [eax + 1e03h]
        mov    byte ptr [ebp - 10h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        shl    eax, 8
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 10h]
        add    eax, ecx
        mov    word ptr [ebp - 18h], ax
        mov    ax, word ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0f9h], ax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f5h]
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        add    eax, ecx
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 14h], ax
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffh
        test   eax, eax
        jne    NEAR PTR loc_5_0C70
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2ah], 1
loc_5_0C70:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 34h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0C8F
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 55h]
        add    edx, 3
        xor    al, al
        out    dx, al
loc_5_0C8F:
        xor    al, al
        jmp    NEAR PTR loc_5_0D22
loc_5_0C96:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 34h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0CB5
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 55h]
        add    edx, 3
        xor    al, al
        out    dx, al
loc_5_0CB5:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        test   eax, eax
        jne    NEAR PTR loc_5_0CFC
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0f1h]
        mov    al, byte ptr [eax + 1e08h]
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        and    eax, 1ch
        mov    byte ptr [ebp - 8], al
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 46h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        not    eax
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0f1h]
        mov    byte ptr [ecx + 1e28h], al
loc_5_0CFC:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 44h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0D1B
        mov    al, 1
        jmp    NEAR PTR loc_5_0D22
        db  0E9h, 007h, 000h, 000h, 000h
loc_5_0D1B:
        xor    al, al
        jmp    NEAR PTR loc_5_0D22
loc_5_0D22:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_5_0D29:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 18h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 29h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 46h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0D89
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 46h]
        and    eax, 0ffh
        push   eax
        push   0ch
        push   8
        push   3
        push   0aa000003h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0D7F:
        add    esp, 18h
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 46h], 0
loc_5_0D89:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 44h]
        and    eax, 0ffh
        test   al, 24h
        je     NEAR PTR loc_5_0DB8
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 47h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_0DB8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_0DF3
loc_5_0DB8:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 44h]
        and    eax, 0ffh
        test   al, 18h
        je     NEAR PTR loc_5_0DEC
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 48h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_0DEC
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_5_1700
        jmp    NEAR PTR loc_5_0DEC
loc_5_0DEC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_5_0DF3:
        push   ebp
        mov    ebp, esp
        sub    esp, 7ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0E8B
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1adh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_0E8B
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2283
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 59h], 0
        jne    NEAR PTR loc_5_0E86
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_0E78
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 10h]
        add    ecx, 65h
        cmp    dword ptr [eax + 65h], ecx
        je     NEAR PTR loc_5_0E86
loc_5_0E78:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_5_37FB
        jmp    NEAR PTR loc_5_0E86
loc_5_0E86:
        jmp    NEAR PTR loc_5_16F9
loc_5_0E8B:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 47h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 1ch], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 18h], eax
        lea    eax, [ebp - 1ch]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_0EB6:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        test   al, 24h
        je     NEAR PTR loc_5_16A3
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        test   al, 20h
        je     NEAR PTR loc_5_1461
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1b4h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_12C2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0f9h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 2ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 2ch]
        test   eax, eax
        jne    NEAR PTR loc_5_11AD
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 36h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_1057
        mov    eax, dword ptr [ebp - 24h]
        mov    ax, word ptr [eax + 0ah]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 3fffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0fbh], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        sar    eax, 0eh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 10bh], al
        mov    eax, dword ptr [ebp - 24h]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 3fffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ffh], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        sar    eax, 0eh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 10ch], al
        mov    eax, dword ptr [ebp - 24h]
        mov    ax, word ptr [eax + 0ch]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 3fffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 103h], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        sar    eax, 0eh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 10dh], al
        mov    eax, dword ptr [ebp - 24h]
        mov    ax, word ptr [eax + 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 3fffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 107h], eax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        sar    eax, 0eh
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 10eh], al
        jmp    NEAR PTR loc_5_1113
loc_5_1057:
        mov    eax, dword ptr [ebp - 24h]
        mov    ax, word ptr [eax + 0ah]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0fbh], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ffh], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    ax, word ptr [eax + 0ch]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 103h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    ax, word ptr [eax + 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 107h], eax
loc_5_1113:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 19fh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b4h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 1b6h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b3h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b5h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5dh]
        mov    dword ptr [ebp - 20h], eax
loc_5_1153:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 5dh
        cmp    eax, dword ptr [ebp - 20h]
        je     NEAR PTR loc_5_119F
        mov    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 1ch]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_1192
        mov    eax, dword ptr [ebp - 28h]
        mov    byte ptr [eax + 1ch], 0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1192:
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_5_1153
loc_5_119F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2C68
        jmp    NEAR PTR loc_5_12BD
loc_5_11AD:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 19fh], 5
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b4h], 0
        mov    eax, dword ptr [ebp - 24h]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1b6h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1b6h]
        and    eax, 0ffh
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 1b6h]
        and    ecx, 0ffffh
        mov    al, ch
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1b6h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b3h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b5h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5dh]
        mov    dword ptr [ebp - 20h], eax
loc_5_121F:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 5dh
        cmp    eax, dword ptr [ebp - 20h]
        je     NEAR PTR loc_5_12BD
        mov    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 1ch]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_12B8
        mov    eax, dword ptr [ebp - 28h]
        mov    byte ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1b6h]
        and    eax, 0ffffh
        or     eax, 0c0011000h
        push   eax
        push   0c0010007h
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1274:
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_12AD:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 59h]
        jmp    NEAR PTR loc_5_12B8
loc_5_12B8:
        jmp    NEAR PTR loc_5_121F
loc_5_12BD:
        jmp    NEAR PTR loc_5_1461
loc_5_12C2:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 112h], 0
        je     NEAR PTR loc_5_12FA
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2A97
        xor    eax, eax
        mov    al, byte ptr [ebp - 34h]
        test   eax, eax
        je     NEAR PTR loc_5_12F5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
loc_5_12F5:
        jmp    NEAR PTR loc_5_1461
loc_5_12FA:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 10fh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_1461
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fbh]
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp - 38h]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 3ch], al
        mov    eax, dword ptr [ebp - 38h]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   eax, eax
        je     NEAR PTR loc_5_1453
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 8
        je     NEAR PTR loc_5_139A
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 7
        je     NEAR PTR loc_5_139A
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 6
        je     NEAR PTR loc_5_139A
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 1eh
        je     NEAR PTR loc_5_139A
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        push   eax
        push   8
        push   4
        push   3
        push   0aa00000bh
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1392:
        add    esp, 18h
        jmp    NEAR PTR loc_5_144E
loc_5_139A:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0d5h], 0
        jne    NEAR PTR loc_5_13AA
loc_5_13AA:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d5h]
        cmp    dword ptr [eax + 10h], 8
        jne    NEAR PTR loc_5_1422
        xor    eax, eax
        mov    al, byte ptr [ebp - 3ch]
        test   eax, eax
        jne    NEAR PTR loc_5_13F4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d5h]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_5_13EB
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2E4C
        jmp    NEAR PTR loc_5_13F4
loc_5_13EB:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2D8F
loc_5_13F4:
        push   0
        push   14h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1407:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0d5h], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
        jmp    NEAR PTR loc_5_144E
loc_5_1422:
        xor    eax, eax
        mov    al, byte ptr [ebp - 3ch]
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_32E7
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_144E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
loc_5_144E:
        jmp    NEAR PTR loc_5_145C
loc_5_1453:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
loc_5_145C:
        jmp    NEAR PTR loc_5_1461
loc_5_1461:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        test   al, 4
        je     NEAR PTR loc_5_167A
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ffh]
        mov    dword ptr [ebp - 40h], eax
        mov    eax, dword ptr [ebp - 40h]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        cmp    eax, 0ah
        jne    NEAR PTR loc_5_1647
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 111h]
        and    eax, 0ffh
        add    eax, 7fh
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 7fh
        xor    eax, edx
        sub    eax, edx
        mov    byte ptr [ebp - 4ch], al
        mov    eax, dword ptr [ebp - 40h]
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [ebp - 48h], al
        mov    eax, dword ptr [ebp - 40h]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 50h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 50h]
        test   eax, eax
        jne    NEAR PTR loc_5_14DB
        mov    dword ptr [ebp - 44h], 0
        jmp    NEAR PTR loc_5_154A
loc_5_14DB:
        xor    eax, eax
        mov    al, byte ptr [ebp - 50h]
        cmp    eax, 22h
        jne    NEAR PTR loc_5_1543
        mov    eax, dword ptr [ebp - 40h]
        mov    al, byte ptr [eax + 6]
        mov    byte ptr [ebp - 5ch], al
        mov    al, byte ptr [ebp - 5ch]
        and    eax, 0c0h
        shr    eax, 6
        mov    byte ptr [ebp - 54h], al
        mov    al, byte ptr [ebp - 5ch]
        and    eax, 0ch
        shr    eax, 2
        mov    byte ptr [ebp - 58h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 58h]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 54h]
        cmp    eax, ecx
        jne    NEAR PTR loc_5_152B
        xor    eax, eax
        mov    al, byte ptr [ebp - 54h]
        test   eax, eax
        je     NEAR PTR loc_5_1537
loc_5_152B:
        mov    dword ptr [ebp - 44h], 10001h
        jmp    NEAR PTR loc_5_153E
loc_5_1537:
        mov    dword ptr [ebp - 44h], 0
loc_5_153E:
        jmp    NEAR PTR loc_5_154A
loc_5_1543:
        mov    dword ptr [ebp - 44h], 0c0000001h
loc_5_154A:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4ch]
        inc    eax
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 7fh
        xor    eax, edx
        sub    eax, edx
        mov    byte ptr [ebp - 4ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 1bch]
        mov    dword ptr [ebp - 64h], eax
        cmp    dword ptr [ebp - 64h], 0
        jne    NEAR PTR loc_5_1580
        jmp    NEAR PTR loc_5_160C
loc_5_1580:
        mov    eax, dword ptr [ebp - 64h]
        add    eax, 24h
        mov    dword ptr [ebp - 6ch], eax
        mov    eax, dword ptr [ebp - 6ch]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_159C
loc_5_159C:
        mov    eax, dword ptr [ebp - 6ch]
        mov    byte ptr [eax + 0ch], 0
        mov    eax, dword ptr [ebp - 6ch]
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + eax*4 + 1bch], 0
        mov    eax, dword ptr [ebp - 64h]
        add    eax, 24h
        mov    dword ptr [ebp - 60h], eax
        mov    eax, dword ptr [ebp - 60h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 68h], eax
        cmp    dword ptr [ebp - 44h], 0
        jne    NEAR PTR loc_5_15E6
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 13bh]
        jmp    NEAR PTR loc_5_15EF
loc_5_15E6:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 143h]
loc_5_15EF:
        mov    eax, dword ptr [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp - 60h]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 68h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1606:
        mov    eax, dword ptr [ebp - 68h]
        dec    dword ptr [eax + 14h]
loc_5_160C:
        xor    eax, eax
        mov    al, byte ptr [ebp - 48h]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4ch]
        cmp    eax, ecx
        jne    NEAR PTR loc_5_154A
        xor    eax, eax
        mov    al, byte ptr [ebp - 4ch]
        inc    eax
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 7fh
        xor    eax, edx
        sub    eax, edx
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 111h], al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
        jmp    NEAR PTR loc_5_166A
loc_5_1647:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        push   eax
        push   8
        push   4
        push   3
        push   0aa00000bh
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1667:
        add    esp, 18h
loc_5_166A:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 1
loc_5_167A:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 70h], eax
        mov    eax, dword ptr [ebp - 70h]
        mov    dword ptr [ebp - 78h], eax
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 74h], eax
        lea    eax, [ebp - 78h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 70h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_169E:
        jmp    NEAR PTR loc_5_0EB6
loc_5_16A3:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 47h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 7ch], eax
        mov    eax, dword ptr [ebp - 7ch]
        dec    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp - 7ch]
        cmp    dword ptr [eax + 59h], 0
        jne    NEAR PTR loc_5_16F9
        mov    eax, dword ptr [ebp - 7ch]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_16EB
        mov    eax, dword ptr [ebp - 7ch]
        mov    ecx, dword ptr [ebp - 7ch]
        add    ecx, 65h
        cmp    dword ptr [eax + 65h], ecx
        je     NEAR PTR loc_5_16F9
loc_5_16EB:
        mov    eax, dword ptr [ebp - 7ch]
        push   eax
        call   sub_5_37FB
        jmp    NEAR PTR loc_5_16F9
loc_5_16F9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_1700:
        push   ebp
        mov    ebp, esp
        sub    esp, 0a0h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 48h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4ch], eax
        mov    eax, dword ptr [ebp - 4ch]
        mov    dword ptr [ebp - 54h], eax
        lea    eax, [ebp - 44h]
        mov    dword ptr [ebp - 50h], eax
        lea    eax, [ebp - 54h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_173D:
        xor    eax, eax
        mov    al, byte ptr [ebp - 44h]
        test   al, 18h
        je     NEAR PTR loc_5_2211
        xor    eax, eax
        mov    al, byte ptr [ebp - 44h]
        test   al, 8
        je     NEAR PTR loc_5_1F81
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 103h]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 14h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        mov    dword ptr [ebp - 0a0h], eax
        jmp    NEAR PTR loc_5_1F4D
loc_5_1775:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 2
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 103h]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffh
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 103h]
        mov    cx, word ptr [ecx + 6]
        and    ecx, 0ffffh
        mov    al, ch
        and    eax, 0ffffh
        test   al, 40h
        je     NEAR PTR loc_5_188B
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        push   14h
        lea    eax, [ebp - 58h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_17CE:
        test   eax, eax
        je     NEAR PTR loc_5_17F8
        push   5
        push   4
        push   2
        push   0aa000002h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_17F0:
        add    esp, 14h
        jmp    NEAR PTR loc_5_1F81
loc_5_17F8:
        mov    eax, dword ptr [ebp - 58h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 58h]
        mov    dword ptr [eax + 10h], 8
        mov    eax, dword ptr [ebp - 58h]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0cdh], 0
        jne    NEAR PTR loc_5_1848
        mov    eax, dword ptr [ebp - 58h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0d1h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d1h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0cdh], eax
        jmp    NEAR PTR loc_5_1862
loc_5_1848:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0cdh]
        mov    ecx, dword ptr [ebp - 58h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 58h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0cdh], eax
loc_5_1862:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 10fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_188B
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 10fh], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
loc_5_188B:
        jmp    NEAR PTR loc_5_1F81
loc_5_1890:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 103h]
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp - 38h]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 20h], ax
        mov    ax, word ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 11ah], ax
        mov    eax, dword ptr [ebp - 20h]
        and    eax, 0ffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 20h]
        and    ecx, 0ffffh
        mov    al, ch
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        add    eax, 2
        mov    dword ptr [ebp - 40h], eax
        mov    eax, dword ptr [ebp - 38h]
        mov    ax, word ptr [eax + 0ah]
        mov    word ptr [ebp - 1ch], ax
        sub    eax, eax
        mov    ah, byte ptr [ebp - 1ch]
        mov    al, byte ptr [ebp - 1bh]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 40h]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 8], ax
        sub    eax, eax
        mov    ax, word ptr [ebp - 8]
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 2
        mov    eax, dword ptr [ebp - 40h]
        add    eax, 6
        mov    dword ptr [ebp - 48h], eax
        mov    eax, dword ptr [ebp - 48h]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_5_1957
        mov    eax, dword ptr [ebp - 48h]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 1fh
        add    eax, 0eh
        mov    dword ptr [ebp - 2ch], eax
        jmp    NEAR PTR loc_5_195E
loc_5_1957:
        mov    dword ptr [ebp - 2ch], 0eh
loc_5_195E:
        mov    eax, dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 11ch], ax
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 13fh]
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 2ch], eax
        jbe    NEAR PTR loc_5_19B6
        cmp    dword ptr [ebp - 8], 6
        jb     NEAR PTR loc_5_19B1
        mov    eax, dword ptr [ebp - 48h]
        mov    dword ptr [ebp - 4], eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 40h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c9h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_19B1:
        jmp    NEAR PTR loc_5_19EC
loc_5_19B6:
        mov    eax, dword ptr [ebp - 48h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 1ch]
        sub    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 2ch]
        add    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 40h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c9h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_19EC:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c9h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_19FB:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 110h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_1A47
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 110h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 11eh], 0
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_375B
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 10h
        jmp    NEAR PTR loc_5_1AF0
loc_5_1A47:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 118h]
        and    eax, 0ffffh
        cmp    eax, 0ffffh
        jne    NEAR PTR loc_5_1A74
        mov    ax, word ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 116h], ax
        jmp    NEAR PTR loc_5_1AB2
loc_5_1A74:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 118h]
        and    eax, 0ffh
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 118h]
        and    ecx, 0ffffh
        mov    al, ch
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    dword ptr [ebp - 5ch], eax
        mov    ax, word ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 5ch]
        mov    word ptr [ecx], ax
loc_5_1AB2:
        mov    ax, word ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 118h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1bbh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_1AF0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1bbh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 4
loc_5_1AF0:
        jmp    NEAR PTR loc_5_1F81
loc_5_1AF5:
        mov    dword ptr [ebp - 60h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 103h]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffh
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 103h]
        mov    cx, word ptr [ecx + 6]
        and    ecx, 0ffffh
        mov    al, ch
        mov    word ptr [ebp - 64h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 2
        mov    eax, dword ptr [ebp - 64h]
        and    eax, 0ffffh
        test   al, 80h
        je     NEAR PTR loc_5_1C19
        db  0A1h, 000h, 000h, 000h, 000h
        push   eax
        push   0
        push   14h
        lea    eax, [ebp - 68h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1B5C:
        test   eax, eax
        je     NEAR PTR loc_5_1B86
        push   5
        push   4
        push   2
        push   0aa000002h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1B7E:
        add    esp, 14h
        jmp    NEAR PTR loc_5_1F81
loc_5_1B86:
        mov    eax, dword ptr [ebp - 68h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 68h]
        mov    dword ptr [eax + 10h], 8
        mov    eax, dword ptr [ebp - 68h]
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0cdh], 0
        jne    NEAR PTR loc_5_1BD6
        mov    eax, dword ptr [ebp - 68h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0d1h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d1h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0cdh], eax
        jmp    NEAR PTR loc_5_1BF0
loc_5_1BD6:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0cdh]
        mov    ecx, dword ptr [ebp - 68h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 68h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0cdh], eax
loc_5_1BF0:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 10fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_1C19
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 10fh], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
loc_5_1C19:
        mov    eax, dword ptr [ebp - 64h]
        and    eax, 0ffffh
        test   al, 20h
        je     NEAR PTR loc_5_1C2D
        or     dword ptr [ebp - 60h], 40h
loc_5_1C2D:
        mov    eax, dword ptr [ebp - 64h]
        and    eax, 0ffffh
        test   al, 40h
        je     NEAR PTR loc_5_1C44
        or     dword ptr [ebp - 60h], 80h
loc_5_1C44:
        mov    eax, dword ptr [ebp - 64h]
        and    eax, 0ffffh
        test   al, 80h
        je     NEAR PTR loc_5_1C5B
        or     dword ptr [ebp - 60h], 100h
loc_5_1C5B:
        mov    eax, dword ptr [ebp - 64h]
        and    eax, 0ffffh
        test   ah, 1
        je     NEAR PTR loc_5_1C73
        or     dword ptr [ebp - 60h], 200h
loc_5_1C73:
        mov    eax, dword ptr [ebp - 64h]
        and    eax, 0ffffh
        test   ah, 4
        je     NEAR PTR loc_5_1C8B
        or     dword ptr [ebp - 60h], 400h
loc_5_1C8B:
        mov    eax, dword ptr [ebp - 64h]
        and    eax, 0ffffh
        test   ah, 8
        je     NEAR PTR loc_5_1CA3
        or     dword ptr [ebp - 60h], 800h
loc_5_1CA3:
        mov    eax, dword ptr [ebp - 64h]
        and    eax, 0ffffh
        test   ah, 10h
        je     NEAR PTR loc_5_1CBB
        or     dword ptr [ebp - 60h], 1000h
loc_5_1CBB:
        mov    eax, dword ptr [ebp - 64h]
        and    eax, 0ffffh
        test   ah, 20h
        je     NEAR PTR loc_5_1CD3
        or     dword ptr [ebp - 60h], 2000h
loc_5_1CD3:
        mov    eax, dword ptr [ebp - 64h]
        and    eax, 0ffffh
        test   ah, 40h
        je     NEAR PTR loc_5_1CEB
        or     dword ptr [ebp - 60h], 4000h
loc_5_1CEB:
        mov    eax, dword ptr [ebp - 64h]
        and    eax, 0ffffh
        test   ah, 80h
        je     NEAR PTR loc_5_1D03
        or     dword ptr [ebp - 60h], 8000h
loc_5_1D03:
        cmp    dword ptr [ebp - 60h], 0
        je     NEAR PTR loc_5_1D73
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5dh]
        mov    dword ptr [ebp - 6ch], eax
loc_5_1D16:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 5dh
        cmp    eax, dword ptr [ebp - 6ch]
        je     NEAR PTR loc_5_1D67
        mov    eax, dword ptr [ebp - 6ch]
        mov    dword ptr [ebp - 70h], eax
        mov    eax, dword ptr [ebp - 70h]
        inc    dword ptr [eax + 14h]
        push   4
        lea    eax, [ebp - 60h]
        push   eax
        push   40010006h
        mov    eax, dword ptr [ebp - 70h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1D48:
        mov    eax, dword ptr [ebp - 70h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1D54:
        mov    eax, dword ptr [ebp - 6ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 6ch], eax
        mov    eax, dword ptr [ebp - 70h]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_1D16
loc_5_1D67:
        mov    eax, dword ptr [ebp - 60h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 19bh], eax
loc_5_1D73:
        jmp    NEAR PTR loc_5_1F81
loc_5_1D78:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 103h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 1]
        mov    byte ptr [ebp - 14h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + eax*4 + 1bch]
        mov    dword ptr [ebp - 28h], eax
        cmp    dword ptr [ebp - 28h], 0
        jne    NEAR PTR loc_5_1DFF
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 10fh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_1DCF
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 112h], 0
        jne    NEAR PTR loc_5_1DCF
loc_5_1DCF:
        mov    byte ptr [ebp - 30h], 0
        lea    eax, [ebp - 74h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2A97
        mov    dword ptr [ebp - 28h], eax
        cmp    dword ptr [ebp - 28h], 0
        jne    NEAR PTR loc_5_1DED
loc_5_1DED:
        xor    eax, eax
        mov    al, byte ptr [ebp - 74h]
        test   eax, eax
        jne    NEAR PTR loc_5_1DFA
loc_5_1DFA:
        jmp    NEAR PTR loc_5_1E0C
loc_5_1DFF:
        mov    byte ptr [ebp - 30h], 0
        mov    eax, dword ptr [ebp - 28h]
        add    eax, 24h
        mov    dword ptr [ebp - 24h], eax
loc_5_1E0C:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 3ch], ax
        mov    eax, dword ptr [ebp - 3ch]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 3ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 2
        lea    eax, [ebp - 10h]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_1E58:
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        call   sub_5_0558
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 110h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_1EBB
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 110h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 11eh], 1
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2CFA
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 10h
        jmp    NEAR PTR loc_5_1EF8
loc_5_1EBB:
        mov    eax, dword ptr [ebp - 28h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3798
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1bbh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_1EF8
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1bbh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 4
loc_5_1EF8:
        xor    eax, eax
        mov    al, byte ptr [ebp - 30h]
        test   eax, eax
        je     NEAR PTR loc_5_1F2E
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 10fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_1F2E
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 10fh], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
loc_5_1F2E:
        jmp    NEAR PTR loc_5_1F81
loc_5_1F33:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 2
        jmp    NEAR PTR loc_5_1F81
        db  0E9h, 034h, 000h, 000h, 000h
loc_5_1F4D:
        sub    dword ptr [ebp - 0a0h], 81h
        cmp    dword ptr [ebp - 0a0h], 3
        ja     NEAR PTR loc_5_1F33
        mov    eax, dword ptr [ebp - 0a0h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_1F81:
        xor    eax, eax
        mov    al, byte ptr [ebp - 44h]
        test   al, 10h
        je     NEAR PTR loc_5_21D6
        mov    byte ptr [ebp - 7ch], 0
        mov    byte ptr [ebp - 78h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 110h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_1FAC
loc_5_1FAC:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1bbh]
        and    eax, 0ffh
        cmp    eax, 1
        je     NEAR PTR loc_5_1FC3
loc_5_1FC3:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 116h]
        and    eax, 0ffffh
        cmp    eax, 0ffffh
        je     NEAR PTR loc_5_1FE1
        mov    byte ptr [ebp - 7ch], 1
loc_5_1FE1:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 133h], 0
        je     NEAR PTR loc_5_1FF5
        mov    byte ptr [ebp - 78h], 1
loc_5_1FF5:
        xor    eax, eax
        mov    al, byte ptr [ebp - 7ch]
        test   eax, eax
        je     NEAR PTR loc_5_211B
        xor    eax, eax
        mov    al, byte ptr [ebp - 78h]
        test   eax, eax
        je     NEAR PTR loc_5_2025
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 11eh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_211B
loc_5_2025:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 116h]
        mov    word ptr [ebp - 84h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 116h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 118h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jne    NEAR PTR loc_5_207A
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 116h], 0ffffh
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 118h], 0ffffh
        jmp    NEAR PTR loc_5_20B6
loc_5_207A:
        mov    eax, dword ptr [ebp - 84h]
        and    eax, 0ffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 84h]
        and    ecx, 0ffffh
        mov    al, ch
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    dword ptr [ebp - 80h], eax
        mov    eax, dword ptr [ebp - 80h]
        mov    ax, word ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 116h], ax
loc_5_20B6:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 110h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 11eh], 0
        mov    eax, dword ptr [ebp - 84h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_375B
        xor    eax, eax
        mov    al, byte ptr [ebp - 78h]
        test   eax, eax
        je     NEAR PTR loc_5_20FC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 14h
        jmp    NEAR PTR loc_5_2116
loc_5_20FC:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1bbh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 10h
loc_5_2116:
        jmp    NEAR PTR loc_5_21D6
loc_5_211B:
        xor    eax, eax
        mov    al, byte ptr [ebp - 78h]
        test   eax, eax
        je     NEAR PTR loc_5_21CC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 133h]
        mov    dword ptr [ebp - 8ch], eax
        mov    eax, dword ptr [ebp - 8ch]
        add    eax, 24h
        mov    dword ptr [ebp - 88h], eax
        mov    eax, dword ptr [ebp - 88h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 133h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 110h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 11eh], 1
        mov    eax, dword ptr [ebp - 8ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2CFA
        xor    eax, eax
        mov    al, byte ptr [ebp - 7ch]
        test   eax, eax
        jne    NEAR PTR loc_5_2198
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 133h], 0
        je     NEAR PTR loc_5_21AD
loc_5_2198:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 14h
        jmp    NEAR PTR loc_5_21C7
loc_5_21AD:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1bbh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 10h
loc_5_21C7:
        jmp    NEAR PTR loc_5_21D6
loc_5_21CC:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 110h], 1
loc_5_21D6:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 90h], eax
        mov    eax, dword ptr [ebp - 90h]
        mov    dword ptr [ebp - 98h], eax
        lea    eax, [ebp - 44h]
        mov    dword ptr [ebp - 94h], eax
        lea    eax, [ebp - 98h]
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 90h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_220C:
        jmp    NEAR PTR loc_5_173D
loc_5_2211:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 48h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 9ch], eax
        mov    eax, dword ptr [ebp - 9ch]
        dec    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp - 9ch]
        cmp    dword ptr [eax + 59h], 0
        jne    NEAR PTR loc_5_227C
        mov    eax, dword ptr [ebp - 9ch]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_226B
        mov    eax, dword ptr [ebp - 9ch]
        mov    ecx, dword ptr [ebp - 9ch]
        add    ecx, 65h
        cmp    dword ptr [eax + 65h], ecx
        je     NEAR PTR loc_5_227C
loc_5_226B:
        mov    eax, dword ptr [ebp - 9ch]
        push   eax
        call   sub_5_37FB
        jmp    NEAR PTR loc_5_227C
loc_5_227C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_2283:
        push   ebp
        mov    ebp, esp
        sub    esp, 58h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1a4h]
        mov    dword ptr [ebp - 58h], eax
        jmp    NEAR PTR loc_5_2A69
loc_5_229D:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    al, byte ptr [eax + 1e02h]
        mov    byte ptr [ebp - 0ch], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    al, byte ptr [eax + 1e03h]
        mov    byte ptr [ebp - 10h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        shl    eax, 8
        and    eax, 0ffffh
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 10h]
        add    eax, ecx
        mov    word ptr [ebp - 18h], ax
        mov    ax, word ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 0f9h], ax
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   eax, eax
        je     NEAR PTR loc_5_241D
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 19fh], 6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5dh]
        mov    dword ptr [ebp - 20h], eax
loc_5_232C:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 5dh
        cmp    eax, dword ptr [ebp - 20h]
        je     NEAR PTR loc_5_237B
        mov    eax, dword ptr [ebp - 20h]
        mov    dword ptr [ebp - 24h], eax
        mov    eax, dword ptr [ebp - 24h]
        inc    dword ptr [eax + 14h]
        push   0
        push   0
        push   40010007h
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_235C:
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2368:
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 24h]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_232C
loc_5_237B:
        push   1
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        push   eax
        push   7
        push   3
        push   4
        push   0aa000003h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_23A0:
        add    esp, 1ch
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1a4h], 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_23B9:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b4h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b3h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b5h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1a3h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1ach], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1adh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1aeh]
        dec    dword ptr [eax + 14h]
        push   0c0000001h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1aeh]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2418:
        jmp    NEAR PTR loc_5_2A90
loc_5_241D:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0f9h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    dword ptr [ebp - 14h], eax
        mov    edi, dword ptr [ebp - 14h]
        xor    eax, eax
        mov    ecx, 0fh
        rep stosd
        mov    eax, dword ptr [ebp - 14h]
        mov    byte ptr [eax], 3
        mov    eax, dword ptr [ebp - 14h]
        mov    word ptr [eax + 8], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 127h]
        and    eax, 0ffh
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 127h]
        and    ecx, 0ffffh
        mov    al, ch
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ecx + 18h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 129h]
        and    eax, 0ffh
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 129h]
        and    ecx, 0ffffh
        mov    al, ch
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ecx + 1ah], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 121h]
        and    eax, 0ffffh
        mov    al, ah
        mov    ecx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ecx + 121h]
        mov    ecx, dword ptr [ebp - 14h]
        mov    word ptr [ecx + 1ch], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 11fh]
        mov    ecx, dword ptr [ebp - 14h]
        mov    byte ptr [ecx + 1eh], al
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1a4h], 2
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 28h
        jmp    NEAR PTR loc_5_2A90
loc_5_24F9:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0f9h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    dword ptr [ebp - 28h], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_268C
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 19fh], 6
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b4h], 0
        mov    eax, dword ptr [ebp - 28h]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1b6h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1b6h]
        and    eax, 0ffh
        mov    ah, al
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 1b6h]
        and    ecx, 0ffffh
        mov    al, ch
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1b6h], ax
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b3h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b5h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1b6h]
        and    eax, 0ffffh
        or     eax, 0c0011000h
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5dh]
        mov    dword ptr [ebp - 2ch], eax
loc_5_25B0:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 5dh
        cmp    eax, dword ptr [ebp - 2ch]
        je     NEAR PTR loc_5_2601
        mov    eax, dword ptr [ebp - 2ch]
        mov    dword ptr [ebp - 34h], eax
        mov    eax, dword ptr [ebp - 34h]
        inc    dword ptr [eax + 14h]
        push   4
        lea    eax, [ebp - 30h]
        push   eax
        push   40010007h
        mov    eax, dword ptr [ebp - 34h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_25E2:
        mov    eax, dword ptr [ebp - 34h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_25EE:
        mov    eax, dword ptr [ebp - 2ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp - 34h]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_25B0
loc_5_2601:
        push   2
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 1b6h]
        and    eax, 0ffffh
        push   eax
        push   7
        push   3
        push   4
        push   0aa000003h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_262D:
        add    esp, 1ch
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1a4h], 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2646:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1a3h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1ach], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1adh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1aeh]
        dec    dword ptr [eax + 14h]
        push   0c0000001h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1aeh]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2687:
        jmp    NEAR PTR loc_5_2A90
loc_5_268C:
        mov    eax, dword ptr [ebp - 28h]
        mov    ax, word ptr [eax + 0ah]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0fbh], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    ax, word ptr [eax + 0eh]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ffh], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    ax, word ptr [eax + 0ch]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 103h], eax
        mov    eax, dword ptr [ebp - 28h]
        mov    ax, word ptr [eax + 8]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 8]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0f5h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 107h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0e5h], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3577
        test   eax, eax
        jne    NEAR PTR loc_5_27A7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_36A4
        test   eax, eax
        jne    NEAR PTR loc_5_2785
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2780:
        jmp    NEAR PTR loc_5_27A2
loc_5_2785:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1a4h], 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 28h
loc_5_27A2:
        jmp    NEAR PTR loc_5_27C4
loc_5_27A7:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1a4h], 3
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 28h
loc_5_27C4:
        jmp    NEAR PTR loc_5_2A90
loc_5_27C9:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fbh]
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp - 38h]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 3ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 3ch]
        test   eax, eax
        jne    NEAR PTR loc_5_282C
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_36A4
        test   eax, eax
        jne    NEAR PTR loc_5_280A
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2805:
        jmp    NEAR PTR loc_5_2827
loc_5_280A:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1a4h], 4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 28h
loc_5_2827:
        jmp    NEAR PTR loc_5_292B
loc_5_282C:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 19fh], 6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5dh]
        mov    dword ptr [ebp - 40h], eax
loc_5_2842:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 5dh
        cmp    eax, dword ptr [ebp - 40h]
        je     NEAR PTR loc_5_2891
        mov    eax, dword ptr [ebp - 40h]
        mov    dword ptr [ebp - 44h], eax
        mov    eax, dword ptr [ebp - 44h]
        inc    dword ptr [eax + 14h]
        push   0
        push   0
        push   40010007h
        mov    eax, dword ptr [ebp - 44h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2872:
        mov    eax, dword ptr [ebp - 44h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_287E:
        mov    eax, dword ptr [ebp - 40h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 40h], eax
        mov    eax, dword ptr [ebp - 44h]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_2842
loc_5_2891:
        push   3
        xor    eax, eax
        mov    al, byte ptr [ebp - 3ch]
        push   eax
        push   7
        push   3
        push   4
        push   0aa000003h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_28B3:
        add    esp, 1ch
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1a4h], 4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_28CC:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b4h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b3h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b5h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1a3h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1ach], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1adh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1aeh]
        dec    dword ptr [eax + 14h]
        push   0c0000001h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1aeh]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_292B:
        jmp    NEAR PTR loc_5_2A90
loc_5_2930:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fbh]
        mov    dword ptr [ebp - 48h], eax
        mov    eax, dword ptr [ebp - 48h]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 4ch], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4ch]
        test   eax, eax
        jne    NEAR PTR loc_5_2960
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_295B:
        jmp    NEAR PTR loc_5_2A5F
loc_5_2960:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 19fh], 6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 5dh]
        mov    dword ptr [ebp - 50h], eax
loc_5_2976:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 5dh
        cmp    eax, dword ptr [ebp - 50h]
        je     NEAR PTR loc_5_29C5
        mov    eax, dword ptr [ebp - 50h]
        mov    dword ptr [ebp - 54h], eax
        mov    eax, dword ptr [ebp - 54h]
        inc    dword ptr [eax + 14h]
        push   0
        push   0
        push   40010007h
        mov    eax, dword ptr [ebp - 54h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_29A6:
        mov    eax, dword ptr [ebp - 54h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_29B2:
        mov    eax, dword ptr [ebp - 50h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 50h], eax
        mov    eax, dword ptr [ebp - 54h]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_2976
loc_5_29C5:
        push   3
        xor    eax, eax
        mov    al, byte ptr [ebp - 4ch]
        push   eax
        push   7
        push   3
        push   4
        push   0aa000003h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_29E7:
        add    esp, 1ch
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1a4h], 3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2A00:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b4h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b3h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1b5h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1a3h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1ach], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1adh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1aeh]
        dec    dword ptr [eax + 14h]
        push   0c0000001h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1aeh]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2A5F:
        jmp    NEAR PTR loc_5_2A90
        db  0E9h, 027h, 000h, 000h, 000h
loc_5_2A69:
        dec    dword ptr [ebp - 58h]
        cmp    dword ptr [ebp - 58h], 3
        ja     NEAR PTR loc_5_2A90
        mov    eax, dword ptr [ebp - 58h]
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_5_2A90:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_2A97:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fbh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0feh
        jne    NEAR PTR loc_5_2AD2
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 0
        xor    eax, eax
        jmp    NEAR PTR loc_5_2BAD
loc_5_2AD2:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 112h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 112h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 24h
        mov    dword ptr [ebp - 0ch], eax
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 0ffh
        je     NEAR PTR loc_5_2B41
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 143h]
        push   0c0000001h
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2B34:
        mov    eax, dword ptr [ebp - 14h]
        dec    dword ptr [eax + 14h]
        xor    eax, eax
        jmp    NEAR PTR loc_5_2BAD
loc_5_2B41:
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 0ch], 1
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 0dh], al
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 24h
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_2B70
loc_5_2B70:
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + eax*4 + 1bch], 0
        je     NEAR PTR loc_5_2B8C
loc_5_2B8C:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 18h]
        mov    cl, byte ptr [ecx + 0dh]
        and    ecx, 0ffh
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + ecx*4 + 1bch], eax
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_5_2BAD
loc_5_2BAD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_2BB4:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0cdh], 0
        je     NEAR PTR loc_5_2BF6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2F52
        cmp    eax, 103h
        jne    NEAR PTR loc_5_2BF6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 20h
        jmp    NEAR PTR loc_5_2C61
loc_5_2BF6:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 12bh], 0
        je     NEAR PTR loc_5_2C57
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 12bh]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 24h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 12bh], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 112h], eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2CC1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 20h
        jmp    NEAR PTR loc_5_2C61
loc_5_2C57:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 10fh], 1
loc_5_2C61:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_2C68:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fbh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 10fh]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_2C93
loc_5_2C93:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 10fh], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2], 0feh
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 20h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_2CC1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fbh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 0ah
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2], 0feh
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 4], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_2CFA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 24h
        mov    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_2D1F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 107h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 0ah
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 0dh]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 2], 0
        sub    eax, eax
        mov    ax, word ptr [ebp - 4]
        mov    al, ah
        mov    ah, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 6], ax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_2D5D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fbh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 1eh
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 4], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2], 80h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_2D8F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fbh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 6]
        and    eax, 0ffffh
        mov    al, ah
        mov    ecx, dword ptr [ebp - 4]
        mov    ah, byte ptr [ecx + 6]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 0fbh]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        and    eax, 0ffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        mov    cx, word ptr [ecx]
        and    ecx, 0ffffh
        mov    al, ch
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 13bh], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 2]
        and    eax, 0ffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 8]
        mov    cx, word ptr [ecx + 2]
        and    ecx, 0ffffh
        mov    al, ch
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 13fh], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 4]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 143h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 5]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 147h], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_2E4C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fbh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 14bh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 14fh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 153h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 9]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 157h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 15bh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 15fh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 163h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 167h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0fh]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 16bh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 10h]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 16fh], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_2F30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fbh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 8
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_2F52:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
loc_5_2F5B:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0cdh], 0
        je     NEAR PTR loc_5_32D9
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0cdh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0cdh]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0cdh], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0cdh], 0
        jne    NEAR PTR loc_5_2FA8
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0d1h], 0
loc_5_2FA8:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 10h], 8
        jne    NEAR PTR loc_5_2FF5
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 4], 0
        je     NEAR PTR loc_5_2FD0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2F30
        jmp    NEAR PTR loc_5_2FD9
loc_5_2FD0:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2D5D
loc_5_2FD9:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 20h
        mov    dword ptr [ebp - 8], 103h
        jmp    NEAR PTR loc_5_31A0
loc_5_2FF5:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_5_315C
loc_5_3003:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 14h], 1010eh
        jne    NEAR PTR loc_5_3045
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d9h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ddh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0d9h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3577
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_3090
loc_5_3045:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 14h], 2010103h
        jne    NEAR PTR loc_5_3075
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0e1h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3577
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_3090
loc_5_3075:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0e9h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_36A4
        mov    dword ptr [ebp - 8], eax
loc_5_3090:
        jmp    NEAR PTR loc_5_31A0
loc_5_3095:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d9h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0ddh], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0d9h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3577
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_31A0
loc_5_30C7:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0e1h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3577
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_31A0
loc_5_30E7:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0e9h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_36A4
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_31A0
loc_5_3107:
        mov    dword ptr [ebp - 8], 103h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2F30
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 4], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f1h]
        mov    byte ptr [eax + 1e4bh], 20h
        jmp    NEAR PTR loc_5_31A0
loc_5_3133:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        push   1
        push   9
        push   3
        push   0aa000006h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3154:
        add    esp, 18h
        jmp    NEAR PTR loc_5_31A0
loc_5_315C:
        dec    dword ptr [ebp - 10h]
        cmp    dword ptr [ebp - 10h], 9
        ja     NEAR PTR loc_5_3133
        mov    eax, dword ptr [ebp - 10h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 005h, 002h, 005h, 005h, 005h, 005h
        db  003h, 004h
loc_5_31A0:
        cmp    dword ptr [ebp - 8], 103h
        jne    NEAR PTR loc_5_31C8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0d5h], eax
        mov    eax, 103h
        jmp    NEAR PTR loc_5_32E0
        db  0E9h, 00Ch, 001h, 000h, 000h
loc_5_31C8:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_5_32B3
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_5_326E
loc_5_31E0:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0ch], eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_31FE:
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_32AE
loc_5_3209:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_32AE
loc_5_3217:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_32AE
loc_5_3225:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c1h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_323A:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 59h]
        jmp    NEAR PTR loc_5_32AE
loc_5_3245:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        push   9
        push   5
        push   3
        push   0aa000006h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3266:
        add    esp, 18h
        jmp    NEAR PTR loc_5_32AE
loc_5_326E:
        dec    dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 14h], 9
        ja     NEAR PTR loc_5_3245
        mov    eax, dword ptr [ebp - 14h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 001h, 004h, 002h, 004h, 004h, 004h, 004h, 003h, 003h
loc_5_32AE:
        jmp    NEAR PTR loc_5_32D4
loc_5_32B3:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0ah
        push   5
        push   3
        push   0aa000006h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_32D1:
        add    esp, 18h
loc_5_32D4:
        jmp    NEAR PTR loc_5_2F5B
loc_5_32D9:
        xor    eax, eax
        jmp    NEAR PTR loc_5_32E0
loc_5_32E0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_32E7:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d5h]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_5_3306
loc_5_3306:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_5_3450
loc_5_3314:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        test   eax, eax
        je     NEAR PTR loc_5_3369
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2E4C
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3337:
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0d5h], 0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c1h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_335E:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 59h]
        jmp    NEAR PTR loc_5_3394
loc_5_3369:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0d5h], 0
        push   0c0000001h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c1h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_338E:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 59h]
loc_5_3394:
        jmp    NEAR PTR loc_5_3490
loc_5_3399:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        test   eax, eax
        je     NEAR PTR loc_5_3404
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d5h]
        cmp    dword ptr [eax + 14h], 2010104h
        jne    NEAR PTR loc_5_33CE
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e9h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0edh], eax
loc_5_33CE:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0d5h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0ch], eax
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_33F9:
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_3438
loc_5_3404:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0d5h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], eax
        push   0c0000001h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3432:
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 14h]
loc_5_3438:
        jmp    NEAR PTR loc_5_3490
loc_5_343D:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_3490
        db  0E9h, 040h, 000h, 000h, 000h
loc_5_3450:
        dec    dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 14h], 9
        ja     NEAR PTR loc_5_3490
        mov    eax, dword ptr [ebp - 14h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 001h, 004h, 002h, 004h, 004h, 004h, 004h, 003h, 003h
loc_5_3490:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0ch]
        test   eax, eax
        jne    NEAR PTR loc_5_3569
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_5_3529
loc_5_34AB:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 14h], 1010eh
        jne    NEAR PTR loc_5_34DF
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ddh]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0d9h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0e5h], 0
        jmp    NEAR PTR loc_5_34EC
loc_5_34DF:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0e1h], 0
loc_5_34EC:
        jmp    NEAR PTR loc_5_3569
loc_5_34F1:
        jmp    NEAR PTR loc_5_3569
loc_5_34F6:
        jmp    NEAR PTR loc_5_3569
loc_5_34FB:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        push   9
        push   6
        push   3
        push   0aa000006h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_351C:
        add    esp, 18h
        jmp    NEAR PTR loc_5_3569
        db  0E9h, 040h, 000h, 000h, 000h
loc_5_3529:
        dec    dword ptr [ebp - 18h]
        cmp    dword ptr [ebp - 18h], 9
        ja     NEAR PTR loc_5_34FB
        mov    eax, dword ptr [ebp - 18h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 001h, 004h, 002h, 004h, 004h, 004h, 004h, 003h, 003h
loc_5_3569:
        mov    al, 1
        jmp    NEAR PTR loc_5_3570
loc_5_3570:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_3577:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 7fffffffh
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0dah], 20h
        je     NEAR PTR loc_5_35A2
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_35CA
loc_5_35A2:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0dah], 40h
        je     NEAR PTR loc_5_35C3
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e1h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_5_35CA
loc_5_35C3:
        mov    dword ptr [ebp - 8], 0
loc_5_35CA:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 0e5h], ecx
        jne    NEAR PTR loc_5_35E8
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_5_3608
loc_5_35E8:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_3617
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0e5h], eax
        mov    dword ptr [ebp - 4], 103h
loc_5_3608:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_5_3610
loc_5_3610:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_3617:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fbh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 7
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2], 0feh
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        shr    eax, 18h
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 14h]
        shr    eax, 10h
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 2], ah
        mov    al, byte ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 3], al
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_367D
loc_5_367A:
        inc    dword ptr [ebp - 0ch]
loc_5_367D:
        cmp    dword ptr [ebp - 0ch], 4
        jae    NEAR PTR loc_5_369D
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ebp + eax - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx + 6], al
        jmp    NEAR PTR loc_5_367A
loc_5_369D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_36A4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e9h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_36CE
        mov    eax, 103h
        jmp    NEAR PTR loc_5_36C7
loc_5_36C7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_5_36CE:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fbh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 6
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2], 0feh
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        shr    eax, 18h
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 14h]
        shr    eax, 10h
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 2], ah
        mov    al, byte ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    byte ptr [ecx + 3], al
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_5_3734
loc_5_3731:
        inc    dword ptr [ebp - 0ch]
loc_5_3734:
        cmp    dword ptr [ebp - 0ch], 4
        jae    NEAR PTR loc_5_3754
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [ebp + eax - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    edx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + edx + 6], al
        jmp    NEAR PTR loc_5_3731
loc_5_3754:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_375B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 107h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 81h
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2], 0
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 4], 0
        mov    ax, word ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 6], ax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_3798:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 24h
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_5_37AF
loc_5_37AF:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 133h], 0
        jne    NEAR PTR loc_5_37D0
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 133h], eax
        jmp    NEAR PTR loc_5_37DF
loc_5_37D0:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 133h]
        mov    dword ptr [ecx + 24h], eax
loc_5_37DF:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 137h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_5_37FB:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 59h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_3846
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 1a4h], 0
        jne    NEAR PTR loc_5_3846
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 1a4h], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3846:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1a3h]
        and    eax, 0ffh
        test   eax, eax
        jne    NEAR PTR loc_5_3A1F
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 6dh
        cmp    dword ptr [eax + 6dh], ecx
        je     NEAR PTR loc_5_3A1F
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 6dh]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 14h]
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c9h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3895:
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_5_3969
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 14h], 2
        jne    NEAR PTR loc_5_3911
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 1a8h]
        cmp    dword ptr [eax + 18h], ecx
        jne    NEAR PTR loc_5_38EE
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_38EE:
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_38FC:
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_390C:
        jmp    NEAR PTR loc_5_3964
loc_5_3911:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 65h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 69h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 69h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 69h], eax
        mov    eax, dword ptr [ebp - 4]
        sub    dword ptr [eax + 14h], 2
loc_5_3964:
        jmp    NEAR PTR loc_5_3A1F
loc_5_3969:
        cmp    dword ptr [ebp - 8], 103h
        jne    NEAR PTR loc_5_39FE
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 10fh]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_399F
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 10fh], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_5_2BB4
loc_5_399F:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1dh], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 65h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 69h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 69h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 69h], eax
        mov    eax, dword ptr [ebp - 4]
        sub    dword ptr [eax + 14h], 2
        jmp    NEAR PTR loc_5_3A1F
loc_5_39FE:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0ah
        push   7
        push   3
        push   0aa000006h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3A1C:
        add    esp, 18h
loc_5_3A1F:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 65h
        cmp    dword ptr [eax + 65h], ecx
        je     NEAR PTR loc_5_3AA4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 65h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 14h], 0
        jne    NEAR PTR loc_5_3AA4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 1a8h]
        cmp    dword ptr [eax + 18h], ecx
        jne    NEAR PTR loc_5_3A65
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3A65:
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3A73:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3AA4:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 59h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_5_3AB1:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 29h]
        and    eax, 0ffh
        test   eax, eax
        je     NEAR PTR loc_5_3D0F
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 112h], 0
        jne    NEAR PTR loc_5_3B03
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 12bh], 0
        jne    NEAR PTR loc_5_3B03
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0cdh], 0
        je     NEAR PTR loc_5_3D0F
loc_5_3B03:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1b5h], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 29h], 0
        push   0
        push   0aa000003h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0c5h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3B2A:
        add    esp, 0ch
loc_5_3B2D:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0cdh], 0
        je     NEAR PTR loc_5_3C57
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0cdh]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0cdh]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 0cdh], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0cdh], 0
        jne    NEAR PTR loc_5_3B7A
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0d1h], 0
loc_5_3B7A:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_5_3C0E
loc_5_3B88:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], eax
        push   0
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3BA3:
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_3C52
loc_5_3BAE:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_3C52
loc_5_3BBC:
        push   0
        push   14h
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3BC9:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0d5h], 0
        jmp    NEAR PTR loc_5_3C52
loc_5_3BDB:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_3C52
loc_5_3BE9:
        push   0
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0c1h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3BFE:
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 59h]
        jmp    NEAR PTR loc_5_3C52
        db  0E9h, 044h, 000h, 000h, 000h
loc_5_3C0E:
        dec    dword ptr [ebp - 18h]
        cmp    dword ptr [ebp - 18h], 9
        ja     NEAR PTR loc_5_3C52
        mov    eax, dword ptr [ebp - 18h]
        db  08Ah, 080h, 000h, 000h, 000h, 000h
        and    eax, 0ffh
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 005h, 002h, 005h, 005h, 005h, 003h
        db  004h, 004h
loc_5_3C52:
        jmp    NEAR PTR loc_5_3B2D
loc_5_3C57:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 112h], 0
        je     NEAR PTR loc_5_3CAA
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 112h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 24h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 112h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], eax
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3CA4:
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 14h]
loc_5_3CAA:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 12bh], 0
        je     NEAR PTR loc_5_3D03
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 12bh]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 24h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 12bh], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], eax
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3CF8:
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_5_3CAA
loc_5_3D03:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 29h], 0
        jmp    NEAR PTR loc_5_3D46
loc_5_3D0F:
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 112h], 0
        jne    NEAR PTR loc_5_3D3F
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 12bh], 0
        jne    NEAR PTR loc_5_3D3F
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0cdh], 0
        je     NEAR PTR loc_5_3D46
loc_5_3D3F:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 29h], 1
loc_5_3D46:
        push   7530h
        mov    eax, dword ptr [ebp - 4]
        add    eax, 1dh
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_3D57:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
        db  000h, 000h
sub_5_3D60:
        pushfd
        pop    eax
        cli
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_3D70:
        push   ebp
        xor    al, al
        mov    ebp, esp
        mov    edx, dword ptr [esp + 8]
        out    dx, al
        push   dword ptr [ebp + 0ch]
        popfd
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_3D90:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 10h
        mov    byte ptr [edx + 56h], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [edx + 52h]
        push   esi
        call   sub_5_3D60
        push   esi
        mov    dword ptr [ebp - 10h], eax
        call   sub_5_4010
        mov    edx, esi
        mov    al, 0d0h
        out    dx, al
        mov    word ptr [ebp - 2], 1
loc_5_3DBE:
        push   esi
        call   sub_5_4010
        push   7
        push   esi
        call   sub_5_4040
        cmp    ax, 6d50h
        jne    NEAR PTR loc_5_3F1F
        push   0
        push   esi
        call   sub_5_4040
        push   1
        push   esi
        call   sub_5_4040
        push   2
        push   esi
        call   sub_5_4040
        xor    ecx, ecx
        mov    edx, dword ptr [ebp + 8]
        mov    cx, word ptr [ebp - 2]
        push   8
        lea    eax, [ecx + ecx*4]
        push   esi
        mov    dword ptr [ebp - 0ch], ecx
        lea    edi, [edx + eax*2]
        call   sub_5_4040
        mov    word ptr [ebp - 6], ax
        mov    ecx, dword ptr [ebp - 0ch]
        mov    bx, ax
        push   9
        mov    word ptr [edi + 4dh], ax
        push   esi
        db  066h, 089h, 01Ch, 04Dh, 000h, 000h, 000h, 000h
        call   sub_5_4040
        mov    word ptr [edi + 4fh], ax
        mov    ecx, dword ptr [ebp - 0ch]
        db  066h, 089h, 004h, 04Dh, 000h, 000h, 000h, 000h
        push   0ah
        mov    word ptr [ebp - 4], ax
        push   esi
        call   sub_5_4040
        mov    ecx, dword ptr [ebp - 0ch]
        test   byte ptr [ebp - 4], 20h
        db  066h, 089h, 004h, 04Dh, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_3E5E
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        jmp    SHORT loc_5_3E77
loc_5_3E5E:
        db  0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        inc    bx
        and    bx, 1
        shl    bx, 2
        add    bx, 0a20h
loc_5_3E77:
        mov    ax, word ptr [ebp - 6]
        and    ax, 180h
        shr    ax, 6
        mov    cx, word ptr [ebp - 4]
        and    eax, 0ffffh
        and    cx, 40h
        shr    cx, 6
        and    ecx, 0ffffh
        add    ecx, eax
        mov    eax, dword ptr [ebp - 0ch]
        db  066h, 08Bh, 014h, 04Dh, 000h, 000h, 000h, 000h
        db  083h, 03Dh, 000h, 000h, 000h, 000h, 000h
        db  066h, 089h, 014h, 045h, 000h, 000h, 000h, 000h
        jne    SHORT loc_5_3EF3
        mov    ax, word ptr [edi + 4fh]
        and    eax, 1fh
        sub    eax, 1fh
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    word ptr [edi + 51h], ax
        test   ax, ax
        jne    SHORT loc_5_3F1F
        mov    ax, word ptr [edi + 4fh]
        mov    ecx, dword ptr [ebp - 0ch]
        and    ax, 1fh
        shl    ax, 4
        add    ax, 200h
        mov    word ptr [edi + 51h], ax
        db  066h, 089h, 004h, 04Dh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_5_3F09
loc_5_3EF3:
        mov    word ptr [edi + 51h], bx
        mov    ecx, dword ptr [ebp - 0ch]
        db  066h, 089h, 01Dh, 000h, 000h, 000h, 000h
        db  066h, 089h, 01Ch, 04Dh, 000h, 000h, 000h, 000h
loc_5_3F09:
        mov    ax, word ptr [ebp - 2]
        mov    edx, esi
        sub    al, 30h
        out    dx, al
        xor    al, al
        out    dx, al
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [edi + 55h], 1
        inc    byte ptr [edx + 56h]
loc_5_3F1F:
        inc    word ptr [ebp - 2]
        cmp    word ptr [ebp - 2], 8
        jb     NEAR PTR loc_5_3DBE
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   esi
        call   sub_5_3D70
        db  00Fh, 0BEh, 00Dh, 000h, 000h, 000h, 000h
        db  066h, 08Bh, 004h, 04Dh, 000h, 000h, 000h, 000h
        lea    edx, [ecx*2]
        db  066h, 08Bh, 09Ah, 000h, 000h, 000h, 000h
        db  066h, 08Bh, 0B2h, 000h, 000h, 000h, 000h
        lea    ecx, [ecx + ecx*4]
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  066h, 08Bh, 082h, 000h, 000h, 000h, 000h
        db  066h, 089h, 01Dh, 000h, 000h, 000h, 000h
        db  066h, 08Bh, 09Ah, 000h, 000h, 000h, 000h
        mov    edx, dword ptr [ebp + 8]
        db  066h, 089h, 035h, 000h, 000h, 000h, 000h
        pop    edi
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        pop    esi
        mov    word ptr [edx + ecx*2 + 53h], bx
        xor    eax, eax
        mov    al, byte ptr [edx + 56h]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_5_3FA0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 52h]
        push   edi
        call   sub_5_3D60
        cmp    byte ptr [esi + 56h], 1
        mov    ebx, eax
        jb     SHORT loc_5_3FFB
        push   edi
        call   sub_5_4010
        mov    edx, edi
        mov    al, 0d1h
        out    dx, al
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        mov    al, 0ffh
        out    dx, al
        jne    SHORT loc_5_3FE7
        db  00Fh, 0BEh, 005h, 000h, 000h, 000h, 000h
        xor    ecx, ecx
        lea    edx, [eax + eax*4]
        mov    cx, word ptr [esi + edx*2 + 53h]
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax], ecx
loc_5_3FE7:
        mov    byte ptr [esi + 60h], 1
        push   ebx
        push   edi
        call   sub_5_3D70
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
loc_5_3FFB:
        push   ebx
        push   edi
        call   sub_5_3D70
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
sub_5_4010:
        push   ebp
        xor    al, al
        mov    ebp, esp
        mov    ecx, 0ffh
        mov    edx, dword ptr [ebp + 8]
        out    dx, al
        out    dx, al
        mov    ax, cx
loc_5_4022:
        out    dx, al
        test   al, 80h
        je     SHORT loc_5_4033
        add    ax, ax
        xor    al, 63h
        dec    ecx
        jne    SHORT loc_5_4022
        pop    ebp
        ret    4
loc_5_4033:
        add    ax, ax
        dec    ecx
        jne    SHORT loc_5_4022
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
sub_5_4040:
        push   ebp
        mov    al, byte ptr [esp + 0ch]
        mov    ebp, esp
        add    al, 80h
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, 1eh
        mov    edx, esi
        out    dx, al
loc_5_4056:
        push   64h
        db  0E8h, 000h, 000h, 000h, 000h
loc_5_405D:
        dec    edi
        jne    SHORT loc_5_4056
        xor    ecx, ecx
        mov    edi, 0fh
loc_5_4067:
        mov    edx, esi
        in     al, dx
        and    eax, 1
        mov    edx, edi
        dec    edi
        lea    ecx, [eax + ecx*2]
        test   edx, edx
        jne    SHORT loc_5_4067
        mov    ax, cx
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  055h, 08Ah, 044h, 024h, 00Ch, 08Bh, 0ECh, 004h, 080h, 056h, 057h, 08Bh, 075h, 008h, 0BFh, 009h
        db  000h, 000h, 000h, 08Bh, 096h, 0A1h, 000h, 000h, 000h, 0EEh, 08Bh, 056h, 055h, 083h, 0C2h, 00Ah
        db  066h, 0EDh, 0F6h, 0C4h, 080h, 074h, 012h, 06Ah, 00Ah, 0E8h, 000h, 000h, 000h, 000h, 08Bh, 056h
        db  055h, 083h, 0C2h, 00Ah, 04Fh, 066h, 0EDh, 075h, 0E9h, 08Bh, 056h, 055h, 05Fh, 083h, 0C2h, 00Ch
        db  05Eh, 066h, 0EDh, 05Dh, 0C2h, 008h, 000h, 08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_5_40D0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    eax, eax
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi + 55h]
        lea    edx, [ecx + 0eh]
        out    dx, ax
        lea    edx, [ecx + 6]
        in     ax, dx
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        lea    edx, [ecx + 8]
        mov    di, ax
        in     ax, dx
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        lea    edx, [ecx + 4]
        mov    bx, ax
        in     ax, dx
        and    bx, 180h
        shr    bx, 6
        and    di, 40h
        shr    di, 6
        xor    edx, edx
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        mov    ecx, edi
        mov    eax, ebx
        and    ecx, 0ffffh
        and    eax, 0ffffh
        add    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        db  066h, 08Bh, 014h, 045h, 000h, 000h, 000h, 000h
        pop    edi
        mov    eax, 100h
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [esi + 55h]
        pop    esi
        add    edx, 0eh
        pop    ebx
        out    dx, ax
        pop    ebp
        ret    8
        db  090h
_PTEXT ends
        end
