; nwlink_lcode_01_01.asm -- NWLINK object 1 segment _LTEXT  (part 1/2)
.386p
_LTEXT segment dword public use32 'LCODE'

NWLINK_Service_06:
        dec    ebx
        js     SHORT loc_1_0029
        cmp    ebx, 10h
        jae    SHORT loc_1_0029
        cmp    eax, 1
        je     SHORT loc_1_0015
        cmp    eax, 2
        je     SHORT loc_1_001F
        mov    al, 0ffh
        ret
loc_1_0015:
        db  089h, 034h, 09Dh, 0C8h, 054h, 000h, 000h
        xor    eax, eax
        ret
loc_1_001F:
        xor    eax, eax
        db  089h, 004h, 09Dh, 0C8h, 054h, 000h, 000h
        ret
loc_1_0029:
        mov    al, 0feh
        ret
loc_1_002C:
        push   ebx
        push   esi
        push   edi
        push   eax
        mov    ebx, dword ptr [eax + 24h]
        movzx  ecx, byte ptr [ebx + 1eh]
        jecxz  loc_1_0050
        mov    eax, dword ptr [esi + 1ch]
        mov    dx, word ptr [esi + 20h]
        lea    edi, [ebx + 1fh]
loc_1_0043:
        scasd
        jne    SHORT loc_1_004B
        cmp    word ptr [edi], dx
        je     SHORT loc_1_005A
loc_1_004B:
        add    edi, 2
        loop   loc_1_0043
loc_1_0050:
        push   ebx
        push   esi
        call   sub_1_5BD3
        add    esp, 8
loc_1_005A:
        pop    esi
        and    dword ptr [esi + 0ch], 0ffffdfffh
        mov    dword ptr [esi + 14h], 0
        call   sub_1_3FF7
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0072:
        push   esi
        push   eax
        call   sub_1_5CD9
        add    esp, 8
        ret
loc_1_007D:
        push   esi
        push   edi
        db  08Bh, 03Dh, 03Ch, 07Ch, 000h, 000h
        cmp    byte ptr [eax + 9], 0
        jne    NEAR PTR loc_1_0117
        cmp    byte ptr [edi + 283h], 0feh
        je     SHORT loc_1_0117
        lea    esi, [edi + 28eh]
        movzx  edx, byte ptr [esi + 1]
        movzx  eax, byte ptr [esi]
        dec    eax
        jns    SHORT loc_1_0125
        test   dword ptr [edi], 4
        je     SHORT loc_1_00B5
        cmp    dl, 4
        jne    SHORT loc_1_00FB
loc_1_00B5:
        sub    edx, 7
        sbb    ecx, ecx
        and    edx, ecx
        add    edx, 7
        push   esi
        lea    esi, [edi + 64h]
        push   esi
        push   edi
        db  0FFh, 014h, 095h, 008h, 055h, 000h, 000h
        add    esp, 0ch
loc_1_00CF:
        or     eax, eax
        je     SHORT loc_1_00FB
        lea    esi, [edi + 10h]
        add    eax, 2ah
        mov    word ptr [esi + 28h], ax
        call   sub_1_518D
        mov    dword ptr [edi + 65h], eax
        mov    dword ptr [esi + 0ch], 80h
        mov    dword ptr [esi + 4], 0
        mov    edx, dword ptr [edi + 8]
        call   sub_1_391B
loc_1_00FB:
        db  08Bh, 035h, 040h, 07Ch, 000h, 000h
        and    dword ptr [esi + 0ch], 0ffffdfffh
        mov    dword ptr [esi + 14h], 0
        call   sub_1_3FF7
        pop    edi
        pop    esi
        ret
loc_1_0117:
        db  0BEh, 0C7h, 05Dh, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        pop    edi
        pop    esi
        ret
loc_1_0125:
        mov    edx, eax
        xor    eax, eax
        push   edi
        db  0BFh, 0C8h, 054h, 000h, 000h
        mov    ecx, 10h
loc_1_0134:
        repe scasd
        je     SHORT loc_1_014B
        dec    edx
        jns    SHORT loc_1_0134
        mov    ecx, dword ptr [edi - 4]
        pop    edi
        lea    eax, [edi + 64h]
        push   eax
        push   esi
        call   ecx
        add    esp, 8
        jmp    SHORT loc_1_00CF
loc_1_014B:
        pop    edi
        jmp    SHORT loc_1_00FB
sub_1_014E:
        push   ebx
        push   esi
        push   edi
        movzx  edx, word ptr [esp + 10h]
        call   sub_1_5346
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_015F:
        test   dword ptr [edi], 4
        je     SHORT loc_1_0181
        or     dword ptr [edi], 20h
        mov    esi, dword ptr [edi + 2f4h]
        or     esi, esi
        je     SHORT loc_1_0181
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        mov    edx, edi
        jmp    NEAR PTR loc_1_027B
loc_1_0181:
        xor    eax, eax
        ret
loc_1_0184:
        push   esi
        push   edi
        mov    edi, dword ptr [eax - 4]
        cmp    byte ptr [eax + 9], 0
        je     SHORT loc_1_0195
        inc    dword ptr [edi + 2f0h]
loc_1_0195:
        test   dword ptr [edi], 20h
        jne    NEAR PTR loc_1_0246
        dec    dword ptr [edi + 2d8h]
        je     SHORT loc_1_020E
        mov    ecx, dword ptr [edi + 2e8h]
        jecxz  loc_1_01D2
        movzx  edx, word ptr [eax + 28h]
        add    ecx, edx
        cmp    ecx, 2ah
        jae    SHORT loc_1_01C1
        mov    ecx, 2ah
loc_1_01C1:
        cmp    ecx, 200h
        jb     SHORT loc_1_01CE
        mov    ecx, 200h
loc_1_01CE:
        mov    word ptr [eax + 28h], cx
loc_1_01D2:
        dec    dword ptr [edi + 2e0h]
        je     SHORT loc_1_01E6
        mov    edi, esi
        mov    esi, eax
        call   sub_1_410D
        pop    edi
        pop    esi
        ret
loc_1_01E6:
        mov    eax, dword ptr [edi + 2dch]
        mov    dword ptr [edi + 2e0h], eax
        mov    eax, dword ptr [edi + 2ech]
        mov    edx, edi
        db  0BEh, 07Bh, 002h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [edi + 2f4h], esi
        pop    edi
        pop    esi
        ret
loc_1_020E:
        push   edi
        lea    esi, [edi + 64h]
        call   sub_1_518D
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [edi + 2f0h]
        mov    word ptr [esi + 5], ax
        mov    edx, dword ptr [edi + 8]
        add    edi, 10h
        mov    word ptr [edi + 28h], 31h
        mov    dword ptr [edi + 0ch], 80h
        mov    dword ptr [edi + 4], 0
        mov    esi, edi
        call   sub_1_391B
        pop    edi
loc_1_0246:
        push   0
        push   22eh
        push   dword ptr [edi + 2d4h]
        db  0CDh, 020h, 00Bh, 000h, 028h, 000h
        mov    dword ptr [edi + 2d4h], 0
        and    dword ptr [edi], 0ffffffdbh
        test   dword ptr [edi], 10h
        jne    SHORT loc_1_0273
        pop    edi
        pop    esi
        xor    eax, eax
        ret
loc_1_0273:
        call   sub_1_5DC7
        pop    edi
        pop    esi
        ret
loc_1_027B:
        push   esi
        push   edi
        mov    dword ptr [edx + 2f4h], 0
        mov    edi, edx
        test   dword ptr [edx], 4
        je     SHORT loc_1_02AA
        test   dword ptr [edx], 20h
        jne    SHORT loc_1_0246
        mov    esi, dword ptr [edx + 2d4h]
        add    esi, 4
        mov    edi, dword ptr [edx + 4]
        call   sub_1_410D
loc_1_02AA:
        pop    edi
        pop    esi
        xor    eax, eax
        ret
loc_1_02AF:
        db  0FFh, 005h, 0C4h, 054h, 000h, 000h
        xor    esi, esi
        ret
loc_1_02B8:
        push   0
        push   58h
        push   eax
        db  0CDh, 020h, 00Bh, 000h, 028h, 000h
        ret
sub_1_02C4:
        push   ebx
        push   esi
        mov    eax, 1
        mov    ebx, 2
        db  0BEh, 026h, 060h, 000h, 000h
        call   0
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h
sub_1_02E0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    ax, word ptr [esi]
        or     ax, ax
        je     SHORT loc_1_030C
        push   eax
        call   sub_1_42A4
        add    esp, 4
        or     eax, eax
        je     SHORT loc_1_030C
        db  066h, 0FFh, 005h, 034h, 059h, 000h, 000h
        mov    eax, 0ffh
        jmp    NEAR PTR loc_1_0418
loc_1_030C:
        db  083h, 03Dh, 044h, 058h, 000h, 000h, 000h
        jne    SHORT loc_1_0326
        db  066h, 0FFh, 005h, 034h, 059h, 000h, 000h
        mov    eax, 0feh
        jmp    NEAR PTR loc_1_0418
loc_1_0326:
        db  0F6h, 005h, 0F8h, 058h, 000h, 000h, 008h
        je     SHORT loc_1_034A
        cmp    dword ptr [esp + 18h], 0
        jne    SHORT loc_1_034A
        push   esi
        push   0
        call   sub_1_544C
        add    esp, 8
        or     eax, eax
        je     SHORT loc_1_0358
        jmp    NEAR PTR loc_1_0418
loc_1_034A:
        cmp    word ptr [esi], 0
        jne    SHORT loc_1_0358
        call   sub_1_439A
        mov    word ptr [esi], ax
loc_1_0358:
        db  0A1h, 040h, 058h, 000h, 000h
        db  08Bh, 00Dh, 040h, 058h, 000h, 000h
        mov    edx, dword ptr [eax]
        lea    eax, [edx + 1]
        mov    dword ptr [ecx], eax
        db  0A1h, 040h, 058h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 032h, 059h, 000h, 000h
        mov    eax, dword ptr [eax]
        cmp    ecx, eax
        jae    SHORT loc_1_0382
        db  066h, 0A3h, 032h, 059h, 000h, 000h
loc_1_0382:
        db  08Bh, 00Dh, 070h, 058h, 000h, 000h
        db  0FFh, 00Dh, 044h, 058h, 000h, 000h
        mov    eax, dword ptr [ecx + 18h]
        db  0A3h, 070h, 058h, 000h, 000h
        db  0A1h, 060h, 058h, 000h, 000h
        mov    dword ptr [eax + edx*4], ecx
        db  0A1h, 05Ch, 058h, 000h, 000h
        mov    di, word ptr [esi]
        mov    word ptr [eax + edx*2], di
        db  0A1h, 064h, 058h, 000h, 000h
        mov    byte ptr [edx + eax], 0
        mov    ax, word ptr [esi]
        mov    word ptr [ecx + 14h], ax
        mov    dword ptr [ecx + 8], edx
        mov    eax, dword ptr [esp + 10h]
        or     eax, eax
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 18h], 0
        je     SHORT loc_1_03D5
        mov    dword ptr [ecx + 18h], 1
loc_1_03D5:
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 1ch], 0
        mov    dword ptr [ecx + 20h], 0
        lea    eax, [ecx + 0ch]
        mov    dword ptr [ecx + 28h], 0
        mov    dword ptr [ecx + 24h], 0
        mov    byte ptr [ecx + 16h], 0
        mov    byte ptr [ecx + 17h], 0
        mov    dword ptr [ecx + 2ch], 0
        mov    dword ptr [eax], eax
        mov    dword ptr [ecx + 10h], eax
        sub    eax, eax
        db  083h, 00Dh, 0F8h, 058h, 000h, 000h, 002h
loc_1_0418:
        pop    edi
        pop    esi
        ret
sub_1_041B:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 14h]
        call   sub_1_4300
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_0436
        jmp    NEAR PTR loc_1_055B
loc_1_0436:
        db  0F6h, 005h, 0F8h, 058h, 000h, 000h, 008h
        je     SHORT loc_1_0455
        cmp    dword ptr [esp + 18h], 0
        jne    SHORT loc_1_0455
        lea    eax, [esp + 14h]
        push   eax
        push   1
        call   sub_1_544C
        add    esp, 8
loc_1_0455:
        mov    eax, dword ptr [esi + 1ch]
        or     eax, eax
        je     SHORT loc_1_0476
loc_1_045C:
        mov    ecx, dword ptr [eax + 10h]
        mov    dword ptr [esi + 1ch], ecx
        push   1
        push   0
        push   eax
        call   sub_1_0562
        add    esp, 0ch
        mov    eax, dword ptr [esi + 1ch]
        or     eax, eax
        jne    SHORT loc_1_045C
loc_1_0476:
        mov    eax, dword ptr [esi + 20h]
        or     eax, eax
        je     SHORT loc_1_0490
loc_1_047D:
        push   1
        push   esi
        push   eax
        call   sub_1_0562
        add    esp, 0ch
        mov    eax, dword ptr [esi + 20h]
        or     eax, eax
        jne    SHORT loc_1_047D
loc_1_0490:
        call   sub_1_2196
        mov    ecx, dword ptr [esi + 0ch]
        mov    eax, esi
        sub    eax, ecx
        cmp    eax, -0ch
        je     SHORT loc_1_04B6
loc_1_04A1:
        push   ecx
        call   sub_1_5130
        add    esp, 4
        mov    ecx, dword ptr [esi + 0ch]
        mov    eax, esi
        sub    eax, ecx
        cmp    eax, -0ch
        jne    SHORT loc_1_04A1
loc_1_04B6:
        mov    eax, dword ptr [esi + 28h]
        or     eax, eax
        je     SHORT loc_1_04D0
loc_1_04BD:
        push   1
        push   esi
        push   eax
        call   sub_1_0562
        add    esp, 0ch
        mov    eax, dword ptr [esi + 28h]
        or     eax, eax
        jne    SHORT loc_1_04BD
loc_1_04D0:
        mov    edi, dword ptr [esi + 2ch]
        or     edi, edi
        je     SHORT loc_1_0530
loc_1_04D7:
        mov    eax, dword ptr [edi + 4]
        test   eax, 10000h
        jne    SHORT loc_1_0516
        and    eax, 0fffffcffh
        mov    dword ptr [edi + 4], eax
        mov    ebp, dword ptr [edi + 24h]
        or     ebp, ebp
        je     SHORT loc_1_051E
        mov    ebx, 0fch
loc_1_04F5:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [edi + 24h], eax
        push   dword ptr [ebp]
        call   sub_1_4608
        add    esp, 4
        mov    byte ptr [ebp + 9], bl
        mov    byte ptr [ebp + 8], 0
        mov    ebp, dword ptr [edi + 24h]
        or     ebp, ebp
        jne    SHORT loc_1_04F5
        jmp    SHORT loc_1_051E
loc_1_0516:
        or     eax, 800000h
        mov    dword ptr [edi + 4], eax
loc_1_051E:
        push   0
        push   edi
        call   sub_1_0BD0
        add    esp, 8
        mov    edi, dword ptr [esi + 2ch]
        or     edi, edi
        jne    SHORT loc_1_04D7
loc_1_0530:
        mov    word ptr [esi + 14h], 0
        db  0A1h, 070h, 058h, 000h, 000h
        mov    dword ptr [esi + 18h], eax
        db  089h, 035h, 070h, 058h, 000h, 000h
        db  0A1h, 040h, 058h, 000h, 000h
        db  0FFh, 005h, 044h, 058h, 000h, 000h
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_055B
        db  083h, 025h, 0F8h, 058h, 000h, 000h, 0FDh
loc_1_055B:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0562:
        push   esi
        push   edi
        cmp    dword ptr [esp + 14h], 0
        mov    esi, dword ptr [esp + 0ch]
        jne    SHORT loc_1_057F
        cmp    byte ptr [esi + 8], 0
        jne    SHORT loc_1_057F
        mov    eax, 0ffh
        jmp    NEAR PTR loc_1_064F
loc_1_057F:
        mov    eax, dword ptr [esi + 0ch]
        test   al, 10h
        je     SHORT loc_1_05A4
        test   al, 80h
        je     SHORT loc_1_058F
        jmp    NEAR PTR loc_1_064A
loc_1_058F:
        test   ah, 10h
        jne    NEAR PTR loc_1_0631
        db  066h, 0FFh, 005h, 03Ah, 059h, 000h, 000h
        jmp    NEAR PTR loc_1_064A
loc_1_05A4:
        test   al, 20h
        je     SHORT loc_1_0603
        test   ah, 20h
        je     SHORT loc_1_05BA
        or     eax, 4000h
        mov    dword ptr [esi + 0ch], eax
        jmp    NEAR PTR loc_1_064A
loc_1_05BA:
        mov    edi, dword ptr [esp + 10h]
        or     edi, edi
        jne    SHORT loc_1_05D1
        mov    ax, word ptr [esi + 0ah]
        push   eax
        call   sub_1_42A4
        add    esp, 4
        mov    edi, eax
loc_1_05D1:
        test   byte ptr [esi + 0ch], 80h
        push   10h
        push   esi
        je     SHORT loc_1_05EB
        lea    eax, [edi + 28h]
        push   eax
        call   sub_1_46EA
        add    esp, 0ch
        dec    dword ptr [edi + 24h]
        jmp    SHORT loc_1_05F7
loc_1_05EB:
        add    edi, 20h
        push   edi
        call   sub_1_46EA
        add    esp, 0ch
loc_1_05F7:
        push   dword ptr [esi]
        call   sub_1_4608
        add    esp, 4
        jmp    SHORT loc_1_0631
loc_1_0603:
        test   al, 40h
        je     SHORT loc_1_0617
        mov    eax, dword ptr [esi]
        add    eax, 58h
        push   eax
        call   sub_1_5130
        add    esp, 4
        jmp    SHORT loc_1_0631
loc_1_0617:
        test   ah, 1
        je     SHORT loc_1_064A
        mov    eax, dword ptr [esi + 10h]
        push   0
        or     dword ptr [eax + 4], 800000h
        push   eax
        call   sub_1_0BD0
        add    esp, 8
loc_1_0631:
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 0ch], 8000h
        mov    byte ptr [esi + 9], 0fch
        mov    byte ptr [esi + 8], 0
        sub    eax, eax
        jmp    SHORT loc_1_064F
loc_1_064A:
        mov    eax, 0f9h
loc_1_064F:
        pop    edi
        pop    esi
        ret
sub_1_0652:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        db  066h, 0FFh, 005h, 042h, 059h, 000h, 000h
        mov    ax, word ptr [esi + 0ah]
        push   eax
        call   sub_1_42A4
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    SHORT loc_1_0689
        mov    byte ptr [esi + 9], 0ffh
        mov    byte ptr [esi + 8], 0
        mov    eax, 0ffh
        jmp    NEAR PTR loc_1_0914
loc_1_0689:
        db  083h, 03Dh, 058h, 058h, 000h, 000h, 000h
        jne    SHORT loc_1_06AB
        mov    byte ptr [esi + 9], 0efh
        mov    byte ptr [esi + 8], 0
        db  066h, 0FFh, 005h, 044h, 059h, 000h, 000h
        mov    eax, 0efh
        jmp    NEAR PTR loc_1_0914
loc_1_06AB:
        db  083h, 03Dh, 048h, 058h, 000h, 000h, 000h
        je     NEAR PTR loc_1_0900
        db  083h, 03Dh, 04Ch, 058h, 000h, 000h, 002h
        jb     NEAR PTR loc_1_0900
        cmp    word ptr [esi + 22h], 1
        jne    NEAR PTR loc_1_08EA
        cmp    word ptr [esi + 28h], 2ah
        jne    NEAR PTR loc_1_08EA
        or     dword ptr [esi + 0ch], 180h
        db  08Bh, 03Dh, 074h, 058h, 000h, 000h
        db  0FFh, 00Dh, 058h, 058h, 000h, 000h
        mov    eax, dword ptr [edi + 10h]
        db  0A3h, 074h, 058h, 000h, 000h
        call   sub_1_43D4
        mov    word ptr [edi + 2], ax
        movzx  ecx, ax
        mov    eax, dword ptr [esp + 24h]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edi + 4], 10001h
        mov    eax, dword ptr [esp + 20h]
        or     eax, eax
        je     SHORT loc_1_071E
        mov    dword ptr [edi + 4], 10401h
loc_1_071E:
        add    al, al
        mov    byte ptr [edi + 1], al
        mov    byte ptr [edi], 2
        mov    dword ptr [edi + 0ch], ebp
        mov    dword ptr [edi + 1ch], 5
        lea    ecx, [edi + 30h]
        mov    eax, dword ptr [esp + 1ch]
        or     eax, eax
        jne    SHORT loc_1_0743
        mov    dword ptr [ecx], 0ah
        jmp    SHORT loc_1_0745
loc_1_0743:
        mov    dword ptr [ecx], eax
loc_1_0745:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edi + 34h], eax
        mov    dword ptr [edi + 24h], esi
        mov    dword ptr [edi + 14h], 0
        mov    dword ptr [edi + 18h], 0
        mov    eax, dword ptr [esi + 10h]
        mov    dword ptr [edi + 8], eax
        lea    eax, [edi + 8]
        mov    ebx, dword ptr [esp + 28h]
        push   eax
        lea    eax, [edi + 2ch]
        push   eax
        lea    eax, [ebx + 6]
        push   eax
        lea    eax, [edi + 38h]
        push   eax
        call   sub_1_621B
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_07A7
        mov    byte ptr [esi + 9], 0fah
        mov    byte ptr [esi + 8], 0
        db  0A1h, 074h, 058h, 000h, 000h
        mov    dword ptr [edi + 10h], eax
        mov    eax, 0fah
        db  089h, 03Dh, 074h, 058h, 000h, 000h
        db  0FFh, 005h, 058h, 058h, 000h, 000h
        jmp    NEAR PTR loc_1_0914
loc_1_07A7:
        mov    eax, dword ptr [edi + 2ch]
        mov    ecx, eax
        shl    ecx, 8
        shr    eax, 8
        or     ecx, eax
        and    ecx, 0ffffh
        lea    eax, [edi + 3eh]
        mov    dword ptr [esp + 10h], eax
        push   0ch
        lea    eax, [ebx + 6]
        push   eax
        mov    dword ptr [edi + 2ch], ecx
        mov    eax, dword ptr [esp + 18h]
        add    eax, 6
        push   eax
        call   sub_1_4AF3
        add    esp, 0ch
        push   6
        lea    eax, [edi + 38h]
        push   eax
        lea    eax, [esi + 1ch]
        push   eax
        call   sub_1_4AF3
        mov    eax, dword ptr [esp + 1ch]
        add    esp, 0ch
        mov    word ptr [eax + 24h], 0
        mov    word ptr [eax + 26h], 0
        mov    eax, dword ptr [ebp + 24h]
        or     eax, eax
        je     SHORT loc_1_081C
        lea    ecx, [eax - 1]
        shr    ecx, 8
        dec    ax
        shl    ax, 8
        or     cx, ax
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax + 28h], cx
        jmp    SHORT loc_1_0826
loc_1_081C:
        mov    eax, dword ptr [esp + 10h]
        mov    word ptr [eax + 28h], 0
loc_1_0826:
        or     dword ptr [ebp + 18h], 2
        mov    eax, dword ptr [ebp + 8]
        db  08Bh, 00Dh, 064h, 058h, 000h, 000h
        or     byte ptr [eax + ecx], 4
        mov    eax, dword ptr [ebp + 2ch]
        mov    dword ptr [edi + 10h], eax
        mov    dword ptr [ebp + 2ch], edi
        inc    byte ptr [ebp + 16h]
        mov    byte ptr [ebx + 5], 5
        mov    byte ptr [ebx + 1eh], 0c0h
        mov    byte ptr [ebx + 1fh], 0
        mov    ax, word ptr [edi + 2]
        mov    word ptr [ebx + 20h], ax
        mov    word ptr [ebx + 22h], 0ffffh
        mov    word ptr [ebx + 24h], 0
        mov    word ptr [ebx + 26h], 0
        mov    eax, dword ptr [ebp + 24h]
        or     eax, eax
        je     SHORT loc_1_0885
        lea    ecx, [eax - 1]
        shr    ecx, 8
        dec    ax
        shl    ax, 8
        or     cx, ax
        mov    word ptr [ebx + 28h], cx
        jmp    SHORT loc_1_088B
loc_1_0885:
        mov    word ptr [ebx + 28h], 0
loc_1_088B:
        mov    dword ptr [edi + 24h], esi
        db  08Bh, 015h, 054h, 058h, 000h, 000h
        db  0FFh, 005h, 054h, 058h, 000h, 000h
        db  0A1h, 068h, 058h, 000h, 000h
        mov    cx, word ptr [edi + 2]
        mov    word ptr [eax + edx*2], cx
        db  0A1h, 06Ch, 058h, 000h, 000h
        mov    dword ptr [eax + edx*4], edi
        mov    eax, dword ptr [edi + 1ch]
        mov    dword ptr [edi + 20h], eax
        mov    byte ptr [esi + 8], 0ffh
        push   dword ptr [edi + 8]
        push   esi
        call   sub_1_4092
        add    esp, 8
        db  0F6h, 005h, 0F8h, 058h, 000h, 000h, 004h
        jne    SHORT loc_1_08E6
        push   0
        db  068h, 033h, 03Eh, 000h, 000h
        push   64h
        call   sub_1_4AAB
        add    esp, 0ch
        db  083h, 00Dh, 0F8h, 058h, 000h, 000h, 004h
loc_1_08E6:
        sub    eax, eax
        jmp    SHORT loc_1_0914
loc_1_08EA:
        mov    byte ptr [esi + 9], 0fdh
        mov    byte ptr [esi + 8], 0
        db  066h, 0FFh, 005h, 044h, 059h, 000h, 000h
        mov    eax, 0fdh
        jmp    SHORT loc_1_0914
loc_1_0900:
        mov    byte ptr [esi + 9], 1
        mov    byte ptr [esi + 8], 0
        db  066h, 0FFh, 005h, 044h, 059h, 000h, 000h
        mov    eax, 1
loc_1_0914:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_091C:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        db  066h, 0FFh, 005h, 046h, 059h, 000h, 000h
        mov    ax, word ptr [edi + 0ah]
        push   eax
        call   sub_1_42A4
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    SHORT loc_1_0951
        mov    byte ptr [edi + 9], 0ffh
        mov    byte ptr [edi + 8], 0
        db  066h, 0FFh, 005h, 048h, 059h, 000h, 000h
        jmp    NEAR PTR loc_1_0A30
loc_1_0951:
        push   edi
        call   sub_1_4488
        add    esp, 4
        or     eax, eax
        jne    SHORT loc_1_0972
        mov    byte ptr [edi + 9], 1
        mov    byte ptr [edi + 8], 0
        db  066h, 0FFh, 005h, 048h, 059h, 000h, 000h
        jmp    NEAR PTR loc_1_0A30
loc_1_0972:
        db  083h, 03Dh, 058h, 058h, 000h, 000h, 000h
        jne    SHORT loc_1_098F
        mov    byte ptr [edi + 9], 0efh
        mov    byte ptr [edi + 8], 0
        db  066h, 0FFh, 005h, 048h, 059h, 000h, 000h
        jmp    NEAR PTR loc_1_0A30
loc_1_098F:
        db  08Bh, 01Dh, 074h, 058h, 000h, 000h
        db  0FFh, 00Dh, 058h, 058h, 000h, 000h
        mov    eax, dword ptr [ebx + 10h]
        db  0A3h, 074h, 058h, 000h, 000h
        call   sub_1_43D4
        mov    word ptr [ebx + 2], ax
        mov    dword ptr [edi + 10h], ebx
        mov    word ptr [edi + 22h], 0
        mov    dword ptr [ebx + 4], 30000h
        mov    eax, dword ptr [esp + 18h]
        or     eax, eax
        je     SHORT loc_1_09CB
        mov    dword ptr [ebx + 4], 30400h
loc_1_09CB:
        add    al, al
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx], 1
        mov    dword ptr [ebx + 0ch], esi
        mov    dword ptr [ebx + 20h], 0ffffffffh
        lea    eax, [ebx + 30h]
        mov    dword ptr [ebx + 1ch], 5
        mov    ecx, dword ptr [esp + 14h]
        or     ecx, ecx
        jne    SHORT loc_1_09F7
        mov    dword ptr [eax], 0ah
        jmp    SHORT loc_1_09F9
loc_1_09F7:
        mov    dword ptr [eax], ecx
loc_1_09F9:
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx + 34h], eax
        mov    dword ptr [ebx + 14h], 0
        mov    dword ptr [ebx + 18h], 0
        or     dword ptr [esi + 18h], 2
        mov    eax, dword ptr [esi + 8]
        db  08Bh, 00Dh, 064h, 058h, 000h, 000h
        or     byte ptr [eax + ecx], 4
        mov    eax, dword ptr [esi + 2ch]
        mov    dword ptr [ebx + 10h], eax
        mov    dword ptr [esi + 2ch], ebx
        inc    byte ptr [esi + 17h]
        mov    dword ptr [ebx + 24h], edi
        mov    byte ptr [edi + 8], 0feh
loc_1_0A30:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0A34:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    ecx, 64h
        mov    byte ptr [esi], 3
        mov    dword ptr [esi + 20h], 0ffffffffh
        mov    dword ptr [esi + 68h], 0
        mov    eax, dword ptr [esi + 2ch]
        and    eax, 0ffffh
        imul   eax, eax, 37h
        add    eax, 1c2h
        sub    edx, edx
        div    ecx
        lea    edi, [esi + 3eh]
        mov    dword ptr [esi + 1ch], eax
        mov    word ptr [edi], 0ffffh
        mov    word ptr [edi + 2], 2a00h
        mov    byte ptr [edi + 4], 0
        mov    byte ptr [edi + 5], 5
        push   0ah
        mov    eax, dword ptr [esi + 8]
        add    eax, 18h
        push   eax
        lea    eax, [edi + 12h]
        push   eax
        call   sub_1_4AF3
        add    esp, 0ch
        mov    ecx, dword ptr [esi + 0ch]
        mov    ax, word ptr [ecx + 14h]
        mov    word ptr [edi + 1ch], ax
        mov    byte ptr [edi + 1eh], 80h
        mov    byte ptr [edi + 1fh], 0
        mov    ax, word ptr [esi + 2]
        mov    word ptr [edi + 20h], ax
        mov    word ptr [edi + 24h], 0
        mov    word ptr [edi + 26h], 0
        mov    eax, dword ptr [ecx + 24h]
        or     eax, eax
        je     SHORT loc_1_0AD5
        lea    ecx, [eax - 1]
        shr    ecx, 8
        dec    ax
        shl    ax, 8
        or     cx, ax
        mov    word ptr [edi + 28h], cx
        jmp    SHORT loc_1_0ADB
