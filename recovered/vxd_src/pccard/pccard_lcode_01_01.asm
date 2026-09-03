; pccard_lcode_01_01.asm -- PCCARD object 1 segment _LTEXT  (part 1/3)
.386p
_LTEXT segment dword public use32 'LCODE'

        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_000C:
        add    dword ptr [ecx], esp
        and    byte ptr [edx], bl
loc_1_0010:
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [edx], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ebx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
loc_1_0030:
        add    dword ptr [0ff201b1ah], edx
        add    byte ptr [eax], al
loc_1_0038:
        dec    ebp
loc_1_0039:
        dec    edi
        inc    esp
        inc    ebp
        dec    ebp
        add    byte ptr [eax], al
        add    byte ptr [esi + 41h], al
        pop    eax
        add    byte ptr [eax + 43h], dl
        dec    ebp
        inc    ebx
        dec    ecx
        inc    ecx
        pop    esp
        add    byte ptr [ebp + 4eh], dl
        dec    ebx
        dec    esi
        dec    edi
        push   edi
        dec    esi
        pop    edi
        dec    ebp
        inc    ecx
        dec    esi
        push   ebp
        inc    esi
        inc    ecx
        inc    ebx
        push   esp
        push   ebp
        push   edx
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0064:
        sub    eax, 564544h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 4eh], dl
        dec    ebx
        dec    esi
        dec    edi
        push   edi
        dec    esi
        pop    edi
        dec    ebp
        inc    ecx
        dec    esi
        push   ebp
        inc    esi
        inc    ecx
        inc    ebx
        push   esp
        push   ebp
        push   edx
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0084:
        inc    esp
        db  065h, 076h, 069h
        arpl   word ptr [ebp + 44h], sp
        db  065h, 073h, 063h
        add    byte ptr [eax], al
loc_1_0090:
        sub    dl, byte ptr [eax + 43h]
        dec    ebp
        inc    ebx
        dec    ecx
        inc    ecx
        pop    esp
        dec    ebp
        inc    esi
        inc    ebx
        add    byte ptr [ebx + 6ch], al
        popad
        jae    SHORT loc_1_0114
        add    byte ptr [eax], al
        add    byte ptr [ebx + 6fh], al
        insd
        jo     SHORT loc_1_010A
        je     SHORT loc_1_0114
        bound  ebp, qword ptr [ebp + 49h]
        inc    esp
        jae    SHORT loc_1_00B2
loc_1_00B2:
        add    byte ptr [eax], al
loc_1_00B4:
        add    dword ptr [edx], eax
        add    al, byte ptr [edx]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
loc_1_00C4:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 35h
        aaa
        cmp    byte ptr [ecx], bh
        inc    ecx
        inc    edx
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
loc_1_00D4:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 01Dh, 00Ah, 000h, 000h, 000h
        mov    ecx, dword ptr [esp + 14h]
        push   ebp
        lea    esi, [ebx + 2]
        test   byte ptr [esi], 1
        je     SHORT loc_1_00EF
        mov    edi, dword ptr [ecx + 28h]
        jmp    SHORT loc_1_00F2
loc_1_00EF:
        mov    edi, dword ptr [ecx + 24h]
loc_1_00F2:
        test   edi, edi
        je     SHORT loc_1_0118
        db  0A1h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 4]
loc_1_00FE:
        mov    ecx, dword ptr [edi + 0ch]
        cmp    eax, ecx
        jb     SHORT loc_1_010E
        mov    edx, dword ptr [edi + 10h]
        add    edx, ecx
loc_1_010A:
        cmp    edx, eax
        ja     SHORT loc_1_0114
loc_1_010E:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    SHORT loc_1_00FE
loc_1_0114:
        test   edi, edi
        jne    SHORT loc_1_012D
loc_1_0118:
        db  0A1h, 00Ah, 000h, 000h, 000h
        cmp    dword ptr [eax + 4], 0
        je     SHORT loc_1_013A
        mov    eax, 7
        jmp    NEAR PTR loc_1_01B7
loc_1_012D:
        test   byte ptr [edi + 0bh], 3
        je     SHORT loc_1_013A
        mov    eax, 7
        jmp    SHORT loc_1_01B7
loc_1_013A:
        push   19h
        call   sub_1_2540
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_014F
        mov    eax, 20h
        jmp    SHORT loc_1_01B7
loc_1_014F:
        mov    word ptr [eax + 4], 1
        xor    edx, edx
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        cmp    ecx, edx
        je     SHORT loc_1_017B
loc_1_0161:
        mov    bp, word ptr [eax + 4]
        cmp    word ptr [ecx + 4], bp
        ja     SHORT loc_1_017B
        jne    SHORT loc_1_0173
        inc    word ptr [eax + 4]
        jmp    SHORT loc_1_0177
loc_1_0173:
        mov    edx, ecx
        mov    ecx, dword ptr [ecx]
loc_1_0177:
        test   ecx, ecx
        jne    SHORT loc_1_0161
loc_1_017B:
        mov    cx, word ptr [eax + 4]
        mov    ebp, dword ptr [esp + 14h]
        db  066h, 089h, 00Dh, 002h, 000h, 000h, 000h
        mov    ecx, dword ptr [esp + 18h]
        mov    dword ptr [eax + 10h], ebp
        push   0
        push   eax
        mov    dword ptr [eax + 0ch], ecx
        mov    si, word ptr [esi]
        push   edx
        mov    word ptr [eax + 6], si
        db  068h, 000h, 000h, 000h, 000h
        mov    ebx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax + 14h], edi
        call   sub_1_2DB5
        add    esp, 10h
        xor    eax, eax
loc_1_01B7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_01BC:
        push   0
        push   dword ptr [esp + 8]
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2E0B
        add    esp, 0ch
        push   dword ptr [esp + 4]
        call   sub_1_254F
        add    esp, 4
        xor    eax, eax
        ret
sub_1_01DE:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        push   edi
        mov    eax, dword ptr [ecx + 14h]
        mov    edi, dword ptr [esp + 14h]
        mov    edx, dword ptr [eax + 10h]
        mov    esi, dword ptr [eax + 0ch]
        add    esi, edx
        cmp    edx, edi
        jae    SHORT loc_1_01FE
        mov    eax, 0ah
        jmp    SHORT loc_1_021E
loc_1_01FE:
        mov    eax, dword ptr [ecx + 8]
        mov    edx, dword ptr [esp + 10h]
        lea    ecx, [edx + eax]
        cmp    ecx, esi
        jbe    SHORT loc_1_0213
        mov    eax, 7
        jmp    SHORT loc_1_021E
loc_1_0213:
        add    eax, edi
        add    eax, edx
        cmp    esi, eax
        sbb    eax, eax
        and    eax, 0ah
loc_1_021E:
        pop    edi
        pop    esi
        ret
sub_1_0221:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [esi]
        cmp    ecx, 4000000h
        jb     SHORT loc_1_0241
        mov    eax, 7
        jmp    NEAR PTR loc_1_038B
loc_1_0241:
        mov    eax, dword ptr [esi + 4]
        mov    edx, 4000000h
        cmp    eax, edx
        ja     NEAR PTR loc_1_0386
        add    eax, ecx
        cmp    eax, edx
        ja     NEAR PTR loc_1_0386
        cmp    dword ptr [ebp + 8], 1
        mov    edi, dword ptr [ebp + 0ch]
        jne    SHORT loc_1_028A
        push   dword ptr [edi + 0ch]
        call   sub_1_27C1
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_038B
        db  0F6h, 005h, 003h, 000h, 000h, 000h, 001h
        je     SHORT loc_1_028A
        mov    eax, 1ah
        jmp    NEAR PTR loc_1_038B
loc_1_028A:
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [edi + 0ch]
        call   sub_1_4432
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_02A7
        mov    eax, 19h
        jmp    NEAR PTR loc_1_038B
loc_1_02A7:
        mov    eax, dword ptr [edi + 14h]
        test   eax, eax
        jne    SHORT loc_1_02B8
        test   byte ptr [edi + 6], 1
        jne    SHORT loc_1_02F9
        test   eax, eax
        je     SHORT loc_1_02C1
loc_1_02B8:
        mov    cl, byte ptr [eax + 9]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 11h], cl
loc_1_02C1:
        test   byte ptr [edi + 6], 1
        jne    SHORT loc_1_02D7
        mov    eax, dword ptr [ebp - 4]
        or     byte ptr [eax + 14h], 8
        mov    eax, dword ptr [ebp - 4]
        and    word ptr [eax + 14h], 0fffdh
loc_1_02D7:
        push   dword ptr [ebp - 4]
        call   sub_1_4053
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_0372
        push   dword ptr [ebp - 4]
        call   sub_1_4154
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_0372
loc_1_02F9:
        mov    eax, dword ptr [esi]
        mov    ebx, dword ptr [esi + 4]
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], ecx
        test   ebx, ebx
        mov    edx, dword ptr [eax + 1ah]
        mov    ecx, dword ptr [edx + 0ch]
        mov    dword ptr [ebp - 14h], ecx
        je     SHORT loc_1_036E
loc_1_031A:
        lea    eax, [ebp - 10h]
        push   eax
        push   dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 4]
        call   sub_1_450F
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_0372
        mov    edx, dword ptr [ebp - 14h]
        mov    eax, dword ptr [ebp - 4]
        sub    edx, dword ptr [eax + 16h]
        sub    edx, dword ptr [ebp - 0ch]
        cmp    edx, ebx
        jbe    SHORT loc_1_0342
        mov    edx, ebx
loc_1_0342:
        cmp    dword ptr [ebp + 8], 0
        jne    SHORT loc_1_0350
        mov    edi, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp - 10h]
        jmp    SHORT loc_1_0356
loc_1_0350:
        mov    edi, dword ptr [ebp - 10h]
        mov    esi, dword ptr [ebp - 8]
loc_1_0356:
        mov    ecx, edx
        shr    ecx, 2
        rep movsd
        mov    ecx, edx
        and    ecx, 3
        rep movsb
        add    dword ptr [ebp - 0ch], edx
        add    dword ptr [ebp - 8], edx
        sub    ebx, edx
        jne    SHORT loc_1_031A
loc_1_036E:
        xor    esi, esi
        jmp    SHORT loc_1_0377
loc_1_0372:
        mov    esi, 19h
loc_1_0377:
        push   dword ptr [ebp - 4]
        call   sub_1_44FF
        add    esp, 4
        mov    eax, esi
        jmp    SHORT loc_1_038B
loc_1_0386:
        mov    eax, 0ah
loc_1_038B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0392:
        push   esi
        push   edi
        db  08Bh, 03Dh, 00Ah, 000h, 000h, 000h
        mov    esi, dword ptr [esp + 0ch]
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_1_03B2
        push   edi
        push   esi
        push   0
        call   sub_1_0221
        add    esp, 0ch
        jmp    SHORT loc_1_03E3
loc_1_03B2:
        push   dword ptr [edi + 4]
        push   dword ptr [edi]
        push   esi
        call   sub_1_01DE
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_03E3
        mov    eax, dword ptr [esi + 14h]
        push   edi
        push   esi
        cmp    dword ptr [eax + 20h], 0
        jne    SHORT loc_1_03DB
        push   0
        call   sub_1_0221
        add    esp, 0ch
        jmp    SHORT loc_1_03E3
loc_1_03DB:
        call   sub_1_0B47
        add    esp, 8
loc_1_03E3:
        pop    edi
        pop    esi
        ret
loc_1_03E6:
        push   esi
        push   edi
        db  08Bh, 03Dh, 00Ah, 000h, 000h, 000h
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi + 14h]
        test   eax, eax
        jne    SHORT loc_1_0407
        push   edi
        push   esi
        push   1
        call   sub_1_0221
        add    esp, 0ch
        jmp    SHORT loc_1_0444
loc_1_0407:
        cmp    dword ptr [eax + 20h], 0
        jne    SHORT loc_1_0428
        cmp    byte ptr [eax + 0ah], 6
        jne    SHORT loc_1_0421
        push   edi
        push   esi
        push   1
        call   sub_1_0221
        add    esp, 0ch
        jmp    SHORT loc_1_0444
loc_1_0421:
        mov    eax, 1ah
        jmp    SHORT loc_1_0444
loc_1_0428:
        push   dword ptr [edi + 4]
        push   dword ptr [edi]
        push   esi
        call   sub_1_01DE
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_0444
        push   edi
        push   esi
        call   sub_1_0B76
        add    esp, 8
loc_1_0444:
        pop    edi
        pop    esi
        ret
loc_1_0447:
        push   esi
        push   edi
        db  08Bh, 03Dh, 00Ah, 000h, 000h, 000h
        mov    esi, dword ptr [esp + 0ch]
        mov    eax, dword ptr [esi + 14h]
        test   eax, eax
        je     SHORT loc_1_0491
        cmp    dword ptr [eax + 20h], 0
        je     SHORT loc_1_0491
        push   dword ptr [edi + 8]
        push   dword ptr [edi]
        push   esi
        call   sub_1_01DE
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_0496
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        push   esi
        call   sub_1_01DE
        add    esp, 0ch
        test   eax, eax
        jne    SHORT loc_1_0496
        push   edi
        push   esi
        call   sub_1_0BF3
        add    esp, 8
        jmp    SHORT loc_1_0496
loc_1_0491:
        mov    eax, 1ah
loc_1_0496:
        pop    edi
        pop    esi
        ret
sub_1_0499:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_04B3
        mov    cx, word ptr [esp + 4]
loc_1_04A7:
        cmp    word ptr [eax + 4], cx
        je     SHORT loc_1_04B3
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_1_04A7
loc_1_04B3:
        ret
sub_1_04B4:
        mov    al, byte ptr [esp + 0ch]
        push   esi
        mov    ecx, dword ptr [esp + 0ch]
        db  0A2h, 010h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx + 0ch]
        add    ecx, 14h
        mov    ax, word ptr [edx + 10h]
        db  066h, 0A3h, 002h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx]
        mov    al, byte ptr [edx + 9]
        db  0A2h, 011h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx]
        mov    si, word ptr [edx + 24h]
        db  066h, 089h, 035h, 012h, 000h, 000h, 000h
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 4]
        db  0A3h, 004h, 000h, 000h, 000h
        db  0A3h, 008h, 000h, 000h, 000h
        movzx  edx, byte ptr [esi + 3]
        db  089h, 015h, 00Ch, 000h, 000h, 000h
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [edx + 20h]
        push   dword ptr [eax + 4]
        db  066h, 08Bh, 015h, 002h, 000h, 000h, 000h
        add    eax, 0ch
        push   eax
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   0
        push   edx
        push   12h
        call   sub_1_24A2
        add    esp, 1ch
        cmp    eax, 18h
        db  00Fh, 0B7h, 005h, 014h, 000h, 000h, 000h
        jne    SHORT loc_1_0569
        test   eax, eax
        je     SHORT loc_1_054C
        cmp    eax, 1
        je     SHORT loc_1_0552
        cmp    eax, 2
        je     SHORT loc_1_0558
        cmp    eax, 3
        je     SHORT loc_1_055E
        jmp    SHORT loc_1_0562
loc_1_054C:
        mov    byte ptr [esi + 2], 4
        jmp    SHORT loc_1_0562
loc_1_0552:
        mov    byte ptr [esi + 2], 2
        jmp    SHORT loc_1_0562
loc_1_0558:
        mov    byte ptr [esi + 2], 1
        jmp    SHORT loc_1_0562
loc_1_055E:
        mov    byte ptr [esi + 2], 3
loc_1_0562:
        mov    eax, 18h
        jmp    SHORT loc_1_0589
loc_1_0569:
        test   eax, eax
        je     SHORT loc_1_0578
        cmp    eax, 1ah
        je     SHORT loc_1_057E
        mov    byte ptr [esi + 2], 0e1h
        jmp    SHORT loc_1_0582
loc_1_0578:
        mov    byte ptr [esi + 2], 0e0h
        jmp    SHORT loc_1_0582
loc_1_057E:
        mov    byte ptr [esi + 2], 84h
loc_1_0582:
        db  00Fh, 0B7h, 005h, 014h, 000h, 000h, 000h
loc_1_0589:
        pop    esi
        ret
sub_1_058B:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_05A0
loc_1_0594:
        cmp    dword ptr [esp + 4], eax
        je     SHORT loc_1_05A6
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_1_0594
loc_1_05A0:
        mov    eax, 21h
loc_1_05A5:
        ret
loc_1_05A6:
        xor    eax, eax
        jmp    SHORT loc_1_05A5
sub_1_05AA:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ax, word ptr [esi]
        push   eax
        call   sub_1_0499
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_05C5
        mov    byte ptr [esi + 2], 0c3h
        jmp    SHORT loc_1_05D2
loc_1_05C5:
        mov    ecx, dword ptr [eax + 14h]
        test   byte ptr [ecx + 7], 10h
        jne    SHORT loc_1_05D4
        mov    byte ptr [esi + 2], 86h
loc_1_05D2:
        xor    eax, eax
loc_1_05D4:
        pop    esi
        ret
loc_1_05D6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_058B
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_06D5
        xor    ebx, ebx
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 4], bx
        mov    dword ptr [ebp - 0ch], ebx
        mov    esi, dword ptr [eax + 12h]
        mov    dword ptr [eax + 0ah], ebx
        cmp    word ptr [esi + 2], bx
        lea    edi, [esi + 4]
        jbe    NEAR PTR loc_1_06C4
loc_1_0611:
        mov    al, byte ptr [edi + 2]
        test   al, 80h
        jne    NEAR PTR loc_1_06AD
        mov    byte ptr [ebp - 1], 0
        movzx  eax, al
        cmp    eax, 1
        je     SHORT loc_1_0639
        cmp    eax, 2
        je     SHORT loc_1_063F
        cmp    eax, 3
        jl     SHORT loc_1_0649
        cmp    eax, 4
        jle    SHORT loc_1_0645
        jmp    SHORT loc_1_0649
loc_1_0639:
        mov    byte ptr [ebp - 1], 10h
        jmp    SHORT loc_1_0649
loc_1_063F:
        mov    byte ptr [ebp - 1], 20h
        jmp    SHORT loc_1_0649
loc_1_0645:
        mov    byte ptr [ebp - 1], 30h
loc_1_0649:
        push   edi
        call   sub_1_05AA
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     SHORT loc_1_06AD
        push   dword ptr [ebp - 1]
        push   eax
        push   edi
        call   sub_1_04B4
        add    esp, 0ch
        cmp    eax, 18h
        jne    SHORT loc_1_0674
        mov    dword ptr [ebp - 0ch], 1
        jmp    SHORT loc_1_06AD
loc_1_0674:
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, 1
        sbb    al, al
        add    al, 0e1h
        mov    byte ptr [edi + 2], al
        mov    eax, dword ptr [ecx + 0eh]
        movzx  edx, word ptr [ecx + 4]
        push   dword ptr [eax + 4]
        add    eax, 0ch
        push   eax
        push   0
        push   edx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0ch]
        push   dword ptr [ebp - 4]
        mov    dx, word ptr [ecx + 10h]
        push   edx
        push   81h
        call   sub_1_24A2
        add    esp, 1ch
loc_1_06AD:
        movzx  eax, word ptr [esi]
        inc    word ptr [ebp - 4]
        add    edi, eax
        mov    cx, word ptr [ebp - 4]
        cmp    word ptr [esi + 2], cx
        ja     NEAR PTR loc_1_0611
loc_1_06C4:
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_06D5
        push   dword ptr [ebp + 8]
        call   sub_1_06DC
        add    esp, 4
loc_1_06D5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_06DC:
        db  00Fh, 0B7h, 005h, 014h, 000h, 000h, 000h
        push   ebx
        cmp    eax, 1
        push   esi
        push   edi
        jl     SHORT loc_1_06F0
        cmp    eax, 2
        jle    SHORT loc_1_06F6
loc_1_06F0:
        mov    bx, 1f4h
        jmp    SHORT loc_1_06FD
loc_1_06F6:
        db  066h, 08Bh, 01Dh, 016h, 000h, 000h, 000h
