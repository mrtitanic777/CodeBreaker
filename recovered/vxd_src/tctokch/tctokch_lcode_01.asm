; tctokch_lcode_01.asm -- TCTOKCH object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        db  0A0h, 098h, 007h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_0025
        db  0FEh, 005h, 098h, 007h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_0027
        push   0
        push   0
        db  0E8h, 080h, 007h, 0FFh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_0027
loc_1_0025:
        clc
        ret
loc_1_0027:
        stc
        ret
TCTOK_Control:
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
        lea    eax, [esi + 18h]
        mov    edx, dword ptr [ecx + 2]
        cmp    dword ptr [eax + 2], edx
        jne    SHORT loc_1_01C7
        mov    ax, word ptr [eax]
        cmp    word ptr [ecx], ax
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
        call   sub_1_060C
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
        call   sub_1_060C
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
        mov    al, byte ptr [edx]
        and    al, byte ptr [edx + 2]
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
        db  0CDh, 020h, 044h, 080h, 028h, 000h
sub_1_05C2:
        db  0CDh, 020h, 048h, 080h, 028h, 000h
sub_1_05C8:
        db  0CDh, 020h, 049h, 080h, 028h, 000h
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 056h, 080h, 028h, 000h
        add    byte ptr [eax], al
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_05E2:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_05E8:
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_05EE:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_05F4:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
sub_1_05FA:
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_0600:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_0606:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_060C:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_0630:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_0654:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
sub_1_065A:
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_0678:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_0684:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_068A:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_0690:
        db  0CDh, 020h, 007h, 080h, 028h, 000h
        db  0CDh, 020h, 042h, 080h, 028h, 000h
sub_1_069C:
        db  0CDh, 020h, 012h, 080h, 028h, 000h
sub_1_06A2:
        db  0CDh, 020h, 013h, 080h, 028h, 000h
        db  0CDh, 020h, 014h, 080h, 028h, 000h
sub_1_06AE:
        db  0CDh, 020h, 015h, 080h, 028h, 000h
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_06D8:
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_06EC:
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
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 054h, 043h, 054h, 04Fh
        db  04Bh, 020h, 020h, 020h, 000h, 040h, 000h, 054h, 029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_07A0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 10h]
        or     edx, edx
        je     SHORT loc_1_07DD
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 127h], 0
        je     SHORT loc_1_07C6
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   edx
        call   sub_1_060C
        pop    ebp
        ret    10h
loc_1_07C6:
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        push   edx
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ecx + 0f8h]
        push   eax
        call   sub_1_05C8
loc_1_07DD:
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_07F0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 167h]
        or     eax, eax
        je     SHORT loc_1_0811
        mov    ecx, dword ptr [esi + 16bh]
        push   ecx
        push   eax
        push   6
        push   esi
        call   sub_1_07A0
loc_1_0811:
        mov    eax, dword ptr [esi + 173h]
        or     eax, eax
        je     SHORT loc_1_082B
        mov    ecx, dword ptr [esi + 177h]
        push   ecx
        push   eax
        push   8
        push   esi
        call   sub_1_07A0
loc_1_082B:
        mov    eax, dword ptr [esi + 183h]
        or     eax, eax
        je     SHORT loc_1_0845
        mov    ecx, dword ptr [esi + 187h]
        push   ecx
        push   eax
        push   12h
        push   esi
        call   sub_1_07A0
loc_1_0845:
        mov    eax, dword ptr [esi + 18bh]
        or     eax, eax
        je     SHORT loc_1_085F
        mov    ecx, dword ptr [esi + 18fh]
        push   ecx
        push   eax
        push   0eh
        push   esi
        call   sub_1_07A0
loc_1_085F:
        mov    eax, dword ptr [esi + 193h]
        or     eax, eax
        je     SHORT loc_1_087E
        mov    ecx, dword ptr [esi + 197h]
        push   ecx
        push   eax
        mov    eax, dword ptr [esi + 19fh]
        push   eax
        push   esi
        call   sub_1_07A0
loc_1_087E:
        mov    eax, dword ptr [esi + 7ah]
        or     eax, eax
        je     SHORT loc_1_089E
        mov    ecx, dword ptr [esi + 7eh]
        push   ecx
        push   eax
        mov    eax, dword ptr [esi + 76h]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    eax, ecx
        push   eax
        push   esi
        call   sub_1_07A0
loc_1_089E:
        mov    eax, dword ptr [esi + 6ah]
        or     eax, eax
        je     SHORT loc_1_08B7
        mov    ecx, dword ptr [esi + 76h]
        shl    ecx, 2
        push   0
        lea    ecx, [ecx + ecx*8]
        push   ecx
        push   eax
        call   sub_1_060C
loc_1_08B7:
        mov    eax, dword ptr [esi + 8ah]
        or     eax, eax
        je     SHORT loc_1_08E1
        mov    ecx, dword ptr [esi + 8eh]
        push   ecx
        push   eax
        mov    eax, dword ptr [esi + 82h]
        imul   eax, dword ptr [esi + 86h]
        imul   eax, dword ptr [esi + 76h]
        push   eax
        push   esi
        call   sub_1_07A0
loc_1_08E1:
        mov    eax, dword ptr [esi + 5eh]
        or     eax, eax
        je     SHORT loc_1_0901
        mov    ecx, dword ptr [esi + 62h]
        push   ecx
        push   eax
        mov    eax, dword ptr [esi + 56h]
        mov    ecx, eax
        lea    eax, [eax + eax*4]
        lea    eax, [eax + eax*4]
        add    eax, ecx
        push   eax
        push   esi
        call   sub_1_07A0
loc_1_0901:
        mov    eax, dword ptr [esi + 42h]
        or     eax, eax
        je     SHORT loc_1_091E
        mov    ecx, dword ptr [esi + 56h]
        mov    edx, ecx
        lea    ecx, [ecx + ecx*4]
        push   0
        lea    ecx, [ecx + ecx*8]
        sub    ecx, edx
        push   ecx
        push   eax
        call   sub_1_060C
loc_1_091E:
        mov    eax, dword ptr [esi + 1ah]
        or     eax, eax
        je     SHORT loc_1_0932
        push   0
        push   8ch
        push   eax
        call   sub_1_060C
loc_1_0932:
        pop    esi
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0940:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        sub    eax, eax
        mov    edi, dword ptr [esi + 17bh]
        mov    ecx, 8
        rep stosd
        mov    eax, dword ptr [esi + 23dh]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        push   esi
        call   sub_1_3B30
        mov    ecx, dword ptr [esi + 17bh]
        mov    word ptr [esi + 102h], ax
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [esi + 23dh]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esi + 108h], eax
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 17bh]
        mov    word ptr [eax + 8], cx
        mov    ax, word ptr [esi + 108h]
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 17bh]
        mov    word ptr [eax + 0ah], cx
        mov    eax, dword ptr [esi + 23dh]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 104h], eax
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 17bh]
        mov    word ptr [eax + 0ch], cx
        mov    ax, word ptr [esi + 104h]
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 17bh]
        mov    word ptr [eax + 0eh], cx
        mov    eax, dword ptr [esi + 17bh]
        mov    word ptr [eax + 10h], 0
        mov    eax, dword ptr [esi + 17bh]
        mov    word ptr [eax + 12h], 0
        mov    eax, dword ptr [esi + 17bh]
        mov    byte ptr [eax + 1ah], 6
        mov    eax, dword ptr [esi + 17bh]
        mov    byte ptr [eax + 1bh], 6
        cmp    byte ptr [esi + 0eah], 1
        jne    SHORT loc_1_0A87
        mov    eax, dword ptr [esi + 17bh]
        mov    word ptr [eax + 14h], 4
        mov    eax, dword ptr [esi + 17bh]
        mov    word ptr [eax + 16h], 610h
        mov    eax, dword ptr [esi + 17bh]
        mov    word ptr [eax + 18h], 0febfh
        jmp    SHORT loc_1_0AC6
loc_1_0A87:
        mov    ax, word ptr [esi + 100h]
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 17bh]
        mov    word ptr [eax + 14h], cx
        mov    eax, dword ptr [esi + 17bh]
        mov    word ptr [eax + 16h], 0
        mov    eax, dword ptr [esi + 17bh]
        mov    word ptr [eax + 18h], 0
loc_1_0AC6:
        mov    eax, dword ptr [esi + 17bh]
        add    eax, 2
        mov    dl, byte ptr [esi + 161h]
        lea    ecx, [esi + 161h]
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
        cmp    byte ptr [esi + 127h], 0
        je     SHORT loc_1_0B36
        mov    eax, dword ptr [esi + 183h]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 17bh]
        mov    word ptr [eax + 1ch], cx
        mov    ax, word ptr [esi + 183h]
        jmp    SHORT loc_1_0B66
loc_1_0B36:
        mov    eax, dword ptr [esi + 187h]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 17bh]
        mov    word ptr [eax + 1ch], cx
        mov    ax, word ptr [esi + 187h]
loc_1_0B66:
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 17bh]
        mov    word ptr [eax + 1eh], cx
        mov    ecx, dword ptr [esi + 167h]
        mov    ax, word ptr [ecx]
        mov    word ptr [ebp - 2], ax
        sub    eax, eax
loc_1_0B95:
        cmp    word ptr [ebp - 2], ax
        jne    SHORT loc_1_0B95
        mov    word ptr [ecx], 300h
        cmp    byte ptr [esi + 127h], 0
        je     SHORT loc_1_0BE0
        mov    eax, dword ptr [esi + 17bh]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 167h]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 17bh]
        jmp    SHORT loc_1_0C10
        db  005h, 000h, 000h, 000h, 000h
loc_1_0BE0:
        mov    eax, dword ptr [esi + 17fh]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 167h]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 17fh]
loc_1_0C10:
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 167h]
        push   esi
        mov    word ptr [eax + 4], cx
        db  068h, 000h, 019h, 000h, 000h
        push   esi
        call   sub_1_05FA
        lea    eax, [esi + 0a3h]
        push   8ca0h
        mov    dword ptr [eax], 0
        db  0C7h, 086h, 0A7h, 000h, 000h, 000h, 080h, 029h, 000h, 000h
        mov    dword ptr [esi + 0abh], esi
        push   eax
        call   sub_1_05E2
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h
sub_1_0C70:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    cl, byte ptr [eax]
        mov    dl, byte ptr [eax + 1]
        mov    byte ptr [eax], dl
        mov    byte ptr [eax + 1], cl
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0C90:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0eah], 1
        mov    edx, dword ptr [ecx + 11fh]
        je     SHORT loc_1_0CE0
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    ax, word ptr [ebp - 2]
        or     ax, 80h
        mov    word ptr [ebp - 6], ax
        mov    cx, word ptr [ecx + 11fh]
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, byte ptr [ebp - 5]
        jmp    SHORT loc_1_0D13
        db  08Dh, 049h, 000h
loc_1_0CE0:
        add    dx, 6
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    ax, word ptr [ebp - 2]
        or     ax, 4000h
        mov    word ptr [ebp - 4], ax
        mov    cx, word ptr [ecx + 11fh]
        lea    edx, [ecx + 6]
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, byte ptr [ebp - 3]
loc_1_0D13:
        out    dx, al
        push   0fah
        call   sub_1_0600
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0D30:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0eah], 1
        je     SHORT loc_1_0DB1
        push   3e8h
        call   sub_1_0600
        mov    ecx, dword ptr [esi + 11fh]
        add    cx, 8
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 3], al
        cmp    byte ptr [esi + 127h], 0
        je     SHORT loc_1_0D7B
        or     word ptr [ebp - 4], 100h
loc_1_0D7B:
        or     word ptr [ebp - 4], 40h
        and    word ptr [ebp - 4], 0ff7fh
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ebp - 8], ax
        mov    cx, word ptr [esi + 11fh]
        push   3e8h
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, byte ptr [ebp - 7]
        out    dx, al
        call   sub_1_0600
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_0DB1:
        mov    word ptr [ebp - 6], 0ff00h
        mov    cx, word ptr [esi + 11fh]
        mov    al, byte ptr [ebp - 6]
        lea    edx, [ecx + 6]
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        push   7d0h
        call   sub_1_0600
        mov    ecx, dword ptr [esi + 11fh]
        add    cx, 9
        mov    dx, cx
        in     al, dx
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 9
        or     al, 28h
        out    dx, al
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0E00:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        sub    cx, cx
        mov    eax, dword ptr [edx + 6ah]
        mov    dword ptr [edx + 6eh], eax
        cmp    dword ptr [edx + 76h], 0
        jbe    SHORT loc_1_0E44
        mov    esi, 88h
        mov    edi, 0feffh
loc_1_0E22:
        mov    ebx, dword ptr [eax + 4]
        inc    cx
        mov    word ptr [ebx + 4], si
        mov    ebx, dword ptr [eax + 4]
        and    word ptr [ebx + 2], di
        mov    dword ptr [edx + 72h], eax
        mov    ebx, ecx
        mov    eax, dword ptr [eax]
        and    ebx, 0ffffh
        cmp    ebx, dword ptr [edx + 76h]
        jb     SHORT loc_1_0E22
loc_1_0E44:
        mov    eax, dword ptr [edx + 72h]
        pop    edi
        mov    eax, dword ptr [eax + 4]
        pop    esi
        or     word ptr [eax + 2], 100h
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0E60:
        push   ebp
        mov    ebp, esp
        sub    esp, 3ch
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 1ch], 0
        mov    byte ptr [ebp - 1bh], 0
        mov    byte ptr [ebp - 1ah], 0c1h
        mov    byte ptr [ebp - 19h], 0e2h
        mov    byte ptr [ebp - 18h], 0d4h
        mov    byte ptr [ebp - 17h], 8bh
        mov    byte ptr [ebp - 24h], 0ffh
        mov    byte ptr [ebp - 23h], 0ffh
        mov    byte ptr [ebp - 22h], 0d1h
        mov    byte ptr [ebp - 21h], 0d7h
        mov    edi, 40h
        mov    byte ptr [ebp - 20h], 0c5h
        mov    byte ptr [ebp - 1fh], 0d9h
        mov    byte ptr [ebp - 1eh], 0c3h
        mov    byte ptr [ebp - 1dh], 0d4h
        mov    word ptr [ebp - 0ah], 0
        mov    esi, dword ptr [ebp + 8]
        nop
loc_1_0EB0:
        mov    ebx, 7530h
loc_1_0EB5:
        push   64h
        call   sub_1_0600
        mov    ecx, dword ptr [esi + 11fh]
        add    cx, 6
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 3], al
        and    word ptr [ebp - 4], 0f0h
        cmp    word ptr [ebp - 4], di
        je     SHORT loc_1_0F16
        dec    ebx
        jne    SHORT loc_1_0EB5
        cmp    word ptr [ebp - 4], di
        je     SHORT loc_1_0F16
        mov    word ptr [ebp - 8], 0ff00h
        mov    cx, word ptr [esi + 11fh]
        mov    al, byte ptr [ebp - 8]
        lea    edx, [ecx + 6]
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, byte ptr [ebp - 7]
        out    dx, al
        inc    word ptr [ebp - 0ah]
        cmp    word ptr [ebp - 0ah], 3
        jb     SHORT loc_1_0EB0
loc_1_0F16:
        cmp    word ptr [ebp - 4], 40h
        jne    NEAR PTR loc_1_1214
        lea    eax, [ebp - 3ch]
        mov    dword ptr [eax], 0
        mov    dword ptr [eax + 4], 0
        mov    dword ptr [eax + 8], 0
        mov    dword ptr [eax + 0ch], 0
        mov    dword ptr [eax + 10h], 0
        mov    word ptr [eax + 14h], 0
        mov    word ptr [ebp - 3ch], 9f00h
        mov    word ptr [ebp - 30h], 505h
        cmp    byte ptr [esi + 127h], 0
        je     SHORT loc_1_0FA7
        mov    ecx, dword ptr [esi + 167h]
        shr    ecx, 10h
        mov    al, cl
        and    eax, 0ffff00ffh
        shl    ax, 8
        mov    cl, ch
        and    ecx, 0ffff00ffh
        add    ax, cx
        mov    word ptr [ebp - 2eh], ax
        mov    cx, word ptr [esi + 167h]
        mov    al, cl
        and    eax, 0ffff00ffh
        shl    ax, 8
        mov    cl, ch
        and    ecx, 0ffff00ffh
        add    ax, cx
        mov    word ptr [ebp - 2ch], ax
        jmp    SHORT loc_1_0FEB