loc_1_0AD5:
        mov    word ptr [edi + 28h], 0
loc_1_0ADB:
        push   dword ptr [esi + 8]
        lea    eax, [esi + 38h]
        push   eax
        push   edi
        call   sub_1_0AFF
        add    esp, 0ch
        mov    dword ptr [esi + 18h], eax
        mov    eax, dword ptr [esi + 24h]
        or     dword ptr [esi + 4], 300h
        mov    byte ptr [eax + 9], 0
        pop    edi
        pop    esi
        ret
sub_1_0AFF:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  083h, 03Dh, 048h, 058h, 000h, 000h, 003h
        jae    SHORT loc_1_0B21
        call   sub_1_1C7D
        db  083h, 03Dh, 048h, 058h, 000h, 000h, 000h
        jne    SHORT loc_1_0B21
        sub    eax, eax
        jmp    NEAR PTR loc_1_0BCB
loc_1_0B21:
        db  083h, 03Dh, 04Ch, 058h, 000h, 000h, 002h
        jae    SHORT loc_1_0B3F
        call   sub_1_1CDF
        db  083h, 03Dh, 04Ch, 058h, 000h, 000h, 002h
        jae    SHORT loc_1_0B3F
        sub    eax, eax
        jmp    NEAR PTR loc_1_0BCB
loc_1_0B3F:
        db  083h, 03Dh, 048h, 058h, 000h, 000h, 000h
        mov    eax, 0
        jbe    SHORT loc_1_0BCB
        db  08Bh, 03Dh, 0B4h, 058h, 000h, 000h
        db  0FFh, 00Dh, 048h, 058h, 000h, 000h
        mov    eax, dword ptr [edi + 38h]
        lea    esi, [edi + 38h]
        db  0A3h, 0B4h, 058h, 000h, 000h
        db  08Bh, 01Dh, 0B8h, 058h, 000h, 000h
        db  083h, 02Dh, 04Ch, 058h, 000h, 000h, 002h
        mov    ecx, dword ptr [ebx]
        mov    ebp, dword ptr [esp + 1ch]
        mov    eax, dword ptr [ecx]
        db  0A3h, 0B8h, 058h, 000h, 000h
        mov    dword ptr [ecx], 0
        mov    eax, dword ptr [esp + 14h]
        mov    dword ptr [ecx + 4], eax
        push   ebp
        push   2ah
        push   esi
        mov    dword ptr [ecx + 0ch], 2ah
        push   dword ptr [esp + 24h]
        call   dword ptr [ebp + 28h]
        add    esp, 10h
        lea    eax, [esi + 20h]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebx + 0ch], eax
        push   ebx
        push   edi
        call   sub_1_2498
        mov    dword ptr [esi + 0ch], 0
        mov    byte ptr [esi + 4], 80h
        lea    eax, [esi + 14h]
        mov    dword ptr [esi + 8], ebp
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 18h], eax
        mov    eax, edi
loc_1_0BCB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0BD0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        mov    edi, dword ptr [esi + 0ch]
        mov    ax, word ptr [esi + 2]
        push   eax
        call   sub_1_434F
        add    esp, 4
        test   byte ptr [esi + 4], 8
        je     SHORT loc_1_0BFD
        push   14h
        push   esi
        db  068h, 090h, 058h, 000h, 000h
        call   sub_1_46EA
        add    esp, 0ch
loc_1_0BFD:
        push   10h
        push   esi
        lea    eax, [edi + 2ch]
        push   eax
        call   sub_1_46EA
        add    esp, 0ch
        mov    eax, dword ptr [esi + 4]
        test   eax, 10000h
        je     SHORT loc_1_0C6C
        mov    ecx, dword ptr [esi + 24h]
        test   eax, 800000h
        je     SHORT loc_1_0C47
        mov    byte ptr [ecx + 9], 0fch
        mov    byte ptr [ecx + 8], 0
        test   byte ptr [esi + 6], 2
        je     SHORT loc_1_0C33
        dec    byte ptr [edi + 17h]
        jmp    SHORT loc_1_0C36
loc_1_0C33:
        dec    byte ptr [edi + 16h]
loc_1_0C36:
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     SHORT loc_1_0C9A
        push   eax
        call   sub_1_4608
        add    esp, 4
        jmp    SHORT loc_1_0C9A
loc_1_0C47:
        test   eax, 400000h
        mov    byte ptr [ecx + 9], 0edh
        jne    SHORT loc_1_0C56
        mov    byte ptr [ecx + 9], 0edh
loc_1_0C56:
        db  066h, 0FFh, 005h, 044h, 059h, 000h, 000h
        push   dword ptr [ecx]
        call   sub_1_4615
        add    esp, 4
        dec    byte ptr [edi + 16h]
        jmp    SHORT loc_1_0C9A
loc_1_0C6C:
        dec    byte ptr [edi + 16h]
        mov    ecx, dword ptr [esi + 24h]
        or     ecx, ecx
        je     SHORT loc_1_0C9A
        cmp    dword ptr [esp + 14h], 1
        sbb    bl, bl
        add    bl, 0edh
loc_1_0C80:
        mov    eax, dword ptr [ecx + 10h]
        mov    dword ptr [esi + 24h], eax
        mov    byte ptr [ecx + 9], bl
        push   dword ptr [ecx]
        call   sub_1_4615
        add    esp, 4
        mov    ecx, dword ptr [esi + 24h]
        or     ecx, ecx
        jne    SHORT loc_1_0C80
loc_1_0C9A:
        cmp    byte ptr [edi + 16h], 0
        jne    SHORT loc_1_0CB7
        cmp    byte ptr [edi + 17h], 0
        jne    SHORT loc_1_0CB7
        and    dword ptr [edi + 18h], 0fffffffdh
        mov    eax, dword ptr [edi + 8]
        db  08Bh, 00Dh, 064h, 058h, 000h, 000h
        and    byte ptr [eax + ecx], 0fbh
loc_1_0CB7:
        mov    ecx, dword ptr [esi + 18h]
        or     ecx, ecx
        je     SHORT loc_1_0CE1
        mov    dl, byte ptr [ecx + 3ch]
        lea    eax, [ecx + 38h]
        test   dl, 4
        je     SHORT loc_1_0CD8
        and    dl, 7fh
        mov    byte ptr [eax + 4], dl
        mov    dword ptr [eax + 0ch], 0
        jmp    SHORT loc_1_0CE1
loc_1_0CD8:
        push   ecx
        call   sub_1_4608
        add    esp, 4
loc_1_0CE1:
        db  0A1h, 074h, 058h, 000h, 000h
        mov    dword ptr [esi + 10h], eax
        db  089h, 035h, 074h, 058h, 000h, 000h
        db  0FFh, 005h, 058h, 058h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_0CF9:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 24h], 0
        jbe    SHORT loc_1_0D3C
        mov    edx, dword ptr [eax + 28h]
        mov    ecx, dword ptr [edx + 10h]
        mov    dword ptr [eax + 28h], ecx
        dec    dword ptr [eax + 24h]
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [esp + 0ch]
        mov    dword ptr [ecx + 40h], eax
        mov    byte ptr [edx + 9], 0edh
        movzx  eax, word ptr [esp + 8]
        shl    eax, 10h
        mov    ecx, dword ptr [edx + 0ch]
        and    ecx, 0ffffh
        or     eax, ecx
        mov    dword ptr [edx + 0ch], eax
        push   dword ptr [edx]
        call   sub_1_4615
        add    esp, 4
loc_1_0D3C:
        ret
        db  000h, 000h, 000h
sub_1_0D40:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 001h, 000h, 001h, 000h
        mov    ebx, dword ptr [ebx + 8]
        mov    ax, 0fdffh
        or     ax, word ptr [ebx + 2ch]
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
loc_1_0D72:
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 038h, 000h, 001h, 000h
        cmp    byte ptr [esi + 8], 0
        jne    SHORT loc_1_0D72
        cmp    ax, 0ffffh
        jne    SHORT loc_1_0D90
        db  0CDh, 020h, 01Bh, 000h, 001h, 000h
loc_1_0D90:
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        pop    esi
        pop    ebx
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0DB0:
        mov    ecx, dword ptr [esp + 4]
        cmp    ecx, 2710h
        jbe    SHORT loc_1_0DBE
        jmp    SHORT loc_1_0DCC
loc_1_0DBE:
        mov    eax, dword ptr [esp + 8]
        add    eax, eax
        sub    edx, edx
        div    ecx
        or     eax, eax
        jne    SHORT loc_1_0DD0
loc_1_0DCC:
        mov    ax, 1
loc_1_0DD0:
        ret
loc_1_0DD1:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 14h]
        mov    esi, dword ptr [ebx + 3ch]
        or     esi, esi
        je     NEAR PTR loc_1_0F27
        mov    eax, dword ptr [esp + 18h]
        cmp    eax, 40010008h
        je     SHORT loc_1_0DFF
        cmp    eax, 40010009h
        je     NEAR PTR loc_1_0EDD
        jmp    NEAR PTR loc_1_0F27
loc_1_0DFF:
        mov    edi, dword ptr [esp + 1ch]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebx + 1ch], eax
        mov    dword ptr [esi + 0ch], eax
        add    eax, 0eh
        mov    dword ptr [esi + 10h], eax
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx + 20h], eax
        push   dword ptr [ebx + 1ch]
        push   eax
        call   sub_1_0DB0
        add    esp, 8
        movzx  eax, ax
        mov    dword ptr [ebx + 24h], eax
        mov    word ptr [esi + 24h], ax
        push   6
        lea    eax, [edi + 14h]
        push   eax
        lea    eax, [esi + 1ch]
        push   eax
        call   sub_1_4AF3
        add    esp, 0ch
        push   6
        add    edi, 0eh
        push   edi
        db  068h, 0A0h, 057h, 000h, 000h
        call   sub_1_4AF3
        add    esp, 0ch
        and    dword ptr [esi + 8], 0ffffffefh
        db  0F6h, 005h, 0F8h, 058h, 000h, 000h, 0C0h
        jne    SHORT loc_1_0EB6
        sub    ebp, ebp
        db  039h, 02Dh, 0C8h, 058h, 000h, 000h
        jbe    SHORT loc_1_0E9D
        db  0BFh, 0DCh, 058h, 000h, 000h
        sub    ebx, ebx
loc_1_0E70:
        db  0F6h, 005h, 0F8h, 058h, 000h, 000h, 010h
        je     SHORT loc_1_0E83
        push   dword ptr [edi]
        call   sub_1_6CBB
        add    esp, 4
loc_1_0E83:
        mov    eax, dword ptr [edi]
        cmp    esi, eax
        je     SHORT loc_1_0E91
        mov    dword ptr [eax + 18h], ebx
        mov    eax, dword ptr [edi]
        mov    byte ptr [eax + 23h], bl
loc_1_0E91:
        add    edi, 4
        inc    ebp
        db  039h, 02Dh, 0C8h, 058h, 000h, 000h
        ja     SHORT loc_1_0E70
loc_1_0E9D:
        db  089h, 035h, 0ECh, 058h, 000h, 000h
        db  0C7h, 005h, 094h, 056h, 000h, 000h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_1_2FF9
        add    esp, 4
loc_1_0EB6:
        db  0F6h, 005h, 0F8h, 058h, 000h, 000h, 010h
        je     SHORT loc_1_0ED1
        db  083h, 03Dh, 008h, 07Dh, 000h, 000h, 000h
        jbe    SHORT loc_1_0ED1
        push   esi
        call   sub_1_7221
        add    esp, 4
loc_1_0ED1:
        db  081h, 025h, 0F8h, 058h, 000h, 000h, 0FFh, 07Fh, 0FFh, 0FFh
        jmp    SHORT loc_1_0F27
loc_1_0EDD:
        mov    eax, dword ptr [esi + 48h]
        or     dword ptr [esi + 8], 10h
        mov    dword ptr [eax + 20h], 0
        db  0F6h, 005h, 0F8h, 058h, 000h, 000h, 010h
        je     SHORT loc_1_0F07
        db  081h, 00Dh, 0F8h, 058h, 000h, 000h, 000h, 080h, 000h, 000h
        push   esi
        call   sub_1_6CBB
        add    esp, 4
loc_1_0F07:
        cmp    dword ptr [esi + 18h], 0
        je     SHORT loc_1_0F18
        mov    dword ptr [esi + 18h], 0
        mov    byte ptr [esi + 23h], 0
loc_1_0F18:
        call   sub_1_1B15
        db  081h, 025h, 0F8h, 058h, 000h, 000h, 03Fh, 0FFh, 0FFh, 0FFh
loc_1_0F27:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    10h
sub_1_0F2E:
        sub    esp, 0ch
        push   esi
        db  083h, 03Dh, 060h, 07Ch, 000h, 000h, 001h
        jne    SHORT loc_1_0F45
        mov    eax, 1
        jmp    NEAR PTR loc_1_104B
loc_1_0F45:
        db  0C7h, 005h, 060h, 07Ch, 000h, 000h, 001h, 000h, 000h, 000h
        push   -1
        push   0
        lea    eax, [esp + 10h]
        push   0ch
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 8], 0
        jne    SHORT loc_1_0F7A
        db  0C7h, 005h, 060h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    NEAR PTR loc_1_104B
loc_1_0F7A:
        db  083h, 03Dh, 078h, 058h, 000h, 000h, 001h
        sbb    eax, eax
        and    eax, 0fffffff0h
        add    eax, 48h
        push   eax
        mov    eax, dword ptr [esp + 0ch]
        db  0FFh, 035h, 02Ch, 058h, 000h, 000h
        add    eax, 8
        push   eax
        lea    eax, [esp + 18h]
        push   eax
        call   sub_1_2474
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_1_0FCA
        push   0
        push   0ch
        push   dword ptr [esp + 10h]
        call   sub_1_246E
        db  0C7h, 005h, 060h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    NEAR PTR loc_1_104B
loc_1_0FCA:
        mov    eax, dword ptr [esp + 8]
        db  0C7h, 000h, 0A4h, 058h, 000h, 000h
        db  08Bh, 00Dh, 0A8h, 058h, 000h, 000h
        mov    eax, dword ptr [esp + 8]
        sub    esi, esi
        mov    dword ptr [eax + 4], ecx
        db  08Bh, 00Dh, 0A8h, 058h, 000h, 000h
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 8]
        db  0A3h, 0A8h, 058h, 000h, 000h
        db  039h, 035h, 02Ch, 058h, 000h, 000h
        jbe    SHORT loc_1_1034
loc_1_1000:
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [eax + 8]
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        call   sub_1_2480
        mov    ecx, dword ptr [esp + 4]
        db  0A1h, 0B4h, 058h, 000h, 000h
        mov    dword ptr [ecx + 38h], eax
        inc    esi
        mov    eax, dword ptr [esp + 4]
        db  0A3h, 0B4h, 058h, 000h, 000h
        db  039h, 035h, 02Ch, 058h, 000h, 000h
        ja     SHORT loc_1_1000
loc_1_1034:
        db  0A1h, 02Ch, 058h, 000h, 000h
        db  001h, 005h, 048h, 058h, 000h, 000h
        db  0C7h, 005h, 060h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        sub    eax, eax
loc_1_104B:
        pop    esi
        add    esp, 0ch
        ret
sub_1_1050:
        sub    esp, 0ch
        push   esi
        db  083h, 03Dh, 064h, 07Ch, 000h, 000h, 001h
        jne    SHORT loc_1_1067
        mov    eax, 1
        jmp    NEAR PTR loc_1_115E
loc_1_1067:
        db  0C7h, 005h, 064h, 07Ch, 000h, 000h, 001h, 000h, 000h, 000h
        push   -1
        push   0
        lea    eax, [esp + 10h]
        push   0ch
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 8], 0
        jne    SHORT loc_1_109C
        db  0C7h, 005h, 064h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    NEAR PTR loc_1_115E
loc_1_109C:
        mov    eax, dword ptr [esp + 8]
        db  0FFh, 035h, 030h, 058h, 000h, 000h
        add    eax, 8
        push   eax
        lea    eax, [esp + 14h]
        push   eax
        call   sub_1_2486
        cmp    dword ptr [esp + 0ch], 0
        je     SHORT loc_1_10DC
        push   0
        push   0ch
        push   dword ptr [esp + 10h]
        call   sub_1_246E
        db  0C7h, 005h, 064h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        mov    eax, 1
        jmp    NEAR PTR loc_1_115E
loc_1_10DC:
        mov    eax, dword ptr [esp + 8]
        db  0C7h, 000h, 0ACh, 058h, 000h, 000h
        db  0A1h, 0B0h, 058h, 000h, 000h
        mov    ecx, dword ptr [esp + 8]
        sub    esi, esi
        mov    dword ptr [ecx + 4], eax
        db  0A1h, 0B0h, 058h, 000h, 000h
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 8]
        db  0A3h, 0B0h, 058h, 000h, 000h
        db  039h, 035h, 030h, 058h, 000h, 000h
        jbe    SHORT loc_1_1147
loc_1_1110:
        push   0
        mov    eax, dword ptr [esp + 0ch]
        push   0
        push   dword ptr [eax + 8]
        lea    eax, [esp + 10h]
        push   eax
        lea    eax, [esp + 1ch]
        push   eax
        call   sub_1_2492
        mov    ecx, dword ptr [esp + 4]
        db  0A1h, 0B8h, 058h, 000h, 000h
        mov    dword ptr [ecx], eax
        inc    esi
        mov    eax, dword ptr [esp + 4]
        db  0A3h, 0B8h, 058h, 000h, 000h
        db  03Bh, 035h, 030h, 058h, 000h, 000h
        jb     SHORT loc_1_1110
loc_1_1147:
        db  0A1h, 030h, 058h, 000h, 000h
        db  001h, 005h, 04Ch, 058h, 000h, 000h
        db  0C7h, 005h, 064h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
        sub    eax, eax
loc_1_115E:
        pop    esi
        add    esp, 0ch
        ret
sub_1_1163:
        sub    esp, 4
        db  0A1h, 014h, 058h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_117A
        push   eax
        lea    eax, [esp + 4]
        push   eax
        call   sub_1_24B0
loc_1_117A:
        add    esp, 4
        ret
loc_1_117E:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, 10000000h
        mov    dword ptr [esi + 4], eax
        test   eax, 1000000h
        je     SHORT loc_1_11A7
        push   dword ptr [esi + 14h]
        push   0c0000001h
        push   dword ptr [esi + 10h]
        call   sub_1_24CE
        jmp    SHORT loc_1_11B8
loc_1_11A7:
        test   eax, 2000000h
        je     SHORT loc_1_11BE
        push   0
        push   dword ptr [esi + 10h]
        call   sub_1_24D4
loc_1_11B8:
        db  0FFh, 00Dh, 0C4h, 058h, 000h, 000h
loc_1_11BE:
        and    dword ptr [esi + 4], 0fcffffffh
        mov    dword ptr [esi + 10h], 0
        pop    esi
        ret    8
sub_1_11D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    esi, dword ptr [ebp + 0ch]
        movzx  eax, word ptr [esi]
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        db  0CDh, 020h, 04Fh, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        or     eax, eax
        jne    SHORT loc_1_1211
        sub    al, al
        jmp    SHORT loc_1_1234
loc_1_1211:
        mov    ax, word ptr [esi]
        mov    word ptr [ecx + 2], ax
        mov    word ptr [ecx], ax
        movzx  eax, word ptr [esi]
        mov    edi, dword ptr [ecx + 4]
        mov    ecx, eax
        mov    esi, dword ptr [esi + 4]
        shr    ecx, 2
        rep movsd
        mov    ecx, eax
        and    ecx, 3
        rep movsb
        mov    al, 1
loc_1_1234:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_123A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi + 2], 0
        je     SHORT loc_1_1282
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp - 4], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        db  0CDh, 020h, 051h, 000h, 001h, 000h
        add    esp, 8
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [esi + 4], 0
        mov    word ptr [esi + 2], 0
        mov    word ptr [esi], 0
loc_1_1282:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_1287:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esp + 10h]
        mov    ax, word ptr [edx]
        cmp    word ptr [edi], ax
        jne    SHORT loc_1_12B7
        movzx  eax, ax
        mov    ecx, eax
        shr    ecx, 2
        mov    esi, dword ptr [edx + 4]
        mov    edi, dword ptr [edi + 4]
        repe cmpsd
        jne    SHORT loc_1_12B2
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb
loc_1_12B2:
        sete   al
        jmp    SHORT loc_1_12B9
loc_1_12B7:
        sub    al, al
loc_1_12B9:
        pop    edi
        pop    esi
        ret
sub_1_12BC:
        sub    esp, 0ch
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 1ch]
        db  0A1h, 0C4h, 058h, 000h, 000h
        mov    dword ptr [esi], eax
        push   0
        mov    edi, dword ptr [esp + 1ch]
        db  068h, 078h, 07Ch, 000h, 000h
        lea    eax, [esp + 14h]
        push   edi
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        call   sub_1_249E
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_133B
        mov    eax, dword ptr [esp + 0ch]
        push   1
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 4], eax
        db  068h, 08Ch, 07Ch, 000h, 000h
        lea    eax, [esp + 14h]
        push   edi
        push   eax
        lea    eax, [esp + 20h]
        push   eax
        call   sub_1_249E
        cmp    dword ptr [esp + 10h], 0
        jne    SHORT loc_1_133B
        mov    eax, dword ptr [esp + 0ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 8], eax
        mov    eax, dword ptr [esp + 8]
        xchg   ah, al
        ror    eax, 10h
        xchg   ah, al
        mov    dword ptr [esp + 8], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi + 8], eax
        sub    eax, eax
        jmp    SHORT loc_1_1340
loc_1_133B:
        mov    eax, 1
loc_1_1340:
        pop    edi
        pop    esi
        add    esp, 0ch
        ret
sub_1_1346:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        or     esi, esi
        je     SHORT loc_1_136E
        mov    ebx, dword ptr [esi + 3ch]
        or     ebx, ebx
        mov    edi, dword ptr [esp + 20h]
        je     SHORT loc_1_13A1
loc_1_1360:
        cmp    dword ptr [ebx + 30h], edi
        je     SHORT loc_1_13A1
        mov    ebx, dword ptr [ebx + 4ch]
        or     ebx, ebx
        jne    SHORT loc_1_1360
        jmp    SHORT loc_1_13A1
loc_1_136E:
        sub    ecx, ecx
        db  039h, 00Dh, 038h, 058h, 000h, 000h
        jbe    SHORT loc_1_1396
        db  0B8h, 0DCh, 058h, 000h, 000h
        mov    edi, dword ptr [esp + 20h]
loc_1_1381:
        mov    ebx, dword ptr [eax]
        cmp    dword ptr [ebx + 30h], edi
        je     SHORT loc_1_139E
        add    eax, 4
        inc    ecx
        db  039h, 00Dh, 038h, 058h, 000h, 000h
        ja     SHORT loc_1_1381
        jmp    SHORT loc_1_139E
loc_1_1396:
        mov    edi, dword ptr [esp + 20h]
        mov    ebx, dword ptr [esp + 10h]
loc_1_139E:
        mov    esi, dword ptr [ebx + 48h]
loc_1_13A1:
        or     ebx, ebx
        mov    eax, dword ptr [esp + 18h]
        je     NEAR PTR loc_1_153B
        mov    dword ptr [eax], 0
        sub    ebp, ebp
        db  039h, 02Dh, 054h, 058h, 000h, 000h
        jbe    SHORT loc_1_13DF
loc_1_13BD:
        db  0A1h, 06Ch, 058h, 000h, 000h
        mov    eax, dword ptr [eax + ebp*4]
        cmp    dword ptr [eax + 8], ebx
        jne    SHORT loc_1_13D6
        push   1
        push   eax
        call   sub_1_0BD0
        add    esp, 8
        dec    ebp
loc_1_13D6:
        inc    ebp
        db  03Bh, 02Dh, 054h, 058h, 000h, 000h
        jb     SHORT loc_1_13BD
loc_1_13DF:
        db  0F6h, 005h, 0F8h, 058h, 000h, 000h, 010h
        je     SHORT loc_1_140C
        db  083h, 03Dh, 0C8h, 058h, 000h, 000h, 002h
        ja     SHORT loc_1_1403
        db  0F6h, 005h, 0F9h, 058h, 000h, 000h, 002h
        jne    SHORT loc_1_1403
        push   0
        call   sub_1_6BC0
        jmp    SHORT loc_1_1409
loc_1_1403:
        push   ebx
        call   sub_1_6CBB
loc_1_1409:
        add    esp, 4
loc_1_140C:
        mov    dword ptr [ebx + 18h], 0
        mov    dword ptr [ebx + 8], 0
        db  0FFh, 00Dh, 0C8h, 058h, 000h, 000h
        mov    ecx, dword ptr [ebx]
        db  039h, 00Dh, 0C8h, 058h, 000h, 000h
        je     SHORT loc_1_147C
        db  08Dh, 014h, 08Dh, 0DCh, 058h, 000h, 000h
        db  0A1h, 0C8h, 058h, 000h, 000h
        db  08Bh, 004h, 085h, 0DCh, 058h, 000h, 000h
        mov    dword ptr [edx], eax
        mov    dword ptr [eax], ecx
        mov    ebp, 1
        mov    eax, dword ptr [edx]
        shl    ebp, cl
        mov    dword ptr [eax + 4], ebp
        db  0A1h, 0C8h, 058h, 000h, 000h
        db  089h, 01Ch, 085h, 0DCh, 058h, 000h, 000h
        db  0A1h, 0C8h, 058h, 000h, 000h
        mov    edx, 1
        mov    dword ptr [ebx], eax
        db  08Ah, 00Dh, 0C8h, 058h, 000h, 000h
        db  0A1h, 0C8h, 058h, 000h, 000h
        db  08Bh, 004h, 085h, 0DCh, 058h, 000h, 000h
        shl    edx, cl
        mov    dword ptr [eax + 4], edx
loc_1_147C:
        db  039h, 01Dh, 0ECh, 058h, 000h, 000h
        jne    SHORT loc_1_1489
        call   sub_1_1B15
loc_1_1489:
        mov    ecx, dword ptr [esi + 3ch]
        cmp    ebx, ecx
        jne    SHORT loc_1_1498
        mov    eax, dword ptr [ebx + 4ch]
        mov    dword ptr [esi + 3ch], eax
        jmp    SHORT loc_1_14B1
loc_1_1498:
        or     ecx, ecx
        je     SHORT loc_1_14B1
loc_1_149C:
        mov    eax, dword ptr [ecx + 4ch]
        cmp    ebx, eax
        je     SHORT loc_1_14AB
        mov    ecx, eax
        or     ecx, ecx
        jne    SHORT loc_1_149C
        jmp    SHORT loc_1_14B1
loc_1_14AB:
        mov    eax, dword ptr [ebx + 4ch]
        mov    dword ptr [ecx + 4ch], eax
loc_1_14B1:
        dec    dword ptr [esi + 8]
        cmp    dword ptr [esi + 3ch], 0
        jne    NEAR PTR loc_1_1541
        and    dword ptr [esi + 4], 0fffffffeh
        call   sub_1_2196
        mov    eax, dword ptr [esi + 4]
        test   eax, 1000000h
        jne    SHORT loc_1_14D9
        or     eax, 2000000h
        mov    dword ptr [esi + 4], eax
loc_1_14D9:
        push   dword ptr [esi + 0ch]
        mov    dword ptr [esi + 10h], edi
        push   dword ptr [esp + 1ch]
        call   sub_1_24BC
        mov    eax, dword ptr [esp + 18h]
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_1541
        db  0FFh, 00Dh, 0C4h, 058h, 000h, 000h
        and    dword ptr [esi + 4], 0fdffffffh
        mov    ecx, dword ptr [esi]
        db  083h, 03Dh, 0C4h, 058h, 000h, 000h, 000h
        jbe    SHORT loc_1_1541
        db  039h, 00Dh, 0C4h, 058h, 000h, 000h
        je     SHORT loc_1_1541
        db  0A1h, 0C4h, 058h, 000h, 000h
        db  08Bh, 004h, 085h, 0CCh, 058h, 000h, 000h
        db  089h, 004h, 08Dh, 0CCh, 058h, 000h, 000h
        mov    dword ptr [eax], ecx
        db  0A1h, 0C4h, 058h, 000h, 000h
        db  089h, 034h, 085h, 0CCh, 058h, 000h, 000h
        db  0A1h, 0C4h, 058h, 000h, 000h
        mov    dword ptr [esi], eax
        jmp    SHORT loc_1_1541
loc_1_153B:
        mov    dword ptr [eax], 0c0000001h
loc_1_1541:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0ch
sub_1_154B:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 00Dh, 0F0h, 07Ch, 000h, 000h
        mov    edx, 1
        mov    eax, 6
        sub    ebx, ebx
loc_1_1561:
        cmp    byte ptr [ecx + 8], bl
        je     SHORT loc_1_1568
        mov    edx, ebx
loc_1_1568:
        add    ecx, 2ah
        dec    eax
        jne    SHORT loc_1_1561
        or     edx, edx
        je     SHORT loc_1_158E
        push   0
        push   1ech
        db  0FFh, 035h, 0F0h, 07Ch, 000h, 000h
        call   sub_1_246E
        db  0C7h, 005h, 0F0h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_158E:
        mov    esi, dword ptr [esp + 14h]
        mov    ebp, 1
        mov    edi, dword ptr [esi + 48h]
        sub    ecx, ecx
        mov    dl, byte ptr [edi + 31h]
        lea    eax, [edi + 30h]
loc_1_15A2:
        mov    bl, byte ptr [eax + ecx]
        cmp    dl, bl
        jae    SHORT loc_1_15AD
        mov    dl, bl
        mov    ebp, ecx
loc_1_15AD:
        inc    ecx
        cmp    ecx, 3
        jl     SHORT loc_1_15A2
        or     ebp, ebp
        je     SHORT loc_1_15C3
        cmp    ebp, 1
        je     SHORT loc_1_15D3
        cmp    ebp, 2
        je     SHORT loc_1_15EA
        jmp    SHORT loc_1_15FF