loc_1_06FD:
        mov    esi, dword ptr [esp + 10h]
        cmp    dword ptr [esi + 0ah], 0
        jne    SHORT loc_1_0745
        push   0eh
        call   sub_1_2540
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     SHORT loc_1_0745
        push   edi
        db  0C7h, 007h, 000h, 000h, 000h, 000h
        movzx  eax, bx
        push   eax
        mov    dword ptr [edi + 4], esi
        mov    word ptr [edi + 0ch], 2
        db  068h, 000h, 000h, 000h, 000h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_0735:
        mov    dword ptr [edi + 8], eax
        test   eax, eax
        jne    SHORT loc_1_0745
        push   edi
        call   sub_1_254F
        add    esp, 4
loc_1_0745:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0749:
        push   ebx
        push   esi
        push   edi
        push   ebp
        db  08Bh, 03Dh, 004h, 000h, 000h, 000h
        cmp    word ptr [edi], 8
        jb     NEAR PTR loc_1_07DD
        cmp    word ptr [edi + 2], 1
        jb     SHORT loc_1_07DD
        push   16h
        call   sub_1_2540
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_077B
        mov    eax, 20h
        jmp    SHORT loc_1_07E2
loc_1_077B:
        lea    esi, [ebx + 4]
        xor    ebp, ebp
        mov    word ptr [esi], 1
        db  08Bh, 015h, 000h, 000h, 000h, 000h
        cmp    edx, ebp
        je     SHORT loc_1_07AC
loc_1_078F:
        mov    cx, word ptr [edx + 4]
        mov    ax, word ptr [esi]
        cmp    cx, ax
        ja     SHORT loc_1_07AC
        jne    SHORT loc_1_07A4
        inc    ax
        mov    word ptr [esi], ax
        jmp    SHORT loc_1_07A8
loc_1_07A4:
        mov    ebp, edx
        mov    edx, dword ptr [edx]
loc_1_07A8:
        test   edx, edx
        jne    SHORT loc_1_078F
loc_1_07AC:
        mov    eax, dword ptr [esp + 14h]
        push   ebx
        mov    dword ptr [ebx + 0eh], eax
        mov    dword ptr [ebx + 12h], edi
        call   sub_1_07E7
        add    esp, 4
        push   0
        push   ebx
        push   ebp
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2DB5
        mov    ax, word ptr [esi]
        add    esp, 10h
        db  066h, 0A3h, 002h, 000h, 000h, 000h
        xor    eax, eax
        jmp    SHORT loc_1_07E2
loc_1_07DD:
        mov    eax, 1ch
loc_1_07E2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_07E7:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [eax + 12h]
        mov    bx, word ptr [esi + 2]
        xor    ebp, ebp
        test   bx, bx
        lea    edi, [esi + 4]
        je     SHORT loc_1_0835
loc_1_0800:
        cmp    byte ptr [edi + 2], 0
        jne    SHORT loc_1_0829
        push   edi
        call   sub_1_05AA
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_0829
        push   0
        push   eax
        push   edi
        call   sub_1_04B4
        add    esp, 0ch
        cmp    eax, 18h
        jne    SHORT loc_1_0829
        mov    ebp, 1
loc_1_0829:
        movzx  eax, word ptr [esi]
        dec    bx
        add    edi, eax
        test   bx, bx
        jne    SHORT loc_1_0800
loc_1_0835:
        test   ebp, ebp
        je     SHORT loc_1_0845
        push   dword ptr [esp + 14h]
        call   sub_1_06DC
        add    esp, 4
loc_1_0845:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_084C:
        push   0
        push   dword ptr [esp + 8]
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_2E0B
        add    esp, 0ch
        push   dword ptr [esp + 4]
        call   sub_1_254F
        add    esp, 4
        xor    eax, eax
        ret
sub_1_086E:
        db  0A1h, 000h, 000h, 000h, 000h
        test   eax, eax
        je     SHORT loc_1_0888
        mov    cx, word ptr [esp + 4]
loc_1_087C:
        cmp    word ptr [eax + 4], cx
        je     SHORT loc_1_0888
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_1_087C
loc_1_0888:
        ret
sub_1_0889:
        push   0
        push   dword ptr [esp + 8]
        call   sub_1_2DDA
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_08A6
        push   eax
        call   sub_1_254F
        add    esp, 4
        jmp    SHORT sub_1_0889
loc_1_08A6:
        ret
loc_1_08A7:
        push   esi
        db  08Bh, 015h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [esp + 0ch]
        lea    esi, [edx + 2]
        test   byte ptr [esi], 1
        je     SHORT loc_1_08BF
        mov    ecx, dword ptr [eax + 28h]
        jmp    SHORT loc_1_08C2
loc_1_08BF:
        mov    ecx, dword ptr [eax + 24h]
loc_1_08C2:
        test   ecx, ecx
        je     SHORT loc_1_08DD
        db  0A1h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 4]
loc_1_08CE:
        cmp    dword ptr [ecx + 0ch], eax
        je     SHORT loc_1_08D9
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_1_08CE
loc_1_08D9:
        test   ecx, ecx
        jne    SHORT loc_1_08E4
loc_1_08DD:
        mov    eax, 7
        jmp    SHORT loc_1_08FC
loc_1_08E4:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx + 20h], eax
        xor    eax, eax
        mov    si, word ptr [esi]
        mov    word ptr [ecx + 6], si
        mov    dx, word ptr [edx + 8]
        mov    word ptr [ecx + 24h], dx
loc_1_08FC:
        pop    esi
        ret
sub_1_08FE:
        mov    ecx, dword ptr [esp + 8]
        test   ecx, ecx
        je     SHORT loc_1_091A
        mov    eax, dword ptr [esp + 4]
        xor    edx, edx
loc_1_090C:
        cmp    dword ptr [ecx + 20h], eax
        jne    SHORT loc_1_0914
        mov    dword ptr [ecx + 20h], edx
loc_1_0914:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_1_090C
loc_1_091A:
        ret
sub_1_091B:
        push   esi
        db  08Bh, 035h, 000h, 000h, 000h, 000h
        test   esi, esi
        je     SHORT loc_1_094B
loc_1_0926:
        push   dword ptr [esi + 28h]
        push   dword ptr [esp + 0ch]
        call   sub_1_08FE
        add    esp, 8
        push   dword ptr [esi + 24h]
        push   dword ptr [esp + 0ch]
        call   sub_1_08FE
        add    esp, 8
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    SHORT loc_1_0926
loc_1_094B:
        pop    esi
        ret
sub_1_094D:
        mov    ecx, dword ptr [esp + 4]
        push   ebx
        push   esi
        add    ecx, 14h
        push   edi
        mov    eax, dword ptr [ecx - 8]
        mov    ax, word ptr [eax + 10h]
        db  066h, 0A3h, 002h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx]
        mov    al, byte ptr [edx + 9]
        db  0A2h, 011h, 000h, 000h, 000h
        mov    esi, dword ptr [esp + 14h]
        mov    edx, dword ptr [ecx]
        mov    bx, word ptr [edx + 24h]
        db  066h, 089h, 01Dh, 012h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx]
        mov    ebx, dword ptr [edx + 20h]
        lea    edi, [ebx + 0ch]
loc_1_0987:
        mov    eax, dword ptr [ebx + 4]
        push   eax
        push   edi
        db  066h, 0A1h, 002h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        push   esi
        push   0
        push   eax
        push   12h
        call   sub_1_24A2
        add    esp, 1ch
        cmp    eax, 18h
        jne    SHORT loc_1_09E9
        db  080h, 025h, 010h, 000h, 000h, 000h, 0CFh
        db  00Fh, 0B7h, 00Dh, 014h, 000h, 000h, 000h
        test   ecx, ecx
        je     SHORT loc_1_09CB
        cmp    ecx, 1
        je     SHORT loc_1_09D4
        cmp    ecx, 2
        je     SHORT loc_1_09DD
        cmp    ecx, 3
        jne    SHORT loc_1_09E4
loc_1_09CB:
        db  080h, 00Dh, 010h, 000h, 000h, 000h, 030h
        jmp    SHORT loc_1_09E4
loc_1_09D4:
        db  080h, 00Dh, 010h, 000h, 000h, 000h, 020h
        jmp    SHORT loc_1_09E4
loc_1_09DD:
        db  080h, 00Dh, 010h, 000h, 000h, 000h, 010h
loc_1_09E4:
        cmp    eax, 18h
        je     SHORT loc_1_0987
loc_1_09E9:
        db  00Fh, 0B7h, 005h, 014h, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_09F4:
        mov    eax, dword ptr [esp + 8]
        sub    esp, 4
        mov    dword ptr [esp], 0
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [eax + 4]
        push   ebp
        test   ebx, ebx
        je     SHORT loc_1_0A75
loc_1_0A0E:
        mov    ebp, 200h
        cmp    ebx, ebp
        jae    SHORT loc_1_0A19
        mov    ebp, ebx
loc_1_0A19:
        mov    eax, dword ptr [esp + 1ch]
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [esp + 1ch]
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 004h, 000h, 000h, 000h
        db  089h, 02Dh, 00Ch, 000h, 000h, 000h
        db  0C6h, 005h, 010h, 000h, 000h, 000h, 0C1h
        call   sub_1_094D
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_0A7F
        db  0BEh, 000h, 000h, 000h, 000h
        db  08Bh, 03Dh, 004h, 000h, 000h, 000h
        mov    ecx, ebp
        mov    eax, dword ptr [esp + 10h]
        shr    ecx, 2
        lea    edi, [edi + eax]
        repe cmpsd
        jne    SHORT loc_1_0A7F
        mov    ecx, ebp
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_1_0A7F
        sub    ebx, ebp
        add    dword ptr [esp + 10h], ebp
        test   ebx, ebx
        jne    SHORT loc_1_0A0E
loc_1_0A75:
        xor    eax, eax
loc_1_0A77:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_0A7F:
        mov    eax, 12h
        jmp    SHORT loc_1_0A77
sub_1_0A86:
        mov    eax, dword ptr [esp + 8]
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [eax + 8]
        push   ebp
        test   ebx, ebx
        je     NEAR PTR loc_1_0B28
loc_1_0A9C:
        mov    ebp, 100h
        cmp    ebx, ebp
        jae    SHORT loc_1_0AA7
        mov    ebp, ebx
loc_1_0AA7:
        mov    eax, dword ptr [esp + 1ch]
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [esp + 1ch]
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 004h, 000h, 000h, 000h
        db  089h, 02Dh, 00Ch, 000h, 000h, 000h
        db  0C6h, 005h, 010h, 000h, 000h, 000h, 0C1h
        call   sub_1_094D
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_0B32
        mov    eax, dword ptr [esp + 1ch]
        db  068h, 000h, 001h, 000h, 000h
        push   dword ptr [esp + 1ch]
        mov    ecx, dword ptr [eax + 4]
        db  089h, 00Dh, 004h, 000h, 000h, 000h
        db  089h, 02Dh, 00Ch, 000h, 000h, 000h
        db  0C6h, 005h, 010h, 000h, 000h, 000h, 0C1h
        call   sub_1_094D
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_0B39
        db  0BEh, 000h, 000h, 000h, 000h
        db  0BFh, 000h, 001h, 000h, 000h
        mov    ecx, ebp
        shr    ecx, 2
        repe cmpsd
        jne    SHORT loc_1_0B40
        mov    ecx, ebp
        and    ecx, 3
        repe cmpsb
        jne    SHORT loc_1_0B40
        sub    ebx, ebp
        jne    NEAR PTR loc_1_0A9C
loc_1_0B28:
        xor    eax, eax
loc_1_0B2A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_0B32:
        mov    eax, 9
        jmp    SHORT loc_1_0B2A
loc_1_0B39:
        mov    eax, 9
        jmp    SHORT loc_1_0B2A
loc_1_0B40:
        mov    eax, 12h
        jmp    SHORT loc_1_0B2A
sub_1_0B47:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
        db  089h, 00Dh, 004h, 000h, 000h, 000h
        mov    edx, dword ptr [eax + 4]
        db  0A1h, 004h, 000h, 000h, 000h
        push   eax
        db  089h, 015h, 00Ch, 000h, 000h, 000h
        push   dword ptr [esp + 8]
        db  0C6h, 005h, 010h, 000h, 000h, 000h, 0C1h
        call   sub_1_094D
        add    esp, 8
        ret
sub_1_0B76:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [esi]
        db  0A3h, 008h, 000h, 000h, 000h
        mov    ecx, dword ptr [esi + 4]
        db  0C6h, 005h, 010h, 000h, 000h, 000h, 0C2h
        db  089h, 00Dh, 00Ch, 000h, 000h, 000h
        test   byte ptr [esi + 8], 4
        je     SHORT loc_1_0BA4
        db  0C6h, 005h, 010h, 000h, 000h, 000h, 0C6h
loc_1_0BA4:
        test   byte ptr [esi + 8], 8
        je     SHORT loc_1_0BC4
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 14h]
        test   byte ptr [ecx + 7], 8
        je     SHORT loc_1_0BBF
        db  080h, 00Dh, 010h, 000h, 000h, 000h, 008h
        jmp    SHORT loc_1_0BC4
loc_1_0BBF:
        mov    edi, 1
loc_1_0BC4:
        db  0A1h, 004h, 000h, 000h, 000h
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_094D
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_0BE9
        test   edi, edi
        je     SHORT loc_1_0BE9
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_1_09F4
        add    esp, 8
loc_1_0BE9:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_1_0BED:
        mov    eax, 15h
        ret
sub_1_0BF3:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        xor    edi, edi
        mov    eax, dword ptr [esi]
        db  0A3h, 004h, 000h, 000h, 000h
        mov    ecx, dword ptr [esi + 4]
        db  089h, 00Dh, 008h, 000h, 000h, 000h
        mov    edx, dword ptr [esi + 8]
        db  0C6h, 005h, 010h, 000h, 000h, 000h, 0C3h
        db  089h, 015h, 00Ch, 000h, 000h, 000h
        test   byte ptr [esi + 0ch], 4
        je     SHORT loc_1_0C29
        db  0C6h, 005h, 010h, 000h, 000h, 000h, 0C7h
loc_1_0C29:
        test   byte ptr [esi + 0ch], 8
        mov    ebx, dword ptr [esp + 10h]
        je     SHORT loc_1_0C4A
        mov    eax, dword ptr [ebx + 14h]
        test   byte ptr [eax + 7], 8
        je     SHORT loc_1_0C45
        db  080h, 00Dh, 010h, 000h, 000h, 000h, 008h
        jmp    SHORT loc_1_0C4A
loc_1_0C45:
        mov    edi, 1
loc_1_0C4A:
        push   0
        push   ebx
        call   sub_1_094D
        add    esp, 8
        cmp    eax, 15h
        jne    SHORT loc_1_0C64
        push   esi
        push   ebx
        call   sub_1_0BED
        add    esp, 8
loc_1_0C64:
        test   eax, eax
        jne    SHORT loc_1_0C76
        test   edi, edi
        je     SHORT loc_1_0C76
        push   esi
        push   ebx
        call   sub_1_0A86
        add    esp, 8
loc_1_0C76:
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_0C7A:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        db  066h, 0A1h, 006h, 000h, 000h, 000h
        push   ebx
        mov    word ptr [ebp - 14h], ax
        push   esi
        db  066h, 08Bh, 035h, 002h, 000h, 000h, 000h
        push   edi
        xor    ebx, ebx
        db  08Bh, 03Dh, 00Ah, 000h, 000h, 000h
        db  0A0h, 003h, 000h, 000h, 000h
        mov    dword ptr [ebp - 10h], ebx
        and    al, 2
        db  08Ah, 00Dh, 002h, 000h, 000h, 000h
        or     al, 4
        mov    byte ptr [ebp - 8], cl
        movzx  ax, al
        mov    word ptr [ebp - 12h], ax
        lea    edx, [ebp - 18h]
        db  00Fh, 0B7h, 005h, 004h, 000h, 000h, 000h
        shl    eax, 0ch
        push   edx
        push   dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 0ch], eax
        call   sub_1_9645
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_0D4D
        push   dword ptr [ebp - 18h]
        db  068h, 000h, 000h, 000h, 000h
        lea    eax, [ebp - 14h]
        db  0A3h, 00Ah, 000h, 000h, 000h
        call   sub_1_48FD
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_0D4D
        db  066h, 08Bh, 00Dh, 002h, 000h, 000h, 000h
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_3AE5
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_0D4D
        db  066h, 0A1h, 002h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 4]
        db  066h, 0A3h, 006h, 000h, 000h, 000h
        add    ecx, 1ah
        mov    eax, dword ptr [ecx]
        mov    eax, dword ptr [eax + 0ch]
        shr    eax, 0ch
        db  066h, 0A3h, 004h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx]
        mov    edx, dword ptr [ecx + 8]
        db  089h, 015h, 00Ch, 000h, 000h, 000h
        test   byte ptr [eax + 15h], 1
        je     SHORT loc_1_0D48
        db  080h, 00Dh, 003h, 000h, 000h, 000h, 002h
loc_1_0D48:
        mov    ebx, 1
loc_1_0D4D:
        db  066h, 089h, 035h, 002h, 000h, 000h, 000h
        mov    eax, ebx
        db  089h, 03Dh, 00Ah, 000h, 000h, 000h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0D63:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        db  066h, 08Bh, 00Dh, 006h, 000h, 000h, 000h
        push   ebx
        push   esi
        lea    eax, [ebp - 8]
        push   edi
        push   eax
        push   ecx
        call   sub_1_3AE5
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_0E60
        mov    eax, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 8]
        add    edi, 16h
        mov    cl, byte ptr [eax + 11h]
        mov    ax, word ptr [eax + 14h]
        mov    byte ptr [ebp - 1], cl
        mov    word ptr [ebp - 4], ax
        mov    esi, dword ptr [edi]
        db  0A1h, 008h, 000h, 000h, 000h
        push   eax
        call   sub_1_2664
        add    esp, 4
        mov    dword ptr [edi], eax
        mov    edi, dword ptr [ebp - 8]
        add    edi, 16h
        mov    eax, dword ptr [edi]
        cmp    eax, -1
        je     NEAR PTR loc_1_0E5E
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 14h
        mov    dx, word ptr [ecx]
        test   dh, 1
        mov    word ptr [ebp - 0ch], dx
        je     SHORT loc_1_0DE3
        mov    edx, dword ptr [ebp - 8]
        mov    ebx, dword ptr [edx + 1ah]
        sub    edx, edx
        div    dword ptr [ebx + 0ch]
        test   edx, edx
        jne    SHORT loc_1_0E5E
loc_1_0DE3:
        db  0F6h, 005h, 003h, 000h, 000h, 000h, 002h
        mov    ax, word ptr [ebp - 0ch]
        je     SHORT loc_1_0DF4
        or     al, 2
        jmp    SHORT loc_1_0DF8
loc_1_0DF4:
        and    ax, 0fffdh
loc_1_0DF8:
        mov    word ptr [ecx], ax
        db  08Ah, 00Dh, 002h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 11h], cl
        push   dword ptr [ebp - 8]
        call   sub_1_4053
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_0E2C
        push   dword ptr [ebp - 8]
        call   sub_1_4154
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_0E2C
        mov    eax, 1
        jmp    SHORT loc_1_0E62
loc_1_0E2C:
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 14h], ax
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ebp - 1]
        mov    dword ptr [ecx + 16h], esi
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [edx + 11h], al
        push   dword ptr [ebp - 8]
        call   sub_1_4053
        add    esp, 4
        push   dword ptr [ebp - 8]
        call   sub_1_4154
        add    esp, 4
        jmp    SHORT loc_1_0E60
loc_1_0E5E:
        mov    dword ptr [edi], esi
loc_1_0E60:
        xor    eax, eax
loc_1_0E62:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0E69:
        db  066h, 08Bh, 00Dh, 006h, 000h, 000h, 000h
        sub    esp, 4
        lea    eax, [esp]
        push   eax
        push   ecx
        call   sub_1_3AE5
        add    esp, 8
        test   eax, eax
        mov    eax, 0
        jne    SHORT loc_1_0E9D
        push   dword ptr [esp]
        call   sub_1_4B4F
        add    esp, 4
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_1_0E9D:
        add    esp, 4
        ret
loc_1_0EA1:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        db  066h, 08Bh, 00Dh, 006h, 000h, 000h, 000h
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    dword ptr [ebp - 8], 0
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_9645
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_0ED1
        xor    eax, eax
        jmp    NEAR PTR loc_1_0F9E
