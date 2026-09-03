; t20n3_lcode_01.asm -- T20N3 object 1 segment _LTEXT
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
        db  0E8h, 0E0h, 0FFh, 0FEh, 0FFh
        cmp    eax, 0
        jne    SHORT loc_1_0027
loc_1_0025:
        clc
        ret
loc_1_0027:
        stc
        ret
T20N3_Control:
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
        db  000h
sub_1_04E4:
        push   esi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep outsw
        pop    esi
        ret    0ch
        db  056h, 08Bh, 054h, 024h, 008h, 08Bh, 074h, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Fh
        db  05Eh, 0C2h, 00Ch, 000h, 057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h
        db  008h, 0FCh, 0F3h, 06Ch, 05Fh, 0C2h, 00Ch, 000h
sub_1_0521:
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep insw
        pop    edi
        ret    0ch
        db  057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch, 024h, 010h, 0FCh, 0F3h, 06Dh
        db  05Fh, 0C2h, 00Ch, 000h
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
        db  0CDh, 020h, 037h, 080h, 028h, 000h
sub_1_0574:
        db  0CDh, 020h, 038h, 080h, 028h, 000h
sub_1_057A:
        db  0CDh, 020h, 039h, 080h, 028h, 000h
        db  0CDh, 020h, 03Ah, 080h, 028h, 000h
sub_1_0586:
        db  0CDh, 020h, 03Bh, 080h, 028h, 000h
        db  0CDh, 020h, 03Ch, 080h, 028h, 000h
sub_1_0592:
        db  0CDh, 020h, 03Dh, 080h, 028h, 000h
sub_1_0598:
        db  0CDh, 020h, 03Eh, 080h, 028h, 000h
sub_1_059E:
        db  0CDh, 020h, 03Fh, 080h, 028h, 000h
sub_1_05A4:
        db  0CDh, 020h, 040h, 080h, 028h, 000h
        db  0CDh, 020h, 041h, 080h, 028h, 000h
        db  0CDh, 020h, 009h, 080h, 028h, 000h
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_05BC:
        db  0CDh, 020h, 044h, 080h, 028h, 000h
sub_1_05C2:
        db  0CDh, 020h, 048h, 080h, 028h, 000h
sub_1_05C8:
        db  0CDh, 020h, 049h, 080h, 028h, 000h
        db  0CDh, 020h, 04Fh, 080h, 028h, 000h
        db  0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_05DA:
        db  0CDh, 020h, 00Ch, 080h, 028h, 000h
sub_1_05E0:
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
sub_1_068E:
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
        db  0CDh, 020h, 046h, 080h, 028h, 000h
sub_1_06D0:
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
        db  000h, 000h, 000h, 000h, 00Ah, 003h, 000h, 000h, 003h, 00Ah, 000h, 000h, 054h, 032h, 030h, 04Eh
        db  033h, 020h, 020h, 020h, 000h, 040h, 000h, 054h, 029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_07A0:
        push   esp
        xor    dh, byte ptr [eax]
        dec    esi
        xor    eax, dword ptr [eax]
        add    byte ptr [eax], al
        dec    ecx
        dec    edi
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        db  000h
loc_1_07B4:
        dec    ecx
        dec    edi
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        inc    ebp
        push   ebx
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4eh], cl
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ecx + 4eh], cl
        push   esp
        inc    ebp
        push   edx
        push   edx
        push   ebp
        push   eax
        push   esp
        add    byte ptr [eax], al
        add    byte ptr [ecx + ecx*2 + 4dh], dl
        dec    ecx
        dec    esi
        inc    edi
        add    byte ptr [eax], al
loc_1_07E0:
        push   esp
        dec    ecx
        dec    ebp
        dec    ecx
        dec    esi
        inc    edi
        add    byte ptr [eax], al
        inc    esp
        dec    ebp
        inc    ecx
        inc    ebx
        dec    eax
        inc    ecx
        dec    esi
        dec    esi
        inc    ebp
        dec    esp
        add    byte ptr [eax], al
loc_1_07F4:
        inc    esp
        dec    ebp
        inc    ecx
        inc    ebx
        dec    eax
        inc    ecx
        dec    esi
        dec    esi
        inc    ebp
        dec    esp
        add    byte ptr [eax], al
        inc    edx
        inc    ecx
        dec    esi
        inc    esp
        push   edi
        dec    ecx
        inc    esp
        push   esp
        dec    eax
        add    byte ptr [eax], al
        add    byte ptr [edx + 41h], al
        dec    esi
        inc    esp
        push   edi
        dec    ecx
        inc    esp
        push   esp
        dec    eax
        add    byte ptr [eax], al
        add    byte ptr [ebp + 45h], cl
        inc    esp
        dec    ecx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [ebp + 45h], cl
        inc    esp
        dec    ecx
        inc    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax + 52h], dl
        dec    ecx
        add    byte ptr [eax + 52h], dl
        dec    ecx
        add    byte ptr [ebx + 45h], dl
        inc    ebx
        db  000h
loc_1_0834:
        push   ebx
        inc    ebp
        inc    ebx
        add    byte ptr [ebp + eax*2 + 46h], al
        db  000h
loc_1_083C:
        inc    esp
        inc    ebp
        inc    esi
        add    byte ptr [ecx + 4ch], al
        push   esp
        add    byte ptr [ecx + 4ch], al
        push   esp
        add    byte ptr [eax + 61h], dl
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_08C2
        imul   edi, dword ptr [edx + 65h], 61500000h
        arpl   word ptr [ebx + 65h], bp
        je     SHORT loc_1_08CE
        imul   edi, dword ptr [edx + 65h], 0
        add    byte ptr [eax + 54682000h], dh
        db  073h, 069h
        arpl   word ptr [eax], sp
        outsd
        and    byte ptr [ebp + 73h], ah
        imul   esi, dword ptr [edx + 20h], 6561656ch
        jae    NEAR PTR loc_1_089B
        outsb
        db  075h, 065h
        and    byte ptr fs:[edx + 69h], dh
        insb
        arpl   word ptr gs:[ebx + 6eh], sp
        and    byte ptr [ebp + 6eh], ah
        dec    edi
        jns    SHORT loc_1_08FA
        and    byte ptr [esp + ebp*2], ch
        inc    ecx
        and    byte ptr [ecx + ebp*2 + 72h], ch
        push   72747367h
loc_1_089B:
        and    byte ptr [ebx + 65h], dh
        jb     SHORT loc_1_0905
        jbe    NEAR PTR loc_1_08D1
        pushad
        add    dword ptr [edx + 1], eax
        add    byte ptr [eax], cl
        or     byte ptr [esi], dl
        mov    al, 3
        loopne loc_1_08B4
        add    byte ptr [eax], al
        and    byte ptr [ecx], al
loc_1_08B4:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FCh, 013h, 080h, 003h, 0A0h, 007h, 042h, 001h, 000h, 008h
loc_1_08C2:
        and    byte ptr [edi], al
        add    byte ptr [eax], al
        db  0E0h, 004h
        add    byte ptr [ecx], al
        mov    eax, dword ptr [edi]
        db  000h, 03Dh
loc_1_08CE:
        pushad
        db  001h, 042h
loc_1_08D1:
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
loc_1_08FA:
        sub    ah, al
        inc    edx
        add    eax, esp
        add    al, 2
        add    eax, esp
        add    al, 12h
loc_1_0905:
        or     eax, esp
        add    al, 82h
        add    dword ptr [eax + 1], esp
        sbb    al, 1
        add    al, 0
        add    edx, dword ptr [esi]
        loopne loc_1_0915
        inc    eax
loc_1_0915:
        add    dword ptr [eax], eax
        or     al, 0a0h
        push   es
        scasb
        mov    cl, 0a0h
        pop    es
        add    al, 1
        sub    eax, 0c22000h
        db  0E0h, 088h
        add    ah, byte ptr [ebx]
        loop   loc_1_093F
        push   ss
        mov    al, byte ptr [2e010407h]
        add    byte ptr [eax + 1], ah
        inc    edx
        add    dword ptr [eax], eax
        add    ecx, dword ptr [407a016h]
        add    dword ptr [ecx], esp