loc_1_15C3:
        db  0C7h, 046h, 028h, 063h, 047h, 000h, 000h
        db  0C7h, 046h, 02Ch, 087h, 01Fh, 000h, 000h
        jmp    SHORT loc_1_15F8
loc_1_15D3:
        db  0C7h, 046h, 028h, 0C2h, 047h, 000h, 000h
        db  0C7h, 046h, 02Ch, 060h, 01Eh, 000h, 000h
        mov    dword ptr [esi + 14h], 3
        jmp    SHORT loc_1_15FF
loc_1_15EA:
        db  0C7h, 046h, 028h, 012h, 047h, 000h, 000h
        db  0C7h, 046h, 02Ch, 031h, 01Fh, 000h, 000h
loc_1_15F8:
        mov    dword ptr [esi + 14h], 0
loc_1_15FF:
        mov    eax, dword ptr [edi + 1ch]
        sub    eax, dword ptr [esi + 14h]
        mov    dword ptr [esi + 0ch], eax
        mov    eax, dword ptr [edi + 1ch]
        add    dword ptr [esi + 10h], eax
        db  083h, 00Dh, 0F8h, 058h, 000h, 000h, 001h
        db  081h, 025h, 0F8h, 058h, 000h, 000h, 0FFh, 0F7h, 0FFh, 0FFh
        db  0F6h, 005h, 0F8h, 058h, 000h, 000h, 010h
        je     SHORT loc_1_1637
        cmp    dword ptr [esi + 18h], 0
        je     SHORT loc_1_1637
        push   esi
        call   sub_1_7221
        add    esp, 4
loc_1_1637:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_163C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 1ch]
        mov    eax, dword ptr [esi + 48h]
        cmp    dword ptr [eax + 18h], 0
        je     SHORT loc_1_1655
        jmp    NEAR PTR loc_1_17C2
loc_1_1655:
        db  066h, 083h, 03Dh, 0F4h, 07Ch, 000h, 000h, 000h
        jne    SHORT loc_1_1683
        push   1
        push   0
        db  068h, 0DFh, 024h, 000h, 000h
        db  068h, 0F4h, 07Ch, 000h, 000h
        call   sub_1_5206
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_1683
        mov    eax, 1
        jmp    NEAR PTR loc_1_17C4
loc_1_1683:
        push   -1
        push   0
        push   1ech
        db  068h, 0F0h, 07Ch, 000h, 000h
        call   sub_1_2468
        db  083h, 03Dh, 0F0h, 07Ch, 000h, 000h, 000h
        jne    SHORT loc_1_16A9
        mov    eax, 1
        jmp    NEAR PTR loc_1_17C4
loc_1_16A9:
        mov    eax, dword ptr [esi + 48h]
        mov    byte ptr [eax + 2fh], 0
        mov    eax, dword ptr [esi + 48h]
        mov    dword ptr [eax + 30h], 0
        db  083h, 025h, 0F8h, 058h, 000h, 000h, 0FEh
        db  08Bh, 01Dh, 0F0h, 07Ch, 000h, 000h
        db  081h, 00Dh, 0F8h, 058h, 000h, 000h, 000h, 008h, 000h, 000h
        lea    ebp, [ebx + 0fch]
        db  0A1h, 0ECh, 058h, 000h, 000h
        mov    dword ptr [esp + 14h], eax
        db  089h, 035h, 0ECh, 058h, 000h, 000h
        mov    dword ptr [esp + 10h], 0
loc_1_16EE:
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 0ah
        rep stosd
        stosw
        mov    dword ptr [ebx + 14h], esi
        db  066h, 0A1h, 0F4h, 07Ch, 000h, 000h
        mov    word ptr [ebx + 0ah], ax
        lea    edi, [ebx + 1ch]
        mov    eax, 0ffffffffh
        stosd
        stosw
        mov    word ptr [ebx + 22h], 1
        mov    dword ptr [ebx + 24h], ebp
        lea    edi, [ebp + 0ah]
        mov    word ptr [ebx + 28h], 28h
        mov    byte ptr [ebp + 5], 1
        mov    dword ptr [ebp + 6], 0
        stosd
        mov    ecx, 3
        stosw
        lea    eax, [ebp + 1eh]
        mov    word ptr [ebp + 10h], 5304h
        mov    word ptr [eax], 100h
        mov    dword ptr [eax + 2], 0ffffffffh
        mov    eax, dword ptr [esp + 10h]
        cdq
        idiv   ecx
        or     edx, edx
        je     SHORT loc_1_1764
        cmp    edx, 1
        je     SHORT loc_1_176D
        cmp    edx, 2
        je     SHORT loc_1_1776
        jmp    SHORT loc_1_177D
loc_1_1764:
        db  0C7h, 046h, 028h, 063h, 047h, 000h, 000h
        jmp    SHORT loc_1_177D
loc_1_176D:
        db  0C7h, 046h, 028h, 0C2h, 047h, 000h, 000h
        jmp    SHORT loc_1_177D
loc_1_1776:
        db  0C7h, 046h, 028h, 012h, 047h, 000h, 000h
loc_1_177D:
        mov    edi, dword ptr [esi + 18h]
        mov    dword ptr [esi + 18h], 0
        push   esi
        push   ebx
        call   sub_1_4092
        add    esp, 8
        mov    dword ptr [esi + 18h], edi
        add    ebx, 2ah
        add    ebp, 28h
        inc    dword ptr [esp + 10h]
        cmp    dword ptr [esp + 10h], 6
        jl     NEAR PTR loc_1_16EE
        mov    eax, dword ptr [esp + 14h]
        db  0A3h, 0ECh, 058h, 000h, 000h
        db  0C7h, 046h, 028h, 0C2h, 047h, 000h, 000h
        push   esi
        call   sub_1_40AC
        add    esp, 4
loc_1_17C2:
        sub    eax, eax
loc_1_17C4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret
loc_1_17CC:
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 2ch]
        mov    dword ptr [esi], 0c0000001h
        db  0A1h, 038h, 058h, 000h, 000h
        db  039h, 005h, 0C8h, 058h, 000h, 000h
        jb     SHORT loc_1_17F5
        mov    dword ptr [esi], 0c000009ah
        jmp    NEAR PTR loc_1_1ACD
loc_1_17F5:
        lea    eax, [esp + 10h]
        push   dword ptr [esp + 38h]
        push   eax
        push   esi
        call   sub_1_24C8
        cmp    dword ptr [esi], 0
        je     SHORT loc_1_1814
        mov    dword ptr [esi], 0c0010015h
        jmp    NEAR PTR loc_1_1ACD
loc_1_1814:
        lea    eax, [esp + 18h]
        push   eax
        push   dword ptr [esp + 14h]
        call   sub_1_12BC
        add    esp, 8
        mov    edi, eax
        push   dword ptr [esp + 10h]
        call   sub_1_24A4
        cmp    edi, 1
        jne    SHORT loc_1_1840
        mov    dword ptr [esi], 0c0010015h
        jmp    NEAR PTR loc_1_1ACD
loc_1_1840:
        sub    edi, edi
        db  039h, 03Dh, 034h, 058h, 000h, 000h
        jbe    SHORT loc_1_187B
        db  0BDh, 0CCh, 058h, 000h, 000h
        mov    ebx, dword ptr [esp + 34h]
loc_1_1853:
        mov    eax, dword ptr [ebp]
        test   byte ptr [eax + 4], 1
        je     SHORT loc_1_186D
        push   ebx
        add    eax, 34h
        push   eax
        call   sub_1_1287
        add    esp, 8
        cmp    al, 1
        je     SHORT loc_1_187F
loc_1_186D:
        add    ebp, 4
        inc    edi
        db  039h, 03Dh, 034h, 058h, 000h, 000h
        ja     SHORT loc_1_1853
        jmp    SHORT loc_1_187F
loc_1_187B:
        mov    ebx, dword ptr [esp + 34h]
loc_1_187F:
        db  039h, 03Dh, 034h, 058h, 000h, 000h
        je     SHORT loc_1_1898
        db  08Bh, 004h, 0BDh, 0CCh, 058h, 000h, 000h
        test   byte ptr [eax + 4], 1
        jne    NEAR PTR loc_1_192E
loc_1_1898:
        sub    edi, edi
        db  039h, 03Dh, 034h, 058h, 000h, 000h
        jbe    SHORT loc_1_18C5
        db  0B8h, 0CCh, 058h, 000h, 000h
        mov    ecx, 1
loc_1_18AC:
        mov    edx, dword ptr [eax]
        test   dword ptr [edx + 4], ecx
        je     SHORT loc_1_18BF
        add    eax, 4
        inc    edi
        db  039h, 03Dh, 034h, 058h, 000h, 000h
        ja     SHORT loc_1_18AC
loc_1_18BF:
        db  039h, 03Dh, 034h, 058h, 000h, 000h
loc_1_18C5:
        jne    SHORT loc_1_18D2
        mov    dword ptr [esi], 0c000009ah
        jmp    NEAR PTR loc_1_1ACD
loc_1_18D2:
        db  08Dh, 02Ch, 0BDh, 0CCh, 058h, 000h, 000h
        push   ebx
        mov    eax, dword ptr [ebp]
        add    eax, 34h
        push   eax
        call   sub_1_11D0
        add    esp, 8
        or     al, al
        jne    SHORT loc_1_18F8
        mov    dword ptr [esi], 0c000009ah
        jmp    NEAR PTR loc_1_1ACD
loc_1_18F8:
        db  0FFh, 035h, 0C4h, 058h, 000h, 000h
        push   dword ptr [ebp]
        push   ebx
        db  0E8h, 0A0h, 0E9h, 0FEh, 0FFh
        add    esp, 0ch
        or     eax, eax
        je     SHORT loc_1_1928
        mov    eax, dword ptr [ebp]
        add    eax, 34h
        push   eax
        call   sub_1_123A
        add    esp, 4
        mov    dword ptr [esi], 0c0010007h
        jmp    NEAR PTR loc_1_1ACD
loc_1_1928:
        db  0FFh, 005h, 0C4h, 058h, 000h, 000h
loc_1_192E:
        sub    ecx, ecx
        db  039h, 00Dh, 038h, 058h, 000h, 000h
        jbe    SHORT loc_1_1955
        db  0B8h, 0DCh, 058h, 000h, 000h
        mov    edx, 1
loc_1_1942:
        mov    ebx, dword ptr [eax]
        test   dword ptr [ebx + 8], edx
        je     SHORT loc_1_1955
        add    eax, 4
        inc    ecx
        db  039h, 00Dh, 038h, 058h, 000h, 000h
        ja     SHORT loc_1_1942
loc_1_1955:
        db  08Dh, 01Ch, 08Dh, 0DCh, 058h, 000h, 000h
        mov    eax, dword ptr [ebx]
        mov    ebp, dword ptr [esp + 30h]
        mov    dword ptr [eax + 30h], ebp
        mov    edx, dword ptr [ebx]
        db  08Dh, 03Ch, 0BDh, 0CCh, 058h, 000h, 000h
        mov    eax, dword ptr [esp + 3ch]
        mov    dword ptr [edx + 34h], eax
        db  0FFh, 005h, 0C8h, 058h, 000h, 000h
        push   ecx
        push   dword ptr [edi]
        lea    eax, [esp + 20h]
        push   dword ptr [ebx]
        push   eax
        db  0E8h, 0F5h, 0EDh, 0FEh, 0FFh
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_199F
        mov    eax, dword ptr [ebx]
        and    dword ptr [eax + 8], 0fffffffeh
        mov    dword ptr [esi], 0c0010015h
        jmp    SHORT loc_1_19E9
loc_1_199F:
        db  066h, 083h, 03Dh, 068h, 07Ch, 000h, 000h, 000h
        je     NEAR PTR loc_1_1A4D
        db  0F6h, 005h, 0F9h, 058h, 000h, 000h, 002h
        jne    SHORT loc_1_19BF
        db  083h, 03Dh, 0C8h, 058h, 000h, 000h, 001h
        jbe    SHORT loc_1_1A3D
loc_1_19BF:
        db  0FFh, 035h, 0E8h, 055h, 000h, 000h
        db  0FFh, 035h, 0E4h, 055h, 000h, 000h
        db  0FFh, 035h, 0E0h, 055h, 000h, 000h
        db  0FFh, 035h, 0DCh, 055h, 000h, 000h
        call   sub_1_6F66
        add    esp, 10h
        or     eax, eax
        je     SHORT loc_1_1A2A
        mov    dword ptr [esi], 0c0000001h
loc_1_19E9:
        db  0FFh, 00Dh, 0C8h, 058h, 000h, 000h
        mov    eax, dword ptr [edi]
        push   ebp
        or     dword ptr [eax + 4], 1000000h
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx + 14h], eax
        lea    eax, [esp + 18h]
        push   dword ptr [edi]
        push   eax
        call   sub_1_1346
        cmp    dword ptr [esp + 14h], 103h
        jne    SHORT loc_1_1A1C
        mov    dword ptr [esi], 103h
loc_1_1A1C:
        mov    eax, dword ptr [edi]
        and    dword ptr [eax + 4], 0feffffffh
        jmp    NEAR PTR loc_1_1ACD
loc_1_1A2A:
        db  083h, 03Dh, 0ECh, 055h, 000h, 000h, 000h
        je     SHORT loc_1_1A3D
        db  081h, 00Dh, 0F8h, 058h, 000h, 000h, 000h, 004h, 000h, 000h
loc_1_1A3D:
        call   sub_1_60B0
        db  066h, 0C7h, 005h, 068h, 07Ch, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_1AC7
loc_1_1A4D:
        db  083h, 03Dh, 0C8h, 058h, 000h, 000h, 001h
        jbe    SHORT loc_1_1AC7
        db  0F6h, 005h, 0F8h, 058h, 000h, 000h, 010h
        jne    SHORT loc_1_1A94
        db  0FFh, 035h, 0E8h, 055h, 000h, 000h
        db  0FFh, 035h, 0E4h, 055h, 000h, 000h
        db  0FFh, 035h, 0E0h, 055h, 000h, 000h
        db  0FFh, 035h, 0DCh, 055h, 000h, 000h
        call   sub_1_6F66
        add    esp, 10h
        db  083h, 03Dh, 0ECh, 055h, 000h, 000h, 000h
        je     SHORT loc_1_1AB2
        db  081h, 00Dh, 0F8h, 058h, 000h, 000h, 000h, 004h, 000h, 000h
        jmp    SHORT loc_1_1AB2
loc_1_1A94:
        push   5204h
        push   dword ptr [ebx]
        call   sub_1_6CEF
        add    esp, 8
        push   5304h
        push   dword ptr [ebx]
        call   sub_1_6CEF
        add    esp, 8
loc_1_1AB2:
        db  0A1h, 0ECh, 058h, 000h, 000h
        mov    eax, dword ptr [eax + 8]
        test   al, 8
        je     SHORT loc_1_1AC7
        test   al, 10h
        je     SHORT loc_1_1AC7
        call   sub_1_1B15
loc_1_1AC7:
        mov    dword ptr [esi], 0
loc_1_1ACD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 18h
        ret    14h
loc_1_1AD7:
        jmp    NEAR PTR loc_1_4CB6
sub_1_1ADC:
        db  0A1h, 0F0h, 058h, 000h, 000h
        imul   eax, eax, 41c64e6dh
        add    eax, 3039h
        db  0A3h, 0F0h, 058h, 000h, 000h
        je     SHORT loc_1_1AF8
        cmp    eax, -1
        jne    SHORT loc_1_1AFD
loc_1_1AF8:
        mov    eax, 1
loc_1_1AFD:
        db  0A3h, 0F0h, 058h, 000h, 000h
        push   eax
        call   sub_1_2D02
        add    esp, 4
        or     eax, eax
        jne    SHORT sub_1_1ADC
        db  0A1h, 0F0h, 058h, 000h, 000h
        ret
sub_1_1B15:
        push   esi
        push   edi
        db  08Bh, 00Dh, 0DCh, 058h, 000h, 000h
        sub    edx, edx
        db  039h, 015h, 0C8h, 058h, 000h, 000h
        jbe    SHORT loc_1_1B4D
        db  0B8h, 0DCh, 058h, 000h, 000h
        mov    esi, 8
loc_1_1B31:
        mov    edi, dword ptr [eax]
        test   dword ptr [edi + 8], esi
        je     SHORT loc_1_1B46
        add    eax, 4
        inc    edx
        db  039h, 015h, 0C8h, 058h, 000h, 000h
        ja     SHORT loc_1_1B31
        jmp    SHORT loc_1_1B4D
loc_1_1B46:
        db  08Bh, 00Ch, 095h, 0DCh, 058h, 000h, 000h
loc_1_1B4D:
        db  039h, 00Dh, 0ECh, 058h, 000h, 000h
        je     SHORT loc_1_1B64
        db  089h, 00Dh, 0ECh, 058h, 000h, 000h
        push   ecx
        call   sub_1_2FF9
        add    esp, 4
loc_1_1B64:
        pop    edi
        pop    esi
        ret
        db  000h
loc_1_1B68:
        ret    8
loc_1_1B6B:
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 020h, 009h, 000h, 000h
        jne    SHORT loc_1_1B94
        mov    eax, dword ptr [esp + 4]
        cld
        sub    esp, 14h
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        call   sub_1_1C36
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        add    esp, 14h
loc_1_1B91:
        ret    1ch
loc_1_1B94:
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 000h, 008h, 000h, 000h
        je     SHORT loc_1_1B91
        mov    eax, dword ptr [esp + 4]
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ecx
        cmp    dword ptr [ebp + 14h], 0eh
        jb     SHORT loc_1_1BD9
        mov    esi, dword ptr [ebp + 10h]
        movzx  ecx, word ptr [esi + 0ch]
        cmp    ecx, 3781h
        je     SHORT loc_1_1BF0
        xchg   cl, ch
        cmp    ecx, 600h
        jae    SHORT loc_1_1BD9
        mov    esi, dword ptr [ebp + 18h]
        cmp    word ptr [esi], -1
        je     SHORT loc_1_1BE9
        cmp    word ptr [esi], 0e0e0h
        je     SHORT loc_1_1BDF
loc_1_1BD9:
        pop    ecx
        pop    edi
        pop    esi
        pop    ebp
        jmp    SHORT loc_1_1B91
loc_1_1BDF:
        mov    eax, 1
        add    esi, 3
        jmp    SHORT loc_1_1BF8
loc_1_1BE9:
        mov    eax, 0
        jmp    SHORT loc_1_1BF8
loc_1_1BF0:
        mov    eax, 2
        mov    esi, dword ptr [ebp + 18h]
loc_1_1BF8:
        cmp    word ptr [esi], -1
        jne    SHORT loc_1_1BD9
        movzx  ecx, word ptr [esi + 2]
        xchg   cl, ch
        cmp    ecx, 28h
        jb     SHORT loc_1_1BD9
        cmp    word ptr [esi + 1ch], 5304h
        jne    SHORT loc_1_1BD9
        cmp    word ptr [esi + 1eh], 200h
        jne    SHORT loc_1_1BD9
        mov    ecx, esi
        mov    esi, dword ptr [ebp + 8]
        xchg   ecx, eax
        push   ecx
        mov    edi, dword ptr [esi + 3ch]
        call   sub_1_211D
        pop    ecx
        cmp    byte ptr [ecx + esi + 30h], 0ffh
        je     SHORT loc_1_1BD9
        inc    byte ptr [ecx + esi + 30h]
        jmp    SHORT loc_1_1BD9
sub_1_1C36:
        mov    ebx, eax
        mov    edi, dword ptr [ebx + 3ch]
        or     edi, edi
        je     SHORT loc_1_1C4F
loc_1_1C3F:
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebp + 14h], eax
        jmp    dword ptr [edi + 2ch]
loc_1_1C48:
        mov    edi, dword ptr [edi + 4ch]
        or     edi, edi
        jne    SHORT loc_1_1C3F
loc_1_1C4F:
        mov    eax, 10001h
        ret
loc_1_1C55:
        call   sub_1_0F2E
        db  0C7h, 005h, 070h, 056h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
sub_1_1C65:
        push   esi
        db  08Bh, 035h, 070h, 056h, 000h, 000h
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        db  0C7h, 005h, 070h, 056h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    esi
sub_1_1C7D:
        push   ebx
        db  0CDh, 020h, 000h, 000h, 007h, 000h
        cmp    bl, 2
        jne    SHORT loc_1_1CB0
        db  083h, 03Dh, 070h, 056h, 000h, 000h, 000h
        jne    SHORT loc_1_1CB5
        push   esi
        db  0BEh, 055h, 01Ch, 000h, 000h
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 2
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        db  089h, 035h, 070h, 056h, 000h, 000h
        pop    esi
        pop    ebx
        ret
loc_1_1CB0:
        call   sub_1_0F2E
loc_1_1CB5:
        pop    ebx
        ret
loc_1_1CB7:
        call   sub_1_1050
        db  0C7h, 005h, 074h, 056h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
sub_1_1CC7:
        push   esi
        db  08Bh, 035h, 074h, 056h, 000h, 000h
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        db  0C7h, 005h, 074h, 056h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    esi
sub_1_1CDF:
        push   ebx
        db  0CDh, 020h, 000h, 000h, 007h, 000h
        cmp    bl, 2
        jne    SHORT loc_1_1D12
        db  083h, 03Dh, 074h, 056h, 000h, 000h, 000h
        jne    SHORT loc_1_1D17
        push   esi
        db  0BEh, 0B7h, 01Ch, 000h, 000h
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 2
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        db  089h, 035h, 074h, 056h, 000h, 000h
        pop    esi
        pop    ebx
        ret
loc_1_1D12:
        call   sub_1_1050
loc_1_1D17:
        pop    ebx
        ret
loc_1_1D19:
        push   eax
        push   edx
        db  083h, 03Dh, 048h, 058h, 000h, 000h, 003h
        jl     NEAR PTR loc_1_1DD7
loc_1_1D28:
        db  083h, 03Dh, 04Ch, 058h, 000h, 000h, 014h
        jl     NEAR PTR loc_1_1DEE
loc_1_1D35:
        push   esi
        push   edi
        mov    ebx, edi
        push   dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        and    cl, byte ptr [edi + 22h]
        push   ecx
        mov    esi, esp
        mov    ebx, edi
        mov    edi, dword ptr [ebp + 2ch]
        mov    ecx, dword ptr [ebp + 30h]
        mov    edx, dword ptr [eax + 34h]
        push   dword ptr [eax + 30h]
        movzx  eax, word ptr [eax + 14h]
        db  0FFh, 00Dh, 048h, 058h, 000h, 000h
        db  083h, 02Dh, 04Ch, 058h, 000h, 000h, 014h
        call   dword ptr [esp]
        db  0FFh, 005h, 048h, 058h, 000h, 000h
        db  083h, 005h, 04Ch, 058h, 000h, 000h, 014h
        add    esp, 0ch
        mov    ecx, esi
        pop    edi
        pop    esi
        jecxz  loc_1_1DD4
        cmp    word ptr [ecx + 22h], 14h
        ja     SHORT loc_1_1DD4
        mov    dword ptr [ecx + 0ch], 0
        db  0C7h, 041h, 018h, 023h, 049h, 000h, 000h
        mov    byte ptr [ecx + 9], 0
        push   ecx
        call   sub_1_4488
        cmp    ebx, -1
        jne    SHORT loc_1_1DA6
        xor    ebx, ebx
        or     byte ptr [eax + 3ch], 8
loc_1_1DA6:
        pop    ecx
        pop    edx
        pop    eax
        sub    dword ptr [ebp + 34h], ebx
        jb     SHORT loc_1_1DC0
        sub    dword ptr [ebp + 30h], ebx
        jb     SHORT loc_1_1DCB
        add    dword ptr [ebp + 2ch], ebx
loc_1_1DB6:
        add    dword ptr [ebp + 14h], ebx
        sub    edx, ebx
        jmp    NEAR PTR loc_1_2045
loc_1_1DC0:
        mov    byte ptr [ecx + 9], 1
        push   dword ptr [ecx]
        jmp    NEAR PTR loc_1_2095
loc_1_1DCB:
        mov    dword ptr [ebp + 30h], 0
        jmp    SHORT loc_1_1DB6
loc_1_1DD4:
        pop    edx
        pop    eax
        ret
loc_1_1DD7:
        push   eax
        push   edx
        call   sub_1_1C7D
        pop    edx
        pop    eax
        db  083h, 03Dh, 048h, 058h, 000h, 000h, 001h
        jl     SHORT loc_1_1E0D
        jmp    NEAR PTR loc_1_1D28
loc_1_1DEE:
        push   eax
        push   edx
        call   sub_1_1CDF
        pop    edx
        pop    eax
        db  083h, 03Dh, 04Ch, 058h, 000h, 000h, 014h
        jl     SHORT loc_1_1E0D
        db  083h, 03Dh, 048h, 058h, 000h, 000h, 001h
        jge    NEAR PTR loc_1_1D35
loc_1_1E0D:
        jmp    SHORT loc_1_1DD4
        db  083h, 07Dh, 028h, 00Eh, 072h, 04Ah, 08Bh, 075h, 024h, 00Fh, 0B7h, 046h, 00Ch, 086h, 0C4h, 03Dh
        db  000h, 006h, 000h, 000h, 00Fh, 083h, 01Fh, 0FEh, 0FFh, 0FFh, 08Bh, 045h, 02Ch, 066h, 081h, 038h
        db  0AAh, 0AAh, 00Fh, 085h, 011h, 0FEh, 0FFh, 0FFh, 083h, 0C0h, 003h, 066h, 081h, 078h, 003h, 081h
        db  037h, 00Fh, 085h, 002h, 0FEh, 0FFh, 0FFh, 089h, 075h, 00Ch, 083h, 0C6h, 006h, 0BAh, 008h, 000h
        db  000h, 000h, 001h, 055h, 02Ch, 029h, 055h, 030h, 029h, 055h, 034h, 0E9h, 05Bh, 001h, 000h, 000h
        db  0C3h
loc_1_1E60:
        cmp    dword ptr [ebp + 28h], 0eh
        jb     SHORT loc_1_1EA1
        mov    esi, dword ptr [ebp + 24h]
        movzx  eax, word ptr [esi + 0ch]
        xchg   ah, al
        cmp    eax, 600h
        jae    NEAR PTR loc_1_1C48
        mov    eax, dword ptr [ebp + 2ch]
        cmp    word ptr [eax], 0e0e0h
        jne    NEAR PTR loc_1_1C48
        mov    dword ptr [ebp + 0ch], esi
        add    esi, 6
        mov    edx, 3
        add    dword ptr [ebp + 2ch], edx
        sub    dword ptr [ebp + 30h], edx
        sub    dword ptr [ebp + 34h], edx
        jmp    NEAR PTR sub_1_1FBA
loc_1_1EA1:
        ret
loc_1_1EA2:
        cmp    dword ptr [ebp + 28h], 0eh
        jb     NEAR PTR loc_1_1F81
        mov    eax, dword ptr [ebp + 2ch]
        cmp    word ptr [eax], 0aaaah
        jne    NEAR PTR loc_1_1F81
        cmp    word ptr [eax + 6], 3781h
        jne    NEAR PTR loc_1_1F81
        call   sub_1_1F0E
        mov    edx, 8
        add    dword ptr [ebp + 2ch], edx
        sub    dword ptr [ebp + 30h], edx
        sub    dword ptr [ebp + 34h], edx
        jmp    NEAR PTR sub_1_1FBA
loc_1_1EDE:
        cmp    dword ptr [ebp + 28h], 0eh
        jb     NEAR PTR loc_1_1F81
        mov    eax, dword ptr [ebp + 2ch]
        cmp    word ptr [eax], 0e0e0h
        jne    NEAR PTR loc_1_1F81
        call   sub_1_1F0E
        mov    edx, 3
        add    dword ptr [ebp + 2ch], edx
        sub    dword ptr [ebp + 30h], edx
        sub    dword ptr [ebp + 34h], edx
        jmp    NEAR PTR sub_1_1FBA
sub_1_1F0E:
        mov    esi, dword ptr [ebp + 24h]
        add    esi, 2
        mov    dword ptr [ebp + 0ch], esi
        add    esi, 6
        test   byte ptr [esi - 6], 80h
        jne    SHORT loc_1_1F30
        test   dword ptr [edi + 8], 2
        je     SHORT loc_1_1F30
        push   eax
        call   sub_1_2292
        pop    eax
loc_1_1F30:
        ret
loc_1_1F31:
        cmp    dword ptr [ebp + 28h], 0eh
        jne    NEAR PTR loc_1_1C48
        mov    esi, dword ptr [ebp + 24h]
        mov    ax, word ptr [esi + 0ch]
        cmp    ax, 3781h
        jne    NEAR PTR loc_1_1C48
        mov    dword ptr [ebp + 0ch], esi
        add    esi, 6
        jmp    SHORT sub_1_1FBA
loc_1_1F54:
        cmp    dword ptr [ebp + 28h], 3
        jne    SHORT loc_1_1F81
        mov    eax, dword ptr [ebp + 24h]
        cmp    byte ptr [eax + 2], 0fah
        lea    esi, [ebp + 4]
        xor    edx, edx
        mov    dword ptr [esi], edx
        mov    byte ptr [esi + 4], dl
        mov    cl, byte ptr [eax]
        mov    byte ptr [esi + 5], cl
        push   edx
        mov    dh, byte ptr [eax + 1]
        push   edx
        mov    dword ptr [ebp + 0ch], esp
        call   sub_1_1FBA
        add    esp, 8
        ret
loc_1_1F81:
        mov    eax, 0
        ret
loc_1_1F87:
        cmp    dword ptr [ebp + 28h], 0eh
        jne    SHORT loc_1_1F81
        mov    esi, dword ptr [ebp + 24h]
        movzx  eax, word ptr [esi + 0ch]
        xchg   ah, al
        cmp    eax, 600h
        jae    NEAR PTR loc_1_1C48
        cmp    ax, 1eh
        jb     SHORT loc_1_1F81
        mov    eax, dword ptr [ebp + 2ch]
        cmp    word ptr [eax], -1
        jne    NEAR PTR loc_1_1C48
        mov    dword ptr [ebp + 0ch], esi
        add    esi, 6
