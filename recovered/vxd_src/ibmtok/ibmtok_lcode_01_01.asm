; ibmtok_lcode_01_01.asm -- IBMTOK object 1 segment _LTEXT  (part 1/2)
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        db  0A0h, 0C8h, 007h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_0025
        db  0FEh, 005h, 0C8h, 007h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_0027
        push   0
        push   0
        db  0E8h, 0E0h, 0FFh, 0FEh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_0027
loc_1_0025:
        clc
        ret
loc_1_0027:
        stc
        ret
IBMTOK_Control:
        sub    eax, 1
        cmp    eax, 1bh
        jae    SHORT loc_1_0038
        db  0FFh, 024h, 085h, 03Ah, 000h, 000h, 000h
loc_1_0038:
        clc
        ret
        db  000h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h
        db  038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h
        db  038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h
        db  038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h
        db  038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h
        db  038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h
        db  038h, 000h, 000h, 000h, 038h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_00B0:
        push   ebp
        mov    ebp, esp
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
        call   sub_1_00E0
        pop    ebp
        ret    1ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_00E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 10h]
        add    ecx, 2
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 8
        cmp    dword ptr [ebp + 14h], 0eh
        jb     NEAR PTR loc_1_0200
        mov    al, byte ptr [eax]
        shr    al, 7
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ecx]
        and    al, 80h
        mov    byte ptr [ebp - 2], al
        or     al, al
        je     NEAR PTR loc_1_01A4
        mov    al, byte ptr [ecx]
        cmp    al, 0ffh
        jne    SHORT loc_1_0122
        cmp    byte ptr [ecx + 1], 0ffh
        je     SHORT loc_1_012C
loc_1_0122:
        cmp    al, 0c0h
        jne    SHORT loc_1_0148
        cmp    byte ptr [ecx + 1], 0
        jne    SHORT loc_1_0148
loc_1_012C:
        cmp    byte ptr [ecx + 2], 0ffh
        jne    SHORT loc_1_0148
        cmp    byte ptr [ecx + 3], 0ffh
        jne    SHORT loc_1_0148
        cmp    byte ptr [ecx + 4], 0ffh
        jne    SHORT loc_1_0148
        cmp    byte ptr [ecx + 5], 0ffh
        mov    byte ptr [ebp - 2], 1
        je     SHORT loc_1_014C
loc_1_0148:
        mov    byte ptr [ebp - 2], 0
loc_1_014C:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_1_0160
        mov    dword ptr [ebp - 8], 8
        jmp    NEAR PTR loc_1_020B
        db  08Bh, 0FFh
loc_1_0160:
        mov    al, byte ptr [ecx + 2]
        and    al, byte ptr [ecx]
        and    al, 80h
        mov    byte ptr [ebp - 2], al
        or     al, al
        mov    dword ptr [ebp - 8], 1000h
        jne    SHORT loc_1_017C
        mov    dword ptr [ebp - 8], 4000h
loc_1_017C:
        xor    edx, edx
        mov    dl, byte ptr [ecx + 4]
        xor    eax, eax
        mov    al, byte ptr [ecx + 2]
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        xor    eax, eax
        mov    al, byte ptr [ecx + 3]
        shl    eax, 10h
        or     edx, eax
        xor    eax, eax
        mov    al, byte ptr [ecx + 5]
        or     edx, eax
        mov    dword ptr [ebp - 0ch], edx
        jmp    SHORT loc_1_020B
loc_1_01A4:
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 10h], 30h
        je     SHORT loc_1_01F0
        lea    edx, [esi + 18h]
        mov    eax, dword ptr [ecx + 2]
        cmp    dword ptr [edx + 2], eax
        jne    SHORT loc_1_01C7
        mov    ax, word ptr [ecx]
        cmp    word ptr [edx], ax
        mov    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_01CE
loc_1_01C7:
        mov    dword ptr [ebp - 10h], 1
loc_1_01CE:
        cmp    dword ptr [ebp - 10h], 0
        jne    SHORT loc_1_01E0
        mov    dword ptr [ebp - 8], 1
        jmp    SHORT loc_1_020E
        db  08Dh, 049h, 000h
loc_1_01E0:
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_1_020E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_01F0:
        mov    dword ptr [ebp - 8], 1
        jmp    SHORT loc_1_020E
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0200:
        mov    dword ptr [ebp - 8], 20h
        mov    byte ptr [ebp - 1], 0
loc_1_020B:
        mov    esi, dword ptr [ebp + 8]
loc_1_020E:
        mov    edi, dword ptr [esi + 14h]
        or     edi, edi
        je     NEAR PTR loc_1_02F1
loc_1_0219:
        mov    ecx, dword ptr [edi + 8]
        mov    eax, ecx
        and    eax, dword ptr [ebp - 8]
        test   al, 9
        jne    SHORT loc_1_025D
        test   ah, 40h
        je     SHORT loc_1_0232
        mov    eax, dword ptr [edi + 0ch]
        test   dword ptr [ebp - 0ch], eax
        jne    SHORT loc_1_025D
loc_1_0232:
        test   byte ptr [ebp - 7], 40h
        je     SHORT loc_1_023D
        test   ch, 20h
        jne    SHORT loc_1_025D
loc_1_023D:
        test   byte ptr [ebp - 7], 10h
        je     SHORT loc_1_0249
        cmp    byte ptr [edi + 1dh], 0
        jne    SHORT loc_1_025D
loc_1_0249:
        test   cl, 10h
        je     SHORT loc_1_0254
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_025D
loc_1_0254:
        test   cl, 20h
        je     NEAR PTR loc_1_02E6
loc_1_025D:
        inc    dword ptr [edi + 18h]
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
        mov    eax, dword ptr [edi + 4]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_0592
        mov    byte ptr [edi + 1eh], 1
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_02E6
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_029F
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_029F:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_02B0
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_02B6
        db  08Bh, 0FFh
loc_1_02B0:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 14h], eax
loc_1_02B6:
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_1_02C5
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0598
loc_1_02C5:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 2ah]
        mov    eax, 1
        mov    cl, byte ptr [edi + 1ch]
        shl    eax, cl
        or     dword ptr [esi + 2eh], eax
        push   0
        push   1fh
        push   edi
        call   sub_1_063C
        mov    edi, ebx
        jmp    SHORT loc_1_02E9
loc_1_02E6:
        mov    edi, dword ptr [edi + 10h]
loc_1_02E9:
        or     edi, edi
        jne    NEAR PTR loc_1_0219
loc_1_02F1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    1ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0300:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0310
        pop    ebp
        ret    4
sub_1_0310:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 14h]
        or     edi, edi
        je     SHORT loc_1_038E
loc_1_0320:
        inc    dword ptr [edi + 18h]
        cmp    byte ptr [edi + 1eh], 0
        je     SHORT loc_1_0336
        mov    byte ptr [edi + 1eh], 0
        mov    eax, dword ptr [edi + 4]
        push   eax
        call   sub_1_0598
loc_1_0336:
        mov    eax, dword ptr [edi + 18h]
        dec    eax
        mov    dword ptr [edi + 18h], eax
        jne    SHORT loc_1_0387
        mov    ebx, dword ptr [edi + 10h]
        or     ebx, ebx
        je     SHORT loc_1_034C
        mov    eax, dword ptr [edi + 14h]
        mov    dword ptr [ebx + 14h], eax
loc_1_034C:
        mov    eax, dword ptr [edi + 14h]
        or     eax, eax
        je     SHORT loc_1_0360
        mov    ecx, dword ptr [edi + 10h]
        mov    dword ptr [eax + 10h], ecx
        jmp    SHORT loc_1_0366
        db  005h, 000h, 000h, 000h, 000h
loc_1_0360:
        mov    eax, dword ptr [edi + 10h]
        mov    dword ptr [esi + 14h], eax
loc_1_0366:
        mov    eax, dword ptr [edi]
        push   eax
        call   dword ptr [esi + 2ah]
        mov    eax, 1
        mov    cl, byte ptr [edi + 1ch]
        shl    eax, cl
        or     dword ptr [esi + 2eh], eax
        push   0
        push   1fh
        push   edi
        call   sub_1_063C
        mov    edi, ebx
        jmp    SHORT loc_1_038A
loc_1_0387:
        mov    edi, dword ptr [edi + 10h]
loc_1_038A:
        or     edi, edi
        jne    SHORT loc_1_0320
loc_1_038E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    edx, dword ptr [ebp + 10h]
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 4]
        xor    eax, eax
        mov    al, byte ptr [edx + 2]
        shl    eax, 10h
        or     ecx, eax
        shl    ecx, 8
        xor    eax, eax
        mov    al, byte ptr [edx + 3]
        shl    eax, 10h
        or     ecx, eax
        xor    eax, eax
        mov    al, byte ptr [edx + 5]
        or     ecx, eax
        mov    al, byte ptr [edx + 2]
        and    al, byte ptr [edx]
        and    al, 80h
        mov    byte ptr [ebp - 1], al
        or     al, al
        mov    eax, dword ptr [ebp + 8]
        je     SHORT loc_1_03F2
        cmp    dword ptr [eax + 8], ecx
        jne    SHORT loc_1_03F2
        cmp    dword ptr [eax + 0ch], 0
        je     SHORT loc_1_03F2
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_03F2:
        mov    esi, dword ptr [eax + 10h]
        or     esi, esi
        jne    SHORT loc_1_0402
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0402:
        test   esi, 20h
        je     SHORT loc_1_0413
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_0413:
        mov    al, byte ptr [edx]
        shr    al, 7
        or     al, al
        je     SHORT loc_1_0430
        test   esi, 10h
        je     SHORT loc_1_0430
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 049h, 000h
loc_1_0430:
        mov    eax, dword ptr [ebp + 0ch]
        mov    cl, byte ptr [eax]
        and    cl, 80h
        mov    byte ptr [ebp - 1], cl
        or     cl, cl
        je     SHORT loc_1_04BA
        mov    cl, byte ptr [eax]
        cmp    cl, 0ffh
        jne    SHORT loc_1_044C
        cmp    byte ptr [eax + 1], 0ffh
        je     SHORT loc_1_0457
loc_1_044C:
        cmp    cl, 0c0h
        jne    SHORT loc_1_0473
        cmp    byte ptr [eax + 1], 0
        jne    SHORT loc_1_0473
loc_1_0457:
        cmp    byte ptr [eax + 2], 0ffh
        jne    SHORT loc_1_0473
        cmp    byte ptr [eax + 3], 0ffh
        jne    SHORT loc_1_0473
        cmp    byte ptr [eax + 4], 0ffh
        jne    SHORT loc_1_0473
        cmp    byte ptr [eax + 5], 0ffh
        mov    byte ptr [ebp - 1], 1
        je     SHORT loc_1_0477
loc_1_0473:
        mov    byte ptr [ebp - 1], 0
loc_1_0477:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_04A0
        test   esi, 8
        je     SHORT loc_1_0490
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_1_0490:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_04A0:
        test   esi, 6000h
        je     SHORT loc_1_04B1
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_04B1:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_04BA:
        sub    al, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 056h, 08Bh, 04Ch
        db  024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Eh, 05Eh, 0C2h, 00Ch
        db  000h, 056h, 08Bh, 04Ch, 024h, 010h, 08Bh, 074h, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h
        db  066h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh
        db  04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh, 05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh
        db  07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh
        db  04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h, 06Dh, 05Fh
        db  0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h
        db  0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
sub_1_054A:
        db  0CDh, 020h, 031h, 080h, 028h, 000h
sub_1_0550:
        db  0CDh, 020h, 032h, 080h, 028h, 000h
sub_1_0556:
        db  0CDh, 020h, 033h, 080h, 028h, 000h
sub_1_055C:
        db  0CDh, 020h, 034h, 080h, 028h, 000h
sub_1_0562:
        db  0CDh, 020h, 035h, 080h, 028h, 000h
sub_1_0568:
        db  0CDh, 020h, 036h, 080h, 028h, 000h
sub_1_056E:
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_0574:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_057A:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_0586:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
sub_1_058C:
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_0592:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_0598:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_059E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_05A4:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
sub_1_05AA:
        db  0CDh, 020h, 041h, 080h, 028h, 000h
sub_1_05B0:
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_05BC:
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
sub_1_0604:
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_0612:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_0618:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_061E:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_0624:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
sub_1_062A:
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_0630:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_0636:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_063C:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_0660:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_0684:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_06A8:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_06B4:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_06BA:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_06C0:
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
sub_1_0702:
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_0708:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
sub_1_070E:
        db  0CDh, 020h, 043h, 080h, 028h, 000h
sub_1_0714:
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_071C:
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
        db  056h, 057h, 09Ch, 08Bh, 04Ch, 024h, 018h, 08Bh, 074h, 024h, 014h, 08Bh, 07Ch, 024h, 010h, 03Bh
        db  0F7h, 073h, 00Dh, 00Bh, 0C9h, 074h, 01Fh, 0FDh, 049h, 003h, 0F1h, 003h, 0F9h, 041h, 0EBh, 014h
        db  08Bh, 0C6h, 02Bh, 0C7h, 083h, 0F8h, 004h, 072h, 00Bh, 08Ah, 0C1h, 024h, 003h, 0C1h, 0E9h, 002h
        db  0F3h, 0A5h, 08Ah, 0C8h, 0F3h, 0A4h, 09Dh, 05Fh, 05Eh, 0C2h, 00Ch, 000h
public VXD_DDB
VXD_DDB label byte
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 049h, 042h, 04Dh, 054h
        db  04Fh, 04Bh, 020h, 020h, 000h, 040h, 000h, 054h, 029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_07D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 2ch
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0A00
        push   esi
        call   sub_1_0A50
        lea    edi, [ebp - 2ch]
        sub    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        cmp    byte ptr [ebp + 14h], 1
        sbb    eax, eax
        and    eax, 0fffffffeh
        add    eax, 3
        mov    dword ptr [ebp - 22h], eax
        mov    dword ptr [ebp - 16h], 1
        mov    eax, dword ptr [esi + 59h]
        mov    dword ptr [ebp - 12h], eax
        lea    edi, [esi + 81h]
        mov    dword ptr [ebp - 0eh], 4
        lea    eax, [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   esi
        mov    eax, dword ptr [esi + 7dh]
        push   eax
        push   edi
        call   sub_1_0562
        mov    dword ptr [ebp - 4], eax
        or     eax, eax
        je     SHORT loc_1_0844
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_0844:
        cmp    byte ptr [ebp + 18h], 0
        je     SHORT loc_1_0871
        push   0
        push   0aa00000ah
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0708
        add    esp, 0ch
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0568
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_0871:
        push   esi
        call   sub_1_2480
        or     al, al
        jne    SHORT loc_1_0890
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0568
        mov    eax, 0c0010006h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_0890:
        cmp    byte ptr [esi + 3ah], 0
        je     SHORT loc_1_08A3
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e18h], 0c0h
loc_1_08A3:
        mov    edx, dword ptr [esi + 59h]
        inc    dx
        sub    al, al
        out    dx, al
        push   0c350h
        call   sub_1_0630
        mov    ecx, dword ptr [esi + 59h]
        add    cx, 2
        mov    dx, cx
        sub    al, al
        out    dx, al
        cmp    byte ptr [esi + 38h], 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        push   eax
        push   0
        mov    eax, dword ptr [esi + 33h]
        push   eax
        push   eax
        db  068h, 040h, 035h, 000h, 000h
        push   esi
        db  068h, 060h, 033h, 000h, 000h
        mov    eax, dword ptr [edi]
        push   eax
        lea    eax, [ebp - 4]
        push   esi
        push   eax
        call   sub_1_061E
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_09CC
        push   esi
        call   sub_1_0AE0
        or     eax, eax
        je     SHORT loc_1_0931
        push   1
        push   0
        push   2
        push   0aa000004h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0708
        add    esp, 14h
        push   esi
        call   sub_1_0624
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0568
        mov    eax, 0c0010006h
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_0931:
        lea    eax, [esi + 85h]
        push   eax
        lea    eax, [esi + 79h]
        push   eax
        lea    eax, [esi + 4dh]
        push   eax
        db  068h, 0A0h, 01Ch, 000h, 000h
        db  068h, 030h, 01Bh, 000h, 000h
        db  068h, 030h, 01Ch, 000h, 000h
        db  068h, 0C0h, 01Bh, 000h, 000h
        db  0E8h, 0A7h, 0F6h, 0FEh, 0FFh
        or     al, al
        jne    SHORT loc_1_0990
        push   2
        push   0
        push   2
        push   0aa000002h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0708
        add    esp, 14h
        push   esi
        call   sub_1_0624
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0568
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
loc_1_0990:
        lea    eax, [esi + 20h]
        db  0C7h, 046h, 01Ch, 0B0h, 053h, 000h, 000h
        mov    dword ptr [eax], 0
        mov    ecx, dword ptr [esi + 1ch]
        mov    dword ptr [esi + 24h], ecx
        mov    dword ptr [esi + 28h], esi
        push   7530h
        push   eax
        call   sub_1_0612
        db  068h, 0F0h, 023h, 000h, 000h
        push   esi
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_070E
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
loc_1_09CC:
        push   3
        push   0
        push   2
        push   0aa000005h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0708
        add    esp, 14h
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_0568
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A00:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        lea    eax, [ecx + 61h]
        mov    dword ptr [ecx + 65h], eax
        mov    dword ptr [eax], eax
        lea    eax, [ecx + 69h]
        mov    dword ptr [ecx + 6dh], eax
        mov    dword ptr [eax], eax
        lea    eax, [ecx + 71h]
        mov    dword ptr [ecx + 75h], eax
        mov    dword ptr [eax], eax
        mov    byte ptr [ecx + 16fh], 1
        mov    byte ptr [ecx + 171h], 1
        mov    word ptr [ecx + 0e5h], 100h
        mov    word ptr [ecx + 0dbh], 1
        mov    byte ptr [ecx + 16eh], 1
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0A50:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edx, dword ptr [ebp + 8]
        sub    eax, eax
        mov    dword ptr [edx + 0e7h], 0
        mov    dword ptr [edx + 0ebh], 0
        mov    dword ptr [edx + 0efh], 0
        mov    dword ptr [edx + 0f3h], 0
        lea    edi, [edx + 179h]
        mov    dword ptr [edx + 0ceh], 0
        mov    ecx, 80h
        rep stosd
        mov    dword ptr [edx + 89h], eax
        mov    dword ptr [edx + 8dh], eax
        mov    byte ptr [edx + 0cbh], 1
        mov    byte ptr [edx + 0cch], 1
        mov    byte ptr [edx + 48h], al
        mov    byte ptr [edx + 4ah], al
        mov    byte ptr [edx + 0cdh], al
        mov    word ptr [edx + 0d2h], 0ffffh
        mov    word ptr [edx + 0d4h], 0ffffh
        mov    byte ptr [edx + 0dah], 1
        pop    edi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_0AE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    edi, 32h
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_1E90
        cmp    byte ptr [esi + 2eh], 0
        jne    SHORT loc_1_0B30
loc_1_0B00:
        or     edi, edi
        je     SHORT loc_1_0B15
        push   186a0h
        call   sub_1_0630
        dec    edi
        cmp    byte ptr [esi + 2eh], 0
        je     SHORT loc_1_0B00
loc_1_0B15:
        cmp    byte ptr [esi + 2eh], 0
        jne    SHORT loc_1_0B30
        mov    eax, 0c0010006h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0B30:
        mov    eax, dword ptr [esi + 0adh]
        mov    al, byte ptr [eax + 1e02h]
        mov    byte ptr [ebp - 3], al
        mov    eax, dword ptr [esi + 0adh]
        mov    cl, byte ptr [ebp - 3]
        and    ecx, 0ffff00ffh
        mov    al, byte ptr [eax + 1e03h]
        mov    byte ptr [ebp - 2], al
        shl    cx, 8
        and    ax, 0ffh
        add    ax, cx
        mov    ecx, eax
        and    ecx, 0ffffh
        add    ecx, dword ptr [esi + 0b1h]
        mov    word ptr [esi + 0b5h], ax
        mov    ax, word ptr [ecx + 6]
        mov    word ptr [ebp - 6], ax
        or     ax, ax
        je     SHORT loc_1_0BE1
        cmp    ax, 30h
        jne    SHORT loc_1_0BB2
        push   7
        push   4
        push   32h
        push   0aa00000ah
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 14h
        mov    eax, 0c0010006h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0BB2:
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        push   eax
        push   7
        push   4
        push   3
        push   0aa000003h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 18h
        mov    eax, 0c0010006h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0BE1:
        mov    byte ptr [esi + 16eh], 0
        mov    byte ptr [esi + 2eh], 1
        mov    al, byte ptr [ecx + 1]
        mov    byte ptr [ebp - 6], al
        lea    edx, [esi + 37h]
        test   byte ptr [ebp - 6], 1
        je     SHORT loc_1_0C00
        mov    byte ptr [edx], 1
        jmp    SHORT loc_1_0C03
