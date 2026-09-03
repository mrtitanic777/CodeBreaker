; proteon_lcode_01_01.asm -- PROTEON object 1 segment _LTEXT  (part 1/2)
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
        call   sub_1_07D0
        cmp    eax, 0
        jne    SHORT loc_1_0027
loc_1_0025:
        clc
        ret
loc_1_0027:
        stc
        ret
PROTEON_Control:
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
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_04D0:
        push   esi
        mov    ecx, dword ptr [esp + 10h]
        mov    esi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep outsb
        pop    esi
        ret    0ch
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
        db  05Eh, 0C2h, 00Ch, 000h
sub_1_050D:
        push   edi
        mov    ecx, dword ptr [esp + 10h]
        mov    edi, dword ptr [esp + 0ch]
        mov    edx, dword ptr [esp + 8]
        cld
        rep insb
        pop    edi
        ret    0ch
        db  057h, 08Bh, 04Ch, 024h, 010h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 054h, 024h, 008h, 0FCh, 0F3h, 066h
        db  06Dh, 05Fh, 0C2h, 00Ch, 000h, 057h, 08Bh, 054h, 024h, 008h, 08Bh, 07Ch, 024h, 00Ch, 08Bh, 04Ch
        db  024h, 010h, 0FCh, 0F3h, 06Dh, 05Fh, 0C2h, 00Ch, 000h
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
sub_1_05B0:
        db  0CDh, 020h, 009h, 080h, 028h, 000h
sub_1_05B6:
        db  0CDh, 020h, 008h, 080h, 028h, 000h
sub_1_05BC:
        db  0CDh, 020h, 044h, 080h, 028h, 000h
sub_1_05C2:
        db  0CDh, 020h, 048h, 080h, 028h, 000h
sub_1_05C8:
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
sub_1_068A:
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
sub_1_06CC:
        db  0CDh, 020h, 012h, 080h, 028h, 000h
sub_1_06D2:
        db  0CDh, 020h, 013h, 080h, 028h, 000h
sub_1_06D8:
        db  0CDh, 020h, 014h, 080h, 028h, 000h
sub_1_06DE:
        db  0CDh, 020h, 015h, 080h, 028h, 000h
        db  0CDh, 020h, 04Ah, 080h, 028h, 000h
        db  0CDh, 020h, 04Bh, 080h, 028h, 000h
        db  0CDh, 020h, 04Ch, 080h, 028h, 000h
        db  0CDh, 020h, 04Dh, 080h, 028h, 000h
        db  0CDh, 020h, 04Eh, 080h, 028h, 000h
        db  0CDh, 020h, 046h, 080h, 028h, 000h
        db  0CDh, 020h, 045h, 080h, 028h, 000h
        db  0CDh, 020h, 043h, 080h, 028h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 003h, 00Ah, 000h, 000h, 050h, 052h, 04Fh, 054h
        db  045h, 04Fh, 04Eh, 020h, 000h, 040h, 000h, 054h, 029h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h
        db  050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_07D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 54h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 50h]
        mov    dword ptr [ebp - 18h], eax
        mov    word ptr [ebp - 14h], 8
        mov    word ptr [ebp - 12h], 8
        db  0C7h, 045h, 0F0h, 030h, 079h, 000h, 000h
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        call   sub_1_054A
        mov    dword ptr [ebp - 54h], 0ffffffffh
        mov    eax, dword ptr [ebp - 54h]
        push   eax
        push   0
        push   8
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0636
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_0832
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_0900
loc_1_0832:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [eax], 3
        mov    eax, dword ptr [ebp - 18h]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 006h, 0B9h, 013h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 00Ah, 064h, 016h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 00Eh, 08Eh, 040h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 012h, 0B0h, 043h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 016h, 0A6h, 027h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 01Ah, 034h, 01Ah, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 01Eh, 0CBh, 026h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 022h, 0F2h, 033h, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 026h, 0D4h, 02Eh, 000h, 000h
        mov    eax, dword ptr [ebp - 18h]
        db  0C7h, 040h, 02Ah, 045h, 033h, 000h, 000h
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 10h]
        mov    edx, dword ptr [ebp - 18h]
        add    edx, 2eh
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], ecx
        push   36h
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        call   sub_1_0556
        cmp    dword ptr [ebp - 8], 0
        je     NEAR PTR loc_1_08F9
        push   0
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_0550
        mov    eax, 0c0000001h
        jmp    NEAR PTR loc_1_0900
loc_1_08F9:
        sub    eax, eax
        jmp    NEAR PTR loc_1_0900
loc_1_0900:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_0907:
        push   ebp
        mov    ebp, esp
        sub    esp, 4ch
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 3ch], 0ch
        mov    word ptr [ebp - 3ah], 0ch
        db  0C7h, 045h, 0C8h, 044h, 079h, 000h, 000h
        mov    dword ptr [ebp - 44h], 0ffffffffh
        jmp    NEAR PTR loc_1_092F
loc_1_092F:
        mov    dword ptr [ebp - 48h], 0ffffffffh
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        push   0
        push   0e9h
        lea    eax, [ebp - 30h]
        push   eax
        call   sub_1_0636
        mov    dword ptr [ebp - 34h], eax
        cmp    dword ptr [ebp - 34h], 0
        je     NEAR PTR loc_1_0961
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_1_0FBF
loc_1_0961:
        mov    edi, dword ptr [ebp - 30h]
        sub    eax, eax
        mov    ecx, 3ah
        rep stosd
        stosb
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 91h], eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 1ch], eax
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 10h]
        and    eax, 0ffffh
        cmp    eax, 10h
        jne    NEAR PTR loc_1_09C3
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [eax + 0dbh], 4642h
        mov    eax, dword ptr [ebp + 14h]
        mov    dword ptr [eax + 1ch], 4642h
        mov    eax, dword ptr [ebp + 14h]
        mov    eax, dword ptr [eax + 1ch]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 0dfh], eax
        jmp    NEAR PTR loc_1_09DD
loc_1_09C3:
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [eax + 0dbh], 1072h
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [eax + 0dfh], 1072h
loc_1_09DD:
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [eax + 0bah], 1052h
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 0dbh]
        sub    eax, 20h
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 0e3h], eax
        lea    edi, [ebp - 2ch]
        sub    eax, eax
        mov    ecx, 9
        rep stosd
        stosw
        mov    byte ptr [ebp - 28h], 1
        mov    byte ptr [ebp - 27h], 0
        mov    eax, dword ptr [ebp + 1ch]
        mov    dword ptr [ebp - 4ch], eax
        jmp    NEAR PTR loc_1_0A99
loc_1_0A20:
        mov    dword ptr [ebp - 22h], 1
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 0eh]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 2ch], eax
        mov    dword ptr [ebp - 1eh], 6
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [eax + 51h], 6
        jmp    NEAR PTR loc_1_0ABC
loc_1_0A4C:
        mov    dword ptr [ebp - 22h], 3
        mov    dword ptr [ebp - 2ch], 0
        mov    dword ptr [ebp - 1eh], 6
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [eax + 51h], 6
        jmp    NEAR PTR loc_1_0ABC
loc_1_0A70:
        mov    dword ptr [ebp - 22h], 2
        mov    dword ptr [ebp - 2ch], 0
        mov    dword ptr [ebp - 1eh], 18h
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [eax + 51h], 18h
        jmp    NEAR PTR loc_1_0ABC
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_0A99:
        cmp    dword ptr [ebp - 4ch], 0
        je     NEAR PTR loc_1_0A20
        cmp    dword ptr [ebp - 4ch], 1
        je     NEAR PTR loc_1_0A70
        cmp    dword ptr [ebp - 4ch], 2
        je     NEAR PTR loc_1_0A4C
        jmp    NEAR PTR loc_1_0ABC
loc_1_0ABC:
        mov    eax, dword ptr [ebp - 30h]
        cmp    dword ptr [eax + 0dfh], 4642h
        jne    NEAR PTR loc_1_0AE0
        mov    dword ptr [ebp - 1eh], 2
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [eax + 51h], 2
loc_1_0AE0:
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 0dfh]
        mov    dword ptr [ebp - 1ah], eax
        mov    dword ptr [ebp - 16h], 1
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 8]
        and    eax, 0ffffh
        mov    dword ptr [ebp - 12h], eax
        mov    dword ptr [ebp - 0eh], 10h
        db  0C7h, 045h, 0DAh, 0E2h, 048h, 000h, 000h
        mov    eax, dword ptr [ebp + 14h]
        mov    ax, word ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 30h]
        mov    word ptr [ecx + 0e7h], ax
        lea    eax, [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 91h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        add    eax, 95h
        push   eax
        call   sub_1_0562
        mov    dword ptr [ebp - 34h], eax
        cmp    dword ptr [ebp - 34h], 0
        je     NEAR PTR loc_1_0BB9
        mov    dword ptr [ebp - 1eh], 6
        lea    eax, [ebp - 2ch]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 91h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        add    eax, 95h
        push   eax
        call   sub_1_0562
        mov    dword ptr [ebp - 34h], eax
        cmp    dword ptr [ebp - 34h], 0
        je     NEAR PTR loc_1_0BAF
        push   0
        push   0e9h
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        call   sub_1_063C
        mov    eax, dword ptr [ebp - 34h]
        jmp    NEAR PTR loc_1_0FBF
        db  0E9h, 00Ah, 000h, 000h, 000h
loc_1_0BAF:
        mov    eax, dword ptr [ebp - 30h]
        mov    dword ptr [eax + 51h], 6
loc_1_0BB9:
        mov    eax, dword ptr [ebp - 44h]
        push   eax
        lea    eax, [ebp - 3ch]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        add    eax, 59h
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        add    eax, 55h
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        call   sub_1_06CC
        cmp    dword ptr [ebp - 34h], 0
        je     NEAR PTR loc_1_0C09
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_0568
        push   0
        push   0e9h
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        call   sub_1_063C
        mov    eax, dword ptr [ebp - 34h]
        jmp    NEAR PTR loc_1_0FBF
loc_1_0C09:
        mov    eax, dword ptr [ebp - 30h]
        add    eax, 25h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   2e4h
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_05C2
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_0C69
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 55h]
        push   eax
        call   sub_1_06DE
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_0568
        push   0
        push   0e9h
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        call   sub_1_063C
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_1_0FBF
loc_1_0C69:
        jmp    NEAR PTR loc_1_0C6E
loc_1_0C6E:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 25h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 21h], eax
        mov    eax, dword ptr [ebp - 30h]
        mov    edi, dword ptr [eax]
        sub    eax, eax
        mov    ecx, 0b9h
        rep stosd
        mov    eax, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 2dch], eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 95h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 2e0h], eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 0dfh]
        lea    eax, [eax + eax*4]
        mov    dword ptr [ebp - 40h], eax
        mov    eax, dword ptr [ebp - 30h]
        add    eax, 31h
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 40h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_05C2
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_0D43
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 25h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax]
        push   eax
        push   0
        push   2e4h
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 55h]
        push   eax
        call   sub_1_06DE
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_0568
        push   0
        push   0e9h
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        call   sub_1_063C
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_1_0FBF
loc_1_0D43:
        jmp    NEAR PTR loc_1_0D48
loc_1_0D48:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 35h], eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 0dfh]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp - 30h]
        add    eax, dword ptr [ecx + 35h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 39h], eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 31h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 29h], eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 0dfh]
        lea    eax, [eax + eax*2]
        mov    ecx, dword ptr [ebp - 30h]
        add    eax, dword ptr [ecx + 29h]
        mov    ecx, dword ptr [ebp - 30h]
        mov    dword ptr [ecx + 2dh], eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        call   sub_1_0FC6
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        call   sub_1_119D
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        call   sub_1_35E5
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        call   sub_1_341F
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_0E3D
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 31h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 35h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 40h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 25h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax]
        push   eax
        push   0
        push   2e4h
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 55h]
        push   eax
        call   sub_1_06DE
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_0568
        push   0
        push   0e9h
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        call   sub_1_063C
        mov    eax, 0c0010006h
        jmp    NEAR PTR loc_1_0FBF
loc_1_0E3D:
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        call   sub_1_12FF
        or     eax, eax
        je     NEAR PTR loc_1_0EC9
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 31h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 35h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 40h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 25h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax]
        push   eax
        push   0
        push   2e4h
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 55h]
        push   eax
        call   sub_1_06DE
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_0568
        push   0
        push   0e9h
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        call   sub_1_063C
        mov    eax, 80010004h
        jmp    NEAR PTR loc_1_0FBF