sub_1_1FBA:
        mov    eax, dword ptr [ebp + 2ch]
        movzx  edx, word ptr [eax + 2]
        xchg   dh, dl
        cmp    dword ptr [ebp + 34h], edx
        jb     SHORT loc_1_1F81
        db  0FFh, 005h, 022h, 059h, 000h, 000h
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 010h, 000h, 000h, 000h
        je     SHORT sub_1_1FFC
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT sub_1_1FFC
        cmp    byte ptr [eax + 5], 14h
        je     NEAR PTR loc_1_2809
        mov    ecx, dword ptr [edi + 18h]
        cmp    dword ptr [eax + 6], ecx
        jne    NEAR PTR loc_1_26E0
sub_1_1FFC:
        cmp    word ptr [eax + 1ch], 5304h
        je     NEAR PTR loc_1_2113
loc_1_2008:
        push   dword ptr [eax + 10h]
        call   sub_1_42A4
        pop    ecx
        jb     NEAR PTR loc_1_1F81
        test   dword ptr [eax + 18h], 8
        jne    NEAR PTR loc_1_1D19
        mov    ecx, dword ptr [eax + 20h]
        or     ecx, ecx
        je     NEAR PTR loc_1_20B9
        mov    ebx, dword ptr [ecx + 10h]
        mov    dword ptr [eax + 20h], ebx
        or     ebx, ebx
        jne    SHORT loc_1_2045
        db  08Bh, 01Dh, 064h, 058h, 000h, 000h
        add    ebx, dword ptr [eax + 8]
        and    byte ptr [ebx], 0feh
loc_1_2045:
        lodsd
        and    al, byte ptr [edi + 22h]
        mov    dword ptr [ecx + 1ch], eax
        lodsw
        mov    word ptr [ecx + 20h], ax
        or     dword ptr [ecx + 0ch], 2000h
        mov    ebx, dword ptr [ecx]
        mov    dword ptr [ebx + 40h], edi
        cmp    dword ptr [ebx + 4], edx
        jb     SHORT loc_1_20D7
loc_1_2063:
        push   ebx
        or     edx, edx
        je     SHORT loc_1_2095
        cmp    dword ptr [ebp + 30h], edx
        jb     SHORT loc_1_20E0
        mov    ebx, dword ptr [ebx + 8]
        mov    esi, dword ptr [ebp + 2ch]
loc_1_2073:
        mov    ecx, dword ptr [ebx + 0ch]
        sub    ecx, edx
        sbb    eax, eax
        and    ecx, eax
        add    ecx, edx
        mov    edi, dword ptr [ebx + 4]
        mov    eax, ecx
        shr    ecx, 2
        rep movsd
        mov    cl, al
        and    cl, 3
        rep movsb
        mov    ebx, dword ptr [ebx]
        sub    edx, eax
        jne    SHORT loc_1_2073
loc_1_2095:
        pop    edx
        test   byte ptr [edx + 3ch], 8
        jne    SHORT loc_1_20B3
        add    edx, 58h
        mov    ebx, edx
        db  087h, 01Dh, 08Ch, 058h, 000h, 000h
        mov    dword ptr [ebx], edx
        mov    dword ptr [edx + 4], ebx
        db  0C7h, 002h, 088h, 058h, 000h, 000h
loc_1_20B2:
        ret
loc_1_20B3:
        call   sub_1_4619
        ret
loc_1_20B9:
        test   dword ptr [eax + 18h], 2
        je     SHORT loc_1_20B2
        mov    ebx, dword ptr [ebp + 2ch]
        call   sub_1_3A69
        jae    NEAR PTR loc_1_2045
        db  0FFh, 005h, 026h, 059h, 000h, 000h
        ret
loc_1_20D7:
        mov    byte ptr [ecx + 9], 0fdh
        mov    edx, dword ptr [ebx + 4]
        jmp    SHORT loc_1_2063
loc_1_20E0:
        lea    ecx, [ebp + 10h]
        push   ecx
        push   ebx
        push   edx
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 20h]
        mov    eax, dword ptr [edi + 48h]
        push   dword ptr [eax + 0ch]
        lea    eax, [ebp + 0ch]
        push   eax
        db  0CDh, 020h, 02Eh, 000h, 028h, 000h
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_2095
        int3
        cmp    dword ptr [ebp + 0ch], 103h
        je     SHORT loc_1_20B2
        pop    edx
        call   sub_1_2236
        ret
loc_1_2113:
        call   sub_1_211D
        jmp    NEAR PTR loc_1_2008
sub_1_211D:
        cmp    word ptr [eax + 1eh], 200h
        jne    SHORT loc_1_217A
        cmp    dword ptr [eax + 6], 0
        je     SHORT loc_1_217A
        mov    ecx, dword ptr [eax + 6]
        cmp    dword ptr [edi + 18h], ecx
        jne    SHORT loc_1_213E
        cmp    byte ptr [edi + 23h], 0ah
        jb     SHORT loc_1_213A
        ret
loc_1_213A:
        inc    byte ptr [edi + 23h]
        ret
loc_1_213E:
        dec    byte ptr [edi + 23h]
        js     SHORT loc_1_2144
        ret
loc_1_2144:
        mov    dword ptr [edi + 18h], ecx
        mov    byte ptr [edi + 23h], 1
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 000h, 008h, 000h, 000h
        jne    SHORT loc_1_217A
        push   edx
        push   eax
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 010h, 000h, 000h, 000h
        je     SHORT loc_1_2171
        push   edi
        call   sub_1_6CBB
        call   sub_1_7221
        pop    edi
loc_1_2171:
        push   edi
        call   sub_1_2FF9
        pop    edi
        pop    eax
        pop    edx
loc_1_217A:
        ret
loc_1_217B:
        db  083h, 03Dh, 090h, 058h, 000h, 000h, 000h
        je     SHORT loc_1_2189
        call   sub_1_3D82
loc_1_2189:
        call   sub_1_2196
        call   sub_1_3D82
        ret    4
sub_1_2196:
        db  0B8h, 088h, 058h, 000h, 000h
        cmp    dword ptr [eax], eax
        je     SHORT loc_1_21B8
        push   ebx
        mov    ebx, eax
loc_1_21A2:
        mov    edx, dword ptr [ebx]
        mov    eax, dword ptr [edx]
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [ebx], eax
        sub    edx, 58h
        call   sub_1_4619
        cmp    dword ptr [ebx], ebx
        jne    SHORT loc_1_21A2
        pop    ebx
loc_1_21B8:
        ret
sub_1_21B9:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        test   byte ptr [edx + 3ch], 0d0h
        jne    SHORT loc_1_21DA
        and    byte ptr [edx + 3ch], 0fbh
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_1_2210
loc_1_21D2:
        call   sub_1_4619
        ret    0ch
loc_1_21DA:
        test   byte ptr [edx + 3ch], 40h
        jne    SHORT loc_1_21F2
loc_1_21E0:
        and    byte ptr [edx + 3ch], 0fbh
        test   byte ptr [edx + 3ch], 10h
        je     SHORT loc_1_21EF
        call   sub_1_27D4
loc_1_21EF:
        ret    0ch
loc_1_21F2:
        and    byte ptr [edx + 3ch], 0bfh
        push   0
        mov    ecx, esp
        push   edx
        push   dword ptr [eax + 0ch]
        push   ecx
        db  0CDh, 020h, 02Dh, 000h, 028h, 000h
        pop    eax
        cmp    eax, 103h
        jne    SHORT loc_1_21E0
        ret    0ch
loc_1_2210:
        mov    ecx, dword ptr [edx + 44h]
        jecxz  loc_1_21D2
        mov    byte ptr [ecx + 9], 0ffh
        jmp    SHORT loc_1_21D2
loc_1_221B:
        mov    edx, dword ptr [esp + 8]
        cmp    dword ptr [esp + 0ch], 0
        jne    SHORT loc_1_222E
        call   sub_1_4619
        ret    10h
loc_1_222E:
        call   sub_1_2236
        ret    10h
sub_1_2236:
        mov    edx, dword ptr [edx + 44h]
        test   dword ptr [edx + 0ch], 4000h
        jne    SHORT loc_1_227D
        push   dword ptr [edx + 0ah]
        call   sub_1_42A4
        pop    ecx
        jb     SHORT loc_1_227D
        test   dword ptr [eax + 18h], 8
        jne    SHORT loc_1_2272
        mov    ecx, edx
        test   dword ptr [edx + 0ch], 80h
        jne    SHORT loc_1_2268
        xchg   dword ptr [eax + 20h], ecx
        mov    dword ptr [edx + 10h], ecx
        ret
loc_1_2268:
        xchg   dword ptr [eax + 28h], ecx
        mov    dword ptr [edx + 10h], ecx
        inc    dword ptr [eax + 24h]
        ret
loc_1_2272:
        mov    byte ptr [edx + 9], 0ffh
        mov    edx, dword ptr [edx]
        jmp    NEAR PTR sub_1_4619
loc_1_227D:
        mov    word ptr [edx + 8], 0fc00h
        push   esi
        push   ebx
        mov    ebx, dword ptr [edx]
        jmp    NEAR PTR loc_1_4677
        db  0C2h, 010h, 000h
loc_1_228F:
        ret    4
sub_1_2292:
        mov    ebx, edi
        call   sub_1_232F
        jb     SHORT loc_1_22A7
        mov    edi, ebx
        ret
loc_1_229E:
        mov    dword ptr [edi], 0
        mov    edi, ebx
        ret
loc_1_22A7:
        mov    ecx, dword ptr [ebx + 38h]
        db  039h, 00Dh, 078h, 058h, 000h, 000h
        je     SHORT loc_1_230E
        inc    dword ptr [ebx + 38h]
loc_1_22B5:
        mov    edi, dword ptr [ebx + 3ch]
        mov    eax, dword ptr [esi + 2]
        mov    dword ptr [edi + ecx*4], eax
        mov    edi, dword ptr [ebx + 40h]
        mov    ax, word ptr [esi]
        mov    dl, al
        and    al, 7fh
        mov    word ptr [edi + ecx*2], ax
        mov    edi, ecx
        lea    edi, [ecx + ecx*2]
        shl    ecx, 4
        lea    edi, [ecx + edi*2]
        add    edi, dword ptr [ebx + 44h]
        db  0FFh, 005h, 07Ch, 058h, 000h, 000h
        db  0A1h, 07Ch, 058h, 000h, 000h
        stosd
        test   dl, 80h
        je     SHORT loc_1_229E
        mov    ax, word ptr [esi + 6]
        movzx  ecx, al
        and    cl, 1fh
        cmp    cl, 2
        je     SHORT loc_1_229E
        and    al, 3fh
        xor    ah, 80h
        stosw
        push   esi
        add    esi, 8
        shr    ecx, 2
        rep movsd
        pop    esi
        mov    edi, ebx
        ret
loc_1_230E:
        mov    edi, dword ptr [ebx + 44h]
        xor    eax, eax
        lea    edx, [eax - 1]
loc_1_2316:
        cmp    edx, dword ptr [edi]
        jb     SHORT loc_1_231E
        mov    edx, dword ptr [edi]
        mov    eax, ecx
loc_1_231E:
        add    edi, 16h
        loop   loc_1_2316
        neg    eax
        db  003h, 005h, 078h, 058h, 000h, 000h
        mov    ecx, eax
        jmp    SHORT loc_1_22B5
sub_1_232F:
        mov    edi, dword ptr [ebx + 3ch]
        mov    ecx, dword ptr [ebx + 38h]
loc_1_2335:
        mov    eax, dword ptr [esi + 2]
        repne scasd
        jne    SHORT loc_1_2363
        push   ecx
        mov    edx, dword ptr [ebx + 38h]
        sub    edx, ecx
        mov    eax, dword ptr [ebx + 40h]
        mov    cx, word ptr [esi]
        and    cl, 7fh
        cmp    word ptr [eax + edx*2 - 2], cx
        pop    ecx
        jne    SHORT loc_1_2335
        dec    edx
        mov    ecx, edx
        lea    edi, [edx + edx*2]
        shl    edx, 4
        lea    edi, [edx + edi*2]
        add    edi, dword ptr [ebx + 44h]
        ret
loc_1_2363:
        stc
        ret
sub_1_2365:
        call   sub_1_232F
        jae    SHORT loc_1_236D
        ret
loc_1_236D:
        push   esi
        cli
        dec    dword ptr [ebx + 38h]
        mov    eax, dword ptr [ebx + 38h]
        cmp    ecx, eax
        je     SHORT loc_1_23A6
        mov    esi, eax
        lea    edx, [esi + esi*2]
        shl    esi, 4
        lea    esi, [esi + edx*2]
        add    esi, dword ptr [ebx + 44h]
        mov    edx, ecx
        mov    ecx, 5
        rep movsd
        movsw
        mov    edi, dword ptr [ebx + 3ch]
        mov    ecx, dword ptr [edi + eax*4]
        mov    dword ptr [edi + edx*4], ecx
        mov    edi, dword ptr [ebx + 40h]
        mov    cx, word ptr [edi + eax*2]
        mov    word ptr [edi + edx*2], cx
loc_1_23A6:
        sti
        pop    esi
        ret
        db  000h, 000h, 000h, 000h
sub_1_23AD:
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
loc_1_23CD:
        db  0CDh, 020h, 038h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        test   dword ptr [esi + 4], 20000000h
        je     SHORT loc_1_23CD
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        and    dword ptr [esi + 4], 0dfffffffh
        pop    esi
        xor    eax, eax
        db  087h, 005h, 0A9h, 023h, 000h, 000h
        ret
        db  08Bh, 074h, 024h, 004h, 083h, 0ECh, 06Ch, 057h, 08Dh, 07Ch, 024h, 004h, 0CDh, 020h, 08Dh, 000h
        db  001h, 000h, 05Fh, 0CDh, 020h, 083h, 000h, 001h, 000h, 0CDh, 020h, 01Ah, 000h, 001h, 000h, 033h
        db  0C0h, 0CDh, 020h, 061h, 001h, 001h, 000h, 0CDh, 020h, 085h, 000h, 001h, 000h, 080h, 03Eh, 000h
        db  075h, 0EDh, 0CDh, 020h, 086h, 000h, 001h, 000h, 056h, 08Dh, 074h, 024h, 004h, 0CDh, 020h, 08Eh
        db  000h, 001h, 000h, 05Eh, 083h, 0C4h, 06Ch, 0C3h
loc_1_2450:
        mov    eax, dword ptr [esp + 0ch]
        db  0A3h, 0A9h, 023h, 000h, 000h
        mov    eax, dword ptr [esp + 4]
        or     dword ptr [eax + 4], 20000000h
        ret    0ch
        db  000h
sub_1_2468:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_246E:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
sub_1_2474:
        db  0CDh, 020h, 016h, 080h, 028h, 000h
sub_1_247A:
        db  0CDh, 020h, 017h, 080h, 028h, 000h
sub_1_2480:
        db  0CDh, 020h, 018h, 080h, 028h, 000h
sub_1_2486:
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
sub_1_248C:
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
sub_1_2492:
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
sub_1_2498:
        db  0CDh, 020h, 023h, 080h, 028h, 000h
sub_1_249E:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_24A4:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
loc_1_24AA:
        db  0CDh, 020h, 029h, 080h, 028h, 000h
sub_1_24B0:
        db  0CDh, 020h, 02Ah, 080h, 028h, 000h
sub_1_24B6:
        db  0CDh, 020h, 02Bh, 080h, 028h, 000h
sub_1_24BC:
        db  0CDh, 020h, 02Ch, 080h, 028h, 000h
sub_1_24C2:
        db  0CDh, 020h, 030h, 080h, 028h, 000h
sub_1_24C8:
        db  0CDh, 020h, 051h, 080h, 028h, 000h
sub_1_24CE:
        db  0CDh, 020h, 052h, 080h, 028h, 000h
sub_1_24D4:
        db  0CDh, 020h, 053h, 080h, 028h, 000h
        add    byte ptr [eax], al
loc_1_24DC:
        xor    esi, esi
        ret
loc_1_24DF:
        cmp    ecx, 28h
        jl     SHORT loc_1_24DC
        cmp    word ptr [edi + 1eh], 200h
        jne    SHORT loc_1_24DC
        mov    ecx, dword ptr [edi + 20h]
        mov    esi, ebx
        mov    edx, edi
        db  08Dh, 01Dh, 05Ch, 056h, 000h, 000h
loc_1_24F9:
        mov    edi, ebx
        mov    ebx, dword ptr [edi + 24h]
        or     ebx, ebx
        je     NEAR PTR loc_1_25AC
        cmp    dword ptr [ebx + 4], esi
        jne    SHORT loc_1_24F9
        mov    eax, dword ptr [ebx + 0ch]
        cmp    dword ptr [eax], ecx
        jne    SHORT loc_1_24F9
        push   edi
        push   esi
        mov    edi, dword ptr [ebx + 10h]
        lea    esi, [edx + 16h]
        cmp    dword ptr [edx + 6], ecx
        jne    SHORT loc_1_2525
        mov    esi, dword ptr [ebx + 0ch]
        add    esi, 4
loc_1_2525:
        movsd
        movsw
        pop    esi
        pop    edi
        movzx  eax, word ptr [edx + 26h]
        mov    dword ptr [ebx + 14h], eax
        or     byte ptr [ebx], 80h
        mov    byte ptr [ebx + 1], 0
        mov    eax, dword ptr [ebx + 24h]
        mov    dword ptr [edi + 24h], eax
        push   esi
        xor    esi, esi
        xchg   dword ptr [ebx + 1ch], esi
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        test   byte ptr [ebx], 2
        jne    SHORT loc_1_2559
        test   byte ptr [ebx], 4
        jne    SHORT loc_1_256E
        mov    ebx, edi
        jmp    SHORT loc_1_24F9
loc_1_2559:
        push   edx
        push   esi
        db  0BEh, 059h, 026h, 000h, 000h
        mov    edx, ebx
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        pop    esi
        pop    edx
        mov    ebx, edi
        jmp    SHORT loc_1_24F9
loc_1_256E:
        push   esi
        mov    esi, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebx + 14h]
        xchg   al, ah
        mov    word ptr [esi + 0ah], ax
        push   edx
        mov    edx, dword ptr [ebx + 10h]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi + 1ch], eax
        mov    ax, word ptr [edx + 4]
        mov    word ptr [esi + 20h], ax
        pop    edx
        mov    byte ptr [esi + 9], 0
        push   esi
        call   sub_1_26BF
        pop    esi
        push   0
        push   8eh
        push   esi
        db  0CDh, 020h, 00Bh, 000h, 028h, 000h
        pop    esi
        jmp    NEAR PTR loc_1_24F9
loc_1_25AC:
        xor    esi, esi
        ret
loc_1_25AF:
        cmp    dword ptr [edx + 1ch], 0
        je     SHORT loc_1_25E5
        dec    dword ptr [edx + 20h]
        je     SHORT loc_1_260A
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 18h]
        cmp    byte ptr [esi + 8], 0ffh
        je     SHORT loc_1_25FC
        mov    eax, 3e8h
        db  0BEh, 0AFh, 025h, 000h, 000h
        mov    edx, ebx
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [ebx + 1ch], esi
        mov    esi, dword ptr [ebx + 18h]
        mov    edi, dword ptr [ebx + 4]
        call   sub_1_410D
loc_1_25E5:
        ret
loc_1_25E6:
        mov    eax, 3e8h
        db  0BEh, 0AFh, 025h, 000h, 000h
        mov    edx, ebx
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    dword ptr [ebx + 1ch], esi
        ret
loc_1_25FC:
        dec    dword ptr [ebx + 14h]
        jne    SHORT loc_1_25E6
        push   esi
        call   sub_1_0562
        pop    esi
        mov    edx, ebx
loc_1_260A:
        mov    dword ptr [edx + 1ch], 0
        or     byte ptr [edx], 80h
        mov    byte ptr [edx + 1], 0fah
        mov    esi, edx
        push   24h
        push   edx
        db  068h, 080h, 056h, 000h, 000h
        call   sub_1_46EA
        add    esp, 0ch
        test   byte ptr [esi], 2
        je     SHORT loc_1_263B
        test   byte ptr [esi], 4
        jne    SHORT loc_1_263C
        mov    edx, esi
        call   sub_1_2659
loc_1_263B:
        ret
loc_1_263C:
        mov    esi, dword ptr [esi + 8]
        mov    byte ptr [esi + 9], 0fah
        push   esi
        call   sub_1_26BF
        pop    esi
        push   0
        push   8eh
        push   esi
        db  0CDh, 020h, 00Bh, 000h, 028h, 000h
        ret
sub_1_2659:
        mov    esi, dword ptr [edx + 8]
        cmp    byte ptr [edx + 1], 0
        jne    SHORT loc_1_269E
        or     dword ptr [esi + 4], 200000h
        mov    eax, dword ptr [edx + 14h]
        xchg   ah, al
        mov    dword ptr [esi + 2ch], eax
        db  068h, 08Bh, 03Ch, 000h, 000h
loc_1_2676:
        cmp    byte ptr [edx + 44h], 0
        jne    SHORT loc_1_268D
        push   0
        push   8eh
        push   edx
        db  0CDh, 020h, 00Bh, 000h, 028h, 000h
        mov    edx, esi
        ret
loc_1_268D:
        mov    dword ptr [edx + 40h], 1
        db  0C7h, 042h, 054h, 0ADh, 026h, 000h, 000h
        mov    edx, esi
        ret
loc_1_269E:
        mov    eax, dword ptr [esi + 0ch]
        or     dword ptr [esi + 4], 20000h
        inc    byte ptr [eax + 17h]
        jmp    SHORT loc_1_2676
loc_1_26AD:
        sub    eax, 3ch
        push   0
        push   8eh
        push   eax
        db  0CDh, 020h, 00Bh, 000h, 028h, 000h
        ret
sub_1_26BF:
        mov    esi, dword ptr [esp]
        mov    byte ptr [esi + 8], 0
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     SHORT loc_1_26CF
        call   dword ptr [eax]
loc_1_26CF:
        ret
loc_1_26D0:
        pop    ecx
        ret
loc_1_26D2:
        db  0FFh, 005h, 0ACh, 056h, 000h, 000h
        ret
loc_1_26D9:
        db  0FFh, 005h, 0A8h, 056h, 000h, 000h
        ret
loc_1_26E0:
        mov    ecx, dword ptr [eax + 6]
        or     ecx, ecx
        je     NEAR PTR sub_1_1FFC
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        mov    ebx, dword ptr [edi + 1ch]
        cmp    dword ptr [esi], ebx
        jne    SHORT loc_1_26D0
        mov    bx, word ptr [edi + 20h]
        cmp    word ptr [esi + 4], bx
        jne    SHORT loc_1_26D0
        mov    ebx, edi
        mov    edi, eax
        mov    eax, ecx
        push   edx
        call   sub_1_2D10
        pop    edx
        jb     SHORT loc_1_26D0
        cmp    word ptr [esi + 14h], 0
        jne    SHORT loc_1_2735
        mov    eax, dword ptr [esi + 20h]
        mov    ecx, dword ptr [eax + 1ch]
        cmp    dword ptr [edi + 0ah], ecx
        jne    SHORT loc_1_2735
        mov    cx, word ptr [eax + 20h]
        cmp    word ptr [edi + 0eh], cx
        jne    SHORT loc_1_2735
        mov    eax, edi
        mov    edi, ebx
        pop    esi
        jmp    NEAR PTR sub_1_1FFC
loc_1_2735:
        pop    ecx
        db  039h, 015h, 0ACh, 07Ch, 000h, 000h
        jb     SHORT loc_1_26D2
        call   sub_1_291A
        jb     SHORT loc_1_26D9
        db  0FFh, 005h, 0A4h, 056h, 000h, 000h
        mov    edi, dword ptr [eax + 48h]
        inc    byte ptr [edi + 4]
        cmp    dword ptr [edi + 12h], 0
        jne    SHORT loc_1_275D
        mov    ecx, dword ptr [ebx + 18h]
        mov    dword ptr [edi + 12h], ecx
loc_1_275D:
        mov    ebx, dword ptr [esi + 20h]
        push   ebx
        push   edx
        lea    edx, [eax + 38h]
        push   edx
        lea    edx, [esi + 0ch]
        mov    ecx, dword ptr [edi + 6]
        cmp    dword ptr [ebx + 18h], ecx
        jne    SHORT loc_1_2774
        lea    edx, [edi + 0ah]
loc_1_2774:
        mov    esi, eax
        push   edx
        call   dword ptr [ebx + 28h]
        add    esp, 10h
loc_1_277D:
        mov    ecx, dword ptr [esi + 44h]
        push   ecx
        mov    eax, dword ptr [esi + 54h]
        mov    dword ptr [ecx + 0ch], eax
        lea    eax, [esi + 58h]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], edi
        push   esi
        db  0CDh, 020h, 023h, 000h, 028h, 000h
        push   esi
        or     byte ptr [esi + 3ch], 4
        mov    dword ptr [esi + 40h], ebx
        mov    ebx, dword ptr [ebx + 48h]
        push   dword ptr [ebx + 0ch]
        lea    eax, [ebp + 0ch]
        push   eax
        db  083h, 00Dh, 0F8h, 058h, 000h, 000h, 020h
        db  0CDh, 020h, 02Dh, 000h, 028h, 000h
        db  083h, 025h, 0F8h, 058h, 000h, 000h, 0DFh
        cmp    dword ptr [ebp + 0ch], 103h
        jne    SHORT loc_1_27C9
        ret
loc_1_27C9:
        push   dword ptr [ebp + 0ch]
        push   esi
        push   ebx
        call   sub_1_21B9
        ret
sub_1_27D4:
        push   edx
        push   edx
        db  0CDh, 020h, 019h, 000h, 028h, 000h
        pop    edx
        test   byte ptr [edx + 3ch], 20h
        jne    SHORT loc_1_27F5
        mov    eax, edx
        db  087h, 005h, 01Ch, 07Dh, 000h, 000h
        mov    dword ptr [edx + 38h], eax
        db  0FFh, 005h, 0B4h, 07Ch, 000h, 000h
        ret
loc_1_27F5:
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        call   sub_1_2884
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_2803:
        jmp    NEAR PTR sub_1_1FFC
loc_1_2808:
        ret
loc_1_2809:
        test   dword ptr [edi + 8], 8
        jne    SHORT loc_1_281E
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 000h, 004h, 000h, 000h
        je     SHORT loc_1_2803
loc_1_281E:
        mov    ecx, 0ffffffffh
        cmp    dword ptr [eax + 0ah], ecx
        jne    SHORT loc_1_2803
        cmp    word ptr [eax + 0eh], cx
        jne    SHORT loc_1_2803
        movzx  ecx, byte ptr [eax + 4]
        cmp    cl, 8
        jae    SHORT loc_1_2803
        mov    ebx, edi
        mov    edi, eax
        jecxz  loc_1_2849
        push   eax
        lea    edi, [eax + 1eh]
        mov    eax, dword ptr [ebx + 18h]
        repne scasd
        pop    edi
        je     SHORT loc_1_2808
loc_1_2849:
        call   sub_1_291A
        jb     SHORT loc_1_2808
        push   eax
        mov    eax, dword ptr [eax + 48h]
        mov    dword ptr [ebp + 2ch], eax
        mov    dword ptr [ebp + 30h], edx
        mov    edi, ebx
        push   ebx
        call   sub_1_1FFC
        pop    eax
        pop    ebx
        mov    esi, dword ptr [ebx + 48h]
        mov    edx, dword ptr [eax + 18h]
        cmp    dword ptr [esi + 12h], 0
        jne    SHORT loc_1_2873
        mov    dword ptr [esi + 12h], edx
loc_1_2873:
        movzx  ecx, byte ptr [esi + 4]
        inc    byte ptr [esi + 4]
        mov    dword ptr [esi + ecx*4 + 1eh], edx
        mov    word ptr [ebx + 3eh], 0ffffh
sub_1_2884:
        mov    esi, dword ptr [ebx + 48h]
        movsx  edx, word ptr [ebx + 3eh]
        or     byte ptr [ebx + 3ch], 20h
        push   ebx
loc_1_2890:
        inc    edx
        db  039h, 015h, 0C8h, 058h, 000h, 000h
        je     SHORT loc_1_28E0
        db  08Bh, 01Ch, 095h, 0DCh, 058h, 000h, 000h
        mov    eax, dword ptr [ebx + 18h]
        or     eax, eax
        je     SHORT loc_1_2890
        movzx  ecx, byte ptr [esi + 4]
        lea    edi, [esi + 1eh]
        repne scasd
        je     SHORT loc_1_2890
        test   dword ptr [ebx + 8], 8
        jne    SHORT loc_1_2890
        pop    edi
        mov    dword ptr [esi + 6], eax
        mov    word ptr [edi + 3eh], dx
        movzx  ecx, word ptr [esi + 2]
        xchg   ch, cl
        push   ebx
        push   ecx
        lea    edx, [edi + 38h]
        push   edx
        lea    edx, [esi + 0ah]
        push   edx
        call   dword ptr [ebx + 28h]
        add    esp, 10h
        xchg   edi, esi
        jmp    NEAR PTR loc_1_277D
loc_1_28E0:
        pop    ebx
        and    byte ptr [ebx + 3ch], 0dfh
        mov    eax, ebx
        db  087h, 005h, 01Ch, 07Dh, 000h, 000h
        mov    dword ptr [ebx + 38h], eax
        db  0FFh, 005h, 0B4h, 07Ch, 000h, 000h
        ret
loc_1_28F7:
        mov    eax, dword ptr [eax - 4]
        db  0FFh, 005h, 0B4h, 07Ch, 000h, 000h
        mov    ecx, eax
        db  087h, 00Dh, 01Ch, 07Dh, 000h, 000h
        mov    dword ptr [eax + 38h], ecx
        db  0FFh, 005h, 09Ch, 056h, 000h, 000h
        ret
loc_1_2912:
        db  0FFh, 005h, 0B4h, 07Ch, 000h, 000h
        stc
        ret