loc_1_0ED1:
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 0ch
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 18h]
        db  089h, 015h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    esi, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 084h
        db  089h, 035h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0ch]
        db  0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A2h, 000h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        je     SHORT loc_1_0F18
        xor    eax, eax
        jmp    NEAR PTR loc_1_0F9E
loc_1_0F18:
        xor    ax, ax
        db  0B9h, 010h, 000h, 000h, 000h
        db  066h, 039h, 005h, 00Eh, 000h, 000h, 000h
        jbe    SHORT loc_1_0F52
loc_1_0F29:
        mov    dl, byte ptr [ecx]
        db  038h, 015h, 002h, 000h, 000h, 000h
        jne    SHORT loc_1_0F39
        movzx  esi, ax
        mov    dword ptr [ebp - 8], esi
loc_1_0F39:
        db  038h, 015h, 003h, 000h, 000h, 000h
        jne    SHORT loc_1_0F44
        movzx  ebx, ax
loc_1_0F44:
        inc    ax
        add    ecx, 2
        db  066h, 03Bh, 005h, 00Eh, 000h, 000h, 000h
        jb     SHORT loc_1_0F29
loc_1_0F52:
        mov    eax, dword ptr [ebp - 4]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Dh
        mov    ecx, dword ptr [eax + 0ch]
        mov    dl, byte ptr [ecx + 0ch]
        db  088h, 015h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [eax + 12h]
        db  0A2h, 002h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        mov    eax, 0
        jne    SHORT loc_1_0F9E
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Eh
        mov    eax, dword ptr [ebp - 8]
        shl    al, 4
        add    al, bl
        db  0A2h, 004h, 000h, 000h, 000h
        call   sub_1_2438
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_1_0F9E:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
loc_1_0FA4:
        mov    eax, 1
        ret
sub_1_0FAA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 10h]
        mov    edi, dword ptr [ebp + 0ch]
        mov    word ptr [edi], ax
        mov    cl, byte ptr [ebp + 10h]
        push   edi
        mov    byte ptr [edi + 4], cl
        mov    byte ptr [edi + 5], 0
        mov    word ptr [edi + 2], 0
        push   esi
        call   sub_1_384D
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_1009
        mov    eax, dword ptr [ebp + 14h]
        test   eax, eax
        je     SHORT loc_1_0FE6
        mov    cl, byte ptr [edi + 11h]
        mov    byte ptr [eax], cl
loc_1_0FE6:
        mov    byte ptr [edi + 5], 0
        mov    word ptr [edi + 10h], 20h
        push   edi
        push   esi
        call   sub_1_3967
        add    esp, 8
        mov    ecx, eax
        mov    eax, 0
        cmp    ecx, 1
        adc    eax, -1
        and    eax, ecx
loc_1_1009:
        pop    edi
        pop    esi
        pop    ebp
        ret
sub_1_100D:
        push   ebp
        mov    al, byte ptr [esp + 10h]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        mov    byte ptr [esi + 4], al
        push   dword ptr [ebp + 8]
        call   sub_1_3906
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_105C
        mov    eax, dword ptr [ebp + 14h]
        test   eax, eax
        je     SHORT loc_1_1037
        mov    cl, byte ptr [esi + 11h]
        mov    byte ptr [eax], cl
loc_1_1037:
        mov    byte ptr [esi + 5], 0
        mov    word ptr [esi + 10h], 20h
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_1_3967
        add    esp, 8
        mov    ecx, eax
        mov    eax, 0
        cmp    ecx, 1
        adc    eax, -1
        and    eax, ecx
loc_1_105C:
        pop    esi
        pop    ebp
        ret
sub_1_105F:
        mov    eax, dword ptr [esp + 4]
loc_1_1063:
        mov    ecx, eax
        inc    eax
        test   byte ptr [ecx], 80h
        jne    SHORT loc_1_1063
        ret
sub_1_106C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 10h
        cmp    word ptr [ebp + 10h], 0
        push   ebx
        push   esi
        push   edi
        jne    SHORT loc_1_1085
        add    eax, 24h
        jmp    SHORT loc_1_1088
loc_1_1085:
        add    eax, 28h
loc_1_1088:
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 0ch]
        movzx  ecx, byte ptr [ebp + 14h]
        lea    esi, [eax + 14h]
        lea    eax, [ecx + eax + 14h]
        mov    dword ptr [ebp - 8], eax
        cmp    esi, eax
        jae    NEAR PTR loc_1_1187
loc_1_10AB:
        mov    bl, byte ptr [esi]
        cmp    bl, 0ffh
        je     NEAR PTR loc_1_1187
        inc    esi
        mov    al, bl
        and    al, 7
        mov    byte ptr [ebp - 1], al
        cmp    al, 7
        jne    SHORT loc_1_10D2
        mov    al, byte ptr [esi]
        push   esi
        mov    byte ptr [ebp - 1], al
        call   sub_1_105F
        add    esp, 4
        mov    esi, eax
loc_1_10D2:
        shr    bl, 4
        cmp    bl, 0eh
        jne    SHORT loc_1_10E5
        push   esi
        call   sub_1_105F
        add    esp, 4
        mov    esi, eax
loc_1_10E5:
        mov    al, byte ptr [esi]
        cmp    al, 0ffh
        je     NEAR PTR loc_1_1187
        inc    esi
        push   37h
        mov    byte ptr [ebp - 2], al
        call   sub_1_2540
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     NEAR PTR loc_1_1187
        push   0
        push   edi
        push   dword ptr [ebp - 10h]
        call   sub_1_2D7C
        mov    dx, word ptr [ebp + 10h]
        add    esp, 0ch
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 10h]
        mov    word ptr [edi + 4], ax
        mov    al, byte ptr [ebp - 1]
        mov    word ptr [edi + 6], dx
        mov    edx, dword ptr [ebp - 0ch]
        mov    byte ptr [edi + 8], 7
        mov    byte ptr [edi + 9], al
        mov    al, byte ptr [ebp - 2]
        shr    al, 3
        mov    dword ptr [edi + 0ch], edx
        movzx  edx, al
        inc    edx
        mov    al, byte ptr [ebp - 2]
        and    al, 7
        add    al, al
        add    al, 9
        mov    cl, al
        shl    edx, cl
        cmp    word ptr [ebp + 10h], 0
        mov    dword ptr [edi + 10h], edx
        je     SHORT loc_1_115E
        add    edx, edx
        mov    dword ptr [edi + 10h], edx
loc_1_115E:
        lea    eax, [edi + 0bh]
        test   bl, bl
        mov    byte ptr [eax], 0
        mov    byte ptr [edi + 0ah], bl
        jne    SHORT loc_1_1170
        or     byte ptr [eax], 1
        jmp    SHORT loc_1_1178
loc_1_1170:
        cmp    bl, 0dh
        jne    SHORT loc_1_1178
        or     byte ptr [eax], 2
loc_1_1178:
        mov    eax, dword ptr [edi + 10h]
        add    dword ptr [ebp - 0ch], eax
        cmp    esi, dword ptr [ebp - 8]
        jb     NEAR PTR loc_1_10AB
loc_1_1187:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_118E:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        cmp    word ptr [esp + 10h], 0
        mov    ebp, esp
        push   esi
        jne    SHORT loc_1_11A3
        mov    esi, dword ptr [eax + 24h]
        jmp    SHORT loc_1_11A6
loc_1_11A3:
        mov    esi, dword ptr [eax + 28h]
loc_1_11A6:
        mov    eax, dword ptr [ebp + 0ch]
        test   esi, esi
        lea    edx, [eax + 14h]
        je     SHORT loc_1_11D0
        movzx  ecx, byte ptr [ebp + 14h]
        lea    eax, [ecx + eax + 14h]
loc_1_11B8:
        cmp    edx, eax
        jae    SHORT loc_1_11D0
        mov    cx, word ptr [edx]
        add    edx, 2
        mov    word ptr [esi + 1ah], cx
        or     byte ptr [esi + 8], 20h
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_1_11B8
loc_1_11D0:
        pop    esi
        pop    ebp
        ret
sub_1_11D3:
        push   ebp
        mov    edx, dword ptr [esp + 0ch]
        cmp    word ptr [esp + 10h], 0
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        lea    eax, [edx + 14h]
        push   esi
        push   edi
        jne    SHORT loc_1_11F1
        mov    esi, dword ptr [ecx + 24h]
        jmp    SHORT loc_1_11F4
loc_1_11F1:
        mov    esi, dword ptr [ecx + 28h]
loc_1_11F4:
        test   esi, esi
        je     SHORT loc_1_1245
        movzx  ecx, byte ptr [ebp + 14h]
        lea    edi, [ecx + edx + 14h]
loc_1_1200:
        cmp    eax, edi
        jae    SHORT loc_1_1245
        lea    edx, [eax + 5]
        mov    ebx, 1
        mov    cl, byte ptr [eax]
        push   edx
        dec    cl
        shl    ebx, cl
        mov    cl, byte ptr [eax + 1]
        dec    cl
        shl    ebx, cl
        mov    cl, byte ptr [edx]
        dec    cl
        shl    ebx, cl
        mov    dword ptr [esi + 14h], ebx
        mov    bx, 1
        mov    cl, byte ptr [eax + 4]
        dec    cl
        shl    bx, cl
        mov    word ptr [esi + 18h], bx
        or     byte ptr [esi + 8], 18h
        call   sub_1_105F
        add    esp, 4
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_1_1200
loc_1_1245:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_124A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        mov    edi, dword ptr [ebp + 0ch]
        push   1
        push   edi
        push   esi
        call   sub_1_0FAA
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_1_127B
        push   dword ptr [ebp - 1]
        push   0
        push   edi
        push   esi
        call   sub_1_106C
        add    esp, 10h
loc_1_127B:
        lea    eax, [ebp - 1]
        push   eax
        push   17h
        push   edi
        push   esi
        call   sub_1_0FAA
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_1_129E
        push   dword ptr [ebp - 1]
        push   1
        push   edi
        push   esi
        call   sub_1_106C
        add    esp, 10h
loc_1_129E:
        lea    eax, [ebp - 1]
        push   eax
        push   18h
        push   edi
        push   esi
        call   sub_1_0FAA
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_1_12C1
        push   dword ptr [ebp - 1]
        push   0
        push   edi
        push   esi
        call   sub_1_118E
        add    esp, 10h
loc_1_12C1:
        lea    eax, [ebp - 1]
        push   eax
        push   19h
        push   edi
        push   esi
        call   sub_1_0FAA
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_1_12E4
        push   dword ptr [ebp - 1]
        push   1
        push   edi
        push   esi
        call   sub_1_118E
        add    esp, 10h
loc_1_12E4:
        lea    eax, [ebp - 1]
        push   eax
        push   1eh
        push   edi
        push   esi
        call   sub_1_0FAA
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_1_1307
        push   dword ptr [ebp - 1]
        push   0
        push   edi
        push   esi
        call   sub_1_11D3
        add    esp, 10h
loc_1_1307:
        lea    eax, [ebp - 1]
        push   eax
        push   1fh
        push   edi
        push   esi
        call   sub_1_0FAA
        add    esp, 10h
        test   eax, eax
        jne    SHORT loc_1_132A
        push   dword ptr [ebp - 1]
        push   1
        push   edi
        push   esi
        call   sub_1_11D3
        add    esp, 10h
loc_1_132A:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_1330:
        mov    ecx, dword ptr [esp + 4]
        push   ebx
        mov    edx, dword ptr [esp + 0ch]
        push   esi
        or     byte ptr [ecx + 8], 40h
        cmp    byte ptr [edx + 14h], 0
        jne    NEAR PTR loc_1_13EB
        add    edx, 15h
        db  0BEh, 000h, 000h, 000h, 000h
        mov    eax, edx
loc_1_1352:
        mov    bl, byte ptr [eax]
        cmp    bl, byte ptr [esi]
        jne    SHORT loc_1_1372
        or     bl, bl
        je     SHORT loc_1_136E
        mov    bl, byte ptr [eax + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    SHORT loc_1_1372
        add    eax, 2
        add    esi, 2
        or     bl, bl
        jne    SHORT loc_1_1352
loc_1_136E:
        xor    eax, eax
        jmp    SHORT loc_1_1377
loc_1_1372:
        sbb    eax, eax
        sbb    eax, -1
loc_1_1377:
        test   eax, eax
        jne    SHORT loc_1_1381
        or     byte ptr [ecx + 26h], 1
        jmp    SHORT loc_1_13F1
loc_1_1381:
        mov    eax, edx
        db  0BEh, 000h, 000h, 000h, 000h
loc_1_1388:
        mov    bl, byte ptr [eax]
        cmp    bl, byte ptr [esi]
        jne    SHORT loc_1_13A8
        or     bl, bl
        je     SHORT loc_1_13A4
        mov    bl, byte ptr [eax + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    SHORT loc_1_13A8
        add    eax, 2
        add    esi, 2
        or     bl, bl
        jne    SHORT loc_1_1388
loc_1_13A4:
        xor    eax, eax
        jmp    SHORT loc_1_13AD
loc_1_13A8:
        sbb    eax, eax
        sbb    eax, -1
loc_1_13AD:
        test   eax, eax
        jne    SHORT loc_1_13B7
        or     byte ptr [ecx + 26h], 2
        jmp    SHORT loc_1_13F1
loc_1_13B7:
        db  0B8h, 000h, 000h, 000h, 000h
loc_1_13BC:
        mov    bl, byte ptr [edx]
        cmp    bl, byte ptr [eax]
        jne    SHORT loc_1_13DC
        or     bl, bl
        je     SHORT loc_1_13D8
        mov    bl, byte ptr [edx + 1]
        cmp    bl, byte ptr [eax + 1]
        jne    SHORT loc_1_13DC
        add    edx, 2
        add    eax, 2
        or     bl, bl
        jne    SHORT loc_1_13BC
loc_1_13D8:
        xor    eax, eax
        jmp    SHORT loc_1_13E1
loc_1_13DC:
        sbb    eax, eax
        sbb    eax, -1
loc_1_13E1:
        test   eax, eax
        jne    SHORT loc_1_13EB
        or     byte ptr [ecx + 26h], 3
        jmp    SHORT loc_1_13F1
loc_1_13EB:
        or     word ptr [ecx + 26h], 7fffh
loc_1_13F1:
        pop    esi
        pop    ebx
        ret
sub_1_13F4:
        sub    esp, 4
        lea    eax, [esp + 3]
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 18h]
        push   ebp
        mov    esi, dword ptr [esp + 18h]
        push   eax
        push   41h
        push   edi
        push   esi
        call   sub_1_0FAA
        add    esp, 10h
        test   eax, eax
        jne    NEAR PTR loc_1_1528
loc_1_141C:
        push   37h
        call   sub_1_2540
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     NEAR PTR loc_1_1528
        push   0
        lea    eax, [esi + 2ch]
        push   ebx
        push   eax
        call   sub_1_2D7C
        mov    ax, word ptr [esi + 10h]
        add    esp, 0ch
        mov    word ptr [ebx + 4], ax
        mov    word ptr [ebx + 6], 0
        mov    byte ptr [ebx + 8], 6
        mov    ecx, dword ptr [edi + 16h]
        mov    eax, 0
        mov    dword ptr [ebx + 0ch], ecx
        mov    edx, dword ptr [edi + 1ah]
        mov    dword ptr [ebx + 10h], edx
        cmp    byte ptr [edi + 15h], 1
        adc    eax, -1
        and    ax, 8000h
        mov    word ptr [ebx + 26h], ax
        mov    ecx, dword ptr [esi + 24h]
        test   ecx, ecx
        je     SHORT loc_1_14B6
loc_1_1478:
        mov    eax, dword ptr [ecx + 0ch]
        mov    edx, dword ptr [ebx + 0ch]
        cmp    eax, edx
        ja     SHORT loc_1_14B0
        mov    ebp, dword ptr [ecx + 10h]
        add    ebp, eax
        cmp    ebp, edx
        jbe    SHORT loc_1_14B0
        mov    al, byte ptr [ecx + 9]
        mov    byte ptr [ebx + 9], al
        mov    edx, dword ptr [ecx + 14h]
        mov    dword ptr [ebx + 14h], edx
        mov    bp, word ptr [ecx + 18h]
        mov    word ptr [ebx + 18h], bp
        mov    ax, word ptr [ecx + 1ah]
        mov    word ptr [ebx + 1ah], ax
        mov    dl, byte ptr [ecx + 8]
        or     byte ptr [ebx + 8], dl
        mov    dword ptr [ebx + 28h], ecx
loc_1_14B0:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_1_1478
loc_1_14B6:
        mov    byte ptr [edi + 4], 0ffh
        push   edi
        push   esi
        call   sub_1_3906
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_14FD
loc_1_14C8:
        mov    al, byte ptr [edi + 10h]
        cmp    al, 41h
        je     SHORT loc_1_14FD
        cmp    al, 46h
        jne    SHORT loc_1_14EB
        push   edi
        push   esi
        call   sub_1_3967
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_14FD
        push   edi
        push   ebx
        call   sub_1_1330
        add    esp, 8
loc_1_14EB:
        mov    byte ptr [edi + 4], 0ffh
        push   edi
        push   esi
        call   sub_1_3906
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_14C8
loc_1_14FD:
        cmp    byte ptr [edi + 10h], 41h
        jne    SHORT loc_1_150F
        push   edi
        push   esi
        call   sub_1_3967
        add    esp, 8
        jmp    SHORT loc_1_1520
loc_1_150F:
        lea    eax, [esp + 13h]
        push   eax
        push   41h
        push   edi
        push   esi
        call   sub_1_100D
        add    esp, 10h
loc_1_1520:
        test   eax, eax
        je     NEAR PTR loc_1_141C
loc_1_1528:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
sub_1_1530:
        mov    edx, dword ptr [esp + 8]
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    ax, word ptr [ecx + 6]
        mov    word ptr [edx + 2], ax
        movzx  si, byte ptr [ecx + 0ah]
        shl    si, 4
        add    si, ax
        mov    word ptr [edx + 2], si
        mov    al, byte ptr [ecx + 8]
        mov    byte ptr [edx + 4], al
        mov    al, byte ptr [ecx + 9]
        mov    byte ptr [edx + 5], al
        mov    esi, dword ptr [ecx + 0ch]
        mov    dword ptr [edx + 10h], esi
        mov    eax, dword ptr [ecx + 10h]
        mov    dword ptr [edx + 14h], eax
        mov    esi, dword ptr [ecx + 14h]
        mov    dword ptr [edx + 18h], esi
        mov    ax, word ptr [ecx + 18h]
        mov    word ptr [edx + 1ch], ax
        mov    si, word ptr [ecx + 1ah]
        mov    word ptr [edx + 1eh], si
        mov    dword ptr [edx + 8], ecx
        pop    esi
        ret
sub_1_1584:
        mov    eax, dword ptr [esp + 4]
        test   eax, eax
        je     SHORT loc_1_1598
loc_1_158C:
        test   byte ptr [eax + 0bh], 3
        je     SHORT loc_1_159A
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_1_158C
loc_1_1598:
        xor    eax, eax
loc_1_159A:
        ret
loc_1_159B:
        mov    eax, dword ptr [esp + 8]
        push   esi
        db  08Bh, 035h, 00Ah, 000h, 000h, 000h
        test   byte ptr [esi + 2], 1
        je     SHORT loc_1_15B1
        mov    eax, dword ptr [eax + 28h]
        jmp    SHORT loc_1_15B4
loc_1_15B1:
        mov    eax, dword ptr [eax + 24h]
loc_1_15B4:
        push   eax
        call   sub_1_1584
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_15C8
        mov    eax, 1fh
        jmp    SHORT loc_1_15D4
loc_1_15C8:
        push   esi
        push   eax
        call   sub_1_1530
        add    esp, 8
        xor    eax, eax
loc_1_15D4:
        pop    esi
        ret
loc_1_15D6:
        mov    eax, dword ptr [esp + 8]
        push   esi
        db  08Bh, 035h, 00Ah, 000h, 000h, 000h
        test   byte ptr [esi + 2], 1
        je     SHORT loc_1_15EC
        mov    ecx, dword ptr [eax + 28h]
        jmp    SHORT loc_1_15EF
loc_1_15EC:
        mov    ecx, dword ptr [eax + 24h]
loc_1_15EF:
        test   ecx, ecx
        je     SHORT loc_1_1609
        db  0A1h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [eax + 8]
loc_1_15FB:
        cmp    ecx, eax
        je     SHORT loc_1_1605
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_1_15FB
loc_1_1605:
        test   ecx, ecx
        jne    SHORT loc_1_1610
loc_1_1609:
        mov    eax, 1ch
        jmp    SHORT loc_1_1636
loc_1_1610:
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     SHORT loc_1_1631
        push   eax
        call   sub_1_1584
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_1631
        push   esi
        push   eax
        call   sub_1_1530
        add    esp, 8
        xor    eax, eax
        jmp    SHORT loc_1_1636
loc_1_1631:
        mov    eax, 1fh
loc_1_1636:
        pop    esi
        ret
sub_1_1638:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        mov    edx, dword ptr [esp + 0ch]
        test   byte ptr [ecx + 2], 1
        je     SHORT loc_1_164C
        add    edx, 28h
        jmp    SHORT loc_1_164F
loc_1_164C:
        add    edx, 24h
loc_1_164F:
        mov    esi, dword ptr [edx]
        test   esi, esi
        je     SHORT loc_1_1695
        mov    eax, dword ptr [ecx + 4]
loc_1_1658:
        cmp    dword ptr [esi + 0ch], eax
        je     SHORT loc_1_1663
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    SHORT loc_1_1658
loc_1_1663:
        test   esi, esi
        je     SHORT loc_1_1695
        cmp    dword ptr [esi + 20h], 0
        jne    SHORT loc_1_1695
        mov    eax, dword ptr [esi + 10h]
        cmp    dword ptr [ecx + 8], eax
        je     SHORT loc_1_167C
        mov    eax, 0ah
        jmp    SHORT loc_1_169A
loc_1_167C:
        push   0
        push   esi
        push   edx
        call   sub_1_2E0B
        add    esp, 0ch
        push   esi
        call   sub_1_254F
        add    esp, 4
        xor    eax, eax
        jmp    SHORT loc_1_169A
loc_1_1695:
        mov    eax, 7
loc_1_169A:
        pop    esi
        ret
sub_1_169C:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    ax, word ptr [edx]
        mov    word ptr [ecx + 4], ax
        mov    ax, word ptr [edx + 2]
        and    ax, 0ff0fh
        mov    word ptr [ecx + 6], ax
        mov    ax, word ptr [edx + 2]
        shr    ax, 4
        and    al, 0fh
        mov    byte ptr [ecx + 0ah], al
        mov    al, byte ptr [edx + 18h]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 0ch], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [ecx + 14h], eax
        mov    ax, word ptr [edx + 10h]
        mov    word ptr [ecx + 18h], ax
        mov    ax, word ptr [edx + 12h]
        mov    word ptr [ecx + 1ah], ax
        mov    edx, dword ptr [edx + 14h]
        mov    dword ptr [ecx + 1ch], edx
        ret