loc_1_0C00:
        mov    byte ptr [edx], 0
loc_1_0C03:
        mov    edi, dword ptr [esi + 3fh]
        cmp    edi, 2000h
        jbe    NEAR PTR loc_1_0D60
        mov    al, byte ptr [esi + 0dbh]
        mov    byte ptr [ebp - 1], al
        cmp    edi, 10000h
        jne    SHORT loc_1_0C30
        sub    edi, 2830h
        jmp    SHORT loc_1_0C36
        db  005h, 000h, 000h, 000h, 000h
loc_1_0C30:
        sub    edi, 630h
loc_1_0C36:
        cmp    byte ptr [edx], 0
        je     SHORT loc_1_0C41
        mov    ax, word ptr [esi + 45h]
        jmp    SHORT loc_1_0C4B
loc_1_0C41:
        lea    eax, [esi + 0ddh]
        mov    ax, word ptr [esi + 43h]
loc_1_0C4B:
        mov    word ptr [esi + 0ddh], ax
        mov    ax, word ptr [esi + 0ddh]
        mov    edx, eax
        and    edx, 0ffffh
        cmp    edx, edi
        jbe    SHORT loc_1_0C78
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0e5h]
        mov    eax, edi
        sub    edx, edx
        div    ebx
        shr    eax, 1
        jmp    SHORT loc_1_0C8E
loc_1_0C78:
        lea    edx, [esi + 0e3h]
        lea    edx, [esi + 0e5h]
        sub    dx, dx
        div    word ptr [esi + 0e5h]
loc_1_0C8E:
        inc    ax
        mov    word ptr [esi + 0e3h], ax
        xor    eax, eax
        xor    edx, edx
        mov    dx, word ptr [esi + 0e5h]
        mov    ax, word ptr [esi + 0e3h]
        imul   eax, edx
        sub    edi, eax
        xor    ebx, ebx
        mov    bl, byte ptr [ebp - 1]
        mov    eax, edi
        sub    edx, edx
        div    ebx
        xor    edx, edx
        mov    dx, word ptr [esi + 0ddh]
        cmp    edx, eax
        jbe    SHORT loc_1_0D09
        cmp    eax, 1000h
        jae    SHORT loc_1_0CD8
        mov    word ptr [esi + 0ddh], 800h
        jmp    SHORT loc_1_0D09
loc_1_0CD8:
        cmp    eax, 2000h
        jae    SHORT loc_1_0CF0
        mov    word ptr [esi + 0ddh], 1000h
        jmp    SHORT loc_1_0D09
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0CF0:
        cmp    eax, 4000h
        mov    word ptr [esi + 0ddh], 2000h
        jb     SHORT loc_1_0D09
        mov    word ptr [esi + 0ddh], 4000h
loc_1_0D09:
        mov    eax, dword ptr [esi + 0dfh]
        xor    edx, edx
        mov    dx, word ptr [esi + 0ddh]
        cmp    edx, eax
        jbe    SHORT loc_1_0D23
        mov    word ptr [esi + 0ddh], ax
loc_1_0D23:
        xor    eax, eax
        mov    ax, word ptr [esi + 0ddh]
        sub    eax, 6
        mov    dword ptr [esi + 0dfh], eax
        xor    eax, eax
        mov    ax, word ptr [esi + 0ddh]
        imul   eax, ebx
        sub    edi, eax
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0e5h]
        mov    eax, edi
        sub    edx, edx
        div    ebx
        add    word ptr [esi + 0e3h], ax
        jmp    SHORT loc_1_0DA7
        db  005h, 000h, 000h, 000h, 000h
loc_1_0D60:
        mov    word ptr [esi + 0ddh], 800h
        mov    word ptr [esi + 0dbh], 1
        mov    eax, dword ptr [esi + 0dfh]
        xor    edx, edx
        mov    dx, word ptr [esi + 0ddh]
        cmp    edx, eax
        jbe    SHORT loc_1_0D8C
        mov    word ptr [esi + 0ddh], ax
loc_1_0D8C:
        xor    eax, eax
        mov    ax, word ptr [esi + 0ddh]
        sub    eax, 6
        mov    dword ptr [esi + 0dfh], eax
        mov    word ptr [esi + 0e3h], 0fh
loc_1_0DA7:
        mov    ax, word ptr [ecx + 8]
        mov    word ptr [ebp - 6], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 5]
        push   6
        mov    edi, eax
        and    edi, 0ffffh
        add    edi, dword ptr [esi + 0b1h]
        push   edi
        lea    eax, [esi + 53h]
        push   eax
        call   sub_1_071C
        lea    eax, [esi + 4dh]
        cmp    byte ptr [eax], 0
        jne    SHORT loc_1_0DFD
        cmp    byte ptr [esi + 4eh], 0
        jne    SHORT loc_1_0DFD
        cmp    byte ptr [esi + 4fh], 0
        jne    SHORT loc_1_0DFD
        cmp    byte ptr [esi + 50h], 0
        jne    SHORT loc_1_0DFD
        cmp    byte ptr [esi + 51h], 0
        jne    SHORT loc_1_0DFD
        cmp    byte ptr [esi + 52h], 0
        jne    SHORT loc_1_0DFD
        push   6
        push   edi
        push   eax
        call   sub_1_071C
loc_1_0DFD:
        mov    eax, dword ptr [esi + 0adh]
        pop    edi
        mov    byte ptr [eax + 1e0fh], 0ffh
        sub    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0E20:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        sub    edx, edx
        mov    esi, dword ptr [ecx + 61h]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, -61h
        je     SHORT loc_1_0E49
loc_1_0E35:
        mov    eax, dword ptr [esi + 18h]
        cmp    eax, edx
        jbe    SHORT loc_1_0E3E
        mov    edx, eax
loc_1_0E3E:
        mov    esi, dword ptr [esi]
        mov    eax, ecx
        sub    eax, esi
        cmp    eax, -61h
        jne    SHORT loc_1_0E35
loc_1_0E49:
        or     edx, edx
        jne    SHORT loc_1_0E52
        mov    edx, 0f0h
loc_1_0E52:
        mov    dword ptr [ecx + 164h], edx
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_0E60:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        inc    dword ptr [edi + 5dh]
        cmp    byte ptr [esi + 1dh], 0
        jne    NEAR PTR loc_1_1060
        inc    dword ptr [esi + 14h]
        push   0
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 85h]
        push   eax
        db  0E8h, 03Fh, 0F3h, 0FEh, 0FFh
        mov    ebx, eax
        or     ebx, ebx
        jne    NEAR PTR loc_1_0F27
        cmp    dword ptr [esi + 14h], 2
        jne    SHORT loc_1_0EF3
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [edi + 61h]
        sub    eax, edi
        cmp    eax, 61h
        jne    SHORT loc_1_0ED0
        mov    dword ptr [edi + 18h], 0feh
loc_1_0ED0:
        mov    eax, dword ptr [edi + 164h]
        cmp    dword ptr [esi + 18h], eax
        jne    SHORT loc_1_0EE1
        push   edi
        call   sub_1_0E20
loc_1_0EE1:
        push   0
        push   96h
        push   esi
        call   sub_1_063C
        jmp    NEAR PTR loc_1_1065
loc_1_0EF3:
        mov    byte ptr [esi + 1dh], 1
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [edi + 69h]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 6dh]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 6dh]
        mov    ebx, 103h
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 6dh], esi
        sub    dword ptr [esi + 14h], 2
        jmp    NEAR PTR loc_1_1065
loc_1_0F27:
        cmp    ebx, 103h
        jne    NEAR PTR loc_1_0FD0
        cmp    dword ptr [edi + 379h], 1
        jne    SHORT loc_1_0F72
        sub    eax, eax
        sub    edx, edx
loc_1_0F40:
        mov    ecx, dword ptr [edi + 0adh]
        mov    cl, byte ptr [edx + ecx + 1f40h]
        and    ecx, 0fh
        shl    eax, 4
        add    eax, ecx
        add    edx, 2
        cmp    edx, 10h
        jb     SHORT loc_1_0F40
        cmp    eax, 5049434fh
        mov    byte ptr [edi + 175h], 1
        jne    SHORT loc_1_0F72
        mov    byte ptr [edi + 175h], 0
loc_1_0F72:
        cmp    byte ptr [edi + 175h], 0
        je     SHORT loc_1_0F85
        push   0
        push   edi
        call   sub_1_5220
        jmp    SHORT loc_1_0F9B
loc_1_0F85:
        cmp    byte ptr [edi + 0cbh], 0
        je     SHORT loc_1_0F9B
        mov    byte ptr [edi + 0cbh], 0
        push   edi
        call   sub_1_48C0
loc_1_0F9B:
        mov    byte ptr [esi + 1dh], 1
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [edi + 69h]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 6dh]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 6dh]
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 6dh], esi
        sub    dword ptr [esi + 14h], 2
        jmp    NEAR PTR loc_1_1065
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0FD0:
        cmp    ebx, 0c001000eh
        jne    SHORT loc_1_1008
        mov    byte ptr [esi + 1dh], 1
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ebx, 103h
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [edi + 69h]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 6dh]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 6dh]
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 6dh], esi
        dec    dword ptr [esi + 14h]
        jmp    SHORT loc_1_1065
loc_1_1008:
        cmp    ebx, 0c001000dh
        jne    SHORT loc_1_1040
        mov    byte ptr [esi + 1dh], 1
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        lea    eax, [edi + 71h]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 75h]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 75h]
        mov    ebx, 103h
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 75h], esi
        dec    dword ptr [esi + 14h]
        jmp    SHORT loc_1_1065
loc_1_1040:
        push   1
        push   0ah
        push   2
        push   0aa000006h
        mov    eax, dword ptr [edi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 14h
        jmp    SHORT loc_1_1065
        db  08Dh, 064h, 024h, 000h
loc_1_1060:
        mov    ebx, 0c0010002h
loc_1_1065:
        mov    eax, dword ptr [edi + 5dh]
        dec    eax
        mov    dword ptr [edi + 5dh], eax
        jne    SHORT loc_1_1087
        cmp    byte ptr [edi + 15fh], 0
        jne    SHORT loc_1_1081
        mov    eax, dword ptr [edi + 69h]
        sub    eax, edi
        cmp    eax, 69h
        je     SHORT loc_1_1087
loc_1_1081:
        push   edi
        call   sub_1_5080
loc_1_1087:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_10A0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        sub    esi, esi
        mov    edi, dword ptr [ecx + 8]
        mov    edx, dword ptr [ebp + 0ch]
        inc    dword ptr [edi + 5dh]
        mov    eax, dword ptr [edx + 10h]
        cmp    eax, esi
        jne    SHORT loc_1_10C9
        push   edx
        push   ecx
        push   edi
        db  0E8h, 0CEh, 004h, 000h, 000h
        mov    esi, eax
        jmp    NEAR PTR loc_1_1148
loc_1_10C9:
        cmp    eax, 1
        jne    SHORT loc_1_1143
        cmp    byte ptr [edi + 174h], 0
        je     SHORT loc_1_10E0
        mov    esi, 0c0010008h
        jmp    SHORT loc_1_1148
        db  08Bh, 0FFh
loc_1_10E0:
        cmp    byte ptr [edi + 171h], 0
        jne    SHORT loc_1_1107
        cmp    byte ptr [ecx + 1dh], 0
        jne    SHORT loc_1_1100
        push   edx
        push   ecx
        push   edi
        call   sub_1_15E0
        mov    esi, eax
        jmp    SHORT loc_1_1148
        db  005h, 000h, 000h, 000h, 000h
loc_1_1100:
        mov    esi, 0c0010002h
        jmp    SHORT loc_1_1148
loc_1_1107:
        cmp    byte ptr [edi + 15fh], 0
        je     SHORT loc_1_1117
        mov    esi, 0c001000dh
        jmp    SHORT loc_1_1148
loc_1_1117:
        cmp    byte ptr [edi + 16fh], 0
        je     SHORT loc_1_1127
        mov    esi, 0c0000001h
        jmp    SHORT loc_1_1148
loc_1_1127:
        push   3
        push   0bh
        push   2
        push   0aa000006h
        mov    eax, dword ptr [edi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 14h
        jmp    SHORT loc_1_1148
loc_1_1143:
        mov    esi, 10001h
loc_1_1148:
        mov    eax, dword ptr [edi + 5dh]
        dec    eax
        mov    dword ptr [edi + 5dh], eax
        jne    SHORT loc_1_116A
        cmp    byte ptr [edi + 15fh], 0
        jne    SHORT loc_1_1164
        mov    eax, dword ptr [edi + 69h]
        sub    eax, edi
        cmp    eax, 69h
        je     SHORT loc_1_116A
loc_1_1164:
        push   edi
        call   sub_1_5080
loc_1_116A:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1180:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 1
        sub    edi, edi
        lea    ecx, [ebp - 8]
        mov    esi, 4
        mov    dword ptr [ebp - 0ch], edi
        mov    edx, dword ptr [ebp + 10h]
        mov    eax, edx
        cmp    eax, 2010101h
        ja     SHORT loc_1_11C2
        je     NEAR PTR loc_1_1430
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_11E3
        shl    eax, 2
        db  0FFh, 0A0h, 040h, 015h, 000h, 000h
loc_1_11C2:
        cmp    eax, 2010102h
        je     NEAR PTR loc_1_1467
        cmp    eax, 2010103h
        je     NEAR PTR loc_1_14A0
        cmp    eax, 2010104h
        je     NEAR PTR loc_1_14D0
loc_1_11E3:
        mov    edi, 0c00000bbh
        jmp    NEAR PTR loc_1_1504
        db  08Dh, 049h, 000h
loc_1_11F0:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_1205
        db  0B9h, 004h, 055h, 000h, 000h
        mov    esi, 5ch
        jmp    NEAR PTR loc_1_1504
loc_1_1205:
        db  0B9h, 080h, 054h, 000h, 000h
        mov    esi, 84h
        jmp    NEAR PTR loc_1_1504
loc_1_1214:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 15fh], 0
        je     SHORT loc_1_1230
        mov    dword ptr [ebp - 0ch], 2
        jmp    SHORT loc_1_1267
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1230:
        cmp    byte ptr [ebx + 16eh], 0
        jne    SHORT loc_1_1260
        cmp    byte ptr [ebx + 170h], 0
        jne    SHORT loc_1_1260
        cmp    byte ptr [ebx + 171h], 0
        je     SHORT loc_1_1254
        mov    dword ptr [ebp - 0ch], 4
        jmp    SHORT loc_1_1267
loc_1_1254:
        mov    dword ptr [ebp - 0ch], 0
        jmp    SHORT loc_1_1267
        db  08Dh, 049h, 000h
loc_1_1260:
        mov    dword ptr [ebp - 0ch], 1
loc_1_1267:
        lea    ecx, [ebp - 0ch]
        mov    esi, 4
        jmp    NEAR PTR loc_1_1504
loc_1_1274:
        lea    ecx, [ebp - 10h]
        mov    esi, 4
        jmp    NEAR PTR loc_1_1504
loc_1_1281:
        mov    dword ptr [ebp - 8], 0f0h
        jmp    NEAR PTR loc_1_1504
        db  08Dh, 049h, 000h
loc_1_1290:
        mov    ebx, dword ptr [ebp + 8]
        cmp    edx, 10106h
        mov    eax, dword ptr [ebx + 0dfh]
        mov    dword ptr [ebp - 8], eax
        jne    NEAR PTR loc_1_1504
        sub    dword ptr [ebp - 8], 0eh
        jmp    NEAR PTR loc_1_1504
loc_1_12B1:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 37h], 1
        sbb    eax, eax
        and    eax, 0fffe2b40h
        add    eax, 27100h
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1504
        db  08Dh, 064h, 024h, 000h
loc_1_12D0:
        mov    ebx, dword ptr [ebp + 8]
        xor    edx, edx
        xor    eax, eax
        mov    ax, word ptr [ebx + 0ddh]
        mov    dx, word ptr [ebx + 0dbh]
        imul   edx, eax
        jmp    NEAR PTR loc_1_1501
        db  08Dh, 049h, 000h
loc_1_12F0:
        mov    ebx, dword ptr [ebp + 8]
        xor    edx, edx
        xor    eax, eax
        mov    ax, word ptr [ebx + 0e5h]
        mov    dx, word ptr [ebx + 0e3h]
        imul   edx, eax
        jmp    NEAR PTR loc_1_1501
        db  08Dh, 049h, 000h
loc_1_1310:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0ddh]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1504
loc_1_1324:
        mov    ebx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [ebx + 0e5h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1504
loc_1_1338:
        mov    ebx, dword ptr [ebp + 8]
        push   3
        lea    eax, [ebx + 53h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_071C
        and    dword ptr [ebp - 8], 0ffffff00h
        cmp    byte ptr [ebx + 38h], 0
        je     SHORT loc_1_135B
        or     dword ptr [ebp - 8], 1
loc_1_135B:
        lea    ecx, [ebp - 8]
        mov    esi, 4
        jmp    NEAR PTR loc_1_1504
loc_1_1368:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebx + 38h], 0
        je     SHORT loc_1_1380
        db  0B9h, 0A0h, 055h, 000h, 000h
        mov    esi, 2ch
        jmp    NEAR PTR loc_1_1504
loc_1_1380:
        db  0B9h, 0CCh, 055h, 000h, 000h
        mov    esi, 29h
        jmp    NEAR PTR loc_1_1504
        db  090h
loc_1_1390:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_13A7
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 95h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1504
loc_1_13A7:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1504
loc_1_13B8:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_13D0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1504
        db  08Dh, 064h, 024h, 000h
loc_1_13D0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, ebx
        mov    edx, dword ptr [ebx + 61h]
        mov    dword ptr [ebp - 8], 0
        sub    eax, edx
        cmp    eax, -61h
        je     NEAR PTR loc_1_1504
loc_1_13EA:
        mov    eax, dword ptr [edx + 18h]
        cmp    eax, dword ptr [ebp - 8]
        jbe    SHORT loc_1_1400
        mov    dword ptr [ebp - 8], eax
        cmp    eax, 0f0h
        je     NEAR PTR loc_1_1504
loc_1_1400:
        mov    edx, dword ptr [edx]
        mov    eax, ebx
        sub    eax, edx
        cmp    eax, -61h
        jne    SHORT loc_1_13EA
        jmp    NEAR PTR loc_1_1504
loc_1_1410:
        mov    word ptr [ebp - 2], 300h
        lea    ecx, [ebp - 2]
        mov    esi, 2
        jmp    NEAR PTR loc_1_1504
loc_1_1423:
        mov    dword ptr [ebp - 8], 6
        jmp    NEAR PTR loc_1_1504
        db  090h
loc_1_1430:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 53h]
        add    ebx, 53h
        mov    byte ptr [ebp - 18h], al
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 17h], al
        mov    al, byte ptr [ebx + 2]
        mov    byte ptr [ebp - 16h], al
        lea    ecx, [ebp - 18h]
        mov    al, byte ptr [ebx + 3]
        mov    byte ptr [ebp - 15h], al
        mov    al, byte ptr [ebx + 4]
        mov    byte ptr [ebp - 14h], al
        mov    esi, 6
        mov    al, byte ptr [ebx + 5]
        mov    byte ptr [ebp - 13h], al
        jmp    NEAR PTR loc_1_1504
loc_1_1467:
        mov    ebx, dword ptr [ebp + 8]
        mov    al, byte ptr [ebx + 4dh]
        add    ebx, 4dh
        mov    byte ptr [ebp - 18h], al
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 17h], al
        mov    al, byte ptr [ebx + 2]
        mov    byte ptr [ebp - 16h], al
        lea    ecx, [ebp - 18h]
        mov    al, byte ptr [ebx + 3]
        mov    byte ptr [ebp - 15h], al
        mov    al, byte ptr [ebx + 4]
        mov    byte ptr [ebp - 14h], al
        mov    esi, 6
        mov    al, byte ptr [ebx + 5]
        mov    byte ptr [ebp - 13h], al
        jmp    SHORT loc_1_1504
        db  005h, 000h, 000h, 000h, 000h