loc_1_0EC9:
        xor    eax, eax
        mov    al, byte ptr [ebp + 18h]
        cmp    eax, 1
        jne    NEAR PTR loc_1_0EE6
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_7363
loc_1_0EE6:
        mov    eax, dword ptr [ebp - 30h]
        add    eax, 99h
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        add    eax, 8dh
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax]
        add    eax, 2b8h
        push   eax
        db  068h, 00Ah, 02Eh, 000h, 000h
        db  068h, 061h, 02Bh, 000h, 000h
        db  068h, 07Dh, 02Ch, 000h, 000h
        db  068h, 00Ch, 02Ch, 000h, 000h
        db  0E8h, 0E4h, 0F0h, 0FEh, 0FFh
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_0FB8
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        call   sub_1_35C5
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_76FE
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 31h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 35h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 40h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 25h]
        push   eax
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax]
        push   eax
        push   0
        push   2e4h
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 55h]
        push   eax
        call   sub_1_06DE
        mov    eax, dword ptr [ebp - 30h]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_0568
        push   0
        push   0e9h
        mov    eax, dword ptr [ebp - 30h]
        push   eax
        call   sub_1_063C
        mov    eax, 0c000009ah
        jmp    NEAR PTR loc_1_0FBF
loc_1_0FB8:
        sub    eax, eax
        jmp    NEAR PTR loc_1_0FBF
loc_1_0FBF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    18h
sub_1_0FC6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 29ah], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 12h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 2aah], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0ah]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 29ch], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 29eh], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 10h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 2a8h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 18h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 2a6h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 16h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 2a2h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 0eh]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 2a0h], ax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2a0h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_1_1082
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 2a4h], 1
loc_1_1082:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 296h], ax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 74h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 78h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 78h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 74h], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 7ch
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 80h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 80h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 7ch], eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 84h
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 88h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 88h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 84h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 70h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0c8h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0b5h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0beh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0cbh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0cfh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0d3h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0d7h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 9dh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0a5h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0a9h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0adh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0b1h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0c2h], 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_119D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 41h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 45h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 3dh], 2
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 3fh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 49h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 51h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 4dh], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 4dh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 6bh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 5dh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 6ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 6eh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 6fh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0c4h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0c6h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0c3h], 1
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_123E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 35h]
        mov    dword ptr [ebp - 4], eax
        db  068h, 089h, 044h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_5E1B
        mov    word ptr [ebp - 8], 0
        jmp    NEAR PTR loc_1_126F
loc_1_126B:
        inc    word ptr [ebp - 8]
loc_1_126F:
        movsx  eax, word ptr [ebp - 8]
        cmp    eax, 3
        jge    NEAR PTR loc_1_12A6
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0dfh]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_57C5
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0dfh]
        add    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_126B
loc_1_12A6:
        sub    ax, ax
        jmp    NEAR PTR loc_1_12AE
loc_1_12AE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_12B5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        db  068h, 034h, 03Ch, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_68BB
        db  068h, 0D5h, 03Fh, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_68A2
        sub    ax, ax
        jmp    NEAR PTR loc_1_12E3
loc_1_12E3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_12EA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ax, ax
        jmp    NEAR PTR loc_1_12F8
loc_1_12F8:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_12FF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 55h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_06D2
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_133B
        cmp    dword ptr [ebp - 4], 0c001001dh
        je     NEAR PTR loc_1_133B
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_13B2
loc_1_133B:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_7101
        movsx  eax, ax
        or     eax, eax
        je     NEAR PTR loc_1_1373
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_35C5
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_76FE
        mov    eax, 80010004h
        jmp    NEAR PTR loc_1_13B2
loc_1_1373:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 55h]
        push   eax
        call   sub_1_06D8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_123E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12B5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12EA
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0c2h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 6bh], 1
        sub    eax, eax
        jmp    NEAR PTR loc_1_13B2
loc_1_13B2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_13B9:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 20h]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 0ch], 0
        jmp    NEAR PTR loc_1_13E7
loc_1_13E4:
        inc    dword ptr [ebp - 0ch]
loc_1_13E7:
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [ebp - 0ch], eax
        jae    NEAR PTR loc_1_140D
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 14h]
        cmp    dword ptr [ecx + eax*4], 1
        jne    NEAR PTR loc_1_1408
        jmp    NEAR PTR loc_1_140D
loc_1_1408:
        jmp    NEAR PTR loc_1_13E4
loc_1_140D:
        mov    eax, dword ptr [ebp + 18h]
        cmp    dword ptr [ebp - 0ch], eax
        jne    NEAR PTR loc_1_1423
        mov    eax, 0c0010019h
        jmp    NEAR PTR loc_1_15E1
loc_1_1423:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 70h]
        mov    dword ptr [ebp - 14h], 0ffffffffh
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        push   0
        push   96h
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_0636
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_15C2
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 1ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 10h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 14h], 0
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1dh], 0
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1ch], 0
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_1529
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0c4h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_1529
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0c4h], 1
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_758C
        movsx  eax, ax
        or     eax, eax
        je     NEAR PTR loc_1_1515
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0c4h], 0
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_063C
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 70h]
        mov    eax, 0c0010007h
        jmp    NEAR PTR loc_1_15E1
loc_1_1515:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0c4h], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0c3h], 0
loc_1_1529:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0ch
        push   eax
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 99h]
        push   eax
        db  0E8h, 0A6h, 0EBh, 0FEh, 0FFh
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_1573
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_063C
        mov    dword ptr [ebp - 10h], 0c0000001h
        jmp    NEAR PTR loc_1_15BD
loc_1_1573:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 18h], 1052h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 18h]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0bah], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 74h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 78h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 78h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 78h], eax
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 14h]
loc_1_15BD:
        jmp    NEAR PTR loc_1_15C9
loc_1_15C2:
        mov    dword ptr [ebp - 10h], 0c000009ah
loc_1_15C9:
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 70h]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0c8h], 0
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_1_15E1
loc_1_15E1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    24h
sub_1_15E8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 74h]
        mov    dword ptr [ebp - 4], eax
loc_1_1601:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 74h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_1_163B
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 18h], ecx
        jbe    NEAR PTR loc_1_162E
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 8], eax
loc_1_162E:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_1601
loc_1_163B:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_1651
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e3h]
        mov    dword ptr [ebp - 8], eax
loc_1_1651:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0bah], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_1664:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 70h]
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_1931
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 14h]
        push   0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 99h]
        push   eax
        db  0E8h, 016h, 0EBh, 0FEh, 0FFh
        mov    dword ptr [ebp - 0ch], eax
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_17A1
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 14h], 2
        jne    NEAR PTR loc_1_1728
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
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 0bah]
        cmp    dword ptr [eax + 18h], ecx
        jne    NEAR PTR loc_1_1713
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_15E8
loc_1_1713:
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_063C
        jmp    NEAR PTR loc_1_179C
loc_1_1728:
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
        mov    eax, dword ptr [ebp - 8]
        add    eax, 7ch
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 80h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 80h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 8ch], 1
        mov    eax, dword ptr [ebp - 4]
        sub    dword ptr [eax + 14h], 2
        mov    dword ptr [ebp - 0ch], 103h
loc_1_179C:
        jmp    NEAR PTR loc_1_192C
loc_1_17A1:
        cmp    dword ptr [ebp - 0ch], 103h
        jne    NEAR PTR loc_1_1820
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1dh], 1
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
        mov    eax, dword ptr [ebp - 8]
        add    eax, 7ch
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 80h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 80h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 8ch], 1
        mov    eax, dword ptr [ebp - 4]
        sub    dword ptr [eax + 14h], 2
        jmp    NEAR PTR loc_1_192C
loc_1_1820:
        cmp    dword ptr [ebp - 0ch], 0c001000eh
        jne    NEAR PTR loc_1_18A5
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1dh], 1
        mov    dword ptr [ebp - 0ch], 103h
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 1ch]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 7ch
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 80h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 80h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 80h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 8ch], 1
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_192C
loc_1_18A5:
        cmp    dword ptr [ebp - 0ch], 0c001000dh
        jne    NEAR PTR loc_1_192C
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1dh], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ecx + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 84h
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 88h]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 88h]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 88h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 8ch], 1
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 14h]
        mov    dword ptr [ebp - 0ch], 103h
        jmp    NEAR PTR loc_1_192C
loc_1_192C:
        jmp    NEAR PTR loc_1_1938
loc_1_1931:
        mov    dword ptr [ebp - 0ch], 0c0010002h
loc_1_1938:
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 70h]
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_1946
loc_1_1946:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_194D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0b5h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_1971
        jmp    NEAR PTR loc_1_1A2D
loc_1_1971:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 8ch]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_198C
        jmp    NEAR PTR loc_1_1A2D
loc_1_198C:
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 70h]
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 8ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 7ch]
        mov    dword ptr [ebp - 4], eax
loc_1_19A5:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 7ch
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_1_1A22
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 14h], 0
        jne    NEAR PTR loc_1_1A0B
        mov    eax, dword ptr [ebp - 8]
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
        push   0
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_0574
        push   0
        push   96h
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_063C
        jmp    NEAR PTR loc_1_1A15
loc_1_1A0B:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 8ch], 1
loc_1_1A15:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_19A5
loc_1_1A22:
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 70h]
        jmp    NEAR PTR loc_1_1A2D
loc_1_1A2D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_1A34:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 70h]
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 0
        jne    NEAR PTR loc_1_1A7F
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_2153
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_1B2A
loc_1_1A7F:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 10h], 1
        jne    NEAR PTR loc_1_1B23
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0c8h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_1ADA
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_1ACE
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_21CD
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_1AD5
loc_1_1ACE:
        mov    dword ptr [ebp - 0ch], 0c0010002h
loc_1_1AD5:
        jmp    NEAR PTR loc_1_1B1E
loc_1_1ADA:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0b5h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_1AFC
        mov    dword ptr [ebp - 0ch], 0c001000dh
        jmp    NEAR PTR loc_1_1B1E
loc_1_1AFC:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_1B1E
        mov    dword ptr [ebp - 0ch], 0c0000001h
        jmp    NEAR PTR loc_1_1B1E
loc_1_1B1E:
        jmp    NEAR PTR loc_1_1B2A
loc_1_1B23:
        mov    dword ptr [ebp - 0ch], 10001h
loc_1_1B2A:
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 70h]
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_1B38
loc_1_1B38:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_1B3F:
        push   ebp
        mov    ebp, esp
        sub    esp, 44h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 1ch], 1
        mov    dword ptr [ebp - 10h], 0
        mov    word ptr [ebp - 14h], 0
        mov    dword ptr [ebp - 18h], 0
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 4
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 40h], eax
        jmp    NEAR PTR loc_1_2058
loc_1_1B82:
        mov    dword ptr [ebp - 10h], 7
        jmp    NEAR PTR loc_1_2101
loc_1_1B8E:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        or     eax, eax
        jne    NEAR PTR loc_1_1BAE
        db  0C7h, 045h, 0F4h, 0CCh, 078h, 000h, 000h
        mov    dword ptr [ebp - 8], 5ch
        jmp    NEAR PTR loc_1_1BBC
loc_1_1BAE:
        db  0C7h, 045h, 0F4h, 050h, 078h, 000h, 000h
        mov    dword ptr [ebp - 8], 7ch
loc_1_1BBC:
        jmp    NEAR PTR loc_1_2101
loc_1_1BC1:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0b5h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_1BE3
        mov    dword ptr [ebp - 4], 2
        jmp    NEAR PTR loc_1_1C44
loc_1_1BE3:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0c2h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_1C0F
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0c4h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_1C1B
loc_1_1C0F:
        mov    dword ptr [ebp - 4], 1
        jmp    NEAR PTR loc_1_1C44
loc_1_1C1B:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0c8h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_1C3D
        mov    dword ptr [ebp - 4], 4
        jmp    NEAR PTR loc_1_1C44
loc_1_1C3D:
        mov    dword ptr [ebp - 4], 0
loc_1_1C44:
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_2101
loc_1_1C56:
        lea    eax, [ebp - 1ch]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_2101
loc_1_1C68:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0e3h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_2101
loc_1_1C86:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0dfh]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp + 10h], 10106h
        jne    NEAR PTR loc_1_1CA3
        sub    dword ptr [ebp - 10h], 0eh
loc_1_1CA3:
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_2101
loc_1_1CB5:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax + 2a8h]
        and    eax, 0ffffh
        cmp    eax, 10h
        jne    NEAR PTR loc_1_1CDB
        mov    dword ptr [ebp - 10h], 27100h
        jmp    NEAR PTR loc_1_1CE2
loc_1_1CDB:
        mov    dword ptr [ebp - 10h], 9c40h
loc_1_1CE2:
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_2101
loc_1_1CF4:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0dfh]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_2101
loc_1_1D12:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0dfh]
        lea    eax, [eax + eax*2]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_2101