loc_1_16F3:
        push   ebx
        push   esi
        push   edi
        db  08Bh, 035h, 00Ah, 000h, 000h, 000h
        mov    dx, word ptr [esi + 2]
        push   ebp
        test   dl, 2
        je     SHORT loc_1_1718
        push   dword ptr [esp + 18h]
        push   esi
        call   sub_1_1638
        add    esp, 8
        jmp    NEAR PTR loc_1_1818
loc_1_1718:
        test   dh, 1
        jne    SHORT loc_1_174E
        mov    ecx, dword ptr [esi + 4]
        cmp    ecx, 4000000h
        jb     SHORT loc_1_1732
        mov    eax, 7
        jmp    NEAR PTR loc_1_1818
loc_1_1732:
        mov    eax, dword ptr [esi + 8]
        mov    edi, 4000000h
        cmp    eax, edi
        jae    SHORT loc_1_1744
        add    eax, ecx
        cmp    eax, edi
        jbe    SHORT loc_1_174E
loc_1_1744:
        mov    eax, 0ah
        jmp    NEAR PTR loc_1_1818
loc_1_174E:
        mov    ebp, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi + 8]
        add    ecx, ebp
        mov    edi, dword ptr [esp + 18h]
        test   dl, 1
        je     SHORT loc_1_1764
        add    edi, 28h
        jmp    SHORT loc_1_1767
loc_1_1764:
        add    edi, 24h
loc_1_1767:
        xor    ebx, ebx
        mov    edx, dword ptr [edi]
        cmp    edx, ebx
        je     SHORT loc_1_1787
loc_1_176F:
        mov    eax, dword ptr [edx + 0ch]
        cmp    ebp, eax
        jbe    SHORT loc_1_1787
        mov    ebx, dword ptr [edx + 10h]
        add    ebx, eax
        cmp    ebx, ebp
        ja     SHORT loc_1_17A2
        mov    ebx, edx
        mov    edx, dword ptr [edx]
        test   edx, edx
        jne    SHORT loc_1_176F
loc_1_1787:
        test   edx, edx
        jne    SHORT loc_1_17C1
        push   37h
        call   sub_1_2540
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_17A9
        mov    eax, 20h
        jmp    SHORT loc_1_1818
loc_1_17A2:
        mov    eax, 7
        jmp    SHORT loc_1_1818
loc_1_17A9:
        push   ebx
        push   esi
        call   sub_1_169C
        add    esp, 8
        push   0
        push   ebx
        push   edi
        call   sub_1_2D7C
        add    esp, 0ch
        jmp    SHORT loc_1_1816
loc_1_17C1:
        mov    ebp, dword ptr [edx + 0ch]
        cmp    ecx, ebp
        jbe    SHORT loc_1_17EA
        mov    eax, dword ptr [edx + 10h]
        add    eax, ebp
        cmp    eax, ecx
        jne    SHORT loc_1_17E3
        cmp    dword ptr [edx + 20h], 0
        jne    SHORT loc_1_17E3
        push   edx
        push   esi
        call   sub_1_169C
        add    esp, 8
        jmp    SHORT loc_1_1816
loc_1_17E3:
        mov    eax, 7
        jmp    SHORT loc_1_1818
loc_1_17EA:
        push   37h
        call   sub_1_2540
        add    esp, 4
        mov    ebp, eax
        mov    eax, 20h
        test   ebp, ebp
        je     SHORT loc_1_1818
        push   ebp
        push   esi
        call   sub_1_169C
        add    esp, 8
        push   0
        push   ebp
        push   ebx
        push   edi
        call   sub_1_2DB5
        add    esp, 10h
loc_1_1816:
        xor    eax, eax
loc_1_1818:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
loc_1_181D:
        mov    eax, dword ptr [esp + 8]
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 2ch]
        mov    eax, 1fh
        test   esi, esi
        je     SHORT loc_1_1849
        db  08Bh, 03Dh, 00Ah, 000h, 000h, 000h
        push   edi
        push   esi
        call   sub_1_1530
        mov    ax, word ptr [esi + 26h]
        add    esp, 8
        mov    word ptr [edi + 20h], ax
        xor    eax, eax
loc_1_1849:
        pop    edi
        pop    esi
        ret
loc_1_184C:
        mov    eax, dword ptr [esp + 8]
        push   esi
        push   edi
        db  08Bh, 035h, 00Ah, 000h, 000h, 000h
        mov    ecx, dword ptr [eax + 2ch]
        test   ecx, ecx
        je     SHORT loc_1_1870
        mov    eax, dword ptr [esi + 8]
loc_1_1862:
        cmp    eax, ecx
        je     SHORT loc_1_186C
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_1_1862
loc_1_186C:
        test   ecx, ecx
        jne    SHORT loc_1_1877
loc_1_1870:
        mov    eax, 1ch
        jmp    SHORT loc_1_1896
loc_1_1877:
        mov    edi, dword ptr [ecx]
        mov    eax, 1fh
        test   edi, edi
        je     SHORT loc_1_1896
        push   esi
        push   edi
        call   sub_1_1530
        mov    ax, word ptr [edi + 26h]
        add    esp, 8
        mov    word ptr [esi + 20h], ax
        xor    eax, eax
loc_1_1896:
        pop    edi
        pop    esi
        ret
        db  0CCh, 0CCh, 0CCh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 07Ch, 009h, 004h, 000h, 000h, 000h, 050h, 043h, 043h, 041h
        db  052h, 044h, 020h, 020h, 000h, 000h, 080h, 016h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1980:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1984:
        add    byte ptr [eax], al
loc_1_1986:
        add    byte ptr [eax], al
loc_1_1988:
        add    byte ptr [eax], al
loc_1_198A:
        add    byte ptr [eax], al
loc_1_198C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1990:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1994:
        add    byte ptr [eax], al
loc_1_1996:
        add    byte ptr [eax], al
loc_1_1998:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_199C:
        add    byte ptr [eax], al
loc_1_199E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19B2:
        add    byte ptr [eax], al
loc_1_19B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19D4:
        add    byte ptr [eax], al
loc_1_19D6:
        add    byte ptr [eax], al
loc_1_19D8:
        add    byte ptr [eax], al
loc_1_19DA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19DE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19E4:
        add    byte ptr [eax], al
loc_1_19E6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19F0:
        add    byte ptr [eax], al
loc_1_19F2:
        add    byte ptr [eax], al
loc_1_19F4:
        add    byte ptr [eax], al
loc_1_19F6:
        add    byte ptr [eax], al
loc_1_19F8:
        add    byte ptr [eax], al
loc_1_19FA:
        add    byte ptr [eax], al
loc_1_19FC:
        add    byte ptr [eax], al
loc_1_19FE:
        add    byte ptr [eax], al
loc_1_1A00:
        add    byte ptr [eax], al
loc_1_1A02:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A14:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A18:
        add    byte ptr [eax], al
loc_1_1A1A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A1E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A22:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A26:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A2A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A2E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A32:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A36:
        add    byte ptr [eax], al
loc_1_1A38:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A58:
        add    byte ptr [eax], al
loc_1_1A5A:
        add    byte ptr [eax], al
loc_1_1A5C:
        add    byte ptr [eax], al
loc_1_1A5E:
        add    byte ptr [eax], al
loc_1_1A60:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A64:
        add    byte ptr [eax], al
loc_1_1A66:
        add    byte ptr [eax], al
loc_1_1A68:
        add    byte ptr [eax], al
loc_1_1A6A:
        add    byte ptr [eax], al
loc_1_1A6C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A70:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A74:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A7C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A80:
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
loc_1_1A84:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A88:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A8C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A90:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A94:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A98:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A9C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1B9C:
        dec    ebp
        imul   esp, dword ptr [ebx + 72h], 666f736fh
        je     SHORT loc_1_1BC6
        inc    ebx
        outsd
        db  072h, 070h
        outsd
        jb     SHORT loc_1_1C0E
        db  074h, 069h
        outsd
        outsb
        or     cl, byte ptr [79706f43h]
        db  072h, 069h
        push   39312074h
        cmp    dword ptr [6c410d0ah], esi
        insb
loc_1_1BC6:
        and    byte ptr [edx + 69h], dl
        push   52207374h
        jae    NEAR PTR loc_1_1C37
        db  072h, 076h
        and    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1BDC:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_1BE0:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_1BE4:
        sub    al, 1
        add    byte ptr [eax], al
loc_1_1BE8:
        sub    al, 1
        add    byte ptr [eax], al
loc_1_1BEC:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
loc_1_1BF0:
        add    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi + ecx*2 + 53h], al
        add    byte ptr [esi + 6ch], al
        popad
        jae    SHORT loc_1_1C71
        add    byte ptr [eax], al
        add    byte ptr [esi + 46h], al
loc_1_1C0E:
        push   ebx
        xor    dh, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1C37:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1C71:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
loc_1_1E16:
        add    byte ptr [eax], al
loc_1_1E18:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1E1C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1E20:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1E24:
        add    byte ptr [eax], al
loc_1_1E26:
        add    byte ptr [eax], al
loc_1_1E28:
        add    byte ptr [eax], al
loc_1_1E2A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1E7C:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], ah
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [18000000h], ah
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [edx], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [edx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [edx], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [edx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], bh
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], dl
        add    byte ptr [eax], al
        pop    eax
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    al, byte ptr [eax]
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   cs
        add    byte ptr [eax], al
        db  000h, 099h, 000h, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, 0
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        xchg   edx, eax
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [ebx], dl
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [2000000h], al
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        db  000h, 099h, 000h, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        db  000h, 099h, 000h, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        db  000h, 099h, 000h, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [19000000h], cl
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        adc    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    es
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    al, byte ptr [eax]
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        sbb    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        or     dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        pop    ecx
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    al, 9
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        db  000h, 099h, 000h, 000h, 000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        cwde
        add    byte ptr [eax], al
        db  000h, 000h
        add    byte ptr [eax], al
        add    byte ptr [80000000h], al
        db  03Dh, 000h, 000h, 000h, 000h
        add    byte ptr [ebp + 25h], dh
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_211E:
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2123:
        or     eax, eax
        je     SHORT loc_1_2129
        stc
        ret
loc_1_2129:
        db  0B8h, 000h, 000h, 000h, 000h
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2134:
        add    esp, 4
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 001h
        clc
        ret
PCCARD_Get_Version:
        mov    eax, 400h
        clc
        ret
PCCARD_Card_Services:
        xor    ah, ah
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  066h, 089h, 015h, 002h, 000h, 000h, 000h
        db  089h, 035h, 004h, 000h, 000h, 000h
        db  066h, 089h, 00Dh, 008h, 000h, 000h, 000h
        db  089h, 01Dh, 00Ah, 000h, 000h, 000h
        call   sub_1_4C9B
        db  066h, 08Bh, 015h, 002h, 000h, 000h, 000h
        db  08Bh, 035h, 004h, 000h, 000h, 000h
        db  066h, 08Bh, 00Dh, 008h, 000h, 000h, 000h
        db  08Bh, 01Dh, 00Ah, 000h, 000h, 000h
        or     eax, eax
        je     SHORT loc_1_218D
        stc
loc_1_218D:
        ret
PCCARD_Control:
        cmp    eax, 1
        db  00Fh, 084h, 079h, 0FFh, 0FFh, 0FFh
        cmp    eax, 1bh
        db  00Fh, 084h, 070h, 0FFh, 0FFh, 0FFh
        cmp    eax, 22h
        jne    SHORT loc_1_21AE
        push   edx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_21AC:
        stc
        ret
loc_1_21AE:
        cmp    eax, 6
        jne    SHORT loc_1_21BC
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_21B8:
        cmp    eax, 1
        ret
loc_1_21BC:
        cmp    eax, 23h
        je     SHORT loc_1_222C
        clc
        ret
PCCARD_PM_API:
        mov    ax, word ptr [ebp + 1ch]
        test   ax, ax
        je     SHORT loc_1_220A
        cmp    ax, 100h
        jne    SHORT loc_1_2205
        mov    ax, 3810h
        db  0CDh, 020h, 01Ch, 000h, 001h, 000h
        cmp    eax, -1
        je     SHORT loc_1_21FF
        movzx  ecx, word ptr [ebp + 18h]
        push   ecx
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_21EC:
        add    esp, 8
        mov    word ptr [ebp + 18h], ax
        shr    eax, 10h
        and    byte ptr [ebp + 2ch], 0feh
        or     word ptr [ebp + 2ch], ax
        ret
loc_1_21FF:
        mov    word ptr [ebp + 18h], 0
loc_1_2205:
        or     byte ptr [ebp + 2ch], 1
        ret
loc_1_220A:
        mov    word ptr [ebp + 1ch], 400h
        and    byte ptr [ebp + 2ch], 0feh
        ret
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_2221:
        inc    esp
        db  065h, 076h, 069h
        arpl   word ptr [ebp + 44h], sp
        db  065h, 073h, 063h
        db  000h
loc_1_222C:
        mov    eax, dword ptr [esi + 0ch]
        cmp    eax, 3
        jae    SHORT loc_1_223D
        db  0FFh, 024h, 085h, 000h, 000h, 000h, 000h
loc_1_223D:
        xor    eax, eax
        inc    eax
        clc
        ret
loc_1_2242:
        xor    eax, eax
        ret
loc_1_2245:
        xor    eax, eax
        mov    al, 57h
        clc
        ret
loc_1_224B:
        cmp    dword ptr [esi + 1ch], 4
        jb     SHORT loc_1_2242
        mov    edi, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 20h]
        mov    dword ptr [edi], 400h
        mov    dword ptr [eax], 4
        jmp    SHORT loc_1_2242
loc_1_2265:
        push   dword ptr [esi + 1ch]
        push   dword ptr [esi + 18h]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2270:
        pop    ecx
        pop    ecx
        movzx  ecx, ax
        mov    edi, dword ptr [esi + 20h]
        shl    ecx, 2
        mov    dword ptr [edi], ecx
        shr    eax, 10h
        test   eax, eax
        je     SHORT loc_1_2242
        jmp    SHORT loc_1_2245
loc_1_2286:
        cmp    dword ptr [esi + 14h], 4
        jne    SHORT loc_1_2245
        push   dword ptr [esi + 1ch]
        mov    ebx, dword ptr [esi + 10h]
        mov    eax, esp
        mov    ebx, dword ptr [ebx]
        push   0
        push   eax
        push   dword ptr [esi + 18h]
        push   1
        db  068h, 000h, 000h, 000h, 000h
        push   0
        push   ebx
        db  0CDh, 020h, 03Eh, 000h, 033h, 000h
        add    esp, 1ch
        mov    edi, dword ptr [esi + 20h]
        pop    dword ptr [edi]
        test   eax, eax
        je     SHORT loc_1_2242
        jmp    SHORT loc_1_2245
        db  0CDh, 020h, 001h, 000h, 00Bh, 000h, 0C3h, 003h, 0B6h, 000h, 000h, 000h
sub_1_22C6:
        push   ecx
        push   esi
        sub    esp, 4
        mov    esi, esp
        pushfd
        cli
        in     al, 61h
        mov    byte ptr [esi], al
        and    al, 0fch
        out    61h, al
        mov    al, 0e8h
        out    43h, al
        in     al, 42h
        or     al, 80h
        and    al, 0bfh
        mov    byte ptr [esi + 1], al
        or     al, 30h
        out    43h, al
        in     al, 42h
        mov    byte ptr [esi + 2], al
        mov    al, byte ptr [edi + 2]
        out    42h, al
        in     al, 42h
        mov    byte ptr [esi + 3], al
        mov    al, byte ptr [edi + 3]
        out    42h, al
        mov    al, byte ptr [edi + 1]
        out    43h, al
        mov    ah, al
        test   ah, 10h
        je     SHORT loc_1_230D
        mov    al, byte ptr [edi + 2]
        out    42h, al
loc_1_230D:
        test   ah, 20h
        je     SHORT loc_1_2317
        mov    al, byte ptr [edi + 3]
        out    42h, al
loc_1_2317:
        mov    ah, byte ptr [edi]
        and    ah, 3
        in     al, 61h
        and    al, 0fch
        or     al, ah
        out    61h, al
        popfd
        mov    ecx, 4
        cld
        rep movsb
        add    esp, 4
        pop    esi
        pop    ecx
        sub    eax, eax
        ret
sub_1_2335:
        push   eax
        push   ecx
        push   edx
        push   edi
        db  0BFh, 000h, 000h, 000h, 000h
        call   sub_1_22C6
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 027h, 001h, 001h, 000h
        add    esp, 4
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
        ret
sub_1_235D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        push   es
        db  0BFh, 000h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [edi + 2], al
        mov    byte ptr [edi + 3], ah
        call   sub_1_22C6
        mov    eax, dword ptr [ebp + 0ch]
        mov    edx, 0
        db  08Dh, 035h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 0FEh, 000h, 001h, 000h
        or     esi, esi
        jne    SHORT loc_1_2396
        call   sub_1_2335
        jmp    SHORT loc_1_23B5
loc_1_2396:
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 001h
        push   0
        db  068h, 000h, 000h, 000h, 000h
        db  0CDh, 020h, 0FAh, 000h, 001h, 000h
        pop    ecx
        pop    ecx
        db  080h, 03Dh, 000h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_2396