loc_1_0FA7:
        mov    ecx, dword ptr [esi + 16bh]
        shr    ecx, 10h
        mov    al, cl
        and    eax, 0ffff00ffh
        shl    ax, 8
        mov    cl, ch
        and    ecx, 0ffff00ffh
        add    ax, cx
        mov    word ptr [ebp - 2eh], ax
        mov    ax, word ptr [esi + 16bh]
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    word ptr [ebp - 2ch], cx
loc_1_0FEB:
        cmp    byte ptr [esi + 127h], 0
        je     SHORT loc_1_1033
        mov    ecx, dword ptr [esi + 173h]
        shr    ecx, 10h
        mov    al, cl
        and    eax, 0ffff00ffh
        shl    ax, 8
        mov    cl, ch
        and    ecx, 0ffff00ffh
        add    ax, cx
        mov    word ptr [ebp - 2ah], ax
        mov    cx, word ptr [esi + 173h]
        mov    al, cl
        and    eax, 0ffff00ffh
        shl    ax, 8
        mov    cl, ch
        and    ecx, 0ffff00ffh
        jmp    SHORT loc_1_106D
loc_1_1033:
        mov    eax, dword ptr [esi + 177h]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    word ptr [ebp - 2ah], cx
        mov    cx, word ptr [esi + 177h]
        mov    al, ch
        and    eax, 0ffff00ffh
        and    cx, 0ffh
        shl    cx, 8
loc_1_106D:
        add    ax, cx
        mov    word ptr [ebp - 28h], ax
        lea    eax, [ebp - 2eh]
        push   eax
        call   sub_1_0C70
        lea    eax, [ebp - 2ch]
        push   eax
        call   sub_1_0C70
        lea    eax, [ebp - 2ah]
        push   eax
        call   sub_1_0C70
        lea    eax, [ebp - 28h]
        push   eax
        call   sub_1_0C70
        cmp    byte ptr [esi + 0eah], 1
        je     SHORT loc_1_10BC
        mov    word ptr [ebp - 10h], 1
        mov    cx, word ptr [esi + 11fh]
        mov    al, byte ptr [ebp - 10h]
        lea    edx, [ecx + 0ch]
        out    dx, al
        lea    edx, [ecx + 0dh]
        mov    al, byte ptr [ebp - 0fh]
        out    dx, al
loc_1_10BC:
        mov    word ptr [ebp - 0eh], 0a00h
        mov    cx, word ptr [esi + 11fh]
        mov    al, byte ptr [ebp - 0eh]
        lea    edx, [ecx + 4]
        out    dx, al
        lea    edx, [ecx + 5]
        mov    al, byte ptr [ebp - 0dh]
        out    dx, al
        lea    eax, [ebp - 3ch]
        mov    dword ptr [ebp - 14h], eax
        mov    di, 0bh
        mov    ecx, 2
loc_1_10E6:
        mov    eax, dword ptr [ebp - 14h]
        mov    ax, word ptr [eax]
        add    dword ptr [ebp - 14h], ecx
        mov    word ptr [ebp - 6], ax
        dec    di
        mov    bx, word ptr [esi + 11fh]
        lea    edx, [ebx + 2]
        out    dx, al
        lea    edx, [ebx + 3]
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        jne    SHORT loc_1_10E6
        mov    word ptr [ebp - 0ch], 9080h
        mov    cx, word ptr [esi + 11fh]
        mov    al, byte ptr [ebp - 0ch]
        lea    edx, [ecx + 6]
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, byte ptr [ebp - 0bh]
        out    dx, al
        mov    dword ptr [esi + 97h], 0
        db  0C7h, 086h, 09Bh, 000h, 000h, 000h, 030h, 012h, 000h, 000h
        mov    dword ptr [esi + 9fh], esi
        push   2af8h
        lea    eax, [esi + 97h]
        push   eax
        call   sub_1_05E2
        push   0ah
        call   sub_1_0600
        mov    ecx, 60h
        add    eax, 0
loc_1_1160:
        cmp    byte ptr [esi + 96h], 0
        jne    NEAR PTR loc_1_1214
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 6
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 3], al
        test   word ptr [ebp - 4], cx
        jne    SHORT loc_1_1160
        test   byte ptr [ebp - 4], 10h
        jne    SHORT loc_1_11EA
        mov    byte ptr [ebp - 1], 0
        lea    edi, [ebp - 1ch]
        mov    eax, dword ptr [esi + 167h]
        mov    dl, 6
loc_1_11A4:
        mov    cl, dl
        dec    dl
        or     cl, cl
        je     SHORT loc_1_11BC
        mov    ebx, eax
        inc    eax
        mov    ecx, edi
        inc    edi
        mov    cl, byte ptr [ecx]
        cmp    byte ptr [ebx], cl
        je     SHORT loc_1_11A4
        mov    byte ptr [ebp - 1], 1
loc_1_11BC:
        lea    edi, [ebp - 24h]
        mov    eax, dword ptr [esi + 173h]
        mov    dl, 8
loc_1_11C7:
        mov    cl, dl
        dec    dl
        or     cl, cl
        je     SHORT loc_1_11DF
        mov    ebx, eax
        inc    eax
        mov    ecx, edi
        inc    edi
        mov    cl, byte ptr [ecx]
        cmp    byte ptr [ebx], cl
        je     SHORT loc_1_11C7
        mov    byte ptr [ebp - 1], 1
loc_1_11DF:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_11EA
        or     word ptr [ebp - 4], 10h
loc_1_11EA:
        lea    eax, [ebp - 2]
        push   eax
        lea    eax, [esi + 97h]
        push   eax
        call   sub_1_05E8
        cmp    byte ptr [esi + 96h], 0
        jne    SHORT loc_1_1214
        test   byte ptr [ebp - 4], 10h
        jne    SHORT loc_1_1214
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1214:
        push   esi
        call   sub_1_0C90
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1230:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0ch]
        pop    ebp
        mov    byte ptr [eax + 96h], 1
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1250:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_0C90
        cmp    byte ptr [esi + 18h], 0
        je     SHORT loc_1_1269
        push   esi
        call   sub_1_05F4
loc_1_1269:
        push   esi
        call   sub_1_07F0
        mov    eax, dword ptr [esi + 23dh]
        or     eax, eax
        je     SHORT loc_1_127F
        push   eax
        db  0E8h, 051h, 0EEh, 0FEh, 0FFh
loc_1_127F:
        cmp    byte ptr [esi + 0e8h], 0
        je     SHORT loc_1_1294
        mov    eax, dword ptr [esi + 0f8h]
        push   eax
        call   sub_1_0568
loc_1_1294:
        push   0
        push   241h
        push   esi
        call   sub_1_060C
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_12B0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [esi + 19h], 0
        jne    NEAR PTR loc_1_1430
        mov    byte ptr [esi + 19h], 1
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        dec    dword ptr [esi + 14h]
        push   0
        mov    eax, dword ptr [esi + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 23dh]
        push   eax
        db  0E8h, 0E2h, 0EEh, 0FEh, 0FFh
        mov    ebx, eax
        or     ebx, ebx
        je     SHORT loc_1_1350
        cmp    ebx, 103h
        je     NEAR PTR loc_1_13A0
        cmp    ebx, 0c001000dh
        je     NEAR PTR loc_1_13D0
        cmp    ebx, 0c001000eh
        je     NEAR PTR loc_1_1400
        lea    eax, [edi + 0bfh]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0c3h]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 0c3h]
        mov    ebx, 0c0000001h
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 0c3h], esi
        mov    byte ptr [esi + 19h], 0
        inc    dword ptr [esi + 14h]
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_1350:
        cmp    dword ptr [esi + 14h], 0
        je     SHORT loc_1_1383
        lea    eax, [edi + 0c7h]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0cbh]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 0cbh]
        mov    ebx, 103h
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 0cbh], esi
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
loc_1_1383:
        push   0
        push   92h
        push   esi
        call   sub_1_060C
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_13A0:
        lea    eax, [edi + 0c7h]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0cbh]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 0cbh]
        push   edi
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 0cbh], esi
        call   sub_1_3170
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_13D0:
        lea    eax, [edi + 0cfh]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0d3h]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 0d3h]
        mov    ebx, 103h
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 0d3h], esi
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_1400:
        lea    eax, [edi + 0c7h]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0cbh]
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [edi + 0cbh]
        mov    ebx, 103h
        mov    dword ptr [eax], esi
        mov    dword ptr [edi + 0cbh], esi
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_1430:
        mov    ebx, 0c0010002h
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1440:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 8]
        cmp    byte ptr [edi + 11eh], 0
        jne    SHORT loc_1_1492
        cmp    byte ptr [esi + 19h], 0
        jne    SHORT loc_1_1487
        inc    dword ptr [esi + 14h]
        push   esi
        push   edi
        call   sub_1_14C0
        cmp    dword ptr [edi + 32h], 0
        je     SHORT loc_1_1473
        mov    byte ptr [edi + 0e7h], 1
        jmp    SHORT loc_1_1479
loc_1_1473:
        push   edi
        call   sub_1_1570
loc_1_1479:
        mov    eax, 103h
        dec    dword ptr [esi + 14h]
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_1487:
        mov    eax, 0c0010002h
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_1492:
        cmp    byte ptr [edi + 11ah], 0
        je     SHORT loc_1_14A6
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_14A6:
        cmp    byte ptr [edi + 11dh], 0
        mov    eax, 0c0000001h
        je     SHORT loc_1_14BA
        pop    edi
        pop    esi
        pop    ebp
        ret    4
loc_1_14BA:
        pop    edi
        pop    esi
        pop    ebp
        ret    4
sub_1_14C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0ch]
        mov    byte ptr [edi + 11eh], 1
        mov    byte ptr [edi + 11ah], 1
        mov    dword ptr [edi + 0d7h], ebx
        cmp    dword ptr [edi + 2ah], 0
        je     SHORT loc_1_152D
        mov    esi, 2
loc_1_14EB:
        mov    ecx, dword ptr [edi + 2ah]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edi + 2ah], eax
        mov    eax, dword ptr [ecx + 4]
        or     eax, eax
        je     SHORT loc_1_1510
        dec    dword ptr [eax + 14h]
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_058C
        jmp    SHORT loc_1_1527
        db  08Bh, 0FFh
loc_1_1510:
        cmp    dword ptr [ecx + 10h], esi
        jne    SHORT loc_1_1527
        push   0c001000ch
        push   ecx
        mov    eax, dword ptr [edi + 0fch]
        push   eax
        call   sub_1_05AA
loc_1_1527:
        cmp    dword ptr [edi + 2ah], 0
        jne    SHORT loc_1_14EB
loc_1_152D:
        mov    esi, dword ptr [edi + 0bfh]
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0ffffff41h
        je     SHORT loc_1_1561
        mov    edi, edi
loc_1_1540:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_059E
        mov    eax, edi
        sub    eax, esi
        cmp    eax, 0ffffff41h
        jne    SHORT loc_1_1540
loc_1_1561:
        or     ebx, ebx
        je     SHORT loc_1_1568
        inc    dword ptr [ebx + 14h]
loc_1_1568:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    8
        db  090h
sub_1_1570:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0eah], 1
        mov    edx, dword ptr [ecx + 11fh]
        lea    esi, [ecx + 11fh]
        je     SHORT loc_1_15C0
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    word ptr [ebp - 2], 0fff7h
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 8], ax
        mov    di, word ptr [esi]
        lea    edx, [edi + 8]
        out    dx, al
        lea    edx, [edi + 9]
        mov    al, byte ptr [ebp - 7]
        jmp    SHORT loc_1_15ED
        db  090h
loc_1_15C0:
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi]
        add    dx, 0ah
        in     al, dx
        mov    byte ptr [ebp - 1], al
        and    word ptr [ebp - 2], 0fffbh
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 6], ax
        mov    di, word ptr [esi]
        lea    edx, [edi + 9]
        out    dx, al
        lea    edx, [edi + 0ah]
        mov    al, byte ptr [ebp - 5]
loc_1_15ED:
        mov    word ptr [ebp - 4], 0ff00h
        mov    si, word ptr [esi]
        out    dx, al
        lea    edx, [esi + 6]
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        lea    edx, [esi + 7]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        lea    eax, [ecx + 0dbh]
        push   3e8h
        mov    dword ptr [eax], 0
        db  0C7h, 081h, 0DFh, 000h, 000h, 000h, 040h, 016h, 000h, 000h
        mov    dword ptr [ecx + 0e3h], ecx
        push   eax
        call   sub_1_05E2
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1640:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        call   sub_1_0E60
        or     al, al
        je     SHORT loc_1_1663
        mov    word ptr [esi + 0b1h], 3
        push   esi
        call   sub_1_0940
loc_1_1663:
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        or     word ptr [ebp - 2], 8
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [ebp - 4], ax
        mov    cx, word ptr [esi + 11fh]
        pop    esi
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_16B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        cmp    byte ptr [ebp + 0ch], 0
        mov    esi, dword ptr [ebp + 8]
        je     NEAR PTR loc_1_1790
        mov    byte ptr [esi + 11eh], 0
        mov    byte ptr [esi + 11ah], 0
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 8], 0
        push   esi
        call   sub_1_0E00
        mov    ecx, dword ptr [esi + 167h]
        mov    ax, word ptr [ecx]
        mov    word ptr [ebp - 2], ax
        sub    eax, eax
loc_1_16F7:
        cmp    word ptr [ebp - 2], ax
        jne    SHORT loc_1_16F7
        mov    word ptr [ecx], 600h
        cmp    byte ptr [esi + 127h], 0
        je     SHORT loc_1_1737
        mov    eax, dword ptr [esi + 7ah]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 167h]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 7ah]
        jmp    SHORT loc_1_1761
loc_1_1737:
        mov    eax, dword ptr [esi + 7eh]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 167h]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 7eh]
loc_1_1761:
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 167h]
        push   esi
        mov    word ptr [eax + 4], cx
        db  068h, 000h, 019h, 000h, 000h
        push   esi
        call   sub_1_05FA
        jmp    SHORT loc_1_1807
        db  090h
loc_1_1790:
        mov    byte ptr [esi + 11eh], 1
        mov    byte ptr [esi + 11dh], 1
        mov    byte ptr [esi + 11bh], 0
        mov    byte ptr [esi + 11ah], 0
        mov    eax, dword ptr [ebp + 10h]
        or     eax, eax
        mov    cx, word ptr [eax + 2]
        mov    word ptr [esi + 0afh], cx
        mov    dword ptr [ebp - 8], 0c0011000h
        mov    dword ptr [ebp - 0ch], 0c0000001h
        je     SHORT loc_1_1807
        cmp    word ptr [eax], 300h
        jne    SHORT loc_1_1807
        mov    ax, word ptr [eax + 2]
        test   al, 2
        je     SHORT loc_1_1807
        mov    al, ah
        and    eax, 0ffh
        dec    eax
        cmp    eax, 9
        ja     SHORT loc_1_1807
        db  0FFh, 024h, 085h, 09Ch, 018h, 000h, 000h
        db  090h
loc_1_17F0:
        mov    dword ptr [ebp - 0ch], 80010003h
        jmp    SHORT loc_1_1807
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1800:
        mov    dword ptr [ebp - 0ch], 80010004h
loc_1_1807:
        mov    edi, dword ptr [esi + 0bfh]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0ffffff41h
        je     SHORT loc_1_1859