loc_1_14A0:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_14B1
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        jmp    SHORT loc_1_14BA
loc_1_14B1:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0a1h]
loc_1_14BA:
        mov    dword ptr [ebp - 8], eax
        mov    edx, dword ptr [ebp - 8]
        and    edx, 0ff00h
        mov    eax, dword ptr [ebp - 8]
        jmp    SHORT loc_1_14E4
        db  005h, 000h, 000h, 000h, 000h
loc_1_14D0:
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0a9h]
        mov    dword ptr [ebp - 8], eax
        mov    edx, eax
        and    edx, 0ff00h
loc_1_14E4:
        shl    eax, 10h
        or     edx, eax
        shl    edx, 8
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ff0000h
        mov    ebx, dword ptr [ebp - 8]
        shr    ebx, 10h
        or     eax, ebx
        shr    eax, 8
        or     edx, eax
loc_1_1501:
        mov    dword ptr [ebp - 8], edx
loc_1_1504:
        or     edi, edi
        jne    SHORT loc_1_1532
        cmp    dword ptr [ebp + 1ch], esi
        jae    SHORT loc_1_1522
        mov    eax, dword ptr [ebp + 20h]
        mov    edi, 0c0010014h
        mov    dword ptr [eax], esi
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_1_1522:
        push   esi
        push   ecx
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_071C
        mov    eax, dword ptr [ebp + 24h]
        add    dword ptr [eax], esi
loc_1_1532:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
        db  08Dh, 049h, 000h
loc_1_1540:
        lock adc dword ptr [eax], eax
        db  000h, 014h, 012h
        add    byte ptr [eax], al
        je     SHORT loc_1_155C
        add    byte ptr [eax], al
        je     SHORT loc_1_1560
        add    byte ptr [eax], al
        db  081h, 012h, 000h, 000h, 090h, 012h
        add    byte ptr [eax], al
        mov    cl, 12h
        add    byte ptr [eax], al
loc_1_155C:
        rcl    byte ptr [edx]
        add    byte ptr [eax], al
loc_1_1560:
        lock adc al, byte ptr [eax]
        db  000h, 010h
        adc    eax, dword ptr [eax]
        db  000h, 024h, 013h
        add    byte ptr [eax], al
        cmp    byte ptr [ebx], dl
        add    byte ptr [eax], al
        db  068h, 013h, 000h, 000h, 090h
        adc    eax, dword ptr [eax]
        db  000h, 0B8h, 013h, 000h, 000h, 010h
        adc    al, 0
        db  000h, 090h, 012h, 000h, 000h, 0E3h
        adc    dword ptr [eax], eax
        db  000h, 023h
        adc    al, 0
        add    byte ptr [ebp + 55002464h], cl
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        mov    eax, dword ptr [esi + 18h]
        push   eax
        push   0
        mov    eax, dword ptr [esi + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1180
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [esi + 20h], ecx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 24h], ecx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
sub_1_15E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 10h]
        sub    ebx, ebx
        mov    esi, dword ptr [edi + 1ch]
        mov    eax, dword ptr [edi + 18h]
        mov    ecx, dword ptr [edi + 14h]
        cmp    esi, 4
        je     SHORT loc_1_1610
        mov    dword ptr [edi + 20h], ebx
        mov    dword ptr [edi + 24h], ebx
        mov    eax, 0c0010014h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_1610:
        cmp    ecx, 1010eh
        je     SHORT loc_1_165C
        cmp    ecx, 1010fh
        je     SHORT loc_1_1680
        cmp    ecx, 10112h
        je     NEAR PTR loc_1_16F0
        cmp    ecx, 2010103h
        je     NEAR PTR loc_1_16F4
        cmp    ecx, 2010104h
        je     NEAR PTR loc_1_1705
        mov    ebx, 0c0010017h
        mov    dword ptr [edi + 20h], 0
        mov    dword ptr [edi + 24h], 0
        jmp    NEAR PTR loc_1_1716
loc_1_165C:
        push   4
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_071C
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1730
        jmp    NEAR PTR loc_1_1714
        db  090h
loc_1_1680:
        push   4
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_071C
        add    dword ptr [ebp - 4], 20h
        cmp    dword ptr [ebp - 4], 0f0h
        ja     SHORT loc_1_16E8
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [eax + 164h]
        cmp    edx, dword ptr [ebp - 4]
        jae    SHORT loc_1_16C0
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], edx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 164h], ecx
        jmp    SHORT loc_1_1716
        db  005h, 000h, 000h, 000h, 000h
loc_1_16C0:
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [ecx + 18h], edx
        jne    SHORT loc_1_16E0
        mov    edx, dword ptr [ecx + 18h]
        cmp    edx, dword ptr [ebp - 4]
        jbe    SHORT loc_1_16E0
        mov    edx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], edx
        push   eax
        call   sub_1_0E20
        jmp    SHORT loc_1_1716
        db  08Bh, 0FFh
loc_1_16E0:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 18h], eax
        jmp    SHORT loc_1_1716
loc_1_16E8:
        mov    ebx, 0c0010014h
        jmp    SHORT loc_1_1716
        db  090h
loc_1_16F0:
        sub    ebx, ebx
        jmp    SHORT loc_1_1716
loc_1_16F4:
        push   eax
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_17A0
        jmp    SHORT loc_1_1714
loc_1_1705:
        push   eax
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1800
loc_1_1714:
        mov    ebx, eax
loc_1_1716:
        or     ebx, ebx
        jne    SHORT loc_1_1724
        mov    dword ptr [edi + 20h], esi
        mov    dword ptr [edi + 24h], 0
loc_1_1724:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h
sub_1_1730:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 14h]
        test   eax, 8076h
        je     SHORT loc_1_1750
        mov    eax, 0c00000bbh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h
loc_1_1750:
        mov    esi, dword ptr [ebp + 0ch]
        push   1
        inc    dword ptr [esi + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        mov    edi, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [edi + 85h]
        push   eax
        db  0E8h, 080h, 0EDh, 0FEh, 0FFh
        mov    ebx, eax
        dec    dword ptr [esi + 14h]
        cmp    ebx, 103h
        jne    SHORT loc_1_1793
        cmp    byte ptr [edi + 0cbh], 0
        je     SHORT loc_1_1793
        mov    byte ptr [edi + 0cbh], 0
        push   edi
        call   sub_1_48C0
loc_1_1793:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h
sub_1_17A0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        push   1
        inc    dword ptr [esi + 14h]
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 85h]
        push   eax
        db  0E8h, 0F8h, 0EAh, 0FEh, 0FFh
        mov    edi, eax
        dec    dword ptr [esi + 14h]
        cmp    edi, 103h
        jne    SHORT loc_1_17EE
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0cbh], 0
        je     SHORT loc_1_17EE
        mov    byte ptr [eax + 0cbh], 0
        push   eax
        call   sub_1_48C0
loc_1_17EE:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1800:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        push   1
        inc    dword ptr [esi + 14h]
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 8]
        mov    eax, dword ptr [eax + 85h]
        push   eax
        db  0E8h, 038h, 0EBh, 0FEh, 0FFh
        mov    edi, eax
        dec    dword ptr [esi + 14h]
        cmp    edi, 103h
        jne    SHORT loc_1_184E
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 0cbh], 0
        je     SHORT loc_1_184E
        mov    byte ptr [eax + 0cbh], 0
        push   eax
        call   sub_1_48C0
loc_1_184E:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1860:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 1ch]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [esi + 18h]
        mov    dword ptr [ebp - 14h], eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        push   1
        mov    eax, dword ptr [esi + 14h]
        push   eax
        push   0
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_1_1180
        mov    edi, eax
        cmp    edi, 0c00000bbh
        jne    NEAR PTR loc_1_19A0
        sub    edi, edi
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 2010105h
        ja     SHORT loc_1_18F0
        je     SHORT loc_1_1940
        cmp    eax, 20101h
        je     SHORT loc_1_1913
        cmp    eax, 20102h
        je     SHORT loc_1_1920
        cmp    eax, 20103h
        je     SHORT loc_1_1928
        cmp    eax, 20104h
        je     SHORT loc_1_1930
        cmp    eax, 20105h
        je     SHORT loc_1_1938
        mov    edi, 0c0010017h
        jmp    NEAR PTR loc_1_1971
loc_1_18F0:
        cmp    eax, 2010106h
        je     SHORT loc_1_1950
        cmp    eax, 2010107h
        je     SHORT loc_1_1958
        cmp    eax, 2020101h
        je     SHORT loc_1_1960
        cmp    eax, 2020102h
        je     SHORT loc_1_1968
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_1971
loc_1_1913:
        mov    eax, dword ptr [ebx + 0f7h]
        jmp    SHORT loc_1_196E
        db  005h, 000h, 000h, 000h, 000h
loc_1_1920:
        mov    eax, dword ptr [ebx + 0fbh]
        jmp    SHORT loc_1_196E
loc_1_1928:
        mov    eax, dword ptr [ebx + 0ffh]
        jmp    SHORT loc_1_196E
loc_1_1930:
        mov    eax, dword ptr [ebx + 103h]
        jmp    SHORT loc_1_196E
loc_1_1938:
        mov    eax, dword ptr [ebx + 11fh]
        jmp    SHORT loc_1_196E
loc_1_1940:
        xor    eax, eax
        mov    ax, word ptr [ebx + 172h]
        or     eax, 0c0011000h
        jmp    SHORT loc_1_196E
loc_1_1950:
        mov    eax, dword ptr [ebx + 157h]
        jmp    SHORT loc_1_196E
loc_1_1958:
        mov    eax, dword ptr [ebx + 15bh]
        jmp    SHORT loc_1_196E
loc_1_1960:
        mov    eax, dword ptr [ebx + 107h]
        jmp    SHORT loc_1_196E
loc_1_1968:
        mov    eax, dword ptr [ebx + 11bh]
loc_1_196E:
        mov    dword ptr [ebp - 4], eax
loc_1_1971:
        or     edi, edi
        jne    SHORT loc_1_19A0
        cmp    dword ptr [ebp - 10h], 4
        jb     SHORT loc_1_1990
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_071C
        add    dword ptr [ebp - 0ch], 4
        jmp    SHORT loc_1_19A0
loc_1_1990:
        mov    eax, 4
        sub    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 8], eax
        mov    edi, 0c0010014h
loc_1_19A0:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 20h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 24h], eax
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_19C0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    edi, edi
        mov    ecx, dword ptr [ebp + 0ch]
        inc    dword ptr [esi + 5dh]
        mov    eax, dword ptr [ecx + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_1_1A00
        cmp    eax, 20101h
        jae    SHORT loc_1_1A23
        cmp    eax, 10101h
        jb     SHORT loc_1_1A1C
        cmp    eax, 10111h
        jbe    SHORT loc_1_1A28
        cmp    eax, 10113h
        je     SHORT loc_1_1A28
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_1A28
        db  08Dh, 049h, 000h
loc_1_1A00:
        cmp    eax, 2010101h
        jb     SHORT loc_1_1A1C
        cmp    eax, 2010107h
        jbe    SHORT loc_1_1A28
        cmp    eax, 2020101h
        jb     SHORT loc_1_1A1C
        cmp    eax, 2020102h
        jbe    SHORT loc_1_1A23
loc_1_1A1C:
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_1A28
loc_1_1A23:
        mov    edi, 103h
loc_1_1A28:
        cmp    edi, 103h
        jne    SHORT loc_1_1A77
        mov    dword ptr [ecx], 0
        mov    byte ptr [ecx + 4], 0
        cmp    dword ptr [esi + 89h], 0
        jne    SHORT loc_1_1A51
        mov    dword ptr [esi + 8dh], ecx
        mov    dword ptr [esi + 89h], ecx
        jmp    SHORT loc_1_1A59
loc_1_1A51:
        mov    eax, dword ptr [esi + 8dh]
        mov    dword ptr [eax], ecx
loc_1_1A59:
        cmp    byte ptr [esi + 0cbh], 0
        je     SHORT loc_1_1A6F
        mov    byte ptr [esi + 0cbh], 0
        push   esi
        call   sub_1_48C0
loc_1_1A6F:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
loc_1_1A77:
        or     edi, edi
        jne    SHORT loc_1_1A84
        push   ecx
        push   esi
        call   sub_1_1860
        mov    edi, eax
loc_1_1A84:
        mov    eax, dword ptr [esi + 5dh]
        dec    eax
        mov    dword ptr [esi + 5dh], eax
        jne    SHORT loc_1_1AA6
        cmp    byte ptr [esi + 15fh], 0
        jne    SHORT loc_1_1AA0
        mov    eax, dword ptr [esi + 69h]
        sub    eax, esi
        cmp    eax, 69h
        je     SHORT loc_1_1AA6
loc_1_1AA0:
        push   esi
        call   sub_1_5080
loc_1_1AA6:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_1AB0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, 103h
        mov    edi, dword ptr [ebx + 8]
        inc    dword ptr [edi + 5dh]
        cmp    byte ptr [edi + 15fh], 0
        je     SHORT loc_1_1AD4
        mov    esi, 0c001000dh
        jmp    SHORT loc_1_1B05
loc_1_1AD4:
        cmp    byte ptr [edi + 16fh], 0
        je     SHORT loc_1_1AE4
        mov    esi, 0c0000001h
        jmp    SHORT loc_1_1B05
loc_1_1AE4:
        cmp    byte ptr [ebx + 1dh], 0
        jne    SHORT loc_1_1B00
        inc    dword ptr [ebx + 14h]
        push   ebx
        push   edi
        call   sub_1_1EF0
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_1_1B05
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1B00:
        mov    esi, 0c0010002h
loc_1_1B05:
        mov    eax, dword ptr [edi + 5dh]
        dec    eax
        mov    dword ptr [edi + 5dh], eax
        jne    SHORT loc_1_1B27
        cmp    byte ptr [edi + 15fh], 0
        jne    SHORT loc_1_1B21
        mov    eax, dword ptr [edi + 69h]
        sub    eax, edi
        cmp    eax, 69h
        je     SHORT loc_1_1B27
loc_1_1B21:
        push   edi
        call   sub_1_5080
loc_1_1B27:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1B30:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [edx + 8]
        or     esi, esi
        jne    SHORT loc_1_1B4B
        mov    dword ptr [edx + 2eh], 4
        lea    esi, [edx + 1eh]
loc_1_1B4B:
        cmp    byte ptr [ecx + 15fh], 0
        je     SHORT loc_1_1B60
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
loc_1_1B60:
        mov    eax, dword ptr [ebp + 0ch]
        test   al, 30h
        je     SHORT loc_1_1B71
        mov    eax, 0c0000001h
        pop    esi
        pop    ebp
        ret    14h
loc_1_1B71:
        mov    dword ptr [esi + 4], edx
        cmp    dword ptr [esi + 10h], 4
        mov    dword ptr [esi + 8], eax
        je     SHORT loc_1_1B80
        mov    dword ptr [esi + 8], eax
loc_1_1B80:
        mov    dword ptr [esi], 0
        cmp    dword ptr [ecx + 89h], 0
        jne    SHORT loc_1_1BA0
        mov    dword ptr [ecx + 8dh], esi
        mov    dword ptr [ecx + 89h], esi
        jmp    SHORT loc_1_1BA8
        db  08Dh, 049h, 000h
loc_1_1BA0:
        mov    eax, dword ptr [ecx + 8dh]
        mov    dword ptr [eax], esi
loc_1_1BA8:
        inc    dword ptr [edx + 14h]
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1BC0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [edx + 8]
        or     esi, esi
        jne    SHORT loc_1_1BDB
        mov    dword ptr [edx + 56h], 9
        lea    esi, [edx + 46h]
loc_1_1BDB:
        cmp    byte ptr [ecx + 15fh], 0
        je     SHORT loc_1_1BF0
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
loc_1_1BF0:
        mov    dword ptr [esi + 4], edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi], 0
        cmp    dword ptr [ecx + 89h], 0
        jne    SHORT loc_1_1C16
        mov    dword ptr [ecx + 8dh], esi
        mov    dword ptr [ecx + 89h], esi
        jmp    SHORT loc_1_1C1E
loc_1_1C16:
        mov    eax, dword ptr [ecx + 8dh]
        mov    dword ptr [eax], esi
loc_1_1C1E:
        inc    dword ptr [edx + 14h]
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
loc_1_1C30:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [edx + 8]
        or     esi, esi
        jne    SHORT loc_1_1C4B
        mov    dword ptr [edx + 7eh], 0ah
        lea    esi, [edx + 6eh]
loc_1_1C4B:
        cmp    byte ptr [ecx + 15fh], 0
        je     SHORT loc_1_1C60
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
loc_1_1C60:
        mov    dword ptr [esi + 4], edx
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi], 0
        cmp    dword ptr [ecx + 89h], 0
        jne    SHORT loc_1_1C86
        mov    dword ptr [ecx + 8dh], esi
        mov    dword ptr [ecx + 89h], esi
        jmp    SHORT loc_1_1C8E
loc_1_1C86:
        mov    eax, dword ptr [ecx + 8dh]
        mov    dword ptr [eax], esi
loc_1_1C8E:
        inc    dword ptr [edx + 14h]
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h
loc_1_1CA0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        dec    dword ptr [eax + 14h]
        ret    4
        db  08Dh, 049h, 000h
sub_1_1CB0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        db  068h, 050h, 033h, 000h, 000h
        push   esi
        call   sub_1_062A
        mov    eax, dword ptr [esi + 59h]
        inc    ax
        mov    dx, ax
        sub    al, al
        out    dx, al
        push   0c350h
        call   sub_1_0630
        mov    ecx, dword ptr [esi + 59h]
        add    cx, 2
        mov    dx, cx
        sub    al, al
        out    dx, al
        cmp    byte ptr [esi + 3ah], al
        je     SHORT loc_1_1CFA
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e18h], 0c0h
loc_1_1CFA:
        cmp    dword ptr [esi + 379h], 1
        jne    SHORT loc_1_1D62
        mov    ecx, 30h
        cmp    dword ptr [esi + 37dh], 10h
        jne    SHORT loc_1_1D16
        mov    ecx, 32h
loc_1_1D16:
        mov    eax, dword ptr [esi + 385h]
        cmp    eax, 4000h
        je     SHORT loc_1_1D33
        cmp    eax, 8000h
        je     SHORT loc_1_1D38
        cmp    eax, 10000h
        je     SHORT loc_1_1D40
        jmp    SHORT loc_1_1D43
loc_1_1D33:
        or     ecx, 4
        jmp    SHORT loc_1_1D43
loc_1_1D38:
        or     ecx, 8
        jmp    SHORT loc_1_1D43
        db  08Dh, 049h, 000h
loc_1_1D40:
        or     ecx, 0ch
loc_1_1D43:
        mov    edx, dword ptr [esi + 59h]
        cmp    edx, 0a20h
        je     SHORT loc_1_1D51
        or     ecx, 1
loc_1_1D51:
        mov    al, 0ch
        out    dx, al
        mov    al, 12h
        out    dx, al
        mov    al, 26h
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    al, 40h
        out    dx, al
        jmp    SHORT loc_1_1D68
loc_1_1D62:
        cmp    byte ptr [esi + 38h], 0
        je     SHORT loc_1_1D77
loc_1_1D68:
        mov    eax, dword ptr [esi + 0adh]
        mov    cl, byte ptr [esi + 47h]
        mov    byte ptr [eax + 1e00h], cl
loc_1_1D77:
        cmp    dword ptr [esi + 379h], 1
        je     SHORT loc_1_1D90
        push   esi
        db  068h, 030h, 033h, 000h, 000h
        push   esi
        call   sub_1_062A
        jmp    SHORT loc_1_1DC1
        db  08Bh, 0FFh
loc_1_1D90:
        mov    eax, dword ptr [esi + 0adh]
        mov    al, byte ptr [eax + 1e08h]
        mov    cl, al
        mov    eax, dword ptr [esi + 0adh]
        and    cl, 3fh
        mov    byte ptr [eax + 1e08h], cl
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [esi + 168h], 1
        mov    byte ptr [eax + 1e08h], 0c0h