loc_1_1D33:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0dfh]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_2101
loc_1_1D51:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0dfh]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_2101
loc_1_1D6F:
        db  0A0h, 04Fh, 078h, 000h, 000h
        mov    byte ptr [ebp - 24h], al
        db  0A0h, 04Eh, 078h, 000h, 000h
        mov    byte ptr [ebp - 23h], al
        db  0A0h, 04Dh, 078h, 000h, 000h
        mov    byte ptr [ebp - 22h], al
        db  0A0h, 04Ch, 078h, 000h, 000h
        mov    byte ptr [ebp - 21h], al
        lea    eax, [ebp - 24h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_2101
loc_1_1DA1:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 44h], eax
        jmp    NEAR PTR loc_1_1DD8
loc_1_1DAF:
        db  0C7h, 045h, 0F4h, 050h, 079h, 000h, 000h
        jmp    NEAR PTR loc_1_1DFB
loc_1_1DBB:
        db  0C7h, 045h, 0F4h, 078h, 079h, 000h, 000h
        jmp    NEAR PTR loc_1_1DFB
loc_1_1DC7:
        db  0C7h, 045h, 0F4h, 0A0h, 079h, 000h, 000h
        jmp    NEAR PTR loc_1_1DFB
        db  0E9h, 023h, 000h, 000h, 000h
loc_1_1DD8:
        cmp    dword ptr [ebp - 44h], 0
        je     NEAR PTR loc_1_1DAF
        cmp    dword ptr [ebp - 44h], 1
        je     NEAR PTR loc_1_1DC7
        cmp    dword ptr [ebp - 44h], 2
        je     NEAR PTR loc_1_1DBB
        jmp    NEAR PTR loc_1_1DFB
loc_1_1DFB:
        mov    dword ptr [ebp - 8], 27h
        jmp    NEAR PTR loc_1_2101
loc_1_1E07:
        mov    word ptr [ebp - 14h], 300h
        lea    eax, [ebp - 14h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 2
        jmp    NEAR PTR loc_1_2101
loc_1_1E1F:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        or     eax, eax
        je     NEAR PTR loc_1_1E3D
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 9dh]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_1E49
loc_1_1E3D:
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 10h], eax
loc_1_1E49:
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_2101
loc_1_1E5B:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        or     eax, eax
        jne    NEAR PTR loc_1_1E76
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_1ED2
loc_1_1E76:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 74h]
        mov    dword ptr [ebp - 28h], eax
        mov    dword ptr [ebp - 10h], 0
loc_1_1E86:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 74h
        cmp    eax, dword ptr [ebp - 28h]
        je     NEAR PTR loc_1_1ED2
        mov    eax, dword ptr [ebp - 28h]
        mov    dword ptr [ebp - 2ch], eax
        mov    eax, dword ptr [ebp - 2ch]
        mov    ecx, dword ptr [ebp - 10h]
        cmp    dword ptr [eax + 18h], ecx
        jbe    NEAR PTR loc_1_1EC5
        mov    eax, dword ptr [ebp - 2ch]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 10h], eax
        cmp    dword ptr [ebp - 10h], 4622h
        jne    NEAR PTR loc_1_1EC5
        jmp    NEAR PTR loc_1_1ED2
loc_1_1EC5:
        mov    eax, dword ptr [ebp - 28h]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 28h], eax
        jmp    NEAR PTR loc_1_1E86
loc_1_1ED2:
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_2101
loc_1_1EE4:
        lea    eax, [ebp - 24h]
        mov    dword ptr [ebp - 30h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        add    eax, 2b2h
        mov    dword ptr [ebp - 34h], eax
        mov    eax, dword ptr [ebp - 34h]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 34h]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 34h]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 34h]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 34h]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 34h]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 30h]
        mov    byte ptr [ecx + 5], al
        lea    eax, [ebp - 24h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 6
        jmp    NEAR PTR loc_1_2101
loc_1_1F4F:
        lea    eax, [ebp - 24h]
        mov    dword ptr [ebp - 38h], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        add    eax, 2b8h
        mov    dword ptr [ebp - 3ch], eax
        mov    eax, dword ptr [ebp - 3ch]
        mov    al, byte ptr [eax]
        mov    ecx, dword ptr [ebp - 38h]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 3ch]
        mov    al, byte ptr [eax + 1]
        mov    ecx, dword ptr [ebp - 38h]
        mov    byte ptr [ecx + 1], al
        mov    eax, dword ptr [ebp - 3ch]
        mov    al, byte ptr [eax + 2]
        mov    ecx, dword ptr [ebp - 38h]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 3ch]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 38h]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp - 3ch]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 38h]
        mov    byte ptr [ecx + 4], al
        mov    eax, dword ptr [ebp - 3ch]
        mov    al, byte ptr [eax + 5]
        mov    ecx, dword ptr [ebp - 38h]
        mov    byte ptr [ecx + 5], al
        lea    eax, [ebp - 24h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 6
        jmp    NEAR PTR loc_1_2101
loc_1_1FBA:
        xor    eax, eax
        mov    al, byte ptr [ebp + 14h]
        or     eax, eax
        jne    NEAR PTR loc_1_1FD8
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_1FE3
loc_1_1FD8:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 1ah]
        mov    dword ptr [ebp - 10h], eax
loc_1_1FE3:
        mov    eax, dword ptr [ebp - 10h]
        shl    eax, 8
        and    eax, 0ff0000h
        mov    ecx, dword ptr [ebp - 10h]
        shr    ecx, 18h
        mov    al, cl
        mov    ecx, dword ptr [ebp - 10h]
        shl    ecx, 18h
        and    ecx, 0ff000000h
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 10h]
        shr    ecx, 8
        and    ecx, 0ff00h
        or     eax, ecx
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_2101
loc_1_2027:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    eax, dword ptr [eax + 2c0h]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_2101
loc_1_2047:
        mov    dword ptr [ebp - 18h], 0c00000bbh
        jmp    NEAR PTR loc_1_2101
        db  0E9h, 0A9h, 000h, 000h, 000h
loc_1_2058:
        cmp    dword ptr [ebp - 40h], 2010101h
        db  00Fh, 087h, 070h, 000h, 000h, 000h
        je     NEAR PTR loc_1_1EE4
        sub    dword ptr [ebp - 40h], 10101h
        cmp    dword ptr [ebp - 40h], 12h
        ja     NEAR PTR loc_1_2047
        shl    dword ptr [ebp - 40h], 2
        mov    eax, dword ptr [ebp - 40h]
        db  0FFh, 0A0h, 089h, 020h, 000h, 000h
loc_1_2089:
        mov    ds, word ptr [ebx]
        add    byte ptr [eax], al
        rcr    dword ptr [ebx], 0
        db  000h, 056h, 01Ch
        add    byte ptr [eax], al
        push   esi
        sbb    al, 0
        db  000h, 068h, 01Ch
        add    byte ptr [eax], al
        xchg   byte ptr [eax + eax], bl
        db  000h, 0B5h, 01Ch, 000h, 000h, 0F4h
        sbb    al, 0
        db  000h, 012h
        db  01Dh, 000h, 000h, 033h, 01Dh
        add    byte ptr [eax], al
        push   ecx
        db  01Dh, 000h, 000h, 06Fh, 01Dh
        add    byte ptr [eax], al
        db  0A1h, 01Dh, 000h, 000h, 01Fh
        push   ds
        add    byte ptr [eax], al
        pop    ebx
        push   ds
        add    byte ptr [eax], al
        pop    es
        push   ds
        add    byte ptr [eax], al
        xchg   byte ptr [eax + eax], bl
        db  000h, 047h, 020h
        add    byte ptr [eax], al
        sbb    byte ptr [ebx], 0
        add    byte ptr [ecx + 102c07dh], al
        add    dword ptr [edx], eax
        je     NEAR PTR loc_1_1F4F
        cmp    dword ptr [ebp - 40h], 2010103h
        je     NEAR PTR loc_1_1FBA
        cmp    dword ptr [ebp - 40h], 2010104h
        je     NEAR PTR loc_1_2027
        jmp    NEAR PTR loc_1_2047
loc_1_2101:
        cmp    dword ptr [ebp - 18h], 0
        jne    NEAR PTR loc_1_2144
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp + 1ch], eax
        jae    NEAR PTR loc_1_212B
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 20h]
        mov    dword ptr [ecx], eax
        mov    dword ptr [ebp - 18h], 0c0010014h
        jmp    NEAR PTR loc_1_2144
loc_1_212B:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        call   sub_1_071C
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 24h]
        add    dword ptr [ecx], eax
loc_1_2144:
        mov    eax, dword ptr [ebp - 18h]
        jmp    NEAR PTR loc_1_214C
loc_1_214C:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    20h
sub_1_2153:
        push   ebp
        mov    ebp, esp
        sub    esp, 14h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 14h], 0
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1B3F
        mov    dword ptr [ebp - 14h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_21C6
loc_1_21C6:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_21CD:
        push   ebp
        mov    ebp, esp
        sub    esp, 24h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 18h], eax
        mov    dword ptr [ebp - 20h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 4
        je     NEAR PTR loc_1_2232
        mov    dword ptr [ebp - 20h], 0c0010014h
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 24h], 0
        mov    eax, dword ptr [ebp - 20h]
        jmp    NEAR PTR loc_1_23E1
loc_1_2232:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 24h], eax
        jmp    NEAR PTR loc_1_2376
loc_1_223D:
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2436
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_23BC
loc_1_225A:
        push   4
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_071C
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_23E8
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_23BC
loc_1_2286:
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_2487
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_23BC
loc_1_22A3:
        mov    dword ptr [ebp - 20h], 0
        jmp    NEAR PTR loc_1_23BC
loc_1_22AF:
        push   4
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        call   sub_1_071C
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 0e3h], ecx
        jb     NEAR PTR loc_1_2345
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 0bah], ecx
        jae    NEAR PTR loc_1_22FC
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0bah], eax
        jmp    NEAR PTR loc_1_2340
loc_1_22FC:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0bah]
        cmp    dword ptr [eax + 18h], ecx
        jne    NEAR PTR loc_1_2337
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 14h]
        cmp    dword ptr [eax + 18h], ecx
        jbe    NEAR PTR loc_1_2337
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_15E8
        jmp    NEAR PTR loc_1_2340
loc_1_2337:
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 18h], eax
loc_1_2340:
        jmp    NEAR PTR loc_1_234C
loc_1_2345:
        mov    dword ptr [ebp - 20h], 0c0010014h
loc_1_234C:
        jmp    NEAR PTR loc_1_23BC
loc_1_2351:
        mov    dword ptr [ebp - 20h], 0c0010017h
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 20h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 24h], 0
        jmp    NEAR PTR loc_1_23BC
        db  0E9h, 046h, 000h, 000h, 000h
loc_1_2376:
        cmp    dword ptr [ebp - 24h], 1010eh
        je     NEAR PTR loc_1_225A
        cmp    dword ptr [ebp - 24h], 1010fh
        je     NEAR PTR loc_1_22AF
        cmp    dword ptr [ebp - 24h], 10112h
        je     NEAR PTR loc_1_22A3
        cmp    dword ptr [ebp - 24h], 2010103h
        je     NEAR PTR loc_1_223D
        cmp    dword ptr [ebp - 24h], 2010104h
        je     NEAR PTR loc_1_2286
        jmp    NEAR PTR loc_1_2351
loc_1_23BC:
        cmp    dword ptr [ebp - 20h], 0
        jne    NEAR PTR loc_1_23D9
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [eax + 24h], 0
loc_1_23D9:
        mov    eax, dword ptr [ebp - 20h]
        jmp    NEAR PTR loc_1_23E1
loc_1_23E1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_23E8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [eax + 14h]
        push   1
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 99h]
        push   eax
        db  0E8h, 0D2h, 0E0h, 0FEh, 0FFh
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_242F
loc_1_242F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_2436:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [eax + 14h]
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
        mov    eax, dword ptr [eax + 99h]
        push   eax
        db  0E8h, 051h, 0DEh, 0FEh, 0FFh
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_2480
loc_1_2480:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_2487:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [eax + 14h]
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
        mov    eax, dword ptr [eax + 99h]
        push   eax
        db  0E8h, 0A0h, 0DEh, 0FEh, 0FFh
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_24D1
loc_1_24D1:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_24D8:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 1ch]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 18h]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 1ch], 0
        mov    dword ptr [ebp - 10h], 0
        mov    dword ptr [ebp - 4], 0ch
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1B3F
        mov    dword ptr [ebp - 1ch], eax
        cmp    dword ptr [ebp - 1ch], 0c00000bbh
        jne    NEAR PTR loc_1_26AA
        mov    dword ptr [ebp - 1ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 20h], eax
        jmp    NEAR PTR loc_1_25F4