loc_1_1818:
        mov    ebx, edi
        mov    edi, dword ptr [edi]
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_1_1830
        add    ebx, 10h
        push   0
        push   0
        push   40010005h
        jmp    SHORT loc_1_183E
loc_1_1830:
        add    ebx, 10h
        push   4
        lea    eax, [ebp - 8]
        push   eax
        push   40010007h
loc_1_183E:
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [ebx]
        push   eax
        call   sub_1_05A4
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0ffffff41h
        jne    SHORT loc_1_1818
loc_1_1859:
        mov    eax, dword ptr [esi + 0d7h]
        or     eax, eax
        je     SHORT loc_1_187D
        mov    dword ptr [esi + 0d7h], 0
        dec    dword ptr [eax + 14h]
        mov    ecx, dword ptr [ebp - 0ch]
        push   ecx
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_0586
loc_1_187D:
        mov    eax, dword ptr [esi + 0cfh]
        sub    eax, esi
        cmp    eax, 0cfh
        je     SHORT loc_1_1892
        push   esi
        call   sub_1_2660
loc_1_1892:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  090h, 0F0h, 017h, 000h, 000h, 000h, 018h, 000h, 000h, 007h, 018h, 000h, 000h, 0F0h, 017h, 000h
        db  000h, 007h, 018h, 000h, 000h, 000h, 018h, 000h, 000h, 007h, 018h, 000h, 000h, 000h, 018h, 000h
        db  000h, 007h, 018h, 000h, 000h, 0F0h, 017h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h
        db  024h, 000h, 000h, 000h, 000h
loc_1_18D0:
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
loc_1_1900:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   byte ptr [ebp - 2], 80h
        je     SHORT loc_1_1937
        mov    esi, 80h
loc_1_1923:
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 6
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   word ptr [ebp - 2], si
        jne    SHORT loc_1_1923
loc_1_1937:
        mov    word ptr [ebp - 2], 90h
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 7
        mov    al, 90h
        out    dx, al
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1960:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   byte ptr [ebp - 2], 80h
        je     SHORT loc_1_1997
        mov    esi, 80h
loc_1_1983:
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 6
        in     al, dx
        mov    byte ptr [ebp - 2], al
        test   word ptr [ebp - 2], si
        jne    SHORT loc_1_1983
loc_1_1997:
        mov    word ptr [ebp - 2], 84h
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 7
        mov    al, 84h
        out    dx, al
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_19C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 0eah], 1
        je     NEAR PTR loc_1_1AF8
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 3], al
        test   byte ptr [ebp - 3], 1
        je     NEAR PTR loc_1_1AF8
        test   byte ptr [ebp - 3], 2
        je     NEAR PTR loc_1_1AF8
        test   byte ptr [ebp - 3], 4
        mov    bl, 1
        jne    SHORT loc_1_1A13
        sub    bl, bl
loc_1_1A13:
        or     word ptr [ebp - 4], 800h
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ebp - 0ah], ax
        mov    si, word ptr [ecx + 11fh]
        lea    edx, [esi + 8]
        out    dx, al
        lea    edx, [esi + 9]
        mov    al, byte ptr [ebp - 9]
        out    dx, al
        lea    edx, [esi + 2]
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 3
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 6
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 7], al
        mov    edx, dword ptr [ebp - 8]
        and    edx, 0ffffh
        shl    edx, 10h
        mov    dword ptr [ebp - 10h], edx
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 4
        in     al, dx
        mov    byte ptr [ebp - 6], al
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 5
        in     al, dx
        mov    byte ptr [ebp - 5], al
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        add    dword ptr [ebp - 10h], eax
        mov    dx, word ptr [ecx + 11fh]
        or     bl, bl
        je     SHORT loc_1_1AC1
        sub    edi, edi
        cmp    word ptr [ebp - 2], di
        jbe    SHORT loc_1_1ADC
loc_1_1AAB:
        mov    esi, dword ptr [ebp - 10h]
        in     al, dx
        mov    byte ptr [esi], al
        inc    edi
        xor    eax, eax
        inc    dword ptr [ebp - 10h]
        mov    ax, word ptr [ebp - 2]
        cmp    eax, edi
        ja     SHORT loc_1_1AAB
        jmp    SHORT loc_1_1ADC
loc_1_1AC1:
        cmp    word ptr [ebp - 2], 0
        jbe    SHORT loc_1_1ADC
        xor    esi, esi
        mov    si, word ptr [ebp - 2]
        mov    edi, edi
loc_1_1AD0:
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax]
        out    dx, al
        inc    dword ptr [ebp - 10h]
        dec    esi
        jne    SHORT loc_1_1AD0
loc_1_1ADC:
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [ecx + 11fh]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 3], al
loc_1_1AF8:
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 22h]
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 6
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 1], al
        test   byte ptr [ebp - 2], 80h
        jne    SHORT loc_1_1B50
        sub    al, al
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1B50:
        push   8
        mov    eax, dword ptr [esi + 173h]
        push   eax
        lea    eax, [edi + 6]
        push   eax
        call   sub_1_06EC
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [edi + 4], ax
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 22h], eax
        test   byte ptr [ebp - 1], 80h
        je     SHORT loc_1_1BA9
        mov    edi, 8000h
        lea    ebx, [ebx]
loc_1_1B80:
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 6
        in     al, dx
        mov    byte ptr [ebp - 2], al
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 7
        in     al, dx
        mov    byte ptr [ebp - 1], al
        push   2
        call   sub_1_0600
        test   word ptr [ebp - 2], di
        jne    SHORT loc_1_1B80
loc_1_1BA9:
        mov    word ptr [ebp - 4], 0a000h
        mov    cx, word ptr [esi + 11fh]
        mov    al, byte ptr [ebp - 4]
        lea    edx, [ecx + 6]
        out    dx, al
        lea    edx, [ecx + 7]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    al, 1
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_1BD0:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 1eh]
        cmp    dword ptr [ecx + 22h], eax
        lea    edx, [ecx + 26h]
        jne    SHORT loc_1_1BF2
        mov    dword ptr [edx], 0
        cmp    dword ptr [edx], 1
        sbb    al, al
        inc    al
        pop    ebp
        ret    4
loc_1_1BF2:
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ecx + 1eh]
        pop    ebp
        mov    eax, dword ptr [eax]
        mov    dword ptr [ecx + 1eh], eax
        cmp    dword ptr [edx], 1
        sbb    al, al
        inc    al
        ret    4
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1C10:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0eah], 1
        mov    edx, dword ptr [esi + 11fh]
        jne    SHORT loc_1_1C48
        add    dx, 9
        in     al, dx
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 9
        or     al, 20h
        out    dx, al
        mov    byte ptr [esi + 19h], 0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1C48:
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 6], al
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 5], al
        mov    ax, word ptr [ebp - 6]
        or     ax, 8
        mov    word ptr [ebp - 4], ax
        mov    cx, word ptr [esi + 11fh]
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    byte ptr [esi + 19h], 0
        mov    al, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1C90:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        sub    bl, bl
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_19C0
        cmp    byte ptr [esi + 19h], bl
        jne    SHORT loc_1_1D13
        push   esi
        call   sub_1_1B10
        cmp    al, bl
        je     SHORT loc_1_1D13
        cmp    byte ptr [esi + 0eah], 1
        mov    edx, dword ptr [esi + 11fh]
        je     SHORT loc_1_1CF8
        add    dx, 8
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 3], al
        and    word ptr [ebp - 4], 0fff7h
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [ebp - 6], ax
        mov    cx, word ptr [esi + 11fh]
        lea    edx, [ecx + 8]
        out    dx, al
        lea    edx, [ecx + 9]
        mov    al, byte ptr [ebp - 5]
        jmp    SHORT loc_1_1D0C
loc_1_1CF8:
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 9
        and    al, 0dfh
loc_1_1D0C:
        out    dx, al
        mov    byte ptr [esi + 19h], 1
        mov    bl, 1
loc_1_1D13:
        cmp    byte ptr [esi + 0eah], 1
        mov    edx, dword ptr [esi + 11fh]
        je     SHORT loc_1_1D4A
        add    dx, 0fh
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 0fh
        mov    cl, al
        mov    al, cl
        or     al, 80h
        out    dx, al
        mov    al, cl
        and    al, 7fh
        out    dx, al
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_1_1D4A:
        add    dx, 9
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    edx, dword ptr [esi + 11fh]
        add    dx, 9
        or     al, 80h
        out    dx, al
        mov    al, byte ptr [ebp - 1]
        and    al, 7fh
        out    dx, al
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  090h
loc_1_1D70:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        push   esi
        db  068h, 0D0h, 01Bh, 000h, 000h
        push   esi
        call   sub_1_05FA
        or     al, al
        je     NEAR PTR loc_1_1E77
        mov    edi, 80h
loc_1_1D95:
        mov    ebx, dword ptr [esi + 26h]
        mov    dword ptr [esi + 26h], 0
        cmp    byte ptr [esi + 11ah], 0
        mov    ax, word ptr [ebx + 4]
        je     SHORT loc_1_1E10
        and    al, 0fh
        cmp    al, 8
        jne    NEAR PTR loc_1_1E4B
        cmp    word ptr [ebx + 6], 300h
        jne    NEAR PTR loc_1_1E4B
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0a3h]
        push   eax
        call   sub_1_05E8
        cmp    word ptr [ebx + 8], di
        jne    SHORT loc_1_1DE4
        push   0
        push   1
        push   esi
        call   sub_1_16B0
        jmp    SHORT loc_1_1E4B
loc_1_1DE4:
        mov    ax, word ptr [esi + 0b1h]
        or     ax, ax
        je     SHORT loc_1_1E01
        dec    ax
        mov    word ptr [esi + 0b1h], ax
        push   esi
        call   sub_1_0940
        jmp    SHORT loc_1_1E4B
loc_1_1E01:
        lea    eax, [ebx + 6]
        push   eax
        push   0
        push   esi
        call   sub_1_16B0
        jmp    SHORT loc_1_1E4B
        db  090h
loc_1_1E10:
        and    eax, 0fh
        cmp    eax, 0ch
        ja     SHORT loc_1_1E4B
        db  0FFh, 024h, 085h, 08Ch, 01Eh, 000h, 000h
        db  090h
loc_1_1E20:
        push   ebx
        push   esi
        call   sub_1_2650
        jmp    SHORT loc_1_1E4B
loc_1_1E29:
        push   ebx
        push   esi
        call   sub_1_2500
        jmp    SHORT loc_1_1E4B
loc_1_1E32:
        push   ebx
        push   esi
        call   sub_1_2060
        jmp    SHORT loc_1_1E4B
loc_1_1E3B:
        push   ebx
        push   esi
        call   sub_1_22E0
        jmp    SHORT loc_1_1E4B
loc_1_1E44:
        push   ebx
        push   esi
        call   sub_1_1EC0
loc_1_1E4B:
        push   esi
        db  068h, 0C0h, 019h, 000h, 000h
        push   esi
        call   sub_1_05FA
        push   esi
        db  068h, 010h, 01Bh, 000h, 000h
        push   esi
        call   sub_1_05FA
        push   esi
        db  068h, 0D0h, 01Bh, 000h, 000h
        push   esi
        call   sub_1_05FA
        or     al, al
        jne    NEAR PTR loc_1_1D95
loc_1_1E77:
        push   esi
        db  068h, 010h, 01Ch, 000h, 000h
        push   esi
        call   sub_1_05FA
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    10h
        db  020h, 01Eh, 000h, 000h, 04Bh, 01Eh, 000h, 000h, 04Bh, 01Eh, 000h, 000h, 04Bh, 01Eh, 000h, 000h
        db  029h, 01Eh, 000h, 000h, 04Bh, 01Eh, 000h, 000h, 04Bh, 01Eh, 000h, 000h, 04Bh, 01Eh, 000h, 000h
        db  032h, 01Eh, 000h, 000h, 04Bh, 01Eh, 000h, 000h, 03Bh, 01Eh, 000h, 000h, 04Bh, 01Eh, 000h, 000h
        db  044h, 01Eh, 000h, 000h
sub_1_1EC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 46h]
        mov    ecx, edx
        mov    eax, dword ptr [edx + 0ch]
        mov    dword ptr [ebp - 4], edx
        mov    di, word ptr [eax + 4]
        test   di, 20h
        je     SHORT loc_1_1F14
        mov    edx, dword ptr [edx + 4]
        mov    dword ptr [ebp - 0ch], edx
        mov    edx, dword ptr [ebp - 4]
        test   di, 4
        mov    edx, dword ptr [edx]
        mov    dword ptr [ebp - 10h], edx
        mov    word ptr [eax + 6], 0
        je     SHORT loc_1_1F05
        inc    dword ptr [esi + 1d4h]
        jmp    SHORT loc_1_1F22
loc_1_1F05:
        inc    dword ptr [esi + 1cch]
        mov    dword ptr [ebp - 8], 0
        jmp    SHORT loc_1_1F29
loc_1_1F14:
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 10h], 0
loc_1_1F22:
        mov    dword ptr [ebp - 8], 0c0000001h
loc_1_1F29:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    word ptr [eax + 8], 80h
        je     SHORT loc_1_1F41
        inc    dword ptr [esi + 1d4h]
        mov    dword ptr [ebp - 8], 0c0000001h
loc_1_1F41:
        sub    dx, dx
        cmp    dword ptr [esi + 5ah], 0
        jbe    SHORT loc_1_1FB2
        lea    ebx, [ebx]
loc_1_1F50:
        cmp    byte ptr [esi + 127h], 0
        jne    SHORT loc_1_1F6F
        mov    eax, edx
        and    eax, 0ffffh
        lea    eax, [ecx + eax*8 + 18h]
        cmp    dword ptr [eax], 0
        je     SHORT loc_1_1F6F
        mov    dword ptr [eax], 0
loc_1_1F6F:
        mov    eax, edx
        and    eax, 0ffffh
        lea    eax, [eax + eax*2]
        lea    edi, [eax*2]
        mov    eax, dword ptr [ecx + 0ch]
        mov    word ptr [edi + eax + 8], 0
        mov    eax, dword ptr [ecx + 0ch]
        add    eax, edi
        inc    dx
        mov    bx, word ptr [eax + 8]
        mov    word ptr [eax + 0ch], bx
        mov    eax, dword ptr [ecx + 0ch]
        add    eax, edi
        mov    di, word ptr [eax + 0ch]
        mov    word ptr [eax + 0ah], di
        mov    eax, edx
        and    eax, 0ffffh
        cmp    eax, dword ptr [esi + 5ah]
        jb     SHORT loc_1_1F50
loc_1_1FB2:
        mov    eax, dword ptr [ecx + 0ch]
        test   byte ptr [eax + 4], 10h
        jne    SHORT loc_1_1FC0
        mov    ecx, dword ptr [ecx + 8]
        jmp    SHORT loc_1_1F41
loc_1_1FC0:
        mov    eax, dword ptr [ecx + 0ch]
        mov    cx, word ptr [eax + 2]
        add    eax, 2
        test   ch, 1
        je     SHORT loc_1_1FD7
        and    cx, 0feffh
        mov    word ptr [eax], cx
loc_1_1FD7:
        mov    eax, dword ptr [ebp - 4]
        lea    ebx, [ebx]
loc_1_1FE0:
        mov    edx, dword ptr [eax + 0ch]
        mov    cx, word ptr [edx + 4]
        add    edx, 4
        mov    word ptr [edx], 0
        inc    dword ptr [esi + 52h]
        test   cl, 10h
        mov    eax, dword ptr [eax + 8]
        jne    SHORT loc_1_2000
        jmp    SHORT loc_1_1FE0
        db  08Dh, 064h, 024h, 000h