loc_1_23B5:
        pop    es
        pop    esi
        pop    edi
        pop    ebx
        leave
        ret
        db  0CCh, 011h, 004h
loc_1_23BE:
        adc    byte ptr [edx], al
        db  001h, 005h
loc_1_23C2:
        push   eax
        call   sub_1_8129
        or     eax, eax
        pop    eax
        je     SHORT loc_1_23D5
        db  0CDh, 020h, 004h, 000h, 003h, 000h
        clc
        ret
loc_1_23D5:
        stc
        ret
loc_1_23D7:
        pushad
        push   edx
        call   sub_1_25E9
        add    esp, 4
        clc
        popad
        ret
loc_1_23E4:
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  066h, 089h, 01Dh, 002h, 000h, 000h, 000h
        db  089h, 00Dh, 004h, 000h, 000h, 000h
        db  066h, 089h, 015h, 008h, 000h, 000h, 000h
        db  089h, 03Dh, 00Eh, 000h, 000h, 000h
        pushad
        call   sub_1_5507
        popad
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        db  066h, 08Bh, 01Dh, 002h, 000h, 000h, 000h
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        db  066h, 08Bh, 015h, 008h, 000h, 000h, 000h
        db  08Bh, 035h, 00Ah, 000h, 000h, 000h
        db  08Bh, 03Dh, 00Eh, 000h, 000h, 000h
        clc
        or     ah, ah
        je     SHORT loc_1_2437
        stc
loc_1_2437:
        ret
sub_1_2438:
        pushad
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        db  066h, 08Bh, 01Dh, 002h, 000h, 000h, 000h
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        db  066h, 08Bh, 015h, 008h, 000h, 000h, 000h
        db  08Bh, 035h, 00Ah, 000h, 000h, 000h
        db  08Bh, 03Dh, 00Eh, 000h, 000h, 000h
        db  066h, 08Bh, 02Dh, 012h, 000h, 000h, 000h
        db  0FFh, 015h, 014h, 000h, 000h, 000h
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  066h, 089h, 01Dh, 002h, 000h, 000h, 000h
        db  089h, 00Dh, 004h, 000h, 000h, 000h
        db  066h, 089h, 015h, 008h, 000h, 000h, 000h
        db  089h, 035h, 00Ah, 000h, 000h, 000h
        db  089h, 03Dh, 00Eh, 000h, 000h, 000h
        db  066h, 089h, 02Dh, 012h, 000h, 000h, 000h
        movzx  eax, ah
        mov    dword ptr [esp + 1ch], eax
        popad
        ret
sub_1_24A2:
        push   ebp
        mov    ebp, esp
        pushad
        mov    ax, word ptr [ebp + 8]
        mov    cx, word ptr [ebp + 0ch]
        mov    dx, word ptr [ebp + 10h]
        mov    ebx, dword ptr [ebp + 1ch]
        mov    di, word ptr [ebx]
        mov    esi, dword ptr [ebx + 4]
        mov    ebx, dword ptr [ebp + 14h]
        db  068h, 000h, 000h, 000h, 000h
        push   dword ptr [ebp + 20h]
        mov    ebp, dword ptr [ebp + 18h]
        ret
loc_1_24CA:
        movzx  eax, ax
        mov    dword ptr [esp + 1ch], eax
        popad
        leave
        ret
loc_1_24D4:
        ret
loc_1_24D5:
        pushad
        db  066h, 0A3h, 000h, 000h, 000h, 000h
        db  066h, 089h, 01Dh, 002h, 000h, 000h, 000h
        db  066h, 089h, 00Dh, 004h, 000h, 000h, 000h
        db  066h, 089h, 015h, 006h, 000h, 000h, 000h
        db  089h, 035h, 008h, 000h, 000h, 000h
        db  089h, 03Dh, 00Ch, 000h, 000h, 000h
        movzx  ebx, ax
        shl    ebx, 2
        cmp    ebx, 14h
        jae    SHORT loc_1_2516
        db  0FFh, 093h, 000h, 000h, 000h, 000h
        or     ax, ax
        jne    SHORT loc_1_2517
loc_1_2516:
        stc
loc_1_2517:
        popad
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        db  066h, 08Bh, 01Dh, 002h, 000h, 000h, 000h
        db  066h, 08Bh, 00Dh, 004h, 000h, 000h, 000h
        db  066h, 08Bh, 015h, 006h, 000h, 000h, 000h
        db  08Bh, 035h, 008h, 000h, 000h, 000h
        db  08Bh, 03Dh, 00Ch, 000h, 000h, 000h
        ret
sub_1_2540:
        push   1
        push   dword ptr [esp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_254B:
        add    esp, 8
        ret
sub_1_254F:
        push   0
        push   dword ptr [esp + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_255A:
        add    esp, 8
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 19h
        ret
sub_1_2566:
        push   esi
        push   0eh
        call   sub_1_2540
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     SHORT loc_1_25AB
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 10h]
        push   esi
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], ecx
        push   dword ptr [esp + 10h]
        db  068h, 000h, 000h, 000h, 000h
        mov    word ptr [esi + 0ch], 1
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2599:
        mov    dword ptr [esi + 8], eax
        test   eax, eax
        mov    eax, esi
        jne    SHORT loc_1_25AD
        push   esi
        call   sub_1_254F
        add    esp, 4
loc_1_25AB:
        xor    eax, eax
loc_1_25AD:
        pop    esi
        ret
sub_1_25AF:
        push   esi
        mov    esi, dword ptr [esp + 8]
        movzx  eax, word ptr [esi + 0ch]
        cmp    eax, 1
        jl     SHORT loc_1_25DE
        cmp    eax, 2
        jle    SHORT loc_1_25C9
        cmp    eax, 3
        je     SHORT loc_1_25D3
        jmp    SHORT loc_1_25DE
loc_1_25C9:
        push   dword ptr [esi + 8]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_25D1:
        jmp    SHORT loc_1_25DE
loc_1_25D3:
        push   dword ptr [esi + 8]
        call   sub_1_9880
        add    esp, 4
loc_1_25DE:
        push   esi
        call   sub_1_254F
        add    esp, 4
        pop    esi
        ret
sub_1_25E9:
        push   esi
        mov    esi, dword ptr [esp + 8]
        movzx  eax, word ptr [esi + 0ch]
        cmp    eax, 1
        je     SHORT loc_1_2603
        cmp    eax, 2
        jl     SHORT loc_1_2633
        cmp    eax, 3
        jle    SHORT loc_1_2622
        jmp    SHORT loc_1_2633
loc_1_2603:
        mov    word ptr [esi + 0ch], 3
        push   1
        push   esi
        db  068h, 000h, 000h, 000h, 000h
        call   sub_1_9888
        add    esp, 0ch
        mov    dword ptr [esi + 8], eax
        test   eax, eax
        jne    SHORT loc_1_2633
        jmp    SHORT loc_1_262A
loc_1_2622:
        push   dword ptr [esi + 4]
        call   dword ptr [esi]
        add    esp, 4
loc_1_262A:
        push   esi
        call   sub_1_254F
        add    esp, 4
loc_1_2633:
        pop    esi
        ret
sub_1_2635:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 18h]
        test   eax, eax
        je     SHORT loc_1_264F
        mov    ecx, dword ptr [esp + 8]
loc_1_2644:
        cmp    dword ptr [eax + 0ch], ecx
        je     SHORT loc_1_264F
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_1_2644
loc_1_264F:
        ret
sub_1_2650:
        test   dword ptr [esp + 4], 0fffh
        mov    eax, 0ffffffffh
        jne    SHORT loc_1_2663
        mov    eax, dword ptr [esp + 4]
loc_1_2663:
        ret
sub_1_2664:
        test   dword ptr [esp + 4], 0fc000fffh
        mov    eax, 0ffffffffh
        jne    SHORT loc_1_2677
        mov    eax, dword ptr [esp + 4]
loc_1_2677:
        ret
sub_1_2678:
        mov    edx, dword ptr [esp + 8]
        test   edx, 0fffh
        je     SHORT loc_1_268B
        mov    eax, 0ffffffffh
        jmp    SHORT loc_1_269D
loc_1_268B:
        mov    eax, dword ptr [esp + 4]
        lea    ecx, [eax + edx]
        cmp    ecx, eax
        mov    eax, 0ffffffffh
        jb     SHORT loc_1_269D
        mov    eax, edx
loc_1_269D:
        ret
sub_1_269E:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   ecx, ecx
        je     SHORT loc_1_26BB
        mov    al, byte ptr [esp + 4]
loc_1_26AC:
        cmp    byte ptr [ecx + 0fh], al
        je     SHORT loc_1_26B7
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_1_26AC
loc_1_26B7:
        test   ecx, ecx
        jne    SHORT loc_1_26C2
loc_1_26BB:
        mov    eax, 1
        jmp    SHORT loc_1_26CA
loc_1_26C2:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_1_26CA:
        ret
sub_1_26CB:
        mov    al, byte ptr [esp + 4]
        mov    cl, al
        and    cl, 7
        test   al, 78h
        jne    SHORT loc_1_270B
        movzx  ecx, cl
        cmp    ecx, 1
        je     SHORT loc_1_26F1
        cmp    ecx, 2
        je     SHORT loc_1_26F5
        cmp    ecx, 3
        je     SHORT loc_1_26F9
        cmp    ecx, 4
        je     SHORT loc_1_26FD
        jmp    SHORT loc_1_270B
loc_1_26F1:
        mov    cl, 6
        jmp    SHORT loc_1_26FF
loc_1_26F5:
        mov    cl, 5
        jmp    SHORT loc_1_26FF
loc_1_26F9:
        mov    cl, 4
        jmp    SHORT loc_1_26FF
loc_1_26FD:
        mov    cl, 1
loc_1_26FF:
        and    al, 80h
        and    cl, 0fh
        shl    cl, 3
        or     al, cl
        or     al, 2
loc_1_270B:
        ret
sub_1_270C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cmp    byte ptr [ebp + 0ch], 80h
        push   ebx
        jne    SHORT loc_1_2724
        cmp    dword ptr [ebp + 8], 0
        je     SHORT loc_1_2724
        mov    al, byte ptr [ebp + 10h]
        jmp    SHORT loc_1_27A0
loc_1_2724:
        push   dword ptr [ebp + 0ch]
        call   sub_1_26CB
        add    esp, 4
        mov    bl, al
        push   dword ptr [ebp + 10h]
        call   sub_1_26CB
        add    esp, 4
        mov    byte ptr [ebp - 1], al
        push   dword ptr [ebp + 14h]
        call   sub_1_26CB
        add    esp, 4
        mov    cl, bl
        and    cl, 7
        and    bl, 78h
        shr    bl, 3
        mov    dl, byte ptr [ebp - 1]
        and    dl, 7
        mov    byte ptr [ebp - 2], dl
        mov    dl, byte ptr [ebp - 1]
        and    dl, 78h
        shr    dl, 3
        mov    byte ptr [ebp - 3], dl
        mov    dl, al
        and    dl, 7
        and    al, 78h
        shr    al, 3
        mov    byte ptr [ebp - 1], dl
        test   bl, bl
        je     SHORT loc_1_279E
        cmp    byte ptr [ebp - 2], cl
        jb     SHORT loc_1_279E
        cmp    dl, cl
        ja     SHORT loc_1_279E
        cmp    byte ptr [ebp - 2], cl
        jne    SHORT loc_1_278E
        cmp    byte ptr [ebp - 3], bl
        jb     SHORT loc_1_279E
loc_1_278E:
        cmp    byte ptr [ebp - 1], cl
        jne    SHORT loc_1_2797
        cmp    al, bl
        ja     SHORT loc_1_279E
loc_1_2797:
        mov    al, byte ptr [ebp + 0ch]
        and    al, 7fh
        jmp    SHORT loc_1_27A0
loc_1_279E:
        xor    al, al
loc_1_27A0:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_27A5:
        mov    ecx, dword ptr [esp + 4]
        test   ecx, ecx
        jne    SHORT loc_1_27B1
        xor    eax, eax
        jmp    SHORT loc_1_27C0
loc_1_27B1:
        mov    eax, 1
loc_1_27B6:
        cmp    eax, ecx
        jae    SHORT loc_1_27C0
        add    eax, eax
        test   eax, eax
        jne    SHORT loc_1_27B6
loc_1_27C0:
        ret
sub_1_27C1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 0ch]
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [eax + 18h]
        db  089h, 00Dh, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Fh
        db  089h, 015h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [edi]
        mov    al, byte ptr [eax + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [esi + 12h]
        db  088h, 00Dh, 002h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_2960
        db  066h, 00Fh, 0B6h, 005h, 003h, 000h, 000h, 000h
        mov    word ptr [ebp - 2], ax
        db  066h, 00Fh, 0B6h, 00Dh, 009h, 000h, 000h, 000h
        mov    word ptr [ebp - 4], cx
        mov    cx, ax
        and    cx, 80h
        je     SHORT loc_1_284C
        db  0F6h, 005h, 00Fh, 000h, 000h, 000h, 003h
        jne    SHORT loc_1_2848
        mov    eax, dword ptr [edi]
        cmp    word ptr [eax + 26h], 500h
        jb     SHORT loc_1_2848
        or     byte ptr [esi + 17h], 10h
        jmp    SHORT loc_1_284C
loc_1_2848:
        and    byte ptr [esi + 17h], 0efh
loc_1_284C:
        db  066h, 0A1h, 00Eh, 000h, 000h, 000h
        and    ah, 3
        cmp    ah, 2
        jne    NEAR PTR loc_1_295E
        test   cx, cx
        je     NEAR PTR loc_1_295E
        and    word ptr [ebp - 2], 0ffceh
        and    word ptr [ebp - 4], 0ff8eh
        or     byte ptr [ebp - 2], 40h
        test   byte ptr [esi + 17h], 2
        mov    edi, dword ptr [esi + 18h]
        je     NEAR PTR loc_1_294D
        test   byte ptr [edi + 8], 4
        je     NEAR PTR loc_1_294D
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        call   sub_1_4432
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_28A9
        mov    eax, 19h
        jmp    NEAR PTR loc_1_2960
loc_1_28A9:
        lea    eax, [ebp - 0ch]
        push   eax
        push   dword ptr [edi + 0ch]
        push   dword ptr [ebp - 8]
        call   sub_1_450F
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_28D4
        push   dword ptr [ebp - 8]
        call   sub_1_44FF
        add    esp, 4
        mov    eax, 19h
        jmp    NEAR PTR loc_1_2960
loc_1_28D4:
        mov    eax, dword ptr [ebp - 0ch]
        push   dword ptr [ebp - 8]
        mov    bl, byte ptr [eax + 4]
        call   sub_1_44FF
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_28F0
        mov    eax, 19h
        jmp    SHORT loc_1_2960
loc_1_28F0:
        mov    al, byte ptr [edi + 11h]
        test   al, 11h
        je     SHORT loc_1_2909
        test   bl, 1
        je     SHORT loc_1_2900
        or     byte ptr [ebp - 2], 1
loc_1_2900:
        test   bl, 10h
        je     SHORT loc_1_2909
        or     byte ptr [ebp - 4], 1
loc_1_2909:
        test   al, 22h
        je     SHORT loc_1_2921
        test   bl, 2
        jne    SHORT loc_1_2918
        and    word ptr [ebp - 2], 0ffbfh
loc_1_2918:
        test   bl, 20h
        je     SHORT loc_1_2921
        or     byte ptr [ebp - 4], 40h
loc_1_2921:
        test   al, 44h
        je     SHORT loc_1_2937
        test   bl, 4
        je     SHORT loc_1_292E
        or     byte ptr [ebp - 2], 20h
loc_1_292E:
        test   bl, 40h
        je     SHORT loc_1_2937
        or     byte ptr [ebp - 4], 20h
loc_1_2937:
        test   al, 88h
        je     SHORT loc_1_294D
        test   bl, 8
        je     SHORT loc_1_2944
        or     byte ptr [ebp - 2], 10h
loc_1_2944:
        test   bl, 80h
        je     SHORT loc_1_294D
        or     byte ptr [ebp - 4], 10h
loc_1_294D:
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 4]
        db  0A2h, 003h, 000h, 000h, 000h
        db  088h, 00Dh, 009h, 000h, 000h, 000h
loc_1_295E:
        xor    eax, eax
loc_1_2960:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_2967:
        push   ecx
        call   sub_1_27C1
        add    esp, 4
        test   eax, eax
        mov    ax, 0
        jne    SHORT loc_1_2980
        db  066h, 00Fh, 0B6h, 005h, 003h, 000h, 000h, 000h
loc_1_2980:
        ret
sub_1_2981:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    ecx, [esi + 0ch]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 18h]
        db  089h, 015h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    edi, dword ptr [eax + 1ch]
        db  089h, 03Dh, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [esi + 12h]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Dh
        db  088h, 00Dh, 002h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_2A69
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Eh
        db  0C6h, 005h, 009h, 000h, 000h, 000h, 0FFh
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_2A69
        cmp    dword ptr [ebp + 0ch], 0
        mov    edi, dword ptr [esi + 18h]
        je     SHORT loc_1_2A67
        db  066h, 0A1h, 00Eh, 000h, 000h, 000h
        and    ah, 3
        cmp    ah, 1
        je     SHORT loc_1_2A67
        test   byte ptr [esi + 17h], 2
        je     SHORT loc_1_2A67
        test   byte ptr [edi + 8], 4
        je     SHORT loc_1_2A67
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   sub_1_4432
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_2A22
        mov    eax, 19h
        jmp    SHORT loc_1_2A69
loc_1_2A22:
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [edi + 0ch]
        push   dword ptr [ebp - 4]
        call   sub_1_450F
        add    esp, 0ch
        test   eax, eax
        je     SHORT loc_1_2A4A
        push   dword ptr [ebp - 4]
        call   sub_1_44FF
        add    esp, 4
        mov    eax, 19h
        jmp    SHORT loc_1_2A69
loc_1_2A4A:
        mov    cl, byte ptr [edi + 11h]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 4], cl
        push   dword ptr [ebp - 4]
        call   sub_1_44FF
        add    esp, 4
        test   eax, eax
        mov    eax, 19h
        jne    SHORT loc_1_2A69
loc_1_2A67:
        xor    eax, eax
loc_1_2A69:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_2A6F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushfd
        pop    dword ptr [ebp - 4]
        cli
        db  0C6h, 005h, 000h, 000h, 000h, 000h, 000h
        xor    eax, eax
loc_1_2A83:
        push   eax
        call   sub_1_4FF6
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_2AA1
        cmp    byte ptr [eax + 20h], 2
        je     SHORT loc_1_2A83
        mov    cl, byte ptr [eax + 0ah]
        db  008h, 00Dh, 000h, 000h, 000h, 000h
        jmp    SHORT loc_1_2A83
loc_1_2AA1:
        push   dword ptr [ebp - 4]
        popfd
        mov    esp, ebp
        pop    ebp
        ret
sub_1_2AA9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 8]
        lea    ebx, [edi + 8]
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [eax + 18h]
        db  089h, 00Dh, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 087h
        db  089h, 015h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [ebx]
        mov    al, byte ptr [eax + 0ch]
        db  0C6h, 005h, 003h, 000h, 000h, 000h, 0FFh
        db  0A2h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [edi + 10h]
        db  0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  088h, 00Dh, 009h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    SHORT loc_1_2B4A
        db  00Fh, 0B6h, 00Dh, 002h, 000h, 000h, 000h
        mov    edi, dword ptr [ebp + 0ch]
        test   edi, ecx
        je     NEAR PTR loc_1_2CBF
        test   edi, 3
        jne    NEAR PTR loc_1_2CB9
        test   edi, 4
        je     NEAR PTR loc_1_2CBF
        db  0F6h, 005h, 002h, 000h, 000h, 000h, 003h
        je     NEAR PTR loc_1_2CB9
        db  0C7h, 006h, 024h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_2CBF