loc_1_255F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0cbh]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_266C
loc_1_2570:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0cfh]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_266C
loc_1_2581:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d3h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_266C
loc_1_2592:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0d7h]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_266C
loc_1_25A3:
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_266C
loc_1_25AF:
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_266C
loc_1_25BB:
        mov    dword ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_266C
loc_1_25C7:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0c6h]
        and    eax, 0ffffh
        or     eax, 0c0011000h
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_266C
loc_1_25E3:
        mov    dword ptr [ebp - 1ch], 0c0010017h
        jmp    NEAR PTR loc_1_266C
        db  0E9h, 078h, 000h, 000h, 000h
loc_1_25F4:
        cmp    dword ptr [ebp - 20h], 20105h
        ja     NEAR PTR loc_1_2640
        je     NEAR PTR loc_1_25A3
        cmp    dword ptr [ebp - 20h], 20101h
        je     NEAR PTR loc_1_255F
        cmp    dword ptr [ebp - 20h], 20102h
        je     NEAR PTR loc_1_2570
        cmp    dword ptr [ebp - 20h], 20103h
        je     NEAR PTR loc_1_2581
        cmp    dword ptr [ebp - 20h], 20104h
        je     NEAR PTR loc_1_2592
        jmp    NEAR PTR loc_1_25E3
loc_1_2640:
        cmp    dword ptr [ebp - 20h], 2010105h
        je     NEAR PTR loc_1_25C7
        cmp    dword ptr [ebp - 20h], 2020101h
        je     NEAR PTR loc_1_25AF
        cmp    dword ptr [ebp - 20h], 2020102h
        je     NEAR PTR loc_1_25BB
        jmp    NEAR PTR loc_1_25E3
loc_1_266C:
        cmp    dword ptr [ebp - 1ch], 0
        jne    NEAR PTR loc_1_26AA
        cmp    dword ptr [ebp - 18h], 4
        jb     NEAR PTR loc_1_2698
        push   4
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        call   sub_1_071C
        add    dword ptr [ebp - 8], 4
        jmp    NEAR PTR loc_1_26AA
loc_1_2698:
        mov    eax, 4
        sub    eax, dword ptr [ebp - 18h]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 1ch], 0c0010014h
loc_1_26AA:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 20h], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    dword ptr [ecx + 24h], eax
        mov    eax, dword ptr [ebp - 1ch]
        jmp    NEAR PTR loc_1_26C4
loc_1_26C4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
loc_1_26CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 70h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    eax, dword ptr [eax + 14h]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_270B
        db  0E9h, 083h, 000h, 000h, 000h
loc_1_26FA:
        mov    dword ptr [ebp - 8], 0c0010017h
        jmp    NEAR PTR loc_1_277D
        db  0E9h, 072h, 000h, 000h, 000h
loc_1_270B:
        cmp    dword ptr [ebp - 0ch], 2010103h
        ja     NEAR PTR loc_1_275E
        cmp    dword ptr [ebp - 0ch], 2010101h
        jae    NEAR PTR loc_1_277D
        cmp    dword ptr [ebp - 0ch], 10101h
        jb     NEAR PTR loc_1_26FA
        cmp    dword ptr [ebp - 0ch], 10113h
        jbe    NEAR PTR loc_1_277D
        cmp    dword ptr [ebp - 0ch], 20101h
        jb     NEAR PTR loc_1_26FA
        cmp    dword ptr [ebp - 0ch], 20105h
        jbe    NEAR PTR loc_1_277D
        jmp    NEAR PTR loc_1_26FA
loc_1_275E:
        cmp    dword ptr [ebp - 0ch], 2020101h
        jb     NEAR PTR loc_1_26FA
        cmp    dword ptr [ebp - 0ch], 2020102h
        jbe    NEAR PTR loc_1_277D
        jmp    NEAR PTR loc_1_26FA
loc_1_277D:
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_2797
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_24D8
        mov    dword ptr [ebp - 8], eax
loc_1_2797:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_279F
loc_1_279F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
loc_1_27A6:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 14h], 103h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [eax + 70h]
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 0c8h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_28A3
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 18h], eax
        mov    eax, dword ptr [ebp - 18h]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_2897
        mov    eax, dword ptr [ebp - 18h]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dx, word ptr [eax + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0fff7h
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, cx
        out    dx, ax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 74h]
        mov    dword ptr [ebp - 4], eax
loc_1_282F:
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 74h
        cmp    eax, dword ptr [ebp - 4]
        je     NEAR PTR loc_1_2866
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
        jmp    NEAR PTR loc_1_282F
loc_1_2866:
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 0b5h], 1
        mov    eax, dword ptr [ebp - 0ch]
        mov    byte ptr [eax + 0c8h], 1
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 0beh], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_28F6
        mov    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_289E
loc_1_2897:
        mov    dword ptr [ebp - 14h], 0c0010002h
loc_1_289E:
        jmp    NEAR PTR loc_1_28E7
loc_1_28A3:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 0b5h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_28C5
        mov    dword ptr [ebp - 14h], 0c001000dh
        jmp    NEAR PTR loc_1_28E7
loc_1_28C5:
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_28E7
        mov    dword ptr [ebp - 14h], 0c0000001h
        jmp    NEAR PTR loc_1_28E7
loc_1_28E7:
        mov    eax, dword ptr [ebp - 14h]
        jmp    NEAR PTR loc_1_28EF
loc_1_28EF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_28F6:
        push   ebp
        mov    ebp, esp
        sub    esp, 20h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 70h]
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0b5h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_2B02
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 41h], 0
        je     NEAR PTR loc_1_29A0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 41h]
        mov    dword ptr [ebp - 1ch], eax
loc_1_2931:
        mov    eax, dword ptr [ebp - 1ch]
        add    eax, 24h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 8]
        inc    dword ptr [eax + 0d3h]
        push   0c001000ch
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_057A
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0fh], 1
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0fh]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_2988
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3BA2
loc_1_2988:
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 1ch], eax
        cmp    dword ptr [ebp - 1ch], 0
        jne    NEAR PTR loc_1_2931
loc_1_29A0:
        mov    byte ptr [ebp - 8], 1
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_76FE
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 55h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        call   sub_1_06D2
        cmp    dword ptr [ebp - 18h], 0
        je     NEAR PTR loc_1_29DA
        cmp    dword ptr [ebp - 18h], 0c001001dh
        jne    NEAR PTR loc_1_2A34
loc_1_29DA:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_6D21
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_7101
        movsx  eax, ax
        or     eax, eax
        jne    NEAR PTR loc_1_2A34
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_123E
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12B5
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_12EA
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_758C
        movsx  eax, ax
        or     eax, eax
        jne    NEAR PTR loc_1_2A34
        mov    byte ptr [ebp - 8], 0
loc_1_2A34:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 55h]
        push   eax
        call   sub_1_06D8
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_119D
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     eax, eax
        je     NEAR PTR loc_1_2A85
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 6bh], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0c3h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0b5h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0c8h], 0
        mov    eax, 80010004h
        jmp    NEAR PTR loc_1_2B5A
loc_1_2A85:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 6bh], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0c3h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0b5h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0c8h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 74h]
        mov    dword ptr [ebp - 0ch], eax
loc_1_2AB3:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 74h
        cmp    eax, dword ptr [ebp - 0ch]
        je     NEAR PTR loc_1_2AF6
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 14h], eax
        push   0
        push   0
        push   40010005h
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_059E
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_05A4
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_2AB3
loc_1_2AF6:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0beh]
        dec    dword ptr [eax + 14h]
loc_1_2B02:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 84h
        cmp    dword ptr [eax + 84h], ecx
        je     NEAR PTR loc_1_2B53
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 84h]
        mov    dword ptr [ebp - 0ch], eax
loc_1_2B26:
        mov    eax, dword ptr [ebp + 8]
        add    eax, 84h
        cmp    eax, dword ptr [ebp - 0ch]
        je     NEAR PTR loc_1_2B53
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_1664
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 0ch], eax
        jmp    NEAR PTR loc_1_2B26
loc_1_2B53:
        sub    eax, eax
        jmp    NEAR PTR loc_1_2B5A
loc_1_2B5A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_2B61:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 0b5h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_2B9B
        mov    dword ptr [ebp - 8], 0c001000dh
        jmp    NEAR PTR loc_1_2BFD
loc_1_2B9B:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0ch], eax
        jne    NEAR PTR loc_1_2BA7
loc_1_2BA7:
        test   byte ptr [ebp + 0ch], 24h
        jne    NEAR PTR loc_1_2BBB
        test   byte ptr [ebp + 0dh], 80h
        je     NEAR PTR loc_1_2BC7
loc_1_2BBB:
        mov    dword ptr [ebp - 8], 0c00000bbh
        jmp    NEAR PTR loc_1_2BFD
loc_1_2BC7:
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 9dh]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 0a1h], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 9dh], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_2D36
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 14h]
loc_1_2BFD:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_2C05
loc_1_2C05:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_1_2C0C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 0b5h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_2C4D
        mov    dword ptr [ebp - 8], 0c001000dh
        jmp    NEAR PTR loc_1_2C6E
loc_1_2C4D:
        mov    eax, dword ptr [ebp - 4]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 0ch]
        mov    dword ptr [ecx + 0a5h], eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_2D36
        mov    eax, dword ptr [ebp - 4]
        dec    dword ptr [eax + 14h]
loc_1_2C6E:
        mov    eax, dword ptr [ebp - 8]
        jmp    NEAR PTR loc_1_2C76
loc_1_2C76:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_1_2C7D:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp - 10h], 0
        mov    eax, dword ptr [ebp + 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    byte ptr [ebp - 8], 0c0h
        mov    byte ptr [ebp - 7], 0
        mov    word ptr [ebp - 18h], 0
        jmp    NEAR PTR loc_1_2CB3
loc_1_2CAF:
        inc    word ptr [ebp - 18h]
loc_1_2CB3:
        mov    eax, dword ptr [ebp - 18h]
        and    eax, 0ffffh
        cmp    eax, 4
        jge    NEAR PTR loc_1_2CEA
        mov    eax, 3
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    al, byte ptr [ebp + eax + 0ch]
        mov    ecx, dword ptr [ebp - 18h]
        and    ecx, 0ffffh
        mov    byte ptr [ebp + ecx - 6], al
        jmp    NEAR PTR loc_1_2CAF
loc_1_2CEA:
        mov    eax, dword ptr [ebp - 14h]
        mov    al, byte ptr [eax + 0b5h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_2D0C
        mov    dword ptr [ebp - 10h], 0c001000dh
        jmp    NEAR PTR loc_1_2D27
loc_1_2D0C:
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [eax + 14h]
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 14h]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_74AC
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
loc_1_2D27:
        mov    eax, dword ptr [ebp - 10h]
        jmp    NEAR PTR loc_1_2D2F
loc_1_2D2F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
sub_1_2D36:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 7fffffffh
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 9eh], 20h
        je     NEAR PTR loc_1_2D61
        mov    eax, dword ptr [ebp - 0ch]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_2D89
loc_1_2D61:
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 9eh], 40h
        je     NEAR PTR loc_1_2D82
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0a5h]
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_2D89
loc_1_2D82:
        mov    dword ptr [ebp - 4], 0
loc_1_2D89:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 0a9h], ecx
        je     NEAR PTR loc_1_2DFC
        mov    word ptr [ebp - 10h], 0
        jmp    NEAR PTR loc_1_2DAA
loc_1_2DA6:
        inc    word ptr [ebp - 10h]
loc_1_2DAA:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 4
        jge    NEAR PTR loc_1_2DE1
        mov    eax, 3
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        sub    eax, ecx
        mov    al, byte ptr [ebp + eax - 4]
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        mov    byte ptr [ebp + ecx - 8], al
        jmp    NEAR PTR loc_1_2DA6
loc_1_2DE1:
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_73B0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 0a9h], eax
loc_1_2DFC:
        sub    eax, eax
        jmp    NEAR PTR loc_1_2E03
loc_1_2E03:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_2E0A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        dec    dword ptr [eax + 14h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_2E1D:
        push   ebp
        mov    ebp, esp
        sub    esp, 154h
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 14h], 1
        jmp    NEAR PTR loc_1_2E38
loc_1_2E35:
        inc    dword ptr [ebp - 14h]
loc_1_2E38:
        cmp    dword ptr [ebp - 14h], 10h
        jae    NEAR PTR loc_1_2E76
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_1_05B0
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_2E71
        mov    eax, 2
        jmp    NEAR PTR loc_1_2ECD
loc_1_2E71:
        jmp    NEAR PTR loc_1_2E35
loc_1_2E76:
        mov    dword ptr [ebp - 14h], 1
        jmp    NEAR PTR loc_1_2E85