loc_1_2000:
        mov    dword ptr [esi + 46h], eax
        mov    dword ptr [esi + 3eh], 0
        mov    edx, dword ptr [ebp - 0ch]
        mov    dword ptr [esi + 19bh], 0
        mov    edi, dword ptr [edx + 28h]
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   edx
        mov    edx, dword ptr [ebp - 10h]
        push   edx
        call   sub_1_057A
        dec    dword ptr [edi + 14h]
        mov    eax, dword ptr [esi + 0c7h]
        sub    eax, esi
        cmp    eax, 0c7h
        je     SHORT loc_1_2040
        push   esi
        call   sub_1_2660
loc_1_2040:
        push   1
        push   esi
        call   sub_1_3D50
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2060:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0ch]
        mov    ax, word ptr [edi + 8]
        and    ax, 0ffh
        sub    ax, 80h
        cmp    ax, 1
        sbb    bl, bl
        neg    bl
        xor    eax, eax
        mov    ax, word ptr [edi + 6]
        cmp    eax, 300h
        je     SHORT loc_1_20F0
        cmp    eax, 800h
        je     NEAR PTR loc_1_22C0
        cmp    eax, 900h
        je     NEAR PTR loc_1_22C0
        cmp    eax, 0a00h
        je     NEAR PTR loc_1_22C0
        cmp    eax, 0d00h
        je     NEAR PTR loc_1_22C0
        mov    esi, dword ptr [ebp + 8]
loc_1_20BC:
        mov    eax, dword ptr [esi + 0c7h]
        sub    eax, esi
        cmp    eax, 0c7h
        je     SHORT loc_1_20D1
        push   esi
        call   sub_1_2660
loc_1_20D1:
        cmp    byte ptr [esi + 0e7h], 0
        je     SHORT loc_1_20E7
        mov    byte ptr [esi + 0e7h], 0
        push   esi
        call   sub_1_1570
loc_1_20E7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_20F0:
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 11bh], 0
        je     SHORT loc_1_20BC
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0a3h]
        push   eax
        call   sub_1_05E8
        mov    ax, word ptr [edi + 8]
        mov    word ptr [esi + 0afh], ax
        or     bl, bl
        je     NEAR PTR loc_1_2220
        mov    byte ptr [esi + 11eh], 0
        mov    byte ptr [esi + 11dh], 0
        mov    byte ptr [esi + 11bh], 0
        mov    edi, dword ptr [esi + 0bfh]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0ffffff41h
        je     SHORT loc_1_216B
loc_1_2145:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        cmp    byte ptr [eax + 18h], 0
        je     SHORT loc_1_2160
        mov    byte ptr [eax + 18h], 0
        push   0
        push   0
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_056E
loc_1_2160:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0ffffff41h
        jne    SHORT loc_1_2145
loc_1_216B:
        push   esi
        call   sub_1_0E00
        mov    ecx, dword ptr [esi + 167h]
        mov    ax, word ptr [ecx]
        mov    word ptr [ebp - 4], ax
        sub    eax, eax
loc_1_2180:
        cmp    word ptr [ebp - 4], ax
        jne    SHORT loc_1_2180
        mov    word ptr [ecx], 600h
        cmp    byte ptr [esi + 127h], 0
        je     SHORT loc_1_21C0
        mov    eax, dword ptr [esi + 7ah]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 167h]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 7ah]
        jmp    SHORT loc_1_21EA
loc_1_21C0:
        mov    eax, dword ptr [esi + 7eh]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 167h]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 7eh]
loc_1_21EA:
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 167h]
        push   esi
        mov    word ptr [eax + 4], cx
        db  068h, 000h, 019h, 000h, 000h
        push   esi
        call   sub_1_05FA
        jmp    NEAR PTR loc_1_20BC
        db  005h, 000h, 000h, 000h, 000h
loc_1_2220:
        mov    ax, word ptr [esi + 0b1h]
        or     ax, ax
        je     SHORT loc_1_2240
        dec    ax
        mov    word ptr [esi + 0b1h], ax
        push   esi
        call   sub_1_0940
        jmp    NEAR PTR loc_1_20BC
loc_1_2240:
        mov    byte ptr [esi + 11bh], 0
        mov    byte ptr [esi + 11eh], 1
        mov    byte ptr [esi + 11dh], 1
        mov    edi, dword ptr [esi + 0bfh]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0ffffff41h
        je     SHORT loc_1_228F
loc_1_2266:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        cmp    byte ptr [eax + 18h], 0
        je     SHORT loc_1_2284
        mov    byte ptr [eax + 18h], 0
        push   0
        push   0c0000001h
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_056E
loc_1_2284:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0ffffff41h
        jne    SHORT loc_1_2266
loc_1_228F:
        xor    eax, eax
        mov    ax, word ptr [esi + 0afh]
        push   eax
        push   0fh
        push   1
        push   3
        push   0aa000003h
        mov    eax, dword ptr [esi + 0f8h]
        push   eax
        call   sub_1_06D8
        add    esp, 18h
        push   esi
        call   sub_1_0C90
        jmp    NEAR PTR loc_1_20BC
        db  08Bh, 0FFh
loc_1_22C0:
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_3470
        or     al, al
        je     NEAR PTR loc_1_20BC
        push   esi
        call   sub_1_3170
        jmp    NEAR PTR loc_1_20BC
        db  08Dh, 049h, 000h
sub_1_22E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    byte ptr [ebp - 3], 1
        mov    ax, word ptr [eax + 8]
        cmp    ax, 80h
        jne    SHORT loc_1_2303
        mov    byte ptr [ebp - 4], 0
        mov    edi, dword ptr [ebp + 8]
        jmp    SHORT loc_1_231F
loc_1_2303:
        cmp    ax, 40h
        jne    SHORT loc_1_2312
        mov    byte ptr [ebp - 4], 1
        mov    edi, dword ptr [ebp + 8]
        jmp    SHORT loc_1_231F
loc_1_2312:
        mov    edi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 4], 0
        inc    dword ptr [edi + 1d8h]
loc_1_231F:
        mov    esi, 10h
loc_1_2324:
        cmp    byte ptr [ebp - 3], 0
        je     NEAR PTR loc_1_24C9
        mov    ebx, dword ptr [edi + 6eh]
        mov    eax, dword ptr [ebx + 4]
        test   byte ptr [eax + 4], 20h
        je     NEAR PTR loc_1_24C0
        inc    dword ptr [edi + 1d0h]
        mov    eax, dword ptr [ebx + 4]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [ebp - 6], ax
        lea    eax, [ebp - 6]
        push   eax
        call   sub_1_0C70
        mov    eax, dword ptr [ebx + 0ch]
        add    eax, 8
        test   byte ptr [eax], 80h
        je     SHORT loc_1_2370
        mov    al, byte ptr [eax + 6]
        and    ax, 1fh
        mov    word ptr [ebp - 8], ax
        jmp    SHORT loc_1_2376
loc_1_2370:
        mov    word ptr [ebp - 8], 0
loc_1_2376:
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        mov    ecx, dword ptr [edi + 82h]
        cmp    ecx, eax
        jb     SHORT loc_1_2388
        mov    ecx, eax
loc_1_2388:
        mov    eax, dword ptr [edi + 23dh]
        mov    byte ptr [ebp - 2], 1
        mov    eax, dword ptr [eax + 10h]
        test   esi, eax
        je     SHORT loc_1_2408
        test   al, 20h
        jne    SHORT loc_1_2408
        mov    eax, dword ptr [ebx + 0ch]
        mov    al, byte ptr [eax + 2]
        and    al, 80h
        mov    byte ptr [ebp - 1], al
        or     al, al
        jne    SHORT loc_1_2408
        mov    edx, dword ptr [ebx + 0ch]
        lea    eax, [edi + 161h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [eax + 2]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [edx + 4]
        add    edx, 2
        mov    dword ptr [ebp - 14h], eax
        cmp    dword ptr [ebp - 10h], eax
        jae    SHORT loc_1_23D2
        mov    byte ptr [ebp - 1], 1
        jmp    SHORT loc_1_23FE
loc_1_23D2:
        mov    eax, dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 10h], eax
        jbe    SHORT loc_1_23E0
        mov    byte ptr [ebp - 1], 0ffh
        jmp    SHORT loc_1_23FE
loc_1_23E0:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dx, word ptr [edx]
        mov    ax, word ptr [eax]
        cmp    ax, dx
        jae    SHORT loc_1_23F4
        mov    byte ptr [ebp - 1], 1
        jmp    SHORT loc_1_23FE
loc_1_23F4:
        mov    byte ptr [ebp - 1], 0ffh
        ja     SHORT loc_1_23FE
        mov    byte ptr [ebp - 1], 0
loc_1_23FE:
        cmp    byte ptr [ebp - 1], 0
        je     SHORT loc_1_2408
        mov    byte ptr [ebp - 2], 0
loc_1_2408:
        cmp    byte ptr [ebp - 2], 0
        je     SHORT loc_1_246F
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        cmp    dword ptr [edi + 13ch], eax
        jb     SHORT loc_1_246F
        cmp    word ptr [ebp - 6], 0eh
        jae    SHORT loc_1_2430
        push   0
        push   0
        push   0
        push   eax
        jmp    SHORT loc_1_2452
        db  08Dh, 064h, 024h, 000h
loc_1_2430:
        mov    edx, dword ptr [ebp - 8]
        and    edx, 0ffffh
        sub    eax, edx
        sub    eax, 0eh
        push   eax
        sub    ecx, edx
        sub    ecx, 0eh
        push   ecx
        mov    eax, dword ptr [ebx + 0ch]
        add    eax, edx
        add    eax, 0eh
        push   eax
        add    edx, 0eh
        push   edx
loc_1_2452:
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        push   ebx
        mov    eax, dword ptr [edi + 23dh]
        push   eax
        call   sub_1_00B0
        mov    eax, dword ptr [edi + 23dh]
        push   eax
        call   sub_1_0300
loc_1_246F:
        mov    eax, dword ptr [ebx + 4]
        test   word ptr [eax + 4], si
        jne    SHORT loc_1_2492
        mov    ecx, 88h
        lea    ecx, [ecx]
loc_1_2480:
        mov    eax, dword ptr [ebx + 4]
        mov    word ptr [eax + 4], cx
        mov    ebx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        test   word ptr [eax + 4], si
        je     SHORT loc_1_2480
loc_1_2492:
        mov    eax, dword ptr [ebx + 4]
        mov    word ptr [eax + 4], 88h
        mov    eax, dword ptr [ebx]
        mov    dword ptr [edi + 6eh], eax
        mov    eax, dword ptr [ebx + 4]
        or     word ptr [eax + 2], 100h
        mov    eax, dword ptr [edi + 72h]
        mov    eax, dword ptr [eax + 4]
        and    word ptr [eax + 2], 0feffh
        mov    dword ptr [edi + 72h], ebx
        jmp    NEAR PTR loc_1_2324
        db  08Dh, 049h, 000h
loc_1_24C0:
        mov    byte ptr [ebp - 3], 0
        jmp    NEAR PTR loc_1_2324
loc_1_24C9:
        cmp    byte ptr [ebp - 4], 0
        je     SHORT loc_1_24DB
        push   edi
        db  068h, 060h, 019h, 000h, 000h
        push   edi
        call   sub_1_05FA
loc_1_24DB:
        mov    eax, dword ptr [edi + 0c7h]
        sub    eax, edi
        cmp    eax, 0c7h
        je     SHORT loc_1_24F0
        push   edi
        call   sub_1_2660
loc_1_24F0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2500:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], 0
        mov    bx, word ptr [eax + 8]
        mov    si, bx
        and    si, 8000h
        je     SHORT loc_1_2576
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 1cbh], 0
        jne    SHORT loc_1_2576
        mov    byte ptr [eax + 1cbh], 1
        mov    edx, eax
        add    edx, 1a3h
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0ah
        rep stosd
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edx + 10h], 8
        cmp    dword ptr [eax + 2ah], 0
        jne    SHORT loc_1_2561
        mov    dword ptr [eax + 2eh], edx
        mov    dword ptr [eax + 2ah], edx
        jmp    SHORT loc_1_256D
loc_1_2561:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 2eh], edx
        mov    dword ptr [edx], 0
loc_1_256D:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3170
loc_1_2576:
        test   bh, 20h
        je     SHORT loc_1_257F
        or     dword ptr [ebp - 4], 40h
loc_1_257F:
        test   bh, 40h
        je     SHORT loc_1_258B
        or     dword ptr [ebp - 4], 80h
loc_1_258B:
        or     si, si
        je     SHORT loc_1_2597
        or     dword ptr [ebp - 4], 100h
loc_1_2597:
        test   bl, 1
        je     SHORT loc_1_25A3
        or     dword ptr [ebp - 4], 200h
loc_1_25A3:
        test   bl, 4
        je     SHORT loc_1_25AF
        or     dword ptr [ebp - 4], 400h
loc_1_25AF:
        test   bl, 8
        je     SHORT loc_1_25BB
        or     dword ptr [ebp - 4], 800h
loc_1_25BB:
        test   bl, 10h
        je     SHORT loc_1_25C7
        or     dword ptr [ebp - 4], 1000h
loc_1_25C7:
        test   bl, 20h
        je     SHORT loc_1_25D3
        or     dword ptr [ebp - 4], 2000h
loc_1_25D3:
        test   bl, 40h
        je     SHORT loc_1_25DF
        or     dword ptr [ebp - 4], 4000h
loc_1_25DF:
        test   bl, 80h
        je     SHORT loc_1_25EB
        or     dword ptr [ebp - 4], 8000h
loc_1_25EB:
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_2638
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [eax + 0bfh]
        sub    eax, esi
        cmp    eax, 0ffffff41h
        je     SHORT loc_1_2638
loc_1_2603:
        mov    ebx, esi
        inc    dword ptr [esi + 14h]
        lea    edi, [esi + 14h]
        mov    esi, dword ptr [esi]
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   40010006h
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_05A4
        dec    dword ptr [edi]
        mov    eax, dword ptr [ebp + 8]
        sub    eax, esi
        cmp    eax, 0ffffff41h
        jne    SHORT loc_1_2603
loc_1_2638:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2650:
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2660:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 11ah], 0
        jne    NEAR PTR loc_1_2777
        mov    eax, dword ptr [edi + 0cfh]
        sub    eax, edi
        cmp    eax, 0cfh
        je     NEAR PTR loc_1_2739
        sub    esi, esi
        add    eax, 0
loc_1_2690:
        mov    ebx, dword ptr [edi + 0cfh]
        push   esi
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebx + 0ch]
        push   eax
        mov    eax, dword ptr [edi + 23dh]
        push   eax
        db  0E8h, 01Ah, 0DBh, 0FEh, 0FFh
        cmp    eax, esi
        je     SHORT loc_1_26C3
        cmp    eax, 103h
        je     SHORT loc_1_2701
        jmp    SHORT loc_1_2726
loc_1_26C3:
        cmp    dword ptr [ebx + 14h], esi
        je     SHORT loc_1_26E9
        lea    eax, [edi + 0c7h]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [edi + 0cbh]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [edi + 0cbh]
        mov    dword ptr [eax], ebx
        mov    dword ptr [edi + 0cbh], ebx
        jmp    SHORT loc_1_2726
loc_1_26E9:
        push   esi
        mov    eax, dword ptr [ebx + 10h]
        push   eax
        call   sub_1_0574
        push   esi
        push   92h
        push   ebx
        call   sub_1_060C
        jmp    SHORT loc_1_2726
loc_1_2701:
        lea    eax, [edi + 0c7h]
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [edi + 0cbh]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [edi + 0cbh]
        push   edi
        mov    dword ptr [eax], ebx
        mov    dword ptr [edi + 0cbh], ebx
        call   sub_1_3170
loc_1_2726:
        mov    eax, dword ptr [edi + 0cfh]
        sub    eax, edi
        cmp    eax, 0cfh
        jne    NEAR PTR loc_1_2690