loc_1_1DC1:
        mov    eax, dword ptr [esi + 0adh]
        push   0c001000ch
        mov    byte ptr [eax + 1e08h], 0c0h
        push   esi
        call   sub_1_52E0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
sub_1_1DE0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0A50
        cmp    byte ptr [esi + 176h], 0
        je     SHORT loc_1_1E0D
        mov    byte ptr [esi + 176h], 0
        mov    byte ptr [esi + 174h], 0
        mov    byte ptr [esi + 177h], 0
loc_1_1E0D:
        mov    byte ptr [esi + 15fh], 0
        mov    byte ptr [esi + 175h], 0
        mov    byte ptr [esi + 168h], 0
        mov    byte ptr [esi + 169h], 0
        mov    byte ptr [esi + 171h], 0
        push   esi
        call   sub_1_4930
        mov    ebx, dword ptr [esi + 16ah]
        or     ebx, ebx
        je     SHORT loc_1_1E80
        mov    edi, dword ptr [esi + 61h]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -61h
        je     SHORT loc_1_1E72
loc_1_1E4C:
        push   0
        push   0
        push   40010005h
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_05A4
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -61h
        jne    SHORT loc_1_1E4C
loc_1_1E72:
        dec    dword ptr [ebx + 14h]
        push   0
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_0586
loc_1_1E80:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1E90:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0adh]
        mov    byte ptr [eax + 1e08h], 0c0h
        mov    eax, dword ptr [ecx + 0adh]
        mov    byte ptr [eax + 1e0fh], 1
        mov    eax, dword ptr [ecx + 0adh]
        mov    byte ptr [eax + 1e0ch], 0
        cmp    dword ptr [ecx + 379h], 1
        jne    SHORT loc_1_1ED7
        mov    eax, dword ptr [ecx + 0adh]
        pop    ebp
        mov    byte ptr [eax + 1e00h], 0d0h
        ret    4
loc_1_1ED7:
        cmp    byte ptr [ecx + 38h], 0
        je     SHORT loc_1_1EEC
        mov    eax, dword ptr [ecx + 0adh]
        mov    dl, byte ptr [ecx + 47h]
        mov    byte ptr [eax + 1e00h], dl
loc_1_1EEC:
        pop    ebp
        ret    4
sub_1_1EF0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        or     edi, edi
        mov    esi, dword ptr [ebp + 8]
        je     SHORT loc_1_1F29
        mov    ebx, dword ptr [esi + 61h]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -61h
        je     SHORT loc_1_1F29
loc_1_1F0C:
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_059E
        mov    ebx, dword ptr [ebx]
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -61h
        jne    SHORT loc_1_1F0C
loc_1_1F29:
        mov    byte ptr [esi + 15fh], 1
        mov    byte ptr [esi + 171h], 1
        mov    dword ptr [esi + 16ah], edi
        mov    dword ptr [esi + 160h], 0
        or     edi, edi
        je     SHORT loc_1_1F4E
        inc    dword ptr [edi + 14h]
loc_1_1F4E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1F60:
        push   ebp
        mov    ebp, esp
        sub    esp, 0a0h
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 60h], 0ch
        mov    word ptr [ebp - 5eh], 0ch
        db  0C7h, 045h, 0A4h, 004h, 056h, 000h, 000h
        mov    word ptr [ebp - 58h], 8
        mov    word ptr [ebp - 56h], 8
        db  0C7h, 045h, 0ACh, 018h, 056h, 000h, 000h
        mov    word ptr [ebp - 38h], 7
        mov    word ptr [ebp - 36h], 7
        db  0C7h, 045h, 0CCh, 028h, 056h, 000h, 000h
        mov    word ptr [ebp - 40h], 0eh
        mov    word ptr [ebp - 3eh], 0eh
        db  0C7h, 045h, 0C4h, 058h, 056h, 000h, 000h
        mov    word ptr [ebp - 50h], 0ah
        mov    bl, 1
        mov    word ptr [ebp - 4eh], 0ah
        db  0C7h, 045h, 0B4h, 084h, 056h, 000h, 000h
        mov    word ptr [ebp - 48h], 0ah
        mov    word ptr [ebp - 46h], 0ah
        db  0C7h, 045h, 0BCh, 0B4h, 056h, 000h, 000h
        mov    dword ptr [ebp - 0ch], 0
        mov    byte ptr [ebp - 1], 0
        mov    byte ptr [ebp - 2], 0
        db  0A1h, 0C8h, 056h, 000h, 000h
        push   eax
        lea    eax, [ebp - 8]
        push   0
        push   38dh
        push   eax
        call   sub_1_0636
        or     eax, eax
        je     SHORT loc_1_2017
        mov    eax, 0c000009ah
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_2017:
        mov    edi, dword ptr [ebp - 8]
        sub    eax, eax
        mov    ecx, 0e3h
        rep stosd
        stosb
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0dfh], 4628h
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 15bh], 2
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    byte ptr [eax + 175h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 7dh], eax
        lea    eax, [ebp - 14h]
        push   esi
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_06B4
        cmp    dword ptr [ebp - 0ch], 0
        push   0
        je     SHORT loc_1_2089
        push   38dh
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_063C
        mov    eax, 0c0000001h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
loc_1_2089:
        lea    eax, [ebp - 60h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_20B3
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 379h], eax
loc_1_20B3:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 379h], 1
        je     SHORT loc_1_20E9
        push   1
        lea    eax, [ebp - 58h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_20E9
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 4], 3
        jne    SHORT loc_1_20E9
        mov    byte ptr [ebp - 2], 1
loc_1_20E9:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_1_2122
        lea    eax, [ebp - 30h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        push   esi
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_05B0
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_2110
        mov    byte ptr [ebp - 1], 1
        jmp    NEAR PTR loc_1_233C
loc_1_2110:
        mov    al, byte ptr [ebp - 2dh]
        and    eax, 1
        je     SHORT loc_1_2188
        cmp    eax, 1
        je     SHORT loc_1_2194
        jmp    NEAR PTR loc_1_219E
loc_1_2122:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 379h], 1
        jne    SHORT loc_1_2159
        push   1
        lea    eax, [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_219E
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 59h], eax
        jmp    SHORT loc_1_219E
loc_1_2159:
        push   0
        lea    eax, [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_2184
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 4]
        dec    eax
        cmp    eax, 1
        sbb    bl, bl
        neg    bl
loc_1_2184:
        or     bl, bl
        je     SHORT loc_1_2194
loc_1_2188:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 59h], 0a20h
        jmp    SHORT loc_1_219E
loc_1_2194:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 59h], 0a24h
loc_1_219E:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 379h], 1
        jne    NEAR PTR loc_1_22C2
        mov    word ptr [ebp - 0a0h], 0
        lea    edi, [ebp - 9eh]
        sub    eax, eax
        mov    ecx, 7
        rep stosd
        stosw
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 28h]
        push   eax
        push   20h
        lea    eax, [ebp - 0a0h]
        push   eax
        push   1
        push   esi
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_0604
        add    esp, 1ch
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_21F7
        mov    byte ptr [ebp - 1], 1
        jmp    NEAR PTR loc_1_233C
loc_1_21F7:
        mov    eax, dword ptr [ebp - 98h]
        sub    eax, dword ptr [ebp - 9ch]
        inc    eax
        cmp    eax, 2000h
        je     SHORT loc_1_2214
        mov    byte ptr [ebp - 1], 1
        jmp    NEAR PTR loc_1_233C
loc_1_2214:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 9ch]
        mov    dword ptr [ecx + 389h], eax
        mov    ecx, dword ptr [ebp - 8ch]
        mov    edx, dword ptr [ebp - 88h]
        sub    edx, ecx
        inc    edx
        cmp    edx, 2000h
        je     SHORT loc_1_225B
        cmp    edx, 4000h
        je     SHORT loc_1_225B
        cmp    edx, 8000h
        je     SHORT loc_1_225B
        cmp    edx, 10000h
        je     SHORT loc_1_225B
        mov    byte ptr [ebp - 1], 1
        jmp    NEAR PTR loc_1_233C
loc_1_225B:
        mov    eax, dword ptr [ebp - 8]
        push   0
        mov    dword ptr [eax + 385h], edx
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 381h], ecx
        lea    eax, [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_2299
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 37dh], eax
loc_1_2299:
        push   0
        lea    eax, [ebp - 48h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_22C2
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 33h], eax
loc_1_22C2:
        push   0
        lea    eax, [ebp - 40h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_22EE
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx + 0dfh], eax
loc_1_22EE:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_05BC
        cmp    dword ptr [ebp - 1ch], 6
        jne    SHORT loc_1_233C
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_233C
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 18h]
        mov    edx, ecx
        add    eax, 4dh
        mov    dl, byte ptr [edx]
        mov    byte ptr [eax], dl
        mov    dl, byte ptr [ecx + 1]
        mov    byte ptr [eax + 1], dl
        mov    dl, byte ptr [ecx + 2]
        mov    byte ptr [eax + 2], dl
        mov    dl, byte ptr [ecx + 3]
        mov    byte ptr [eax + 3], dl
        mov    dl, byte ptr [ecx + 4]
        mov    byte ptr [eax + 4], dl
        mov    cl, byte ptr [ecx + 5]
        mov    byte ptr [eax + 5], cl
loc_1_233C:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_06C0
        mov    al, byte ptr [ebp - 1]
        push   eax
        mov    al, byte ptr [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_07D0
        mov    dword ptr [ebp - 0ch], eax
        or     eax, eax
        je     SHORT loc_1_2372
        push   0
        push   38dh
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_063C
loc_1_2372:
        mov    eax, dword ptr [ebp - 0ch]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Bh, 0FFh
loc_1_2380:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0714
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 20h]
        push   eax
        call   sub_1_0618
        mov    ecx, dword ptr [esi + 59h]
        inc    cx
        mov    dx, cx
        sub    al, al
        out    dx, al
        push   2625a0h
        call   sub_1_0630
        push   esi
        call   sub_1_0624
        mov    eax, dword ptr [esi + 85h]
        push   eax
        db  0E8h, 006h, 0DDh, 0FEh, 0FFh
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0568
        push   0
        push   38dh
        push   esi
        call   sub_1_063C
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_23F0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 171h], 1
        cmp    byte ptr [eax + 0cbh], 0
        je     SHORT loc_1_243A
        mov    ecx, dword ptr [eax + 0b7h]
        or     ecx, ecx
        je     SHORT loc_1_243A
        mov    edx, dword ptr [eax + 0adh]
        pop    ebp
        mov    byte ptr [edx + 1e08h], 80h
        mov    word ptr [ecx], 0
        mov    byte ptr [ecx + 2], 0
        mov    byte ptr [ecx], 4
        mov    eax, dword ptr [eax + 0adh]
        mov    byte ptr [eax + 1e4bh], 20h
        ret    4
loc_1_243A:
        mov    edx, dword ptr [eax + 59h]
        inc    dx
        sub    al, al
        out    dx, al
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2450:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_055C
        push   0
        mov    eax, dword ptr [esi]
        push   eax
        call   sub_1_0550
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_2480:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 379h], 1
        jne    NEAR PTR loc_1_2639
        mov    ecx, 30h
        cmp    dword ptr [esi + 37dh], 10h
        jne    SHORT loc_1_24AC
        mov    ecx, 32h
loc_1_24AC:
        mov    eax, dword ptr [esi + 385h]
        cmp    eax, 4000h
        je     SHORT loc_1_24C9
        cmp    eax, 8000h
        je     SHORT loc_1_24D0
        cmp    eax, 10000h
        je     SHORT loc_1_24D5
        jmp    SHORT loc_1_24D8
loc_1_24C9:
        or     ecx, 4
        jmp    SHORT loc_1_24D8
        db  08Bh, 0FFh
loc_1_24D0:
        or     ecx, 8
        jmp    SHORT loc_1_24D8
loc_1_24D5:
        or     ecx, 0ch
loc_1_24D8:
        mov    edx, dword ptr [esi + 59h]
        cmp    edx, 0a20h
        je     SHORT loc_1_24E6
        or     ecx, 1
loc_1_24E6:
        mov    al, 0ch
        out    dx, al
        mov    al, 12h
        out    dx, al
        mov    al, 26h
        out    dx, al
        mov    al, cl
        out    dx, al
        mov    al, 40h
        out    dx, al
        push   2000h
        mov    eax, dword ptr [esi + 389h]
        push   eax
        mov    eax, dword ptr [esi + 81h]
        push   eax
        lea    eax, [esi + 0adh]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_0702
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_2540
        push   0
        push   0aa000001h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 0ch
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_2540:
        sub    eax, eax
        lea    ecx, [ebp - 38h]
loc_1_2545:
        mov    dword ptr [ecx], 0
        sub    edi, edi
        lea    ecx, [ecx]