loc_1_2E82:
        inc    dword ptr [ebp - 14h]
loc_1_2E85:
        cmp    dword ptr [ebp - 14h], 10h
        jae    NEAR PTR loc_1_2EC6
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 4], eax
        lea    eax, [ebp - 154h]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        call   sub_1_05B6
        cmp    dword ptr [ebp - 10h], 0
        jne    NEAR PTR loc_1_2EC1
        mov    eax, 1
        jmp    NEAR PTR loc_1_2ECD
loc_1_2EC1:
        jmp    NEAR PTR loc_1_2E82
loc_1_2EC6:
        sub    eax, eax
        jmp    NEAR PTR loc_1_2ECD
loc_1_2ECD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_2ED4:
        push   ebp
        mov    ebp, esp
        sub    esp, 88h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    word ptr [ebp - 80h], 7
        mov    word ptr [ebp - 7eh], 7
        db  0C7h, 045h, 084h, 0D0h, 079h, 000h, 000h
        mov    word ptr [ebp - 70h], 9
        mov    word ptr [ebp - 6eh], 9
        db  0C7h, 045h, 094h, 0E4h, 079h, 000h, 000h
        mov    word ptr [ebp - 78h], 0bh
        mov    word ptr [ebp - 76h], 0bh
        db  0C7h, 045h, 08Ch, 0FCh, 079h, 000h, 000h
        mov    word ptr [ebp - 58h], 0ah
        mov    word ptr [ebp - 56h], 0ah
        db  0C7h, 045h, 0ACh, 014h, 07Ah, 000h, 000h
        mov    word ptr [ebp - 68h], 0ah
        mov    word ptr [ebp - 66h], 0ah
        db  0C7h, 045h, 09Ch, 02Ch, 07Ah, 000h, 000h
        mov    word ptr [ebp - 50h], 0bh
        mov    word ptr [ebp - 4eh], 0bh
        db  0C7h, 045h, 0B4h, 044h, 07Ah, 000h, 000h
        mov    word ptr [ebp - 18h], 4
        mov    word ptr [ebp - 16h], 4
        db  0C7h, 045h, 0ECh, 054h, 07Ah, 000h, 000h
        mov    word ptr [ebp - 0ch], 0bh
        mov    word ptr [ebp - 0ah], 0bh
        db  0C7h, 045h, 0F8h, 064h, 07Ah, 000h, 000h
        mov    byte ptr [ebp - 24h], 1
        mov    dword ptr [ebp - 1ch], 0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_2E1D
        mov    dword ptr [ebp - 10h], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_1_06B4
        cmp    dword ptr [ebp - 1ch], 0
        je     NEAR PTR loc_1_2FB8
        mov    eax, dword ptr [ebp - 1ch]
        jmp    NEAR PTR loc_1_333E
loc_1_2FB8:
        lea    eax, [ebp - 44h]
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 4], 0
        mov    word ptr [ebp - 3ch], 0a20h
        mov    word ptr [ebp - 3ah], 5
        mov    word ptr [ebp - 38h], 1
        mov    word ptr [ebp - 36h], 5
        mov    word ptr [ebp - 34h], 10h
        mov    word ptr [ebp - 32h], 1
        mov    word ptr [ebp - 30h], 0
        mov    word ptr [ebp - 2ch], 1
        mov    word ptr [ebp - 2eh], 8
        mov    dword ptr [ebp - 28h], 1072h
        push   0
        lea    eax, [ebp - 80h]
        push   eax
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        lea    eax, [ebp - 5ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 1ch], 0
        jne    NEAR PTR loc_1_3036
        mov    eax, dword ptr [ebp - 5ch]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [ebp - 3ch], ax
        mov    dword ptr [ebp - 10h], 0
loc_1_3036:
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        lea    eax, [ebp - 5ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 1ch], 0
        jne    NEAR PTR loc_1_3085
        mov    eax, dword ptr [ebp - 5ch]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 28h], eax
        cmp    dword ptr [ebp - 28h], 4642h
        jbe    NEAR PTR loc_1_3074
        mov    dword ptr [ebp - 28h], 4642h
loc_1_3074:
        cmp    dword ptr [ebp - 28h], 0fh
        jae    NEAR PTR loc_1_3085
        mov    dword ptr [ebp - 28h], 0fh
loc_1_3085:
        push   0
        lea    eax, [ebp - 70h]
        push   eax
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        lea    eax, [ebp - 5ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 1ch], 0
        jne    NEAR PTR loc_1_30B0
        mov    eax, dword ptr [ebp - 5ch]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [ebp - 3ah], ax
loc_1_30B0:
        push   0
        lea    eax, [ebp - 78h]
        push   eax
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        lea    eax, [ebp - 5ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 1ch], 0
        jne    NEAR PTR loc_1_30DB
        mov    eax, dword ptr [ebp - 5ch]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [ebp - 36h], ax
loc_1_30DB:
        push   0
        lea    eax, [ebp - 58h]
        push   eax
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        lea    eax, [ebp - 5ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 1ch], 0
        jne    NEAR PTR loc_1_3106
        mov    eax, dword ptr [ebp - 5ch]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [ebp - 34h], ax
loc_1_3106:
        push   0
        lea    eax, [ebp - 50h]
        push   eax
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        lea    eax, [ebp - 5ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 1ch], 0
        jne    NEAR PTR loc_1_3131
        mov    eax, dword ptr [ebp - 5ch]
        mov    eax, dword ptr [eax + 4]
        mov    word ptr [ebp - 30h], ax
loc_1_3131:
        push   0
        lea    eax, [ebp - 68h]
        push   eax
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        lea    eax, [ebp - 5ch]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_1_06BA
        cmp    dword ptr [ebp - 1ch], 0
        jne    NEAR PTR loc_1_3178
        mov    eax, dword ptr [ebp - 5ch]
        mov    ax, word ptr [eax + 4]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_3172
        mov    word ptr [ebp - 32h], 1
        jmp    NEAR PTR loc_1_3178
loc_1_3172:
        mov    word ptr [ebp - 32h], 0
loc_1_3178:
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        lea    eax, [ebp - 60h]
        push   eax
        lea    eax, [ebp - 20h]
        push   eax
        lea    eax, [ebp - 1ch]
        push   eax
        call   sub_1_05BC
        cmp    dword ptr [ebp - 1ch], 0
        jne    NEAR PTR loc_1_3206
        cmp    dword ptr [ebp - 60h], 6
        jne    NEAR PTR loc_1_3206
        mov    eax, dword ptr [ebp - 20h]
        mov    al, byte ptr [eax + 2]
        and    eax, 0ffh
        mov    ecx, dword ptr [ebp - 20h]
        mov    cl, byte ptr [ecx + 3]
        and    ecx, 0ffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 20h]
        mov    cl, byte ptr [ecx + 4]
        and    ecx, 0ffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 20h]
        mov    cl, byte ptr [ecx + 5]
        and    ecx, 0ffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 20h]
        mov    cl, byte ptr [ecx]
        and    ecx, 0ffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 20h]
        mov    cl, byte ptr [ecx + 1]
        and    ecx, 0ffh
        or     eax, ecx
        je     NEAR PTR loc_1_3206
        push   6
        mov    eax, dword ptr [ebp - 20h]
        push   eax
        lea    eax, [ebp - 44h]
        push   eax
        call   sub_1_071C
loc_1_3206:
        mov    eax, dword ptr [ebp - 48h]
        push   eax
        call   sub_1_06C0
        cmp    dword ptr [ebp - 10h], 2
        jne    NEAR PTR loc_1_3291
        mov    eax, dword ptr [ebp - 30h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_1_3284
        mov    word ptr [ebp - 84h], 1
        jmp    NEAR PTR loc_1_323E
loc_1_3237:
        inc    word ptr [ebp - 84h]
loc_1_323E:
        mov    eax, dword ptr [ebp - 84h]
        and    eax, 0ffffh
        cmp    eax, 10h
        jge    NEAR PTR loc_1_327F
        mov    ax, word ptr [ebp - 84h]
        mov    word ptr [ebp - 30h], ax
        lea    eax, [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_3618
        movsx  eax, ax
        or     eax, eax
        jne    NEAR PTR loc_1_327A
        jmp    NEAR PTR loc_1_327F
loc_1_327A:
        jmp    NEAR PTR loc_1_3237
loc_1_327F:
        jmp    NEAR PTR loc_1_3291
loc_1_3284:
        lea    eax, [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_3618
loc_1_3291:
        cmp    dword ptr [ebp - 10h], 1
        jne    NEAR PTR loc_1_3313
        mov    eax, dword ptr [ebp - 30h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_1_3306
        mov    word ptr [ebp - 88h], 1
        jmp    NEAR PTR loc_1_32C0
loc_1_32B9:
        inc    word ptr [ebp - 88h]
loc_1_32C0:
        mov    eax, dword ptr [ebp - 88h]
        and    eax, 0ffffh
        cmp    eax, 10h
        jge    NEAR PTR loc_1_3301
        mov    ax, word ptr [ebp - 88h]
        mov    word ptr [ebp - 30h], ax
        lea    eax, [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_3760
        movsx  eax, ax
        or     eax, eax
        jne    NEAR PTR loc_1_32FC
        jmp    NEAR PTR loc_1_3301
loc_1_32FC:
        jmp    NEAR PTR loc_1_32B9
loc_1_3301:
        jmp    NEAR PTR loc_1_3313
loc_1_3306:
        lea    eax, [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_3760
loc_1_3313:
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        mov    al, byte ptr [ebp - 24h]
        push   eax
        lea    eax, [ebp - 44h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        push   eax
        call   sub_1_0907
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        jmp    NEAR PTR loc_1_333E
loc_1_333E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
loc_1_3345:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_35C5
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_76FE
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 99h]
        push   eax
        db  0E8h, 059h, 0CDh, 0FEh, 0FFh
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0dfh]
        lea    eax, [eax + eax*4]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 31h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 35h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 25h]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        push   eax
        push   0
        push   2e4h
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_05C8
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 95h]
        push   eax
        call   sub_1_0568
        push   0
        push   0e9h
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_063C
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_33F2:
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
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_341F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_6D21
        mov    word ptr [ebp - 4], ax
        movsx  ecx, word ptr [ebp - 4]
        or     ecx, ecx
        je     NEAR PTR loc_1_344F
        sub    al, al
        jmp    NEAR PTR loc_1_3456
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_344F:
        mov    al, 1
        jmp    NEAR PTR loc_1_3456
loc_1_3456:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
        db  000h, 000h, 000h
sub_1_3460:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 29eh]
        and    eax, 0ffffh
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 1
        inc    eax
        push   eax
        push   1
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 29ch]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 29ch]
        and    eax, 0ffffh
        push   eax
        db  068h, 09Dh, 039h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        push   eax
        db  068h, 056h, 038h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2e0h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        add    eax, 4
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_061E
        cmp    dword ptr [ebp - 4], 0
        jne    NEAR PTR loc_1_34EC
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        mov    byte ptr [eax + 0cah], 1
loc_1_34EC:
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_34F4
loc_1_34F4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_34FB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        mov    al, byte ptr [eax + 0cah]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_3540
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        add    eax, 4
        push   eax
        call   sub_1_0624
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        mov    byte ptr [eax + 0cah], 0
loc_1_3540:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_3547:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_194D
        push   32h
        mov    eax, dword ptr [ebp - 4]
        add    eax, 5fh
        push   eax
        call   sub_1_0612
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_3574:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   0c350h
        call   sub_1_0630
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2dch]
        inc    word ptr [ecx + 5dh]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2dch]
        mov    ax, word ptr [ecx + 5dh]
        jmp    NEAR PTR loc_1_35A3
loc_1_35A3:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_35AA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   32h
        mov    eax, dword ptr [ebp + 8]
        add    eax, 5fh
        push   eax
        call   sub_1_0612
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_35C5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        add    eax, 5fh
        push   eax
        call   sub_1_0618
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_35E5:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 5fh], 0
        mov    eax, dword ptr [ebp + 8]
        db  0C7h, 040h, 063h, 047h, 035h, 000h, 000h
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 67h], eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_35AA
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_3618:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_05B0
        cmp    dword ptr [ebp - 4], 0
        je     NEAR PTR loc_1_3649
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_3759
loc_1_3649:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 202h
        jne    NEAR PTR loc_1_3689
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 6], 1890h
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 12h], 0
        xor    eax, eax
        mov    al, byte ptr [ebp - 0eh]
        test   al, 8
        je     NEAR PTR loc_1_3684
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 0eh], 0
loc_1_3684:
        jmp    NEAR PTR loc_1_370E