loc_1_2739:
        mov    esi, dword ptr [edi + 0c7h]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0c7h
        je     SHORT loc_1_2777
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_1_2777
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_0574
        push   0
        push   92h
        push   esi
        call   sub_1_060C
loc_1_2777:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Bh, 0FFh
loc_1_2780:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 18h], esi
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [ebp - 1ch], ecx
        cmp    byte ptr [ecx + 11eh], 0
        jne    NEAR PTR loc_1_2931
        cmp    byte ptr [eax + 19h], 0
        jne    NEAR PTR loc_1_2923
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [eax], 0
        lea    eax, [ebp - 8]
        push   0
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_0630
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_1_2912
        mov    byte ptr [ebp - 1], 0
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0654
        mov    eax, dword ptr [esi + 0ch]
        add    eax, 8
        test   byte ptr [eax], 80h
        je     SHORT loc_1_2810
        mov    al, byte ptr [eax + 6]
        and    ax, 1fh
        mov    word ptr [ebp - 4], ax
        jmp    SHORT loc_1_2816
        db  08Bh, 0FFh
loc_1_2810:
        mov    word ptr [ebp - 4], 0
loc_1_2816:
        mov    edx, dword ptr [ebp - 4]
        and    edx, 0ffffh
        mov    eax, dword ptr [esi + 0ch]
        mov    ecx, dword ptr [ebp - 1ch]
        lea    edi, [edx + eax + 0eh]
        mov    esi, dword ptr [ecx + 82h]
        sub    esi, edx
        sub    esi, 0eh
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_290A
        mov    edi, edi
loc_1_2840:
        cmp    dword ptr [ebp - 0ch], 0
        jne    SHORT loc_1_2873
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0678
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_290A
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0654
        jmp    NEAR PTR loc_1_28FE
loc_1_2873:
        or     esi, esi
        jne    SHORT loc_1_28B0
        inc    byte ptr [ebp - 1]
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 1ch]
        mov    al, byte ptr [ebp - 1]
        cmp    eax, dword ptr [ecx + 86h]
        jne    SHORT loc_1_2896
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [ebp - 1], 0
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 18h], eax
loc_1_2896:
        mov    ecx, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [ebp - 18h]
        mov    esi, dword ptr [ecx + 82h]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 1]
        mov    edi, dword ptr [eax + ecx*8 + 0ch]
        jmp    SHORT loc_1_28FE
        db  08Dh, 049h, 000h
loc_1_28B0:
        cmp    dword ptr [ebp + 10h], 0
        je     SHORT loc_1_28CF
        cmp    esi, dword ptr [ebp + 10h]
        jae    SHORT loc_1_28C2
        sub    dword ptr [ebp + 10h], esi
        sub    esi, esi
        jmp    SHORT loc_1_28FE
loc_1_28C2:
        add    edi, dword ptr [ebp + 10h]
        sub    esi, dword ptr [ebp + 10h]
        mov    dword ptr [ebp + 10h], 0
loc_1_28CF:
        mov    ebx, dword ptr [ebp + 14h]
        sub    ebx, dword ptr [ebp - 10h]
        mov    eax, esi
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    eax, ecx
        jb     SHORT loc_1_28E0
        mov    eax, ecx
loc_1_28E0:
        cmp    ebx, eax
        jb     SHORT loc_1_28E6
        mov    ebx, eax
loc_1_28E6:
        push   ebx
        push   edi
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_06EC
        add    dword ptr [ebp - 14h], ebx
        add    edi, ebx
        add    dword ptr [ebp - 10h], ebx
        sub    esi, ebx
        sub    dword ptr [ebp - 0ch], ebx
loc_1_28FE:
        mov    ecx, dword ptr [ebp - 10h]
        cmp    ecx, dword ptr [ebp + 14h]
        jb     NEAR PTR loc_1_2840
loc_1_290A:
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dword ptr [eax], ecx
loc_1_2912:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        dec    dword ptr [eax + 14h]
        sub    eax, eax
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_2923:
        mov    eax, 0c001000ch
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_2931:
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    byte ptr [ecx + 11ah], 0
        je     SHORT loc_1_2950
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  005h, 000h, 000h, 000h, 000h
loc_1_2950:
        mov    ecx, dword ptr [ebp - 1ch]
        mov    eax, 0c0000001h
        cmp    byte ptr [ecx + 11bh], 0
        je     SHORT loc_1_296A
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
loc_1_296A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    18h
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2980:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0ch]
        cmp    byte ptr [esi + 11ah], 0
        je     SHORT loc_1_29A7
        push   esi
        call   sub_1_0C90
        push   0
        push   0
        push   esi
        call   sub_1_16B0
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
loc_1_29A7:
        cmp    byte ptr [esi + 11bh], 0
        je     SHORT loc_1_2A05
        mov    byte ptr [esi + 11eh], 1
        mov    byte ptr [esi + 11dh], 1
        mov    byte ptr [esi + 11bh], 0
        mov    edi, dword ptr [esi + 0bfh]
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0ffffff41h
        je     SHORT loc_1_29FF
loc_1_29D6:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        cmp    byte ptr [eax + 18h], 0
        je     SHORT loc_1_29F4
        mov    byte ptr [eax + 18h], 0
        push   0
        push   0c0000001h
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_056E
loc_1_29F4:
        mov    eax, esi
        sub    eax, edi
        cmp    eax, 0ffffff41h
        jne    SHORT loc_1_29D6
loc_1_29FF:
        push   esi
        call   sub_1_0C90
loc_1_2A05:
        pop    edi
        pop    esi
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h
loc_1_2A10:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0ch]
        mov    edx, dword ptr [ecx + 8]
        mov    eax, dword ptr [esi + 10h]
        or     eax, eax
        jne    SHORT loc_1_2A46
        lea    eax, [esi + 20h]
        push   eax
        lea    eax, [esi + 24h]
        push   eax
        mov    eax, dword ptr [esi + 1ch]
        push   eax
        mov    eax, dword ptr [esi + 18h]
        push   eax
        push   0
        mov    eax, dword ptr [esi + 14h]
        push   eax
        push   ecx
        push   edx
        call   sub_1_2D10
        pop    esi
        pop    ebp
        ret    8
loc_1_2A46:
        cmp    eax, 1
        jne    SHORT loc_1_2AA0
        cmp    byte ptr [edx + 11eh], 0
        je     SHORT loc_1_2A83
        cmp    byte ptr [edx + 11ah], 0
        je     SHORT loc_1_2A67
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    8
loc_1_2A67:
        cmp    byte ptr [edx + 11dh], 0
        mov    eax, 0c0000001h
        pop    esi
        pop    ebp
        je     SHORT loc_1_2A80
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2A80:
        ret    8
loc_1_2A83:
        cmp    byte ptr [ecx + 19h], 0
        je     SHORT loc_1_2A93
        mov    eax, 0c0010002h
        pop    esi
        pop    ebp
        ret    8
loc_1_2A93:
        push   esi
        push   ecx
        push   edx
        call   sub_1_2AE0
        pop    esi
        pop    ebp
        ret    8
loc_1_2AA0:
        mov    eax, 10001h
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2AB0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_1_3680
        mov    esi, eax
        cmp    esi, 103h
        jne    SHORT loc_1_2AD2
        push   edi
        call   sub_1_3170
loc_1_2AD2:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_2AE0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        sub    ebx, ebx
        mov    edi, dword ptr [esi + 1ch]
        mov    ecx, dword ptr [esi + 18h]
        mov    eax, dword ptr [esi + 14h]
        cmp    edi, 4
        je     SHORT loc_1_2B10
        mov    dword ptr [esi + 20h], ebx
        mov    dword ptr [esi + 24h], ebx
        mov    ebx, 0c0010014h
        jmp    NEAR PTR loc_1_2B97
        db  08Dh, 064h, 024h, 000h
loc_1_2B10:
        cmp    eax, 1010eh
        je     SHORT loc_1_2B41
        cmp    eax, 1010fh
        je     SHORT loc_1_2B61
        cmp    eax, 2010103h
        je     SHORT loc_1_2B75
        cmp    eax, 2010104h
        je     SHORT loc_1_2B86
        mov    ebx, 0c0010017h
        mov    dword ptr [esi + 20h], 0
        mov    dword ptr [esi + 24h], 0
        jmp    SHORT loc_1_2B97
loc_1_2B41:
        push   4
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_06EC
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3850
        jmp    SHORT loc_1_2B95
loc_1_2B61:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        cmp    dword ptr [eax + 82h], ecx
        jae    SHORT loc_1_2B97
        mov    ebx, 0c0010014h
        jmp    SHORT loc_1_2B97
loc_1_2B75:
        push   ecx
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3780
        jmp    SHORT loc_1_2B95
loc_1_2B86:
        push   ecx
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_36D0
loc_1_2B95:
        mov    ebx, eax
loc_1_2B97:
        or     ebx, ebx
        jne    SHORT loc_1_2BA5
        mov    dword ptr [esi + 20h], edi
        mov    dword ptr [esi + 24h], 0
loc_1_2BA5:
        cmp    ebx, 103h
        jne    SHORT loc_1_2BB6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3170
loc_1_2BB6:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0ch
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_2BD0:
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
        call   sub_1_2D10
        mov    edi, eax
        cmp    edi, 0c00000bbh
        jne    NEAR PTR loc_1_2CF0
        sub    edi, edi
        mov    eax, dword ptr [esi + 14h]
        cmp    eax, 20105h
        ja     SHORT loc_1_2C56
        je     SHORT loc_1_2C98
        cmp    eax, 20101h
        je     SHORT loc_1_2C72
        cmp    eax, 20102h
        je     SHORT loc_1_2C80
        cmp    eax, 20103h
        je     SHORT loc_1_2C88
        cmp    eax, 20104h
        je     SHORT loc_1_2C90
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_2CC1
loc_1_2C56:
        cmp    eax, 2010105h
        je     SHORT loc_1_2CA0
        cmp    eax, 2020101h
        je     SHORT loc_1_2CB0
        cmp    eax, 2020102h
        je     SHORT loc_1_2CB8
        mov    edi, 0c0010017h
        jmp    SHORT loc_1_2CC1
loc_1_2C72:
        mov    eax, dword ptr [ebx + 1cch]
        jmp    SHORT loc_1_2CBE
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_2C80:
        mov    eax, dword ptr [ebx + 1d0h]
        jmp    SHORT loc_1_2CBE
loc_1_2C88:
        mov    eax, dword ptr [ebx + 1d4h]
        jmp    SHORT loc_1_2CBE
loc_1_2C90:
        mov    eax, dword ptr [ebx + 1d8h]
        jmp    SHORT loc_1_2CBE
loc_1_2C98:
        mov    eax, dword ptr [ebx + 1f4h]
        jmp    SHORT loc_1_2CBE
loc_1_2CA0:
        xor    eax, eax
        mov    ax, word ptr [ebx + 0afh]
        or     eax, 0c0011000h
        jmp    SHORT loc_1_2CBE
loc_1_2CB0:
        mov    eax, dword ptr [ebx + 1dch]
        jmp    SHORT loc_1_2CBE
loc_1_2CB8:
        mov    eax, dword ptr [ebx + 1f0h]
loc_1_2CBE:
        mov    dword ptr [ebp - 4], eax
loc_1_2CC1:
        or     edi, edi
        jne    SHORT loc_1_2CF0
        cmp    dword ptr [ebp - 10h], 4
        jb     SHORT loc_1_2CE0
        push   4
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_06EC
        add    dword ptr [ebp - 0ch], 4
        jmp    SHORT loc_1_2CF0
loc_1_2CE0:
        mov    eax, 4
        sub    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 8], eax
        mov    edi, 0c0010014h
loc_1_2CF0:
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
sub_1_2D10:
        push   ebp
        mov    ebp, esp
        sub    esp, 68h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 1
        sub    ebx, ebx
        lea    edi, [ebp - 8]
        mov    esi, 4
        mov    eax, dword ptr [ebp + 10h]
        cmp    eax, 2010101h
        ja     SHORT loc_1_2D50
        je     NEAR PTR loc_1_2FA0
        sub    eax, 10101h
        cmp    eax, 12h
        ja     SHORT loc_1_2D71
        shl    eax, 2
        db  0FFh, 0A0h, 0CCh, 030h, 000h, 000h
        db  08Dh, 049h, 000h
loc_1_2D50:
        cmp    eax, 2010102h
        je     NEAR PTR loc_1_2FE0
        cmp    eax, 2010103h
        je     NEAR PTR loc_1_3020
        cmp    eax, 2010104h
        je     NEAR PTR loc_1_3061
loc_1_2D71:
        mov    ebx, 0c00000bbh
        jmp    NEAR PTR loc_1_3093
        db  005h, 000h, 000h, 000h, 000h
loc_1_2D80:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_2D95
        db  0BFh, 090h, 044h, 000h, 000h
        mov    esi, 5ch
        jmp    NEAR PTR loc_1_3093
loc_1_2D95:
        db  0BFh, 014h, 044h, 000h, 000h
        mov    esi, 7ch
        jmp    NEAR PTR loc_1_3093
loc_1_2DA4:
        mov    ecx, dword ptr [ebp + 8]
        cmp    byte ptr [ecx + 11ah], 0
        je     SHORT loc_1_2DC0
        mov    dword ptr [ebp - 0ch], 2
        jmp    SHORT loc_1_2DE9
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_2DC0:
        cmp    byte ptr [ecx + 11bh], 0
        je     SHORT loc_1_2DD2
        mov    dword ptr [ebp - 0ch], 1
        jmp    SHORT loc_1_2DE9
loc_1_2DD2:
        cmp    byte ptr [ecx + 11eh], 0
        mov    dword ptr [ebp - 0ch], 4
        jne    SHORT loc_1_2DE9
        mov    dword ptr [ebp - 0ch], 0
loc_1_2DE9:
        lea    edi, [ebp - 0ch]
        mov    esi, 4
        jmp    NEAR PTR loc_1_3093
loc_1_2DF6:
        lea    edi, [ebp - 10h]
        mov    esi, 4
        jmp    NEAR PTR loc_1_3093
loc_1_2E03:
        lea    edi, [ebp - 10h]
        mov    esi, 4
        jmp    NEAR PTR loc_1_3093
loc_1_2E10:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 82h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3093
loc_1_2E21:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 13ch]
        sub    eax, 0eh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3093
loc_1_2E35:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 130h]
        sub    eax, 10h
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 1d4c0h
        add    eax, 9c40h
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3093
loc_1_2E58:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 13ch]
        imul   eax, dword ptr [ecx + 56h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3093
        db  08Dh, 049h, 000h
loc_1_2E70:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 82h]
        imul   eax, dword ptr [ecx + 86h]
        imul   eax, dword ptr [ecx + 76h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3093
        db  08Dh, 064h, 024h, 000h
loc_1_2E90:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 13ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3093
loc_1_2EA1:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 13ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3093
loc_1_2EB2:
        push   3
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 15bh
        push   ecx
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_06EC
        and    dword ptr [ebp - 8], 0ffffff00h
        mov    esi, 3
        jmp    NEAR PTR loc_1_3093
loc_1_2ED8:
        db  0BEh, 0ECh, 044h, 000h, 000h
        lea    edi, [ebp - 40h]
        mov    ecx, 9
        rep movsd
        movsb
        db  0BEh, 014h, 045h, 000h, 000h
        lea    edi, [ebp - 68h]
        mov    ecx, 9
        rep movsd
        movsb
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 134h]
        cmp    eax, 1
        je     SHORT loc_1_2F18
        cmp    eax, 2
        je     SHORT loc_1_2F18
        lea    edi, [ebp - 68h]
        mov    esi, 25h
        jmp    NEAR PTR loc_1_3093
loc_1_2F18:
        lea    edi, [ebp - 40h]
        mov    esi, 25h
        jmp    NEAR PTR loc_1_3093
loc_1_2F25:
        cmp    byte ptr [ebp + 14h], 0
        je     SHORT loc_1_2F40
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 23dh]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3093
        db  090h