loc_1_2B4A:
        cmp    eax, 11h
        jne    NEAR PTR loc_1_2CBF
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [eax + 18h]
        db  089h, 00Dh, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 087h
        db  089h, 015h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [ebx]
        mov    al, byte ptr [eax + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [edi + 10h]
        db  0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  088h, 00Dh, 003h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    NEAR PTR loc_1_2CBF
        db  0BAh, 004h, 000h, 000h, 000h
        mov    edi, dword ptr [ebp + 0ch]
        db  00Fh, 0B6h, 00Dh, 002h, 000h, 000h, 000h
        test   edi, ecx
        je     NEAR PTR loc_1_2CBF
        test   edi, 3
        je     NEAR PTR loc_1_2C55
        db  066h, 08Bh, 00Dh, 004h, 000h, 000h, 000h
        db  066h, 089h, 00Dh, 004h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 006h, 000h, 000h, 000h
        shl    ecx, 0ch
        db  089h, 00Dh, 006h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 008h, 000h, 000h, 000h
        shl    ecx, 0ch
        db  089h, 00Dh, 00Ah, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 00Ah, 000h, 000h, 000h
        shl    ecx, 0ch
        db  089h, 00Dh, 00Eh, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 00Ch, 000h, 000h, 000h
        shl    ecx, 0ch
        db  089h, 00Dh, 012h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 00Eh, 000h, 000h, 000h
        shl    ecx, 0ch
        db  089h, 00Dh, 016h, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 010h, 000h, 000h, 000h
        shl    ecx, 0ch
        db  089h, 00Dh, 01Ah, 000h, 000h, 000h
        db  00Fh, 0B7h, 00Dh, 012h, 000h, 000h, 000h
        shl    ecx, 0ch
        db  089h, 00Dh, 01Eh, 000h, 000h, 000h
        db  08Ah, 015h, 014h, 000h, 000h, 000h
        db  088h, 015h, 022h, 000h, 000h, 000h
        db  08Ah, 01Dh, 015h, 000h, 000h, 000h
        db  088h, 01Dh, 023h, 000h, 000h, 000h
        jmp    SHORT loc_1_2CB9
loc_1_2C55:
        test   edi, 4
        je     SHORT loc_1_2CBF
        db  0F6h, 005h, 002h, 000h, 000h, 000h, 003h
        je     SHORT loc_1_2C6B
        db  0BAh, 016h, 000h, 000h, 000h
loc_1_2C6B:
        mov    cx, word ptr [edx]
        db  066h, 089h, 00Dh, 004h, 000h, 000h, 000h
        movzx  ebx, word ptr [edx + 2]
        db  089h, 01Dh, 006h, 000h, 000h, 000h
        movzx  edi, word ptr [edx + 4]
        db  089h, 03Dh, 00Ah, 000h, 000h, 000h
        movzx  ecx, word ptr [edx + 6]
        db  089h, 00Dh, 00Eh, 000h, 000h, 000h
        movzx  ebx, word ptr [edx + 8]
        db  089h, 01Dh, 012h, 000h, 000h, 000h
        movzx  edi, word ptr [edx + 0ah]
        db  089h, 03Dh, 016h, 000h, 000h, 000h
        mov    cl, byte ptr [edx + 0ch]
        db  088h, 00Dh, 01Ah, 000h, 000h, 000h
        mov    dl, byte ptr [edx + 0dh]
        db  088h, 015h, 01Bh, 000h, 000h, 000h
loc_1_2CB9:
        db  0C7h, 006h, 004h, 000h, 000h, 000h
loc_1_2CBF:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret
sub_1_2CC4:
        push   esi
        xor    esi, esi
loc_1_2CC7:
        push   0
        push   186a0h
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_2CD3:
        mov    ecx, dword ptr [esp + 10h]
        add    esp, 8
        call   sub_1_2967
        test   al, 80h
        je     SHORT loc_1_2CED
        test   al, 40h
        jne    SHORT loc_1_2CF4
        inc    esi
        cmp    esi, 1eh
        jb     SHORT loc_1_2CC7
loc_1_2CED:
        mov    eax, 19h
loc_1_2CF2:
        pop    esi
        ret
loc_1_2CF4:
        xor    eax, eax
        jmp    SHORT loc_1_2CF2
sub_1_2CF8:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        lea    edx, [ecx + 0ch]
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [edx]
        mov    eax, dword ptr [eax + 18h]
        db  0A3h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [edx]
        mov    eax, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Ch
        db  0A3h, 00Ah, 000h, 000h, 000h
        mov    edx, dword ptr [edx]
        mov    al, byte ptr [edx + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [ecx + 12h]
        db  0C7h, 005h, 00Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  088h, 00Dh, 002h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    SHORT loc_1_2D54
        db  08Bh, 00Dh, 00Ah, 000h, 000h, 000h
        mov    dword ptr [esi], ecx
loc_1_2D54:
        pop    edi
        pop    esi
        ret
sub_1_2D57:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        pushfd
        pop    dword ptr [ebp - 4]
        cli
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    esi, dword ptr [eax]
        mov    dword ptr [ecx + edx], esi
        mov    dword ptr [eax], ecx
        push   dword ptr [ebp - 4]
        popfd
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_2D7C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        pushfd
        pop    dword ptr [ebp - 4]
        cli
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp + 10h]
        xor    eax, eax
        mov    esi, dword ptr [ebp + 8]
        sub    esi, edx
        mov    dword ptr [edx + ecx], eax
        je     SHORT loc_1_2DAC
loc_1_2D9A:
        add    esi, edx
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     SHORT loc_1_2DAA
        mov    esi, eax
        test   esi, esi
        jne    SHORT loc_1_2D9A
        jmp    SHORT loc_1_2DAC
loc_1_2DAA:
        mov    dword ptr [esi], ecx
loc_1_2DAC:
        push   dword ptr [ebp - 4]
        popfd
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_2DB5:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        test   eax, eax
        je     SHORT loc_1_2DC7
        mov    ecx, dword ptr [ebp + 14h]
        add    ecx, eax
        jmp    SHORT loc_1_2DCA
loc_1_2DC7:
        mov    ecx, dword ptr [ebp + 8]
loc_1_2DCA:
        push   dword ptr [ebp + 14h]
        push   dword ptr [ebp + 10h]
        push   ecx
        call   sub_1_2D57
        mov    esp, ebp
        pop    ebp
        ret
sub_1_2DDA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        pushfd
        pop    dword ptr [ebp - 4]
        cli
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        xor    eax, eax
        mov    ecx, dword ptr [esi]
        test   ecx, ecx
        je     SHORT loc_1_2E01
        mov    eax, ecx
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + ecx]
        mov    dword ptr [esi], edx
        mov    dword ptr [eax + ecx], edi
loc_1_2E01:
        push   dword ptr [ebp - 4]
        popfd
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_2E0B:
        push   ebp
        mov    ecx, dword ptr [esp + 10h]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   edi
        lea    esi, [eax + ecx]
        pushfd
        pop    dword ptr [ebp - 4]
        cli
        mov    edi, dword ptr [ebp + 8]
        sub    edi, ecx
        je     SHORT loc_1_2E3D
loc_1_2E29:
        add    edi, ecx
        mov    edx, dword ptr [edi]
        cmp    eax, edx
        je     SHORT loc_1_2E39
        mov    edi, edx
        test   edi, edi
        jne    SHORT loc_1_2E29
        jmp    SHORT loc_1_2E3D
loc_1_2E39:
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi], eax
loc_1_2E3D:
        push   dword ptr [ebp - 4]
        popfd
        pop    edi
        mov    dword ptr [esi], 0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
sub_1_2E4D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        db  08Bh, 00Dh, 004h, 000h, 000h, 000h
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 0ch]
        add    esi, dword ptr [ebp + 0ch]
        test   byte ptr [ecx + 17h], 10h
        jne    NEAR PTR loc_1_2F86
        db  083h, 03Dh, 008h, 000h, 000h, 000h, 000h
        jne    SHORT loc_1_2E8E
        db  068h, 008h, 000h, 000h, 000h
        push   ecx
        call   sub_1_4432
        add    esp, 8
        test   eax, eax
        jne    NEAR PTR loc_1_3108
loc_1_2E8E:
        xor    edx, edx
        add    edi, 6
        db  08Bh, 01Dh, 008h, 000h, 000h, 000h
        test   byte ptr [edi], 5
        je     SHORT loc_1_2EC0
        add    ebx, 14h
        mov    cx, word ptr [ebx]
        mov    ax, cx
        and    al, 0ah
        cmp    al, 8
        je     SHORT loc_1_2EE6
        and    cx, 0fffdh
        mov    word ptr [ebx], cx
        db  0A1h, 008h, 000h, 000h, 000h
        or     byte ptr [eax + 14h], 8
        jmp    SHORT loc_1_2EE1
loc_1_2EC0:
        add    ebx, 14h
        mov    ax, word ptr [ebx]
        mov    cx, ax
        and    cl, 0ah
        cmp    cl, 2
        je     SHORT loc_1_2EE6
        or     al, 2
        mov    word ptr [ebx], ax
        db  0A1h, 008h, 000h, 000h, 000h
        and    word ptr [eax + 14h], 0fff7h
loc_1_2EE1:
        mov    edx, 1
loc_1_2EE6:
        test   edx, edx
        je     SHORT loc_1_2EFA
        mov    eax, esi
        db  08Bh, 00Dh, 008h, 000h, 000h, 000h
        and    eax, 0fffff000h
        mov    dword ptr [ecx + 16h], eax
loc_1_2EFA:
        db  0A1h, 008h, 000h, 000h, 000h
        push   eax
        call   sub_1_4053
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_3108
        db  0A1h, 008h, 000h, 000h, 000h
        push   eax
        call   sub_1_4154
        add    esp, 4
        test   eax, eax
        jne    NEAR PTR loc_1_3108
        test   byte ptr [edi], 4
        lea    eax, [ebp - 4]
        push   eax
        je     SHORT loc_1_2F65
        push   2
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        call   sub_1_450F
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_3108
        xor    ecx, ecx
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edi]
        and    ecx, 1
        mov    word ptr [eax], cx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 2], esi
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 6]
        jmp    NEAR PTR loc_1_310A
loc_1_2F65:
        push   esi
        db  0FFh, 035h, 008h, 000h, 000h, 000h
        call   sub_1_450F
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_3108
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        jmp    NEAR PTR loc_1_310A
loc_1_2F86:
        db  039h, 035h, 010h, 000h, 000h, 000h
        ja     SHORT loc_1_2FBE
        db  0A1h, 014h, 000h, 000h, 000h
        db  003h, 005h, 010h, 000h, 000h, 000h
        cmp    eax, esi
        jbe    SHORT loc_1_2FBE
        mov    ax, word ptr [edi + 6]
        db  066h, 033h, 005h, 000h, 000h, 000h, 000h
        test   ah, 7
        jne    SHORT loc_1_2FBE
        db  02Bh, 035h, 010h, 000h, 000h, 000h
        db  08Ah, 086h, 018h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_310A
loc_1_2FBE:
        movzx  ecx, byte ptr [edi + 1]
        db  0A1h, 004h, 000h, 000h, 000h
        imul   ecx, ecx, 28h
        mov    edx, dword ptr [eax + 18h]
        xor    eax, eax
        mov    ebx, dword ptr [ecx + edx + 4]
        xor    ecx, ecx
        mov    edx, 700h
        db  089h, 00Dh, 014h, 000h, 000h, 000h
        db  080h, 025h, 001h, 000h, 000h, 000h, 0F8h
        mov    ax, word ptr [edi + 6]
        and    eax, edx
        db  089h, 035h, 010h, 000h, 000h, 000h
        db  009h, 005h, 000h, 000h, 000h, 000h
        mov    ax, word ptr [edi + 6]
        and    ax, dx
        jne    SHORT loc_1_3035
        mov    eax, esi
        push   ecx
        and    al, 0fch
        push   4
        db  068h, 018h, 000h, 000h, 000h
        db  0A3h, 010h, 000h, 000h, 000h
        push   eax
        push   ecx
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_301B:
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_1_3108
        db  0C7h, 005h, 014h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        jmp    NEAR PTR loc_1_30E3
loc_1_3035:
        cmp    ax, 600h
        push   0
        push   4
        ja     SHORT loc_1_3097
        movzx  eax, ax
        lea    ecx, [ebp - 0ch]
        dec    eax
        sar    eax, 6
        push   ecx
        and    al, 0fch
        add    eax, 10h
        push   eax
        push   0
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3058:
        add    esp, 18h
        test   eax, eax
        jne    NEAR PTR loc_1_3108
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   100h
        and    al, 0f0h
        add    eax, esi
        push   eax
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3077:
        add    esp, 0ch
        mov    dword ptr [ebp - 4], eax
        xor    eax, eax
loc_1_307F:
        mov    ecx, dword ptr [ebp - 4]
        inc    eax
        mov    cl, byte ptr [ecx]
        db  088h, 088h, 017h, 000h, 000h, 000h
        inc    dword ptr [ebp - 4]
        cmp    eax, 100h
        jb     SHORT loc_1_307F
        jmp    SHORT loc_1_30DE
loc_1_3097:
        lea    ecx, [ebp - 8]
        push   ecx
        push   0
        push   3
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_30A5:
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_1_3108
        cmp    dword ptr [ebp - 8], esi
        jbe    SHORT loc_1_3108
        mov    eax, 100h
        sub    dword ptr [ebp - 8], esi
        cmp    dword ptr [ebp - 8], eax
        jbe    SHORT loc_1_30C1
        mov    dword ptr [ebp - 8], eax
loc_1_30C1:
        push   0
        push   dword ptr [ebp - 8]
        db  068h, 018h, 000h, 000h, 000h
        push   esi
        push   4
        push   ebx
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_30D4:
        add    esp, 18h
        test   eax, eax
        jne    SHORT loc_1_3108
        mov    eax, dword ptr [ebp - 8]
loc_1_30DE:
        db  0A3h, 014h, 000h, 000h, 000h
loc_1_30E3:
        db  039h, 035h, 010h, 000h, 000h, 000h
        ja     SHORT loc_1_3108
        db  0A1h, 014h, 000h, 000h, 000h
        db  003h, 005h, 010h, 000h, 000h, 000h
        cmp    eax, esi
        jbe    SHORT loc_1_3108
        db  02Bh, 035h, 010h, 000h, 000h, 000h
        db  08Ah, 086h, 018h, 000h, 000h, 000h
        jmp    SHORT loc_1_310A
loc_1_3108:
        mov    al, 0ffh
loc_1_310A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3111:
        push   esi
        db  0A1h, 004h, 000h, 000h, 000h
        mov    esi, dword ptr [esp + 8]
        push   edi
        test   byte ptr [eax + 17h], 10h
        jne    SHORT loc_1_3132
        mov    word ptr [esi + 6], 10h
        xor    eax, eax
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 0ch], eax
        jmp    SHORT loc_1_3181
loc_1_3132:
        db  0A1h, 004h, 000h, 000h, 000h
        lea    edi, [esi + 0ch]
        movzx  edx, byte ptr [esi + 1]
        mov    ecx, dword ptr [eax + 18h]
        push   0
        imul   edx, edx, 28h
        push   4
        push   edi
        push   28h
        push   0
        push   dword ptr [edx + ecx + 4]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3156:
        add    esp, 18h
        test   eax, eax
        je     SHORT loc_1_3164
        mov    eax, 1fh
        jmp    SHORT loc_1_3183
loc_1_3164:
        mov    word ptr [esi + 6], 11h
        mov    eax, dword ptr [edi]
        mov    ecx, eax
        and    al, 0f8h
        and    cx, 7
        mov    dword ptr [edi], eax
        mov    ch, cl
        mov    dword ptr [esi + 8], eax
        mov    cl, 11h
        mov    word ptr [esi + 6], cx
loc_1_3181:
        xor    eax, eax
loc_1_3183:
        pop    edi
        pop    esi
        ret
sub_1_3186:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        mov    ebx, 2
        mov    ax, word ptr [esi + 6]
        push   0
        and    ax, 1
        push   esi
        cmp    ax, 1
        adc    ebx, -1
        call   sub_1_2E4D
        add    esp, 8
        cmp    al, 13h
        jne    SHORT loc_1_31FB
        push   ebx
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        cmp    al, 3
        jb     SHORT loc_1_31FB
        lea    eax, [ebx*2]
        push   eax
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        cmp    al, 43h
        jne    SHORT loc_1_31FB
        mov    eax, ebx
        imul   eax, eax, 3
        push   eax
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        cmp    al, 49h
        jne    SHORT loc_1_31FB
        shl    ebx, 2
        push   ebx
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        cmp    al, 53h
        mov    eax, 1
        je     SHORT loc_1_31FD
loc_1_31FB:
        xor    eax, eax
loc_1_31FD:
        pop    esi
        pop    ebx
        ret
sub_1_3200:
        push   ebx
        xor    eax, eax
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 10h]
        lea    edi, [esi + 6]
        mov    dx, word ptr [edi]
        mov    ax, dx
        and    eax, 70h
        cmp    eax, 10h
        je     SHORT loc_1_3230
        cmp    eax, 30h
        je     SHORT loc_1_3240
        cmp    eax, 40h
        je     SHORT loc_1_3230
        cmp    eax, 50h
        je     SHORT loc_1_3252
        mov    eax, 1fh
        jmp    SHORT loc_1_32AB
loc_1_3230:
        or     dl, 1
        lea    ebx, [esi + 0ch]
        mov    word ptr [edi], dx
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebx], eax
        jmp    SHORT loc_1_3275
loc_1_3240:
        and    dx, 0fffeh
        lea    ebx, [esi + 0ch]
        mov    word ptr [edi], dx
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebx], eax
        jmp    SHORT loc_1_3275
loc_1_3252:
        and    dx, 0f8ffh
        mov    ecx, dword ptr [esi + 8]
        mov    word ptr [edi], dx
        mov    eax, ecx
        and    ax, 7
        lea    ebx, [esi + 0ch]
        shl    ax, 8
        and    cl, 0f8h
        or     ax, dx
        mov    dword ptr [ebx], ecx
        mov    word ptr [edi], ax
loc_1_3275:
        push   esi
        call   sub_1_3186
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_32A4
        test   byte ptr [edi], 5
        je     SHORT loc_1_328E
        mov    eax, 1fh
        jmp    SHORT loc_1_32AB
loc_1_328E:
        shr    dword ptr [ebx], 1
        push   esi
        call   sub_1_3186
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_32A4
        mov    eax, 1fh
        jmp    SHORT loc_1_32AB
loc_1_32A4:
        and    word ptr [edi], 0ff8fh
        xor    eax, eax
loc_1_32AB:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_32AF:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_3200
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_32C5
        xor    eax, eax
        jmp    SHORT loc_1_3307
loc_1_32C5:
        lea    ecx, [esi + 6]
        mov    ax, word ptr [ecx]
        test   al, 8
        je     SHORT loc_1_3302
        test   al, 4
        jne    SHORT loc_1_3302
        and    ax, 0ff86h
        push   esi
        mov    dword ptr [esi + 0ch], 0
        mov    word ptr [ecx], ax
        or     al, 4
        mov    word ptr [ecx], ax
        call   sub_1_3186
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_32F7
        xor    eax, eax
        jmp    SHORT loc_1_3307
loc_1_32F7:
        push   esi
        call   sub_1_3200
        add    esp, 4
        jmp    SHORT loc_1_3307
loc_1_3302:
        mov    eax, 1fh
loc_1_3307:
        pop    esi
        ret
sub_1_3309:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0ch]
        push   edi
        mov    dx, word ptr [esi + 6]
        push   ebp
        mov    ax, dx
        mov    edi, 2
        and    ax, 1
        cmp    ax, 1
        mov    al, byte ptr [esi + 10h]
        movzx  ecx, al
        adc    edi, -1
        cmp    ecx, 2
        je     SHORT loc_1_3365
        cmp    ecx, 3
        je     NEAR PTR loc_1_33D0
        cmp    ecx, 6
        je     NEAR PTR loc_1_33D8
        cmp    ecx, 11h
        jl     NEAR PTR loc_1_3546
        cmp    ecx, 12h
        jle    NEAR PTR loc_1_34AB
        cmp    ecx, 14h
        je     NEAR PTR loc_1_353A
        jmp    NEAR PTR loc_1_3546
loc_1_3365:
        cmp    byte ptr [esi + 11h], 4
        jae    SHORT loc_1_3375
        mov    eax, 1fh
        jmp    NEAR PTR loc_1_3548