sub_1_291A:
        db  0FFh, 00Dh, 0B4h, 07Ch, 000h, 000h
        js     SHORT loc_1_2912
        push   esi
        push   ebx
        db  08Bh, 01Dh, 01Ch, 07Dh, 000h, 000h
        mov    ecx, dword ptr [ebx + 38h]
        db  089h, 00Dh, 01Ch, 07Dh, 000h, 000h
        mov    esi, dword ptr [ebx + 48h]
        cmp    dword ptr [ebp + 30h], edx
        jb     SHORT loc_1_2959
        xchg   esi, edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    cl, dl
        and    cl, 3
        rep movsb
        clc
        mov    eax, dword ptr [ebx + 44h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [eax + 0ch], edx
        mov    eax, ebx
        pop    ebx
        pop    esi
        ret
loc_1_2959:
        mov    ecx, dword ptr [ebx + 44h]
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [ecx + 0ch], edx
        mov    esi, edx
        mov    edi, dword ptr [ecx]
        mov    dword ptr [ecx], 0
        push   ecx
        push   ebx
        db  0CDh, 020h, 023h, 000h, 028h, 000h
        lea    ecx, [ebx + 54h]
        mov    eax, dword ptr [esp]
        push   ecx
        push   ebx
        push   esi
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 20h]
        mov    eax, dword ptr [eax + 48h]
        push   dword ptr [eax + 0ch]
        lea    eax, [ebp + 0ch]
        push   eax
        db  0CDh, 020h, 02Eh, 000h, 028h, 000h
        mov    ecx, dword ptr [ebx + 44h]
        mov    dword ptr [ecx], edi
        push   ebx
        db  0CDh, 020h, 019h, 000h, 028h, 000h
        cmp    dword ptr [ebp + 0ch], 0
        jne    SHORT loc_1_29B1
        mov    edx, dword ptr [ebx + 54h]
        mov    dword ptr [edi + 0ch], edx
        clc
        mov    eax, ebx
loc_1_29AE:
        pop    ebx
        pop    esi
        ret
loc_1_29B1:
        cmp    dword ptr [ebp + 0ch], 103h
        je     SHORT loc_1_29CE
        mov    eax, ebx
        db  087h, 005h, 01Ch, 07Dh, 000h, 000h
        mov    dword ptr [ebx + 38h], eax
        db  0FFh, 005h, 0B4h, 07Ch, 000h, 000h
        stc
        jmp    SHORT loc_1_29AE
loc_1_29CE:
        jmp    SHORT loc_1_29CE
loc_1_29D0:
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 040h, 000h, 000h, 000h
        je     NEAR PTR loc_1_2AEA
        mov    eax, dword ptr [ebx + 18h]
        mov    ecx, dword ptr [edi + 12h]
        jecxz  loc_1_29ED
        cmp    dword ptr [ebx + 18h], ecx
        jne    SHORT loc_1_2A11
loc_1_29ED:
        mov    eax, dword ptr [edi + 20h]
        cmp    eax, -1
        je     SHORT loc_1_2A70
        call   sub_1_2D10
        jb     SHORT loc_1_2A11
        cmp    word ptr [esi + 14h], 0e00h
        je     SHORT loc_1_2A11
        mov    ecx, dword ptr [ebx + 4]
        test   dword ptr [esi + 4], ecx
        je     SHORT loc_1_2A25
        cmp    dword ptr [ebx + 18h], eax
        je     SHORT loc_1_2A25
loc_1_2A11:
        xor    esi, esi
        ret
loc_1_2A14:
        call   sub_1_2D4D
        xor    esi, esi
        ret
loc_1_2A1C:
        db  0FFh, 005h, 0A0h, 056h, 000h, 000h
        xor    esi, esi
        ret
loc_1_2A25:
        db  08Bh, 00Dh, 084h, 056h, 000h, 000h
        cmp    dword ptr [esi + 18h], ecx
        jb     SHORT loc_1_2A14
        call   sub_1_2A8C
        jb     SHORT loc_1_2A1C
        mov    dword ptr [ecx + 24h], edx
        mov    word ptr [ecx + 28h], 28h
        mov    byte ptr [edx + 5], 1
        mov    word ptr [edx + 1eh], 200h
        mov    dword ptr [edx + 20h], eax
        mov    eax, dword ptr [esi + 14h]
        mov    si, word ptr [ebx + 24h]
        shl    esi, 18h
        add    eax, esi
        inc    ah
        mov    dword ptr [edx + 24h], eax
        mov    esi, ecx
        mov    edi, ebx
        db  0FFh, 005h, 098h, 056h, 000h, 000h
        call   sub_1_410D
        xor    esi, esi
        ret
loc_1_2A70:
        lea    eax, [edi + 16h]
        push   0
        push   0
        push   eax
        push   ebx
        call   sub_1_6AB9
        add    esp, 10h
        xor    esi, esi
        ret
loc_1_2A84:
        db  0FFh, 005h, 0B4h, 07Ch, 000h, 000h
        stc
        ret
sub_1_2A8C:
        db  0FFh, 00Dh, 0B4h, 07Ch, 000h, 000h
        js     SHORT loc_1_2A84
        db  08Bh, 015h, 01Ch, 07Dh, 000h, 000h
        mov    ecx, dword ptr [edx + 38h]
        db  089h, 00Dh, 01Ch, 07Dh, 000h, 000h
        mov    ecx, dword ptr [edx + 48h]
        mov    dword ptr [ecx], edx
        add    ecx, 4
        db  0C7h, 041h, 018h, 0F7h, 028h, 000h, 000h
        mov    dword ptr [ecx + 4], 1
        mov    word ptr [ecx + 0ah], 5304h
        mov    dword ptr [ecx + 0ch], 0
        mov    edx, dword ptr [edi + 16h]
        mov    dword ptr [ecx + 1ch], edx
        mov    dword ptr [ecx + 34h], edx
        mov    edx, dword ptr [edi + 1ah]
        mov    word ptr [ecx + 20h], dx
        mov    dword ptr [ecx + 38h], edx
        mov    edx, dword ptr [ebx + 18h]
        mov    dword ptr [ecx + 30h], edx
        mov    word ptr [ecx + 22h], 1
        lea    edx, [ecx + 2ah]
        ret
loc_1_2AE9:
        pop    eax
loc_1_2AEA:
        xor    esi, esi
        ret
loc_1_2AED:
        mov    eax, dword ptr [ebx + 1ch]
        cmp    dword ptr [esi], eax
        jne    SHORT loc_1_2AFE
        mov    ax, word ptr [ebx + 20h]
        cmp    word ptr [esi + 4], ax
        je     SHORT loc_1_2AEA
loc_1_2AFE:
        cmp    word ptr [edi + 1eh], 100h
        je     NEAR PTR loc_1_29D0
        db  0C7h, 005h, 094h, 056h, 000h, 000h, 006h, 000h, 000h, 000h
        mov    eax, dword ptr [ebx + 18h]
        cmp    dword ptr [edi + 12h], eax
        jne    SHORT loc_1_2AEA
        push   0
        movzx  edx, word ptr [edi + 2]
        xchg   dh, dl
        sub    ecx, edx
        sbb    esi, esi
        and    ecx, esi
        add    ecx, edx
        cmp    ecx, edx
        je     SHORT loc_1_2B3F
        call   sub_1_291A
        jb     SHORT loc_1_2AE9
        mov    ecx, edx
        mov    edi, dword ptr [eax + 48h]
        mov    dword ptr [esp], eax
loc_1_2B3F:
        sub    ecx, 20h
        lea    edx, [edi + 16h]
        add    edi, 20h
        shr    ecx, 3
        jecxz  loc_1_2B59
        push   edx
loc_1_2B4E:
        call   sub_1_2BD2
        add    edi, 8
        loop   loc_1_2B4E
        pop    ecx
loc_1_2B59:
        pop    ecx
        jecxz  loc_1_2B6D
        mov    eax, ecx
        db  087h, 005h, 01Ch, 07Dh, 000h, 000h
        mov    dword ptr [ecx + 38h], eax
        db  0FFh, 005h, 0B4h, 07Ch, 000h, 000h
loc_1_2B6D:
        xor    esi, esi
        ret
loc_1_2B70:
        call   sub_1_6640
        db  0C7h, 005h, 0BCh, 056h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
sub_1_2B80:
        push   esi
        db  08Bh, 035h, 0BCh, 056h, 000h, 000h
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        db  0C7h, 005h, 0BCh, 056h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    esi
sub_1_2B98:
        push   ebx
        db  0CDh, 020h, 000h, 000h, 007h, 000h
        cmp    bl, 2
        jne    SHORT loc_1_2BCB
        db  083h, 03Dh, 0BCh, 056h, 000h, 000h, 000h
        jne    SHORT loc_1_2BD0
        push   esi
        db  0BEh, 070h, 02Bh, 000h, 000h
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 2
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        db  089h, 035h, 0BCh, 056h, 000h, 000h
        pop    esi
        jmp    SHORT loc_1_2BD0
loc_1_2BCB:
        call   sub_1_6640
loc_1_2BD0:
        pop    ebx
        ret
sub_1_2BD2:
        push   ecx
        mov    eax, dword ptr [edi]
        call   sub_1_2D10
        jb     NEAR PTR loc_1_2C92
        cmp    dword ptr [esi + 20h], ebx
        jne    SHORT loc_1_2C24
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 2]
        cmp    dword ptr [esi + 0eh], eax
        jne    SHORT loc_1_2C24
        mov    ax, word ptr [ecx]
        cmp    word ptr [esi + 0ch], ax
        jne    SHORT loc_1_2C24
        mov    eax, dword ptr [edi + 4]
        cmp    ah, 10h
        jae    SHORT loc_1_2C1C
        mov    dword ptr [esi + 14h], eax
        mov    eax, dword ptr [ebx + 4]
        or     dword ptr [esi + 8], eax
        db  08Bh, 00Dh, 084h, 056h, 000h, 000h
        add    ecx, 2bf20h
        mov    dword ptr [esi + 18h], ecx
        pop    ecx
        ret
loc_1_2C1C:
        dec    edx
        call   sub_1_2D4D
        pop    ecx
        ret
loc_1_2C24:
        db  08Bh, 00Dh, 084h, 056h, 000h, 000h
        cmp    dword ptr [esi + 18h], ecx
        jb     SHORT loc_1_2C5E
        mov    ax, word ptr [edi + 6]
        mov    dx, word ptr [esi + 16h]
        cmp    ah, dh
        jb     SHORT loc_1_2C5E
        je     SHORT loc_1_2C52
        mov    cx, word ptr [ebx + 24h]
        add    dh, cl
        cmp    ah, dh
        jae    SHORT loc_1_2C50
loc_1_2C47:
        mov    eax, dword ptr [ebx + 4]
        or     dword ptr [esi + 4], eax
        or     dword ptr [esi + 8], eax
loc_1_2C50:
        pop    ecx
        ret
loc_1_2C52:
        mov    ax, word ptr [edi + 4]
        mov    dx, word ptr [esi + 14h]
        cmp    ah, dh
        jae    SHORT loc_1_2C47
loc_1_2C5E:
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi + 0ch], eax
        mov    ax, word ptr [edx + 4]
        mov    word ptr [esi + 10h], ax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 14h], eax
        mov    dword ptr [esi + 20h], ebx
        mov    eax, dword ptr [ebx + 4]
        or     dword ptr [esi + 4], eax
        or     dword ptr [esi + 8], eax
        db  08Bh, 00Dh, 084h, 056h, 000h, 000h
        add    ecx, 2bf20h
        mov    dword ptr [esi + 18h], ecx
loc_1_2C90:
        pop    ecx
        ret
loc_1_2C92:
        cmp    word ptr [edi + 4], 1000h
        je     SHORT loc_1_2C90
        db  0FFh, 00Dh, 008h, 07Dh, 000h, 000h
        js     SHORT loc_1_2CE4
loc_1_2CA2:
        db  08Bh, 035h, 020h, 07Dh, 000h, 000h
        mov    ecx, dword ptr [esi + 24h]
        db  089h, 00Dh, 020h, 07Dh, 000h, 000h
        mov    dword ptr [esi], eax
        db  08Bh, 00Dh, 0B8h, 056h, 000h, 000h
        db  08Bh, 015h, 0B0h, 056h, 000h, 000h
        mov    dword ptr [edx + ecx*4], eax
        db  08Bh, 015h, 0B4h, 056h, 000h, 000h
        mov    dword ptr [edx + ecx*4], esi
        db  0FFh, 005h, 0B8h, 056h, 000h, 000h
        xor    ecx, ecx
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 24h], ecx
        mov    dword ptr [esi + 1ch], ecx
        jmp    NEAR PTR loc_1_2C5E
loc_1_2CE4:
        db  0FFh, 005h, 008h, 07Dh, 000h, 000h
        call   sub_1_2B98
        db  083h, 03Dh, 008h, 07Dh, 000h, 000h, 000h
        je     SHORT loc_1_2D00
        db  0FFh, 00Dh, 008h, 07Dh, 000h, 000h
        jmp    SHORT loc_1_2CA2
loc_1_2D00:
        pop    ecx
        ret
sub_1_2D02:
        push   esi
        mov    eax, dword ptr [esp + 8]
        call   sub_1_2D10
        mov    eax, esi
        pop    esi
        ret
sub_1_2D10:
        cld
        db  08Bh, 00Dh, 0B8h, 056h, 000h, 000h
        jecxz  loc_1_2D37
        push   edi
        db  08Bh, 03Dh, 0B0h, 056h, 000h, 000h
        mov    edx, ecx
        or     esi, esi
        repne scasd
        pop    edi
        jne    SHORT loc_1_2D37
        sub    edx, ecx
        db  08Bh, 035h, 0B4h, 056h, 000h, 000h
        mov    esi, dword ptr [esi + edx*4 - 4]
        clc
        ret
loc_1_2D37:
        xor    esi, esi
        stc
        ret
sub_1_2D3B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0ch]
        call   sub_1_2D4D
        xor    eax, eax
        pop    esi
        ret
sub_1_2D4D:
        push   edi
        push   edx
        db  08Bh, 00Dh, 0F4h, 058h, 000h, 000h
        dec    ecx
        cmp    edx, ecx
        jg     SHORT loc_1_2D7A
        db  089h, 00Dh, 0F4h, 058h, 000h, 000h
        db  08Bh, 03Dh, 0B0h, 056h, 000h, 000h
        mov    eax, dword ptr [edi + ecx*4]
        mov    dword ptr [edi + edx*4], eax
        db  08Bh, 03Dh, 0B4h, 056h, 000h, 000h
        mov    eax, dword ptr [edi + ecx*4]
        mov    dword ptr [edi + edx*4], eax
        mov    edx, ecx
loc_1_2D7A:
        db  08Bh, 00Dh, 0B8h, 056h, 000h, 000h
        dec    ecx
        db  08Bh, 03Dh, 0B0h, 056h, 000h, 000h
        mov    eax, dword ptr [edi + ecx*4]
        mov    dword ptr [edi + edx*4], eax
        db  08Bh, 03Dh, 0B4h, 056h, 000h, 000h
        mov    eax, dword ptr [edi + ecx*4]
        mov    dword ptr [edi + edx*4], eax
        db  0FFh, 00Dh, 0B8h, 056h, 000h, 000h
        pop    edx
        mov    edi, esi
        xor    esi, esi
        xchg   dword ptr [edi + 1ch], esi
        push   ebx
        push   edx
loc_1_2DA9:
        or     esi, esi
        je     SHORT loc_1_2DC0
        mov    dword ptr [esi + 0ch], 0
        mov    ebx, dword ptr [esi + 8]
        call   sub_1_34DE
        mov    esi, ebx
        jmp    SHORT loc_1_2DA9
loc_1_2DC0:
        pop    edx
        pop    ebx
        mov    eax, edi
        db  087h, 005h, 020h, 07Dh, 000h, 000h
        mov    dword ptr [edi + 24h], eax
        db  0FFh, 005h, 008h, 07Dh, 000h, 000h
        pop    edi
        ret
loc_1_2DD5:
        mov    ebx, 0ffffffffh
        jmp    NEAR PTR loc_1_2E66
loc_1_2DDF:
        db  0C7h, 005h, 090h, 056h, 000h, 000h, 000h, 000h, 000h, 000h
        db  083h, 005h, 084h, 056h, 000h, 000h, 06Eh
        jb     SHORT loc_1_2DD5
        db  0FFh, 00Dh, 088h, 056h, 000h, 000h
        je     SHORT loc_1_2E11
loc_1_2DFA:
        db  0BEh, 0DFh, 02Dh, 000h, 000h
        mov    eax, 6eh
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        db  089h, 035h, 090h, 056h, 000h, 000h
        ret
loc_1_2E11:
        db  0C7h, 005h, 088h, 056h, 000h, 000h, 010h, 001h, 000h, 000h
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 000h, 010h, 000h, 000h
        je     SHORT loc_1_2E2C
        call   sub_1_79E7
loc_1_2E2C:
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 010h, 000h, 000h, 000h
        je     SHORT loc_1_2DFA
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_1_2E49
        call   sub_1_2EBE
loc_1_2E49:
        db  083h, 03Dh, 094h, 056h, 000h, 000h, 000h
        je     SHORT loc_1_2E58
        db  0FFh, 00Dh, 094h, 056h, 000h, 000h
loc_1_2E58:
        db  0FFh, 00Dh, 08Ch, 056h, 000h, 000h
        jne    SHORT loc_1_2DFA
        db  08Bh, 01Dh, 084h, 056h, 000h, 000h
loc_1_2E66:
        db  08Bh, 035h, 0B4h, 056h, 000h, 000h
        db  08Bh, 00Dh, 0B8h, 056h, 000h, 000h
        db  08Bh, 015h, 0F4h, 058h, 000h, 000h
        sub    ecx, edx
        jecxz  loc_1_2E95
        lea    esi, [esi + edx*4]
        cld
loc_1_2E80:
        lodsd
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax + 8], 0
        cmp    dword ptr [eax + 18h], ebx
        jb     SHORT loc_1_2EA4
        loop   loc_1_2E80
loc_1_2E95:
        db  0C7h, 005h, 08Ch, 056h, 000h, 000h, 006h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_2DFA
loc_1_2EA4:
        db  08Bh, 015h, 0B8h, 056h, 000h, 000h
        sub    edx, ecx
        push   ecx
        push   esi
        mov    esi, eax
        call   sub_1_2D4D
        pop    esi
        pop    ecx
        sub    esi, 4
        loop   loc_1_2E80
        jmp    SHORT loc_1_2E95
sub_1_2EBE:
        call   sub_1_6B69
        ret
sub_1_2EC4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    edx, dword ptr [eax]
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        push   32h
loc_1_2ED7:
        db  08Bh, 035h, 0B4h, 056h, 000h, 000h
        mov    esi, dword ptr [esi + edx*4]
        db  0A1h, 084h, 056h, 000h, 000h
        cmp    dword ptr [esi + 18h], eax
        jb     SHORT loc_1_2F2A
        mov    eax, dword ptr [ebx + 4]
        test   dword ptr [esi + 4], eax
        jne    SHORT loc_1_2F0F
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 14h]
        inc    ah
        mov    cx, word ptr [ebx + 24h]
        shl    ecx, 18h
        add    eax, ecx
        mov    dword ptr [edi + 4], eax
        add    edi, 8
        dec    dword ptr [esp]
        je     SHORT loc_1_2F31
loc_1_2F0F:
        inc    edx
loc_1_2F10:
        db  039h, 015h, 0B8h, 056h, 000h, 000h
        jne    SHORT loc_1_2ED7
        xor    edx, edx
loc_1_2F1A:
        pop    ecx
        mov    ebx, dword ptr [ebp + 10h]
        mov    dword ptr [ebx], edx
        neg    ecx
        lea    eax, [ecx + 32h]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_2F2A:
        call   sub_1_2D4D
        jmp    SHORT loc_1_2F10
loc_1_2F31:
        inc    edx
        db  039h, 015h, 0B8h, 056h, 000h, 000h
        jne    SHORT loc_1_2F1A
        xor    edx, edx
        jmp    SHORT loc_1_2F1A
loc_1_2F3E:
        movzx  ecx, byte ptr [eax + 9]
        sub    eax, 18h
        jecxz  loc_1_2F51
        test   byte ptr [eax + 15h], 1
        je     SHORT loc_1_2F51
        and    dword ptr [esi + 8], 0ffffffdfh
loc_1_2F51:
        cmp    dword ptr [eax + 8], 0
        jne    SHORT loc_1_2F6B
loc_1_2F57:
        mov    edx, dword ptr [eax]
        db  0FFh, 005h, 0B4h, 07Ch, 000h, 000h
        mov    eax, edx
        db  087h, 005h, 01Ch, 07Dh, 000h, 000h
        mov    dword ptr [edx + 38h], eax
        ret
loc_1_2F6B:
        mov    edx, eax
        db  0BEh, 07Eh, 02Fh, 000h, 000h
        mov    eax, 37h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        ret
loc_1_2F7E:
        mov    edi, edx
        push   edx
        call   sub_1_69D1
        pop    edx
        or     eax, eax
        mov    eax, edi
        jne    SHORT loc_1_2F57
        ret
sub_1_2F8E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebx + 0ch]
        mov    eax, dword ptr [ecx]
        call   sub_1_2D10
        jb     SHORT loc_1_2FEC
        mov    edi, dword ptr [ebx + 10h]
        cmp    word ptr [esi + 14h], 0
        je     SHORT loc_1_2FCE
        mov    eax, dword ptr [esi + 0ch]
        mov    dword ptr [edi], eax
        mov    ax, word ptr [esi + 10h]
        mov    word ptr [edi + 4], ax
        movzx  eax, word ptr [esi + 16h]
        mov    dword ptr [ebx + 14h], eax
        mov    ecx, dword ptr [esi + 20h]
loc_1_2FC3:
        mov    dword ptr [ebx + 28h], ecx
        mov    byte ptr [ebx + 1], 0
loc_1_2FCA:
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_2FCE:
        mov    ecx, dword ptr [ebx + 0ch]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edi], eax
        mov    ax, word ptr [ecx + 8]
        mov    word ptr [edi + 4], ax
        mov    ecx, dword ptr [esi + 20h]
        movzx  eax, word ptr [ecx + 24h]
        xchg   ah, al
        mov    dword ptr [ebx + 14h], eax
        jmp    SHORT loc_1_2FC3
loc_1_2FEC:
        mov    byte ptr [ebx + 1], 0fah
        mov    dword ptr [ebx + 14h], 0
        jmp    SHORT loc_1_2FCA
sub_1_2FF9:
        mov    eax, dword ptr [esp + 4]
        db  039h, 005h, 0ECh, 058h, 000h, 000h
        jne    SHORT loc_1_302B
        mov    eax, dword ptr [eax + 18h]
        push   eax
        db  0CDh, 020h, 000h, 000h, 092h, 004h
        jb     SHORT loc_1_3017
        db  0CDh, 020h, 003h, 000h, 092h, 004h
loc_1_3017:
        db  0CDh, 020h, 000h, 000h, 094h, 004h
        jb     SHORT loc_1_3025
        db  0CDh, 020h, 002h, 000h, 094h, 004h
loc_1_3025:
        pop    eax
        call   sub_1_76E9
loc_1_302B:
        ret
sub_1_302C:
        push   esi
        mov    eax, dword ptr [esp + 8]
        push   0
        mov    edx, esp
        db  0BEh, 051h, 030h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    esi, edx
loc_1_3042:
        cmp    byte ptr [esi], 0
        jne    SHORT loc_1_304E
        call   sub_1_4A98
        jmp    SHORT loc_1_3042
loc_1_304E:
        pop    edx
        pop    esi
        ret
loc_1_3051:
        mov    byte ptr [edx], 0ffh
        ret
        db  000h, 000h, 000h
NWLINK_Service_02:
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 010h, 000h, 000h, 000h
        je     SHORT loc_1_3070
        db  0B8h, 0D0h, 056h, 000h, 000h
        db  0BEh, 03Ah, 034h, 000h, 000h
        clc
        ret
loc_1_3070:
        xor    eax, eax
        stc
        ret
NWLINK_Service_05:
        cmp    eax, 101h
        je     SHORT loc_1_3088
        cmp    eax, 102h
        je     SHORT loc_1_3090
        mov    eax, 0ffffffffh
        ret
loc_1_3088:
        push   esi
        call   sub_1_75B3
        pop    esi
        ret
loc_1_3090:
        push   esi
        call   sub_1_770D
        pop    esi
        ret
        db  0ACh, 030h, 000h, 000h, 0C1h, 030h, 000h, 000h, 0EDh, 030h, 000h, 000h, 074h, 031h, 000h, 000h
        db  077h, 032h, 000h, 000h
loc_1_30AC:
        xor    esi, esi
        ret
loc_1_30AF:
        movzx  eax, word ptr [edi + 1eh]
        xchg   ah, al
        cmp    eax, 5
        jae    SHORT loc_1_30AC
        db  0FFh, 024h, 085h, 098h, 030h, 000h, 000h
loc_1_30C1:
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 010h, 000h, 000h, 000h
        je     SHORT loc_1_30DF
        lea    eax, [edi + 16h]
        push   dword ptr [edi + 20h]
        push   1
        push   eax
        push   ebx
        call   sub_1_6AB9
        add    esp, 10h
loc_1_30DF:
        xor    esi, esi
        ret
loc_1_30E2:
        pop    eax
        xor    esi, esi
        ret
loc_1_30E6:
        and    dword ptr [ebx + 8], 0ffffffdfh
loc_1_30EA:
        xor    esi, esi
        ret
loc_1_30ED:
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 010h, 000h, 000h, 000h
        je     SHORT loc_1_30EA
        xor    edx, edx
        mov    eax, dword ptr [ebx + 1ch]
        cmp    dword ptr [esi], eax
        jne    SHORT loc_1_3116
        mov    ax, word ptr [ebx + 20h]
        cmp    word ptr [esi + 4], ax
        jne    SHORT loc_1_3116
        test   dword ptr [ebx + 8], 20h
        jne    SHORT loc_1_30E6
        inc    edx
loc_1_3116:
        push   0
        mov    esi, edx
        movzx  edx, word ptr [edi + 2]
        xchg   dh, dl
        sub    ecx, edx
        sbb    eax, eax
        and    ecx, eax
        add    ecx, edx
        cmp    ecx, edx
        je     SHORT loc_1_313B
        call   sub_1_291A
        jb     SHORT loc_1_30E2
        mov    ecx, edx
        mov    edi, dword ptr [eax + 48h]
        mov    dword ptr [esp], eax
loc_1_313B:
        sub    ecx, 20h
        lea    edx, [edi + 16h]
        add    edi, 20h
        shr    ecx, 6
        jecxz  loc_1_3155
        push   edx
loc_1_314A:
        call   sub_1_3311
        add    edi, 40h
        loop   loc_1_314A
        pop    ecx
loc_1_3155:
        pop    ecx
        jecxz  loc_1_3169
        mov    eax, ecx
        db  087h, 005h, 01Ch, 07Dh, 000h, 000h
        mov    dword ptr [ecx + 38h], eax
        db  0FFh, 005h, 0B4h, 07Ch, 000h, 000h
loc_1_3169:
        xor    esi, esi
        ret
        db  0EBh, 0E7h
loc_1_316E:
        add    esp, 0ch
loc_1_3171:
        xor    esi, esi
        ret
loc_1_3174:
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 000h, 010h, 000h, 000h
        je     SHORT loc_1_318A
        push   ebx
        push   edi
        call   sub_1_78F3
        add    esp, 8
loc_1_318A:
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 040h, 000h, 000h, 000h
        je     SHORT loc_1_3171
        mov    ax, word ptr [edi + 20h]
        push   edi
        db  08Bh, 03Dh, 0D0h, 056h, 000h, 000h
        db  08Bh, 00Dh, 0D8h, 056h, 000h, 000h
        xor    esi, esi
loc_1_31A9:
        jecxz  loc_1_3229
        repne scasw
        jne    SHORT loc_1_3229
        push   ecx
        push   edi
        neg    ecx
        db  003h, 00Dh, 0D8h, 056h, 000h, 000h
        db  08Bh, 03Dh, 0D4h, 056h, 000h, 000h
        mov    ecx, dword ptr [edi + ecx*4 - 4]
        db  08Bh, 03Dh, 084h, 056h, 000h, 000h
        cmp    dword ptr [ecx + 58h], edi
        jb     SHORT loc_1_31FA
        mov    edi, dword ptr [ecx + 46h]
        cmp    dword ptr [ebx + 18h], edi
        je     SHORT loc_1_316E
        or     esi, esi
        je     SHORT loc_1_320E
        push   eax
        mov    edi, dword ptr [ecx + 0ch]
        mov    dh, byte ptr [edi + 17h]
        mov    eax, dword ptr [ebx + 4]
        test   dword ptr [edi + 4], eax
        jne    SHORT loc_1_31ED
        add    dh, byte ptr [ebx + 24h]
loc_1_31ED:
        pop    eax
        cmp    dh, dl
        jae    SHORT loc_1_31F6
        mov    dl, dh
        mov    esi, ecx
loc_1_31F6:
        pop    edi
        pop    ecx
        jmp    SHORT loc_1_31A9
loc_1_31FA:
        push   eax
        push   esi
        push   edx
        mov    esi, ecx
        call   sub_1_34DE
        pop    edx
        pop    esi
        pop    eax
        pop    edi
        sub    edi, 2
        pop    ecx
        jmp    SHORT loc_1_31A9
loc_1_320E:
        mov    esi, ecx
        mov    edi, dword ptr [esi + 0ch]
        mov    dl, byte ptr [edi + 17h]
        mov    ecx, dword ptr [ebx + 4]
        test   dword ptr [edi + 4], ecx
        jne    SHORT loc_1_3221
        add    dl, byte ptr [ebx + 24h]
loc_1_3221:
        pop    edi
        pop    ecx
        jmp    SHORT loc_1_31A9
loc_1_3225:
        pop    edi
        xor    esi, esi
        ret
loc_1_3229:
        or     esi, esi
        je     SHORT loc_1_3225
        mov    edx, dword ptr [esi + 0ch]
        mov    eax, dword ptr [ebx + 4]
        test   dword ptr [edx + 4], eax
        jne    SHORT loc_1_3225
        pop    edi
        call   sub_1_2A8C
        jb     SHORT loc_1_3274
        mov    word ptr [ecx + 0ah], 5204h
        mov    dword ptr [ecx + 24h], edx
        mov    word ptr [ecx + 28h], 60h
        mov    byte ptr [edx + 5], 4
        mov    word ptr [edx + 1eh], 400h
        mov    eax, ecx
        add    esi, 14h
        lea    edi, [edx + 20h]
        mov    ecx, 10h
        rep movsd
        inc    byte ptr [edx + 5fh]
        mov    esi, eax
        mov    edi, ebx
        call   sub_1_410D
loc_1_3274:
        xor    esi, esi
        ret