loc_1_2F40:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3093
loc_1_2F51:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 82h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3093
loc_1_2F62:
        mov    word ptr [ebp - 2], 30ah
        lea    edi, [ebp - 2]
        mov    esi, 2
        jmp    NEAR PTR loc_1_3093
loc_1_2F75:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 13ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3093
loc_1_2F86:
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_3093
loc_1_2F92:
        mov    dword ptr [ebp - 8], 6
        jmp    NEAR PTR loc_1_3093
        db  08Bh, 0FFh
loc_1_2FA0:
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 15bh]
        add    ecx, 15bh
        mov    byte ptr [ebp - 18h], al
        mov    al, byte ptr [ecx + 1]
        mov    byte ptr [ebp - 17h], al
        mov    al, byte ptr [ecx + 2]
        mov    byte ptr [ebp - 16h], al
        lea    edi, [ebp - 18h]
        mov    al, byte ptr [ecx + 3]
        mov    byte ptr [ebp - 15h], al
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [ebp - 14h], al
        mov    esi, 6
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [ebp - 13h], al
        jmp    NEAR PTR loc_1_3093
        db  08Dh, 049h, 000h
loc_1_2FE0:
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx + 161h]
        add    ecx, 161h
        mov    byte ptr [ebp - 18h], al
        mov    al, byte ptr [ecx + 1]
        mov    byte ptr [ebp - 17h], al
        mov    al, byte ptr [ecx + 2]
        mov    byte ptr [ebp - 16h], al
        lea    edi, [ebp - 18h]
        mov    al, byte ptr [ecx + 3]
        mov    byte ptr [ebp - 15h], al
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [ebp - 14h], al
        mov    esi, 6
        mov    al, byte ptr [ecx + 5]
        mov    byte ptr [ebp - 13h], al
        jmp    NEAR PTR loc_1_3093
        db  08Dh, 049h, 000h
loc_1_3020:
        cmp    byte ptr [ebp + 14h], 0
        jne    SHORT loc_1_3031
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        jmp    SHORT loc_1_303D
loc_1_3031:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 23dh]
        mov    eax, dword ptr [eax + 4]
loc_1_303D:
        mov    ecx, eax
        and    ecx, 0ff00h
        mov    edx, eax
        shl    edx, 10h
        or     ecx, edx
        shl    ecx, 8
        mov    edx, eax
        and    edx, 0ff0000h
        shr    eax, 10h
        or     edx, eax
        shr    edx, 8
        jmp    SHORT loc_1_308E
loc_1_3061:
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 23dh]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, eax
        shr    ecx, 10h
        mov    edx, eax
        and    edx, 0ff0000h
        or     ecx, edx
        shr    ecx, 8
        mov    edx, eax
        shl    edx, 10h
        and    eax, 0ff00h
        or     edx, eax
        shl    edx, 8
loc_1_308E:
        or     ecx, edx
        mov    dword ptr [ebp - 8], ecx
loc_1_3093:
        or     ebx, ebx
        jne    SHORT loc_1_30C1
        cmp    esi, dword ptr [ebp + 1ch]
        jbe    SHORT loc_1_30B1
        mov    eax, dword ptr [ebp + 20h]
        mov    ebx, 0c0010014h
        mov    dword ptr [eax], esi
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_1_30B1:
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_06EC
        mov    eax, dword ptr [ebp + 24h]
        add    dword ptr [eax], esi
loc_1_30C1:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    20h
loc_1_30CC:
        db  080h, 02Dh, 000h, 000h, 0A4h, 02Dh, 000h
        db  000h, 0F6h
        db  02Dh, 000h, 000h, 003h, 02Eh
        add    byte ptr [eax], al
        adc    byte ptr [esi], ch
        add    byte ptr [eax], al
        and    dword ptr [esi], ebp
        add    byte ptr [eax], al
        db  035h, 02Eh, 000h, 000h, 058h
        add    byte ptr cs:[eax], al
        jo     SHORT loc_1_311C
        add    byte ptr [eax], al
        nop
        add    byte ptr cs:[eax], al
        db  0A1h, 02Eh, 000h, 000h, 0B2h
        add    byte ptr cs:[eax], al
        fsubr  dword ptr [esi]
        add    byte ptr [eax], al
        db  025h, 02Fh, 000h, 000h, 051h
        das
        add    byte ptr [eax], al
        bound  ebp, qword ptr [edi]
        add    byte ptr [eax], al
        db  075h, 02Fh
        add    byte ptr [eax], al
        xchg   byte ptr [edi], ch
        add    byte ptr [eax], al
        xchg   edx, eax
        das
        add    byte ptr [eax], al
        lea    esp, [esp]
loc_1_311C:
        lea    esp, [esp]
loc_1_3120:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [eax + 0c7h]
        sub    ecx, eax
        cmp    ecx, 0c7h
        je     SHORT loc_1_3167
        cmp    dword ptr [esi + 14h], 0
        jne    SHORT loc_1_3167
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        push   0
        mov    eax, dword ptr [esi + 10h]
        push   eax
        call   sub_1_0574
        push   0
        push   92h
        push   esi
        call   sub_1_060C
loc_1_3167:
        pop    esi
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
sub_1_3170:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], 0
        cmp    dword ptr [esi + 32h], 0
        je     SHORT loc_1_31A0
        mov    dword ptr [ebp - 8], 103h
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_31A0:
        sub    ebx, ebx
loc_1_31A2:
        mov    edi, dword ptr [esi + 2ah]
        cmp    edi, ebx
        je     NEAR PTR loc_1_343C
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi + 2ah], eax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [edi + 10h]
        dec    eax
        cmp    eax, 9
        ja     SHORT loc_1_31C8
        db  0FFh, 024h, 085h, 048h, 034h, 000h, 000h
loc_1_31C8:
        mov    dword ptr [ebp - 8], 0c0000001h
        jmp    NEAR PTR loc_1_33AC
loc_1_31D4:
        mov    eax, dword ptr [edi + 14h]
        cmp    eax, 1010eh
        je     SHORT loc_1_31F1
        cmp    eax, 2010103h
        je     SHORT loc_1_324B
        cmp    eax, 2010104h
        je     SHORT loc_1_3260
        jmp    NEAR PTR loc_1_33AC
loc_1_31F1:
        mov    eax, dword ptr [edi + 8]
        push   eax
        push   esi
        call   sub_1_3AC0
        mov    dword ptr [ebp - 8], eax
        cmp    eax, ebx
        jne    SHORT loc_1_3240
        mov    word ptr [edi + 8], 2
        mov    eax, dword ptr [esi + 23dh]
        test   byte ptr [eax + 11h], 20h
        je     SHORT loc_1_3227
        push   7fffffffh
        push   esi
        call   sub_1_3A10
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_33AC
loc_1_3227:
        mov    eax, dword ptr [eax + 4]
        push   eax
        push   esi
        call   sub_1_3A10
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_33AC
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3240:
        mov    word ptr [edi + 8], 1
        jmp    NEAR PTR loc_1_33AC
loc_1_324B:
        mov    eax, dword ptr [edi + 8]
        push   eax
        push   esi
        call   sub_1_3A10
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_33AC
        db  08Dh, 049h, 000h
loc_1_3260:
        mov    eax, dword ptr [edi + 8]
        push   eax
        push   esi
        call   sub_1_3960
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_33AC
loc_1_3272:
        mov    eax, dword ptr [edi + 8]
        push   eax
        push   esi
        call   sub_1_3AC0
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_33AC
loc_1_3284:
        mov    eax, dword ptr [edi + 8]
        push   eax
        push   esi
        call   sub_1_3A10
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_33AC
loc_1_3296:
        mov    eax, dword ptr [edi + 8]
        push   eax
        push   esi
        call   sub_1_3960
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_33AC
loc_1_32A8:
        mov    eax, dword ptr [esi + 167h]
        mov    cx, word ptr [eax]
        mov    word ptr [ebp - 4], cx
loc_1_32B5:
        cmp    word ptr [ebp - 4], bx
        jne    SHORT loc_1_32B5
        mov    word ptr [eax], 0a00h
        cmp    byte ptr [esi + 127h], bl
        je     NEAR PTR loc_1_3355
        mov    eax, dword ptr [esi + 18bh]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 167h]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 18bh]
        jmp    NEAR PTR loc_1_3385
loc_1_3301:
        mov    eax, dword ptr [esi + 167h]
        mov    cx, word ptr [eax]
        mov    word ptr [ebp - 2], cx
        mov    edi, edi
loc_1_3310:
        cmp    word ptr [ebp - 2], bx
        jne    SHORT loc_1_3310
        mov    word ptr [eax], 0a00h
        cmp    byte ptr [esi + 127h], bl
        je     SHORT loc_1_3355
        mov    eax, dword ptr [esi + 18bh]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 167h]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 18bh]
        jmp    SHORT loc_1_3385
loc_1_3355:
        mov    eax, dword ptr [esi + 18fh]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 167h]
        mov    word ptr [eax + 2], cx
        mov    ax, word ptr [esi + 18fh]
loc_1_3385:
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [esi + 167h]
        mov    dword ptr [ebp - 8], 103h
        mov    word ptr [eax + 4], cx
loc_1_33AC:
        cmp    dword ptr [ebp - 8], ebx
        jne    SHORT loc_1_3420
        mov    eax, dword ptr [edi + 10h]
        cmp    eax, 1
        je     SHORT loc_1_33D0
        cmp    eax, 4
        je     SHORT loc_1_33F0
        cmp    eax, 9
        je     SHORT loc_1_3400
        cmp    eax, 0ah
        je     SHORT loc_1_3410
        jmp    NEAR PTR loc_1_31A2
        db  08Dh, 049h, 000h
loc_1_33D0:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   edi
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_058C
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_31A2
        db  08Dh, 064h, 024h, 000h
loc_1_33F0:
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_31A2
        db  005h, 000h, 000h, 000h, 000h
loc_1_3400:
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_31A2
        db  005h, 000h, 000h, 000h, 000h
loc_1_3410:
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_31A2
        db  005h, 000h, 000h, 000h, 000h
loc_1_3420:
        cmp    dword ptr [ebp - 8], 103h
        jne    NEAR PTR loc_1_31A2
        mov    dword ptr [esi + 32h], edi
        push   esi
        db  068h, 000h, 019h, 000h, 000h
        push   esi
        call   sub_1_05FA
loc_1_343C:
        mov    eax, dword ptr [ebp - 8]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
        db  0D4h, 031h, 000h, 000h, 0A8h, 032h, 000h, 000h, 0C8h, 031h, 000h, 000h, 072h, 032h, 000h, 000h
        db  0C8h, 031h, 000h, 000h, 0C8h, 031h, 000h, 000h, 0C8h, 031h, 000h, 000h, 001h, 033h, 000h, 000h
        db  084h, 032h, 000h, 000h, 096h, 032h, 000h, 000h
sub_1_3470:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 32h]
        or     edi, edi
        je     NEAR PTR loc_1_363D
        mov    dword ptr [esi + 32h], 0
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [edi + 10h]
        dec    eax
        cmp    eax, 9
        ja     NEAR PTR loc_1_362B
        db  0FFh, 024h, 085h, 050h, 036h, 000h, 000h
loc_1_34A8:
        mov    eax, dword ptr [edi + 14h]
        cmp    eax, 1010eh
        je     SHORT loc_1_34F2
        cmp    eax, 2010103h
        je     NEAR PTR loc_1_3571
        cmp    eax, 2010104h
        je     NEAR PTR loc_1_3590
        mov    bl, byte ptr [ebp + 0ch]
loc_1_34CB:
        cmp    dword ptr [esi + 32h], 0
        jne    NEAR PTR loc_1_363D
        cmp    bl, 1
        sbb    eax, eax
        and    eax, 0c0000001h
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   edi
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_058C
        jmp    NEAR PTR loc_1_362B
loc_1_34F2:
        mov    bl, byte ptr [ebp + 0ch]
        or     bl, bl
        je     SHORT loc_1_34CB
        xor    eax, eax
        mov    ax, word ptr [edi + 8]
        cmp    eax, 1
        je     SHORT loc_1_3510
        cmp    eax, 2
        je     SHORT loc_1_3560
        jmp    SHORT loc_1_34CB
        db  005h, 000h, 000h, 000h, 000h
loc_1_3510:
        mov    ax, word ptr [esi + 10ch]
        mov    word ptr [esi + 102h], ax
        mov    eax, dword ptr [esi + 23dh]
        test   byte ptr [eax + 11h], 20h
        je     SHORT loc_1_3531
        mov    eax, 7fffffffh
        jmp    SHORT loc_1_3534
loc_1_3531:
        mov    eax, dword ptr [eax + 4]
loc_1_3534:
        push   eax
        push   esi
        call   sub_1_3A10
        cmp    eax, 103h
        jne    SHORT loc_1_34CB
        mov    dword ptr [esi + 32h], edi
        mov    word ptr [edi + 8], 2
        push   esi
        db  068h, 000h, 019h, 000h, 000h
        push   esi
        call   sub_1_05FA
        jmp    NEAR PTR loc_1_34CB
        db  08Dh, 064h, 024h, 000h
loc_1_3560:
        mov    eax, dword ptr [esi + 10eh]
        mov    dword ptr [esi + 104h], eax
        jmp    NEAR PTR loc_1_34CB
loc_1_3571:
        mov    bl, byte ptr [ebp + 0ch]
        or     bl, bl
        je     NEAR PTR loc_1_34CB
        mov    eax, dword ptr [esi + 10eh]
        mov    dword ptr [esi + 104h], eax
        jmp    NEAR PTR loc_1_34CB
        db  08Dh, 049h, 000h
loc_1_3590:
        mov    bl, byte ptr [ebp + 0ch]
        or     bl, bl
        je     NEAR PTR loc_1_34CB
        mov    eax, dword ptr [esi + 112h]
        mov    dword ptr [esi + 108h], eax
        jmp    NEAR PTR loc_1_34CB
        db  08Dh, 064h, 024h, 000h
loc_1_35B0:
        cmp    byte ptr [ebp + 0ch], 0
        mov    eax, 0c0000001h
        je     SHORT loc_1_35CA
        push   esi
        call   sub_1_3B90
        push   edi
        push   esi
        call   sub_1_2BD0
        sub    eax, eax
loc_1_35CA:
        push   eax
        push   edi
        mov    eax, dword ptr [esi + 0fch]
        push   eax
        call   sub_1_05AA
        jmp    SHORT loc_1_362B
loc_1_35DA:
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_1_362B
        mov    ax, word ptr [esi + 10ch]
        mov    word ptr [esi + 102h], ax
        jmp    SHORT loc_1_362B
loc_1_35F0:
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_1_35FC
        push   esi
        call   sub_1_3B90
loc_1_35FC:
        mov    byte ptr [esi + 1cbh], 0
        jmp    SHORT loc_1_362B
loc_1_3605:
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_1_362B
        mov    eax, dword ptr [esi + 10eh]
        mov    dword ptr [esi + 104h], eax
        jmp    SHORT loc_1_362B
loc_1_3619:
        cmp    byte ptr [ebp + 0ch], 0
        je     SHORT loc_1_362B
        mov    eax, dword ptr [esi + 112h]
        mov    dword ptr [esi + 108h], eax
loc_1_362B:
        cmp    dword ptr [esi + 32h], 0
        jne    SHORT loc_1_363D
        cmp    dword ptr [ebp - 4], 0
        je     SHORT loc_1_363D
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 14h]
loc_1_363D:
        cmp    dword ptr [esi + 32h], 1
        sbb    al, al
        neg    al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 0A8h, 034h, 000h, 000h, 0B0h, 035h, 000h, 000h, 02Bh, 036h, 000h, 000h, 0DAh, 035h
        db  000h, 000h, 02Bh, 036h, 000h, 000h, 02Bh, 036h, 000h, 000h, 02Bh, 036h, 000h, 000h, 0F0h, 035h
        db  000h, 000h, 005h, 036h, 000h, 000h, 019h, 036h, 000h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h
        db  024h, 000h