loc_1_3375:
        and    dx, 0ffdfh
        push   5
        or     dl, 50h
        push   esi
        mov    word ptr [esi + 6], dx
        call   sub_1_2E4D
        movzx  edi, al
        shl    edi, 8
        add    esp, 8
        push   4
        push   esi
        call   sub_1_2E4D
        movzx  eax, al
        add    esp, 8
        add    edi, eax
        shl    edi, 8
        push   3
        push   esi
        call   sub_1_2E4D
        movzx  eax, al
        add    esp, 8
        add    edi, eax
        shl    edi, 8
        push   2
        push   esi
        call   sub_1_2E4D
        movzx  eax, al
        add    esp, 8
        add    edi, eax
        mov    dword ptr [esi + 8], edi
        jmp    NEAR PTR loc_1_3546
loc_1_33D0:
        or     dl, 8
        jmp    NEAR PTR loc_1_3542
loc_1_33D8:
        mov    bl, byte ptr [esi + 1]
        movzx  ecx, byte ptr [esi + 11h]
        movzx  eax, bl
        imul   eax, eax, 5
        add    eax, 6
        cmp    eax, ecx
        jg     NEAR PTR loc_1_34A1
        lea    eax, [edi*2]
        push   eax
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        cmp    al, bl
        jbe    NEAR PTR loc_1_34A1
        movzx  eax, byte ptr [esi + 1]
        imul   eax, eax, 5
        inc    eax
        imul   eax, edi
        lea    ebp, [eax + edi*2]
        push   ebp
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        cmp    al, 1
        sbb    ecx, ecx
        add    ebp, edi
        mov    ax, word ptr [esi + 6]
        and    cl, 0f0h
        and    ax, 0ff8fh
        add    ecx, 40h
        or     cx, ax
        mov    eax, edi
        imul   eax, eax, 3
        mov    word ptr [esi + 6], cx
        add    eax, ebp
        push   eax
        push   esi
        call   sub_1_2E4D
        movzx  ebx, al
        shl    ebx, 8
        add    esp, 8
        lea    eax, [ebp + edi*2]
        push   eax
        push   esi
        add    edi, ebp
        call   sub_1_2E4D
        movzx  ecx, al
        add    esp, 8
        add    ebx, ecx
        shl    ebx, 8
        push   edi
        push   esi
        call   sub_1_2E4D
        movzx  eax, al
        add    esp, 8
        add    ebx, eax
        shl    ebx, 8
        push   ebp
        push   esi
        call   sub_1_2E4D
        movzx  eax, al
        add    esp, 8
        add    ebx, eax
        test   byte ptr [esi + 6], 30h
        mov    dword ptr [esi + 8], ebx
        je     NEAR PTR loc_1_3546
        shl    dword ptr [esi + 8], 1
        jmp    NEAR PTR loc_1_3546
loc_1_34A1:
        mov    eax, 1fh
        jmp    NEAR PTR loc_1_3548
loc_1_34AB:
        cmp    byte ptr [esi + 11h], 4
        jae    SHORT loc_1_34BB
        mov    eax, 1fh
        jmp    NEAR PTR loc_1_3548
loc_1_34BB:
        lea    ebp, [edi*2]
        sub    al, 11h
        cmp    al, 1
        sbb    eax, eax
        and    dx, 0ff8fh
        and    al, 0f0h
        add    eax, 40h
        or     ax, dx
        mov    word ptr [esi + 6], ax
        mov    eax, edi
        imul   eax, eax, 3
        add    eax, ebp
        push   eax
        push   esi
        call   sub_1_2E4D
        movzx  ebx, al
        shl    ebx, 8
        add    esp, 8
        lea    eax, [ebp*2]
        push   eax
        push   esi
        add    edi, ebp
        call   sub_1_2E4D
        movzx  ecx, al
        add    esp, 8
        add    ebx, ecx
        shl    ebx, 8
        push   edi
        push   esi
        call   sub_1_2E4D
        movzx  eax, al
        add    esp, 8
        add    ebx, eax
        shl    ebx, 8
        push   ebp
        push   esi
        call   sub_1_2E4D
        movzx  eax, al
        add    esp, 8
        add    ebx, eax
        test   byte ptr [esi + 6], 30h
        mov    dword ptr [esi + 8], ebx
        je     SHORT loc_1_3546
        shl    dword ptr [esi + 8], 1
        jmp    SHORT loc_1_3546
loc_1_353A:
        and    dx, 0ffafh
        or     dl, 20h
loc_1_3542:
        mov    word ptr [esi + 6], dx
loc_1_3546:
        xor    eax, eax
loc_1_3548:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_354D:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0ch]
        mov    edi, 2
        mov    ax, word ptr [esi + 6]
        push   0
        and    ax, 1
        push   esi
        cmp    ax, 1
        adc    edi, -1
        call   sub_1_2E4D
        movzx  eax, al
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_359C
        cmp    eax, 0ffh
        je     SHORT loc_1_35A6
        push   edi
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        cmp    al, 0ffh
        je     SHORT loc_1_35A6
        movzx  eax, al
        add    eax, 2
        imul   eax, edi
        add    dword ptr [esi + 0ch], eax
        jmp    SHORT loc_1_359F
loc_1_359C:
        add    dword ptr [esi + 0ch], edi
loc_1_359F:
        mov    eax, 1
        jmp    SHORT loc_1_35A8
loc_1_35A6:
        xor    eax, eax
loc_1_35A8:
        pop    edi
        pop    esi
        ret
sub_1_35AB:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 10h]
        push   edi
        mov    ax, word ptr [esi + 6]
        push   ebp
        and    ax, 1
        mov    ebp, 2
        cmp    ax, 1
        push   0
        adc    ebp, -1
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        mov    bl, al
        db  0A1h, 004h, 000h, 000h, 000h
        test   byte ptr [eax + 17h], 10h
        je     SHORT loc_1_35EB
        cmp    bl, 13h
        jne    NEAR PTR loc_1_3715
loc_1_35EB:
        cmp    bl, 0ffh
        je     SHORT loc_1_35F5
        cmp    bl, 13h
        jne    SHORT loc_1_362A
loc_1_35F5:
        push   esi
        call   sub_1_32AF
        add    esp, 4
        cmp    eax, 1fh
        je     NEAR PTR loc_1_3715
        push   esi
        call   sub_1_354D
        add    esp, 4
        test   eax, eax
        je     NEAR PTR loc_1_3715
        mov    ebp, 1
        push   0
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        mov    bl, al
loc_1_362A:
        db  0A1h, 004h, 000h, 000h, 000h
        test   byte ptr [eax + 17h], 10h
        jne    SHORT loc_1_363E
        cmp    bl, 1
        jne    NEAR PTR loc_1_3715
loc_1_363E:
        xor    edi, edi
        mov    dword ptr [esp + 10h], edi
loc_1_3644:
        push   ebp
        push   esi
        call   sub_1_2E4D
        movzx  ecx, bl
        add    esp, 8
        cmp    ecx, 6
        jg     SHORT loc_1_366D
        je     SHORT loc_1_36A1
        test   ecx, ecx
        je     SHORT loc_1_36C0
        cmp    ecx, 1
        je     SHORT loc_1_3693
        cmp    ecx, 2
        jl     SHORT loc_1_3684
        cmp    ecx, 3
        jle    SHORT loc_1_36A1
        jmp    SHORT loc_1_3684
loc_1_366D:
        cmp    ecx, 11h
        jl     SHORT loc_1_3684
        cmp    ecx, 12h
        jle    SHORT loc_1_36A1
        cmp    ecx, 14h
        je     SHORT loc_1_36A1
        cmp    ecx, 0ffh
        je     SHORT loc_1_36C0
loc_1_3684:
        test   al, al
        jne    SHORT loc_1_36C0
        inc    edi
        cmp    edi, 8
        jne    SHORT loc_1_36C0
        jmp    NEAR PTR loc_1_3715
loc_1_3693:
        test   al, al
        je     SHORT loc_1_3715
        cmp    al, 20h
        jbe    SHORT loc_1_36C0
        cmp    al, 0ffh
        jae    SHORT loc_1_36C0
        jmp    SHORT loc_1_3715
loc_1_36A1:
        mov    cx, word ptr [esi + 6]
        and    cl, 70h
        cmp    cl, 10h
        ja     SHORT loc_1_3715
        push   esi
        mov    byte ptr [esi + 10h], bl
        mov    byte ptr [esi + 11h], al
        call   sub_1_3309
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_3715
loc_1_36C0:
        push   esi
        call   sub_1_354D
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_36F6
        db  066h, 0FFh, 005h, 000h, 000h, 000h, 000h
        push   esi
        call   sub_1_32AF
        add    esp, 4
        cmp    eax, 1fh
        je     SHORT loc_1_3722
        mov    ax, word ptr [esi + 6]
        mov    ebp, 2
        and    ax, 1
        cmp    ax, 1
        adc    ebp, -1
loc_1_36F6:
        push   0
        push   esi
        call   sub_1_2E4D
        inc    dword ptr [esp + 18h]
        add    esp, 8
        mov    bl, al
        cmp    dword ptr [esp + 10h], 100h
        jl     NEAR PTR loc_1_3644
loc_1_3715:
        mov    eax, 1fh
loc_1_371A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret
loc_1_3722:
        xor    eax, eax
        jmp    SHORT loc_1_371A
sub_1_3726:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0ch]
        mov    esi, 2
        mov    ax, word ptr [edi + 6]
        push   edi
        and    ax, 1
        cmp    ax, 1
        adc    esi, -1
        call   sub_1_354D
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_376E
        push   edi
        call   sub_1_32AF
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_3792
        mov    ax, word ptr [edi + 6]
        mov    esi, 2
        and    ax, 1
        cmp    ax, 1
        adc    esi, -1
loc_1_376E:
        push   0
        push   edi
        call   sub_1_2E4D
        add    esp, 8
        mov    byte ptr [edi + 10h], al
        push   esi
        push   edi
        call   sub_1_2E4D
        add    esp, 8
        mov    byte ptr [edi + 11h], al
        push   edi
        call   sub_1_3309
        add    esp, 4
loc_1_3792:
        pop    edi
        pop    esi
        ret
sub_1_3795:
        mov    eax, dword ptr [esp + 4]
        mov    dl, byte ptr [eax + 4]
        mov    cl, byte ptr [eax + 10h]
        cmp    dl, cl
        jne    SHORT loc_1_37AA
        mov    eax, 1
        jmp    SHORT loc_1_37E6
loc_1_37AA:
        cmp    dl, 0ffh
        jne    SHORT loc_1_37E4
        test   byte ptr [eax + 2], 1
        je     SHORT loc_1_37BC
        mov    eax, 1
        jmp    SHORT loc_1_37E6
loc_1_37BC:
        cmp    cl, 2
        je     SHORT loc_1_37E4
        cmp    cl, 3
        je     SHORT loc_1_37E4
        cmp    cl, 6
        je     SHORT loc_1_37E4
        cmp    cl, 11h
        je     SHORT loc_1_37E4
        cmp    cl, 12h
        je     SHORT loc_1_37E4
        cmp    cl, 14h
        je     SHORT loc_1_37E4
        mov    eax, 1
        cmp    cl, 13h
        jne    SHORT loc_1_37E6
loc_1_37E4:
        xor    eax, eax
loc_1_37E6:
        ret
sub_1_37E7:
        push   esi
        mov    esi, dword ptr [esp + 8]
        db  039h, 035h, 004h, 000h, 000h, 000h
        je     SHORT loc_1_3804
        push   0
        call   sub_1_3806
        add    esp, 4
        db  089h, 035h, 004h, 000h, 000h, 000h
loc_1_3804:
        pop    esi
        ret
sub_1_3806:
        push   esi
        xor    esi, esi
        db  039h, 035h, 008h, 000h, 000h, 000h
        je     SHORT loc_1_3825
        db  0A1h, 008h, 000h, 000h, 000h
        push   eax
        call   sub_1_44FF
        add    esp, 4
        db  089h, 035h, 008h, 000h, 000h, 000h
loc_1_3825:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     SHORT loc_1_3835
        db  03Bh, 005h, 004h, 000h, 000h, 000h
        jne    SHORT loc_1_384B
loc_1_3835:
        xor    eax, eax
        db  0A3h, 004h, 000h, 000h, 000h
        db  0A3h, 010h, 000h, 000h, 000h
        db  0A3h, 014h, 000h, 000h, 000h
        db  0A3h, 00Ch, 000h, 000h, 000h
loc_1_384B:
        pop    esi
        ret
sub_1_384D:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 18h]
        test   word ptr [edi + 2], 0fffeh
        je     SHORT loc_1_3867
        mov    eax, 2
        jmp    NEAR PTR loc_1_3901
loc_1_3867:
        mov    esi, dword ptr [esp + 14h]
        push   esi
        call   sub_1_37E7
        add    esp, 4
        push   edi
        call   sub_1_3111
        add    esp, 4
        test   eax, eax
        je     SHORT loc_1_3888
        mov    eax, 1fh
        jmp    SHORT loc_1_3901
loc_1_3888:
        mov    ax, word ptr [edi + 6]
        xor    ebx, ebx
        and    ax, 1
        mov    ebp, 2
        cmp    ax, 1
        push   ebx
        adc    ebp, -1
        push   edi
        call   sub_1_2E4D
        add    esp, 8
        mov    byte ptr [edi + 10h], al
        push   ebp
        push   edi
        call   sub_1_2E4D
        add    esp, 8
        mov    byte ptr [edi + 11h], al
        cmp    byte ptr [edi + 10h], 13h
        jne    SHORT loc_1_38CE
        push   edi
        call   sub_1_32AF
        add    esp, 4
        cmp    eax, 1fh
        jne    SHORT loc_1_38D9
        jmp    SHORT loc_1_38D4
loc_1_38CE:
        test   byte ptr [esi + 17h], 10h
        je     SHORT loc_1_38D9
loc_1_38D4:
        mov    ebx, 1fh
loc_1_38D9:
        test   ebx, ebx
        jne    SHORT loc_1_38F6
        push   edi
        call   sub_1_3795
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_38F6
        push   edi
        push   esi
        call   sub_1_3906
        add    esp, 8
        jmp    SHORT loc_1_3901
loc_1_38F6:
        push   esi
        call   sub_1_3806
        add    esp, 4
        mov    eax, ebx
loc_1_3901:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_3906:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 14h]
        test   word ptr [edi + 2], 0fffeh
        je     SHORT loc_1_391C
        mov    eax, 2
        jmp    SHORT loc_1_3963
loc_1_391C:
        push   dword ptr [esp + 10h]
        call   sub_1_37E7
        xor    si, si
        add    esp, 4
loc_1_392B:
        push   edi
        call   sub_1_3726
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    SHORT loc_1_3955
        push   edi
        call   sub_1_3795
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_3955
        inc    si
        mov    ebx, 1fh
        cmp    si, 100h
        jb     SHORT loc_1_392B
loc_1_3955:
        push   dword ptr [esp + 10h]
        call   sub_1_3806
        add    esp, 4
        mov    eax, ebx
loc_1_3963:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_3967:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        test   word ptr [esi + 2], 0fffeh
        je     SHORT loc_1_3985
        mov    eax, 2
        jmp    NEAR PTR loc_1_3A15
loc_1_3985:
        push   dword ptr [ebp + 8]
        call   sub_1_37E7
        mov    ax, word ptr [esi + 6]
        add    esp, 4
        and    ax, 1
        cmp    ax, 1
        mov    eax, 2
        adc    eax, -1
        push   eax
        mov    dword ptr [ebp - 8], eax
        push   esi
        call   sub_1_2E4D
        movzx  di, al
        mov    word ptr [esi + 12h], di
        add    esp, 8
        movzx  ax, byte ptr [esi + 5]
        mov    word ptr [ebp - 2], ax
        cmp    di, ax
        jae    SHORT loc_1_39CE
        mov    esi, 1fh
        jmp    SHORT loc_1_3A08
loc_1_39CE:
        mov    ax, word ptr [esi + 10h]
        sub    di, word ptr [ebp - 2]
        cmp    di, ax
        jbe    SHORT loc_1_39DE
        mov    di, ax
loc_1_39DE:
        test   di, di
        lea    ebx, [esi + 14h]
        je     SHORT loc_1_3A06
loc_1_39E6:
        movzx  eax, word ptr [ebp - 2]
        add    eax, 2
        imul   eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_1_2E4D
        mov    byte ptr [ebx], al
        inc    ebx
        inc    word ptr [ebp - 2]
        add    esp, 8
        dec    di
        jne    SHORT loc_1_39E6
loc_1_3A06:
        xor    esi, esi
loc_1_3A08:
        push   dword ptr [ebp + 8]
        call   sub_1_3806
        add    esp, 4
        mov    eax, esi
loc_1_3A15:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3A1C:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_1_37E7
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 000h, 000h
        add    esp, 4
        mov    esi, dword ptr [ebp + 0ch]
        mov    ax, word ptr [esi]
        mov    word ptr [ebp - 14h], ax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_3111
        add    esp, 4
        test   eax, eax
        jne    SHORT loc_1_3A7F
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_35AB
        add    esp, 4
        cmp    eax, 1fh
        jne    SHORT loc_1_3A6A
        mov    word ptr [esi + 2], 0
        jmp    SHORT loc_1_3A74
loc_1_3A6A:
        db  066h, 0A1h, 000h, 000h, 000h, 000h
        mov    word ptr [esi + 2], ax
loc_1_3A74:
        push   dword ptr [ebp + 8]
        call   sub_1_3806
        add    esp, 4
loc_1_3A7F:
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
loc_1_3A86:
        mov    ecx, dword ptr [esp + 8]
        test   byte ptr [ecx + 17h], 10h
        je     SHORT loc_1_3A97
        mov    eax, 1fh
        jmp    SHORT loc_1_3AE4
loc_1_3A97:
        db  00Fh, 0B7h, 005h, 000h, 000h, 000h, 000h
        db  08Bh, 015h, 00Ah, 000h, 000h, 000h
        cmp    eax, 7
        je     SHORT loc_1_3ABF
        cmp    eax, 0ah
        je     SHORT loc_1_3AC8
        cmp    eax, 0dh
        je     SHORT loc_1_3AD1
        cmp    eax, 2bh
        je     SHORT loc_1_3ADA
        mov    eax, 1ch
        jmp    SHORT loc_1_3AE4
loc_1_3ABF:
        push   edx
        push   ecx
        call   sub_1_384D
        jmp    SHORT loc_1_3AE1
loc_1_3AC8:
        push   edx
        push   ecx
        call   sub_1_3906
        jmp    SHORT loc_1_3AE1
loc_1_3AD1:
        push   edx
        push   ecx
        call   sub_1_3967
        jmp    SHORT loc_1_3AE1
loc_1_3ADA:
        push   edx
        push   ecx
        call   sub_1_3A1C
loc_1_3AE1:
        add    esp, 8
loc_1_3AE4:
        ret
sub_1_3AE5:
        db  08Bh, 00Dh, 000h, 000h, 000h, 000h
        test   ecx, ecx
        je     SHORT loc_1_3B05
        mov    ax, word ptr [esp + 4]
loc_1_3AF4:
        cmp    word ptr [ecx + 0eh], ax
        je     SHORT loc_1_3B01
        mov    ecx, dword ptr [ecx + 4]
        test   ecx, ecx
        jne    SHORT loc_1_3AF4
loc_1_3B01:
        test   ecx, ecx
        jne    SHORT loc_1_3B0C
loc_1_3B05:
        mov    eax, 21h
        jmp    SHORT loc_1_3B14
loc_1_3B0C:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_1_3B14:
        ret
sub_1_3B15:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_269E
        add    esp, 8
        test   eax, eax
        jne    SHORT loc_1_3B58
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 14h]
        test   ecx, ecx
        je     SHORT loc_1_3B4A
        mov    al, byte ptr [ebp + 0ch]
loc_1_3B3B:
        cmp    byte ptr [ecx + 10h], al
        je     SHORT loc_1_3B46
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    SHORT loc_1_3B3B
loc_1_3B46:
        test   ecx, ecx
        jne    SHORT loc_1_3B51