loc_1_3277:
        xor    esi, esi
        ret
loc_1_327A:
        call   sub_1_67CD
        db  0C7h, 005h, 0CCh, 056h, 000h, 000h, 000h, 000h, 000h, 000h
        ret
sub_1_328A:
        push   esi
        db  08Bh, 035h, 0CCh, 056h, 000h, 000h
        db  0CDh, 020h, 05Bh, 001h, 001h, 000h
        db  0C7h, 005h, 0CCh, 056h, 000h, 000h, 000h, 000h, 000h, 000h
        pop    esi
sub_1_32A2:
        push   ebx
        db  0CDh, 020h, 000h, 000h, 007h, 000h
        cmp    bl, 2
        jne    SHORT loc_1_32D5
        db  083h, 03Dh, 0CCh, 056h, 000h, 000h, 000h
        jne    SHORT loc_1_32DA
        push   esi
        db  0BEh, 07Ah, 032h, 000h, 000h
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 2
        db  0CDh, 020h, 05Ah, 001h, 001h, 000h
        db  089h, 035h, 0CCh, 056h, 000h, 000h
        pop    esi
        jmp    SHORT loc_1_32DA
loc_1_32D5:
        call   sub_1_67CD
loc_1_32DA:
        pop    ebx
        ret
loc_1_32DC:
        push   edi
        mov    ax, word ptr [edi]
        db  083h, 03Dh, 0E0h, 056h, 000h, 000h, 000h
        je     SHORT loc_1_32FD
        db  08Bh, 03Dh, 0E8h, 056h, 000h, 000h
        db  08Bh, 00Dh, 0E0h, 056h, 000h, 000h
        repne scasw
        pop    edi
        je     SHORT loc_1_331E
        pop    ecx
        ret
loc_1_32FD:
        db  08Bh, 03Dh, 0ECh, 056h, 000h, 000h
        db  08Bh, 00Dh, 0E4h, 056h, 000h, 000h
        repne scasw
        pop    edi
        jne    SHORT loc_1_331E
        pop    ecx
        ret
sub_1_3311:
        push   ecx
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 000h, 000h, 001h, 000h
        jne    SHORT loc_1_32DC
loc_1_331E:
        mov    esi, edi
        call   sub_1_343A
        jb     SHORT loc_1_3384
        cmp    word ptr [edi + 3eh], 1000h
        je     SHORT loc_1_334E
        db  08Bh, 00Dh, 084h, 056h, 000h, 000h
        add    ecx, 2bf20h
        mov    dword ptr [esi + 58h], ecx
        push   edi
        xchg   edi, esi
        add    edi, 14h
        mov    ecx, 10h
        rep movsd
        pop    edi
        pop    ecx
        ret
loc_1_334E:
        call   sub_1_34DE
        pop    ecx
        ret
loc_1_3355:
        mov    eax, edx
        db  087h, 005h, 024h, 07Dh, 000h, 000h
        mov    dword ptr [edx], eax
        pop    esi
        pop    edi
        db  0FFh, 005h, 00Ch, 07Dh, 000h, 000h
        pop    ecx
        ret
loc_1_3369:
        db  0FFh, 005h, 00Ch, 07Dh, 000h, 000h
        call   sub_1_32A2
        db  0FFh, 00Dh, 00Ch, 07Dh, 000h, 000h
        jns    SHORT loc_1_3394
        db  0FFh, 005h, 00Ch, 07Dh, 000h, 000h
loc_1_3382:
        pop    ecx
        ret
loc_1_3384:
        cmp    word ptr [edi + 3eh], 1000h
        je     SHORT loc_1_3382
        db  0FFh, 00Dh, 00Ch, 07Dh, 000h, 000h
        js     SHORT loc_1_3369
loc_1_3394:
        push   edi
        push   esi
        db  08Bh, 015h, 024h, 07Dh, 000h, 000h
        mov    esi, dword ptr [edx]
        db  089h, 035h, 024h, 07Dh, 000h, 000h
        mov    dword ptr [edx + 54h], ecx
        push   eax
        push   edx
        mov    eax, dword ptr [edi + 32h]
        call   sub_1_2D10
        pop    edx
        pop    eax
        jb     SHORT loc_1_3355
        mov    dword ptr [edx + 0ch], esi
        mov    ecx, edx
        xchg   dword ptr [esi + 1ch], ecx
        mov    dword ptr [edx + 8], ecx
        mov    ecx, 10h
        lea    esi, [edx + 14h]
        xchg   esi, edi
        rep movsd
        db  08Bh, 00Dh, 084h, 056h, 000h, 000h
        add    ecx, 2bf20h
        mov    dword ptr [edx + 58h], ecx
        mov    dword ptr [edx + 64h], ebx
        db  08Bh, 00Dh, 0D8h, 056h, 000h, 000h
        db  08Bh, 035h, 0D4h, 056h, 000h, 000h
        mov    dword ptr [esi + ecx*4], edx
        db  08Bh, 035h, 0D0h, 056h, 000h, 000h
        mov    di, word ptr [edx + 14h]
        mov    word ptr [esi + ecx*2], di
        db  0FFh, 005h, 0D8h, 056h, 000h, 000h
        pop    ecx
        jecxz  loc_1_3412
        mov    dword ptr [edx], 0
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        pop    edi
        pop    ecx
        ret
loc_1_3412:
        db  08Bh, 03Dh, 0C8h, 056h, 000h, 000h
        db  0FFh, 005h, 0C8h, 056h, 000h, 000h
        db  08Bh, 035h, 0C0h, 056h, 000h, 000h
        mov    dword ptr [esi + edi*4], eax
        db  08Bh, 035h, 0C4h, 056h, 000h, 000h
        lea    eax, [esi + edi*4]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], eax
        pop    edi
        pop    ecx
        ret
sub_1_343A:
        push   ebx
        push   edi
        mov    edi, esi
        call   sub_1_34AF
        mov    eax, edx
        mov    ebx, ecx
        call   sub_1_3488
        jb     SHORT loc_1_3484
        mov    eax, esi
        mov    edx, edi
loc_1_3452:
        cmp    dword ptr [eax + 54h], ebx
        jne    SHORT loc_1_3476
        lea    esi, [eax + 14h]
        mov    edi, edx
        mov    ecx, ebx
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_1_3476
        mov    ecx, ebx
        and    cl, 3
        repe cmpsb
        jne    SHORT loc_1_3476
        mov    esi, eax
        clc
        mov    ecx, ebx
        pop    edi
        pop    ebx
        ret
loc_1_3476:
        mov    esi, eax
        mov    eax, dword ptr [esi]
        or     eax, eax
        jne    SHORT loc_1_3452
loc_1_347E:
        stc
        mov    ecx, ebx
        pop    edi
        pop    ebx
        ret
loc_1_3484:
        xor    esi, esi
        jmp    SHORT loc_1_347E
sub_1_3488:
        cld
        push   edi
        db  08Bh, 03Dh, 0C0h, 056h, 000h, 000h
        db  08Bh, 00Dh, 0C8h, 056h, 000h, 000h
        mov    edx, ecx
        or     esi, esi
        repne scasd
        pop    edi
        jne    SHORT loc_1_34AD
        sub    edx, ecx
        db  08Bh, 035h, 0C4h, 056h, 000h, 000h
        mov    esi, dword ptr [esi + edx*4 - 4]
        clc
        ret
loc_1_34AD:
        stc
        ret
sub_1_34AF:
        push   edi
        mov    ecx, 30h
        xor    eax, eax
        lea    edi, [esi + 2]
        repne scasb
        neg    ecx
        lea    ecx, [ecx + 32h]
        push   ecx
        shr    ecx, 2
        xor    edx, edx
        jecxz  loc_1_34CE
loc_1_34C9:
        lodsd
        xor    edx, eax
        loop   loc_1_34C9
loc_1_34CE:
        mov    ecx, dword ptr [esp]
        and    cl, 3
        je     SHORT loc_1_34DB
loc_1_34D6:
        lodsb
        xor    dl, al
        loop   loc_1_34D6
loc_1_34DB:
        pop    ecx
        pop    edi
        ret
sub_1_34DE:
        push   edi
        mov    ecx, dword ptr [esi + 0ch]
        jecxz  loc_1_3504
        mov    eax, dword ptr [ecx + 1ch]
        cmp    eax, esi
        jne    SHORT loc_1_34F3
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ecx + 1ch], eax
        jmp    SHORT loc_1_3504
loc_1_34F3:
        mov    ecx, dword ptr [eax + 8]
        cmp    ecx, esi
        je     SHORT loc_1_34FE
        mov    eax, ecx
        jmp    SHORT loc_1_34F3
loc_1_34FE:
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [eax + 8], ecx
loc_1_3504:
        mov    eax, esi
        db  08Bh, 00Dh, 0D8h, 056h, 000h, 000h
        mov    edx, ecx
        db  08Bh, 03Dh, 0D4h, 056h, 000h, 000h
        push   edi
        repne scasd
        neg    ecx
        lea    ecx, [edx + ecx - 1]
        pop    edi
        mov    eax, dword ptr [edi + edx*4 - 4]
        mov    dword ptr [edi + ecx*4], eax
        db  08Bh, 03Dh, 0D0h, 056h, 000h, 000h
        mov    ax, word ptr [edi + edx*2 - 2]
        mov    word ptr [edi + ecx*2], ax
        db  0FFh, 00Dh, 0D8h, 056h, 000h, 000h
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    edi, esi
        db  087h, 03Dh, 024h, 07Dh, 000h, 000h
        mov    dword ptr [esi], edi
        db  0FFh, 005h, 00Ch, 07Dh, 000h, 000h
        jecxz  loc_1_3558
        mov    dword ptr [ecx + 4], eax
        pop    edi
        ret
loc_1_3558:
        mov    edx, eax
        db  02Bh, 015h, 0C4h, 056h, 000h, 000h
        db  08Bh, 00Dh, 0C8h, 056h, 000h, 000h
        shl    ecx, 2
        cmp    edx, ecx
        jb     SHORT loc_1_356F
        pop    edi
        ret
loc_1_356F:
        lea    edi, [ecx - 4]
        cmp    edi, edx
        je     SHORT loc_1_3595
        db  08Bh, 035h, 0C4h, 056h, 000h, 000h
        mov    edi, dword ptr [ecx + esi - 4]
        lea    esi, [edx + esi]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        db  08Bh, 035h, 0C0h, 056h, 000h, 000h
        mov    edi, dword ptr [ecx + esi - 4]
        mov    dword ptr [edx + esi], edi
loc_1_3595:
        db  0FFh, 00Dh, 0C8h, 056h, 000h, 000h
        pop    edi
        ret
loc_1_359D:
        mov    ax, word ptr [esp + 4]
        push   edi
        db  08Bh, 03Dh, 0D0h, 056h, 000h, 000h
        db  08Bh, 00Dh, 0D8h, 056h, 000h, 000h
        sub    ecx, edx
        push   ecx
        jecxz  loc_1_35C6
        lea    edi, [edi + edx*2]
        repne scasw
        jne    SHORT loc_1_35C6
        pop    eax
        sub    eax, ecx
        lea    edx, [eax + edx - 1]
        pop    edi
        jmp    SHORT loc_1_35F1
loc_1_35C6:
        pop    ecx
        pop    edi
        jmp    SHORT loc_1_3638
sub_1_35CA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 8]
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [eax + 4]
        push   dword ptr [eax + 16h]
        push   7
        db  03Bh, 015h, 0D8h, 056h, 000h, 000h
        jae    SHORT loc_1_3638
loc_1_35E9:
        cmp    word ptr [esp + 4], -1
        jne    SHORT loc_1_359D
loc_1_35F1:
        db  08Bh, 035h, 0D4h, 056h, 000h, 000h
        mov    esi, dword ptr [esi + edx*4]
        db  0A1h, 084h, 056h, 000h, 000h
        cmp    dword ptr [esi + 58h], eax
        jb     SHORT loc_1_364A
        mov    ecx, dword ptr [esi + 0ch]
        mov    eax, dword ptr [ebx + 4]
        test   dword ptr [ecx + 4], eax
        jne    SHORT loc_1_362D
        mov    ax, word ptr [esi + 52h]
        cmp    ah, 0fh
        je     SHORT loc_1_362D
        inc    ah
        mov    ecx, 10h
        add    esi, 14h
        rep movsd
        mov    word ptr [edi - 2], ax
        dec    dword ptr [esp]
        je     SHORT loc_1_3653
loc_1_362D:
        inc    edx
loc_1_362E:
        db  039h, 015h, 0D8h, 056h, 000h, 000h
        jne    SHORT loc_1_35E9
        xor    edx, edx
loc_1_3638:
        pop    ecx
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax + 8], edx
        neg    ecx
        lea    eax, [ecx + 7]
        pop    edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
loc_1_364A:
        push   edx
        call   sub_1_34DE
        pop    edx
        jmp    SHORT loc_1_362E
loc_1_3653:
        inc    edx
        db  039h, 015h, 0D8h, 056h, 000h, 000h
        jne    SHORT loc_1_3638
        xor    edx, edx
        jmp    SHORT loc_1_3638
loc_1_3660:
        push   0
        push   8ah
        push   eax
        db  0CDh, 020h, 00Bh, 000h, 028h, 000h
        ret
        db  000h
loc_1_3670:
        db  083h, 03Dh, 0C8h, 058h, 000h, 000h, 000h
        je     SHORT loc_1_368A
        db  08Bh, 03Dh, 0ECh, 058h, 000h, 000h
        mov    eax, dword ptr [edi + 0ch]
        and    al, 0feh
        mov    ecx, 0ah
        ret
loc_1_368A:
        xor    eax, eax
        ret
loc_1_368D:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
loc_1_3693:
        mov    eax, ebx
        movzx  ebx, word ptr [ebp + 10h]
        cmp    ebx, 1bh
        jae    NEAR PTR loc_1_3750
        db  0F6h, 083h, 07Ch, 057h, 000h, 000h, 001h
        je     SHORT loc_1_36F9
        push   eax
        movzx  ecx, word ptr [ebp + 38h]
        push   0
        push   ecx
        push   eax
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        movzx  edx, word ptr [ebp + 4]
        lea    esi, [edx + eax]
        movzx  edi, word ptr [ebp]
        add    edi, eax
        pop    eax
        db  0F6h, 083h, 07Ch, 057h, 000h, 000h, 002h
        je     SHORT loc_1_36F9
        shl    dword ptr [esi + 0ch], 10h
        cmp    ebx, 7
        je     SHORT loc_1_36E7
        db  0C7h, 046h, 018h, 071h, 049h, 000h, 000h
        mov    dword ptr [esi + 14h], eax
loc_1_36E7:
        mov    word ptr [esi + 0ch], 1
        mov    cx, word ptr [ebp + 38h]
        shl    ecx, 10h
        mov    cx, dx
        mov    dword ptr [esi], ecx
loc_1_36F9:
        movzx  edx, word ptr [ebp + 14h]
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_1_3756
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 000h, 000h, 002h, 000h
        je     SHORT loc_1_371E
        push   dword ptr [ebp + 2ch]
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
loc_1_371E:
        xor    ecx, ecx
        db  0FFh, 014h, 09Dh, 0F0h, 056h, 000h, 000h
loc_1_3727:
        mov    word ptr [ebp + 1ch], ax
        mov    word ptr [ebp + 18h], cx
        mov    word ptr [ebp + 14h], dx
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 000h, 000h, 002h, 000h
        jne    SHORT loc_1_3740
        ret
loc_1_3740:
        pop    eax
        test   eax, 200h
        je     SHORT loc_1_3749
        ret
loc_1_3749:
        db  0CDh, 020h, 01Bh, 000h, 001h, 000h
        ret
loc_1_3750:
        mov    ax, 1
        jmp    SHORT loc_1_3727
loc_1_3756:
        mov    ax, 2
        jmp    SHORT loc_1_3727
loc_1_375C:
        cmp    word ptr [ebp + 1ch], 7a00h
        je     SHORT loc_1_3788
        cmp    word ptr [ebp + 1ch], 1684h
        je     SHORT loc_1_377E
loc_1_376C:
        db  08Bh, 00Dh, 004h, 059h, 000h, 000h
        db  08Bh, 015h, 008h, 059h, 000h, 000h
        db  0CDh, 020h, 049h, 080h, 001h, 000h
loc_1_377E:
        cmp    word ptr [ebp + 10h], 200h
        jne    SHORT loc_1_376C
        jmp    SHORT loc_1_378C
loc_1_3788:
        mov    byte ptr [ebp + 1ch], 0ffh
loc_1_378C:
        db  0A1h, 000h, 059h, 000h, 000h
        mov    word ptr [ebp], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
        db  0CDh, 020h, 047h, 080h, 001h, 000h
loc_1_37A2:
        db  0CDh, 020h, 04Ah, 000h, 001h, 000h
loc_1_37A8:
        mov    eax, ebx
        movzx  ebx, word ptr [ebp + 10h]
        cmp    ebx, 1bh
        jae    NEAR PTR loc_1_3869
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 001h, 000h, 000h, 000h
        je     NEAR PTR loc_1_3863
        db  0F6h, 083h, 07Ch, 057h, 000h, 000h, 001h
        je     SHORT loc_1_3815
        movzx  edx, word ptr [ebp + 38h]
        shl    edx, 4
        add    edx, dword ptr [eax + 4]
        movzx  esi, word ptr [ebp + 4]
        add    esi, edx
        movzx  edi, word ptr [ebp]
        add    edi, edx
        db  0F6h, 083h, 07Ch, 057h, 000h, 000h, 002h
        je     SHORT loc_1_3815
        shl    dword ptr [esi + 0ch], 10h
        cmp    ebx, 7
        je     SHORT loc_1_3802
        db  0C7h, 046h, 018h, 071h, 049h, 000h, 000h
        mov    dword ptr [esi + 14h], eax
loc_1_3802:
        mov    word ptr [esi + 0ch], 2
        mov    cx, word ptr [ebp + 38h]
        shl    ecx, 10h
        mov    cx, word ptr [ebp + 4]
        mov    dword ptr [esi], ecx
loc_1_3815:
        movzx  edx, word ptr [ebp + 14h]
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 000h, 000h, 002h, 000h
        je     SHORT loc_1_382E
        push   dword ptr [ebp + 2ch]
        db  0CDh, 020h, 01Ah, 000h, 001h, 000h
loc_1_382E:
        xor    ecx, ecx
        db  0FFh, 014h, 09Dh, 0F0h, 056h, 000h, 000h
loc_1_3837:
        mov    word ptr [ebp + 1ch], ax
        mov    word ptr [ebp + 18h], cx
        mov    word ptr [ebp + 14h], dx
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 000h, 000h, 002h, 000h
        jne    SHORT loc_1_3851
        clc
        ret
loc_1_3851:
        pop    eax
        test   eax, 200h
        je     SHORT loc_1_385B
        clc
        ret
loc_1_385B:
        db  0CDh, 020h, 01Bh, 000h, 001h, 000h
        clc
        ret
loc_1_3863:
        mov    ax, 2
        jmp    SHORT loc_1_3837
loc_1_3869:
        mov    ax, 1
        stc
        ret
loc_1_386F:
        cmp    word ptr [ebp + 1ch], 7a00h
        je     SHORT loc_1_3879
        stc
        ret
loc_1_3879:
        mov    byte ptr [ebp + 1ch], 0ffh
        db  0A1h, 0FCh, 058h, 000h, 000h
        mov    word ptr [ebp], ax
        shr    eax, 10h
        mov    word ptr [ebp + 38h], ax
        db  0A1h, 030h, 055h, 000h, 000h
        mov    word ptr [ebp + 10h], ax
        clc
        ret
loc_1_3898:
        mov    byte ptr [esi + 9], 1
        mov    byte ptr [esi + 8], 0
        ret
loc_1_38A1:
        mov    byte ptr [esi + 9], 0fdh
        mov    byte ptr [esi + 8], 0
        ret
sub_1_38AA:
        and    dword ptr [eax + 4], 0fffffdffh
        or     dword ptr [eax + 4], 40000h
        cmp    word ptr [esi + 22h], 1
        jb     SHORT loc_1_38A1
        cmp    word ptr [esi + 28h], 2ah
        jb     SHORT loc_1_38A1
        mov    byte ptr [esi + 9], 0
        mov    byte ptr [esi + 8], 0ffh
        mov    ebx, eax
        push   esi
        call   sub_1_4488
        or     eax, eax
        je     SHORT loc_1_3898
        pop    edx
        mov    edi, dword ptr [eax + 8]
        mov    edi, dword ptr [edi + 4]
        mov    byte ptr [edi + 1fh], 0feh
        jmp    NEAR PTR loc_1_396A
loc_1_38EA:
        mov    byte ptr [esi + 9], 0fdh
        mov    edx, eax
        call   sub_1_4619
        mov    al, 0fdh
        db  066h, 0FFh, 005h, 052h, 059h, 000h, 000h
        ret
loc_1_38FF:
        mov    byte ptr [esi + 9], 1
        jmp    SHORT loc_1_390F
loc_1_3905:
        mov    byte ptr [esi + 9], 0eeh
        jmp    SHORT loc_1_390F
loc_1_390B:
        mov    byte ptr [esi + 9], 0fdh
loc_1_390F:
        mov    byte ptr [esi + 8], 0
        db  066h, 0FFh, 005h, 052h, 059h, 000h, 000h
        ret
sub_1_391B:
        or     dword ptr [esi + 0ch], 80h
        db  0FFh, 005h, 04Ah, 059h, 000h, 000h
        db  0FFh, 005h, 01Ch, 059h, 000h, 000h
        push   edx
        call   sub_1_42D3
        pop    edx
        jb     SHORT loc_1_3905
        test   dword ptr [eax + 4], 200h
        je     SHORT loc_1_3905
        cmp    word ptr [esi + 28h], 2ah
        jb     SHORT loc_1_390B
        mov    byte ptr [esi + 9], 0
        mov    ebx, eax
        push   esi
        call   sub_1_4488
        or     eax, eax
        je     SHORT loc_1_38FF
        pop    edx
        jb     SHORT loc_1_390B
        push   eax
        mov    eax, dword ptr [eax + 4]
        db  08Bh, 015h, 0ECh, 058h, 000h, 000h
        cmp    dword ptr [edx + 10h], eax
        pop    eax
        jb     SHORT loc_1_38EA
loc_1_396A:
        mov    byte ptr [esi + 8], 0fah
        mov    edx, esi
        lea    esi, [ebx + 38h]
        lea    edi, [edx + 1ch]
        movsd
        movsw
        mov    edi, dword ptr [eax + 8]
        mov    edi, dword ptr [edi + 4]
        push   edi
        and    byte ptr [edi + 1eh], 1fh
        or     byte ptr [edi + 1eh], 40h
        mov    ecx, dword ptr [ebx + 5eh]
        mov    dword ptr [edi + 20h], ecx
        mov    byte ptr [edi + 5], 5
        add    edi, 6
        lea    esi, [ebx + 44h]
        movsd
        movsd
        movsd
        pop    edi
        xor    ecx, ecx
        mov    dword ptr [edx + 10h], ecx
        mov    esi, edx
        cmp    dword ptr [ebx + 24h], ecx
        jne    SHORT loc_1_39F9
        test   dword ptr [ebx + 4], 80h
        jne    SHORT loc_1_39F9
        test   dword ptr [ebx + 4], 20h
        jne    SHORT loc_1_3A1C
        mov    ecx, dword ptr [ebx + 30h]
        mov    dword ptr [ebx + 34h], ecx
        mov    ecx, dword ptr [ebx + 1ch]
        mov    dword ptr [ebx + 20h], ecx
        or     dword ptr [ebx + 4], 1
        push   eax
        mov    eax, ebx
        mov    dx, word ptr [eax + 62h]
        mov    word ptr [edi + 24h], dx
        mov    dword ptr [ebx + 24h], esi
        mov    dword ptr [ebx + 28h], esi
        add    dh, 1
        adc    dl, 0
        mov    word ptr [eax + 62h], dx
        call   sub_1_441F
        mov    ebx, eax
        pop    eax
        mov    edi, dword ptr [ebx + 8]
        mov    dx, word ptr [ebx + 5ah]
        jmp    NEAR PTR sub_1_41D1
loc_1_39F9:
        mov    edi, dword ptr [ebx + 8]
        mov    dx, word ptr [ebx + 5ah]
        call   sub_1_41D1
        cmp    dword ptr [ebx + 24h], 0
        jne    SHORT loc_1_3A12
        mov    dword ptr [ebx + 24h], esi
        mov    dword ptr [ebx + 28h], esi
        ret
loc_1_3A12:
        mov    ecx, dword ptr [ebx + 28h]
        mov    dword ptr [ecx + 10h], esi
        mov    dword ptr [ebx + 28h], esi
        ret
loc_1_3A1C:
        db  0FFh, 005h, 04Eh, 059h, 000h, 000h
        jmp    SHORT loc_1_39F9
loc_1_3A24:
        test   dword ptr [eax + 4], 10000h
        je     SHORT loc_1_3A64
        test   dword ptr [eax + 4], 100000h
        jne    SHORT loc_1_3A64
        or     dword ptr [eax + 4], 100000h
        db  0BEh, 08Bh, 03Ch, 000h, 000h
        mov    edx, eax
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    cx, word ptr [ebx + 20h]
        mov    word ptr [edx + 60h], cx
        mov    cx, word ptr [ebx + 1ch]
        mov    word ptr [edx + 4eh], cx
        jmp    SHORT loc_1_3A64
loc_1_3A5C:
        mov    dword ptr [eax + 68h], ecx
        jmp    NEAR PTR loc_1_3B26
loc_1_3A64:
        pop    edx
        pop    esi
        pop    edi
loc_1_3A67:
        stc
        ret
sub_1_3A69:
        cmp    edx, 2ah
        jb     SHORT loc_1_3A67
        cmp    byte ptr [ebx + 5], 5
        jne    SHORT loc_1_3A67
        db  0FFh, 005h, 05Eh, 059h, 000h, 000h
        push   eax
        push   dword ptr [ebx + 22h]
        call   sub_1_42D3
        pop    ecx
        jb     NEAR PTR loc_1_3BE0
        xchg   dword ptr [esp], edi
        push   esi
        push   edx
        cmp    dword ptr [eax + 0ch], edi
        jne    SHORT loc_1_3A64
        test   dword ptr [eax + 4], 100h
        je     SHORT loc_1_3A24
        or     dword ptr [eax + 4], 2
        and    dword ptr [eax + 4], 0ffffff7fh
        mov    edx, dword ptr [ebx + 26h]
        mov    ecx, edx
        xchg   dword ptr [eax + 68h], ecx
        cmp    cx, dx
        je     SHORT loc_1_3AF4
        cmp    word ptr [eax + 62h], dx
        jne    SHORT loc_1_3A5C
        test   dword ptr [eax + 4], 1
        je     SHORT loc_1_3AF4
        and    dword ptr [eax + 4], 0fffffffeh
        mov    dword ptr [eax + 20h], 0ffffffffh
        mov    edx, dword ptr [eax + 24h]
        mov    esi, dword ptr [edx + 10h]
        mov    dword ptr [eax + 24h], esi
        mov    edx, dword ptr [edx]
        add    edx, 58h
        db  08Bh, 035h, 08Ch, 058h, 000h, 000h
        db  089h, 015h, 08Ch, 058h, 000h, 000h
        mov    dword ptr [esi], edx
        mov    dword ptr [edx + 4], esi
        db  0C7h, 002h, 088h, 058h, 000h, 000h
loc_1_3AF4:
        mov    dx, word ptr [ebx + 28h]
        add    dh, 1
        adc    dl, 0
        cmp    word ptr [eax + 62h], dx
        je     SHORT loc_1_3B7E
        and    dword ptr [eax + 4], 0ffffffdfh
        test   dword ptr [eax + 4], 9
        jne    SHORT loc_1_3B26
        cmp    dword ptr [eax + 24h], 0
        je     SHORT loc_1_3B26
        mov    edx, eax
        db  087h, 015h, 090h, 058h, 000h, 000h
        mov    dword ptr [eax + 14h], edx
        or     dword ptr [eax + 4], 8
loc_1_3B26:
        xor    ecx, ecx
        test   byte ptr [ebx + 1eh], 80h
        jne    SHORT loc_1_3B8F
        mov    dx, word ptr [ebx + 24h]
        cmp    word ptr [eax + 64h], dx
        jne    SHORT loc_1_3B84
        mov    dl, byte ptr [ebx + 1fh]
        cmp    dl, 0ffh
        je     SHORT loc_1_3BA9
        dec    dword ptr [edi + 24h]
        js     SHORT loc_1_3B9E
        add    byte ptr [eax + 65h], 1
        adc    byte ptr [eax + 64h], 0
        mov    ecx, dword ptr [edi + 28h]
        mov    esi, dword ptr [ecx + 10h]
        mov    dword ptr [edi + 28h], esi
        cmp    dl, 0feh
        je     SHORT loc_1_3BBB
loc_1_3B5B:
        or     dword ptr [eax + 4], 40h
        test   dword ptr [eax + 4], 8
        jne    SHORT loc_1_3B77
        mov    edx, eax
        db  087h, 015h, 090h, 058h, 000h, 000h
        mov    dword ptr [eax + 14h], edx
        or     dword ptr [eax + 4], 8
loc_1_3B77:
        cmp    ecx, 1
loc_1_3B7A:
        pop    edx
        pop    esi
        pop    edi
        ret
loc_1_3B7E:
        or     dword ptr [eax + 4], 20h
        jmp    SHORT loc_1_3B26
loc_1_3B84:
        inc    word ptr [eax + 6eh]
        db  066h, 0FFh, 005h, 064h, 059h, 000h, 000h
loc_1_3B8F:
        cmp    byte ptr [ebx + 1fh], 0ffh
        je     SHORT loc_1_3BA9
loc_1_3B95:
        test   byte ptr [ebx + 1eh], 40h
        jne    SHORT loc_1_3B5B
        stc
        jmp    SHORT loc_1_3B7A
loc_1_3B9E:
        or     dword ptr [eax + 4], 10h
        inc    dword ptr [edi + 24h]
        xor    ecx, ecx
        jmp    SHORT loc_1_3B5B
loc_1_3BA9:
        test   dword ptr [eax + 4], 40000h
        je     SHORT loc_1_3B95
        or     dword ptr [eax + 4], 800h
        jmp    SHORT loc_1_3B5B