loc_1_3689:
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 700h
        je     NEAR PTR loc_1_36BD
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        cmp    eax, 130h
        je     NEAR PTR loc_1_36BD
        mov    ax, 0fffeh
        jmp    NEAR PTR loc_1_3759
        db  0E9h, 051h, 000h, 000h, 000h
loc_1_36BD:
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 6], 1892h
        xor    eax, eax
        mov    al, byte ptr [ebp - 0eh]
        test   al, 4
        je     NEAR PTR loc_1_36E1
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 12h], 0
        jmp    NEAR PTR loc_1_36EA
loc_1_36E1:
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 12h], 1
loc_1_36EA:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0eh]
        test   al, 8
        je     NEAR PTR loc_1_3705
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 10h], 10h
        jmp    NEAR PTR loc_1_370E
loc_1_3705:
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 10h], 4
loc_1_370E:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 14h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 0ch], 0
        mov    al, byte ptr [ebp - 0bh]
        and    eax, 38h
        shr    eax, 3
        db  08Ah, 080h, 070h, 07Ah, 000h, 000h
        and    eax, 0ffff00ffh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 0ah], ax
        mov    al, byte ptr [ebp - 0bh]
        and    eax, 7
        db  066h, 08Bh, 004h, 045h, 078h, 07Ah, 000h, 000h
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 8], ax
        sub    ax, ax
        jmp    NEAR PTR loc_1_3759
loc_1_3759:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_3760:
        push   ebp
        mov    ebp, esp
        sub    esp, 148h
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 140h]
        push   eax
        lea    eax, [ebp - 148h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        lea    eax, [ebp - 144h]
        push   eax
        call   sub_1_05B6
        cmp    dword ptr [ebp - 144h], 0
        je     NEAR PTR loc_1_37A0
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_382E
loc_1_37A0:
        cmp    dword ptr [ebp - 140h], 60110eh
        je     NEAR PTR loc_1_37CF
        mov    eax, dword ptr [ebp - 140h]
        and    eax, 0ffffffh
        cmp    eax, 604f42h
        je     NEAR PTR loc_1_37CF
        mov    ax, 0fffeh
        jmp    NEAR PTR loc_1_382E
loc_1_37CF:
        mov    eax, dword ptr [ebp - 148h]
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 14h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ax, word ptr [eax + 14h]
        and    eax, 0ffffh
        shl    eax, 0ch
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 8], ax
        mov    al, byte ptr [ebp - 8eh]
        and    ax, 0fh
        mov    ecx, dword ptr [ebp + 0ch]
        mov    word ptr [ecx + 0ah], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 0ch], 1
        mov    al, byte ptr [ebp - 8eh]
        shr    al, 5
        test   al, 1
        je     NEAR PTR loc_1_3826
        mov    eax, dword ptr [ebp + 0ch]
        mov    word ptr [eax + 0ch], 0
loc_1_3826:
        sub    ax, ax
        jmp    NEAR PTR loc_1_382E
loc_1_382E:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret
loc_1_3856:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dx, word ptr [eax + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        test   cl, 8
        jne    NEAR PTR loc_1_388E
        sub    al, al
        jmp    NEAR PTR loc_1_392F
loc_1_388E:
        mov    eax, dword ptr [ebp - 8]
        mov    dx, word ptr [eax + 1eah]
        in     ax, dx
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        test   al, 80h
        je     NEAR PTR loc_1_3928
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        test   al, 88h
        je     NEAR PTR loc_1_38CF
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 6]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 2d0h], ax
loc_1_38CF:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 2dch]
        mov    ax, word ptr [eax + 6ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ecx + 2dch]
        mov    word ptr [ecx + 6ch], ax
        mov    eax, dword ptr [ebp - 8]
        mov    dx, word ptr [eax + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0fff7h
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, cx
        out    dx, ax
        mov    al, 1
        jmp    NEAR PTR loc_1_392F
loc_1_3928:
        sub    al, al
        jmp    NEAR PTR loc_1_392F
loc_1_392F:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_3936:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0c9h]
        and    eax, 0ffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_3996
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax + 0c5h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_3996
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0c5h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0c9h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 99h]
        push   eax
        call   sub_1_0300
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0c5h], 0
loc_1_3996:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_399D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_1_53EF
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 2dch]
        push   eax
        call   sub_1_3FD5
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 2dch]
        push   eax
        call   sub_1_3936
        mov    ecx, dword ptr [ebp - 4]
        mov    dx, word ptr [ecx + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        or     ecx, 8
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, cx
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_3A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        mov    eax, dword ptr [eax + 21h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, dword ptr [ebp + 8]
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 4]
        shr    eax, 10h
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx], ax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
sub_1_3A52:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        mov    eax, dword ptr [eax + 29h]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 2dch]
        sub    eax, dword ptr [ecx + 35h]
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 14h]
        mov    word ptr [ecx], ax
        mov    eax, dword ptr [ebp - 4]
        shr    eax, 10h
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx], ax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
        db  000h, 000h, 000h
sub_1_3AA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 41h], 0
        jne    NEAR PTR loc_1_3AC0
        int3
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_3B9B
loc_1_3AC0:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 41h]
        add    eax, 24h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0ch]
        cmp    dword ptr [eax + 41h], ecx
        jne    NEAR PTR loc_1_3B13
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_3B00
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 41h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [eax + 45h], 0
        jmp    NEAR PTR loc_1_3B0B
loc_1_3B00:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 41h], eax
loc_1_3B0B:
        sub    ax, ax
        jmp    NEAR PTR loc_1_3B9B
loc_1_3B13:
        int3
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 41h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
loc_1_3B25:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_3B92
        mov    eax, dword ptr [ebp - 10h]
        add    eax, 24h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 10h]
        cmp    dword ptr [ebp + 0ch], eax
        jne    NEAR PTR loc_1_3B7F
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax], 0
        jne    NEAR PTR loc_1_3B5E
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 45h], eax
        jmp    NEAR PTR loc_1_3B77
loc_1_3B5E:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 0ch]
        add    eax, 24h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx], eax
loc_1_3B77:
        sub    ax, ax
        jmp    NEAR PTR loc_1_3B9B
loc_1_3B7F:
        mov    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 10h], eax
        jmp    NEAR PTR loc_1_3B25
loc_1_3B92:
        mov    ax, 0fffeh
        jmp    NEAR PTR loc_1_3B9B
loc_1_3B9B:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_3BA2:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 24h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, word ptr [eax + 8]
        cmp    eax, -1
        jne    NEAR PTR loc_1_3BD0
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 3dh]
        jmp    NEAR PTR loc_1_3C2D
loc_1_3BD0:
        push   0
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 10h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_0660
        mov    eax, dword ptr [ebp - 4]
        movsx  eax, word ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
loc_1_3BEF:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_3C24
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 51h], ecx
        jne    NEAR PTR loc_1_3C12
        mov    dword ptr [ebp - 8], 0
loc_1_3C12:
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_06A8
        jmp    NEAR PTR loc_1_3BEF
loc_1_3C24:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 8]
        add    dword ptr [ecx + 4dh], eax
loc_1_3C2D:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
loc_1_3C34:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 24h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_3BA2
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0fh], 2
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 0cbh]
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_3AA0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 10h]
        push   eax
        call   sub_1_057A
        mov    ecx, dword ptr [ebp - 0ch]
        dec    dword ptr [ecx + 14h]
        sub    ax, ax
        jmp    NEAR PTR loc_1_3CA5
loc_1_3CA5:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_3CAC:
        push   ebp
        mov    ebp, esp
        sub    esp, 18h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 24h
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 0ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 3fh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        imul   eax, dword ptr [ecx + 0dfh]
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 39h]
        mov    dword ptr [ebp - 8], eax
        push   0
        lea    eax, [ebp - 10h]
        push   eax
        push   0
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_0660
loc_1_3CF7:
        cmp    dword ptr [ebp - 10h], 0
        je     NEAR PTR loc_1_3D41
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_0684
        mov    eax, dword ptr [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_071C
        mov    eax, dword ptr [ebp - 14h]
        add    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 14h]
        add    dword ptr [ebp - 0ch], eax
        lea    eax, [ebp - 10h]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_06A8
        jmp    NEAR PTR loc_1_3CF7
loc_1_3D41:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        cmp    dword ptr [eax + 0dfh], ecx
        jae    NEAR PTR loc_1_3D53
loc_1_3D53:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 3fh]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        imul   eax, dword ptr [ecx + 0dfh]
        mov    ecx, dword ptr [ebp + 8]
        add    eax, dword ptr [ecx + 2dh]
        mov    ecx, dword ptr [ebp + 10h]
        mov    dword ptr [ecx + 0eh], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + 0ch], ax
        mov    eax, dword ptr [ebp + 10h]
        mov    word ptr [eax + 0ah], 1
        mov    eax, dword ptr [ebp - 4]
        mov    word ptr [eax + 8], 0ffffh
        mov    eax, dword ptr [ebp + 8]
        dec    word ptr [eax + 3dh]
        mov    eax, dword ptr [ebp + 8]
        inc    word ptr [eax + 3fh]
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 3fh]
        and    eax, 0ffffh
        cmp    eax, 2
        jne    NEAR PTR loc_1_3DBD
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 3fh], 0
loc_1_3DBD:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_3DC4:
        push   ebp
        mov    ebp, esp
        sub    esp, 50h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 24h
        mov    dword ptr [ebp - 4], eax
        mov    word ptr [ebp - 4ch], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 49h]
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebp - 10h]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_0660
loc_1_3DFC:
        cmp    dword ptr [ebp - 0ch], 0
        je     NEAR PTR loc_1_3EA0
        lea    eax, [ebp - 18h]
        push   eax
        lea    eax, [ebp - 48h]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_068A
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax + 51h], ecx
        jne    NEAR PTR loc_1_3E30
        mov    dword ptr [ebp - 8], 0
loc_1_3E30:
        mov    dword ptr [ebp - 50h], 0
        jmp    NEAR PTR loc_1_3E3F
loc_1_3E3C:
        inc    dword ptr [ebp - 50h]
loc_1_3E3F:
        mov    eax, dword ptr [ebp - 18h]
        cmp    dword ptr [ebp - 50h], eax
        jae    NEAR PTR loc_1_3E8E
        jmp    NEAR PTR loc_1_3E50
loc_1_3E50:
        mov    eax, dword ptr [ebp - 50h]
        mov    eax, dword ptr [ebp + eax*8 - 48h]
        mov    ecx, dword ptr [ebp - 4ch]
        and    ecx, 0ffffh
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp + 10h]
        mov    dword ptr [edx + ecx*2 + 0eh], eax
        mov    eax, dword ptr [ebp - 50h]
        mov    eax, dword ptr [ebp + eax*8 - 44h]
        mov    ecx, dword ptr [ebp - 4ch]
        and    ecx, 0ffffh
        lea    ecx, [ecx + ecx*2]
        mov    edx, dword ptr [ebp + 10h]
        mov    word ptr [edx + ecx*2 + 0ch], ax
        inc    word ptr [ebp - 4ch]
        jmp    NEAR PTR loc_1_3E3C
loc_1_3E8E:
        lea    eax, [ebp - 0ch]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        push   eax
        call   sub_1_06A8
        jmp    NEAR PTR loc_1_3DFC
loc_1_3EA0:
        mov    eax, dword ptr [ebp - 4ch]
        and    eax, 0ffffh
        cmp    eax, 6
        jle    NEAR PTR loc_1_3EB1
loc_1_3EB1:
        mov    ax, word ptr [ebp - 4ch]
        mov    ecx, dword ptr [ebp + 10h]
        mov    word ptr [ecx + 0ah], ax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 49h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 8], ax
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 49h], eax
        sub    eax, eax
        sub    eax, dword ptr [ebp - 14h]
        neg    eax
        mov    ecx, dword ptr [ebp + 8]
        sub    dword ptr [ecx + 4dh], eax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_3EE6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 24h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 4dh]
        ja     NEAR PTR loc_1_3F25
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        cmp    eax, 6
        jle    NEAR PTR loc_1_3F7B
loc_1_3F25:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 3dh]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_1_3F42
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_3FCE
loc_1_3F42:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        mov    ax, word ptr [eax + 244h]
        and    eax, 0ffffh
        cmp    eax, 1
        jg     NEAR PTR loc_1_3F65
        mov    ax, 0fffdh
        jmp    NEAR PTR loc_1_3FCE
loc_1_3F65:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3CAC
        jmp    NEAR PTR loc_1_3FC6