loc_1_2550:
        mov    edx, dword ptr [esi + 0adh]
        add    edx, edi
        mov    dl, byte ptr [eax + edx + 1f30h]
        mov    byte ptr [ebp - 2], dl
        and    edx, 0fh
        mov    ebx, dword ptr [ecx]
        shl    ebx, 4
        add    edx, ebx
        mov    dword ptr [ecx], edx
        add    edi, 2
        cmp    edi, 10h
        jb     SHORT loc_1_2550
        add    eax, 10h
        add    ecx, 4
        lea    edx, [ebp - 2ch]
        cmp    ecx, edx
        jb     SHORT loc_1_2545
        db  0A1h, 060h, 055h, 000h, 000h
        cmp    dword ptr [ebp - 38h], eax
        jne    NEAR PTR loc_1_2618
        db  0A1h, 064h, 055h, 000h, 000h
        cmp    dword ptr [ebp - 34h], eax
        jne    SHORT loc_1_2618
        db  0A1h, 068h, 055h, 000h, 000h
        cmp    dword ptr [ebp - 30h], eax
        jne    SHORT loc_1_2618
        mov    byte ptr [esi + 38h], 1
        mov    eax, dword ptr [esi + 385h]
        push   eax
        mov    eax, dword ptr [esi + 381h]
        push   eax
        mov    eax, dword ptr [esi + 81h]
        push   eax
        lea    eax, [esi + 0b1h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_0702
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_25F5
        push   0
        push   0aa000001h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 0ch
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_25F5:
        mov    byte ptr [esi + 3ah], 0
        mov    eax, dword ptr [esi + 385h]
        mov    dword ptr [esi + 3fh], eax
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [esi + 47h], 0d0h
        mov    byte ptr [eax + 1e00h], 0d0h
        jmp    NEAR PTR loc_1_29C3
loc_1_2618:
        push   0
        push   0aa00000dh
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 0ch
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2639:
        mov    dx, word ptr [esi + 59h]
        in     al, dx
        mov    byte ptr [ebp - 5], al
        mov    edx, dword ptr [esi + 59h]
        add    dx, 2
        in     al, dx
        mov    byte ptr [ebp - 6], al
        mov    cl, al
        and    ecx, 1
        shl    ecx, 13h
        mov    al, byte ptr [ebp - 5]
        and    eax, 0fch
        shl    eax, 0bh
        or     ecx, eax
        lea    eax, [esi + 0adh]
        push   2000h
        push   ecx
        mov    eax, dword ptr [esi + 81h]
        push   eax
        lea    eax, [esi + 0adh]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_0702
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_26AB
        push   0
        push   0aa000001h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 0ch
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_26AB:
        sub    ecx, ecx
        lea    eax, [ebp - 2ch]
loc_1_26B0:
        mov    dword ptr [eax], 0
        sub    edi, edi
loc_1_26B8:
        mov    edx, dword ptr [esi + 0adh]
        add    edx, edi
        mov    dl, byte ptr [ecx + edx + 1f30h]
        mov    byte ptr [ebp - 1], dl
        and    edx, 0fh
        mov    ebx, dword ptr [eax]
        shl    ebx, 4
        add    edx, ebx
        mov    dword ptr [eax], edx
        add    edi, 2
        cmp    edi, 10h
        jb     SHORT loc_1_26B8
        add    ecx, 10h
        add    eax, 4
        lea    edx, [ebp - 20h]
        cmp    eax, edx
        jb     SHORT loc_1_26B0
        mov    eax, dword ptr [ebp - 2ch]
        db  039h, 005h, 078h, 055h, 000h, 000h
        jne    SHORT loc_1_2712
        mov    eax, dword ptr [ebp - 28h]
        db  039h, 005h, 07Ch, 055h, 000h, 000h
        jne    SHORT loc_1_2712
        mov    eax, dword ptr [ebp - 24h]
        db  039h, 005h, 080h, 055h, 000h, 000h
        jne    SHORT loc_1_2712
        mov    byte ptr [esi + 38h], 1
        jmp    SHORT loc_1_2746
loc_1_2712:
        mov    eax, dword ptr [ebp - 2ch]
        db  039h, 005h, 084h, 055h, 000h, 000h
        jne    NEAR PTR loc_1_2A84
        mov    eax, dword ptr [ebp - 28h]
        db  039h, 005h, 088h, 055h, 000h, 000h
        jne    NEAR PTR loc_1_2A84
        mov    eax, dword ptr [ebp - 24h]
        db  039h, 005h, 08Ch, 055h, 000h, 000h
        jne    NEAR PTR loc_1_2A84
        lea    eax, [esi + 38h]
        mov    byte ptr [esi + 38h], 0
loc_1_2746:
        mov    eax, dword ptr [esi + 0adh]
        mov    al, byte ptr [eax + 1e01h]
        mov    byte ptr [ebp - 4], al
        and    al, 0ch
        shr    al, 2
        mov    byte ptr [ebp - 4], al
        cmp    byte ptr [esi + 38h], 0
        je     NEAR PTR loc_1_2840
        mov    dl, byte ptr [ebp - 5]
        and    dl, 0fdh
        shr    dl, 1
        or     dl, 80h
        add    dl, 2
        mov    byte ptr [esi + 47h], dl
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        jl     SHORT loc_1_279C
        cmp    eax, 1
        jle    NEAR PTR loc_1_280A
        cmp    eax, 2
        je     NEAR PTR loc_1_2820
        cmp    eax, 3
        je     NEAR PTR loc_1_2830
loc_1_279C:
        mov    edi, dword ptr [ebp - 18h]
        mov    al, byte ptr [ebp - 7]
loc_1_27A2:
        xor    ecx, ecx
        mov    cl, al
        xor    ebx, ebx
        mov    bl, dl
        dec    ecx
        test   ebx, ecx
        je     SHORT loc_1_27BB
        lea    ecx, [eax - 1]
        not    cl
        and    cl, dl
        add    cl, al
        mov    byte ptr [esi + 47h], cl
loc_1_27BB:
        mov    dword ptr [esi + 3fh], edi
        xor    eax, eax
        mov    al, byte ptr [esi + 47h]
        shl    eax, 0ch
        push   edi
        push   eax
        mov    eax, dword ptr [esi + 81h]
        push   eax
        lea    eax, [esi + 0b1h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_0702
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_28CA
        push   0
        push   0aa000001h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 0ch
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_280A:
        cmp    byte ptr [ebp - 4], 1
        sbb    edi, edi
        and    edi, 0ffffe000h
        add    edi, 4000h
        mov    al, 4
        jmp    SHORT loc_1_27A2
loc_1_2820:
        mov    edi, 8000h
        mov    al, 8
        jmp    NEAR PTR loc_1_27A2
        db  08Dh, 064h, 024h, 000h
loc_1_2830:
        mov    edi, 10000h
        mov    al, 10h
        jmp    NEAR PTR loc_1_27A2
        db  08Dh, 064h, 024h, 000h
loc_1_2840:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        or     eax, eax
        je     SHORT loc_1_28A6
        cmp    eax, 1
        je     SHORT loc_1_28B0
        cmp    eax, 2
        je     SHORT loc_1_28B7
        cmp    eax, 3
        je     SHORT loc_1_28C0
        mov    edi, dword ptr [ebp - 1ch]
loc_1_285B:
        mov    al, byte ptr [ebp - 6]
        and    eax, 0feh
        shl    eax, 0ch
        push   edi
        push   eax
        mov    eax, dword ptr [esi + 81h]
        push   eax
        lea    eax, [esi + 0b1h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_0702
        cmp    dword ptr [ebp - 0ch], 0
        je     SHORT loc_1_28C7
        push   0
        push   0aa000001h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 0ch
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_28A6:
        mov    edi, 2000h
        jmp    SHORT loc_1_285B
        db  08Dh, 049h, 000h
loc_1_28B0:
        mov    edi, 4000h
        jmp    SHORT loc_1_285B
loc_1_28B7:
        mov    edi, 8000h
        jmp    SHORT loc_1_285B
        db  08Bh, 0FFh
loc_1_28C0:
        mov    edi, 10000h
        jmp    SHORT loc_1_285B
loc_1_28C7:
        mov    dword ptr [esi + 3fh], edi
loc_1_28CA:
        mov    al, byte ptr [ebp - 5]
        and    eax, 3
        je     SHORT loc_1_2912
        cmp    eax, 1
        je     SHORT loc_1_2919
        cmp    eax, 2
        je     SHORT loc_1_2920
        cmp    eax, 3
        je     SHORT loc_1_2930
        mov    eax, dword ptr [ebp - 20h]
loc_1_28E4:
        mov    dword ptr [esi + 33h], eax
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [esi + 39h], 0
        mov    al, byte ptr [eax + 1fa6h]
        mov    byte ptr [ebp - 3], al
        xor    eax, eax
        and    byte ptr [ebp - 3], 0fh
        mov    al, byte ptr [ebp - 3]
        lea    ecx, [eax - 0ah]
        cmp    ecx, 5
        ja     SHORT loc_1_2940
        db  0FFh, 024h, 08Dh, 0A8h, 02Ah, 000h, 000h
loc_1_2912:
        mov    eax, 2
        jmp    SHORT loc_1_28E4
loc_1_2919:
        mov    eax, 3
        jmp    SHORT loc_1_28E4
loc_1_2920:
        cmp    byte ptr [esi + 38h], 1
        sbb    eax, eax
        and    eax, 4
        add    eax, 6
        jmp    SHORT loc_1_28E4
        db  08Bh, 0FFh
loc_1_2930:
        cmp    byte ptr [esi + 38h], 1
        sbb    eax, eax
        and    eax, 4
        add    eax, 7
        jmp    SHORT loc_1_28E4
        db  08Bh, 0FFh
loc_1_2940:
        push   eax
        push   6
        push   2
        push   3
        push   0aa00000ah
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 18h
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2966:
        mov    dword ptr [esi + 3bh], 10000h
        jmp    SHORT loc_1_29B3
        db  090h
loc_1_2970:
        mov    dword ptr [esi + 3bh], 10000h
        mov    byte ptr [esi + 39h], 1
        jmp    SHORT loc_1_29B3
        db  08Dh, 049h, 000h
loc_1_2980:
        mov    dword ptr [esi + 3bh], 8000h
        jmp    SHORT loc_1_29B3
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2990:
        mov    dword ptr [esi + 3bh], 4000h
        jmp    SHORT loc_1_29B3
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_29A0:
        mov    dword ptr [esi + 3bh], 2000h
        jmp    SHORT loc_1_29B3
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_29B0:
        mov    dword ptr [esi + 3bh], edi
loc_1_29B3:
        mov    eax, dword ptr [esi + 0adh]
        mov    al, byte ptr [eax + 1fa8h]
        mov    byte ptr [esi + 3ah], 0
loc_1_29C3:
        mov    eax, dword ptr [esi + 0adh]
        mov    al, byte ptr [eax + 1faah]
        mov    byte ptr [ebp - 3], al
        and    byte ptr [ebp - 3], 0fh
        xor    eax, eax
        mov    al, byte ptr [ebp - 3]
        cmp    eax, 0dh
        je     SHORT loc_1_29F0
        cmp    eax, 0eh
        je     SHORT loc_1_29F8
        mov    word ptr [esi + 43h], 800h
        jmp    SHORT loc_1_29FE
        db  08Dh, 049h, 000h
loc_1_29F0:
        mov    word ptr [esi + 43h], 1170h
        jmp    SHORT loc_1_29FE
loc_1_29F8:
        mov    word ptr [esi + 43h], 1000h
loc_1_29FE:
        mov    eax, dword ptr [esi + 0adh]
        mov    al, byte ptr [eax + 1fach]
        mov    byte ptr [ebp - 3], al
        and    byte ptr [ebp - 3], 0fh
        xor    eax, eax
        mov    al, byte ptr [ebp - 3]
        cmp    eax, 0bh
        je     SHORT loc_1_2A40
        cmp    eax, 0ch
        je     SHORT loc_1_2A51
        cmp    eax, 0dh
        je     SHORT loc_1_2A62
        cmp    eax, 0eh
        je     SHORT loc_1_2A73
        mov    word ptr [esi + 45h], 800h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_2A40:
        mov    word ptr [esi + 45h], 4628h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2A51:
        mov    word ptr [esi + 45h], 4000h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2A62:
        mov    word ptr [esi + 45h], 2000h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2A73:
        mov    word ptr [esi + 45h], 1000h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_2A84:
        push   0
        push   0aa00000dh
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 0ch
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 066h, 029h, 000h, 000h, 070h, 029h, 000h, 000h, 080h, 029h, 000h, 000h, 090h
        db  029h, 000h, 000h, 0A0h, 029h, 000h, 000h, 0B0h, 029h, 000h, 000h
loc_1_2AC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 8]
        mov    edi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 103h
        mov    esi, dword ptr [edi + 8]
        push   eax
        push   0
        push   0
        push   0
        mov    ebx, dword ptr [ebp + 0ch]
        push   ebx
        call   sub_1_0660
        cmp    dword ptr [ebp - 8], 0eh
        jb     NEAR PTR loc_1_2BFA
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [esi + 0dfh], eax
        jb     NEAR PTR loc_1_2BFA
        inc    dword ptr [esi + 5dh]
        cmp    byte ptr [esi + 174h], 0
        je     SHORT loc_1_2B20
        mov    dword ptr [ebp - 4], 0c0010008h
        jmp    NEAR PTR loc_1_2BCC
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2B20:
        cmp    byte ptr [esi + 171h], 0
        jne    SHORT loc_1_2B87
        lea    eax, [ebx + 24h]
        cmp    byte ptr [edi + 1dh], 0
        je     SHORT loc_1_2B40
        mov    dword ptr [ebp - 4], 0c0010002h
        jmp    NEAR PTR loc_1_2BCC
        db  08Bh, 0FFh
loc_1_2B40:
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [eax + 8], ebx
        cmp    dword ptr [esi + 0e7h], 0
        jne    SHORT loc_1_2B57
        mov    dword ptr [esi + 0e7h], ebx
        jmp    SHORT loc_1_2B60
loc_1_2B57:
        mov    ecx, dword ptr [esi + 0ebh]
        mov    dword ptr [ecx + 24h], ebx
loc_1_2B60:
        mov    dword ptr [esi + 0ebh], ebx
        mov    dword ptr [eax], 0
        inc    dword ptr [edi + 14h]
        cmp    byte ptr [esi + 0cbh], 0
        je     SHORT loc_1_2BCC
        mov    byte ptr [esi + 0cbh], 0
        push   esi
        call   sub_1_48C0
        jmp    SHORT loc_1_2BCC
loc_1_2B87:
        cmp    byte ptr [esi + 15fh], 0
        je     SHORT loc_1_2BA0
        mov    dword ptr [ebp - 4], 0c001000dh
        jmp    SHORT loc_1_2BCC
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2BA0:
        cmp    byte ptr [esi + 16fh], 0
        je     SHORT loc_1_2BB2
        mov    dword ptr [ebp - 4], 0c0000001h
        jmp    SHORT loc_1_2BCC
loc_1_2BB2:
        push   2
        push   0bh
        push   2
        push   0aa000006h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 14h
loc_1_2BCC:
        mov    eax, dword ptr [esi + 5dh]
        dec    eax
        mov    dword ptr [esi + 5dh], eax
        jne    SHORT loc_1_2BEE
        cmp    byte ptr [esi + 15fh], 0
        jne    SHORT loc_1_2BE8
        mov    eax, dword ptr [esi + 69h]
        sub    eax, esi
        cmp    eax, 69h
        je     SHORT loc_1_2BEE
loc_1_2BE8:
        push   esi
        call   sub_1_5080
loc_1_2BEE:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_2BFA:
        mov    eax, 0c001000fh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_2C10:
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
        inc    dword ptr [eax + 5dh]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 171h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_2EB9
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_2EAD
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0d6h]
        mov    word ptr [ebp - 10h], ax
        mov    dword ptr [ebp - 34h], 0
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [eax], 0
        jmp    NEAR PTR loc_1_2C80
loc_1_2C80:
        jmp    NEAR PTR loc_1_2C85
loc_1_2C85:
        push   0
        lea    eax, [ebp - 24h]
        push   eax
        lea    eax, [ebp - 2ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_0660
        cmp    dword ptr [ebp - 2ch], 0
        je     NEAR PTR loc_1_2E9B
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_0684
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        add    eax, dword ptr [ecx + 0b1h]
        add    eax, 2
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0d8h]
        and    eax, 0ffffh
        add    eax, dword ptr [ebp - 14h]
        add    eax, 6
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 28h], ax
        mov    eax, dword ptr [ebp - 28h]
        and    eax, 0ffffh
        mov    ah, al
        mov    ecx, dword ptr [ebp - 28h]
        and    ecx, 0ffffh
        mov    al, ch
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        mov    cx, word ptr [ecx + 0d8h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    dword ptr [ebp - 18h], eax
loc_1_2D28:
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [ebp - 34h], eax
        jae    NEAR PTR loc_1_2E93
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_1_2D70
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_06A8
        cmp    dword ptr [ebp - 24h], 0
        jne    NEAR PTR loc_1_2D5A
        jmp    NEAR PTR loc_1_2E93
loc_1_2D5A:
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_0684
        jmp    NEAR PTR loc_1_2D28
loc_1_2D70:
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_2DE0
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_1_2D99
        jmp    NEAR PTR loc_1_2E93
loc_1_2D99:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    al, ah
        mov    ah, byte ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        add    eax, dword ptr [ecx + 0b1h]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        add    eax, 6
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [ebp - 18h], ax
        sub    eax, eax
        mov    ah, byte ptr [ebp - 18h]
        mov    al, byte ptr [ebp - 17h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_2D28
loc_1_2DE0:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_2E28
        mov    eax, dword ptr [ebp - 18h]
        cmp    dword ptr [ebp + 10h], eax
        jbe    NEAR PTR loc_1_2E11
        sub    eax, eax
        sub    eax, dword ptr [ebp - 18h]
        neg    eax
        sub    dword ptr [ebp + 10h], eax
        mov    dword ptr [ebp - 18h], 0
        jmp    NEAR PTR loc_1_2D28
        db  0E9h, 017h, 000h, 000h, 000h
loc_1_2E11:
        mov    eax, dword ptr [ebp + 10h]
        add    dword ptr [ebp - 30h], eax
        sub    eax, eax
        sub    eax, dword ptr [ebp + 10h]
        neg    eax
        sub    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp + 10h], 0
loc_1_2E28:
        mov    eax, dword ptr [ebp + 14h]
        sub    eax, dword ptr [ebp - 34h]
        mov    dword ptr [ebp - 3ch], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 20h]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_2E41
        mov    eax, ecx
loc_1_2E41:
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp - 3ch]
        mov    ecx, dword ptr [ebp - 38h]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_2E54
        mov    eax, ecx
loc_1_2E54:
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        call   sub_1_071C
        mov    eax, dword ptr [ebp - 38h]
        add    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 38h]
        add    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp - 38h]
        add    dword ptr [ebp - 34h], eax
        sub    eax, eax
        sub    eax, dword ptr [ebp - 38h]
        neg    eax
        sub    dword ptr [ebp - 18h], eax
        sub    eax, eax
        sub    eax, dword ptr [ebp - 38h]
        neg    eax
        sub    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_2D28
loc_1_2E93:
        mov    eax, dword ptr [ebp - 34h]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
loc_1_2E9B:
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_2EB4
loc_1_2EAD:
        mov    dword ptr [ebp - 8], 0c001000ch
loc_1_2EB4:
        jmp    NEAR PTR loc_1_2F1A
loc_1_2EB9:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 15fh]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_2EDB
        mov    dword ptr [ebp - 8], 0c001000dh
        jmp    NEAR PTR loc_1_2F1A
loc_1_2EDB:
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 170h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_2EFD
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_1_2F1A
loc_1_2EFD:
        push   1
        push   0bh
        push   2
        push   0aa000006h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 14h
loc_1_2F1A:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 40h], eax
        mov    eax, dword ptr [ebp - 40h]
        dec    dword ptr [eax + 5dh]
        mov    eax, dword ptr [ebp - 40h]
        cmp    dword ptr [eax + 5dh], 0
        jne    NEAR PTR loc_1_2F69
        mov    eax, dword ptr [ebp - 40h]
        mov    al, byte ptr [eax + 15fh]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_2F5B
        mov    eax, dword ptr [ebp - 40h]
        mov    ecx, dword ptr [ebp - 40h]
        add    ecx, 69h
        cmp    dword ptr [eax + 69h], ecx
        je     NEAR PTR loc_1_2F69
loc_1_2F5B:
        mov    eax, dword ptr [ebp - 40h]
        push   eax
        call   sub_1_5080
        jmp    NEAR PTR loc_1_2F69
loc_1_2F69:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_2F71
loc_1_2F71:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    18h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2F80:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        sub    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 10h], esi
        je     NEAR PTR loc_1_304A
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0660
        cmp    dword ptr [ebp - 10h], esi
        je     NEAR PTR loc_1_304A
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0684
        cmp    dword ptr [ebp + 10h], esi
        je     SHORT loc_1_3045
        mov    edi, dword ptr [ebp + 0ch]
        mov    edi, edi
loc_1_2FD0:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_2FFC
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_06A8
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_3045
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0684
        jmp    SHORT loc_1_3040
loc_1_2FFC:
        or     edi, edi
        je     SHORT loc_1_3019
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_1_3011
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_3040
loc_1_3011:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_3019:
        mov    ebx, dword ptr [ebp + 10h]
        sub    ebx, esi
        mov    eax, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     SHORT loc_1_3027
        mov    ebx, eax
loc_1_3027:
        push   ebx
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        add    esi, ebx
        push   eax
        call   sub_1_071C
        add    dword ptr [ebp + 14h], ebx
        add    dword ptr [ebp - 0ch], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_3040:
        cmp    esi, dword ptr [ebp + 10h]
        jb     SHORT loc_1_2FD0
loc_1_3045:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_304A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        sub    esi, esi
        mov    dword ptr [eax], esi
        cmp    dword ptr [ebp + 0ch], esi
        je     NEAR PTR loc_1_312B
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_0660
        cmp    dword ptr [ebp - 14h], esi
        je     NEAR PTR loc_1_312B
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0684
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp + 0ch], esi
        je     SHORT loc_1_3126
        mov    edi, dword ptr [ebp + 14h]
loc_1_30B4:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_30E0
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_06A8
        cmp    dword ptr [ebp - 8], 0
        je     SHORT loc_1_3126
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0684
        jmp    SHORT loc_1_3121
loc_1_30E0:
        or     edi, edi
        je     SHORT loc_1_30FD
        cmp    dword ptr [ebp - 4], edi
        jae    SHORT loc_1_30F5
        sub    edi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_3121
loc_1_30F5:
        add    dword ptr [ebp - 0ch], edi
        sub    dword ptr [ebp - 4], edi
        sub    edi, edi
loc_1_30FD:
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, esi
        mov    ebx, dword ptr [ebp - 4]
        cmp    ebx, eax
        jb     SHORT loc_1_310B
        mov    ebx, eax
loc_1_310B:
        push   ebx
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        add    esi, ebx
        push   eax
        call   sub_1_071C
        add    dword ptr [ebp - 10h], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_3121:
        cmp    esi, dword ptr [ebp + 0ch]
        jb     SHORT loc_1_30B4
loc_1_3126:
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], esi
loc_1_312B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    14h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 1ch], 0
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_32C1
        push   0
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0660
        cmp    dword ptr [ebp - 24h], 0
        je     NEAR PTR loc_1_32C1
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0684
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        call   sub_1_0660
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_1_32C1
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_0684
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_32B9
        mov    esi, dword ptr [ebp + 0ch]
        mov    edi, dword ptr [ebp + 18h]
loc_1_31D3:
        cmp    dword ptr [ebp - 4], 0
        jne    SHORT loc_1_3206
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_06A8
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_32B9
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0684
        jmp    NEAR PTR loc_1_32AD
loc_1_3206:
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_3236
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_06A8
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_32B9
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_0684
        jmp    SHORT loc_1_32AD
loc_1_3236:
        or     esi, esi
        je     SHORT loc_1_3258
        cmp    esi, dword ptr [ebp - 4]
        jbe    SHORT loc_1_3250
        sub    esi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 4], 0
        jmp    SHORT loc_1_32AD
        db  005h, 000h, 000h, 000h, 000h
loc_1_3250:
        add    dword ptr [ebp - 18h], esi
        sub    dword ptr [ebp - 4], esi
        sub    esi, esi
loc_1_3258:
        or     edi, edi
        je     SHORT loc_1_3278
        cmp    edi, dword ptr [ebp - 8]
        jbe    SHORT loc_1_3270
        sub    edi, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_1_32AD
        db  08Dh, 049h, 000h
loc_1_3270:
        add    dword ptr [ebp - 14h], edi
        sub    dword ptr [ebp - 8], edi
        sub    edi, edi
loc_1_3278:
        mov    ebx, dword ptr [ebp + 10h]
        sub    ebx, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    eax, ecx
        jb     SHORT loc_1_328A
        mov    eax, ecx
loc_1_328A:
        cmp    ebx, eax
        jb     SHORT loc_1_3290
        mov    ebx, eax
loc_1_3290:
        push   ebx
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_071C
        add    dword ptr [ebp - 18h], ebx
        add    dword ptr [ebp - 14h], ebx
        add    dword ptr [ebp - 1ch], ebx
        sub    dword ptr [ebp - 8], ebx
        sub    dword ptr [ebp - 4], ebx
loc_1_32AD:
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    dword ptr [ebp + 10h], ecx
        ja     NEAR PTR loc_1_31D3
loc_1_32B9:
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dword ptr [eax], ecx
loc_1_32C1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_32D0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx]
        mov    dl, byte ptr [eax + 48h]
        mov    eax, dword ptr [ecx + 4]
        and    dl, 24h
        mov    byte ptr [eax], dl
        mov    eax, dword ptr [ecx]
        pop    ebp
        and    byte ptr [eax + 48h], 0dbh
        mov    al, 1
        ret    4
        db  090h
loc_1_32F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi]
        cmp    dword ptr [eax + 379h], 1
        jne    SHORT loc_1_330D
        db  0A1h, 0C0h, 056h, 000h, 000h
        push   eax
        call   sub_1_0630
loc_1_330D:
        mov    eax, dword ptr [esi]
        mov    cl, byte ptr [eax + 48h]
        mov    eax, dword ptr [esi + 4]
        and    cl, 18h
        mov    byte ptr [eax], cl
        mov    eax, dword ptr [esi]
        pop    esi
        and    byte ptr [eax + 48h], 0e7h
        mov    al, 1
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3330:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    byte ptr [eax + 168h], 1
        mov    al, 1
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3350:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    byte ptr [eax + 48h], 0
        mov    al, 1
        ret    4
loc_1_3360:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0adh]
        mov    al, byte ptr [eax + 1e09h]
        cmp    al, 0ffh
        mov    byte ptr [ebp - 1], al
        jne    SHORT loc_1_33CF
        cmp    byte ptr [ecx + 16eh], 0
        jne    SHORT loc_1_33CF
        sub    eax, eax
        sub    edx, edx
        add    eax, 0
loc_1_3390:
        mov    esi, dword ptr [ecx + 0adh]
        mov    bl, byte ptr [edx + esi + 1f40h]
        mov    byte ptr [ebp - 2], bl
        and    ebx, 0fh
        shl    eax, 4
        add    eax, ebx
        add    edx, 2
        cmp    edx, 10h
        jb     SHORT loc_1_3390
        cmp    eax, 5049434fh
        je     SHORT loc_1_33C8
        mov    byte ptr [ecx + 175h], 1
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_33C8:
        mov    byte ptr [ecx + 175h], 0