loc_1_3BBB:
        or     dword ptr [eax + 4], 80000h
        and    dword ptr [eax + 4], 0fffffdffh
        mov    byte ptr [eax], 4
        jmp    SHORT loc_1_3B5B
loc_1_3BCE:
        db  066h, 0FFh, 005h, 062h, 059h, 000h, 000h
        stc
        ret
loc_1_3BD7:
        db  066h, 0FFh, 005h, 066h, 059h, 000h, 000h
        stc
        ret
loc_1_3BE0:
        pop    eax
        cmp    word ptr [ebx + 22h], -1
        jne    SHORT loc_1_3BCE
        cmp    byte ptr [eax + 17h], 0
        je     SHORT loc_1_3BD7
        push   edx
        push   edi
        push   esi
        push   eax
        xor    edx, edx
        mov    eax, dword ptr [eax + 2ch]
loc_1_3BF7:
        or     eax, eax
        je     SHORT loc_1_3C49
        test   dword ptr [eax + 4], 20000h
        je     SHORT loc_1_3C0B
        mov    edx, eax
loc_1_3C06:
        mov    eax, dword ptr [eax + 10h]
        jmp    SHORT loc_1_3BF7
loc_1_3C0B:
        mov    cx, word ptr [ebx + 20h]
        cmp    word ptr [eax + 60h], cx
        jne    SHORT loc_1_3C06
        lea    esi, [ebx + 12h]
        lea    edi, [eax + 44h]
        mov    ecx, 3
        repe cmpsd
        jne    SHORT loc_1_3C06
        or     dword ptr [eax + 4], 40h
        test   dword ptr [eax + 4], 8
        jne    SHORT loc_1_3C43
        db  08Bh, 015h, 090h, 058h, 000h, 000h
        mov    dword ptr [eax + 14h], edx
        db  0A3h, 090h, 058h, 000h, 000h
        or     dword ptr [eax + 4], 8
loc_1_3C43:
        pop    eax
        pop    edi
        pop    esi
        pop    edx
        stc
        ret
loc_1_3C49:
        pop    eax
        dec    byte ptr [eax + 17h]
        and    dword ptr [edx + 4], 0fffdffffh
        mov    cx, word ptr [ebx + 20h]
        mov    word ptr [edx + 60h], cx
        lea    esi, [ebx + 12h]
        lea    edi, [edx + 44h]
        movsd
        movsd
        movsd
        pop    esi
        lea    edi, [edx + 38h]
        lodsd
        pop    ecx
        and    al, byte ptr [ecx + 22h]
        stosd
        movsw
        mov    edi, ecx
        mov    dword ptr [edx + 8], edi
        db  0BEh, 08Bh, 03Ch, 000h, 000h
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        pop    edx
        stc
        ret
loc_1_3C84:
        cmp    al, 0ffh
        jne    SHORT loc_1_3C89
        ret
loc_1_3C89:
        int3
        ret
loc_1_3C8B:
        mov    edi, edx
        test   dword ptr [edi + 4], 1
        jne    SHORT loc_1_3CB5
        test   dword ptr [edi + 4], 200000h
        jne    SHORT loc_1_3CB5
        push   0
        mov    edx, esp
        push   edi
        push   edx
        call   sub_1_6360
        add    esp, 8
        pop    edx
        cmp    al, 0
        jne    SHORT loc_1_3C84
        mov    dword ptr [edi + 2ch], edx
loc_1_3CB5:
        push   edi
        call   sub_1_0A34
        add    esp, 4
        mov    eax, dword ptr [edi + 4]
        and    dword ptr [edi + 4], 0fffefffeh
        mov    ebx, dword ptr [edi + 24h]
        mov    dword ptr [edi + 24h], 0
        mov    cx, word ptr [edi + 5eh]
        mov    word ptr [ebx + 0eh], cx
        test   eax, 1
        jne    SHORT loc_1_3D28
        mov    eax, edi
        mov    edx, dword ptr [eax + 18h]
        call   sub_1_3F7A
        db  08Bh, 00Dh, 054h, 058h, 000h, 000h
        or     ecx, ecx
        jne    SHORT loc_1_3D05
        db  0BEh, 033h, 03Eh, 000h, 000h
        mov    eax, 64h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
loc_1_3D05:
        db  0FFh, 005h, 054h, 058h, 000h, 000h
        mov    dx, word ptr [edi + 2]
        db  08Bh, 035h, 068h, 058h, 000h, 000h
        mov    word ptr [esi + ecx*2], dx
        db  08Bh, 035h, 06Ch, 058h, 000h, 000h
        mov    dword ptr [esi + ecx*4], edi
        mov    eax, dword ptr [edi + 0ch]
        inc    byte ptr [eax + 16h]
loc_1_3D28:
        db  0A1h, 054h, 058h, 000h, 000h
        db  066h, 039h, 005h, 040h, 059h, 000h, 000h
        jae    SHORT loc_1_3D3C
        db  066h, 0A3h, 040h, 059h, 000h, 000h
loc_1_3D3C:
        mov    edx, dword ptr [ebx]
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [edx + 40h], eax
        call   sub_1_4619
        ret
sub_1_3D4A:
        test   dword ptr [edx + 4], 80000000h
        jne    SHORT loc_1_3D81
        push   0
        push   edx
        test   dword ptr [edx + 4], 80000h
        je     SHORT loc_1_3D79
        lea    edi, [edx + 3eh]
        mov    eax, edx
        mov    byte ptr [eax + 5dh], 0ffh
        and    byte ptr [eax + 5ch], 7fh
        call   sub_1_441F
        mov    edx, dword ptr [eax + 18h]
        call   sub_1_3F7A
loc_1_3D79:
        call   sub_1_0BD0
        add    esp, 8
loc_1_3D81:
        ret
sub_1_3D82:
        push   ebx
        push   edi
loc_1_3D84:
        db  0A1h, 090h, 058h, 000h, 000h
        or     eax, eax
        je     NEAR PTR loc_1_3E13
        mov    edx, dword ptr [eax + 14h]
        db  089h, 015h, 090h, 058h, 000h, 000h
        and    dword ptr [eax + 4], 0fffffff7h
        cmp    dword ptr [eax + 24h], 0
        je     SHORT loc_1_3DE4
        test   dword ptr [eax + 4], 0a1h
        jne    SHORT loc_1_3DED
        or     dword ptr [eax + 4], 1
        and    dword ptr [eax + 4], 0ffffffbfh
        mov    edx, dword ptr [eax + 30h]
        mov    dword ptr [eax + 34h], edx
        mov    edx, dword ptr [eax + 1ch]
        mov    dword ptr [eax + 20h], edx
        mov    edx, dword ptr [eax + 24h]
        mov    edx, dword ptr [edx]
        call   sub_1_4417
        mov    cx, word ptr [eax + 62h]
        mov    word ptr [edi + 24h], cx
        add    ch, 1
        adc    cl, 0
        mov    word ptr [eax + 62h], cx
        call   sub_1_3F7A
        jmp    SHORT loc_1_3D84
loc_1_3DE4:
        test   dword ptr [eax + 4], 80800h
        jne    SHORT loc_1_3E16
loc_1_3DED:
        test   dword ptr [eax + 4], 800h
        jne    SHORT loc_1_3E16
        and    dword ptr [eax + 4], 0ffffffbfh
        lea    edi, [eax + 3eh]
        mov    byte ptr [edi + 1eh], 80h
        call   sub_1_441F
        mov    edx, dword ptr [eax + 18h]
        call   sub_1_3F7A
        jmp    NEAR PTR loc_1_3D84
loc_1_3E13:
        pop    edi
        pop    ebx
        ret
loc_1_3E16:
        push   esi
        mov    edx, eax
        call   sub_1_3D4A
        pop    esi
        jmp    NEAR PTR loc_1_3D84
loc_1_3E24:
        db  083h, 025h, 0F8h, 058h, 000h, 000h, 0FBh
        ret
loc_1_3E2C:
        call   sub_1_3D82
        jmp    SHORT loc_1_3E3D
loc_1_3E33:
        cld
        db  083h, 03Dh, 090h, 058h, 000h, 000h, 000h
        jne    SHORT loc_1_3E2C
loc_1_3E3D:
        db  08Bh, 00Dh, 054h, 058h, 000h, 000h
        jecxz  loc_1_3E24
        db  08Bh, 035h, 06Ch, 058h, 000h, 000h
loc_1_3E4B:
        lodsd
        dec    dword ptr [eax + 20h]
        je     SHORT loc_1_3E70
        loop   loc_1_3E4B
loc_1_3E53:
        db  0FFh, 00Dh, 0C0h, 058h, 000h, 000h
        je     NEAR PTR loc_1_3EFC
loc_1_3E5F:
        db  0BEh, 033h, 03Eh, 000h, 000h
        mov    eax, 64h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        ret
loc_1_3E70:
        push   ecx
        dec    dword ptr [eax + 34h]
        js     SHORT loc_1_3EB9
        test   dword ptr [eax + 4], 1
        jne    SHORT loc_1_3E99
        test   dword ptr [eax + 4], 80h
        je     SHORT loc_1_3EB4
        inc    word ptr [eax + 6ch]
        mov    edx, dword ptr [eax + 18h]
        lea    edi, [eax + 3eh]
        call   sub_1_441F
        jmp    SHORT loc_1_3EA7
loc_1_3E99:
        mov    edx, dword ptr [eax + 24h]
        mov    edx, dword ptr [edx]
        call   sub_1_4417
        or     byte ptr [edi + 1eh], 40h
loc_1_3EA7:
        mov    ecx, dword ptr [eax + 1ch]
        add    ecx, ecx
        mov    dword ptr [eax + 20h], ecx
        call   sub_1_3F7A
loc_1_3EB4:
        pop    ecx
        loop   loc_1_3E4B
        jmp    SHORT loc_1_3E53
loc_1_3EB9:
        test   dword ptr [eax + 4], 1
        je     SHORT loc_1_3ECB
        db  066h, 0FFh, 005h, 054h, 059h, 000h, 000h
        jmp    SHORT loc_1_3ED2
loc_1_3ECB:
        db  066h, 0FFh, 005h, 068h, 059h, 000h, 000h
loc_1_3ED2:
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 2]
        push   dword ptr [eax + 0ch]
        push   eax
        call   sub_1_0BD0
        pop    eax
        call   sub_1_0CF9
        add    esp, 0ch
        pop    ecx
        dec    ecx
        jne    NEAR PTR loc_1_3E4B
        jmp    NEAR PTR loc_1_3E53
loc_1_3EF7:
        jmp    NEAR PTR loc_1_3E5F
loc_1_3EFC:
        db  0C7h, 005h, 0C0h, 058h, 000h, 000h, 037h, 000h, 000h, 000h
        db  08Bh, 00Dh, 054h, 058h, 000h, 000h
        jecxz  loc_1_3EF7
        db  08Bh, 035h, 06Ch, 058h, 000h, 000h
loc_1_3F14:
        lodsd
        test   dword ptr [eax + 4], 400h
        je     SHORT loc_1_3F6F
        test   dword ptr [eax + 4], 81h
        jne    SHORT loc_1_3F6F
        test   dword ptr [eax + 4], 2
        jne    SHORT loc_1_3F4A
        or     dword ptr [eax + 4], 80h
        mov    dword ptr [eax + 34h], 0ah
        mov    edx, dword ptr [eax + 1ch]
        mov    dword ptr [eax + 20h], edx
        mov    byte ptr [eax + 5ch], 0c0h
        jmp    SHORT loc_1_3F57
loc_1_3F4A:
        test   dword ptr [eax + 4], 4
        jne    SHORT loc_1_3F6F
        mov    byte ptr [eax + 5ch], 80h
loc_1_3F57:
        push   ecx
        lea    edi, [eax + 3eh]
        call   sub_1_441F
        and    dword ptr [eax + 4], 0ffffffbfh
        mov    edx, dword ptr [eax + 18h]
        push   eax
        call   sub_1_3F7A
        pop    eax
        pop    ecx
loc_1_3F6F:
        and    dword ptr [eax + 4], 0fffffff9h
        loop   loc_1_3F14
        jmp    NEAR PTR loc_1_3E5F
sub_1_3F7A:
        or     dword ptr [eax + 4], 4
        test   byte ptr [edx + 3ch], 4
        je     SHORT loc_1_3F89
        or     byte ptr [edx + 3ch], 40h
        ret
loc_1_3F89:
        mov    eax, dword ptr [eax + 8]
        push   edx
        push   0
        mov    ecx, esp
        push   edx
        mov    eax, dword ptr [eax + 48h]
        push   dword ptr [eax + 0ch]
        push   ecx
        or     byte ptr [edx + 3ch], 4
        db  0CDh, 020h, 02Dh, 000h, 028h, 000h
        pop    eax
        pop    edx
        cmp    eax, 103h
        jne    SHORT loc_1_3FAD
        ret
loc_1_3FAD:
        and    byte ptr [edx + 3ch], 0fbh
        ret
        db  000h, 000h
sub_1_3FB4:
        push   esi
        mov    esi, dword ptr [esp + 8]
        call   sub_1_3FF7
        pop    esi
        ret
loc_1_3FC0:
        mov    eax, 1
        mov    byte ptr [esi + 9], al
        mov    byte ptr [esi + 8], 0
        ret
loc_1_3FCD:
        mov    eax, 0ffh
        mov    byte ptr [esi + 9], al
        mov    byte ptr [esi + 8], 0
        ret
loc_1_3FDA:
        mov    eax, 0fdh
        mov    byte ptr [esi + 9], al
        mov    byte ptr [esi + 8], 0
        test   dword ptr [esi + 0ch], 80h
        je     SHORT loc_1_3FF6
        db  066h, 0FFh, 005h, 05Ch, 059h, 000h, 000h
loc_1_3FF6:
        ret
sub_1_3FF7:
        push   dword ptr [esi + 0ah]
        call   sub_1_42A4
        pop    edx
        jb     SHORT loc_1_3FCD
        mov    edx, dword ptr [esi + 14h]
        cmp    dword ptr [eax], edx
        jne    SHORT loc_1_3FCD
        db  0FFh, 005h, 036h, 059h, 000h, 000h
        push   eax
        or     dword ptr [esi + 0ch], 20h
        push   esi
        call   sub_1_4488
        pop    edx
        pop    edx
        jb     SHORT loc_1_3FDA
        or     eax, eax
        je     SHORT loc_1_3FC0
        test   dword ptr [esi + 0ch], 80h
        jne    SHORT loc_1_404B
        mov    ecx, dword ptr [edx + 20h]
        mov    dword ptr [esi + 10h], ecx
        mov    dword ptr [edx + 20h], esi
        mov    eax, dword ptr [edx + 8]
        db  003h, 005h, 064h, 058h, 000h, 000h
        or     byte ptr [eax], 1
loc_1_4040:
        mov    byte ptr [esi + 9], 0
        mov    byte ptr [esi + 8], 0feh
        xor    eax, eax
        ret
loc_1_404B:
        db  0FFh, 005h, 058h, 059h, 000h, 000h
        inc    dword ptr [edx + 24h]
        mov    ecx, dword ptr [edx + 28h]
        mov    dword ptr [esi + 10h], ecx
        mov    dword ptr [edx + 28h], esi
        cmp    dword ptr [edx + 24h], 1
        jne    SHORT loc_1_4040
        mov    ecx, dword ptr [edx + 2ch]
loc_1_4066:
        jecxz  loc_1_4040
        test   dword ptr [ecx + 4], 10h
        je     SHORT loc_1_408D
        and    dword ptr [ecx + 4], 0ffffffefh
        test   dword ptr [ecx + 4], 8
        jne    SHORT loc_1_4089
        mov    edi, ecx
        db  087h, 03Dh, 090h, 058h, 000h, 000h
        mov    dword ptr [ecx + 14h], edi
loc_1_4089:
        or     dword ptr [ecx + 4], 48h
loc_1_408D:
        mov    ecx, dword ptr [ecx + 10h]
        jmp    SHORT loc_1_4066
sub_1_4092:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, dword ptr [esp + 10h]
        call   sub_1_410D
        pop    edi
        pop    esi
        ret
loc_1_40A4:
        push   edx
        call   sub_1_154B
        pop    edx
        ret
sub_1_40AC:
        push   esi
        db  0A1h, 03Ch, 058h, 000h, 000h
        mov    edx, dword ptr [esp + 8]
        db  0BEh, 0A4h, 040h, 000h, 000h
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        pop    esi
        ret
loc_1_40C3:
        mov    byte ptr [esi + 9], 1
        mov    byte ptr [esi + 8], 0
        mov    al, 1
        ret
loc_1_40CE:
        mov    byte ptr [esi + 9], 0fdh
        mov    byte ptr [esi + 8], 0
        mov    edx, eax
        call   sub_1_4619
        db  066h, 0FFh, 005h, 020h, 059h, 000h, 000h
        mov    al, 0fdh
        ret
loc_1_40E7:
        mov    edx, dword ptr [edi + 1ch]
        mov    dword ptr [esi + 1ch], edx
        mov    dx, word ptr [edi + 20h]
        mov    word ptr [esi + 20h], dx
        jmp    NEAR PTR loc_1_41B1
loc_1_40FA:
        mov    byte ptr [esi + 9], 4
        mov    edx, eax
        mov    dword ptr [eax + 40h], edi
        call   sub_1_4619
        mov    al, 4
        ret
sub_1_410B:
        xor    edi, edi
sub_1_410D:
        or     dword ptr [esi + 0ch], 10h
        mov    byte ptr [esi + 9], 0
        push   esi
        call   sub_1_4488
        pop    ecx
        jb     SHORT loc_1_40CE
        or     eax, eax
        je     SHORT loc_1_40C3
        cmp    word ptr [esi + 28h], 1eh
        jb     SHORT loc_1_40CE
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 000h, 080h, 000h, 000h
        je     SHORT loc_1_414D
        db  08Bh, 015h, 0A0h, 057h, 000h, 000h
        cmp    edx, dword ptr [esi + 1ch]
        jne    SHORT loc_1_414D
        db  0BAh, 0A0h, 057h, 000h, 000h
        mov    edx, dword ptr [edx + 2]
        cmp    edx, dword ptr [esi + 1eh]
        je     SHORT loc_1_40FA
loc_1_414D:
        or     edi, edi
        jne    SHORT loc_1_41A2
        db  08Bh, 03Dh, 0ECh, 058h, 000h, 000h
        db  08Bh, 00Dh, 0C8h, 058h, 000h, 000h
        jecxz  loc_1_40FA
        cmp    cl, 1
        je     SHORT loc_1_41A2
        mov    edx, dword ptr [eax + 8]
        mov    edx, dword ptr [edx + 4]
        mov    edx, dword ptr [edx + 6]
        or     edx, edx
        je     SHORT loc_1_41A2
        inc    ecx
loc_1_4172:
        cmp    dword ptr [edi + 18h], edx
        je     SHORT loc_1_41A2
        db  08Bh, 03Ch, 08Dh, 0D4h, 058h, 000h, 000h
        loop   loc_1_4172
        db  08Bh, 03Dh, 0ECh, 058h, 000h, 000h
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 010h, 000h, 000h, 000h
        je     SHORT loc_1_41A2
        push   eax
        push   esi
        mov    eax, edx
        call   sub_1_2D10
        jb     SHORT loc_1_41A0
        mov    edi, dword ptr [esi + 20h]
loc_1_41A0:
        pop    esi
        pop    eax
loc_1_41A2:
        mov    ecx, dword ptr [esi + 1ch]
        movzx  edx, word ptr [esi + 20h]
        or     edx, ecx
        je     NEAR PTR loc_1_40E7
loc_1_41B1:
        mov    dx, word ptr [esi + 0ah]
        test   dword ptr [edi + 8], 1
        je     NEAR PTR loc_1_40FA
        test   dword ptr [edi + 8], 10h
        jne    NEAR PTR loc_1_40FA
        mov    cl, 1
sub_1_41D1:
        mov    byte ptr [esi + 8], 0ffh
        mov    dword ptr [eax + 40h], edi
        push   ebx
        push   ecx
        push   edi
        mov    ebx, eax
        push   dword ptr [ebx + 4]
        lea    ecx, [ebx + 38h]
        push   ecx
        lea    eax, [esi + 1ch]
        push   eax
        call   dword ptr [edi + 28h]
        add    esp, 8
        pop    edi
        push   edx
        db  08Bh, 015h, 0B8h, 058h, 000h, 000h
        mov    ecx, dword ptr [edx]
        db  089h, 00Dh, 0B8h, 058h, 000h, 000h
        mov    dword ptr [edx], 0
        db  0FFh, 00Dh, 04Ch, 058h, 000h, 000h
        lea    ecx, [ebx + 58h]
        mov    dword ptr [edx + 4], ecx
        mov    ecx, dword ptr [ebx + 54h]
        mov    dword ptr [edx + 0ch], ecx
        push   edi
        mov    edi, dword ptr [ebx + 8]
        push   edx
        push   ebx
        db  0CDh, 020h, 023h, 000h, 028h, 000h
        pop    ecx
        pop    edx
        xchg   dword ptr [esp], esi
        mov    eax, dword ptr [ebx + 4]
        cmp    dword ptr [esi + 10h], eax
        jb     SHORT loc_1_4287
        mov    eax, dword ptr [edi + 4]
        xchg   cl, ch
        mov    word ptr [eax + 2], cx
        mov    byte ptr [eax + 4], 0
        mov    word ptr [eax + 1ch], dx
        db  08Bh, 035h, 0ECh, 058h, 000h, 000h
        add    esi, 18h
        lea    edi, [eax + 12h]
        movsd
        movsd
        movsw
        pop    esi
        mov    word ptr [eax], 0ffffh
        pop    ecx
        jecxz  loc_1_4283
        push   eax
        mov    edx, esp
        push   ebx
        mov    edi, dword ptr [ebx + 40h]
        mov    edi, dword ptr [edi + 48h]
        push   dword ptr [edi + 0ch]
        push   edx
        or     byte ptr [ebx + 3ch], 4
        db  0CDh, 020h, 02Dh, 000h, 028h, 000h
        db  0FFh, 005h, 01Ch, 059h, 000h, 000h
        pop    eax
        cmp    eax, 103h
        jne    SHORT loc_1_4298
        pop    ebx
        xor    eax, eax
        ret
loc_1_4283:
        pop    ebx
        xor    eax, eax
        ret
loc_1_4287:
        pop    esi
        mov    byte ptr [esi + 9], 0fdh
        mov    edx, ebx
        call   sub_1_4619
        pop    ecx
        pop    ebx
        mov    al, 0fdh
        ret
loc_1_4298:
        push   eax
        push   ebx
        push   edi
        call   sub_1_21B9
        pop    ebx
        xor    eax, eax
        ret
sub_1_42A4:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 00Dh, 040h, 058h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        jecxz  loc_1_42CF
        push   edi
        db  08Bh, 03Dh, 05Ch, 058h, 000h, 000h
        lea    edi, [edi + ecx*2 - 2]
        std
        repne scasw
        cld
        pop    edi
        jne    SHORT loc_1_42CF
        db  0A1h, 060h, 058h, 000h, 000h
        mov    eax, dword ptr [eax + ecx*4]
        clc
        ret
loc_1_42CF:
        xor    eax, eax
        stc
        ret
sub_1_42D3:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 00Dh, 054h, 058h, 000h, 000h
        jecxz  loc_1_42FC
        push   edi
        db  08Bh, 03Dh, 068h, 058h, 000h, 000h
        lea    edi, [edi + ecx*2 - 2]
        std
        repne scasw
        cld
        pop    edi
        jne    SHORT loc_1_42FC
        db  0A1h, 06Ch, 058h, 000h, 000h
        mov    eax, dword ptr [eax + ecx*4]
        clc
        ret
loc_1_42FC:
        xor    eax, eax
        stc
        ret
sub_1_4300:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 00Dh, 040h, 058h, 000h, 000h
        mov    ecx, dword ptr [ecx]
        jecxz  loc_1_434C
        lea    edx, [ecx - 1]
        push   edi
        db  08Bh, 03Dh, 05Ch, 058h, 000h, 000h
        lea    edi, [edi + ecx*2 - 2]
        std
        repne scasw
        cld
        jne    SHORT loc_1_434B
        db  08Bh, 03Dh, 060h, 058h, 000h, 000h
        push   dword ptr [edi + ecx*4]
        mov    eax, dword ptr [edi + edx*4]
        mov    dword ptr [edi + ecx*4], eax
        db  08Bh, 03Dh, 05Ch, 058h, 000h, 000h
        mov    ax, word ptr [edi + edx*2]
        mov    word ptr [edi + ecx*2], ax
        db  08Bh, 00Dh, 040h, 058h, 000h, 000h
        dec    dword ptr [ecx]
        pop    eax
        pop    edi
        ret
loc_1_434B:
        pop    edi
loc_1_434C:
        xor    eax, eax
        ret
sub_1_434F:
        mov    eax, dword ptr [esp + 4]
        db  08Bh, 00Dh, 054h, 058h, 000h, 000h
        jecxz  loc_1_4397
        lea    edx, [ecx - 1]
        push   edi
        db  08Bh, 03Dh, 068h, 058h, 000h, 000h
        lea    edi, [edi + ecx*2 - 2]
        std
        repne scasw
        cld
        jne    SHORT loc_1_4396
        db  08Bh, 03Dh, 06Ch, 058h, 000h, 000h
        push   dword ptr [edi + ecx*4]
        mov    eax, dword ptr [edi + edx*4]
        mov    dword ptr [edi + ecx*4], eax
        db  08Bh, 03Dh, 068h, 058h, 000h, 000h
        mov    ax, word ptr [edi + edx*2]
        mov    word ptr [edi + ecx*2], ax
        db  0FFh, 00Dh, 054h, 058h, 000h, 000h
        pop    eax
        pop    edi
        ret
loc_1_4396:
        pop    edi
loc_1_4397:
        xor    eax, eax
        ret
sub_1_439A:
        cld
        push   edi
        db  08Bh, 015h, 040h, 058h, 000h, 000h
        mov    edx, dword ptr [edx]
        xor    eax, eax
loc_1_43A6:
        db  066h, 0A1h, 0A6h, 057h, 000h, 000h
        xchg   ah, al
        mov    ecx, edx
        db  066h, 083h, 005h, 0A6h, 057h, 000h, 000h, 002h
        jb     SHORT loc_1_43C9
loc_1_43BA:
        jecxz  loc_1_43C7
        db  08Bh, 03Dh, 05Ch, 058h, 000h, 000h
        repne scasw
        je     SHORT loc_1_43A6
loc_1_43C7:
        pop    edi
        ret
loc_1_43C9:
        db  066h, 0C7h, 005h, 0A6h, 057h, 000h, 000h, 000h, 040h
        jmp    SHORT loc_1_43BA
sub_1_43D4:
        push   edi
loc_1_43D5:
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        movzx  eax, al
        db  003h, 005h, 0BCh, 058h, 000h, 000h
        inc    eax
        inc    eax
        db  0A3h, 0BCh, 058h, 000h, 000h
        and    ax, 0fffeh
        db  08Bh, 00Dh, 040h, 058h, 000h, 000h
        mov    ecx, dword ptr [ecx]
loc_1_43F7:
        dec    ecx
        js     SHORT loc_1_4415
        db  08Bh, 03Dh, 060h, 058h, 000h, 000h
        mov    edi, dword ptr [edi + ecx*4]
        mov    edi, dword ptr [edi + 2ch]
loc_1_4406:
        or     edi, edi
        je     SHORT loc_1_43F7
        cmp    ax, word ptr [edi + 2]
        je     SHORT loc_1_43D5
        mov    edi, dword ptr [edi + 10h]
        jmp    SHORT loc_1_4406
loc_1_4415:
        pop    edi
        ret
sub_1_4417:
        mov    edi, dword ptr [edx + 8]
        mov    edi, dword ptr [edi]
        mov    edi, dword ptr [edi + 4]
sub_1_441F:
        mov    ebx, dword ptr [eax + 0ch]
        mov    ecx, dword ptr [ebx + 24h]
        jecxz  loc_1_443B
loc_1_4427:
        mov    bx, word ptr [eax + 64h]
        mov    word ptr [edi + 26h], bx
        xchg   bh, bl
        add    ecx, ebx
        dec    ecx
        xchg   ch, cl
        mov    word ptr [edi + 28h], cx
        ret
loc_1_443B:
        or     dword ptr [eax + 4], 10h
        jmp    SHORT loc_1_4427
loc_1_4441:
        push   ecx
        call   sub_1_1CDF
        pop    ecx
        db  039h, 00Dh, 04Ch, 058h, 000h, 000h
        jg     NEAR PTR loc_1_44F5
        db  0FFh, 005h, 048h, 058h, 000h, 000h
        db  08Bh, 035h, 0B4h, 058h, 000h, 000h
        mov    dword ptr [edi + 38h], esi
        db  089h, 03Dh, 0B4h, 058h, 000h, 000h
loc_1_4469:
        pop    esi
        pop    edi
        pop    ebx
        xor    eax, eax
        ret
loc_1_446F:
        push   ecx
        call   sub_1_1C7D
        pop    ecx
        db  083h, 03Dh, 048h, 058h, 000h, 000h, 000h
        jg     SHORT loc_1_4498
        jmp    SHORT loc_1_4469
loc_1_4481:
        mov    eax, edi
        pop    esi
        pop    edi
        pop    ebx
        stc
        ret
sub_1_4488:
        push   ebx
        push   edi
        push   esi
        mov    ebx, dword ptr [esp + 10h]
        db  083h, 03Dh, 048h, 058h, 000h, 000h, 003h
        jl     SHORT loc_1_446F
loc_1_4498:
        db  0FFh, 00Dh, 048h, 058h, 000h, 000h
        db  08Bh, 03Dh, 0B4h, 058h, 000h, 000h
        mov    esi, dword ptr [edi + 38h]
        db  089h, 035h, 0B4h, 058h, 000h, 000h
        lea    esi, [edi + 4ch]
        mov    eax, esi
        db  087h, 005h, 094h, 058h, 000h, 000h
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [esi], eax
        db  0C7h, 046h, 004h, 094h, 058h, 000h, 000h
        mov    edx, dword ptr [ebx]
        mov    dword ptr [edi + 48h], edx
        mov    dword ptr [edi + 44h], ebx
        mov    dword ptr [ebx], edi
        mov    dx, word ptr [ebx + 0ch]
        and    dl, 83h
        mov    byte ptr [edi + 3ch], dl
        test   word ptr [ebx + 0ch], 840h
        jne    SHORT loc_1_4481
        movzx  ecx, word ptr [ebx + 22h]
        jecxz  loc_1_4481
        lea    eax, [ecx + 14h]
        db  039h, 005h, 04Ch, 058h, 000h, 000h
        jbe    NEAR PTR loc_1_4441