loc_1_3F7B:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0ch]
        and    eax, 0ffffh
        mov    ecx, 3
        add    eax, 2
        cdq
        idiv   ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx]
        mov    cx, word ptr [ecx + 244h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_3FB5
        mov    ax, 0fffdh
        jmp    NEAR PTR loc_1_3FCE
loc_1_3FB5:
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3DC4
loc_1_3FC6:
        sub    ax, ax
        jmp    NEAR PTR loc_1_3FCE
loc_1_3FCE:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_3FD5:
        push   ebp
        mov    ebp, esp
        sub    esp, 38h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 41h], 0
        je     NEAR PTR loc_1_407F
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 41h]
        mov    dword ptr [ebp - 38h], eax
loc_1_3FF4:
        mov    eax, dword ptr [ebp - 38h]
        add    eax, 24h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0fh]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_406D
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3EE6
        movsx  eax, ax
        or     eax, eax
        je     NEAR PTR loc_1_4031
        jmp    NEAR PTR loc_1_407F
loc_1_4031:
        mov    eax, dword ptr [ebp - 38h]
        push   eax
        lea    eax, [ebp - 34h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax]
        push   eax
        call   sub_1_5FBB
        movsx  eax, ax
        or     eax, eax
        jne    NEAR PTR loc_1_405B
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0fh], 1
        jmp    NEAR PTR loc_1_406D
loc_1_405B:
        mov    eax, dword ptr [ebp - 38h]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3BA2
        jmp    NEAR PTR loc_1_407F
loc_1_406D:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebp - 38h], eax
        cmp    dword ptr [ebp - 38h], 0
        jne    NEAR PTR loc_1_3FF4
loc_1_407F:
        sub    ax, ax
        jmp    NEAR PTR loc_1_4087
loc_1_4087:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
loc_1_408E:
        push   ebp
        mov    ebp, esp
        sub    esp, 1ch
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 0ch], 103h
        mov    eax, dword ptr [ebp + 0ch]
        add    eax, 24h
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 10h], eax
        lea    eax, [ebp - 18h]
        push   eax
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 14h]
        push   eax
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_0660
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp - 18h]
        cmp    dword ptr [eax + 0dfh], ecx
        jb     NEAR PTR loc_1_40E3
        cmp    dword ptr [ebp - 18h], 0fh
        jae    NEAR PTR loc_1_40ED
loc_1_40E3:
        mov    eax, 0c001000fh
        jmp    NEAR PTR loc_1_41E9
loc_1_40ED:
        mov    eax, dword ptr [ebp - 10h]
        inc    dword ptr [eax + 70h]
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 0c8h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_4196
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_4196
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_418A
        mov    eax, dword ptr [ebp - 1ch]
        inc    dword ptr [eax + 14h]
        test   byte ptr [ebp + 0ch], 1
        je     NEAR PTR loc_1_4148
loc_1_4148:
        mov    eax, dword ptr [ebp - 18h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0ah], ax
        mov    eax, dword ptr [ebp - 14h]
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [ecx + 0ch], ax
        mov    al, byte ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0eh], al
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_41F0
        mov    eax, dword ptr [ebp - 10h]
        push   eax
        call   sub_1_3FD5
        mov    eax, dword ptr [ebp - 1ch]
        dec    dword ptr [eax + 14h]
        jmp    NEAR PTR loc_1_4191
loc_1_418A:
        mov    dword ptr [ebp - 0ch], 0c0010002h
loc_1_4191:
        jmp    NEAR PTR loc_1_41DB
loc_1_4196:
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 0b5h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_41B8
        mov    dword ptr [ebp - 0ch], 0c001000dh
        jmp    NEAR PTR loc_1_41DB
loc_1_41B8:
        mov    eax, dword ptr [ebp - 10h]
        mov    al, byte ptr [eax + 0c3h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_41DA
        mov    dword ptr [ebp - 0ch], 0c0000001h
        jmp    NEAR PTR loc_1_41DB
loc_1_41DA:
        int3
loc_1_41DB:
        mov    eax, dword ptr [ebp - 10h]
        dec    dword ptr [eax + 70h]
        mov    eax, dword ptr [ebp - 0ch]
        jmp    NEAR PTR loc_1_41E9
loc_1_41E9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_41F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 10h]
        add    eax, 24h
        mov    dword ptr [ebp - 4], eax
        jmp    NEAR PTR loc_1_4207
loc_1_4207:
        mov    eax, dword ptr [ebp + 0ch]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0fh], 0
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 41h], 0
        jne    NEAR PTR loc_1_4232
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 41h], eax
        jmp    NEAR PTR loc_1_423E
loc_1_4232:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 45h]
        mov    dword ptr [ecx + 24h], eax
loc_1_423E:
        mov    eax, dword ptr [ebp + 10h]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx + 45h], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp + 0ch]
        inc    dword ptr [eax + 14h]
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
        db  000h, 000h, 000h
sub_1_4260:
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
        jne    NEAR PTR loc_1_4288
        jmp    NEAR PTR loc_1_43A9
loc_1_4288:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    eax, [ebp - 0ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 10h]
        push   eax
        call   sub_1_0660
        cmp    dword ptr [ebp - 0ch], 0
        jne    NEAR PTR loc_1_42AC
        jmp    NEAR PTR loc_1_43A9
loc_1_42AC:
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0684
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
loc_1_42C3:
        mov    eax, dword ptr [ebp + 0ch]
        cmp    dword ptr [ebp - 10h], eax
        jae    NEAR PTR loc_1_43A1
        cmp    dword ptr [ebp - 14h], 0
        jne    NEAR PTR loc_1_430B
        lea    eax, [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_06A8
        cmp    dword ptr [ebp - 8], 0
        jne    NEAR PTR loc_1_42F5
        jmp    NEAR PTR loc_1_43A1
loc_1_42F5:
        lea    eax, [ebp - 14h]
        push   eax
        lea    eax, [ebp - 18h]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_0684
        jmp    NEAR PTR loc_1_42C3
loc_1_430B:
        cmp    dword ptr [ebp + 14h], 0
        je     NEAR PTR loc_1_4353
        mov    eax, dword ptr [ebp + 14h]
        cmp    dword ptr [ebp - 14h], eax
        jae    NEAR PTR loc_1_433C
        sub    eax, eax
        sub    eax, dword ptr [ebp - 14h]
        neg    eax
        sub    dword ptr [ebp + 14h], eax
        mov    dword ptr [ebp - 14h], 0
        jmp    NEAR PTR loc_1_42C3
        db  0E9h, 017h, 000h, 000h, 000h
loc_1_433C:
        mov    eax, dword ptr [ebp + 14h]
        add    dword ptr [ebp - 18h], eax
        sub    eax, eax
        sub    eax, dword ptr [ebp + 14h]
        neg    eax
        sub    dword ptr [ebp - 14h], eax
        mov    dword ptr [ebp + 14h], 0
loc_1_4353:
        mov    eax, dword ptr [ebp + 0ch]
        sub    eax, dword ptr [ebp - 10h]
        mov    dword ptr [ebp - 20h], eax
        mov    eax, dword ptr [ebp - 14h]
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 20h]
        mov    ecx, dword ptr [ebp - 1ch]
        cmp    eax, ecx
        jb     NEAR PTR loc_1_4372
        mov    eax, ecx
loc_1_4372:
        mov    dword ptr [ebp - 1ch], eax
        mov    eax, dword ptr [ebp - 1ch]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 18h]
        push   eax
        call   sub_1_071C
        mov    eax, dword ptr [ebp - 1ch]
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 1ch]
        add    dword ptr [ebp - 10h], eax
        sub    eax, eax
        sub    eax, dword ptr [ebp - 1ch]
        neg    eax
        sub    dword ptr [ebp - 14h], eax
        jmp    NEAR PTR loc_1_42C3
loc_1_43A1:
        mov    eax, dword ptr [ebp - 10h]
        mov    ecx, dword ptr [ebp + 18h]
        mov    dword ptr [ecx], eax
loc_1_43A9:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    14h
loc_1_43B0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        inc    dword ptr [eax + 70h]
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0c8h]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_4435
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0ch], eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    al, byte ptr [eax + 1dh]
        and    eax, 0ffh
        or     eax, eax
        jne    NEAR PTR loc_1_4429
        mov    eax, dword ptr [ebp - 0ch]
        inc    dword ptr [eax + 14h]
        mov    eax, dword ptr [ebp + 1ch]
        push   eax
        push   0
        mov    eax, dword ptr [ebp + 18h]
        push   eax
        mov    eax, dword ptr [ebp + 14h]
        push   eax
        mov    eax, dword ptr [ebp + 10h]
        add    eax, dword ptr [ebp + 0ch]
        push   eax
        call   sub_1_4260
        mov    eax, dword ptr [ebp - 0ch]
        dec    dword ptr [eax + 14h]
        mov    dword ptr [ebp - 4], 0
        jmp    NEAR PTR loc_1_4430
loc_1_4429:
        mov    dword ptr [ebp - 4], 0c001000ch
loc_1_4430:
        jmp    NEAR PTR loc_1_4474
loc_1_4435:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0b5h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_4457
        mov    dword ptr [ebp - 4], 0c001000dh
        jmp    NEAR PTR loc_1_4474
loc_1_4457:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0c4h]
        and    eax, 0ffh
        or     eax, eax
        je     NEAR PTR loc_1_4474
        mov    dword ptr [ebp - 4], 0c0000001h
loc_1_4474:
        mov    eax, dword ptr [ebp - 8]
        dec    dword ptr [eax + 70h]
        mov    eax, dword ptr [ebp - 4]
        jmp    NEAR PTR loc_1_4482
loc_1_4482:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    18h
loc_1_4489:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 2dch]
        mov    dword ptr [ebp - 0ch], eax
        mov    ax, word ptr [ebp + 14h]
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_1_44C5
        mov    ax, 1
        jmp    NEAR PTR loc_1_4590
loc_1_44C5:
        mov    eax, dword ptr [ebp - 0ch]
        mov    ax, word ptr [eax + 0bah]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 10h]
        and    ecx, 0ffffh
        cmp    eax, ecx
        jl     NEAR PTR loc_1_44E7
        mov    eax, ecx
loc_1_44E7:
        mov    word ptr [ebp - 10h], ax
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 8]
        and    eax, 0ffh
        test   al, 80h
        je     NEAR PTR loc_1_4513
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0eh]
        and    eax, 1fh
        add    eax, 0eh
        mov    word ptr [ebp - 4], ax
        jmp    NEAR PTR loc_1_4519
loc_1_4513:
        mov    word ptr [ebp - 4], 0eh
loc_1_4519:
        mov    eax, dword ptr [ebp + 14h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        sub    eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 10h]
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        sub    eax, ecx
        push   eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0ch]
        mov    eax, dword ptr [eax + 99h]
        push   eax
        call   sub_1_00B0
        mov    ecx, dword ptr [ebp - 0ch]
        mov    byte ptr [ecx + 0c9h], 1
        mov    ecx, dword ptr [ebp - 0ch]
        inc    dword ptr [ecx + 0cfh]
        sub    ax, ax
        jmp    NEAR PTR loc_1_4590
loc_1_4590:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    10h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_45A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 29ah]
        mov    word ptr [ebp - 4], ax
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1e2h], ax
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1e4h], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 2
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1e6h], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1e8h], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 6
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1eah], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 6
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1ech], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 8
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1eeh], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 0ah
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1f0h], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 0ch
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1f2h], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 0eh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 1f4h], ax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_4682:
        push   ebp
        mov    ebp, esp
        sub    esp, 10h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 10h], eax
loc_1_4691:
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 0ch], ax
        add    dword ptr [ebp - 10h], 2
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 8], ax
        add    dword ptr [ebp - 10h], 2
        mov    eax, dword ptr [ebp - 10h]
        mov    ax, word ptr [eax]
        mov    word ptr [ebp - 4], ax
        add    dword ptr [ebp - 10h], 2
        movsx  eax, word ptr [ebp - 0ch]
        cmp    eax, 1
        jne    NEAR PTR loc_1_46E5
        movsx  eax, word ptr [ebp - 8]
        or     eax, eax
        jne    NEAR PTR loc_1_46E5
        movsx  eax, word ptr [ebp - 4]
        or     eax, eax
        jne    NEAR PTR loc_1_46E5
        jmp    NEAR PTR loc_1_4746
loc_1_46E5:
        movsx  eax, word ptr [ebp - 4]
        or     eax, eax
        je     NEAR PTR loc_1_4741
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1f2h]
        mov    ax, word ptr [ebp - 0ch]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1f0h]
        mov    ax, word ptr [ebp - 8]
        out    dx, ax
        jmp    NEAR PTR loc_1_471A
loc_1_4716:
        dec    word ptr [ebp - 4]
loc_1_471A:
        movsx  eax, word ptr [ebp - 4]
        or     eax, eax
        jle    NEAR PTR loc_1_4741
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 10h]
        mov    dx, word ptr [eax + 1e6h]
        mov    ax, word ptr [ecx]
        out    dx, ax
        add    dword ptr [ebp - 10h], 2
        jmp    NEAR PTR loc_1_4716