loc_1_33CF:
        cmp    byte ptr [ecx + 2eh], 0
        jne    SHORT loc_1_33F3
        mov    eax, dword ptr [ecx + 18h]
        inc    eax
        mov    dword ptr [ecx + 18h], eax
        cmp    eax, 0ffh
        jb     SHORT loc_1_33F3
        mov    edx, dword ptr [ecx + 59h]
        inc    dx
        sub    al, al
        out    dx, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_33F3:
        mov    dl, byte ptr [ebp - 1]
        mov    eax, dword ptr [ecx + 0adh]
        not    dl
        mov    byte ptr [eax + 1e29h], dl
        cmp    byte ptr [ecx + 171h], 0
        je     SHORT loc_1_3420
        mov    al, byte ptr [ebp - 1]
        and    al, 0dfh
        or     byte ptr [ecx + 49h], al
        and    byte ptr [ebp - 1], 20h
        jmp    SHORT loc_1_342A
        db  005h, 000h, 000h, 000h, 000h
loc_1_3420:
        mov    al, byte ptr [ecx + 49h]
        or     byte ptr [ebp - 1], al
        mov    byte ptr [ecx + 49h], 0
loc_1_342A:
        mov    al, byte ptr [ebp - 1]
        or     byte ptr [ecx + 48h], al
        cmp    byte ptr [ecx + 168h], 0
        je     SHORT loc_1_3440
        mov    byte ptr [ecx + 169h], 1
loc_1_3440:
        cmp    byte ptr [ecx + 16eh], 0
        je     NEAR PTR loc_1_34E0
        mov    eax, dword ptr [ecx + 0adh]
        mov    al, byte ptr [eax + 1e02h]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ecx + 0adh]
        mov    al, byte ptr [eax + 1e03h]
        mov    byte ptr [ebp - 3], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 0ffff00ffh
        shl    ax, 8
        mov    dl, byte ptr [ebp - 3]
        and    edx, 0ffff00ffh
        add    ax, dx
        test   al, 1
        je     SHORT loc_1_34A1
        cmp    byte ptr [ecx + 38h], 0
        je     SHORT loc_1_3497
        mov    edx, dword ptr [ecx + 59h]
        add    dx, 3
        sub    al, al
        out    dx, al
loc_1_3497:
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_34A1:
        mov    word ptr [ecx + 0b5h], ax
        mov    edx, dword ptr [ecx + 0b1h]
        and    eax, 0ffffh
        mov    ax, word ptr [edx + eax + 6]
        or     ax, ax
        jne    SHORT loc_1_34C1
        mov    byte ptr [ecx + 2eh], 1
loc_1_34C1:
        cmp    byte ptr [ecx + 38h], 0
        je     SHORT loc_1_34D1
        mov    edx, dword ptr [ecx + 59h]
        add    dx, 3
        sub    al, al
        out    dx, al
loc_1_34D1:
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_34E0:
        cmp    byte ptr [ecx + 38h], 0
        je     SHORT loc_1_34F0
        mov    edx, dword ptr [ecx + 59h]
        add    dx, 3
        sub    al, al
        out    dx, al
loc_1_34F0:
        cmp    byte ptr [ebp - 1], 0
        jne    SHORT loc_1_3520
        mov    eax, dword ptr [ecx + 0adh]
        mov    al, byte ptr [eax + 1e08h]
        mov    byte ptr [ebp - 1], al
        and    byte ptr [ebp - 1], 1ch
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ecx + 4ah], al
        mov    dl, byte ptr [ebp - 1]
        mov    eax, dword ptr [ecx + 0adh]
        not    dl
        mov    byte ptr [eax + 1e28h], dl
loc_1_3520:
        cmp    byte ptr [ecx + 48h], 0
        je     SHORT loc_1_3530
        mov    al, 1
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_3530:
        sub    al, al
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_3540:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 18h], 0
        mov    byte ptr [esi + 2ch], 0
        cmp    byte ptr [esi + 4ah], 0
        je     SHORT loc_1_355C
        mov    byte ptr [esi + 4ah], 0
loc_1_355C:
        test   byte ptr [esi + 48h], 24h
        je     SHORT loc_1_356E
        cmp    byte ptr [esi + 4bh], 0
        jne    SHORT loc_1_356E
        push   esi
        call   sub_1_3590
loc_1_356E:
        test   byte ptr [esi + 48h], 18h
        je     SHORT loc_1_3580
        cmp    byte ptr [esi + 4ch], 0
        jne    SHORT loc_1_3580
        push   esi
        call   sub_1_3BE0
loc_1_3580:
        pop    esi
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3590:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 5dh]
        cmp    byte ptr [esi + 15fh], 0
        je     SHORT loc_1_35F0
        cmp    byte ptr [esi + 169h], 0
        je     SHORT loc_1_35F0
        push   esi
        call   sub_1_44F0
        mov    eax, dword ptr [esi + 5dh]
        dec    eax
        mov    dword ptr [esi + 5dh], eax
        jne    NEAR PTR loc_1_3BD2
        cmp    byte ptr [esi + 15fh], 0
        jne    SHORT loc_1_35DB
        mov    eax, dword ptr [esi + 69h]
        sub    eax, esi
        cmp    eax, 69h
        je     NEAR PTR loc_1_3BD2
loc_1_35DB:
        push   esi
        call   sub_1_5080
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_35F0:
        mov    byte ptr [esi + 4bh], 1
        cmp    dword ptr [esi + 379h], 1
        je     SHORT loc_1_3617
        mov    dword ptr [ebp - 24h], esi
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 20h], eax
        lea    eax, [ebp - 24h]
        push   eax
        db  068h, 0D0h, 032h, 000h, 000h
        push   esi
        call   sub_1_062A
        jmp    SHORT loc_1_3652
loc_1_3617:
        mov    eax, dword ptr [esi + 0adh]
        mov    al, byte ptr [eax + 1e08h]
        mov    byte ptr [ebp - 8], al
        mov    cl, al
        and    cl, 3fh
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e08h], cl
        cli
        mov    al, byte ptr [esi + 48h]
        and    al, 24h
        mov    byte ptr [ebp - 4], al
        and    byte ptr [esi + 48h], 0dbh
        sti
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e08h], 0c0h
loc_1_3652:
        test   byte ptr [ebp - 4], 24h
        je     NEAR PTR loc_1_3BAC
        lea    esp, [esp]
loc_1_3660:
        cmp    byte ptr [esi + 174h], 0
        je     SHORT loc_1_3676
        cmp    byte ptr [esi + 176h], 0
        je     NEAR PTR loc_1_3BAC
loc_1_3676:
        test   byte ptr [ebp - 4], 20h
        je     NEAR PTR loc_1_3A01
        cmp    byte ptr [esi + 170h], 0
        je     NEAR PTR loc_1_3926
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0b5h]
        add    ecx, dword ptr [esi + 0b1h]
        mov    al, byte ptr [ecx + 2]
        or     al, al
        jne    NEAR PTR loc_1_3886
        mov    ax, word ptr [ecx + 0ah]
        cmp    byte ptr [esi + 3ah], 0
        mov    word ptr [ebp - 0ch], ax
        je     NEAR PTR loc_1_3782
        mov    ah, al
        mov    al, byte ptr [ebp - 0bh]
        mov    word ptr [ebp - 0ch], ax
        and    eax, 3fffh
        mov    edx, dword ptr [esi + 0b1h]
        add    edx, eax
        mov    dword ptr [esi + 0b7h], edx
        mov    ax, word ptr [ebp - 0ch]
        shr    ax, 0eh
        mov    byte ptr [esi + 0c7h], al
        mov    ax, word ptr [ecx + 0eh]
        mov    word ptr [ebp - 0ch], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 0bh]
        mov    word ptr [ebp - 0ch], ax
        and    eax, 3fffh
        mov    edx, dword ptr [esi + 0b1h]
        add    edx, eax
        mov    dword ptr [esi + 0bbh], edx
        mov    ax, word ptr [ebp - 0ch]
        shr    ax, 0eh
        mov    byte ptr [esi + 0c8h], al
        mov    ax, word ptr [ecx + 0ch]
        mov    word ptr [ebp - 0ch], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 0bh]
        mov    word ptr [ebp - 0ch], ax
        lea    edx, [esi + 0bfh]
        and    eax, 3fffh
        mov    edi, dword ptr [esi + 0b1h]
        add    edi, eax
        mov    dword ptr [edx], edi
        mov    ax, word ptr [ebp - 0ch]
        shr    ax, 0eh
        mov    byte ptr [esi + 0c9h], al
        mov    ax, word ptr [ecx + 8]
        mov    word ptr [ebp - 0ch], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 0bh]
        mov    word ptr [ebp - 0ch], ax
        lea    edi, [esi + 0c3h]
        and    eax, 3fffh
        mov    ebx, dword ptr [esi + 0b1h]
        add    ebx, eax
        mov    dword ptr [edi], ebx
        mov    ax, word ptr [ebp - 0ch]
        shr    ax, 0eh
        mov    byte ptr [esi + 0cah], al
        jmp    NEAR PTR loc_1_3804
loc_1_3782:
        lea    eax, [esi + 0b7h]
        mov    ah, byte ptr [ebp - 0ch]
        mov    al, byte ptr [ebp - 0bh]
        and    eax, 0ffffh
        add    eax, dword ptr [esi + 0b1h]
        mov    dword ptr [esi + 0b7h], eax
        mov    ax, word ptr [ecx + 0eh]
        mov    word ptr [ebp - 0ch], ax
        lea    eax, [esi + 0bbh]
        mov    ah, byte ptr [ebp - 0ch]
        mov    al, byte ptr [ebp - 0bh]
        and    eax, 0ffffh
        add    eax, dword ptr [esi + 0b1h]
        mov    dword ptr [esi + 0bbh], eax
        mov    ax, word ptr [ecx + 0ch]
        mov    word ptr [ebp - 0ch], ax
        lea    edx, [esi + 0bfh]
        mov    ah, al
        mov    al, byte ptr [ebp - 0bh]
        and    eax, 0ffffh
        add    eax, dword ptr [esi + 0b1h]
        mov    dword ptr [edx], eax
        mov    ax, word ptr [ecx + 8]
        mov    word ptr [ebp - 0ch], ax
        lea    edi, [esi + 0c3h]
        mov    ah, al
        mov    al, byte ptr [ebp - 0bh]
        and    eax, 0ffffh
        add    eax, dword ptr [esi + 0b1h]
        mov    dword ptr [edi], eax
loc_1_3804:
        mov    eax, dword ptr [esi + 3fh]
        add    eax, dword ptr [esi + 0b1h]
        cmp    dword ptr [esi + 0b7h], eax
        ja     SHORT loc_1_3886
        cmp    dword ptr [esi + 0bbh], eax
        ja     SHORT loc_1_3886
        cmp    dword ptr [edx], eax
        ja     SHORT loc_1_3886
        cmp    dword ptr [edi], eax
        ja     SHORT loc_1_3886
        mov    dword ptr [esi + 15bh], 1
        mov    byte ptr [esi + 170h], 0
        mov    word ptr [esi + 172h], 0
        mov    byte ptr [esi + 16fh], 0
        mov    eax, esi
        mov    byte ptr [esi + 171h], 0
        mov    edi, dword ptr [esi + 61h]
        sub    eax, edi
        cmp    eax, -61h
        je     SHORT loc_1_387B
loc_1_3859:
        cmp    byte ptr [edi + 1ch], 0
        je     SHORT loc_1_3870
        mov    byte ptr [edi + 1ch], 0
        push   0
        push   0
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_056E
loc_1_3870:
        mov    edi, dword ptr [edi]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -61h
        jne    SHORT loc_1_3859
loc_1_387B:
        push   esi
        call   sub_1_4930
        jmp    NEAR PTR loc_1_3A01
loc_1_3886:
        mov    dword ptr [esi + 15bh], 5
        mov    byte ptr [esi + 170h], 0
        mov    ax, word ptr [ecx + 6]
        mov    word ptr [esi + 172h], ax
        mov    ch, al
        mov    cl, ah
        mov    word ptr [esi + 172h], cx
        mov    byte ptr [esi + 16fh], 1
        mov    byte ptr [esi + 171h], 1
        mov    edi, dword ptr [esi + 61h]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -61h
        je     NEAR PTR loc_1_3A01
        add    eax, 0
loc_1_38D0:
        mov    ebx, edi
        cmp    byte ptr [edi + 1ch], 0
        je     SHORT loc_1_3918
        mov    byte ptr [edi + 1ch], 0
        xor    eax, eax
        mov    ax, word ptr [esi + 172h]
        or     eax, 0c0011000h
        push   eax
        push   0c0010007h
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_056E
        mov    eax, dword ptr [ebx + 4]
        mov    edi, dword ptr [edi]
        mov    dword ptr [eax], edi
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        push   96h
        push   ebx
        call   sub_1_063C
        dec    dword ptr [esi + 5dh]
loc_1_3918:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -61h
        jne    SHORT loc_1_38D0
        jmp    NEAR PTR loc_1_3A01
loc_1_3926:
        cmp    dword ptr [esi + 0ceh], 0
        je     SHORT loc_1_3948
        lea    eax, [ebp - 9]
        push   eax
        push   esi
        call   sub_1_4810
        cmp    byte ptr [ebp - 9], 0
        je     NEAR PTR loc_1_3A01
        jmp    NEAR PTR loc_1_39FB
loc_1_3948:
        cmp    byte ptr [esi + 0cbh], 0
        jne    NEAR PTR loc_1_3A01
        mov    eax, dword ptr [esi + 0b7h]
        mov    cl, byte ptr [eax + 2]
        mov    byte ptr [ebp - 7], cl
        mov    al, byte ptr [eax]
        or     al, al
        mov    byte ptr [ebp - 2], al
        je     NEAR PTR loc_1_39FB
        cmp    al, 8
        je     SHORT loc_1_39A7
        cmp    al, 7
        je     SHORT loc_1_39A7
        cmp    al, 6
        je     SHORT loc_1_39A7
        cmp    al, 1eh
        je     SHORT loc_1_39A7
        and    eax, 0ffh
        push   eax
        push   8
        push   4
        push   3
        push   0aa00000bh
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 18h
        mov    byte ptr [esi + 175h], 1
        jmp    SHORT loc_1_3A01
loc_1_39A7:
        mov    eax, dword ptr [esi + 91h]
        cmp    dword ptr [eax + 10h], 8
        jne    SHORT loc_1_39E8
        cmp    byte ptr [ebp - 7], 0
        jne    SHORT loc_1_39CC
        cmp    dword ptr [eax + 4], 0
        push   esi
        je     SHORT loc_1_39C7
        call   sub_1_4A80
        jmp    SHORT loc_1_39CC
loc_1_39C7:
        call   sub_1_4A00
loc_1_39CC:
        push   0
        push   14h
        mov    eax, dword ptr [esi + 91h]
        push   eax
        call   sub_1_063C
        mov    dword ptr [esi + 91h], 0
        jmp    SHORT loc_1_39FB
loc_1_39E8:
        cmp    byte ptr [ebp - 7], 1
        sbb    al, al
        neg    al
        push   eax
        push   esi
        call   sub_1_4D60
        or     al, al
        je     SHORT loc_1_3A01
loc_1_39FB:
        push   esi
        call   sub_1_48C0
loc_1_3A01:
        test   byte ptr [ebp - 4], 4
        je     NEAR PTR loc_1_3B44
        mov    ecx, dword ptr [esi + 0bbh]
        mov    al, byte ptr [ecx]
        mov    byte ptr [ebp - 2], al
        cmp    al, 0ah
        jne    NEAR PTR loc_1_3B10
        xor    eax, eax
        mov    al, byte ptr [esi + 0cdh]
        add    eax, 7fh
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 7fh
        xor    eax, edx
        sub    eax, edx
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ecx + 1]
        mov    byte ptr [ebp - 3], al
        mov    al, byte ptr [ecx + 2]
        or     al, al
        mov    byte ptr [ebp - 6], al
        jne    SHORT loc_1_3A50
        sub    edi, edi
        jmp    SHORT loc_1_3A85
        db  08Dh, 064h, 024h, 000h
loc_1_3A50:
        cmp    byte ptr [ebp - 6], 22h
        jne    SHORT loc_1_3A80
        mov    al, byte ptr [ecx + 6]
        mov    byte ptr [ebp - 5], al
        shr    al, 6
        mov    cl, byte ptr [ebp - 5]
        and    cl, 0ch
        shr    cl, 2
        cmp    cl, al
        jne    SHORT loc_1_3A74
        or     al, al
        jne    SHORT loc_1_3A74
        sub    edi, edi
        jmp    SHORT loc_1_3A85
loc_1_3A74:
        mov    edi, 10001h
        jmp    SHORT loc_1_3A85
        db  005h, 000h, 000h, 000h, 000h
loc_1_3A80:
        mov    edi, 0c0000001h
loc_1_3A85:
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        inc    eax
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 7fh
        xor    eax, edx
        sub    eax, edx
        mov    byte ptr [ebp - 1], al
        and    eax, 0ffh
        mov    ecx, dword ptr [esi + eax*4 + 179h]
        or     ecx, ecx
        je     SHORT loc_1_3AE7
        mov    byte ptr [ecx + 30h], 0
        xor    eax, eax
        mov    al, byte ptr [ecx + 31h]
        mov    dword ptr [esi + eax*4 + 179h], 0
        mov    ebx, dword ptr [ecx + 28h]
        or     edi, edi
        jne    SHORT loc_1_3AD0
        inc    dword ptr [esi + 0f7h]
        jmp    SHORT loc_1_3AD6
        db  08Dh, 049h, 000h
loc_1_3AD0:
        inc    dword ptr [esi + 0ffh]
loc_1_3AD6:
        push   edi
        mov    eax, dword ptr [ecx + 2ch]
        push   eax
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_057A
        dec    dword ptr [ebx + 14h]
loc_1_3AE7:
        mov    al, byte ptr [ebp - 1]
        cmp    al, byte ptr [ebp - 3]
        jne    SHORT loc_1_3A85
        and    eax, 0ffh
        inc    eax
        cdq
        xor    eax, edx
        sub    eax, edx
        and    eax, 7fh
        xor    eax, edx
        sub    eax, edx
        mov    byte ptr [esi + 0cdh], al
        push   esi
        call   sub_1_48C0
        jmp    SHORT loc_1_3B37
        db  090h
loc_1_3B10:
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        push   eax
        push   8
        push   4
        push   3
        push   0aa00000bh
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 18h
        mov    byte ptr [esi + 175h], 1
loc_1_3B37:
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e4bh], 1
loc_1_3B44:
        cmp    dword ptr [esi + 379h], 1
        je     SHORT loc_1_3B67
        mov    dword ptr [ebp - 1ch], esi
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 18h], eax
        lea    eax, [ebp - 1ch]
        push   eax
        db  068h, 0D0h, 032h, 000h, 000h
        push   esi
        call   sub_1_062A
        jmp    SHORT loc_1_3BA2
loc_1_3B67:
        mov    eax, dword ptr [esi + 0adh]
        mov    al, byte ptr [eax + 1e08h]
        mov    byte ptr [ebp - 8], al
        mov    cl, al
        and    cl, 3fh
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e08h], cl
        cli
        mov    al, byte ptr [esi + 48h]
        and    al, 24h
        mov    byte ptr [ebp - 4], al
        and    byte ptr [esi + 48h], 0dbh
        sti
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e08h], 0c0h
loc_1_3BA2:
        test   byte ptr [ebp - 4], 24h
        jne    NEAR PTR loc_1_3660
loc_1_3BAC:
        mov    byte ptr [esi + 4bh], 0
        mov    eax, dword ptr [esi + 5dh]
        dec    eax
        mov    dword ptr [esi + 5dh], eax
        jne    SHORT loc_1_3BD2
        cmp    byte ptr [esi + 15fh], 0
        jne    SHORT loc_1_3BCC
        mov    eax, dword ptr [esi + 69h]
        sub    eax, esi
        cmp    eax, 69h
        je     SHORT loc_1_3BD2
loc_1_3BCC:
        push   esi
        call   sub_1_5080
loc_1_3BD2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_3BE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 3ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        inc    dword ptr [esi + 5dh]
        mov    byte ptr [esi + 4ch], 1
        cmp    dword ptr [esi + 379h], 1
        je     SHORT loc_1_3C16
        mov    dword ptr [ebp - 3ch], esi
        lea    eax, [ebp - 2]
        mov    dword ptr [ebp - 38h], eax
        lea    eax, [ebp - 3ch]
        push   eax
        db  068h, 0F0h, 032h, 000h, 000h
        push   esi
        call   sub_1_062A
        jmp    SHORT loc_1_3C51