loc_1_44F5:
        push   edi
        db  029h, 00Dh, 04Ch, 058h, 000h, 000h
        lea    esi, [ebx + 24h]
        db  0A1h, 0B8h, 058h, 000h, 000h
        push   ecx
        push   eax
loc_1_4506:
        mov    eax, dword ptr [eax]
        loop   loc_1_4506
        db  0A3h, 0B8h, 058h, 000h, 000h
        pop    eax
        pop    ecx
        test   dword ptr [ebx + 0ch], 3
        je     NEAR PTR loc_1_45A6
        push   eax
        push   ecx
        mov    edi, eax
        mov    eax, ebx
        mov    ecx, 24h
        call   sub_1_45CE
        mov    ecx, dword ptr [ebx + 14h]
        mov    eax, edi
        test   dword ptr [ebx + 0ch], 1
        jne    SHORT loc_1_456C
        mov    ebx, dword ptr [ecx + 4]
loc_1_453F:
        mov    edi, eax
        mov    edx, dword ptr [esi]
        movzx  eax, dx
        xor    dh, dh
        shr    edx, 0ch
        add    eax, edx
        add    eax, ebx
        mov    dword ptr [edi + 4], eax
        movzx  ecx, word ptr [esi + 4]
        mov    dword ptr [edi + 0ch], ecx
        call   sub_1_45CE
        add    esi, 6
        mov    eax, dword ptr [edi]
        dec    dword ptr [esp]
        jne    SHORT loc_1_453F
        pop    ecx
        pop    edx
        jmp    SHORT loc_1_45BD
loc_1_456C:
        mov    ebx, ecx
loc_1_456E:
        mov    edi, eax
        mov    edx, dword ptr [esi]
        movzx  ecx, dx
        push   ecx
        shr    edx, 10h
        push   0
        push   edx
        push   ebx
        db  0CDh, 020h, 06Eh, 000h, 001h, 000h
        add    esp, 0ch
        pop    ecx
        add    eax, ecx
        mov    dword ptr [edi + 4], eax
        mov    cx, word ptr [esi + 4]
        mov    dword ptr [edi + 0ch], ecx
        call   sub_1_45CE
        add    esi, 6
        mov    eax, dword ptr [edi]
        dec    dword ptr [esp]
        jne    SHORT loc_1_456E
        pop    ecx
        pop    edx
        jmp    SHORT loc_1_45BD
loc_1_45A6:
        mov    edx, eax
loc_1_45A8:
        mov    edi, eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 4], eax
        movzx  eax, word ptr [esi + 4]
        mov    dword ptr [edi + 0ch], eax
        add    esi, 6
        mov    eax, dword ptr [edi]
        loop   loc_1_45A8
loc_1_45BD:
        mov    dword ptr [edi], ecx
        pop    edi
        push   edx
        push   edi
        db  0CDh, 020h, 023h, 000h, 028h, 000h
        mov    eax, edi
        pop    esi
        pop    edi
        pop    ebx
        ret
sub_1_45CE:
        lea    ecx, [ecx + eax]
        shr    eax, 0ch
        shr    ecx, 0ch
        sub    ecx, eax
        inc    ecx
        push   100h
        push   ecx
        push   eax
        db  0CDh, 020h, 063h, 000h, 001h, 000h
        add    esp, 0ch
        ret
sub_1_45EB:
        lea    ecx, [ecx + eax]
        shr    eax, 0ch
        shr    ecx, 0ch
        sub    ecx, eax
        inc    ecx
        push   100h
        push   ecx
        push   eax
        db  0CDh, 020h, 064h, 000h, 001h, 000h
        add    esp, 0ch
        ret
sub_1_4608:
        push   esi
        push   ebx
        mov    ebx, dword ptr [esp + 0ch]
        jmp    SHORT loc_1_4677
loc_1_4610:
        and    byte ptr [edx + 3ch], 7fh
        ret
sub_1_4615:
        mov    edx, dword ptr [esp + 4]
sub_1_4619:
        test   byte ptr [edx + 3ch], 4
        jne    SHORT loc_1_4610
        push   esi
        push   ebx
        mov    eax, dword ptr [edx + 44h]
        mov    ecx, dword ptr [edx + 48h]
        push   edx
        or     eax, eax
        je     SHORT loc_1_4676
        mov    byte ptr [eax + 8], 0
        cmp    dword ptr [eax + 4], 0
        je     SHORT loc_1_466E
        mov    esi, dword ptr [edx + 40h]
        mov    dl, 0ffh
        mov    ebx, dword ptr [eax + 18h]
        test   dword ptr [eax + 0ch], 400h
        je     SHORT loc_1_464A
        mov    dword ptr [eax + 18h], edi
loc_1_464A:
        call   ebx
        jmp    SHORT loc_1_4676
loc_1_464E:
        ror    dword ptr [eax + 0ch], 10h
        test   word ptr [eax + 0eh], 400h
        je     SHORT loc_1_4676
        mov    ecx, dword ptr [edi + 44h]
        mov    dword ptr [eax + 10h], ecx
        mov    ecx, dword ptr [edi + 48h]
        mov    dword ptr [eax + 14h], ecx
        mov    ecx, dword ptr [edi + 4ch]
        mov    dword ptr [eax + 18h], ecx
        jmp    SHORT loc_1_4676
loc_1_466E:
        test   word ptr [eax + 0ch], 500h
        jne    SHORT loc_1_464E
loc_1_4676:
        pop    ebx
loc_1_4677:
        mov    eax, dword ptr [ebx + 4ch]
        mov    ecx, dword ptr [ebx + 50h]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebx + 14h]
        jecxz  loc_1_46CF
        test   byte ptr [ebx + 3ch], 3
        je     SHORT loc_1_46B2
        mov    eax, dword ptr [ebx + 44h]
        mov    ecx, 24h
        call   sub_1_45EB
        mov    esi, dword ptr [ebx + 8]
loc_1_469D:
        mov    eax, dword ptr [esi + 4]
        cmp    eax, esi
        je     SHORT loc_1_46AC
        mov    ecx, dword ptr [esi + 0ch]
        call   sub_1_45EB
loc_1_46AC:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    SHORT loc_1_469D
loc_1_46B2:
        mov    edx, dword ptr [ebx + 8]
        db  08Bh, 00Dh, 0B8h, 058h, 000h, 000h
        db  089h, 015h, 0B8h, 058h, 000h, 000h
        mov    edx, dword ptr [ebx + 0ch]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [ebx + 14h]
        db  001h, 00Dh, 04Ch, 058h, 000h, 000h
loc_1_46CF:
        push   ebx
        db  0CDh, 020h, 019h, 000h, 028h, 000h
        mov    eax, ebx
        db  087h, 005h, 0B4h, 058h, 000h, 000h
        mov    dword ptr [ebx + 38h], eax
        db  0FFh, 005h, 048h, 058h, 000h, 000h
        pop    ebx
        pop    esi
        ret
sub_1_46EA:
        push   ebx
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        mov    ebx, dword ptr [esp + 10h]
        mov    edx, ecx
        sub    edx, ebx
        mov    ecx, dword ptr [ecx]
loc_1_46FD:
        jecxz  loc_1_4710
        cmp    eax, ecx
        je     SHORT loc_1_470A
        mov    edx, ecx
        mov    ecx, dword ptr [ebx + ecx]
        jmp    SHORT loc_1_46FD
loc_1_470A:
        mov    ecx, dword ptr [ebx + ecx]
        mov    dword ptr [ebx + edx], ecx
loc_1_4710:
        pop    ebx
        ret
loc_1_4712:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    eax, 8137h
        jmp    SHORT loc_1_476B
loc_1_471E:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        test   al, 1
        je     SHORT loc_1_476B
        push   edx
        inc    eax
        push   eax
        db  0FFh, 00Dh, 04Ch, 058h, 000h, 000h
        db  08Bh, 03Dh, 0B8h, 058h, 000h, 000h
        mov    esi, dword ptr [edi]
        db  089h, 035h, 0B8h, 058h, 000h, 000h
        mov    dword ptr [edi + 4], edi
        mov    dword ptr [edi + 0ch], 1
        mov    dword ptr [edi], 0
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        sub    esi, 38h
        push   esi
        db  0CDh, 020h, 024h, 000h, 028h, 000h
        pop    eax
        pop    edx
        jmp    SHORT loc_1_476B
loc_1_4763:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 10h]
loc_1_476B:
        cld
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 1ch], 0eh
        add    edi, 20h
loc_1_4779:
        mov    esi, dword ptr [ebp + 8]
        movsd
        movsw
        mov    esi, dword ptr [ebp + 14h]
        add    esi, 1ch
        movsd
        movsw
        xchg   ah, al
        mov    word ptr [edi], ax
        pop    esi
        pop    edi
        pop    ebp
        ret
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 8
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 1ch], 16h
        add    edi, 20h
        mov    word ptr [edi + 0eh], 0e0e0h
        mov    byte ptr [edi + 10h], 3
        mov    dword ptr [edi + 11h], 0
        mov    word ptr [edi + 14h], 3781h
        jmp    SHORT loc_1_4779
loc_1_47C2:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 3
        mov    edi, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 1ch], 11h
        add    edi, 20h
        mov    word ptr [edi + 0eh], 0e0e0h
        mov    byte ptr [edi + 10h], 3
        jmp    SHORT loc_1_4779
loc_1_47E6:
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 1ch], 3
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [ebx + 21h]
        mov    byte ptr [edx + 20h], al
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [eax + 5]
        cmp    dword ptr [eax], -1
        jne    SHORT loc_1_4811
        cmp    word ptr [eax + 4], -1
        jne    SHORT loc_1_4811
        xor    ecx, ecx
loc_1_4811:
        mov    byte ptr [edx + 21h], cl
        mov    byte ptr [edx + 22h], 0fah
        pop    edx
        pop    ebp
        ret
loc_1_481B:
        push   ebp
        mov    ebp, esp
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 1ch], 11h
        call   sub_1_486C
        mov    word ptr [edi], 0e0e0h
        mov    byte ptr [edi + 2], 3
        pop    edi
        pop    edx
        pop    ebp
        ret
loc_1_483C:
        push   ebp
        mov    ebp, esp
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 0ch]
        mov    dword ptr [edx + 1ch], 16h
        call   sub_1_486C
        mov    word ptr [edi], 0aaaah
        mov    byte ptr [edi + 2], 3
        add    edi, 3
        mov    dword ptr [edi], 0
        mov    word ptr [edi + 3], 3781h
        pop    edi
        pop    edx
        pop    ebp
        ret
sub_1_486C:
        push   ebx
        push   esi
        cld
        mov    edx, dword ptr [ebp + 0ch]
        lea    edi, [edx + 20h]
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 4010h
        stosw
        movsd
        movsw
        mov    ebx, dword ptr [ebp + 14h]
        lea    esi, [ebx + 1ch]
        movsd
        movsw
        test   dword ptr [ebx + 8], 2
        je     SHORT loc_1_48D6
        mov    esi, dword ptr [ebp + 8]
        push   edx
        push   edi
        call   sub_1_232F
        jb     SHORT loc_1_48C6
        db  0FFh, 005h, 07Ch, 058h, 000h, 000h
        db  0A1h, 07Ch, 058h, 000h, 000h
        stosd
        mov    esi, edi
        pop    edi
        pop    edx
        movzx  ecx, byte ptr [esi]
        and    cl, 1fh
        jecxz  loc_1_48D6
        add    dword ptr [edx + 1ch], ecx
        or     byte ptr [edi - 6], 80h
        shr    ecx, 1
        rep movsw
        jmp    SHORT loc_1_48D6
loc_1_48C6:
        pop    edi
        pop    edx
        add    dword ptr [edx + 1ch], 2
        or     byte ptr [edi - 6], 80h
        mov    ax, 70c2h
        stosw
loc_1_48D6:
        pop    esi
        pop    ebx
        ret
sub_1_48D9:
        mov    ebx, dword ptr [edx + 4ch]
        mov    ecx, dword ptr [edx + 50h]
        mov    dword ptr [ecx], ebx
        mov    dword ptr [ebx + 4], ecx
        xor    ecx, ecx
        xchg   dword ptr [edx + 54h], ecx
        jecxz  loc_1_4911
        mov    eax, dword ptr [edx + 58h]
        mov    ecx, dword ptr [edx + 5ch]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 44h]
        mov    ecx, dword ptr [edx + 48h]
        mov    byte ptr [eax + 9], 0
        mov    byte ptr [eax + 8], 0
        cmp    dword ptr [eax + 4], 0
        je     SHORT loc_1_4911
        push   edx
        call   sub_1_4971
        pop    edx
loc_1_4911:
        mov    ebx, edx
        db  087h, 01Dh, 0B4h, 058h, 000h, 000h
        mov    dword ptr [edx + 38h], ebx
        db  0FFh, 005h, 048h, 058h, 000h, 000h
        ret
loc_1_4923:
        push   ebp
        push   ebx
        push   edi
        push   esi
        test   dword ptr [eax + 0ch], 400h
        jne    SHORT loc_1_4944
loc_1_4930:
        ror    dword ptr [eax + 0ch], 10h
        push   ebx
        push   edx
        push   eax
        mov    esi, eax
        call   dword ptr [eax + 4]
        add    esp, 0ch
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret
loc_1_4944:
        mov    esi, dword ptr [eax + 18h]
        mov    edi, dword ptr [esi + 44h]
        mov    dword ptr [eax + 10h], edi
        mov    edi, dword ptr [esi + 48h]
        mov    dword ptr [eax + 14h], edi
        mov    edi, dword ptr [esi + 4ch]
        mov    dword ptr [eax + 18h], edi
        jmp    SHORT loc_1_4930
        db  0DEh, 049h, 000h, 000h, 0F5h, 049h, 000h, 000h
loc_1_4963:
        mov    ebx, dword ptr [eax]
        mov    ebx, dword ptr [ebx + 40h]
        mov    dword ptr [eax + 0ah], ecx
        mov    byte ptr [eax + 8], 0aah
        jmp    SHORT loc_1_4993
sub_1_4971:
        push   ebx
        push   edi
        push   esi
        movzx  edi, word ptr [eax + 0ch]
        and    edi, 3
        db  08Bh, 034h, 0BDh, 057h, 049h, 000h, 000h
        test   word ptr [eax + 0ch], 800h
        jne    SHORT loc_1_4963
        mov    ebx, ecx
        xchg   dword ptr [eax + 14h], ebx
        mov    byte ptr [eax + 8], 0fbh
loc_1_4993:
        mov    edx, ebx
        db  003h, 015h, 020h, 058h, 000h, 000h
        xor    ecx, ecx
        cmp    dword ptr [edx], ecx
        jne    SHORT loc_1_49C1
        cmp    dword ptr [edx + 4], ecx
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], ecx
        jne    SHORT loc_1_49BD
        db  0CDh, 020h, 039h, 000h, 001h, 000h
        db  0CDh, 020h, 002h, 000h, 001h, 000h
        jne    SHORT loc_1_49CC
        call   esi
loc_1_49BD:
        pop    esi
        pop    edi
        pop    ebx
        ret
loc_1_49C1:
        mov    ebx, eax
        xchg   dword ptr [edx + 4], ebx
        mov    dword ptr [ebx], eax
        mov    dword ptr [eax], ecx
        jmp    SHORT loc_1_49BD
loc_1_49CC:
        mov    ecx, 1
        mov    eax, 1000h
        db  0CDh, 020h, 014h, 000h, 001h, 000h
        jmp    SHORT loc_1_49BD
loc_1_49DE:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        jmp    SHORT loc_1_4A0A
loc_1_49F5:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
loc_1_4A0A:
        mov    ecx, dword ptr [edx]
        jecxz  loc_1_4A5F
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        push   edx
        mov    eax, dword ptr [ebx + 8]
        cmp    byte ptr [ecx + 8], 0aah
        je     SHORT loc_1_4A78
        mov    edx, dword ptr [ecx + 14h]
        test   dword ptr [ecx + 0ch], 40h
        jne    SHORT loc_1_4A7B
        mov    byte ptr [eax + 1ch], 0ffh
        test   word ptr [ecx + 0ch], 400h
        jne    SHORT loc_1_4A81
loc_1_4A34:
        ror    dword ptr [ecx + 0ch], 10h
        mov    byte ptr [ecx + 8], 0
        mov    word ptr [eax + 4], dx
        shr    edx, 10h
        mov    word ptr [eax + 38h], dx
        mov    ecx, dword ptr [ecx + 4]
        movzx  edx, cx
        shr    ecx, 10h
        db  0CDh, 020h, 048h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        pop    edx
        jmp    SHORT loc_1_4A0A
loc_1_4A5F:
        mov    dword ptr [edx + 4], ecx
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        ret
loc_1_4A78:
        mov    edx, dword ptr [ecx + 0ah]
loc_1_4A7B:
        mov    byte ptr [eax + 1ch], 0
        jmp    SHORT loc_1_4A34
loc_1_4A81:
        mov    esi, dword ptr [ecx + 18h]
        mov    edi, dword ptr [esi + 44h]
        mov    dword ptr [ecx + 10h], edi
        mov    edi, dword ptr [esi + 48h]
        mov    dword ptr [ecx + 14h], edi
        mov    edi, dword ptr [esi + 4ch]
        mov    dword ptr [ecx + 18h], edi
        jmp    SHORT loc_1_4A34
sub_1_4A98:
        db  0CDh, 020h, 083h, 000h, 001h, 000h
        db  0CDh, 020h, 085h, 000h, 001h, 000h
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        ret
sub_1_4AAB:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 10h]
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret
sub_1_4AC2:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0ch]
        db  0CDh, 020h, 00Eh, 000h, 001h, 000h
        mov    eax, esi
        pop    esi
        ret
sub_1_4AD5:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h
        pop    esi
        ret
sub_1_4AE2:
        push   edi
        mov    edi, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0ch]
        mov    ecx, dword ptr [esp + 10h]
        rep stosb
        pop    edi
        ret
sub_1_4AF3:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, dword ptr [esp + 10h]
        mov    ecx, dword ptr [esp + 14h]
        rep movsb
        pop    edi
        pop    esi
        ret
        db  000h, 000h
NWLINK_Control:
        cmp    eax, 1
        db  00Fh, 084h, 060h, 0B8h, 0FEh, 0FFh
        cmp    eax, 1bh
        db  00Fh, 084h, 057h, 0B8h, 0FEh, 0FFh
        cmp    eax, 7
        je     NEAR PTR loc_1_4C5D
        cmp    eax, 24h
        je     NEAR PTR loc_1_4CB6
        cmp    eax, 23h
        je     NEAR PTR loc_1_4D5C
        cmp    eax, 0bh
        je     SHORT loc_1_4B42
        clc
        ret
NWLINK_Service_00:
        mov    ax, 1e03h
        clc
        ret
loc_1_4B42:
        push   ebx
        mov    edx, ebx
        mov    ebx, 0
        call   sub_1_53F6
        pop    ebx
        clc
        ret
NWLINK_Service_03:
        mov    edi, dword ptr [esi + 3ch]
        cmp    ebx, 3
        jne    SHORT loc_1_4B5F
        mov    edx, dword ptr [edi + 18h]
        jmp    SHORT loc_1_4B8A
loc_1_4B5F:
        cmp    ebx, 5
        jne    SHORT loc_1_4B8D
        mov    edx, dword ptr [edi + 18h]
        or     edx, edx
        jne    SHORT loc_1_4B8A
        db  08Bh, 015h, 0F0h, 058h, 000h, 000h
        or     edx, edx
        jne    SHORT loc_1_4B80
        db  0CDh, 020h, 03Fh, 000h, 001h, 000h
        db  0A3h, 0F0h, 058h, 000h, 000h
loc_1_4B80:
        call   sub_1_1ADC
        mov    edx, eax
        mov    dword ptr [edi + 18h], edx
loc_1_4B8A:
        xor    eax, eax
        ret
loc_1_4B8D:
        cmp    ebx, 4
        jne    SHORT loc_1_4B9E
        or     edx, edx
        je     SHORT loc_1_4B9C
        mov    dword ptr [edi + 18h], edx
        xor    eax, eax
        ret
loc_1_4B9C:
        jmp    SHORT loc_1_4BC6
loc_1_4B9E:
        cmp    ebx, 2
        jne    SHORT loc_1_4BB7
        db  083h, 025h, 0F8h, 058h, 000h, 000h, 0BFh
        db  081h, 00Dh, 0F8h, 058h, 000h, 000h, 080h, 000h, 000h, 000h
        xor    eax, eax
        ret
loc_1_4BB7:
        cmp    ebx, 1
        jne    SHORT loc_1_4BC6
        db  083h, 00Dh, 0F8h, 058h, 000h, 000h, 040h
        xor    eax, eax
        ret
loc_1_4BC6:
        xor    eax, eax
        dec    eax
        ret
NWLINK_Service_04:
        or     ebx, ebx
        je     SHORT loc_1_4C1B
        cmp    ebx, 2
        ja     SHORT loc_1_4C1B
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 010h, 000h, 000h, 000h
        jne    SHORT loc_1_4BE9
        db  0A1h, 0ECh, 058h, 000h, 000h
        mov    eax, dword ptr [eax + 34h]
        clc
        ret
loc_1_4BE9:
        push   ebx
        push   esi
        push   edi
        cmp    ebx, 1
        jne    SHORT loc_1_4C03
        mov    eax, edx
        call   sub_1_2D10
        jb     SHORT loc_1_4C59
        mov    eax, dword ptr [esi + 20h]
        mov    eax, dword ptr [eax + 34h]
        clc
        jmp    SHORT loc_1_4C59
loc_1_4C03:
        push   edx
        and    edx, 0ffffh
        call   sub_1_42D3
        pop    edx
        jb     SHORT loc_1_4C59
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 34h]
        clc
        jmp    SHORT loc_1_4C59
loc_1_4C1B:
        push   ebx
        push   esi
        push   edi
        cmp    ebx, 3
        jne    SHORT loc_1_4C58
        db  08Bh, 00Dh, 034h, 058h, 000h, 000h
        db  0BEh, 0CCh, 058h, 000h, 000h
        sub    esi, 4
loc_1_4C31:
        dec    ecx
        js     SHORT loc_1_4C53
        add    esi, 4
        mov    edi, dword ptr [esi]
        mov    eax, dword ptr [edi + 4]
        and    eax, 1
        je     SHORT loc_1_4C31
        mov    eax, dword ptr [edi + 3ch]
        or     eax, eax
        je     SHORT loc_1_4C31
        mov    eax, dword ptr [eax + 34h]
        sub    eax, edx
        jne    SHORT loc_1_4C31
        not    eax
        jmp    SHORT loc_1_4C59
loc_1_4C53:
        clc
        xor    eax, eax
        jmp    SHORT loc_1_4C59
loc_1_4C58:
        stc
loc_1_4C59:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_4C5D:
        sub    esp, 6ch
        push   edi
        lea    edi, [esp + 4]
        db  0CDh, 020h, 08Dh, 000h, 001h, 000h
        pop    edi
        db  0CDh, 020h, 082h, 000h, 001h, 000h
        mov    byte ptr [ebp + 1dh], 34h
        mov    eax, 21h
        db  0CDh, 020h, 084h, 000h, 001h, 000h
        movzx  edx, word ptr [ebp + 38h]
        shl    edx, 4
        movzx  ecx, word ptr [ebp + 10h]
        add    edx, ecx
        db  0CDh, 020h, 086h, 000h, 001h, 000h
        push   esi
        lea    esi, [esp + 4]
        db  0CDh, 020h, 08Eh, 000h, 001h, 000h
        pop    esi
        add    esp, 6ch
        db  003h, 01Dh, 020h, 058h, 000h, 000h
        mov    dword ptr [ebx], 0
        add    edx, 0fh
        mov    dword ptr [ebx + 8], edx
        ret
loc_1_4CB6:
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 001h, 000h, 000h, 000h
        je     NEAR PTR loc_1_4D5B
        db  083h, 025h, 0F8h, 058h, 000h, 000h, 0FEh
        db  081h, 025h, 0F8h, 058h, 000h, 000h, 0FFh, 0DFh, 0FFh, 0FFh
        db  083h, 03Dh, 070h, 056h, 000h, 000h, 000h
        je     SHORT loc_1_4CE5
        call   sub_1_1C65
loc_1_4CE5:
        db  083h, 03Dh, 074h, 056h, 000h, 000h, 000h
        je     SHORT loc_1_4CF3
        call   sub_1_1CC7
loc_1_4CF3:
        db  08Bh, 00Dh, 04Ah, 05Ah, 000h, 000h
        jecxz  loc_1_4D05
        push   ecx
        db  0CDh, 020h, 002h, 000h, 048h, 000h
        add    esp, 4
loc_1_4D05:
        xor    edx, edx
        call   sub_1_53F6
        push   ebp
        push   eax
        mov    ebp, esp
        db  08Bh, 01Dh, 0C4h, 058h, 000h, 000h
        or     ebx, ebx
        je     SHORT loc_1_4D54
loc_1_4D1A:
        db  08Bh, 03Ch, 09Dh, 0C8h, 058h, 000h, 000h
        test   dword ptr [edi + 4], 1
        je     SHORT loc_1_4D51
        and    dword ptr [edi + 4], 0fffffffeh
        push   dword ptr [edi + 0ch]
        push   ebp
        db  0CDh, 020h, 02Ch, 000h, 028h, 000h
        cmp    dword ptr [ebp], 103h
        jne    SHORT loc_1_4D51
loc_1_4D41:
        test   dword ptr [edi + 4], 10000000h
        jne    SHORT loc_1_4D51
        call   sub_1_4A98
        jmp    SHORT loc_1_4D41
loc_1_4D51:
        dec    ebx
        jne    SHORT loc_1_4D1A
loc_1_4D54:
        call   sub_1_1163
        pop    eax
        pop    ebp
loc_1_4D5B:
        ret
loc_1_4D5C:
        mov    ebx, dword ptr [esi + 0ch]
        or     ebx, ebx
        je     SHORT loc_1_4DD1
        cmp    ebx, -1
        je     SHORT loc_1_4DD6
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_1_4DD9
        cmp    ebx, 100h
        je     SHORT loc_1_4DB6
        cmp    ebx, 101h
        je     SHORT loc_1_4D92
        cmp    ebx, 102h
        je     SHORT loc_1_4DA4
        mov    eax, 1
        ret
loc_1_4D92:
        cmp    dword ptr [esi + 14h], 40h
        jb     SHORT loc_1_4DCB
        push   dword ptr [esi + 10h]
        call   sub_1_75B3
        add    esp, 4
        ret
loc_1_4DA4:
        cmp    dword ptr [esi + 14h], 40h
        jb     SHORT loc_1_4DCB
        push   dword ptr [esi + 10h]
        call   sub_1_770D
        add    esp, 4
        ret
loc_1_4DB6:
        mov    ebx, dword ptr [esi + 14h]
        cmp    ebx, 4
        jb     SHORT loc_1_4DCB
        mov    ebx, dword ptr [esi + 10h]
        mov    ebx, dword ptr [ebx]
        db  0FFh, 024h, 09Dh, 0DCh, 05Ah, 000h, 000h
        db  0C3h
loc_1_4DCB:
        mov    eax, 1
        ret
loc_1_4DD1:
        mov    ecx, 1e03h
loc_1_4DD6:
        xor    eax, eax
        ret
loc_1_4DD9:
        mov    eax, 2
        ret
loc_1_4DDF:
        cmp    dword ptr [esi + 1ch], 0ah
        jb     SHORT loc_1_4DFF
        mov    edi, dword ptr [esi + 18h]
        xchg   esi, edi
        call   sub_1_519F
        xchg   esi, edi
        mov    ebx, dword ptr [esi + 20h]
        or     eax, eax
        jne    SHORT loc_1_4E04
        mov    dword ptr [ebx], 0ah
        ret
loc_1_4DFF:
        mov    eax, 1
loc_1_4E04:
        mov    dword ptr [ebx], 0
        ret
NWLINK_Service_01:
        cmp    ebx, 1bh
        jae    NEAR PTR loc_1_4E97
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_1_4E45
loc_1_4E20:
        db  0F6h, 083h, 07Ch, 057h, 000h, 000h, 002h
        je     SHORT loc_1_4E3E
        mov    dword ptr [esi + 0ch], 4
        db  0C7h, 046h, 018h, 023h, 049h, 000h, 000h
        mov    dword ptr [esi + 14h], 0
loc_1_4E3E:
        db  0FFh, 024h, 09Dh, 050h, 05Ah, 000h, 000h
loc_1_4E45:
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 000h, 020h, 000h, 000h
        je     SHORT loc_1_4E83
        db  0F6h, 083h, 07Ch, 057h, 000h, 000h, 004h
        jne    SHORT loc_1_4E20
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 000h, 008h, 000h, 000h
        je     SHORT loc_1_4E83
loc_1_4E66:
        call   sub_1_4A98
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 000h, 008h, 000h, 000h
        jne    SHORT loc_1_4E66
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 001h, 000h, 000h, 000h
        jne    SHORT loc_1_4E20
loc_1_4E83:
        db  0F6h, 083h, 07Ch, 057h, 000h, 000h, 002h
        je     SHORT loc_1_4E94
        mov    byte ptr [esi + 9], 4
        mov    byte ptr [esi + 8], 0
loc_1_4E94:
        mov    al, 2
        ret
loc_1_4E97:
        cmp    ebx, 101h
        je     SHORT loc_1_4EA2
        mov    al, 1
        ret
loc_1_4EA2:
        db  0F7h, 005h, 0F8h, 058h, 000h, 000h, 001h, 000h, 000h, 000h
        je     SHORT loc_1_4EB1
        mov    al, 2
        ret
loc_1_4EB1:
        push   ebx
        call   sub_1_4F84
        jb     SHORT loc_1_4EC2
        push   edi
        push   esi
        call   sub_1_644C
        pop    esi
        pop    edi
loc_1_4EC2:
        pop    ebx
        ret
_LTEXT ends
        end
