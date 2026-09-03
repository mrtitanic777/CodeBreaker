; ctndw_lcode_01.asm -- CTNDW object 1 segment _LTEXT
.386p
_LTEXT segment dword public use32 'LCODE'

loc_1_0000:
        db  0A0h, 090h, 007h, 000h, 000h
        cmp    al, 0
        jne    SHORT loc_1_0025
        db  0FEh, 005h, 090h, 007h, 000h, 000h
        db  0CDh, 020h, 000h, 000h, 028h, 000h
        jb     SHORT loc_1_0027
        push   0
        push   0
        call   sub_1_07A0
        cmp    eax, 0
        jne    SHORT loc_1_0027
loc_1_0025:
        clc
        ret
loc_1_0027:
        stc
        ret
CTNDW_Control:
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
        call   sub_1_0604
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
        call   sub_1_0604
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
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
        db  0CDh, 020h, 044h, 080h, 028h, 000h
        db  0CDh, 020h, 048h, 080h, 028h, 000h
        db  0CDh, 020h, 049h, 080h, 028h, 000h
sub_1_05CE:
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 000h, 080h, 028h, 000h
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
        db  0CDh, 020h, 00Dh, 080h, 028h, 000h
sub_1_05E6:
        db  0CDh, 020h, 00Fh, 080h, 028h, 000h
sub_1_05EC:
        db  0CDh, 020h, 010h, 080h, 028h, 000h
        db  0CDh, 020h, 011h, 080h, 028h, 000h
sub_1_05F8:
        db  0CDh, 020h, 00Eh, 080h, 028h, 000h
sub_1_05FE:
        db  0CDh, 020h, 00Ah, 080h, 028h, 000h
sub_1_0604:
        db  0CDh, 020h, 00Bh, 080h, 028h, 000h
        db  0CDh, 020h, 016h, 080h, 028h, 000h
        db  0CDh, 020h, 017h, 080h, 028h, 000h
        db  0CDh, 020h, 018h, 080h, 028h, 000h
        db  0CDh, 020h, 019h, 080h, 028h, 000h
        db  0CDh, 020h, 01Ah, 080h, 028h, 000h
sub_1_0628:
        db  0CDh, 020h, 01Bh, 080h, 028h, 000h
        db  0CDh, 020h, 01Ch, 080h, 028h, 000h
        db  0CDh, 020h, 01Dh, 080h, 028h, 000h
        db  0CDh, 020h, 01Eh, 080h, 028h, 000h
        db  0CDh, 020h, 01Fh, 080h, 028h, 000h
        db  0CDh, 020h, 020h, 080h, 028h, 000h
sub_1_064C:
        db  0CDh, 020h, 021h, 080h, 028h, 000h
        db  0CDh, 020h, 022h, 080h, 028h, 000h
        db  0CDh, 020h, 023h, 080h, 028h, 000h
        db  0CDh, 020h, 024h, 080h, 028h, 000h
        db  0CDh, 020h, 025h, 080h, 028h, 000h
        db  0CDh, 020h, 026h, 080h, 028h, 000h
sub_1_0670:
        db  0CDh, 020h, 027h, 080h, 028h, 000h
        db  0CDh, 020h, 028h, 080h, 028h, 000h
sub_1_067C:
        db  0CDh, 020h, 005h, 080h, 028h, 000h
sub_1_0682:
        db  0CDh, 020h, 006h, 080h, 028h, 000h
sub_1_0688:
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
sub_1_06CA:
        db  0CDh, 020h, 046h, 080h, 028h, 000h
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
        db  0CDh, 020h, 047h, 080h, 028h, 000h
        add    byte ptr [eax], al
sub_1_06E4:
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
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 043h, 054h, 04Eh, 044h
        db  057h, 020h, 020h, 020h, 000h, 040h, 000h, 054h, 029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_07A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 50h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 38h]
        mov    dword ptr [ebp - 50h], eax
        mov    word ptr [ebp - 40h], 6
        mov    word ptr [ebp - 3eh], 6
        db  0C7h, 045h, 0C4h, 018h, 043h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 44h]
        push   eax
        call   sub_1_054A
        db  0A1h, 0C0h, 0A8h, 000h, 000h
        push   eax
        push   0
        push   8
        lea    eax, [ebp - 48h]
        push   eax
        call   sub_1_05FE
        mov    dword ptr [ebp - 4ch], eax
        cmp    dword ptr [ebp - 4ch], 0
        je     NEAR PTR loc_1_07FF
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_1_08CD
loc_1_07FF:
        mov    eax, dword ptr [ebp - 44h]
        mov    ecx, dword ptr [ebp - 48h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 50h]
        mov    byte ptr [eax], 3
        mov    eax, dword ptr [ebp - 50h]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp - 50h]
        db  0C7h, 040h, 006h, 0D4h, 008h, 000h, 000h
        mov    eax, dword ptr [ebp - 50h]
        db  0C7h, 040h, 00Ah, 004h, 00Bh, 000h, 000h
        mov    eax, dword ptr [ebp - 50h]
        db  0C7h, 040h, 01Ah, 05Bh, 00Ch, 000h, 000h
        mov    eax, dword ptr [ebp - 50h]
        db  0C7h, 040h, 00Eh, 0C0h, 021h, 000h, 000h
        mov    eax, dword ptr [ebp - 50h]
        db  0C7h, 040h, 012h, 031h, 026h, 000h, 000h
        mov    eax, dword ptr [ebp - 50h]
        db  0C7h, 040h, 016h, 049h, 010h, 000h, 000h
        mov    eax, dword ptr [ebp - 50h]
        db  0C7h, 040h, 022h, 0FCh, 01Bh, 000h, 000h
        mov    eax, dword ptr [ebp - 50h]
        db  0C7h, 040h, 01Eh, 035h, 00Eh, 000h, 000h
        mov    eax, dword ptr [ebp - 50h]
        db  0C7h, 040h, 026h, 066h, 014h, 000h, 000h
        mov    eax, dword ptr [ebp - 50h]
        db  0C7h, 040h, 02Ah, 0A5h, 01Bh, 000h, 000h
        mov    eax, dword ptr [ebp - 40h]
        mov    ecx, dword ptr [ebp - 3ch]
        mov    edx, dword ptr [ebp - 50h]
        add    edx, 2eh
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], ecx
        push   36h
        mov    eax, dword ptr [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        mov    eax, dword ptr [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp - 48h]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 4ch]
        push   eax
        call   sub_1_0556
        cmp    dword ptr [ebp - 4ch], 0
        je     NEAR PTR loc_1_08C6
        push   0
        mov    eax, dword ptr [ebp - 44h]
        push   eax
        call   sub_1_0550
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_08CD
loc_1_08C6:
        sub    eax, eax
        jmp    NEAR PTR loc_1_08CD
loc_1_08CD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
loc_1_08D4:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 184h], 0d0h
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax], 0ffffffffh
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_0911
loc_1_090E:
        inc    dword ptr [ebp - 10h]
loc_1_0911:
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [ebp - 10h], eax
        jae    NEAR PTR loc_1_0937
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 14h]
        cmp    dword ptr [ecx + eax*4], 1
        jne    NEAR PTR loc_1_0932
        jmp    NEAR PTR loc_1_0937
loc_1_0932:
        jmp    NEAR PTR loc_1_090E
loc_1_0937:
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [ebp - 10h], eax
        jne    NEAR PTR loc_1_094D
        mov    eax, 0c0010019h
        jmp    NEAR PTR loc_1_0AFD
loc_1_094D:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        db  0A1h, 0C0h, 0A8h, 000h, 000h
        push   eax
        push   0
        push   0a4h
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_05FE
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_0982
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_1_0AFD
loc_1_0982:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp + 28h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 98h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 9ch], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0a0h], 0
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 120h]
        push   eax
        db  0E8h, 0F9h, 0F6h, 0FEh, 0FFh
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_0A0E
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_0AFD
loc_1_0A0E:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 0d4h
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 0d8h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ecx + 0d8h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 0d8h], eax
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 12ch], 1
        jne    NEAR PTR loc_1_0A5E
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_0AFD
loc_1_0A5E:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 128h], 1
        jne    NEAR PTR loc_1_0A7E
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [eax + 13ch]
        sub    eax, eax
        jmp    NEAR PTR loc_1_0AFD
loc_1_0A7E:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 13ch], 1
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 13ch], 1
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 0f0h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 12ch], 1
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1ch], 1
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_2A68
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_2D7F
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_0AE1
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_0AFD
loc_1_0AE1:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_2810
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_2F21
        mov    eax, 103h
        jmp    NEAR PTR loc_1_0AFD
loc_1_0AFD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    24h
loc_1_0B04:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 130h], 1
        jne    NEAR PTR loc_1_0B36
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_0C54
loc_1_0B36:
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 130h], 1
        push   0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 120h]
        push   eax
        db  0E8h, 075h, 0F6h, 0FEh, 0FFh
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 13ch], 1
        je     NEAR PTR loc_1_0BAC
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 130h], 0
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 13ch]
        mov    eax, dword ptr [ebp - 8]
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
        sub    eax, eax
        jmp    NEAR PTR loc_1_0C54
loc_1_0BAC:
        mov    eax, dword ptr [ebp - 8]
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
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 0f4h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 13ch], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 128h], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 158h], 0
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 15ch], 0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_293F
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_0C2A
        sub    eax, eax
        jmp    NEAR PTR loc_1_0C54
loc_1_0C2A:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 24h
        push   eax
        push   7
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_32A7
        push   1000h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3760
        mov    eax, 103h
        jmp    NEAR PTR loc_1_0C54
loc_1_0C54:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_0C5B:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 10h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_0CC4
loc_1_0C81:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1F39
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_0CDD
loc_1_0C9A:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_1FB3
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_0CDD
loc_1_0CB3:
        mov    dword ptr [ebp - 4], 10001h
        jmp    NEAR PTR loc_1_0CDD
        db  0E9h, 019h, 000h, 000h, 000h
loc_1_0CC4:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_0C81
        cmp    dword ptr [ebp - 10h], 1
        je     NEAR PTR loc_1_0C9A
        jmp    NEAR PTR loc_1_0CB3
loc_1_0CDD:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0CE5
loc_1_0CE5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_0CEC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        mov    dword ptr [ebp - 8], eax
        shl    dword ptr [ebp - 8], 8
        mov    eax, dword ptr [ebp - 8]
        add    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        mov    dword ptr [ebp - 8], eax
        shl    dword ptr [ebp - 8], 10h
        mov    eax, dword ptr [ebp - 8]
        add    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        mov    dword ptr [ebp - 8], eax
        shl    dword ptr [ebp - 8], 18h
        mov    eax, dword ptr [ebp - 8]
        add    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 9ch], eax
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 120h]
        push   eax
        db  0E8h, 043h, 0F5h, 0FEh, 0FFh
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0D88
loc_1_0D88:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_0D8F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 1]
        and    eax, 0ffh
        mov    dword ptr [ebp - 8], eax
        shl    dword ptr [ebp - 8], 8
        mov    eax, dword ptr [ebp - 8]
        add    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        mov    dword ptr [ebp - 8], eax
        shl    dword ptr [ebp - 8], 10h
        mov    eax, dword ptr [ebp - 8]
        add    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    al, byte ptr [eax + 3]
        and    eax, 0ffh
        mov    dword ptr [ebp - 8], eax
        shl    dword ptr [ebp - 8], 18h
        mov    eax, dword ptr [ebp - 8]
        add    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 0a0h], eax
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 120h]
        push   eax
        db  0E8h, 03Dh, 0F5h, 0FEh, 0FFh
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0E2E
loc_1_0E2E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
loc_1_0E35:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_0EEE
loc_1_0E60:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0E65:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0E6A:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0E6F:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0E74:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0E79:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0E7E:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0E83:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0E88:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0E8D:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0E92:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0E97:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0E9C:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0EA1:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0EA6:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0EAB:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0EB0:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0EB5:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0EBA:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0EBF:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0EC4:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0EC9:
        jmp    NEAR PTR loc_1_0FA1
        db  0E9h, 0CEh, 000h, 000h, 000h
loc_1_0ED3:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0ED8:
        jmp    NEAR PTR loc_1_0FA1
loc_1_0EDD:
        mov    dword ptr [ebp - 4], 0c0010017h
        jmp    NEAR PTR loc_1_0FA1
        db  0E9h, 0B3h, 000h, 000h, 000h
loc_1_0EEE:
        cmp    dword ptr [ebp - 10h], 2010101h
        db  00Fh, 087h, 070h, 000h, 000h, 000h
        je     NEAR PTR loc_1_0FA1
        sub    dword ptr [ebp - 10h], 10101h
        cmp    dword ptr [ebp - 10h], 12h
        ja     NEAR PTR loc_1_0EDD
        shl    dword ptr [ebp - 10h], 2
        mov    eax, dword ptr [ebp - 10h]
        db  0FFh, 0A0h, 01Fh, 00Fh, 000h, 000h
loc_1_0F1F:
        pushad
        push   cs
        add    byte ptr [eax], al
        push   cs
        add    byte ptr [eax], al
        push   0eh
        add    byte ptr [eax], al
        outsd
        push   cs
        add    byte ptr [eax], al
        je     SHORT loc_1_0F3F
        add    byte ptr [eax], al
        jns    SHORT loc_1_0F43
        add    byte ptr [eax], al
        mov    byte ptr [esi], cl
        add    byte ptr [eax], al
        lea    ecx, [esi]
        add    byte ptr [eax], al
loc_1_0F3F:
        xchg   edx, eax
        push   cs
        add    byte ptr [eax], al
loc_1_0F43:
        xchg   edi, eax
        push   cs
        add    byte ptr [eax], al
        pushfd
        push   cs
        add    byte ptr [eax], al
        db  0A1h, 00Eh, 000h, 000h, 0A6h
        push   cs
        add    byte ptr [eax], al
        mov    al, 0eh
        add    byte ptr [eax], al
        mov    ch, 0eh
        add    byte ptr [eax], al
        stosd
        push   cs
        add    byte ptr [eax], al
        db  07Eh, 00Eh
        add    byte ptr [eax], al
        fisttp qword ptr [esi]
        add    byte ptr [eax], al
        or     dword ptr [esi], 0
        add    byte ptr [ecx + 102f06dh], al
        add    dword ptr [edx], eax
        cmp    dword ptr [ebp - 10h], 5
        ja     NEAR PTR loc_1_0EDD
        shl    dword ptr [ebp - 10h], 2
        mov    eax, dword ptr [ebp - 10h]
        db  0FFh, 0A0h, 089h, 00Fh, 000h, 000h
loc_1_0F89:
        ror    dword ptr [esi], cl
        add    byte ptr [eax], al
        fmul   dword ptr [esi]
        add    byte ptr [eax], al
        db  0BAh, 00Eh, 000h, 000h, 0BFh
        push   cs
        add    byte ptr [eax], al
        les    ecx, ptr [esi]
        add    byte ptr [eax], al
        leave
        push   cs
        add    byte ptr [eax], al
loc_1_0FA1:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_0FBB
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_0FCA
        mov    dword ptr [ebp - 4], eax
loc_1_0FBB:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_0FC3
loc_1_0FC3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_0FCA:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 14h], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 10h], 0ch
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1C2E
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1042
loc_1_1042:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
loc_1_1049:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1089
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_10DE
        sub    eax, eax
        jmp    NEAR PTR loc_1_1082
loc_1_1082:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_1089:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d4h]
        mov    dword ptr [ebp - 4], eax
loc_1_109E:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0d4h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_1_10D7
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_109E
loc_1_10D7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_10DE:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d4h]
        mov    dword ptr [ebp - 4], eax
loc_1_10F3:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0d4h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_1_112C
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0
        push   0
        push   40010005h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_10F3
loc_1_112C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 138h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d4h]
        mov    dword ptr [ebp - 4], eax
loc_1_1155:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0d4h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_1_118E
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   0
        push   0
        push   40010004h
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1155
loc_1_118E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_1195:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 100h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 138h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d4h]
        mov    dword ptr [ebp - 4], eax
loc_1_11C3:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0d4h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_1_11FC
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        push   0
        push   0
        push   40010005h
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_11C3
loc_1_11FC:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d4h]
        mov    dword ptr [ebp - 4], eax
loc_1_1208:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0d4h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_1_122C
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1208
loc_1_122C:
        push   0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_0586
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_1241:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 10h], eax
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [ebp + 14h], 0
        jne    NEAR PTR loc_1_1277
        sub    eax, eax
        jmp    NEAR PTR loc_1_12AE
loc_1_1277:
        test   byte ptr [ebp + 0ch], 30h
        je     NEAR PTR loc_1_1290
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_12AE
        db  0E9h, 01Eh, 000h, 000h, 000h
loc_1_1290:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 10h], 4
        jne    NEAR PTR loc_1_12A7
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_12AE
loc_1_12A7:
        sub    eax, eax
        jmp    NEAR PTR loc_1_12AE
loc_1_12AE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_1_12B5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 128h], 0
        jne    NEAR PTR loc_1_12E7
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_1380
loc_1_12E7:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 10ch], 1
        jne    NEAR PTR loc_1_1301
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_1380
loc_1_1301:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 130h], 1
        jne    NEAR PTR loc_1_1328
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 13ch], 1
        jne    NEAR PTR loc_1_1328
        sub    eax, eax
        jmp    NEAR PTR loc_1_1380
loc_1_1328:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 10ch], 1
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 104h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0f8h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 158h], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   9
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_32A7
        push   1000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_3760
        mov    eax, 103h
        jmp    NEAR PTR loc_1_1380
loc_1_1380:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_1_1387:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 128h], 0
        jne    NEAR PTR loc_1_13B9
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_1452
loc_1_13B9:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 110h], 1
        jne    NEAR PTR loc_1_13D3
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_1452
loc_1_13D3:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 130h], 1
        jne    NEAR PTR loc_1_13FA
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 13ch], 1
        jne    NEAR PTR loc_1_13FA
        sub    eax, eax
        jmp    NEAR PTR loc_1_1452
loc_1_13FA:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [eax + 110h], 1
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 108h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0fch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 15ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        push   8
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_32A7
        push   1000h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_3760
        mov    eax, 103h
        jmp    NEAR PTR loc_1_1452
loc_1_1452:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_1_1459:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_1466:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    byte ptr [ebp - 3ch], 0
        mov    word ptr [ebp - 1ch], 0eh
        mov    word ptr [ebp - 1ah], 0eh
        db  0C7h, 045h, 0E8h, 030h, 043h, 000h, 000h
        mov    word ptr [ebp - 34h], 0eh
        mov    word ptr [ebp - 32h], 0eh
        db  0C7h, 045h, 0D0h, 050h, 043h, 000h, 000h
        mov    word ptr [ebp - 4ch], 9
        mov    word ptr [ebp - 4ah], 9
        db  0C7h, 045h, 0B8h, 06Ch, 043h, 000h, 000h
        mov    word ptr [ebp - 10h], 12h
        mov    word ptr [ebp - 0eh], 12h
        db  0C7h, 045h, 0F4h, 08Ch, 043h, 000h, 000h
        mov    word ptr [ebp - 2ch], 10h
        mov    word ptr [ebp - 2ah], 10h
        db  0C7h, 045h, 0D8h, 0B0h, 043h, 000h, 000h
        mov    word ptr [ebp - 44h], 0ah
        mov    word ptr [ebp - 42h], 0ah
        db  0C7h, 045h, 0C0h, 0CCh, 043h, 000h, 000h
        db  0A1h, 0C0h, 0A8h, 000h, 000h
        push   eax
        push   0
        push   18ch
        lea    eax, [ebp - 24h]
        push   eax
        call   sub_1_05FE
        mov    dword ptr [ebp - 20h], eax
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_1_1519
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_1_187A
loc_1_1519:
        mov    edi, dword ptr [ebp - 24h]
        sub    eax, eax
        mov    ecx, 63h
        rep stosd
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 24h]
        mov    dword ptr [ecx + 0b4h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [eax + 128h], 0
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [eax + 12ch], 0
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [eax + 138h], 0
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [eax + 10ch], 0
        db  0A1h, 0C0h, 0A8h, 000h, 000h
        push   eax
        push   0
        push   1000h
        mov    eax, dword ptr [ebp - 24h]
        add    eax, 0b0h
        push   eax
        call   sub_1_05FE
        mov    dword ptr [ebp - 20h], eax
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_1_159A
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_1_187A
loc_1_159A:
        db  0A1h, 0C0h, 0A8h, 000h, 000h
        push   eax
        push   0
        push   1000h
        mov    eax, dword ptr [ebp - 24h]
        add    eax, 0ach
        push   eax
        call   sub_1_05FE
        mov    dword ptr [ebp - 20h], eax
        cmp    dword ptr [ebp - 20h], 0
        je     NEAR PTR loc_1_15CC
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_1_187A
loc_1_15CC:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_067C
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1601
        push   0
        push   18ch
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_0604
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_187A
loc_1_1601:
        push   1
        lea    eax, [ebp - 4ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0682
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1669
        push   1
        lea    eax, [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0682
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1655
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [eax + 140h], 5
        jmp    NEAR PTR loc_1_1664
loc_1_1655:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 24h]
        mov    dword ptr [ecx + 140h], eax
loc_1_1664:
        jmp    NEAR PTR loc_1_1678
loc_1_1669:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 24h]
        mov    dword ptr [ecx + 140h], eax
loc_1_1678:
        push   1
        lea    eax, [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0682
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_16AB
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [eax + 148h], 300h
        jmp    NEAR PTR loc_1_16BA
loc_1_16AB:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 24h]
        mov    dword ptr [ecx + 148h], eax
loc_1_16BA:
        push   1
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0682
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_172C
        push   1
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0682
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_170E
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [eax + 14ch], 0d000h
        jmp    NEAR PTR loc_1_1727
loc_1_170E:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 24h]
        mov    dword ptr [ecx + 14ch], eax
        mov    eax, dword ptr [ebp - 24h]
        shr    dword ptr [eax + 14ch], 4
loc_1_1727:
        jmp    NEAR PTR loc_1_1745
loc_1_172C:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 24h]
        mov    dword ptr [ecx + 14ch], eax
        mov    eax, dword ptr [ebp - 24h]
        shr    dword ptr [eax + 14ch], 4
loc_1_1745:
        push   1
        lea    eax, [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0682
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1785
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [eax + 154h], 0
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [eax + 188h], 0
        jmp    NEAR PTR loc_1_17A1
loc_1_1785:
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 24h]
        mov    dword ptr [ecx + 154h], eax
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [eax + 188h], 1
loc_1_17A1:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0688
        mov    dword ptr [ebp - 38h], 0
        jmp    NEAR PTR loc_1_17B9
loc_1_17B6:
        inc    dword ptr [ebp - 38h]
loc_1_17B9:
        cmp    dword ptr [ebp - 38h], 3e8h
        jae    NEAR PTR loc_1_17D2
        push   64h
        call   sub_1_05F8
        jmp    NEAR PTR loc_1_17B6
loc_1_17D2:
        mov    al, 1
        out    61h, al
        mov    eax, dword ptr [ebp - 24h]
        mov    dword ptr [eax + 144h], 10h
        mov    al, byte ptr [ebp - 3ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_1881
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_181F
        push   0
        push   18ch
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        call   sub_1_0604
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_187A
loc_1_181F:
        push   1
        push   0
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax + 140h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax + 140h]
        push   eax
        db  068h, 00Fh, 038h, 000h, 000h
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        db  068h, 0C3h, 037h, 000h, 000h
        mov    eax, dword ptr [ebp - 24h]
        mov    eax, dword ptr [eax + 0b8h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        add    eax, 0bch
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05E6
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1873
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_187A
loc_1_1873:
        sub    eax, eax
        jmp    NEAR PTR loc_1_187A
loc_1_187A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_1881:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0d4h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0d8h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d8h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0d4h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0dch
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0e0h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e0h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0dch], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0e4h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0e8h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e8h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0e4h], eax
        mov    dword ptr [ebp - 10h], 32h
        db  0A1h, 0C0h, 0A8h, 000h, 000h
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        call   sub_1_05FE
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1924
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1B9E
loc_1_1924:
        mov    edx, dword ptr [ebp - 10h]
        mov    edi, dword ptr [ebp - 18h]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd
        mov    ecx, edx
        and    ecx, 3
        rep stosb
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 0ah], 1
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 16h], 2
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 148h]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 1ah], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 1eh], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 16ch
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 22h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        lea    eax, [eax + eax*4]
        shl    eax, 3
        add    eax, 3e0h
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 26h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 2ah], 2
        mov    eax, dword ptr [ebp + 8]
        add    eax, 170h
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [ecx + 2eh], eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b4h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0b8h
        push   eax
        call   sub_1_0562
        mov    dword ptr [ebp - 4], eax
        push   0
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_0604
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_19F9
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1B9E
loc_1_19F9:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14ch]
        shl    eax, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        push   1000h
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b8h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 164h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_06CA
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1A65
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b8h]
        push   eax
        call   sub_1_05CE
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b8h]
        push   eax
        call   sub_1_0568
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_1B9E
loc_1_1A65:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14ch]
        shl    eax, 4
        add    eax, 1000h
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 14h], eax
        push   2000h
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b8h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 168h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_06CA
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_1AD6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b8h]
        push   eax
        call   sub_1_05CE
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b8h]
        push   eax
        call   sub_1_0568
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_1B9E
loc_1_1AD6:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 178h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 17ch], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_285E
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_1B2E
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b8h]
        push   eax
        call   sub_1_05CE
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b8h]
        push   eax
        call   sub_1_0568
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_1B9E
loc_1_1B2E:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 120h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 114h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 86h
        push   eax
        db  068h, 059h, 014h, 000h, 000h
        db  068h, 041h, 012h, 000h, 000h
        db  068h, 087h, 013h, 000h, 000h
        db  068h, 0B5h, 012h, 000h, 000h
        db  0E8h, 09Eh, 0E4h, 0FEh, 0FFh
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_1B97
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b8h]
        push   eax
        call   sub_1_05CE
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b8h]
        push   eax
        call   sub_1_0568
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_1B9E
loc_1_1B97:
        sub    eax, eax
        jmp    NEAR PTR loc_1_1B9E
loc_1_1B9E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
loc_1_1BA5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0bch
        push   eax
        call   sub_1_05EC
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 120h]
        push   eax
        db  0E8h, 0FFh, 0E4h, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0b8h]
        push   eax
        call   sub_1_0568
        push   0
        push   18ch
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_0604
        jmp    NEAR PTR loc_1_1BF5
loc_1_1BF5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_1BFC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 4]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_055C
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_0550
        jmp    NEAR PTR loc_1_1C27
loc_1_1C27:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_1C2E:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 14h], 1
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 24h], eax
        mov    dword ptr [ebp - 20h], 4
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 28h], eax
        jmp    NEAR PTR loc_1_1E3E
loc_1_1C64:
        mov    dword ptr [ebp - 8], 2
        jmp    NEAR PTR loc_1_1EE7
loc_1_1C70:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        or     eax, eax
        jne    NEAR PTR loc_1_1C90
        db  0C7h, 045h, 0DCh, 0B4h, 042h, 000h, 000h
        mov    dword ptr [ebp - 20h], 5ch
        jmp    NEAR PTR loc_1_1C9E
loc_1_1C90:
        db  0C7h, 045h, 0DCh, 030h, 042h, 000h, 000h
        mov    dword ptr [ebp - 20h], 84h
loc_1_1C9E:
        jmp    NEAR PTR loc_1_1EE7
loc_1_1CA3:
        mov    dword ptr [ebp - 0ch], 0
        lea    eax, [ebp - 0ch]
        mov    dword ptr [ebp - 24h], eax
        mov    dword ptr [ebp - 20h], 4
        jmp    NEAR PTR loc_1_1EE7
loc_1_1CBC:
        lea    eax, [ebp - 14h]
        mov    dword ptr [ebp - 24h], eax
        mov    dword ptr [ebp - 20h], 4
        jmp    NEAR PTR loc_1_1EE7
loc_1_1CCE:
        mov    dword ptr [ebp - 8], 0d0h
        jmp    NEAR PTR loc_1_1EE7
loc_1_1CDA:
        mov    dword ptr [ebp - 8], 0ff2h
        jmp    NEAR PTR loc_1_1EE7
loc_1_1CE6:
        mov    dword ptr [ebp - 8], 1000h
        jmp    NEAR PTR loc_1_1EE7
loc_1_1CF2:
        jmp    NEAR PTR loc_1_1EE7
loc_1_1CF7:
        mov    dword ptr [ebp - 8], 1000h
        jmp    NEAR PTR loc_1_1EE7
loc_1_1D03:
        mov    dword ptr [ebp - 8], 1000h
        jmp    NEAR PTR loc_1_1EE7
loc_1_1D0F:
        mov    dword ptr [ebp - 8], 1000h
        jmp    NEAR PTR loc_1_1EE7
loc_1_1D1B:
        mov    dword ptr [ebp - 8], 1000h
        jmp    NEAR PTR loc_1_1EE7
loc_1_1D27:
        push   3
        mov    eax, dword ptr [ebp + 8]
        add    eax, 80h
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_06E4
        and    dword ptr [ebp - 8], 0ffffff00h
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 24h], eax
        mov    dword ptr [ebp - 20h], 4
        jmp    NEAR PTR loc_1_1EE7
loc_1_1D54:
        db  0C7h, 045h, 0DCh, 0D8h, 043h, 000h, 000h
        mov    dword ptr [ebp - 20h], 1ch
        jmp    NEAR PTR loc_1_1EE7
loc_1_1D67:
        mov    word ptr [ebp - 10h], 300h
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 24h], eax
        mov    dword ptr [ebp - 20h], 2
        jmp    NEAR PTR loc_1_1EE7
loc_1_1D7F:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        or     eax, eax
        je     NEAR PTR loc_1_1D9D
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 124h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1DA9
loc_1_1D9D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
loc_1_1DA9:
        jmp    NEAR PTR loc_1_1EE7
loc_1_1DAE:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 184h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1EE7
loc_1_1DBF:
        push   6
        mov    eax, dword ptr [ebp + 8]
        add    eax, 80h
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_1_06E4
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 24h], eax
        mov    dword ptr [ebp - 20h], 6
        jmp    NEAR PTR loc_1_1EE7
loc_1_1DE5:
        push   6
        mov    eax, dword ptr [ebp + 8]
        add    eax, 86h
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_1_06E4
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 24h], eax
        mov    dword ptr [ebp - 20h], 6
        jmp    NEAR PTR loc_1_1EE7
loc_1_1E0B:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 9ch]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1EE7
loc_1_1E1C:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0a0h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_1EE7
loc_1_1E2D:
        mov    dword ptr [ebp - 4], 0c00000bbh
        jmp    NEAR PTR loc_1_1EE7
        db  0E9h, 0A9h, 000h, 000h, 000h
loc_1_1E3E:
        cmp    dword ptr [ebp - 28h], 2010101h
        db  00Fh, 087h, 070h, 000h, 000h, 000h
        je     NEAR PTR loc_1_1DBF
        sub    dword ptr [ebp - 28h], 10101h
        cmp    dword ptr [ebp - 28h], 12h
        ja     NEAR PTR loc_1_1E2D
        shl    dword ptr [ebp - 28h], 2
        mov    eax, dword ptr [ebp - 28h]
        db  0FFh, 0A0h, 06Fh, 01Eh, 000h, 000h
loc_1_1E6F:
        db  070h, 01Ch
        add    byte ptr [eax], al
        db  0A3h, 01Ch, 000h, 000h, 0BCh
        sbb    al, 0
        db  000h, 0BCh, 01Ch, 000h, 000h, 0CEh, 01Ch
        add    byte ptr [eax], al
        ficomp dword ptr [eax + eax]
        db  000h, 0F2h
        sbb    al, 0
        db  000h, 0F7h
        sbb    al, 0
        db  000h, 003h
        db  01Dh, 000h, 000h, 00Fh, 01Dh
        add    byte ptr [eax], al
        db  01Bh, 01Dh, 000h, 000h, 027h, 01Dh
        add    byte ptr [eax], al
        push   esp
        db  01Dh, 000h, 000h, 07Fh, 01Dh
        add    byte ptr [eax], al
        scasb
        db  01Dh, 000h, 000h, 067h, 01Dh
        add    byte ptr [eax], al
        out    1ch, al
        add    byte ptr [eax], al
        db  02Dh, 01Eh, 000h, 000h, 064h
        sbb    al, 0
        add    byte ptr [ecx + 102d87dh], al
        add    dword ptr [edx], eax
        je     NEAR PTR loc_1_1DE5
        cmp    dword ptr [ebp - 28h], 2010103h
        je     NEAR PTR loc_1_1E0B
        cmp    dword ptr [ebp - 28h], 2010104h
        je     NEAR PTR loc_1_1E1C
        jmp    NEAR PTR loc_1_1E2D
loc_1_1EE7:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_1F2A
        mov    eax, dword ptr [ebp - 20h]
        cmp    dword ptr [ebp + 1ch], eax
        jae    NEAR PTR loc_1_1F11
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebp - 4], 0c0010014h
        jmp    NEAR PTR loc_1_1F2A
loc_1_1F11:
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_06E4
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], eax
loc_1_1F2A:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1F32
loc_1_1F32:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    20h
sub_1_1F39:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 4], 0
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1C2E
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_1FAC
loc_1_1FAC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_1FB3:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 1ch], eax
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 4
        je     NEAR PTR loc_1_2018
        mov    dword ptr [ebp - 4], 0c0010014h
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_2145
loc_1_2018:
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_20DA
loc_1_2023:
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0CEC
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_2120
loc_1_2040:
        push   4
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        call   sub_1_06E4
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_214C
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_2120
loc_1_206C:
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0D8F
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_2120
loc_1_2089:
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_2120
loc_1_2095:
        push   4
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 184h
        push   eax
        call   sub_1_06E4
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_2120
loc_1_20B5:
        mov    dword ptr [ebp - 4], 0c0010017h
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 24h], 0
        jmp    NEAR PTR loc_1_2120
        db  0E9h, 046h, 000h, 000h, 000h
loc_1_20DA:
        cmp    dword ptr [ebp - 20h], 1010eh
        je     NEAR PTR loc_1_2040
        cmp    dword ptr [ebp - 20h], 1010fh
        je     NEAR PTR loc_1_2095
        cmp    dword ptr [ebp - 20h], 10112h
        je     NEAR PTR loc_1_2089
        cmp    dword ptr [ebp - 20h], 2010103h
        je     NEAR PTR loc_1_2023
        cmp    dword ptr [ebp - 20h], 2010104h
        je     NEAR PTR loc_1_206C
        jmp    NEAR PTR loc_1_20B5
loc_1_2120:
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_213D
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 24h], 0
loc_1_213D:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_2145
loc_1_2145:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_214C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        test   dword ptr [ebp + 14h], 0a076h
        je     NEAR PTR loc_1_2173
        mov    eax, 0c00000bbh
        jmp    NEAR PTR loc_1_21AA
loc_1_2173:
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 120h]
        push   eax
        db  0E8h, 05Dh, 0E3h, 0FEh, 0FFh
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 124h], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_21AA
loc_1_21AA:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_21C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 128h], 0
        jne    NEAR PTR loc_1_21F2
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_1_2226
loc_1_21F2:
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2493
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 178h], 1
        jne    NEAR PTR loc_1_221C
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_222D
loc_1_221C:
        mov    eax, 103h
        jmp    NEAR PTR loc_1_2226
loc_1_2226:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_222D:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a8h]
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_0628
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 24h]
        push   eax
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        call   sub_1_064C
        mov    al, byte ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0a8h]
        mov    byte ptr [ecx + 7], al
        mov    al, byte ptr [ebp - 13h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0a8h]
        mov    byte ptr [ecx + 6], al
        mov    al, byte ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0a8h]
        mov    byte ptr [ecx + 9], al
        mov    al, byte ptr [ebp - 13h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0a8h]
        mov    byte ptr [ecx + 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a8h]
        mov    eax, dword ptr [eax + 0ah]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_31EC
        mov    dword ptr [ebp - 18h], eax
        push   2
        lea    eax, [ebp - 18h]
        push   eax
        call   sub_1_324D
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b0h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_234F
        mov    byte ptr [ebp - 10h], 0b9h
        mov    al, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0a8h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a8h]
        mov    byte ptr [eax + 5], 0
        push   100h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3760
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a8h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_31EC
        mov    dword ptr [ebp - 18h], eax
        push   2
        lea    eax, [ebp - 18h]
        push   eax
        call   sub_1_324D
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0a8h], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_234F:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 10h], 0
        jne    NEAR PTR loc_1_2377
        jmp    NEAR PTR loc_1_248C
loc_1_2377:
        push   0
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0628
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_239B
        jmp    NEAR PTR loc_1_248C
loc_1_239B:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_064C
loc_1_23AC:
        mov    eax, dword ptr [ebp + 10h]
        cmp    dword ptr [ebp - 10h], eax
        jae    NEAR PTR loc_1_2484
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_23F4
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_0670
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_23DE
        jmp    NEAR PTR loc_1_2484
loc_1_23DE:
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_064C
        jmp    NEAR PTR loc_1_23AC
loc_1_23F4:
        cmp    dword ptr [ebp + 0ch], 0
        je     NEAR PTR loc_1_243C
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 0ch], eax
        jae    NEAR PTR loc_1_2425
        sub    eax, eax
        sub    eax, dword ptr [ebp - 0ch]
        neg    eax
        sub    dword ptr [ebp + 0ch], eax
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_23AC
        db  0E9h, 017h, 000h, 000h, 000h
loc_1_2425:
        mov    eax, dword ptr [ebp + 0ch]
        add    dword ptr [ebp - 8], eax
        sub    eax, eax
        sub    eax, dword ptr [ebp + 0ch]
        neg    eax
        sub    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp + 0ch], 0
loc_1_243C:
        mov    eax, dword ptr [ebp + 10h]
        sub    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_244F
        mov    eax, ecx
loc_1_244F:
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        call   sub_1_06E4
        mov    eax, dword ptr [ebp - 18h]
        add    dword ptr [ebp + 14h], eax
        mov    eax, dword ptr [ebp - 18h]
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 18h]
        add    dword ptr [ebp - 10h], eax
        sub    eax, eax
        sub    eax, dword ptr [ebp - 18h]
        neg    eax
        sub    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_23AC
loc_1_2484:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
loc_1_248C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
sub_1_2493:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 17ch], 0
        jne    NEAR PTR loc_1_24CC
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 17ch], eax
        jmp    NEAR PTR loc_1_24DB
loc_1_24CC:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 180h]
        mov    dword ptr [ecx + 24h], eax
loc_1_24DB:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 28h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 180h], eax
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 178h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_2500:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 178h], 1
        jne    NEAR PTR loc_1_2531
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 17ch], 0
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 178h]
        jmp    NEAR PTR loc_1_2562
loc_1_2531:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 17ch]
        mov    eax, dword ptr [eax + 24h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 17ch], eax
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 178h]
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 17ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_222D
loc_1_2562:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_2570:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 5ch]
        mov    byte ptr [ebp - 0ch], al
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 5dh]
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 0ch]
        mov    dword ptr [ebp - 8], eax
        shl    dword ptr [ebp - 8], 8
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ach]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_25D2
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ach]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 1fh
        add    eax, 0eh
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_25D9
loc_1_25D2:
        mov    dword ptr [ebp - 10h], 0eh
loc_1_25D9:
        mov    eax, dword ptr [ebp - 8]
        sub    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        sub    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ach]
        add    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ach]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ach]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 120h]
        push   eax
        call   sub_1_00B0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 120h]
        push   eax
        call   sub_1_0300
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_2631:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0ach]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_267C
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0ach]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 1fh
        add    eax, 0eh
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_2683
loc_1_267C:
        mov    dword ptr [ebp - 10h], 0eh
loc_1_2683:
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0ach]
        add    eax, dword ptr [ebp - 10h]
        add    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_26BC
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 1ch]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        jmp    NEAR PTR loc_1_26B5
loc_1_26B5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    18h
sub_1_26BC:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 18h], 0
        mov    eax, dword ptr [ebp + 18h]
        mov    dword ptr [eax], 0
        cmp    dword ptr [ebp + 0ch], 0
        jne    NEAR PTR loc_1_26E4
        jmp    NEAR PTR loc_1_2805
loc_1_26E4:
        push   0
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_0628
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_2708
        jmp    NEAR PTR loc_1_2805
loc_1_2708:
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_064C
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], eax
loc_1_271F:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 18h], eax
        jae    NEAR PTR loc_1_27FD
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_2767
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0670
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_2751
        jmp    NEAR PTR loc_1_27FD
loc_1_2751:
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_064C
        jmp    NEAR PTR loc_1_271F
loc_1_2767:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_27AF
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [ebp - 4], eax
        jae    NEAR PTR loc_1_2798
        sub    eax, eax
        sub    eax, dword ptr [ebp - 4]
        neg    eax
        sub    dword ptr [ebp + 14h], eax
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_271F
        db  0E9h, 017h, 000h, 000h, 000h
loc_1_2798:
        mov    eax, dword ptr [ebp + 14h]
        add    dword ptr [ebp - 8], eax
        sub    eax, eax
        sub    eax, dword ptr [ebp + 14h]
        neg    eax
        sub    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp + 14h], 0
loc_1_27AF:
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_27CE
        mov    eax, ecx
loc_1_27CE:
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_06E4
        mov    eax, dword ptr [ebp - 1ch]
        add    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        add    dword ptr [ebp - 18h], eax
        sub    eax, eax
        sub    eax, dword ptr [ebp - 1ch]
        neg    eax
        sub    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_271F
loc_1_27FD:
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
loc_1_2805:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
        db  000h, 000h, 000h, 000h
sub_1_2810:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   8
        push   10h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_31A1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_282A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0
        push   10h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_31A1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   18h
        push   10h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_31A1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_285E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 188h], 1
        jne    NEAR PTR loc_1_28B3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_293F
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_2891
        sub    al, al
        jmp    NEAR PTR loc_1_2938
loc_1_2891:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_29CF
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2A01
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2A68
        mov    al, 1
        jmp    NEAR PTR loc_1_2938
loc_1_28B3:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_293F
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_2916
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 150h], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_32F0
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_2916
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 150h], 1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_32F0
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_2916
        sub    al, al
        jmp    NEAR PTR loc_1_2938
loc_1_2916:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_29CF
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2A01
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2A68
        mov    al, 1
        jmp    NEAR PTR loc_1_2938
loc_1_2938:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_293F:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        db  0BEh, 0C8h, 0A6h, 000h, 000h
        lea    edi, [ebp - 20h]
        mov    ecx, 7
        rep movsd
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_2966
loc_1_2963:
        inc    dword ptr [ebp - 4]
loc_1_2966:
        cmp    dword ptr [ebp - 4], 1ch
        jge    NEAR PTR loc_1_2992
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 164h]
        mov    al, byte ptr [ecx + eax*2 + 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 8ch], al
        jmp    NEAR PTR loc_1_2963
loc_1_2992:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 92h], 20h
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [ebp - 20h]
        mov    ecx, 7
        lea    esi, [esi + 8ch]
        repe cmpsd
        jne    NEAR PTR loc_1_29C1
        mov    al, 1
        jmp    NEAR PTR loc_1_29C8
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_29C1:
        sub    al, al
        jmp    NEAR PTR loc_1_29C8
loc_1_29C8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_29CF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 168h]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 164h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 800h], 41h
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_2A01:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_2A19
loc_1_2A16:
        inc    dword ptr [ebp - 4]
loc_1_2A19:
        cmp    dword ptr [ebp - 4], 6
        jge    NEAR PTR loc_1_2A48
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 164h]
        mov    al, byte ptr [ecx + eax*2 + 104h]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx + 80h], al
        jmp    NEAR PTR loc_1_2A16
loc_1_2A48:
        push   6
        mov    eax, dword ptr [ebp + 8]
        add    eax, 80h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 86h
        push   eax
        call   sub_1_06E4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_2A68:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 164h]
        mov    dword ptr [ebp - 4], eax
        push   4
        push   10h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_31A1
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_2A99
loc_1_2A96:
        inc    dword ptr [ebp - 8]
loc_1_2A99:
        cmp    dword ptr [ebp - 8], 59a6h
        jae    NEAR PTR loc_1_2AB2
        push   64h
        call   sub_1_05F8
        jmp    NEAR PTR loc_1_2A96
loc_1_2AB2:
        push   0
        push   10h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_31A1
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_2ACE
loc_1_2ACB:
        inc    dword ptr [ebp - 8]
loc_1_2ACE:
        cmp    dword ptr [ebp - 8], 59a6h
        jae    NEAR PTR loc_1_2AE7
        push   64h
        call   sub_1_05F8
        jmp    NEAR PTR loc_1_2ACB
loc_1_2AE7:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 154h], 0
        jne    NEAR PTR loc_1_2B2C
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 7eeh]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_2B1F
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 154h], 10h
        jmp    NEAR PTR loc_1_2B2C
loc_1_2B1F:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 154h], 4
loc_1_2B2C:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 154h], 4
        jne    NEAR PTR loc_1_2B61
        push   0ebh
        push   8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_317F
        push   16bh
        push   8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_317F
        jmp    NEAR PTR loc_1_2B81
loc_1_2B61:
        push   0e9h
        push   8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_317F
        push   169h
        push   8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_317F
loc_1_2B81:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_2B88:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 14h], 0
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 14h]
        shr    eax, 1
        mov    dword ptr [ebp - 8], eax
loc_1_2BB5:
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_2C15
        mov    dword ptr [ebp - 0ch], 0
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        mov    dword ptr [ebp - 0ch], eax
        inc    dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax]
        and    eax, 0ffh
        shl    eax, 8
        mov    dword ptr [ebp - 10h], eax
        inc    dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0fh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_317F
        dec    dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_2BB5
loc_1_2C15:
        test   byte ptr [ebp + 14h], 1
        je     NEAR PTR loc_1_2C35
        mov    eax, dword ptr [ebp + 10h]
        mov    al, byte ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0fh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_31A1
loc_1_2C35:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_2C3C:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 14h]
        shr    eax, 1
        mov    dword ptr [ebp - 4], eax
loc_1_2C4D:
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_2C9C
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3151
        and    eax, 0ffffh
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffh
        mov    dword ptr [ebp - 8], eax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp + 10h]
        mov    eax, dword ptr [ebp - 0ch]
        shr    eax, 8
        mov    dword ptr [ebp - 10h], eax
        mov    al, byte ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx], al
        inc    dword ptr [ebp + 10h]
        dec    dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_2C4D
loc_1_2C9C:
        test   byte ptr [ebp + 14h], 1
        je     NEAR PTR loc_1_2CB6
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_31C1
        mov    ecx, dword ptr [ebp + 10h]
        mov    byte ptr [ecx], al
loc_1_2CB6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0fffffffch
        mov    dword ptr [ebp - 4], eax
        and    dword ptr [ebp + 10h], 3
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 168h]
        push   eax
        call   sub_1_06E4
loc_1_2CEA:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_2D17
        push   1
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 168h]
        push   eax
        call   sub_1_06E4
        dec    dword ptr [ebp + 10h]
        inc    dword ptr [ebp + 0ch]
        jmp    NEAR PTR loc_1_2CEA
loc_1_2D17:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 0fffffffch
        mov    dword ptr [ebp - 4], eax
        and    dword ptr [ebp + 10h], 3
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 168h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_06E4
loc_1_2D4B:
        cmp    dword ptr [ebp + 10h], 0
        je     NEAR PTR loc_1_2D78
        push   1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 168h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_06E4
        dec    dword ptr [ebp + 10h]
        inc    dword ptr [ebp + 0ch]
        jmp    NEAR PTR loc_1_2D4B
loc_1_2D78:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_2D7F:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
loc_1_2D8F:
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp - 4]
        db  08Ah, 080h, 098h, 046h, 000h, 000h
        and    eax, 0ffh
        shl    eax, 8
        mov    dword ptr [ebp - 8], eax
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        db  08Ah, 080h, 098h, 046h, 000h, 000h
        and    eax, 0ffh
        add    dword ptr [ebp - 8], eax
        inc    dword ptr [ebp - 4]
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp - 4]
        db  08Ah, 080h, 098h, 046h, 000h, 000h
        and    eax, 0ffh
        shl    eax, 8
        mov    dword ptr [ebp - 10h], eax
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        db  08Ah, 080h, 098h, 046h, 000h, 000h
        and    eax, 0ffh
        add    dword ptr [ebp - 10h], eax
        inc    dword ptr [ebp - 4]
        mov    dword ptr [ebp - 14h], 0
        mov    eax, dword ptr [ebp - 4]
        db  08Ah, 080h, 098h, 046h, 000h, 000h
        and    eax, 0ffh
        shl    eax, 8
        mov    dword ptr [ebp - 14h], eax
        inc    dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 4]
        db  08Ah, 080h, 098h, 046h, 000h, 000h
        and    eax, 0ffh
        add    dword ptr [ebp - 14h], eax
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_2E39
        jmp    NEAR PTR loc_1_2E83
        db  0E9h, 04Ah, 000h, 000h, 000h
loc_1_2E39:
        mov    dword ptr [ebp - 0ch], 22h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0ch
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_317F
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_317F
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        db  005h, 098h, 046h, 000h, 000h
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2B88
        mov    eax, dword ptr [ebp - 14h]
        add    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_2D8F
loc_1_2E83:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 154h], 4
        jne    NEAR PTR loc_1_2EA8
        push   12bh
        push   8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_317F
        jmp    NEAR PTR loc_1_2EB8
loc_1_2EA8:
        push   129h
        push   8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_317F
loc_1_2EB8:
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3151
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_2ED2
loc_1_2ECF:
        inc    dword ptr [ebp - 4]
loc_1_2ED2:
        cmp    dword ptr [ebp - 4], 59a6h
        jae    NEAR PTR loc_1_2EEB
        push   64h
        call   sub_1_05F8
        jmp    NEAR PTR loc_1_2ECF
loc_1_2EEB:
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3151
        mov    ecx, eax
        and    ecx, 0ffffh
        cmp    ecx, 40h
        je     NEAR PTR loc_1_2F13
        sub    al, al
        jmp    NEAR PTR loc_1_2F1A
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_2F13:
        mov    al, 1
        jmp    NEAR PTR loc_1_2F1A
loc_1_2F1A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_2F21:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1], 9fh
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0ch], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0dh], 1
        mov    eax, dword ptr [ebp + 8]
        add    eax, 16h
        push   eax
        call   sub_1_31EC
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 0eh], ecx
        mov    eax, dword ptr [ebp + 8]
        add    eax, 1ch
        push   eax
        call   sub_1_31EC
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 12h], ecx
        mov    dword ptr [ebp - 0ch], 2
        mov    dword ptr [ebp - 8], 1
        mov    dword ptr [ebp - 14h], 0a00h
        mov    dword ptr [ebp - 10h], 16h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        push   0ch
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_317F
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_317F
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2B88
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 134h], 0
        push   1000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3760
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_2FEE
loc_1_2FEB:
        inc    dword ptr [ebp - 4]
loc_1_2FEE:
        cmp    dword ptr [ebp - 4], 59a6h
        jae    NEAR PTR loc_1_3007
        push   64h
        call   sub_1_05F8
        jmp    NEAR PTR loc_1_2FEB
loc_1_3007:
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3151
        and    eax, 0ffffh
        test   al, 10h
        je     NEAR PTR loc_1_3029
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_30D9
loc_1_3029:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 24h], 2
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 25h], 0
        push   6
        mov    eax, dword ptr [ebp + 8]
        add    eax, 86h
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 26h
        push   eax
        call   sub_1_06E4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 31h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 30h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 35h], 0eh
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 37h], 0eh
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 38h], 14h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 39h], 14h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 3fh], 3
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 3eh], 2
        db  0B8h, 0E8h, 0A6h, 000h, 000h
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 44h
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    edx, dword ptr [eax + 4]
        mov    dword ptr [ecx + 4], edx
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 8], edx
        mov    edx, dword ptr [eax + 0ch]
        mov    dword ptr [ecx + 0ch], edx
        mov    ax, word ptr [eax + 10h]
        mov    word ptr [ecx + 10h], ax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 44h
        push   eax
        call   sub_1_31EC
        mov    dword ptr [ebp - 18h], eax
        push   2
        lea    eax, [ebp - 18h]
        push   eax
        call   sub_1_324D
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 18h]
        mov    dword ptr [eax + 40h], ecx
        sub    eax, eax
        jmp    NEAR PTR loc_1_30D9
loc_1_30D9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0ff00h
        push   8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_317F
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_3108
loc_1_3105:
        inc    dword ptr [ebp - 4]
loc_1_3108:
        cmp    dword ptr [ebp - 4], 59a6h
        jae    NEAR PTR loc_1_3121
        push   64h
        call   sub_1_05F8
        jmp    NEAR PTR loc_1_3105
loc_1_3121:
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3151
        and    eax, 0ffffh
        test   al, 30h
        je     NEAR PTR loc_1_3143
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_314A
loc_1_3143:
        sub    eax, eax
        jmp    NEAR PTR loc_1_314A
loc_1_314A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_3151:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 16ch]
        add    eax, dword ptr [ebp + 0ch]
        mov    dx, ax
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    ax, word ptr [ebp - 4]
        jmp    NEAR PTR loc_1_3178
loc_1_3178:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_317F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 16ch]
        add    eax, dword ptr [ebp + 0ch]
        mov    dx, ax
        mov    ax, word ptr [ebp + 10h]
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_31A1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 16ch]
        add    eax, dword ptr [ebp + 0ch]
        mov    dx, ax
        mov    al, byte ptr [ebp + 10h]
        out    dx, al
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_31C1:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 16ch]
        add    eax, dword ptr [ebp + 0ch]
        mov    dx, ax
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        jmp    NEAR PTR loc_1_31E5
loc_1_31E5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_31EC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 10h
        mov    dword ptr [ebp - 4], eax
        shl    dword ptr [ebp + 8], 10h
        mov    eax, dword ptr [ebp + 8]
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_3217
loc_1_3217:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_321E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        shl    dword ptr [ebp - 4], 10h
        mov    eax, dword ptr [ebp + 0ch]
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_3246
loc_1_3246:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_324D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_3265
loc_1_3262:
        inc    dword ptr [ebp - 4]
loc_1_3265:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 4], eax
        jge    NEAR PTR loc_1_32A0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 8], al
        inc    dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 0ch], al
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx], al
        dec    dword ptr [ebp + 8]
        mov    al, byte ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx], al
        add    dword ptr [ebp + 8], 2
        jmp    NEAR PTR loc_1_3262
loc_1_32A0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_32A7:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 16h], 0
        mov    al, byte ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + 17h], al
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_31EC
        mov    dword ptr [ebp + 10h], eax
        push   2
        lea    eax, [ebp + 10h]
        push   eax
        call   sub_1_324D
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 18h], ecx
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_32F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_365F
        push   0
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        push   15h
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_3334
loc_1_3331:
        inc    dword ptr [ebp - 8]
loc_1_3334:
        cmp    dword ptr [ebp - 8], 46h
        jae    NEAR PTR loc_1_334A
        push   64h
        call   sub_1_05F8
        jmp    NEAR PTR loc_1_3331
loc_1_334A:
        push   95h
        push   2
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_365F
        and    eax, 0ffh
        and    al, 10h
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        or     al, byte ptr [ecx + 140h]
        mov    byte ptr [ebp - 10h], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        or     eax, 20h
        push   eax
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        xor    eax, eax
        mov    al, byte ptr [ebp - 10h]
        or     eax, 60h
        push   eax
        push   3
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
loc_1_33A8:
        cmp    dword ptr [ebp - 0ch], 32h
        jae    NEAR PTR loc_1_33E0
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_365F
        and    eax, 0ffh
        test   al, 20h
        jne    NEAR PTR loc_1_33E0
        push   64h
        call   sub_1_05F8
        push   64h
        call   sub_1_05F8
        inc    dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_33A8
loc_1_33E0:
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_33EF
loc_1_33EC:
        inc    dword ptr [ebp - 8]
loc_1_33EF:
        cmp    dword ptr [ebp - 8], 46h
        jae    NEAR PTR loc_1_3405
        push   64h
        call   sub_1_05F8
        jmp    NEAR PTR loc_1_33EC
loc_1_3405:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 148h]
        and    eax, 0ffh
        push   eax
        push   8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 148h]
        shr    eax, 8
        push   eax
        push   9
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 148h]
        add    eax, 1fh
        and    eax, 0ffh
        push   eax
        push   0ah
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 148h]
        add    eax, 1fh
        shr    eax, 8
        push   eax
        push   0bh
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        push   2
        push   7
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14ch]
        shr    eax, 8
        push   eax
        push   10h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        push   0
        push   11h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14ch]
        shr    eax, 8
        push   eax
        push   12h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        push   0
        push   13h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14ch]
        shr    eax, 8
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        not    eax
        inc    eax
        mov    dword ptr [ebp - 0ch], eax
        and    dword ptr [ebp - 0ch], 7fffh
        or     dword ptr [ebp - 0ch], 4000h
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffh
        push   eax
        push   14h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        mov    eax, dword ptr [ebp - 0ch]
        shr    eax, 8
        push   eax
        push   15h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14ch]
        add    eax, 100h
        shr    eax, 8
        push   eax
        push   18h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        push   0
        push   19h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14ch]
        add    eax, 100h
        shr    eax, 8
        push   eax
        push   1ah
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        push   0
        push   1bh
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 14ch]
        add    eax, 100h
        shr    eax, 8
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        not    eax
        inc    eax
        mov    dword ptr [ebp - 0ch], eax
        add    dword ptr [ebp - 0ch], 20h
        and    dword ptr [ebp - 0ch], 3fffh
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffh
        push   eax
        push   1ch
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        push   3fh
        push   1dh
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        push   63h
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3631
        mov    dword ptr [ebp - 14h], 7
loc_1_35C3:
        mov    dword ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_35D2
loc_1_35CF:
        inc    dword ptr [ebp - 8]
loc_1_35D2:
        cmp    dword ptr [ebp - 8], 14h
        jae    NEAR PTR loc_1_35E8
        push   64h
        call   sub_1_05F8
        jmp    NEAR PTR loc_1_35CF
loc_1_35E8:
        push   1
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_365F
        and    eax, 0ffh
        test   al, 0ch
        jne    NEAR PTR loc_1_3613
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 18h], eax
        dec    dword ptr [ebp - 14h]
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_35C3
loc_1_3613:
        inc    dword ptr [ebp - 14h]
        jne    NEAR PTR loc_1_3623
        sub    al, al
        jmp    NEAR PTR loc_1_362A
loc_1_3623:
        mov    al, 1
        jmp    NEAR PTR loc_1_362A
loc_1_362A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_3631:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        shl    eax, 6
        add    eax, dword ptr [ebp + 0ch]
        mov    dx, 3e0h
        out    dx, al
        mov    dx, 3e1h
        mov    al, byte ptr [ebp + 10h]
        out    dx, al
        jmp    NEAR PTR loc_1_3658
loc_1_3658:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_365F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        shl    eax, 6
        add    eax, dword ptr [ebp + 0ch]
        mov    dx, 3e0h
        out    dx, al
        mov    dx, 3e1h
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        jmp    NEAR PTR loc_1_368C
loc_1_368C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
loc_1_36A3:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        shl    eax, 6
        add    eax, dword ptr [ebp - 4]
        mov    dx, 3e0h
        out    dx, al
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 8
        jl     NEAR PTR loc_1_36A3
loc_1_36C4:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        shl    eax, 6
        add    eax, dword ptr [ebp - 4]
        mov    dx, 3e0h
        out    dx, al
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0ch
        jl     NEAR PTR loc_1_36C4
loc_1_36E5:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        shl    eax, 6
        add    eax, dword ptr [ebp - 4]
        mov    dx, 3e0h
        out    dx, al
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 10h
        jl     NEAR PTR loc_1_36E5
loc_1_3706:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        shl    eax, 6
        add    eax, dword ptr [ebp - 4]
        mov    dx, 3e0h
        out    dx, al
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 16h
        jl     NEAR PTR loc_1_3706
        inc    dword ptr [ebp - 4]
        inc    dword ptr [ebp - 4]
loc_1_372D:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 150h]
        shl    eax, 6
        add    eax, dword ptr [ebp - 4]
        mov    dx, 3e0h
        out    dx, al
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 1eh
        jl     NEAR PTR loc_1_372D
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3760:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_293F
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_3783
        sub    al, al
        jmp    NEAR PTR loc_1_37BC
loc_1_3783:
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 8080h
        push   eax
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_317F
loc_1_3797:
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3151
        and    eax, 0ffffh
        test   ah, 80h
        je     NEAR PTR loc_1_37B5
        jmp    NEAR PTR loc_1_3797
loc_1_37B5:
        mov    al, 1
        jmp    NEAR PTR loc_1_37BC
loc_1_37BC:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
loc_1_37C3:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        push   8
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_3151
        mov    ecx, eax
        and    ecx, 0ffffh
        cmp    ecx, 0ffffh
        jne    NEAR PTR loc_1_37F8
        sub    al, al
        jmp    NEAR PTR loc_1_3808
loc_1_37F8:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_282A
        mov    al, 1
        jmp    NEAR PTR loc_1_3808
loc_1_3808:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_380F:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        push   8
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3151
        and    eax, 0ffffh
        cmp    eax, 0ffffh
        jne    NEAR PTR loc_1_383E
        jmp    NEAR PTR loc_1_3A3D
loc_1_383E:
        push   8
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3151
        and    eax, 0ffffh
        mov    dword ptr [ebp - 10h], eax
        test   byte ptr [ebp - 0fh], 2
        je     NEAR PTR loc_1_3942
        mov    eax, dword ptr [ebp - 10h]
        or     eax, 800h
        push   eax
        push   8
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_317F
        push   4
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3151
        and    eax, 0ffffh
        push   eax
        push   6
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3151
        and    eax, 0ffffh
        push   eax
        call   sub_1_321E
        mov    dword ptr [ebp - 4], eax
        push   2
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3151
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        test   byte ptr [ebp - 0fh], 4
        je     NEAR PTR loc_1_38CE
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_2C3C
        jmp    NEAR PTR loc_1_38E1
loc_1_38CE:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_2B88
loc_1_38E1:
        mov    eax, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 134h], 0
        jne    NEAR PTR loc_1_393D
        push   64h
        call   sub_1_05F8
        push   6
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3151
        and    eax, 0ffffh
        test   al, 70h
        jne    NEAR PTR loc_1_393D
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [eax + 134h], 1
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 24h
        push   eax
        push   3
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_32A7
        push   1000h
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3760
loc_1_393D:
        jmp    NEAR PTR loc_1_3A34
loc_1_3942:
        push   6
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3151
        and    eax, 0ffffh
        mov    dword ptr [ebp - 14h], eax
        test   byte ptr [ebp - 14h], 80h
        je     NEAR PTR loc_1_3A34
        mov    eax, dword ptr [ebp - 14h]
        and    eax, 0ffffff7fh
        push   eax
        push   6
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_317F
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 18h], eax
        jmp    NEAR PTR loc_1_39E2
loc_1_397E:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3A44
        jmp    NEAR PTR loc_1_3A34
loc_1_3990:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3F9A
        jmp    NEAR PTR loc_1_3A34
loc_1_39A2:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_3A6E
        jmp    NEAR PTR loc_1_3A34
loc_1_39B4:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_40DD
        jmp    NEAR PTR loc_1_3A34
loc_1_39C6:
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_411D
        jmp    NEAR PTR loc_1_3A34
loc_1_39D8:
        jmp    NEAR PTR loc_1_3A34
        db  0E9h, 052h, 000h, 000h, 000h
loc_1_39E2:
        sub    dword ptr [ebp - 18h], 80h
        cmp    dword ptr [ebp - 18h], 0ch
        ja     NEAR PTR loc_1_3A34
        shl    dword ptr [ebp - 18h], 2
        mov    eax, dword ptr [ebp - 18h]
        db  0FFh, 0A0h, 000h, 03Ah, 000h, 000h
loc_1_3A00:
        db  07Eh, 039h
        add    byte ptr [eax], al
        fdivr  dword ptr [ecx]
        add    byte ptr [eax], al
        fdivr  dword ptr [ecx]
        add    byte ptr [eax], al
        fdivr  dword ptr [ecx]
        add    byte ptr [eax], al
        db  0A2h, 039h, 000h, 000h, 0D8h
        cmp    dword ptr [eax], eax
        db  000h, 090h, 039h, 000h, 000h, 0D8h
        cmp    dword ptr [eax], eax
        db  000h, 0A2h, 039h, 000h, 000h, 0D8h
        cmp    dword ptr [eax], eax
        db  000h, 0B4h, 039h, 000h, 000h, 0D8h, 039h
        add    byte ptr [eax], al
        db  0C6h, 039h, 000h, 000h
loc_1_3A34:
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_2810
loc_1_3A3D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_3A44:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   1
        push   0ch
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_317F
        push   5e0h
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_317F
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_3A6E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   2000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3760
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1eh]
        and    eax, 0ffff00ffh
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    edx, dword ptr [ebp + 8]
        mov    dl, byte ptr [edx + 1fh]
        and    edx, 0ffh
        add    edx, 8
        mov    cl, dl
        shl    eax, cl
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3B2F
loc_1_3AC6:
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_403C
        jmp    NEAR PTR loc_1_3B66
loc_1_3AD9:
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3B6D
        jmp    NEAR PTR loc_1_3B66
loc_1_3AEC:
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3C00
        jmp    NEAR PTR loc_1_3B66
loc_1_3AFF:
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3D49
        jmp    NEAR PTR loc_1_3B66
loc_1_3B12:
        mov    ax, word ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3C82
        jmp    NEAR PTR loc_1_3B66
        db  0E9h, 03Ch, 000h, 000h, 000h, 0E9h, 037h, 000h, 000h, 000h
loc_1_3B2F:
        cmp    dword ptr [ebp - 8], 1
        je     NEAR PTR loc_1_3AC6
        cmp    dword ptr [ebp - 8], 3
        je     NEAR PTR loc_1_3AD9
        cmp    dword ptr [ebp - 8], 7
        je     NEAR PTR loc_1_3AEC
        cmp    dword ptr [ebp - 8], 8
        je     NEAR PTR loc_1_3AFF
        cmp    dword ptr [ebp - 8], 9
        je     NEAR PTR loc_1_3B12
        jmp    NEAR PTR loc_1_3B66
loc_1_3B66:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_3B6D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f0h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 12ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3BE7
loc_1_3BA6:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3DE2
        jmp    NEAR PTR loc_1_3BF9
loc_1_3BB4:
        push   0
        push   0c0000001h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_056E
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 13ch]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 128h], 0
        jmp    NEAR PTR loc_1_3BF9
        db  0E9h, 012h, 000h, 000h, 000h
loc_1_3BE7:
        cmp    dword ptr [ebp - 8], 8000h
        je     NEAR PTR loc_1_3BA6
        jmp    NEAR PTR loc_1_3BB4
loc_1_3BF9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_3C00:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f4h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 130h], 0
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3C69
loc_1_3C32:
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_0574
        jmp    NEAR PTR loc_1_3C7B
loc_1_3C45:
        push   0c0000001h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_0574
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 13ch]
        jmp    NEAR PTR loc_1_3C7B
        db  0E9h, 012h, 000h, 000h, 000h
loc_1_3C69:
        cmp    dword ptr [ebp - 8], 8000h
        je     NEAR PTR loc_1_3C32
        jmp    NEAR PTR loc_1_3C45
loc_1_3C7B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_3C82:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 10ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3D30
loc_1_3CA8:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 138h], 1
        je     NEAR PTR loc_1_3CE1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f8h]
        mov    dword ptr [ebp - 4], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 104h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_058C
        jmp    NEAR PTR loc_1_3CEA
loc_1_3CE1:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1195
loc_1_3CEA:
        jmp    NEAR PTR loc_1_3D42
loc_1_3CEF:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 138h], 1
        je     NEAR PTR loc_1_3D26
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f8h]
        mov    dword ptr [ebp - 4], eax
        push   0c0000001h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 104h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_058C
loc_1_3D26:
        jmp    NEAR PTR loc_1_3D42
        db  0E9h, 012h, 000h, 000h, 000h
loc_1_3D30:
        cmp    dword ptr [ebp - 8], 8000h
        je     NEAR PTR loc_1_3CA8
        jmp    NEAR PTR loc_1_3CEF
loc_1_3D42:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_3D49:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 110h], 0
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_3DC9
loc_1_3D6F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fch]
        mov    dword ptr [ebp - 4], eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 108h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_058C
        jmp    NEAR PTR loc_1_3DDB
loc_1_3D98:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0fch]
        mov    dword ptr [ebp - 4], eax
        push   0c0000001h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 108h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_058C
        jmp    NEAR PTR loc_1_3DDB
        db  0E9h, 012h, 000h, 000h, 000h
loc_1_3DC9:
        cmp    dword ptr [ebp - 8], 8000h
        je     NEAR PTR loc_1_3D6F
        jmp    NEAR PTR loc_1_3D98
loc_1_3DDB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_3DE2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 72h
        push   eax
        call   sub_1_31EC
        mov    dword ptr [ebp - 4], eax
        push   2
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_324D
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 64h], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 68h], 39h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 69h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 6ch], 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 6dh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b0h]
        push   eax
        call   sub_1_31EC
        mov    dword ptr [ebp - 4], eax
        push   2
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_324D
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 6eh], ecx
        mov    eax, dword ptr [ebp + 8]
        add    eax, 64h
        push   eax
        call   sub_1_31EC
        mov    dword ptr [ebp - 4], eax
        push   2
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_324D
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 72h], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 76h], 39h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 77h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 7ah], 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 7bh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0b0h]
        push   eax
        call   sub_1_31EC
        mov    dword ptr [ebp - 4], eax
        push   2
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_324D
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 7ch], ecx
        mov    eax, dword ptr [ebp + 8]
        add    eax, 64h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0a8h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 64h
        push   eax
        push   4
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_32A7
        push   1800h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3760
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 174h], 4
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_3EF8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        add    eax, 56h
        push   eax
        call   sub_1_31EC
        mov    dword ptr [ebp - 4], eax
        push   2
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_324D
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 56h], ecx
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 5ah], 0bch
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 5bh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 5eh], 10h
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 5fh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0ach]
        push   eax
        call   sub_1_31EC
        mov    dword ptr [ebp - 4], eax
        push   2
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_324D
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 60h], ecx
        mov    eax, dword ptr [ebp + 8]
        add    eax, 56h
        push   eax
        push   6
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_32A7
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 174h], 6
        push   1800h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3760
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_3F9A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0f0h]
        mov    dword ptr [ebp - 4], eax
        push   2000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3760
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 174h]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_401C
loc_1_3FCE:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3EF8
        jmp    NEAR PTR loc_1_4035
loc_1_3FDC:
        push   0
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_056E
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 128h], 1
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 138h], 1
        jne    NEAR PTR loc_1_4012
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_41D7
loc_1_4012:
        jmp    NEAR PTR loc_1_4035
        db  0E9h, 019h, 000h, 000h, 000h
loc_1_401C:
        cmp    dword ptr [ebp - 8], 4
        je     NEAR PTR loc_1_3FCE
        cmp    dword ptr [ebp - 8], 6
        je     NEAR PTR loc_1_3FDC
        jmp    NEAR PTR loc_1_4035
loc_1_4035:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_403C:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0ch]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_40C4
loc_1_405C:
        cmp    dword ptr [ebp - 4], 80010004h
        jne    NEAR PTR loc_1_406E
        jmp    NEAR PTR loc_1_406E
loc_1_406E:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d4h]
        mov    dword ptr [ebp - 8], eax
loc_1_407A:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0d4h
        cmp    eax, dword ptr [ebp - 8]
        je     NEAR PTR loc_1_40B5
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0ch], eax
        push   4
        lea    eax, [ebp - 4]
        push   eax
        push   40010006h
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        jmp    NEAR PTR loc_1_407A
loc_1_40B5:
        jmp    NEAR PTR loc_1_40D6
        db  0E9h, 017h, 000h, 000h, 000h, 0E9h, 012h, 000h, 000h, 000h
loc_1_40C4:
        cmp    dword ptr [ebp - 10h], 5800h
        je     NEAR PTR loc_1_405C
        jmp    NEAR PTR loc_1_40D6
loc_1_40D6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_40DD:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2570
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 5ah], 0bch
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 5bh], 0
        push   2000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3760
        push   400h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3760
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_411D:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 1eh]
        and    eax, 0ffff00ffh
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx + 1fh]
        and    ecx, 0ffh
        add    ecx, 8
        shl    eax, cl
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_41BE
loc_1_4162:
        push   2000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3760
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 17ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 17ch]
        add    eax, 24h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2500
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_057A
        jmp    NEAR PTR loc_1_41D0
        db  0E9h, 017h, 000h, 000h, 000h, 0E9h, 012h, 000h, 000h, 000h
loc_1_41BE:
        cmp    dword ptr [ebp - 14h], 4000h
        je     NEAR PTR loc_1_4162
        jmp    NEAR PTR loc_1_41D0
loc_1_41D0:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_41D7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 158h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 10ch], 1
        mov    eax, dword ptr [ebp - 4]
        push   eax
        push   9
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_32A7
        push   1000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3760
        jmp    NEAR PTR loc_1_421B
loc_1_421B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_4230:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
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
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
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
        add    eax, 6020101h
        add    dword ptr [ecx], eax
        add    al, byte ptr [edi]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
        add    dword ptr [edx], eax
        add    al, byte ptr [edx]
        add    dword ptr [edx], eax
        db  002h
loc_1_42B4:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
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
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
        inc    ebx
        push   esp
        dec    esi
        inc    esp
        push   edi
        add    byte ptr [eax], al
        add    byte ptr [ebx + 54h], al
        dec    esi
        inc    esp
        push   edi
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6fh], cl
        inc    edx
        popad
        jae    SHORT loc_1_438B
        inc    ecx
        jb     NEAR PTR loc_1_4390
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6fh], cl
        inc    edx
        popad
        jae    SHORT loc_1_439B
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 65h], cl
        insd
        outsd
        jb     SHORT loc_1_43BF
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [ebp + 65h], cl
        insd
        outsd
        jb     SHORT loc_1_43CF
        inc    ecx
        db  064h, 064h, 072h, 065h
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [ecx + 52h], cl
        push   ecx
        push   esi
        popad
        insb
        db  075h, 065h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_436C:
        dec    ecx
        push   edx
        push   ecx
        push   esi
        popad
        insb
        db  075h, 065h
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        dec    ebp
        insd
        outsd
        db  072h, 079h
        inc    edx
        popad
        db  073h, 065h
        inc    ecx
        jb     NEAR PTR loc_1_43EC
        jae    SHORT loc_1_43FC
        add    byte ptr [eax], al
loc_1_438B:
        add    byte ptr [ebp + 65h], cl
        insd
        outsd
loc_1_4390:
        db  072h, 079h
        inc    edx
        popad
        db  073h, 065h
        inc    ecx
        jb     NEAR PTR loc_1_4400
loc_1_439B:
        db  073h, 073h
        add    byte ptr [eax], al
        add    byte ptr [ecx + 6eh], cl
        db  074h, 065h
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_1_43F8
        jne    SHORT loc_1_4419
        bound  esp, qword ptr [ebp + 72h]
        add    byte ptr [ecx + 6eh], cl
        je     SHORT loc_1_4419
        db  072h, 072h
        db  075h, 070h
        je     SHORT loc_1_4408
        jne    SHORT loc_1_4429
        bound  esp, qword ptr [ebp + 72h]
loc_1_43BF:
        add    byte ptr [edx + 69h], dl
        outsb
        push   ebx
        db  070h, 065h
        add    byte ptr fs:[eax], al
        add    byte ptr [edx + 69h], dl
        outsb
loc_1_43CF:
        push   ebx
        db  070h, 065h
        add    byte ptr fs:[eax], al
        add    byte ptr [eax + 69h], bl
        jb     SHORT loc_1_443F
        outsd
        insd
        and    byte ptr [ebx + 72h], al
        imul   esi, dword ptr fs:[ebx + eax*2 + 61h], 54206472h
        outsd
loc_1_43EC:
        imul   esp, dword ptr [ebp + 6eh], 20h
        push   edx
        imul   ebp, dword ptr [esi + 67h], 0
loc_1_43F8:
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
loc_1_43FC:
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
loc_1_4400:
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
loc_1_4408:
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
loc_1_4419:
        add    dword ptr [ecx], eax
        add    byte ptr [edi], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], cl
loc_1_4429:
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx + eax], cl
        add    dword ptr [eax], eax
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
loc_1_443F:
        add    byte ptr [edi], cl
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
        add    eax, 6020101h
        add    dword ptr [ecx], eax
        add    al, byte ptr [edi]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
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
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
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
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
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
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
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
        add    eax, 6020101h
        add    dword ptr [ecx], eax
        add    al, byte ptr [edi]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
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
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
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
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
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
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
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
        add    eax, 6020101h
        add    dword ptr [ecx], eax
        add    al, byte ptr [edi]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
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
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
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
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
loc_1_4698:
        add    byte ptr [eax], al
        mov    al, 0
        inc    eax
        add    byte ptr [eax + 54h], ch
        db  073h, 069h
        arpl   word ptr [eax], sp
        outsd
        and    byte ptr [ebp + 73h], ah
        imul   esi, dword ptr [edx + 20h], 6561656ch
        jae    NEAR PTR loc_1_46D3
        outsb
        db  075h, 065h
        and    byte ptr fs:[edx + 69h], dh
        insb
        arpl   word ptr gs:[ebx + 6eh], sp
        and    byte ptr [ebp + 6eh], ah
        dec    edi
        jns    SHORT loc_1_4732
        and    byte ptr [esp + ebp*2], ch
        inc    ecx
        and    byte ptr [ecx + ebp*2 + 72h], ch
        push   72747367h
loc_1_46D3:
        and    byte ptr [ebx + 65h], dh
        jb     SHORT loc_1_473D
        jbe    NEAR PTR loc_1_4709
        pushad
        add    dword ptr [edx + 1], eax
        add    byte ptr [eax], cl
        or     byte ptr [esi], dl
        mov    al, 3
        loopne loc_1_46EC
        add    byte ptr [eax], al
        and    byte ptr [ecx], al
loc_1_46EC:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FCh, 013h, 080h, 003h, 0A0h, 007h, 042h, 001h, 000h, 008h, 020h, 007h, 000h, 000h
        db  0E0h, 004h, 000h, 001h, 08Bh, 007h, 000h, 03Dh, 060h, 001h, 042h
loc_1_4709:
        add    dword ptr [eax - 74ecfe00h], eax
        pop    es
        add    byte ptr [14ac80bh], ch
        add    byte ptr [edx], al
        add    byte ptr [eax - 37fff660h], ah
        add    ax, sp
        add    ah, byte ptr [eax + 407a000h]
        add    dword ptr [eax], esp
        add    byte ptr [eax + 14001h], ah
        db  0FEh, 020h, 048h
loc_1_4732:
        sub    ah, al
        inc    edx
        add    eax, esp
        add    al, 2
        add    eax, esp
        add    al, 60h
loc_1_473D:
        or     eax, esp
        add    al, 82h
        add    dword ptr [eax + 1], esp
        sbb    al, 1
        add    al, 0
        add    edx, dword ptr [esi]
        loopne loc_1_474D
        inc    eax
loc_1_474D:
        add    dword ptr [eax], eax
        or     al, 0a0h
        push   es
        scasb
        mov    cl, 0a0h
        pop    es
        add    al, 1
        sub    eax, 0c22000h
        db  0E0h, 088h
        add    ah, byte ptr [esi]
        loop   loc_1_4777
        push   ss
        mov    al, byte ptr [2e010407h]
        add    byte ptr [eax + 1], ah
        inc    edx
        add    dword ptr [eax], eax
        add    ecx, dword ptr [407a016h]
        add    dword ptr [ecx], esp
loc_1_4777:
        add    byte ptr [eax - 764ffff9h], cl
        pop    es
        db  0FEh, 0FFh, 0A8h, 009h, 0A9h, 009h, 08Ah, 007h, 002h, 0E0h, 0A0h, 006h, 012h, 0F4h, 056h, 010h
        db  088h, 007h, 000h, 0A0h, 089h, 007h, 0FEh, 0AFh, 0A8h, 009h, 0A9h, 009h, 08Ah, 007h, 07Ah, 0E0h
        db  0A0h, 006h, 012h, 0F4h, 04Bh, 010h, 0A0h, 005h, 004h, 001h, 088h, 007h, 008h, 000h, 089h, 007h
        db  07Ah, 000h, 000h, 003h, 001h, 000h, 0A0h, 006h, 04Ah, 0F4h, 040h, 010h, 0A0h, 006h, 0AEh, 0B1h
        db  0E0h, 002h, 0F4h, 003h, 088h, 007h, 0A0h, 000h, 089h, 007h, 0FEh, 000h, 0A0h, 006h, 04Ah, 0F4h
        db  035h, 010h, 0E0h, 002h, 0A0h, 000h, 0E0h, 004h, 07Eh, 001h, 0C8h, 004h, 009h, 002h, 0F2h, 003h
        db  048h, 062h, 0E0h, 0C1h, 040h, 001h, 0A0h, 001h, 040h, 001h, 000h, 004h, 018h, 0CEh, 009h, 006h
        db  0FDh, 016h, 0A0h, 001h, 040h, 001h, 000h, 040h, 007h, 0C8h, 040h, 001h, 088h, 007h, 0F4h, 003h
        db  089h, 007h, 0FEh, 03Fh, 0A0h, 006h, 04Ah, 0F4h, 019h, 010h, 0E0h, 002h, 0A0h, 000h, 0A0h, 006h
        db  072h, 0F5h, 014h, 010h, 008h, 0C8h, 044h, 004h, 009h, 0C8h, 046h, 004h, 0A0h, 006h, 0A0h, 0F4h
        db  00Dh, 010h, 081h, 007h, 07Eh, 0E0h, 0B1h, 0C0h, 026h, 013h, 001h, 0C8h, 0E0h, 000h, 0A0h, 005h
        db  004h, 001h, 092h, 006h, 003h, 010h, 060h, 0C0h, 0E0h, 000h, 0F5h, 010h, 0E0h, 001h, 004h, 001h
        db  010h, 000h, 0B0h, 003h, 0FFh, 010h, 0A0h, 001h, 004h, 001h, 000h, 080h, 080h, 003h, 080h, 007h
        db  0A0h, 000h, 0C2h, 004h, 080h, 0CCh, 081h, 007h, 09Ch, 0B1h, 082h, 002h, 01Eh, 000h, 002h, 016h
        db  081h, 007h, 0A6h, 0B1h, 081h, 0C4h, 081h, 08Ch, 0E9h, 016h, 082h, 002h, 07Ch, 000h, 0F2h, 016h
        db  000h, 003h, 00Fh, 000h, 05Bh, 004h, 081h, 007h, 00Ah, 0E1h, 082h, 007h, 004h, 000h, 0E0h, 004h
        db  080h, 001h, 0E0h, 004h, 082h, 001h, 091h, 0C4h, 0B1h, 08Ch, 0D8h, 016h, 082h, 002h, 07Ch, 000h
        db  0FAh, 016h, 020h, 0C8h, 004h, 0E0h, 082h, 001h, 020h, 0E8h, 00Ch, 0E0h, 082h, 001h, 020h, 0C8h
        db  010h, 0E0h, 080h, 001h, 081h, 007h, 088h, 0E0h, 0B1h, 0C0h, 007h, 013h, 0B1h, 0C4h, 0FCh, 010h
        db  0A0h, 007h, 004h, 001h, 02Eh, 000h, 060h, 004h, 09Ch, 0B1h, 081h, 007h, 034h, 0E0h, 082h, 007h
        db  0FCh, 005h, 083h, 007h, 00Ah, 000h, 0B1h, 0CCh, 043h, 006h, 0FDh, 016h, 002h, 002h, 000h, 006h
        db  060h, 0D0h, 04Eh, 001h, 0EDh, 013h, 021h, 002h, 000h, 0F7h, 021h, 002h, 000h, 0C0h, 081h, 0DCh
        db  060h, 0D0h, 04Fh, 001h, 0C1h, 0C0h, 041h, 009h, 021h, 002h, 000h, 0F0h, 081h, 0DCh, 043h, 002h
        db  000h, 00Fh, 023h, 002h, 000h, 0F0h, 083h, 0DCh, 001h, 002h, 032h, 00Ch, 0A0h, 0C0h, 044h, 004h
        db  0E0h, 0C0h, 046h, 004h, 003h, 0C1h, 002h, 061h, 084h, 005h, 004h, 0C8h, 048h, 004h, 003h, 0C1h
        db  084h, 005h, 004h, 0A1h, 001h, 0A1h, 004h, 0C8h, 030h, 00Ch, 003h, 0C1h, 084h, 005h, 0F1h, 004h
        db  004h, 006h, 0FDh, 016h, 008h, 002h, 000h, 0B0h, 0A8h, 009h, 060h, 0C2h, 030h, 00Ch, 029h, 002h
        db  0FFh, 003h, 0A9h, 009h, 029h, 002h, 040h, 000h, 080h, 007h, 000h, 0A0h, 0A0h, 009h, 08Ah, 007h
        db  0FEh, 0AFh, 02Ah, 002h, 0FFh, 003h, 0AAh, 009h, 001h, 002h, 032h, 00Ch, 005h, 002h, 000h, 000h
        db  003h, 0C1h, 084h, 005h, 011h, 007h, 0C1h, 005h, 085h, 005h, 004h, 006h, 00Bh, 013h, 085h, 080h
        db  0F9h, 01Ah, 005h, 080h, 0F8h, 01Ah, 085h, 082h, 0F5h, 01Ah, 005h, 082h, 0F4h, 01Ah, 045h, 082h
        db  0F1h, 01Ah, 0F1h, 010h, 020h, 02Dh, 002h, 000h, 060h, 001h, 040h, 001h, 000h, 040h, 006h, 016h
        db  08Ah, 007h, 000h, 008h, 0A0h, 001h, 040h, 001h, 000h, 040h, 048h, 010h, 060h, 001h, 042h, 001h
        db  000h, 080h, 006h, 016h, 08Ah, 007h, 000h, 010h, 0A0h, 001h, 042h, 001h, 000h, 080h, 03Eh, 010h
        db  060h, 001h, 002h, 001h, 000h, 010h, 00Ah, 016h, 060h, 001h, 000h, 001h, 000h, 004h, 006h, 016h
        db  08Ah, 007h, 000h, 080h, 0A0h, 001h, 002h, 001h, 000h, 010h, 030h, 010h, 060h, 001h, 002h, 001h
        db  000h, 008h, 00Ah, 016h, 060h, 001h, 000h, 001h, 000h, 004h, 006h, 016h, 0A0h, 001h, 002h, 001h
        db  000h, 008h, 00Dh, 002h, 001h, 000h, 00Dh, 010h, 060h, 001h, 002h, 001h, 000h, 004h, 016h, 016h
        db  060h, 001h, 000h, 001h, 000h, 008h, 012h, 016h, 0A0h, 001h, 002h, 001h, 000h, 004h, 00Dh, 002h
        db  002h, 000h, 0A0h, 0C3h, 00Eh, 001h, 0E0h, 0C3h, 010h, 001h, 08Ah, 007h, 000h, 020h, 060h, 001h
        db  000h, 001h, 000h, 080h, 00Bh, 013h, 08Ah, 007h, 000h, 040h, 008h, 010h, 08Ah, 007h, 004h, 000h
        db  005h, 010h, 08Ah, 007h, 002h, 000h, 002h, 010h, 08Ah, 007h, 008h, 000h, 000h, 003h, 000h, 000h
        db  0E0h, 004h, 082h, 001h, 08Bh, 007h, 0E0h, 005h, 0CAh, 0CEh, 0CDh, 0CEh, 0CEh, 0CEh, 0CFh, 0C6h
        db  020h, 0C3h, 058h, 007h, 020h, 023h, 004h, 0E0h, 012h, 013h, 08Bh, 007h, 018h, 0FFh, 08Ah, 002h
        db  000h, 080h, 00Ah, 013h, 08Bh, 005h, 0CDh, 0A2h, 08Ah, 002h, 000h, 040h, 005h, 013h, 08Ah, 002h
        db  000h, 020h, 002h, 013h, 08Bh, 007h, 01Dh, 0FFh, 00Bh, 0C8h, 004h, 001h, 00Dh, 010h, 020h, 0D3h
        db  005h, 001h, 0FDh, 011h, 020h, 0D8h, 0DFh, 007h, 017h, 001h, 08Bh, 007h, 080h, 0FFh, 00Bh, 0C8h
        db  004h, 001h, 020h, 0E8h, 00Ah, 0E0h, 000h, 001h, 0E0h, 0C2h, 004h, 001h, 0E0h, 022h, 088h, 0E1h
        db  0FBh, 016h, 0E0h, 002h, 0A0h, 000h, 0E0h, 004h, 082h, 001h, 020h, 0E8h, 00Ah, 0E0h, 000h, 001h
        db  0E0h, 0C2h, 000h, 001h, 0E0h, 022h, 006h, 0E0h, 0F8h, 013h, 0A0h, 001h, 040h, 001h, 000h, 0F6h
        db  060h, 004h, 082h, 0B0h, 000h, 003h, 002h, 000h, 0A0h, 007h, 002h, 001h, 0FFh, 0DFh, 090h, 003h
        db  0FFh, 0FFh, 080h, 003h, 000h, 003h, 002h, 000h, 009h, 007h, 0A0h, 0C2h, 004h, 001h, 08Ah, 001h
        db  080h, 000h, 04Ah, 052h, 089h, 0D2h, 00Ah, 0C8h, 004h, 001h, 0A0h, 0D2h, 004h, 001h, 0F9h, 016h
        db  049h, 005h, 089h, 001h, 000h, 080h, 049h, 001h, 000h, 040h, 00Eh, 013h, 009h, 0F8h, 03Ah, 007h
        db  060h, 0C2h, 036h, 007h, 003h, 016h, 090h, 003h, 0FFh, 0FFh, 080h, 003h, 0E0h, 004h, 036h, 007h
        db  054h, 004h, 090h, 003h, 0FFh, 0FFh, 080h, 003h, 060h, 022h, 088h, 0E1h, 0C2h, 013h, 0E0h, 004h
        db  082h, 001h, 060h, 004h, 0D2h, 0B3h, 000h, 003h, 002h, 000h, 0C9h, 004h, 0A0h, 0C1h, 034h, 006h
        db  004h, 016h, 0A0h, 006h, 094h, 0C2h, 0E0h, 004h, 020h, 009h, 086h, 007h, 0E8h, 005h, 0A0h, 001h
        db  040h, 001h, 000h, 080h, 020h, 0C2h, 084h, 001h, 020h, 048h, 008h, 0E0h, 084h, 001h, 020h, 022h
        db  008h, 0E0h, 008h, 013h, 060h, 001h, 0AEh, 001h, 001h, 000h, 004h, 016h, 0E0h, 001h, 034h, 006h
        db  000h, 080h, 006h, 010h, 020h, 0C2h, 032h, 009h, 006h, 013h, 0E0h, 001h, 034h, 006h, 000h, 008h
        db  0E0h, 004h, 030h, 006h, 009h, 007h, 0A0h, 005h, 0EEh, 005h, 020h, 006h, 0ECh, 005h, 002h, 016h
        db  016h, 0C2h, 003h, 016h, 049h, 0C2h, 012h, 016h, 080h, 003h, 098h, 0C5h, 0E8h, 0C1h, 002h, 000h
        db  0E0h, 0E9h, 014h, 0E0h, 004h, 000h, 0D7h, 004h, 027h, 002h, 008h, 000h, 0A0h, 006h, 02Ah, 0C2h
        db  016h, 0C2h, 004h, 013h, 028h, 0C8h, 008h, 000h, 0ECh, 005h, 0EFh, 013h, 054h, 004h, 000h, 003h
        db  002h, 000h, 0E0h, 0C1h, 086h, 001h, 047h, 002h, 00Eh, 000h, 0A7h, 0C2h, 092h, 0E1h, 05Ah, 004h
        db  08Ah, 007h, 000h, 0A0h, 00Ah, 0C8h, 086h, 001h, 0C7h, 0A1h, 027h, 002h, 09Ah, 0E1h, 037h, 0E8h
        db  034h, 006h, 017h, 0E8h, 0D2h, 006h, 0E0h, 004h, 030h, 006h, 060h, 004h, 0C6h, 0B9h, 00Ah, 0E8h
        db  0D2h, 006h, 0E0h, 001h, 034h, 006h, 000h, 008h, 0E0h, 004h, 030h, 006h, 020h, 0E0h, 018h, 0E0h
        db  05Bh, 004h, 0A0h, 005h, 020h, 009h, 020h, 088h, 020h, 009h, 016h, 0E0h, 0E5h, 01Ah, 0E0h, 004h
        db  020h, 009h, 0A0h, 006h, 0E8h, 0E0h, 080h, 003h, 0A0h, 005h, 032h, 009h, 080h, 003h, 001h, 0C3h
        db  0FBh, 013h, 060h, 001h, 066h, 009h, 001h, 000h, 078h, 013h, 0A0h, 005h, 032h, 009h, 075h, 010h
        db  041h, 0C0h, 006h, 013h, 001h, 0C8h, 06Ch, 001h, 0E0h, 0C2h, 002h, 0FCh, 001h, 011h, 07Bh, 010h
        db  060h, 001h, 09Ch, 001h, 040h, 000h, 079h, 016h, 020h, 0D8h, 02Fh, 009h, 083h, 001h, 041h, 0C0h
        db  004h, 013h, 001h, 0C8h, 08Ah, 001h, 001h, 0C8h, 018h, 009h, 086h, 007h, 043h, 000h, 006h, 0C8h
        db  06Ch, 001h, 007h, 002h, 002h, 0FCh, 017h, 0C2h, 060h, 004h, 0E6h, 0B6h, 0E0h, 004h, 018h, 009h
        db  0C7h, 061h, 008h, 007h, 060h, 001h, 006h, 0FCh, 040h, 000h, 002h, 013h, 008h, 002h, 001h, 000h
        db  009h, 010h, 04Ch, 0C2h, 020h, 0C3h, 000h, 0FCh, 02Ah, 013h, 00Ch, 0C8h, 06Ch, 001h, 0E0h, 0C2h
        db  002h, 0FCh, 01Bh, 011h, 04Bh, 001h, 000h, 001h, 0F4h, 016h, 0C8h, 022h, 012h, 013h, 0CBh, 001h
        db  000h, 040h, 00Bh, 0C8h, 002h, 0FCh, 00Dh, 010h, 0E0h, 0C1h, 018h, 009h, 001h, 0C3h, 021h, 013h
        db  04Ch, 0C2h, 015h, 013h, 00Ch, 0C8h, 06Ch, 001h, 0E0h, 0C2h, 002h, 0FCh, 006h, 011h, 0CCh, 081h
        db  0D5h, 013h, 04Ch, 0C2h, 020h, 0C3h, 000h, 0FCh, 0F4h, 010h, 009h, 0C8h, 06Ch, 001h, 0E0h, 0C2h
        db  002h, 0FCh, 01Eh, 016h, 0A0h, 007h, 002h, 0FCh, 000h, 080h, 009h, 0C3h, 019h, 010h, 009h, 0C8h
        db  06Ch, 001h, 0E0h, 0C2h, 002h, 0FCh, 005h, 016h, 0A0h, 007h, 002h, 0FCh, 000h, 080h, 009h, 0C3h
        db  00Fh, 010h, 0E0h, 0C2h, 002h, 00Ch, 001h, 011h, 01Eh, 010h, 020h, 0D8h, 002h, 0E2h, 083h, 001h
        db  08Bh, 009h, 08Bh, 009h, 08Bh, 009h, 08Bh, 009h, 0A0h, 007h, 08Ah, 001h, 043h, 000h, 013h, 010h
        db  00Ch, 0C8h, 08Ah, 001h, 00Ch, 0C8h, 018h, 009h, 00Eh, 010h, 000h, 003h, 002h, 000h, 0E0h, 0C0h
        db  06Ch, 001h, 020h, 0C3h, 08Ah, 001h, 020h, 098h, 083h, 001h, 002h, 0E2h, 081h, 013h, 060h, 001h
        db  09Ch, 001h, 040h, 000h, 0B9h, 013h, 001h, 083h, 031h, 016h, 003h, 0C8h, 06Ch, 001h, 040h, 001h
        db  010h, 000h, 014h, 016h, 0E0h, 0C2h, 02Eh, 006h, 011h, 013h, 0E0h, 0C2h, 0F8h, 005h, 00Eh, 013h
        db  0E0h, 001h, 03Ah, 007h, 000h, 080h, 080h, 001h, 010h, 000h, 0E0h, 0C2h, 036h, 007h, 006h, 013h
        db  0E0h, 004h, 036h, 007h, 080h, 001h, 020h, 000h, 060h, 004h, 0C6h, 0B9h, 040h, 001h, 020h, 000h
        db  0F9h, 013h, 090h, 003h, 0FFh, 011h, 080h, 003h, 008h, 001h, 000h, 004h, 019h, 016h, 060h, 001h
        db  066h, 009h, 001h, 000h, 015h, 016h, 088h, 001h, 000h, 01Ah, 0C8h, 001h, 000h, 001h, 0C8h, 0C5h
        db  00Fh, 010h, 0E0h, 004h, 018h, 009h, 0C0h, 001h, 004h, 000h, 015h, 010h, 081h, 0C1h, 001h, 0C8h
        db  06Ch, 001h, 007h, 002h, 000h, 0FCh, 077h, 0C0h, 017h, 0C2h, 048h, 001h, 000h, 018h, 0E4h, 013h
        db  040h, 001h, 040h, 000h, 015h, 016h, 080h, 001h, 045h, 000h, 046h, 0C1h, 020h, 0D0h, 007h, 0FCh
        db  060h, 081h, 018h, 009h, 0E6h, 013h, 0E0h, 0C2h, 008h, 0FCh, 008h, 011h, 0E0h, 0C2h, 00Eh, 0FCh
        db  007h, 015h, 006h, 013h, 0E0h, 0C2h, 014h, 0FCh, 003h, 015h, 002h, 013h, 0C0h, 001h, 001h, 000h
        db  048h, 001h, 000h, 001h, 011h, 013h, 040h, 001h, 080h, 040h, 062h, 013h, 060h, 004h, 052h, 0B6h
        db  048h, 001h, 001h, 000h, 003h, 016h, 040h, 001h, 000h, 040h, 00Bh, 016h, 0C8h, 001h, 000h, 040h
        db  0A0h, 005h, 032h, 009h, 0C8h, 0C5h, 005h, 010h, 0C0h, 001h, 040h, 000h, 040h, 001h, 004h, 000h
        db  0EFh, 013h, 0B7h, 001h, 020h, 000h, 0D7h, 0C2h, 0C4h, 062h, 00Bh, 005h, 02Bh, 002h, 0FCh, 0FFh
        db  0CBh, 0C5h, 002h, 015h, 046h, 081h, 063h, 013h, 008h, 001h, 000h, 05Eh, 060h, 016h, 008h, 001h
        db  088h, 000h, 013h, 016h, 086h, 002h, 043h, 000h, 01Eh, 016h, 040h, 001h, 000h, 040h, 00Bh, 013h
        db  008h, 001h, 003h, 000h, 008h, 013h, 084h, 0C2h, 02Ah, 002h, 0D8h, 0FFh, 006h, 0C8h, 06Ch, 001h
        db  00Ah, 068h, 004h, 0FCh, 06Ch, 010h, 060h, 004h, 0B0h, 0B8h, 040h, 001h, 001h, 000h, 0EAh, 013h
        db  008h, 001h, 002h, 000h, 0E7h, 016h, 048h, 001h, 001h, 000h, 0E4h, 016h, 08Ah, 007h, 080h, 000h
        db  0A0h, 006h, 01Eh, 0B5h, 0DFh, 010h, 000h, 0C0h, 0EEh, 011h, 060h, 0C2h, 066h, 009h, 040h, 001h
        db  000h, 040h, 00Ah, 013h, 048h, 001h, 001h, 000h, 034h, 013h, 048h, 001h, 002h, 000h, 00Ah, 013h
        db  049h, 001h, 004h, 000h, 0E0h, 016h, 006h, 010h, 049h, 001h, 002h, 000h, 003h, 013h, 008h, 001h
        db  003h, 000h, 06Eh, 013h, 049h, 001h, 001h, 000h, 012h, 013h, 040h, 001h, 080h, 040h, 001h, 016h
        db  046h, 0C1h, 0E0h, 004h, 000h, 0FCh, 087h, 007h, 0F8h, 005h, 017h, 0C2h, 014h, 013h, 0C7h, 005h
        db  017h, 0C8h, 06Ch, 001h, 005h, 0C8h, 000h, 0FCh, 0C6h, 0C5h, 060h, 004h, 052h, 0B6h, 007h, 002h
        db  002h, 0FCh, 0E0h, 0A1h, 02Ch, 009h, 0E0h, 0CDh, 0EEh, 005h, 0E0h, 0C5h, 004h, 0FCh, 020h, 0C8h
        db  02Ch, 009h, 004h, 0FCh, 0E2h, 010h, 0C5h, 0CDh, 0C6h, 0C5h, 060h, 004h, 052h, 0B6h, 060h, 004h
        db  094h, 0B8h, 006h, 0C8h, 06Ch, 001h, 085h, 081h, 01Ah, 013h, 0E0h, 0C2h, 004h, 0FCh, 017h, 015h
        db  086h, 0C2h, 08Ah, 0A2h, 0AAh, 0C1h, 032h, 00Ch, 006h, 0C8h, 06Ch, 001h, 00Bh, 0A8h, 004h, 0FCh
        db  01Ah, 009h, 00Ah, 0C8h, 06Ch, 001h, 0E0h, 0C2h, 002h, 0FCh, 0E0h, 004h, 000h, 0FCh, 006h, 0C8h
        db  06Ch, 001h, 00Bh, 0C8h, 002h, 0FCh, 0A0h, 006h, 096h, 0C1h, 006h, 0C8h, 06Ch, 001h, 0E0h, 004h
        db  000h, 0FCh, 0A0h, 001h, 002h, 0FCh, 002h, 000h, 087h, 007h, 030h, 006h, 0E7h, 001h, 004h, 000h
        db  040h, 000h, 0D7h, 004h, 027h, 002h, 00Ch, 000h, 005h, 0C2h, 060h, 001h, 066h, 009h, 004h, 000h
        db  003h, 016h, 0E0h, 001h, 002h, 0FCh, 020h, 000h, 0A0h, 006h, 040h, 0C2h, 0C0h, 001h, 020h, 000h
        db  060h, 004h, 052h, 0B6h, 048h, 001h, 000h, 018h, 003h, 013h, 048h, 001h, 000h, 010h, 002h, 016h
        db  0A0h, 005h, 032h, 009h, 086h, 002h, 043h, 000h, 003h, 013h, 040h, 001h, 080h, 040h, 098h, 013h
        db  006h, 0C8h, 06Ch, 001h, 0E0h, 004h, 000h, 0FCh, 085h, 0C2h, 0A0h, 006h, 096h, 0C1h, 0E8h, 010h
        db  0A0h, 0C2h, 0F6h, 005h, 056h, 016h, 019h, 0C8h, 0F0h, 005h, 0A9h, 0C2h, 00Ah, 000h, 00Dh, 011h
        db  0A0h, 0F2h, 02Eh, 009h, 00Ah, 0D8h, 080h, 001h, 029h, 0C8h, 006h, 000h, 08Ch, 001h, 0A0h, 007h
        db  016h, 009h, 004h, 000h, 009h, 0C8h, 0F4h, 005h, 046h, 010h, 029h, 0C8h, 006h, 000h, 06Ch, 001h
        db  020h, 0C8h, 00Eh, 0FCh, 0BCh, 001h, 020h, 0C8h, 010h, 0FCh, 0B0h, 001h, 020h, 0C8h, 012h, 0FCh
        db  0B2h, 001h, 0A0h, 0F2h, 02Eh, 009h, 08Ah, 001h, 000h, 010h, 0A0h, 001h, 080h, 001h, 000h, 0C4h
        db  0E1h, 010h, 047h, 001h, 008h, 000h, 006h, 016h, 0A8h, 0C2h, 006h, 000h, 0A0h, 006h, 096h, 0C1h
        db  0E8h, 004h, 006h, 000h, 007h, 001h, 020h, 000h, 031h, 013h, 0E8h, 004h, 002h, 000h, 03Bh, 010h
        db  0E0h, 004h, 000h, 0FCh, 0A0h, 006h, 096h, 0C1h, 029h, 010h, 000h, 003h, 002h, 000h, 020h, 0C2h
        db  08Ch, 001h, 0E0h, 0C0h, 06Ch, 001h, 020h, 0C2h, 0F4h, 005h, 028h, 0C8h, 008h, 000h, 06Ch, 001h
        db  0E8h, 0C1h, 00Ah, 000h, 020h, 0C3h, 002h, 0FCh, 08Ch, 001h, 020h, 000h, 00Ch, 0C8h, 002h, 0FCh
        db  00Ch, 001h, 000h, 0FEh, 03Bh, 016h, 047h, 001h, 040h, 000h, 050h, 013h, 060h, 0C2h, 0F0h, 005h
        db  0A7h, 016h, 0E0h, 004h, 0F4h, 005h, 00Ch, 0CAh, 008h, 000h, 047h, 001h, 080h, 000h, 0C9h, 016h
        db  028h, 0C8h, 006h, 000h, 06Ch, 001h, 0A0h, 0C2h, 000h, 0FCh, 0D2h, 016h, 0E8h, 0C1h, 002h, 000h
        db  0D7h, 0C2h, 00Fh, 016h, 027h, 002h, 010h, 000h, 0D8h, 004h, 057h, 0C2h, 00Eh, 013h, 0C7h, 005h
        db  057h, 0C2h, 048h, 0C6h, 0C8h, 0C5h, 003h, 0C8h, 06Ch, 001h, 00Dh, 011h, 090h, 003h, 0FFh, 011h
        db  080h, 003h, 0D7h, 004h, 0C3h, 001h, 000h, 080h, 0EDh, 010h, 0E7h, 001h, 0F4h, 0FFh, 020h, 000h
        db  0C8h, 0CDh, 0C8h, 0C5h, 0F0h, 010h, 090h, 003h, 0F8h, 011h, 0E0h, 002h, 0C0h, 000h, 060h, 0C3h
        db  0FAh, 000h, 0A0h, 0C3h, 0FCh, 000h, 0E0h, 0C3h, 0FEh, 000h, 054h, 004h, 0E8h, 0C2h, 008h, 000h
        db  0A8h, 0C2h, 006h, 000h, 00Ch, 0C3h, 033h, 011h, 020h, 023h, 00Ah, 0E0h, 045h, 013h, 020h, 023h
        db  010h, 0E0h, 046h, 013h, 020h, 023h, 00Eh, 0E0h, 013h, 013h, 0E0h, 021h, 016h, 0E0h, 0B6h, 016h
        db  020h, 023h, 006h, 0E0h, 003h, 016h, 020h, 027h, 06Ah, 0E4h, 00Ah, 013h, 0E8h, 0C2h, 008h, 000h
        db  0A8h, 0C2h, 006h, 000h, 04Ch, 001h, 088h, 000h, 0A9h, 016h, 00Ch, 001h, 044h, 000h, 0A6h, 016h
        db  020h, 006h, 016h, 009h, 0A3h, 013h, 00Ah, 0C8h, 06Ch, 001h, 020h, 0C8h, 004h, 0E0h, 002h, 0FCh
        db  00Bh, 0C8h, 06Ch, 001h, 0A0h, 007h, 002h, 0FCh, 000h, 081h, 020h, 0C3h, 080h, 001h, 0A0h, 001h
        db  080h, 001h, 000h, 0C4h, 00Ch, 0C8h, 080h, 001h, 00Ah, 0C8h, 08Ch, 001h, 0ACh, 010h, 00Ah, 0C2h
        db  00Fh, 013h, 008h, 0C8h, 06Ch, 001h, 0A0h, 0C2h, 000h, 0FCh, 020h, 0C3h, 002h, 0FCh, 020h, 023h
        db  012h, 0E0h, 0F5h, 016h, 00Bh, 0C8h, 06Ch, 001h, 00Ch, 0C8h, 002h, 0FCh, 060h, 004h, 046h, 0B9h
        db  08Ah, 007h, 000h, 004h, 060h, 004h, 07Ch, 0B3h, 08Ah, 007h, 020h, 000h, 060h, 004h, 07Ch, 0B3h
        db  08Ah, 007h, 000h, 002h, 020h, 027h, 00Eh, 0E0h, 002h, 016h, 0A0h, 006h, 01Eh, 0B5h, 0A8h, 0C2h
        db  006h, 000h, 060h, 004h, 06Ch, 0B9h, 000h, 003h, 002h, 000h, 0C0h, 001h, 010h, 000h, 0E0h, 0C2h
        db  02Eh, 006h, 008h, 013h, 0E0h, 0C2h, 0F8h, 005h, 005h, 013h, 0E0h, 001h, 03Ah, 007h, 000h, 080h
        db  080h, 001h, 010h, 000h, 090h, 003h, 0FFh, 07Fh, 080h, 003h, 000h, 003h, 002h, 000h, 020h, 0C2h
        db  0F6h, 005h, 020h, 0E2h, 0F4h, 005h, 00Eh, 016h, 020h, 0D8h, 02Eh, 009h, 080h, 001h, 02Bh, 0C8h
        db  006h, 000h, 08Ch, 001h, 0A0h, 007h, 016h, 009h, 004h, 000h, 00Bh, 0C8h, 0F4h, 005h, 090h, 003h
        db  0FFh, 0FFh, 080h, 003h, 087h, 007h, 0F0h, 005h, 0DBh, 004h, 057h, 0C2h, 005h, 016h, 0CBh, 0CDh
        db  0CBh, 0C5h, 090h, 003h, 0FFh, 0FFh, 080h, 003h, 0C7h, 005h, 057h, 0C2h, 04Bh, 0C6h, 0CBh, 0C5h
        db  090h, 003h, 0FFh, 0FFh, 080h, 003h, 000h, 003h, 002h, 000h, 00Bh, 0C2h, 020h, 0C3h, 0F4h, 005h
        db  00Fh, 013h, 0A8h, 0C2h, 00Ah, 000h, 04Ah, 001h, 010h, 000h, 016h, 016h, 0A0h, 022h, 004h, 0E0h
        db  01Ah, 016h, 008h, 0C3h, 0A0h, 006h, 004h, 0BCh, 00Ch, 0C2h, 020h, 0C3h, 0F4h, 005h, 013h, 016h
        db  068h, 001h, 00Ah, 000h, 010h, 000h, 003h, 013h, 0E0h, 0C2h, 0F6h, 005h, 005h, 016h, 0A0h, 006h
        db  046h, 0BCh, 090h, 003h, 0FFh, 0FFh, 080h, 003h, 087h, 007h, 0F0h, 005h, 0A0h, 006h, 02Ah, 0C2h
        db  090h, 003h, 0FFh, 0FFh, 080h, 003h, 087h, 007h, 0F0h, 005h, 0A0h, 006h, 070h, 0C2h, 080h, 003h
        db  000h, 003h, 002h, 000h, 087h, 007h, 0F0h, 005h, 0CBh, 0C2h, 008h, 016h, 0A0h, 006h, 004h, 0BCh
        db  020h, 007h, 0F6h, 005h, 060h, 0CBh, 0F4h, 005h, 002h, 000h, 080h, 003h, 0E0h, 004h, 0F6h, 005h
        db  020h, 0C2h, 0F4h, 005h, 005h, 016h, 017h, 0C2h, 003h, 013h, 0D8h, 0C5h, 0A0h, 006h, 046h, 0BCh
        db  080h, 003h, 000h, 003h, 002h, 000h, 08Bh, 0C2h, 0A0h, 006h, 004h, 0BCh, 0CAh, 0C1h, 027h, 002h
        db  010h, 000h, 088h, 007h, 0F0h, 005h, 088h, 0C1h, 018h, 0C2h, 026h, 013h, 0A8h, 082h, 002h, 000h
        db  0FAh, 016h, 0E8h, 0C2h, 00Ah, 000h, 0E0h, 022h, 01Eh, 0E0h, 0F5h, 016h, 098h, 0C5h, 0E0h, 022h
        db  01Ch, 0E0h, 00Bh, 016h, 028h, 0C8h, 006h, 000h, 0F4h, 000h, 0E0h, 002h, 0E0h, 000h, 0A0h, 006h
        db  096h, 0C1h, 0E0h, 002h, 0C0h, 000h, 0E8h, 004h, 006h, 000h, 0E0h, 022h, 018h, 0E0h, 0E4h, 013h
        db  020h, 0EAh, 022h, 0E0h, 00Ah, 000h, 0A0h, 0EAh, 018h, 0E0h, 004h, 000h, 0DAh, 004h, 0A0h, 006h
        db  02Ah, 0C2h, 047h, 006h, 006h, 0C2h, 0D8h, 010h, 006h, 0C8h, 0F2h, 005h, 060h, 0CBh, 0F4h, 005h
        db  002h, 000h, 054h, 004h, 020h, 0C2h, 0F4h, 005h, 013h, 013h, 0E0h, 001h, 09Ch, 001h, 000h, 040h
        db  08Bh, 00Bh, 08Bh, 00Bh, 060h, 001h, 09Ch, 001h, 000h, 040h, 00Ah, 016h, 060h, 0C2h, 06Ch, 001h
        db  028h, 0C8h, 006h, 000h, 06Ch, 001h, 0A0h, 0C2h, 002h, 0FCh, 003h, 011h, 009h, 0C8h, 06Ch, 001h
        db  05Bh, 004h, 009h, 0C8h, 06Ch, 001h, 04Bh, 0C2h, 087h, 007h, 0F0h, 005h, 0A0h, 006h, 070h, 0C2h
        db  0E0h, 004h, 0F4h, 005h, 059h, 004h, 0A8h, 0C2h, 00Ah, 000h, 00Dh, 011h, 0A0h, 0F2h, 02Eh, 009h
        db  00Ah, 0D8h, 080h, 001h, 028h, 0C8h, 006h, 000h, 08Ch, 001h, 0A0h, 007h, 016h, 009h, 004h, 000h
        db  008h, 0C8h, 0F4h, 005h, 05Bh, 004h, 020h, 0C3h, 06Ch, 001h, 028h, 0C8h, 006h, 000h, 06Ch, 001h
        db  020h, 0C8h, 00Eh, 0FCh, 0BCh, 001h, 020h, 0C8h, 010h, 0FCh, 0B0h, 001h, 020h, 0C8h, 012h, 0FCh
        db  0B2h, 001h, 00Ch, 0C8h, 06Ch, 001h, 0A0h, 0F2h, 02Eh, 009h, 08Ah, 001h, 000h, 010h, 0A0h, 001h
        db  080h, 001h, 000h, 0C4h, 0DDh, 010h, 033h, 007h, 033h, 007h, 00Ch, 010h, 013h, 007h, 023h, 007h
        db  002h, 000h, 0CBh, 0C8h, 006h, 000h, 023h, 002h, 018h, 000h, 0E0h, 0CCh, 06Ch, 001h, 0CDh, 0CCh
        db  0CEh, 0CCh, 0CFh, 0CCh, 083h, 007h, 030h, 006h, 0D3h, 0C1h, 00Ah, 013h, 083h, 007h, 036h, 007h
        db  0D3h, 0C1h, 006h, 013h, 083h, 007h, 0A0h, 000h, 093h, 000h, 00Ch, 0C8h, 06Ch, 001h, 080h, 003h
        db  063h, 007h, 002h, 000h, 02Ah, 015h, 063h, 0C2h, 004h, 000h, 063h, 042h, 006h, 000h, 0DBh, 013h
        db  063h, 0C3h, 01Ah, 000h, 049h, 0D2h, 00Ch, 013h, 0C9h, 006h, 049h, 072h, 069h, 0D2h, 0C2h, 0E1h
        db  0C9h, 006h, 049h, 072h, 0E9h, 048h, 004h, 0E0h, 004h, 000h, 049h, 0CBh, 002h, 000h, 052h, 004h
        db  069h, 0C2h, 0C2h, 0E1h, 049h, 072h, 029h, 002h, 010h, 000h, 0C3h, 0C2h, 0E9h, 0A2h, 0AAh, 0E1h
        db  01Bh, 0C3h, 089h, 002h, 012h, 000h, 00Fh, 013h, 0DCh, 0C6h, 003h, 016h, 0E9h, 048h, 004h, 0E0h
        db  004h, 000h, 049h, 0CBh, 002h, 000h, 04Ch, 0CBh, 004h, 000h, 090h, 003h, 0FFh, 001h, 093h, 000h
        db  00Ch, 0C8h, 06Ch, 001h, 080h, 003h, 00Ch, 0C8h, 06Ch, 001h, 0E0h, 0C6h, 000h, 0FCh, 0F1h, 016h
        db  0E9h, 048h, 004h, 0E0h, 004h, 000h, 049h, 0CBh, 002h, 000h, 04Ch, 0CBh, 004h, 000h, 0EDh, 010h
        db  000h, 003h, 002h, 000h, 0DBh, 0C2h, 063h, 0C2h, 004h, 000h, 04Bh, 042h, 09Fh, 013h, 049h, 0D2h
        db  00Eh, 013h, 0C9h, 006h, 049h, 072h, 069h, 0D2h, 0C2h, 0E1h, 0C9h, 006h, 049h, 072h, 0E9h, 048h
        db  004h, 0E0h, 004h, 000h, 049h, 0CBh, 002h, 000h, 090h, 003h, 0FFh, 0FFh, 080h, 003h, 069h, 0C2h
        db  0C2h, 0E1h, 049h, 072h, 029h, 002h, 010h, 000h, 0C3h, 0C2h, 0E9h, 0A2h, 0AAh, 0E1h, 01Bh, 0C3h
        db  089h, 002h, 012h, 000h, 00Ch, 013h, 0DCh, 0C6h, 003h, 016h, 0E9h, 048h, 004h, 0E0h, 004h, 000h
        db  049h, 0CBh, 002h, 000h, 04Ch, 0CBh, 004h, 000h, 090h, 003h, 0FFh, 0FFh, 080h, 003h, 00Ch, 0C8h
        db  06Ch, 001h, 0E0h, 0C6h, 000h, 0FCh, 0F4h, 016h, 0F0h, 010h, 000h, 003h, 002h, 000h, 0BBh, 0C2h
        db  0BBh, 0C1h, 086h, 0D1h, 003h, 013h, 086h, 0EAh, 004h, 000h, 013h, 010h, 0A6h, 0D1h, 0C2h, 0E1h
        db  0C6h, 006h, 086h, 071h, 0CAh, 0C1h, 0E6h, 0A1h, 0BAh, 0E1h, 0A6h, 0EAh, 014h, 0E0h, 004h, 000h
        db  01Bh, 0C2h, 086h, 002h, 002h, 000h, 003h, 016h, 0A0h, 006h, 050h, 0C2h, 002h, 010h, 0A0h, 006h
        db  02Ah, 0C2h, 0DAh, 004h, 080h, 003h, 000h, 003h, 002h, 000h, 0ABh, 0C2h, 006h, 000h, 08Ch, 007h
        db  0E8h, 005h, 05Ch, 0C2h, 016h, 013h, 0A0h, 0C1h, 0ECh, 005h, 08Ah, 081h, 01Ah, 01Ah, 0C6h, 0C1h
        db  009h, 0C2h, 059h, 0C2h, 020h, 013h, 0E9h, 0A1h, 008h, 000h, 087h, 082h, 0F9h, 012h, 0A9h, 0A2h
        db  008h, 000h, 087h, 062h, 0CAh, 0CAh, 008h, 000h, 04Ah, 06Ah, 008h, 000h, 0C9h, 0C6h, 00Bh, 0C6h
        db  080h, 003h, 0CAh, 0CAh, 008h, 000h, 00Ah, 0C8h, 0ECh, 005h, 0DBh, 004h, 00Bh, 0CFh, 00Bh, 0C7h
        db  080h, 003h, 08Ah, 061h, 046h, 0CAh, 008h, 000h, 0CAh, 0CAh, 008h, 000h, 00Ah, 0C8h, 0ECh, 005h
        db  0C9h, 0C6h, 00Bh, 0C7h, 080h, 003h, 087h, 062h, 0CAh, 0CAh, 008h, 000h, 0DBh, 004h, 00Bh, 0C6h
        db  00Bh, 0CBh, 002h, 000h, 080h, 003h, 000h, 003h, 002h, 000h, 0BBh, 0C1h, 0DBh, 0C2h, 08Ch, 007h
        db  0E8h, 005h, 04Ch, 0C2h, 0EDh, 004h, 002h, 000h, 009h, 0C2h, 059h, 0C2h, 018h, 013h, 0A9h, 081h
        db  002h, 000h, 0FAh, 016h, 0E9h, 082h, 004h, 000h, 0F7h, 016h, 049h, 0CBh, 004h, 000h, 099h, 0C2h
        db  00Ah, 0C6h, 00Ah, 013h, 008h, 083h, 004h, 013h, 0A9h, 0AAh, 008h, 000h, 008h, 000h, 080h, 003h
        db  02Ah, 0A8h, 008h, 000h, 0ECh, 005h, 080h, 003h, 008h, 0CBh, 002h, 000h, 080h, 003h, 02Dh, 007h
        db  002h, 000h, 08Ch, 007h, 008h, 000h, 006h, 0A3h, 04Ch, 0C2h, 009h, 0C2h, 059h, 0C2h, 013h, 013h
        db  0E9h, 082h, 004h, 000h, 0FAh, 016h, 0ADh, 007h, 002h, 000h, 001h, 000h, 049h, 0CBh, 004h, 000h
        db  019h, 0C6h, 001h, 013h, 080h, 003h, 008h, 083h, 004h, 016h, 0A0h, 049h, 014h, 0E0h, 004h, 000h
        db  080h, 003h, 008h, 0CBh, 002h, 000h, 080h, 003h, 000h, 003h, 002h, 000h, 00Bh, 006h, 015h, 011h
        db  043h, 013h, 08Bh, 007h, 000h, 04Eh, 060h, 001h, 042h, 001h, 080h, 000h, 002h, 013h, 08Bh, 007h
        db  000h, 03Ah, 00Bh, 0C8h, 044h, 001h, 0E0h, 001h, 040h, 001h, 000h, 002h, 0E0h, 001h, 040h, 001h
        db  000h, 020h, 084h, 007h, 0A6h, 0BCh, 060h, 004h, 0B4h, 0BCh, 020h, 0C8h, 016h, 0E0h, 0E0h, 000h
        db  0E0h, 0C2h, 066h, 009h, 0E0h, 022h, 010h, 0E0h, 003h, 013h, 020h, 0E8h, 014h, 0E0h, 0E0h, 000h
        db  020h, 0C8h, 004h, 0E0h, 082h, 001h, 020h, 0C8h, 0E2h, 000h, 08Ah, 001h, 0E0h, 004h, 018h, 009h
        db  0E0h, 004h, 0F4h, 005h, 0E0h, 004h, 0F8h, 005h, 0E0h, 004h, 0F0h, 005h, 0E0h, 004h, 042h, 007h
        db  0A0h, 007h, 088h, 001h, 020h, 000h, 0E0h, 0C2h, 030h, 009h, 009h, 013h, 0A0h, 007h, 088h, 001h
        db  080h, 000h, 020h, 0E8h, 016h, 0E0h, 080h, 001h, 0E0h, 001h, 082h, 001h, 000h, 003h, 08Bh, 007h
        db  000h, 0A0h, 00Bh, 0E8h, 086h, 001h, 080h, 003h, 0E0h, 004h, 086h, 001h, 0E0h, 001h, 09Ch, 001h
        db  040h, 000h, 0E0h, 001h, 09Ch, 001h, 000h, 040h, 0CBh, 004h, 0B0h, 003h, 00Bh, 006h, 004h, 013h
        db  060h, 001h, 09Ch, 001h, 000h, 040h, 0F9h, 016h, 0E0h, 004h, 080h, 001h, 020h, 0E8h, 008h, 0E0h
        db  066h, 009h, 08Bh, 007h, 000h, 080h, 00Bh, 0C8h, 098h, 007h, 00Bh, 0C8h, 078h, 007h, 08Bh, 007h
        db  06Fh, 087h, 00Bh, 048h, 03Ah, 007h, 0E0h, 0C2h, 050h, 007h, 08Bh, 002h, 058h, 007h, 010h, 013h
        db  020h, 0E8h, 00Ah, 0E0h, 000h, 001h, 0E0h, 0C2h, 000h, 001h, 0E0h, 022h, 006h, 0E0h, 0F8h, 013h
        db  08Bh, 007h, 058h, 007h, 00Bh, 0C8h, 050h, 007h, 08Bh, 007h, 050h, 0C5h, 00Bh, 0C8h, 052h, 007h
        db  080h, 003h, 000h, 003h, 002h, 000h, 0E0h, 0C2h, 01Ah, 009h, 00Ch, 013h, 020h, 006h, 01Ch, 009h
        db  00Bh, 0C8h, 06Ch, 001h, 020h, 0C8h, 000h, 0FCh, 01Ah, 009h, 04Bh, 0CBh, 002h, 000h, 090h, 003h
        db  0FFh, 0FFh, 080h, 003h, 041h, 0C0h, 00Fh, 013h, 081h, 080h, 00Dh, 013h, 082h, 0A0h, 0E2h, 0C2h
        db  032h, 00Ch, 012h, 009h, 00Bh, 0C8h, 06Ch, 001h, 0E0h, 004h, 000h, 0FCh, 020h, 0C3h, 002h, 0FCh
        db  007h, 011h, 002h, 0C8h, 000h, 0FCh, 0EDh, 004h, 002h, 000h, 0E0h, 004h, 06Ch, 001h, 080h, 003h
        db  042h, 0CBh, 002h, 000h, 002h, 0C8h, 06Ch, 001h, 08Bh, 0C0h, 080h, 003h, 000h, 003h, 002h, 000h
        db  083h, 007h, 000h, 080h, 060h, 0C2h, 07Ah, 009h, 009h, 0C1h, 024h, 002h, 0F8h, 0FFh, 0A9h, 008h
        db  001h, 002h, 000h, 000h, 002h, 002h, 000h, 000h, 00Bh, 002h, 000h, 000h, 00Ch, 002h, 000h, 000h
        db  007h, 002h, 000h, 000h, 02Ch, 0CBh, 032h, 00Ch, 032h, 00Ch, 006h, 013h, 08Bh, 005h, 0CCh, 005h
        db  00Bh, 088h, 046h, 004h, 027h, 01Bh, 0F6h, 010h, 009h, 0C2h, 08Bh, 0C2h, 008h, 006h, 00Ah, 013h
        db  08Bh, 005h, 0CCh, 005h, 00Bh, 088h, 046h, 004h, 01Dh, 01Bh, 02Ch, 0CBh, 032h, 00Ch, 032h, 00Ch
        db  0EDh, 016h, 0F4h, 010h, 082h, 0C0h, 014h, 013h, 002h, 0C8h, 06Ch, 001h, 00Ah, 0C8h, 000h, 0FCh
        db  00Ah, 0C8h, 06Ch, 001h, 0E0h, 004h, 000h, 0FCh, 0A0h, 007h, 002h, 0FCh, 000h, 080h, 004h, 0C8h
        db  004h, 0FCh, 00Ah, 0C2h, 008h, 0A2h, 002h, 0CAh, 032h, 00Ch, 08Ah, 0C0h, 087h, 005h, 0D6h, 010h
        db  04Ah, 0C0h, 0EEh, 010h, 047h, 0CBh, 002h, 000h, 0E0h, 004h, 06Ch, 001h, 08Bh, 007h, 043h, 000h
        db  0E0h, 004h, 000h, 00Ch, 000h, 003h, 002h, 000h, 00Bh, 0C8h, 06Ch, 001h, 08Bh, 002h, 043h, 000h
        db  004h, 013h, 060h, 001h, 002h, 0FCh, 020h, 000h, 006h, 013h, 08Bh, 0C2h, 0A0h, 006h, 09Ah, 0C1h
        db  090h, 003h, 07Fh, 000h, 080h, 003h, 0A0h, 001h, 002h, 0FCh, 020h, 000h, 060h, 001h, 066h, 009h
        db  001h, 000h, 00Bh, 016h, 00Ah, 002h, 002h, 0FCh, 0A0h, 0A2h, 02Ch, 009h, 0A0h, 0CEh, 0EEh, 005h
        db  0A0h, 0C6h, 004h, 0FCh, 020h, 0C8h, 02Ch, 009h, 004h, 0FCh, 08Ah, 007h, 0F8h, 005h, 05Ah, 0C2h
        db  008h, 013h, 0CAh, 005h, 05Ah, 0C2h, 009h, 0C8h, 06Ch, 001h, 00Bh, 0C8h, 000h, 0FCh, 08Bh, 0C6h
        db  002h, 010h, 08Bh, 0CEh, 08Bh, 0C6h, 020h, 020h, 01Ah, 0E0h, 005h, 016h, 020h, 0E8h, 004h, 0E0h
        db  03Ah, 007h, 0E0h, 004h, 036h, 007h, 090h, 003h, 07Fh, 000h, 080h, 003h, 000h, 003h, 002h, 000h
        db  00Bh, 0C8h, 06Ch, 001h, 0CCh, 004h, 0E0h, 004h, 000h, 0FCh, 08Bh, 0C2h, 0A0h, 006h, 0A8h, 0C1h
        db  090h, 003h, 07Fh, 000h, 080h, 003h, 0A0h, 007h, 002h, 0FCh, 000h, 080h, 020h, 0C8h, 08Eh, 0E1h
        db  004h, 0FCh, 041h, 0C0h, 00Fh, 016h, 020h, 0D8h, 002h, 0E2h, 083h, 001h, 08Bh, 00Bh, 08Bh, 00Bh
        db  08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh
        db  00Ah, 0C8h, 08Ah, 001h, 05Bh, 004h, 00Ah, 0C8h, 06Ch, 001h, 020h, 0C3h, 000h, 0FCh, 0E0h, 004h
        db  000h, 0FCh, 08Ah, 002h, 043h, 000h, 0DFh, 013h, 0A0h, 007h, 002h, 0FCh, 000h, 080h, 004h, 0C8h
        db  004h, 0FCh, 020h, 098h, 080h, 009h, 01Dh, 009h, 00Ah, 013h, 020h, 0C8h, 01Ah, 009h, 000h, 0FCh
        db  00Ah, 0C8h, 01Ah, 009h, 0A0h, 005h, 01Ch, 009h, 08Ch, 0C2h, 0E5h, 016h, 05Bh, 004h, 041h, 0C0h
        db  010h, 013h, 08Ah, 0A2h, 082h, 0CAh, 032h, 00Ch, 01Ah, 009h, 002h, 0C8h, 06Ch, 001h, 00Ah, 0C8h
        db  000h, 0FCh, 08Ah, 0C0h, 020h, 098h, 083h, 001h, 002h, 0E2h, 009h, 013h, 08Ch, 0C2h, 0D3h, 016h
        db  05Bh, 004h, 04Ah, 0C0h, 08Ah, 0C0h, 020h, 098h, 083h, 001h, 002h, 0E2h, 011h, 016h, 0E0h, 001h
        db  09Ch, 001h, 040h, 000h, 060h, 001h, 09Ch, 001h, 040h, 000h, 0FCh, 016h, 060h, 001h, 002h, 00Ch
        db  000h, 001h, 0ECh, 013h, 020h, 0D8h, 02Fh, 009h, 083h, 001h, 0A0h, 007h, 002h, 00Ch, 000h, 080h
        db  00Ah, 0C8h, 08Ah, 001h, 00Ah, 0C8h, 018h, 009h, 0E1h, 010h, 0D8h, 004h, 057h, 0C2h, 003h, 016h
        db  0C8h, 0CDh, 0C8h, 0C5h, 05Bh, 004h, 0C7h, 005h, 057h, 0C2h, 048h, 0C6h, 0C8h, 0C5h, 05Bh, 004h
        db  008h, 0C8h, 06Ch, 001h, 008h, 0A2h, 020h, 0CAh, 000h, 0FCh, 032h, 00Ch, 018h, 009h, 002h, 010h
        db  008h, 0C8h, 06Ch, 001h, 0E0h, 004h, 000h, 0FCh, 057h, 0C2h, 003h, 016h, 0C8h, 0CDh, 0C8h, 0C5h
        db  05Bh, 004h, 0C7h, 005h, 017h, 0C8h, 06Ch, 001h, 008h, 0C8h, 000h, 0FCh, 0C8h, 0C5h, 05Bh, 004h
        db  017h, 0C6h, 002h, 016h, 0C8h, 0C9h, 002h, 000h, 0C8h, 0C5h, 05Bh, 004h, 017h, 0C2h, 008h, 0C8h
        db  06Ch, 001h, 007h, 013h, 0E0h, 0C5h, 000h, 0FCh, 008h, 0A2h, 028h, 0C8h, 032h, 00Ch, 000h, 0FCh
        db  018h, 009h, 05Bh, 004h, 060h, 001h, 082h, 001h, 000h, 020h, 00Ah, 016h, 060h, 0C2h, 084h, 001h
        db  0A0h, 001h, 082h, 001h, 000h, 020h, 0E0h, 001h, 082h, 001h, 000h, 020h, 009h, 0C8h, 084h, 001h
        db  0C9h, 004h, 05Bh, 004h, 0A0h, 006h, 002h, 0C5h, 0D3h, 004h, 0E0h, 004h, 002h, 001h, 020h, 0E8h
        db  014h, 0E0h, 000h, 001h, 020h, 0C8h, 016h, 0E0h, 004h, 001h, 005h, 02Ch, 020h, 048h, 014h, 0E0h
        db  000h, 001h, 08Ch, 007h, 000h, 00Ah, 08Dh, 007h, 0D8h, 007h, 08Eh, 007h, 018h, 000h, 07Ch, 0CFh
        db  04Eh, 006h, 0FDh, 016h, 0E0h, 002h, 0D8h, 007h, 08Fh, 007h, 011h, 0FFh, 08Bh, 002h, 03Bh, 059h
        db  021h, 016h, 08Ah, 002h, 03Bh, 059h, 01Eh, 013h, 08Fh, 005h, 020h, 020h, 016h, 0E0h, 001h, 016h
        db  019h, 010h, 020h, 020h, 004h, 0E0h, 016h, 016h, 000h, 001h, 0BFh, 000h, 013h, 016h, 08Bh, 007h
        db  0C0h, 040h, 000h, 001h, 000h, 060h, 010h, 013h, 040h, 001h, 000h, 060h, 00Bh, 016h, 08Bh, 007h
        db  0C4h, 044h, 0A0h, 0C3h, 002h, 001h, 00Eh, 048h, 002h, 001h, 04Eh, 001h, 000h, 010h, 004h, 016h
        db  08Fh, 007h, 018h, 0FFh, 060h, 004h, 0D8h, 0C4h, 00Bh, 0C3h, 04Bh, 0C3h, 020h, 020h, 00Ah, 0E0h
        db  002h, 016h, 06Bh, 002h, 020h, 020h, 020h, 020h, 00Ch, 0E0h, 002h, 016h, 06Ch, 002h, 000h, 020h
        db  020h, 020h, 00Eh, 0E0h, 002h, 016h, 06Ch, 002h, 020h, 000h, 08Fh, 005h, 020h, 020h, 010h, 0E0h
        db  007h, 016h, 06Dh, 002h, 020h, 000h, 020h, 021h, 022h, 0E0h, 0E4h, 013h, 004h, 0C1h, 002h, 016h
        db  084h, 007h, 0FEh, 07Fh, 08Fh, 005h, 020h, 020h, 012h, 0E0h, 002h, 016h, 06Dh, 002h, 000h, 020h
        db  060h, 021h, 022h, 0E0h, 0D7h, 013h, 045h, 0C1h, 002h, 016h, 085h, 007h, 0FEh, 07Fh, 08Fh, 005h
        db  086h, 0D1h, 00Bh, 013h, 0A0h, 025h, 026h, 0E0h, 008h, 013h, 08Fh, 005h, 020h, 026h, 022h, 0E0h
        db  004h, 016h, 08Fh, 005h, 0A0h, 026h, 022h, 0E0h, 002h, 013h, 060h, 004h, 0D8h, 0C4h, 001h, 0D8h
        db  0ECh, 008h, 020h, 0D8h, 0DBh, 007h, 000h, 009h, 002h, 0D8h, 0F6h, 008h, 020h, 0D8h, 0DDh, 007h
        db  0E2h, 008h, 0E0h, 002h, 058h, 007h, 020h, 0D8h, 0EFh, 007h, 0F4h, 007h, 020h, 0D8h, 0F1h, 007h
        db  0F6h, 007h, 020h, 0D8h, 0F3h, 007h, 0F8h, 007h, 009h, 002h, 006h, 000h, 0CBh, 004h, 00Fh, 002h
        db  0EEh, 007h, 08Fh, 005h, 0CBh, 0DFh, 009h, 006h, 0FCh, 016h, 0A0h, 006h, 002h, 0C5h, 089h, 007h
        db  05Eh, 0E3h, 0E0h, 004h, 01Ah, 001h, 020h, 0C8h, 0E4h, 007h, 018h, 001h, 019h, 0C8h, 00Ch, 001h
        db  039h, 0C8h, 00Ah, 001h, 039h, 0C8h, 012h, 001h, 009h, 016h, 079h, 0C3h, 00Fh, 002h, 000h, 0E0h
        db  04Fh, 063h, 02Dh, 002h, 000h, 0A0h, 00Dh, 0C8h, 014h, 001h, 002h, 010h, 039h, 0C8h, 014h, 001h
        db  0F9h, 0C3h, 03Fh, 0C8h, 00Eh, 001h, 01Fh, 0C8h, 010h, 001h, 0E0h, 004h, 014h, 009h, 0B9h, 0C2h
        db  01Ah, 0C8h, 000h, 001h, 096h, 006h, 089h, 002h, 086h, 0E3h, 0E0h, 016h, 08Fh, 007h, 01Ch, 0FFh
        db  08Ch, 007h, 000h, 00Ah, 08Dh, 007h, 086h, 0E3h, 08Eh, 007h, 010h, 000h, 07Ch, 08Fh, 044h, 016h
        db  04Eh, 006h, 0FCh, 016h, 0A0h, 0C3h, 0E2h, 007h, 0E0h, 0C3h, 0E0h, 007h, 0CEh, 083h, 001h, 014h
        db  0CEh, 0C3h, 00Fh, 0C8h, 01Ah, 001h, 08Ch, 007h, 096h, 0E3h, 08Dh, 007h, 000h, 00Ah, 08Eh, 007h
        db  0A6h, 0E3h, 08Ch, 063h, 07Ch, 0CFh, 04Eh, 006h, 0FDh, 016h, 0E0h, 004h, 030h, 009h, 020h, 001h
        db  042h, 001h, 000h, 004h, 002h, 016h, 020h, 007h, 030h, 009h, 060h, 0C2h, 062h, 001h, 0E0h, 004h
        db  062h, 001h, 08Eh, 007h, 000h, 080h, 08Ch, 007h, 034h, 009h, 08Dh, 007h, 006h, 000h, 03Eh, 0DFh
        db  08Eh, 005h, 00Dh, 006h, 0FCh, 016h, 0FEh, 0D3h, 0CFh, 006h, 08Eh, 005h, 0FEh, 0D3h, 0CFh, 006h
        db  08Ch, 007h, 034h, 009h, 009h, 0C8h, 062h, 001h, 0C9h, 004h, 05Ch, 0A3h, 07Ch, 0E2h, 05Ch, 0A3h
        db  07Ch, 0E2h, 05Ch, 0A3h, 07Ch, 0E2h, 002h, 013h, 0CDh, 083h, 009h, 013h, 020h, 007h, 034h, 009h
        db  006h, 010h, 08Fh, 007h, 019h, 0FFh, 0CDh, 0A3h, 00Fh, 0C8h, 004h, 001h, 0FFh, 010h, 0A0h, 001h
        db  002h, 001h, 000h, 010h, 0E0h, 0C3h, 0EEh, 007h, 0E0h, 043h, 006h, 0E0h, 00Fh, 0C8h, 000h, 001h
        db  020h, 0C0h, 004h, 0E0h, 0E0h, 004h, 0FEh, 006h, 0D3h, 004h, 0E0h, 004h, 004h, 001h, 060h, 004h
        db  050h, 0C5h, 08Ch, 007h, 000h, 00Ah, 08Dh, 007h, 018h, 000h, 08Eh, 007h, 03Bh, 059h, 00Eh, 0CFh
        db  04Dh, 006h, 0FDh, 016h, 05Bh, 004h, 093h, 001h, 000h, 080h, 020h, 004h, 0C2h, 0E2h, 060h, 0D0h
        db  098h, 007h, 01Ch, 013h, 000h, 003h, 002h, 000h, 0A0h, 0C0h, 046h, 007h, 012h, 0C8h, 046h, 007h
        db  002h, 016h, 093h, 001h, 020h, 000h, 000h, 003h, 00Fh, 000h, 020h, 004h, 0EAh, 0E2h, 093h, 001h
        db  000h, 020h, 080h, 001h, 000h, 040h, 000h, 001h, 0FEh, 000h, 049h, 016h, 0C4h, 0C3h, 025h, 016h
        db  0D3h, 0C3h, 0C5h, 043h, 00Ch, 016h, 0E0h, 0C3h, 098h, 007h, 003h, 011h, 0E0h, 002h, 098h, 007h
        db  051h, 004h, 0E0h, 0C3h, 078h, 007h, 00Ah, 011h, 0E0h, 002h, 078h, 007h, 051h, 004h, 0D3h, 011h
        db  04Fh, 001h, 000h, 020h, 0E4h, 013h, 04Fh, 001h, 020h, 000h, 0D1h, 013h, 005h, 02Ch, 041h, 0A0h
        db  021h, 004h, 0C2h, 0E2h, 08Bh, 007h, 050h, 0C5h, 000h, 001h, 000h, 040h, 00Fh, 013h, 0DDh, 0C3h
        db  04Fh, 002h, 00Fh, 000h, 02Fh, 0E1h, 014h, 0E0h, 05Bh, 004h, 0E4h, 0C3h, 0C2h, 0E1h, 0CFh, 073h
        db  02Fh, 041h, 014h, 0E0h, 06Fh, 0C3h, 08Eh, 0EAh, 08Bh, 007h, 050h, 0C5h, 04Bh, 0C2h, 0A0h, 0C2h
        db  0F4h, 007h, 08Ch, 007h, 008h, 000h, 0BDh, 0C0h, 0A0h, 0C3h, 0EAh, 007h, 0E0h, 0C3h, 0ECh, 007h
        db  0A0h, 006h, 044h, 0C7h, 0C0h, 001h, 000h, 040h, 002h, 0D8h, 017h, 001h, 062h, 002h, 080h, 0FFh
        db  0A0h, 006h, 098h, 0C7h, 002h, 0C8h, 004h, 001h, 090h, 003h, 03Fh, 060h, 059h, 004h, 0C0h, 0C3h
        db  0CFh, 073h, 0EFh, 0C3h, 0C2h, 0E1h, 0CFh, 073h, 0AFh, 0C3h, 080h, 0EAh, 09Eh, 0C3h, 04Eh, 002h
        db  00Fh, 000h, 02Eh, 021h, 014h, 0E0h, 008h, 013h, 02Fh, 040h, 014h, 0E0h, 0CFh, 0A3h, 02Fh, 004h
        db  0F2h, 0E2h, 040h, 001h, 000h, 040h, 0A4h, 013h, 0C4h, 0C3h, 0C7h, 016h, 000h, 001h, 0FEh, 000h
        db  0E6h, 016h, 09Eh, 010h, 040h, 001h, 000h, 040h, 005h, 016h, 020h, 0E0h, 014h, 0E0h, 065h, 002h
        db  000h, 058h, 096h, 010h, 020h, 0D8h, 0DEh, 007h, 017h, 001h, 08Fh, 007h, 086h, 0FFh, 00Fh, 0C8h
        db  004h, 001h, 0C0h, 001h, 000h, 040h, 045h, 002h, 0FFh, 0A7h, 08Ah, 010h, 020h, 0C3h, 0FEh, 006h
        db  020h, 027h, 03Ah, 0E3h, 007h, 013h, 020h, 023h, 022h, 0E0h, 01Ah, 013h, 065h, 002h, 0FFh, 0DFh
        db  020h, 040h, 014h, 0E0h, 020h, 0E0h, 016h, 0E0h, 00Ch, 0C8h, 0E6h, 008h, 08Dh, 007h, 0E2h, 008h
        db  058h, 004h, 020h, 048h, 008h, 0E0h, 0FEh, 006h, 020h, 0C3h, 0E6h, 008h, 020h, 027h, 03Ah, 0E3h
        db  019h, 016h, 080h, 003h, 002h, 0C3h, 06Ch, 0C2h, 00Ah, 000h, 099h, 006h, 060h, 004h, 050h, 0C5h
        db  0A0h, 0C2h, 0F4h, 007h, 08Ch, 007h, 001h, 000h, 08Dh, 007h, 006h, 006h, 0CEh, 004h, 0E0h, 0C3h
        db  008h, 006h, 001h, 013h, 097h, 006h, 020h, 0D8h, 007h, 006h, 017h, 001h, 08Bh, 007h, 082h, 0FFh
        db  00Bh, 0C8h, 004h, 001h, 0A0h, 006h, 0CAh, 0CBh, 060h, 004h, 050h, 0C5h, 0A0h, 0C2h, 0EEh, 007h
        db  08Ch, 007h, 006h, 000h, 08Dh, 007h, 0EEh, 008h, 0A0h, 0C3h, 0E6h, 007h, 0E0h, 0C3h, 0E8h, 007h
        db  097h, 006h, 0A0h, 0C2h, 0F4h, 007h, 08Dh, 007h, 0F4h, 008h, 0DDh, 004h, 08Ch, 007h, 002h, 000h
        db  097h, 006h, 08Dh, 007h, 000h, 080h, 0A0h, 0C2h, 0EEh, 008h, 00Ah, 088h, 00Ch, 006h, 014h, 01Bh
        db  082h, 007h, 014h, 0C7h, 0A0h, 0C3h, 0F0h, 008h, 0E0h, 0C3h, 0F2h, 008h, 08Bh, 007h, 00Eh, 0E3h
        db  08Ah, 002h, 014h, 000h, 004h, 01Ah, 08Bh, 007h, 07Ch, 0EAh, 02Ah, 002h, 0ECh, 0FFh, 08Ah, 0A2h
        db  0CAh, 0A2h, 0DBh, 0C2h, 001h, 013h, 09Bh, 006h, 020h, 0C8h, 0EEh, 008h, 0F2h, 008h, 020h, 0C8h
        db  020h, 0E0h, 0EEh, 008h, 00Dh, 0C8h, 0F0h, 008h, 08Dh, 007h, 0ECh, 008h, 020h, 0E0h, 018h, 0E0h
        db  065h, 002h, 000h, 058h, 058h, 004h, 045h, 002h, 0FFh, 0A7h, 080h, 003h, 060h, 0C0h, 0EEh, 005h
        db  021h, 002h, 0E8h, 003h, 020h, 001h, 002h, 001h, 006h, 000h, 007h, 016h, 001h, 088h, 0EEh, 005h
        db  0F9h, 016h, 039h, 010h, 060h, 0D0h, 003h, 001h, 0F1h, 013h, 001h, 002h, 00Ah, 001h, 04Ch, 0CCh
        db  04Ch, 0CCh, 04Eh, 0CCh, 04Fh, 0CCh, 0B1h, 007h, 040h, 000h, 04Dh, 0CCh, 00Ah, 0C8h, 000h, 001h
        db  05Bh, 004h, 060h, 0C0h, 0EEh, 005h, 021h, 002h, 0E8h, 003h, 020h, 001h, 002h, 001h, 006h, 000h
        db  007h, 016h, 001h, 088h, 0EEh, 005h, 0F9h, 016h, 01Eh, 010h, 060h, 0D0h, 003h, 001h, 0F1h, 013h
        db  001h, 002h, 00Ah, 001h, 04Ch, 0CCh, 04Ch, 0CCh, 04Eh, 0CCh, 04Fh, 0CCh, 0B1h, 007h, 040h, 000h
        db  04Dh, 0CCh, 00Ah, 0C8h, 000h, 001h, 0A0h, 003h, 060h, 0D0h, 003h, 001h, 001h, 013h, 05Bh, 004h
        db  060h, 0C0h, 0EEh, 005h, 021h, 002h, 0E8h, 003h, 020h, 001h, 002h, 001h, 006h, 000h, 0F7h, 016h
        db  001h, 088h, 0EEh, 005h, 0F9h, 016h, 0CDh, 004h, 08Ah, 007h, 000h, 040h, 020h, 0C3h, 000h, 001h
        db  00Ch, 001h, 000h, 080h, 002h, 013h, 08Ah, 007h, 000h, 020h, 0A0h, 0C3h, 00Eh, 001h, 0E0h, 0C3h
        db  010h, 001h, 0B0h, 003h, 020h, 0C3h, 058h, 007h, 020h, 023h, 004h, 0E0h, 002h, 013h, 060h, 004h
        db  0D2h, 0C4h, 060h, 004h, 07Ch, 0B3h, 08Dh, 007h, 000h, 020h, 020h, 020h, 00Ah, 0E0h, 001h, 016h
        db  05Bh, 004h, 00Dh, 002h, 032h, 00Ch, 05Dh, 0C2h, 001h, 011h, 0DDh, 004h, 0CDh, 005h, 00Dh, 088h
        db  030h, 00Ch, 0F9h, 016h, 060h, 0C2h, 00Ah, 006h, 08Dh, 007h, 066h, 009h, 0A0h, 006h, 00Ah, 0CCh
        db  009h, 002h, 068h, 000h, 0E0h, 0C3h, 030h, 009h, 003h, 016h, 0E0h, 001h, 066h, 009h, 010h, 000h
        db  0E0h, 0C2h, 066h, 009h, 00Fh, 002h, 000h, 001h, 0C9h, 026h, 002h, 013h, 060h, 004h, 09Ch, 0CAh
        db  009h, 002h, 000h, 012h, 04Bh, 001h, 010h, 000h, 002h, 013h, 009h, 002h, 000h, 013h, 009h, 0D8h
        db  02Eh, 009h, 08Fh, 007h, 000h, 040h, 089h, 007h, 068h, 009h, 0CBh, 004h, 0F9h, 0E2h, 0F9h, 0E2h
        db  0F9h, 0E2h, 007h, 016h, 08Bh, 007h, 034h, 009h, 08Ch, 007h, 068h, 009h, 03Bh, 0CFh, 03Bh, 0CFh
        db  01Bh, 0C7h, 020h, 0C3h, 068h, 009h, 019h, 011h, 08Fh, 007h, 000h, 020h, 089h, 007h, 076h, 009h
        db  0A0h, 006h, 07Eh, 0C8h, 0A0h, 006h, 07Eh, 0C8h, 012h, 010h, 04Ch, 0CEh, 05Bh, 004h, 019h, 0C3h
        db  002h, 016h, 08Ch, 007h, 01Ah, 000h, 04Ch, 0C3h, 02Dh, 002h, 0F8h, 0FFh, 00Ah, 002h, 009h, 000h
        db  02Dh, 002h, 0FAh, 0FFh, 0F2h, 013h, 00Ah, 006h, 0FBh, 016h, 060h, 004h, 09Ch, 0CAh, 08Fh, 007h
        db  000h, 010h, 0D9h, 0C2h, 0FAh, 011h, 002h, 016h, 08Bh, 007h, 000h, 004h, 04Bh, 0C3h, 08Dh, 002h
        db  020h, 000h, 002h, 014h, 00Dh, 002h, 020h, 000h, 08Dh, 002h, 000h, 004h, 002h, 012h, 00Dh, 002h
        db  000h, 004h, 02Dh, 002h, 0F8h, 0FFh, 00Dh, 0C8h, 02Ch, 009h, 02Bh, 002h, 0FFh, 003h, 08Bh, 001h
        db  0FFh, 003h, 04Bh, 0CEh, 0CBh, 004h, 0E0h, 004h, 0F8h, 005h, 0E0h, 004h, 062h, 009h, 020h, 0C8h
        db  030h, 00Ch, 07Ch, 009h, 0A0h, 007h, 07Eh, 009h, 0FEh, 0DFh, 08Dh, 007h, 0FEh, 0DFh, 0E0h, 0C3h
        db  0D8h, 007h, 0E0h, 023h, 016h, 0E0h, 024h, 016h, 0E0h, 0C3h, 030h, 00Ch, 04Fh, 063h, 0FFh, 004h
        db  0FFh, 004h, 04Dh, 006h, 0FDh, 016h, 08Dh, 007h, 0FEh, 0DFh, 020h, 004h, 064h, 0EAh, 0A0h, 0C3h
        db  064h, 0EAh, 0EEh, 0C3h, 012h, 000h, 0C1h, 016h, 06Eh, 0C3h, 018h, 000h, 0ADh, 009h, 08Ch, 007h
        db  000h, 0E0h, 0ACh, 009h, 00Dh, 063h, 00Ch, 013h, 06Eh, 0C3h, 018h, 000h, 0ADh, 009h, 02Dh, 002h
        db  040h, 000h, 01Dh, 00Ah, 02Dh, 002h, 032h, 00Ch, 0BDh, 007h, 0FFh, 07Fh, 00Ch, 006h, 0FCh, 016h
        db  020h, 0C3h, 046h, 004h, 08Ch, 002h, 080h, 000h, 013h, 01Ah, 0ACh, 002h, 00Ch, 0C8h, 09Ah, 000h
        db  0E0h, 002h, 080h, 000h, 088h, 007h, 080h, 000h, 060h, 0C2h, 046h, 004h, 0A0h, 006h, 0A0h, 0F4h
        db  002h, 010h, 09Dh, 000h, 005h, 010h, 09Dh, 000h, 08Fh, 007h, 000h, 008h, 060h, 004h, 09Ch, 0CAh
        db  04Bh, 02Dh, 081h, 0C3h, 0C9h, 005h, 08Fh, 007h, 000h, 010h, 08Eh, 002h, 002h, 000h, 0F6h, 011h
        db  08Fh, 007h, 000h, 004h, 0C9h, 005h, 0D9h, 0C2h, 0E0h, 026h, 026h, 0E0h, 002h, 016h, 02Bh, 002h
        db  006h, 000h, 04Bh, 0C6h, 04Bh, 0C3h, 0CBh, 072h, 02Eh, 002h, 0FEh, 0FFh, 08Bh, 083h, 0E6h, 01Bh
        db  0CDh, 006h, 04Dh, 073h, 0CDh, 082h, 0E2h, 01Bh, 0E0h, 004h, 01Ah, 009h, 0E0h, 004h, 01Ch, 009h
        db  04Dh, 0C3h, 002h, 013h, 060h, 066h, 012h, 0E0h, 0C9h, 005h, 0CFh, 004h, 081h, 02Dh, 001h, 0C8h
        db  06Ch, 001h, 0D4h, 013h, 00Fh, 0C8h, 000h, 0FCh, 0C1h, 0C3h, 00Dh, 006h, 0F7h, 015h, 00Dh, 002h
        db  036h, 007h, 00Eh, 002h, 098h, 008h, 00Ch, 002h, 003h, 000h, 08Dh, 0CBh, 002h, 000h, 081h, 02Dh
        db  081h, 0CBh, 006h, 000h, 0C3h, 013h, 0EEh, 004h, 00Ch, 000h, 02Eh, 002h, 018h, 000h, 00Ch, 006h
        db  0F4h, 016h, 0E0h, 004h, 096h, 008h, 01Fh, 02Eh, 0B9h, 0C3h, 0D9h, 0C3h, 089h, 007h, 012h, 000h
        db  08Dh, 007h, 03Ah, 009h, 0A0h, 006h, 00Ah, 0CCh, 060h, 0C3h, 0D8h, 007h, 060h, 023h, 016h, 0E0h
        db  009h, 016h, 020h, 0E8h, 010h, 0E0h, 066h, 009h, 020h, 0E8h, 018h, 0E0h, 098h, 007h, 020h, 0E8h
        db  012h, 0E0h, 078h, 007h, 060h, 0C3h, 066h, 009h, 060h, 023h, 01Eh, 0E0h, 003h, 016h, 020h, 048h
        db  0A6h, 0E3h, 066h, 009h, 060h, 023h, 022h, 0E0h, 006h, 013h, 060h, 027h, 0A8h, 0E3h, 003h, 013h
        db  020h, 0E8h, 010h, 0E0h, 066h, 009h, 020h, 02Dh, 000h, 000h, 08Eh, 007h, 000h, 000h, 0A0h, 006h
        db  0EAh, 0CBh, 04Eh, 005h, 00Eh, 02Ch, 0A0h, 0C0h, 004h, 008h, 0EFh, 0C3h, 006h, 000h, 01Bh, 016h
        db  0A0h, 0C3h, 06Eh, 009h, 0E0h, 0C3h, 070h, 009h, 0A0h, 006h, 0D8h, 0CAh, 0A0h, 0C3h, 072h, 009h
        db  0E0h, 0C3h, 074h, 009h, 0A0h, 006h, 0F6h, 0CAh, 020h, 0E0h, 00Ah, 0E0h, 060h, 0C3h, 0D8h, 007h
        db  060h, 023h, 016h, 0E0h, 005h, 016h, 0E0h, 004h, 02Eh, 006h, 060h, 041h, 004h, 0E0h, 04Dh, 02Eh
        db  08Dh, 007h, 000h, 080h, 052h, 004h, 0CFh, 073h, 02Fh, 002h, 000h, 002h, 04Fh, 0C3h, 052h, 004h
        db  020h, 020h, 00Ah, 0E0h, 003h, 013h, 08Dh, 007h, 000h, 010h, 05Bh, 004h, 020h, 040h, 00Ah, 0E0h
        db  040h, 002h, 0FFh, 0F0h, 08Eh, 007h, 002h, 000h, 0A0h, 006h, 0EAh, 0CBh, 04Eh, 005h, 00Eh, 02Ch
        db  0A0h, 0C0h, 004h, 008h, 0A0h, 006h, 0CAh, 0CBh, 060h, 0C3h, 0D8h, 007h, 060h, 023h, 016h, 0E0h
        db  066h, 016h, 020h, 004h, 078h, 0EAh, 063h, 010h, 06Eh, 002h, 000h, 080h, 08Dh, 007h, 000h, 0C0h
        db  00Dh, 0C8h, 0A6h, 001h, 00Eh, 0C8h, 06Eh, 009h, 00Fh, 0C8h, 070h, 009h, 00Eh, 0C8h, 0A8h, 001h
        db  00Fh, 0C8h, 0AAh, 001h, 012h, 010h, 08Fh, 001h, 001h, 000h, 08Ah, 007h, 072h, 009h, 0A0h, 0E3h
        db  04Eh, 009h, 08Eh, 0CEh, 09Ah, 001h, 0FEh, 0FFh, 0E0h, 0E3h, 050h, 009h, 08Fh, 0E6h, 08Ah, 007h
        db  0ACh, 001h, 08Eh, 0CEh, 09Ah, 001h, 0FEh, 0FFh, 08Fh, 0E6h, 020h, 020h, 00Ah, 0E0h, 03Fh, 013h
        db  08Dh, 007h, 000h, 010h, 05Bh, 004h, 020h, 020h, 00Ah, 0E0h, 003h, 013h, 00Dh, 002h, 000h, 010h
        db  05Bh, 004h, 08Eh, 0C3h, 004h, 013h, 0E0h, 001h, 050h, 009h, 000h, 001h, 006h, 010h, 0A0h, 001h
        db  050h, 009h, 000h, 001h, 0A0h, 001h, 074h, 009h, 000h, 001h, 0A0h, 0C3h, 072h, 009h, 0E0h, 0C3h
        db  074h, 009h, 0A0h, 0E3h, 04Eh, 009h, 0E0h, 0E3h, 050h, 009h, 00Eh, 0C8h, 0ACh, 001h, 00Fh, 0C8h
        db  0AEh, 001h, 00Eh, 0C8h, 072h, 009h, 00Fh, 0C8h, 074h, 009h, 019h, 010h, 06Eh, 002h, 000h, 080h
        db  00Eh, 0C8h, 0A6h, 001h, 020h, 020h, 00Ah, 0E0h, 012h, 013h, 00Dh, 002h, 000h, 010h, 05Bh, 004h
        db  08Dh, 007h, 028h, 007h, 089h, 007h, 00Eh, 000h, 0A0h, 006h, 010h, 0CCh, 08Dh, 007h, 028h, 007h
        db  0FDh, 004h, 08Dh, 002h, 036h, 007h, 0FCh, 016h, 020h, 048h, 014h, 0E0h, 0FEh, 006h, 08Dh, 007h
        db  000h, 080h, 052h, 004h, 0A0h, 0C2h, 0EEh, 007h, 08Ch, 007h, 004h, 000h, 08Dh, 007h, 0F0h, 008h
        db  097h, 006h, 07Dh, 0C2h, 05Dh, 0C3h, 060h, 043h, 022h, 0E0h, 0A0h, 006h, 010h, 0CCh, 0EFh, 010h
        db  00Eh, 0C8h, 006h, 006h, 00Fh, 0C8h, 008h, 006h, 0EAh, 010h, 0B0h, 003h, 0A0h, 001h, 060h, 007h
        db  026h, 000h, 040h, 002h, 000h, 0C0h, 0E0h, 004h, 006h, 006h, 08Ch, 007h, 010h, 040h, 0CCh, 044h
        db  0E0h, 004h, 0FEh, 006h, 085h, 007h, 040h, 080h, 05Bh, 004h, 002h, 0C8h, 004h, 008h, 08Fh, 007h
        db  0FAh, 007h, 0CEh, 0CBh, 002h, 000h, 08Eh, 007h, 036h, 007h, 0CEh, 0CBh, 004h, 000h, 08Dh, 007h
        db  030h, 006h, 08Eh, 007h, 010h, 000h, 04Dh, 02Ch, 05Bh, 004h, 0A0h, 0C2h, 0F2h, 007h, 002h, 010h
        db  0A0h, 0C2h, 0F8h, 007h, 00Bh, 0C8h, 0EAh, 008h, 009h, 0C3h, 00Ah, 013h, 0A0h, 006h, 07Ah, 0C7h
        db  0A0h, 0C2h, 000h, 001h, 0A0h, 0E2h, 006h, 0E0h, 04Ch, 0A3h, 0CCh, 0A3h, 001h, 017h, 08Eh, 005h
        db  04Ch, 062h, 0E0h, 0C2h, 0EAh, 008h, 05Bh, 004h, 08Dh, 007h, 000h, 010h, 020h, 020h, 00Ah, 0E0h
        db  001h, 013h, 05Bh, 004h, 00Dh, 002h, 068h, 000h, 0E0h, 0C3h, 030h, 009h, 002h, 016h, 0CEh, 001h
        db  010h, 000h, 08Dh, 027h, 003h, 013h, 00Dh, 002h, 000h, 001h, 052h, 004h, 000h, 003h, 002h, 000h
        db  060h, 0C3h, 066h, 009h, 04Dh, 002h, 008h, 080h, 04Eh, 002h, 0F7h, 07Fh, 08Dh, 0E3h, 0E0h, 0C3h
        db  0D8h, 007h, 0E0h, 023h, 016h, 0E0h, 004h, 013h, 08Dh, 007h, 006h, 000h, 08Dh, 027h, 002h, 013h
        db  0A0h, 0E3h, 010h, 0E0h, 00Eh, 0C8h, 066h, 009h, 00Dh, 002h, 000h, 012h, 04Eh, 001h, 010h, 000h
        db  002h, 013h, 00Dh, 002h, 000h, 013h, 00Dh, 0D8h, 02Eh, 009h, 060h, 0C3h, 080h, 001h, 04Eh, 002h
        db  001h, 000h, 04Dh, 002h, 0FEh, 0FFh, 04Eh, 0E3h, 00Dh, 0C8h, 080h, 001h, 020h, 0D8h, 042h, 0E2h
        db  02Fh, 009h, 020h, 001h, 066h, 009h, 006h, 000h, 003h, 013h, 020h, 0D8h, 0D2h, 0E1h, 02Fh, 009h
        db  020h, 098h, 083h, 001h, 002h, 0E2h, 003h, 013h, 020h, 0D8h, 02Fh, 009h, 083h, 001h, 000h, 003h
        db  00Fh, 000h, 060h, 004h, 09Eh, 0CBh, 020h, 020h, 00Ah, 0E0h, 003h, 013h, 00Dh, 002h, 000h, 010h
        db  05Bh, 004h, 009h, 002h, 008h, 000h, 00Dh, 002h, 058h, 009h, 0A0h, 006h, 00Ah, 0CCh, 0A0h, 007h
        db  002h, 002h, 000h, 000h, 00Dh, 002h, 000h, 004h, 0E0h, 0C3h, 058h, 009h, 00Fh, 001h, 000h, 07Ch
        db  001h, 013h, 052h, 004h, 08Fh, 0C3h, 04Eh, 002h, 00Fh, 000h, 0FBh, 013h, 08Eh, 002h, 00Fh, 000h
        db  0F8h, 013h, 00Dh, 002h, 000h, 040h, 04Fh, 0C2h, 049h, 009h, 049h, 002h, 03Fh, 000h, 009h, 001h
        db  001h, 000h, 0EFh, 016h, 089h, 002h, 006h, 000h, 0ECh, 01Ah, 089h, 002h, 020h, 000h, 0E9h, 014h
        db  0C9h, 006h, 01Fh, 009h, 04Fh, 002h, 000h, 040h, 04Fh, 0E2h, 069h, 002h, 000h, 080h, 009h, 0C8h
        db  058h, 009h, 00Fh, 002h, 0FFh, 0FFh, 04Eh, 0C2h, 01Fh, 009h, 009h, 006h, 0FDh, 016h, 04Fh, 005h
        db  00Dh, 002h, 000h, 020h, 060h, 0C2h, 05Ah, 009h, 0D4h, 013h, 04Fh, 026h, 0D2h, 016h, 00Dh, 002h
        db  000h, 010h, 060h, 0C2h, 05Ch, 009h, 0CDh, 013h, 04Fh, 026h, 0CBh, 016h, 00Dh, 002h, 000h, 030h
        db  020h, 088h, 05Ah, 009h, 05Ch, 009h, 0C5h, 013h, 0E0h, 0C3h, 05Ah, 009h, 04Eh, 0C2h, 01Fh, 00Ah
        db  009h, 006h, 0FDh, 016h, 0E0h, 0E3h, 05Eh, 009h, 00Fh, 0C8h, 05Ah, 009h, 0E0h, 0C3h, 05Ch, 009h
        db  04Eh, 0C2h, 01Fh, 00Ah, 009h, 006h, 0FDh, 016h, 0E0h, 0E3h, 05Eh, 009h, 00Fh, 0C8h, 05Ch, 009h
        db  00Fh, 002h, 0FFh, 0FFh, 04Eh, 0C2h, 01Fh, 00Ah, 009h, 006h, 0FDh, 016h, 00Dh, 002h, 000h, 008h
        db  060h, 0C2h, 05Eh, 009h, 04Fh, 026h, 0A5h, 016h, 04Fh, 005h, 00Fh, 0C8h, 05Eh, 009h, 00Fh, 002h
        db  002h, 002h, 00Eh, 002h, 003h, 000h, 060h, 0C3h, 040h, 001h, 00Ch, 002h, 00Ch, 0CEh, 0A0h, 001h
        db  040h, 001h, 000h, 004h, 0CFh, 005h, 009h, 002h, 055h, 055h, 09Ch, 006h, 049h, 005h, 09Ch, 006h
        db  009h, 007h, 09Ch, 006h, 049h, 005h, 09Ch, 006h, 00Eh, 006h, 0F4h, 016h, 0A0h, 001h, 040h, 001h
        db  000h, 040h, 00Dh, 0C8h, 040h, 001h, 009h, 002h, 008h, 000h, 00Eh, 002h, 058h, 009h, 00Fh, 002h
        db  002h, 002h, 0FEh, 0CFh, 049h, 006h, 0FDh, 016h, 060h, 004h, 09Eh, 0CBh, 0C9h, 0C7h, 05Fh, 082h
        db  001h, 016h, 05Bh, 004h, 0A0h, 001h, 040h, 001h, 000h, 040h, 00Dh, 0C8h, 040h, 001h, 00Dh, 002h
        db  000h, 001h, 052h, 004h, 08Dh, 007h, 000h, 010h, 020h, 020h, 00Ah, 0E0h, 00Ah, 016h, 08Dh, 007h
        db  000h, 008h, 020h, 020h, 010h, 0E0h, 005h, 013h, 08Dh, 007h, 000h, 040h, 04Fh, 001h, 001h, 000h
        db  001h, 016h, 05Bh, 004h, 020h, 0E0h, 010h, 0E0h, 020h, 007h, 09Ch, 008h, 020h, 007h, 0B4h, 008h
        db  020h, 007h, 0CCh, 008h, 0A0h, 007h, 0A2h, 008h, 084h, 002h, 0A0h, 007h, 0BAh, 008h, 084h, 002h
        db  0A0h, 007h, 0D2h, 008h, 084h, 002h, 0A0h, 007h, 004h, 009h, 000h, 040h, 0E0h, 004h, 006h, 009h
        db  0E0h, 004h, 008h, 009h, 00Eh, 0C8h, 04Ch, 008h, 00Fh, 0C8h, 04Eh, 008h, 00Eh, 0C8h, 08Eh, 008h
        db  00Fh, 0C8h, 090h, 008h, 0E0h, 004h, 05Ah, 008h, 0E0h, 004h, 060h, 008h, 0E0h, 002h, 078h, 007h
        db  0E0h, 004h, 094h, 008h, 020h, 040h, 042h, 0E3h, 020h, 0E0h, 00Ch, 0E0h, 060h, 004h, 0C6h, 0D3h
        db  080h, 001h, 000h, 0F0h, 0C0h, 001h, 000h, 040h, 010h, 010h, 080h, 001h, 000h, 0F0h, 00Dh, 010h
        db  0C0h, 001h, 000h, 0F0h, 020h, 040h, 006h, 0E0h, 008h, 010h, 0C0h, 001h, 000h, 0F0h, 080h, 001h
        db  000h, 020h, 0E0h, 0C3h, 094h, 008h, 001h, 016h, 05Bh, 004h, 04Bh, 0C0h, 020h, 004h, 07Ch, 0EAh
        db  040h, 001h, 000h, 020h, 0FBh, 016h, 051h, 004h, 0A0h, 0C2h, 0D8h, 007h, 04Ah, 001h, 040h, 000h
        db  001h, 016h, 05Bh, 004h, 0E0h, 002h, 078h, 007h, 020h, 020h, 00Ch, 0E0h, 0EFh, 016h, 043h, 0C2h
        db  002h, 013h, 0A0h, 006h, 028h, 0D0h, 020h, 02Fh, 036h, 007h, 020h, 040h, 00Ch, 0E0h, 0A0h, 006h
        db  0BAh, 0CEh, 0A0h, 0C3h, 094h, 008h, 0FBh, 016h, 0A0h, 006h, 048h, 0CFh, 08Eh, 007h, 004h, 009h
        db  09Eh, 007h, 000h, 080h, 020h, 020h, 010h, 0E0h, 005h, 016h, 08Fh, 007h, 04Ch, 008h, 0BFh, 0CFh
        db  0BFh, 0CFh, 09Fh, 0C7h, 0A0h, 006h, 068h, 0CFh, 020h, 0E8h, 03Eh, 0E3h, 062h, 007h, 0A0h, 006h
        db  048h, 0CFh, 020h, 048h, 03Eh, 0E3h, 062h, 007h, 020h, 040h, 042h, 0E3h, 020h, 0E0h, 004h, 0E0h
        db  020h, 048h, 010h, 0E0h, 058h, 007h, 05Bh, 004h, 080h, 001h, 000h, 0F0h, 020h, 0E0h, 004h, 0E0h
        db  060h, 001h, 060h, 007h, 002h, 000h, 002h, 013h, 09Bh, 006h, 0B8h, 010h, 020h, 0E8h, 01Eh, 0E0h
        db  058h, 007h, 0B4h, 010h, 09Bh, 006h, 080h, 003h, 0E0h, 002h, 058h, 007h, 000h, 001h, 000h, 040h
        db  007h, 016h, 08Dh, 007h, 000h, 009h, 0A0h, 006h, 0ACh, 0C5h, 0E0h, 002h, 078h, 007h, 05Bh, 004h
        db  0C4h, 001h, 002h, 000h, 0E0h, 002h, 078h, 007h, 05Bh, 004h, 00Eh, 068h, 096h, 008h, 0E9h, 004h
        db  00Ch, 000h, 011h, 010h, 00Eh, 002h, 000h, 023h, 04Eh, 0DBh, 001h, 000h, 0CCh, 001h, 000h, 004h
        db  04Ch, 0D7h, 01Ch, 010h, 060h, 0C2h, 05Ch, 007h, 020h, 006h, 094h, 008h, 0A9h, 0C2h, 008h, 000h
        db  0A9h, 0C3h, 00Ch, 000h, 0EAh, 016h, 029h, 007h, 004h, 000h, 069h, 001h, 00Ah, 000h, 001h, 000h
        db  02Dh, 013h, 049h, 0C3h, 02Dh, 002h, 00Eh, 000h, 00Ah, 0C3h, 01Dh, 0D3h, 08Ch, 001h, 000h, 084h
        db  0CCh, 001h, 000h, 040h, 00Ah, 001h, 000h, 05Eh, 0DDh, 016h, 04Ch, 0C7h, 0A9h, 0C3h, 010h, 000h
        db  0E9h, 0C3h, 012h, 000h, 041h, 0CAh, 010h, 000h, 02Fh, 002h, 004h, 000h, 001h, 017h, 08Eh, 005h
        db  08Ch, 007h, 002h, 000h, 0A0h, 0C2h, 0F6h, 007h, 0A0h, 006h, 044h, 0C7h, 069h, 0C0h, 010h, 000h
        db  029h, 0C8h, 014h, 000h, 006h, 009h, 029h, 0C8h, 016h, 000h, 008h, 009h, 069h, 001h, 00Eh, 000h
        db  000h, 008h, 004h, 016h, 090h, 003h, 07Fh, 000h, 0A0h, 006h, 068h, 0CFh, 040h, 001h, 000h, 040h
        db  001h, 016h, 051h, 004h, 060h, 004h, 0CCh, 0CEh, 0A9h, 0C3h, 00Ch, 000h, 00Bh, 013h, 00Eh, 068h
        db  096h, 008h, 0E9h, 004h, 00Ch, 000h, 029h, 0C8h, 006h, 000h, 06Ch, 001h, 0A0h, 0C3h, 000h, 0FCh
        db  001h, 013h, 01Eh, 02Eh, 029h, 007h, 004h, 000h, 05Bh, 004h, 081h, 007h, 020h, 020h, 089h, 007h
        db  04Ch, 008h, 041h, 0CEh, 063h, 0CEh, 010h, 000h, 063h, 0C6h, 012h, 000h, 0A0h, 006h, 098h, 0C7h
        db  043h, 0C2h, 002h, 013h, 0A0h, 006h, 028h, 0D0h, 020h, 0E0h, 010h, 0E0h, 060h, 004h, 0FAh, 0CEh
        db  040h, 001h, 000h, 004h, 0EAh, 016h, 0A0h, 006h, 0BAh, 0CEh, 0A0h, 0C2h, 0F0h, 007h, 08Ch, 007h
        db  004h, 000h, 08Dh, 007h, 04Ch, 008h, 0A0h, 0C3h, 08Eh, 008h, 0E0h, 0C3h, 090h, 008h, 0A0h, 006h
        db  07Ah, 0C7h, 0E0h, 0C3h, 04Eh, 008h, 04Fh, 001h, 001h, 000h, 013h, 016h, 0E0h, 0C2h, 094h, 008h
        db  0EAh, 016h, 060h, 004h, 0FAh, 0CEh, 0E0h, 0C3h, 04Eh, 008h, 04Fh, 001h, 001h, 000h, 009h, 016h
        db  060h, 004h, 0FAh, 0CEh, 0A0h, 006h, 098h, 0C7h, 0E0h, 0C3h, 04Eh, 008h, 04Fh, 001h, 001h, 000h
        db  0D7h, 013h, 0A0h, 0C2h, 0F0h, 007h, 020h, 0C3h, 078h, 009h, 08Dh, 007h, 04Ch, 008h, 09Dh, 0C3h
        db  0A0h, 006h, 07Ah, 0C7h, 0C0h, 006h, 020h, 0D0h, 050h, 008h, 0C0h, 006h, 040h, 001h, 000h, 004h
        db  00Ah, 016h, 040h, 001h, 080h, 000h, 007h, 013h, 00Eh, 0C8h, 04Ch, 008h, 00Fh, 0C8h, 04Eh, 008h
        db  0A0h, 006h, 0B0h, 0CEh, 0D8h, 010h, 00Eh, 0C8h, 08Eh, 008h, 00Fh, 0C8h, 090h, 008h, 040h, 001h
        db  000h, 004h, 00Ch, 013h, 040h, 001h, 020h, 000h, 058h, 016h, 081h, 007h, 010h, 020h, 09Fh, 010h
        db  0A0h, 006h, 0BAh, 0CEh, 0A0h, 0C3h, 08Eh, 008h, 0E0h, 0C3h, 090h, 008h, 083h, 007h, 098h, 008h
        db  063h, 007h, 004h, 000h, 02Dh, 011h, 083h, 007h, 0B0h, 008h, 063h, 007h, 004h, 000h, 028h, 011h
        db  083h, 007h, 0C8h, 008h, 063h, 007h, 004h, 000h, 023h, 011h, 0C3h, 060h, 060h, 0C2h, 046h, 007h
        db  0E7h, 013h, 069h, 001h, 00Eh, 000h, 000h, 008h, 0E3h, 013h, 000h, 003h, 002h, 000h, 019h, 0C8h
        db  046h, 007h, 003h, 016h, 0A0h, 001h, 03Ah, 007h, 020h, 000h, 000h, 003h, 00Fh, 000h, 0C0h, 001h
        db  000h, 0F0h, 080h, 001h, 000h, 020h, 001h, 002h, 014h, 0D1h, 060h, 004h, 0A8h, 0CFh, 081h, 007h
        db  080h, 020h, 0E0h, 0C8h, 08Eh, 008h, 014h, 000h, 0E0h, 0C8h, 090h, 008h, 016h, 000h, 0C7h, 010h
        db  0E0h, 0C8h, 050h, 008h, 00Eh, 000h, 0CEh, 0C8h, 010h, 000h, 0CFh, 0C8h, 012h, 000h, 040h, 001h
        db  020h, 000h, 0BBh, 016h, 0E3h, 0C1h, 006h, 000h, 0C7h, 0C8h, 008h, 000h, 007h, 0C8h, 06Ch, 001h
        db  007h, 0C8h, 0E0h, 008h, 008h, 002h, 002h, 0FCh, 0B8h, 007h, 000h, 081h, 0E0h, 0C1h, 0E8h, 000h
        db  007h, 0CEh, 020h, 0C8h, 052h, 008h, 092h, 008h, 0DAh, 013h, 0CEh, 0C8h, 014h, 000h, 0CFh, 0C8h
        db  016h, 000h, 080h, 001h, 000h, 004h, 082h, 007h, 054h, 008h, 032h, 0C1h, 008h, 011h, 072h, 0C1h
        db  092h, 0C1h, 082h, 007h, 08Ah, 008h, 004h, 0C1h, 007h, 016h, 060h, 004h, 09Ch, 0D2h, 072h, 0C1h
        db  0B2h, 0C1h, 084h, 001h, 000h, 080h, 0F9h, 013h, 004h, 068h, 092h, 008h, 0C7h, 0C1h, 037h, 016h
        db  020h, 098h, 097h, 008h, 081h, 009h, 016h, 016h, 081h, 007h, 040h, 020h, 0E0h, 0C1h, 094h, 008h
        db  057h, 013h, 0A0h, 006h, 0BAh, 0CEh, 0F4h, 010h, 0E0h, 0C2h, 03Ah, 007h, 0E0h, 042h, 062h, 007h
        db  0E0h, 026h, 03Ch, 0E3h, 002h, 013h, 0A0h, 006h, 0A0h, 0CEh, 0A0h, 006h, 098h, 0C7h, 022h, 010h
        db  0A0h, 006h, 0AAh, 0CEh, 081h, 02Dh, 001h, 0C2h, 0FBh, 013h, 0A0h, 005h, 096h, 008h, 023h, 0C8h
        db  008h, 000h, 06Ch, 001h, 0A0h, 007h, 002h, 0FCh, 000h, 080h, 0C3h, 0C1h, 027h, 002h, 006h, 000h
        db  0A0h, 006h, 050h, 0C2h, 0A3h, 005h, 00Ch, 000h, 008h, 0C8h, 06Ch, 001h, 008h, 0C8h, 0E0h, 008h
        db  008h, 002h, 002h, 0FCh, 0B8h, 007h, 000h, 081h, 0F8h, 0C1h, 004h, 0C1h, 037h, 013h, 0E0h, 0D2h
        db  003h, 001h, 0D2h, 013h, 00Bh, 002h, 00Ah, 001h, 0C4h, 0CEh, 0C7h, 0CEh, 0C5h, 0CEh, 0C6h, 0CEh
        db  0FBh, 004h, 009h, 002h, 000h, 004h, 048h, 0A2h, 0C9h, 0C6h, 020h, 0A8h, 0E0h, 008h, 012h, 001h
        db  020h, 0C8h, 0F2h, 007h, 000h, 001h, 047h, 0C2h, 0C4h, 081h, 001h, 014h, 044h, 0C2h, 0C9h, 061h
        db  009h, 0A2h, 089h, 0A1h, 001h, 017h, 085h, 005h, 009h, 061h, 0A8h, 016h, 082h, 002h, 08Ah, 008h
        db  005h, 016h, 040h, 001h, 010h, 000h, 012h, 013h, 060h, 004h, 0B4h, 0D0h, 060h, 004h, 0CAh, 0D1h
        db  060h, 004h, 04Eh, 0D0h, 081h, 007h, 080h, 020h, 0FBh, 010h, 081h, 007h, 080h, 020h, 0F8h, 010h
        db  081h, 007h, 002h, 020h, 0F5h, 010h, 081h, 007h, 004h, 020h, 0F2h, 010h, 023h, 0C8h, 008h, 000h
        db  06Ch, 001h, 007h, 005h, 0E0h, 0A1h, 0E8h, 000h, 00Ch, 002h, 004h, 0FCh, 007h, 0CFh, 0E0h, 0C2h
        db  092h, 008h, 0E8h, 016h, 0E0h, 0D2h, 003h, 001h, 010h, 016h, 0E0h, 0C2h, 03Ah, 007h, 0E0h, 042h
        db  062h, 007h, 0E0h, 026h, 03Ch, 0E3h, 007h, 013h, 090h, 003h, 0C8h, 02Fh, 0A0h, 006h, 0A0h, 0CEh
        db  0E0h, 0D2h, 003h, 001h, 002h, 016h, 0A0h, 006h, 098h, 0C7h, 023h, 0C8h, 006h, 000h, 06Ch, 001h
        db  0A3h, 0C2h, 00Eh, 000h, 04Ah, 001h, 000h, 001h, 00Bh, 013h, 00Ch, 002h, 00Eh, 0FCh, 05Ch, 0C2h
        db  049h, 002h, 000h, 080h, 00Dh, 002h, 068h, 009h, 07Dh, 0E2h, 009h, 0CFh, 03Dh, 0CFh, 03Dh, 0CFh
        db  00Ch, 002h, 000h, 0FCh, 06Ch, 0C3h, 006h, 000h, 04Dh, 002h, 0FFh, 0E0h, 04Ah, 002h, 000h, 002h
        db  08Ah, 0A2h, 08Ah, 0A2h, 04Ah, 0E3h, 00Dh, 0CBh, 006h, 000h, 0CDh, 006h, 00Bh, 002h, 00Fh, 000h
        db  0ECh, 082h, 004h, 000h, 0AFh, 011h, 0ECh, 0C3h, 00Eh, 000h, 011h, 015h, 010h, 013h, 06Ch, 0C2h
        db  014h, 000h, 049h, 002h, 000h, 01Fh, 0A9h, 013h, 0C9h, 006h, 089h, 002h, 012h, 000h, 0A5h, 01Bh
        db  049h, 001h, 001h, 000h, 0A2h, 013h, 0C9h, 0A2h, 0ECh, 082h, 004h, 000h, 09Eh, 011h, 04Dh, 0A3h
        db  09Fh, 018h, 014h, 011h, 060h, 001h, 066h, 009h, 000h, 080h, 018h, 013h, 01Dh, 009h, 0CCh, 0A2h
        db  0EBh, 0C2h, 008h, 000h, 07Bh, 009h, 04Bh, 002h, 01Eh, 000h, 0A0h, 0C3h, 0F0h, 006h, 0ABh, 023h
        db  004h, 0E0h, 091h, 016h, 060h, 027h, 040h, 0E3h, 08Eh, 016h, 04Dh, 0A3h, 04Dh, 0A3h, 04Dh, 0A3h
        db  0CDh, 006h, 04Dh, 002h, 007h, 000h, 00Dh, 088h, 0EEh, 006h, 00Ah, 015h, 090h, 003h, 0FFh, 06Fh
        db  053h, 02Fh, 0A0h, 005h, 094h, 008h, 0C3h, 004h, 0C0h, 001h, 000h, 004h, 060h, 004h, 0B8h, 0D0h
        db  060h, 001h, 066h, 009h, 000h, 080h, 0F2h, 013h, 001h, 002h, 008h, 020h, 060h, 004h, 0B0h, 0D2h
        db  08Dh, 007h, 000h, 010h, 020h, 020h, 00Ah, 0E0h, 00Ah, 016h, 08Dh, 007h, 000h, 008h, 020h, 020h
        db  00Eh, 0E0h, 005h, 013h, 08Dh, 007h, 000h, 040h, 04Fh, 001h, 001h, 000h, 001h, 016h, 05Bh, 004h
        db  020h, 0E0h, 00Eh, 0E0h, 0A0h, 007h, 0FAh, 008h, 000h, 080h, 00Eh, 0C8h, 0FAh, 007h, 00Fh, 0C8h
        db  0FCh, 007h, 00Eh, 0C8h, 03Ch, 008h, 00Fh, 0C8h, 03Eh, 008h, 0E0h, 004h, 008h, 008h, 0E0h, 004h
        db  00Eh, 008h, 0E0h, 002h, 098h, 007h, 020h, 040h, 04Eh, 0E3h, 020h, 007h, 02Eh, 006h, 060h, 004h
        db  01Ch, 0D7h, 000h, 070h, 04Bh, 0C0h, 0E0h, 004h, 02Eh, 006h, 00Bh, 010h, 020h, 0F0h, 04Dh, 0E3h
        db  002h, 010h, 020h, 0F0h, 04Ch, 0E3h, 04Bh, 0C0h, 0E0h, 004h, 02Eh, 006h, 0E0h, 001h, 062h, 007h
        db  040h, 000h, 020h, 0E8h, 048h, 0E3h, 062h, 007h, 020h, 004h, 07Ch, 0EAh, 040h, 001h, 000h, 020h
        db  004h, 013h, 0FAh, 010h, 040h, 001h, 000h, 040h, 0F7h, 016h, 020h, 007h, 02Eh, 006h, 020h, 050h
        db  052h, 0E3h, 051h, 004h, 0F1h, 010h, 0E0h, 002h, 058h, 007h, 000h, 001h, 000h, 040h, 007h, 016h
        db  08Dh, 007h, 0F6h, 008h, 0A0h, 006h, 0ACh, 0C5h, 0E0h, 002h, 098h, 007h, 05Bh, 004h, 0C4h, 001h
        db  004h, 000h, 0E0h, 002h, 098h, 007h, 05Bh, 004h, 060h, 001h, 060h, 007h, 004h, 000h, 006h, 016h
        db  020h, 0E8h, 01Ch, 0E0h, 058h, 007h, 080h, 003h, 0E0h, 002h, 098h, 007h, 020h, 0D8h, 0DCh, 007h
        db  017h, 001h, 08Fh, 007h, 08Eh, 0FFh, 00Fh, 0C8h, 004h, 001h, 020h, 0E8h, 006h, 0E0h, 058h, 007h
        db  080h, 001h, 000h, 080h, 05Bh, 004h, 0E0h, 0C2h, 04Ah, 008h, 0C3h, 082h, 003h, 013h, 0DBh, 02Dh
        db  003h, 0C8h, 04Ah, 008h, 049h, 001h, 000h, 001h, 002h, 016h, 060h, 004h, 05Ch, 0D6h, 0E0h, 0C0h
        db  0F8h, 005h, 0FDh, 013h, 003h, 0C8h, 06Ch, 001h, 020h, 0C8h, 000h, 0FCh, 0F8h, 005h, 088h, 007h
        db  002h, 0FCh, 078h, 0C2h, 0F8h, 0C1h, 028h, 002h, 000h, 004h, 049h, 001h, 000h, 001h, 04Dh, 016h
        db  009h, 001h, 000h, 05Eh, 029h, 016h, 049h, 001h, 002h, 000h, 00Bh, 016h, 060h, 001h, 046h, 008h
        db  000h, 002h, 00Ah, 016h, 027h, 002h, 004h, 000h, 007h, 088h, 07Ah, 009h, 005h, 012h, 027h, 002h
        db  0FCh, 0FFh, 0A0h, 001h, 046h, 008h, 000h, 002h, 0C7h, 0C1h, 037h, 015h, 0D3h, 02Dh, 0E0h, 0C0h
        db  04Ah, 008h, 007h, 0A8h, 048h, 008h, 007h, 0A8h, 044h, 008h, 00Ch, 015h, 020h, 0C8h, 03Ch, 008h
        db  0FAh, 007h, 020h, 0C8h, 03Eh, 008h, 0FCh, 007h, 020h, 0C8h, 040h, 008h, 03Ch, 008h, 020h, 0C8h
        db  042h, 008h, 03Eh, 008h, 060h, 004h, 05Ch, 0D6h, 0A0h, 006h, 098h, 0C7h, 0D3h, 02Dh, 0E0h, 0C2h
        db  04Ah, 008h, 0C3h, 082h, 001h, 013h, 0DBh, 02Dh, 020h, 088h, 03Eh, 008h, 03Ah, 008h, 00Dh, 016h
        db  020h, 088h, 03Ch, 008h, 038h, 008h, 009h, 016h, 0E0h, 004h, 044h, 008h, 082h, 007h, 002h, 008h
        db  004h, 061h, 0E0h, 004h, 048h, 008h, 060h, 004h, 028h, 0D7h, 020h, 0C8h, 038h, 008h, 0FAh, 007h
        db  020h, 0C8h, 03Ah, 008h, 0FCh, 007h, 060h, 004h, 01Ch, 0D7h, 007h, 0A8h, 048h, 008h, 004h, 0C1h
        db  01Bh, 016h, 082h, 002h, 038h, 008h, 00Ah, 016h, 060h, 001h, 0FCh, 007h, 001h, 000h, 002h, 016h
        db  0A0h, 006h, 078h, 0D8h, 0A0h, 006h, 006h, 0D8h, 080h, 001h, 010h, 000h, 032h, 0C1h, 007h, 011h
        db  072h, 0C1h, 092h, 0C1h, 082h, 007h, 038h, 008h, 004h, 0C1h, 006h, 016h, 0EAh, 010h, 072h, 0C1h
        db  0B2h, 0C1h, 084h, 001h, 000h, 080h, 0E5h, 013h, 0E0h, 0D2h, 003h, 001h, 034h, 013h, 00Bh, 002h
        db  00Ah, 001h, 0C4h, 0CEh, 0C7h, 0CEh, 0C5h, 0CEh, 0C6h, 0CEh, 0FBh, 004h, 0C8h, 0C6h, 003h, 0A8h
        db  012h, 001h, 020h, 0C8h, 0F8h, 007h, 000h, 001h, 0C7h, 0C2h, 0C4h, 081h, 001h, 014h, 0C4h, 0C2h
        db  00Bh, 0A8h, 044h, 008h, 00Bh, 061h, 00Bh, 0A2h, 08Bh, 0A1h, 001h, 017h, 085h, 005h, 0CBh, 061h
        db  0C6h, 016h, 040h, 001h, 040h, 000h, 015h, 016h, 087h, 007h, 020h, 000h, 0E0h, 061h, 044h, 008h
        db  0C4h, 081h, 008h, 01Ah, 007h, 0A8h, 048h, 008h, 007h, 0A8h, 044h, 008h, 007h, 061h, 087h, 0A1h
        db  001h, 017h, 085h, 005h, 080h, 001h, 040h, 000h, 003h, 0C8h, 06Ch, 001h, 0E0h, 0C1h, 004h, 0FCh
        db  0ACh, 010h, 060h, 004h, 0C6h, 0D4h, 020h, 001h, 03Ah, 007h, 000h, 070h, 004h, 013h, 0A0h, 006h
        db  032h, 0D4h, 020h, 007h, 02Eh, 006h, 0A0h, 006h, 098h, 0C7h, 0C1h, 010h, 0E0h, 0D2h, 003h, 001h
        db  00Ah, 016h, 020h, 001h, 03Ah, 007h, 000h, 070h, 004h, 013h, 0A0h, 006h, 032h, 0D4h, 020h, 007h
        db  02Eh, 006h, 0A0h, 006h, 098h, 0C7h, 090h, 003h, 0BFh, 04Fh, 0D3h, 02Dh, 060h, 001h, 0FCh, 007h
        db  001h, 000h, 002h, 016h, 0A0h, 006h, 078h, 0D8h, 060h, 0D2h, 046h, 008h, 089h, 001h, 000h, 0F1h
        db  0C9h, 001h, 000h, 070h, 040h, 001h, 010h, 000h, 01Ch, 013h, 020h, 088h, 03Eh, 008h, 03Ah, 008h
        db  004h, 016h, 020h, 088h, 03Ch, 008h, 038h, 008h, 014h, 013h, 089h, 001h, 000h, 010h, 08Dh, 007h
        db  044h, 008h, 09Dh, 007h, 000h, 050h, 0A0h, 0C2h, 0F6h, 007h, 08Ch, 007h, 002h, 000h, 0A0h, 0C3h
        db  03Ch, 008h, 0E0h, 0C3h, 03Eh, 008h, 02Fh, 002h, 004h, 000h, 001h, 017h, 08Eh, 005h, 0A0h, 006h
        db  044h, 0C7h, 08Dh, 007h, 046h, 008h, 049h, 0C7h, 0A0h, 0C2h, 0F6h, 007h, 08Ch, 007h, 004h, 000h
        db  0A0h, 0C3h, 038h, 008h, 0E0h, 0C3h, 03Ah, 008h, 0CCh, 0A3h, 001h, 017h, 08Eh, 005h, 0A0h, 006h
        db  044h, 0C7h, 020h, 0C8h, 03Ch, 008h, 0FCh, 008h, 020h, 0C8h, 03Eh, 008h, 0FEh, 008h, 009h, 001h
        db  000h, 00Ch, 00Ch, 013h, 049h, 001h, 000h, 004h, 005h, 016h, 0A0h, 006h, 076h, 0D4h, 0A0h, 006h
        db  042h, 0D4h, 004h, 010h, 090h, 003h, 07Fh, 040h, 0A0h, 006h, 076h, 0D4h, 0C0h, 001h, 090h, 000h
        db  0A0h, 006h, 006h, 0D8h, 00Bh, 0C8h, 046h, 008h, 0E0h, 0C2h, 042h, 007h, 02Dh, 013h, 0E0h, 0C2h
        db  02Eh, 006h, 02Ah, 013h, 0E0h, 002h, 058h, 007h, 08Fh, 007h, 0BFh, 0FFh, 00Fh, 02Ch, 0E0h, 002h
        db  098h, 007h, 0E0h, 0C0h, 05Ch, 007h, 003h, 0C8h, 04Ah, 008h, 003h, 0C8h, 06Ch, 001h, 0C3h, 0C2h
        db  0CBh, 0A2h, 0EBh, 0C2h, 032h, 00Ch, 032h, 013h, 00Bh, 0C8h, 000h, 0FCh, 00Bh, 0C3h, 04Bh, 0C3h
        db  00Bh, 0C8h, 06Ch, 001h, 0E0h, 0C2h, 000h, 0FCh, 0FAh, 016h, 000h, 003h, 002h, 000h, 020h, 0C8h
        db  0F8h, 005h, 000h, 0FCh, 002h, 016h, 00Dh, 0C8h, 0FAh, 005h, 00Ch, 0C8h, 0F8h, 005h, 000h, 003h
        db  00Fh, 000h, 003h, 0C8h, 06Ch, 001h, 01Ah, 010h, 0A0h, 0C3h, 02Eh, 006h, 003h, 013h, 0E0h, 0C0h
        db  0F8h, 005h, 00Dh, 016h, 04Fh, 02Eh, 0C0h, 001h, 000h, 080h, 0A0h, 001h, 062h, 007h, 000h, 080h
        db  08Eh, 0C3h, 003h, 013h, 0A0h, 001h, 062h, 007h, 040h, 000h, 060h, 004h, 058h, 0D4h, 003h, 0C8h
        db  06Ch, 001h, 020h, 0C8h, 000h, 0FCh, 0F8h, 005h, 003h, 0C8h, 04Ah, 008h, 060h, 001h, 066h, 009h
        db  000h, 004h, 002h, 013h, 060h, 004h, 0EEh, 0D4h, 08Ch, 007h, 00Eh, 000h, 020h, 0C2h, 00Eh, 0FCh
        db  00Ah, 015h, 009h, 013h, 020h, 0C2h, 014h, 0FCh, 048h, 002h, 000h, 01Fh, 0C8h, 006h, 088h, 002h
        db  012h, 000h, 0F0h, 01Bh, 008h, 0A3h, 088h, 007h, 002h, 0FCh, 078h, 0C2h, 0F8h, 0C1h, 028h, 002h
        db  000h, 004h, 007h, 083h, 0E7h, 01Ah, 0CCh, 061h, 007h, 0C8h, 004h, 0FCh, 0CCh, 0C1h, 0C0h, 001h
        db  040h, 000h, 060h, 004h, 0FAh, 0D4h, 04Bh, 0C1h, 0A0h, 0C2h, 0F0h, 007h, 020h, 0C3h, 076h, 009h
        db  08Dh, 007h, 0FAh, 007h, 09Dh, 0C3h, 0E0h, 0C3h, 0FCh, 007h, 0A0h, 006h, 044h, 0C7h, 020h, 0C8h
        db  03Ch, 008h, 040h, 008h, 020h, 0C8h, 03Eh, 008h, 042h, 008h, 00Eh, 0C8h, 03Ch, 008h, 00Fh, 0C8h
        db  03Eh, 008h, 0C4h, 004h, 082h, 007h, 002h, 008h, 0E0h, 004h, 044h, 008h, 040h, 001h, 080h, 000h
        db  006h, 016h, 00Eh, 0C8h, 038h, 008h, 00Fh, 0C8h, 03Ah, 008h, 0E0h, 004h, 048h, 008h, 0A0h, 006h
        db  098h, 0C7h, 0E0h, 0C2h, 0FEh, 007h, 00Dh, 011h, 00Eh, 0C8h, 0FAh, 007h, 00Fh, 0C8h, 0FCh, 007h
        db  020h, 0C8h, 040h, 008h, 03Ch, 008h, 020h, 0C8h, 042h, 008h, 03Eh, 008h, 0A0h, 006h, 03Ch, 0D4h
        db  0CBh, 010h, 080h, 001h, 080h, 000h, 055h, 004h, 08Bh, 0C0h, 0A0h, 0C2h, 0F0h, 007h, 08Ch, 007h
        db  004h, 000h, 08Dh, 007h, 0FAh, 007h, 0A0h, 0C3h, 03Ch, 008h, 0E0h, 0C3h, 03Eh, 008h, 0A0h, 006h
        db  07Ah, 0C7h, 060h, 001h, 0FCh, 007h, 001h, 000h, 004h, 013h, 0A0h, 007h, 0FAh, 008h, 000h, 080h
        db  052h, 004h, 060h, 001h, 060h, 007h, 004h, 000h, 007h, 016h, 020h, 0D0h, 004h, 0E0h, 020h, 0E8h
        db  01Ah, 0E0h, 058h, 007h, 060h, 004h, 048h, 0D4h, 0A0h, 007h, 0FAh, 008h, 000h, 040h, 020h, 0C8h
        db  03Ch, 008h, 0FCh, 008h, 020h, 0C8h, 03Eh, 008h, 0FEh, 008h, 0A0h, 006h, 076h, 0D4h, 0A0h, 006h
        db  042h, 0D4h, 0D3h, 010h, 0ADh, 0C2h, 002h, 000h, 06Dh, 0C2h, 000h, 000h, 005h, 016h, 0AAh, 007h
        db  002h, 000h, 036h, 007h, 09Ah, 02Ch, 080h, 003h, 0EAh, 02Ch, 002h, 000h, 041h, 0CBh, 000h, 000h
        db  080h, 003h, 02Dh, 0C3h, 018h, 000h, 0ACh, 007h, 002h, 000h, 036h, 007h, 020h, 04Bh, 0A8h, 0EAh
        db  00Ah, 000h, 020h, 0EBh, 0A2h, 0EAh, 00Ah, 000h, 09Ch, 02Eh, 080h, 003h, 0A0h, 0C2h, 022h, 0E0h
        db  060h, 004h, 07Ch, 0B3h, 0EDh, 0C0h, 018h, 000h, 0A0h, 006h, 044h, 0D9h, 080h, 003h, 044h, 0C2h
        db  0C3h, 0C0h, 002h, 013h, 0A0h, 006h, 044h, 0D9h, 019h, 0C3h, 009h, 0CBh, 018h, 000h, 0C9h, 005h
        db  019h, 0CBh, 016h, 000h, 04Ch, 0C2h, 02Ch, 002h, 01Ah, 000h, 00Dh, 0CFh, 00Eh, 0CFh, 00Fh, 0C7h
        db  099h, 000h, 05Bh, 004h, 08Ch, 007h, 00Ah, 009h, 09Ch, 0C2h, 0A0h, 022h, 014h, 0E0h, 006h, 013h
        db  0A0h, 0C2h, 058h, 007h, 0A0h, 022h, 020h, 0E0h, 001h, 016h, 080h, 003h, 003h, 0C1h, 0C3h, 004h
        db  08Ah, 007h, 004h, 000h, 084h, 0A2h, 03Ah, 0CFh, 03Ah, 0CFh, 03Ah, 0CFh, 03Ah, 0CFh, 03Ah, 0CFh
        db  0E0h, 002h, 058h, 007h, 08Dh, 007h, 00Ah, 009h, 00Bh, 0C8h, 0C2h, 007h, 0A0h, 006h, 088h, 0C5h
        db  0E0h, 0C2h, 0C2h, 007h, 020h, 0E0h, 020h, 0E0h, 0E0h, 002h, 0B8h, 007h, 05Bh, 004h, 02Dh, 0C3h
        db  018h, 000h, 08Ch, 0C2h, 060h, 0C2h, 06Ch, 001h, 00Ah, 0C8h, 06Ch, 001h, 0E0h, 0C2h, 000h, 0FCh
        db  002h, 013h, 08Bh, 0C2h, 0F9h, 010h, 009h, 0C8h, 06Ch, 001h, 08Bh, 007h, 0F8h, 005h, 05Bh, 0C2h
        db  00Ch, 013h, 0CBh, 005h, 05Bh, 0C2h, 0CAh, 0C6h, 0E0h, 0C2h, 06Ch, 001h, 009h, 0C8h, 06Ch, 001h
        db  00Ch, 0C8h, 000h, 0FCh, 00Bh, 0C8h, 06Ch, 001h, 002h, 010h, 0CCh, 0CEh, 0CAh, 0C6h, 0A0h, 0C2h
        db  0E0h, 000h, 0A0h, 022h, 01Ah, 0E0h, 006h, 016h, 020h, 0E8h, 004h, 0E0h, 03Ah, 007h, 020h, 048h
        db  01Ah, 0E0h, 0E0h, 000h, 080h, 003h, 0E0h, 0D3h, 0ADh, 0E3h, 0E0h, 0C1h, 0A8h, 006h, 005h, 016h
        db  007h, 002h, 0A2h, 006h, 0A0h, 006h, 07Ch, 0C2h, 00Bh, 016h, 0E0h, 0C1h, 0BAh, 006h, 020h, 016h
        db  007h, 002h, 0B4h, 006h, 0A0h, 006h, 07Ch, 0C2h, 01Bh, 013h, 007h, 002h, 0B8h, 006h, 002h, 010h
        db  007h, 002h, 0A6h, 006h, 060h, 0C1h, 002h, 0FCh, 025h, 0C8h, 00Ch, 000h, 002h, 0FCh, 0C5h, 0C9h
        db  00Ch, 000h, 0F5h, 0CDh, 0F5h, 0CDh, 0F5h, 0CDh, 0F5h, 0CDh, 0F5h, 0CDh, 0F5h, 0C5h, 0B7h, 001h
        db  028h, 000h, 027h, 002h, 0F4h, 0FFh, 0A7h, 007h, 004h, 000h, 02Eh, 0DBh, 097h, 02Eh, 0D5h, 010h
        db  000h, 003h, 002h, 000h, 0A0h, 006h, 094h, 0C2h, 000h, 003h, 00Fh, 000h, 020h, 02Ch, 04Ah, 0EDh
        db  0E0h, 093h, 0ADh, 0E3h, 003h, 016h, 081h, 002h, 016h, 000h, 0C7h, 016h, 021h, 0C1h, 0B2h, 0EAh
        db  054h, 004h, 0E0h, 093h, 010h, 0E0h, 003h, 016h, 0A0h, 0D2h, 0AAh, 0E3h, 00Bh, 010h, 0CFh, 0D3h
        db  009h, 016h, 0A0h, 023h, 008h, 0E0h, 006h, 016h, 084h, 007h, 020h, 000h, 004h, 0E8h, 0D2h, 006h
        db  0A0h, 0D2h, 00Ch, 0E0h, 060h, 004h, 0A6h, 0DBh, 060h, 004h, 016h, 0DEh, 07Bh, 010h, 022h, 0C1h
        db  004h, 000h, 0E2h, 004h, 002h, 000h, 054h, 004h, 002h, 0C8h, 06Ch, 001h, 082h, 0A0h, 022h, 0C8h
        db  032h, 00Ch, 000h, 0FCh, 002h, 002h, 000h, 0FCh, 0E0h, 093h, 0ACh, 0E3h, 070h, 013h, 0A0h, 006h
        db  0A8h, 0DCh, 0A0h, 048h, 004h, 0E0h, 00Eh, 000h, 085h, 002h, 007h, 000h, 00Eh, 013h, 00Eh, 001h
        db  003h, 000h, 00Bh, 013h, 0A0h, 023h, 022h, 0E0h, 003h, 016h, 0A0h, 0D2h, 00Eh, 0E0h, 002h, 010h
        db  0A0h, 0D2h, 0AAh, 0E3h, 08Eh, 001h, 003h, 000h, 05Ah, 010h, 005h, 0C8h, 0FCh, 006h, 0C3h, 0C0h
        db  053h, 016h, 0A0h, 043h, 010h, 0E0h, 022h, 088h, 00Eh, 000h, 068h, 009h, 00Ah, 016h, 022h, 088h
        db  010h, 000h, 06Ah, 009h, 006h, 016h, 022h, 088h, 012h, 000h, 06Ch, 009h, 002h, 016h, 0A0h, 0E3h
        db  010h, 0E0h, 085h, 002h, 009h, 000h, 002h, 013h, 0A0h, 006h, 0B4h, 0E2h, 045h, 0A1h, 065h, 0C1h
        db  0AEh, 0E3h, 055h, 004h, 062h, 0C0h, 004h, 000h, 022h, 0C8h, 006h, 000h, 06Ch, 001h, 082h, 002h
        db  048h, 004h, 002h, 01Bh, 0A0h, 043h, 00Ch, 0E0h, 022h, 0C1h, 00Eh, 000h, 051h, 004h, 042h, 0C0h
        db  0E1h, 004h, 002h, 000h, 0A2h, 0C0h, 00Ch, 000h, 022h, 0C1h, 00Ah, 000h, 020h, 021h, 018h, 0E0h
        db  007h, 013h, 0A1h, 0C8h, 00Ah, 000h, 00Ah, 000h, 0A1h, 0C8h, 008h, 000h, 008h, 000h, 0E2h, 010h
        db  022h, 0C8h, 006h, 000h, 06Ch, 001h, 0A0h, 006h, 072h, 0E1h, 060h, 004h, 084h, 0DBh, 002h, 0C8h
        db  0D4h, 006h, 062h, 0C1h, 002h, 000h, 065h, 0C1h, 0D0h, 0E3h, 055h, 004h, 00Bh, 010h, 085h, 007h
        db  0F4h, 003h, 0F5h, 004h, 060h, 0CDh, 028h, 0EDh, 0A0h, 006h, 09Eh, 0E3h, 0A0h, 0E3h, 00Ch, 0E0h
        db  0A0h, 02Eh, 0F4h, 003h, 060h, 004h, 0EAh, 0D9h, 0A0h, 006h, 03Eh, 0E0h, 00Ch, 010h, 0A0h, 006h
        db  072h, 0E1h, 009h, 010h, 0A0h, 006h, 026h, 0E3h, 006h, 010h, 0A0h, 006h, 072h, 0E1h, 003h, 0C8h
        db  02Ah, 009h, 0A0h, 0D2h, 0ACh, 0E3h, 0A0h, 006h, 018h, 0DCh, 0A0h, 092h, 026h, 0E0h, 00Ch, 016h
        db  0E0h, 0D3h, 026h, 0E0h, 0E0h, 023h, 014h, 0E0h, 00Ah, 013h, 00Ah, 0C1h, 0C4h, 083h, 007h, 013h
        db  0C4h, 0C3h, 024h, 0C1h, 0D4h, 0E3h, 054h, 004h, 0CAh, 093h, 0DCh, 013h, 0CAh, 0D3h, 0B0h, 003h
        db  00Fh, 0D8h, 059h, 006h, 004h, 071h, 024h, 0C1h, 0E0h, 0E3h, 054h, 004h, 0A0h, 023h, 00Ch, 0E0h
        db  0D1h, 013h, 04Dh, 0C3h, 0CFh, 013h, 086h, 007h, 002h, 000h, 084h, 007h, 026h, 000h, 046h, 023h
        db  003h, 013h, 044h, 006h, 086h, 0A1h, 0FBh, 010h, 046h, 043h, 0B9h, 010h, 085h, 007h, 01Ch, 007h
        db  086h, 007h, 01Ah, 004h, 076h, 06Dh, 076h, 06Dh, 076h, 06Dh, 0C6h, 005h, 076h, 06Dh, 076h, 06Dh
        db  076h, 06Dh, 083h, 007h, 000h, 090h, 0BEh, 010h, 00Bh, 0C3h, 086h, 007h, 000h, 001h, 085h, 007h
        db  000h, 080h, 020h, 0C1h, 0D2h, 006h, 037h, 013h, 0C4h, 004h, 060h, 0C0h, 0D2h, 006h, 045h, 020h
        db  004h, 013h, 084h, 005h, 015h, 009h, 0F9h, 016h, 02Eh, 010h, 0CFh, 0D3h, 006h, 016h, 0E0h, 023h
        db  014h, 0E0h, 003h, 016h, 00Eh, 001h, 003h, 000h, 003h, 013h, 0E0h, 004h, 0D2h, 006h, 023h, 010h
        db  064h, 0D0h, 01Ch, 007h, 046h, 0B0h, 010h, 018h, 001h, 0D9h, 01Ch, 007h, 060h, 023h, 020h, 0E0h
        db  00Bh, 013h, 081h, 007h, 018h, 000h, 061h, 0C0h, 0F0h, 0E3h, 011h, 088h, 028h, 0EDh, 004h, 013h
        db  008h, 002h, 018h, 080h, 0A0h, 006h, 0F2h, 0DFh, 064h, 0D0h, 028h, 007h, 046h, 0B0h, 008h, 018h
        db  001h, 0D9h, 028h, 007h, 046h, 0B0h, 004h, 017h, 083h, 007h, 040h, 080h, 0A0h, 006h, 026h, 0E3h
        db  005h, 048h, 0D2h, 006h, 0CAh, 016h, 020h, 0C1h, 032h, 009h, 001h, 016h, 05Ch, 004h, 004h, 002h
        db  007h, 000h, 020h, 006h, 032h, 009h, 0C9h, 010h, 00Bh, 0C3h, 0C5h, 004h, 042h, 0C0h, 0C7h, 004h
        db  020h, 0C2h, 06Ch, 001h, 0E1h, 0A1h, 004h, 000h, 011h, 0C8h, 06Ch, 001h, 0FBh, 016h, 008h, 0C8h
        db  06Ch, 001h, 0C8h, 004h, 0A0h, 043h, 01Ah, 0E0h, 022h, 0C1h, 00Eh, 000h, 00Dh, 015h, 00Ch, 013h
        db  0A0h, 0E3h, 01Ah, 0E0h, 0A0h, 006h, 010h, 0E3h, 008h, 0C2h, 048h, 013h, 088h, 002h, 012h, 000h
        db  045h, 01Bh, 020h, 022h, 022h, 0E0h, 042h, 013h, 002h, 0C1h, 008h, 0A1h, 008h, 005h, 028h, 002h
        db  0F2h, 0FFh, 007h, 0A2h, 083h, 007h, 001h, 080h, 088h, 002h, 004h, 000h, 06Eh, 011h, 064h, 0C2h
        db  016h, 000h, 049h, 0D2h, 002h, 016h, 002h, 081h, 031h, 016h, 009h, 001h, 000h, 0F0h, 028h, 016h
        db  049h, 0C1h, 045h, 071h, 0C3h, 004h, 085h, 002h, 009h, 000h, 07Ch, 013h, 083h, 007h, 002h, 080h
        db  0A4h, 0C1h, 014h, 000h, 088h, 081h, 076h, 016h, 083h, 005h, 085h, 002h, 010h, 000h, 013h, 01Bh
        db  083h, 005h, 049h, 099h, 0D2h, 0EAh, 00Ah, 013h, 009h, 098h, 00Eh, 0E0h, 06Bh, 016h, 025h, 098h
        db  0D2h, 0EAh, 00Ch, 0E0h, 067h, 016h, 0E0h, 0C1h, 0ECh, 006h, 064h, 016h, 0C3h, 004h, 052h, 0C2h
        db  00Fh, 013h, 083h, 007h, 009h, 080h, 0E0h, 0C1h, 066h, 009h, 047h, 001h, 000h, 010h, 05Ah, 016h
        db  0A0h, 0C0h, 06Ch, 001h, 0A0h, 006h, 0CAh, 0E1h, 060h, 004h, 084h, 0DBh, 060h, 004h, 08Eh, 0DBh
        db  089h, 007h, 00Eh, 007h, 0C7h, 004h, 0E5h, 0D1h, 0E3h, 0EAh, 005h, 013h, 0C7h, 006h, 027h, 002h
        db  0F4h, 0EAh, 077h, 0CEh, 0FEh, 015h, 044h, 0C0h, 021h, 002h, 018h, 000h, 028h, 002h, 0FCh, 0FFh
        db  036h, 013h, 091h, 0C1h, 086h, 0D1h, 01Fh, 013h, 0C6h, 006h, 087h, 007h, 00Eh, 007h, 0F7h, 0C0h
        db  046h, 002h, 0FFh, 0BFh, 043h, 002h, 0FFh, 03Fh, 0A0h, 091h, 04Fh, 0EDh, 009h, 016h, 0B0h, 003h
        db  020h, 098h, 00Eh, 0E0h, 05Dh, 006h, 00Fh, 016h, 021h, 0C8h, 002h, 000h, 00Ch, 007h, 017h, 010h
        db  047h, 082h, 00Ch, 01Bh, 0C6h, 090h, 0EBh, 016h, 047h, 006h, 0F7h, 004h, 0B0h, 003h, 020h, 098h
        db  05Dh, 006h, 057h, 006h, 00Ch, 013h, 083h, 007h, 005h, 080h, 01Ch, 010h, 0D1h, 0C0h, 0E0h, 020h
        db  016h, 0E0h, 003h, 016h, 083h, 007h, 008h, 080h, 015h, 010h, 060h, 044h, 026h, 0E0h, 086h, 071h
        db  046h, 0A0h, 006h, 062h, 083h, 007h, 005h, 080h, 008h, 0C2h, 0CBh, 015h, 00Bh, 016h, 0C3h, 004h
        db  087h, 007h, 00Eh, 007h, 077h, 0C0h, 047h, 082h, 005h, 01Bh, 060h, 020h, 006h, 0E0h, 0FAh, 016h
        db  083h, 007h, 007h, 080h, 05Ch, 004h, 0A0h, 092h, 00Eh, 0E0h, 00Fh, 016h, 020h, 0C8h, 020h, 0E0h
        db  008h, 007h, 0E0h, 004h, 084h, 001h, 060h, 005h, 002h, 007h, 03Bh, 013h, 020h, 048h, 006h, 0E0h
        db  082h, 001h, 083h, 007h, 000h, 0C0h, 0A0h, 006h, 026h, 0E3h, 020h, 0C8h, 01Eh, 0E0h, 002h, 007h
        db  0A0h, 0E3h, 004h, 0E0h, 008h, 002h, 024h, 080h, 0A0h, 006h, 0F2h, 0DFh, 034h, 010h, 020h, 0C1h
        db  084h, 001h, 044h, 002h, 000h, 088h, 01Eh, 013h, 004h, 048h, 084h, 001h, 020h, 006h, 002h, 007h
        db  0F1h, 016h, 0A0h, 023h, 022h, 0E0h, 005h, 016h, 0A0h, 043h, 022h, 0E0h, 0A0h, 0D2h, 00Eh, 0E0h
        db  0D5h, 010h, 0E0h, 023h, 014h, 0E0h, 004h, 013h, 020h, 098h, 0ABh, 0E3h, 065h, 006h, 006h, 016h
        db  0A0h, 092h, 00Eh, 0E0h, 0CBh, 013h, 0A0h, 0D2h, 0AAh, 0E3h, 0D7h, 010h, 083h, 007h, 002h, 000h
        db  060h, 004h, 09Eh, 0DBh, 020h, 0E8h, 006h, 0E0h, 082h, 001h, 020h, 007h, 002h, 007h, 0A0h, 043h
        db  004h, 0E0h, 020h, 0C8h, 070h, 0E4h, 086h, 001h, 020h, 088h, 020h, 0E0h, 008h, 007h, 003h, 016h
        db  020h, 0C8h, 00Ch, 0EBh, 008h, 007h, 060h, 004h, 0A6h, 0DBh, 00Eh, 001h, 003h, 000h, 016h, 013h
        db  0CFh, 0D3h, 008h, 016h, 0A0h, 023h, 020h, 0E0h, 003h, 016h, 0A0h, 0D2h, 0AAh, 0E3h, 002h, 010h
        db  0A0h, 0D2h, 00Eh, 0E0h, 08Eh, 001h, 003h, 000h, 009h, 010h, 060h, 0C1h, 084h, 001h, 060h, 021h
        db  00Ah, 0E0h, 004h, 016h, 083h, 007h, 000h, 084h, 060h, 004h, 09Eh, 0DBh, 020h, 0C8h, 02Eh, 0E0h
        db  084h, 001h, 008h, 002h, 006h, 080h, 0A0h, 006h, 0F2h, 0DFh, 060h, 004h, 0A6h, 0DBh, 060h, 0E3h
        db  020h, 0E0h, 060h, 004h, 0A6h, 0DBh, 0E0h, 093h, 026h, 0E0h, 010h, 016h, 0A0h, 023h, 008h, 0E0h
        db  00Dh, 016h, 0A0h, 023h, 006h, 0E0h, 002h, 013h, 060h, 0E3h, 01Ch, 0E0h, 060h, 0E3h, 018h, 0E0h
        db  0A0h, 043h, 006h, 0E0h, 008h, 002h, 03Ch, 080h, 0A0h, 006h, 0F2h, 0DFh, 060h, 004h, 0A6h, 0DBh
        db  0A0h, 092h, 0AAh, 0E3h, 003h, 013h, 0A0h, 092h, 0ABh, 0E3h, 01Eh, 016h, 0E0h, 023h, 014h, 0E0h
        db  008h, 013h, 020h, 098h, 0ABh, 0E3h, 065h, 006h, 004h, 013h, 083h, 007h, 007h, 000h, 060h, 004h
        db  09Eh, 0DBh, 0A0h, 0D2h, 00Eh, 0E0h, 020h, 0C8h, 020h, 0E0h, 008h, 007h, 0A0h, 027h, 004h, 0E0h
        db  00Bh, 016h, 020h, 0C8h, 01Eh, 0E0h, 008h, 007h, 0E0h, 093h, 0AAh, 0E3h, 005h, 016h, 0A0h, 023h
        db  012h, 0E0h, 002h, 013h, 020h, 006h, 008h, 007h, 060h, 004h, 0A6h, 0DBh, 0E0h, 023h, 014h, 0E0h
        db  036h, 013h, 0B0h, 003h, 020h, 098h, 00Eh, 0E0h, 06Fh, 006h, 00Fh, 016h, 0CFh, 0D3h, 02Fh, 016h
        db  0A0h, 0D2h, 0AAh, 0E3h, 060h, 004h, 0A6h, 0DBh, 0A0h, 092h, 00Ch, 0E0h, 028h, 016h, 0E0h, 023h
        db  014h, 0E0h, 0F6h, 013h, 083h, 007h, 006h, 000h, 007h, 010h, 083h, 007h, 005h, 000h, 0E0h, 093h
        db  00Eh, 0E0h, 002h, 016h, 083h, 007h, 007h, 000h, 060h, 004h, 09Eh, 0DBh, 060h, 0E3h, 012h, 0E0h
        db  0E0h, 023h, 014h, 0E0h, 00Dh, 013h, 020h, 098h, 00Ch, 0E0h, 065h, 006h, 003h, 016h, 020h, 0D8h
        db  0ABh, 0E3h, 065h, 006h, 00Ch, 010h, 020h, 0C1h, 084h, 001h, 020h, 021h, 006h, 0E0h, 0D6h, 016h
        db  020h, 048h, 006h, 0E0h, 084h, 001h, 008h, 002h, 030h, 080h, 0A0h, 006h, 0F2h, 0DFh, 060h, 004h
        db  0A6h, 0DBh, 0A0h, 0E3h, 00Ah, 0E0h, 008h, 0C2h, 002h, 011h, 0A0h, 043h, 00Ah, 0E0h, 020h, 042h
        db  004h, 0E0h, 028h, 002h, 0F0h, 0E3h, 058h, 0C0h, 002h, 0C0h, 011h, 088h, 028h, 0EDh, 003h, 016h
        db  0D1h, 02Ch, 058h, 0C0h, 0D1h, 004h, 080h, 0C0h, 00Eh, 001h, 000h, 010h, 00Fh, 013h, 060h, 0CCh
        db  028h, 0EDh, 0C8h, 005h, 078h, 0CCh, 003h, 016h, 041h, 006h, 060h, 0CCh, 0D6h, 006h, 058h, 0C4h
        db  002h, 016h, 060h, 0C4h, 000h, 007h, 021h, 002h, 0FAh, 0FFh, 091h, 02Ch, 05Bh, 004h, 00Bh, 0C3h
        db  0A0h, 006h, 0DAh, 0E0h, 0A0h, 006h, 0B4h, 0E0h, 008h, 0C2h, 005h, 016h, 062h, 0C2h, 002h, 000h
        db  060h, 026h, 06Ah, 0E4h, 00Dh, 016h, 042h, 0C2h, 0C9h, 005h, 060h, 0CEh, 04Ch, 0EDh, 060h, 0C6h
        db  00Eh, 0EBh, 0A0h, 006h, 01Ch, 0E1h, 018h, 0CAh, 00Ah, 000h, 020h, 046h, 026h, 0E0h, 004h, 016h
        db  0A0h, 0C0h, 06Ch, 001h, 012h, 02Eh, 01Dh, 010h, 012h, 0C1h, 005h, 013h, 060h, 0C1h, 06Ch, 001h
        db  014h, 02Eh, 005h, 0C8h, 06Ch, 001h, 0D2h, 004h, 048h, 006h, 084h, 007h, 002h, 000h, 048h, 0C1h
        db  0A0h, 0C0h, 06Ch, 001h, 002h, 0C0h, 0A0h, 006h, 09Eh, 0E3h, 060h, 0C5h, 002h, 0FCh, 007h, 002h
        db  0A2h, 006h, 025h, 002h, 0F4h, 0FFh, 005h, 0C8h, 002h, 0FCh, 020h, 0C2h, 06Ch, 001h, 0A0h, 006h
        db  040h, 0C2h, 05Ch, 004h, 042h, 0C2h, 029h, 002h, 008h, 000h, 039h, 0C2h, 048h, 002h, 000h, 0C0h
        db  088h, 002h, 000h, 0C0h, 008h, 016h, 060h, 08Eh, 02Eh, 0E0h, 005h, 016h, 060h, 086h, 02Eh, 0E0h
        db  002h, 016h, 0C8h, 004h, 05Bh, 004h, 008h, 007h, 05Bh, 004h, 020h, 088h, 090h, 0E1h, 06Ch, 001h
        db  002h, 016h, 060h, 004h, 08Eh, 0DBh, 05Bh, 004h, 088h, 007h, 0AEh, 001h, 020h, 0E8h, 00Eh, 0E0h
        db  080h, 001h, 008h, 006h, 0FEh, 016h, 020h, 048h, 00Eh, 0E0h, 080h, 001h, 05Bh, 004h, 0C2h, 004h
        db  0A0h, 023h, 00Ch, 0E0h, 00Ah, 016h, 020h, 02Fh, 030h, 006h, 082h, 007h, 0DFh, 0FFh, 002h, 02Ch
        db  082h, 002h, 0F4h, 003h, 0FAh, 016h, 0A0h, 043h, 00Ch, 0E0h, 05Bh, 004h, 042h, 0C2h, 088h, 007h
        db  00Eh, 000h, 009h, 0A2h, 029h, 002h, 008h, 000h, 078h, 0CEh, 078h, 0CEh, 078h, 0CEh, 060h, 0CEh
        db  068h, 009h, 060h, 0CEh, 06Ah, 009h, 060h, 0CEh, 06Ch, 009h, 0A0h, 023h, 01Ah, 0E0h, 00Fh, 016h
        db  058h, 0C2h, 049h, 002h, 080h, 01Fh, 060h, 02Ah, 014h, 0E0h, 0A0h, 0E8h, 004h, 0E0h, 00Eh, 000h
        db  009h, 0C6h, 049h, 002h, 000h, 01Fh, 0C9h, 006h, 009h, 0A2h, 089h, 0A8h, 004h, 000h, 028h, 002h
        db  002h, 000h, 058h, 0C2h, 049h, 00Ah, 049h, 002h, 000h, 0F0h, 009h, 0D6h, 0E2h, 004h, 006h, 000h
        db  05Bh, 004h, 000h, 007h, 082h, 0C0h, 053h, 013h, 0A0h, 0C0h, 06Ch, 001h, 0A0h, 0C1h, 006h, 0FCh
        db  046h, 002h, 00Fh, 000h, 086h, 002h, 001h, 000h, 03Dh, 012h, 006h, 088h, 0F2h, 006h, 012h, 016h
        db  001h, 002h, 00Eh, 0FCh, 031h, 088h, 0F4h, 006h, 00Dh, 016h, 031h, 088h, 0F6h, 006h, 00Ah, 016h
        db  031h, 088h, 0F8h, 006h, 007h, 016h, 086h, 002h, 002h, 000h, 02Ch, 016h, 020h, 088h, 00Ah, 007h
        db  0FAh, 006h, 028h, 013h, 020h, 0C1h, 066h, 009h, 044h, 001h, 000h, 008h, 006h, 013h, 086h, 002h
        db  002h, 000h, 020h, 016h, 044h, 001h, 080h, 000h, 01Dh, 016h, 000h, 007h, 0E0h, 023h, 014h, 0E0h
        db  019h, 016h, 082h, 002h, 043h, 000h, 016h, 013h, 000h, 002h, 002h, 0FCh, 040h, 0C0h, 0B0h, 001h
        db  020h, 000h, 060h, 001h, 066h, 009h, 001h, 000h, 007h, 016h, 060h, 0A0h, 02Ch, 009h, 060h, 0CCh
        db  0EEh, 005h, 050h, 0C4h, 020h, 0C4h, 02Ch, 009h, 080h, 007h, 036h, 007h, 081h, 007h, 040h, 000h
        db  040h, 02Ch, 0C0h, 004h, 084h, 007h, 0F2h, 006h, 006h, 0CDh, 001h, 002h, 00Eh, 0FCh, 031h, 0CDh
        db  031h, 0CDh, 031h, 0CDh, 020h, 0C5h, 00Ah, 007h, 000h, 0C0h, 001h, 013h, 012h, 02Eh, 0E0h, 004h
        db  06Ch, 001h, 05Bh, 004h, 00Bh, 0C8h, 022h, 009h, 0A0h, 006h, 042h, 0E2h, 008h, 002h, 042h, 080h
        db  0A0h, 006h, 0F2h, 0DFh, 008h, 002h, 030h, 080h, 0A0h, 006h, 0F2h, 0DFh, 0E0h, 0C2h, 022h, 009h
        db  05Bh, 004h, 00Bh, 0C3h, 020h, 048h, 010h, 0EBh, 082h, 001h, 020h, 048h, 06Eh, 0E4h, 080h, 001h
        db  020h, 048h, 022h, 0E0h, 0AEh, 001h, 020h, 048h, 022h, 0E0h, 074h, 009h, 060h, 043h, 018h, 0E0h
        db  0A0h, 043h, 008h, 0E0h, 008h, 002h, 042h, 000h, 0A0h, 006h, 0F2h, 0DFh, 05Ch, 004h, 00Bh, 0C3h
        db  020h, 0E8h, 00Eh, 0E0h, 082h, 001h, 020h, 0E8h, 022h, 0E0h, 0AEh, 001h, 020h, 0E8h, 022h, 0E0h
        db  074h, 009h, 0A0h, 0E3h, 008h, 0E0h, 060h, 0E3h, 018h, 0E0h, 0A0h, 043h, 006h, 0E0h, 008h, 002h
        db  03Ch, 080h, 0A0h, 006h, 0F2h, 0DFh, 008h, 002h, 042h, 080h, 0A0h, 006h, 0F2h, 0DFh, 05Ch, 004h
        db  00Bh, 0C3h, 083h, 007h, 000h, 068h, 0A0h, 006h, 026h, 0E3h, 083h, 007h, 010h, 080h, 0A0h, 006h
        db  026h, 0E3h, 05Ch, 004h, 00Bh, 0C3h, 0A0h, 006h, 010h, 0E3h, 002h, 0A2h, 068h, 0C2h, 014h, 000h
        db  029h, 002h, 0FCh, 0FFh, 024h, 013h, 028h, 002h, 018h, 000h, 087h, 007h, 00Eh, 000h, 081h, 007h
        db  00Eh, 007h, 0F1h, 004h, 047h, 006h, 0FDh, 015h, 058h, 0C0h, 0B0h, 003h, 001h, 078h, 063h, 006h
        db  041h, 002h, 03Fh, 000h, 00Eh, 013h, 081h, 002h, 01Fh, 000h, 00Bh, 01Bh, 041h, 0A0h, 061h, 0C0h
        db  050h, 0E4h, 0F8h, 0C1h, 0C7h, 006h, 0C7h, 071h, 047h, 006h, 078h, 0CCh, 047h, 006h, 0FDh, 015h
        db  004h, 010h, 058h, 0C0h, 0C1h, 006h, 041h, 070h, 001h, 0A2h, 049h, 0C2h, 0E5h, 015h, 05Ch, 004h
        db  0A0h, 023h, 01Ah, 0E0h, 002h, 013h, 0C8h, 004h, 05Bh, 004h, 022h, 0C2h, 014h, 000h, 048h, 002h
        db  000h, 01Fh, 0C8h, 006h, 05Bh, 004h, 083h, 002h, 00Fh, 000h, 017h, 01Bh, 0A0h, 0C1h, 0D4h, 006h
        db  035h, 013h, 026h, 002h, 004h, 000h, 0A0h, 0CDh, 028h, 0EDh, 083h, 0C5h, 004h, 013h, 04Ah, 0C2h
        db  039h, 00Ah, 0C9h, 0E0h, 083h, 0C5h, 086h, 007h, 036h, 007h, 087h, 007h, 010h, 000h, 020h, 0C2h
        db  0D4h, 006h, 0E0h, 004h, 0D4h, 006h, 046h, 02Ch, 05Bh, 004h, 060h, 0C0h, 0FEh, 006h, 020h, 0C2h
        db  066h, 009h, 048h, 002h, 000h, 060h, 020h, 022h, 006h, 0E0h, 004h, 016h, 020h, 0E2h, 00Ah, 0E0h
        db  020h, 0E2h, 018h, 0E0h, 013h, 00Ah, 004h, 018h, 041h, 005h, 003h, 048h, 0FEh, 006h, 006h, 010h
        db  083h, 002h, 002h, 000h, 001h, 016h, 013h, 009h, 003h, 0E8h, 0FEh, 006h, 0C8h, 040h, 0C1h, 040h
        db  005h, 013h, 088h, 007h, 036h, 007h, 089h, 007h, 000h, 040h, 048h, 02Ch, 05Bh, 004h, 0C9h, 004h
        db  024h, 0C1h, 026h, 0EBh, 084h, 0C1h, 086h, 071h, 026h, 002h, 0D8h, 0EBh, 0C4h, 006h, 004h, 071h
        db  024h, 002h, 04Eh, 0EBh, 014h, 0D2h, 0C8h, 009h, 008h, 0A2h, 0B0h, 003h, 034h, 0D8h, 05Fh, 006h
        db  047h, 002h, 00Fh, 000h, 0C7h, 0A1h, 028h, 0C2h, 014h, 0EBh, 058h, 004h, 076h, 0CDh, 047h, 006h
        db  0FDh, 016h, 032h, 010h, 036h, 0C2h, 026h, 010h, 017h, 009h, 047h, 0A1h, 02Dh, 010h, 017h, 009h
        db  047h, 061h, 02Ah, 010h, 0A0h, 043h, 016h, 0E0h, 05Bh, 004h, 0A0h, 043h, 016h, 0E0h, 049h, 0C2h
        db  003h, 016h, 044h, 0C2h, 006h, 0C8h, 022h, 009h, 027h, 0C1h, 0E8h, 0ECh, 084h, 0C1h, 086h, 071h
        db  026h, 002h, 01Eh, 0EDh, 0C4h, 006h, 004h, 071h, 024h, 002h, 004h, 0EDh, 0D3h, 010h, 009h, 0C1h
        db  0A0h, 0C1h, 022h, 009h, 0C9h, 004h, 010h, 010h, 036h, 0C2h, 078h, 0D5h, 060h, 041h, 022h, 0E0h
        db  0C5h, 005h, 00Ah, 010h, 078h, 0CDh, 047h, 006h, 0FDh, 015h, 006h, 010h, 0A0h, 023h, 016h, 0E0h
        db  0CDh, 016h, 049h, 0C2h, 0ECh, 016h, 0D6h, 010h, 0A0h, 0E3h, 016h, 0E0h, 0BBh, 010h, 008h, 002h
        db  05Ah, 080h, 0A0h, 006h, 0F2h, 0DFh, 03Eh, 010h, 0A0h, 092h, 00Ch, 0E0h, 015h, 016h, 044h, 002h
        db  000h, 05Eh, 014h, 016h, 020h, 048h, 06Eh, 0E4h, 080h, 001h, 0A0h, 006h, 06Eh, 0E2h, 020h, 0C8h
        db  09Eh, 001h, 09Eh, 001h, 0E0h, 02Eh, 001h, 000h, 0A0h, 043h, 018h, 0E0h, 0A0h, 0D2h, 026h, 0E0h
        db  083h, 007h, 010h, 000h, 0A0h, 006h, 026h, 0E3h, 060h, 004h, 0A6h, 0DBh, 084h, 007h, 008h, 000h
        db  060h, 004h, 06Eh, 0DBh, 085h, 007h, 003h, 002h, 005h, 0C8h, 0CEh, 006h, 0A0h, 043h, 012h, 0E0h
        db  0E0h, 004h, 0FAh, 006h, 0A0h, 006h, 0A0h, 0E2h, 008h, 002h, 048h, 080h, 0A0h, 006h, 0F2h, 0DFh
        db  011h, 010h, 0A0h, 027h, 02Ch, 0E0h, 004h, 016h, 008h, 002h, 054h, 080h, 0A0h, 006h, 0F2h, 0DFh
        db  083h, 007h, 000h, 0A8h, 020h, 088h, 008h, 007h, 020h, 0E0h, 002h, 016h, 083h, 007h, 000h, 0E8h
        db  0A0h, 006h, 026h, 0E3h, 008h, 002h, 036h, 000h, 0A0h, 006h, 0F2h, 0DFh, 020h, 0E8h, 00Ch, 0E0h
        db  082h, 001h, 0A0h, 023h, 018h, 0E0h, 006h, 013h, 0A0h, 0E3h, 018h, 0E0h, 0E0h, 02Eh, 000h, 000h
        db  041h, 0C0h, 0FAh, 016h, 0A0h, 006h, 0FEh, 0E0h, 082h, 0C0h, 002h, 013h, 04Fh, 002h, 080h, 0FFh
        db  0C4h, 004h, 00Fh, 0D1h, 0C4h, 006h, 060h, 004h, 06Eh, 0DBh, 0A0h, 006h, 020h, 0E5h, 008h, 002h
        db  036h, 080h, 0A0h, 007h, 0D6h, 006h, 00Eh, 0E5h, 0A0h, 006h, 0F2h, 0DFh, 010h, 010h, 0A0h, 006h
        db  020h, 0E5h, 020h, 0D1h, 0CEh, 006h, 0E6h, 013h, 020h, 078h, 012h, 0E0h, 0CEh, 006h, 0E2h, 010h
        db  020h, 0C1h, 016h, 004h, 014h, 00Ah, 0C4h, 006h, 00Ah, 091h, 001h, 016h, 05Bh, 004h, 060h, 004h
        db  0A6h, 0DBh, 0CAh, 004h, 0CDh, 004h, 0CEh, 004h, 0CFh, 004h, 0E0h, 004h, 0A8h, 006h, 0E0h, 004h
        db  0BAh, 006h, 084h, 007h, 0A0h, 001h, 085h, 007h, 010h, 000h, 0F4h, 004h, 045h, 006h, 0FDh, 015h
        db  084h, 007h, 0D8h, 006h, 085h, 007h, 034h, 007h, 044h, 061h, 0F4h, 004h, 045h, 006h, 0FDh, 015h
        db  084h, 007h, 0C8h, 000h, 004h, 0C8h, 000h, 007h, 084h, 007h, 0FFh, 07Fh, 004h, 0C8h, 0F0h, 006h
        db  084h, 007h, 006h, 000h, 004h, 0C8h, 0EEh, 006h, 0B0h, 003h, 020h, 0D8h, 010h, 0E0h, 065h, 006h
        db  060h, 004h, 0A6h, 0DBh, 020h, 0F8h, 073h, 0EDh, 082h, 001h, 020h, 0C8h, 010h, 0E0h, 0C6h, 006h
        db  020h, 0C8h, 020h, 0E0h, 0C8h, 006h, 0E0h, 02Eh, 000h, 000h, 0A0h, 006h, 0FEh, 0E0h, 020h, 0C8h
        db  068h, 009h, 0A0h, 001h, 020h, 0C8h, 06Ah, 009h, 0A2h, 001h, 020h, 0C8h, 06Ch, 009h, 0A4h, 001h
        db  020h, 0C8h, 06Ah, 009h, 0B0h, 001h, 020h, 0C8h, 06Ch, 009h, 0B2h, 001h, 020h, 0C8h, 06Ch, 009h
        db  0CCh, 006h, 020h, 0F8h, 072h, 0EDh, 080h, 001h, 0A0h, 006h, 0E8h, 0E0h, 020h, 048h, 004h, 0E0h
        db  080h, 001h, 0E0h, 004h, 09Eh, 001h, 008h, 002h, 012h, 080h, 0A0h, 006h, 0F2h, 0DFh, 084h, 007h
        db  00Ah, 000h, 085h, 007h, 0F4h, 003h, 020h, 088h, 0C6h, 006h, 020h, 0E0h, 004h, 01Bh, 084h, 007h
        db  01Ch, 000h, 085h, 007h, 0F8h, 003h, 0A0h, 006h, 09Eh, 0E3h, 085h, 007h, 012h, 0E6h, 005h, 0C8h
        db  0F8h, 003h, 0A0h, 023h, 00Ch, 0E0h, 023h, 013h, 0A0h, 0E3h, 00Ch, 0E0h, 0A0h, 02Eh, 0F4h, 003h
        db  01Eh, 010h, 0B0h, 003h, 020h, 098h, 010h, 0E0h, 065h, 006h, 019h, 016h, 044h, 002h, 000h, 05Eh
        db  004h, 016h, 020h, 006h, 0C6h, 006h, 0D7h, 016h, 008h, 010h, 0B0h, 003h, 020h, 098h, 010h, 0E0h
        db  065h, 006h, 00Dh, 016h, 020h, 006h, 0C8h, 006h, 0C4h, 016h, 00Eh, 001h, 003h, 000h, 003h, 013h
        db  083h, 007h, 000h, 082h, 002h, 010h, 083h, 007h, 001h, 000h, 060h, 004h, 09Eh, 0DBh, 060h, 004h
        db  084h, 0DBh, 00Eh, 001h, 003h, 000h, 00Ah, 013h, 008h, 002h, 00Ch, 080h, 0A0h, 006h, 0F2h, 0DFh
        db  0E0h, 0E3h, 014h, 0E0h, 020h, 0C8h, 070h, 0E4h, 086h, 001h, 026h, 010h, 020h, 048h, 00Ch, 0E0h
        db  082h, 001h, 0E0h, 02Eh, 001h, 000h, 060h, 0C1h, 01Eh, 009h, 035h, 00Ah, 005h, 0E8h, 082h, 001h
        db  020h, 0C1h, 066h, 009h, 004h, 001h, 006h, 000h, 006h, 013h, 020h, 0D8h, 0D2h, 0E1h, 02Fh, 009h
        db  020h, 0D8h, 0D2h, 0E1h, 083h, 001h, 020h, 021h, 022h, 0E0h, 003h, 016h, 020h, 0E8h, 022h, 0E0h
        db  080h, 001h, 020h, 021h, 004h, 0E0h, 004h, 016h, 0A0h, 0E3h, 014h, 0E0h, 060h, 004h, 090h, 0DFh
        db  008h, 002h, 000h, 080h, 0A0h, 006h, 0F2h, 0DFh, 020h, 0E8h, 008h, 0E0h, 082h, 001h, 060h, 004h
        db  084h, 0DBh, 020h, 0C8h, 070h, 0E4h, 086h, 001h, 008h, 002h, 000h, 080h, 0A0h, 006h, 0F2h, 0DFh
        db  020h, 0C2h, 01Eh, 009h, 008h, 0A2h, 008h, 005h, 028h, 0C8h, 022h, 0E0h, 0CAh, 006h, 020h, 0C8h
        db  020h, 0E0h, 0C6h, 006h, 020h, 0C8h, 020h, 0E0h, 0C8h, 006h, 060h, 0E3h, 016h, 0E0h, 060h, 004h
        db  0A6h, 0DBh, 044h, 0C1h, 044h, 002h, 000h, 05Eh, 0F8h, 016h, 060h, 025h, 06Ah, 0E4h, 00Fh, 016h
        db  020h, 006h, 0C6h, 006h, 0F2h, 016h, 020h, 006h, 0CAh, 006h, 003h, 013h, 0A0h, 005h, 0CCh, 006h
        db  0E6h, 010h, 0B0h, 003h, 020h, 0D8h, 00Ch, 0E0h, 065h, 006h, 060h, 004h, 0A6h, 0DBh, 020h, 006h
        db  0C8h, 006h, 0E3h, 016h, 020h, 088h, 06Ch, 009h, 0CCh, 006h, 003h, 016h, 083h, 007h, 008h, 000h
        db  002h, 010h, 083h, 007h, 00Ch, 000h, 060h, 004h, 09Eh, 0DBh, 060h, 004h, 0A6h, 0DBh, 0A0h, 023h
        db  008h, 0E0h, 003h, 013h, 060h, 023h, 012h, 0E0h, 006h, 016h, 0B0h, 003h, 020h, 0D8h, 0ABh, 0E3h
        db  065h, 006h, 060h, 004h, 0A6h, 0DBh, 008h, 002h, 000h, 080h, 0A0h, 006h, 0F2h, 0DFh, 060h, 004h
        db  084h, 0DBh, 008h, 002h, 000h, 080h, 0A0h, 006h, 0F2h, 0DFh, 020h, 0C8h, 01Eh, 0E0h, 0C6h, 006h
        db  020h, 0C8h, 01Eh, 0E0h, 0C8h, 006h, 060h, 0E3h, 010h, 0E0h, 060h, 004h, 0A6h, 0DBh, 0E0h, 023h
        db  014h, 0E0h, 019h, 013h, 044h, 0C1h, 044h, 002h, 000h, 01Eh, 0F5h, 016h, 060h, 025h, 06Ah, 0E4h
        db  006h, 016h, 020h, 006h, 0C8h, 006h, 0EFh, 016h, 0C2h, 004h, 060h, 004h, 0F0h, 0EBh, 008h, 002h
        db  01Eh, 080h, 0A0h, 006h, 0F2h, 0DFh, 007h, 010h, 020h, 006h, 0C6h, 006h, 0E4h, 016h, 083h, 007h
        db  009h, 000h, 0A0h, 006h, 09Eh, 0DBh, 060h, 004h, 084h, 0DBh, 0CEh, 004h, 0E0h, 004h, 02Ah, 009h
        db  0E0h, 0D3h, 0ACh, 0E3h, 08Fh, 0C2h, 020h, 0C8h, 072h, 0E4h, 086h, 001h, 020h, 048h, 008h, 0E0h
        db  082h, 001h, 086h, 007h, 005h, 000h, 084h, 007h, 072h, 006h, 054h, 0C1h, 001h, 013h, 0D4h, 02Ch
        db  024h, 002h, 00Ah, 000h, 006h, 006h, 0F9h, 016h, 008h, 002h, 02Ah, 080h, 0A0h, 006h, 0F2h, 0DFh
        db  020h, 02Ch, 01Ah, 0E0h, 060h, 004h, 050h, 0DAh, 0A0h, 006h, 042h, 0E2h, 0CDh, 004h, 0A0h, 023h
        db  01Ch, 0E0h, 00Dh, 013h, 00Eh, 001h, 003h, 000h, 00Ah, 013h, 0A0h, 0E3h, 01Ch, 0E0h, 0B0h, 003h
        db  020h, 0D8h, 010h, 0E0h, 065h, 006h, 0A0h, 0D2h, 026h, 0E0h, 0CFh, 004h, 008h, 010h, 020h, 02Dh
        db  001h, 000h, 0E0h, 0C0h, 02Ah, 009h, 0A0h, 006h, 026h, 0E3h, 0A0h, 0D2h, 0ADh, 0E3h, 060h, 004h
        db  0A6h, 0DBh, 0A0h, 006h, 0B4h, 0E0h, 008h, 0C2h, 019h, 013h, 083h, 007h, 080h, 080h, 0E0h, 023h
        db  014h, 0E0h, 002h, 013h, 083h, 007h, 00Ah, 000h, 060h, 004h, 09Ah, 0DBh, 020h, 0C1h, 006h, 006h
        db  00Dh, 013h, 0A0h, 006h, 0B4h, 0E0h, 008h, 0C2h, 009h, 013h, 083h, 007h, 00Bh, 000h, 0E0h, 023h
        db  014h, 0E0h, 002h, 016h, 083h, 007h, 001h, 080h, 060h, 004h, 09Ah, 0DBh, 083h, 007h, 00Ah, 080h
        db  060h, 004h, 088h, 0DBh, 0E0h, 093h, 00Eh, 0E0h, 05Eh, 013h, 0E0h, 093h, 010h, 0E0h, 017h, 013h
        db  0E0h, 023h, 014h, 0E0h, 004h, 013h, 083h, 007h, 007h, 000h, 060h, 004h, 09Ah, 0DBh, 083h, 007h
        db  000h, 0A0h, 0A0h, 006h, 026h, 0E3h, 083h, 007h, 000h, 048h, 0A0h, 006h, 026h, 0E3h, 0A0h, 0D2h
        db  010h, 0E0h, 020h, 0C8h, 01Ch, 0E0h, 0CAh, 006h, 020h, 0C8h, 020h, 0E0h, 0CCh, 006h, 0A0h, 006h
        db  042h, 0E2h, 008h, 002h, 04Eh, 080h, 0A0h, 006h, 0F2h, 0DFh, 0A0h, 023h, 01Ch, 0E0h, 020h, 013h
        db  020h, 088h, 068h, 009h, 00Eh, 007h, 01Ch, 016h, 020h, 088h, 06Ah, 009h, 010h, 007h, 018h, 016h
        db  020h, 088h, 06Ch, 009h, 012h, 007h, 014h, 016h, 020h, 088h, 00Ah, 007h, 022h, 0E0h, 010h, 013h
        db  020h, 006h, 0CAh, 006h, 038h, 016h, 0A0h, 0E3h, 020h, 0E0h, 006h, 010h, 0E0h, 023h, 014h, 0E0h
        db  0CAh, 016h, 0A0h, 0E3h, 022h, 0E0h, 0C2h, 004h, 0A0h, 0D2h, 0ACh, 0E3h, 060h, 004h, 08Eh, 0DBh
        db  020h, 0C8h, 01Ch, 0E0h, 0CAh, 006h, 0A0h, 088h, 0DCh, 006h, 00Eh, 000h, 010h, 016h, 0A0h, 088h
        db  0DEh, 006h, 010h, 000h, 00Ch, 016h, 0A0h, 088h, 0E0h, 006h, 012h, 000h, 008h, 016h, 020h, 006h
        db  0CCh, 006h, 019h, 016h, 020h, 0E8h, 00Eh, 0E0h, 082h, 001h, 0A0h, 0E3h, 01Eh, 0E0h, 020h, 0C8h
        db  020h, 0E0h, 0CCh, 006h, 010h, 010h, 0A0h, 023h, 010h, 0E0h, 008h, 016h, 064h, 0C1h, 006h, 000h
        db  060h, 021h, 00Ch, 0E0h, 008h, 013h, 0A0h, 0D2h, 0AAh, 0E3h, 005h, 010h, 020h, 088h, 00Ah, 007h
        db  008h, 007h, 096h, 012h, 000h, 010h, 060h, 004h, 08Eh, 0DBh, 0E0h, 093h, 00Eh, 0E0h, 050h, 013h
        db  0E0h, 093h, 0ABh, 0E3h, 04Dh, 013h, 0E0h, 093h, 0AAh, 0E3h, 01Ch, 013h, 0A0h, 006h, 0A0h, 0E2h
        db  0A0h, 023h, 010h, 0E0h, 045h, 013h, 0A0h, 023h, 008h, 0E0h, 006h, 016h, 060h, 0E3h, 01Eh, 0E0h
        db  020h, 0C8h, 022h, 0E0h, 006h, 007h, 034h, 010h, 0E0h, 023h, 014h, 0E0h, 031h, 016h, 060h, 0C1h
        db  066h, 009h, 060h, 021h, 012h, 0E0h, 02Ch, 016h, 0A0h, 006h, 004h, 0EAh, 031h, 010h, 0A0h, 0D2h
        db  0AAh, 0E3h, 02Eh, 010h, 0A0h, 0E3h, 012h, 0E0h, 0A0h, 006h, 004h, 0EAh, 064h, 0C1h, 006h, 000h
        db  060h, 021h, 00Ch, 0E0h, 025h, 013h, 020h, 088h, 00Eh, 007h, 0DCh, 006h, 014h, 016h, 020h, 088h
        db  010h, 007h, 0DEh, 006h, 010h, 016h, 020h, 088h, 012h, 007h, 0E0h, 006h, 00Ch, 016h, 020h, 098h
        db  0CEh, 006h, 0CFh, 006h, 015h, 013h, 020h, 006h, 0CEh, 006h, 012h, 016h, 060h, 0E3h, 01Ah, 0E0h
        db  0A0h, 0D2h, 00Ch, 0E0h, 00Dh, 010h, 060h, 0E3h, 01Eh, 0E0h, 020h, 0C8h, 032h, 0E0h, 006h, 007h
        db  0A0h, 006h, 042h, 0E2h, 008h, 002h, 048h, 080h, 0A0h, 006h, 0F2h, 0DFh, 0A0h, 0D2h, 0ABh, 0E3h
        db  060h, 004h, 08Eh, 0DBh, 022h, 088h, 00Eh, 000h, 068h, 009h, 0C9h, 01Ah, 00Bh, 01Bh, 022h, 088h
        db  010h, 000h, 06Ah, 009h, 0C4h, 01Ah, 006h, 01Bh, 022h, 088h, 012h, 000h, 06Ch, 009h, 0BFh, 01Ah
        db  001h, 01Bh, 05Bh, 004h, 060h, 0C1h, 06Ch, 001h, 085h, 002h, 043h, 000h, 0E1h, 013h, 0E0h, 093h
        db  0AAh, 0E3h, 0DEh, 016h, 0A0h, 0C8h, 05Ah, 0EDh, 002h, 000h, 084h, 007h, 00Eh, 000h, 042h, 0C1h
        db  0A0h, 0C0h, 06Ch, 001h, 002h, 0C0h, 025h, 002h, 048h, 000h, 081h, 007h, 056h, 0EAh, 083h, 007h
        db  014h, 0AEh, 060h, 004h, 062h, 0EDh, 002h, 002h, 000h, 0FCh, 0CAh, 010h, 020h, 098h, 00Eh, 0E0h
        db  065h, 006h, 003h, 016h, 020h, 0D8h, 0AAh, 0E3h, 065h, 006h, 0E0h, 093h, 0ABh, 0E3h, 00Dh, 013h
        db  0A0h, 023h, 008h, 0E0h, 019h, 016h, 0A0h, 023h, 010h, 0E0h, 016h, 013h, 060h, 0E3h, 01Eh, 0E0h
        db  020h, 0C8h, 020h, 0E0h, 006h, 007h, 0A0h, 006h, 042h, 0E2h, 0A0h, 043h, 018h, 0E0h, 0E0h, 02Eh
        db  001h, 000h, 0A0h, 0D2h, 026h, 0E0h, 083h, 007h, 010h, 000h, 0A0h, 006h, 026h, 0E3h, 0E0h, 023h
        db  014h, 0E0h, 002h, 016h, 0A0h, 006h, 024h, 0E2h, 0A0h, 043h, 02Ch, 0E0h, 020h, 0C8h, 020h, 0E0h
        db  024h, 009h, 060h, 004h, 08Eh, 0DBh, 0A0h, 043h, 00Eh, 0E0h, 0A0h, 0C1h, 024h, 009h, 002h, 013h
        db  020h, 006h, 024h, 009h, 0E0h, 023h, 014h, 0E0h, 003h, 013h, 0A0h, 023h, 008h, 0E0h, 026h, 016h
        db  008h, 002h, 042h, 080h, 0A0h, 006h, 0F2h, 0DFh, 0A0h, 023h, 008h, 0E0h, 01Fh, 016h, 0A0h, 023h
        db  010h, 0E0h, 00Ah, 016h, 022h, 0C1h, 002h, 000h, 020h, 025h, 06Ah, 0E4h, 023h, 016h, 083h, 007h
        db  020h, 080h, 0A0h, 006h, 026h, 0E3h, 012h, 010h, 0A0h, 006h, 042h, 0E2h, 0E0h, 023h, 014h, 0E0h
        db  002h, 016h, 0A0h, 006h, 024h, 0E2h, 060h, 0E3h, 01Eh, 0E0h, 020h, 0C8h, 020h, 0E0h, 006h, 007h
        db  0A0h, 023h, 008h, 0E0h, 003h, 016h, 0A0h, 023h, 006h, 0E0h, 051h, 013h, 020h, 098h, 00Eh, 0E0h
        db  065h, 006h, 003h, 016h, 020h, 0D8h, 0AAh, 0E3h, 065h, 006h, 022h, 0C1h, 002h, 000h, 020h, 025h
        db  06Ah, 0E4h, 00Eh, 013h, 083h, 007h, 020h, 000h, 0A0h, 006h, 026h, 0E3h, 022h, 0C8h, 00Eh, 000h
        db  0E6h, 006h, 022h, 0C8h, 010h, 000h, 0E8h, 006h, 022h, 0C8h, 012h, 000h, 0EAh, 006h, 037h, 010h
        db  022h, 088h, 00Eh, 000h, 0DCh, 006h, 008h, 016h, 022h, 088h, 010h, 000h, 0DEh, 006h, 004h, 016h
        db  022h, 088h, 012h, 000h, 0E0h, 006h, 00Bh, 013h, 022h, 0C8h, 00Eh, 000h, 0DCh, 006h, 022h, 0C8h
        db  010h, 000h, 0DEh, 006h, 022h, 0C8h, 012h, 000h, 0E0h, 006h, 060h, 0E3h, 014h, 0E0h, 0A0h, 023h
        db  00Eh, 0E0h, 008h, 016h, 0A0h, 0C1h, 024h, 009h, 01Ah, 016h, 086h, 007h, 000h, 010h, 006h, 0E8h
        db  0D2h, 006h, 015h, 010h, 0A0h, 0E3h, 00Eh, 0E0h, 0A0h, 023h, 008h, 0E0h, 009h, 016h, 0A0h, 0E3h
        db  006h, 0E0h, 0E0h, 004h, 0E6h, 006h, 0E0h, 004h, 0E8h, 006h, 0E0h, 004h, 0EAh, 006h, 007h, 010h
        db  008h, 002h, 036h, 080h, 0A0h, 007h, 0D6h, 006h, 0BCh, 0DFh, 0A0h, 006h, 0F2h, 0DFh, 060h, 004h
        db  08Eh, 0DBh, 020h, 098h, 065h, 006h, 010h, 0E0h, 003h, 016h, 020h, 0D8h, 00Eh, 0E0h, 065h, 006h
        db  060h, 004h, 08Eh, 0DBh, 0E0h, 023h, 014h, 0E0h, 002h, 013h, 060h, 004h, 08Eh, 0DBh, 02Eh, 010h
        db  0B0h, 003h, 020h, 098h, 0ABh, 0E3h, 06Fh, 006h, 019h, 016h, 024h, 0C2h, 008h, 000h, 016h, 011h
        db  0E0h, 0E3h, 014h, 0E0h, 083h, 007h, 000h, 000h, 0A0h, 006h, 026h, 0E3h, 0A0h, 023h, 014h, 0E0h
        db  004h, 013h, 008h, 002h, 006h, 080h, 0A0h, 006h, 0F2h, 0DFh, 008h, 002h, 01Eh, 000h, 0A0h, 006h
        db  0F2h, 0DFh, 0A0h, 023h, 008h, 0E0h, 002h, 013h, 0A0h, 006h, 024h, 0E2h, 082h, 0C0h, 002h, 016h
        db  060h, 004h, 0A6h, 0DBh, 020h, 0E8h, 076h, 0EDh, 0F0h, 006h, 020h, 099h, 00Eh, 0E0h, 016h, 000h
        db  005h, 016h, 0E0h, 004h, 0ECh, 006h, 020h, 048h, 014h, 0E0h, 0F0h, 006h, 083h, 007h, 001h, 000h
        db  060h, 004h, 088h, 0DBh, 064h, 0C2h, 014h, 000h, 024h, 002h, 018h, 000h, 0C4h, 0C1h, 0C2h, 061h
        db  027h, 002h, 0FCh, 0FFh, 074h, 0C1h, 085h, 0C1h, 045h, 071h, 085h, 002h, 027h, 000h, 046h, 016h
        db  054h, 0C1h, 045h, 002h, 0CFh, 0FFh, 042h, 016h, 0C8h, 004h, 064h, 0C1h, 008h, 000h, 006h, 015h
        db  005h, 013h, 024h, 0C2h, 00Eh, 000h, 048h, 002h, 000h, 01Fh, 0C8h, 006h, 028h, 002h, 011h, 000h
        db  004h, 0A2h, 018h, 098h, 07Bh, 0EDh, 032h, 016h, 042h, 0C1h, 025h, 002h, 004h, 000h, 047h, 065h
        db  035h, 0C2h, 074h, 0CDh, 048h, 006h, 0FDh, 015h, 0A0h, 0C0h, 06Ch, 001h, 002h, 0C1h, 004h, 0C8h
        db  06Ch, 001h, 0A0h, 0C1h, 000h, 0FCh, 005h, 013h, 020h, 0C8h, 012h, 0EBh, 002h, 0FCh, 006h, 0C1h
        db  0F6h, 010h, 002h, 0C8h, 06Ch, 001h, 020h, 0C8h, 05Ah, 0EDh, 002h, 0FCh, 081h, 007h, 0CCh, 0ECh
        db  004h, 0C0h, 083h, 007h, 010h, 002h, 084h, 007h, 00Eh, 000h, 03Bh, 010h, 084h, 007h, 00Ch, 000h
        db  0E2h, 0C0h, 008h, 000h, 005h, 002h, 000h, 0FCh, 0A0h, 0C0h, 06Ch, 001h, 002h, 0C0h, 0A0h, 0C1h
        db  000h, 0FCh, 02Fh, 013h, 0E0h, 004h, 000h, 0FCh, 016h, 02Eh, 02Bh, 010h, 0A0h, 0C8h, 07Ch, 0EDh
        db  00Eh, 000h, 0A0h, 0C8h, 07Eh, 0EDh, 010h, 000h, 0A0h, 0C8h, 080h, 0EDh, 012h, 000h, 083h, 007h
        db  006h, 080h, 060h, 004h, 088h, 0DBh, 060h, 004h, 0A6h, 0DBh, 084h, 007h, 010h, 000h, 085h, 007h
        db  034h, 000h, 009h, 010h, 084h, 007h, 012h, 000h, 085h, 007h, 032h, 000h, 004h, 010h, 084h, 007h
        db  014h, 000h, 085h, 007h, 038h, 000h, 0A0h, 006h, 0DAh, 0E0h, 085h, 0C8h, 004h, 000h, 0A0h, 006h
        db  01Ch, 0E1h, 0A0h, 0C8h, 05Ah, 0EDh, 002h, 000h, 0A0h, 0C0h, 06Ch, 001h, 002h, 0C0h, 048h, 006h
        db  048h, 0C1h, 0A0h, 006h, 09Eh, 0E3h, 007h, 002h, 0A2h, 006h, 060h, 0C5h, 002h, 0FCh, 025h, 002h
        db  0F4h, 0FFh, 005h, 0C8h, 002h, 0FCh, 020h, 0C2h, 06Ch, 001h, 0A0h, 006h, 040h, 0C2h, 060h, 004h
        db  084h, 0DBh, 0A0h, 006h, 09Eh, 0E3h, 007h, 002h, 0B4h, 006h, 0EFh, 010h, 00Bh, 0C3h, 000h, 003h
        db  002h, 000h, 082h, 007h, 0C0h, 000h, 020h, 0C8h, 00Ch, 000h, 0C0h, 000h, 020h, 0C8h, 00Eh, 000h
        db  0C2h, 000h, 020h, 0C8h, 010h, 000h, 0C4h, 000h, 020h, 0C8h, 012h, 000h, 0C6h, 000h, 020h, 0C8h
        db  014h, 000h, 0C8h, 000h, 020h, 0C8h, 016h, 000h, 0CAh, 000h, 020h, 0C8h, 004h, 000h, 0CCh, 000h
        db  020h, 0C8h, 006h, 000h, 0CEh, 000h, 002h, 0C8h, 00Ch, 000h, 0A0h, 007h, 00Eh, 000h, 00Ch, 0EEh
        db  002h, 0C8h, 010h, 000h, 0A0h, 007h, 012h, 000h, 016h, 0EEh, 002h, 0C8h, 014h, 000h, 0A0h, 007h
        db  016h, 000h, 046h, 0EEh, 002h, 0C8h, 004h, 000h, 0A0h, 007h, 006h, 000h, 05Ch, 0EEh, 060h, 001h
        db  01Ch, 001h, 004h, 000h, 009h, 016h, 0E0h, 001h, 040h, 001h, 000h, 008h, 0A0h, 001h, 040h, 001h
        db  000h, 040h, 0E0h, 001h, 040h, 001h, 000h, 004h, 0A0h, 006h, 01Ch, 0F1h, 005h, 002h, 000h, 080h
        db  005h, 0D8h, 080h, 004h, 0C7h, 004h, 000h, 003h, 00Fh, 000h, 088h, 007h, 000h, 010h, 009h, 002h
        db  000h, 020h, 08Ah, 007h, 074h, 0EEh, 003h, 002h, 000h, 0E6h, 05Ah, 004h, 000h, 003h, 000h, 000h
        db  020h, 0D2h, 087h, 001h, 006h, 010h, 000h, 003h, 000h, 000h, 020h, 0C2h, 08Ah, 001h, 008h, 002h
        db  000h, 01Ah, 060h, 0C2h, 0AEh, 000h, 048h, 0DAh, 080h, 004h, 089h, 005h, 089h, 002h, 006h, 000h
        db  007h, 015h, 088h, 007h, 000h, 080h, 048h, 0DAh, 080h, 004h, 009h, 0C8h, 0AEh, 000h, 080h, 003h
        db  0E0h, 002h, 0A0h, 000h, 05Ch, 004h, 000h, 003h, 000h, 000h, 060h, 001h, 09Ch, 001h, 020h, 000h
        db  0E2h, 013h, 020h, 0C2h, 08Ch, 001h, 008h, 002h, 000h, 01Ch, 0E3h, 010h, 000h, 003h, 000h, 000h
        db  060h, 001h, 040h, 001h, 000h, 040h, 0ECh, 016h, 0A0h, 001h, 040h, 001h, 000h, 040h, 008h, 002h
        db  000h, 002h, 0D7h, 010h, 0B3h, 0C0h, 092h, 006h, 0FDh, 010h, 0B3h, 0C0h, 048h, 0C0h, 072h, 0CCh
        db  072h, 0CCh, 032h, 0C1h, 044h, 0CCh, 072h, 0DCh, 004h, 006h, 0FDh, 016h, 05Bh, 004h, 048h, 0C0h
        db  002h, 002h, 092h, 0E9h, 084h, 007h, 006h, 000h, 0F6h, 010h, 002h, 002h, 0E0h, 0E5h, 049h, 0C0h
        db  084h, 007h, 006h, 000h, 0F0h, 010h, 0B3h, 0C0h, 032h, 0C1h, 001h, 002h, 001h, 000h, 044h, 0D0h
        db  0C1h, 006h, 044h, 002h, 0FFh, 000h, 0E7h, 010h, 033h, 0C1h, 073h, 0C0h, 044h, 0D1h, 044h, 002h
        db  0FFh, 000h, 045h, 0DCh, 004h, 006h, 0FDh, 016h, 05Ah, 004h, 0A0h, 006h, 09Ch, 0F0h, 033h, 0C8h
        db  09Eh, 001h, 05Ah, 004h, 0A0h, 006h, 09Ah, 0EEh, 089h, 0C1h, 0A6h, 009h, 066h, 002h, 040h, 000h
        db  006h, 0C8h, 08Ah, 001h, 088h, 0C1h, 0A6h, 009h, 066h, 002h, 040h, 000h, 006h, 0C8h, 08Ch, 001h
        db  0C2h, 004h, 0C7h, 0C1h, 003h, 016h, 002h, 006h, 0FCh, 016h, 04Dh, 010h, 05Ah, 004h, 0A0h, 006h
        db  0E6h, 0EFh, 089h, 0C1h, 0A6h, 009h, 066h, 002h, 040h, 000h, 006h, 0C8h, 08Ah, 001h, 088h, 0C1h
        db  0A6h, 009h, 066h, 002h, 040h, 000h, 006h, 0C8h, 08Ch, 001h, 033h, 0C8h, 09Eh, 001h, 0E8h, 010h
        db  033h, 08Ah, 002h, 000h, 038h, 016h, 073h, 08Ah, 002h, 000h, 035h, 016h, 05Ah, 004h, 020h, 08Ah
        db  08Ch, 0E9h, 002h, 000h, 030h, 016h, 060h, 08Ah, 090h, 0E9h, 002h, 000h, 02Ch, 016h, 082h, 007h
        db  036h, 0EAh, 001h, 010h, 0B3h, 0C0h, 004h, 002h, 080h, 004h, 052h, 0D1h, 003h, 013h, 032h, 09Dh
        db  022h, 016h, 0FBh, 010h, 085h, 007h, 000h, 080h, 005h, 0D8h, 080h, 004h, 0C7h, 004h, 05Ah, 004h
        db  020h, 0C8h, 0C0h, 000h, 00Ch, 000h, 020h, 0C8h, 0C2h, 000h, 00Eh, 000h, 020h, 0C8h, 0C4h, 000h
        db  010h, 000h, 020h, 0C8h, 0C6h, 000h, 012h, 000h, 020h, 0C8h, 0C8h, 000h, 014h, 000h, 020h, 0C8h
        db  0CAh, 000h, 016h, 000h, 020h, 0C8h, 0CCh, 000h, 004h, 000h, 020h, 0C8h, 0CEh, 000h, 006h, 000h
        db  000h, 003h, 00Fh, 000h, 0CCh, 005h, 05Ch, 004h, 0E0h, 004h, 082h, 001h, 002h, 002h, 0DAh, 0E5h
        db  032h, 0C8h, 082h, 001h, 032h, 0C8h, 080h, 001h, 0A0h, 006h, 0B2h, 0EFh, 012h, 0C8h, 082h, 001h
        db  0CAh, 0C2h, 084h, 007h, 0D0h, 007h, 0E0h, 004h, 084h, 001h, 004h, 006h, 0FCh, 016h, 020h, 0C1h
        db  084h, 001h, 0E9h, 016h, 004h, 002h, 032h, 000h, 085h, 007h, 000h, 080h, 005h, 0D8h, 080h, 004h
        db  0C7h, 004h, 060h, 0C1h, 086h, 001h, 004h, 006h, 0FCh, 016h, 020h, 0C1h, 084h, 001h, 05Bh, 004h
        db  0B3h, 0C0h, 0B3h, 0C4h, 05Bh, 004h, 048h, 0C0h, 0B3h, 0C0h, 073h, 0A0h, 042h, 0C4h, 05Bh, 004h
        db  033h, 088h, 084h, 001h, 0E6h, 016h, 05Ah, 004h, 089h, 0C1h, 0A6h, 009h, 066h, 002h, 040h, 000h
        db  000h, 000h, 0F0h, 000h, 00Fh, 0B8h, 006h, 0C8h, 08Ah, 001h, 05Bh, 004h, 0C5h, 004h, 0A0h, 007h
        db  09Ch, 001h, 040h, 000h, 060h, 001h, 09Ch, 001h, 040h, 000h, 003h, 013h, 005h, 006h, 0F7h, 016h
        db  05Ch, 004h, 05Bh, 004h, 0A0h, 006h, 03Ah, 0F0h, 060h, 0C0h, 040h, 001h, 005h, 0C8h, 040h, 001h
        db  002h, 0C5h, 001h, 0C8h, 040h, 001h, 05Ah, 004h, 0A0h, 006h, 03Ah, 0F0h, 008h, 0A1h, 0F4h, 010h
        db  0B3h, 0C0h, 033h, 0C1h, 060h, 0C1h, 040h, 001h, 085h, 001h, 000h, 004h, 0C5h, 001h, 000h, 010h
        db  05Bh, 004h, 008h, 0C1h, 009h, 0C2h, 044h, 0C2h, 05Bh, 004h, 005h, 002h, 0C8h, 000h, 005h, 006h
        db  0FEh, 016h, 05Bh, 004h, 033h, 0C1h, 003h, 0C0h, 0C4h, 0C0h, 05Bh, 004h, 0C0h, 0C0h, 05Bh, 004h
        db  0E0h, 094h, 09Eh, 001h, 0C2h, 016h, 0C3h, 005h, 05Bh, 004h, 073h, 0C0h, 0A0h, 006h, 0B4h, 0F0h
        db  02Dh, 002h, 008h, 000h, 085h, 007h, 008h, 000h, 071h, 09Fh, 0B7h, 016h, 005h, 006h, 0FCh, 016h
        db  05Ah, 004h, 002h, 002h, 0E6h, 0E5h, 060h, 004h, 09Eh, 0EEh, 0E9h, 08Ch, 004h, 000h, 0ADh, 016h
        db  05Bh, 004h, 020h, 0C1h, 080h, 001h, 085h, 007h, 0D0h, 007h, 0E0h, 001h, 080h, 001h, 000h, 004h
        db  045h, 006h, 0FEh, 016h, 004h, 0C8h, 080h, 001h, 05Bh, 004h, 033h, 0C1h, 048h, 0C3h, 004h, 0C1h
        db  004h, 013h, 02Dh, 002h, 000h, 004h, 004h, 006h, 0FCh, 016h, 05Bh, 004h, 08Dh, 0C3h, 0A0h, 006h
        db  0B4h, 0F0h, 08Dh, 0C1h, 0A6h, 009h, 066h, 002h, 040h, 000h, 086h, 0C7h, 05Ah, 004h, 08Dh, 0C1h
        db  0A6h, 009h, 066h, 002h, 040h, 000h, 006h, 0C8h, 08Ah, 001h, 05Bh, 004h, 08Dh, 0C1h, 0A6h, 009h
        db  066h, 002h, 040h, 000h, 006h, 0C8h, 08Ch, 001h, 05Bh, 004h, 04Dh, 0C0h, 004h, 002h, 028h, 000h
        db  085h, 007h, 000h, 055h, 060h, 004h, 0C2h, 0EEh, 04Dh, 0C0h, 0B3h, 0C0h, 032h, 0C1h, 060h, 004h
        db  086h, 0EEh, 033h, 0C1h, 060h, 001h, 01Ch, 001h, 004h, 000h, 001h, 016h, 05Bh, 004h, 0C4h, 0C0h
        db  05Bh, 004h, 089h, 007h, 028h, 0E5h, 039h, 0C2h, 007h, 013h, 039h, 0C6h, 039h, 086h, 025h, 016h
        db  039h, 0C6h, 039h, 086h, 022h, 016h, 0F7h, 010h, 002h, 002h, 03Ah, 0F1h, 0C4h, 004h, 0C5h, 004h
        db  039h, 0C2h, 002h, 013h, 060h, 004h, 076h, 0F1h, 002h, 002h, 048h, 0F1h, 0C4h, 004h, 039h, 0C2h
        db  003h, 013h, 079h, 0C1h, 060h, 004h, 076h, 0F1h, 002h, 002h, 058h, 0F1h, 0C5h, 004h, 039h, 0C2h
        db  003h, 013h, 039h, 0C1h, 060h, 004h, 076h, 0F1h, 079h, 0C0h, 0B9h, 0C0h, 081h, 060h, 0C2h, 005h
        db  012h, 009h, 0F1h, 004h, 002h, 006h, 0FDh, 016h, 05Bh, 004h, 05Ch, 004h, 001h, 002h, 0AAh, 0AAh
        db  001h, 0C6h, 044h, 0E0h, 045h, 040h, 058h, 080h, 0F8h, 016h, 001h, 002h, 014h, 000h, 001h, 006h
        db  0FEh, 016h, 001h, 002h, 055h, 055h, 001h, 0C6h, 044h, 0E0h, 045h, 040h, 058h, 080h, 0EDh, 016h
        db  052h, 004h, 0E0h, 002h, 0A0h, 000h, 088h, 007h, 0C0h, 000h, 009h, 002h, 0F0h, 0F1h, 084h, 007h
        db  0ECh, 0E5h, 005h, 002h, 001h, 000h, 08Bh, 0C2h, 0CCh, 004h, 0A0h, 006h, 0FAh, 0F1h, 060h, 02Ch
        db  001h, 000h, 099h, 006h, 0A0h, 02Ch, 002h, 000h, 099h, 006h, 020h, 02Dh, 004h, 000h, 099h, 006h
        db  020h, 02Eh, 008h, 000h, 099h, 006h, 0A0h, 02Fh, 010h, 000h, 08Ch, 005h, 009h, 016h, 080h, 0CCh
        db  081h, 0C4h, 083h, 007h, 03Eh, 0F2h, 088h, 0C0h, 002h, 004h, 08Ch, 005h, 001h, 016h, 033h, 010h
        db  0E0h, 002h, 0A0h, 000h, 05Ah, 004h, 08Ch, 005h, 0FBh, 016h, 080h, 0CCh, 081h, 0C4h, 015h, 00Ah
        db  0B4h, 0C0h, 012h, 0C0h, 088h, 0CCh, 052h, 0C0h, 0B4h, 0C4h, 042h, 006h, 05Bh, 004h, 02Dh, 007h
        db  018h, 000h, 041h, 08Bh, 00Ah, 000h, 0ECh, 016h, 0C1h, 082h, 0EAh, 016h, 0C2h, 002h, 042h, 002h
        db  000h, 002h, 0E6h, 016h, 080h, 003h, 081h, 007h, 001h, 000h, 0F1h, 010h, 001h, 002h, 002h, 000h
        db  0EEh, 010h, 001h, 002h, 004h, 000h, 0EBh, 010h, 001h, 002h, 008h, 000h, 0E8h, 010h, 001h, 002h
        db  010h, 000h, 0E5h, 010h, 0A1h, 002h, 041h, 08Bh, 010h, 000h, 002h, 013h, 060h, 004h, 0EAh, 0F1h
        db  02Dh, 007h, 018h, 000h, 080h, 003h, 009h, 002h, 000h, 008h, 003h, 002h, 004h, 000h, 0C7h, 004h
        db  0A0h, 006h, 06Ah, 0F3h, 060h, 001h, 01Ch, 001h, 004h, 000h, 01Ch, 016h, 0A0h, 001h, 040h, 001h
        db  000h, 008h, 0E0h, 001h, 040h, 001h, 000h, 010h, 004h, 002h, 001h, 000h, 044h, 0CEh, 0C4h, 006h
        db  044h, 0C6h, 0A0h, 001h, 040h, 001h, 000h, 010h, 049h, 006h, 0A0h, 001h, 040h, 001h, 000h, 040h
        db  0E0h, 001h, 040h, 001h, 000h, 004h, 0E0h, 001h, 040h, 001h, 000h, 008h, 0A0h, 006h, 008h, 0F4h
        db  0A0h, 006h, 008h, 0F4h, 0C7h, 005h, 004h, 002h, 0A6h, 0E4h, 0E0h, 004h, 0D0h, 003h, 074h, 0C1h
        db  0B4h, 0C1h, 086h, 005h, 01Ch, 013h, 0E0h, 002h, 0C0h, 000h, 000h, 002h, 000h, 000h, 040h, 0C0h
        db  080h, 0C0h, 0C0h, 0C0h, 000h, 0C1h, 040h, 0C1h, 080h, 0C1h, 0C0h, 0C1h, 000h, 0C2h, 040h, 0C2h
        db  080h, 0C2h, 0C0h, 0C2h, 000h, 0C3h, 040h, 0C3h, 080h, 0C3h, 0C0h, 0C3h, 0A0h, 004h, 0AAh, 000h
        db  0D0h, 003h, 0D0h, 003h, 03Fh, 010h, 085h, 005h, 085h, 081h, 0E1h, 013h, 0E4h, 010h, 0C7h, 005h
        db  005h, 002h, 0FFh, 07Fh, 045h, 0A1h, 0D0h, 003h, 0D0h, 003h, 034h, 010h, 0C0h, 0CCh, 0C1h, 0C4h
        db  003h, 002h, 028h, 000h, 0A0h, 006h, 06Ah, 0F3h, 0E0h, 001h, 042h, 001h, 000h, 010h, 0C7h, 005h
        db  0D0h, 003h, 0D0h, 003h, 027h, 010h, 0C7h, 005h, 0A0h, 0C1h, 04Ah, 001h, 0A0h, 007h, 04Ah, 001h
        db  000h, 00Eh, 0A0h, 001h, 040h, 001h, 000h, 002h, 020h, 007h, 044h, 001h, 060h, 0C1h, 044h, 001h
        db  085h, 002h, 000h, 0FFh, 017h, 016h, 0E0h, 001h, 040h, 001h, 000h, 022h, 005h, 002h, 080h, 000h
        db  005h, 006h, 0D0h, 003h, 0FDh, 016h, 060h, 0C1h, 046h, 001h, 085h, 002h, 000h, 0FFh, 00Ah, 013h
        db  005h, 002h, 0DDh, 021h, 005h, 006h, 000h, 010h, 000h, 010h, 000h, 010h, 000h, 010h, 000h, 010h
        db  0D0h, 003h, 0F8h, 016h, 051h, 010h, 006h, 0C8h, 04Ah, 001h, 0C0h, 0CCh, 0C1h, 0C4h, 04Bh, 010h
        db  013h, 0C0h, 0C8h, 0CCh, 053h, 0C0h, 002h, 002h, 07Ah, 0F3h, 0C2h, 0C4h, 043h, 006h, 05Bh, 004h
        db  060h, 0C0h, 0AEh, 000h, 0C4h, 002h, 044h, 002h, 00Fh, 000h, 044h, 088h, 08Ch, 0E4h, 03Ch, 016h
        db  081h, 002h, 008h, 000h, 027h, 013h, 021h, 0C1h, 09Eh, 0E4h, 014h, 0C1h, 021h, 021h, 07Ch, 0E4h
        db  033h, 016h, 021h, 0C1h, 084h, 0E4h, 081h, 002h, 000h, 000h, 00Bh, 013h, 00Dh, 002h, 0A0h, 000h
        db  084h, 083h, 009h, 013h, 0C4h, 005h, 084h, 083h, 006h, 013h, 0C4h, 005h, 084h, 083h, 003h, 013h
        db  023h, 010h, 00Eh, 081h, 021h, 016h, 021h, 0C1h, 09Eh, 0E4h, 021h, 045h, 07Ch, 0E4h, 0E0h, 001h
        db  042h, 001h, 000h, 010h, 0A0h, 001h, 042h, 001h, 000h, 010h, 0A1h, 0C3h, 096h, 0E4h, 00Fh, 002h
        db  02Fh, 000h, 080h, 003h, 0A0h, 001h, 040h, 001h, 000h, 002h, 0A0h, 001h, 040h, 001h, 000h, 080h
        db  06Dh, 0C0h, 00Ah, 000h, 009h, 013h, 081h, 002h, 00Ah, 000h, 006h, 015h, 00Eh, 002h, 060h, 0F3h
        db  00Fh, 002h, 00Fh, 000h, 080h, 003h, 0CAh, 005h, 0E0h, 002h, 0A0h, 000h, 05Ah, 004h, 059h, 0CEh
        db  020h, 088h, 0A6h, 0E4h, 0A6h, 0E4h, 0F8h, 010h, 0C1h, 004h, 048h, 062h, 089h, 005h, 0A0h, 0C0h
        db  06Ch, 001h, 008h, 0C8h, 06Ch, 001h, 003h, 002h, 000h, 0FCh, 004h, 002h, 000h, 002h, 073h, 0A0h
        db  004h, 006h, 0FDh, 016h, 088h, 005h, 009h, 006h, 0F4h, 016h, 002h, 0C8h, 06Ch, 001h, 081h, 086h
        db  001h, 016h, 0CBh, 005h, 05Bh, 004h, 048h, 0C0h, 089h, 0C0h, 081h, 060h, 0C2h, 005h, 05Bh, 004h
        db  00Bh, 0C3h, 0A0h, 006h, 040h, 0F4h, 041h, 0CCh, 042h, 006h, 0FDh, 016h, 0A0h, 006h, 040h, 0F4h
        db  001h, 0C1h, 044h, 08Ch, 012h, 016h, 0C4h, 005h, 042h, 006h, 0FBh, 016h, 004h, 002h, 086h, 0F4h
        db  003h, 002h, 001h, 001h, 094h, 006h, 003h, 002h, 05Ah, 05Ah, 094h, 006h, 043h, 005h, 094h, 006h
        db  003h, 007h, 094h, 006h, 0C3h, 004h, 094h, 006h, 0CCh, 005h, 05Ch, 004h, 0CBh, 0C1h, 0A0h, 006h
        db  040h, 0F4h, 043h, 0CCh, 042h, 006h, 0FDh, 016h, 0A0h, 006h, 040h, 0F4h, 043h, 08Ch, 0F5h, 016h
        db  042h, 006h, 0FCh, 016h, 057h, 004h, 08Bh, 0C2h, 008h, 0C0h, 049h, 0C1h, 085h, 005h, 080h, 002h
        db  040h, 000h, 003h, 011h, 080h, 002h, 04Fh, 000h, 045h, 012h, 001h, 002h, 040h, 0F4h, 0A1h, 009h
        db  001h, 080h, 040h, 013h, 001h, 002h, 070h, 0F5h, 0A1h, 009h, 001h, 080h, 03Bh, 013h, 060h, 0C0h
        db  006h, 000h, 0A1h, 009h, 001h, 080h, 036h, 013h, 081h, 005h, 001h, 080h, 033h, 013h, 04Ah, 0C0h
        db  0A1h, 009h, 001h, 080h, 02Fh, 013h, 000h, 0C8h, 06Ah, 001h, 080h, 002h, 080h, 000h, 017h, 014h
        db  001h, 002h, 000h, 0F8h, 0A0h, 0C1h, 040h, 001h, 0A0h, 001h, 040h, 001h, 000h, 004h, 002h, 002h
        db  000h, 010h, 003h, 002h, 000h, 004h, 0B1h, 0CCh, 043h, 006h, 0FDh, 016h, 0A0h, 001h, 040h, 001h
        db  000h, 040h, 008h, 002h, 010h, 0F8h, 006h, 0C8h, 040h, 001h, 000h, 0C0h, 002h, 013h, 008h, 002h
        db  000h, 0F8h, 009h, 002h, 0FEh, 0FBh, 0A0h, 006h, 04Ah, 0F4h, 025h, 010h, 080h, 002h, 080h, 000h
        db  009h, 014h, 001h, 002h, 000h, 0F8h, 002h, 002h, 000h, 010h, 003h, 002h, 000h, 004h, 072h, 0CCh
        db  043h, 006h, 0FDh, 016h, 080h, 005h, 080h, 002h, 080h, 000h, 004h, 012h, 060h, 001h, 004h, 001h
        db  020h, 000h, 005h, 013h, 040h, 081h, 0ABh, 016h, 080h, 002h, 080h, 000h, 00Bh, 014h, 0A0h, 007h
        db  06Ah, 001h, 07Eh, 000h, 002h, 002h, 000h, 010h, 003h, 002h, 000h, 004h, 0C1h, 004h, 081h, 0CCh
        db  043h, 006h, 0FDh, 016h, 0CAh, 005h, 05Ah, 004h, 000h, 002h, 062h, 0F5h, 001h, 002h, 092h, 0F6h
        db  040h, 002h, 000h, 0FCh, 041h, 002h, 000h, 0FCh, 040h, 080h, 004h, 013h, 0A0h, 007h, 004h, 001h
        db  03Ch, 000h, 05Bh, 004h, 0C0h, 004h, 001h, 002h, 008h, 000h, 002h, 002h, 000h, 012h, 0E0h, 0C1h
        db  040h, 001h, 0A0h, 001h, 040h, 001h, 000h, 004h, 003h, 002h, 000h, 001h, 000h, 0C8h, 06Ah, 001h
        db  0A0h, 0CCh, 010h, 0F8h, 080h, 005h, 003h, 006h, 0F9h, 016h, 022h, 002h, 000h, 002h, 001h, 006h
        db  0F3h, 016h, 0A0h, 001h, 040h, 001h, 000h, 040h, 007h, 0C8h, 040h, 001h, 000h, 002h, 000h, 008h
        db  040h, 0C0h, 001h, 006h, 001h, 0C8h, 06Ah, 001h, 061h, 002h, 000h, 080h, 001h, 0C8h, 010h, 0F8h
        db  000h, 006h, 0F6h, 016h, 0C0h, 004h, 0C8h, 004h, 0C9h, 004h, 003h, 002h, 000h, 008h, 000h, 0C8h
        db  06Ah, 001h, 080h, 0C1h, 066h, 002h, 000h, 080h, 020h, 0C1h, 010h, 0F8h, 006h, 081h, 015h, 016h
        db  008h, 0C2h, 006h, 013h, 080h, 005h, 003h, 006h, 0F2h, 016h, 008h, 0C2h, 00Dh, 013h, 019h, 010h
        db  0A0h, 007h, 010h, 0F8h, 055h, 055h, 020h, 0C1h, 010h, 0F8h, 084h, 002h, 055h, 055h, 002h, 016h
        db  006h, 0C2h, 0F0h, 010h, 006h, 081h, 0EEh, 013h, 05Bh, 004h, 0A0h, 007h, 010h, 0F8h, 055h, 055h
        db  060h, 0C1h, 010h, 0F8h, 005h, 081h, 003h, 013h, 085h, 002h, 055h, 055h, 0F5h, 016h, 008h, 0C2h
        db  0E1h, 013h, 040h, 0C2h, 009h, 006h, 048h, 002h, 0FFh, 007h, 0C0h, 004h, 001h, 002h, 008h, 000h
        db  002h, 002h, 000h, 012h, 003h, 002h, 000h, 001h, 000h, 0C8h, 06Ah, 001h, 032h, 0C8h, 010h, 0F8h
        db  080h, 005h, 003h, 006h, 0F9h, 016h, 022h, 002h, 000h, 002h, 001h, 006h, 0F3h, 016h, 088h, 002h
        db  040h, 000h, 013h, 015h, 089h, 002h, 04Fh, 000h, 010h, 011h, 0C0h, 004h, 002h, 002h, 000h, 012h
        db  001h, 002h, 008h, 000h, 003h, 002h, 000h, 001h, 080h, 0CCh, 003h, 006h, 0FDh, 016h, 022h, 002h
        db  000h, 002h, 001h, 006h, 0F7h, 016h, 0CBh, 005h, 05Bh, 004h, 0A0h, 007h, 004h, 001h, 037h, 000h
        db  05Bh, 004h, 043h, 028h, 031h, 029h, 038h, 039h, 02Dh, 033h, 038h, 038h, 042h, 020h, 020h, 059h
        db  049h, 054h, 04Bh, 0C2h, 0A8h, 002h, 098h, 000h, 083h, 007h, 002h, 000h, 028h, 002h, 008h, 000h
        db  023h, 0C6h, 0F8h, 0E4h, 048h, 006h, 0C4h, 0C0h, 073h, 00Ah, 065h, 017h, 0A0h, 006h, 092h, 0F7h
        db  0C8h, 0C1h, 0C7h, 005h, 003h, 002h, 0A5h, 000h, 0B0h, 003h, 0F8h, 0CDh, 0F8h, 0CDh, 0A6h, 002h
        db  006h, 062h, 088h, 002h, 00Ah, 000h, 057h, 016h, 003h, 029h, 055h, 016h, 005h, 029h, 0C4h, 080h
        db  052h, 016h, 015h, 009h, 050h, 017h, 015h, 009h, 04Eh, 018h, 085h, 002h, 029h, 000h, 04Bh, 016h
        db  0C6h, 005h, 096h, 000h, 003h, 007h, 0C4h, 004h, 045h, 006h, 095h, 000h, 044h, 005h, 043h, 016h
        db  044h, 081h, 041h, 016h, 000h, 003h, 005h, 000h, 0C4h, 002h, 000h, 003h, 00Ah, 000h, 044h, 002h
        db  00Fh, 000h, 084h, 002h, 005h, 000h, 037h, 016h, 0C4h, 002h, 000h, 003h, 00Fh, 000h, 044h, 002h
        db  00Fh, 000h, 084h, 002h, 00Ah, 000h, 02Fh, 016h, 004h, 002h, 0FEh, 0FFh, 02Ch, 013h, 02Bh, 015h
        db  02Ah, 01Ah, 084h, 005h, 028h, 012h, 027h, 015h, 026h, 01Ah, 025h, 018h, 084h, 005h, 023h, 016h
        db  022h, 01Bh, 021h, 017h, 084h, 005h, 01Fh, 013h, 01Eh, 01Ah, 01Dh, 011h, 004h, 006h, 01Bh, 016h
        db  0A5h, 002h, 0C5h, 0C1h, 025h, 002h, 006h, 000h, 003h, 002h, 0A5h, 0A5h, 083h, 0C1h, 095h, 000h
        db  003h, 038h, 094h, 000h, 083h, 002h, 02Eh, 06Bh, 00Eh, 016h, 084h, 002h, 059h, 01Ch, 00Bh, 016h
        db  024h, 002h, 069h, 000h, 095h, 000h, 003h, 03Ch, 094h, 000h, 083h, 081h, 004h, 016h, 084h, 002h
        db  069h, 000h, 001h, 016h, 0C9h, 005h, 059h, 004h, 0C3h, 0D0h, 0FDh, 013h, 001h, 01Ch, 0FBh, 010h
        db  0E0h, 090h, 0FFh, 0E4h, 0F8h, 016h, 0C3h, 006h, 0C3h, 0D0h, 0F5h, 01Ch, 0F4h, 016h, 0E0h, 090h
        db  0FCh, 0E4h, 0F1h, 016h, 05Bh, 004h, 00Bh, 0C3h, 009h, 002h, 000h, 0E5h, 0A0h, 006h, 020h, 0F1h
        db  0CCh, 005h, 05Ch, 004h, 088h, 007h, 000h, 0B0h, 089h, 007h, 0FEh, 0FFh, 0A8h, 009h, 0A9h, 009h
        db  08Ah, 007h, 002h, 0E0h, 0A0h, 006h, 012h, 0F4h, 000h, 000h, 088h, 007h, 000h, 0A0h, 089h, 007h
        db  0FEh, 0AFh, 0A8h, 009h, 0A9h, 009h, 08Ah, 007h, 07Ah, 0E0h, 0A0h, 006h, 012h, 0F4h, 000h, 000h
        db  0A0h, 006h, 0ACh, 0F6h, 000h, 000h, 0E6h, 010h, 0E5h, 010h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0A0h, 000h, 010h, 000h, 026h, 0E2h, 0F9h, 0CAh, 000h, 080h, 000h, 040h, 000h, 020h, 000h, 010h
        db  000h, 008h, 000h, 004h, 000h, 002h, 000h, 001h, 080h, 000h, 040h, 000h, 020h, 000h, 010h, 000h
        db  008h, 000h, 004h, 000h, 002h, 000h, 001h, 000h, 000h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 000h, 0F0h
        db  00Fh, 000h, 0FFh, 0FFh, 0FFh, 07Fh, 003h, 000h, 000h, 000h, 0C3h, 000h, 0E7h, 0E7h, 0F2h, 0E7h
        db  0F6h, 0F2h, 043h, 028h, 020h, 029h, 04Fh, 043h, 059h, 050h, 049h, 052h, 048h, 047h, 020h, 054h
        db  042h, 049h, 020h, 04Dh, 039h, 031h, 033h, 038h, 034h, 02Ch, 035h, 02Ch, 036h, 02Ch, 043h, 028h
        db  020h, 029h, 04Fh, 043h, 059h, 050h, 049h, 052h, 048h, 047h, 020h, 054h, 049h, 054h, 031h, 020h
        db  038h, 039h, 02Dh, 033h, 039h, 038h, 039h, 02Ch, 02Ch, 030h, 02Ch, 031h, 000h, 032h, 000h, 000h
        db  03Bh, 09Fh, 0ACh, 0F6h, 09Ch, 0F1h, 06Ch, 0EDh, 0B0h, 0F7h, 000h, 000h, 0C4h, 000h, 02Ah, 0BDh
        db  04Ah, 006h, 050h, 006h, 04Ch, 006h, 0E6h, 0D9h, 04Eh, 006h, 00Fh, 000h, 032h, 006h, 001h, 000h
        db  050h, 007h, 058h, 007h, 052h, 007h, 0B4h, 0C2h, 054h, 007h, 00Fh, 000h, 038h, 007h, 001h, 000h
        db  0BAh, 000h, 0A0h, 000h, 0BCh, 000h, 0BEh, 0F7h, 0BEh, 000h, 00Fh, 000h, 05Eh, 007h, 03Ah, 007h
        db  062h, 007h, 040h, 080h, 064h, 007h, 098h, 0C7h, 066h, 007h, 07Ah, 0C7h, 068h, 007h, 084h, 0C5h
        db  098h, 007h, 000h, 080h, 078h, 007h, 000h, 080h, 0E2h, 008h, 004h, 000h, 0E4h, 008h, 001h, 000h
        db  0ECh, 008h, 008h, 000h, 0F6h, 008h, 00Ah, 000h, 0F8h, 008h, 006h, 000h, 000h, 009h, 00Ch, 000h
        db  002h, 009h, 004h, 000h, 0AEh, 001h, 000h, 000h, 01Eh, 009h, 000h, 000h, 062h, 009h, 000h, 000h
        db  00Ch, 006h, 013h, 000h, 00Ah, 006h, 020h, 000h, 000h, 000h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h
        db  0D8h, 0B2h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h, 0EEh, 0B4h, 0E0h, 000h, 04Ah, 0B6h, 0E0h, 000h
        db  03Ah, 0B9h, 0E0h, 000h, 004h, 0B4h, 0C0h, 000h, 014h, 0B4h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h
        db  078h, 0B3h, 0C0h, 000h, 066h, 0B4h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h
        db  078h, 0B3h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h, 078h, 0B3h, 0C0h, 000h, 050h, 0BDh, 0C0h, 000h
        db  0BAh, 0BDh, 0C0h, 000h, 0F6h, 0BDh, 0C0h, 000h, 066h, 0BEh, 0C0h, 000h, 0E8h, 0BEh, 0E0h, 000h
        db  03Ch, 0C0h, 0E0h, 000h, 0E2h, 0BFh, 0E0h, 000h, 04Ch, 0C1h, 0E0h, 000h, 0D4h, 0C0h, 0E0h, 000h
        db  096h, 0BAh, 0C0h, 000h, 006h, 0BBh, 0C0h, 000h, 060h, 0BBh, 0C0h, 000h, 092h, 0BBh, 0C0h, 000h
        db  0BAh, 0BAh, 0C0h, 000h, 072h, 0B3h, 0C0h, 000h, 072h, 0B3h, 000h, 03Fh, 000h, 07Fh, 000h, 05Eh
        db  030h, 000h, 028h, 000h, 043h, 000h, 0A2h, 0B6h, 0A2h, 0B6h, 008h, 0B5h, 000h, 0B5h, 032h, 0B5h
        db  032h, 0B5h, 04Eh, 0B5h, 0A2h, 0B6h, 000h, 040h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 008h
        db  000h, 020h, 000h, 008h, 000h, 080h, 000h, 008h, 000h, 001h, 000h, 010h, 000h, 000h, 008h, 000h
        db  00Ch, 000h, 010h, 000h, 014h, 000h, 00Eh, 010h, 00Ch, 00Ch, 00Ah, 00Ah, 00Ah, 00Ah, 008h, 008h
        db  008h, 008h, 008h, 008h, 008h, 008h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h, 006h
        db  006h, 006h, 006h, 006h, 006h, 006h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h, 004h
        db  004h, 004h, 004h, 004h, 004h, 004h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h
        db  002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h
        db  002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h
        db  002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h, 002h
        db  002h, 002h, 002h, 002h, 002h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 098h, 007h, 088h, 0D7h, 058h, 007h, 03Ch, 0C6h, 058h, 007h
        db  042h, 0C5h, 058h, 007h, 0ACh, 0C6h, 058h, 007h, 014h, 0C6h, 098h, 007h, 064h, 0D4h, 098h, 007h
        db  05Ch, 0D4h, 078h, 007h, 0D0h, 0CEh, 058h, 007h, 074h, 0C6h, 098h, 007h, 042h, 0D7h, 078h, 007h
        db  0A4h, 0CFh, 058h, 007h, 074h, 0D4h, 058h, 007h, 024h, 0C6h, 058h, 007h, 062h, 0C6h, 058h, 007h
        db  026h, 0C7h, 098h, 007h, 0B8h, 0D8h, 098h, 007h, 098h, 0D4h, 078h, 007h, 064h, 0CFh, 0B8h, 007h
        db  01Eh, 0D9h, 000h, 000h, 000h, 000h, 000h, 000h, 0E6h, 0C7h, 024h, 0CEh, 0D8h, 0CEh, 0E0h, 0D3h
        db  0A0h, 0CAh, 0D8h, 0CAh, 0F6h, 0CAh, 080h, 0CBh, 0A4h, 0CBh, 0C0h, 0CBh, 038h, 0CCh, 038h, 0CCh
        db  06Ch, 0CBh, 0D6h, 0CCh, 026h, 0CBh, 000h, 000h, 000h, 000h, 000h, 02Ch, 000h, 00Ch, 001h, 00Fh
        db  0FFh, 0FEh, 000h, 058h, 000h, 00Eh, 0FFh, 0FEh, 00Eh, 000h, 000h, 070h, 040h, 080h, 000h, 05Eh
        db  0A0h, 0C0h, 0DFh, 0FFh, 000h, 018h, 000h, 0E0h, 000h, 078h, 000h, 050h, 000h, 060h, 000h, 070h
        db  000h, 00Ch, 006h, 000h, 000h, 000h, 086h, 0E3h, 0E6h, 007h, 0F4h, 007h, 008h, 000h, 000h, 000h
        db  08Ch, 0E3h, 0EAh, 007h, 0F4h, 007h, 006h, 000h, 040h, 000h, 000h, 00Ah, 0E6h, 007h, 0EEh, 007h
        db  008h, 000h, 040h, 000h, 006h, 00Ah, 0EAh, 007h, 0EEh, 007h, 000h, 000h, 0E2h, 0C1h, 08Bh, 0D4h
        db  0FFh, 0FFh, 0D7h, 0D1h, 0D9h, 0C5h, 0D4h, 0C3h, 03Bh, 059h, 034h, 009h, 0FCh, 005h, 068h, 009h
        db  0D8h, 006h, 006h, 004h, 07Ch, 0EAh, 030h, 009h, 048h, 004h, 080h, 008h, 006h, 000h, 00Ah, 006h
        db  00Eh, 00Ch, 08Eh, 0DBh, 04Ch, 0E8h, 074h, 0E8h, 05Ah, 0E9h, 05Ch, 0EAh, 0B6h, 0EAh, 010h, 0EBh
        db  0C2h, 0EBh, 08Eh, 0DBh, 044h, 0ECh, 0D4h, 0EBh, 032h, 0E8h, 0E0h, 0EBh, 0E0h, 0EBh, 00Ah, 0EDh
        db  014h, 0EDh, 01Eh, 0EDh, 032h, 0E5h, 0BAh, 0E7h, 084h, 0DBh, 084h, 0E5h, 052h, 0E6h, 0C2h, 0E6h
        db  03Eh, 0E7h, 062h, 0E7h, 0DCh, 0DBh, 084h, 0DBh, 0A2h, 0E4h, 084h, 0E4h, 03Eh, 0E4h, 084h, 0DBh
        db  0C6h, 0E7h, 084h, 0DBh, 072h, 006h, 07Ch, 0DFh, 008h, 007h, 072h, 006h, 0DAh, 0DEh, 0F4h, 001h
        db  072h, 006h, 0BAh, 0DEh, 008h, 007h, 07Ch, 006h, 02Ah, 0E6h, 004h, 000h, 07Ch, 006h, 0FEh, 0DEh
        db  000h, 000h, 07Ch, 006h, 0A8h, 0E7h, 0FAh, 000h, 086h, 006h, 04Eh, 0DEh, 005h, 000h, 090h, 006h
        db  0F8h, 0E7h, 028h, 000h, 090h, 006h, 0D6h, 0DFh, 004h, 001h, 090h, 006h, 000h, 000h, 002h, 000h
        db  090h, 006h, 006h, 0DFh, 0BCh, 002h, 09Ah, 006h, 08Ch, 0DFh, 0DCh, 005h, 09Ah, 006h, 030h, 0DFh
        db  064h, 000h, 09Ah, 006h, 090h, 0DFh, 014h, 000h, 09Ah, 006h, 0ECh, 0E8h, 040h, 006h, 09Ah, 006h
        db  098h, 0DFh, 064h, 000h, 000h, 000h, 00Ah, 007h, 00Eh, 007h, 004h, 007h, 0D8h, 006h, 000h, 007h
        db  0F0h, 006h, 0EEh, 006h, 0ECh, 006h, 00Ch, 007h, 0E6h, 006h, 018h, 007h, 000h, 050h, 0CCh, 000h
        db  003h, 000h, 000h, 084h, 000h, 0A8h, 000h, 0A0h, 000h, 020h, 000h, 080h, 000h, 040h, 000h, 008h
        db  000h, 040h, 000h, 080h, 000h, 040h, 000h, 010h, 010h, 0F4h, 0D6h, 0F2h, 0F0h, 0F2h, 00Ah, 0F3h
        db  000h, 000h, 000h, 000h, 000h, 000h, 009h, 000h, 009h, 000h, 078h, 0F3h, 0E0h, 0F2h, 0F6h, 0F2h
        db  010h, 0F3h, 040h, 001h, 042h, 001h, 042h, 001h, 042h, 001h, 000h, 000h, 07Fh, 000h, 0A0h, 000h
        db  0FFh, 000h, 010h, 002h, 01Fh, 002h, 030h, 002h, 03Fh, 002h, 050h, 002h, 05Fh, 002h, 070h, 002h
        db  07Fh, 002h, 090h, 002h, 09Fh, 002h, 0B0h, 002h, 0BFh, 002h, 0D0h, 002h, 0DFh, 002h, 0E1h, 002h
        db  0FFh, 002h, 001h, 003h, 07Fh, 003h, 081h, 003h, 08Fh, 003h, 091h, 003h, 09Fh, 003h, 0A1h, 003h
        db  0AFh, 003h, 0B1h, 003h, 0BFh, 003h, 0C1h, 003h, 0CFh, 003h, 0E1h, 003h, 0FFh, 003h, 0C0h, 007h
        db  0FFh, 007h, 000h, 00Ch, 0FFh, 00Fh, 000h, 030h, 0FFh, 037h, 0FFh, 0FFh, 0FFh, 0FFh, 0BCh, 0FEh
        db  007h, 000h, 05Eh, 002h, 000h, 001h, 0FFh, 0BAh, 080h, 0BAh, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Ch, 001h, 00Ah, 001h, 00Eh, 001h, 010h, 001h, 014h, 001h, 000h, 000h, 012h, 001h, 000h, 0F8h
        db  016h, 001h, 000h, 0FFh, 000h, 000h, 000h, 000h, 00Ah, 001h, 01Ch, 001h, 082h, 001h, 066h, 096h
        db  066h, 096h, 055h, 055h, 000h, 000h, 082h, 001h, 02Ah, 08Ah, 02Ah, 08Ah, 018h, 0C9h, 018h, 0C9h
        db  086h, 001h, 0AAh, 0A2h, 01Eh, 0A0h, 055h, 055h, 01Eh, 054h, 08Ah, 001h, 000h, 050h, 000h, 000h
        db  000h, 0A8h, 000h, 000h, 08Ch, 001h, 000h, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 08Eh, 001h
        db  000h, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 090h, 001h, 000h, 050h, 000h, 000h, 000h, 0A8h
        db  000h, 000h, 092h, 001h, 000h, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 094h, 001h, 000h, 050h
        db  000h, 000h, 000h, 0A8h, 000h, 000h, 096h, 001h, 000h, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h
        db  098h, 001h, 000h, 050h, 000h, 000h, 000h, 0A8h, 000h, 000h, 09Ah, 001h, 000h, 050h, 000h, 000h
        db  000h, 0A8h, 000h, 000h, 09Ch, 001h, 055h, 055h, 0C0h, 07Fh, 0AAh, 0AAh, 0C0h, 07Fh, 000h, 000h
        db  0A2h, 001h, 0A4h, 001h, 0A8h, 001h, 0AAh, 001h, 0AEh, 001h, 0B0h, 001h, 0B2h, 001h, 080h, 001h
        db  000h, 000h, 088h, 001h, 000h, 0FFh, 09Eh, 001h, 0FFh, 000h, 0A0h, 001h, 000h, 080h, 0ACh, 001h
        db  000h, 080h, 000h, 000h, 0A6h, 001h, 000h, 080h, 000h, 000h, 080h, 001h, 0B2h, 001h, 000h, 088h
        db  000h, 006h, 000h, 0C8h, 000h, 000h, 000h, 080h, 030h, 000h, 000h, 000h, 000h, 080h, 002h, 000h
        db  044h, 000h, 020h, 0F2h, 048h, 000h, 026h, 0F2h, 050h, 000h, 02Ch, 0F2h, 060h, 000h, 032h, 0F2h
        db  078h, 000h, 038h, 0F2h, 098h, 0EFh, 0A6h, 0EEh, 0FEh, 0E9h, 0B8h, 0EEh, 014h, 055h, 0A0h, 001h
        db  07Ah, 0EEh, 092h, 0E9h, 0D4h, 0EEh, 02Eh, 0EFh, 08Eh, 0EEh, 0E6h, 0EFh, 000h, 000h, 01Eh, 000h
        db  0D4h, 0EEh, 020h, 0EFh, 000h, 041h, 001h, 041h, 044h, 0EFh, 035h, 0EAh, 0A6h, 0EEh, 00Ah, 0EAh
        db  07Ah, 0EEh, 0C6h, 0E9h, 0E4h, 0EEh, 0F0h, 0EEh, 044h, 0EFh, 030h, 0EAh, 0F0h, 0EFh, 000h, 000h
        db  0C4h, 0EFh, 07Ah, 0EEh, 0BCh, 0E9h, 0E4h, 0EEh, 0F0h, 0EEh, 0C4h, 0EFh, 0F0h, 0EFh, 000h, 000h
        db  07Ah, 0EEh, 0B2h, 0E9h, 09Ah, 0EEh, 0D8h, 0EEh, 0F0h, 0EEh, 0C4h, 0EFh, 07Ah, 0EEh, 0BCh, 0E9h
        db  0E4h, 0EEh, 0F0h, 0EEh, 0C4h, 0EFh, 0F0h, 0EFh, 000h, 020h, 0B8h, 0EEh, 014h, 055h, 0A0h, 001h
        db  0A6h, 0EEh, 012h, 0EAh, 07Ah, 0EEh, 092h, 0E9h, 0E6h, 0EFh, 050h, 055h, 00Ch, 000h, 0D4h, 0EEh
        db  020h, 0EFh, 000h, 001h, 000h, 000h, 044h, 0EFh, 037h, 0EAh, 08Eh, 0EEh, 0E6h, 0EFh, 055h, 055h
        db  00Ch, 000h, 0E4h, 0EEh, 02Eh, 0EFh, 08Eh, 0EEh, 0E6h, 0EFh, 0FFh, 0FFh, 008h, 000h, 0E6h, 0EFh
        db  002h, 010h, 006h, 000h, 0D4h, 0EEh, 020h, 0EFh, 000h, 001h, 001h, 001h, 044h, 0EFh, 042h, 0EAh
        db  08Eh, 0EEh, 0E6h, 0EFh, 000h, 0C0h, 008h, 000h, 0E6h, 0EFh, 0FFh, 0FFh, 00Ah, 000h, 0E6h, 0EFh
        db  0FFh, 0FFh, 00Ch, 000h, 0E6h, 0EFh, 00Dh, 010h, 006h, 000h, 0D4h, 0EEh, 020h, 0EFh, 000h, 001h
        db  001h, 001h, 044h, 0EFh, 036h, 0EAh, 0F0h, 0EFh, 008h, 020h, 08Eh, 0EEh, 0E0h, 0EFh, 082h, 001h
        db  002h, 0C9h, 0D4h, 0EEh, 044h, 0EFh, 042h, 0EAh, 0F0h, 0EFh, 034h, 020h, 08Eh, 0EEh, 0E6h, 0EFh
        db  000h, 010h, 006h, 000h, 0D4h, 0EEh, 054h, 0F0h, 044h, 0EFh, 03Ah, 0EAh, 0E0h, 0EFh, 09Ch, 001h
        db  040h, 000h, 0A6h, 0EEh, 012h, 0EAh, 0B8h, 0EEh, 0FFh, 000h, 080h, 007h, 0B4h, 0F0h, 003h, 000h
        db  0F4h, 0F0h, 002h, 0F1h, 0D4h, 0E9h, 0C6h, 0F0h, 000h, 000h, 002h, 0F1h, 0DEh, 0E9h, 0C6h, 0F0h
        db  004h, 000h, 002h, 0F1h, 0E6h, 0E9h, 0C6h, 0F0h, 007h, 000h, 002h, 0F1h, 0EEh, 0E9h, 0B4h, 0F0h
        db  001h, 000h, 002h, 0F1h, 0F6h, 0E9h, 0C6h, 0F0h, 002h, 000h, 002h, 0F1h, 0F6h, 0E9h, 0C6h, 0F0h
        db  006h, 000h, 002h, 0F1h, 0F6h, 0E9h, 0C6h, 0F0h, 005h, 000h, 002h, 0F1h, 0F6h, 0E9h, 0B4h, 0F0h
        db  001h, 000h, 0D8h, 0F0h, 0B4h, 0F0h, 003h, 000h, 0E6h, 0F0h, 0F0h, 0EEh, 074h, 0F0h, 09Ah, 0E9h
        db  001h, 000h, 074h, 0F0h, 0E7h, 0E9h, 002h, 000h, 074h, 0F0h, 0F1h, 0E9h, 006h, 000h, 074h, 0F0h
        db  0FCh, 0E9h, 005h, 000h, 044h, 0EFh, 036h, 0EAh, 0C4h, 0EFh, 07Ah, 0EEh, 092h, 0E9h, 0E4h, 0EEh
        db  054h, 0F0h, 09Ah, 0EEh, 020h, 0EFh, 000h, 001h, 000h, 080h, 044h, 0EFh, 03Ah, 0EAh, 08Eh, 0EEh
        db  08Ch, 0F0h, 0E0h, 0EFh, 080h, 001h, 041h, 08Eh, 0D8h, 0EEh, 020h, 0EFh, 000h, 001h, 001h, 01Bh
        db  094h, 0F0h, 0E4h, 0FFh, 044h, 0EFh, 03Eh, 0EAh, 04Ch, 0F0h, 0A6h, 0EEh, 018h, 0EAh, 09Ah, 0EEh
        db  0F8h, 0EFh, 0CAh, 0EEh, 000h, 0E0h, 054h, 0F0h, 044h, 0EFh, 048h, 0EAh, 0CAh, 0EEh, 000h, 0E8h
        db  0F0h, 0EEh, 044h, 0EFh, 047h, 0EAh, 0CAh, 0EEh, 000h, 008h, 054h, 0F0h, 044h, 0EFh, 048h, 0EAh
        db  0CAh, 0EEh, 000h, 0F8h, 0F0h, 0EEh, 044h, 0EFh, 047h, 0EAh, 0E0h, 0EFh, 080h, 001h, 000h, 002h
        db  0CAh, 0EEh, 000h, 0E0h, 0F0h, 0EEh, 044h, 0EFh, 047h, 0EAh, 0E0h, 0EFh, 084h, 001h, 000h, 000h
        db  0F0h, 0EFh, 034h, 000h, 0CAh, 0EEh, 000h, 000h, 054h, 0F0h, 0F0h, 0EFh, 034h, 060h, 09Ch, 0F0h
        db  0E0h, 0EFh, 084h, 001h, 000h, 000h, 044h, 0EFh, 048h, 0EAh, 0E0h, 0EFh, 082h, 001h, 000h, 0C8h
        db  0CAh, 0EEh, 000h, 0E0h, 054h, 0F0h, 0CAh, 0EEh, 000h, 010h, 054h, 0F0h, 0F0h, 0EFh, 034h, 060h
        db  0E0h, 0EFh, 080h, 001h, 000h, 006h, 0CAh, 0EEh, 010h, 000h, 006h, 0F0h, 0C4h, 0EFh, 0E0h, 0EFh
        db  084h, 001h, 000h, 000h, 0F0h, 0EFh, 034h, 000h, 07Ah, 0EEh, 092h, 0E9h, 0A6h, 0EEh, 01Eh, 0EAh
        db  05Eh, 0F0h, 054h, 0E9h, 08Eh, 0EEh, 09Ah, 0EEh, 0FEh, 0EEh, 040h, 0F0h, 006h, 000h, 000h, 0C7h
        db  02Eh, 0EFh, 06Ah, 0F0h, 000h, 0E0h, 08Eh, 0EEh, 09Ah, 0EEh, 0FEh, 0EEh, 040h, 0D0h, 006h, 000h
        db  000h, 0E0h, 02Eh, 0EFh, 06Ah, 0F0h, 000h, 0C0h, 08Eh, 0EEh, 09Ah, 0EEh, 0FEh, 0EEh, 040h, 090h
        db  006h, 000h, 000h, 0A0h, 02Eh, 0EFh, 06Ah, 0F0h, 000h, 080h, 08Eh, 0EEh, 09Ah, 0EEh, 0FEh, 0EEh
        db  040h, 050h, 006h, 000h, 000h, 060h, 02Eh, 0EFh, 06Ah, 0F0h, 000h, 040h, 08Eh, 0EEh, 09Ah, 0EEh
        db  0FEh, 0EEh, 040h, 010h, 006h, 000h, 000h, 020h, 02Eh, 0EFh, 06Ah, 0F0h, 000h, 000h, 05Eh, 0F0h
        db  054h, 0E9h, 08Eh, 0EEh, 09Ah, 0EEh, 0FEh, 0EEh, 040h, 0D0h, 006h, 000h, 000h, 0A6h, 02Eh, 0EFh
        db  06Ah, 0F0h, 000h, 0C0h, 08Eh, 0EEh, 09Ah, 0EEh, 0FEh, 0EEh, 040h, 090h, 006h, 000h, 000h, 0C0h
        db  02Eh, 0EFh, 06Ah, 0F0h, 000h, 080h, 08Eh, 0EEh, 09Ah, 0EEh, 0FEh, 0EEh, 040h, 050h, 006h, 000h
        db  000h, 040h, 02Eh, 0EFh, 06Ah, 0F0h, 000h, 040h, 08Eh, 0EEh, 09Ah, 0EEh, 0FEh, 0EEh, 040h, 070h
        db  006h, 000h, 000h, 060h, 02Eh, 0EFh, 06Ah, 0F0h, 000h, 060h, 00Ch, 0F1h, 052h, 0E9h, 0A6h, 0EEh
        db  024h, 0EAh, 07Ah, 0EEh, 092h, 0E9h, 032h, 0F0h, 055h, 055h, 016h, 000h, 0D4h, 0EEh, 020h, 0EFh
        db  000h, 000h, 000h, 000h, 044h, 0EFh, 04Dh, 0EAh, 098h, 0EFh, 0A6h, 0EEh, 024h, 0EAh, 0E6h, 0EFh
        db  055h, 055h, 016h, 000h, 08Eh, 0EEh, 0D4h, 0EEh, 02Eh, 0EFh, 0B8h, 0EEh, 0FFh, 000h, 000h, 008h
        db  0B8h, 0EEh, 0FFh, 000h, 000h, 00Ch, 0B8h, 0EEh, 0FFh, 000h, 000h, 010h, 0B8h, 0EEh, 0FFh, 000h
        db  000h, 014h, 0B8h, 0EEh, 0FFh, 000h, 000h, 018h, 0B8h, 0EEh, 0FFh, 000h, 000h, 01Ch, 0B8h, 0EEh
        db  0FFh, 000h, 000h, 020h, 0B8h, 0EEh, 0FFh, 000h, 000h, 024h, 0B8h, 0EEh, 0FFh, 000h, 000h, 028h
        db  0B8h, 0EEh, 0FFh, 000h, 000h, 02Ch, 060h, 0EFh, 08Eh, 0EEh, 09Ah, 0EEh, 0FEh, 0EEh, 040h, 030h
        db  006h, 000h, 000h, 001h, 02Eh, 0EFh, 06Ah, 0F0h, 000h, 020h, 08Eh, 0EEh, 09Ah, 0EEh, 0FEh, 0EEh
        db  040h, 070h, 006h, 000h, 000h, 043h, 02Eh, 0EFh, 06Ah, 0F0h, 000h, 060h, 08Eh, 0EEh, 09Ah, 0EEh
        db  0FEh, 0EEh, 040h, 0B0h, 006h, 000h, 000h, 085h, 02Eh, 0EFh, 06Ah, 0F0h, 000h, 0A0h, 066h, 0F0h
        db  000h, 001h, 003h, 001h, 001h, 001h, 000h, 000h, 000h, 081h, 01Ah, 000h, 040h, 010h, 055h, 055h
        db  055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 055h, 072h, 082h, 04Ah, 0A9h, 0A5h, 05Ah
        db  0DAh, 0E7h, 003h, 009h, 011h, 09Dh, 000h, 000h, 000h, 081h, 004h, 000h, 0D8h, 090h, 000h, 010h
        db  000h, 000h, 000h, 081h, 004h, 000h, 0D8h, 090h, 0D8h, 0B4h, 000h, 000h, 000h, 081h, 008h, 000h
        db  0D8h, 090h, 046h, 016h, 000h, 040h, 0D8h, 0B4h, 008h, 000h, 000h, 000h, 000h, 080h, 013h, 000h
        db  040h, 010h, 016h, 000h, 000h, 000h, 000h, 080h, 010h, 000h, 015h, 000h, 000h, 000h, 000h, 080h
        db  00Fh, 000h, 015h, 000h, 000h, 000h, 000h, 081h, 00Fh, 000h, 006h, 000h, 000h, 000h, 000h, 080h
        db  012h, 000h, 00Ah, 080h, 040h, 09Eh, 000h, 0C8h, 000h, 000h, 05Eh, 080h, 00Fh, 000h, 006h, 080h
        db  040h, 0FEh, 000h, 0CCh, 000h, 000h, 004h, 080h, 040h, 08Eh, 000h, 0C9h, 004h, 080h, 000h, 006h
        db  000h, 0CCh, 004h, 080h, 040h, 00Ah, 000h, 0C8h, 00Ah, 080h, 040h, 08Ah, 000h, 0C8h, 000h, 000h
        db  05Eh, 080h, 00Fh, 000h, 00Ah, 008h, 080h, 01Ch, 00Ah, 000h, 01Ch, 01Ah, 000h, 080h, 01Ch, 00Ch
        db  000h, 080h, 01Ch, 01Ah, 000h, 080h, 01Ah, 00Eh, 080h, 01Ch, 004h, 000h, 000h, 080h, 080h, 002h
        db  002h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  058h, 007h, 050h, 0C5h, 016h, 0E0h, 0E2h, 008h, 0ECh, 008h, 0F6h, 008h, 016h, 0E0h, 000h, 009h
        db  00Ah, 009h, 000h, 000h, 000h, 000h, 0E2h, 008h, 000h, 000h, 0ECh, 008h, 0F6h, 008h, 000h, 009h
        db  000h, 000h, 0B8h, 007h, 0D4h, 0D8h, 080h, 002h, 0B8h, 007h, 0F2h, 0D8h, 084h, 0FFh, 0B8h, 007h
        db  014h, 0D9h, 0B8h, 007h, 08Eh, 0D9h, 06Eh, 0DAh, 062h, 0DAh, 088h, 0DAh, 08Ch, 0DAh, 084h, 0DAh
        db  06Ch, 0DBh, 06Ch, 0DBh, 06Ch, 0DBh, 08Eh, 0DAh, 098h, 0DAh, 014h, 0DBh, 05Eh, 0DBh, 06Ch, 0DBh
        db  06Ch, 0DBh, 06Ch, 0DBh, 06Ch, 0DBh, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 008h, 000h
        db  008h, 001h, 005h, 008h, 008h, 008h, 000h, 008h, 002h, 000h, 004h, 004h, 004h, 004h, 008h, 000h
        db  000h, 00Ah, 00Ch, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 004h, 041h, 006h, 00Bh, 008h, 0C2h
        db  000h, 0E6h, 000h, 0E7h, 004h, 006h, 004h, 007h, 004h, 003h, 006h, 004h, 004h, 005h, 004h, 088h
        db  003h, 000h, 01Ch, 000h, 000h, 00Ch, 000h, 080h, 0CCh, 0E3h, 0D4h, 0E3h, 018h, 0E4h, 0D8h, 0E3h
        db  0E4h, 0E3h, 0EAh, 0E3h, 00Eh, 0E4h, 0DEh, 0E3h, 02Ch, 0E4h, 000h, 006h, 000h, 000h, 006h, 007h
        db  014h, 00Eh, 01Eh, 014h, 04Ch, 02Ah, 07Ch, 038h, 078h, 03Eh, 092h, 043h, 09Ch, 058h, 0B2h, 04Dh
        db  064h, 02Fh, 034h, 01Dh, 0D0h, 06Ch, 03Eh, 023h, 02Ah, 01Dh, 0C0h, 065h, 0E0h, 072h, 0EEh, 078h
        db  0FCh, 07Fh, 032h, 001h, 001h, 057h, 032h, 011h, 081h, 051h, 002h, 056h, 003h, 055h, 054h, 011h
        db  056h, 081h, 055h, 002h, 054h, 002h, 056h, 081h, 001h, 076h, 002h, 034h, 002h, 055h, 081h, 054h
        db  002h, 058h, 002h, 055h, 081h, 054h, 002h, 058h, 011h, 012h, 002h, 052h, 058h, 083h, 052h, 005h
        db  081h, 004h, 005h, 058h, 003h, 052h, 05Ch, 015h, 053h, 05Bh, 052h, 087h, 011h, 003h, 041h, 051h
        db  078h, 051h, 034h, 011h, 081h, 011h, 020h, 031h, 054h, 057h, 001h, 053h, 05Ah, 012h, 081h, 051h
        db  020h, 031h, 05Bh, 057h, 001h, 05Ah, 001h, 011h, 051h, 011h, 031h, 081h, 057h, 020h, 015h, 001h
        db  013h, 001h, 011h, 001h, 011h, 011h, 081h, 051h, 005h, 058h, 002h, 052h, 05Bh, 054h, 05Dh, 081h
        db  052h, 005h, 054h, 002h, 058h, 081h, 050h, 002h, 013h, 003h, 058h, 081h, 050h, 002h, 011h, 003h
        db  081h, 054h, 072h, 05Dh, 050h, 003h, 013h, 003h, 013h, 001h, 040h, 054h, 00Eh, 000h, 020h, 006h
        db  056h, 006h, 0FAh, 0E4h, 024h, 000h, 002h, 010h, 016h, 000h, 002h, 000h, 001h, 004h, 008h, 007h
        db  0FAh, 0E4h, 020h, 000h, 003h, 010h, 012h, 000h, 003h, 000h, 048h, 0E4h, 014h, 08Eh, 020h, 000h
        db  004h, 010h, 012h, 000h, 004h, 000h, 0A6h, 0DBh, 020h, 000h, 005h, 0E0h, 012h, 000h, 005h, 000h
        db  0A6h, 0DBh, 020h, 000h, 006h, 060h, 012h, 000h, 006h, 000h, 0F2h, 0E6h, 012h, 000h, 001h, 060h
        db  068h, 009h, 0CCh, 006h, 004h, 000h, 007h, 000h, 000h, 000h, 030h, 006h, 012h, 0E6h, 0F0h, 005h
        db  000h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 0E2h, 005h, 008h, 000h, 026h, 0FFh, 0DCh, 005h
        db  07Eh, 0E7h, 03Eh, 000h, 000h, 060h, 000h, 0C0h, 000h, 000h, 002h, 000h, 030h, 000h, 020h, 050h
        db  023h, 00Ch, 0FCh, 005h, 052h, 006h, 056h, 006h, 000h, 000h, 000h, 081h, 016h, 000h, 000h, 060h
        db  000h, 0C0h, 000h, 000h, 010h, 000h, 008h, 000h, 02Ah, 040h, 02Ah, 004h, 056h, 006h, 026h, 000h
        db  073h, 0ECh, 02Bh, 006h, 06Eh, 009h, 022h, 000h, 024h, 000h, 089h, 0ECh, 023h, 00Ch, 0FCh, 005h
        db  028h, 008h, 034h, 009h, 029h, 008h, 058h, 007h, 078h, 007h, 098h, 007h, 023h, 000h, 02Ah, 000h
        db  097h, 0ECh, 006h, 004h, 0F0h, 006h, 007h, 004h, 0EEh, 006h, 024h, 000h, 0A6h, 0DBh, 034h, 000h
        db  000h, 060h, 000h, 0C0h, 000h, 000h, 010h, 000h, 026h, 000h, 025h, 040h, 0A6h, 0DBh, 020h, 000h
        db  000h, 060h, 000h, 0C0h, 000h, 000h, 010h, 000h, 012h, 000h, 026h, 040h, 0A6h, 0DBh, 01Ah, 000h
        db  000h, 060h, 00Ch, 000h, 027h, 060h, 00Ah, 008h, 0E6h, 006h, 0A6h, 0DBh, 024h, 000h, 000h, 060h
        db  016h, 000h, 028h, 060h, 030h, 004h, 006h, 007h, 0FCh, 0DBh, 000h, 081h, 030h, 000h, 000h, 060h
        db  022h, 000h, 029h, 060h, 02Dh, 008h, 01Ch, 007h, 02Eh, 008h, 022h, 007h, 000h, 000h, 008h, 002h
        db  006h, 001h, 014h, 006h, 018h, 008h, 020h, 00Ch, 026h, 00Eh, 030h, 00Fh, 034h, 011h, 03Eh, 012h
        db  042h, 014h, 046h, 016h, 01Ch, 00Ah, 04Ah, 018h, 013h, 003h, 011h, 083h, 001h, 011h, 011h, 081h
        db  012h, 081h, 013h, 001h, 052h, 083h, 081h, 085h, 085h, 011h, 012h, 081h, 012h, 081h, 019h, 081h
        db  060h, 085h, 000h, 0C0h, 000h, 000h, 008h, 000h, 068h, 009h, 000h, 000h, 030h, 006h, 0CCh, 0ECh
        db  054h, 006h, 050h, 006h, 038h, 002h, 021h, 004h, 01Eh, 009h, 00Bh, 006h, 0D8h, 006h, 002h, 008h
        db  0DCh, 006h, 000h, 0C0h, 0FFh, 0FFh, 0FFh, 0FFh, 041h, 000h, 041h, 000h, 014h, 0AEh, 000h, 000h
        db  000h, 081h, 009h, 004h, 00Ch, 007h, 041h, 000h, 041h, 000h, 014h, 002h, 000h, 000h, 000h, 081h
        db  00Bh, 006h, 0D8h, 006h, 02Ch, 006h, 072h, 009h, 022h, 014h, 03Ah, 009h, 041h, 000h, 041h, 000h
        db  054h, 002h, 000h, 000h, 000h, 081h, 0D8h, 006h, 000h, 084h, 000h, 048h, 0FCh, 0FFh, 009h, 000h
        db  000h, 0C0h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0B8h, 000h, 040h, 043h, 028h, 031h, 029h, 038h, 039h
        db  02Dh, 033h, 039h, 038h, 039h, 02Ch, 02Ch, 030h, 02Ch, 031h, 020h, 032h, 065h, 054h, 061h, 078h
        db  020h, 073h, 06Eh, 049h, 074h, 073h, 075h, 072h, 065h, 06Dh, 074h, 06Eh, 00Ah, 073h, 043h, 028h
        db  031h, 029h, 038h, 039h, 02Ch, 033h, 02Ch, 034h, 02Ch, 035h, 020h, 036h, 042h, 049h, 020h, 04Dh
        db  06Fh, 043h, 070h, 072h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0F8h, 000h, 002h
        db  034h, 0A0h, 000h, 000h, 0FFh, 0FAh, 000h, 002h, 0B8h, 0FFh, 000h, 000h, 0FFh, 0FCh, 000h, 004h
        db  080h, 000h, 03Eh, 0B0h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_A6C8:
        pop    eax
        imul   esi, dword ptr [edx + 63h], 43206d6fh
        db  072h, 065h
        imul   esi, dword ptr fs:[ebx + eax*2 + 61h], 54206472h
        outsd
        imul   esp, dword ptr [ebp + 6eh], 20h
        push   edx
        imul   ebp, dword ptr [esi + 67h], 0
loc_1_A6E8:
        pop    eax
        imul   esi, dword ptr [edx + 63h], 43206d6fh
        push   esp
        and    byte ptr [ecx + 64h], al
        popad
        db  070h, 074h
        db  065h, 072h, 020h
        add    byte ptr [eax], al
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
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
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
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
        add    eax, 6020101h
        add    dword ptr [ecx], eax
        add    al, byte ptr [edi]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
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
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
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
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
        add    dword ptr [ecx], eax
        add    dword ptr [eax], eax
        add    al, byte ptr [ecx]
        add    dword ptr [eax], eax
        add    eax, dword ptr [ecx]
        add    dword ptr [eax], eax
        add    al, 1
        add    dword ptr [eax], eax
        add    eax, 6000101h
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
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
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
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
        add    eax, 6020101h
        add    dword ptr [ecx], eax
        add    al, byte ptr [edi]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx]
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
        add    byte ptr [ecx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [ebx], dl
        add    dword ptr [ecx], eax
        add    byte ptr [edx], dl
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
        or     eax, 10000101h
        add    dword ptr [ecx], eax
        add    byte ptr [esi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [edi], cl
        add    dword ptr [ecx], eax
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    al, byte ptr [edx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ebx]
        add    dword ptr [ecx], eax
        add    al, byte ptr [ecx + eax]
        add    dword ptr [edx], eax
loc_1_A8BC:
        add    byte ptr [eax], al
loc_1_A8BE:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
_LTEXT ends
        end