loc_1_3C16:
        mov    eax, dword ptr [esi + 0adh]
        mov    al, byte ptr [eax + 1e08h]
        mov    byte ptr [ebp - 5], al
        mov    cl, al
        and    cl, 3fh
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e08h], cl
        cli
        mov    al, byte ptr [esi + 48h]
        and    al, 18h
        mov    byte ptr [ebp - 2], al
        and    byte ptr [esi + 48h], 0e7h
        sti
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e08h], 0c0h
loc_1_3C51:
        test   byte ptr [ebp - 2], 18h
        je     NEAR PTR loc_1_43B5
loc_1_3C5B:
        test   byte ptr [ebp - 2], 8
        je     NEAR PTR loc_1_4213
        mov    eax, dword ptr [esi + 0bfh]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 1], al
        and    eax, 0ffh
        cmp    eax, 81h
        je     SHORT loc_1_3CB0
        cmp    eax, 82h
        je     NEAR PTR loc_1_3E40
        cmp    eax, 83h
        je     NEAR PTR loc_1_3F60
        cmp    eax, 84h
        je     NEAR PTR loc_1_4020
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e4bh], 2
        jmp    NEAR PTR loc_1_4213
        db  090h
loc_1_3CB0:
        cmp    byte ptr [esi + 174h], 0
        je     SHORT loc_1_3CC6
        cmp    byte ptr [esi + 176h], 0
        je     NEAR PTR loc_1_4213
loc_1_3CC6:
        mov    edx, dword ptr [esi + 0bfh]
        mov    ax, word ptr [edx + 6]
        mov    word ptr [ebp - 0ah], ax
        mov    word ptr [esi + 0d6h], ax
        mov    ah, byte ptr [ebp - 0ah]
        mov    al, byte ptr [ebp - 9]
        mov    ecx, eax
        and    ecx, 0ffffh
        mov    eax, dword ptr [esi + 0b1h]
        lea    ecx, [ecx + eax + 2]
        mov    ax, word ptr [edx + 0ah]
        mov    word ptr [ebp - 20h], ax
        mov    al, ah
        mov    ah, byte ptr [ebp - 20h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 20h], eax
        mov    ax, word ptr [ecx + 4]
        mov    word ptr [ebp - 14h], ax
        mov    al, ah
        mov    ah, byte ptr [ebp - 14h]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [esi + 0adh]
        lea    edi, [ecx + 6]
        mov    byte ptr [eax + 1e4bh], 2
        mov    al, byte ptr [edi + 8]
        mov    byte ptr [ebp - 1], al
        test   al, 80h
        je     SHORT loc_1_3D47
        mov    al, byte ptr [edi + 0eh]
        mov    byte ptr [ebp - 1], al
        mov    dl, al
        and    edx, 1fh
        add    edx, 0eh
        jmp    SHORT loc_1_3D4C
loc_1_3D47:
        mov    edx, 0eh
loc_1_3D4C:
        mov    word ptr [esi + 0d8h], dx
        inc    dword ptr [esi + 0fbh]
        cmp    edx, dword ptr [ebp - 14h]
        jbe    SHORT loc_1_3D74
        cmp    dword ptr [ebp - 14h], 6
        jb     SHORT loc_1_3D97
        or     edi, edi
        je     SHORT loc_1_3D97
        push   0
        push   0
        push   0
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        jmp    SHORT loc_1_3D89
loc_1_3D74:
        or     edi, edi
        je     SHORT loc_1_3D97
        mov    eax, dword ptr [ebp - 20h]
        sub    eax, edx
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        sub    eax, edx
        push   eax
        lea    eax, [edi + edx]
        push   eax
        push   edx
loc_1_3D89:
        push   edi
        push   ecx
        mov    eax, dword ptr [esi + 85h]
        push   eax
        call   sub_1_00B0
loc_1_3D97:
        mov    eax, dword ptr [esi + 85h]
        push   eax
        call   sub_1_0300
        cmp    byte ptr [esi + 0cch], 0
        je     SHORT loc_1_3DD7
        mov    byte ptr [esi + 0cch], 0
        mov    byte ptr [esi + 0dah], 0
        mov    ax, word ptr [ebp - 0ah]
        push   eax
        push   esi
        call   sub_1_5010
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e4bh], 10h
        jmp    NEAR PTR loc_1_4213
loc_1_3DD7:
        mov    ax, word ptr [esi + 0d4h]
        cmp    ax, 0ffffh
        jne    SHORT loc_1_3DF1
        mov    ax, word ptr [ebp - 0ah]
        mov    word ptr [esi + 0d2h], ax
        jmp    SHORT loc_1_3E0A
loc_1_3DF1:
        mov    ch, al
        mov    cl, ah
        mov    edx, dword ptr [esi + 0b1h]
        mov    eax, ecx
        and    eax, 0ffffh
        mov    cx, word ptr [ebp - 0ah]
        mov    word ptr [eax + edx], cx
loc_1_3E0A:
        mov    ax, word ptr [ebp - 0ah]
        cmp    byte ptr [esi + 178h], 0
        mov    word ptr [esi + 0d4h], ax
        jne    NEAR PTR loc_1_4213
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [esi + 178h], 1
        mov    byte ptr [eax + 1e4bh], 4
        jmp    NEAR PTR loc_1_4213
        db  005h, 000h, 000h, 000h, 000h
loc_1_3E40:
        cmp    byte ptr [esi + 174h], 0
        je     SHORT loc_1_3E56
        cmp    byte ptr [esi + 176h], 0
        je     NEAR PTR loc_1_4213
loc_1_3E56:
        mov    ebx, dword ptr [esi + 0bfh]
        mov    byte ptr [ebp - 4], 0
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [ebp - 1], al
        and    eax, 0ffh
        mov    edi, dword ptr [esi + eax*4 + 179h]
        or     edi, edi
        je     SHORT loc_1_3E78
        jmp    SHORT loc_1_3E96
loc_1_3E78:
        lea    eax, [ebp - 6]
        push   eax
        push   esi
        call   sub_1_4810
        mov    edi, eax
        or     edi, edi
        je     NEAR PTR loc_1_4213
        cmp    byte ptr [ebp - 6], 0
        je     NEAR PTR loc_1_4213
loc_1_3E96:
        mov    ax, word ptr [ebx + 6]
        mov    word ptr [ebp - 0ch], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 0bh]
        mov    ebx, eax
        and    ebx, 0ffffh
        mov    eax, dword ptr [esi + 0adh]
        add    ebx, dword ptr [esi + 0b1h]
        mov    byte ptr [eax + 1e4bh], 2
        lea    eax, [ebp - 24h]
        push   eax
        push   0
        push   0
        push   0
        push   edi
        call   sub_1_0660
        lea    eax, [ebp - 2ch]
        push   eax
        push   ebx
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        push   0
        push   edi
        call   sub_1_2F80
        cmp    byte ptr [esi + 0cch], 0
        je     SHORT loc_1_3F0C
        mov    byte ptr [esi + 0cch], 0
        mov    byte ptr [esi + 0dah], 1
        push   edi
        push   esi
        call   sub_1_4990
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e4bh], 10h
        jmp    SHORT loc_1_3F30
loc_1_3F0C:
        push   edi
        push   esi
        call   sub_1_5040
        cmp    byte ptr [esi + 178h], 0
        jne    SHORT loc_1_3F30
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [esi + 178h], 1
        mov    byte ptr [eax + 1e4bh], 4
loc_1_3F30:
        cmp    byte ptr [ebp - 4], 0
        je     NEAR PTR loc_1_4213
        cmp    byte ptr [esi + 0cbh], 0
        je     NEAR PTR loc_1_4213
        mov    byte ptr [esi + 0cbh], 0
        push   esi
        call   sub_1_48C0
        jmp    NEAR PTR loc_1_4213
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3F60:
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e4bh], 2
        mov    eax, dword ptr [esi + 0bfh]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 0ch], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 0bh]
        test   al, 40h
        je     NEAR PTR loc_1_4213
        db  0A1h, 0C8h, 056h, 000h, 000h
        push   eax
        push   0
        push   14h
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_1_0636
        or     eax, eax
        je     SHORT loc_1_3FC0
        push   5
        push   4
        push   2
        push   0aa000002h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 14h
        jmp    NEAR PTR loc_1_4213
        db  08Bh, 0FFh
loc_1_3FC0:
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 10h], 8
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [eax + 4], 0
        mov    eax, dword ptr [esi + 89h]
        or     eax, eax
        jne    SHORT loc_1_3FF2
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [esi + 8dh], eax
        jmp    SHORT loc_1_3FFA
loc_1_3FF2:
        mov    ecx, dword ptr [ebp - 1ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 1ch]
loc_1_3FFA:
        mov    dword ptr [esi + 89h], eax
        cmp    byte ptr [esi + 0cbh], 0
        je     NEAR PTR loc_1_4213
        mov    byte ptr [esi + 0cbh], 0
        push   esi
        call   sub_1_48C0
        jmp    NEAR PTR loc_1_4213
        db  090h
loc_1_4020:
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [esi + 0bfh]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 8], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 7]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e4bh], 2
        test   byte ptr [ebp - 8], 80h
        je     NEAR PTR loc_1_40E6
        db  0A1h, 0C8h, 056h, 000h, 000h
        push   eax
        push   0
        push   14h
        lea    eax, [ebp - 18h]
        push   eax
        call   sub_1_0636
        or     eax, eax
        je     SHORT loc_1_4090
        push   5
        push   4
        push   2
        push   0aa000002h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 14h
        jmp    NEAR PTR loc_1_4213
        db  005h, 000h, 000h, 000h, 000h
loc_1_4090:
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 10h], 8
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [esi + 89h]
        or     eax, eax
        jne    SHORT loc_1_40C2
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [esi + 8dh], eax
        jmp    SHORT loc_1_40CA
loc_1_40C2:
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 18h]
loc_1_40CA:
        mov    dword ptr [esi + 89h], eax
        cmp    byte ptr [esi + 0cbh], 0
        je     SHORT loc_1_40E6
        mov    byte ptr [esi + 0cbh], 0
        push   esi
        call   sub_1_48C0
loc_1_40E6:
        test   byte ptr [ebp - 8], 20h
        je     SHORT loc_1_40F0
        or     dword ptr [ebp - 10h], 40h
loc_1_40F0:
        test   byte ptr [ebp - 8], 40h
        je     SHORT loc_1_40FD
        or     dword ptr [ebp - 10h], 80h
loc_1_40FD:
        test   byte ptr [ebp - 8], 80h
        je     SHORT loc_1_410A
        or     dword ptr [ebp - 10h], 100h
loc_1_410A:
        test   byte ptr [ebp - 7], 1
        je     SHORT loc_1_4117
        or     dword ptr [ebp - 10h], 200h
loc_1_4117:
        test   byte ptr [ebp - 7], 4
        je     SHORT loc_1_4124
        or     dword ptr [ebp - 10h], 400h
loc_1_4124:
        test   byte ptr [ebp - 7], 8
        je     SHORT loc_1_4131
        or     dword ptr [ebp - 10h], 800h
loc_1_4131:
        test   byte ptr [ebp - 7], 10h
        je     SHORT loc_1_413E
        or     dword ptr [ebp - 10h], 1000h
loc_1_413E:
        test   byte ptr [ebp - 7], 20h
        je     SHORT loc_1_414B
        or     dword ptr [ebp - 10h], 2000h
loc_1_414B:
        test   byte ptr [ebp - 7], 40h
        je     SHORT loc_1_4158
        or     dword ptr [ebp - 10h], 4000h
loc_1_4158:
        test   byte ptr [ebp - 7], 80h
        je     SHORT loc_1_4165
        or     dword ptr [ebp - 10h], 8000h
loc_1_4165:
        cmp    dword ptr [ebp - 10h], 0
        je     SHORT loc_1_41B0
        mov    edi, dword ptr [esi + 61h]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -61h
        je     SHORT loc_1_41A7
loc_1_4177:
        inc    dword ptr [edi + 14h]
        lea    ebx, [edi + 14h]
        push   4
        lea    eax, [ebp - 10h]
        push   eax
        push   40010006h
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_05A4
        mov    edi, dword ptr [edi]
        dec    dword ptr [ebx]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -61h
        jne    SHORT loc_1_4177
loc_1_41A7:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [esi + 157h], eax
loc_1_41B0:
        mov    ax, word ptr [ebp - 8]
        and    ah, 50h
        cmp    ah, 50h
        jne    SHORT loc_1_41E0
        mov    byte ptr [esi + 174h], 1
        push   0c0010008h
        push   esi
        call   sub_1_5220
        test   byte ptr [ebp - 7], 8
        je     SHORT loc_1_4213
        mov    byte ptr [esi + 177h], 1
        jmp    SHORT loc_1_4213
        db  08Dh, 049h, 000h
loc_1_41E0:
        test   byte ptr [ebp - 8], 20h
        je     SHORT loc_1_4213
        test   byte ptr [ebp - 7], 40h
        jne    SHORT loc_1_4213
        cmp    byte ptr [esi + 174h], 0
        je     SHORT loc_1_4213
        cmp    byte ptr [esi + 176h], 0
        jne    SHORT loc_1_4213
        mov    byte ptr [esi + 176h], 1
        push   0
        push   esi
        call   sub_1_1EF0
        push   esi
        call   sub_1_5080
loc_1_4213:
        test   byte ptr [ebp - 2], 10h
        je     NEAR PTR loc_1_4347
        mov    byte ptr [ebp - 3], 0
        sub    bl, bl
        cmp    byte ptr [esi + 174h], bl
        je     SHORT loc_1_4237
        cmp    byte ptr [esi + 176h], bl
        je     NEAR PTR loc_1_43B5
loc_1_4237:
        mov    cx, word ptr [esi + 0d2h]
        cmp    cx, 0ffffh
        je     SHORT loc_1_4249
        mov    byte ptr [ebp - 3], 1
loc_1_4249:
        mov    edx, dword ptr [esi + 0efh]
        or     edx, edx
        je     SHORT loc_1_4255
        mov    bl, 1
loc_1_4255:
        cmp    byte ptr [ebp - 3], 0
        je     NEAR PTR loc_1_42E6
        or     bl, bl
        je     SHORT loc_1_426C
        cmp    byte ptr [esi + 0dah], 0
        je     SHORT loc_1_42E6
loc_1_426C:
        cmp    word ptr [esi + 0d4h], cx
        jne    SHORT loc_1_4289
        mov    word ptr [esi + 0d2h], 0ffffh
        mov    word ptr [esi + 0d4h], 0ffffh
        jmp    SHORT loc_1_42A3
loc_1_4289:
        mov    ah, cl
        mov    al, ch
        and    eax, 0ffffh
        mov    edx, dword ptr [esi + 0b1h]
        mov    ax, word ptr [eax + edx]
        mov    word ptr [esi + 0d2h], ax
loc_1_42A3:
        mov    byte ptr [esi + 0cch], 0
        mov    byte ptr [esi + 0dah], 0
        push   ecx
        push   esi
        call   sub_1_5010
        or     bl, bl
        je     SHORT loc_1_42D0
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e4bh], 14h
        jmp    SHORT loc_1_4347
        db  005h, 000h, 000h, 000h, 000h
loc_1_42D0:
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [esi + 178h], 0
        mov    byte ptr [eax + 1e4bh], 10h
        jmp    SHORT loc_1_4347
loc_1_42E6:
        or     bl, bl
        je     SHORT loc_1_4340
        mov    eax, dword ptr [edx + 24h]
        mov    dword ptr [esi + 0efh], eax
        mov    byte ptr [esi + 0cch], 0
        mov    byte ptr [esi + 0dah], 1
        push   edx
        push   esi
        call   sub_1_4990
        cmp    byte ptr [ebp - 3], 0
        jne    SHORT loc_1_4330
        cmp    dword ptr [esi + 0efh], 0
        jne    SHORT loc_1_4330
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [esi + 178h], 0
        mov    byte ptr [eax + 1e4bh], 10h
        jmp    SHORT loc_1_4347
        db  08Dh, 049h, 000h
loc_1_4330:
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e4bh], 14h
        jmp    SHORT loc_1_4347
        db  090h
loc_1_4340:
        mov    byte ptr [esi + 0cch], 1
loc_1_4347:
        cmp    dword ptr [esi + 379h], 1
        je     SHORT loc_1_4370
        mov    dword ptr [ebp - 34h], esi
        lea    eax, [ebp - 2]
        mov    dword ptr [ebp - 30h], eax
        lea    eax, [ebp - 34h]
        push   eax
        db  068h, 0F0h, 032h, 000h, 000h
        push   esi
        call   sub_1_062A
        jmp    SHORT loc_1_43AB
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_4370:
        mov    eax, dword ptr [esi + 0adh]
        mov    al, byte ptr [eax + 1e08h]
        mov    byte ptr [ebp - 5], al
        mov    cl, al
        and    cl, 3fh
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e08h], cl
        cli
        mov    al, byte ptr [esi + 48h]
        and    al, 18h
        mov    byte ptr [ebp - 2], al
        and    byte ptr [esi + 48h], 0e7h
        sti
        mov    eax, dword ptr [esi + 0adh]
        mov    byte ptr [eax + 1e08h], 0c0h
loc_1_43AB:
        test   byte ptr [ebp - 2], 18h
        jne    NEAR PTR loc_1_3C5B
loc_1_43B5:
        mov    byte ptr [esi + 4ch], 0
        mov    eax, dword ptr [esi + 5dh]
        dec    eax
        mov    dword ptr [esi + 5dh], eax
        jne    SHORT loc_1_43DB
        cmp    byte ptr [esi + 15fh], 0
        jne    SHORT loc_1_43D5
        mov    eax, dword ptr [esi + 69h]
        sub    eax, esi
        cmp    eax, 69h
        je     SHORT loc_1_43DB
loc_1_43D5:
        push   esi
        call   sub_1_5080
loc_1_43DB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_43F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 176h], 0
        jne    SHORT loc_1_4483
        mov    dword ptr [esi + 15bh], 6
        mov    edi, dword ptr [esi + 61h]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -61h
        je     SHORT loc_1_445F
        add    eax, 0
loc_1_4420:
        inc    dword ptr [edi + 14h]
        lea    ebx, [edi + 14h]
        cmp    dword ptr [ebp + 0ch], 0
        je     SHORT loc_1_4434
        push   4
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        jmp    SHORT loc_1_443B
loc_1_4434:
        lea    eax, [edi + 10h]
        push   0
        push   0
loc_1_443B:
        push   40010007h
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [edi + 10h]
        push   eax
        call   sub_1_05A4
        mov    edi, dword ptr [edi]
        dec    dword ptr [ebx]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -61h
        jne    SHORT loc_1_4420
loc_1_445F:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        push   7
        push   3
        push   4
        push   0aa000003h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 1ch
        jmp    SHORT loc_1_4491
loc_1_4483:
        mov    byte ptr [esi + 177h], 1
        mov    byte ptr [esi + 176h], 0
loc_1_4491:
        mov    dword ptr [esi + 160h], 4
        push   esi
        call   sub_1_0A50
        mov    byte ptr [esi + 170h], 0
        mov    byte ptr [esi + 171h], 1
        mov    byte ptr [esi + 15fh], 0
        mov    byte ptr [esi + 175h], 0
        mov    byte ptr [esi + 168h], 0
        mov    byte ptr [esi + 169h], 0
        mov    eax, dword ptr [esi + 16ah]
        or     eax, eax
        je     SHORT loc_1_44E6
        dec    dword ptr [eax + 14h]
        push   0c0000001h
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_0586
loc_1_44E6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  090h
sub_1_44F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 160h]
        cmp    eax, 1
        je     SHORT loc_1_4530
        cmp    eax, 2
        je     NEAR PTR loc_1_4628
        cmp    eax, 3
        je     NEAR PTR loc_1_4768
        cmp    eax, 4
        je     NEAR PTR loc_1_47D0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