loc_1_4741:
        jmp    NEAR PTR loc_1_4691
loc_1_4746:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_474D:
        push   ebp
        mov    ebp, esp
        sub    esp, 28h
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 8], eax
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 8]
        movsx  eax, byte ptr [eax]
        cmp    eax, 47h
        je     NEAR PTR loc_1_4777
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_48DB
loc_1_4777:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 0ch], eax
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 0ch]
        movsx  eax, byte ptr [eax]
        cmp    eax, 41h
        je     NEAR PTR loc_1_4798
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_48DB
loc_1_4798:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 10h], eax
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 10h]
        movsx  eax, byte ptr [eax]
        cmp    eax, 48h
        je     NEAR PTR loc_1_47B9
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_48DB
loc_1_47B9:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 14h], eax
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 14h]
        movsx  eax, byte ptr [eax]
        cmp    eax, 21h
        je     NEAR PTR loc_1_47DA
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_48DB
loc_1_47DA:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 18h], eax
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 18h]
        movsx  eax, byte ptr [eax]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 10h]
        cmp    eax, ecx
        je     NEAR PTR loc_1_4861
        inc    dword ptr [ebp + 0ch]
        inc    dword ptr [ebp + 0ch]
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 0ch]
        movsx  ax, byte ptr [eax]
        mov    word ptr [ebp - 4], ax
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp + 0ch]
        movsx  ax, byte ptr [eax]
        and    eax, 0ffffh
        shl    eax, 8
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0ffffh
        or     eax, ecx
        mov    word ptr [ebp - 4], ax
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        add    eax, 68h
        add    dword ptr [ebp + 0ch], eax
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 1ch], eax
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 1ch]
        movsx  eax, byte ptr [eax]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 10h]
        cmp    eax, ecx
        je     NEAR PTR loc_1_4861
        mov    ax, 3bh
        jmp    NEAR PTR loc_1_48DB
loc_1_4861:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 20h], eax
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 20h]
        movsx  eax, byte ptr [eax]
        or     eax, eax
        je     NEAR PTR loc_1_4881
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_48DB
loc_1_4881:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 24h], eax
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 24h]
        movsx  eax, byte ptr [eax]
        cmp    eax, 2
        je     NEAR PTR loc_1_48A2
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_48DB
loc_1_48A2:
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 28h], eax
        inc    dword ptr [ebp + 0ch]
        mov    eax, dword ptr [ebp - 28h]
        movsx  eax, byte ptr [eax]
        or     eax, eax
        je     NEAR PTR loc_1_48C2
        mov    ax, 0ffffh
        jmp    NEAR PTR loc_1_48DB
loc_1_48C2:
        add    dword ptr [ebp + 0ch], 66h
        mov    eax, dword ptr [ebp + 0ch]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4682
        sub    ax, ax
        jmp    NEAR PTR loc_1_48DB
loc_1_48DB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
loc_1_48E2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0ch]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 95h], eax
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 1ch], 0
        jne    NEAR PTR loc_1_4951
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0e7h]
        and    eax, 0ffffh
        add    eax, 13h
        mov    dword ptr [ebp - 8], eax
        mov    dx, word ptr [ebp - 8]
        in     al, dx
        mov    byte ptr [ebp - 0ch], al
        mov    al, byte ptr [ebp - 0ch]
        and    eax, 0cfh
        mov    byte ptr [ebp - 0ch], al
        mov    eax, dword ptr [ebp + 10h]
        and    eax, 3
        db  08Ah, 080h, 094h, 07Ah, 000h, 000h
        and    eax, 0ffh
        or     al, byte ptr [ebp - 0ch]
        mov    byte ptr [ebp - 0ch], al
        mov    dx, word ptr [ebp - 8]
        mov    al, byte ptr [ebp - 0ch]
        out    dx, al
loc_1_4951:
        sub    eax, eax
        jmp    NEAR PTR loc_1_4958
loc_1_4958:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    0ch
sub_1_495F:
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
sub_1_496C:
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
sub_1_4979:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [ebp - 8], 0f8h
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2a4h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_49B3
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2a0h], 0
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        or     al, 2
        mov    byte ptr [ebp - 8], al
loc_1_49B3:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2a8h]
        and    eax, 0ffffh
        cmp    eax, 10h
        jne    NEAR PTR loc_1_49D6
        mov    al, byte ptr [ebp - 8]
        and    eax, 0dfh
        mov    byte ptr [ebp - 8], al
loc_1_49D6:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2aah]
        and    eax, 0ffffh
        cmp    eax, 1
        jne    NEAR PTR loc_1_49F7
        mov    al, byte ptr [ebp - 8]
        and    eax, 7fh
        mov    byte ptr [ebp - 8], al
loc_1_49F7:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2a6h]
        and    eax, 0ffffh
        or     eax, eax
        jne    NEAR PTR loc_1_4A19
        mov    al, byte ptr [ebp - 8]
        and    eax, 0efh
        mov    byte ptr [ebp - 8], al
loc_1_4A19:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 28ah]
        mov    al, byte ptr [ebp - 8]
        out    dx, al
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 28ah]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        or     cl, 4
        mov    dx, word ptr [eax + 28ah]
        mov    al, cl
        out    dx, al
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 28ah]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [ebp - 8]
        and    ecx, 0fbh
        mov    dx, word ptr [eax + 28ah]
        mov    al, cl
        out    dx, al
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 28ah]
        in     al, dx
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 294h]
        and    eax, 0ffffh
        cmp    eax, 1392h
        jne    NEAR PTR loc_1_4AFD
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 29ch]
        and    eax, 0ffffh
        db  08Ah, 080h, 085h, 07Ah, 000h, 000h
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2a0h]
        and    eax, 3
        db  08Ah, 080h, 094h, 07Ah, 000h, 000h
        and    eax, 0ffh
        or     al, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2a2h]
        and    eax, 0ffffh
        sar    eax, 2
        db  08Ah, 080h, 098h, 07Ah, 000h, 000h
        and    eax, 0ffh
        or     al, byte ptr [ebp - 8]
        mov    byte ptr [ebp - 8], al
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 28ch]
        mov    al, byte ptr [ebp - 8]
        out    dx, al
loc_1_4AFD:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        or     ecx, 0c0h
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, cx
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 1eeh]
        and    ecx, 0ffffh
        inc    ecx
        mov    dx, cx
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_4B82
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        or     eax, 10h
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        or     ecx, 0d0h
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, cx
        out    dx, ax
loc_1_4B82:
        mov    eax, dword ptr [ebp - 0ch]
        and    eax, 0ffffh
        or     eax, 40h
        and    eax, 0ffffff7fh
        mov    word ptr [ebp - 0ch], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, word ptr [ebp - 0ch]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2a4h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_4BDB
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        or     ecx, 100h
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, cx
        out    dx, ax
loc_1_4BDB:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_4BE2:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 29ah]
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_45A0
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 294h], 1390h
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    ecx, 11h
        mov    ebx, dword ptr [ebp + 8]
        mov    word ptr [ebx + 28ah], cx
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    ecx, 1fh
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        cmp    ecx, 1fh
        jne    NEAR PTR loc_1_4CEC
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    ecx, 1eh
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    ecx, 1eh
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        add    ecx, 1eh
        mov    dx, cx
        in     al, dx
        mov    byte ptr [ebp - 4], al
        mov    al, byte ptr [ebp - 4]
        and    eax, 7
        cmp    al, 2
        jne    NEAR PTR loc_1_4CEC
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 294h], 1392h
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        add    eax, 13h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 28ch], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        add    eax, 12h
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 292h], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        add    eax, 1eh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 28eh], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        add    eax, 1fh
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ecx + 290h], ax
loc_1_4CEC:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 28ah]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 1
        jne    NEAR PTR loc_1_4D18
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 298h], 1
        jmp    NEAR PTR loc_1_4D3C
loc_1_4D18:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2a4h], 1
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 298h], 0
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 2a0h], 0
loc_1_4D3C:
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 29eh], 1
        sub    ax, ax
        jmp    NEAR PTR loc_1_4D50
loc_1_4D50:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_4D57:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        movsx  eax, word ptr [ebp + 0ch]
        or     eax, eax
        je     NEAR PTR loc_1_4DB1
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 28ah]
        in     al, dx
        mov    byte ptr [ebp - 4], al
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 4]
        test   cl, 4
        jne    NEAR PTR loc_1_4DB1
        mov    ecx, dword ptr [ebp + 8]
        mov    dx, word ptr [ecx + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        test   al, 0c0h
        jne    NEAR PTR loc_1_4DB1
        mov    ax, 1
        jmp    NEAR PTR loc_1_4DC2
loc_1_4DB1:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4979
        sub    ax, ax
        jmp    NEAR PTR loc_1_4DC2
loc_1_4DC2:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ax, ax
        jmp    NEAR PTR loc_1_4DD7
loc_1_4DD7:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_4DDE:
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
sub_1_4DEB:
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
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ax, ax
        jmp    NEAR PTR loc_1_4E06
loc_1_4E06:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_4E0D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        or     ecx, 0c0h
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, cx
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 1eeh]
        and    ecx, 0ffffh
        inc    ecx
        mov    dx, cx
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_4E9B
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        or     eax, 10h
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        or     ecx, 0d0h
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, cx
        out    dx, ax
loc_1_4E9B:
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        or     eax, 40h
        and    eax, 0ffffff7fh
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, word ptr [ebp - 8]
        out    dx, ax
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 2a4h]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_4EF4
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        or     ecx, 100h
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, cx
        out    dx, ax
loc_1_4EF4:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_4EFB:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 298h], 0bh
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 29eh], 0
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_45A0
        sub    ax, ax
        jmp    NEAR PTR loc_1_4F2A
loc_1_4F2A:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_4F31:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        movsx  eax, word ptr [ebp + 0ch]
        or     eax, eax
        je     NEAR PTR loc_1_4F6F
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        test   al, 0c0h
        jne    NEAR PTR loc_1_4F6F
        mov    ax, 1
        jmp    NEAR PTR loc_1_4F80
loc_1_4F6F:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_4E0D
        sub    ax, ax
        jmp    NEAR PTR loc_1_4F80
loc_1_4F80:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    8
sub_1_4F87:
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
sub_1_4F94:
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
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ax, ax
        jmp    NEAR PTR loc_1_4FAF
loc_1_4FAF:
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
sub_1_4FB6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0ch
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 29ah]
        and    eax, 0ffffh
        add    eax, 0c84h
        mov    dx, ax
        sub    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        mov    word ptr [ebp - 0ch], ax
loc_1_4FE6:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        sub    eax, ecx
        cmp    eax, 2
        jge    NEAR PTR loc_1_500D
        jmp    NEAR PTR loc_1_4FE6
loc_1_500D:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 29ah]
        and    eax, 0ffffh
        add    eax, 0c84h
        mov    dx, ax
        mov    al, 1
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        mov    word ptr [ebp - 0ch], ax
loc_1_5034:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        sub    eax, ecx
        cmp    eax, 5
        jge    NEAR PTR loc_1_505B
        jmp    NEAR PTR loc_1_5034
loc_1_505B:
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 29ah]
        and    eax, 0ffffh
        add    eax, 80h
        mov    dx, ax
        sub    al, al
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        mov    word ptr [ebp - 0ch], ax
loc_1_5082:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_3574
        and    eax, 0ffffh
        mov    ecx, dword ptr [ebp - 0ch]
        and    ecx, 0ffffh
        sub    eax, ecx
        cmp    eax, 5
        jge    NEAR PTR loc_1_50A9
        jmp    NEAR PTR loc_1_5082
loc_1_50A9:
        mov    eax, dword ptr [ebp + 8]
        mov    dx, word ptr [eax + 1eeh]
        in     ax, dx
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        or     ecx, 0c0h
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, cx
        out    dx, ax
        mov    ecx, dword ptr [ebp + 8]
        mov    cx, word ptr [ecx + 1eeh]
        and    ecx, 0ffffh
        inc    ecx
        mov    dx, cx
        in     ax, dx
        mov    word ptr [ebp - 4], ax
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0ffffh
        or     eax, eax
        je     NEAR PTR loc_1_512E
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0ffffh
        or     eax, 10h
        mov    word ptr [ebp - 8], ax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        or     ecx, 0d0h
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, cx
        out    dx, ax
loc_1_512E:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 0ffffh
        or     ecx, 40h
        and    ecx, 0ffffff7fh
        mov    dx, word ptr [eax + 1eeh]
        mov    ax, cx
        out    dx, ax
        pop    edi
        pop    esi
        pop    ebx
        leave
        ret    4
_LTEXT ends
        end