loc_1_093F:
        add    byte ptr [eax - 764ffff9h], cl
        pop    es
        db  0FEh, 0FFh, 0A8h, 009h, 0A9h, 009h, 08Ah, 007h, 002h, 0E0h, 0A0h, 006h, 098h, 0F5h, 056h, 010h
        db  088h, 007h, 000h, 0A0h, 089h, 007h, 0FEh, 0AFh, 0A8h, 009h, 0A9h, 009h, 08Ah, 007h, 07Ah, 0E0h
        db  0A0h, 006h, 098h, 0F5h, 04Bh, 010h, 0A0h, 005h, 004h, 001h, 088h, 007h, 008h, 000h, 089h, 007h
        db  07Ah, 000h, 000h, 003h, 001h, 000h, 0A0h, 006h, 0D0h, 0F0h, 040h, 010h, 0A0h, 006h, 0AEh, 0B1h
        db  0E0h, 002h, 0F4h, 003h, 088h, 007h, 0A0h, 000h, 089h, 007h, 0FEh, 000h, 0A0h, 006h, 0D0h, 0F0h
        db  035h, 010h, 0E0h, 002h, 0A0h, 000h, 0E0h, 004h, 07Eh, 001h, 0C8h, 004h, 009h, 002h, 0F2h, 003h
        db  048h, 062h, 0E0h, 0C1h, 040h, 001h, 0A0h, 001h, 040h, 001h, 000h, 004h, 018h, 0CEh, 009h, 006h
        db  0FDh, 016h, 0A0h, 001h, 040h, 001h, 000h, 040h, 007h, 0C8h, 040h, 001h, 088h, 007h, 0F4h, 003h
        db  089h, 007h, 0FEh, 03Fh, 0A0h, 006h, 0D0h, 0F0h, 019h, 010h, 0E0h, 002h, 0A0h, 000h, 0A0h, 006h
        db  0F8h, 0F1h, 014h, 010h, 008h, 0C8h, 044h, 004h, 009h, 0C8h, 046h, 004h, 0A0h, 006h, 026h, 0F1h
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
        db  004h, 016h, 0A0h, 006h, 010h, 0C2h, 0E0h, 004h, 0D2h, 008h, 086h, 007h, 0E8h, 005h, 0A0h, 001h
        db  040h, 001h, 000h, 080h, 020h, 0C2h, 084h, 001h, 020h, 048h, 008h, 0E0h, 084h, 001h, 020h, 022h
        db  008h, 0E0h, 008h, 013h, 060h, 001h, 0AEh, 001h, 001h, 000h, 004h, 016h, 0E0h, 001h, 034h, 006h
        db  000h, 080h, 006h, 010h, 020h, 0C2h, 0E4h, 008h, 006h, 013h, 0E0h, 001h, 034h, 006h, 000h, 008h
        db  0E0h, 004h, 030h, 006h, 009h, 007h, 0A0h, 005h, 0EEh, 005h, 020h, 006h, 0ECh, 005h, 002h, 016h
        db  016h, 0C2h, 003h, 016h, 049h, 0C2h, 012h, 016h, 080h, 003h, 098h, 0C5h, 0E8h, 0C1h, 002h, 000h
        db  0E0h, 0E9h, 014h, 0E0h, 004h, 000h, 0D7h, 004h, 027h, 002h, 008h, 000h, 0A0h, 006h, 0A6h, 0C1h
        db  016h, 0C2h, 004h, 013h, 028h, 0C8h, 008h, 000h, 0ECh, 005h, 0EFh, 013h, 054h, 004h, 000h, 003h
        db  002h, 000h, 0E0h, 0C1h, 086h, 001h, 047h, 002h, 00Eh, 000h, 0A7h, 0C2h, 092h, 0E1h, 05Ah, 004h
        db  08Ah, 007h, 000h, 0A0h, 00Ah, 0C8h, 086h, 001h, 0C7h, 0A1h, 027h, 002h, 09Ah, 0E1h, 037h, 0E8h
        db  034h, 006h, 017h, 0E8h, 0D2h, 006h, 0E0h, 004h, 030h, 006h, 060h, 004h, 050h, 0B9h, 00Ah, 0E8h
        db  0D2h, 006h, 0E0h, 001h, 034h, 006h, 000h, 008h, 0E0h, 004h, 030h, 006h, 020h, 0E0h, 018h, 0E0h
        db  05Bh, 004h, 0A0h, 005h, 0D2h, 008h, 020h, 088h, 0D2h, 008h, 016h, 0E0h, 0E5h, 01Ah, 0E0h, 004h
        db  0D2h, 008h, 0A0h, 006h, 012h, 0E0h, 080h, 003h, 0A0h, 005h, 0E4h, 008h, 080h, 003h, 001h, 0C3h
        db  0FBh, 013h, 060h, 001h, 018h, 009h, 001h, 000h, 002h, 013h, 0A0h, 005h, 0E4h, 008h, 000h, 003h
        db  002h, 000h, 0E0h, 0C0h, 06Ch, 001h, 020h, 0C3h, 08Ah, 001h, 020h, 098h, 083h, 001h, 002h, 0E2h
        db  03Ch, 016h, 041h, 0C0h, 006h, 013h, 001h, 0C8h, 06Ch, 001h, 0E0h, 0C2h, 002h, 0FCh, 001h, 011h
        db  070h, 010h, 060h, 001h, 09Ch, 001h, 040h, 000h, 06Eh, 016h, 020h, 0D8h, 0E1h, 008h, 083h, 001h
        db  041h, 0C0h, 004h, 013h, 001h, 0C8h, 08Ah, 001h, 001h, 0C8h, 0CAh, 008h, 086h, 007h, 043h, 000h
        db  006h, 0C8h, 06Ch, 001h, 007h, 002h, 002h, 0FCh, 017h, 0C2h, 060h, 004h, 0DEh, 0B6h, 0E0h, 004h
        db  0CAh, 008h, 0C7h, 061h, 008h, 007h, 060h, 001h, 006h, 0FCh, 040h, 000h, 002h, 013h, 008h, 002h
        db  001h, 000h, 009h, 010h, 04Ch, 0C2h, 020h, 0C3h, 000h, 0FCh, 02Eh, 013h, 00Ch, 0C8h, 06Ch, 001h
        db  0E0h, 0C2h, 002h, 0FCh, 01Fh, 011h, 04Bh, 001h, 000h, 001h, 0F4h, 016h, 0C8h, 022h, 016h, 013h
        db  0CBh, 001h, 000h, 040h, 00Bh, 0C8h, 002h, 0FCh, 011h, 010h, 060h, 001h, 09Ch, 001h, 040h, 000h
        db  038h, 016h, 0E0h, 0C1h, 0CAh, 008h, 001h, 0C3h, 021h, 013h, 04Ch, 0C2h, 015h, 013h, 00Ch, 0C8h
        db  06Ch, 001h, 0E0h, 0C2h, 002h, 0FCh, 006h, 011h, 0CCh, 081h, 0D1h, 013h, 04Ch, 0C2h, 020h, 0C3h
        db  000h, 0FCh, 0F4h, 010h, 009h, 0C8h, 06Ch, 001h, 0E0h, 0C2h, 002h, 0FCh, 01Eh, 016h, 0A0h, 007h
        db  002h, 0FCh, 000h, 080h, 009h, 0C3h, 019h, 010h, 009h, 0C8h, 06Ch, 001h, 0E0h, 0C2h, 002h, 0FCh
        db  005h, 016h, 0A0h, 007h, 002h, 0FCh, 000h, 080h, 009h, 0C3h, 00Fh, 010h, 0E0h, 0C2h, 002h, 00Ch
        db  001h, 011h, 00Fh, 010h, 020h, 0D8h, 002h, 0E2h, 083h, 001h, 08Bh, 009h, 08Bh, 009h, 08Bh, 009h
        db  08Bh, 009h, 0A0h, 007h, 08Ah, 001h, 043h, 000h, 004h, 010h, 00Ch, 0C8h, 08Ah, 001h, 00Ch, 0C8h
        db  0CAh, 008h, 001h, 083h, 02Fh, 016h, 003h, 0C8h, 06Ch, 001h, 040h, 001h, 010h, 000h, 017h, 016h
        db  0E0h, 0C2h, 02Eh, 006h, 003h, 013h, 0E0h, 0C2h, 0F8h, 005h, 003h, 016h, 0E0h, 0C2h, 014h, 009h
        db  00Eh, 013h, 0E0h, 001h, 03Ah, 007h, 000h, 080h, 080h, 001h, 010h, 000h, 0E0h, 0C2h, 036h, 007h
        db  006h, 013h, 0E0h, 004h, 036h, 007h, 080h, 001h, 020h, 000h, 060h, 004h, 050h, 0B9h, 040h, 001h
        db  020h, 000h, 0F9h, 013h, 090h, 003h, 0FFh, 011h, 080h, 003h, 008h, 001h, 000h, 004h, 014h, 016h
        db  060h, 001h, 018h, 009h, 001h, 000h, 010h, 016h, 088h, 001h, 000h, 01Ah, 0C8h, 001h, 000h, 001h
        db  0C8h, 0C5h, 00Ah, 010h, 081h, 0C1h, 001h, 0C8h, 06Ch, 001h, 007h, 002h, 000h, 0FCh, 077h, 0C0h
        db  017h, 0C2h, 048h, 001h, 000h, 018h, 0E9h, 013h, 040h, 001h, 040h, 000h, 01Bh, 016h, 080h, 001h
        db  045h, 000h, 046h, 0C1h, 020h, 0D0h, 007h, 0FCh, 020h, 028h, 006h, 0E0h, 060h, 081h, 0CAh, 008h
        db  004h, 016h, 0E0h, 004h, 0CAh, 008h, 0C0h, 001h, 004h, 000h, 0E0h, 0C2h, 008h, 0FCh, 008h, 011h
        db  0E0h, 0C2h, 00Eh, 0FCh, 007h, 015h, 006h, 013h, 0E0h, 0C2h, 014h, 0FCh, 003h, 015h, 002h, 013h
        db  0C0h, 001h, 001h, 000h, 048h, 001h, 000h, 001h, 005h, 013h, 000h, 001h, 080h, 040h, 06Dh, 013h
        db  060h, 004h, 066h, 0B5h, 0C0h, 001h, 040h, 000h, 040h, 001h, 004h, 000h, 00Bh, 016h, 048h, 001h
        db  001h, 000h, 003h, 016h, 040h, 001h, 000h, 040h, 005h, 013h, 0C8h, 001h, 000h, 040h, 0A0h, 005h
        db  0E4h, 008h, 0C8h, 0C5h, 0B7h, 001h, 020h, 000h, 0D7h, 0C2h, 0C4h, 062h, 00Bh, 005h, 02Bh, 002h
        db  0FCh, 0FFh, 0CBh, 0C5h, 002h, 015h, 046h, 081h, 063h, 013h, 008h, 001h, 000h, 05Eh, 060h, 016h
        db  008h, 001h, 088h, 000h, 00Dh, 013h, 040h, 001h, 001h, 000h, 00Ah, 013h, 008h, 001h, 002h, 000h
        db  007h, 016h, 048h, 001h, 001h, 000h, 004h, 016h, 08Ah, 007h, 080h, 000h, 0A0h, 006h, 01Eh, 0B5h
        db  086h, 002h, 043h, 000h, 010h, 016h, 040h, 001h, 000h, 040h, 00Bh, 016h, 008h, 001h, 003h, 000h
        db  008h, 013h, 084h, 0C2h, 02Ah, 002h, 0D8h, 0FFh, 006h, 0C8h, 06Ch, 001h, 00Ah, 068h, 004h, 0FCh
        db  05Fh, 010h, 060h, 004h, 0B2h, 0B8h, 000h, 0C0h, 0FCh, 011h, 060h, 0C2h, 018h, 009h, 040h, 001h
        db  000h, 040h, 00Ah, 016h, 048h, 001h, 001h, 000h, 035h, 013h, 048h, 001h, 002h, 000h, 00Ah, 013h
        db  049h, 001h, 004h, 000h, 0EEh, 016h, 006h, 010h, 049h, 001h, 002h, 000h, 003h, 013h, 008h, 001h
        db  003h, 000h, 06Fh, 013h, 049h, 001h, 001h, 000h, 00Dh, 016h, 006h, 0C8h, 06Ch, 001h, 007h, 002h
        db  002h, 0FCh, 0E0h, 0A1h, 0DEh, 008h, 0E0h, 0CDh, 0EEh, 005h, 0E0h, 0C5h, 004h, 0FCh, 020h, 0C8h
        db  0DEh, 008h, 004h, 0FCh, 000h, 001h, 080h, 040h, 001h, 016h, 046h, 0C1h, 0E0h, 004h, 000h, 0FCh
        db  087h, 007h, 0F8h, 005h, 017h, 0C2h, 008h, 013h, 0C7h, 005h, 017h, 0C8h, 06Ch, 001h, 005h, 0C8h
        db  000h, 0FCh, 0C6h, 0C5h, 060h, 004h, 066h, 0B5h, 0C5h, 0CDh, 0C6h, 0C5h, 060h, 004h, 066h, 0B5h
        db  060h, 004h, 096h, 0B8h, 006h, 0C8h, 06Ch, 001h, 085h, 081h, 01Ah, 013h, 0E0h, 0C2h, 004h, 0FCh
        db  017h, 015h, 086h, 0C2h, 08Ah, 0A2h, 0AAh, 0C1h, 032h, 00Ch, 006h, 0C8h, 06Ch, 001h, 00Bh, 0A8h
        db  004h, 0FCh, 01Ah, 009h, 00Ah, 0C8h, 06Ch, 001h, 0E0h, 0C2h, 002h, 0FCh, 0E0h, 004h, 000h, 0FCh
        db  006h, 0C8h, 06Ch, 001h, 00Bh, 0C8h, 002h, 0FCh, 0A0h, 006h, 012h, 0C1h, 006h, 0C8h, 06Ch, 001h
        db  0E0h, 004h, 000h, 0FCh, 0A0h, 001h, 002h, 0FCh, 002h, 000h, 087h, 007h, 030h, 006h, 0E7h, 001h
        db  004h, 000h, 040h, 000h, 0D7h, 004h, 027h, 002h, 00Ch, 000h, 005h, 0C2h, 060h, 001h, 018h, 009h
        db  004h, 000h, 003h, 016h, 0E0h, 001h, 002h, 0FCh, 020h, 000h, 0A0h, 006h, 0BCh, 0C1h, 0C0h, 001h
        db  020h, 000h, 060h, 004h, 066h, 0B5h, 048h, 001h, 000h, 018h, 003h, 013h, 048h, 001h, 000h, 010h
        db  002h, 016h, 0A0h, 005h, 0E4h, 008h, 086h, 002h, 043h, 000h, 003h, 013h, 000h, 001h, 080h, 040h
        db  0A4h, 013h, 006h, 0C8h, 06Ch, 001h, 0E0h, 004h, 000h, 0FCh, 085h, 0C2h, 0A0h, 006h, 012h, 0C1h
        db  0E8h, 010h, 000h, 003h, 002h, 000h, 020h, 0C2h, 08Ch, 001h, 0E0h, 0C0h, 06Ch, 001h, 020h, 0C2h
        db  0F4h, 005h, 028h, 0C8h, 008h, 000h, 06Ch, 001h, 0E8h, 0C1h, 00Ah, 000h, 020h, 0C3h, 002h, 0FCh
        db  08Ch, 001h, 020h, 000h, 00Ch, 0C8h, 002h, 0FCh, 00Ch, 001h, 000h, 0FEh, 03Fh, 016h, 047h, 001h
        db  040h, 000h, 054h, 013h, 0E0h, 004h, 0F4h, 005h, 00Ch, 0CAh, 008h, 000h, 047h, 001h, 008h, 000h
        db  006h, 016h, 0A8h, 0C2h, 006h, 000h, 0A0h, 006h, 012h, 0C1h, 0E8h, 004h, 006h, 000h, 007h, 001h
        db  020h, 000h, 029h, 016h, 0E8h, 0C1h, 002h, 000h, 0E7h, 001h, 004h, 000h, 020h, 000h, 0D7h, 0C2h
        db  003h, 013h, 0D7h, 004h, 0C3h, 001h, 000h, 080h, 027h, 002h, 010h, 000h, 0A0h, 006h, 0A6h, 0C1h
        db  020h, 0C2h, 0F0h, 005h, 007h, 013h, 0E0h, 0C2h, 0F6h, 005h, 004h, 016h, 018h, 0C8h, 0F0h, 005h
        db  0A0h, 006h, 0BCh, 0BBh, 003h, 0C8h, 06Ch, 001h, 003h, 011h, 090h, 003h, 0FFh, 011h, 080h, 003h
        db  090h, 003h, 0F8h, 011h, 0E0h, 002h, 0C0h, 000h, 060h, 0C3h, 0FAh, 000h, 0A0h, 0C3h, 0FCh, 000h
        db  0E0h, 0C3h, 0FEh, 000h, 054h, 004h, 0E8h, 004h, 002h, 000h, 0E2h, 010h, 0E8h, 0C2h, 008h, 000h
        db  0A8h, 0C2h, 006h, 000h, 00Ch, 0C3h, 033h, 011h, 020h, 023h, 00Ah, 0E0h, 045h, 013h, 020h, 023h
        db  010h, 0E0h, 046h, 013h, 020h, 023h, 00Eh, 0E0h, 013h, 013h, 0E0h, 021h, 016h, 0E0h, 0B2h, 016h
        db  020h, 023h, 006h, 0E0h, 003h, 016h, 020h, 027h, 06Ah, 0E4h, 00Ah, 013h, 0E8h, 0C2h, 008h, 000h
        db  0A8h, 0C2h, 006h, 000h, 04Ch, 001h, 088h, 000h, 0A5h, 016h, 00Ch, 001h, 044h, 000h, 0A2h, 016h
        db  020h, 006h, 0C8h, 008h, 09Fh, 013h, 00Ah, 0C8h, 06Ch, 001h, 020h, 0C8h, 004h, 0E0h, 002h, 0FCh
        db  00Bh, 0C8h, 06Ch, 001h, 0A0h, 007h, 002h, 0FCh, 000h, 081h, 020h, 0C3h, 080h, 001h, 0A0h, 001h
        db  080h, 001h, 000h, 0C4h, 00Ch, 0C8h, 080h, 001h, 00Ah, 0C8h, 08Ch, 001h, 0B3h, 010h, 00Ah, 0C2h
        db  00Fh, 013h, 008h, 0C8h, 06Ch, 001h, 0A0h, 0C2h, 000h, 0FCh, 020h, 0C3h, 002h, 0FCh, 020h, 023h
        db  012h, 0E0h, 0F5h, 016h, 00Bh, 0C8h, 06Ch, 001h, 00Ch, 0C8h, 002h, 0FCh, 060h, 004h, 0CEh, 0B8h
        db  08Ah, 007h, 000h, 004h, 060h, 004h, 07Ch, 0B3h, 08Ah, 007h, 020h, 000h, 060h, 004h, 07Ch, 0B3h
        db  08Ah, 007h, 000h, 002h, 020h, 027h, 00Eh, 0E0h, 002h, 016h, 0A0h, 006h, 01Eh, 0B5h, 0A8h, 0C2h
        db  006h, 000h, 060h, 004h, 0F4h, 0B8h, 000h, 003h, 002h, 000h, 0C0h, 001h, 010h, 000h, 0E0h, 0C2h
        db  02Eh, 006h, 003h, 013h, 0E0h, 0C2h, 0F8h, 005h, 003h, 016h, 0E0h, 0C2h, 014h, 009h, 005h, 013h
        db  0E0h, 001h, 03Ah, 007h, 000h, 080h, 080h, 001h, 010h, 000h, 090h, 003h, 0FFh, 07Fh, 080h, 003h
        db  000h, 003h, 002h, 000h, 020h, 0C2h, 0F6h, 005h, 020h, 0E2h, 0F4h, 005h, 02Fh, 016h, 020h, 0D8h
        db  0E0h, 008h, 080h, 001h, 02Bh, 0C8h, 006h, 000h, 08Ch, 001h, 0A0h, 007h, 0C8h, 008h, 004h, 000h
        db  00Bh, 0C8h, 0F4h, 005h, 090h, 003h, 0FFh, 0FFh, 080h, 003h, 000h, 003h, 002h, 000h, 00Bh, 0C2h
        db  020h, 0C3h, 0F4h, 005h, 00Fh, 013h, 0A8h, 0C2h, 00Ah, 000h, 04Ah, 001h, 010h, 000h, 017h, 016h
        db  0A0h, 022h, 004h, 0E0h, 01Bh, 016h, 008h, 0C3h, 0A0h, 006h, 07Ah, 0BBh, 00Ch, 0C2h, 020h, 0C3h
        db  0F4h, 005h, 014h, 016h, 068h, 001h, 00Ah, 000h, 010h, 000h, 003h, 013h, 0E0h, 0C2h, 0F6h, 005h
        db  006h, 016h, 0A0h, 006h, 0BCh, 0BBh, 090h, 003h, 0FFh, 0FFh, 080h, 003h, 00Bh, 0C2h, 087h, 007h
        db  0F0h, 005h, 0A0h, 006h, 0A6h, 0C1h, 090h, 003h, 0FFh, 0FFh, 080h, 003h, 087h, 007h, 0F0h, 005h
        db  0A0h, 006h, 0ECh, 0C1h, 080h, 003h, 000h, 003h, 002h, 000h, 087h, 007h, 0F0h, 005h, 0CBh, 0C2h
        db  008h, 016h, 0A0h, 006h, 07Ah, 0BBh, 020h, 007h, 0F6h, 005h, 060h, 0CBh, 0F4h, 005h, 002h, 000h
        db  080h, 003h, 0E0h, 004h, 0F6h, 005h, 020h, 0C2h, 0F4h, 005h, 005h, 016h, 017h, 0C2h, 003h, 013h
        db  0D8h, 0C5h, 0A0h, 006h, 0BCh, 0BBh, 080h, 003h, 000h, 003h, 002h, 000h, 08Bh, 0C2h, 0A0h, 006h
        db  07Ah, 0BBh, 0CAh, 0C1h, 027h, 002h, 010h, 000h, 088h, 007h, 0F0h, 005h, 088h, 0C1h, 018h, 0C2h
        db  026h, 013h, 0A8h, 082h, 002h, 000h, 0FAh, 016h, 0E8h, 0C2h, 00Ah, 000h, 0E0h, 022h, 01Eh, 0E0h
        db  0F5h, 016h, 098h, 0C5h, 0E0h, 022h, 01Ch, 0E0h, 00Bh, 016h, 028h, 0C8h, 006h, 000h, 0F4h, 000h
        db  0E0h, 002h, 0E0h, 000h, 0A0h, 006h, 012h, 0C1h, 0E0h, 002h, 0C0h, 000h, 0E8h, 004h, 006h, 000h
        db  0E0h, 022h, 018h, 0E0h, 0E4h, 013h, 020h, 0EAh, 022h, 0E0h, 00Ah, 000h, 0A0h, 0EAh, 018h, 0E0h
        db  004h, 000h, 0DAh, 004h, 0A0h, 006h, 0A6h, 0C1h, 047h, 006h, 006h, 0C2h, 0D8h, 010h, 006h, 0C8h
        db  0F2h, 005h, 060h, 0CBh, 0F4h, 005h, 002h, 000h, 054h, 004h, 020h, 0C2h, 0F4h, 005h, 013h, 013h
        db  0E0h, 001h, 09Ch, 001h, 000h, 040h, 08Bh, 00Bh, 08Bh, 00Bh, 060h, 001h, 09Ch, 001h, 000h, 040h
        db  00Ah, 016h, 060h, 0C2h, 06Ch, 001h, 028h, 0C8h, 006h, 000h, 06Ch, 001h, 0A0h, 0C2h, 002h, 0FCh
        db  003h, 011h, 009h, 0C8h, 06Ch, 001h, 05Bh, 004h, 009h, 0C8h, 06Ch, 001h, 04Bh, 0C2h, 087h, 007h
        db  0F0h, 005h, 0A0h, 006h, 0ECh, 0C1h, 0E0h, 004h, 0F4h, 005h, 059h, 004h, 0A8h, 0C2h, 00Ah, 000h
        db  00Dh, 011h, 0A0h, 0F2h, 0E0h, 008h, 00Ah, 0D8h, 080h, 001h, 028h, 0C8h, 006h, 000h, 08Ch, 001h
        db  0A0h, 007h, 0C8h, 008h, 004h, 000h, 008h, 0C8h, 0F4h, 005h, 05Bh, 004h, 020h, 0C3h, 06Ch, 001h
        db  028h, 0C8h, 006h, 000h, 06Ch, 001h, 020h, 0C8h, 00Eh, 0FCh, 0BCh, 001h, 020h, 0C8h, 010h, 0FCh
        db  0B0h, 001h, 020h, 0C8h, 012h, 0FCh, 0B2h, 001h, 00Ch, 0C8h, 06Ch, 001h, 0A0h, 0F2h, 0E0h, 008h
        db  08Ah, 001h, 000h, 010h, 0A0h, 001h, 080h, 001h, 000h, 0C4h, 0DDh, 010h, 033h, 007h, 033h, 007h
        db  00Ch, 010h, 013h, 007h, 023h, 007h, 002h, 000h, 0CBh, 0C8h, 006h, 000h, 023h, 002h, 018h, 000h
        db  0E0h, 0CCh, 06Ch, 001h, 0CDh, 0CCh, 0CEh, 0CCh, 0CFh, 0CCh, 083h, 007h, 030h, 006h, 0D3h, 0C1h
        db  00Ah, 013h, 083h, 007h, 036h, 007h, 0D3h, 0C1h, 006h, 013h, 083h, 007h, 0A0h, 000h, 093h, 000h
        db  00Ch, 0C8h, 06Ch, 001h, 080h, 003h, 063h, 007h, 002h, 000h, 039h, 015h, 063h, 0C2h, 004h, 000h
        db  063h, 042h, 006h, 000h, 0DBh, 013h, 063h, 0C3h, 01Ah, 000h, 049h, 0D2h, 00Ch, 013h, 0C9h, 006h
        db  049h, 072h, 069h, 0D2h, 0C2h, 0E1h, 0C9h, 006h, 049h, 072h, 0E9h, 048h, 004h, 0E0h, 004h, 000h
        db  049h, 0CBh, 002h, 000h, 052h, 004h, 069h, 0D2h, 0C2h, 0E1h, 0C9h, 006h, 049h, 072h, 029h, 002h
        db  010h, 000h, 0C3h, 0C2h, 0E9h, 0A2h, 0AAh, 0E1h, 01Bh, 0C3h, 089h, 002h, 012h, 000h, 00Dh, 016h
        db  00Ch, 0C8h, 06Ch, 001h, 0E0h, 0C6h, 000h, 0FCh, 00Dh, 016h, 0E9h, 048h, 004h, 0E0h, 004h, 000h
        db  049h, 0CBh, 002h, 000h, 04Ch, 0CBh, 004h, 000h, 052h, 004h, 0DCh, 0C6h, 003h, 016h, 0E9h, 048h
        db  004h, 0E0h, 004h, 000h, 049h, 0CBh, 002h, 000h, 04Ch, 0CBh, 004h, 000h, 052h, 004h, 090h, 003h
        db  0FFh, 001h, 093h, 000h, 00Ch, 0C8h, 06Ch, 001h, 080h, 003h, 000h, 003h, 002h, 000h, 0DBh, 0C2h
        db  063h, 0C2h, 004h, 000h, 04Bh, 042h, 09Dh, 013h, 049h, 0D2h, 00Eh, 013h, 0C9h, 006h, 049h, 072h
        db  069h, 0D2h, 0C2h, 0E1h, 0C9h, 006h, 049h, 072h, 0E9h, 048h, 004h, 0E0h, 004h, 000h, 049h, 0CBh
        db  002h, 000h, 090h, 003h, 0FFh, 0FFh, 080h, 003h, 069h, 0D2h, 0C2h, 0E1h, 0C9h, 006h, 049h, 072h
        db  029h, 002h, 010h, 000h, 0C3h, 0C2h, 0E9h, 0A2h, 0AAh, 0E1h, 01Bh, 0C3h, 089h, 002h, 012h, 000h
        db  00Fh, 016h, 00Ch, 0C8h, 06Ch, 001h, 0E0h, 0C6h, 000h, 0FCh, 003h, 016h, 0E9h, 048h, 004h, 0E0h
        db  004h, 000h, 049h, 0CBh, 002h, 000h, 04Ch, 0CBh, 004h, 000h, 090h, 003h, 0FFh, 0FFh, 080h, 003h
        db  0DCh, 0C6h, 0F4h, 013h, 0F6h, 010h, 000h, 003h, 002h, 000h, 0BBh, 0C2h, 0BBh, 0C1h, 086h, 0D1h
        db  003h, 013h, 086h, 0EAh, 004h, 000h, 013h, 010h, 0A6h, 0D1h, 0C2h, 0E1h, 0C6h, 006h, 086h, 071h
        db  0CAh, 0C1h, 0E6h, 0A1h, 0BAh, 0E1h, 0A6h, 0EAh, 014h, 0E0h, 004h, 000h, 01Bh, 0C2h, 086h, 002h
        db  002h, 000h, 003h, 016h, 0A0h, 006h, 0CCh, 0C1h, 002h, 010h, 0A0h, 006h, 0A6h, 0C1h, 0DAh, 004h
        db  080h, 003h, 000h, 003h, 002h, 000h, 0ABh, 0C2h, 006h, 000h, 08Ch, 007h, 0E8h, 005h, 05Ch, 0C2h
        db  016h, 013h, 0A0h, 0C1h, 0ECh, 005h, 08Ah, 081h, 01Ah, 01Ah, 0C6h, 0C1h, 009h, 0C2h, 059h, 0C2h
        db  020h, 013h, 0E9h, 0A1h, 008h, 000h, 087h, 082h, 0F9h, 012h, 0A9h, 0A2h, 008h, 000h, 087h, 062h
        db  0CAh, 0CAh, 008h, 000h, 04Ah, 06Ah, 008h, 000h, 0C9h, 0C6h, 00Bh, 0C6h, 080h, 003h, 0CAh, 0CAh
        db  008h, 000h, 00Ah, 0C8h, 0ECh, 005h, 0DBh, 004h, 00Bh, 0CFh, 00Bh, 0C7h, 080h, 003h, 08Ah, 061h
        db  046h, 0CAh, 008h, 000h, 0CAh, 0CAh, 008h, 000h, 00Ah, 0C8h, 0ECh, 005h, 0C9h, 0C6h, 00Bh, 0C7h
        db  080h, 003h, 087h, 062h, 0CAh, 0CAh, 008h, 000h, 0DBh, 004h, 00Bh, 0C6h, 00Bh, 0CBh, 002h, 000h
        db  080h, 003h, 000h, 003h, 002h, 000h, 0BBh, 0C1h, 0DBh, 0C2h, 08Ch, 007h, 0E8h, 005h, 04Ch, 0C2h
        db  0EDh, 004h, 002h, 000h, 009h, 0C2h, 059h, 0C2h, 018h, 013h, 0A9h, 081h, 002h, 000h, 0FAh, 016h
        db  0E9h, 082h, 004h, 000h, 0F7h, 016h, 049h, 0CBh, 004h, 000h, 099h, 0C2h, 00Ah, 0C6h, 00Ah, 013h
        db  008h, 083h, 004h, 013h, 0A9h, 0AAh, 008h, 000h, 008h, 000h, 080h, 003h, 02Ah, 0A8h, 008h, 000h
        db  0ECh, 005h, 080h, 003h, 008h, 0CBh, 002h, 000h, 080h, 003h, 02Dh, 007h, 002h, 000h, 08Ch, 007h
        db  008h, 000h, 006h, 0A3h, 04Ch, 0C2h, 009h, 0C2h, 059h, 0C2h, 013h, 013h, 0E9h, 082h, 004h, 000h
        db  0FAh, 016h, 0ADh, 007h, 002h, 000h, 001h, 000h, 049h, 0CBh, 004h, 000h, 019h, 0C6h, 001h, 013h
        db  080h, 003h, 008h, 083h, 004h, 016h, 0A0h, 049h, 014h, 0E0h, 004h, 000h, 080h, 003h, 008h, 0CBh
        db  002h, 000h, 080h, 003h, 000h, 003h, 002h, 000h, 00Bh, 006h, 015h, 011h, 043h, 013h, 08Bh, 007h
        db  000h, 04Eh, 060h, 001h, 042h, 001h, 080h, 000h, 002h, 013h, 08Bh, 007h, 000h, 03Ah, 00Bh, 0C8h
        db  044h, 001h, 0E0h, 001h, 040h, 001h, 000h, 002h, 0E0h, 001h, 040h, 001h, 000h, 020h, 084h, 007h
        db  01Ch, 0BCh, 060h, 004h, 02Ah, 0BCh, 020h, 0C8h, 016h, 0E0h, 0E0h, 000h, 0E0h, 0C2h, 018h, 009h
        db  0E0h, 022h, 010h, 0E0h, 003h, 016h, 020h, 0E8h, 014h, 0E0h, 0E0h, 000h, 020h, 0C8h, 004h, 0E0h
        db  082h, 001h, 020h, 0C8h, 0E2h, 000h, 08Ah, 001h, 0E0h, 004h, 0CAh, 008h, 0E0h, 004h, 0F4h, 005h
        db  0E0h, 004h, 0F8h, 005h, 0E0h, 004h, 0F0h, 005h, 0E0h, 004h, 042h, 007h, 0A0h, 007h, 088h, 001h
        db  020h, 000h, 0E0h, 0C2h, 0E2h, 008h, 009h, 013h, 0A0h, 007h, 088h, 001h, 080h, 000h, 020h, 0E8h
        db  016h, 0E0h, 080h, 001h, 0E0h, 001h, 082h, 001h, 000h, 003h, 08Bh, 007h, 000h, 0A0h, 00Bh, 0E8h
        db  086h, 001h, 080h, 003h, 0E0h, 004h, 086h, 001h, 0E0h, 001h, 09Ch, 001h, 040h, 000h, 0E0h, 001h
        db  09Ch, 001h, 000h, 040h, 0CBh, 004h, 0B0h, 003h, 00Bh, 006h, 004h, 013h, 060h, 001h, 09Ch, 001h
        db  000h, 040h, 0F9h, 016h, 0E0h, 004h, 080h, 001h, 020h, 0E8h, 008h, 0E0h, 018h, 009h, 08Bh, 007h
        db  000h, 080h, 00Bh, 0C8h, 098h, 007h, 00Bh, 0C8h, 078h, 007h, 08Bh, 007h, 06Fh, 087h, 00Bh, 048h
        db  03Ah, 007h, 0E0h, 0C2h, 050h, 007h, 08Bh, 002h, 058h, 007h, 010h, 013h, 020h, 0E8h, 00Ah, 0E0h
        db  000h, 001h, 0E0h, 0C2h, 000h, 001h, 0E0h, 022h, 006h, 0E0h, 0F8h, 013h, 08Bh, 007h, 058h, 007h
        db  00Bh, 0C8h, 050h, 007h, 08Bh, 007h, 05Ch, 0C4h, 00Bh, 0C8h, 052h, 007h, 080h, 003h, 000h, 003h
        db  002h, 000h, 0E0h, 0C2h, 0CCh, 008h, 00Ch, 013h, 020h, 006h, 0CEh, 008h, 00Bh, 0C8h, 06Ch, 001h
        db  020h, 0C8h, 000h, 0FCh, 0CCh, 008h, 04Bh, 0CBh, 002h, 000h, 090h, 003h, 0FFh, 0FFh, 080h, 003h
        db  041h, 0C0h, 00Fh, 013h, 081h, 080h, 00Dh, 013h, 082h, 0A0h, 0E2h, 0C2h, 032h, 00Ch, 012h, 009h
        db  00Bh, 0C8h, 06Ch, 001h, 0E0h, 004h, 000h, 0FCh, 020h, 0C3h, 002h, 0FCh, 007h, 011h, 002h, 0C8h
        db  000h, 0FCh, 0EDh, 004h, 002h, 000h, 0E0h, 004h, 06Ch, 001h, 080h, 003h, 042h, 0CBh, 002h, 000h
        db  002h, 0C8h, 06Ch, 001h, 08Bh, 0C0h, 080h, 003h, 000h, 003h, 002h, 000h, 083h, 007h, 000h, 080h
        db  060h, 0C2h, 02Ch, 009h, 009h, 0C1h, 024h, 002h, 0F8h, 0FFh, 0A9h, 008h, 001h, 002h, 000h, 000h
        db  002h, 002h, 000h, 000h, 00Bh, 002h, 000h, 000h, 00Ch, 002h, 000h, 000h, 007h, 002h, 000h, 000h
        db  02Ch, 0CBh, 032h, 00Ch, 032h, 00Ch, 006h, 013h, 08Bh, 005h, 0CCh, 005h, 00Bh, 088h, 046h, 004h
        db  027h, 01Bh, 0F6h, 010h, 009h, 0C2h, 08Bh, 0C2h, 008h, 006h, 00Ah, 013h, 08Bh, 005h, 0CCh, 005h
        db  00Bh, 088h, 046h, 004h, 01Dh, 01Bh, 02Ch, 0CBh, 032h, 00Ch, 032h, 00Ch, 0EDh, 016h, 0F4h, 010h
        db  082h, 0C0h, 014h, 013h, 002h, 0C8h, 06Ch, 001h, 00Ah, 0C8h, 000h, 0FCh, 00Ah, 0C8h, 06Ch, 001h
        db  0E0h, 004h, 000h, 0FCh, 0A0h, 007h, 002h, 0FCh, 000h, 080h, 004h, 0C8h, 004h, 0FCh, 00Ah, 0C2h
        db  008h, 0A2h, 002h, 0CAh, 032h, 00Ch, 08Ah, 0C0h, 087h, 005h, 0D6h, 010h, 04Ah, 0C0h, 0EEh, 010h
        db  047h, 0CBh, 002h, 000h, 0E0h, 004h, 06Ch, 001h, 08Bh, 007h, 043h, 000h, 0E0h, 004h, 000h, 00Ch
        db  000h, 003h, 002h, 000h, 00Bh, 0C8h, 06Ch, 001h, 08Bh, 002h, 043h, 000h, 004h, 013h, 060h, 001h
        db  002h, 0FCh, 020h, 000h, 006h, 013h, 08Bh, 0C2h, 0A0h, 006h, 016h, 0C1h, 090h, 003h, 07Fh, 000h
        db  080h, 003h, 0A0h, 001h, 002h, 0FCh, 020h, 000h, 060h, 001h, 018h, 009h, 001h, 000h, 00Bh, 016h
        db  00Ah, 002h, 002h, 0FCh, 0A0h, 0A2h, 0DEh, 008h, 0A0h, 0CEh, 0EEh, 005h, 0A0h, 0C6h, 004h, 0FCh
        db  020h, 0C8h, 0DEh, 008h, 004h, 0FCh, 08Ah, 007h, 0F8h, 005h, 05Ah, 0C2h, 008h, 013h, 0CAh, 005h
        db  05Ah, 0C2h, 009h, 0C8h, 06Ch, 001h, 00Bh, 0C8h, 000h, 0FCh, 08Bh, 0C6h, 002h, 010h, 08Bh, 0CEh
        db  08Bh, 0C6h, 020h, 020h, 01Ah, 0E0h, 005h, 016h, 020h, 0E8h, 004h, 0E0h, 03Ah, 007h, 0E0h, 004h
        db  036h, 007h, 090h, 003h, 07Fh, 000h, 080h, 003h, 000h, 003h, 002h, 000h, 00Bh, 0C8h, 06Ch, 001h
        db  0CCh, 004h, 0E0h, 004h, 000h, 0FCh, 08Bh, 0C2h, 0A0h, 006h, 024h, 0C1h, 090h, 003h, 07Fh, 000h
        db  080h, 003h, 0A0h, 007h, 002h, 0FCh, 000h, 080h, 020h, 0C8h, 08Eh, 0E1h, 004h, 0FCh, 041h, 0C0h
        db  00Fh, 016h, 020h, 0D8h, 002h, 0E2h, 083h, 001h, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh
        db  08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 08Bh, 00Bh, 00Ah, 0C8h, 08Ah, 001h
        db  05Bh, 004h, 00Ah, 0C8h, 06Ch, 001h, 020h, 0C3h, 000h, 0FCh, 0E0h, 004h, 000h, 0FCh, 08Ah, 002h
        db  043h, 000h, 0DFh, 013h, 0A0h, 007h, 002h, 0FCh, 000h, 080h, 004h, 0C8h, 004h, 0FCh, 020h, 098h
        db  032h, 009h, 0CFh, 008h, 00Ah, 013h, 020h, 0C8h, 0CCh, 008h, 000h, 0FCh, 00Ah, 0C8h, 0CCh, 008h
        db  0A0h, 005h, 0CEh, 008h, 08Ch, 0C2h, 0E5h, 016h, 05Bh, 004h, 041h, 0C0h, 010h, 013h, 08Ah, 0A2h
        db  082h, 0CAh, 032h, 00Ch, 01Ah, 009h, 002h, 0C8h, 06Ch, 001h, 00Ah, 0C8h, 000h, 0FCh, 08Ah, 0C0h
        db  020h, 098h, 083h, 001h, 002h, 0E2h, 009h, 013h, 08Ch, 0C2h, 0D3h, 016h, 05Bh, 004h, 04Ah, 0C0h
        db  08Ah, 0C0h, 020h, 098h, 083h, 001h, 002h, 0E2h, 011h, 016h, 0E0h, 001h, 09Ch, 001h, 040h, 000h
        db  060h, 001h, 09Ch, 001h, 040h, 000h, 0FCh, 016h, 060h, 001h, 002h, 00Ch, 000h, 001h, 0ECh, 013h
        db  020h, 0D8h, 0E1h, 008h, 083h, 001h, 0A0h, 007h, 002h, 00Ch, 000h, 080h, 00Ah, 0C8h, 08Ah, 001h
        db  00Ah, 0C8h, 0CAh, 008h, 0E1h, 010h, 0D8h, 004h, 057h, 0C2h, 003h, 016h, 0C8h, 0CDh, 0C8h, 0C5h
        db  05Bh, 004h, 0C7h, 005h, 057h, 0C2h, 048h, 0C6h, 0C8h, 0C5h, 05Bh, 004h, 008h, 0C8h, 06Ch, 001h
        db  008h, 0A2h, 020h, 0CAh, 000h, 0FCh, 032h, 00Ch, 018h, 009h, 002h, 010h, 008h, 0C8h, 06Ch, 001h
        db  0E0h, 004h, 000h, 0FCh, 057h, 0C2h, 003h, 016h, 0C8h, 0CDh, 0C8h, 0C5h, 05Bh, 004h, 0C7h, 005h
        db  017h, 0C8h, 06Ch, 001h, 008h, 0C8h, 000h, 0FCh, 0C8h, 0C5h, 05Bh, 004h, 017h, 0C6h, 002h, 016h
        db  0C8h, 0C9h, 002h, 000h, 0C8h, 0C5h, 05Bh, 004h, 017h, 0C2h, 008h, 0C8h, 06Ch, 001h, 007h, 013h
        db  0E0h, 0C5h, 000h, 0FCh, 008h, 0A2h, 028h, 0C8h, 032h, 00Ch, 000h, 0FCh, 018h, 009h, 05Bh, 004h
        db  060h, 001h, 082h, 001h, 000h, 020h, 00Ah, 016h, 060h, 0C2h, 084h, 001h, 0A0h, 001h, 082h, 001h
        db  000h, 020h, 0E0h, 001h, 082h, 001h, 000h, 020h, 009h, 0C8h, 084h, 001h, 0C9h, 004h, 05Bh, 004h
        db  0A0h, 006h, 048h, 0C4h, 0D3h, 004h, 0E0h, 004h, 002h, 001h, 020h, 0E8h, 014h, 0E0h, 000h, 001h
        db  020h, 0C8h, 016h, 0E0h, 004h, 001h, 005h, 02Ch, 020h, 048h, 014h, 0E0h, 000h, 001h, 08Ch, 007h
        db  000h, 00Ah, 08Dh, 007h, 0D8h, 007h, 08Eh, 007h, 018h, 000h, 07Ch, 0CFh, 04Eh, 006h, 0FDh, 016h
        db  0E0h, 002h, 0D8h, 007h, 08Fh, 007h, 011h, 0FFh, 08Bh, 002h, 03Bh, 059h, 021h, 016h, 08Ah, 002h
        db  03Bh, 059h, 01Eh, 013h, 08Fh, 005h, 020h, 020h, 016h, 0E0h, 001h, 016h, 019h, 010h, 020h, 020h
        db  004h, 0E0h, 016h, 016h, 000h, 001h, 0BFh, 000h, 013h, 016h, 08Bh, 007h, 0C0h, 040h, 000h, 001h
        db  000h, 060h, 010h, 013h, 040h, 001h, 000h, 060h, 00Bh, 016h, 08Bh, 007h, 0C4h, 044h, 0A0h, 0C3h
        db  002h, 001h, 00Eh, 048h, 002h, 001h, 04Eh, 001h, 000h, 010h, 004h, 016h, 08Fh, 007h, 018h, 0FFh
        db  060h, 004h, 01Eh, 0C4h, 00Bh, 0C3h, 04Bh, 0C3h, 020h, 020h, 00Ah, 0E0h, 002h, 016h, 06Bh, 002h
        db  020h, 020h, 020h, 020h, 00Ch, 0E0h, 002h, 016h, 06Ch, 002h, 000h, 020h, 020h, 020h, 00Eh, 0E0h
        db  002h, 016h, 06Ch, 002h, 020h, 000h, 08Fh, 005h, 020h, 020h, 010h, 0E0h, 007h, 016h, 06Dh, 002h
        db  020h, 000h, 020h, 021h, 022h, 0E0h, 0E4h, 013h, 004h, 0C1h, 002h, 016h, 084h, 007h, 0FEh, 07Fh
        db  08Fh, 005h, 020h, 020h, 012h, 0E0h, 002h, 016h, 06Dh, 002h, 000h, 020h, 060h, 021h, 022h, 0E0h
        db  0D7h, 013h, 045h, 0C1h, 002h, 016h, 085h, 007h, 0FEh, 07Fh, 08Fh, 005h, 086h, 0D1h, 00Bh, 013h
        db  0A0h, 025h, 026h, 0E0h, 008h, 013h, 08Fh, 005h, 020h, 026h, 022h, 0E0h, 004h, 016h, 08Fh, 005h
        db  0A0h, 026h, 022h, 0E0h, 002h, 013h, 060h, 004h, 01Eh, 0C4h, 001h, 0D8h, 09Eh, 008h, 020h, 0D8h
        db  0DBh, 007h, 0B2h, 008h, 002h, 0D8h, 0A8h, 008h, 020h, 0D8h, 0DDh, 007h, 094h, 008h, 0E0h, 002h
        db  058h, 007h, 0A0h, 006h, 048h, 0C4h, 089h, 007h, 05Eh, 0E3h, 0E0h, 004h, 01Ah, 001h, 020h, 0C8h
        db  0E4h, 007h, 018h, 001h, 019h, 0C8h, 00Ch, 001h, 039h, 0C8h, 00Ah, 001h, 039h, 0C8h, 012h, 001h
        db  009h, 016h, 079h, 0C3h, 00Fh, 002h, 000h, 0E0h, 04Fh, 063h, 02Dh, 002h, 000h, 0A0h, 00Dh, 0C8h
        db  014h, 001h, 002h, 010h, 039h, 0C8h, 014h, 001h, 0F9h, 0C3h, 03Fh, 0C8h, 00Eh, 001h, 01Fh, 0C8h
        db  010h, 001h, 0E0h, 004h, 0C6h, 008h, 0B9h, 0C2h, 01Ah, 0D8h, 000h, 001h, 096h, 006h, 089h, 002h
        db  086h, 0E3h, 0E0h, 016h, 08Fh, 007h, 01Ch, 0FFh, 08Ch, 007h, 000h, 00Ah, 08Dh, 007h, 086h, 0E3h
        db  08Eh, 007h, 010h, 000h, 07Ch, 08Fh, 03Bh, 016h, 04Eh, 006h, 0FCh, 016h, 08Ch, 007h, 096h, 0E3h
        db  08Dh, 007h, 000h, 00Ah, 08Eh, 007h, 0A6h, 0E3h, 08Ch, 063h, 07Ch, 0CFh, 04Eh, 006h, 0FDh, 016h
        db  0E0h, 004h, 0E2h, 008h, 020h, 001h, 042h, 001h, 000h, 004h, 002h, 016h, 020h, 007h, 0E2h, 008h
        db  060h, 0C2h, 062h, 001h, 0E0h, 004h, 062h, 001h, 08Eh, 007h, 000h, 080h, 08Ch, 007h, 0E6h, 008h
        db  08Dh, 007h, 006h, 000h, 03Eh, 0DFh, 08Eh, 005h, 00Dh, 006h, 0FCh, 016h, 0FEh, 0D3h, 0CFh, 006h
        db  08Eh, 005h, 0FEh, 0D3h, 0CFh, 006h, 08Ch, 007h, 0E6h, 008h, 009h, 0C8h, 062h, 001h, 0C9h, 004h
        db  05Ch, 0A3h, 07Ch, 0E2h, 05Ch, 0A3h, 07Ch, 0E2h, 05Ch, 0A3h, 07Ch, 0E2h, 002h, 013h, 0CDh, 083h
        db  009h, 013h, 020h, 007h, 0E6h, 008h, 006h, 010h, 08Fh, 007h, 019h, 0FFh, 0CDh, 0A3h, 00Fh, 0C8h
        db  004h, 001h, 0FFh, 010h, 0A0h, 001h, 002h, 001h, 000h, 010h, 0E0h, 0C3h, 0EEh, 007h, 0E0h, 043h
        db  006h, 0E0h, 00Fh, 0D8h, 000h, 001h, 020h, 0C0h, 004h, 0E0h, 0E0h, 004h, 0FEh, 006h, 0D3h, 004h
        db  0E0h, 004h, 004h, 001h, 060h, 004h, 05Ch, 0C4h, 08Ch, 007h, 000h, 00Ah, 08Dh, 007h, 018h, 000h
        db  08Eh, 007h, 03Bh, 059h, 00Eh, 0CFh, 04Dh, 006h, 0FDh, 016h, 05Bh, 004h, 0D3h, 0C3h, 0C5h, 043h
        db  00Ch, 016h, 0E0h, 0C3h, 098h, 007h, 003h, 011h, 0E0h, 002h, 098h, 007h, 051h, 004h, 0E0h, 0C3h
        db  078h, 007h, 003h, 011h, 0E0h, 002h, 078h, 007h, 051h, 004h, 005h, 02Ch, 041h, 0A0h, 021h, 004h
        db  0C2h, 0E2h, 08Bh, 007h, 05Ch, 0C4h, 000h, 001h, 000h, 040h, 005h, 013h, 0C0h, 001h, 000h, 020h
        db  0DDh, 001h, 080h, 000h, 05Bh, 004h, 04Bh, 0C2h, 0A0h, 0D2h, 0EFh, 007h, 08Ch, 007h, 008h, 000h
        db  09Dh, 001h, 080h, 000h, 0BDh, 0C0h, 0A0h, 0C3h, 0EAh, 007h, 0E0h, 0C3h, 0ECh, 007h, 0A0h, 006h
        db  054h, 0C6h, 0C0h, 001h, 000h, 040h, 002h, 0D8h, 017h, 001h, 062h, 002h, 080h, 0FFh, 0A0h, 006h
        db  0B0h, 0C6h, 002h, 0C8h, 004h, 001h, 059h, 004h, 080h, 001h, 000h, 040h, 000h, 001h, 0FEh, 000h
        db  014h, 013h, 0C0h, 0C3h, 0CFh, 073h, 0EFh, 0D3h, 0C2h, 0E1h, 0CFh, 006h, 0CFh, 073h, 0AFh, 0C3h
        db  080h, 0EAh, 09Eh, 0C3h, 04Eh, 001h, 080h, 000h, 008h, 013h, 02Fh, 040h, 014h, 0E0h, 0CFh, 0A3h
        db  02Fh, 004h, 0F2h, 0E2h, 040h, 001h, 000h, 040h, 0B1h, 013h, 040h, 001h, 000h, 020h, 004h, 013h
        db  000h, 001h, 0FEh, 000h, 0E6h, 016h, 0AAh, 010h, 08Dh, 007h, 094h, 008h, 05Dh, 001h, 080h, 000h
        db  008h, 013h, 02Dh, 002h, 00Ah, 000h, 08Dh, 002h, 0C6h, 008h, 0F8h, 016h, 080h, 001h, 000h, 020h
        db  0EFh, 010h, 058h, 004h, 040h, 001h, 000h, 040h, 005h, 016h, 020h, 0E0h, 014h, 0E0h, 065h, 002h
        db  000h, 058h, 094h, 010h, 020h, 0D8h, 0DEh, 007h, 017h, 001h, 08Fh, 007h, 086h, 0FFh, 00Fh, 0C8h
        db  004h, 001h, 0C0h, 001h, 000h, 040h, 045h, 002h, 0FFh, 0A7h, 088h, 010h, 020h, 0C3h, 0FEh, 006h
        db  020h, 027h, 03Ah, 0E3h, 007h, 013h, 020h, 023h, 022h, 0E0h, 01Ah, 013h, 065h, 002h, 0FFh, 0DFh
        db  020h, 040h, 014h, 0E0h, 020h, 0E0h, 016h, 0E0h, 00Ch, 0C8h, 098h, 008h, 08Dh, 007h, 094h, 008h
        db  058h, 004h, 020h, 048h, 008h, 0E0h, 0FEh, 006h, 020h, 0C3h, 098h, 008h, 020h, 027h, 03Ah, 0E3h
        db  019h, 016h, 080h, 003h, 002h, 0C3h, 06Ch, 0C2h, 00Ah, 000h, 099h, 006h, 060h, 004h, 05Ch, 0C4h
        db  0A0h, 0D2h, 0EFh, 007h, 08Ch, 007h, 001h, 000h, 08Dh, 007h, 006h, 006h, 0CEh, 004h, 0E0h, 0C3h
        db  008h, 006h, 001h, 013h, 097h, 006h, 020h, 0D8h, 007h, 006h, 017h, 001h, 08Bh, 007h, 082h, 0FFh
        db  00Bh, 0C8h, 004h, 001h, 0A0h, 006h, 098h, 0CAh, 060h, 004h, 05Ch, 0C4h, 0A0h, 0C2h, 0EEh, 007h
        db  08Ch, 007h, 006h, 000h, 08Dh, 007h, 0A0h, 008h, 0A0h, 0C3h, 0E6h, 007h, 0E0h, 0C3h, 0E8h, 007h
        db  097h, 006h, 0A0h, 0D2h, 0EFh, 007h, 08Dh, 007h, 0A6h, 008h, 0DDh, 004h, 08Ch, 007h, 002h, 000h
        db  097h, 006h, 08Dh, 007h, 000h, 080h, 0A0h, 0C2h, 0A0h, 008h, 00Ah, 088h, 00Ch, 006h, 014h, 01Bh
        db  082h, 007h, 024h, 0C6h, 0A0h, 0C3h, 0A2h, 008h, 0E0h, 0C3h, 0A4h, 008h, 08Bh, 007h, 00Eh, 0E3h
        db  08Ah, 002h, 014h, 000h, 004h, 01Ah, 08Bh, 007h, 07Ch, 0EAh, 02Ah, 002h, 0ECh, 0FFh, 08Ah, 0A2h
        db  0CAh, 0A2h, 0DBh, 0C2h, 001h, 013h, 09Bh, 006h, 020h, 0C8h, 0A0h, 008h, 0A4h, 008h, 020h, 0C8h
        db  020h, 0E0h, 0A0h, 008h, 00Dh, 0C8h, 0A2h, 008h, 08Dh, 007h, 09Eh, 008h, 020h, 0E0h, 018h, 0E0h
        db  065h, 002h, 000h, 058h, 058h, 004h, 045h, 002h, 0FFh, 0A7h, 080h, 003h, 060h, 0C0h, 0EEh, 005h
        db  021h, 002h, 0E8h, 003h, 020h, 001h, 002h, 001h, 006h, 000h, 007h, 016h, 001h, 088h, 0EEh, 005h
        db  0F9h, 016h, 03Dh, 010h, 060h, 0D0h, 003h, 001h, 0F1h, 013h, 0E0h, 004h, 01Ah, 001h, 001h, 002h
        db  00Ah, 001h, 04Ch, 0CCh, 04Ch, 0CCh, 04Eh, 0CCh, 04Fh, 0CCh, 0B1h, 007h, 040h, 000h, 04Dh, 0CCh
        db  00Ah, 0D8h, 000h, 001h, 05Bh, 004h, 060h, 0C0h, 0EEh, 005h, 021h, 002h, 0E8h, 003h, 020h, 001h
        db  002h, 001h, 006h, 000h, 007h, 016h, 001h, 088h, 0EEh, 005h, 0F9h, 016h, 020h, 010h, 060h, 0D0h
        db  003h, 001h, 0F1h, 013h, 0E0h, 004h, 01Ah, 001h, 001h, 002h, 00Ah, 001h, 04Ch, 0CCh, 04Ch, 0CCh
        db  04Eh, 0CCh, 04Fh, 0CCh, 0B1h, 007h, 040h, 000h, 04Dh, 0CCh, 00Ah, 0D8h, 000h, 001h, 0B0h, 003h
        db  060h, 0D0h, 003h, 001h, 001h, 013h, 05Bh, 004h, 060h, 0C0h, 0EEh, 005h, 021h, 002h, 0E8h, 003h
        db  020h, 001h, 002h, 001h, 006h, 000h, 0F7h, 016h, 001h, 088h, 0EEh, 005h, 0F9h, 016h, 0CDh, 004h
        db  08Ah, 007h, 000h, 040h, 020h, 0C3h, 000h, 001h, 00Ch, 001h, 000h, 080h, 002h, 013h, 08Ah, 007h
        db  000h, 020h, 0A0h, 0C3h, 00Eh, 001h, 0E0h, 0C3h, 010h, 001h, 0B0h, 003h, 020h, 0C3h, 058h, 007h
        db  020h, 023h, 004h, 0E0h, 002h, 013h, 060h, 004h, 018h, 0C4h, 060h, 004h, 07Ch, 0B3h, 08Dh, 007h
        db  000h, 020h, 020h, 020h, 00Ah, 0E0h, 001h, 016h, 05Bh, 004h, 00Dh, 002h, 032h, 00Ch, 05Dh, 0C2h
        db  001h, 011h, 0DDh, 004h, 0CDh, 005h, 00Dh, 088h, 030h, 00Ch, 0F9h, 016h, 060h, 0C2h, 00Ah, 006h
        db  08Dh, 007h, 018h, 009h, 0A0h, 006h, 0E0h, 0CAh, 009h, 002h, 068h, 000h, 0E0h, 0C3h, 0E2h, 008h
        db  003h, 016h, 0E0h, 001h, 018h, 009h, 010h, 000h, 0E0h, 0C2h, 018h, 009h, 00Fh, 002h, 000h, 001h
        db  0C9h, 026h, 002h, 013h, 060h, 004h, 06Ah, 0C9h, 009h, 002h, 000h, 012h, 04Bh, 001h, 010h, 000h
        db  002h, 013h, 009h, 002h, 000h, 013h, 009h, 0D8h, 0E0h, 008h, 08Fh, 007h, 000h, 040h, 089h, 007h
        db  01Ah, 009h, 0CBh, 004h, 0F9h, 0E2h, 0F9h, 0E2h, 0F9h, 0E2h, 007h, 016h, 08Bh, 007h, 0E6h, 008h
        db  08Ch, 007h, 01Ah, 009h, 03Bh, 0CFh, 03Bh, 0CFh, 01Bh, 0C7h, 020h, 0C3h, 01Ah, 009h, 018h, 011h
        db  08Fh, 007h, 000h, 020h, 089h, 007h, 028h, 009h, 0A0h, 006h, 092h, 0C7h, 0A0h, 006h, 092h, 0C7h
        db  011h, 010h, 019h, 0C3h, 002h, 016h, 08Ch, 007h, 01Ah, 000h, 08Ch, 002h, 00Eh, 000h, 006h, 013h
        db  08Ch, 002h, 014h, 000h, 003h, 013h, 08Ch, 002h, 01Ah, 000h, 002h, 016h, 04Ch, 0CEh, 05Bh, 004h
        db  060h, 004h, 06Ah, 0C9h, 08Fh, 007h, 000h, 010h, 0D9h, 0C2h, 0FAh, 011h, 002h, 016h, 08Bh, 007h
        db  000h, 004h, 04Bh, 0C3h, 08Dh, 002h, 020h, 000h, 002h, 014h, 00Dh, 002h, 020h, 000h, 08Dh, 002h
        db  000h, 004h, 002h, 012h, 00Dh, 002h, 000h, 004h, 02Dh, 002h, 0F8h, 0FFh, 00Dh, 0C8h, 0DEh, 008h
        db  02Bh, 002h, 0FFh, 003h, 08Bh, 001h, 0FFh, 003h, 04Bh, 0CEh, 0CBh, 004h, 0E0h, 004h, 0F8h, 005h
        db  0E0h, 004h, 014h, 009h, 020h, 0C8h, 030h, 00Ch, 02Eh, 009h, 0A0h, 007h, 030h, 009h, 0FEh, 0DFh
        db  08Dh, 007h, 0FEh, 0DFh, 0E0h, 0C3h, 0D8h, 007h, 0E0h, 023h, 016h, 0E0h, 024h, 016h, 0E0h, 0C3h
        db  030h, 00Ch, 04Fh, 063h, 0FFh, 004h, 0FFh, 004h, 04Dh, 006h, 0FDh, 016h, 08Dh, 007h, 0FEh, 0DFh
        db  020h, 004h, 064h, 0EAh, 0A0h, 0C3h, 064h, 0EAh, 0EEh, 0C3h, 012h, 000h, 0C1h, 016h, 06Eh, 0C3h
        db  018h, 000h, 0ADh, 009h, 08Ch, 007h, 000h, 0E0h, 0ACh, 009h, 00Dh, 063h, 00Ch, 013h, 06Eh, 0C3h
        db  018h, 000h, 0ADh, 009h, 02Dh, 002h, 040h, 000h, 01Dh, 00Ah, 02Dh, 002h, 032h, 00Ch, 0BDh, 007h
        db  0FFh, 07Fh, 00Ch, 006h, 0FCh, 016h, 020h, 0C3h, 046h, 004h, 08Ch, 002h, 080h, 000h, 013h, 01Ah
        db  0ACh, 002h, 00Ch, 0C8h, 09Ah, 000h, 0E0h, 002h, 080h, 000h, 088h, 007h, 080h, 000h, 060h, 0C2h
        db  046h, 004h, 0A0h, 006h, 026h, 0F1h, 002h, 010h, 09Dh, 000h, 005h, 010h, 09Dh, 000h, 08Fh, 007h
        db  000h, 008h, 060h, 004h, 06Ah, 0C9h, 04Bh, 02Dh, 081h, 0C3h, 0C9h, 005h, 08Fh, 007h, 000h, 010h
        db  08Eh, 002h, 002h, 000h, 06Ah, 011h, 08Fh, 007h, 000h, 004h, 0C9h, 005h, 0D9h, 0C2h, 0E0h, 026h
        db  026h, 0E0h, 002h, 016h, 02Bh, 002h, 006h, 000h, 04Bh, 0CEh, 04Bh, 0C3h, 0CBh, 072h, 02Eh, 002h
        db  0FEh, 0FFh, 08Bh, 083h, 05Ah, 01Bh, 0CDh, 006h, 04Dh, 073h, 0CDh, 082h, 056h, 01Bh, 0E0h, 004h
        db  0CCh, 008h, 0E0h, 004h, 0CEh, 008h, 0B9h, 0C3h, 0D9h, 0C3h, 089h, 007h, 012h, 000h, 08Dh, 007h
        db  0ECh, 008h, 0A0h, 006h, 0E0h, 0CAh, 060h, 0C3h, 0D8h, 007h, 060h, 023h, 016h, 0E0h, 009h, 016h
        db  020h, 0E8h, 010h, 0E0h, 018h, 009h, 020h, 0E8h, 018h, 0E0h, 098h, 007h, 020h, 0E8h, 012h, 0E0h
        db  078h, 007h, 060h, 0C3h, 018h, 009h, 060h, 023h, 01Eh, 0E0h, 003h, 016h, 020h, 048h, 0A6h, 0E3h
        db  018h, 009h, 060h, 023h, 022h, 0E0h, 006h, 013h, 060h, 027h, 0A8h, 0E3h, 003h, 013h, 020h, 0E8h
        db  010h, 0E0h, 018h, 009h, 020h, 02Dh, 000h, 000h, 08Eh, 007h, 000h, 000h, 0A0h, 006h, 0C0h, 0CAh
        db  04Eh, 005h, 00Eh, 02Ch, 0A0h, 0C0h, 0FEh, 007h, 0EFh, 0C3h, 006h, 000h, 01Bh, 016h, 0A0h, 0C3h
        db  020h, 009h, 0E0h, 0C3h, 022h, 009h, 0A0h, 006h, 0A6h, 0C9h, 0A0h, 0C3h, 024h, 009h, 0E0h, 0C3h
        db  026h, 009h, 0A0h, 006h, 0C4h, 0C9h, 020h, 0E0h, 00Ah, 0E0h, 060h, 0C3h, 0D8h, 007h, 060h, 023h
        db  016h, 0E0h, 005h, 016h, 0E0h, 004h, 02Eh, 006h, 060h, 041h, 004h, 0E0h, 04Dh, 02Eh, 08Dh, 007h
        db  000h, 080h, 052h, 004h, 0CFh, 073h, 02Fh, 002h, 000h, 002h, 04Fh, 0C3h, 052h, 004h, 020h, 020h
        db  00Ah, 0E0h, 003h, 013h, 08Dh, 007h, 000h, 010h, 05Bh, 004h, 020h, 040h, 00Ah, 0E0h, 040h, 002h
        db  0FFh, 0F0h, 08Eh, 007h, 002h, 000h, 0A0h, 006h, 0C0h, 0CAh, 04Eh, 005h, 00Eh, 02Ch, 0A0h, 0C0h
        db  0FEh, 007h, 0A0h, 006h, 098h, 0CAh, 060h, 0C3h, 0D8h, 007h, 060h, 023h, 016h, 0E0h, 066h, 016h
        db  020h, 004h, 078h, 0EAh, 063h, 010h, 06Eh, 002h, 000h, 080h, 08Dh, 007h, 000h, 0C0h, 00Dh, 0C8h
        db  0A6h, 001h, 00Eh, 0C8h, 020h, 009h, 00Fh, 0C8h, 022h, 009h, 00Eh, 0C8h, 0A8h, 001h, 00Fh, 0C8h
        db  0AAh, 001h, 012h, 010h, 08Fh, 001h, 001h, 000h, 08Ah, 007h, 024h, 009h, 0A0h, 0E3h, 000h, 009h
        db  08Eh, 0CEh, 09Ah, 001h, 0FEh, 0FFh, 0E0h, 0E3h, 002h, 009h, 08Fh, 0E6h, 08Ah, 007h, 0ACh, 001h
        db  08Eh, 0CEh, 09Ah, 001h, 0FEh, 0FFh, 08Fh, 0E6h, 020h, 020h, 00Ah, 0E0h, 03Fh, 013h, 08Dh, 007h
        db  000h, 010h, 05Bh, 004h, 020h, 020h, 00Ah, 0E0h, 003h, 013h, 00Dh, 002h, 000h, 010h, 05Bh, 004h
        db  08Eh, 0C3h, 004h, 013h, 0E0h, 001h, 002h, 009h, 000h, 001h, 006h, 010h, 0A0h, 001h, 002h, 009h
        db  000h, 001h, 0A0h, 001h, 026h, 009h, 000h, 001h, 0A0h, 0C3h, 024h, 009h, 0E0h, 0C3h, 026h, 009h
        db  0A0h, 0E3h, 000h, 009h, 0E0h, 0E3h, 002h, 009h, 00Eh, 0C8h, 0ACh, 001h, 00Fh, 0C8h, 0AEh, 001h
        db  00Eh, 0C8h, 024h, 009h, 00Fh, 0C8h, 026h, 009h, 019h, 010h, 06Eh, 002h, 000h, 080h, 00Eh, 0C8h
        db  0A6h, 001h, 020h, 020h, 00Ah, 0E0h, 012h, 013h, 00Dh, 002h, 000h, 010h, 05Bh, 004h, 08Dh, 007h
        db  028h, 007h, 089h, 007h, 00Eh, 000h, 0A0h, 006h, 0ECh, 0CAh, 08Dh, 007h, 028h, 007h, 0FDh, 004h
        db  08Dh, 002h, 036h, 007h, 0FCh, 016h, 020h, 048h, 014h, 0E0h, 0FEh, 006h, 08Dh, 007h, 000h, 080h
        db  052h, 004h, 0A0h, 0C2h, 0EEh, 007h, 08Ch, 007h, 004h, 000h, 08Dh, 007h, 0A2h, 008h, 097h, 006h
        db  07Dh, 0C2h, 05Dh, 0C3h, 060h, 043h, 022h, 0E0h, 0A0h, 006h, 0ECh, 0CAh, 0EFh, 010h, 00Eh, 0C8h
        db  006h, 006h, 00Fh, 0C8h, 008h, 006h, 0EAh, 010h, 08Ch, 007h, 014h, 0E0h, 01Ch, 048h, 094h, 008h
        db  01Ch, 048h, 0A8h, 008h, 01Ch, 048h, 0B2h, 008h, 040h, 002h, 000h, 0C0h, 0E0h, 004h, 006h, 006h
        db  08Ch, 007h, 010h, 040h, 0CCh, 044h, 0E0h, 004h, 0FEh, 006h, 085h, 007h, 040h, 080h, 05Bh, 004h
        db  002h, 0C8h, 0FEh, 007h, 08Fh, 007h, 0F4h, 007h, 0CEh, 0CBh, 002h, 000h, 08Eh, 007h, 036h, 007h
        db  0CEh, 0CBh, 004h, 000h, 08Dh, 007h, 030h, 006h, 08Eh, 007h, 010h, 000h, 04Dh, 02Ch, 05Bh, 004h
        db  020h, 0C8h, 0E2h, 007h, 01Ah, 001h, 0A0h, 0C2h, 0F2h, 007h, 005h, 010h, 020h, 0C8h, 0E0h, 007h
        db  01Ah, 001h, 0A0h, 0D2h, 0F3h, 007h, 00Bh, 0C8h, 09Ch, 008h, 009h, 0C3h, 00Ah, 013h, 0A0h, 006h
        db  08Eh, 0C6h, 0A0h, 0C2h, 000h, 001h, 0A0h, 0E2h, 006h, 0E0h, 04Ch, 0A3h, 0CCh, 0A3h, 001h, 017h
        db  08Eh, 005h, 04Ch, 062h, 0E0h, 0C2h, 09Ch, 008h, 05Bh, 004h, 08Dh, 007h, 000h, 010h, 020h, 020h
        db  00Ah, 0E0h, 001h, 013h, 05Bh, 004h, 00Dh, 002h, 068h, 000h, 0E0h, 0C3h, 0E2h, 008h, 002h, 016h
        db  0CEh, 001h, 010h, 000h, 08Dh, 027h, 003h, 013h, 00Dh, 002h, 000h, 001h, 052h, 004h, 000h, 003h
        db  002h, 000h, 060h, 0C3h, 018h, 009h, 04Dh, 002h, 008h, 080h, 04Eh, 002h, 0F7h, 07Fh, 08Dh, 0E3h
        db  0E0h, 0C3h, 0D8h, 007h, 0E0h, 023h, 016h, 0E0h, 004h, 013h, 08Dh, 007h, 006h, 000h, 08Dh, 027h
        db  002h, 013h, 0A0h, 0E3h, 010h, 0E0h, 00Eh, 0C8h, 018h, 009h, 00Dh, 002h, 000h, 012h, 04Eh, 001h
        db  010h, 000h, 002h, 013h, 00Dh, 002h, 000h, 013h, 00Dh, 0D8h, 0E0h, 008h, 060h, 0C3h, 080h, 001h
        db  04Eh, 002h, 001h, 000h, 04Dh, 002h, 0FEh, 0FFh, 04Eh, 0E3h, 00Dh, 0C8h, 080h, 001h, 020h, 0D8h
        db  042h, 0E2h, 0E1h, 008h, 020h, 001h, 018h, 009h, 006h, 000h, 003h, 013h, 020h, 0D8h, 0D2h, 0E1h
        db  0E1h, 008h, 020h, 098h, 083h, 001h, 002h, 0E2h, 003h, 013h, 020h, 0D8h, 0E1h, 008h, 083h, 001h
        db  000h, 003h, 00Fh, 000h, 060h, 004h, 06Ch, 0CAh, 020h, 020h, 00Ah, 0E0h, 003h, 013h, 00Dh, 002h
        db  000h, 010h, 05Bh, 004h, 009h, 002h, 008h, 000h, 00Dh, 002h, 00Ah, 009h, 0A0h, 006h, 0E0h, 0CAh
        db  0A0h, 007h, 002h, 002h, 000h, 000h, 00Dh, 002h, 000h, 004h, 0E0h, 0C3h, 00Ah, 009h, 00Fh, 001h
        db  000h, 07Ch, 001h, 013h, 052h, 004h, 08Fh, 0C3h, 04Eh, 002h, 00Fh, 000h, 0FBh, 013h, 08Eh, 002h
        db  00Fh, 000h, 0F8h, 013h, 00Dh, 002h, 000h, 040h, 04Fh, 0C2h, 049h, 009h, 049h, 002h, 03Fh, 000h
        db  009h, 001h, 001h, 000h, 0EFh, 016h, 089h, 002h, 006h, 000h, 0ECh, 01Ah, 089h, 002h, 020h, 000h
        db  0E9h, 014h, 0C9h, 006h, 01Fh, 009h, 04Fh, 002h, 000h, 040h, 04Fh, 0E2h, 069h, 002h, 000h, 080h
        db  009h, 0C8h, 00Ah, 009h, 00Fh, 002h, 0FFh, 0FFh, 04Eh, 0C2h, 01Fh, 009h, 009h, 006h, 0FDh, 016h
        db  04Fh, 005h, 00Dh, 002h, 000h, 020h, 060h, 0C2h, 00Ch, 009h, 0D4h, 013h, 04Fh, 026h, 0D2h, 016h
        db  00Dh, 002h, 000h, 010h, 060h, 0C2h, 00Eh, 009h, 0CDh, 013h, 04Fh, 026h, 0CBh, 016h, 00Dh, 002h
        db  000h, 030h, 020h, 088h, 00Ch, 009h, 00Eh, 009h, 0C5h, 013h, 0E0h, 0C3h, 00Ch, 009h, 04Eh, 0C2h
        db  01Fh, 00Ah, 009h, 006h, 0FDh, 016h, 0E0h, 0E3h, 010h, 009h, 00Fh, 0C8h, 00Ch, 009h, 0E0h, 0C3h
        db  00Eh, 009h, 04Eh, 0C2h, 01Fh, 00Ah, 009h, 006h, 0FDh, 016h, 0E0h, 0E3h, 010h, 009h, 00Fh, 0C8h
        db  00Eh, 009h, 00Fh, 002h, 0FFh, 0FFh, 04Eh, 0C2h, 01Fh, 00Ah, 009h, 006h, 0FDh, 016h, 00Dh, 002h
        db  000h, 008h, 060h, 0C2h, 010h, 009h, 04Fh, 026h, 0A5h, 016h, 04Fh, 005h, 00Fh, 0C8h, 010h, 009h
        db  00Fh, 002h, 002h, 002h, 00Eh, 002h, 003h, 000h, 060h, 0C3h, 040h, 001h, 00Ch, 002h, 0EEh, 0CCh
        db  0A0h, 001h, 040h, 001h, 000h, 004h, 0CFh, 005h, 009h, 002h, 055h, 055h, 09Ch, 006h, 049h, 005h
        db  09Ch, 006h, 009h, 007h, 09Ch, 006h, 049h, 005h, 09Ch, 006h, 00Eh, 006h, 0F4h, 016h, 0A0h, 001h
        db  040h, 001h, 000h, 040h, 00Dh, 0C8h, 040h, 001h, 009h, 002h, 008h, 000h, 00Eh, 002h, 00Ah, 009h
        db  00Fh, 002h, 002h, 002h, 0FEh, 0CFh, 049h, 006h, 0FDh, 016h, 060h, 004h, 06Ch, 0CAh, 0C9h, 0C7h
        db  05Fh, 082h, 001h, 016h, 05Bh, 004h, 0A0h, 001h, 040h, 001h, 000h, 040h, 00Dh, 0C8h, 040h, 001h
        db  00Dh, 002h, 000h, 001h, 052h, 004h, 08Dh, 007h, 000h, 010h, 020h, 020h, 00Ah, 0E0h, 00Ah, 016h
        db  08Dh, 007h, 000h, 008h, 020h, 020h, 010h, 0E0h, 005h, 013h, 08Dh, 007h, 000h, 040h, 04Fh, 001h
        db  001h, 000h, 001h, 016h, 05Bh, 004h, 020h, 0E0h, 010h, 0E0h, 08Dh, 007h, 036h, 007h, 089h, 007h
        db  04Ah, 008h, 04Dh, 0CAh, 002h, 000h, 0A0h, 006h, 006h, 0CFh, 089h, 007h, 062h, 008h, 04Dh, 0CAh
        db  002h, 000h, 0A0h, 006h, 006h, 0CFh, 089h, 007h, 07Ah, 008h, 04Dh, 0CAh, 002h, 000h, 0A0h, 006h
        db  006h, 0CFh, 0E0h, 004h, 0B8h, 008h, 0E0h, 004h, 0BAh, 008h, 00Eh, 0C8h, 022h, 008h, 00Fh, 0C8h
        db  024h, 008h, 00Eh, 0C8h, 040h, 008h, 00Fh, 0C8h, 042h, 008h, 0E0h, 004h, 030h, 008h, 0E0h, 004h
        db  036h, 008h, 0E0h, 002h, 078h, 007h, 0E0h, 004h, 046h, 008h, 0E0h, 004h, 048h, 008h, 020h, 040h
        db  042h, 0E3h, 020h, 0E0h, 00Ch, 0E0h, 060h, 004h, 058h, 0D2h, 080h, 001h, 000h, 0F0h, 00Dh, 010h
        db  0C0h, 001h, 000h, 0F0h, 020h, 040h, 006h, 0E0h, 008h, 010h, 0C0h, 001h, 000h, 0F0h, 080h, 001h
        db  000h, 020h, 0E0h, 0C3h, 046h, 008h, 001h, 016h, 05Bh, 004h, 04Bh, 0C0h, 020h, 004h, 07Ch, 0EAh
        db  040h, 001h, 000h, 020h, 0FBh, 016h, 051h, 004h, 0A0h, 0C2h, 0D8h, 007h, 04Ah, 001h, 040h, 000h
        db  001h, 016h, 05Bh, 004h, 0E0h, 002h, 078h, 007h, 020h, 020h, 00Ch, 0E0h, 0EFh, 016h, 043h, 0C2h
        db  002h, 013h, 0A0h, 006h, 0F4h, 0CEh, 020h, 02Fh, 036h, 007h, 020h, 040h, 00Ch, 0E0h, 0A0h, 006h
        db  09Ah, 0CDh, 0A0h, 0C3h, 046h, 008h, 0FBh, 016h, 0A0h, 006h, 02Eh, 0CEh, 08Eh, 007h, 0B6h, 008h
        db  09Eh, 007h, 000h, 080h, 020h, 020h, 010h, 0E0h, 005h, 016h, 08Fh, 007h, 022h, 008h, 0BFh, 0CFh
        db  0BFh, 0CFh, 09Fh, 0C7h, 0A0h, 006h, 052h, 0CEh, 0B0h, 003h, 020h, 0E8h, 03Eh, 0E3h, 062h, 007h
        db  0A0h, 006h, 02Eh, 0CEh, 0B0h, 003h, 020h, 048h, 03Eh, 0E3h, 062h, 007h, 020h, 040h, 042h, 0E3h
        db  020h, 0E0h, 004h, 0E0h, 0B0h, 003h, 020h, 048h, 010h, 0E0h, 058h, 007h, 05Bh, 004h, 080h, 001h
        db  000h, 0F0h, 020h, 0E0h, 004h, 0E0h, 0E0h, 0C3h, 0B2h, 008h, 0E0h, 023h, 014h, 0E0h, 002h, 013h
        db  09Bh, 006h, 0B4h, 010h, 0B0h, 003h, 020h, 0E8h, 01Eh, 0E0h, 058h, 007h, 0AFh, 010h, 09Bh, 006h
        db  080h, 003h, 0E0h, 002h, 058h, 007h, 08Dh, 007h, 0B2h, 008h, 0A0h, 006h, 086h, 0C4h, 0E0h, 002h
        db  078h, 007h, 05Bh, 004h, 060h, 0C2h, 05Ch, 007h, 020h, 006h, 046h, 008h, 0A9h, 0C2h, 008h, 000h
        db  0A0h, 006h, 0F4h, 0CEh, 069h, 001h, 00Ah, 000h, 001h, 000h, 036h, 013h, 049h, 0C3h, 02Dh, 002h
        db  00Eh, 000h, 00Ah, 0C3h, 01Dh, 0D3h, 08Ch, 001h, 000h, 084h, 0CCh, 001h, 000h, 040h, 00Ah, 001h
        db  000h, 05Eh, 008h, 013h, 00Eh, 002h, 000h, 023h, 04Eh, 0DBh, 001h, 000h, 0CCh, 001h, 000h, 004h
        db  04Ch, 0D7h, 001h, 010h, 04Ch, 0C7h, 0A9h, 0C3h, 010h, 000h, 0E9h, 0C3h, 012h, 000h, 041h, 0CAh
        db  010h, 000h, 02Fh, 002h, 004h, 000h, 001h, 017h, 08Eh, 005h, 08Ch, 007h, 002h, 000h, 0A0h, 0D2h
        db  0F1h, 007h, 0A0h, 006h, 054h, 0C6h, 069h, 0C0h, 010h, 000h, 029h, 0C8h, 014h, 000h, 0B8h, 008h
        db  029h, 0C8h, 016h, 000h, 0BAh, 008h, 0A0h, 007h, 0B6h, 008h, 000h, 040h, 069h, 001h, 00Eh, 000h
        db  000h, 008h, 002h, 016h, 0A0h, 006h, 052h, 0CEh, 040h, 001h, 000h, 040h, 001h, 016h, 051h, 004h
        db  060h, 004h, 0ACh, 0CDh, 0A9h, 0C3h, 00Ch, 000h, 006h, 013h, 00Eh, 068h, 048h, 008h, 0A9h, 0C3h
        db  006h, 000h, 001h, 013h, 01Eh, 02Eh, 0E9h, 004h, 006h, 000h, 0E9h, 004h, 00Ch, 000h, 029h, 007h
        db  004h, 000h, 05Bh, 004h, 081h, 007h, 020h, 020h, 089h, 007h, 022h, 008h, 041h, 0CEh, 063h, 0CEh
        db  010h, 000h, 063h, 0C6h, 012h, 000h, 0A0h, 006h, 0B0h, 0C6h, 043h, 0C2h, 002h, 013h, 0A0h, 006h
        db  0F4h, 0CEh, 020h, 0E0h, 010h, 0E0h, 060h, 004h, 0DAh, 0CDh, 0A0h, 006h, 0B0h, 0C6h, 0A0h, 0C3h
        db  022h, 008h, 0E0h, 0C3h, 024h, 008h, 04Fh, 001h, 001h, 000h, 01Dh, 016h, 040h, 001h, 000h, 004h
        db  0E1h, 016h, 0A0h, 006h, 09Ah, 0CDh, 0A0h, 0C3h, 046h, 008h, 0FBh, 016h, 0A0h, 0C2h, 0F0h, 007h
        db  08Ch, 007h, 004h, 000h, 08Dh, 007h, 022h, 008h, 0A0h, 0C3h, 040h, 008h, 0E0h, 0C3h, 042h, 008h
        db  0A0h, 006h, 08Eh, 0C6h, 0A0h, 0C3h, 022h, 008h, 0E0h, 0C3h, 024h, 008h, 04Fh, 001h, 001h, 000h
        db  002h, 016h, 060h, 004h, 0DAh, 0CDh, 0A0h, 0C2h, 0F0h, 007h, 020h, 0C3h, 02Ah, 009h, 08Dh, 007h
        db  022h, 008h, 0A0h, 006h, 08Eh, 0C6h, 0C0h, 006h, 020h, 0D0h, 026h, 008h, 0C0h, 006h, 040h, 001h
        db  000h, 004h, 00Ah, 016h, 040h, 001h, 080h, 000h, 007h, 013h, 00Eh, 0C8h, 022h, 008h, 00Fh, 0C8h
        db  024h, 008h, 0A0h, 006h, 090h, 0CDh, 0DEh, 010h, 00Eh, 0C8h, 040h, 008h, 00Fh, 0C8h, 042h, 008h
        db  040h, 001h, 000h, 004h, 006h, 013h, 040h, 001h, 020h, 000h, 030h, 016h, 081h, 007h, 010h, 020h
        db  0A3h, 010h, 083h, 007h, 04Ah, 008h, 063h, 007h, 004h, 000h, 00Eh, 011h, 083h, 007h, 062h, 008h
        db  063h, 007h, 004h, 000h, 009h, 011h, 083h, 007h, 07Ah, 008h, 063h, 007h, 004h, 000h, 004h, 011h
        db  0C3h, 060h, 0A0h, 006h, 09Ah, 0CDh, 0EDh, 010h, 0E0h, 0C8h, 026h, 008h, 00Eh, 000h, 0E0h, 0C8h
        db  040h, 008h, 010h, 000h, 0E0h, 0C8h, 042h, 008h, 012h, 000h, 0C7h, 004h, 040h, 001h, 020h, 000h
        db  0DDh, 016h, 020h, 0C8h, 028h, 008h, 044h, 008h, 009h, 016h, 081h, 007h, 080h, 020h, 0E0h, 0C8h
        db  040h, 008h, 014h, 000h, 0E0h, 0C8h, 042h, 008h, 016h, 000h, 0D2h, 010h, 0E0h, 0C8h, 040h, 008h
        db  014h, 000h, 0E0h, 0C8h, 042h, 008h, 016h, 000h, 080h, 001h, 000h, 004h, 082h, 007h, 02Ah, 008h
        db  032h, 0C1h, 008h, 011h, 072h, 0C1h, 092h, 0C1h, 082h, 007h, 03Ch, 008h, 004h, 0C1h, 007h, 016h
        db  060h, 004h, 006h, 0D1h, 072h, 0C1h, 0B2h, 0C1h, 084h, 001h, 000h, 080h, 0F9h, 013h, 004h, 068h
        db  044h, 008h, 0C7h, 0C1h, 023h, 016h, 020h, 098h, 049h, 008h, 033h, 009h, 00Ah, 016h, 081h, 007h
        db  040h, 020h, 0E0h, 0C1h, 046h, 008h, 065h, 013h, 0A0h, 006h, 09Ah, 0CDh, 0F4h, 010h, 0A0h, 006h
        db  08Ah, 0CDh, 081h, 02Dh, 001h, 0C2h, 0FBh, 013h, 0A0h, 005h, 048h, 008h, 0C3h, 0C1h, 027h, 002h
        db  006h, 000h, 0A0h, 006h, 0CCh, 0C1h, 0A3h, 005h, 00Ch, 000h, 008h, 0C8h, 06Ch, 001h, 008h, 0C8h
        db  092h, 008h, 008h, 002h, 004h, 0FCh, 0F8h, 0C1h, 004h, 0C1h, 059h, 013h, 0E0h, 0D2h, 003h, 001h
        db  00Bh, 016h, 0E0h, 0C2h, 03Ah, 007h, 0E0h, 042h, 062h, 007h, 0E0h, 026h, 03Ch, 0E3h, 002h, 013h
        db  0A0h, 006h, 08Ah, 0CDh, 0A0h, 006h, 0B0h, 0C6h, 020h, 0C8h, 0E2h, 007h, 01Ah, 001h, 00Bh, 002h
        db  00Ah, 001h, 0C4h, 0CEh, 0C7h, 0CEh, 0C5h, 0CEh, 0C6h, 0CEh, 0FBh, 004h, 009h, 002h, 000h, 004h
        db  048h, 0A2h, 0C9h, 0C6h, 020h, 0A8h, 092h, 008h, 012h, 001h, 020h, 0D8h, 0F2h, 007h, 000h, 001h
        db  047h, 0C2h, 0C4h, 081h, 001h, 014h, 044h, 0C2h, 0C9h, 061h, 009h, 0A2h, 089h, 0A1h, 001h, 017h
        db  085h, 005h, 009h, 061h, 0AEh, 016h, 082h, 002h, 03Ch, 008h, 005h, 016h, 040h, 001h, 010h, 000h
        db  026h, 013h, 060h, 004h, 03Ah, 0CFh, 060h, 004h, 040h, 0D0h, 0C1h, 006h, 0C1h, 0D8h, 00Fh, 000h
        db  0A0h, 006h, 09Ah, 0CDh, 020h, 0E0h, 006h, 0E0h, 043h, 0C2h, 081h, 007h, 058h, 0D2h, 060h, 004h
        db  06Ch, 0CEh, 020h, 020h, 012h, 0E0h, 005h, 016h, 081h, 007h, 044h, 000h, 020h, 020h, 012h, 0E0h
        db  0ECh, 013h, 060h, 004h, 018h, 0CFh, 081h, 007h, 080h, 020h, 0F8h, 010h, 081h, 007h, 080h, 020h
        db  0F0h, 010h, 081h, 007h, 002h, 020h, 0F2h, 010h, 081h, 007h, 004h, 020h, 0EFh, 010h, 023h, 0C8h
        db  008h, 000h, 06Ch, 001h, 007h, 005h, 0E0h, 0A1h, 0E8h, 000h, 00Ch, 002h, 002h, 0FCh, 0BCh, 007h
        db  000h, 081h, 007h, 0CFh, 0E0h, 0D2h, 003h, 001h, 00Bh, 016h, 0E0h, 0C2h, 03Ah, 007h, 0E0h, 042h
        db  062h, 007h, 0E0h, 026h, 03Ch, 0E3h, 002h, 013h, 0A0h, 006h, 08Ah, 0CDh, 0A0h, 006h, 0B0h, 0C6h
        db  023h, 0C8h, 006h, 000h, 06Ch, 001h, 0A3h, 0C2h, 00Eh, 000h, 04Ah, 001h, 000h, 001h, 00Bh, 013h
        db  00Ch, 002h, 00Eh, 0FCh, 05Ch, 0C2h, 049h, 002h, 000h, 080h, 00Dh, 002h, 01Ah, 009h, 07Dh, 0E2h
        db  009h, 0CFh, 03Dh, 0CFh, 03Dh, 0CFh, 00Ch, 002h, 000h, 0FCh, 06Ch, 0C3h, 006h, 000h, 04Dh, 002h
        db  0FFh, 0E0h, 04Ah, 002h, 000h, 002h, 08Ah, 0A2h, 08Ah, 0A2h, 04Ah, 0E3h, 00Dh, 0CBh, 006h, 000h
        db  0CDh, 006h, 0E0h, 0C2h, 044h, 008h, 0B7h, 016h, 00Bh, 002h, 00Fh, 000h, 0ECh, 082h, 004h, 000h
        db  0B2h, 011h, 0ECh, 0C3h, 00Eh, 000h, 011h, 015h, 010h, 013h, 06Ch, 0C2h, 014h, 000h, 049h, 002h
        db  000h, 01Fh, 0ACh, 013h, 0C9h, 006h, 089h, 002h, 012h, 000h, 0A8h, 01Bh, 049h, 001h, 001h, 000h
        db  0A5h, 013h, 0C9h, 0A2h, 0ECh, 082h, 004h, 000h, 0A1h, 011h, 04Dh, 0A3h, 0A2h, 018h, 014h, 011h
        db  060h, 001h, 018h, 009h, 000h, 080h, 018h, 013h, 01Dh, 009h, 0CCh, 0A2h, 0EBh, 0C2h, 008h, 000h
        db  07Bh, 009h, 04Bh, 002h, 01Eh, 000h, 0A0h, 0C3h, 0F0h, 006h, 0ABh, 023h, 004h, 0E0h, 094h, 016h
        db  060h, 027h, 040h, 0E3h, 091h, 016h, 04Dh, 0A3h, 04Dh, 0A3h, 04Dh, 0A3h, 0CDh, 006h, 04Dh, 002h
        db  007h, 000h, 00Dh, 088h, 0EEh, 006h, 00Dh, 015h, 0A3h, 007h, 00Ah, 000h, 004h, 002h, 090h, 003h
        db  07Fh, 040h, 053h, 02Fh, 0A0h, 005h, 046h, 008h, 0C3h, 004h, 0C0h, 001h, 000h, 004h, 060h, 004h
        db  03Eh, 0CFh, 060h, 001h, 018h, 009h, 000h, 080h, 0EFh, 013h, 001h, 002h, 008h, 020h, 060h, 004h
        db  03Ch, 0D1h, 08Dh, 007h, 000h, 010h, 020h, 020h, 00Ah, 0E0h, 00Ah, 016h, 08Dh, 007h, 000h, 008h
        db  020h, 020h, 00Eh, 0E0h, 005h, 013h, 08Dh, 007h, 000h, 040h, 04Fh, 001h, 001h, 000h, 001h, 016h
        db  05Bh, 004h, 020h, 0E0h, 00Eh, 0E0h, 0A0h, 007h, 0ACh, 008h, 000h, 080h, 00Eh, 0C8h, 0F4h, 007h
        db  00Fh, 0C8h, 0F6h, 007h, 00Eh, 0C8h, 014h, 008h, 00Fh, 0C8h, 016h, 008h, 0E0h, 004h, 002h, 008h
        db  0E0h, 004h, 008h, 008h, 0E0h, 002h, 098h, 007h, 020h, 040h, 04Eh, 0E3h, 020h, 007h, 02Eh, 006h
        db  060h, 004h, 02Eh, 0D6h, 000h, 070h, 04Bh, 0C0h, 0E0h, 004h, 02Eh, 006h, 00Bh, 010h, 020h, 0F0h
        db  04Dh, 0E3h, 002h, 010h, 020h, 0F0h, 04Ch, 0E3h, 04Bh, 0C0h, 0E0h, 004h, 02Eh, 006h, 0E0h, 001h
        db  062h, 007h, 040h, 000h, 020h, 0E8h, 048h, 0E3h, 062h, 007h, 020h, 004h, 07Ch, 0EAh, 040h, 001h
        db  000h, 020h, 004h, 013h, 0FAh, 010h, 040h, 001h, 000h, 040h, 0F7h, 016h, 020h, 007h, 02Eh, 006h
        db  020h, 050h, 052h, 0E3h, 051h, 004h, 020h, 024h, 054h, 0E3h, 0EFh, 013h, 0E0h, 080h, 020h, 008h
        db  003h, 013h, 0E0h, 0C2h, 020h, 008h, 01Bh, 02Eh, 003h, 0C3h, 00Ch, 0C8h, 06Ch, 001h, 0E0h, 0C2h
        db  002h, 0FCh, 04Bh, 001h, 000h, 001h, 003h, 013h, 020h, 0C3h, 000h, 0FCh, 0F6h, 010h, 0CCh, 080h
        db  003h, 013h, 020h, 0C8h, 000h, 0FCh, 0F8h, 005h, 0E0h, 004h, 000h, 0FCh, 020h, 024h, 050h, 0E3h
        db  00Bh, 013h, 08Bh, 007h, 001h, 000h, 020h, 0C3h, 060h, 0EAh, 00Bh, 0CBh, 018h, 000h, 003h, 0CBh
        db  014h, 000h, 020h, 004h, 060h, 0EAh, 001h, 010h, 013h, 02Eh, 020h, 040h, 054h, 0E3h, 020h, 007h
        db  02Eh, 006h, 060h, 004h, 02Eh, 0D6h, 0C1h, 010h, 0E0h, 002h, 058h, 007h, 08Dh, 007h, 0A8h, 008h
        db  0A0h, 006h, 086h, 0C4h, 0E0h, 002h, 098h, 007h, 05Bh, 004h, 0E0h, 002h, 058h, 007h, 020h, 020h
        db  006h, 0E0h, 010h, 016h, 020h, 0E0h, 01Ch, 0E0h, 0E0h, 002h, 098h, 007h, 0C0h, 001h, 000h, 080h
        db  0A4h, 010h, 020h, 0C1h, 0A8h, 008h, 020h, 021h, 014h, 0E0h, 006h, 016h, 020h, 0E8h, 01Ch, 0E0h
        db  058h, 007h, 080h, 003h, 0E0h, 002h, 098h, 007h, 020h, 0D8h, 0DCh, 007h, 017h, 001h, 08Fh, 007h
        db  08Eh, 0FFh, 00Fh, 0C8h, 004h, 001h, 020h, 0E8h, 006h, 0E0h, 058h, 007h, 080h, 001h, 000h, 080h
        db  05Bh, 004h, 0E0h, 0C2h, 020h, 008h, 0C3h, 082h, 003h, 013h, 0DBh, 02Dh, 003h, 0C8h, 020h, 008h
        db  003h, 0C8h, 06Ch, 001h, 060h, 0C2h, 002h, 0FCh, 049h, 001h, 000h, 001h, 002h, 016h, 060h, 004h
        db  072h, 0D5h, 0E0h, 0C0h, 0F8h, 005h, 0FDh, 013h, 000h, 003h, 002h, 000h, 0E0h, 0C0h, 0F8h, 005h
        db  003h, 0C8h, 06Ch, 001h, 020h, 0C8h, 000h, 0FCh, 0F8h, 005h, 000h, 003h, 00Fh, 000h, 003h, 0C8h
        db  06Ch, 001h, 088h, 007h, 002h, 0FCh, 078h, 0C2h, 0F8h, 0C1h, 049h, 001h, 000h, 001h, 04Dh, 016h
        db  009h, 001h, 000h, 05Eh, 029h, 016h, 049h, 001h, 002h, 000h, 00Bh, 016h, 060h, 001h, 012h, 008h
        db  000h, 002h, 00Ah, 016h, 027h, 002h, 004h, 000h, 007h, 088h, 02Ch, 009h, 005h, 012h, 027h, 002h
        db  0FCh, 0FFh, 0A0h, 001h, 012h, 008h, 000h, 002h, 0C7h, 0C1h, 037h, 015h, 0D3h, 02Dh, 0E0h, 0C0h
        db  020h, 008h, 007h, 0A8h, 01Eh, 008h, 007h, 0A8h, 01Ch, 008h, 00Ch, 015h, 020h, 0C8h, 014h, 008h
        db  0F4h, 007h, 020h, 0C8h, 016h, 008h, 0F6h, 007h, 020h, 0C8h, 018h, 008h, 014h, 008h, 020h, 0C8h
        db  01Ah, 008h, 016h, 008h, 060h, 004h, 072h, 0D5h, 0A0h, 006h, 0B0h, 0C6h, 0D3h, 02Dh, 0E0h, 0C2h
        db  020h, 008h, 0C3h, 082h, 001h, 013h, 0DBh, 02Dh, 020h, 088h, 016h, 008h, 010h, 008h, 00Dh, 016h
        db  020h, 088h, 014h, 008h, 00Eh, 008h, 009h, 016h, 0E0h, 004h, 01Ch, 008h, 082h, 007h, 0FCh, 007h
        db  004h, 061h, 0E0h, 004h, 01Eh, 008h, 060h, 004h, 036h, 0D6h, 020h, 0C8h, 00Eh, 008h, 0F4h, 007h
        db  020h, 0C8h, 010h, 008h, 0F6h, 007h, 060h, 004h, 02Eh, 0D6h, 007h, 0A8h, 01Eh, 008h, 004h, 0C1h
        db  019h, 016h, 082h, 002h, 00Eh, 008h, 008h, 016h, 060h, 001h, 0F6h, 007h, 001h, 000h, 002h, 016h
        db  0A0h, 006h, 0A0h, 0D7h, 0A0h, 006h, 020h, 0D7h, 032h, 0C1h, 007h, 011h, 072h, 0C1h, 092h, 0C1h
        db  082h, 007h, 00Eh, 008h, 004h, 0C1h, 006h, 016h, 0ECh, 010h, 072h, 0C1h, 0B2h, 0C1h, 084h, 001h
        db  000h, 080h, 0E7h, 013h, 0E0h, 0D2h, 003h, 001h, 00Ah, 016h, 020h, 001h, 03Ah, 007h, 000h, 070h
        db  004h, 013h, 0A0h, 006h, 0C4h, 0D2h, 020h, 007h, 02Eh, 006h, 0A0h, 006h, 0B0h, 0C6h, 020h, 0C8h
        db  0E0h, 007h, 01Ah, 001h, 00Bh, 002h, 00Ah, 001h, 0C4h, 0CEh, 0C7h, 0CEh, 0C5h, 0CEh, 0C6h, 0CEh
        db  0FBh, 004h, 009h, 002h, 000h, 004h, 048h, 0A2h, 0C9h, 0C6h, 003h, 0A8h, 012h, 001h, 020h, 0D8h
        db  0F3h, 007h, 000h, 001h, 047h, 0C2h, 0C4h, 081h, 001h, 014h, 044h, 0C2h, 009h, 0A8h, 01Ch, 008h
        db  009h, 061h, 009h, 0A2h, 089h, 0A1h, 001h, 017h, 085h, 005h, 0C9h, 061h, 0B8h, 016h, 040h, 001h
        db  040h, 000h, 015h, 016h, 087h, 007h, 020h, 000h, 0E0h, 061h, 01Ch, 008h, 0C4h, 081h, 008h, 01Ah
        db  007h, 0A8h, 01Eh, 008h, 007h, 0A8h, 01Ch, 008h, 007h, 061h, 087h, 0A1h, 001h, 017h, 085h, 005h
        db  080h, 001h, 040h, 000h, 003h, 0C8h, 06Ch, 001h, 0E0h, 0C1h, 004h, 0FCh, 09Eh, 010h, 060h, 004h
        db  0C2h, 0D3h, 0E0h, 0D2h, 003h, 001h, 00Ah, 016h, 020h, 001h, 03Ah, 007h, 000h, 070h, 004h, 013h
        db  0A0h, 006h, 0C4h, 0D2h, 020h, 007h, 02Eh, 006h, 0A0h, 006h, 0B0h, 0C6h, 090h, 003h, 03Fh, 040h
        db  0D3h, 02Dh, 060h, 001h, 0F6h, 007h, 001h, 000h, 002h, 016h, 0A0h, 006h, 0A0h, 0D7h, 060h, 0D2h
        db  012h, 008h, 089h, 001h, 000h, 0F1h, 0C9h, 001h, 000h, 070h, 020h, 088h, 016h, 008h, 010h, 008h
        db  004h, 016h, 020h, 088h, 014h, 008h, 00Eh, 008h, 014h, 013h, 089h, 001h, 000h, 010h, 08Dh, 007h
        db  01Ch, 008h, 09Dh, 007h, 000h, 050h, 0A0h, 0D2h, 0F1h, 007h, 08Ch, 007h, 002h, 000h, 0A0h, 0C3h
        db  014h, 008h, 0E0h, 0C3h, 016h, 008h, 02Fh, 002h, 004h, 000h, 001h, 017h, 08Eh, 005h, 0A0h, 006h
        db  054h, 0C6h, 08Dh, 007h, 0F8h, 007h, 049h, 0CFh, 060h, 0C7h, 01Eh, 008h, 04Dh, 006h, 0A0h, 0D2h
        db  0F1h, 007h, 08Ch, 007h, 004h, 000h, 0A0h, 0C3h, 00Eh, 008h, 0E0h, 0C3h, 010h, 008h, 0CCh, 0A3h
        db  001h, 017h, 08Eh, 005h, 0A0h, 006h, 054h, 0C6h, 020h, 0C8h, 014h, 008h, 0AEh, 008h, 020h, 0C8h
        db  016h, 008h, 0B0h, 008h, 009h, 001h, 000h, 00Ch, 00Ah, 013h, 049h, 001h, 000h, 004h, 005h, 016h
        db  0A0h, 006h, 068h, 0D3h, 0A0h, 006h, 0D4h, 0D2h, 002h, 010h, 0A0h, 006h, 068h, 0D3h, 0C0h, 001h
        db  080h, 000h, 0A0h, 006h, 020h, 0D7h, 0E0h, 0C2h, 042h, 007h, 02Eh, 013h, 0E0h, 0C2h, 02Eh, 006h
        db  02Bh, 013h, 0E0h, 002h, 058h, 007h, 08Fh, 007h, 0BFh, 0FFh, 00Fh, 02Ch, 0E0h, 002h, 098h, 007h
        db  0E0h, 0C0h, 05Ch, 007h, 003h, 0C8h, 020h, 008h, 003h, 0C8h, 06Ch, 001h, 0C3h, 0C2h, 0CBh, 0A2h
        db  0EBh, 0C2h, 032h, 00Ch, 015h, 013h, 00Bh, 0C8h, 000h, 0FCh, 00Bh, 0C3h, 04Bh, 0C3h, 00Bh, 0C8h
        db  06Ch, 001h, 0E0h, 0C2h, 000h, 0FCh, 0FAh, 016h, 000h, 003h, 002h, 000h, 020h, 0C8h, 0F8h, 005h
        db  000h, 0FCh, 002h, 016h, 00Dh, 0C8h, 0FAh, 005h, 00Ch, 0C8h, 0F8h, 005h, 000h, 003h, 00Fh, 000h
        db  040h, 001h, 020h, 000h, 020h, 016h, 01Fh, 010h, 0A0h, 0C3h, 02Eh, 006h, 003h, 013h, 0E0h, 0C0h
        db  0F8h, 005h, 012h, 016h, 020h, 0C3h, 014h, 009h, 002h, 013h, 020h, 004h, 058h, 0EAh, 04Fh, 02Eh
        db  0C0h, 001h, 000h, 080h, 0A0h, 001h, 062h, 007h, 000h, 080h, 08Eh, 0C3h, 003h, 013h, 0A0h, 001h
        db  062h, 007h, 040h, 000h, 060h, 004h, 0EAh, 0D2h, 003h, 0C8h, 06Ch, 001h, 020h, 0C8h, 000h, 0FCh
        db  0F8h, 005h, 003h, 0C8h, 020h, 008h, 060h, 001h, 018h, 009h, 000h, 004h, 002h, 013h, 060h, 004h
        db  0FEh, 0D3h, 08Ch, 007h, 00Eh, 000h, 003h, 0C8h, 06Ch, 001h, 020h, 0C2h, 00Eh, 0FCh, 00Ah, 015h
        db  009h, 013h, 020h, 0C2h, 014h, 0FCh, 048h, 002h, 000h, 01Fh, 0C8h, 006h, 088h, 002h, 012h, 000h
        db  0EEh, 01Bh, 008h, 0A3h, 088h, 007h, 002h, 0FCh, 078h, 0C2h, 0F8h, 0C1h, 007h, 083h, 0E7h, 01Ah
        db  0CCh, 061h, 007h, 0C8h, 004h, 0FCh, 0CCh, 0C1h, 0C0h, 001h, 040h, 000h, 060h, 004h, 00Ah, 0D4h
        db  00Bh, 0C1h, 0A0h, 0C2h, 0F0h, 007h, 020h, 0C3h, 028h, 009h, 08Dh, 007h, 0F4h, 007h, 0A0h, 0C3h
        db  0F4h, 007h, 0E0h, 0C3h, 0F6h, 007h, 0A0h, 006h, 054h, 0C6h, 020h, 0C8h, 014h, 008h, 018h, 008h
        db  020h, 0C8h, 016h, 008h, 01Ah, 008h, 00Eh, 0C8h, 014h, 008h, 00Fh, 0C8h, 016h, 008h, 082h, 007h
        db  0FCh, 007h, 0E0h, 004h, 01Ch, 008h, 040h, 001h, 080h, 000h, 006h, 016h, 00Eh, 0C8h, 00Eh, 008h
        db  00Fh, 0C8h, 010h, 008h, 0E0h, 004h, 01Eh, 008h, 0A0h, 006h, 0B0h, 0C6h, 0E0h, 0C2h, 0F8h, 007h
        db  00Dh, 011h, 00Eh, 0C8h, 0F4h, 007h, 00Fh, 0C8h, 0F6h, 007h, 020h, 0C8h, 018h, 008h, 014h, 008h
        db  020h, 0C8h, 01Ah, 008h, 016h, 008h, 0A0h, 006h, 0CEh, 0D2h, 0CBh, 010h, 040h, 001h, 080h, 000h
        db  002h, 016h, 00Bh, 0C8h, 012h, 008h, 0C4h, 0C2h, 004h, 061h, 080h, 001h, 080h, 000h, 05Bh, 004h
        db  08Bh, 0C0h, 0A0h, 0C2h, 0F0h, 007h, 08Ch, 007h, 004h, 000h, 08Dh, 007h, 0F4h, 007h, 0A0h, 0C3h
        db  014h, 008h, 0E0h, 0C3h, 016h, 008h, 0A0h, 006h, 08Eh, 0C6h, 060h, 001h, 0F6h, 007h, 001h, 000h
        db  004h, 013h, 0A0h, 007h, 0ACh, 008h, 000h, 080h, 052h, 004h, 0E0h, 0C3h, 0A8h, 008h, 0E0h, 023h
        db  014h, 0E0h, 007h, 016h, 020h, 0D0h, 004h, 0E0h, 020h, 0E8h, 01Ah, 0E0h, 058h, 007h, 060h, 004h
        db  0DAh, 0D2h, 0A0h, 007h, 0ACh, 008h, 000h, 040h, 020h, 0C8h, 014h, 008h, 0AEh, 008h, 020h, 0C8h
        db  016h, 008h, 0B0h, 008h, 0A0h, 006h, 068h, 0D3h, 0A0h, 006h, 0D4h, 0D2h, 0D2h, 010h, 0ADh, 0C2h
        db  002h, 000h, 06Dh, 0C2h, 000h, 000h, 005h, 016h, 0AAh, 007h, 002h, 000h, 036h, 007h, 09Ah, 02Ch
        db  080h, 003h, 0EAh, 02Ch, 002h, 000h, 041h, 0CBh, 000h, 000h, 080h, 003h, 02Dh, 0C3h, 018h, 000h
        db  0ACh, 007h, 002h, 000h, 036h, 007h, 020h, 04Bh, 098h, 0EAh, 00Ah, 000h, 020h, 0EBh, 092h, 0EAh
        db  00Ah, 000h, 09Ch, 02Eh, 080h, 003h, 0A0h, 0C2h, 022h, 0E0h, 060h, 004h, 07Ch, 0B3h, 0EDh, 0C0h
        db  018h, 000h, 0A0h, 006h, 06Eh, 0D8h, 080h, 003h, 044h, 0C2h, 0C3h, 0C0h, 002h, 013h, 0A0h, 006h
        db  06Eh, 0D8h, 019h, 0C3h, 009h, 0CBh, 018h, 000h, 0C9h, 005h, 019h, 0CBh, 016h, 000h, 04Ch, 0C2h
        db  02Ch, 002h, 01Ah, 000h, 00Dh, 0CFh, 00Eh, 0CFh, 00Fh, 0C7h, 099h, 000h, 05Bh, 004h, 08Ch, 007h
        db  0BCh, 008h, 09Ch, 0C2h, 0A0h, 022h, 014h, 0E0h, 006h, 013h, 0A0h, 0C2h, 058h, 007h, 0A0h, 022h
        db  020h, 0E0h, 001h, 016h, 080h, 003h, 003h, 0C1h, 0C3h, 004h, 08Ah, 007h, 004h, 000h, 084h, 0A2h
        db  03Ah, 0CFh, 03Ah, 0CFh, 03Ah, 0CFh, 03Ah, 0CFh, 03Ah, 0CFh, 0E0h, 002h, 058h, 007h, 08Dh, 007h
        db  0BCh, 008h, 00Bh, 0C8h, 0C2h, 007h, 0A0h, 006h, 086h, 0C4h, 0E0h, 0C2h, 0C2h, 007h, 020h, 0E0h
        db  020h, 0E0h, 0E0h, 002h, 0B8h, 007h, 05Bh, 004h, 02Dh, 0C3h, 018h, 000h, 08Ch, 0C2h, 060h, 0C2h
        db  06Ch, 001h, 00Ah, 0C8h, 06Ch, 001h, 0E0h, 0C2h, 000h, 0FCh, 002h, 013h, 08Bh, 0C2h, 0F9h, 010h
        db  009h, 0C8h, 06Ch, 001h, 08Bh, 007h, 0F8h, 005h, 05Bh, 0C2h, 00Ch, 013h, 0CBh, 005h, 05Bh, 0C2h
        db  0CAh, 0C6h, 0E0h, 0C2h, 06Ch, 001h, 009h, 0C8h, 06Ch, 001h, 00Ch, 0C8h, 000h, 0FCh, 00Bh, 0C8h
        db  06Ch, 001h, 002h, 010h, 0CCh, 0CEh, 0CAh, 0C6h, 0A0h, 0C2h, 0E0h, 000h, 0A0h, 022h, 01Ah, 0E0h
        db  006h, 016h, 020h, 0E8h, 004h, 0E0h, 03Ah, 007h, 020h, 048h, 01Ah, 0E0h, 0E0h, 000h, 080h, 003h
        db  0E0h, 0D3h, 0ADh, 0E3h, 0E0h, 0C1h, 0A8h, 006h, 005h, 016h, 007h, 002h, 0A2h, 006h, 0A0h, 006h
        db  0F8h, 0C1h, 00Bh, 016h, 0E0h, 0C1h, 0BAh, 006h, 020h, 016h, 007h, 002h, 0B4h, 006h, 0A0h, 006h
        db  0F8h, 0C1h, 01Bh, 013h, 007h, 002h, 0B8h, 006h, 002h, 010h, 007h, 002h, 0A6h, 006h, 060h, 0C1h
        db  002h, 0FCh, 025h, 0C8h, 00Ch, 000h, 002h, 0FCh, 0C5h, 0C9h, 00Ch, 000h, 0F5h, 0CDh, 0F5h, 0CDh
        db  0F5h, 0CDh, 0F5h, 0CDh, 0F5h, 0CDh, 0F5h, 0C5h, 0B7h, 001h, 028h, 000h, 027h, 002h, 0F4h, 0FFh
        db  0A7h, 007h, 004h, 000h, 058h, 0DAh, 097h, 02Eh, 0D5h, 010h, 000h, 003h, 002h, 000h, 0A0h, 006h
        db  010h, 0C2h, 000h, 003h, 00Fh, 000h, 020h, 02Ch, 03Ah, 0EDh, 0E0h, 093h, 0ADh, 0E3h, 003h, 016h
        db  081h, 002h, 016h, 000h, 0C7h, 016h, 021h, 0C1h, 0A2h, 0EAh, 054h, 004h, 0E0h, 093h, 010h, 0E0h
        db  003h, 016h, 0A0h, 0D2h, 0AAh, 0E3h, 00Bh, 010h, 0CFh, 0D3h, 009h, 016h, 0A0h, 023h, 008h, 0E0h
        db  006h, 016h, 084h, 007h, 020h, 000h, 004h, 0E8h, 0D2h, 006h, 0A0h, 0D2h, 00Ch, 0E0h, 060h, 004h
        db  0D0h, 0DAh, 060h, 004h, 040h, 0DDh, 07Bh, 010h, 022h, 0C1h, 004h, 000h, 0E2h, 004h, 002h, 000h
        db  054h, 004h, 002h, 0C8h, 06Ch, 001h, 082h, 0A0h, 022h, 0C8h, 032h, 00Ch, 000h, 0FCh, 002h, 002h
        db  000h, 0FCh, 0E0h, 093h, 0ACh, 0E3h, 070h, 013h, 0A0h, 006h, 0D2h, 0DBh, 0A0h, 048h, 004h, 0E0h
        db  00Eh, 000h, 085h, 002h, 007h, 000h, 00Eh, 013h, 00Eh, 001h, 003h, 000h, 00Bh, 013h, 0A0h, 023h
        db  022h, 0E0h, 003h, 016h, 0A0h, 0D2h, 00Eh, 0E0h, 002h, 010h, 0A0h, 0D2h, 0AAh, 0E3h, 08Eh, 001h
        db  003h, 000h, 05Ah, 010h, 005h, 0C8h, 0FCh, 006h, 0C3h, 0C0h, 053h, 016h, 0A0h, 043h, 010h, 0E0h
        db  022h, 088h, 00Eh, 000h, 01Ah, 009h, 00Ah, 016h, 022h, 088h, 010h, 000h, 01Ch, 009h, 006h, 016h
        db  022h, 088h, 012h, 000h, 01Eh, 009h, 002h, 016h, 0A0h, 0E3h, 010h, 0E0h, 085h, 002h, 009h, 000h
        db  002h, 013h, 0A0h, 006h, 0DEh, 0E1h, 045h, 0A1h, 065h, 0C1h, 0AEh, 0E3h, 055h, 004h, 062h, 0C0h
        db  004h, 000h, 022h, 0C8h, 006h, 000h, 06Ch, 001h, 082h, 002h, 048h, 004h, 002h, 01Bh, 0A0h, 043h
        db  00Ch, 0E0h, 022h, 0C1h, 00Eh, 000h, 051h, 004h, 042h, 0C0h, 0E1h, 004h, 002h, 000h, 0A2h, 0C0h
        db  00Ch, 000h, 022h, 0C1h, 00Ah, 000h, 020h, 021h, 018h, 0E0h, 007h, 013h, 0A1h, 0C8h, 00Ah, 000h
        db  00Ah, 000h, 0A1h, 0C8h, 008h, 000h, 008h, 000h, 0E2h, 010h, 022h, 0C8h, 006h, 000h, 06Ch, 001h
        db  0A0h, 006h, 09Ch, 0E0h, 060h, 004h, 0AEh, 0DAh, 002h, 0C8h, 0D4h, 006h, 062h, 0C1h, 002h, 000h
        db  065h, 0C1h, 0D0h, 0E3h, 055h, 004h, 00Bh, 010h, 085h, 007h, 0F4h, 003h, 0F5h, 004h, 060h, 0CDh
        db  018h, 0EDh, 0A0h, 006h, 0C8h, 0E2h, 0A0h, 0E3h, 00Ch, 0E0h, 0A0h, 02Eh, 0F4h, 003h, 060h, 004h
        db  014h, 0D9h, 0A0h, 006h, 068h, 0DFh, 00Ch, 010h, 0A0h, 006h, 09Ch, 0E0h, 009h, 010h, 0A0h, 006h
        db  050h, 0E2h, 006h, 010h, 0A0h, 006h, 09Ch, 0E0h, 003h, 0C8h, 0DCh, 008h, 0A0h, 0D2h, 0ACh, 0E3h
        db  0A0h, 006h, 042h, 0DBh, 0A0h, 092h, 026h, 0E0h, 00Ch, 016h, 0E0h, 0D3h, 026h, 0E0h, 0E0h, 023h
        db  014h, 0E0h, 00Ah, 013h, 00Ah, 0C1h, 0C4h, 083h, 007h, 013h, 0C4h, 0C3h, 024h, 0C1h, 0D4h, 0E3h
        db  054h, 004h, 0CAh, 093h, 0DCh, 013h, 0CAh, 0D3h, 0B0h, 003h, 00Fh, 0D8h, 059h, 006h, 004h, 071h
        db  024h, 0C1h, 0E0h, 0E3h, 054h, 004h, 0A0h, 023h, 00Ch, 0E0h, 0D1h, 013h, 04Dh, 0C3h, 0CFh, 013h
        db  086h, 007h, 002h, 000h, 084h, 007h, 026h, 000h, 046h, 023h, 003h, 013h, 044h, 006h, 086h, 0A1h
        db  0FBh, 010h, 046h, 043h, 0B9h, 010h, 085h, 007h, 01Ch, 007h, 086h, 007h, 01Ah, 004h, 076h, 06Dh
        db  076h, 06Dh, 076h, 06Dh, 0C6h, 005h, 076h, 06Dh, 076h, 06Dh, 076h, 06Dh, 083h, 007h, 000h, 090h
        db  0BEh, 010h, 00Bh, 0C3h, 086h, 007h, 000h, 001h, 085h, 007h, 000h, 080h, 020h, 0C1h, 0D2h, 006h
        db  037h, 013h, 0C4h, 004h, 060h, 0C0h, 0D2h, 006h, 045h, 020h, 004h, 013h, 084h, 005h, 015h, 009h
        db  0F9h, 016h, 02Eh, 010h, 0CFh, 0D3h, 006h, 016h, 0E0h, 023h, 014h, 0E0h, 003h, 016h, 00Eh, 001h
        db  003h, 000h, 003h, 013h, 0E0h, 004h, 0D2h, 006h, 023h, 010h, 064h, 0D0h, 01Ch, 007h, 046h, 0B0h
        db  010h, 018h, 001h, 0D9h, 01Ch, 007h, 060h, 023h, 020h, 0E0h, 00Bh, 013h, 081h, 007h, 018h, 000h
        db  061h, 0C0h, 0F0h, 0E3h, 011h, 088h, 018h, 0EDh, 004h, 013h, 008h, 002h, 018h, 080h, 0A0h, 006h
        db  01Ch, 0DFh, 064h, 0D0h, 028h, 007h, 046h, 0B0h, 008h, 018h, 001h, 0D9h, 028h, 007h, 046h, 0B0h
        db  004h, 017h, 083h, 007h, 040h, 080h, 0A0h, 006h, 050h, 0E2h, 005h, 048h, 0D2h, 006h, 0CAh, 016h
        db  020h, 0C1h, 0E4h, 008h, 001h, 016h, 05Ch, 004h, 004h, 002h, 007h, 000h, 020h, 006h, 0E4h, 008h
        db  0C9h, 010h, 00Bh, 0C3h, 0C5h, 004h, 042h, 0C0h, 0C7h, 004h, 020h, 0C2h, 06Ch, 001h, 0E1h, 0A1h
        db  004h, 000h, 011h, 0C8h, 06Ch, 001h, 0FBh, 016h, 008h, 0C8h, 06Ch, 001h, 0C8h, 004h, 0A0h, 043h
        db  01Ah, 0E0h, 022h, 0C1h, 00Eh, 000h, 00Dh, 015h, 00Ch, 013h, 0A0h, 0E3h, 01Ah, 0E0h, 0A0h, 006h
        db  03Ah, 0E2h, 008h, 0C2h, 048h, 013h, 088h, 002h, 012h, 000h, 045h, 01Bh, 020h, 022h, 022h, 0E0h
        db  042h, 013h, 002h, 0C1h, 008h, 0A1h, 008h, 005h, 028h, 002h, 0F2h, 0FFh, 007h, 0A2h, 083h, 007h
        db  001h, 080h, 088h, 002h, 004h, 000h, 06Eh, 011h, 064h, 0C2h, 016h, 000h, 049h, 0D2h, 002h, 016h
        db  002h, 081h, 031h, 016h, 009h, 001h, 000h, 0F0h, 028h, 016h, 049h, 0C1h, 045h, 071h, 0C3h, 004h
        db  085h, 002h, 009h, 000h, 07Ch, 013h, 083h, 007h, 002h, 080h, 0A4h, 0C1h, 014h, 000h, 088h, 081h
        db  076h, 016h, 083h, 005h, 085h, 002h, 010h, 000h, 013h, 01Bh, 083h, 005h, 049h, 099h, 0C2h, 0EAh
        db  00Ah, 013h, 009h, 098h, 00Eh, 0E0h, 06Bh, 016h, 025h, 098h, 0C2h, 0EAh, 00Ch, 0E0h, 067h, 016h
        db  0E0h, 0C1h, 0ECh, 006h, 064h, 016h, 0C3h, 004h, 052h, 0C2h, 00Fh, 013h, 083h, 007h, 009h, 080h
        db  0E0h, 0C1h, 018h, 009h, 047h, 001h, 000h, 010h, 05Ah, 016h, 0A0h, 0C0h, 06Ch, 001h, 0A0h, 006h
        db  0F4h, 0E0h, 060h, 004h, 0AEh, 0DAh, 060h, 004h, 0B8h, 0DAh, 089h, 007h, 00Eh, 007h, 0C7h, 004h
        db  0E5h, 0D1h, 0D3h, 0EAh, 005h, 013h, 0C7h, 006h, 027h, 002h, 0E4h, 0EAh, 077h, 0CEh, 0FEh, 015h
        db  044h, 0C0h, 021h, 002h, 018h, 000h, 028h, 002h, 0FCh, 0FFh, 036h, 013h, 091h, 0C1h, 086h, 0D1h
        db  01Fh, 013h, 0C6h, 006h, 087h, 007h, 00Eh, 007h, 0F7h, 0C0h, 046h, 002h, 0FFh, 0BFh, 043h, 002h
        db  0FFh, 03Fh, 0A0h, 091h, 03Fh, 0EDh, 009h, 016h, 0B0h, 003h, 020h, 098h, 00Eh, 0E0h, 05Dh, 006h
        db  00Fh, 016h, 021h, 0C8h, 002h, 000h, 00Ch, 007h, 017h, 010h, 047h, 082h, 00Ch, 01Bh, 0C6h, 090h
        db  0EBh, 016h, 047h, 006h, 0F7h, 004h, 0B0h, 003h, 020h, 098h, 05Dh, 006h, 057h, 006h, 00Ch, 013h
        db  083h, 007h, 005h, 080h, 01Ch, 010h, 0D1h, 0C0h, 0E0h, 020h, 016h, 0E0h, 003h, 016h, 083h, 007h
        db  008h, 080h, 015h, 010h, 060h, 044h, 026h, 0E0h, 086h, 071h, 046h, 0A0h, 006h, 062h, 083h, 007h
        db  005h, 080h, 008h, 0C2h, 0CBh, 015h, 00Bh, 016h, 0C3h, 004h, 087h, 007h, 00Eh, 007h, 077h, 0C0h
        db  047h, 082h, 005h, 01Bh, 060h, 020h, 006h, 0E0h, 0FAh, 016h, 083h, 007h, 007h, 080h, 05Ch, 004h
        db  0A0h, 092h, 00Eh, 0E0h, 00Fh, 016h, 020h, 0C8h, 020h, 0E0h, 008h, 007h, 0E0h, 004h, 084h, 001h
        db  060h, 005h, 002h, 007h, 03Bh, 013h, 020h, 048h, 006h, 0E0h, 082h, 001h, 083h, 007h, 000h, 0C0h
        db  0A0h, 006h, 050h, 0E2h, 020h, 0C8h, 01Eh, 0E0h, 002h, 007h, 0A0h, 0E3h, 004h, 0E0h, 008h, 002h
        db  024h, 080h, 0A0h, 006h, 01Ch, 0DFh, 034h, 010h, 020h, 0C1h, 084h, 001h, 044h, 002h, 000h, 088h
        db  01Eh, 013h, 004h, 048h, 084h, 001h, 020h, 006h, 002h, 007h, 0F1h, 016h, 0A0h, 023h, 022h, 0E0h
        db  005h, 016h, 0A0h, 043h, 022h, 0E0h, 0A0h, 0D2h, 00Eh, 0E0h, 0D5h, 010h, 0E0h, 023h, 014h, 0E0h
        db  004h, 013h, 020h, 098h, 0ABh, 0E3h, 065h, 006h, 006h, 016h, 0A0h, 092h, 00Eh, 0E0h, 0CBh, 013h
        db  0A0h, 0D2h, 0AAh, 0E3h, 0D7h, 010h, 083h, 007h, 002h, 000h, 060h, 004h, 0C8h, 0DAh, 020h, 0E8h
        db  006h, 0E0h, 082h, 001h, 020h, 007h, 002h, 007h, 0A0h, 043h, 004h, 0E0h, 020h, 0C8h, 070h, 0E4h
        db  086h, 001h, 020h, 088h, 020h, 0E0h, 008h, 007h, 003h, 016h, 020h, 0C8h, 0FCh, 0EAh, 008h, 007h
        db  060h, 004h, 0D0h, 0DAh, 00Eh, 001h, 003h, 000h, 016h, 013h, 0CFh, 0D3h, 008h, 016h, 0A0h, 023h
        db  020h, 0E0h, 003h, 016h, 0A0h, 0D2h, 0AAh, 0E3h, 002h, 010h, 0A0h, 0D2h, 00Eh, 0E0h, 08Eh, 001h
        db  003h, 000h, 009h, 010h, 060h, 0C1h, 084h, 001h, 060h, 021h, 00Ah, 0E0h, 004h, 016h, 083h, 007h
        db  000h, 084h, 060h, 004h, 0C8h, 0DAh, 020h, 0C8h, 02Eh, 0E0h, 084h, 001h, 008h, 002h, 006h, 080h
        db  0A0h, 006h, 01Ch, 0DFh, 060h, 004h, 0D0h, 0DAh, 060h, 0E3h, 020h, 0E0h, 060h, 004h, 0D0h, 0DAh
        db  0E0h, 093h, 026h, 0E0h, 010h, 016h, 0A0h, 023h, 008h, 0E0h, 00Dh, 016h, 0A0h, 023h, 006h, 0E0h
        db  002h, 013h, 060h, 0E3h, 01Ch, 0E0h, 060h, 0E3h, 018h, 0E0h, 0A0h, 043h, 006h, 0E0h, 008h, 002h
        db  03Ch, 080h, 0A0h, 006h, 01Ch, 0DFh, 060h, 004h, 0D0h, 0DAh, 0A0h, 092h, 0AAh, 0E3h, 003h, 013h
        db  0A0h, 092h, 0ABh, 0E3h, 01Eh, 016h, 0E0h, 023h, 014h, 0E0h, 008h, 013h, 020h, 098h, 0ABh, 0E3h
        db  065h, 006h, 004h, 013h, 083h, 007h, 007h, 000h, 060h, 004h, 0C8h, 0DAh, 0A0h, 0D2h, 00Eh, 0E0h
        db  020h, 0C8h, 020h, 0E0h, 008h, 007h, 0A0h, 027h, 004h, 0E0h, 00Bh, 016h, 020h, 0C8h, 01Eh, 0E0h
        db  008h, 007h, 0E0h, 093h, 0AAh, 0E3h, 005h, 016h, 0A0h, 023h, 012h, 0E0h, 002h, 013h, 020h, 006h
        db  008h, 007h, 060h, 004h, 0D0h, 0DAh, 0E0h, 023h, 014h, 0E0h, 036h, 013h, 0B0h, 003h, 020h, 098h
        db  00Eh, 0E0h, 06Fh, 006h, 00Fh, 016h, 0CFh, 0D3h, 02Fh, 016h, 0A0h, 0D2h, 0AAh, 0E3h, 060h, 004h
        db  0D0h, 0DAh, 0A0h, 092h, 00Ch, 0E0h, 028h, 016h, 0E0h, 023h, 014h, 0E0h, 0F6h, 013h, 083h, 007h
        db  006h, 000h, 007h, 010h, 083h, 007h, 005h, 000h, 0E0h, 093h, 00Eh, 0E0h, 002h, 016h, 083h, 007h
        db  007h, 000h, 060h, 004h, 0C8h, 0DAh, 060h, 0E3h, 012h, 0E0h, 0E0h, 023h, 014h, 0E0h, 00Dh, 013h
        db  020h, 098h, 00Ch, 0E0h, 065h, 006h, 003h, 016h, 020h, 0D8h, 0ABh, 0E3h, 065h, 006h, 00Ch, 010h
        db  020h, 0C1h, 084h, 001h, 020h, 021h, 006h, 0E0h, 0D6h, 016h, 020h, 048h, 006h, 0E0h, 084h, 001h
        db  008h, 002h, 030h, 080h, 0A0h, 006h, 01Ch, 0DFh, 060h, 004h, 0D0h, 0DAh, 0A0h, 0E3h, 00Ah, 0E0h
        db  008h, 0C2h, 002h, 011h, 0A0h, 043h, 00Ah, 0E0h, 020h, 042h, 004h, 0E0h, 028h, 002h, 0F0h, 0E3h
        db  058h, 0C0h, 002h, 0C0h, 011h, 088h, 018h, 0EDh, 003h, 016h, 0D1h, 02Ch, 058h, 0C0h, 0D1h, 004h
        db  080h, 0C0h, 00Eh, 001h, 000h, 010h, 00Fh, 013h, 060h, 0CCh, 018h, 0EDh, 0C8h, 005h, 078h, 0CCh
        db  003h, 016h, 041h, 006h, 060h, 0CCh, 0D6h, 006h, 058h, 0C4h, 002h, 016h, 060h, 0C4h, 000h, 007h
        db  021h, 002h, 0FAh, 0FFh, 091h, 02Ch, 05Bh, 004h, 00Bh, 0C3h, 0A0h, 006h, 004h, 0E0h, 0A0h, 006h
        db  0DEh, 0DFh, 008h, 0C2h, 005h, 016h, 062h, 0C2h, 002h, 000h, 060h, 026h, 06Ah, 0E4h, 00Dh, 016h
        db  042h, 0C2h, 0C9h, 005h, 060h, 0CEh, 03Ch, 0EDh, 060h, 0C6h, 0FEh, 0EAh, 0A0h, 006h, 046h, 0E0h
        db  018h, 0CAh, 00Ah, 000h, 020h, 046h, 026h, 0E0h, 004h, 016h, 0A0h, 0C0h, 06Ch, 001h, 012h, 02Eh
        db  01Dh, 010h, 012h, 0C1h, 005h, 013h, 060h, 0C1h, 06Ch, 001h, 014h, 02Eh, 005h, 0C8h, 06Ch, 001h
        db  0D2h, 004h, 048h, 006h, 084h, 007h, 002h, 000h, 048h, 0C1h, 0A0h, 0C0h, 06Ch, 001h, 002h, 0C0h
        db  0A0h, 006h, 0C8h, 0E2h, 060h, 0C5h, 002h, 0FCh, 007h, 002h, 0A2h, 006h, 025h, 002h, 0F4h, 0FFh
        db  005h, 0C8h, 002h, 0FCh, 020h, 0C2h, 06Ch, 001h, 0A0h, 006h, 0BCh, 0C1h, 05Ch, 004h, 042h, 0C2h
        db  029h, 002h, 008h, 000h, 039h, 0C2h, 048h, 002h, 000h, 0C0h, 088h, 002h, 000h, 0C0h, 008h, 016h
        db  060h, 08Eh, 02Eh, 0E0h, 005h, 016h, 060h, 086h, 02Eh, 0E0h, 002h, 016h, 0C8h, 004h, 05Bh, 004h
        db  008h, 007h, 05Bh, 004h, 020h, 088h, 090h, 0E1h, 06Ch, 001h, 002h, 016h, 060h, 004h, 0B8h, 0DAh
        db  05Bh, 004h, 088h, 007h, 0AEh, 001h, 020h, 0E8h, 00Eh, 0E0h, 080h, 001h, 008h, 006h, 0FEh, 016h
        db  020h, 048h, 00Eh, 0E0h, 080h, 001h, 05Bh, 004h, 0C2h, 004h, 0A0h, 023h, 00Ch, 0E0h, 00Ah, 016h
        db  020h, 02Fh, 030h, 006h, 082h, 007h, 0DFh, 0FFh, 002h, 02Ch, 082h, 002h, 0F4h, 003h, 0FAh, 016h
        db  0A0h, 043h, 00Ch, 0E0h, 05Bh, 004h, 042h, 0C2h, 088h, 007h, 00Eh, 000h, 009h, 0A2h, 029h, 002h
        db  008h, 000h, 078h, 0CEh, 078h, 0CEh, 078h, 0CEh, 060h, 0CEh, 01Ah, 009h, 060h, 0CEh, 01Ch, 009h
        db  060h, 0CEh, 01Eh, 009h, 0A0h, 023h, 01Ah, 0E0h, 00Fh, 016h, 058h, 0C2h, 049h, 002h, 080h, 01Fh
        db  060h, 02Ah, 014h, 0E0h, 0A0h, 0E8h, 004h, 0E0h, 00Eh, 000h, 009h, 0C6h, 049h, 002h, 000h, 01Fh
        db  0C9h, 006h, 009h, 0A2h, 089h, 0A8h, 004h, 000h, 028h, 002h, 002h, 000h, 058h, 0C2h, 049h, 00Ah
        db  049h, 002h, 000h, 0F0h, 009h, 0D6h, 0E2h, 004h, 006h, 000h, 05Bh, 004h, 000h, 007h, 082h, 0C0h
        db  053h, 013h, 0A0h, 0C0h, 06Ch, 001h, 0A0h, 0C1h, 006h, 0FCh, 046h, 002h, 00Fh, 000h, 086h, 002h
        db  001h, 000h, 03Dh, 012h, 006h, 088h, 0F2h, 006h, 012h, 016h, 001h, 002h, 00Eh, 0FCh, 031h, 088h
        db  0F4h, 006h, 00Dh, 016h, 031h, 088h, 0F6h, 006h, 00Ah, 016h, 031h, 088h, 0F8h, 006h, 007h, 016h
        db  086h, 002h, 002h, 000h, 02Ch, 016h, 020h, 088h, 00Ah, 007h, 0FAh, 006h, 028h, 013h, 020h, 0C1h
        db  018h, 009h, 044h, 001h, 000h, 008h, 006h, 013h, 086h, 002h, 002h, 000h, 020h, 016h, 044h, 001h
        db  080h, 000h, 01Dh, 016h, 000h, 007h, 0E0h, 023h, 014h, 0E0h, 019h, 016h, 082h, 002h, 043h, 000h
        db  016h, 013h, 000h, 002h, 002h, 0FCh, 040h, 0C0h, 0B0h, 001h, 020h, 000h, 060h, 001h, 018h, 009h
        db  001h, 000h, 007h, 016h, 060h, 0A0h, 0DEh, 008h, 060h, 0CCh, 0EEh, 005h, 050h, 0C4h, 020h, 0C4h
        db  0DEh, 008h, 080h, 007h, 036h, 007h, 081h, 007h, 040h, 000h, 040h, 02Ch, 0C0h, 004h, 084h, 007h
        db  0F2h, 006h, 006h, 0CDh, 001h, 002h, 00Eh, 0FCh, 031h, 0CDh, 031h, 0CDh, 031h, 0CDh, 020h, 0C5h
        db  00Ah, 007h, 000h, 0C0h, 001h, 013h, 012h, 02Eh, 0E0h, 004h, 06Ch, 001h, 05Bh, 004h, 00Bh, 0C8h
        db  0D4h, 008h, 0A0h, 006h, 06Ch, 0E1h, 008h, 002h, 042h, 080h, 0A0h, 006h, 01Ch, 0DFh, 008h, 002h
        db  030h, 080h, 0A0h, 006h, 01Ch, 0DFh, 0E0h, 0C2h, 0D4h, 008h, 05Bh, 004h, 00Bh, 0C3h, 020h, 048h
        db  000h, 0EBh, 082h, 001h, 020h, 048h, 06Eh, 0E4h, 080h, 001h, 020h, 048h, 022h, 0E0h, 0AEh, 001h
        db  020h, 048h, 022h, 0E0h, 026h, 009h, 060h, 043h, 018h, 0E0h, 0A0h, 043h, 008h, 0E0h, 008h, 002h
        db  042h, 000h, 0A0h, 006h, 01Ch, 0DFh, 05Ch, 004h, 00Bh, 0C3h, 020h, 0E8h, 00Eh, 0E0h, 082h, 001h
        db  020h, 0E8h, 022h, 0E0h, 0AEh, 001h, 020h, 0E8h, 022h, 0E0h, 026h, 009h, 0A0h, 0E3h, 008h, 0E0h
        db  060h, 0E3h, 018h, 0E0h, 0A0h, 043h, 006h, 0E0h, 008h, 002h, 03Ch, 080h, 0A0h, 006h, 01Ch, 0DFh
        db  008h, 002h, 042h, 080h, 0A0h, 006h, 01Ch, 0DFh, 05Ch, 004h, 00Bh, 0C3h, 083h, 007h, 000h, 068h
        db  0A0h, 006h, 050h, 0E2h, 083h, 007h, 010h, 080h, 0A0h, 006h, 050h, 0E2h, 05Ch, 004h, 00Bh, 0C3h
        db  0A0h, 006h, 03Ah, 0E2h, 002h, 0A2h, 068h, 0C2h, 014h, 000h, 029h, 002h, 0FCh, 0FFh, 024h, 013h
        db  028h, 002h, 018h, 000h, 087h, 007h, 00Eh, 000h, 081h, 007h, 00Eh, 007h, 0F1h, 004h, 047h, 006h
        db  0FDh, 015h, 058h, 0C0h, 0B0h, 003h, 001h, 078h, 063h, 006h, 041h, 002h, 03Fh, 000h, 00Eh, 013h
        db  081h, 002h, 01Fh, 000h, 00Bh, 01Bh, 041h, 0A0h, 061h, 0C0h, 050h, 0E4h, 0F8h, 0C1h, 0C7h, 006h
        db  0C7h, 071h, 047h, 006h, 078h, 0CCh, 047h, 006h, 0FDh, 015h, 004h, 010h, 058h, 0C0h, 0C1h, 006h
        db  041h, 070h, 001h, 0A2h, 049h, 0C2h, 0E5h, 015h, 05Ch, 004h, 0A0h, 023h, 01Ah, 0E0h, 002h, 013h
        db  0C8h, 004h, 05Bh, 004h, 022h, 0C2h, 014h, 000h, 048h, 002h, 000h, 01Fh, 0C8h, 006h, 05Bh, 004h
        db  083h, 002h, 00Fh, 000h, 017h, 01Bh, 0A0h, 0C1h, 0D4h, 006h, 035h, 013h, 026h, 002h, 004h, 000h
        db  0A0h, 0CDh, 018h, 0EDh, 083h, 0C5h, 004h, 013h, 04Ah, 0C2h, 039h, 00Ah, 0C9h, 0E0h, 083h, 0C5h
        db  086h, 007h, 036h, 007h, 087h, 007h, 010h, 000h, 020h, 0C2h, 0D4h, 006h, 0E0h, 004h, 0D4h, 006h
        db  046h, 02Ch, 05Bh, 004h, 060h, 0C0h, 0FEh, 006h, 020h, 0C2h, 018h, 009h, 048h, 002h, 000h, 060h
        db  020h, 022h, 006h, 0E0h, 004h, 016h, 020h, 0E2h, 00Ah, 0E0h, 020h, 0E2h, 018h, 0E0h, 013h, 00Ah
        db  004h, 018h, 041h, 005h, 003h, 048h, 0FEh, 006h, 006h, 010h, 083h, 002h, 002h, 000h, 001h, 016h
        db  013h, 009h, 003h, 0E8h, 0FEh, 006h, 0C8h, 040h, 0C1h, 040h, 005h, 013h, 088h, 007h, 036h, 007h
        db  089h, 007h, 000h, 040h, 048h, 02Ch, 05Bh, 004h, 0C9h, 004h, 024h, 0C1h, 016h, 0EBh, 084h, 0C1h
        db  086h, 071h, 026h, 002h, 0C8h, 0EBh, 0C4h, 006h, 004h, 071h, 024h, 002h, 03Eh, 0EBh, 014h, 0D2h
        db  0C8h, 009h, 008h, 0A2h, 0B0h, 003h, 034h, 0D8h, 05Fh, 006h, 047h, 002h, 00Fh, 000h, 0C7h, 0A1h
        db  028h, 0C2h, 004h, 0EBh, 058h, 004h, 076h, 0CDh, 047h, 006h, 0FDh, 016h, 032h, 010h, 036h, 0C2h
        db  026h, 010h, 017h, 009h, 047h, 0A1h, 02Dh, 010h, 017h, 009h, 047h, 061h, 02Ah, 010h, 0A0h, 043h
        db  016h, 0E0h, 05Bh, 004h, 0A0h, 043h, 016h, 0E0h, 049h, 0C2h, 003h, 016h, 044h, 0C2h, 006h, 0C8h
        db  0D4h, 008h, 027h, 0C1h, 0D8h, 0ECh, 084h, 0C1h, 086h, 071h, 026h, 002h, 00Eh, 0EDh, 0C4h, 006h
        db  004h, 071h, 024h, 002h, 0F4h, 0ECh, 0D3h, 010h, 009h, 0C1h, 0A0h, 0C1h, 0D4h, 008h, 0C9h, 004h
        db  010h, 010h, 036h, 0C2h, 078h, 0D5h, 060h, 041h, 022h, 0E0h, 0C5h, 005h, 00Ah, 010h, 078h, 0CDh
        db  047h, 006h, 0FDh, 015h, 006h, 010h, 0A0h, 023h, 016h, 0E0h, 0CDh, 016h, 049h, 0C2h, 0ECh, 016h
        db  0D6h, 010h, 0A0h, 0E3h, 016h, 0E0h, 0BBh, 010h, 008h, 002h, 05Ah, 080h, 0A0h, 006h, 01Ch, 0DFh
        db  03Eh, 010h, 0A0h, 092h, 00Ch, 0E0h, 015h, 016h, 044h, 002h, 000h, 05Eh, 014h, 016h, 020h, 048h
        db  06Eh, 0E4h, 080h, 001h, 0A0h, 006h, 098h, 0E1h, 020h, 0C8h, 09Eh, 001h, 09Eh, 001h, 0E0h, 02Eh
        db  001h, 000h, 0A0h, 043h, 018h, 0E0h, 0A0h, 0D2h, 026h, 0E0h, 083h, 007h, 010h, 000h, 0A0h, 006h
        db  050h, 0E2h, 060h, 004h, 0D0h, 0DAh, 084h, 007h, 008h, 000h, 060h, 004h, 098h, 0DAh, 085h, 007h
        db  003h, 002h, 005h, 0C8h, 0CEh, 006h, 0A0h, 043h, 012h, 0E0h, 0E0h, 004h, 0FAh, 006h, 0A0h, 006h
        db  0CAh, 0E1h, 008h, 002h, 048h, 080h, 0A0h, 006h, 01Ch, 0DFh, 011h, 010h, 0A0h, 027h, 02Ch, 0E0h
        db  004h, 016h, 008h, 002h, 054h, 080h, 0A0h, 006h, 01Ch, 0DFh, 083h, 007h, 000h, 0A8h, 020h, 088h
        db  008h, 007h, 020h, 0E0h, 002h, 016h, 083h, 007h, 000h, 0E8h, 0A0h, 006h, 050h, 0E2h, 008h, 002h
        db  036h, 000h, 0A0h, 006h, 01Ch, 0DFh, 020h, 0E8h, 00Ch, 0E0h, 082h, 001h, 0A0h, 023h, 018h, 0E0h
        db  006h, 013h, 0A0h, 0E3h, 018h, 0E0h, 0E0h, 02Eh, 000h, 000h, 041h, 0C0h, 0FAh, 016h, 0A0h, 006h
        db  028h, 0E0h, 082h, 0C0h, 002h, 013h, 04Fh, 002h, 080h, 0FFh, 0C4h, 004h, 00Fh, 0D1h, 0C4h, 006h
        db  060h, 004h, 098h, 0DAh, 0A0h, 006h, 04Ah, 0E4h, 008h, 002h, 036h, 080h, 0A0h, 007h, 0D6h, 006h
        db  038h, 0E4h, 0A0h, 006h, 01Ch, 0DFh, 010h, 010h, 0A0h, 006h, 04Ah, 0E4h, 020h, 0D1h, 0CEh, 006h
        db  0E6h, 013h, 020h, 078h, 012h, 0E0h, 0CEh, 006h, 0E2h, 010h, 020h, 0C1h, 016h, 004h, 014h, 00Ah
        db  0C4h, 006h, 00Ah, 091h, 001h, 016h, 05Bh, 004h, 060h, 004h, 0D0h, 0DAh, 0CAh, 004h, 0CDh, 004h
        db  0CEh, 004h, 0CFh, 004h, 0E0h, 004h, 0A8h, 006h, 0E0h, 004h, 0BAh, 006h, 084h, 007h, 0A0h, 001h
        db  085h, 007h, 010h, 000h, 0F4h, 004h, 045h, 006h, 0FDh, 015h, 084h, 007h, 0D8h, 006h, 085h, 007h
        db  034h, 007h, 044h, 061h, 0F4h, 004h, 045h, 006h, 0FDh, 015h, 084h, 007h, 0C8h, 000h, 004h, 0C8h
        db  000h, 007h, 084h, 007h, 0FFh, 07Fh, 004h, 0C8h, 0F0h, 006h, 084h, 007h, 006h, 000h, 004h, 0C8h
        db  0EEh, 006h, 0B0h, 003h, 020h, 0D8h, 010h, 0E0h, 065h, 006h, 060h, 004h, 0D0h, 0DAh, 020h, 0F8h
        db  063h, 0EDh, 082h, 001h, 020h, 0C8h, 010h, 0E0h, 0C6h, 006h, 020h, 0C8h, 020h, 0E0h, 0C8h, 006h
        db  0E0h, 02Eh, 000h, 000h, 0A0h, 006h, 028h, 0E0h, 020h, 0C8h, 01Ah, 009h, 0A0h, 001h, 020h, 0C8h
        db  01Ch, 009h, 0A2h, 001h, 020h, 0C8h, 01Eh, 009h, 0A4h, 001h, 020h, 0C8h, 01Ch, 009h, 0B0h, 001h
        db  020h, 0C8h, 01Eh, 009h, 0B2h, 001h, 020h, 0C8h, 01Eh, 009h, 0CCh, 006h, 020h, 0F8h, 062h, 0EDh
        db  080h, 001h, 0A0h, 006h, 012h, 0E0h, 020h, 048h, 004h, 0E0h, 080h, 001h, 0E0h, 004h, 09Eh, 001h
        db  008h, 002h, 012h, 080h, 0A0h, 006h, 01Ch, 0DFh, 084h, 007h, 00Ah, 000h, 085h, 007h, 0F4h, 003h
        db  020h, 088h, 0C6h, 006h, 020h, 0E0h, 004h, 01Bh, 084h, 007h, 01Ch, 000h, 085h, 007h, 0F8h, 003h
        db  0A0h, 006h, 0C8h, 0E2h, 085h, 007h, 03Ch, 0E5h, 005h, 0C8h, 0F8h, 003h, 0A0h, 023h, 00Ch, 0E0h
        db  023h, 013h, 0A0h, 0E3h, 00Ch, 0E0h, 0A0h, 02Eh, 0F4h, 003h, 01Eh, 010h, 0B0h, 003h, 020h, 098h
        db  010h, 0E0h, 065h, 006h, 019h, 016h, 044h, 002h, 000h, 05Eh, 004h, 016h, 020h, 006h, 0C6h, 006h
        db  0D7h, 016h, 008h, 010h, 0B0h, 003h, 020h, 098h, 010h, 0E0h, 065h, 006h, 00Dh, 016h, 020h, 006h
        db  0C8h, 006h, 0C4h, 016h, 00Eh, 001h, 003h, 000h, 003h, 013h, 083h, 007h, 000h, 082h, 002h, 010h
        db  083h, 007h, 001h, 000h, 060h, 004h, 0C8h, 0DAh, 060h, 004h, 0AEh, 0DAh, 00Eh, 001h, 003h, 000h
        db  00Ah, 013h, 008h, 002h, 00Ch, 080h, 0A0h, 006h, 01Ch, 0DFh, 0E0h, 0E3h, 014h, 0E0h, 020h, 0C8h
        db  070h, 0E4h, 086h, 001h, 026h, 010h, 020h, 048h, 00Ch, 0E0h, 082h, 001h, 0E0h, 02Eh, 001h, 000h
        db  060h, 0C1h, 0D0h, 008h, 035h, 00Ah, 005h, 0E8h, 082h, 001h, 020h, 0C1h, 018h, 009h, 004h, 001h
        db  006h, 000h, 006h, 013h, 020h, 0D8h, 0D2h, 0E1h, 0E1h, 008h, 020h, 0D8h, 0D2h, 0E1h, 083h, 001h
        db  020h, 021h, 022h, 0E0h, 003h, 016h, 020h, 0E8h, 022h, 0E0h, 080h, 001h, 020h, 021h, 004h, 0E0h
        db  004h, 016h, 0A0h, 0E3h, 014h, 0E0h, 060h, 004h, 0BAh, 0DEh, 008h, 002h, 000h, 080h, 0A0h, 006h
        db  01Ch, 0DFh, 020h, 0E8h, 008h, 0E0h, 082h, 001h, 060h, 004h, 0AEh, 0DAh, 020h, 0C8h, 070h, 0E4h
        db  086h, 001h, 008h, 002h, 000h, 080h, 0A0h, 006h, 01Ch, 0DFh, 020h, 0C2h, 0D0h, 008h, 008h, 0A2h
        db  008h, 005h, 028h, 0C8h, 022h, 0E0h, 0CAh, 006h, 020h, 0C8h, 020h, 0E0h, 0C6h, 006h, 020h, 0C8h
        db  020h, 0E0h, 0C8h, 006h, 060h, 0E3h, 016h, 0E0h, 060h, 004h, 0D0h, 0DAh, 044h, 0C1h, 044h, 002h
        db  000h, 05Eh, 0F8h, 016h, 060h, 025h, 06Ah, 0E4h, 00Fh, 016h, 020h, 006h, 0C6h, 006h, 0F2h, 016h
        db  020h, 006h, 0CAh, 006h, 003h, 013h, 0A0h, 005h, 0CCh, 006h, 0E6h, 010h, 0B0h, 003h, 020h, 0D8h
        db  00Ch, 0E0h, 065h, 006h, 060h, 004h, 0D0h, 0DAh, 020h, 006h, 0C8h, 006h, 0E3h, 016h, 020h, 088h
        db  01Eh, 009h, 0CCh, 006h, 003h, 016h, 083h, 007h, 008h, 000h, 002h, 010h, 083h, 007h, 00Ch, 000h
        db  060h, 004h, 0C8h, 0DAh, 060h, 004h, 0D0h, 0DAh, 0A0h, 023h, 008h, 0E0h, 003h, 013h, 060h, 023h
        db  012h, 0E0h, 006h, 016h, 0B0h, 003h, 020h, 0D8h, 0ABh, 0E3h, 065h, 006h, 060h, 004h, 0D0h, 0DAh
        db  008h, 002h, 000h, 080h, 0A0h, 006h, 01Ch, 0DFh, 060h, 004h, 0AEh, 0DAh, 008h, 002h, 000h, 080h
        db  0A0h, 006h, 01Ch, 0DFh, 020h, 0C8h, 01Eh, 0E0h, 0C6h, 006h, 020h, 0C8h, 01Eh, 0E0h, 0C8h, 006h
        db  060h, 0E3h, 010h, 0E0h, 060h, 004h, 0D0h, 0DAh, 0E0h, 023h, 014h, 0E0h, 019h, 013h, 044h, 0C1h
        db  044h, 002h, 000h, 01Eh, 0F5h, 016h, 060h, 025h, 06Ah, 0E4h, 006h, 016h, 020h, 006h, 0C8h, 006h
        db  0EFh, 016h, 0C2h, 004h, 060h, 004h, 01Ah, 0EBh, 008h, 002h, 01Eh, 080h, 0A0h, 006h, 01Ch, 0DFh
        db  007h, 010h, 020h, 006h, 0C6h, 006h, 0E4h, 016h, 083h, 007h, 009h, 000h, 0A0h, 006h, 0C8h, 0DAh
        db  060h, 004h, 0AEh, 0DAh, 0CEh, 004h, 0E0h, 004h, 0DCh, 008h, 0E0h, 0D3h, 0ACh, 0E3h, 08Fh, 0C2h
        db  020h, 0C8h, 072h, 0E4h, 086h, 001h, 020h, 048h, 008h, 0E0h, 082h, 001h, 086h, 007h, 005h, 000h
        db  084h, 007h, 072h, 006h, 054h, 0C1h, 001h, 013h, 0D4h, 02Ch, 024h, 002h, 00Ah, 000h, 006h, 006h
        db  0F9h, 016h, 008h, 002h, 02Ah, 080h, 0A0h, 006h, 01Ch, 0DFh, 020h, 02Ch, 01Ah, 0E0h, 060h, 004h
        db  07Ah, 0D9h, 0A0h, 006h, 06Ch, 0E1h, 0CDh, 004h, 0A0h, 023h, 01Ch, 0E0h, 00Dh, 013h, 00Eh, 001h
        db  003h, 000h, 00Ah, 013h, 0A0h, 0E3h, 01Ch, 0E0h, 0B0h, 003h, 020h, 0D8h, 010h, 0E0h, 065h, 006h
        db  0A0h, 0D2h, 026h, 0E0h, 0CFh, 004h, 008h, 010h, 020h, 02Dh, 001h, 000h, 0E0h, 0C0h, 0DCh, 008h
        db  0A0h, 006h, 050h, 0E2h, 0A0h, 0D2h, 0ADh, 0E3h, 060h, 004h, 0D0h, 0DAh, 0A0h, 006h, 0DEh, 0DFh
        db  008h, 0C2h, 019h, 013h, 083h, 007h, 080h, 080h, 0E0h, 023h, 014h, 0E0h, 002h, 013h, 083h, 007h
        db  00Ah, 000h, 060h, 004h, 0C4h, 0DAh, 020h, 0C1h, 006h, 006h, 00Dh, 013h, 0A0h, 006h, 0DEh, 0DFh
        db  008h, 0C2h, 009h, 013h, 083h, 007h, 00Bh, 000h, 0E0h, 023h, 014h, 0E0h, 002h, 016h, 083h, 007h
        db  001h, 080h, 060h, 004h, 0C4h, 0DAh, 083h, 007h, 00Ah, 080h, 060h, 004h, 0B2h, 0DAh, 0E0h, 093h
        db  00Eh, 0E0h, 05Eh, 013h, 0E0h, 093h, 010h, 0E0h, 017h, 013h, 0E0h, 023h, 014h, 0E0h, 004h, 013h
        db  083h, 007h, 007h, 000h, 060h, 004h, 0C4h, 0DAh, 083h, 007h, 000h, 0A0h, 0A0h, 006h, 050h, 0E2h
        db  083h, 007h, 000h, 048h, 0A0h, 006h, 050h, 0E2h, 0A0h, 0D2h, 010h, 0E0h, 020h, 0C8h, 01Ch, 0E0h
        db  0CAh, 006h, 020h, 0C8h, 020h, 0E0h, 0CCh, 006h, 0A0h, 006h, 06Ch, 0E1h, 008h, 002h, 04Eh, 080h
        db  0A0h, 006h, 01Ch, 0DFh, 0A0h, 023h, 01Ch, 0E0h, 020h, 013h, 020h, 088h, 01Ah, 009h, 00Eh, 007h
        db  01Ch, 016h, 020h, 088h, 01Ch, 009h, 010h, 007h, 018h, 016h, 020h, 088h, 01Eh, 009h, 012h, 007h
        db  014h, 016h, 020h, 088h, 00Ah, 007h, 022h, 0E0h, 010h, 013h, 020h, 006h, 0CAh, 006h, 038h, 016h
        db  0A0h, 0E3h, 020h, 0E0h, 006h, 010h, 0E0h, 023h, 014h, 0E0h, 0CAh, 016h, 0A0h, 0E3h, 022h, 0E0h
        db  0C2h, 004h, 0A0h, 0D2h, 0ACh, 0E3h, 060h, 004h, 0B8h, 0DAh, 020h, 0C8h, 01Ch, 0E0h, 0CAh, 006h
        db  0A0h, 088h, 0DCh, 006h, 00Eh, 000h, 010h, 016h, 0A0h, 088h, 0DEh, 006h, 010h, 000h, 00Ch, 016h
        db  0A0h, 088h, 0E0h, 006h, 012h, 000h, 008h, 016h, 020h, 006h, 0CCh, 006h, 019h, 016h, 020h, 0E8h
        db  00Eh, 0E0h, 082h, 001h, 0A0h, 0E3h, 01Eh, 0E0h, 020h, 0C8h, 020h, 0E0h, 0CCh, 006h, 010h, 010h
        db  0A0h, 023h, 010h, 0E0h, 008h, 016h, 064h, 0C1h, 006h, 000h, 060h, 021h, 00Ch, 0E0h, 008h, 013h
        db  0A0h, 0D2h, 0AAh, 0E3h, 005h, 010h, 020h, 088h, 00Ah, 007h, 008h, 007h, 096h, 012h, 000h, 010h
        db  060h, 004h, 0B8h, 0DAh, 0E0h, 093h, 00Eh, 0E0h, 050h, 013h, 0E0h, 093h, 0ABh, 0E3h, 04Dh, 013h
        db  0E0h, 093h, 0AAh, 0E3h, 01Ch, 013h, 0A0h, 006h, 0CAh, 0E1h, 0A0h, 023h, 010h, 0E0h, 045h, 013h
        db  0A0h, 023h, 008h, 0E0h, 006h, 016h, 060h, 0E3h, 01Eh, 0E0h, 020h, 0C8h, 022h, 0E0h, 006h, 007h
        db  034h, 010h, 0E0h, 023h, 014h, 0E0h, 031h, 016h, 060h, 0C1h, 018h, 009h, 060h, 021h, 012h, 0E0h
        db  02Ch, 016h, 0A0h, 006h, 02Eh, 0E9h, 031h, 010h, 0A0h, 0D2h, 0AAh, 0E3h, 02Eh, 010h, 0A0h, 0E3h
        db  012h, 0E0h, 0A0h, 006h, 02Eh, 0E9h, 064h, 0C1h, 006h, 000h, 060h, 021h, 00Ch, 0E0h, 025h, 013h
        db  020h, 088h, 00Eh, 007h, 0DCh, 006h, 014h, 016h, 020h, 088h, 010h, 007h, 0DEh, 006h, 010h, 016h
        db  020h, 088h, 012h, 007h, 0E0h, 006h, 00Ch, 016h, 020h, 098h, 0CEh, 006h, 0CFh, 006h, 015h, 013h
        db  020h, 006h, 0CEh, 006h, 012h, 016h, 060h, 0E3h, 01Ah, 0E0h, 0A0h, 0D2h, 00Ch, 0E0h, 00Dh, 010h
        db  060h, 0E3h, 01Eh, 0E0h, 020h, 0C8h, 032h, 0E0h, 006h, 007h, 0A0h, 006h, 06Ch, 0E1h, 008h, 002h
        db  048h, 080h, 0A0h, 006h, 01Ch, 0DFh, 0A0h, 0D2h, 0ABh, 0E3h, 060h, 004h, 0B8h, 0DAh, 022h, 088h
        db  00Eh, 000h, 01Ah, 009h, 0C9h, 01Ah, 00Bh, 01Bh, 022h, 088h, 010h, 000h, 01Ch, 009h, 0C4h, 01Ah
        db  006h, 01Bh, 022h, 088h, 012h, 000h, 01Eh, 009h, 0BFh, 01Ah, 001h, 01Bh, 05Bh, 004h, 060h, 0C1h
        db  06Ch, 001h, 085h, 002h, 043h, 000h, 0E1h, 013h, 0E0h, 093h, 0AAh, 0E3h, 0DEh, 016h, 0A0h, 0C8h
        db  04Ah, 0EDh, 002h, 000h, 084h, 007h, 00Eh, 000h, 042h, 0C1h, 0A0h, 0C0h, 06Ch, 001h, 002h, 0C0h
        db  025h, 002h, 048h, 000h, 081h, 007h, 080h, 0E9h, 083h, 007h, 014h, 0AEh, 060h, 004h, 08Ch, 0ECh
        db  002h, 002h, 000h, 0FCh, 0CAh, 010h, 020h, 098h, 00Eh, 0E0h, 065h, 006h, 003h, 016h, 020h, 0D8h
        db  0AAh, 0E3h, 065h, 006h, 0E0h, 093h, 0ABh, 0E3h, 00Dh, 013h, 0A0h, 023h, 008h, 0E0h, 019h, 016h
        db  0A0h, 023h, 010h, 0E0h, 016h, 013h, 060h, 0E3h, 01Eh, 0E0h, 020h, 0C8h, 020h, 0E0h, 006h, 007h
        db  0A0h, 006h, 06Ch, 0E1h, 0A0h, 043h, 018h, 0E0h, 0E0h, 02Eh, 001h, 000h, 0A0h, 0D2h, 026h, 0E0h
        db  083h, 007h, 010h, 000h, 0A0h, 006h, 050h, 0E2h, 0E0h, 023h, 014h, 0E0h, 002h, 016h, 0A0h, 006h
        db  04Eh, 0E1h, 0A0h, 043h, 02Ch, 0E0h, 020h, 0C8h, 020h, 0E0h, 0D6h, 008h, 060h, 004h, 0B8h, 0DAh
        db  0A0h, 043h, 00Eh, 0E0h, 0A0h, 0C1h, 0D6h, 008h, 002h, 013h, 020h, 006h, 0D6h, 008h, 0E0h, 023h
        db  014h, 0E0h, 003h, 013h, 0A0h, 023h, 008h, 0E0h, 026h, 016h, 008h, 002h, 042h, 080h, 0A0h, 006h
        db  01Ch, 0DFh, 0A0h, 023h, 008h, 0E0h, 01Fh, 016h, 0A0h, 023h, 010h, 0E0h, 00Ah, 016h, 022h, 0C1h
        db  002h, 000h, 020h, 025h, 06Ah, 0E4h, 023h, 016h, 083h, 007h, 020h, 080h, 0A0h, 006h, 050h, 0E2h
        db  012h, 010h, 0A0h, 006h, 06Ch, 0E1h, 0E0h, 023h, 014h, 0E0h, 002h, 016h, 0A0h, 006h, 04Eh, 0E1h
        db  060h, 0E3h, 01Eh, 0E0h, 020h, 0C8h, 020h, 0E0h, 006h, 007h, 0A0h, 023h, 008h, 0E0h, 003h, 016h
        db  0A0h, 023h, 006h, 0E0h, 051h, 013h, 020h, 098h, 00Eh, 0E0h, 065h, 006h, 003h, 016h, 020h, 0D8h
        db  0AAh, 0E3h, 065h, 006h, 022h, 0C1h, 002h, 000h, 020h, 025h, 06Ah, 0E4h, 00Eh, 013h, 083h, 007h
        db  020h, 000h, 0A0h, 006h, 050h, 0E2h, 022h, 0C8h, 00Eh, 000h, 0E6h, 006h, 022h, 0C8h, 010h, 000h
        db  0E8h, 006h, 022h, 0C8h, 012h, 000h, 0EAh, 006h, 037h, 010h, 022h, 088h, 00Eh, 000h, 0DCh, 006h
        db  008h, 016h, 022h, 088h, 010h, 000h, 0DEh, 006h, 004h, 016h, 022h, 088h, 012h, 000h, 0E0h, 006h
        db  00Bh, 013h, 022h, 0C8h, 00Eh, 000h, 0DCh, 006h, 022h, 0C8h, 010h, 000h, 0DEh, 006h, 022h, 0C8h
        db  012h, 000h, 0E0h, 006h, 060h, 0E3h, 014h, 0E0h, 0A0h, 023h, 00Eh, 0E0h, 008h, 016h, 0A0h, 0C1h
        db  0D6h, 008h, 01Ah, 016h, 086h, 007h, 000h, 010h, 006h, 0E8h, 0D2h, 006h, 015h, 010h, 0A0h, 0E3h
        db  00Eh, 0E0h, 0A0h, 023h, 008h, 0E0h, 009h, 016h, 0A0h, 0E3h, 006h, 0E0h, 0E0h, 004h, 0E6h, 006h
        db  0E0h, 004h, 0E8h, 006h, 0E0h, 004h, 0EAh, 006h, 007h, 010h, 008h, 002h, 036h, 080h, 0A0h, 007h
        db  0D6h, 006h, 0E6h, 0DEh, 0A0h, 006h, 01Ch, 0DFh, 060h, 004h, 0B8h, 0DAh, 020h, 098h, 065h, 006h
        db  010h, 0E0h, 003h, 016h, 020h, 0D8h, 00Eh, 0E0h, 065h, 006h, 060h, 004h, 0B8h, 0DAh, 0E0h, 023h
        db  014h, 0E0h, 002h, 013h, 060h, 004h, 0B8h, 0DAh, 02Eh, 010h, 0B0h, 003h, 020h, 098h, 0ABh, 0E3h
        db  06Fh, 006h, 019h, 016h, 024h, 0C2h, 008h, 000h, 016h, 011h, 0E0h, 0E3h, 014h, 0E0h, 083h, 007h
        db  000h, 000h, 0A0h, 006h, 050h, 0E2h, 0A0h, 023h, 014h, 0E0h, 004h, 013h, 008h, 002h, 006h, 080h
        db  0A0h, 006h, 01Ch, 0DFh, 008h, 002h, 01Eh, 000h, 0A0h, 006h, 01Ch, 0DFh, 0A0h, 023h, 008h, 0E0h
        db  002h, 013h, 0A0h, 006h, 04Eh, 0E1h, 082h, 0C0h, 002h, 016h, 060h, 004h, 0D0h, 0DAh, 020h, 0E8h
        db  066h, 0EDh, 0F0h, 006h, 020h, 099h, 00Eh, 0E0h, 016h, 000h, 005h, 016h, 0E0h, 004h, 0ECh, 006h
        db  020h, 048h, 014h, 0E0h, 0F0h, 006h, 083h, 007h, 001h, 000h, 060h, 004h, 0B2h, 0DAh, 064h, 0C2h
        db  014h, 000h, 024h, 002h, 018h, 000h, 0C4h, 0C1h, 0C2h, 061h, 027h, 002h, 0FCh, 0FFh, 074h, 0C1h
        db  085h, 0C1h, 045h, 071h, 085h, 002h, 027h, 000h, 046h, 016h, 054h, 0C1h, 045h, 002h, 0CFh, 0FFh
        db  042h, 016h, 0C8h, 004h, 064h, 0C1h, 008h, 000h, 006h, 015h, 005h, 013h, 024h, 0C2h, 00Eh, 000h
        db  048h, 002h, 000h, 01Fh, 0C8h, 006h, 028h, 002h, 011h, 000h, 004h, 0A2h, 018h, 098h, 06Bh, 0EDh
        db  032h, 016h, 042h, 0C1h, 025h, 002h, 004h, 000h, 047h, 065h, 035h, 0C2h, 074h, 0CDh, 048h, 006h
        db  0FDh, 015h, 0A0h, 0C0h, 06Ch, 001h, 002h, 0C1h, 004h, 0C8h, 06Ch, 001h, 0A0h, 0C1h, 000h, 0FCh
        db  005h, 013h, 020h, 0C8h, 002h, 0EBh, 002h, 0FCh, 006h, 0C1h, 0F6h, 010h, 002h, 0C8h, 06Ch, 001h
        db  020h, 0C8h, 04Ah, 0EDh, 002h, 0FCh, 081h, 007h, 0F6h, 0EBh, 004h, 0C0h, 083h, 007h, 010h, 002h
        db  084h, 007h, 00Eh, 000h, 03Bh, 010h, 084h, 007h, 00Ch, 000h, 0E2h, 0C0h, 008h, 000h, 005h, 002h
        db  000h, 0FCh, 0A0h, 0C0h, 06Ch, 001h, 002h, 0C0h, 0A0h, 0C1h, 000h, 0FCh, 02Fh, 013h, 0E0h, 004h
        db  000h, 0FCh, 016h, 02Eh, 02Bh, 010h, 0A0h, 0C8h, 06Ch, 0EDh, 00Eh, 000h, 0A0h, 0C8h, 06Eh, 0EDh
        db  010h, 000h, 0A0h, 0C8h, 070h, 0EDh, 012h, 000h, 083h, 007h, 006h, 080h, 060h, 004h, 0B2h, 0DAh
        db  060h, 004h, 0D0h, 0DAh, 084h, 007h, 010h, 000h, 085h, 007h, 034h, 000h, 009h, 010h, 084h, 007h
        db  012h, 000h, 085h, 007h, 032h, 000h, 004h, 010h, 084h, 007h, 014h, 000h, 085h, 007h, 038h, 000h
        db  0A0h, 006h, 004h, 0E0h, 085h, 0C8h, 004h, 000h, 0A0h, 006h, 046h, 0E0h, 0A0h, 0C8h, 04Ah, 0EDh
        db  002h, 000h, 0A0h, 0C0h, 06Ch, 001h, 002h, 0C0h, 048h, 006h, 048h, 0C1h, 0A0h, 006h, 0C8h, 0E2h
        db  007h, 002h, 0A2h, 006h, 060h, 0C5h, 002h, 0FCh, 025h, 002h, 0F4h, 0FFh, 005h, 0C8h, 002h, 0FCh
        db  020h, 0C2h, 06Ch, 001h, 0A0h, 006h, 0BCh, 0C1h, 060h, 004h, 0AEh, 0DAh, 0A0h, 006h, 0C8h, 0E2h
        db  007h, 002h, 0B4h, 006h, 0EFh, 010h, 00Bh, 0C3h, 000h, 003h, 002h, 000h, 082h, 007h, 0C0h, 000h
        db  020h, 0C8h, 00Ch, 000h, 0C0h, 000h, 020h, 0C8h, 00Eh, 000h, 0C2h, 000h, 020h, 0C8h, 010h, 000h
        db  0C4h, 000h, 020h, 0C8h, 012h, 000h, 0C6h, 000h, 020h, 0C8h, 014h, 000h, 0C8h, 000h, 020h, 0C8h
        db  016h, 000h, 0CAh, 000h, 020h, 0C8h, 004h, 000h, 0CCh, 000h, 020h, 0C8h, 006h, 000h, 0CEh, 000h
        db  002h, 0C8h, 00Ch, 000h, 0A0h, 007h, 00Eh, 000h, 036h, 0EDh, 002h, 0C8h, 010h, 000h, 0A0h, 007h
        db  012h, 000h, 040h, 0EDh, 002h, 0C8h, 014h, 000h, 0A0h, 007h, 016h, 000h, 070h, 0EDh, 002h, 0C8h
        db  004h, 000h, 0A0h, 007h, 006h, 000h, 086h, 0EDh, 060h, 001h, 01Ch, 001h, 004h, 000h, 009h, 016h
        db  0E0h, 001h, 040h, 001h, 000h, 008h, 0A0h, 001h, 040h, 001h, 000h, 040h, 0E0h, 001h, 040h, 001h
        db  000h, 004h, 0A0h, 006h, 046h, 0F0h, 005h, 002h, 000h, 080h, 005h, 0D8h, 080h, 004h, 0C7h, 004h
        db  000h, 003h, 00Fh, 000h, 088h, 007h, 000h, 010h, 009h, 002h, 000h, 020h, 08Ah, 007h, 09Eh, 0EDh
        db  003h, 002h, 000h, 0E6h, 05Ah, 004h, 000h, 003h, 000h, 000h, 020h, 0D2h, 087h, 001h, 006h, 010h
        db  000h, 003h, 000h, 000h, 020h, 0C2h, 08Ah, 001h, 008h, 002h, 000h, 01Ah, 060h, 0C2h, 0AEh, 000h
        db  048h, 0DAh, 080h, 004h, 089h, 005h, 089h, 002h, 006h, 000h, 007h, 015h, 088h, 007h, 000h, 080h
        db  048h, 0DAh, 080h, 004h, 009h, 0C8h, 0AEh, 000h, 080h, 003h, 0E0h, 002h, 0A0h, 000h, 05Ch, 004h
        db  000h, 003h, 000h, 000h, 060h, 001h, 09Ch, 001h, 020h, 000h, 0E2h, 013h, 020h, 0C2h, 08Ch, 001h
        db  008h, 002h, 000h, 01Ch, 0E3h, 010h, 000h, 003h, 000h, 000h, 060h, 001h, 040h, 001h, 000h, 040h
        db  0ECh, 016h, 0A0h, 001h, 040h, 001h, 000h, 040h, 008h, 002h, 000h, 002h, 0D7h, 010h, 0B3h, 0C0h
        db  092h, 006h, 0FDh, 010h, 0B3h, 0C0h, 048h, 0C0h, 072h, 0CCh, 072h, 0CCh, 032h, 0C1h, 044h, 0CCh
        db  072h, 0DCh, 004h, 006h, 0FDh, 016h, 05Bh, 004h, 048h, 0C0h, 002h, 002h, 092h, 0E9h, 084h, 007h
        db  006h, 000h, 0F6h, 010h, 002h, 002h, 0E0h, 0E5h, 049h, 0C0h, 084h, 007h, 006h, 000h, 0F0h, 010h
        db  0B3h, 0C0h, 032h, 0C1h, 001h, 002h, 001h, 000h, 044h, 0D0h, 0C1h, 006h, 044h, 002h, 0FFh, 000h
        db  0E7h, 010h, 033h, 0C1h, 073h, 0C0h, 044h, 0D1h, 044h, 002h, 0FFh, 000h, 045h, 0DCh, 004h, 006h
        db  0FDh, 016h, 05Ah, 004h, 0A0h, 006h, 0C6h, 0EFh, 033h, 0C8h, 09Eh, 001h, 05Ah, 004h, 0A0h, 006h
        db  0C4h, 0EDh, 089h, 0C1h, 0A6h, 009h, 066h, 002h, 040h, 000h, 006h, 0C8h, 08Ah, 001h, 088h, 0C1h
        db  0A6h, 009h, 066h, 002h, 040h, 000h, 006h, 0C8h, 08Ch, 001h, 0C2h, 004h, 0C7h, 0C1h, 003h, 016h
        db  002h, 006h, 0FCh, 016h, 04Dh, 010h, 05Ah, 004h, 0A0h, 006h, 010h, 0EFh, 089h, 0C1h, 0A6h, 009h
        db  066h, 002h, 040h, 000h, 006h, 0C8h, 08Ah, 001h, 088h, 0C1h, 0A6h, 009h, 066h, 002h, 040h, 000h
        db  006h, 0C8h, 08Ch, 001h, 033h, 0C8h, 09Eh, 001h, 0E8h, 010h, 033h, 08Ah, 002h, 000h, 038h, 016h
        db  073h, 08Ah, 002h, 000h, 035h, 016h, 05Ah, 004h, 020h, 08Ah, 08Ch, 0E9h, 002h, 000h, 030h, 016h
        db  060h, 08Ah, 090h, 0E9h, 002h, 000h, 02Ch, 016h, 082h, 007h, 036h, 0EAh, 001h, 010h, 0B3h, 0C0h
        db  004h, 002h, 080h, 004h, 052h, 0D1h, 003h, 013h, 032h, 09Dh, 022h, 016h, 0FBh, 010h, 085h, 007h
        db  000h, 080h, 005h, 0D8h, 080h, 004h, 0C7h, 004h, 05Ah, 004h, 020h, 0C8h, 0C0h, 000h, 00Ch, 000h
        db  020h, 0C8h, 0C2h, 000h, 00Eh, 000h, 020h, 0C8h, 0C4h, 000h, 010h, 000h, 020h, 0C8h, 0C6h, 000h
        db  012h, 000h, 020h, 0C8h, 0C8h, 000h, 014h, 000h, 020h, 0C8h, 0CAh, 000h, 016h, 000h, 020h, 0C8h
        db  0CCh, 000h, 004h, 000h, 020h, 0C8h, 0CEh, 000h, 006h, 000h, 000h, 003h, 00Fh, 000h, 0CCh, 005h
        db  05Ch, 004h, 0E0h, 004h, 082h, 001h, 002h, 002h, 0DAh, 0E5h, 032h, 0C8h, 082h, 001h, 032h, 0C8h
        db  080h, 001h, 0A0h, 006h, 0DCh, 0EEh, 012h, 0C8h, 082h, 001h, 0CAh, 0C2h, 084h, 007h, 0D0h, 007h
        db  0E0h, 004h, 084h, 001h, 004h, 006h, 0FCh, 016h, 020h, 0C1h, 084h, 001h, 0E9h, 016h, 004h, 002h
        db  032h, 000h, 085h, 007h, 000h, 080h, 005h, 0D8h, 080h, 004h, 0C7h, 004h, 060h, 0C1h, 086h, 001h
        db  004h, 006h, 0FCh, 016h, 020h, 0C1h, 084h, 001h, 05Bh, 004h, 0B3h, 0C0h, 0B3h, 0C4h, 05Bh, 004h
        db  048h, 0C0h, 0B3h, 0C0h, 073h, 0A0h, 042h, 0C4h, 05Bh, 004h, 033h, 088h, 084h, 001h, 0E6h, 016h
        db  05Ah, 004h, 089h, 0C1h, 0A6h, 009h, 066h, 002h, 040h, 000h, 006h, 0C8h, 08Ah, 001h, 05Bh, 004h
        db  0C5h, 004h, 0A0h, 007h, 09Ch, 001h, 040h, 000h, 060h, 001h, 09Ch, 001h, 040h, 000h, 003h, 013h
        db  005h, 006h, 0F7h, 016h, 05Ch, 004h, 05Bh, 004h, 0A0h, 006h, 064h, 0EFh, 060h, 0C0h, 040h, 001h
        db  005h, 0C8h, 040h, 001h, 002h, 0C5h, 001h, 0C8h, 040h, 001h, 05Ah, 004h, 0A0h, 006h, 064h, 0EFh
        db  008h, 0A1h, 0F4h, 010h, 0B3h, 0C0h, 033h, 0C1h, 060h, 0C1h, 040h, 001h, 085h, 001h, 000h, 004h
        db  0C5h, 001h, 000h, 010h, 05Bh, 004h, 008h, 0C1h, 009h, 0C2h, 044h, 0C2h, 05Bh, 004h, 005h, 002h
        db  0C8h, 000h, 005h, 006h, 0FEh, 016h, 05Bh, 004h, 033h, 0C1h, 003h, 0C0h, 0C4h, 0C0h, 05Bh, 004h
        db  0C0h, 0C0h, 05Bh, 004h, 0E0h, 094h, 09Eh, 001h, 0C2h, 016h, 0C3h, 005h, 05Bh, 004h, 073h, 0C0h
        db  0A0h, 006h, 0DEh, 0EFh, 02Dh, 002h, 008h, 000h, 085h, 007h, 008h, 000h, 071h, 09Fh, 0B7h, 016h
        db  005h, 006h, 0FCh, 016h, 05Ah, 004h, 002h, 002h, 0E6h, 0E5h, 060h, 004h, 0C8h, 0EDh, 0E9h, 08Ch
        db  004h, 000h, 0ADh, 016h, 05Bh, 004h, 020h, 0C1h, 080h, 001h, 085h, 007h, 0D0h, 007h, 0E0h, 001h
        db  080h, 001h, 000h, 004h, 045h, 006h, 0FEh, 016h, 004h, 0C8h, 080h, 001h, 05Bh, 004h, 033h, 0C1h
        db  048h, 0C3h, 004h, 0C1h, 004h, 013h, 02Dh, 002h, 000h, 004h, 004h, 006h, 0FCh, 016h, 05Bh, 004h
        db  08Dh, 0C3h, 0A0h, 006h, 0DEh, 0EFh, 08Dh, 0C1h, 0A6h, 009h, 066h, 002h, 040h, 000h, 086h, 0C7h
        db  000h, 000h, 000h, 0F0h, 0DCh, 007h, 05Ah, 004h, 08Dh, 0C1h, 0A6h, 009h, 066h, 002h, 040h, 000h
        db  006h, 0C8h, 08Ah, 001h, 05Bh, 004h, 08Dh, 0C1h, 0A6h, 009h, 066h, 002h, 040h, 000h, 006h, 0C8h
        db  08Ch, 001h, 05Bh, 004h, 04Dh, 0C0h, 004h, 002h, 028h, 000h, 085h, 007h, 000h, 055h, 060h, 004h
        db  0ECh, 0EDh, 04Dh, 0C0h, 0B3h, 0C0h, 032h, 0C1h, 060h, 004h, 0B0h, 0EDh, 033h, 0C1h, 060h, 001h
        db  01Ch, 001h, 004h, 000h, 001h, 016h, 05Bh, 004h, 0C4h, 0C0h, 05Bh, 004h, 089h, 007h, 028h, 0E5h
        db  039h, 0C2h, 007h, 013h, 039h, 0C6h, 039h, 086h, 025h, 016h, 039h, 0C6h, 039h, 086h, 022h, 016h
        db  0F7h, 010h, 002h, 002h, 064h, 0F0h, 0C4h, 004h, 0C5h, 004h, 039h, 0C2h, 002h, 013h, 060h, 004h
        db  0A0h, 0F0h, 002h, 002h, 072h, 0F0h, 0C4h, 004h, 039h, 0C2h, 003h, 013h, 079h, 0C1h, 060h, 004h
        db  0A0h, 0F0h, 002h, 002h, 082h, 0F0h, 0C5h, 004h, 039h, 0C2h, 003h, 013h, 039h, 0C1h, 060h, 004h
        db  0A0h, 0F0h, 079h, 0C0h, 0B9h, 0C0h, 081h, 060h, 0C2h, 005h, 012h, 009h, 0F1h, 004h, 002h, 006h
        db  0FDh, 016h, 05Bh, 004h, 05Ch, 004h, 001h, 002h, 0AAh, 0AAh, 001h, 0C6h, 044h, 0E0h, 045h, 040h
        db  058h, 080h, 0F8h, 016h, 001h, 002h, 014h, 000h, 001h, 006h, 0FEh, 016h, 001h, 002h, 055h, 055h
        db  001h, 0C6h, 044h, 0E0h, 045h, 040h, 058h, 080h, 0EDh, 016h, 052h, 004h, 048h, 0C0h, 089h, 0C0h
        db  081h, 060h, 0C2h, 005h, 05Bh, 004h, 00Bh, 0C3h, 0A0h, 006h, 0C6h, 0F0h, 041h, 0CCh, 042h, 006h
        db  0FDh, 016h, 0A0h, 006h, 0C6h, 0F0h, 001h, 0C1h, 044h, 08Ch, 012h, 016h, 0C4h, 005h, 042h, 006h
        db  0FBh, 016h, 004h, 002h, 00Ch, 0F1h, 003h, 002h, 001h, 001h, 094h, 006h, 003h, 002h, 05Ah, 05Ah
        db  094h, 006h, 043h, 005h, 094h, 006h, 003h, 007h, 094h, 006h, 0C3h, 004h, 094h, 006h, 0CCh, 005h
        db  05Ch, 004h, 0CBh, 0C1h, 0A0h, 006h, 0C6h, 0F0h, 043h, 0CCh, 042h, 006h, 0FDh, 016h, 0A0h, 006h
        db  0C6h, 0F0h, 043h, 08Ch, 0F5h, 016h, 042h, 006h, 0FCh, 016h, 057h, 004h, 08Bh, 0C2h, 008h, 0C0h
        db  049h, 0C1h, 085h, 005h, 080h, 002h, 040h, 000h, 003h, 011h, 080h, 002h, 04Fh, 000h, 045h, 012h
        db  001h, 002h, 0C6h, 0F0h, 0A1h, 009h, 001h, 080h, 040h, 013h, 001h, 002h, 0F6h, 0F1h, 0A1h, 009h
        db  001h, 080h, 03Bh, 013h, 060h, 0C0h, 006h, 000h, 0A1h, 009h, 001h, 080h, 036h, 013h, 081h, 005h
        db  001h, 080h, 033h, 013h, 04Ah, 0C0h, 0A1h, 009h, 001h, 080h, 02Fh, 013h, 000h, 0C8h, 06Ah, 001h
        db  080h, 002h, 080h, 000h, 017h, 014h, 001h, 002h, 000h, 0F8h, 0A0h, 0C1h, 040h, 001h, 0A0h, 001h
        db  040h, 001h, 000h, 004h, 002h, 002h, 000h, 010h, 003h, 002h, 000h, 004h, 0B1h, 0CCh, 043h, 006h
        db  0FDh, 016h, 0A0h, 001h, 040h, 001h, 000h, 040h, 008h, 002h, 010h, 0F8h, 006h, 0C8h, 040h, 001h
        db  000h, 0C0h, 002h, 013h, 008h, 002h, 000h, 0F8h, 009h, 002h, 0FEh, 0FBh, 0A0h, 006h, 0D0h, 0F0h
        db  025h, 010h, 080h, 002h, 080h, 000h, 009h, 014h, 001h, 002h, 000h, 0F8h, 002h, 002h, 000h, 010h
        db  003h, 002h, 000h, 004h, 072h, 0CCh, 043h, 006h, 0FDh, 016h, 080h, 005h, 080h, 002h, 080h, 000h
        db  004h, 012h, 060h, 001h, 004h, 001h, 020h, 000h, 005h, 013h, 040h, 081h, 0ABh, 016h, 080h, 002h
        db  080h, 000h, 00Bh, 014h, 0A0h, 007h, 06Ah, 001h, 07Eh, 000h, 002h, 002h, 000h, 010h, 003h, 002h
        db  000h, 004h, 0C1h, 004h, 081h, 0CCh, 043h, 006h, 0FDh, 016h, 0CAh, 005h, 05Ah, 004h, 000h, 002h
        db  0E8h, 0F1h, 001h, 002h, 018h, 0F3h, 040h, 002h, 000h, 0FCh, 041h, 002h, 000h, 0FCh, 040h, 080h
        db  004h, 013h, 0A0h, 007h, 004h, 001h, 03Ch, 000h, 05Bh, 004h, 0C0h, 004h, 001h, 002h, 008h, 000h
        db  002h, 002h, 000h, 012h, 0E0h, 0C1h, 040h, 001h, 0A0h, 001h, 040h, 001h, 000h, 004h, 003h, 002h
        db  000h, 001h, 000h, 0C8h, 06Ah, 001h, 0A0h, 0CCh, 010h, 0F8h, 080h, 005h, 003h, 006h, 0F9h, 016h
        db  022h, 002h, 000h, 002h, 001h, 006h, 0F3h, 016h, 0A0h, 001h, 040h, 001h, 000h, 040h, 007h, 0C8h
        db  040h, 001h, 000h, 002h, 000h, 008h, 040h, 0C0h, 001h, 006h, 001h, 0C8h, 06Ah, 001h, 061h, 002h
        db  000h, 080h, 001h, 0C8h, 010h, 0F8h, 000h, 006h, 0F6h, 016h, 0C0h, 004h, 0C8h, 004h, 0C9h, 004h
        db  003h, 002h, 000h, 008h, 000h, 0C8h, 06Ah, 001h, 080h, 0C1h, 066h, 002h, 000h, 080h, 020h, 0C1h
        db  010h, 0F8h, 006h, 081h, 015h, 016h, 008h, 0C2h, 006h, 013h, 080h, 005h, 003h, 006h, 0F2h, 016h
        db  008h, 0C2h, 00Dh, 013h, 019h, 010h, 0A0h, 007h, 010h, 0F8h, 055h, 055h, 020h, 0C1h, 010h, 0F8h
        db  084h, 002h, 055h, 055h, 002h, 016h, 006h, 0C2h, 0F0h, 010h, 006h, 081h, 0EEh, 013h, 05Bh, 004h
        db  0A0h, 007h, 010h, 0F8h, 055h, 055h, 060h, 0C1h, 010h, 0F8h, 005h, 081h, 003h, 013h, 085h, 002h
        db  055h, 055h, 0F5h, 016h, 008h, 0C2h, 0E1h, 013h, 040h, 0C2h, 009h, 006h, 048h, 002h, 0FFh, 007h
        db  0C0h, 004h, 001h, 002h, 008h, 000h, 002h, 002h, 000h, 012h, 003h, 002h, 000h, 001h, 000h, 0C8h
        db  06Ah, 001h, 032h, 0C8h, 010h, 0F8h, 080h, 005h, 003h, 006h, 0F9h, 016h, 022h, 002h, 000h, 002h
        db  001h, 006h, 0F3h, 016h, 088h, 002h, 040h, 000h, 013h, 015h, 089h, 002h, 04Fh, 000h, 010h, 011h
        db  0C0h, 004h, 002h, 002h, 000h, 012h, 001h, 002h, 008h, 000h, 003h, 002h, 000h, 001h, 080h, 0CCh
        db  003h, 006h, 0FDh, 016h, 022h, 002h, 000h, 002h, 001h, 006h, 0F7h, 016h, 0CBh, 005h, 05Bh, 004h
        db  0A0h, 007h, 004h, 001h, 037h, 000h, 05Bh, 004h, 0E0h, 002h, 0A0h, 000h, 088h, 007h, 0C0h, 000h
        db  009h, 002h, 076h, 0F3h, 084h, 007h, 0ECh, 0E5h, 005h, 002h, 001h, 000h, 08Bh, 0C2h, 0CCh, 004h
        db  0A0h, 006h, 080h, 0F3h, 060h, 02Ch, 001h, 000h, 099h, 006h, 0A0h, 02Ch, 002h, 000h, 099h, 006h
        db  020h, 02Dh, 004h, 000h, 099h, 006h, 020h, 02Eh, 008h, 000h, 099h, 006h, 0A0h, 02Fh, 010h, 000h
        db  08Ch, 005h, 009h, 016h, 080h, 0CCh, 081h, 0C4h, 083h, 007h, 0C4h, 0F3h, 088h, 0C0h, 002h, 004h
        db  08Ch, 005h, 001h, 016h, 033h, 010h, 0E0h, 002h, 0A0h, 000h, 05Ah, 004h, 08Ch, 005h, 0FBh, 016h
        db  080h, 0CCh, 081h, 0C4h, 015h, 00Ah, 0B4h, 0C0h, 012h, 0C0h, 088h, 0CCh, 052h, 0C0h, 0B4h, 0C4h
        db  042h, 006h, 05Bh, 004h, 02Dh, 007h, 018h, 000h, 041h, 08Bh, 00Ah, 000h, 0ECh, 016h, 0C1h, 082h
        db  0EAh, 016h, 0C2h, 002h, 042h, 002h, 000h, 002h, 0E6h, 016h, 080h, 003h, 081h, 007h, 001h, 000h
        db  0F1h, 010h, 001h, 002h, 002h, 000h, 0EEh, 010h, 001h, 002h, 004h, 000h, 0EBh, 010h, 001h, 002h
        db  008h, 000h, 0E8h, 010h, 001h, 002h, 010h, 000h, 0E5h, 010h, 0A1h, 002h, 041h, 08Bh, 010h, 000h
        db  002h, 013h, 060h, 004h, 070h, 0F3h, 02Dh, 007h, 018h, 000h, 080h, 003h, 009h, 002h, 000h, 008h
        db  003h, 002h, 004h, 000h, 0C7h, 004h, 0A0h, 006h, 0F0h, 0F4h, 060h, 001h, 01Ch, 001h, 004h, 000h
        db  01Ch, 016h, 0A0h, 001h, 040h, 001h, 000h, 008h, 0E0h, 001h, 040h, 001h, 000h, 010h, 004h, 002h
        db  001h, 000h, 044h, 0CEh, 0C4h, 006h, 044h, 0C6h, 0A0h, 001h, 040h, 001h, 000h, 010h, 049h, 006h
        db  0A0h, 001h, 040h, 001h, 000h, 040h, 0E0h, 001h, 040h, 001h, 000h, 004h, 0E0h, 001h, 040h, 001h
        db  000h, 008h, 0A0h, 006h, 08Eh, 0F5h, 0A0h, 006h, 08Eh, 0F5h, 0C7h, 005h, 004h, 002h, 0A6h, 0E4h
        db  0E0h, 004h, 0D0h, 003h, 074h, 0C1h, 0B4h, 0C1h, 086h, 005h, 01Ch, 013h, 0E0h, 002h, 0C0h, 000h
        db  000h, 002h, 000h, 000h, 040h, 0C0h, 080h, 0C0h, 0C0h, 0C0h, 000h, 0C1h, 040h, 0C1h, 080h, 0C1h
        db  0C0h, 0C1h, 000h, 0C2h, 040h, 0C2h, 080h, 0C2h, 0C0h, 0C2h, 000h, 0C3h, 040h, 0C3h, 080h, 0C3h
        db  0C0h, 0C3h, 0A0h, 004h, 0AAh, 000h, 0D0h, 003h, 0D0h, 003h, 03Fh, 010h, 085h, 005h, 085h, 081h
        db  0E1h, 013h, 0E4h, 010h, 0C7h, 005h, 005h, 002h, 0FFh, 07Fh, 045h, 0A1h, 0D0h, 003h, 0D0h, 003h
        db  034h, 010h, 0C0h, 0CCh, 0C1h, 0C4h, 003h, 002h, 028h, 000h, 0A0h, 006h, 0F0h, 0F4h, 0E0h, 001h
        db  042h, 001h, 000h, 010h, 0C7h, 005h, 0D0h, 003h, 0D0h, 003h, 027h, 010h, 0C7h, 005h, 0A0h, 0C1h
        db  04Ah, 001h, 0A0h, 007h, 04Ah, 001h, 000h, 00Eh, 0A0h, 001h, 040h, 001h, 000h, 002h, 020h, 007h
        db  044h, 001h, 060h, 0C1h, 044h, 001h, 085h, 002h, 000h, 0FFh, 017h, 016h, 0E0h, 001h, 040h, 001h
        db  000h, 022h, 005h, 002h, 080h, 000h, 005h, 006h, 0D0h, 003h, 0FDh, 016h, 060h, 0C1h, 046h, 001h
        db  085h, 002h, 000h, 0FFh, 00Ah, 013h, 005h, 002h, 0DDh, 021h, 005h, 006h, 000h, 010h, 000h, 010h
        db  000h, 010h, 000h, 010h, 000h, 010h, 0D0h, 003h, 0F8h, 016h, 051h, 010h, 006h, 0C8h, 04Ah, 001h
        db  0C0h, 0CCh, 0C1h, 0C4h, 04Bh, 010h, 013h, 0C0h, 0C8h, 0CCh, 053h, 0C0h, 002h, 002h, 000h, 0F5h
        db  0C2h, 0C4h, 043h, 006h, 05Bh, 004h, 060h, 0C0h, 0AEh, 000h, 0C4h, 002h, 044h, 002h, 00Fh, 000h
        db  044h, 088h, 08Ch, 0E4h, 03Ch, 016h, 081h, 002h, 008h, 000h, 027h, 013h, 021h, 0C1h, 09Eh, 0E4h
        db  014h, 0C1h, 021h, 021h, 07Ch, 0E4h, 033h, 016h, 021h, 0C1h, 084h, 0E4h, 081h, 002h, 000h, 000h
        db  00Bh, 013h, 00Dh, 002h, 0A0h, 000h, 084h, 083h, 009h, 013h, 0C4h, 005h, 084h, 083h, 006h, 013h
        db  0C4h, 005h, 084h, 083h, 003h, 013h, 023h, 010h, 00Eh, 081h, 021h, 016h, 021h, 0C1h, 09Eh, 0E4h
        db  021h, 045h, 07Ch, 0E4h, 0E0h, 001h, 042h, 001h, 000h, 010h, 0A0h, 001h, 042h, 001h, 000h, 010h
        db  0A1h, 0C3h, 096h, 0E4h, 00Fh, 002h, 02Fh, 000h, 080h, 003h, 0A0h, 001h, 040h, 001h, 000h, 002h
        db  0A0h, 001h, 040h, 001h, 000h, 080h, 06Dh, 0C0h, 00Ah, 000h, 009h, 013h, 081h, 002h, 00Ah, 000h
        db  006h, 015h, 00Eh, 002h, 0E6h, 0F4h, 00Fh, 002h, 00Fh, 000h, 080h, 003h, 0CAh, 005h, 0E0h, 002h
        db  0A0h, 000h, 05Ah, 004h, 059h, 0CEh, 020h, 088h, 0A6h, 0E4h, 0A6h, 0E4h, 0F8h, 010h, 0C1h, 004h
        db  048h, 062h, 089h, 005h, 0A0h, 0C0h, 06Ch, 001h, 008h, 0C8h, 06Ch, 001h, 003h, 002h, 000h, 0FCh
        db  004h, 002h, 000h, 002h, 073h, 0A0h, 004h, 006h, 0FDh, 016h, 088h, 005h, 009h, 006h, 0F4h, 016h
        db  002h, 0C8h, 06Ch, 001h, 081h, 086h, 001h, 016h, 0CBh, 005h, 05Bh, 004h, 043h, 028h, 031h, 029h
        db  038h, 039h, 02Dh, 033h, 038h, 038h, 042h, 020h, 020h, 059h, 049h, 054h, 04Bh, 0C2h, 0A8h, 002h
        db  098h, 000h, 083h, 007h, 002h, 000h, 028h, 002h, 008h, 000h, 023h, 0C6h, 0F8h, 0E4h, 048h, 006h
        db  0C4h, 0C0h, 073h, 00Ah, 065h, 017h, 0A0h, 006h, 0BCh, 0F6h, 0C8h, 0C1h, 0C7h, 005h, 003h, 002h
        db  0A5h, 000h, 0B0h, 003h, 0F8h, 0CDh, 0F8h, 0CDh, 0A6h, 002h, 006h, 062h, 088h, 002h, 00Ah, 000h
        db  057h, 016h, 003h, 029h, 055h, 016h, 005h, 029h, 0C4h, 080h, 052h, 016h, 015h, 009h, 050h, 017h
        db  015h, 009h, 04Eh, 018h, 085h, 002h, 029h, 000h, 04Bh, 016h, 0C6h, 005h, 096h, 000h, 003h, 007h
        db  0C4h, 004h, 045h, 006h, 095h, 000h, 044h, 005h, 043h, 016h, 044h, 081h, 041h, 016h, 000h, 003h
        db  005h, 000h, 0C4h, 002h, 000h, 003h, 00Ah, 000h, 044h, 002h, 00Fh, 000h, 084h, 002h, 005h, 000h
        db  037h, 016h, 0C4h, 002h, 000h, 003h, 00Fh, 000h, 044h, 002h, 00Fh, 000h, 084h, 002h, 00Ah, 000h
        db  02Fh, 016h, 004h, 002h, 0FEh, 0FFh, 02Ch, 013h, 02Bh, 015h, 02Ah, 01Ah, 084h, 005h, 028h, 012h
        db  027h, 015h, 026h, 01Ah, 025h, 018h, 084h, 005h, 023h, 016h, 022h, 01Bh, 021h, 017h, 084h, 005h
        db  01Fh, 013h, 01Eh, 01Ah, 01Dh, 011h, 004h, 006h, 01Bh, 016h, 0A5h, 002h, 0C5h, 0C1h, 025h, 002h
        db  006h, 000h, 003h, 002h, 0A5h, 0A5h, 083h, 0C1h, 095h, 000h, 003h, 038h, 094h, 000h, 083h, 002h
        db  02Eh, 06Bh, 00Eh, 016h, 084h, 002h, 059h, 01Ch, 00Bh, 016h, 024h, 002h, 069h, 000h, 095h, 000h
        db  003h, 03Ch, 094h, 000h, 083h, 081h, 004h, 016h, 084h, 002h, 069h, 000h, 001h, 016h, 0C9h, 005h
        db  059h, 004h, 0C3h, 0D0h, 0FDh, 013h, 001h, 01Ch, 0FBh, 010h, 0E0h, 090h, 0FFh, 0E4h, 0F8h, 016h
        db  0C3h, 006h, 0C3h, 0D0h, 0F5h, 01Ch, 0F4h, 016h, 0E0h, 090h, 0FCh, 0E4h, 0F1h, 016h, 05Bh, 004h
        db  00Bh, 0C3h, 009h, 002h, 000h, 0E5h, 0A0h, 006h, 04Ah, 0F0h, 0CCh, 005h, 05Ch, 004h, 088h, 007h
        db  000h, 0B0h, 089h, 007h, 0FEh, 0FFh, 0A8h, 009h, 0A9h, 009h, 08Ah, 007h, 002h, 0E0h, 0A0h, 006h
        db  098h, 0F5h, 000h, 000h, 088h, 007h, 000h, 0A0h, 089h, 007h, 0FEh, 0AFh, 0A8h, 009h, 0A9h, 009h
        db  08Ah, 007h, 07Ah, 0E0h, 0A0h, 006h, 098h, 0F5h, 000h, 000h, 0A0h, 006h, 0D6h, 0F5h, 000h, 000h
        db  0E6h, 010h, 0E5h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 0A0h, 000h, 008h, 023h, 0E2h, 0FFh, 09Ah, 000h, 080h, 000h, 040h, 000h, 020h, 000h, 010h
        db  000h, 008h, 000h, 004h, 000h, 002h, 000h, 001h, 080h, 000h, 040h, 000h, 020h, 000h, 010h, 000h
        db  008h, 000h, 004h, 000h, 002h, 000h, 001h, 000h, 000h, 0FFh, 0FFh, 000h, 0F0h, 0FFh, 000h, 0F0h
        db  00Fh, 000h, 0FFh, 0FFh, 0FFh, 07Fh, 003h, 000h, 000h, 000h, 0C3h, 000h, 0E7h, 0E7h, 0F2h, 0E7h
        db  0F3h, 0F2h, 043h, 028h, 020h, 029h, 04Fh, 043h, 059h, 050h, 049h, 052h, 048h, 047h, 020h, 054h
        db  042h, 049h, 020h, 04Dh, 039h, 031h, 033h, 038h, 034h, 02Ch, 035h, 02Ch, 036h, 02Ch, 043h, 028h
        db  020h, 029h, 04Fh, 043h, 059h, 050h, 049h, 052h, 048h, 047h, 020h, 054h, 049h, 054h, 031h, 020h
        db  038h, 039h, 02Dh, 033h, 039h, 038h, 039h, 02Ch, 02Ch, 030h, 02Ch, 031h, 000h, 032h, 000h, 000h
        db  075h, 031h, 0D6h, 0F5h, 022h, 0F3h, 096h, 0ECh, 0DAh, 0F6h, 000h, 000h, 0C4h, 000h, 0BEh, 0BCh
        db  04Ah, 006h, 050h, 006h, 04Ch, 006h, 010h, 0D9h, 04Eh, 006h, 00Fh, 000h, 032h, 006h, 001h, 000h
        db  050h, 007h, 058h, 007h, 052h, 007h, 030h, 0C2h, 054h, 007h, 00Fh, 000h, 038h, 007h, 001h, 000h
        db  0BAh, 000h, 0A0h, 000h, 0BCh, 000h, 0E8h, 0F6h, 0BEh, 000h, 00Fh, 000h, 05Eh, 007h, 03Ah, 007h
        db  062h, 007h, 040h, 080h, 064h, 007h, 0B0h, 0C6h, 066h, 007h, 08Eh, 0C6h, 068h, 007h, 082h, 0C4h
        db  098h, 007h, 000h, 080h, 078h, 007h, 000h, 080h, 094h, 008h, 004h, 000h, 096h, 008h, 001h, 000h
        db  09Eh, 008h, 008h, 000h, 0A8h, 008h, 00Ah, 000h, 0AAh, 008h, 006h, 000h, 0B2h, 008h, 00Ch, 000h
        db  0B4h, 008h, 004h, 000h, 0AEh, 001h, 000h, 000h, 0D0h, 008h, 000h, 000h, 014h, 009h, 000h, 000h
        db  00Ch, 006h, 013h, 000h, 00Ah, 006h, 020h, 000h, 000h, 000h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h
        db  0D8h, 0B2h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h, 0EEh, 0B4h, 0E0h, 000h, 05Eh, 0B5h, 0E0h, 000h
        db  0C2h, 0B8h, 0E0h, 000h, 004h, 0B4h, 0C0h, 000h, 014h, 0B4h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h
        db  078h, 0B3h, 0C0h, 000h, 066h, 0B4h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h
        db  078h, 0B3h, 0E0h, 000h, 078h, 0B3h, 0E0h, 000h, 078h, 0B3h, 0C0h, 000h, 0CAh, 0BCh, 0C0h, 000h
        db  036h, 0BDh, 0C0h, 000h, 072h, 0BDh, 0C0h, 000h, 0E2h, 0BDh, 0C0h, 000h, 064h, 0BEh, 0E0h, 000h
        db  0B8h, 0BFh, 0E0h, 000h, 05Eh, 0BFh, 0E0h, 000h, 0C8h, 0C0h, 0E0h, 000h, 050h, 0C0h, 0E0h, 000h
        db  026h, 0BAh, 0C0h, 000h, 07Ah, 0BAh, 0C0h, 000h, 0D6h, 0BAh, 0C0h, 000h, 008h, 0BBh, 0C0h, 000h
        db  050h, 0BAh, 0C0h, 000h, 072h, 0B3h, 0C0h, 000h, 072h, 0B3h, 000h, 03Fh, 000h, 07Fh, 000h, 05Eh
        db  030h, 000h, 028h, 000h, 043h, 000h, 0A4h, 0B6h, 0A4h, 0B6h, 008h, 0B5h, 000h, 0B5h, 032h, 0B5h
        db  032h, 0B5h, 04Eh, 0B5h, 0A4h, 0B6h, 000h, 040h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 008h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 098h, 007h, 098h, 0D6h, 058h, 007h, 04Ch, 0C5h, 058h, 007h
        db  0C8h, 0C4h, 058h, 007h, 0BCh, 0C5h, 058h, 007h, 024h, 0C5h, 098h, 007h, 0F6h, 0D2h, 098h, 007h
        db  0EEh, 0D2h, 078h, 007h, 0B0h, 0CDh, 058h, 007h, 084h, 0C5h, 098h, 007h, 050h, 0D6h, 078h, 007h
        db  064h, 0CEh, 058h, 007h, 066h, 0D3h, 058h, 007h, 034h, 0C5h, 058h, 007h, 072h, 0C5h, 058h, 007h
        db  036h, 0C6h, 098h, 007h, 0E2h, 0D7h, 098h, 007h, 092h, 0D3h, 078h, 007h, 04Eh, 0CEh, 0B8h, 007h
        db  048h, 0D8h, 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 0C6h, 006h, 0CDh, 0B8h, 0CDh, 072h, 0D2h
        db  06Eh, 0C9h, 0A6h, 0C9h, 0C4h, 0C9h, 04Eh, 0CAh, 072h, 0CAh, 08Eh, 0CAh, 01Ah, 0CBh, 01Ah, 0CBh
        db  03Ah, 0CAh, 0B8h, 0CBh, 0F4h, 0C9h, 000h, 000h, 000h, 000h, 000h, 02Ch, 000h, 00Ch, 001h, 00Fh
        db  0FFh, 0FEh, 000h, 058h, 000h, 00Eh, 0FFh, 0FEh, 00Eh, 000h, 000h, 070h, 040h, 080h, 000h, 05Eh
        db  0A0h, 0C0h, 0DFh, 0FFh, 000h, 018h, 000h, 0E0h, 000h, 078h, 000h, 050h, 000h, 060h, 000h, 070h
        db  000h, 00Ch, 006h, 000h, 000h, 000h, 086h, 0E3h, 0E6h, 007h, 0EFh, 007h, 008h, 000h, 000h, 000h
        db  08Ch, 0E3h, 0EAh, 007h, 0EFh, 007h, 006h, 000h, 040h, 000h, 000h, 00Ah, 0E6h, 007h, 0EEh, 007h
        db  008h, 000h, 040h, 000h, 006h, 00Ah, 0EAh, 007h, 0EEh, 007h, 000h, 000h, 0E2h, 0C1h, 08Bh, 0D4h
        db  0FFh, 0FFh, 0D7h, 0D1h, 0D9h, 0C5h, 0D4h, 0C3h, 03Bh, 059h, 0E6h, 008h, 0FCh, 005h, 01Ah, 009h
        db  0D8h, 006h, 006h, 004h, 07Ch, 0EAh, 0E2h, 008h, 048h, 004h, 080h, 008h, 006h, 000h, 00Ah, 006h
        db  00Eh, 00Ch, 0B8h, 0DAh, 076h, 0E7h, 09Eh, 0E7h, 084h, 0E8h, 086h, 0E9h, 0E0h, 0E9h, 03Ah, 0EAh
        db  0ECh, 0EAh, 0B8h, 0DAh, 06Eh, 0EBh, 0FEh, 0EAh, 05Ch, 0E7h, 00Ah, 0EBh, 00Ah, 0EBh, 034h, 0ECh
        db  03Eh, 0ECh, 048h, 0ECh, 05Ch, 0E4h, 0E4h, 0E6h, 0AEh, 0DAh, 0AEh, 0E4h, 07Ch, 0E5h, 0ECh, 0E5h
        db  068h, 0E6h, 08Ch, 0E6h, 006h, 0DBh, 0AEh, 0DAh, 0CCh, 0E3h, 0AEh, 0E3h, 068h, 0E3h, 0AEh, 0DAh
        db  0F0h, 0E6h, 0AEh, 0DAh, 072h, 006h, 0A6h, 0DEh, 008h, 007h, 072h, 006h, 004h, 0DEh, 0F4h, 001h
        db  072h, 006h, 0E4h, 0DDh, 008h, 007h, 07Ch, 006h, 054h, 0E5h, 004h, 000h, 07Ch, 006h, 028h, 0DEh
        db  000h, 000h, 07Ch, 006h, 0D2h, 0E6h, 0FAh, 000h, 086h, 006h, 078h, 0DDh, 005h, 000h, 090h, 006h
        db  022h, 0E7h, 028h, 000h, 090h, 006h, 000h, 0DFh, 004h, 001h, 090h, 006h, 000h, 000h, 002h, 000h
        db  090h, 006h, 030h, 0DEh, 0BCh, 002h, 09Ah, 006h, 0B6h, 0DEh, 0DCh, 005h, 09Ah, 006h, 05Ah, 0DEh
        db  064h, 000h, 09Ah, 006h, 0BAh, 0DEh, 014h, 000h, 09Ah, 006h, 016h, 0E8h, 040h, 006h, 09Ah, 006h
        db  0C2h, 0DEh, 064h, 000h, 000h, 000h, 00Ah, 007h, 00Eh, 007h, 004h, 007h, 0D8h, 006h, 000h, 007h
        db  0F0h, 006h, 0EEh, 006h, 0ECh, 006h, 00Ch, 007h, 0E6h, 006h, 018h, 007h, 000h, 050h, 0CCh, 000h
        db  003h, 000h, 000h, 084h, 000h, 0A8h, 000h, 0A0h, 000h, 020h, 000h, 080h, 000h, 040h, 000h, 008h
        db  000h, 040h, 000h, 080h, 000h, 040h, 000h, 010h, 096h, 0F5h, 05Ch, 0F4h, 076h, 0F4h, 090h, 0F4h
        db  000h, 000h, 000h, 000h, 000h, 000h, 009h, 000h, 009h, 000h, 0FEh, 0F4h, 066h, 0F4h, 07Ch, 0F4h
        db  096h, 0F4h, 040h, 001h, 042h, 001h, 042h, 001h, 042h, 001h, 000h, 000h, 07Fh, 000h, 0A0h, 000h
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
        db  044h, 000h, 0A6h, 0F3h, 048h, 000h, 0ACh, 0F3h, 050h, 000h, 0B2h, 0F3h, 060h, 000h, 0B8h, 0F3h
        db  078h, 000h, 0BEh, 0F3h, 0C2h, 0EEh, 0D0h, 0EDh, 0FEh, 0E9h, 0E2h, 0EDh, 014h, 055h, 0A0h, 001h
        db  0A4h, 0EDh, 092h, 0E9h, 0FEh, 0EDh, 058h, 0EEh, 0B8h, 0EDh, 010h, 0EFh, 000h, 000h, 01Eh, 000h
        db  0FEh, 0EDh, 04Ah, 0EEh, 000h, 041h, 001h, 041h, 06Eh, 0EEh, 035h, 0EAh, 0D0h, 0EDh, 00Ah, 0EAh
        db  0A4h, 0EDh, 0C6h, 0E9h, 00Eh, 0EEh, 01Ah, 0EEh, 06Eh, 0EEh, 030h, 0EAh, 01Ah, 0EFh, 000h, 000h
        db  0EEh, 0EEh, 0A4h, 0EDh, 0BCh, 0E9h, 00Eh, 0EEh, 01Ah, 0EEh, 0EEh, 0EEh, 01Ah, 0EFh, 000h, 000h
        db  0A4h, 0EDh, 0B2h, 0E9h, 0C4h, 0EDh, 002h, 0EEh, 01Ah, 0EEh, 0EEh, 0EEh, 0A4h, 0EDh, 0BCh, 0E9h
        db  00Eh, 0EEh, 01Ah, 0EEh, 0EEh, 0EEh, 01Ah, 0EFh, 000h, 020h, 0E2h, 0EDh, 014h, 055h, 0A0h, 001h
        db  0D0h, 0EDh, 012h, 0EAh, 0A4h, 0EDh, 092h, 0E9h, 010h, 0EFh, 050h, 055h, 00Ch, 000h, 0FEh, 0EDh
        db  04Ah, 0EEh, 000h, 001h, 000h, 000h, 06Eh, 0EEh, 037h, 0EAh, 0B8h, 0EDh, 010h, 0EFh, 055h, 055h
        db  00Ch, 000h, 00Eh, 0EEh, 058h, 0EEh, 0B8h, 0EDh, 010h, 0EFh, 0FFh, 0FFh, 008h, 000h, 010h, 0EFh
        db  002h, 010h, 006h, 000h, 0FEh, 0EDh, 04Ah, 0EEh, 000h, 001h, 001h, 001h, 06Eh, 0EEh, 042h, 0EAh
        db  0B8h, 0EDh, 010h, 0EFh, 000h, 0C0h, 008h, 000h, 010h, 0EFh, 0FFh, 0FFh, 00Ah, 000h, 010h, 0EFh
        db  0FFh, 0FFh, 00Ch, 000h, 010h, 0EFh, 00Dh, 010h, 006h, 000h, 0FEh, 0EDh, 04Ah, 0EEh, 000h, 001h
        db  001h, 001h, 06Eh, 0EEh, 036h, 0EAh, 01Ah, 0EFh, 008h, 020h, 0B8h, 0EDh, 00Ah, 0EFh, 082h, 001h
        db  002h, 0C9h, 0FEh, 0EDh, 06Eh, 0EEh, 042h, 0EAh, 01Ah, 0EFh, 034h, 020h, 0B8h, 0EDh, 010h, 0EFh
        db  000h, 010h, 006h, 000h, 0FEh, 0EDh, 07Eh, 0EFh, 06Eh, 0EEh, 03Ah, 0EAh, 00Ah, 0EFh, 09Ch, 001h
        db  040h, 000h, 0D0h, 0EDh, 012h, 0EAh, 0E2h, 0EDh, 0FFh, 000h, 080h, 007h, 0DEh, 0EFh, 003h, 000h
        db  01Eh, 0F0h, 02Ch, 0F0h, 0D4h, 0E9h, 0F0h, 0EFh, 000h, 000h, 02Ch, 0F0h, 0DEh, 0E9h, 0F0h, 0EFh
        db  004h, 000h, 02Ch, 0F0h, 0E6h, 0E9h, 0F0h, 0EFh, 007h, 000h, 02Ch, 0F0h, 0EEh, 0E9h, 0DEh, 0EFh
        db  001h, 000h, 02Ch, 0F0h, 0F6h, 0E9h, 0F0h, 0EFh, 002h, 000h, 02Ch, 0F0h, 0F6h, 0E9h, 0F0h, 0EFh
        db  006h, 000h, 02Ch, 0F0h, 0F6h, 0E9h, 0F0h, 0EFh, 005h, 000h, 02Ch, 0F0h, 0F6h, 0E9h, 0DEh, 0EFh
        db  001h, 000h, 002h, 0F0h, 0DEh, 0EFh, 003h, 000h, 010h, 0F0h, 01Ah, 0EEh, 09Eh, 0EFh, 09Ah, 0E9h
        db  001h, 000h, 09Eh, 0EFh, 0E7h, 0E9h, 002h, 000h, 09Eh, 0EFh, 0F1h, 0E9h, 006h, 000h, 09Eh, 0EFh
        db  0FCh, 0E9h, 005h, 000h, 06Eh, 0EEh, 036h, 0EAh, 0EEh, 0EEh, 0A4h, 0EDh, 092h, 0E9h, 00Eh, 0EEh
        db  07Eh, 0EFh, 0C4h, 0EDh, 04Ah, 0EEh, 000h, 001h, 000h, 080h, 06Eh, 0EEh, 03Ah, 0EAh, 0B8h, 0EDh
        db  0B6h, 0EFh, 00Ah, 0EFh, 080h, 001h, 041h, 08Eh, 002h, 0EEh, 04Ah, 0EEh, 000h, 001h, 001h, 01Bh
        db  0BEh, 0EFh, 0E4h, 0FFh, 06Eh, 0EEh, 03Eh, 0EAh, 076h, 0EFh, 0D0h, 0EDh, 018h, 0EAh, 0C4h, 0EDh
        db  022h, 0EFh, 0F4h, 0EDh, 000h, 0E0h, 07Eh, 0EFh, 06Eh, 0EEh, 048h, 0EAh, 0F4h, 0EDh, 000h, 0E8h
        db  01Ah, 0EEh, 06Eh, 0EEh, 047h, 0EAh, 0F4h, 0EDh, 000h, 008h, 07Eh, 0EFh, 06Eh, 0EEh, 048h, 0EAh
        db  0F4h, 0EDh, 000h, 0F8h, 01Ah, 0EEh, 06Eh, 0EEh, 047h, 0EAh, 00Ah, 0EFh, 080h, 001h, 000h, 002h
        db  0F4h, 0EDh, 000h, 0E0h, 01Ah, 0EEh, 06Eh, 0EEh, 047h, 0EAh, 00Ah, 0EFh, 084h, 001h, 000h, 000h
        db  01Ah, 0EFh, 034h, 000h, 0F4h, 0EDh, 000h, 000h, 07Eh, 0EFh, 01Ah, 0EFh, 034h, 060h, 0C6h, 0EFh
        db  00Ah, 0EFh, 084h, 001h, 000h, 000h, 06Eh, 0EEh, 048h, 0EAh, 00Ah, 0EFh, 082h, 001h, 000h, 0C8h
        db  0F4h, 0EDh, 000h, 0E0h, 07Eh, 0EFh, 0F4h, 0EDh, 000h, 010h, 07Eh, 0EFh, 01Ah, 0EFh, 034h, 060h
        db  00Ah, 0EFh, 080h, 001h, 000h, 006h, 0F4h, 0EDh, 010h, 000h, 030h, 0EFh, 0EEh, 0EEh, 00Ah, 0EFh
        db  084h, 001h, 000h, 000h, 01Ah, 0EFh, 034h, 000h, 0A4h, 0EDh, 092h, 0E9h, 0D0h, 0EDh, 01Eh, 0EAh
        db  088h, 0EFh, 054h, 0E9h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh, 040h, 0F0h, 006h, 000h, 000h, 0C7h
        db  058h, 0EEh, 094h, 0EFh, 000h, 0E0h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh, 040h, 0D0h, 006h, 000h
        db  000h, 0E0h, 058h, 0EEh, 094h, 0EFh, 000h, 0C0h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh, 040h, 090h
        db  006h, 000h, 000h, 0A0h, 058h, 0EEh, 094h, 0EFh, 000h, 080h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh
        db  040h, 050h, 006h, 000h, 000h, 060h, 058h, 0EEh, 094h, 0EFh, 000h, 040h, 0B8h, 0EDh, 0C4h, 0EDh
        db  028h, 0EEh, 040h, 010h, 006h, 000h, 000h, 020h, 058h, 0EEh, 094h, 0EFh, 000h, 000h, 088h, 0EFh
        db  054h, 0E9h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh, 040h, 0D0h, 006h, 000h, 000h, 0A6h, 058h, 0EEh
        db  094h, 0EFh, 000h, 0C0h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh, 040h, 090h, 006h, 000h, 000h, 0C0h
        db  058h, 0EEh, 094h, 0EFh, 000h, 080h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh, 040h, 050h, 006h, 000h
        db  000h, 040h, 058h, 0EEh, 094h, 0EFh, 000h, 040h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh, 040h, 070h
        db  006h, 000h, 000h, 060h, 058h, 0EEh, 094h, 0EFh, 000h, 060h, 036h, 0F0h, 052h, 0E9h, 0D0h, 0EDh
        db  024h, 0EAh, 0A4h, 0EDh, 092h, 0E9h, 05Ch, 0EFh, 055h, 055h, 016h, 000h, 0FEh, 0EDh, 04Ah, 0EEh
        db  000h, 000h, 000h, 000h, 06Eh, 0EEh, 04Dh, 0EAh, 0C2h, 0EEh, 0D0h, 0EDh, 024h, 0EAh, 010h, 0EFh
        db  055h, 055h, 016h, 000h, 0B8h, 0EDh, 0FEh, 0EDh, 058h, 0EEh, 0E2h, 0EDh, 0FFh, 000h, 000h, 008h
        db  0E2h, 0EDh, 0FFh, 000h, 000h, 00Ch, 0E2h, 0EDh, 0FFh, 000h, 000h, 010h, 0E2h, 0EDh, 0FFh, 000h
        db  000h, 014h, 0E2h, 0EDh, 0FFh, 000h, 000h, 018h, 0E2h, 0EDh, 0FFh, 000h, 000h, 01Ch, 0E2h, 0EDh
        db  0FFh, 000h, 000h, 020h, 0E2h, 0EDh, 0FFh, 000h, 000h, 024h, 0E2h, 0EDh, 0FFh, 000h, 000h, 028h
        db  0E2h, 0EDh, 0FFh, 000h, 000h, 02Ch, 08Ah, 0EEh, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh, 040h, 030h
        db  006h, 000h, 000h, 001h, 058h, 0EEh, 094h, 0EFh, 000h, 020h, 0B8h, 0EDh, 0C4h, 0EDh, 028h, 0EEh
        db  040h, 070h, 006h, 000h, 000h, 043h, 058h, 0EEh, 094h, 0EFh, 000h, 060h, 0B8h, 0EDh, 0C4h, 0EDh
        db  028h, 0EEh, 040h, 0B0h, 006h, 000h, 000h, 085h, 058h, 0EEh, 094h, 0EFh, 000h, 0A0h, 090h, 0EFh
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
        db  058h, 007h, 05Ch, 0C4h, 016h, 0E0h, 094h, 008h, 09Eh, 008h, 0A8h, 008h, 016h, 0E0h, 0B2h, 008h
        db  0BCh, 008h, 0B8h, 007h, 0FEh, 0D7h, 080h, 002h, 0B8h, 007h, 01Ch, 0D8h, 084h, 0FFh, 0B8h, 007h
        db  03Eh, 0D8h, 0B8h, 007h, 0B8h, 0D8h, 098h, 0D9h, 08Ch, 0D9h, 0B2h, 0D9h, 0B6h, 0D9h, 0AEh, 0D9h
        db  096h, 0DAh, 096h, 0DAh, 096h, 0DAh, 0B8h, 0D9h, 0C2h, 0D9h, 03Eh, 0DAh, 088h, 0DAh, 096h, 0DAh
        db  096h, 0DAh, 096h, 0DAh, 096h, 0DAh, 007h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 008h, 000h
        db  008h, 001h, 005h, 008h, 008h, 008h, 000h, 008h, 002h, 000h, 004h, 004h, 004h, 004h, 008h, 000h
        db  000h, 00Ah, 00Ch, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 004h, 041h, 006h, 00Bh, 008h, 0C2h
        db  000h, 0E6h, 000h, 0E7h, 004h, 006h, 004h, 007h, 004h, 003h, 006h, 004h, 004h, 005h, 004h, 088h
        db  003h, 000h, 01Ch, 000h, 000h, 00Ch, 000h, 080h, 0F6h, 0E2h, 0FEh, 0E2h, 042h, 0E3h, 002h, 0E3h
        db  00Eh, 0E3h, 014h, 0E3h, 038h, 0E3h, 008h, 0E3h, 056h, 0E3h, 000h, 006h, 000h, 000h, 006h, 007h
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
        db  056h, 006h, 024h, 0E4h, 024h, 000h, 002h, 010h, 016h, 000h, 002h, 000h, 001h, 004h, 008h, 007h
        db  024h, 0E4h, 020h, 000h, 003h, 010h, 012h, 000h, 003h, 000h, 072h, 0E3h, 014h, 08Eh, 020h, 000h
        db  004h, 010h, 012h, 000h, 004h, 000h, 0D0h, 0DAh, 020h, 000h, 005h, 0E0h, 012h, 000h, 005h, 000h
        db  0D0h, 0DAh, 020h, 000h, 006h, 060h, 012h, 000h, 006h, 000h, 01Ch, 0E6h, 012h, 000h, 001h, 060h
        db  01Ah, 009h, 0CCh, 006h, 004h, 000h, 007h, 000h, 000h, 000h, 030h, 006h, 03Ch, 0E5h, 0F0h, 005h
        db  000h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 0E2h, 005h, 008h, 000h, 026h, 0FFh, 0DCh, 005h
        db  0A8h, 0E6h, 03Eh, 000h, 000h, 060h, 000h, 0C0h, 000h, 000h, 002h, 000h, 030h, 000h, 020h, 050h
        db  023h, 00Ch, 0FCh, 005h, 052h, 006h, 056h, 006h, 000h, 000h, 000h, 081h, 016h, 000h, 000h, 060h
        db  000h, 0C0h, 000h, 000h, 010h, 000h, 008h, 000h, 02Ah, 040h, 02Ah, 004h, 056h, 006h, 026h, 000h
        db  063h, 0ECh, 02Bh, 006h, 020h, 009h, 022h, 000h, 024h, 000h, 079h, 0ECh, 023h, 00Ch, 0FCh, 005h
        db  028h, 008h, 0E6h, 008h, 029h, 008h, 058h, 007h, 078h, 007h, 098h, 007h, 023h, 000h, 02Ah, 000h
        db  087h, 0ECh, 006h, 004h, 0F0h, 006h, 007h, 004h, 0EEh, 006h, 024h, 000h, 0D0h, 0DAh, 034h, 000h
        db  000h, 060h, 000h, 0C0h, 000h, 000h, 010h, 000h, 026h, 000h, 025h, 040h, 0D0h, 0DAh, 020h, 000h
        db  000h, 060h, 000h, 0C0h, 000h, 000h, 010h, 000h, 012h, 000h, 026h, 040h, 0D0h, 0DAh, 01Ah, 000h
        db  000h, 060h, 00Ch, 000h, 027h, 060h, 00Ah, 008h, 0E6h, 006h, 0D0h, 0DAh, 024h, 000h, 000h, 060h
        db  016h, 000h, 028h, 060h, 030h, 004h, 006h, 007h, 026h, 0DBh, 000h, 081h, 030h, 000h, 000h, 060h
        db  022h, 000h, 029h, 060h, 02Dh, 008h, 01Ch, 007h, 02Eh, 008h, 022h, 007h, 000h, 000h, 008h, 002h
        db  006h, 001h, 014h, 006h, 018h, 008h, 020h, 00Ch, 026h, 00Eh, 030h, 00Fh, 034h, 011h, 03Eh, 012h
        db  042h, 014h, 046h, 016h, 01Ch, 00Ah, 04Ah, 018h, 013h, 003h, 011h, 083h, 001h, 011h, 011h, 081h
        db  012h, 081h, 013h, 001h, 052h, 083h, 081h, 085h, 085h, 011h, 012h, 081h, 012h, 081h, 019h, 081h
        db  060h, 085h, 000h, 0C0h, 000h, 000h, 008h, 000h, 01Ah, 009h, 000h, 000h, 030h, 006h, 0F6h, 0EBh
        db  054h, 006h, 050h, 006h, 038h, 002h, 021h, 004h, 0D0h, 008h, 00Bh, 006h, 0D8h, 006h, 002h, 008h
        db  0DCh, 006h, 000h, 0C0h, 0FFh, 0FFh, 0FFh, 0FFh, 041h, 000h, 041h, 000h, 014h, 0AEh, 000h, 000h
        db  000h, 081h, 009h, 004h, 00Ch, 007h, 041h, 000h, 041h, 000h, 014h, 002h, 000h, 000h, 000h, 081h
        db  00Bh, 006h, 0D8h, 006h, 02Ch, 006h, 024h, 009h, 022h, 014h, 0ECh, 008h, 041h, 000h, 041h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0B8h, 0FFh, 020h, 000h, 043h, 028h, 031h, 029h, 038h, 039h
        db  02Dh, 033h, 039h, 038h, 039h, 02Ch, 02Ch, 030h, 02Ch, 031h, 020h, 032h, 065h, 054h, 061h, 078h
        db  020h, 073h, 06Eh, 049h, 074h, 073h, 075h, 072h, 065h, 06Dh, 074h, 06Eh, 00Ah, 073h, 043h, 028h
        db  031h, 029h, 038h, 039h, 02Ch, 033h, 02Ch, 034h, 02Ch, 035h, 020h, 036h, 042h, 049h, 020h, 04Dh
        db  06Fh, 043h, 070h, 072h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0F8h, 0FFh, 001h, 000h
        db  034h, 0A0h, 000h, 000h, 0FAh, 0FFh, 001h, 000h, 0B8h, 0FFh, 000h, 000h, 0FCh, 0FFh, 002h, 000h
        db  080h, 000h, 03Eh, 0B0h, 001h, 000h, 000h, 000h, 000h, 000h, 043h, 054h, 052h, 04Fh, 04Eh, 020h
        db  034h, 02Fh, 031h, 036h, 020h, 04Dh, 062h, 070h, 073h, 020h, 054h, 052h, 000h, 000h
loc_1_68A4:
        inc    ebx
        push   esp
        push   edx
        dec    edi
        dec    esi
        and    byte ptr [edi + ebp], dh
        xor    dword ptr [esi], esi
        and    byte ptr [ebp + 62h], cl
        db  070h, 073h
        and    byte ptr [edx + edx*2], dl
        db  000h
loc_1_68B8:
        inc    ebx
        popad
        bound  ebp, qword ptr [ebp + 74h]
        db  072h, 06Fh
        outsb
        and    byte ptr [edi + ebp*2 + 6bh], dl
        outsb
        and    byte ptr [edx + 69h], dl
        outsb
        and    byte ptr [bp + 65h], cl
        db  074h, 077h
        outsd
        db  072h, 06Bh
        and    byte ptr [ebx + 61h], al
        db  072h, 064h
        and    byte ptr [esi + 6fh], ah
        db  072h, 020h
        dec    ecx
        push   ebx
        inc    ecx
        and    byte ptr [edx + 75h], ah
        db  073h, 02Eh
        add    byte ptr [eax], al
loc_1_68E8:
        add    byte ptr [eax], al
loc_1_68EA:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
_LTEXT ends
        end