loc_1_4530:
        mov    eax, dword ptr [esi + 0adh]
        mov    al, byte ptr [eax + 1e02h]
        mov    byte ptr [ebp - 5], al
        mov    eax, dword ptr [esi + 0adh]
        mov    al, byte ptr [eax + 1e03h]
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 5]
        and    eax, 0ffff00ffh
        shl    ax, 8
        mov    cl, byte ptr [ebp - 4]
        and    ecx, 0ffff00ffh
        add    ax, cx
        mov    word ptr [esi + 0b5h], ax
        and    eax, 0ffffh
        mov    ecx, dword ptr [esi + 0b1h]
        mov    ax, word ptr [eax + ecx + 6]
        mov    word ptr [ebp - 8], ax
        or     ax, ax
        je     SHORT loc_1_45A0
        push   1
        and    eax, 0ffffh
        push   eax
        push   0
        push   esi
        call   sub_1_43F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_45A0:
        xor    ebx, ebx
        mov    bx, word ptr [esi + 0b5h]
        add    ebx, dword ptr [esi + 0b1h]
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 0fh
        rep stosd
        mov    byte ptr [ebx], 3
        mov    word ptr [ebx + 8], 1
        push   6
        lea    eax, [esi + 4dh]
        push   eax
        lea    eax, [ebx + 0ah]
        push   eax
        call   sub_1_071C
        mov    ax, word ptr [esi + 0e3h]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 18h], cx
        mov    ax, word ptr [esi + 0e5h]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 1ah], cx
        mov    ax, word ptr [esi + 0ddh]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx + 1ch], cx
        mov    al, byte ptr [esi + 0dbh]
        mov    byte ptr [ebx + 1eh], al
        mov    eax, dword ptr [esi + 0adh]
        mov    dword ptr [esi + 160h], 2
        mov    byte ptr [eax + 1e4bh], 28h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4628:
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0b5h]
        add    ecx, dword ptr [esi + 0b1h]
        mov    al, byte ptr [ecx + 2]
        mov    byte ptr [ebp - 3], al
        or     al, al
        je     SHORT loc_1_4683
        mov    cx, word ptr [ecx + 6]
        mov    word ptr [esi + 172h], cx
        mov    ah, cl
        mov    al, ch
        mov    word ptr [esi + 172h], ax
        and    eax, 0ffffh
        or     eax, 0c0011000h
        mov    dword ptr [ebp - 0ch], eax
        push   2
        xor    eax, eax
        mov    ax, word ptr [esi + 172h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   esi
        call   sub_1_43F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4683:
        mov    ax, word ptr [ecx + 0ah]
        mov    word ptr [ebp - 8], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 7]
        and    eax, 0ffffh
        add    eax, dword ptr [esi + 0b1h]
        mov    dword ptr [esi + 0b7h], eax
        mov    ax, word ptr [ecx + 0eh]
        mov    word ptr [ebp - 8], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 7]
        and    eax, 0ffffh
        add    eax, dword ptr [esi + 0b1h]
        mov    dword ptr [esi + 0bbh], eax
        mov    ax, word ptr [ecx + 0ch]
        mov    word ptr [ebp - 8], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 7]
        and    eax, 0ffffh
        add    eax, dword ptr [esi + 0b1h]
        mov    dword ptr [esi + 0bfh], eax
        mov    ax, word ptr [ecx + 8]
        mov    word ptr [ebp - 8], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 7]
        and    eax, 0ffffh
        add    eax, dword ptr [esi + 0b1h]
        mov    dword ptr [esi + 0c3h], eax
        mov    dword ptr [esi + 0a1h], 0
        push   esi
        call   sub_1_4EF0
        or     eax, eax
        jne    SHORT loc_1_4748
        push   esi
        call   sub_1_4FA0
        or     eax, eax
        jne    SHORT loc_1_4728
        push   esi
        call   sub_1_1DE0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4728:
        mov    eax, dword ptr [esi + 0adh]
        mov    dword ptr [esi + 160h], 4
        mov    byte ptr [eax + 1e4bh], 28h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4748:
        mov    eax, dword ptr [esi + 0adh]
        mov    dword ptr [esi + 160h], 3
        mov    byte ptr [eax + 1e4bh], 28h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4768:
        mov    eax, dword ptr [esi + 0b7h]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 2], al
        or     al, al
        jne    SHORT loc_1_47B1
        push   esi
        call   sub_1_4FA0
        or     eax, eax
        jne    SHORT loc_1_4791
        push   esi
        call   sub_1_1DE0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4791:
        mov    eax, dword ptr [esi + 0adh]
        mov    dword ptr [esi + 160h], 4
        mov    byte ptr [eax + 1e4bh], 28h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_47B1:
        cmp    byte ptr [ebp - 2], 0feh
        je     SHORT loc_1_4806
        push   3
        xor    eax, eax
        mov    al, byte ptr [ebp - 2]
        push   eax
        push   0
        push   esi
        call   sub_1_43F0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_47D0:
        mov    eax, dword ptr [esi + 0b7h]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 1], al
        or     al, al
        jne    SHORT loc_1_47F0
        push   esi
        call   sub_1_1DE0
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_47F0:
        cmp    byte ptr [ebp - 1], 0feh
        je     SHORT loc_1_4806
        push   4
        xor    eax, eax
        mov    al, byte ptr [ebp - 1]
        push   eax
        push   0
        push   esi
        call   sub_1_43F0
loc_1_4806:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
sub_1_4810:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    edx, dword ptr [edi + 0b7h]
        mov    al, byte ptr [edx + 2]
        cmp    al, 0feh
        mov    byte ptr [ebp - 1], al
        jne    SHORT loc_1_4840
        mov    ecx, dword ptr [ebp + 0ch]
        sub    eax, eax
        mov    byte ptr [ecx], 0
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_4840:
        mov    eax, dword ptr [edi + 0ceh]
        mov    dword ptr [edi + 0ceh], 0
        or     eax, eax
        mov    ecx, dword ptr [ebp + 0ch]
        jne    SHORT loc_1_4864
        mov    byte ptr [ecx], 0
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_4864:
        mov    byte ptr [ecx], 1
        cmp    byte ptr [ebp - 1], 0ffh
        je     SHORT loc_1_4895
        mov    esi, dword ptr [eax + 28h]
        inc    dword ptr [edi + 0ffh]
        push   0c0000001h
        mov    eax, dword ptr [eax + 2ch]
        push   eax
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_057A
        dec    dword ptr [esi + 14h]
        sub    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_4895:
        mov    byte ptr [eax + 30h], 1
        mov    cl, byte ptr [edx + 1]
        mov    byte ptr [eax + 31h], cl
        and    ecx, 0ffh
        mov    dword ptr [edi + ecx*4 + 179h], eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_48C0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 89h], 0
        je     SHORT loc_1_48F0
        push   esi
        call   sub_1_4B50
        cmp    eax, 103h
        jne    SHORT loc_1_48F0
        mov    eax, dword ptr [esi + 0adh]
        pop    esi
        mov    byte ptr [eax + 1e4bh], 20h
        pop    ebp
        ret    4
        db  090h
loc_1_48F0:
        mov    eax, dword ptr [esi + 0e7h]
        or     eax, eax
        je     SHORT loc_1_4922
        mov    ecx, dword ptr [eax + 24h]
        mov    dword ptr [esi + 0e7h], ecx
        mov    dword ptr [esi + 0ceh], eax
        push   eax
        push   esi
        call   sub_1_4960
        mov    eax, dword ptr [esi + 0adh]
        pop    esi
        mov    byte ptr [eax + 1e4bh], 20h
        pop    ebp
        ret    4
loc_1_4922:
        mov    byte ptr [esi + 0cbh], 1
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh
sub_1_4930:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    ecx, dword ptr [eax + 0b7h]
        mov    byte ptr [eax + 0cbh], 0
        mov    byte ptr [ecx], 0
        mov    byte ptr [ecx + 2], 0feh
        mov    eax, dword ptr [eax + 0adh]
        mov    byte ptr [eax + 1e4bh], 20h
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_4960:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 0b7h]
        mov    byte ptr [eax], 0ah
        mov    byte ptr [eax + 1], 0
        mov    byte ptr [eax + 2], 0feh
        mov    word ptr [eax + 4], 0
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4990:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   0
        push   0
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_0660
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0c3h]
        mov    byte ptr [eax], 0ah
        mov    cl, byte ptr [esi + 31h]
        mov    byte ptr [eax + 1], cl
        mov    byte ptr [eax + 2], 0
        mov    cl, byte ptr [ebp - 3]
        mov    ch, byte ptr [ebp - 4]
        mov    word ptr [eax + 6], cx
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_49E0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 0b7h]
        mov    byte ptr [eax], 1eh
        mov    word ptr [eax + 4], 0
        mov    byte ptr [eax + 2], 80h
        ret    4
        db  08Dh, 049h, 000h
sub_1_4A00:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 0b7h]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 4], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 3]
        mov    edx, eax
        and    edx, 0ffffh
        add    edx, dword ptr [ecx + 0b7h]
        mov    ax, word ptr [edx]
        mov    word ptr [ebp - 4], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 3]
        and    eax, 0ffffh
        add    dword ptr [ecx + 0f7h], eax
        mov    ax, word ptr [edx + 2]
        mov    word ptr [ebp - 4], ax
        mov    ah, al
        mov    al, byte ptr [ebp - 3]
        and    eax, 0ffffh
        add    dword ptr [ecx + 0fbh], eax
        mov    al, byte ptr [edx + 4]
        and    eax, 0ffh
        add    dword ptr [ecx + 0ffh], eax
        mov    al, byte ptr [edx + 5]
        and    eax, 0ffh
        add    dword ptr [ecx + 103h], eax
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_4A80:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 0b7h]
        mov    al, byte ptr [edx + 6]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 107h], eax
        mov    al, byte ptr [edx + 7]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 10bh], eax
        mov    al, byte ptr [edx + 8]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 10fh], eax
        mov    al, byte ptr [edx + 9]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 113h], eax
        mov    al, byte ptr [edx + 0ah]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 117h], eax
        mov    al, byte ptr [edx + 0ch]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 11bh], eax
        mov    al, byte ptr [edx + 0dh]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 11fh], eax
        mov    al, byte ptr [edx + 0eh]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 123h], eax
        mov    al, byte ptr [edx + 0fh]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 127h], eax
        mov    al, byte ptr [edx + 10h]
        mov    byte ptr [ebp - 4], al
        mov    eax, dword ptr [ebp - 4]
        add    dword ptr [ecx + 12bh], eax
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_4B30:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    eax, dword ptr [eax + 0b7h]
        mov    byte ptr [eax], 8
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4B50:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 89h], 0
        je     NEAR PTR loc_1_4D39
        mov    edi, dword ptr [ebp - 8]
        lea    esp, [esp]
loc_1_4B70:
        mov    ebx, dword ptr [esi + 89h]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 89h], eax
        or     eax, eax
        jne    SHORT loc_1_4B8C
        mov    dword ptr [esi + 8dh], 0
loc_1_4B8C:
        mov    eax, dword ptr [ebx + 10h]
        cmp    eax, 8
        jne    SHORT loc_1_4BC0
        cmp    dword ptr [ebx + 4], 0
        push   esi
        je     SHORT loc_1_4BA2
        call   sub_1_4B30
        jmp    SHORT loc_1_4BA7
loc_1_4BA2:
        call   sub_1_49E0
loc_1_4BA7:
        mov    eax, dword ptr [esi + 0adh]
        mov    edi, 103h
        mov    byte ptr [eax + 1e4bh], 20h
        jmp    NEAR PTR loc_1_4CA5
        db  08Bh, 0FFh
loc_1_4BC0:
        cmp    eax, 1
        je     SHORT loc_1_4C05
        cmp    eax, 2
        je     SHORT loc_1_4C48
        cmp    eax, 4
        je     NEAR PTR loc_1_4C66
        cmp    eax, 9
        je     NEAR PTR loc_1_4C83
        cmp    eax, 0ah
        je     NEAR PTR loc_1_4C94
        push   eax
        push   1
        push   9
        push   3
        push   0aa000006h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 18h
        jmp    NEAR PTR loc_1_4CA5
loc_1_4C05:
        mov    eax, dword ptr [ebx + 14h]
        cmp    eax, 1010eh
        jne    SHORT loc_1_4C30
        mov    eax, dword ptr [esi + 95h]
        mov    dword ptr [esi + 99h], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 95h], eax
        push   esi
        call   sub_1_4EF0
        jmp    SHORT loc_1_4CA3
        db  08Dh, 064h, 024h, 000h
loc_1_4C30:
        cmp    eax, 2010103h
        mov    eax, dword ptr [ebx + 8]
        jne    SHORT loc_1_4C97
        mov    dword ptr [esi + 9dh], eax
        push   esi
        call   sub_1_4EF0
        jmp    SHORT loc_1_4CA3
loc_1_4C48:
        push   esi
        call   sub_1_4B30
        mov    byte ptr [ebx + 4], 1
        mov    eax, dword ptr [esi + 0adh]
        mov    edi, 103h
        mov    byte ptr [eax + 1e4bh], 20h
        jmp    SHORT loc_1_4CA5
loc_1_4C66:
        mov    eax, dword ptr [esi + 95h]
        mov    dword ptr [esi + 99h], eax
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 95h], eax
        push   esi
        call   sub_1_4EF0
        jmp    SHORT loc_1_4CA3
loc_1_4C83:
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 9dh], eax
        push   esi
        call   sub_1_4EF0
        jmp    SHORT loc_1_4CA3
loc_1_4C94:
        mov    eax, dword ptr [ebx + 8]
loc_1_4C97:
        mov    dword ptr [esi + 0a5h], eax
        push   esi
        call   sub_1_4FA0
loc_1_4CA3:
        mov    edi, eax
loc_1_4CA5:
        cmp    edi, 103h
        je     NEAR PTR loc_1_4D44
        or     edi, edi
        jne    SHORT loc_1_4D11
        mov    eax, dword ptr [ebx + 10h]
        cmp    eax, 1
        je     SHORT loc_1_4CD6
        cmp    eax, 2
        je     SHORT loc_1_4CF0
        cmp    eax, 4
        je     SHORT loc_1_4D01
        cmp    eax, 9
        jl     SHORT loc_1_4CD1
        cmp    eax, 0ah
        jle    SHORT loc_1_4D09
loc_1_4CD1:
        push   eax
        push   9
        jmp    SHORT loc_1_4D14
loc_1_4CD6:
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebp - 4], eax
        push   0
        push   ebx
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_058C
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 14h]
        jmp    SHORT loc_1_4D2C
loc_1_4CF0:
        push   0
        push   ebx
        mov    eax, dword ptr [esi + 7dh]
        push   eax
        call   sub_1_05AA
        dec    dword ptr [esi + 5dh]
        jmp    SHORT loc_1_4D2C
loc_1_4D01:
        mov    eax, dword ptr [ebx + 4]
        dec    dword ptr [eax + 14h]
        jmp    SHORT loc_1_4D2C
loc_1_4D09:
        mov    eax, dword ptr [ebx + 4]
        dec    dword ptr [eax + 14h]
        jmp    SHORT loc_1_4D2C
loc_1_4D11:
        push   edi
        push   0ah
loc_1_4D14:
        push   5
        push   3
        push   0aa000006h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 18h
loc_1_4D2C:
        cmp    dword ptr [esi + 89h], 0
        jne    NEAR PTR loc_1_4B70
loc_1_4D39:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_4D44:
        mov    dword ptr [esi + 91h], ebx
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4D60:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 91h]
        mov    eax, dword ptr [edi + 10h]
        cmp    eax, 1
        je     SHORT loc_1_4DA0
        cmp    eax, 2
        je     NEAR PTR loc_1_4E01
        cmp    eax, 4
        je     NEAR PTR loc_1_4E50
        cmp    eax, 9
        jl     NEAR PTR loc_1_4E56
        cmp    eax, 0ah
        jle    NEAR PTR loc_1_4E50
        jmp    NEAR PTR loc_1_4E56
loc_1_4DA0:
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_1_4DE0
        cmp    dword ptr [edi + 14h], 2010104h
        jne    SHORT loc_1_4DBB
        mov    eax, dword ptr [esi + 0a5h]
        mov    dword ptr [esi + 0a9h], eax
loc_1_4DBB:
        mov    dword ptr [esi + 91h], 0
        mov    ebx, dword ptr [edi + 4]
        push   0
        push   edi
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_058C
        dec    dword ptr [ebx + 14h]
        jmp    NEAR PTR loc_1_4E56
        db  08Dh, 064h, 024h, 000h
loc_1_4DE0:
        mov    dword ptr [esi + 91h], 0
        mov    ebx, dword ptr [edi + 4]
        push   0c0000001h
        push   edi
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_058C
        dec    dword ptr [ebx + 14h]
        jmp    SHORT loc_1_4E56
loc_1_4E01:
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_1_4E30
        push   esi
        call   sub_1_4A80
        push   edi
        push   esi
        call   sub_1_1860
        mov    dword ptr [esi + 91h], 0
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 7dh]
        push   eax
        call   sub_1_05AA
        dec    dword ptr [esi + 5dh]
        jmp    SHORT loc_1_4E56
        db  08Bh, 0FFh
loc_1_4E30:
        mov    dword ptr [esi + 91h], 0
        push   0c0000001h
        push   edi
        mov    eax, dword ptr [esi + 7dh]
        push   eax
        call   sub_1_05AA
        dec    dword ptr [esi + 5dh]
        jmp    SHORT loc_1_4E56
        db  08Bh, 0FFh
loc_1_4E50:
        mov    eax, dword ptr [edi + 4]
        dec    dword ptr [eax + 14h]
loc_1_4E56:
        cmp    byte ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_4EDA
        mov    eax, dword ptr [edi + 10h]
        cmp    eax, 1
        je     SHORT loc_1_4EA0
        cmp    eax, 2
        je     SHORT loc_1_4EDA
        cmp    eax, 4
        je     SHORT loc_1_4EDA
        cmp    eax, 9
        jl     SHORT loc_1_4E7C
        cmp    eax, 0ah
        jle    SHORT loc_1_4EDA
loc_1_4E7C:
        push   eax
        push   9
        push   6
        push   3
        push   0aa000006h
        mov    eax, dword ptr [esi + 81h]
        push   eax
        call   sub_1_0708
        add    esp, 18h
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
loc_1_4EA0:
        cmp    dword ptr [edi + 14h], 1010eh
        jne    SHORT loc_1_4ED0
        mov    eax, dword ptr [esi + 99h]
        mov    dword ptr [esi + 95h], eax
        mov    dword ptr [esi + 0a1h], 0
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_4ED0:
        mov    dword ptr [esi + 9dh], 0
loc_1_4EDA:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4EF0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    eax, dword ptr [edi + 95h]
        test   ah, 20h
        je     SHORT loc_1_4F10
        mov    esi, 7fffffffh
        jmp    SHORT loc_1_4F20
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_4F10:
        test   ah, 40h
        mov    esi, 0
        je     SHORT loc_1_4F20
        mov    esi, dword ptr [edi + 9dh]
loc_1_4F20:
        cmp    dword ptr [edi + 0a1h], esi
        jne    SHORT loc_1_4F30
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_4F30:
        push   esi
        push   edi
        call   sub_1_4F50
        mov    dword ptr [edi + 0a1h], esi
        mov    eax, 103h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_4F50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0b7h]
        mov    eax, ecx
        mov    byte ptr [edx], 7
        mov    byte ptr [edx + 2], 0feh
        shr    eax, 18h
        mov    byte ptr [ebp - 4], al
        mov    eax, ecx
        shr    eax, 10h
        mov    byte ptr [ebp - 3], al
        mov    byte ptr [ebp - 2], ch
        mov    byte ptr [ebp - 1], cl
        sub    eax, eax
loc_1_4F81:
        mov    cl, byte ptr [ebp + eax - 4]
        mov    byte ptr [edx + eax + 6], cl
        inc    eax
        cmp    eax, 4
        jb     SHORT loc_1_4F81
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_4FA0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0a5h]
        push   ecx
        push   eax
        call   sub_1_4FC0
        mov    eax, 103h
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_4FC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [eax + 0b7h]
        mov    eax, ecx
        mov    byte ptr [edx], 6
        mov    byte ptr [edx + 2], 0feh
        shr    eax, 18h
        mov    byte ptr [ebp - 4], al
        mov    eax, ecx
        shr    eax, 10h
        mov    byte ptr [ebp - 3], al
        mov    byte ptr [ebp - 2], ch
        mov    byte ptr [ebp - 1], cl
        sub    eax, eax
loc_1_4FF1:
        mov    cl, byte ptr [ebp + eax - 4]
        mov    byte ptr [edx + eax + 6], cl
        inc    eax
        cmp    eax, 4
        jb     SHORT loc_1_4FF1
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
_LTEXT ends
        end