loc_1_3B4A:
        mov    eax, 21h
        jmp    SHORT loc_1_3B58
loc_1_3B51:
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_1_3B58:
        mov    esp, ebp
        pop    ebp
        ret
sub_1_3B5C:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 14h]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [edx + 0ch]
        mov    eax, dword ptr [eax + 14h]
        test   eax, eax
        je     SHORT loc_1_3B7A
loc_1_3B6F:
        cmp    dword ptr [eax + 1ah], ecx
        je     SHORT loc_1_3B7A
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    SHORT loc_1_3B6F
loc_1_3B7A:
        ret
sub_1_3B7B:
        push   ebp
        mov    eax, dword ptr [esp + 0ch]
        mov    ebp, esp
        sub    esp, 20h
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, 1
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 14h], ecx
        add    ecx, 14h
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 18h], ecx
        mov    dword ptr [ebp - 0ch], 20h
        mov    eax, dword ptr [eax]
        mov    cl, byte ptr [eax + 12h]
        mov    eax, dword ptr [eax + 0ch]
        shl    dx, cl
        mov    word ptr [ebp - 4], dx
        mov    ebx, dword ptr [eax + 14h]
        test   ebx, ebx
        je     NEAR PTR loc_1_4044
loc_1_3BC4:
        mov    ax, word ptr [ebx + 0ch]
        test   al, 6
        jne    NEAR PTR loc_1_3FC5
        test   al, 1
        je     SHORT loc_1_3BEB
        db  066h, 08Bh, 00Dh, 016h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 14h]
        mov    edx, dword ptr [eax + 10h]
        cmp    word ptr [edx + 16h], cx
        jne    NEAR PTR loc_1_3FC5
loc_1_3BEB:
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 1ah
        mov    dword ptr [ebp - 1ch], eax
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_3C0C
        mov    ax, word ptr [esi + 8]
        and    ax, 2
        cmp    ax, 1
        sbb    eax, eax
        add    eax, 2
        jmp    SHORT loc_1_3C11
loc_1_3C0C:
        mov    eax, 4
loc_1_3C11:
        lea    ecx, [ebp - 8]
        push   ecx
        push   eax
        push   ebx
        call   sub_1_2AA9
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_404C
        mov    ax, word ptr [ebp - 4]
        db  08Ah, 00Dh, 002h, 000h, 000h, 000h
        db  066h, 085h, 005h, 004h, 000h, 000h, 000h
        je     NEAR PTR loc_1_3FC5
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_3FC5
        mov    eax, dword ptr [ebp - 1ch]
        cmp    byte ptr [eax], 0
        jne    NEAR PTR loc_1_3CD9
        test   byte ptr [esi + 8], 10h
        je     SHORT loc_1_3C76
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        test   ah, 1
        je     SHORT loc_1_3C6A
        test   ah, 2
        je     SHORT loc_1_3C76
loc_1_3C6A:
        mov    dword ptr [ebp - 0ch], 2
        jmp    NEAR PTR loc_1_3FC5
loc_1_3C76:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 8]
        and    cl, 80h
        mov    al, byte ptr [eax + 1fh]
        push   eax
        cmp    cl, 1
        mov    al, byte ptr [edx + 1eh]
        mov    dl, byte ptr [esi + 22h]
        push   eax
        push   edx
        sbb    eax, eax
        inc    eax
        push   eax
        call   sub_1_270C
        add    esp, 10h
        mov    byte ptr [ebp - 1], al
        test   al, al
        jne    SHORT loc_1_3CAD
        mov    dword ptr [ebp - 0ch], 17h
        jmp    NEAR PTR loc_1_3FC5
loc_1_3CAD:
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 20h
        je     SHORT loc_1_3CBD
        mov    eax, dword ptr [esi + 0eh]
        mov    dword ptr [ebp - 10h], eax
        jmp    SHORT loc_1_3CC6
loc_1_3CBD:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 16h]
        mov    dword ptr [ebp - 10h], ecx
loc_1_3CC6:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 12h
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_3D1B
        mov    dword ptr [eax], 1000h
        jmp    SHORT loc_1_3D1B
loc_1_3CD9:
        cmp    byte ptr [esi + 22h], 80h
        je     SHORT loc_1_3CEB
        mov    dword ptr [ebp - 0ch], 17h
        jmp    NEAR PTR loc_1_3FC5
loc_1_3CEB:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 1], 0
        test   byte ptr [eax], 20h
        je     SHORT loc_1_3CFC
        mov    eax, dword ptr [esi + 0eh]
        jmp    SHORT loc_1_3D07
loc_1_3CFC:
        push   dword ptr [esi + 0eh]
        call   sub_1_27A5
        add    esp, 4
loc_1_3D07:
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 12h
        cmp    dword ptr [eax], 0
        jne    SHORT loc_1_3D1B
        mov    dword ptr [eax], 1
loc_1_3D1B:
        mov    cx, word ptr [esi + 8]
        test   cl, 20h
        je     SHORT loc_1_3D30
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 4
        je     NEAR PTR loc_1_3FC5
loc_1_3D30:
        and    cx, 8
        jne    SHORT loc_1_3D3E
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 8
        je     SHORT loc_1_3D4B
loc_1_3D3E:
        test   cx, cx
        je     SHORT loc_1_3D57
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 10h
        jne    SHORT loc_1_3D57
loc_1_3D4B:
        mov    dword ptr [ebp - 0ch], 2
        jmp    NEAR PTR loc_1_3FC5
loc_1_3D57:
        test   byte ptr [esi + 3], 40h
        mov    eax, dword ptr [ebp - 8]
        je     SHORT loc_1_3DC3
        mov    eax, dword ptr [eax + 0ah]
        cmp    dword ptr [esi + 0eh], eax
        jae    SHORT loc_1_3D6B
        mov    dword ptr [esi + 0eh], eax
loc_1_3D6B:
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 2
        je     SHORT loc_1_3DA1
        mov    edi, dword ptr [eax + 12h]
        mov    ecx, dword ptr [esi + 0eh]
        sub    edx, edx
        lea    eax, [ecx + edi - 1]
        div    edi
        imul   eax, edi
        cmp    ecx, eax
        jae    SHORT loc_1_3D8B
        mov    dword ptr [esi + 0eh], eax
loc_1_3D8B:
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 40h
        je     SHORT loc_1_3DA1
        push   dword ptr [esi + 0eh]
        call   sub_1_27A5
        add    esp, 4
        mov    dword ptr [esi + 0eh], eax
loc_1_3DA1:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0eh]
        test   eax, eax
        je     NEAR PTR loc_1_3E30
        cmp    dword ptr [esi + 0eh], eax
        jbe    SHORT loc_1_3E30
        mov    dword ptr [ebp - 0ch], 0ah
        mov    dword ptr [esi + 1ah], eax
        jmp    NEAR PTR loc_1_3FC5
loc_1_3DC3:
        test   byte ptr [eax], 2
        je     SHORT loc_1_3E13
        mov    edi, dword ptr [esi + 0eh]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + 0ah], edi
        ja     SHORT loc_1_3DFE
        mov    eax, dword ptr [ecx + 0eh]
        test   eax, eax
        je     SHORT loc_1_3DDE
        cmp    eax, edi
        jb     SHORT loc_1_3DFE
loc_1_3DDE:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, edi
        sub    edx, edx
        div    dword ptr [ecx + 12h]
        test   edx, edx
        jne    SHORT loc_1_3DFE
        test   byte ptr [ecx], 40h
        je     SHORT loc_1_3E30
        push   edi
        call   sub_1_27A5
        add    esp, 4
        cmp    eax, edi
        je     SHORT loc_1_3E30
loc_1_3DFE:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], 0ah
        mov    ecx, dword ptr [eax + 0eh]
        mov    dword ptr [esi + 1ah], ecx
        jmp    NEAR PTR loc_1_3FC5
loc_1_3E13:
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi + 0eh]
        cmp    dword ptr [edx + 0ah], ecx
        je     SHORT loc_1_3E30
        mov    dword ptr [ebp - 0ch], 0ah
        mov    ecx, dword ptr [edx + 0eh]
        mov    dword ptr [esi + 1ah], ecx
        jmp    NEAR PTR loc_1_3FC5
loc_1_3E30:
        test   byte ptr [esi + 3], 40h
        je     SHORT loc_1_3E66
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [esi + 1eh], eax
        jae    SHORT loc_1_3E41
        mov    dword ptr [esi + 1eh], eax
loc_1_3E41:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 2]
        cmp    dword ptr [esi + 12h], eax
        jae    SHORT loc_1_3E4F
        mov    dword ptr [esi + 12h], eax
loc_1_3E4F:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 6]
        cmp    dword ptr [esi + 16h], eax
        jbe    NEAR PTR loc_1_3EEA
        mov    dword ptr [esi + 16h], eax
        jmp    NEAR PTR loc_1_3EEA
loc_1_3E66:
        mov    ecx, dword ptr [esi + 0ah]
        test   ecx, ecx
        je     SHORT loc_1_3ED8
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 1
        je     SHORT loc_1_3E96
        lea    edi, [eax + 2]
        cmp    dword ptr [edi], ecx
        ja     SHORT loc_1_3E8A
        mov    eax, dword ptr [esi + 0eh]
        mov    edx, dword ptr [ebp - 8]
        add    eax, ecx
        dec    eax
        cmp    eax, dword ptr [edx + 6]
        jbe    SHORT loc_1_3EAC
loc_1_3E8A:
        mov    dword ptr [ebp - 0ch], 3
        jmp    NEAR PTR loc_1_3FC5
loc_1_3E96:
        mov    edi, dword ptr [ebp - 8]
        add    edi, 2
        cmp    dword ptr [edi], ecx
        je     SHORT loc_1_3EAC
        mov    dword ptr [ebp - 0ch], 3
        jmp    NEAR PTR loc_1_3FC5
loc_1_3EAC:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_3EC9
        mov    eax, ecx
        sub    edx, edx
        div    dword ptr [ebp - 10h]
        test   edx, edx
        je     SHORT loc_1_3EC9
        mov    dword ptr [ebp - 0ch], 3
        jmp    NEAR PTR loc_1_3FC5
loc_1_3EC9:
        mov    dword ptr [edi], ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + 0ah]
        add    eax, dword ptr [esi + 0eh]
        dec    eax
        mov    dword ptr [ecx + 6], eax
loc_1_3ED8:
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 1
        jne    SHORT loc_1_3EEA
        mov    ecx, dword ptr [eax + 2]
        add    ecx, dword ptr [esi + 0eh]
        dec    ecx
        mov    dword ptr [eax + 6], ecx
loc_1_3EEA:
        test   byte ptr [esi + 3], 10h
        je     NEAR PTR loc_1_3FE2
        db  0BAh, 000h, 000h, 000h, 000h
        xor    eax, eax
        mov    edi, edx
        mov    ecx, 9
        rep stosd
        mov    eax, dword ptr [ebp - 1ch]
        cmp    byte ptr [eax], 0
        db  066h, 0C7h, 005h, 000h, 000h, 000h, 000h, 001h, 000h
        jne    SHORT loc_1_3F5C
        mov    eax, dword ptr [ebp - 10h]
        mov    edi, dword ptr [ebp - 8]
        dec    eax
        push   edx
        not    eax
        db  066h, 0C7h, 005h, 002h, 000h, 000h, 000h, 014h, 000h
        db  0A3h, 010h, 000h, 000h, 000h
        mov    ecx, dword ptr [esi + 0eh]
        db  089h, 00Dh, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [edi + 2]
        db  0A3h, 018h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [edi + 6]
        db  089h, 00Dh, 01Ch, 000h, 000h, 000h
        mov    edx, dword ptr [eax]
        push   dword ptr [edx]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3F53:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_3FD1
        jmp    SHORT loc_1_3FB6
loc_1_3F5C:
        mov    eax, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 8]
        dec    ax
        db  068h, 000h, 000h, 000h, 000h
        not    ax
        db  066h, 0C7h, 005h, 002h, 000h, 000h, 000h, 00Ch, 000h
        db  066h, 0A3h, 00Ch, 000h, 000h, 000h
        mov    ecx, dword ptr [esi + 0eh]
        db  066h, 089h, 00Dh, 00Eh, 000h, 000h, 000h
        mov    eax, dword ptr [edx + 2]
        db  066h, 0A3h, 010h, 000h, 000h, 000h
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [edx + 6]
        db  066h, 089h, 00Dh, 012h, 000h, 000h, 000h
        mov    ecx, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 18h]
        db  0A2h, 017h, 000h, 000h, 000h
        mov    edx, dword ptr [ecx]
        push   dword ptr [edx]
        db  0E8h, 000h, 000h, 000h, 000h
loc_1_3FAF:
        add    esp, 8
        test   eax, eax
        je     SHORT loc_1_3FD8
loc_1_3FB6:
        cmp    dword ptr [esi + 0ah], 1
        sbb    eax, eax
        and    eax, 1dh
        add    eax, 3
        mov    dword ptr [ebp - 0ch], eax
loc_1_3FC5:
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    NEAR PTR loc_1_3BC4
        jmp    SHORT loc_1_4044
loc_1_3FD1:
        db  0A1h, 004h, 000h, 000h, 000h
        jmp    SHORT loc_1_3FDF
loc_1_3FD8:
        db  00Fh, 0B7h, 005h, 004h, 000h, 000h, 000h
loc_1_3FDF:
        mov    dword ptr [esi + 0ah], eax
loc_1_3FE2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [eax], ebx
        mov    eax, dword ptr [esi]
        and    eax, 40000000h
        mov    cl, byte ptr [ebp - 1]
        cmp    eax, 1
        sbb    eax, eax
        and    al, 0feh
        add    eax, 4
        or     word ptr [ebx + 0ch], ax
        mov    byte ptr [ebx + 11h], cl
        xor    ecx, ecx
        mov    edi, dword ptr [ebp - 14h]
        mov    byte ptr [ebx + 12h], cl
        add    esi, 8
        mov    dword ptr [ebx + 16h], ecx
        mov    eax, dword ptr [esi + 2]
        mov    dword ptr [ebx + 1eh], eax
        mov    edx, dword ptr [esi + 6]
        mov    dword ptr [ebx + 22h], edx
        mov    dword ptr [ebx + 1ah], edi
        mov    dx, word ptr [esi]
        and    dx, 0feffh
        mov    word ptr [esi], dx
        cmp    byte ptr [edi + 1ah], cl
        jne    SHORT loc_1_403D
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 80h
        je     SHORT loc_1_403D
        or     dh, 1
        mov    word ptr [esi], dx
loc_1_403D:
        mov    ax, word ptr [esi]
        mov    word ptr [ebx + 14h], ax
loc_1_4044:
        cmp    ebx, 1
        sbb    eax, eax
        and    eax, dword ptr [ebp - 0ch]
loc_1_404C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret
sub_1_4053:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 10h]
        test   byte ptr [edi + 0ch], 2
        mov    esi, dword ptr [edi + 1ah]
        jne    SHORT loc_1_406D
        mov    eax, 21h
        jmp    NEAR PTR loc_1_4150
loc_1_406D:
        xor    bl, bl
        cmp    byte ptr [esi + 1ah], bl
        jne    SHORT loc_1_4080
        lea    eax, [edi + 14h]
        test   byte ptr [eax], 10h
        je     SHORT loc_1_408C
        mov    bl, 8
        jmp    SHORT loc_1_408C
loc_1_4080:
        mov    bl, 1
        lea    eax, [edi + 14h]
        test   byte ptr [eax], 2
        je     SHORT loc_1_408C
        mov    bl, 9
loc_1_408C:
        mov    ax, word ptr [eax]
        test   al, 4
        je     SHORT loc_1_4096
        or     bl, 2
loc_1_4096:
        test   al, 8
        je     SHORT loc_1_409D
        or     bl, 4
loc_1_409D:
        lea    ecx, [edi + 8]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 18h]
        db  089h, 015h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 088h
        db  089h, 015h, 00Ah, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    cl, byte ptr [edi + 10h]
        db  088h, 00Dh, 003h, 000h, 000h, 000h
        call   sub_1_2438
        test   eax, eax
        jne    SHORT loc_1_4150
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 089h
        mov    eax, dword ptr [esi + 14h]
        mov    ecx, dword ptr [eax]
        mov    al, byte ptr [ecx + 12h]
        db  08Ah, 00Dh, 002h, 000h, 000h, 000h
        and    cl, 0f0h
        db  088h, 01Dh, 009h, 000h, 000h, 000h
        or     al, cl
        db  0A2h, 002h, 000h, 000h, 000h
        mov    dl, byte ptr [edi + 11h]
        db  088h, 015h, 008h, 000h, 000h, 000h
        cmp    byte ptr [esi + 1ah], 0
        jne    SHORT loc_1_412B
        test   al, 10h
        jne    SHORT loc_1_412B
        mov    eax, dword ptr [edi + 22h]
        shr    eax, 0ch
        db  066h, 0A3h, 004h, 000h, 000h, 000h
        mov    eax, dword ptr [edi + 1eh]
        shr    eax, 0ch
        db  066h, 0A3h, 00Eh, 000h, 000h, 000h
        jmp    SHORT loc_1_413C
loc_1_412B:
        mov    eax, dword ptr [edi + 22h]
        db  0A3h, 004h, 000h, 000h, 000h
        mov    ecx, dword ptr [edi + 1eh]
        db  089h, 00Dh, 00Eh, 000h, 000h, 000h
loc_1_413C:
        call   sub_1_2438
        cmp    eax, 1
        mov    ecx, eax
        mov    eax, 0
        adc    eax, -1
        and    eax, ecx
loc_1_4150:
        pop    edi
        pop    esi
        pop    ebx
        ret
sub_1_4154:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        mov    ax, word ptr [esi + 14h]
        and    ax, 2
        cmp    ax, 1
        sbb    eax, eax
        add    eax, 2
        push   eax
        push   esi
        call   sub_1_2AA9
        add    esp, 0ch
        test   eax, eax
        jne    NEAR PTR loc_1_423F
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_4196
        mov    eax, 11h
        jmp    NEAR PTR loc_1_423F
loc_1_4196:
        mov    eax, dword ptr [ebp - 4]
        lea    ecx, [esi + 16h]
        test   byte ptr [eax], 80h
        je     SHORT loc_1_41B6
        mov    eax, dword ptr [ecx]
        sub    edx, edx
        div    dword ptr [esi + 22h]
        test   edx, edx
        je     SHORT loc_1_41CB
        mov    eax, 7
        jmp    NEAR PTR loc_1_423F
loc_1_41B6:
        mov    edi, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx]
        sub    edx, edx
        div    dword ptr [edi + 1ah]
        test   edx, edx
        je     SHORT loc_1_41CB
        mov    eax, 7
        jmp    SHORT loc_1_423F
loc_1_41CB:
        xor    dx, dx
        mov    ax, word ptr [esi + 14h]
        test   al, 4
        je     SHORT loc_1_41DA
        mov    dx, 2
loc_1_41DA:
        test   al, 2
        je     SHORT loc_1_41E1
        or     dl, 1
loc_1_41E1:
        lea    edi, [esi + 8]
        mov    eax, dword ptr [edi]
        mov    eax, dword ptr [eax + 18h]
        db  0A3h, 014h, 000h, 000h, 000h
        mov    eax, dword ptr [edi]
        mov    eax, dword ptr [eax + 1ch]
        db  0C6h, 005h, 001h, 000h, 000h, 000h, 08Bh
        db  0A3h, 00Ah, 000h, 000h, 000h
        mov    edi, dword ptr [edi]
        mov    al, byte ptr [edi + 0ch]
        db  0A2h, 000h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 10h]
        db  0A2h, 003h, 000h, 000h, 000h
        mov    al, byte ptr [esi + 12h]
        db  066h, 089h, 015h, 008h, 000h, 000h, 000h
        db  0A2h, 002h, 000h, 000h, 000h
        mov    eax, dword ptr [ecx]
        shr    eax, 0ch
        db  066h, 0A3h, 00Eh, 000h, 000h, 000h
        call   sub_1_2438
        cmp    eax, 1
        mov    ecx, eax
        mov    eax, 0
        adc    eax, -1
        and    eax, ecx
loc_1_423F:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret
_LTEXT ends
        end