sub_1_3680:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    byte ptr [eax + 11ah], 0
        je     SHORT loc_1_3698
        mov    eax, 0c001000dh
        pop    ebp
        ret    8
loc_1_3698:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx], 0
        cmp    dword ptr [eax + 2ah], 0
        je     SHORT loc_1_36C0
        mov    eax, dword ptr [eax + 2eh]
        pop    ebp
        mov    dword ptr [eax], ecx
        mov    eax, 103h
        ret    8
        db  08Dh, 064h, 024h, 000h
loc_1_36C0:
        mov    dword ptr [eax + 2eh], ecx
        mov    dword ptr [eax + 2ah], ecx
        mov    eax, 103h
        pop    ebp
        ret    8
        db  090h
sub_1_36D0:
        push   ebp
        mov    ebp, esp
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   ecx
        mov    eax, dword ptr [eax + 23dh]
        push   eax
        db  0E8h, 06Dh, 0CCh, 0FEh, 0FFh
        pop    ebp
        ret    10h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3700:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ecx + 8]
        or     esi, esi
        jne    SHORT loc_1_371B
        mov    dword ptr [ecx + 7ah], 0ah
        lea    esi, [ecx + 6ah]
loc_1_371B:
        cmp    byte ptr [edx + 11ah], 0
        je     SHORT loc_1_3730
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
loc_1_3730:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [edx + 108h], eax
        jne    SHORT loc_1_3742
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    14h
loc_1_3742:
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi + 8], eax
        inc    dword ptr [ecx + 14h]
        mov    dword ptr [esi], 0
        cmp    dword ptr [edx + 2ah], 0
        je     SHORT loc_1_3766
        mov    eax, dword ptr [edx + 2eh]
        mov    dword ptr [eax], esi
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    14h
loc_1_3766:
        mov    dword ptr [edx + 2eh], esi
        mov    dword ptr [edx + 2ah], esi
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3780:
        push   ebp
        mov    ebp, esp
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        push   ecx
        mov    eax, dword ptr [eax + 23dh]
        push   eax
        db  0E8h, 01Dh, 0CBh, 0FEh, 0FFh
        pop    ebp
        ret    10h
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_37B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    ecx, dword ptr [ebp + 10h]
        mov    esi, dword ptr [ebp + 14h]
        mov    edx, dword ptr [ecx + 8]
        or     esi, esi
        jne    SHORT loc_1_37CB
        mov    dword ptr [ecx + 52h], 9
        lea    esi, [ecx + 42h]
loc_1_37CB:
        cmp    byte ptr [edx + 11ah], 0
        je     SHORT loc_1_37E0
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
loc_1_37E0:
        mov    eax, dword ptr [edx + 23dh]
        test   byte ptr [eax + 11h], 20h
        je     SHORT loc_1_3800
        cmp    dword ptr [edx + 104h], 7fffffffh
        jne    SHORT loc_1_3800
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    14h
        db  090h
loc_1_3800:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [edx + 104h], eax
        jne    SHORT loc_1_3812
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    14h
loc_1_3812:
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 4], ecx
        inc    dword ptr [ecx + 14h]
        mov    dword ptr [esi], 0
        cmp    dword ptr [edx + 2ah], 0
        je     SHORT loc_1_3836
        mov    eax, dword ptr [edx + 2eh]
        mov    dword ptr [eax], esi
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    14h
loc_1_3836:
        mov    dword ptr [edx + 2eh], esi
        mov    dword ptr [edx + 2ah], esi
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    14h
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3850:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 14h]
        test   al, 46h
        je     SHORT loc_1_3863
        mov    eax, 0c00000bbh
        pop    ebp
        ret    10h
loc_1_3863:
        push   1
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 23dh]
        push   eax
        db  0E8h, 070h, 0CCh, 0FEh, 0FFh
        pop    ebp
        ret    10h
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3890:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 10h]
        mov    edi, dword ptr [ebp + 14h]
        or     edi, edi
        mov    ebx, dword ptr [esi + 8]
        jne    SHORT loc_1_38AD
        mov    dword ptr [esi + 2ah], 4
        lea    edi, [esi + 1ah]
loc_1_38AD:
        cmp    byte ptr [ebx + 11ah], 0
        je     SHORT loc_1_38C2
        mov    eax, 0c001000dh
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
loc_1_38C2:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   ebx
        call   sub_1_3B30
        xor    ecx, ecx
        mov    cx, word ptr [ebx + 102h]
        and    eax, 0ffffh
        cmp    ecx, eax
        jne    SHORT loc_1_3920
        test   byte ptr [ebp + 0dh], 20h
        mov    eax, 7fffffffh
        jne    SHORT loc_1_38F2
        mov    eax, dword ptr [ebx + 23dh]
        mov    eax, dword ptr [eax + 4]
loc_1_38F2:
        cmp    dword ptr [ebx + 104h], eax
        je     SHORT loc_1_3915
        mov    word ptr [edi + 8], 2
        mov    cl, byte ptr [ebp + 18h]
        push   ecx
        push   edi
        push   esi
        push   eax
        push   0
        call   sub_1_37B0
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
loc_1_3915:
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
        db  08Bh, 0FFh
loc_1_3920:
        cmp    dword ptr [edi + 10h], 4
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [edi + 8], eax
        jne    SHORT loc_1_392C
loc_1_392C:
        mov    eax, 103h
        mov    dword ptr [edi + 4], esi
        inc    dword ptr [esi + 14h]
        mov    dword ptr [edi], 0
        cmp    dword ptr [ebx + 2ah], 0
        je     SHORT loc_1_3950
        mov    ecx, dword ptr [ebx + 2eh]
        mov    dword ptr [ecx], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
        db  090h
loc_1_3950:
        mov    dword ptr [ebx + 2eh], edi
        mov    dword ptr [ebx + 2ah], edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    14h
        db  08Dh, 049h, 000h
sub_1_3960:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 108h], eax
        jne    SHORT loc_1_3980
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_3980:
        mov    eax, dword ptr [esi + 167h]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 2], ax
        sub    eax, eax
loc_1_3990:
        cmp    word ptr [ebp - 2], ax
        jne    SHORT loc_1_3990
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 112h], eax
        mov    eax, dword ptr [esi + 167h]
        push   4
        mov    word ptr [eax], 800h
        lea    eax, [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 167h]
        add    eax, 2
        push   eax
        call   sub_1_06EC
        mov    eax, dword ptr [esi + 167h]
        add    eax, 2
        push   eax
        call   sub_1_0C70
        mov    eax, dword ptr [esi + 167h]
        add    eax, 4
        push   eax
        call   sub_1_0C70
        mov    eax, dword ptr [esi + 167h]
        mov    dx, word ptr [eax + 2]
        mov    cx, word ptr [eax + 4]
        mov    word ptr [eax + 2], cx
        mov    eax, dword ptr [esi + 167h]
        pop    esi
        mov    word ptr [eax + 4], dx
        mov    eax, 103h
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [esi + 104h], eax
        jne    SHORT loc_1_3A30
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh
loc_1_3A30:
        mov    eax, dword ptr [esi + 167h]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 2], ax
        sub    eax, eax
loc_1_3A40:
        cmp    word ptr [ebp - 2], ax
        jne    SHORT loc_1_3A40
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [esi + 10eh], eax
        mov    eax, dword ptr [esi + 167h]
        push   4
        mov    word ptr [eax], 900h
        lea    eax, [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [esi + 167h]
        add    eax, 2
        push   eax
        call   sub_1_06EC
        mov    eax, dword ptr [esi + 167h]
        add    eax, 2
        push   eax
        call   sub_1_0C70
        mov    eax, dword ptr [esi + 167h]
        add    eax, 4
        push   eax
        call   sub_1_0C70
        mov    eax, dword ptr [esi + 167h]
        mov    dx, word ptr [eax + 2]
        mov    cx, word ptr [eax + 4]
        mov    word ptr [eax + 2], cx
        mov    eax, dword ptr [esi + 167h]
        pop    esi
        mov    word ptr [eax + 4], dx
        mov    eax, 103h
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_3AC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_3B30
        cmp    word ptr [esi + 102h], ax
        jne    SHORT loc_1_3AE6
        sub    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3AE6:
        mov    ecx, dword ptr [esi + 167h]
        mov    cx, word ptr [ecx]
        mov    word ptr [ebp - 2], cx
        sub    ecx, ecx
loc_1_3AF5:
        cmp    word ptr [ebp - 2], cx
        jne    SHORT loc_1_3AF5
        mov    ecx, dword ptr [esi + 167h]
        mov    word ptr [esi + 10ch], ax
        mov    word ptr [ecx], 0d00h
        mov    ecx, dword ptr [esi + 167h]
        mov    word ptr [ecx + 2], ax
        mov    eax, dword ptr [esi + 167h]
        pop    esi
        mov    word ptr [eax + 4], 0
        mov    eax, 103h
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_1_3B30:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   esi
        mov    dword ptr [ebp - 14h], 10h
        mov    word ptr [ebp - 10h], 200h
        mov    dword ptr [ebp - 0eh], 8000h
        sub    si, si
        mov    word ptr [ebp - 0ah], 400h
        mov    dword ptr [ebp - 8], 20h
        mov    word ptr [ebp - 4], 8218h
        mov    ax, 21h
        mov    edx, dword ptr [ebp + 0ch]
loc_1_3B68:
        mov    ecx, esi
        and    ecx, 0ffffh
        lea    ecx, [ecx + ecx*2]
        add    ecx, ecx
        test   dword ptr [ebp + ecx - 14h], edx
        je     SHORT loc_1_3B80
        or     ax, word ptr [ebp + ecx - 10h]
loc_1_3B80:
        inc    si
        cmp    si, 3
        jb     SHORT loc_1_3B68
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
        db  090h
sub_1_3B90:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 18bh]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        add    dword ptr [ecx + 1dch], eax
        mov    eax, dword ptr [ecx + 18bh]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        add    dword ptr [ecx + 1e4h], eax
        mov    eax, dword ptr [ecx + 18bh]
        mov    al, byte ptr [eax + 6]
        and    eax, 0ffh
        add    dword ptr [ecx + 1f0h], eax
        mov    eax, dword ptr [ecx + 18bh]
        mov    al, byte ptr [eax + 7]
        and    eax, 0ffh
        add    dword ptr [ecx + 1f4h], eax
        mov    eax, dword ptr [ecx + 18bh]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        add    dword ptr [ecx + 1f8h], eax
        mov    eax, dword ptr [ecx + 18bh]
        mov    al, byte ptr [eax + 0ah]
        and    eax, 0ffh
        add    dword ptr [ecx + 200h], eax
        mov    eax, dword ptr [ecx + 18bh]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        add    dword ptr [ecx + 204h], eax
        mov    eax, dword ptr [ecx + 18bh]
        mov    al, byte ptr [eax + 0ch]
        and    eax, 0ffh
        add    dword ptr [ecx + 208h], eax
        mov    eax, dword ptr [ecx + 18bh]
        pop    ebp
        mov    al, byte ptr [eax + 0dh]
        and    eax, 0ffh
        add    dword ptr [ecx + 20ch], eax
        ret    4
        db  08Bh, 0FFh
sub_1_3C50:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 0ch]
        sub    edi, edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], ecx
        mov    esi, dword ptr [eax + 193h]
        cmp    ecx, edi
        mov    ebx, dword ptr [eax + 19fh]
        je     SHORT loc_1_3CB4
loc_1_3C74:
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0654
        cmp    dword ptr [ebp - 4], ebx
        ja     SHORT loc_1_3CC8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   esi
        call   sub_1_06EC
        add    esi, dword ptr [ebp - 4]
        add    edi, dword ptr [ebp - 4]
        sub    ebx, dword ptr [ebp - 4]
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0678
        cmp    dword ptr [ebp - 8], 0
        jne    SHORT loc_1_3C74
loc_1_3CB4:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 19bh], edi
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_3CC8:
        sub    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_3CE0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 8]
        cmp    byte ptr [edx + 11ch], 0
        je     SHORT loc_1_3D00
        mov    eax, 0c0000001h
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h
loc_1_3D00:
        cmp    byte ptr [edx + 11ah], 0
        je     SHORT loc_1_3D13
        mov    eax, 0c001000dh
        pop    esi
        pop    ebp
        ret    8
loc_1_3D13:
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 28h], esi
        mov    dword ptr [ecx + 2ch], ecx
        lea    eax, [ecx + 24h]
        mov    dword ptr [eax], 0
        cmp    dword ptr [edx + 36h], 0
        jne    SHORT loc_1_3D30
        mov    dword ptr [edx + 36h], ecx
        jmp    SHORT loc_1_3D36
loc_1_3D30:
        mov    eax, dword ptr [edx + 3ah]
        mov    dword ptr [eax + 24h], ecx
loc_1_3D36:
        mov    dword ptr [edx + 3ah], ecx
        inc    dword ptr [esi + 14h]
        push   0
        push   edx
        call   sub_1_3D50
        mov    eax, 103h
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh
sub_1_3D50:
        push   ebp
        mov    ebp, esp
        sub    esp, 84h
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 36h]
        or     edi, edi
        je     NEAR PTR loc_1_42A5
        cmp    dword ptr [esi + 52h], 0
        je     NEAR PTR loc_1_42A5
        cmp    dword ptr [esi + 3eh], 0
        jne    NEAR PTR loc_1_42A5
        mov    dword ptr [esi + 3eh], edi
        mov    ecx, dword ptr [edi + 24h]
        lea    eax, [edi + 24h]
        mov    dword ptr [esi + 36h], ecx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 34h], eax
        lea    eax, [ebp - 30h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 2ch]
        push   eax
        push   edi
        call   sub_1_0630
        and    dword ptr [ebp - 2ch], 7fffffffh
        dec    dword ptr [esi + 52h]
        mov    edx, dword ptr [esi + 4eh]
        mov    dword ptr [ebp - 24h], edx
        mov    eax, dword ptr [ebp - 34h]
        mov    ebx, edx
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], edi
        mov    ax, word ptr [ebp - 30h]
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [edx + 0ch]
        mov    word ptr [eax + 6], cx
        cmp    byte ptr [esi + 127h], 0
        mov    eax, dword ptr [esi + 5ah]
        jne    NEAR PTR loc_1_3FE4
        imul   eax, dword ptr [esi + 52h]
        cmp    eax, dword ptr [ebp - 2ch]
        jb     NEAR PTR loc_1_3FA0
        cmp    byte ptr [esi + 15ah], 0
        jne    NEAR PTR loc_1_3FA0
        mov    word ptr [ebp - 6], 0
        cmp    dword ptr [ebp - 18h], 0
        mov    word ptr [ebp - 2], 0
        mov    edi, dword ptr [ebp - 28h]
        jbe    NEAR PTR loc_1_3F94
loc_1_3E27:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [eax + 0ch]
        or     ecx, ecx
        je     SHORT loc_1_3E45
        mov    eax, dword ptr [eax + 4]
        add    ecx, eax
        dec    ecx
        shr    ecx, 0ch
        shr    eax, 0ch
        sub    ecx, eax
        inc    ecx
        mov    dword ptr [ebp - 10h], ecx
        jmp    SHORT loc_1_3E4C
loc_1_3E45:
        mov    dword ptr [ebp - 10h], 0
loc_1_3E4C:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_3F58
        mov    eax, dword ptr [ebp - 14h]
        xor    ecx, ecx
        mov    cx, word ptr [ebp - 2]
        mov    dword ptr [ebx + ecx*8 + 18h], eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 84h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_065A
        cmp    dword ptr [ebp - 10h], 0
        mov    word ptr [ebp - 4], 0
        jbe    NEAR PTR loc_1_3F58
loc_1_3E87:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        lea    ecx, [eax*8]
        mov    eax, dword ptr [ebp + ecx - 84h]
        xor    edx, edx
        mov    dx, word ptr [ebp - 2]
        shr    eax, 10h
        lea    edx, [edx + edx*2]
        add    edx, edx
        mov    di, ax
        and    edi, 0ffff00ffh
        shl    di, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    di, ax
        mov    eax, dword ptr [ebx + 0ch]
        mov    word ptr [edx + eax + 0ah], di
        mov    ax, word ptr [ebp + ecx - 84h]
        movzx  di, ah
        and    ax, 0ffh
        shl    ax, 8
        add    di, ax
        mov    eax, dword ptr [ebx + 0ch]
        mov    word ptr [edx + eax + 0ch], di
        mov    ax, word ptr [ebp + ecx - 80h]
        mov    cx, ax
        shr    cx, 8
        or     cl, 80h
        and    cx, 0ffh
        and    ax, 0ffh
        inc    word ptr [ebp - 4]
        shl    ax, 8
        inc    word ptr [ebp - 2]
        add    cx, ax
        mov    eax, dword ptr [ebx + 0ch]
        mov    word ptr [edx + eax + 8], cx
        mov    eax, dword ptr [ebx + 0ch]
        lea    edi, [edx + eax + 8]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        cmp    dword ptr [ebp - 10h], ecx
        ja     SHORT loc_1_3F38
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        cmp    eax, dword ptr [ebp - 18h]
        jae    SHORT loc_1_3F4F
loc_1_3F38:
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        cmp    eax, dword ptr [esi + 5ah]
        jne    SHORT loc_1_3F4F
        mov    word ptr [ebp - 2], 0
        dec    dword ptr [esi + 52h]
        mov    ebx, dword ptr [ebx + 8]
loc_1_3F4F:
        cmp    dword ptr [ebp - 10h], ecx
        ja     NEAR PTR loc_1_3E87
loc_1_3F58:
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_0678
        cmp    dword ptr [ebp - 14h], 0
        jne    SHORT loc_1_3F77
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        inc    eax
        cmp    eax, dword ptr [ebp - 18h]
        jb     SHORT loc_1_3F94
loc_1_3F77:
        xor    eax, eax
        inc    word ptr [ebp - 6]
        mov    ax, word ptr [ebp - 6]
        cmp    eax, dword ptr [ebp - 18h]
        jb     NEAR PTR loc_1_3E27
        and    word ptr [edi], 0ff7fh
        jmp    NEAR PTR loc_1_419A
loc_1_3F94:
        and    word ptr [edi], 0ff7fh
        jmp    NEAR PTR loc_1_419A
        db  08Bh, 0FFh
loc_1_3FA0:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        push   esi
        call   sub_1_3C50
        or     al, al
        je     NEAR PTR loc_1_419A
        mov    eax, dword ptr [esi + 197h]
        shr    eax, 10h
        mov    cl, ah
        and    ecx, 0ffff00ffh
        and    ax, 0ffh
        shl    ax, 8
        mov    edx, dword ptr [ebp - 24h]
        add    cx, ax
        mov    eax, dword ptr [edx + 0ch]
        mov    word ptr [eax + 0ah], cx
        mov    ax, word ptr [esi + 197h]
        jmp    NEAR PTR loc_1_4159
loc_1_3FE4:
        imul   eax, dword ptr [esi + 52h]
        cmp    eax, dword ptr [ebp - 18h]
        jb     NEAR PTR loc_1_411B
        cmp    byte ptr [esi + 15ah], 0
        jne    NEAR PTR loc_1_411B
        mov    word ptr [ebp - 6], 0
        mov    word ptr [ebp - 2], 0
        mov    edi, dword ptr [ebp - 28h]
loc_1_4010:
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        cmp    eax, dword ptr [ebp - 18h]
        jae    NEAR PTR loc_1_4111
        lea    eax, [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_0654
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_40E6
        mov    eax, dword ptr [ebp - 20h]
        xor    ecx, ecx
        mov    cx, word ptr [ebp - 2]
        shr    eax, 10h
        lea    ecx, [ecx + ecx*2]
        lea    edx, [ecx*2]
        mov    cl, ah
        and    ecx, 0ffff00ffh
        and    ax, 0ffh
        shl    ax, 8
        add    cx, ax
        mov    eax, dword ptr [ebx + 0ch]
        mov    word ptr [edx + eax + 0ah], cx
        mov    ax, word ptr [ebp - 20h]
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [ebx + 0ch]
        mov    word ptr [edx + eax + 0ch], cx
        mov    ax, word ptr [ebp - 1ch]
        mov    cx, ax
        shr    cx, 8
        or     cl, 80h
        and    cx, 0ffh
        and    ax, 0ffh
        inc    word ptr [ebp - 6]
        shl    ax, 8
        inc    word ptr [ebp - 2]
        add    cx, ax
        mov    eax, dword ptr [ebx + 0ch]
        mov    word ptr [edx + eax + 8], cx
        mov    eax, dword ptr [ebx + 0ch]
        lea    edi, [edx + eax + 8]
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        cmp    eax, dword ptr [ebp - 18h]
        jae    SHORT loc_1_40EA
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        cmp    eax, dword ptr [esi + 5ah]
        jne    SHORT loc_1_40EA
        mov    word ptr [ebp - 2], 0
        dec    dword ptr [esi + 52h]
        mov    ebx, dword ptr [ebx + 8]
        jmp    SHORT loc_1_40EA
loc_1_40E6:
        inc    word ptr [ebp - 6]
loc_1_40EA:
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_0678
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_4010
        xor    eax, eax
        mov    ax, word ptr [ebp - 6]
        inc    eax
        cmp    eax, dword ptr [ebp - 18h]
        jae    NEAR PTR loc_1_4010
loc_1_4111:
        and    word ptr [edi], 0ff7fh
        jmp    NEAR PTR loc_1_419A
loc_1_411B:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        push   esi
        call   sub_1_3C50
        or     al, al
        je     SHORT loc_1_419A
        mov    eax, dword ptr [esi + 193h]
        shr    eax, 10h
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        mov    edx, dword ptr [ebp - 24h]
        add    cx, ax
        mov    eax, dword ptr [edx + 0ch]
        mov    word ptr [eax + 0ah], cx
        mov    ax, word ptr [esi + 193h]
loc_1_4159:
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [edx + 0ch]
        mov    word ptr [eax + 0ch], cx
        mov    ax, word ptr [esi + 19bh]
        mov    cl, al
        and    ecx, 0ffff00ffh
        shl    cx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    cx, ax
        mov    eax, dword ptr [edx + 0ch]
        mov    word ptr [eax + 8], cx
loc_1_419A:
        mov    eax, dword ptr [ebx + 0ch]
        mov    edx, dword ptr [ebp - 24h]
        or     word ptr [eax + 4], 10h
        mov    eax, dword ptr [ebx + 0ch]
        or     word ptr [eax + 2], 100h
        mov    dword ptr [esi + 4ah], ebx
        lea    ecx, [esi + 167h]
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 4eh], eax
        mov    eax, dword ptr [edx + 0ch]
        mov    bl, byte ptr [ebp + 0ch]
        or     word ptr [eax + 4], 0a1h
        mov    eax, dword ptr [ecx]
        or     bl, bl
        mov    ax, word ptr [eax]
        je     SHORT loc_1_41E0
        mov    word ptr [ebp - 0ah], ax
        sub    eax, eax
loc_1_41D8:
        cmp    word ptr [ebp - 0ah], ax
        jne    SHORT loc_1_41D8
        jmp    SHORT loc_1_41EC
loc_1_41E0:
        mov    word ptr [ebp - 8], ax
        sub    eax, eax
loc_1_41E6:
        cmp    word ptr [ebp - 8], ax
        jne    SHORT loc_1_41E6
loc_1_41EC:
        mov    eax, dword ptr [ecx]
        mov    word ptr [eax], 400h
        cmp    byte ptr [esi + 127h], 0
        mov    eax, dword ptr [esi + 46h]
        je     SHORT loc_1_4240
        mov    eax, dword ptr [eax + 0ch]
        shr    eax, 10h
        mov    dl, al
        and    edx, 0ffff00ffh
        shl    dx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    dx, ax
        mov    eax, dword ptr [ecx]
        mov    word ptr [eax + 2], dx
        mov    eax, dword ptr [esi + 46h]
        mov    ax, word ptr [eax + 0ch]
        mov    dl, al
        and    edx, 0ffff00ffh
        shl    dx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        jmp    SHORT loc_1_4279
        db  08Dh, 049h, 000h
loc_1_4240:
        mov    eax, dword ptr [eax + 10h]
        shr    eax, 10h
        mov    dl, al
        and    edx, 0ffff00ffh
        shl    dx, 8
        mov    al, ah
        and    eax, 0ffff00ffh
        add    dx, ax
        mov    eax, dword ptr [ecx]
        mov    word ptr [eax + 2], dx
        mov    eax, dword ptr [esi + 46h]
        mov    ax, word ptr [eax + 10h]
        mov    dl, ah
        and    edx, 0ffff00ffh
        and    ax, 0ffh
        shl    ax, 8
loc_1_4279:
        add    dx, ax
        mov    eax, dword ptr [ecx]
        or     bl, bl
        mov    word ptr [eax + 4], dx
        push   esi
        db  068h, 000h, 019h, 000h, 000h
        push   esi
        je     SHORT loc_1_42A0
        call   sub_1_05FA
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  005h, 000h, 000h, 000h, 000h
loc_1_42A0:
        call   sub_1_05FA
loc_1_42A5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 054h, 068h, 06Fh, 06Dh, 061h, 073h, 02Dh, 043h, 06Fh, 06Eh, 072h, 061h, 064h, 020h
        db  054h, 06Fh, 06Bh, 065h, 06Eh, 020h, 052h, 069h, 06Eh, 067h, 020h, 044h, 072h, 069h, 076h, 065h
        db  072h, 020h, 066h, 06Fh, 072h, 020h, 04Eh, 044h, 049h, 053h, 020h, 033h, 02Eh, 030h, 02Ch, 020h
        db  028h, 043h, 029h, 020h, 043h, 06Fh, 070h, 079h, 072h, 069h, 067h, 068h, 074h, 020h, 031h, 039h
        db  039h, 030h, 02Dh, 039h, 034h, 020h, 054h, 068h, 06Fh, 06Dh, 061h, 073h, 02Dh, 043h, 06Fh, 06Eh
        db  072h, 061h, 064h, 02Ch, 020h, 049h, 06Eh, 063h, 02Eh, 02Ch, 020h, 041h, 06Ch, 06Ch, 020h, 052h
        db  069h, 067h, 068h, 074h, 073h, 020h, 052h, 065h, 073h, 065h, 072h, 076h, 065h, 064h, 02Ch, 020h
        db  031h, 02Eh, 030h, 033h, 028h, 039h, 034h, 030h, 037h, 030h, 036h, 029h, 000h, 000h
loc_1_432C:
        push   esp
        arpl   word ptr [edi + ebp*2 + 6bh], si
        add    byte ptr [eax], al
        add    byte ptr [edx + 75h], al
        jae    SHORT loc_1_438C
        jns    SHORT loc_1_43AA
        db  065h, 000h
loc_1_433C:
        inc    edx
        db  075h, 073h
        push   esp
        db  079h, 070h
        add    byte ptr gs:[ebx + 70h], dl
        add    byte ptr fs:[eax], al
        add    byte ptr [ebx + 70h], dl
        add    byte ptr fs:[eax], al
        add    byte ptr [ebx + 6ch], dl
        outsd
        je     SHORT loc_1_43A7
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
loc_1_4360:
        push   ebx
        insb
        outsd
        db  074h, 04Eh
        db  075h, 06Dh
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [eax], al
        inc    esp
        insd
        popad
        inc    ebx
        push   656e6e61h
        insb
        add    byte ptr [eax], al
loc_1_4378:
        inc    esp
        insd
        popad
        inc    ebx
        push   656e6e61h
        insb
        add    byte ptr [eax], al
        dec    ecx
        outsb
        db  074h, 065h
        jb     SHORT loc_1_43FC
        jne    SHORT loc_1_43FC
loc_1_438C:
        je     SHORT loc_1_438E
loc_1_438E:
        add    byte ptr [eax], al
loc_1_4390:
        dec    ecx
        outsb
        db  074h, 065h
        jb     SHORT loc_1_4408
        jne    SHORT loc_1_4408
        je     SHORT loc_1_439A
loc_1_439A:
        add    byte ptr [eax], al
        dec    ecx
        outsd
        inc    edx
        popad
        db  073h, 065h
        add    byte ptr [eax], al
loc_1_43A4:
        dec    ecx
        outsd
        inc    edx
loc_1_43A7:
        popad
        db  073h, 065h
loc_1_43AA:
        add    byte ptr [eax], al
        dec    esi
        db  065h, 074h, 041h
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_1_4429
        add    byte ptr [eax], al
loc_1_43B8:
        dec    esi
        db  065h, 074h, 041h
        db  064h, 064h, 072h, 065h
        jae    SHORT loc_1_4435
        add    byte ptr [eax], al
        push   eax
        popad
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_441E
        imul   edi, dword ptr [edx + 65h], 61500000h
        arpl   word ptr [ebx + 65h], bp
        db  074h, 053h
        imul   edi, dword ptr [edx + 65h], 72500000h
        outsd
        db  064h, 075h, 063h
        je     SHORT loc_1_442D
        add    byte ptr fs:[eax], al
        add    byte ptr [eax + 72h], dl
        outsd
        jne    NEAR PTR loc_1_4451
        je     SHORT loc_1_4439
        add    byte ptr fs:[eax], al
        add    byte ptr [ebp + 61h], ah
        insb
        arpl   word ptr gs:[ecx + 66h], sp
loc_1_43FC:
        bound  ebp, qword ptr cs:[ecx + 6eh]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4404:
        popad
        insb
loc_1_4408:
        arpl   word ptr gs:[ecx + 66h], sp
        bound  ebp, qword ptr cs:[ecx + 6eh]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_4414:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
loc_1_441E:
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
loc_1_4429:
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
loc_1_442D:
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
loc_1_4435:
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
loc_1_4439:
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 0e000101h
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [eax], dl
loc_1_4451:
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
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
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
        add    eax, 1020101h
        add    dword ptr [edx], eax
        add    al, byte ptr [edx]
        add    dword ptr [edx], eax
        add    al, byte ptr [ecx]
        add    dword ptr [ecx], eax
        add    byte ptr [edx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], al
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], al
        add    dword ptr [eax], eax
        add    eax, 6000101h
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
        or     eax, 0e000101h
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [eax], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
loc_1_44EC:
        push   esp
        push   73616d6fh
        sub    eax, 726e6f43h
        popad
        and    byte ptr fs:[edi + ebp*2 + 6bh], dl
        outsb
        and    byte ptr [edx + 69h], dl
        outsb
        and    byte ptr [bx + di + 64h], al
        popad
        db  070h, 074h
        jb     NEAR PTR loc_1_453C
        inc    ecx
        push   esp
        add    byte ptr cs:[eax], al
        add    byte ptr [eax], al
loc_1_4514:
        push   esp
        push   73616d6fh
        sub    eax, 726e6f43h
        popad
        and    byte ptr fs:[edi + ebp*2 + 6bh], dl
        outsb
        and    byte ptr [edx + 69h], dl
        outsb
        and    byte ptr [bx + di + 64h], al
        popad
        db  070h, 074h
        db  065h, 072h, 02Fh
        dec    ebp
        inc    ebx
        add    byte ptr cs:[eax], al
        add    byte ptr [eax], al
loc_1_453C:
        add    byte ptr [eax], al
loc_1_453E:
        add    byte ptr [eax], al
loc_1_4540:
        push   es
        add    byte ptr [esi], al
        db  000h, 02Ch, 043h
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
_LTEXT ends
        end
